/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：ThreadDataSync.cpp
 * 概   要：数据同步线程。
 *                前置任务：数据处理线程 | 数据通信线程
 *                内容：1.基于前置任务，根据系统配置生成有效的显示数据和事件数据
 *                            2.添加有效的显示数据和事件数据至各自的缓冲区，并更新相应的数据库
 *                            3.根据配置决定是否刷新到数据库
 *
 * 当前版本：V1.0.0
 * 作   者：王 品
 * 完成日期：2015-6-16
 *
 * 修改版本：
 * 修改说明：
 * 修改作者：
 *
 ********************************************************************/
#include "threaddatasync.h"
#include "threaddatarecord.h"
#include "../GlobalData/globalDef.h"
#include "../Common/eventaction.h"

#include <QMutex>
#include <QSharedMemory>
#include <QDateTime>
#include <QDebug>

typedef struct DATA_STATUS_STRUCT {
    //系统当前记录状态，false--记录停止， true-记录运行中
    bool  recordStatus;
    //停止标记，true：表示下个测量周期数据是第一个采样数据点
    bool   stopFlag;
    //手动采样标记
    bool   manualSampleFlag;
    //记录个数，根据测量周期、显示记录周期计算：多少个测量数据生成一个显示数据
    quint32 dispRecordCount;
    //显示测量周期计数
    quint32 dispcounts;
    //记录个数，根据测量周期、事件记录周期计算：多少个测量数据生成一个事件数据
    quint32 eventRecordCount;
    //事件测量周期计数
    quint32 eventCounts;
    //趋势界面水平分割数
//    quint32 screenDiv;
    //趋势切换最大、最小周期
    quint32 trendPeriodMin;
    quint32 trendPeriodMax;
}DATA_STATUS;

static QMutex   _statusLocker;    //数据状态锁
static DATA_STATUS  _dataStatus;//数据状态
static DISPLAY_DATA_RECORD     _displayDataRecord;    //显示数据缓冲
static EVENT_DATA_RECORD        _eventDataRecord;       //事件数据缓冲
static EVENT_DATA_RECORD        _manualDataRecord;    //手动采样数据缓冲
static REPORT_DATA_RECORD     _reportDataRecord;       //报表数据缓冲

//事件记录周期表
static const quint32 s_eventRecordInterval[EVNET_INTERVAL_NUM] = {
    Event_Record_Interval_100ms,
    Event_Record_Interval_200ms,
    Event_Record_Interval_500ms,
    Event_Record_Interval_1s,
    Event_Record_Interval_2s,
    Event_Record_Interval_5s,
    Event_Record_Interval_10s,
    Event_Record_Interval_15s,
    Event_Record_Interval_20s,
    Event_Record_Interval_30s,
    Event_Record_Interval_1min,
    Event_Record_Interval_2min,
    Event_Record_Interval_5min,
    Event_Record_Interval_10min,
    Event_Record_Interval_15min,
    Event_Record_Interval_20min,
    Event_Record_Interval_30min,
};

//事件记录周期对应的趋势周期表
static const quint32 s_trendDiv[EVNET_INTERVAL_NUM] = {
    5, 10, 15, 30, 60, 150, 300, 450, 600, 900, 1800, 3600, 9000, 18000, 27000, 36000, 54000,
};

static QMutex s_mutex;
static QQueue<MODBUS_REGISTER_MAP> s_dataBuffer;

extern CfgTrendGroup *p_gCfgTrendGroup;

/*
 * 功能：
 * 	  数据同步处理类静态成员函数，返回静态数据同步处理实例指针
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
ThreadDataSync *ThreadDataSync::instance()
{
    static ThreadDataSync data;
    return &data;
}

/*
 * 功能：
 * 	  数据记录状态修改
 * 参数：
 * 	  1.bool onoff: 记录on/off
 * 返回值：
 * 	  无
 */
void ThreadDataSync::setRecordStatus(bool onoff)
{
    bool status;

    _statusLocker.lock();
    status = !_dataStatus.recordStatus;
    _statusLocker.unlock();

    if (status != onoff)    return;

    _statusLocker.lock();
    _dataStatus.recordStatus = status;
    _dataStatus.stopFlag = true;
    _statusLocker.unlock();

    if (status) { //每次启动记录时，重新计算一次相关参数 && 计数清零
        calculateParameter();

        _displayDataRecord.dataCount = 0;
        _displayDataRecord.recordInterval = _dataStatus.dispRecordCount * DevConfigPtr->measureInterval_ms;
        _eventDataRecord.dataCount = 0;
        _eventDataRecord.recordInterval = _dataStatus.eventRecordCount * DevConfigPtr->measureInterval_ms;

        //趋势状态设置
        p_gCfgTrendGroup->setRTState(Running);
    } else {    //记录停止
        if (_displayDataRecord.dataCount>0 && \
            DevConfigPtr->recordMode != Record_Mode_Event) {
            emit sigSyncStopDisp(); //记录结束时，发送显示数据停止记录信号
        }
        if (_eventDataRecord.dataCount>0 && \
            DevConfigPtr->recordMode != Record_Mode_Display) {
            emit sigSyncStopEvent();//记录结束时，发送事件数据停止记录信号
        }
        emit sigSyncStop(); //记录停止信号

        //清楚缓冲区
        s_mutex.lock();
        s_dataBuffer.clear();
        s_mutex.unlock();
        //趋势状态设置
//        p_gCfgTrendGroup->setRTState(StandBy);
    }

    /*仪表状态设定*/
    CfgConfig::instance()->setDeviceStatus_Record(onoff);
}

/**
 * @brief ThreadDataSync::getRecordStatus
 *          获取记录状态
 * @return
 *          true--记录中，false--记录停止
 */
bool ThreadDataSync::getRecordStatus()
{
    return _dataStatus.recordStatus;
}
/*
 * 功能：
 * 	  启动手动采样
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
void ThreadDataSync::setManualSample()
{
    _statusLocker.lock();
    _dataStatus.manualSampleFlag = true;
    _statusLocker.unlock();
}

/*
 * 功能：
 * 	  数据帧缓冲
 * 参数：
 * 	  MODBUS_REGISTER_MAP *pData : 数据帧对象指针
 * 返回值：
 * 	  无
 */
void ThreadDataSync::buffDataFrame(MODBUS_REGISTER_MAP *pData)
{
    s_mutex.lock();

    if (s_dataBuffer.count()>=FRAME_BUFFER_NUM) {
        qDebug()<<"Warning----Buffer Over\n\n\n\n";
        s_dataBuffer.dequeue();
    }
    s_dataBuffer.enqueue(*pData);

    s_mutex.unlock();
}

/*
 * 功能：
 * 	  数据同步处理类构造函数
 * 参数：
 * 	  1.QObject *parent: 父对象，默认为0
 * 返回值：
 * 	  无
 */
ThreadDataSync::ThreadDataSync(QObject *parent) :
    QObject(parent)
{
    mapInit();

    SyncProcess *syncProcess = new SyncProcess;
    syncProcess->moveToThread(&m_dataSyncThread);
    connect(&m_dataSyncThread, &QThread::finished, syncProcess, &QObject::deleteLater);
    connect(&m_dataSyncThread, &QThread::started, syncProcess, &SyncProcess::slotStart);

    connect(this, &ThreadDataSync::sigSyncStopDisp, syncProcess, &SyncProcess::slotSyncDispStoped);
    connect(this, &ThreadDataSync::sigSyncStopEvent, syncProcess, &SyncProcess::slotSyncEventStoped);
    connect(this, &ThreadDataSync::sigSyncStop, syncProcess, &SyncProcess::slotSyncReportStoped);

    calculateParameter();

    m_dataSyncThread.start();
}

/*
 * 功能：
 * 	  事件记录周期与趋势周期映射表初始化
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
void ThreadDataSync::mapInit()
{
    for (int i=0; i<EVNET_INTERVAL_NUM; ++i){
        m_event2display[s_eventRecordInterval[i]] = s_trendDiv[i];
    }
}

/*
 * 功能：
 * 	  事件/显示数据同步缓冲相关参数计算
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
void ThreadDataSync::calculateParameter()
{
    quint8 countDIV;

    quint32 testPeriod, eventPeriod;

    //测量周期
    testPeriod = DevConfigPtr->measureInterval_ms;
    //事件记录周期
    eventPeriod = DevConfigPtr->eventRecordInterval_ms;

    //事件记录模式时，参考事件记录周期与趋势周期关系表
    if (Record_Mode_Event == DevConfigPtr->recordMode &&
         (DevConfigPtr->displayTrend.trendIntervalSecondSwitch == false)) {
        m_trendTimeDiv = m_event2display.value(eventPeriod);
    } else {
        if (DevConfigPtr->displayTrend.trendIntervalSecondSwitch)
            m_trendTimeDiv = qMin(DevConfigPtr->displayTrend.trendInterval_s, DevConfigPtr->displayTrend.trendIntervalSecond);
        else
            m_trendTimeDiv = DevConfigPtr->displayTrend.trendInterval_s;
    }

    countDIV = m_trendTimeDiv>Trend_Interval_10s ? 30 : 50;

    _statusLocker.lock();
    _dataStatus.dispRecordCount = m_trendTimeDiv * 1000 / countDIV / testPeriod;
    _dataStatus.eventRecordCount = eventPeriod / testPeriod;
    if (DevConfigPtr->displayTrend.trendIntervalSecondSwitch) {
        _dataStatus.trendPeriodMax = qMax(DevConfigPtr->displayTrend.trendInterval_s, DevConfigPtr->displayTrend.trendIntervalSecond);
        _dataStatus.trendPeriodMin = qMin(DevConfigPtr->displayTrend.trendInterval_s, DevConfigPtr->displayTrend.trendIntervalSecond);
    } else {
        _dataStatus.trendPeriodMax = _dataStatus.trendPeriodMin = m_trendTimeDiv;
    }
    _statusLocker.unlock();

    qDebug("Test Period: %dms, Event Period:%dms, Display Period:%ds\n", testPeriod, eventPeriod, m_trendTimeDiv);
    qDebug("Display buffer count:%d, Event buffer count:%d\n", _dataStatus.dispRecordCount, _dataStatus.eventRecordCount);
}

/*************************数据同步处理工作对象***************************/

SyncProcess::SyncProcess(QObject *parent):
    QObject(parent)
{
    p_dataSyncTimer = new QTimer(this);
}
/*
 * 功能：
 * 	  槽函数，运行于数据同步线程，根据系统当前状态及配置，生成显示数据、事件数据
 * 以及手动采样数据，并根据需要刷新响应的数据库
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
void SyncProcess::slotSyncProcess()
{
    cpyBufferData();

    if (m_dataBufferCount==0)
        return;

    //显示、事件数据生成
    _statusLocker.lock();
    if (_dataStatus.recordStatus && _dataStatus.stopFlag) { //记录开始且是第一次
//        qDebug("Sync First....%d\n", m_dataBufferCount);
        dataBufferProcess(true, m_dataBufferCount);
        _dataStatus.dispcounts += m_dataBufferCount;
        _dataStatus.eventCounts += m_dataBufferCount;
        _dataStatus.stopFlag = false;
        /*事件记录*/
        DevConfigPtr->eventSingleFlag = false;  //单次循环模式重新计算
        DevConfigPtr->eventTriggered = false;   //记录开始前的触发清零
    } else if (_dataStatus.recordStatus) { //记录运行中
//        qDebug("Sync ing....%d\n", m_dataBufferCount);
        if (0 == _dataStatus.dispcounts)
            dataBufferProcess(true, m_dataBufferCount);
        else
            dataBufferProcess(false, m_dataBufferCount);
        _dataStatus.dispcounts += m_dataBufferCount;
        _dataStatus.eventCounts += m_dataBufferCount;
    } else {    //记录已停止，清空测量周期计数
//        qDebug("Sync Stoped....%d", m_dataBufferCount);
        _dataStatus.dispcounts = 0;
        _dataStatus.eventCounts = 0;
    }

    //手动采样数据处理
    if (_dataStatus.manualSampleFlag){
        dataSampleProcess();
        _dataStatus.manualSampleFlag  = false;
        _manualDataRecord.dataStartTime = QDateTime::currentMSecsSinceEpoch();
        ThreadDataRecord::instance()->newManualData(&_manualDataRecord);
    }

    //有效显示数据信号发射
    if (_dataStatus.recordStatus && _dataStatus.dispcounts >= _dataStatus.dispRecordCount){
        do {
            _dataStatus.dispcounts -= _dataStatus.dispRecordCount;
            if (DevConfigPtr->recordMode != Record_Mode_Event) {
                _displayDataRecord.dataCount++;
                if (_displayDataRecord.dataCount == 1) {
//                    _displayDataRecord.screenDivs = _dataStatus.screenDiv;
                    _displayDataRecord.trendPeriodMax = _dataStatus.trendPeriodMax;
                    _displayDataRecord.trendPeriodMin = _dataStatus.trendPeriodMin;
                    _displayDataRecord.recordInterval = _dataStatus.dispRecordCount * DevConfigPtr->measureInterval_ms;
                    _displayDataRecord.dataStartTime = QDateTime::currentMSecsSinceEpoch();
                }
                ThreadDataRecord::instance()->newDispData(&_displayDataRecord);
            }
        }while(_dataStatus.dispcounts >= _dataStatus.dispRecordCount);
    }

    //有效事件数据信号发射
    if (_dataStatus.recordStatus && _dataStatus.eventCounts >= _dataStatus.eventRecordCount){
        do {
            _dataStatus.eventCounts -= _dataStatus.eventRecordCount;
            if (DevConfigPtr->recordMode != Record_Mode_Display) {
                if ((DevConfigPtr->eventRecordMode == Event_Record_Mode_Free) ||                  /*自由记录*/
                     (DevConfigPtr->eventRecordMode != Event_Record_Mode_Free && DevConfigPtr->eventTriggered)) {   /*非自由记录下事件触发，则记录*/
                    _eventDataRecord.dataCount++;
                    if (_eventDataRecord.dataCount == 1) {
//                        _eventDataRecord.screenDivs = _dataStatus.screenDiv;
                        _eventDataRecord.trendPeriodMax = _dataStatus.trendPeriodMax;
                        _eventDataRecord.trendPeriodMin = _dataStatus.trendPeriodMin;
                        _eventDataRecord.recordInterval = _dataStatus.eventRecordCount * DevConfigPtr->measureInterval_ms;
                        _eventDataRecord.dataStartTime = QDateTime::currentMSecsSinceEpoch();
                    }
                    ThreadDataRecord::instance()->newEventData(&_eventDataRecord);
                }
            }
        }while(_dataStatus.eventCounts >= _dataStatus.eventRecordCount);
    }
    _statusLocker.unlock();

    //报表数据处理
    dataReportProcess();

    //拷贝最新的数据包至共享内存
    char * pDataTable, *pDataTableTemp;
    if(DevShareMem.isAttached()) {
        DevShareMem.lock();
        pDataTable = static_cast<char*>(DevShareMem.data());
        pDataTableTemp = (char *)(&m_dataTemp[m_dataBufferCount-1]);
        ::memcpy(pDataTable, pDataTableTemp, sizeof(MODBUS_REGISTER_MAP));
        DevShareMem.unlock();
    }
    //通道报警状态切换
    chanAlarmStatusTrans();
    //通道报警输出执行
    chanAlarmOutput();
    //报警事件生成
    EventAction::instance()->genEvent_alarmIO();
    EventAction::instance()->genEvent_alarmMath();
    EventAction::instance()->genEvent_alarmComm();
    EventAction::instance()->genEvent_interSW();
    EventAction::instance()->genEvent_relayDO();

    m_dataBufferCount = 0;
}

/*
 * 功能：
 * 	  槽函数，发送显示数据记录停止包
 */
void SyncProcess::slotSyncDispStoped()
{
    _displayDataRecord.stopFlag = true;
    ThreadDataRecord::instance()->newDispData(&_displayDataRecord);
    _displayDataRecord.stopFlag = false;
}

/*
 * 功能：
 * 	  槽函数，发送事件数据记录停止包
 */
void SyncProcess::slotSyncEventStoped()
{
    _eventDataRecord.stopFlag = true;
    ThreadDataRecord::instance()->newEventData(&_eventDataRecord);
    _eventDataRecord.stopFlag = false;
}

/*
 * 功能：
 * 	  槽函数，发送报表数据停止包
 */
void SyncProcess::slotSyncReportStoped()
{
    sendReportDataWhole();
}

/*
 * 功能：
 * 	  数据帧缓冲，并根据配置生成相应的显示数据、事件数据
 * 参数：
 * 	  bool first：ture--记录开始的第一帧数据， false---非记录开始的第一帧数据
 * 返回值：
 * 	  无
 */
void SyncProcess::dataBufferProcess(bool first, quint8 count)
{
    MODBUS_REGISTER_MAP *pModbusRegMap;
    float * pData;
    quint16 *pDataDO;
    qint16 *pStatus;

    for (int i=0; i<count; ++i) {
        pModbusRegMap = &m_dataTemp[i];
            if (first) {
                first = false;
                //输入寄存器--本机--IO通道（AI/DI）
                for (int i=0; i<MAIN_UNIT_MODULE_NUM; i++){
                    for (int j=0; j<UNIT_CHAN_NUM_PRE; j++){
                        pData = (float *)&pModbusRegMap->inputReg_MainUnit_IO_Data_Float32[i][j<<1];
                        pStatus = &pModbusRegMap->inputReg_MainUnit_IO_Status_Int16[i][j];
                        _displayDataRecord.mainUnit_AI_Data[i][j].Max = *pData;
                        _displayDataRecord.mainUnit_AI_Data[i][j].Min = *pData;
                        _displayDataRecord.mainUnit_AI_Data[i][j].status = *pStatus;
                        _eventDataRecord.mainUnit_AI_Data[i][j].Max = *pData;
                        _eventDataRecord.mainUnit_AI_Data[i][j].Min = *pData;
                        _eventDataRecord.mainUnit_AI_Data[i][j].status = *pStatus;

                        _displayDataRecord.mainUnit_DI_Data[i][j].Max = *pData;
                        _displayDataRecord.mainUnit_DI_Data[i][j].Min = *pData;
                        _eventDataRecord.mainUnit_DI_Data[i][j].Max = *pData;
                        _eventDataRecord.mainUnit_DI_Data[i][j].Min = *pData;
                    }
                }

                //输入寄存器--扩展单元--IO通道（AI/DI）
                for (int i=0; i<EXT_UNIT_NUM; ++i){
                    for (int j=0; j<EXT_UNIT_MODULE_NUM; ++j){
                        for (int k=0; k<UNIT_CHAN_NUM_PRE; ++k){
                            switch (i) {
                            case 0:
                            case 1:
                            case 2:
                            case 3:
                            case 4:
                            case 5:
                                pData = (float *)&pModbusRegMap->inputReg_ExtUnit_IO_Data_Float32[i][j][k<<1];
                                pStatus = &pModbusRegMap->inputReg_ExtUnit_IO_Status_Int16[i][j][k];
                                break;
                            }

                            _displayDataRecord.extUnit_AI_Data[i][j][k].Max = *pData;
                            _displayDataRecord.extUnit_AI_Data[i][j][k].Min = *pData;
                            _displayDataRecord.extUnit_AI_Data[i][j][k].status = *pStatus;
                            _eventDataRecord.extUnit_AI_Data[i][j][k].Max = *pData;
                            _eventDataRecord.extUnit_AI_Data[i][j][k].Min = *pData;
                            _eventDataRecord.extUnit_AI_Data[i][j][k].status = *pStatus;

                            _displayDataRecord.extUnit_DI_Data[i][j][k].Max = *pData;
                            _displayDataRecord.extUnit_DI_Data[i][j][k].Min = *pData;
                            _eventDataRecord.extUnit_DI_Data[i][j][k].Max = *pData;
                            _eventDataRecord.extUnit_DI_Data[i][j][k].Min = *pData;
                        }
                    }
                }

                //保持寄存器--本机--IO通道（DO）
                for (int i=0; i<MAIN_UNIT_MODULE_NUM; ++i) {
                    for(int j=0; j<UNIT_CHAN_NUM_PRE; ++j){
                        pDataDO = (quint16 *)&pModbusRegMap->holdReg_mainUnit_IO_Data_Int16[i][j];
                        pStatus = &pModbusRegMap->holdReg_mainUnit_IO_Status_Int16[i][j];
                        _displayDataRecord.mainUnit_DO_Data[i][j].Max = *pDataDO;
                        _displayDataRecord.mainUnit_DO_Data[i][j].Min = *pDataDO;
                        _displayDataRecord.mainUnit_DO_Data[i][j].status = *pStatus;

                        _eventDataRecord.mainUnit_DO_Data[i][j].Max = *pDataDO;
                        _eventDataRecord.mainUnit_DO_Data[i][j].Min = *pDataDO;
                        _eventDataRecord.mainUnit_DO_Data[i][j].status = *pStatus;
                    }
                }

                //保持寄存器--扩展单元--IO通道（DO）
                for (int i=0; i<EXT_UNIT_NUM; ++i){
                    for (int j=0; j<EXT_UNIT_MODULE_NUM; ++j){
                        for (int k=0; k<UNIT_CHAN_NUM_PRE; ++k){
                            switch (i) {
                            case 0:
                            case 1:
                            case 2:
                            case 3:
                            case 4:
                            case 5:
                                pDataDO = (quint16 *)&pModbusRegMap->holdReg_ExtUnit_IO_Data_Int16[i][j][k];
                                pStatus = &pModbusRegMap->holdReg_ExtUnit_IO_Status_Int16[i][j][k];
                                break;
                            }

                            _displayDataRecord.extUnit_DO_Data[i][j][k].Max = *pDataDO;
                            _displayDataRecord.extUnit_DO_Data[i][j][k].Min = *pDataDO;
                            _displayDataRecord.extUnit_DO_Data[i][j][k].status = *pStatus;
                            _eventDataRecord.extUnit_DO_Data[i][j][k].Max = *pDataDO;
                            _eventDataRecord.extUnit_DO_Data[i][j][k].Min = *pDataDO;
                            _eventDataRecord.extUnit_DO_Data[i][j][k].status = *pStatus;
                        }
                    }
                }

                //MATH通道数据
                for (int i=0; i<MAIN_UNIT_MATH_CHAN_NUM_PRE; ++i){
                    pData = (float *)&pModbusRegMap->inputReg_MainUnit_Math_Data_Float32[i<<1];
                    pStatus = &pModbusRegMap->inputReg_MainUnit_Math_Status_Int16[i];
                    _displayDataRecord.device_Math_Data[i].Max = *pData;
                    _displayDataRecord.device_Math_Data[i].Min = *pData;
                    _displayDataRecord.device_Math_Data[i].status = *pStatus;

                    _eventDataRecord.device_Math_Data[i].Max = *pData;
                    _eventDataRecord.device_Math_Data[i].Min = *pData;
                    _eventDataRecord.device_Math_Data[i].status = *pStatus;
                }

                //COMM通道数据
                for (int i=0; i<MAIN_UNIT_COMM_CHAN_NUM_PRE; ++i){
                    pData = (float *)&pModbusRegMap->holdReg_MainUnit_Comm_Data_Float32[i<<1];
                    pStatus = &pModbusRegMap->holdReg_MainUnit_Comm_Status_Int16[i];
                    _displayDataRecord.device_Comm_Data[i].Max = *pData;
                    _displayDataRecord.device_Comm_Data[i].Min = *pData;
                    _displayDataRecord.device_Comm_Data[i].status = *pStatus;

                    _eventDataRecord.device_Comm_Data[i].Max = *pData;
                    _eventDataRecord.device_Comm_Data[i].Min = *pData;
                    _eventDataRecord.device_Comm_Data[i].status = *pStatus;
                }

            } else {
                //输入寄存器--本机--IO通道（AI/DI）
                for (int i=0; i<MAIN_UNIT_MODULE_NUM; i++){
                    for (int j=0; j<UNIT_CHAN_NUM_PRE; j++){
                        pData = (float *)&pModbusRegMap->inputReg_MainUnit_IO_Data_Float32[i][j<<1];
                        pStatus = &pModbusRegMap->inputReg_MainUnit_IO_Status_Int16[i][j];

                        _displayDataRecord.mainUnit_AI_Data[i][j].Max =\
                                _displayDataRecord.mainUnit_AI_Data[i][j].Max>(*pData) ? _displayDataRecord.mainUnit_AI_Data[i][j].Max : (*pData);
                        _displayDataRecord.mainUnit_AI_Data[i][j].Min = \
                                _displayDataRecord.mainUnit_AI_Data[i][j].Min<(*pData) ? _displayDataRecord.mainUnit_AI_Data[i][j].Min : (*pData);
                        _displayDataRecord.mainUnit_AI_Data[i][j].status = *pStatus;
                        _eventDataRecord.mainUnit_AI_Data[i][j].Max = *pData;
                        _eventDataRecord.mainUnit_AI_Data[i][j].Min = *pData;
                        _eventDataRecord.mainUnit_AI_Data[i][j].status = *pStatus;


                        _displayDataRecord.mainUnit_DI_Data[i][j].Max =\
                                _displayDataRecord.mainUnit_DI_Data[i][j].Max>(*pData) ? _displayDataRecord.mainUnit_DI_Data[i][j].Max : (*pData);
                        _displayDataRecord.mainUnit_DI_Data[i][j].Min = \
                                _displayDataRecord.mainUnit_DI_Data[i][j].Min<(*pData) ? _displayDataRecord.mainUnit_DI_Data[i][j].Min : (*pData);
                        _eventDataRecord.mainUnit_DI_Data[i][j].Max = *pData;
                        _eventDataRecord.mainUnit_DI_Data[i][j].Min = *pData;
                    }
                }

                //输入寄存器--扩展单元--IO通道（AI/DI）
                for (int i=0; i<EXT_UNIT_NUM; ++i){
                    for (int j=0; j<EXT_UNIT_MODULE_NUM; ++j){
                        for (int k=0; k<UNIT_CHAN_NUM_PRE; ++k){
                            switch (i) {
                            case 0:
                            case 1:
                            case 2:
                            case 3:
                            case 4:
                            case 5:
                                pData = (float *)&pModbusRegMap->inputReg_ExtUnit_IO_Data_Float32[i][j][k<<1];
                                pStatus = &pModbusRegMap->inputReg_ExtUnit_IO_Status_Int16[i][j][k];
                                break;
                            }

                            _displayDataRecord.extUnit_AI_Data[i][j][k].Max = \
                                    _displayDataRecord.extUnit_AI_Data[i][j][k].Max>(*pData) ? _displayDataRecord.mainUnit_AI_Data[i][j].Max : (*pData);
                            _displayDataRecord.extUnit_AI_Data[i][j][k].Min = \
                                    _displayDataRecord.extUnit_AI_Data[i][j][k].Min<(*pData) ? _displayDataRecord.extUnit_AI_Data[i][j][k].Min : (*pData);
                            _displayDataRecord.extUnit_AI_Data[i][j][k].status = *pStatus;
                            _eventDataRecord.extUnit_AI_Data[i][j][k].Max = *pData;
                            _eventDataRecord.extUnit_AI_Data[i][j][k].Min = *pData;
                            _eventDataRecord.extUnit_AI_Data[i][j][k].status = *pStatus;

                            _displayDataRecord.extUnit_DI_Data[i][j][k].Max = \
                                    _displayDataRecord.extUnit_DI_Data[i][j][k].Max>(*pData) ? _displayDataRecord.mainUnit_DI_Data[i][j].Max : (*pData);
                            _displayDataRecord.extUnit_DI_Data[i][j][k].Min = \
                                    _displayDataRecord.extUnit_DI_Data[i][j][k].Min<(*pData) ? _displayDataRecord.extUnit_DI_Data[i][j][k].Min : (*pData);
                            _eventDataRecord.extUnit_DI_Data[i][j][k].Max = *pData;
                            _eventDataRecord.extUnit_DI_Data[i][j][k].Min = *pData;
                        }
                    }
                }

                //保持寄存器--本机--IO通道（DO）
                for (int i=0; i<MAIN_UNIT_MODULE_NUM; ++i) {
                    for(int j=0; j<UNIT_CHAN_NUM_PRE; ++j){
                        pDataDO = (quint16 *)&pModbusRegMap->holdReg_mainUnit_IO_Data_Int16[i][j];
                        pStatus = &pModbusRegMap->holdReg_mainUnit_IO_Status_Int16[i][j];
                        _displayDataRecord.mainUnit_DO_Data[i][j].Max = \
                                _displayDataRecord.mainUnit_DO_Data[i][j].Max>(*pDataDO) ? _displayDataRecord.mainUnit_DO_Data[i][j].Max : (*pDataDO);
                        _displayDataRecord.mainUnit_DO_Data[i][j].Min = \
                                _displayDataRecord.mainUnit_DO_Data[i][j].Min<(*pDataDO) ? _displayDataRecord.mainUnit_DO_Data[i][j].Min : (*pDataDO);
                        _displayDataRecord.mainUnit_DO_Data[i][j].status = *pStatus;
                        _eventDataRecord.mainUnit_DO_Data[i][j].Max = *pDataDO;
                        _eventDataRecord.mainUnit_DO_Data[i][j].Min = *pDataDO;
                        _eventDataRecord.mainUnit_DO_Data[i][j].status = *pStatus;
                    }
                }

                //保持寄存器--扩展单元--IO通道（DO）
                for (int i=0; i<EXT_UNIT_NUM; ++i){
                    for (int j=0; j<EXT_UNIT_MODULE_NUM; ++j){
                        for (int k=0; k<UNIT_CHAN_NUM_PRE; ++k){
                            switch (i) {
                            case 0:
                            case 1:
                            case 2:
                            case 3:
                            case 4:
                            case 5:
                                pDataDO = (quint16 *)&pModbusRegMap->holdReg_ExtUnit_IO_Data_Int16[i][j][k];
                                pStatus = &pModbusRegMap->holdReg_ExtUnit_IO_Status_Int16[i][j][k];
                                break;
                            }
                            _displayDataRecord.extUnit_DO_Data[i][j][k].Max = \
                                    _displayDataRecord.extUnit_DO_Data[i][j][k].Max>(*pDataDO) ? _displayDataRecord.extUnit_DO_Data[i][j][k].Max : (*pDataDO);
                            _displayDataRecord.extUnit_DO_Data[i][j][k].Min = \
                                    _displayDataRecord.extUnit_DO_Data[i][j][k].Min<(*pDataDO) ? _displayDataRecord.extUnit_DO_Data[i][j][k].Min : (*pDataDO);
                            _displayDataRecord.extUnit_DO_Data[i][j][k].status = *pStatus;
                            _eventDataRecord.extUnit_DO_Data[i][j][k].Max = *pDataDO;
                            _eventDataRecord.extUnit_DO_Data[i][j][k].Min = *pDataDO;
                            _eventDataRecord.extUnit_DO_Data[i][j][k].status = *pStatus;
                        }
                    }
                }

                //MATH通道数据
                for (int i=0; i<MAIN_UNIT_MATH_CHAN_NUM_PRE; ++i){
                    pData = (float *)&pModbusRegMap->inputReg_MainUnit_Math_Data_Float32[i<<1];
                    pStatus = &pModbusRegMap->inputReg_MainUnit_Math_Status_Int16[i];

                    _displayDataRecord.device_Math_Data[i].Max = \
                            _displayDataRecord.device_Math_Data[i].Max>(*pData) ? _displayDataRecord.device_Math_Data[i].Max : (*pData);
                    _displayDataRecord.device_Math_Data[i].Min = \
                            _displayDataRecord.device_Math_Data[i].Min<(*pData) ? _displayDataRecord.device_Math_Data[i].Min : (*pData);
                    _displayDataRecord.device_Math_Data[i].status = *pStatus;

                    _eventDataRecord.device_Math_Data[i].Max = *pData;
                    _eventDataRecord.device_Math_Data[i].Min = *pData;
                    _eventDataRecord.device_Math_Data[i].status = *pStatus;
                }

                //COMM通道数据
                for (int i=0; i<MAIN_UNIT_COMM_CHAN_NUM_PRE; ++i){
                    pData = (float *)&pModbusRegMap->holdReg_MainUnit_Comm_Data_Float32[i<<1];
                    pStatus = &pModbusRegMap->holdReg_MainUnit_Comm_Status_Int16[i];

                    _displayDataRecord.device_Comm_Data[i].Max = \
                            _displayDataRecord.device_Comm_Data[i].Max>(*pData) ? _displayDataRecord.device_Comm_Data[i].Max : (*pData);
                    _displayDataRecord.device_Comm_Data[i].Min = \
                            _displayDataRecord.device_Comm_Data[i].Min<(*pData) ? _displayDataRecord.device_Comm_Data[i].Min : (*pData);
                    _displayDataRecord.device_Comm_Data[i].status = *pStatus;

                    _eventDataRecord.device_Comm_Data[i].Max = *pData;
                    _eventDataRecord.device_Comm_Data[i].Min = *pData;
                    _eventDataRecord.device_Comm_Data[i].status = *pStatus;
                }

            }
    }
}

/*
 * 功能：
 * 	  采样数据处理函数
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
void SyncProcess::dataSampleProcess()
{
    MODBUS_REGISTER_MAP *pModbusRegMap;
    float * pData;
    quint16 *pDataDO;
    qint16 *pStatus;

    DevShareMem.lock();
    pModbusRegMap = const_cast<MODBUS_REGISTER_MAP*>(static_cast<MODBUS_REGISTER_MAP const *>(DevShareMem.constData()));

    //输入寄存器--本机--IO通道（AI/DI）
    for (int i=0; i<MAIN_UNIT_MODULE_NUM; i++){
        for (int j=0; j<UNIT_CHAN_NUM_PRE; j++){
            pData = (float *)&pModbusRegMap->inputReg_MainUnit_IO_Data_Float32[i][j<<1];
            pStatus = &pModbusRegMap->inputReg_MainUnit_IO_Status_Int16[i][j];
            _manualDataRecord.mainUnit_AI_Data[i][j].Max = *pData;
            _manualDataRecord.mainUnit_AI_Data[i][j].Min = *pData;
            _manualDataRecord.mainUnit_AI_Data[i][j].status = *pStatus;

            _manualDataRecord.mainUnit_DI_Data[i][j].Max = *pData;
            _manualDataRecord.mainUnit_DI_Data[i][j].Min = *pData;
            _manualDataRecord.mainUnit_DI_Data[i][j].status = *pStatus;
        }
    }

     //输入寄存器--扩展单元--IO通道（AI/DI）
    for (int i=0; i<EXT_UNIT_NUM; ++i){
        for (int j=0; j<EXT_UNIT_MODULE_NUM; ++j){
            for (int k=0; k<UNIT_CHAN_NUM_PRE; ++k){
                switch (i) {
                case 0:
                case 1:
                case 2:
                case 3:
                case 4:
                case 5:
                    pData = (float *)&pModbusRegMap->inputReg_ExtUnit_IO_Data_Float32[i][j][k<<1];
                    pStatus = &pModbusRegMap->inputReg_ExtUnit_IO_Status_Int16[i][j][k];
                    break;
                }
                _manualDataRecord.extUnit_AI_Data[i][j][k].Max = *pData;
                _manualDataRecord.extUnit_AI_Data[i][j][k].Min = *pData;
                _manualDataRecord.extUnit_AI_Data[i][j][k].status = *pStatus;

                _manualDataRecord.extUnit_DI_Data[i][j][k].Max = *pData;
                _manualDataRecord.extUnit_DI_Data[i][j][k].Min = *pData;
                _manualDataRecord.extUnit_DI_Data[i][j][k].status = *pStatus;
            }
        }
    }

    //保持寄存器--本机--IO通道（DO）
    for (int i=0; i<MAIN_UNIT_MODULE_NUM; ++i) {
        for(int j=0; j<UNIT_CHAN_NUM_PRE; ++j){
            pDataDO = (quint16 *)&pModbusRegMap->holdReg_mainUnit_IO_Data_Int16[i][j];
            pStatus = &pModbusRegMap->holdReg_mainUnit_IO_Status_Int16[i][j];
            _manualDataRecord.mainUnit_DO_Data[i][j].Max = *pDataDO;
            _manualDataRecord.mainUnit_DO_Data[i][j].Min = *pDataDO;
            _manualDataRecord.mainUnit_DO_Data[i][j].status = *pStatus;
        }
    }

    //保持寄存器--扩展单元--IO通道（DO）
    for (int i=0; i<EXT_UNIT_NUM; ++i){
        for (int j=0; j<EXT_UNIT_MODULE_NUM; ++j){
            for (int k=0; k<UNIT_CHAN_NUM_PRE; ++k){
                switch (i) {
                case 0:
                case 1:
                case 2:
                case 3:
                case 4:
                case 5:
                    pDataDO = (quint16 *)&pModbusRegMap->holdReg_ExtUnit_IO_Data_Int16[i][j][k];
                    pStatus = &pModbusRegMap->holdReg_ExtUnit_IO_Status_Int16[i][j][k];
                    break;
                }

                _manualDataRecord.extUnit_DO_Data[i][j][k].Max = *pDataDO;
                _manualDataRecord.extUnit_DO_Data[i][j][k].Min = *pDataDO;
                _manualDataRecord.extUnit_DO_Data[i][j][k].status = *pStatus;
            }
        }
    }

    //MATH通道数据
    for (int i=0; i<MAIN_UNIT_MATH_CHAN_NUM_PRE; ++i){
        pData = (float *)&pModbusRegMap->inputReg_MainUnit_Math_Data_Float32[i<<1];
        pStatus = &pModbusRegMap->inputReg_MainUnit_Math_Status_Int16[i];
        _manualDataRecord.device_Math_Data[i].Max = *pData;
        _manualDataRecord.device_Math_Data[i].Min = *pData;
        _manualDataRecord.device_Math_Data[i].status = *pStatus;
    }

    //COMM通道数据
    for (int i=0; i<MAIN_UNIT_COMM_CHAN_NUM_PRE; ++i){
        pData = (float *)&pModbusRegMap->holdReg_MainUnit_Comm_Data_Float32[i<<1];
        pStatus = &pModbusRegMap->holdReg_MainUnit_Comm_Status_Int16[i];
        _manualDataRecord.device_Comm_Data[i].Max = *pData;
        _manualDataRecord.device_Comm_Data[i].Min = *pData;
        _manualDataRecord.device_Comm_Data[i].status = *pStatus;
    }
    DevShareMem.unlock();
}

void SyncProcess::sendReportDataSingle()
{
    for (int i=0; i<REPORT_CHAN_NUM; ++i) {
        if (DevConfigPtr->reportConfig.reportChannelType[i] != Channel_Type_Off) {

            if (0 == _reportDataRecord.dataSingle)
                _reportDataRecord.reportAve[i] = 0;
            else
                _reportDataRecord.reportAve[i] = _reportDataRecord.reportSum[i]/_reportDataRecord.dataSingle;

            switch (DevConfigPtr->reportConfig.reportSumUnit[i]) {
            case Report_Sum_Unit_S:
                _reportDataRecord.reportSum[i] *= _reportDataRecord.recordInterval/1000;
                break;
            case Report_Sum_Unit_Min:
                _reportDataRecord.reportSum[i] *= _reportDataRecord.recordInterval/60000;
                break;
            case Report_Sum_Unit_H:
                _reportDataRecord.reportSum[i] *= _reportDataRecord.recordInterval/3600000;
                break;
            case Report_Sum_Unit_Day:
                _reportDataRecord.reportSum[i] *= _reportDataRecord.recordInterval/86400000;
                break;
            default:
                break;
            }
        }
    }

    if (_reportDataRecord.dataCount!=0 || _reportDataRecord.dataSingle!=0) {
        /*发送时报*/
        ThreadDataRecord::instance()->newReportData(&_reportDataRecord);
    }
    /*单次清理*/
    _reportDataRecord.dataSingle = 0;
}

void SyncProcess::cpyBufferData()
{
    int i=0;
    s_mutex.lock();
    while(!s_dataBuffer.isEmpty()) {
        m_dataTemp[i++] = s_dataBuffer.dequeue();
    }
    s_mutex.unlock();
    m_dataBufferCount = i;
}

void SyncProcess::sendReportDataWhole()
{
    /*发送时报*/
    sendReportDataSingle();
    /*发送结束包*/
    _reportDataRecord.stopFlag = true;
    ThreadDataRecord::instance()->newReportData(&_reportDataRecord);
    /*数据清除*/
    for (int i=0; i<REPORT_CHAN_NUM; ++i) {
        _reportDataRecord.reportAve[i] = 0;
        _reportDataRecord.reportSum[i] = 0;
        _reportDataRecord.reportMin[i] = 0;
        _reportDataRecord.reportMax[i] = 0;
        _reportDataRecord.reportReal[i] = 0;
    }

    _reportDataRecord.dataSingle = 0;
    _reportDataRecord.dataCount = 0;
    _reportDataRecord.lastTime = -1;
    _reportDataRecord.stopFlag = false;
}

void SyncProcess::transAlarmStatus(quint32 chanCoder, quint8 alarmIndex)
{
    Q_ASSERT(alarmIndex<4);
    QVariant chanStatus = CfgChannel::instance()->getChannelStatus(chanCoder);
    QVariant alarmStatus = CfgChannel::instance()->getChannelAlarm(chanCoder);

    if (chanStatus.isValid() && alarmStatus.isValid()) {
        quint16 status = chanStatus.toUInt();
        quint16 alarm = alarmStatus.toUInt();

        if (DATA_ERROR_OFF == status) {
            return;
        } else {
            if (alarm & (0x1<<alarmIndex)) {
                EventAction::instance()->raiseAlarm(chanCoder, alarmIndex);
            } else {
                EventAction::instance()->releaseAlarm(chanCoder, alarmIndex);
            }
        }
    }
}

/*
 * 功能：
 * 	  遍历所有有效通道，实现报警状态的转换
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
void SyncProcess::chanAlarmStatusTrans()
{
    AI_CONFIG *pAIConfig;
    DI_CONFIG *pDIConfig;
    MATH_CONFIG *pMathConfig;
    COMM_CONFIG *pCommConfig;

    foreach (quint32 chanCoder, CfgChannel::instance()->getEffectiveChannel()) {
        switch (CHANNEL_TYPE(chanCoder)) {
        case Channel_Type_AI:
            pAIConfig = CfgChannel::instance()->getAIConfig(chanCoder);
            if (pAIConfig) {
                for (int i=0; i<4; ++i) {
                    if (pAIConfig->alarmConfig.alarmOnOff[i])
                        transAlarmStatus(chanCoder, i);
                }
            }
            break;
        case Channel_Type_DI:
            pDIConfig = CfgChannel::instance()->getDIConfig(chanCoder);
            if (pDIConfig) {
                for (int i=0; i<4; ++i) {
                    if (pDIConfig->alarmConfig.alarmOnOff[i])
                        transAlarmStatus(chanCoder, i);
                }
            }
            break;
        case Channel_Type_Comm:
            pCommConfig = CfgChannel::instance()->getCommConfig(chanCoder);
            if (pCommConfig) {
                for (int i=0; i<4; ++i) {
                    if (pCommConfig->alarmConfig.alarmOnOff[i])
                        transAlarmStatus(chanCoder, i);
                }
            }
            break;
        case Channel_Type_Math:
            pMathConfig = CfgChannel::instance()->getMathConfig(chanCoder);
            if (pMathConfig) {
                for (int i=0; i<4; ++i) {
                    if (pMathConfig->alarmConfig.alarmOnOff[i])
                        transAlarmStatus(chanCoder, i);
                }
            }
            break;
        default:
            return;
        }
    }
}

/*
 * 功能：
 * 	  各个通道报警输出动作执行
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
void SyncProcess::chanAlarmOutput()
{
    const QMap<quint32, QMap<quint32, quint8>> &mapSW = CfgChannel::instance()->getMapOfInterSW();
    const QMap<quint32, QMap<quint32, quint8>> &mapDO = CfgChannel::instance()->getMapOfRelayDO();
    bool result;

    //内部开关输出执行
    foreach (quint32 index, mapSW.keys()) {
        QMap<quint32, quint8> map = mapSW[index];   //报警输出绑定到内部开关（index）的报警map
        if (DevConfigPtr->systemConfig.interSwitchOperate[index] == InterSwitch_Operate_And) {  //报警与操作
            result = true;
            foreach (quint32 coder, map.keys()) {
                QVariant alarm = CfgChannel::instance()->getChannelAlarm(coder);
                if (alarm.isValid() && ((alarm.toUInt() & map[coder]) != map[coder])) {    //任意一个报警未发生，则SW输出为false
                    result = false;
                    break;
                }
            }
        } else {    //报警或操作
            result = false;
            foreach (quint32 coder, map.keys()) {
                QVariant alarm = CfgChannel::instance()->getChannelAlarm(coder);
                if (alarm.isValid() && (alarm.toUInt() & map[coder])) {    //任意一个报警发生，则SW输出为true
                    result = true;
                    break;
                }
            }
        }

        EventAction::instance()->setInterSW(index, result); // 内部开关设置
    }

    //继电器DO输出执行
    foreach (quint32 DOcoder, mapDO.keys()) {
        QMap<quint32, quint8> map = mapDO[DOcoder];   //报警输出绑定到通道DO（DOcoder）的报警map
        DO_CONFIG *pConfig = CfgChannel::instance()->getDOConfig(DOcoder);
        if (pConfig && pConfig->activeConfig.actionMode == 0)  {   //DO输出“或”操作
            result = false;
            foreach (quint32 coder, map.keys()) {
                QVariant alarm = CfgChannel::instance()->getChannelAlarm(coder);
                if (alarm.isValid() && (alarm.toUInt() & map[coder])) {    //任意一个报警发生，则DO输出为true
                    result = true;
                    break;
                }
            }

            if (pConfig->activeConfig.actionHold == 0) {    //保持执行
                if (result) {
                    EventAction::instance()->setRelayDO(DOcoder, result);
                } else {
                    SQL_Alarm_Status status;
                    qint32 index = 0;
                    //或操作，判断是否有对应通道未ACK，是则输出DO
                    foreach (quint32 coder, map.keys()) {
                        for (int i=0; i<4; ++i) {
                            if (map[coder] & (0x01<<i)) {
                                status = EventAction::instance()->getAlarmStatus(coder, i);
                                if (status == SQL_Alarm_Status_Hold_Occur_NoAck || \
                                    status == SQL_Alarm_Status_Hold_Release_NoAck) {
                                    index++;
                                    break;
                                }
                            }
                        }
                    }
                    if (index == 0) {
                        EventAction::instance()->setRelayDO(DOcoder, false);
                    } else {
                        EventAction::instance()->setRelayDO(DOcoder, true);
                    }
                }
            } else {    //非保持执行
                EventAction::instance()->setRelayDO(DOcoder, result);
            }

        } else if (pConfig && pConfig->activeConfig.actionMode == 1) {  //DO输出“与”操作
            result = true;
            foreach (quint32 coder, map.keys()) {
                QVariant alarm = CfgChannel::instance()->getChannelAlarm(coder);
                if (alarm.isValid() && ((alarm.toUInt() & map[coder]) != map[coder])) {    //任意一个报警未发生，则DO输出为false
                    result = false;
                    break;
                }
            }

            if (pConfig->activeConfig.actionHold == 0) {    //保持执行
                if (result) {
                    EventAction::instance()->setRelayDO(DOcoder, result);
                } else {
                    SQL_Alarm_Status status;
                    qint32 index = 0;
                    qint32 count = 0;
                    //与操作，所有的都未ACK，则DO输出
                    foreach (quint32 coder, map.keys()) {
                        for (int i=0; i<4; ++i) {
                            if (map[coder] & (0x01<<i)) {
                                count++;
                                status = EventAction::instance()->getAlarmStatus(coder, i);
                                if (status == SQL_Alarm_Status_Hold_Occur_NoAck || \
                                    status == SQL_Alarm_Status_Hold_Release_NoAck)
                                    index++;
                            }
                        }
                    }
                    if (index == count) {
                        EventAction::instance()->setRelayDO(DOcoder, true);
                    } else {
                        EventAction::instance()->setRelayDO(DOcoder, false);
                    }

                }
            } else {    //非保持执行
                EventAction::instance()->setRelayDO(DOcoder, result);
            }

        } else {    //DO输出“再故障再报警”
            foreach (quint32 coder, map.keys()) {
                QVariant alarm = CfgChannel::instance()->getChannelAlarm(coder);
                if (alarm.isValid() && (alarm.toUInt() & map[coder])) {    //任意一个报警发生，则DO输出
                    EventAction::instance()->setRelayDO(DOcoder, true);
                }
            }

        }
    }
}
/*
 * 功能：
 * 	  报表数据处理函数
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
void SyncProcess::dataReportProcess()
{
    quint32 chCoder;
    float data;
    int hour, weekday, day, msCurDay;
    quint64 singleTime, totalTime;  //单位s
    QTime setTime(DevConfigPtr->reportConfig.reportHourly, DevConfigPtr->reportConfig.reportMinute);

    if (0 == _reportDataRecord.dataCount) {
        _reportDataRecord.dataStartTime = QDateTime::currentMSecsSinceEpoch();
        _reportDataRecord.recordInterval = DevConfigPtr->measureInterval_ms;
        for (int i=0; i<REPORT_CHAN_NUM; ++i) {
            quint32 chanCoder = DevConfigPtr->reportConfig.reportChannelNum[i];
            if (Channel_Type_Off != DevConfigPtr->reportConfig.reportChannelType[i] &&
                CfgChannel::instance()->getEffectiveChannel().contains(chanCoder)) {
                QString unit = CfgChannel::instance()->getChannelUnit(chanCoder).toString();
                unit.truncate(6);
                memcpy(&_reportDataRecord.reportUnit[i][0], unit.constData(), unit.count());
            } else {
                memset(&_reportDataRecord.reportUnit[i][0], 0x0, 8);
            }
        }
    }

    switch (DevConfigPtr->reportConfig.reportType) {
    case Report_Type_Hourly_Daily:
        hour = QTime::currentTime().hour();
        if (_reportDataRecord.lastTime != hour) {
            _reportDataRecord.lastTime = hour;
            if (0 == _reportDataRecord.dataCount) {//无数据，则返回
                break;
            } else if(DevConfigPtr->reportConfig.reportHourly == hour) {  //每日指定时间创建至此一天的报表数据
                sendReportDataWhole();
            } else {    //时报
                sendReportDataSingle();
            }
        }
        break;
    case Report_Type_Daily_Weekly:
        weekday = QDate::currentDate().dayOfWeek();
        if (_reportDataRecord.lastTime != weekday) {
            _reportDataRecord.lastTime = weekday;
            if (0 == _reportDataRecord.dataCount) {//无数据，则返回
                break;
            } else if (DevConfigPtr->reportConfig.reportWeek == weekday) {  //每周指定时间创建至此一周的报表数据
                sendReportDataWhole();
            } else {    //日报
                sendReportDataSingle();
            }
        }
        break;
    case Report_Type_Daily_Monthly:
        day = QDate::currentDate().day();
        if (_reportDataRecord.lastTime != day) {
            _reportDataRecord.lastTime = day;
            if (0 == _reportDataRecord.dataCount) {//无数据，则返回
                break;
            } else if (DevConfigPtr->reportConfig.reportDay == day) {  //每月指定时间创建至此一月的报表数据
                sendReportDataWhole();
            } else {    //日报
                sendReportDataSingle();
            }
        }
        break;
    case Report_Type_Batch:
        singleTime = _reportDataRecord.dataSingle * _reportDataRecord.recordInterval / 1000;
        totalTime = _reportDataRecord.dataCount * _reportDataRecord.recordInterval / 1000;
        if (totalTime >= (200 * DevConfigPtr->reportConfig.reportPeriod * 60)) {    //200个数据点，则自动分割
            sendReportDataWhole();
        } else if (singleTime >= (DevConfigPtr->reportConfig.reportPeriod * 60)) {  //单次数据点发送
            sendReportDataSingle();
        }
        break;
    case Report_Type_Daily_Custom:  //相关配置：时、分、保存周期、文件创建周期
        msCurDay = QTime::currentTime().msecsSinceStartOfDay();
        if (setTime.msecsSinceStartOfDay() == msCurDay) {

            if (-1 != _reportDataRecord.lastTime) {
                sendReportDataWhole();
            }

            _reportDataRecord.lastTime = msCurDay;
            _reportDataRecord.dataStartTime = QDateTime::currentMSecsSinceEpoch();
            _reportDataRecord.recordInterval = DevConfigPtr->measureInterval_ms;
            _reportDataRecord.dataCount  = 0;
            _reportDataRecord.dataSingle = 0;

        } else if (-1 != _reportDataRecord.lastTime) {

            singleTime = _reportDataRecord.dataSingle * _reportDataRecord.recordInterval;
            totalTime = msCurDay - _reportDataRecord.lastTime;
            if (totalTime >= (DevConfigPtr->reportConfig.reportFileCreatePeriod * 3600000)) {    //文件创建周期
                sendReportDataWhole();
            } else if (singleTime >= (DevConfigPtr->reportConfig.reportPeriod * 60000)) {  //单次数据点发送
                sendReportDataSingle();
            }
        }
        break;
    default:
        return; //其它，直接返回
    }

     _reportDataRecord.dataCount++;
     _reportDataRecord.dataSingle++;

    for (int i=0; i<REPORT_CHAN_NUM; ++i) {
        if (DevConfigPtr->reportConfig.reportChannelType[i] != Channel_Type_Off) {
            chCoder = DevConfigPtr->reportConfig.reportChannelNum[i];
            if (CfgChannel::instance()->getEffectiveChannel().contains(chCoder)) {
                data = CfgChannel::instance()->getChannelData(chCoder).toFloat();
                _reportDataRecord.reportStatus[i] = CfgChannel::instance()->getChannelStatus(chCoder).toUInt();
            } else {
                data = 0;
                _reportDataRecord.reportStatus[i] = DATA_ERROR_INVALID;
            }
            if (1 == _reportDataRecord.dataCount) {
                _reportDataRecord.reportMin[i] = data;
                _reportDataRecord.reportMax[i] = data;
            } else {
                _reportDataRecord.reportMin[i] = (_reportDataRecord.reportMin[i] < data) ? _reportDataRecord.reportMin[i] : data;
                _reportDataRecord.reportMax[i] = (_reportDataRecord.reportMax[i] > data) ? _reportDataRecord.reportMax[i] : data;
            }
            _reportDataRecord.reportSum[i] += data;
            _reportDataRecord.reportReal[i] = data;
        }
    }
}
