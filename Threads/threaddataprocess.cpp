/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：ThreadDataProcess.cpp
 * 概   要：数据处理线程，包含子板卡通信工作对象、运算通道计算工作对象
 *      ---基于Modbus协议，对板内RS485总线上的子板卡进行数据查询，并根据通道配
 * 置做简单的运算，最后将数据各通道数据映射到原始数据对象中。
 *      ---基于原始数据对象，对各运算通道进行计算，并更新Modbus寄存器分配表（共
 * 享 内存）
 *
 * 当前版本：V1.0.0
 * 作   者：王 品
 * 完成日期：2015-7-8
 *
 * 修改版本：
 * 修改说明：
 * 修改作者：
 *
 ********************************************************************/
#include "threaddataprocess.h"
#include "../GlobalData/globalDef.h"
#include "./threaddatasync.h"
#include "Common/devboard.h"
#include "Common/devchannelmath.h"
#include "Common/timer.h"

#include <QDebug>
#include <QBitArray>
#include <QSharedMemory>
#include <QPluginLoader>
#include <QDateTime>
#include <QDir>

MODBUS_REGISTER_MAP  g_originDataMap;   //全局变量，原始数据对象

static Math_Status_Type _mathStatus;    //系统当前运算状态
static BoardInterface *_boardInterface = NULL;       //子板接口类指针

#define MISC_DEV    "/dev/pl_misc0"     //LED，板卡状态查询、设置设备节点
#define MSIC_DATA_COUNT     8             //misc 设备节点数据个数
#define BAORD_POWERON_TIME  5000   //子板卡启动时间，单位ms

/*获取子板通讯接口对象指针*/
BoardInterface *ThreadDataProcess::getBoardInterface()
{
    return _boardInterface;
}

/*
 * 功能：
 * 	  子板卡数据处理类静态成员函数，返回静态数据处理实例指针
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
ThreadDataProcess *ThreadDataProcess::instance()
{
    static ThreadDataProcess data;
    return &data;
}

/*
 * 功能：
 * 	  运算状态设定
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
void ThreadDataProcess::setMathStatus(Math_Status_Type status)
{
    //运算复位---TLOG计算复位
    if (Math_Status_Type_On_Reset == status) {
        MathExpAnalysis *pMathExpAnalysis;
        for (int i=0; i<MathExpAnalysis::s_mathExplist.count(); ++i){
            pMathExpAnalysis = MathExpAnalysis::s_mathExplist.at(i);
            pMathExpAnalysis->m_tlogCalculate.tlogValueReset();
        }
    }

    _mathStatus = status;

    /*仪表状态设定*/
    CfgConfig::instance()->setDeviceStatus_Math(status==Math_Status_Type_Off ? false : true);
}

/**
 * @brief getMathStatus
 *          获取运算状态
 * @return
 *          运算状态：
 *              Math_Status_Type_Off：运算关闭
 *              Math_Status_Type_On：运算开始
 *              Math_Status_Type_On_Reset：运算以复位形式开始
 */
Math_Status_Type ThreadDataProcess::getMathStatus()
{
    return _mathStatus;
}

/*
 * 功能：
 * 	  测量周期切换, 启动定时器，定时驱动DataProcess对象进行数据查询
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
void ThreadDataProcess::changeMeasurePeriod()
{
    emit sigProcessDataPeriodly();
}

/*
 * 功能：
 * 	  子板卡数据处理类构造函数
 * 参数：
 * 	  1.QObject *parent: 父对象，默认为0
 * 返回值：
 * 	  无
 */
ThreadDataProcess::ThreadDataProcess(QObject *parent) :
    QObject(parent)
{
    //数据处理工作
    DataProcess *dataProcess = new DataProcess;
    dataProcess->moveToThread(&m_dataProcessThread);
    connect(&m_dataProcessThread, &QThread::finished, dataProcess, &QObject::deleteLater);
    connect(&m_dataProcessThread, &QThread::started, dataProcess, &DataProcess::slotStart);

    /*子板数据、状态相关信号槽连接*/
    connect(this, SIGNAL(sigBoardStatusQuery()), dataProcess, SLOT(slotStatusQuery()), Qt::BlockingQueuedConnection);
    connect(this, SIGNAL(sigRelayDoOutput(quint32,bool)), dataProcess, SLOT(slotRelayDOOutput(quint32,bool)));
    connect(this, SIGNAL(sigProcessDataPeriodly()), dataProcess, SLOT(slotTimerStart()));
    connect(this, SIGNAL(sigDataSampleStop()), dataProcess, SLOT(slotTimerStop()), Qt::BlockingQueuedConnection);

    //板卡拔插->构建或销毁对应板卡对象->更新通道链表模型->信号-同步构建或销毁配置树
    connect(dataProcess, SIGNAL(sigBoardPluginout(quint8,bool)), CfgChannel::instance(), SLOT(slotBoardPluginout(quint8,bool)));

    /*Timer连接*/
    connect(Timer::instance(), SIGNAL(sigTimerTrigger(int)), dataProcess, SLOT(slotTimerTrigger(int)));
    connect(Timer::instance(), SIGNAL(sigTimerMatchTrigger(int)), dataProcess, SLOT(slotTimerMatchTrigger(int)));

    m_dataProcessThread.start(QThread::TimeCriticalPriority);
}

/************************子板卡数据处理工作对象*******************************************/

DataProcess::DataProcess(QObject *parent) :
    QObject(parent),
    m_extModuleSlave(-1),
    m_newExtBoard(false)
{
}

void DataProcess::slotStart()
{
    bool plugin = loadPlugin();
    qDebug()<<"Plugin loader: "<<plugin;

    p_dataQueryTimer = new QTimer(this);
    connect(p_dataQueryTimer, SIGNAL(timeout()), this, SLOT(slotDataQuery()));

    p_boardPowerOn = new QTimer(this);
    p_boardPowerOn->setInterval(BAORD_POWERON_TIME);
    p_boardPowerOn->setSingleShot(true);
    connect(p_boardPowerOn, SIGNAL(timeout()), this, SLOT(slotBoardChanged()));

    m_miscDev.setFileName(QString(MISC_DEV));
    m_miscDev.open(QIODevice::ReadWrite);
    p_notifier = new QSocketNotifier(m_miscDev.handle(), QSocketNotifier::Read, this);
    if (p_notifier) {
        connect(p_notifier, SIGNAL(activated(int)), p_boardPowerOn, SLOT(start()));
        p_notifier->setEnabled(true);
    }

    m_boardValidCur.resize(MAIN_UNIT_MODULE_NUM);
    m_boardValidCur.fill(false);
    m_boardValidPre.resize(MAIN_UNIT_MODULE_NUM);
    m_boardValidPre.fill(false);
}

void DataProcess::slotTimerStart()
{
    if (p_dataQueryTimer->isActive()) {
        p_dataQueryTimer->stop();
    }
    p_dataQueryTimer->start(DevConfigPtr->measureInterval_ms<100 ? 100 : DevConfigPtr->measureInterval_ms);
}

void DataProcess::slotTimerStop()
{
    p_dataQueryTimer->stop();
}

/*
 * 功能：
 * 	  槽函数，运行于数据处理线程，根据系统当前状态，获取各模块数据(输入寄存器)
 *       Step1: 获取各板卡输入数据，并映射到原始数据对象中
 *       Step2: 基于Step1中的原始数据对象，对各板卡数据进行查表、通道运算以及通道状态确定操作
 * 参数：
 * 	  无
 * 返回值：
 * 	  成功返回0; 失败返回-1
 */
int DataProcess::slotDataQuery()
{
    SUB_BOARD_DATA  dataRev;
    SUB_BOARD_DATA_PROCESSED dataProcessed;

    if (0 == _boardInterface) {
        qDebug()<<"BoardInterface Plugin load failed";
        return -1;
    }

    /*获取IO板卡(AI/DI/DO/DIO)数据--预处理--映射*/
    foreach (DevBoard *pBoard, CfgChannel::instance()->getList_BoardIO()) {
        quint8 unitNum = pBoard->getBoardUnit();                       //板卡单元号
        quint8 moduleNum = pBoard->getBoardModule();           //板卡模块号
        quint16 boardStatus = pBoard->getBoardStatus();            //板卡状态

        //运算数据清零
        ::memset(&dataProcessed, 0x00, sizeof(SUB_BOARD_DATA_PROCESSED));
        //获取板卡数据
        int boardRegMapSize = sizeof(SUB_BOARD_DATA)>>1;
        int regAddr = pBoard->getBoardDataRegBaseAddr();
        int slave = pBoard->getBoardBusSlaveAddr();
        _boardInterface->tcflushDataBus();
        int ret = _boardInterface->getInputRegister(regAddr, boardRegMapSize, (uint16_t *)&dataRev, slave);

        //通道运算预处理
       if (!ret) {
           ::memset(dataProcessed.dataStatus, DATA_ERROR_INVALID<<DATA_STATUS_ERROR_BITSOFF, \
                            sizeof(dataProcessed.dataStatus));
       } else {
           if (pBoard->getBoardType() == STATUS_BOARD_TYPE_AI) {
               pBoard->setBoardColdTemperature(dataRev.tempAI);
           }
           dataInputProcess(&dataRev, &dataProcessed);
       }

       //测量输入数据（输入值）映射
       dataInputRemap(unitNum, moduleNum, boardStatus, &dataProcessed);
    }

    /*IO板卡测试值查表、计算机状态确定*/
    foreach (DevBoard *pBoard, CfgChannel::instance()->getList_BoardIO()) {
        foreach (Channel* pChan, pBoard->getBoardChannel()) {
            switch (pChan->getChannelType()) {
            case Channel_Type_AI:
                pChan->chanCalculate();
                break;
            case Channel_Type_DI:
                pChan->chanCalculate();
                break;
            default:
                break;
            }
        }
    }

    //运算通道处理
    if (_mathStatus) {
        //各运算通道相关TLOG计算
        foreach(DevBoard *pBoard, CfgChannel::instance()->getList_BoardVirtual()) {
            if (pBoard->getBoardType() == STATUS_BOARD_TYPE_MATH) {
                foreach (Channel *pChan, pBoard->getBoardChannel()) {
                    qobject_cast<DevChannelMath *>(pChan)->chanTlogCalculate();
                }
            }
        }

        //计算运算通道数据
        foreach(DevBoard *pBoard, CfgChannel::instance()->getList_BoardVirtual()) {
            if (pBoard->getBoardType() == STATUS_BOARD_TYPE_MATH) {
                foreach (Channel *pChan, pBoard->getBoardChannel()) {
                    pChan->chanCalculate();
                }
            }
        }
    }

    //共享内存Modbus寄存器表更新
    ThreadDataSync::instance()->buffDataFrame(&g_originDataMap);

    return 0;
}

/*
 * 功能：
 * 	  槽函数，子板卡状态查询（保持寄存器）
 * 参数：
 * 	  无
 * 返回值：
 * 	  成功返回0; 失败返回-1
 */
int DataProcess::slotStatusQuery()
{
//    m_boardValidPre.fill(false);
//    slotBoardChanged();

    quint16 boardStatus;
    bool ret;
    bool extValidFlag = false;

    if (0 == _boardInterface) {
        qDebug()<<"BoardInterface Plugin load failed";
        return -1;
    }

    //清除所有板卡状态寄存器（配置包ID除外!!）
    ::memset(&DevConfigPtr->sysStatus, 0x00, sizeof(SYSTEM_STATUS)-sizeof(SYSTEM_CONFIG_PACKID));

    //获取板卡有效标记位
    if (readBoardValidFlag()) {
        m_boardValidPre = m_boardValidCur;
    } else {
        m_boardValidCur.fill(true);
        m_boardValidPre = m_boardValidCur;
    }


    //本机模拟板卡
    for (int i=0; i<MAIN_UNIT_MODULE_NUM; ++i) {
        if (m_boardValidCur.testBit(i) == false)
            continue;

        _boardInterface->tcflushConfigBus();
        ret = _boardInterface->getHoldRegister(0x00, sizeof(BOARD_IO_CONFIG)>>1, (uint16_t *)&DevConfigPtr->sysStatus.mainUnit_Recognition[i], i+1);
        if (ret) {
            boardStatus = DevConfigPtr->sysStatus.mainUnit_Recognition[i].boardReg_status;
            qDebug("MainBoard[%d], Status:0x%x\n", i, boardStatus);

            /*设置当前状态寄存器*/
            DevConfigPtr->sysStatus.mainUnit_ModuleStatus[i] = boardStatus;

            /* 设置主机扩展板卡标记*/
            if (STATUS_BOARD_TYPE(boardStatus) == STATUS_BOARD_TYPE_EXT && \
                STATUS_BOARD_EXT_STATUS(boardStatus)){
                extValidFlag = true;
                m_extModuleSlave = i+1;
            }
        }
    }

    //扩展模拟板卡
    if (extValidFlag) {
        //获取各扩展单元板卡状态
        for (int i=0; i<EXT_UNIT_NUM; ++i){
            for (int j=0; j<EXT_UNIT_MODULE_NUM; ++j) {
                int boardRegMapSize = sizeof(BOARD_IO_CONFIG)>>1;
                int regAddr = (sizeof(BOARD_EXT_CONFIG)>>1) + i * (boardRegMapSize * EXT_UNIT_MODULE_NUM) + boardRegMapSize * j;
                _boardInterface->tcflushConfigBus();
                ret = _boardInterface->getHoldRegister(regAddr, sizeof(BOARD_IO_CONFIG)>>1, \
                                                       (uint16_t *)&DevConfigPtr->sysStatus.extUnit_Recognition[i][j], m_extModuleSlave);
                if (ret) {
                    boardStatus = DevConfigPtr->sysStatus.extUnit_Recognition[i][j].boardReg_status;
                    qDebug("ExtBoard[%d, %d], Status:0x%x\n", i+1, j, boardStatus);
                    /*设置当前状态寄存器*/
                    DevConfigPtr->sysStatus.extUnit_ModuleStatus[i][j] = boardStatus;
                }
            }
        }
    }

    /*状态检查，模块相关配置更新*/
    for (int i=0; i<MAIN_UNIT_MODULE_NUM; ++i) {
        if (boardStatusCheck(&DevConfigPtr->sysStatus.mainUnit_ModuleStatus[i])) {
            boardConfigUpdate(0, i);
            qDebug("Board[0][%d]:0x%x", i, DevConfigPtr->sysStatus.mainUnit_ModuleStatus[i]);
        } else {
            DevConfigPtr->sysStatus.mainUnit_ModuleStatus[i] = 0;
            m_boardValidPre.clearBit(i);
            m_boardValidCur.clearBit(i);
        }
    }
    for (int i=0; i<EXT_UNIT_NUM; ++i) {
        for (int j=0; j<EXT_UNIT_MODULE_NUM; ++j) {
            if (boardStatusCheck(&DevConfigPtr->sysStatus.extUnit_ModuleStatus[i][j])) {
                boardConfigUpdate(i+1, j);
                qDebug("Board[%d][%d]:0x%x", i+1, j, DevConfigPtr->sysStatus.extUnit_ModuleStatus[i][j]);
            } else {
                DevConfigPtr->sysStatus.extUnit_ModuleStatus[i][j] = 0;
            }
        }
    }
    return 0;
}

/**
 * @brief DataProcess::readBoardValidFlag
 *          板卡有效标记位读取
 * @return
 */
bool DataProcess::readBoardValidFlag()
{
    char miscData[MSIC_DATA_COUNT];

    if (m_miscDev.read(miscData, MSIC_DATA_COUNT) < 0 ) {
        qDebug("Error--Get Misc Data failed!");
        return false;
    } else {
        qDebug("Read Misc Data:0x%x, 0x%x", miscData[4], miscData[5]);
        for(int i=0; i<8; ++i) {
            if (miscData[4] & (0x01<<i)) {
                m_boardValidCur.clearBit(i);
            } else {
                m_boardValidCur.setBit(i);
            }
        }

        for(int i=0; i<2; ++i) {
            if (miscData[5] & (0x01<<i)) {
                m_boardValidCur.clearBit(i+8);
            } else {
                m_boardValidCur.setBit(i+8);
            }
        }
        return true;
    }
}

/**
 * @brief DataProcess::updateBoardStatus
 *          板卡状态及配置更新
 * @param unit
 *          单元号
 * @param module
 *          模块号
 * @return
 */
bool DataProcess::updateBoardStatus(quint8 unit, quint8 module)
{
    bool ret;
    //清除板卡配置及状态
    CfgConfig::instance()->clearBoardStatus(unit, module);

    qDebug("%s, unit:%d, module:%d", __func__, unit, module);
    if (0 == _boardInterface) {
        qDebug()<<"BoardInterface Plugin load failed";
        return false;
    }

    if (unit == CHANNEL_UNIN_MAIN) {    //本机
        _boardInterface->tcflushConfigBus();
        ret = _boardInterface->getHoldRegister(0x00, sizeof(BOARD_IO_CONFIG)>>1,
                                               (uint16_t *)&DevConfigPtr->sysStatus.mainUnit_Recognition[module], module+1);
        if (ret) {
            quint16 boardStatus = DevConfigPtr->sysStatus.mainUnit_Recognition[module].boardReg_status;
            DevConfigPtr->sysStatus.mainUnit_ModuleStatus[module] =boardStatus;

            /*只支持一块扩展板卡*/
            if (STATUS_BOARD_TYPE(boardStatus) == STATUS_BOARD_TYPE_EXT && \
                STATUS_BOARD_EXT_STATUS(boardStatus)){
                if (CfgConfig::instance()->isExtBoardValid()) {
                    CfgConfig::instance()->clearBoardStatus(unit, module);
                    return false;
                } else {
                    m_extModuleSlave = module+1;
                    m_newExtBoard = true;
                    return true;
                }
            }

            return true;
        }

    } else {        //扩展单元
        int slave = CfgConfig::instance()->getBoardSlaveAddr(unit, module);
        int boardRegMapSize = sizeof(BOARD_IO_CONFIG)>>1;
        int regAddr = (sizeof(BOARD_EXT_CONFIG)>>1) + (unit-1) * (boardRegMapSize * EXT_UNIT_MODULE_NUM) + boardRegMapSize * module;
        _boardInterface->tcflushConfigBus();
        ret = _boardInterface->getHoldRegister(regAddr, sizeof(BOARD_IO_CONFIG)>>1, \
                                               (uint16_t *)&DevConfigPtr->sysStatus.extUnit_Recognition[unit-1][module], slave);

        if (ret) {
            /*设置当前状态寄存器*/
            DevConfigPtr->sysStatus.extUnit_ModuleStatus[unit-1][module] = DevConfigPtr->sysStatus.extUnit_Recognition[unit-1][module].boardReg_status;
            return true;
        }
    }

    return false;
}

/**
 * @brief DataProcess::checkBoardStatus
 *          板卡状态及配置检查
 * @param unit
 *          单元号
 * @param module
 *          模块号
 * @return
 */
bool DataProcess::checkBoardStatus(quint8 unit, quint8 module)
{
    quint16 boardStatus;

    boardStatus = CfgConfig::instance()->getBoardStatus(unit, module);

    if (0 == STATUS_BOARD_STATUS(boardStatus)) {
        goto Error;
    }

    switch (STATUS_BOARD_TYPE(boardStatus)) {
    case STATUS_BOARD_TYPE_AI:
        if (STATUS_BOARD_CHAN0_NUM(boardStatus) > MAIN_UNIT_MODULE_NUM) {
            goto Error;
        }
        break;
    case STATUS_BOARD_TYPE_DI:
        if (STATUS_BOARD_CHAN0_NUM(boardStatus) > DIO_UNIT_CHAN_NUM_USE_MAX) {
            goto Error;
        }
        break;
    case STATUS_BOARD_TYPE_DO:
        if (STATUS_BOARD_CHAN0_NUM(boardStatus) > DIO_UNIT_CHAN_NUM_USE_MAX) {
            goto Error;
        }
        break;
    case STATUS_BOARD_TYPE_DIO:
        if ((STATUS_BOARD_CHAN0_NUM(boardStatus) + STATUS_BOARD_CHAN1_NUM(boardStatus)) \
             > DIO_UNIT_CHAN_NUM_USE_MAX) {
            goto Error;
        }
        break;
    case STATUS_BOARD_TYPE_EXT:
        break;
    default:
        goto Error;
    }

    return true;

Error:
    CfgConfig::instance()->clearBoardStatus(unit, module);
    return false;
}

/**
 * @brief DataProcess::slotBoardChanged
 *          板卡热插拔处理函数
 */
void DataProcess::slotBoardChanged()
{
    if (readBoardValidFlag()) {
        QBitArray xorBit(m_boardValidCur);
        xorBit.operator ^=(m_boardValidPre);
        for (int i=0; i<MAIN_UNIT_MODULE_NUM; ++i) {
            if (xorBit.testBit(i) && m_boardValidCur.testBit(i)) {  //板卡插入
                if (updateBoardStatus(0, i) && checkBoardStatus(0, i)) {

                    /*扩展模块，更新各个扩展板卡配置及状态*/
                    if (m_newExtBoard) {
                        m_newExtBoard = false;
                        for (int unit=0; unit<EXT_UNIT_NUM; ++unit) {
                            for (int module=0; module<EXT_UNIT_MODULE_NUM; ++module) {
                                updateBoardStatus(unit+1, module);
                                checkBoardStatus(unit+1, module);
                            }
                        }
                    }

                    emit sigBoardPluginout(i, true);
                    m_boardValidPre.setBit(i);
                } else {
                    m_boardValidCur.clearBit(i);
                    m_boardValidPre.clearBit(i);
                }
            } else if (xorBit.testBit(i) && m_boardValidPre.testBit(i)) {   //板卡拔出
                emit sigBoardPluginout(i, false);
                m_boardValidPre.clearBit(i);
            }
        }
    } else {
        qDebug("Error--get board valid flag failed!");
    }
}

/*
 * 功能：
 * 	  DO通道输出设置
 * 参数：
 * 	  1.quint32 chanCoder：DO通道编码
 *       2.bool value：输出值，true--1, false--0
 * 返回值：
 * 	  无
 */
void DataProcess::slotRelayDOOutput(quint32 chanCoder, bool value)
{
    quint16 number = CHANNEL_NUM(chanCoder) - 1;              //通道号

    foreach (DevBoard *pBoard, CfgChannel::instance()->getList_BoardIO()) {
        if ((STATUS_BOARD_TYPE_DO == pBoard->getBoardType() ||STATUS_BOARD_TYPE_DIO == pBoard->getBoardType()) &&
            CHANNEL_UNIT(chanCoder) == pBoard->getBoardUnit() &&
            CHANNEL_MODULE(chanCoder) == pBoard->getBoardModule()) {
                pBoard->setDoOutput(number, value);
        }
    }
}

/**
 * @brief DataProcess::slotTimerTrigger
 *          定时器触发处理槽函数，Math通道TLOG复位
 * @param i
 *          定时器索引号
 */
void DataProcess::slotTimerTrigger(int i)
{
    //各运算通道相关TLOG计算
    foreach(DevBoard *pBoard, CfgChannel::instance()->getList_BoardVirtual()) {
        if (pBoard->getBoardType() == STATUS_BOARD_TYPE_MATH) {
            foreach (Channel *pChan, pBoard->getBoardChannel()) {
                quint32 chanCoder = pChan->getChannelCoder();
                quint16 number = CHANNEL_NUM(chanCoder) - 1;
                if (DevConfigPtr->chanMath[number].expressConfig.expSwtich &&
                    0 == DevConfigPtr->chanMath[number].expressConfig.expTlogType &&
                    (quint32)(i+1) == DevConfigPtr->chanMath[number].expressConfig.expTlogNo &&
                    DevConfigPtr->chanMath[number].expressConfig.expTlogReset) {
                    qobject_cast<DevChannelMath *>(pChan)->p_expAnalysis->m_tlogCalculate.tlogValueReset();
                }
            }
        }
    }
}

/**
 * @brief DataProcess::slotTimerMatchTrigger
 *          匹配定时器触发处理槽函数，Math通道TLOG复位
 * @param i
 *          匹配定时器索引号
 */
void DataProcess::slotTimerMatchTrigger(int i)
{
    //各运算通道相关TLOG计算
    foreach(DevBoard *pBoard, CfgChannel::instance()->getList_BoardVirtual()) {
        if (pBoard->getBoardType() == STATUS_BOARD_TYPE_MATH) {
            foreach (Channel *pChan, pBoard->getBoardChannel()) {
                quint32 chanCoder = pChan->getChannelCoder();
                quint16 number = CHANNEL_NUM(chanCoder) - 1;
                if (DevConfigPtr->chanMath[number].expressConfig.expSwtich &&
                    1 == DevConfigPtr->chanMath[number].expressConfig.expTlogType &&
                    (quint32)(i+1) == DevConfigPtr->chanMath[number].expressConfig.expTlogNo &&
                    DevConfigPtr->chanMath[number].expressConfig.expTlogReset) {
                    qobject_cast<DevChannelMath *>(pChan)->p_expAnalysis->m_tlogCalculate.tlogValueReset();
                }
            }
        }
    }
}

/*
 * 功能：
 * 	  输入值处理，测量通道输入数据预处理
 * 参数：
 * 	  1.SUB_BOARD_DATA *dataSrc：模块查询数据
 *       2.SUB_BOARD_DATA_PROCESSED *dataDest：模块查询数据结算结果
 * 返回值：
 * 	  无
 */
void DataProcess::dataInputProcess(SUB_BOARD_DATA *dataSrc, SUB_BOARD_DATA_PROCESSED *dataDest)
{
    //AI类型通道测量输入数据拷贝
    ::memcpy(dataDest->dataSample, dataSrc->dataSample, sizeof(float)*UNIT_CHAN_NUM_USE_MAX);
    //DI脉冲类型脉冲计数数据拷贝
    ::memcpy(dataDest->dataDICount, dataSrc->dataDICount, sizeof(quint32)*UNIT_CHAN_NUM_USE_MAX);
    //DI、DO类型通道输入数据、状态数据拷贝
    for (int i=0; i<UNIT_CHAN_NUM_USE_MAX; ++i) {
        if (dataSrc->bitsInputDI & (1<<i)) {
            dataDest->bitsInputDI[i] = ON;
            dataDest->bitsStatusDI[i] = ON;
        } else {
            dataDest->bitsInputDI[i] = OFF;
            dataDest->bitsStatusDI[i] = OFF;
        }

        if (dataSrc->bitsInputDO & (1<<i)) {
            dataDest->bitsInputDO[i] = ON;
            dataDest->bitsStatusDO[i] = ON;
        } else {
            dataDest->bitsInputDO[i] = OFF;
            dataDest->bitsStatusDO[i] = OFF;
        }
    }
}

/*
 * 功能：
 * 	  测量输入数据（输入值）映射-->原始数据对象
 * 参数：
 * 	  1.quint8 unit：单元号（0: 本机，1-6:扩展）
 *       2.quint8 module：模块号(本机：0-9, 扩展：0-5)
 *       3.quint16 status : 板卡状态
 *       4.SUB_BOARD_DATA_PROCESSED *pData： 数据指针
 * 返回值：
 * 	  无
 */
void DataProcess::dataInputRemap(quint8 unit, quint8 module, quint16 status, SUB_BOARD_DATA_PROCESSED *pData)
{
    void *ptrModbusReg;
    quint8 *ptrBitReg, *ptrBitStatusReg;
    quint8 chan0Num = STATUS_BOARD_CHAN0_NUM(status);
    quint8 chan1Num = STATUS_BOARD_CHAN1_NUM(status);

    switch (STATUS_BOARD_TYPE(status))
    {
    case STATUS_BOARD_TYPE_AI:
        chan0Num = chan0Num>AI_UNIT_CHAN_NUM_USE ? AI_UNIT_CHAN_NUM_USE : chan0Num;
        switch (unit)
        {
        case 0: //本机
            //采样输入数据
            ptrModbusReg = &g_originDataMap.inputReg_MainUnit_IO_Data_Int32[module];
            ::memcpy((void*)ptrModbusReg, (void*)pData->dataSample, sizeof(float)*chan0Num);
            ptrModbusReg = &g_originDataMap.inputReg_MainUnit_IO_Data_Int32_Linear[module];
            ::memcpy((void*)ptrModbusReg, (void*)pData->dataSample, sizeof(float)*UNIT_CHAN_NUM_LINEAR);//连续区域每个模块最多10个通道
            //通道状态值
            ptrModbusReg = &g_originDataMap.inputReg_MainUnit_IO_Status_Int16[module];
            ::memcpy((void*)ptrModbusReg, (void*)pData->dataStatus, sizeof(qint16)*chan0Num);
            ptrModbusReg = &g_originDataMap.inputReg_MainUnit_IO_Status_Int16_Linear[module];
            ::memcpy((void*)ptrModbusReg, (void*)pData->dataStatus, sizeof(qint16)*UNIT_CHAN_NUM_LINEAR);//连续区域每个模块最多10个通道
            break;
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
            //采样输入数据
            ptrModbusReg = &g_originDataMap.inputReg_ExtUnit_IO_Data_Int32[unit-1][module];
            ::memcpy((void*)ptrModbusReg, (void*)pData->dataSample, sizeof(float)*chan0Num);
            //通道状态值
            ptrModbusReg = &g_originDataMap.inputReg_ExtUnit_IO_Status_Int16[unit-1][module];
            ::memcpy((void*)ptrModbusReg, (void*)pData->dataStatus, sizeof(qint16)*chan0Num);
            break;
        default:
            break;
        }
        break;
    case STATUS_BOARD_TYPE_DI:
        //本机DI----输入寄存器、输入继电器；扩展DI----输入寄存器
        chan0Num = chan0Num>DIO_UNIT_CHAN_NUM_USE_MAX ? DIO_UNIT_CHAN_NUM_USE_MAX : chan0Num;
        switch (unit) {
        case 0: //本机
            //DI脉冲计数模式---输入寄存器INT32
            ptrModbusReg = &g_originDataMap.inputReg_MainUnit_IO_Data_Int32[module];
            ::memcpy((void*)ptrModbusReg, (void *)pData->dataDICount, sizeof(quint32) * chan0Num);
            ptrModbusReg = &g_originDataMap.inputReg_MainUnit_IO_Data_Int32_Linear[module];
            ::memcpy((void*)ptrModbusReg, (void *)pData->dataDICount, sizeof(quint32) * UNIT_CHAN_NUM_LINEAR);//连续区域每个模块最多10个通道
            //DI非脉冲计数模式---输入寄存器INT16
            ptrModbusReg = &g_originDataMap.inputReg_MainUnit_IO_Data_Int16[module];
            ::memcpy((void*)ptrModbusReg, (void *)pData->bitsInputDI, sizeof(quint16) * chan0Num);
            ptrModbusReg = &g_originDataMap.inputReg_MainUnit_IO_Data_Int16_Linear[module];
            ::memcpy((void*)ptrModbusReg, (void *)pData->bitsInputDI, sizeof(quint16) * UNIT_CHAN_NUM_LINEAR);//连续区域每个模块最多10个通道
            //通道状态---输入寄存器
            ptrModbusReg = &g_originDataMap.inputReg_MainUnit_IO_Status_Int16[module];
            ::memcpy((void*)ptrModbusReg, (void *)pData->dataStatus, sizeof(quint16) *chan0Num);
            ptrModbusReg = &g_originDataMap.inputReg_MainUnit_IO_Status_Int16_Linear[module];
            ::memcpy((void*)ptrModbusReg, (void *)pData->dataStatus, sizeof(quint16) *UNIT_CHAN_NUM_LINEAR);//连续区域每个模块最多10个通道
            //输入继电器
            ptrBitReg = (quint8 *)&g_originDataMap.relay_MainUnit_IO_Data_Bit[module];
            ptrBitStatusReg = (quint8 *)&g_originDataMap.relay_MainUnit_IO_Status_Bit[module];
            for (int i=0; i<chan0Num; ++i) {
                ptrBitReg[i] = pData->bitsInputDI[i];
                ptrBitStatusReg[i] = pData->bitsStatusDI[i];
            }
            break;
        case 1: //扩展单元
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
            //DI脉冲计数模式---输入寄存器INT32
            ptrModbusReg = &g_originDataMap.inputReg_ExtUnit_IO_Data_Int32[unit - 1][module];
            ::memcpy((void*)ptrModbusReg, (void *)pData->dataDICount, sizeof(quint32) * chan0Num);
            //DI非脉冲计数模式---输入寄存器INT16
            ptrModbusReg = &g_originDataMap.inputReg_ExtUnit_IO_Data_Int16[unit - 1][module];
            ::memcpy((void*)ptrModbusReg, (void *)pData->bitsInputDI, sizeof(quint16) * chan0Num);
            //通道状态---输入寄存器
            ptrModbusReg = &g_originDataMap.inputReg_ExtUnit_IO_Status_Int16[unit - 1][module];
            ::memcpy((void*)ptrModbusReg, (void *)pData->dataStatus, sizeof(quint16) *chan0Num);
            break;
        }
        break;
    case STATUS_BOARD_TYPE_DO:
        //本机DO----保持寄存器、线圈；扩展DO----保持寄存器
        chan0Num = chan0Num>DIO_UNIT_CHAN_NUM_USE_MAX ? DIO_UNIT_CHAN_NUM_USE_MAX : chan0Num;
        switch (unit) {
        case 0: //本机
            //DO---保持寄存器INT16
            ptrModbusReg = &g_originDataMap.holdReg_mainUnit_IO_Data_Int16[module];
            ::memcpy((void*)ptrModbusReg, (void *)pData->bitsInputDO, sizeof(quint16) * chan0Num);
            ptrModbusReg = &g_originDataMap.holdReg_MainUnit_IO_Data_Int16_Linear[module];
            ::memcpy((void*)ptrModbusReg, (void *)pData->bitsInputDO, sizeof(quint16) * AI_UNIT_CHAN_NUM_USE);//连续区域每个模块最多10个通道
            //DO通道状态---保存寄存器
            ptrModbusReg = &g_originDataMap.holdReg_mainUnit_IO_Status_Int16[module];
            ::memcpy((void*)ptrModbusReg, (void *)pData->dataStatus, sizeof(quint16) *chan0Num);
            ptrModbusReg = &g_originDataMap.holdReg_MainUnit_IO_Status_Int16_Linear[module];
            ::memcpy((void*)ptrModbusReg, (void *)pData->dataStatus, sizeof(quint16) *AI_UNIT_CHAN_NUM_USE);//连续区域每个模块最多10个通道
            //保持寄存器
            ptrBitReg = (quint8 *)&g_originDataMap.coil_MainUnit_IO_Data_Bit[module];
            ptrBitStatusReg = (quint8 *)&g_originDataMap.coil_MainUnit_IO_Status_Bit[module];
            for (int i=0; i<chan0Num; ++i) {
                ptrBitReg[i] = pData->bitsInputDO[i];
                ptrBitStatusReg[i] = pData->bitsStatusDO[i];
            }
            break;
        case 1: //扩展单元DO
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
            //DO---保持寄存器INT16
            ptrModbusReg = &g_originDataMap.holdReg_ExtUnit_IO_Data_Int16[unit - 1][module];
            ::memcpy((void*)ptrModbusReg, (void *)pData->bitsInputDO, sizeof(quint16) * chan0Num);
            //DO通道状态---保存寄存器
            ptrModbusReg = &g_originDataMap.holdReg_ExtUnit_IO_Status_Int16[unit - 1][module];
            ::memcpy((void*)ptrModbusReg, (void *)pData->dataStatus, sizeof(quint16) *chan0Num);
            break;
        }
        break;
    case STATUS_BOARD_TYPE_DIO:
        //本机DIO---输入寄存器（DI）、保持寄存器（DO）、输入继电器（DI）、线圈（DO）
        chan0Num = chan0Num>DIO_UNIT_CHAN_NUM_USE_MAX ? DIO_UNIT_CHAN_NUM_USE_MAX : chan0Num;
        chan1Num = chan1Num>DIO_UNIT_CHAN_NUM_USE_MAX ? DIO_UNIT_CHAN_NUM_USE_MAX : chan1Num;
        //DI
        switch (unit) {
        case 0: //本机
            //DI脉冲计数模式---输入寄存器INT32
            ptrModbusReg = &g_originDataMap.inputReg_MainUnit_IO_Data_Int32[module];
            ::memcpy((void*)ptrModbusReg, (void *)pData->dataDICount, sizeof(quint32) * chan0Num);
            ptrModbusReg = &g_originDataMap.inputReg_MainUnit_IO_Data_Int32_Linear[module];
            ::memcpy((void*)ptrModbusReg, (void *)pData->dataDICount, sizeof(quint32) * UNIT_CHAN_NUM_LINEAR);//连续区域每个模块最多10个通道
            //DI非脉冲计数模式---输入寄存器INT16
            ptrModbusReg = &g_originDataMap.inputReg_MainUnit_IO_Data_Int16[module];
            ::memcpy((void*)ptrModbusReg, (void *)pData->bitsInputDI, sizeof(quint16) * chan0Num);
            ptrModbusReg = &g_originDataMap.inputReg_MainUnit_IO_Data_Int16_Linear[module];
            ::memcpy((void*)ptrModbusReg, (void *)pData->bitsInputDI, sizeof(quint16) * UNIT_CHAN_NUM_LINEAR);//连续区域每个模块最多10个通道
            //通道状态---输入寄存器
            ptrModbusReg = &g_originDataMap.inputReg_MainUnit_IO_Status_Int16[module];
            ::memcpy((void*)ptrModbusReg, (void *)pData->dataStatus, sizeof(quint16) *chan0Num);
            ptrModbusReg = &g_originDataMap.inputReg_MainUnit_IO_Status_Int16_Linear[module];
            ::memcpy((void*)ptrModbusReg, (void *)pData->dataStatus, sizeof(quint16) *UNIT_CHAN_NUM_LINEAR);//连续区域每个模块最多10个通道
            //输入继电器
            ptrBitReg = (quint8 *)&g_originDataMap.relay_MainUnit_IO_Data_Bit[module];
            ptrBitStatusReg = (quint8 *)&g_originDataMap.relay_MainUnit_IO_Status_Bit[module];
            for (int i=0; i<chan0Num; ++i) {
                ptrBitReg[i] = pData->bitsInputDI[i];
                ptrBitStatusReg[i] = pData->bitsStatusDI[i];
            }
            break;
        case 1: //扩展单元
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
            //DI脉冲计数模式---输入寄存器INT32
            ptrModbusReg = &g_originDataMap.inputReg_ExtUnit_IO_Data_Int32[unit - 1][module];
            ::memcpy((void*)ptrModbusReg, (void *)pData->dataDICount, sizeof(quint32) * chan0Num);
            //DI非脉冲计数模式---输入寄存器INT16
            ptrModbusReg = &g_originDataMap.inputReg_ExtUnit_IO_Data_Int16[unit - 1][module];
            ::memcpy((void*)ptrModbusReg, (void *)pData->bitsInputDI, sizeof(quint16) * chan0Num);
            //通道状态---输入寄存器
            ptrModbusReg = &g_originDataMap.inputReg_ExtUnit_IO_Status_Int16[unit - 1][module];
            ::memcpy((void*)ptrModbusReg, (void *)pData->dataStatus, sizeof(quint16) *chan0Num);
            break;
        }

        //DO
        switch (unit) {
        case 0: //本机
            //DO---保持寄存器INT16
            ptrModbusReg = &g_originDataMap.holdReg_mainUnit_IO_Data_Int16[module];
            ::memcpy((void*)ptrModbusReg, (void *)pData->bitsInputDO, sizeof(quint16) * chan1Num);
            ptrModbusReg = &g_originDataMap.holdReg_MainUnit_IO_Data_Int16_Linear[module];
            ::memcpy((void*)ptrModbusReg, (void *)pData->dataDICount, sizeof(quint16) * AI_UNIT_CHAN_NUM_USE);//连续区域每个模块最多10个通道
            //DO通道状态---保存寄存器
            ptrModbusReg = &g_originDataMap.holdReg_mainUnit_IO_Status_Int16[module];
            ::memcpy((void*)ptrModbusReg, (void *)pData->dataStatus, sizeof(quint16) *chan1Num);
            ptrModbusReg = &g_originDataMap.holdReg_MainUnit_IO_Status_Int16_Linear[module];
            ::memcpy((void*)ptrModbusReg, (void *)pData->dataStatus, sizeof(quint16) *AI_UNIT_CHAN_NUM_USE);//连续区域每个模块最多10个通道
            //保持寄存器
            ptrBitReg = (quint8 *)&g_originDataMap.coil_MainUnit_IO_Data_Bit[module];
            ptrBitStatusReg = (quint8 *)&g_originDataMap.coil_MainUnit_IO_Status_Bit[module];
            for (int i=0; i<chan1Num; ++i) {
                ptrBitReg[i] = pData->bitsInputDO[i];
                ptrBitStatusReg[i] = pData->bitsStatusDO[i];
            }
            break;
        case 1: //扩展单元DO
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
            //DO---保持寄存器INT16
            ptrModbusReg = &g_originDataMap.holdReg_ExtUnit_IO_Data_Int16[unit - 1][module];
            ::memcpy((void*)ptrModbusReg, (void *)pData->bitsInputDO, sizeof(quint16) * chan1Num);
            //DO通道状态---保存寄存器
            ptrModbusReg = &g_originDataMap.holdReg_ExtUnit_IO_Status_Int16[unit - 1][module];
            ::memcpy((void*)ptrModbusReg, (void *)pData->dataStatus, sizeof(quint16) *chan1Num);
            break;
        }
        break;
    case STATUS_BOARD_TYPE_EXT:
        break;
    default:
        break;
    }
}

/**
 * @brief DataProcess::boardStatusCheck
 *       板卡状态检查，错误配置，则清除;
 * @param pConfigStatus
 *       配置状态指针
 * @return
 * 	  true--配置正常, 需要进行配置表更新
 *       false--配置错误
 */
bool DataProcess::boardStatusCheck(quint16 *pConfigStatus)
{
    if (0 == STATUS_BOARD_STATUS(*pConfigStatus)) {
        return false;
    }

    switch (STATUS_BOARD_TYPE(*pConfigStatus)) {
    case STATUS_BOARD_TYPE_AI:
        if (STATUS_BOARD_CHAN0_NUM(*pConfigStatus) > MAIN_UNIT_MODULE_NUM) {
            *pConfigStatus = 0;
            return false;
        } else {
            return true;
        }
    case STATUS_BOARD_TYPE_DI:
        if (STATUS_BOARD_CHAN0_NUM(*pConfigStatus) > DIO_UNIT_CHAN_NUM_USE_MAX) {
            *pConfigStatus = 0;
            return false;
        } else {
            return true;
        }
    case STATUS_BOARD_TYPE_DO:
        if (STATUS_BOARD_CHAN0_NUM(*pConfigStatus) > DIO_UNIT_CHAN_NUM_USE_MAX) {
            *pConfigStatus = 0;
            return false;
        } else {
            return true;
        }
    case STATUS_BOARD_TYPE_DIO:
        if ((STATUS_BOARD_CHAN0_NUM(*pConfigStatus) + STATUS_BOARD_CHAN1_NUM(*pConfigStatus)) \
             > DIO_UNIT_CHAN_NUM_USE_MAX) {
            *pConfigStatus = 0;
            return false;
        } else {
            return true;
        }
    case STATUS_BOARD_TYPE_EXT:
        return false;
    default:
        return false;
    }
}

/**
 * @brief DataProcess::boardConfigUpdate
 *          根据当前识别的配置信息，更新板卡相关配置
 * @param unit
 *          单元号 0-6, 0：主机，其它：扩展
 * @param module
 *          模块号，主机：0-9, 扩展： 0-6
 */
void DataProcess::boardConfigUpdate(quint8 unit, quint8 module)
{
    BOARD_IO_CONFIG *pBoardConfig;
    quint16 *pPreConfigPackID;

    if (CHANNEL_UNIN_MAIN == unit) {
        pBoardConfig = &DevConfigPtr->sysStatus.mainUnit_Recognition[module];
        pPreConfigPackID = &DevConfigPtr->sysStatus.configPacketID.mainUnit_PrePackID[module];
    } else {
        pBoardConfig = &DevConfigPtr->sysStatus.extUnit_Recognition[unit-1][module];
        pPreConfigPackID = &DevConfigPtr->sysStatus.configPacketID.extUnit_PrePackID[unit-1][module];
    }

    /*板卡动态插拔支持*/
    if (*pPreConfigPackID != pBoardConfig->boardReg_config[BOARD_IO_CONFIG_NUM-1]) {

        *pPreConfigPackID = pBoardConfig->boardReg_config[BOARD_IO_CONFIG_NUM-1];
        boardReset(unit, module, pBoardConfig->boardReg_status);
        configReset(unit, module, pBoardConfig->boardReg_status);
    }
}


/*板卡硬件配置复位*/
void DataProcess::boardReset(quint8 unit, quint8 module, quint16 status)
{
    Q_ASSERT(unit<=EXT_UNIT_NUM);
    int addr, slave;
    quint16 config[BOARD_IO_CONFIG_NUM];

    ::memset(config, 0x00, sizeof(config));

    switch (STATUS_BOARD_TYPE(status)) {
    case STATUS_BOARD_TYPE_AI:
        for (int i=0; i<AI_UNIT_CHAN_NUM_USE; ++i) {
            config[i] = AI_CHANNEL_CONFIG(AI_RANGE_Type_Vlot, AI_Range_Volt_2V, 0);
        }
        config[BOARD_IO_AI_AD_REG] = Board_ADTime_Auto;
        config[BOARD_IO_AI_BURNUP_REG] = 1100;
        config[BOARD_IO_AI_BURNDOWN_REG] = -100;
        break;
    case STATUS_BOARD_TYPE_DI:
        for (int i=0; i<DIO_UNIT_CHAN_NUM_USE_MAX; ++i) {
            config[i] = DI_CHANNEL_CONFIG(0);
        }
        break;
     case STATUS_BOARD_TYPE_DO:
        for (int i=0; i<DIO_UNIT_CHAN_NUM_USE_MAX; ++i) {
            config[i] = DO_CHANNEL_CONFIG(0);
        }
        break;
     case STATUS_BOARD_TYPE_DIO:
        for (int i=0; i<8; ++i) {
            config[i] = DI_CHANNEL_CONFIG(0);
        }
        for (int i=8; i<DIO_UNIT_CHAN_NUM_USE_MAX; ++i) {
            config[i] = DO_CHANNEL_CONFIG(0);
        }
        break;
     default:
        return;
    }

    //子板配置
    if (unit==CHANNEL_UNIN_MAIN) {  //本机
        addr = BOARD_IO_CONFIG_START;
        slave = module+1;
    } else {    //扩展单元
        addr = sizeof(BOARD_EXT_CONFIG) +
                (unit-1)*sizeof(BOARD_IO_CONFIG)*EXT_UNIT_MODULE_NUM + module*sizeof(BOARD_IO_CONFIG);
        slave = m_extModuleSlave;
    }
    _boardInterface->tcflushConfigBus();
    _boardInterface->setHoldRegister(addr, BOARD_IO_CONFIG_NUM, config, slave);

    //扩展单元，发送IO板卡修改标记
    if(unit != CHANNEL_UNIN_MAIN) {
        uint16_t data = 0xAA;
        int regAddr = BOARD_IO_CONFIG_START + (unit-1)*EXT_UNIT_MODULE_NUM + module;
        _boardInterface->setHoldRegister(regAddr, 1, &data, slave);
    }
}

/*板卡系统配置复位*/
void DataProcess::configReset(quint8 unit, quint8 module, quint16 status)
{
    switch (STATUS_BOARD_TYPE(status)) {
    case STATUS_BOARD_TYPE_AI:
        for (int i=0; i<AI_UNIT_CHAN_NUM_USE; ++i) {
            CfgConfig::instance()->resetChannelConfig(unit, module, i, Channel_Type_AI);
        }
        break;
    case STATUS_BOARD_TYPE_DI:
        for (int i=0; i<DIO_UNIT_CHAN_NUM_USE_MAX; ++i) {
            CfgConfig::instance()->resetChannelConfig(unit, module, i, Channel_Type_DI);
        }
        break;
     case STATUS_BOARD_TYPE_DO:
        for (int i=0; i<DIO_UNIT_CHAN_NUM_USE_MAX; ++i) {
            CfgConfig::instance()->resetChannelConfig(unit, module, i, Channel_Type_DO);
        }
        break;
     case STATUS_BOARD_TYPE_DIO:
        for (int i=0; i<(DIO_UNIT_CHAN_NUM_USE_MAX>>1); ++i) {
            CfgConfig::instance()->resetChannelConfig(unit, module, i, Channel_Type_DI);
            CfgConfig::instance()->resetChannelConfig(unit, module, i, Channel_Type_DO);
        }
        break;
     default:
        return;
    }
}

bool DataProcess::loadPlugin()
{
    QDir pluginsDir(qApp->applicationDirPath());
    pluginsDir.cd("plugins");

    foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
        QPluginLoader pluginLoader(pluginsDir.absoluteFilePath(fileName));
        QObject *plugin = pluginLoader.instance();
        if (plugin) {
            _boardInterface = qobject_cast<BoardInterface *>(plugin);
            if (_boardInterface)
                return true;
        }
    }

    return false;
}
