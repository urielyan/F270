/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：EventAction.cpp
 * 概   要：事件动作类，主要根据当前的系统状态、各个通道值、报警等等，决定是否
 * 触发相应的事件，如果该事件有关联的动作，则执行相应的动作。
 *
 * 依赖：配置结构体 && Modbus寄存器表
 *
 * 当前版本：V1.0.0
 * 作   者：王 品
 * 完成日期：2015-8-27
 *
 * 修改版本：
 * 修改说明：
 * 修改作者：
 *
 ********************************************************************/
#include "eventaction.h"
#include "timer.h"
#include "application.h"
#include "qactionevent.h"

#include "../GlobalData/cfgchannel.h"
#include "../GlobalData/dataGlobalDef_ModbusReg.h"

#include "timer.h"
#include "../WorkWin/winmainframe.h"
#include "../WorkWin/wintrendgrouppanel.h"

#include "./Threads/threaddatacomm.h"
#include "./Threads/threaddataprocess.h"
#include "./Threads/threaddatarecord.h"
#include "./Threads/threaddatasync.h"

#include <QVariant>
#include <QDateTime>
#include <QSharedMemory>

extern CfgTrendGroup *p_gCfgTrendGroup;

EventAction *EventAction::instance()
{
    static EventAction data;
    return &data;
}

void EventAction::setInterFlag(int index, bool val)
{
    p_InterFlag->setBit(index, val);
}

quint8 EventAction::getInterFlag(int index)
{
    if (p_InterFlag->at(index))
        return 1;
    else
        return 0;
}

void EventAction::setInterSW(int index, bool val)
{
    MODBUS_REGISTER_MAP* data;

    p_InterSW_cur->setBit(index, val);

    //设置Modbus寄存器表中的内部开关值
    if(DevShareMem.isAttached()) {
        DevShareMem.lock();
        data = static_cast<MODBUS_REGISTER_MAP*>(DevShareMem.data());
        data->holdReg_Device_Switch_Data_Int16[index] = val ? 1 : 0;
        DevShareMem.unlock();
    }
}

void EventAction::setRelayDO(quint32 chanCoder, bool val)
{
    MODBUS_REGISTER_MAP* data;
    quint32 unit, module, number;

    //按CfgChannel:：m_map_DO中chanCoder的索引序列分配bit位
    if (CfgChannel::instance()->getExistChannelDO().values().contains(chanCoder)) {
        quint32 index = CfgChannel::instance()->getExistChannelDO().values().indexOf(chanCoder);
        p_RelayDO_cur->setBit(index, val);

        //设置Modbus寄存器表中DO通道输出值
        unit = CHANNEL_UNIT(chanCoder);
        module = CHANNEL_MODULE(chanCoder);
        number = CHANNEL_NUM(chanCoder) - 1;
        if(DevShareMem.isAttached()) {
            DevShareMem.lock();
            data = static_cast<MODBUS_REGISTER_MAP*>(DevShareMem.data());
            if (unit == CHANNEL_UNIN_MAIN) {
                data->holdReg_mainUnit_IO_Data_Int16[module][number] = val ? 1 : 0;
                data->holdReg_MainUnit_IO_Data_Int16_Linear[module][number] = val ? 1 : 0;
                data->coil_MainUnit_IO_Data_Bit[module][number] = val ? 1 : 0;
            } else {
                data->holdReg_ExtUnit_IO_Data_Int16[unit][module][number] = val ? 1 : 0;
            }
            DevShareMem.unlock();
        }

        //设置DO输出
        emit ThreadDataProcess::instance()->sigRelayDoOutput(chanCoder, val);

    } else {
        qDebug("Error: setRelayDO no chanCoder:0x%x\n", chanCoder);
        return;
    }
}

/*
 * 功能：
 * 	  遍历内部开关有效事件，检查事件是否触发，如触发，则执行
 * 对应的动作。
 */
void EventAction::genEvent_interSW()
{
    quint32 eventIndex;
    quint32 swIndex;

    //内部开关事件判定及动作执行
    for (quint8 i=0; i<m_eventInterSW.count(); ++i) {
        eventIndex = m_eventInterSW.at(i);
        swIndex = m_eventObj[eventIndex].number-1;
        switch (m_eventObj[eventIndex].opeMode) {
        case Operation_Mode_Type_RisingEdge:
            if (false == p_InterSW_pre->at(swIndex) && \
                true == p_InterSW_cur->at(swIndex)) {
                m_actionObj[eventIndex].trigType = Trigger_Type_Raise;
                execAction(m_actionObj[eventIndex]);
            }
            break;
        case Operation_Mode_Type_FallingEdge:
            if (true == p_InterSW_pre->at(swIndex) && \
                false == p_InterSW_cur->at(swIndex)) {
                m_actionObj[eventIndex].trigType = Trigger_Type_Falling;
                execAction(m_actionObj[eventIndex]);
            }
            break;
        case Operation_Mode_Type_Both:
            if (false == p_InterSW_pre->at(swIndex) && true == p_InterSW_cur->at(swIndex)) {
                m_actionObj[eventIndex].trigType = Trigger_Type_Raise;
                execAction(m_actionObj[eventIndex]);
            } else if (true == p_InterSW_pre->at(swIndex) && false == p_InterSW_cur->at(swIndex)) {
                m_actionObj[eventIndex].trigType = Trigger_Type_Falling;
                execAction(m_actionObj[eventIndex]);
            }
            break;
        default:
            break;
        }
    }

    *p_InterSW_pre = *p_InterSW_cur;
}

/*
 * 功能：
 * 	  遍历远程控制输入有效事件，检查事件是否触发，如触发，则执行
 * 对应的动作。
 */
void EventAction::genEvent_remoteDI()
{
    quint32 eventIndex;
    quint32 chanCoder;    //远程控制输入DI通道编码
    bool dataIn;

    for (int i=0; i<m_eventRemoteDI.count(); ++i) {
        eventIndex = m_eventRemoteDI.at(i);
        chanCoder = m_eventObj[eventIndex].number;
        dataIn = CfgChannel::instance()->getChannelData(chanCoder).toBool();
        quint16 number = CHANNEL_NUM(chanCoder) - 1;              //通道号
        switch (m_eventObj[eventIndex].opeMode) {
        case Operation_Mode_Type_RisingEdge:
            if (false == p_RemoteDI_pre->at(number) && true == dataIn) {
                m_actionObj[eventIndex].trigType = Trigger_Type_Raise;
                execAction(m_actionObj[eventIndex]);
            }
            break;
        case Operation_Mode_Type_FallingEdge:
            if (true == p_RemoteDI_pre->at(number) && false == dataIn) {
                m_actionObj[eventIndex].trigType = Trigger_Type_Falling;
                execAction(m_actionObj[eventIndex]);
            }
            break;
        case Operation_Mode_Type_Both:
            if(false == p_RemoteDI_pre->at(number) && true == dataIn) {
                m_actionObj[eventIndex].trigType = Trigger_Type_Raise;
                execAction(m_actionObj[eventIndex]);
            } else if (true == p_RemoteDI_pre->at(number) && false == dataIn) {
                m_actionObj[eventIndex].trigType = Trigger_Type_Falling;
                execAction(m_actionObj[eventIndex]);
            }
            break;
        default:
            break;
        }
    }

    //TODO：Modbus寄存器表中远程控制输入DI值 ---> p_RemoteDI_pre
}

void EventAction::genEvent_relayDO()
{
    quint32 eventIndex;
    quint32 chanCoder;    //继电器DO通道编码
    quint32 index;

    //继电器事件判定及动作执行
    for (int i=0; i<m_eventRelayDO.count(); ++i) {
        eventIndex = m_eventRelayDO.at(i);
        chanCoder = m_eventObj[eventIndex].number;
        index = CfgChannel::instance()->getExistChannelDO().values().indexOf(chanCoder);
        switch (m_eventObj[eventIndex].opeMode) {
        case Operation_Mode_Type_RisingEdge:
            if (false == p_RelayDO_pre->at(index) && \
                true == p_RelayDO_cur->at(index)) {
                m_actionObj[eventIndex].trigType = Trigger_Type_Raise;
                execAction(m_actionObj[eventIndex]);
            }
            break;
        case Operation_Mode_Type_FallingEdge:
            if (true == p_RelayDO_pre->at(index) && \
                false == p_RelayDO_cur->at(index)) {
                m_actionObj[eventIndex].trigType = Trigger_Type_Falling;
                execAction(m_actionObj[eventIndex]);
            }
            break;
        case Operation_Mode_Type_Both:
            if (false == p_RelayDO_pre->at(index) && true == p_RelayDO_cur->at(index)) {
                m_actionObj[eventIndex].trigType = Trigger_Type_Raise;
                execAction(m_actionObj[eventIndex]);
            } else if (true == p_RelayDO_pre->at(index) && false == p_RelayDO_cur->at(index)) {
                m_actionObj[eventIndex].trigType = Trigger_Type_Falling;
                execAction(m_actionObj[eventIndex]);
            }
            break;
        default:
            break;
        }
    }

    if (NULL != p_RelayDO_pre && NULL != p_RelayDO_cur)
        *p_RelayDO_pre = *p_RelayDO_cur;
}

void EventAction::genEvent_alarmIO()
{
    quint32 eventIndex;    //事件索引
    quint32 chanCoder;    //IO通道(AI/DI)编码
    quint8   alarmNum;     //报警电平
    quint32 index;              //BIT索引
    bool alarm;


    //IO报警事件判定及动作执行, AI/DI
    for (int i=0; i<m_eventAlarmIO.count(); ++i) {
        eventIndex = m_eventAlarmIO.at(i);
        chanCoder = m_eventObj[eventIndex].number;
        alarmNum = m_eventObj[eventIndex].alarmNumber-1;
        if (CfgChannel::instance()->getExistChannelAI().values().contains(chanCoder)) {
            index = CfgChannel::instance()->getExistChannelAI().values().indexOf(chanCoder);
        } else if (CfgChannel::instance()->getExistChannelDI().values().contains(chanCoder)) {
            index = CfgChannel::instance()->getExistChannelAI().values().count() + \
                         CfgChannel::instance()->getExistChannelDI().values().indexOf(chanCoder);
        } else {
            return;
        }

        index = index<<2;   //每个通道4个报警(电平1~4)
        index += alarmNum;

        alarm = CfgChannel::instance()->getChannelAlarm(chanCoder).toUInt() & (1<<alarmNum);

        switch (m_eventObj[eventIndex].opeMode) {
        case Operation_Mode_Type_RisingEdge:
            if (false == p_AlarmIO_pre->at(index) && true == alarm) {
                m_actionObj[eventIndex].trigType = Trigger_Type_Raise;
                execAction(m_actionObj[eventIndex]);
            }
            break;
        case Operation_Mode_Type_FallingEdge:
            if (true == p_AlarmIO_pre->at(index) && false == alarm) {
                m_actionObj[eventIndex].trigType = Trigger_Type_Falling;
                execAction(m_actionObj[eventIndex]);
            }
            break;
        case Operation_Mode_Type_Both:
            if (false == p_AlarmIO_pre->at(index) && true == alarm) {
                m_actionObj[eventIndex].trigType = Trigger_Type_Raise;
                execAction(m_actionObj[eventIndex]);
            } else if  (true == p_AlarmIO_pre->at(index) && false == alarm) {
                m_actionObj[eventIndex].trigType = Trigger_Type_Falling;
                execAction(m_actionObj[eventIndex]);
            }
            break;
        default:
            break;
        }
    }

    //IO通道报警状态-->p_AlarmIO_pre
    if (p_AlarmIO_pre != NULL) {
        delete p_AlarmIO_pre;
    }
    resetChanAlarmIO();
}

void EventAction::genEvent_alarmMath()
{
    quint32 eventIndex;    //事件索引
    quint32 chanCoder;    //Math通道编码
    quint8   alarmNum;     //报警电平
    quint32 index;              //BIT索引
    bool alarm;

    //Math通道报警事件判定及动作执行
    for (int i=0; i<m_eventAlarmMath.count(); ++i) {
        eventIndex = m_eventAlarmMath.at(i);
        chanCoder = m_eventObj[eventIndex].number;
        alarmNum = m_eventObj[eventIndex].alarmNumber-1;
        if (CfgChannel::instance()->getExistChannelMath().values().contains(chanCoder)) {
            index = CfgChannel::instance()->getExistChannelMath().values().indexOf(chanCoder);
        } else {
            return;
        }

        index = index<<2;   //每个通道4个报警(电平1~4)
        index += alarmNum;

        alarm = CfgChannel::instance()->getChannelAlarm(chanCoder).toUInt() & (1<<alarmNum);

        switch (m_eventObj[eventIndex].opeMode) {
        case Operation_Mode_Type_RisingEdge:
            if (false == p_AlarmMath_pre->at(index) && true == alarm) {
                m_actionObj[eventIndex].trigType = Trigger_Type_Raise;
                execAction(m_actionObj[eventIndex]);
            }
            break;
        case Operation_Mode_Type_FallingEdge:
            if (true == p_AlarmMath_pre->at(index) && false == alarm) {
                m_actionObj[eventIndex].trigType = Trigger_Type_Falling;
                execAction(m_actionObj[eventIndex]);
            }
            break;
        case Operation_Mode_Type_Both:
            if (false == p_AlarmMath_pre->at(index) && true == alarm) {
                m_actionObj[eventIndex].trigType = Trigger_Type_Raise;
                execAction(m_actionObj[eventIndex]);
            } else if (true == p_AlarmMath_pre->at(index) && false == alarm) {
                m_actionObj[eventIndex].trigType = Trigger_Type_Falling;
                execAction(m_actionObj[eventIndex]);
            }
            break;
        default:
            break;
        }
    }

    //Math通道报警状态-->p_AlarmMath_pre
    if (p_AlarmMath_pre != NULL) {
        delete p_AlarmMath_pre;
    }
    resetChanAlarmMath();
}

void EventAction::genEvent_alarmComm()
{
    quint32 eventIndex;    //事件索引
    quint32 chanCoder;    //Comm通道编码
    quint8   alarmNum;     //报警电平
    quint32 index;              //BIT索引
    bool alarm;

    //Comm通道报警事件判定及动作执行
    for (int i=0; i<m_eventAlarmComm.count(); ++i) {
        eventIndex = m_eventAlarmComm.at(i);
        chanCoder = m_eventObj[eventIndex].number;
        alarmNum = m_eventObj[eventIndex].alarmNumber-1;

        if (CfgChannel::instance()->getExistChannelComm().values().contains(chanCoder)) {
            index = CfgChannel::instance()->getExistChannelComm().values().indexOf(chanCoder);
        } else {
            return;
        }

        index = index<<2;   //每个通道4个报警(电平1~4)
        index += alarmNum;

        alarm = CfgChannel::instance()->getChannelAlarm(chanCoder).toUInt() & (1<<alarmNum);

        switch (m_eventObj[eventIndex].opeMode) {
        case Operation_Mode_Type_RisingEdge:
            if (false == p_AlarmComm_pre->at(index) && true == alarm) {
                m_actionObj[eventIndex].trigType = Trigger_Type_Raise;
                execAction(m_actionObj[eventIndex]);
            }
            break;
        case Operation_Mode_Type_FallingEdge:
            if (true == p_AlarmComm_pre->at(index) && false == alarm) {
                m_actionObj[eventIndex].trigType = Trigger_Type_Falling;
                execAction(m_actionObj[eventIndex]);
            }
            break;
        case Operation_Mode_Type_Both:
            if (false == p_AlarmComm_pre->at(index) && true == alarm) {
                m_actionObj[eventIndex].trigType = Trigger_Type_Raise;
                execAction(m_actionObj[eventIndex]);
            } else if  (true == p_AlarmComm_pre->at(index) && false == alarm) {
                m_actionObj[eventIndex].trigType = Trigger_Type_Falling;
                execAction(m_actionObj[eventIndex]);
            }
            break;
        default:
            break;
        }
    }

    //Comm通道报警状态-->p_AlarmComm_pre
    if (p_AlarmComm_pre != NULL) {
        delete p_AlarmComm_pre;
    }

    resetChanAlarmComm();
}

void EventAction::genEvent_alarmAll()
{
    quint32 eventIndex;    //事件索引
    quint32 chanCoder;     //通道编码
    bool alarmAll = false;

    //所有报警事件判定及动作执行
    for (int i=0; i<m_eventAlarmAll.count(); ++i) {
        eventIndex = m_eventAlarmAll.at(i);
        for (int j=0; i<CfgChannel::instance()->getEffectiveChannel().count(); ++j) {
            chanCoder = CfgChannel::instance()->getEffectiveChannel().at(i);
            alarmAll = CfgChannel::instance()->getChannelAlarm(chanCoder).toBool();
            if (alarmAll)   break;
        }

        switch (m_eventObj[eventIndex].opeMode) {
        case Operation_Mode_Type_RisingEdge:
            if (false == m_allAlarm_pre && true == alarmAll) {
                m_actionObj[eventIndex].trigType = Trigger_Type_Raise;
                execAction(m_actionObj[eventIndex]);
            }
            break;
        case Operation_Mode_Type_FallingEdge:
            if (true == m_allAlarm_pre && false == alarmAll) {
                m_actionObj[eventIndex].trigType = Trigger_Type_Falling;
                execAction(m_actionObj[eventIndex]);
            }
            break;
        case Operation_Mode_Type_Both:
            if (false == m_allAlarm_pre && true == alarmAll) {
                m_actionObj[eventIndex].trigType = Trigger_Type_Raise;
                execAction(m_actionObj[eventIndex]);
             } else if (true == m_allAlarm_pre && false == alarmAll) {
                m_actionObj[eventIndex].trigType = Trigger_Type_Falling;
                execAction(m_actionObj[eventIndex]);
            }
            break;
        default:
            break;
        }
    }
}

SQL_Alarm_Status EventAction::getAlarmStatus(quint32 chanCoder, quint8 levelNum)
{
    quint32 index;  //通道在报警状态中的索引

    if (false == getIndexofAlarmStatus(chanCoder, &index)) {
        qDebug("Error:No such channel coder: 0x%x....\n", chanCoder);
        return SQL_Alarm_Status_NonHold_Release;
    }

    Q_ASSERT(levelNum<4);
    index = index<<2;
    index += levelNum;

    return m_alarmStatus.at(index);
}

EventAction::EventAction(QObject *parent) :
    QObject(parent)
{
    connect(Timer::instance(), SIGNAL(sigTimerTrigger(int)), this, SLOT(slotSysTimerHandle(int)));
    connect(Timer::instance(), SIGNAL(sigTimerMatchTrigger(int)), this, SLOT(slotSysMatchTimerHandle(int)));

    p_InterFlag = NULL;
    p_InterSW_cur = p_InterSW_pre =NULL;
    p_RemoteDI_pre = NULL;
    p_RelayDO_cur = p_RelayDO_pre = NULL;
    p_AlarmIO_pre = NULL;
    p_AlarmMath_pre = NULL;
    p_AlarmComm_pre = NULL;
    m_allAlarm_pre = false;

    slotEventConfigChanged();
}

void EventAction::resetBitArray()
{
    if (p_InterFlag != NULL)  delete p_InterFlag;
    if (p_InterSW_cur != NULL)  delete p_InterSW_cur;
    if (p_InterSW_pre != NULL)  delete p_InterSW_pre;
    if (p_RemoteDI_pre != NULL)  delete p_RemoteDI_pre;
    if (p_RelayDO_cur != NULL)  delete p_RelayDO_cur;
    if (p_RelayDO_pre != NULL)  delete p_RelayDO_pre;
    if (p_AlarmIO_pre != NULL)  delete p_AlarmIO_pre;
    if (p_AlarmMath_pre != NULL)  delete p_AlarmMath_pre;
    if (p_AlarmComm_pre != NULL)  delete p_AlarmComm_pre;

    //内部标志F01~F20
    p_InterFlag = new QBitArray(INTER_FLAG_COUNT);

    //内部开关
    resetInterSW();

    //远程控制输入，TODO，如果存在远程控制输入DI，从Modbus寄存器
    //表中获取当前DI输入值
    p_RemoteDI_pre = new QBitArray(UNIT_CHAN_NUM_PRE);

    //DO通道
    resetChanDO();

    //所有通道报警状态
    quint32 channelCount = CfgChannel::instance()->getExistChannelAI().count();
    channelCount += CfgChannel::instance()->getExistChannelDI().count();
    channelCount += CfgChannel::instance()->getExistChannelMath().count();
    channelCount += CfgChannel::instance()->getExistChannelComm().count();

    m_alarmStatus.resize(channelCount<<2);
    if (Alarm_Indicator_Hold == DevConfigPtr->systemConfig.alramIndicator) {
        m_alarmStatus.fill(SQL_Alarm_Status_Hold_Release_Ack);
    } else {
        m_alarmStatus.fill(SQL_Alarm_Status_NonHold_Release);
    }

    //IO通道（AI/DI）报警
    resetChanAlarmIO();

    //Math通道报警
    resetChanAlarmMath();

    //Comm通道报警
    resetChanAlarmComm();

    //所有报警状态设置-->m_allAlarm_pre
    m_allAlarm_pre = false;
}

void EventAction::resetInterSW()
{
    MODBUS_REGISTER_MAP *data;

    //内部开关
    p_InterSW_cur = new QBitArray(MAIN_UNIT_INTERNAL_SWITCH);
    p_InterSW_pre = new QBitArray(MAIN_UNIT_INTERNAL_SWITCH);
    //从Modbus寄存器表中获取当前内部开关值
    if(DevShareMem.isAttached()) {
        DevShareMem.lock();
        data = static_cast<MODBUS_REGISTER_MAP*>(DevShareMem.data());
        for (int i=0; i<MAIN_UNIT_INTERNAL_SWITCH; ++i) {
            if (data->holdReg_Device_Switch_Data_Int16[i] != 0) {
                p_InterSW_cur->setBit(i);
                p_InterSW_pre->setBit(i);
            } else {
                p_InterSW_cur->clearBit(i);
                p_InterSW_pre->clearBit(i);
            }
        }
        DevShareMem.unlock();
    }
}

void EventAction::resetChanDO()
{
    MODBUS_REGISTER_MAP *data;
    quint16 uninNum, moduleIndex, number, index=0;
    qint16 *pDataDO;

    quint32 countDO = CfgChannel::instance()->getExistChannelDO().count();
    if (countDO>0) {
        p_RelayDO_cur = new QBitArray(countDO);
        p_RelayDO_pre = new QBitArray(countDO);

        //DO初始化
        foreach (quint32 chanCoder, CfgChannel::instance()->getExistChannelDO().values()) {
            uninNum = CHANNEL_UNIT(chanCoder);           //单元号
            moduleIndex = CHANNEL_MODULE(chanCoder);  //卡槽号
            number = CHANNEL_NUM(chanCoder) - 1;        //通道号

            //从Modbus寄存器表中获取当前内部DO
            if(DevShareMem.isAttached()) {
                DevShareMem.lock();
                data = static_cast<MODBUS_REGISTER_MAP*>(DevShareMem.data());
                if (uninNum == CHANNEL_UNIN_MAIN) {
                    pDataDO = &data->holdReg_mainUnit_IO_Data_Int16[moduleIndex][number];
                } else {
                    pDataDO = &data->holdReg_ExtUnit_IO_Data_Int16[uninNum][moduleIndex][number];
                }
                if (*pDataDO != 0) {
                    p_RelayDO_cur->setBit(index);
                    p_RelayDO_pre->setBit(index);
                } else {
                    p_RelayDO_cur->clearBit(index);
                    p_RelayDO_pre->clearBit(index);
                }
                DevShareMem.unlock();
            }

            index++;
        }
    }
}

void EventAction::resetChanAlarmIO()
{
    MODBUS_REGISTER_MAP *data;
    qint16 *pStatus;
    quint16 uninNum, moduleIndex, number, index=0;
    quint32 countAI = CfgChannel::instance()->getExistChannelAI().count();
    quint32 countDI = CfgChannel::instance()->getExistChannelDI().count();

    if ((countAI+countDI) > 0) {
        p_AlarmIO_pre = new QBitArray((countAI+countDI)<<2);

        //IO Alarm status 初始化
        if (countAI != 0) {
            foreach (quint32 chanCoder, CfgChannel::instance()->getExistChannelAI().values()) {
                uninNum = CHANNEL_UNIT(chanCoder);           //单元号
                moduleIndex = CHANNEL_MODULE(chanCoder);  //卡槽号
                number = CHANNEL_NUM(chanCoder) - 1;        //通道号

                //从Modbus寄存器表中获取当前AI报警
                if(DevShareMem.isAttached()) {
                    DevShareMem.lock();
                    data = static_cast<MODBUS_REGISTER_MAP*>(DevShareMem.data());
                    if (uninNum == CHANNEL_UNIN_MAIN) {
                        pStatus = &data->inputReg_MainUnit_IO_Status_Int16[moduleIndex][number];
                    } else {
                        pStatus = &data->inputReg_ExtUnit_IO_Status_Int16[uninNum][moduleIndex][number];
                    }
                    for (int k=0; k<4; ++k) {
                        if (*pStatus & (0x01<<k)) {
                            p_AlarmIO_pre->setBit(index);
                        } else {
                            p_AlarmIO_pre->clearBit(index);
                        }
                        index++;
                    }
                    DevShareMem.unlock();
                } else {
                    index += 4;
                }
            }
        }

        if (countDI != 0) {
            foreach (quint32 chanCoder, CfgChannel::instance()->getExistChannelDI().values()) {
                uninNum = CHANNEL_UNIT(chanCoder);           //单元号
                moduleIndex = CHANNEL_MODULE(chanCoder);  //卡槽号
                number = CHANNEL_NUM(chanCoder) - 1;        //通道号

                //从Modbus寄存器表中获取当前AI报警
                if(DevShareMem.isAttached()) {
                    DevShareMem.lock();
                    data = static_cast<MODBUS_REGISTER_MAP*>(DevShareMem.data());
                    if (uninNum == CHANNEL_UNIN_MAIN) {
                        pStatus = &data->inputReg_MainUnit_IO_Status_Int16[moduleIndex][number];
                    } else {
                        pStatus = &data->inputReg_ExtUnit_IO_Status_Int16[uninNum][moduleIndex][number];
                    }
                    for (int k=0; k<4; ++k) {
                        if (*pStatus & (0x01<<k)) {
                            p_AlarmIO_pre->setBit(index);
                        } else {
                            p_AlarmIO_pre->clearBit(index);
                        }
                        index++;
                    }
                    DevShareMem.unlock();
                } else {
                    index += 4;
                }
            }
        }
    }
}

void EventAction::resetChanAlarmMath()
{
    MODBUS_REGISTER_MAP *data;
    qint16 *pStatus;
    quint16 number, index=0;
    quint32 countMath = CfgChannel::instance()->getExistChannelMath().count();
    if (countMath>0) {
        p_AlarmMath_pre = new QBitArray(countMath<<2);

        foreach (quint32 chanCoder, CfgChannel::instance()->getExistChannelMath().values()) {
//            uninNum = CHANNEL_UNIT(chanCoder);           //单元号
//            moduleIndex = CHANNEL_MODULE(chanCoder);  //卡槽号
            number = CHANNEL_NUM(chanCoder) - 1;        //通道号

            //从Modbus寄存器表中获取当前Math报警
            if(DevShareMem.isAttached()) {
                DevShareMem.lock();
                data = static_cast<MODBUS_REGISTER_MAP*>(DevShareMem.data());
                pStatus = &data->inputReg_MainUnit_Math_Status_Int16[number];

                for (int k=0; k<4; ++k) {
                    if (*pStatus & (0x01<<k)) {
                        p_AlarmMath_pre->setBit(index);
                    } else {
                        p_AlarmMath_pre->clearBit(index);
                    }
                    index++;
                }
                DevShareMem.unlock();
            } else {
                index += 4;
            }
        }
    }
}

void EventAction::resetChanAlarmComm()
{
    MODBUS_REGISTER_MAP *data;
    qint16 *pStatus;
    quint16 number, index=0;
    quint32 countComm = CfgChannel::instance()->getExistChannelComm().count();
    if (countComm>0) {
        p_AlarmComm_pre = new QBitArray(countComm<<2);

        foreach (quint32 chanCoder, CfgChannel::instance()->getExistChannelComm().values()) {
//            uninNum = CHANNEL_UNIT(chanCoder);           //单元号
//            moduleIndex = CHANNEL_MODULE(chanCoder);  //卡槽号
            number = CHANNEL_NUM(chanCoder) - 1;        //通道号

            //从Modbus寄存器表中获取当前Math报警
            if(DevShareMem.isAttached()) {
                DevShareMem.lock();
                data = static_cast<MODBUS_REGISTER_MAP*>(DevShareMem.data());
                pStatus = &data->holdReg_MainUnit_Comm_Status_Int16[number];

                for (int k=0; k<4; ++k) {
                    if (*pStatus & (0x01<<k)) {
                        p_AlarmComm_pre->setBit(index);
                    } else {
                        p_AlarmComm_pre->clearBit(index);
                    }
                    index++;
                }
                DevShareMem.unlock();
            } else {
                index += 4;
            }
        }
    }
}

bool EventAction::getIndexofAlarmStatus(quint32 chanCoder, quint32 *index)
{
    quint32 temp = 0;
    if (!CfgChannel::instance()->getExistChannelAI().isEmpty()) {
         if (CfgChannel::instance()->getExistChannelAI().values().contains(chanCoder)) {
            *index = CfgChannel::instance()->getExistChannelAI().values().indexOf(chanCoder);
             return true;
         } else {
            temp += CfgChannel::instance()->getExistChannelAI().values().count();
         }
    }

    if (!CfgChannel::instance()->getExistChannelDI().isEmpty()) {
        if (CfgChannel::instance()->getExistChannelDI().values().contains(chanCoder)) {
            *index = temp + CfgChannel::instance()->getExistChannelDI().values().indexOf(chanCoder);
            return true;
        } else {
            temp += CfgChannel::instance()->getExistChannelDI().values().count();
        }
    }

    if (!CfgChannel::instance()->getExistChannelMath().isEmpty()) {
        if (CfgChannel::instance()->getExistChannelMath().values().contains(chanCoder)) {
            *index = temp + CfgChannel::instance()->getExistChannelMath().values().indexOf(chanCoder);
            return true;
        } else {
            temp += CfgChannel::instance()->getExistChannelMath().values().count();
        }
    }

    if (!CfgChannel::instance()->getExistChannelComm().isEmpty()) {
        if (CfgChannel::instance()->getExistChannelComm().values().contains(chanCoder)) {
            *index = temp + CfgChannel::instance()->getExistChannelComm().values().indexOf(chanCoder);
            return true;
        } else {
            temp += CfgChannel::instance()->getExistChannelComm().values().count();
        }
    }

    return false;
}

void EventAction::initAlarmData(quint32 chanCoder, quint32 level, SQL_Alarm_Status status, ALARM_DATA *pData)
{
    ::memset(pData, 0x00, sizeof(ALARM_DATA));

    pData->alarmStatus = status;

    if (status == SQL_Alarm_Status_NonHold_AllOff ||\
        status == SQL_Alarm_Status_Hold_Ack_All)
        return;

    std::string chanTag = CfgChannel::instance()->getChannelTag(chanCoder).toString().toStdString();
    ::memcpy(pData->alarmTagChar, chanTag.data(), qMin((int)chanTag.size(), 40));
    pData->alarmChanNum = chanCoder;
    pData->alarmLevelNo = level+1;
    switch (level) {
    case 0:
        pData->alarmType = (Alarm_Type)(CfgChannel::instance()->getChannelAlarm1Type(chanCoder).toUInt());
        break;
    case 1:
        pData->alarmType = (Alarm_Type)(CfgChannel::instance()->getChannelAlarm2Type(chanCoder).toUInt());
        break;
    case 2:
        pData->alarmType = (Alarm_Type)(CfgChannel::instance()->getChannelAlarm3Type(chanCoder).toUInt());
        break;
    case 3:
        pData->alarmType = (Alarm_Type)(CfgChannel::instance()->getChannelAlarm4Type(chanCoder).toUInt());
        break;
    default:
        break;
    }
    pData->alarmTime = QDateTime::currentMSecsSinceEpoch();
}

/**
 * @brief EventAction::ackAlarmAll
 *  解除所有报警
 */
void EventAction::ackAlarmAll()
{
    ALARM_DATA alarmData;

    //报警保持状态ACK响应
    if (Alarm_Indicator_Hold == DevConfigPtr->systemConfig.alramIndicator) {
        for (int i=0; i<m_alarmStatus.size(); ++i) {
            switch (m_alarmStatus.at(i)) {
            case SQL_Alarm_Status_Hold_Occur_NoAck: //报警发生，且未解除（Non ACK）
                m_alarmStatus.replace(i, SQL_Alarm_Status_Hold_Occur_Ack);
                break;
            case SQL_Alarm_Status_Hold_Occur_Ack:       //报警发生，且已解除（ACK）
                break;
            case SQL_Alarm_Status_Hold_Release_NoAck:   //报警释放，且未解除（Non ACK）
                m_alarmStatus.replace(i, SQL_Alarm_Status_Hold_Release_Ack);
                break;
            case SQL_Alarm_Status_Hold_Release_Ack:     //报警释放，且已解除（ACK）
                break;
            default:
                m_alarmStatus.replace(i, SQL_Alarm_Status_Hold_Release_Ack);
                break;
            }
        }

        //ACK ALL报警消息
        initAlarmData(0, 0, SQL_Alarm_Status_Hold_Ack_All, &alarmData);
    } else {
        //ACK ALL报警消息
        initAlarmData(0, 0, SQL_Alarm_Status_NonHold_AllOff, &alarmData);
    }

    ThreadDataRecord::instance()->newAlarmData(&alarmData);
}

/**
 * @brief EventAction::ackAlarmSingle
 *  解除单个报警
 * @param chanCoder：通道编码
 * @param levelNum：报警电平号1-4
 */
void EventAction::ackAlarmSingle(quint32 chanCoder, quint8 levelNum)
{
    quint32 index;  //通道在报警状态中的索引
    ALARM_DATA alarmData;

    if (false == getIndexofAlarmStatus(chanCoder, &index))
        return;

    Q_ASSERT(levelNum<4);
    index = index<<2;
    index += levelNum;

    //报警保持状态才对ACK响应
    if (Alarm_Indicator_Hold == DevConfigPtr->systemConfig.alramIndicator) {
        switch (m_alarmStatus.at(index)) {
        case SQL_Alarm_Status_Hold_Occur_NoAck: //报警发生，且未解除（Non ACK）
            m_alarmStatus.replace(index, SQL_Alarm_Status_Hold_Occur_Ack);
            break;
        case SQL_Alarm_Status_Hold_Occur_Ack:       //报警发生，且已解除（ACK）
            break;
        case SQL_Alarm_Status_Hold_Release_NoAck:   //报警释放，且未解除（Non ACK）
            m_alarmStatus.replace(index, SQL_Alarm_Status_Hold_Release_Ack);
            break;
        case SQL_Alarm_Status_Hold_Release_Ack:     //报警释放，且已解除（ACK）
            break;
        default:
            m_alarmStatus.replace(index, SQL_Alarm_Status_Hold_Release_Ack);
            break;
        }

        initAlarmData(chanCoder, levelNum, SQL_Alarm_Status_Hold_Ack_Single, &alarmData);
        ThreadDataRecord::instance()->newAlarmData(&alarmData);
    }
}

/**
 * @brief EventAction::raiseAlarm
 *  报警发生处理
 * @param chanCoder：通道编码
 * @param levelNum：报警电平号1-4[0-3]
 */
void EventAction::raiseAlarm(quint32 chanCoder, quint8 levelNum)
{
    quint32 index;  //通道在报警状态中的索引
    ALARM_DATA alarmData;

    if (false == getIndexofAlarmStatus(chanCoder, &index))
        return;

    Q_ASSERT(levelNum<4);
    index = index<<2;
    index += levelNum;

//    qDebug("raiseAlarm coder:0x%x, level:%d, status:%d", chanCoder, levelNum, m_alarmStatus.at(index));
    //报警保持
    if (Alarm_Indicator_Hold == DevConfigPtr->systemConfig.alramIndicator) {
        switch (m_alarmStatus.at(index)) {
        case SQL_Alarm_Status_Hold_Occur_NoAck: //报警发生，且未解除（Non ACK）
            break;
        case SQL_Alarm_Status_Hold_Occur_Ack:       //报警发生，且已解除（ACK）
            break;
        case SQL_Alarm_Status_Hold_Release_NoAck:   //报警释放，且未解除（Non ACK）
            qDebug("报警发生：之前 Release + NoAck,coder:0x%x, lev:%d\n", chanCoder, levelNum);
            m_alarmStatus.replace(index, SQL_Alarm_Status_Hold_Occur_NoAck);
            initAlarmData(chanCoder, levelNum, SQL_Alarm_Status_Hold_Occur_NoAck, &alarmData);
            ThreadDataRecord::instance()->newAlarmData(&alarmData);
            break;
        case SQL_Alarm_Status_Hold_Release_Ack:     //报警释放，且已解除（ACK）
            qDebug("报警发生：之前 Release + Ack,coder:0x%x, lev:%d\n", chanCoder, levelNum);
            m_alarmStatus.replace(index, SQL_Alarm_Status_Hold_Occur_NoAck);
            initAlarmData(chanCoder, levelNum, SQL_Alarm_Status_Hold_Occur_NoAck, &alarmData);
            ThreadDataRecord::instance()->newAlarmData(&alarmData);
            break;
        default:
            //should never run here!
            m_alarmStatus.replace(index, SQL_Alarm_Status_Hold_Occur_NoAck);
            break;
        }
    } else {    //报警非保持
        switch (m_alarmStatus.at(index)) {
        case SQL_Alarm_Status_NonHold_Occur:
            break;
        case SQL_Alarm_Status_NonHold_Release:
            m_alarmStatus.replace(index, SQL_Alarm_Status_NonHold_Occur);
            initAlarmData(chanCoder, levelNum, SQL_Alarm_Status_NonHold_Occur, &alarmData);
            ThreadDataRecord::instance()->newAlarmData(&alarmData);
            break;
        default:
            m_alarmStatus.replace(index, SQL_Alarm_Status_NonHold_Occur);
            break;
        }
    }
}

/**
 * @brief EventAction::releaseAlarm
 *  报警释放处理
 * @param chanCoder：通道编码
 * @param levelNum：报警电平号1-4
 */
void EventAction::releaseAlarm(quint32 chanCoder, quint8 levelNum)
{
    quint32 index;  //通道在报警状态中的索引
    ALARM_DATA alarmData;

    if (false == getIndexofAlarmStatus(chanCoder, &index))
        return;

    Q_ASSERT(levelNum<4);
    index = index<<2;
    index += levelNum;

    //报警保持
    if (Alarm_Indicator_Hold == DevConfigPtr->systemConfig.alramIndicator) {
        switch (m_alarmStatus.at(index)) {
        case SQL_Alarm_Status_Hold_Occur_NoAck: //报警发生，且未解除（Non ACK）
            qDebug("报警释放：之前 Occur + NoAck,coder:0x%x, lev:%d\n", chanCoder, levelNum);
            m_alarmStatus.replace(index, SQL_Alarm_Status_Hold_Release_NoAck);
            initAlarmData(chanCoder, levelNum, SQL_Alarm_Status_Hold_Release_NoAck, &alarmData);
            ThreadDataRecord::instance()->newAlarmData(&alarmData);
            break;
        case SQL_Alarm_Status_Hold_Occur_Ack:       //报警发生，且已解除（ACK）
            qDebug("报警释放：之前 Occur + Ack,coder:0x%x, lev:%d\n", chanCoder, levelNum);
            m_alarmStatus.replace(index, SQL_Alarm_Status_Hold_Release_Ack);
            initAlarmData(chanCoder, levelNum, SQL_Alarm_Status_Hold_Occur_Ack, &alarmData);
            ThreadDataRecord::instance()->newAlarmData(&alarmData);
            break;
        case SQL_Alarm_Status_Hold_Release_NoAck:   //报警释放，且未解除（Non ACK）
            break;
        case SQL_Alarm_Status_Hold_Release_Ack:     //报警释放，且已解除（ACK）
            break;
        default:
            m_alarmStatus.replace(index, SQL_Alarm_Status_Hold_Release_Ack);
            break;
        }
    } else {    //报警非保持
        switch (m_alarmStatus.at(index)) {
        case SQL_Alarm_Status_NonHold_Occur:
            m_alarmStatus.replace(index, SQL_Alarm_Status_NonHold_Release);
            initAlarmData(chanCoder, levelNum, SQL_Alarm_Status_Hold_Release_NoAck, &alarmData);
            ThreadDataRecord::instance()->newAlarmData(&alarmData);
            break;
        case SQL_Alarm_Status_NonHold_Release:
            break;
        default:
            m_alarmStatus.replace(index, SQL_Alarm_Status_NonHold_Release);
            break;
        }
    }
}

/*
 * 功能：
 * 	  根据传入的动作对象，执行相应的动作
 * 参数：
 * 	  ACTION_OBJECT &action：待执行的动作对象
 * 返回值：
 * 	  无
 */
void EventAction::execAction(ACTION_OBJECT &action)
{
    quint8 msgWriteMode;
    if (action.typeClass == 1) {            //Action_Type
        switch (action.type1) {
        case Action_Type1_Recorded: //记录
            if (Action_Detail_Start == action.detail) {
                ThreadDataSync::instance()->setRecordStatus(true);
            } else if (Action_Detail_Stop == action.detail) {
                ThreadDataSync::instance()->setRecordStatus(false);
            }
            break;
        case Action_Type1_Computation: //运算
            if (Action_Detail_Start == action.detail)
                ThreadDataProcess::instance()->setMathStatus(Math_Status_Type_On);
            else if (Action_Detail_Stop == action.detail)
                ThreadDataProcess::instance()->setMathStatus(Math_Status_Type_Off);
            else if (Action_Detail_Reset == action.detail)
                ThreadDataProcess::instance()->setMathStatus(Math_Status_Type_On_Reset);
            break;
        case Action_Type1_SwitchRate:   //切换显示率
            p_gCfgTrendGroup->getRealTimeTrendBuf()->changeTrendPeriod();
            break;
        case Action_Type1_Flag:   //标志
            this->setInterFlag(action.number-1, true);
            break;
        case Action_Type1_ManualSample: //手动采样
            ThreadDataSync::instance()->setManualSample();
            break;
        case Action_Type1_AlarmAck: //解除报警
            if (0 == action.number) {
                ackAlarmAll();
            } else {
                ackAlarmSingle(action.number, action.msgGroup-1);
            }
            break;
        case Action_Type1_Snapshot: //画面存储
            WinMainFrame::instance()->slotSnapshot();
            break;
//        case Action_Type1_AdjustTime:   //时间调整
//            break;
        case Action_Type1_SaveDispData: //保存显示数据
            ThreadDataSave::instance()->saveRecordingDisp();
            break;
        case Action_Type1_SaveEventData://保存事件数据
            ThreadDataSave::instance()->saveRecordingEvent();
            break;
        case Action_Type1_EventTrigger:     //事件触发
            DevConfigPtr->eventTriggered = true;
            break;
        case Action_Type1_Message:  //信息写入
            msgWriteMode = DevConfigPtr->displayTrend.trendMsgWriteMode;
            if (Action_Detail_AllGrp == action.detail) {
                DevConfigPtr->displayTrend.trendMsgWriteMode = 0;
            } else {
                DevConfigPtr->displayTrend.trendMsgWriteMode = action.msgGroup;
            }
            WinMainFrame::instance()->getTrendGroup()->setCharMsg(action.number,
                                                                  QString((const char *)DevConfigPtr->displayMessage.preMessage[action.number-1]));
            DevConfigPtr->displayTrend.trendMsgWriteMode = msgWriteMode;
            break;
        case Action_Type1_SwitchGroup:  //切换显示组
            WinMainFrame::instance()->slotGroupChanged(action.number-1);
            break;
        case Action_Type1_TimerReset:     //相对时间计时器复位
            Timer::instance()->slotRelativeTimerReset();
            break;
//        case Action_Type1_LoadSetting:  //读入设置数据
//            break;
        case Action_Type1_HomeWin:      //HOME界面
            WinMainFrame::instance()->slotFrameChanged();
            break;
//        case Action_Type1_Relay:              //继电器
//            break;
//        case Action_Type1_InternalSwitch:   //内部开关
//            break;
        default:
            break;
        }
    } else if (action.typeClass == 2){  //Action_Type2
        switch (action.type2) {
        case Action_Type2_Record_Start_Stop:            //记录开始/停止
            if (action.trigType == Trigger_Type_Raise) {
                ThreadDataSync::instance()->setRecordStatus(true);
            } else if (action.trigType == Trigger_Type_Falling) {
                ThreadDataSync::instance()->setRecordStatus(false);
            }
            break;
        case Action_Type2_Computation_Start_Stop:   //运算开始/停止
            if (action.trigType == Trigger_Type_Raise) {
                ThreadDataProcess::instance()->setMathStatus(Math_Status_Type_On);
            } else if (action.trigType == Trigger_Type_Falling) {
                ThreadDataProcess::instance()->setMathStatus(Math_Status_Type_Off);
            }
            break;
        case Action_Type2_SwitchRate_1_2:                   //切换显示率1/2
            p_gCfgTrendGroup->getRealTimeTrendBuf()->changeTrendPeriod();
            break;
        case Action_Type2_Flag_On_Off:                      //标志1-on, 0-off
            if (action.trigType == Trigger_Type_Raise) {
                this->setInterFlag(action.number-1, true);
            } else if (action.trigType == Trigger_Type_Falling) {
                this->setInterFlag(action.number-1, false);
            }
            break;
//        case Action_Type2_Relay_On_Off:                     //继电器on/off
//            break;
//        case Action_Type2_InternalSwitch_On_Off:     //内部开关on/off
//            break;
        default:
            break;
        }
    } else {
        qDebug("Error! unknown action type");
    }
}

/**
 * @brief EventAction::slotEventConfigChanged
 *          事件动作配置变更处理槽函数
 * 调用前提：
 *          1.任意事件动作配置变更时
 */
void EventAction::slotEventConfigChanged()
{
    resetBitArray();

    //事件动作配置分类预处理
    m_eventInterSW.clear();
    m_eventRemoteDI.clear();
    m_eventRelayDO.clear();
    m_eventAlarmIO.clear();
    m_eventAlarmMath.clear();
    m_eventAlarmComm.clear();
    m_eventAlarmAll.clear();
    m_eventTimer.clear();
    m_eventMatchTimer.clear();
    m_eventUserFunc.clear();
    m_eventDevStaus.clear();

    for (int i=0; i<EVENT_ACTION_NUMBER; ++i) {
        if (DevConfigPtr->eventActiveConfig.eventActionSwitch[i]) {
            //事件对象
            m_eventObj[i].type = DevConfigPtr->eventActiveConfig.eventType[i];
            m_eventObj[i].number = DevConfigPtr->eventActiveConfig.eventNumber[i];
            m_eventObj[i].alarmNumber = DevConfigPtr->eventActiveConfig.eventAlarmNumber[i];
            m_eventObj[i].status = DevConfigPtr->eventActiveConfig.eventStatus[i];
            m_eventObj[i].opeMode = DevConfigPtr->eventActiveConfig.eventOpeMode[i];
            //动作对象
            if (m_eventObj[i].opeMode == Operation_Mode_Type_Both) {
                m_actionObj[i].typeClass = 2;
                m_actionObj[i].type2 = (Action_Type2)DevConfigPtr->eventActiveConfig.activeType[i];
            } else {
                m_actionObj[i].typeClass = 1;
                m_actionObj[i].type1 = DevConfigPtr->eventActiveConfig.activeType[i];
            }
            m_actionObj[i].number = DevConfigPtr->eventActiveConfig.activeNo[i];
            m_actionObj[i].detail = DevConfigPtr->eventActiveConfig.activeDeatil[i];
            m_actionObj[i].msgGroup = DevConfigPtr->eventActiveConfig.actionGroupNo[i];
            switch (DevConfigPtr->eventActiveConfig.eventType[i]) {
            case Event_Type_InterSwitch:
                m_eventInterSW.append(i);
                break;
            case Event_Type_Remote:
                m_eventRemoteDI.append(i);
                //TODO,远程控制输入模块变更处理
                break;
            case Event_Type_Relay:
                m_eventRelayDO.append(i);
                break;
            case Event_Type_Alarm_IO:
                m_eventAlarmIO.append(i);
                break;
            case Event_Type_Alarm_Math:
                m_eventAlarmMath.append(i);
                break;
            case Event_Type_Alarm_Comm:
                m_eventAlarmComm.append(i);
                break;
            case Event_Type_Alarm_All:
                m_eventAlarmAll.append(i);
                break;
            case Event_Type_Timer:
                m_eventTimer.append(i);
                break;
            case Event_Type_Timer_Match:
                m_eventMatchTimer.append(i);
                break;
            case Event_Type_UserFunc:
                m_eventUserFunc.append(i);
                break;
            case Event_Type_Status:
                m_eventDevStaus.append(i);
                break;
            }
        }
    }
}

/*
 * 功能：
 * 	  系统定时器1~4时间触发槽函数，根据事件动作配置，决定
 * 是否执行相应的动作
 * 参数：
 * 	  int index：定时器索引0~3
 * 返回值：
 * 	  无
 */
void EventAction::slotSysTimerHandle(int index)
{
    quint32 eventIndex;
    quint8 timeNum;
    for (quint8 i=0; i<m_eventTimer.count(); ++i) {
        eventIndex = m_eventTimer.at(i);
        timeNum = m_eventObj[eventIndex].number-1;
        if (timeNum == index) {
            m_actionObj[eventIndex].trigType = Trigger_Type_Edge;
            execAction(m_actionObj[eventIndex]);
        }
    }
}

/*
 * 功能：
 * 	  系统匹配定时器1~4时间触发槽函数，根据事件动作配置，决定
 * 是否执行相应的动作
 * 参数：
 * 	  int index：匹配定时器索引0~3
 * 返回值：
 * 	  无
 */
void EventAction::slotSysMatchTimerHandle(int index)
{
    quint32 eventIndex;
    quint8 timeNum;
    for (quint8 i=0; i<m_eventMatchTimer.count(); ++i) {
        eventIndex = m_eventMatchTimer.at(i);
        timeNum = m_eventObj[eventIndex].number-1;
        if (timeNum == index) {
            execAction(m_actionObj[eventIndex]);
        }
    }
}

/*
 * 功能：
 * 	  用户自定义功能键处理槽函数
 * 参数：
 * 	  int index：用户自定义功能号0~1
 * 返回值：
 * 	  无
 */
void EventAction::slotUserFunc(int index)
{
    quint32 eventIndex;
    quint8 funcNum;

    for (quint8 i=0; i<m_eventUserFunc.count(); ++i) {
        eventIndex = m_eventUserFunc.at(i);
        funcNum = m_eventObj[eventIndex].number-1;
        if (index == funcNum) {
            m_actionObj[eventIndex].trigType = Trigger_Type_Edge;
            execAction(m_actionObj[eventIndex]);
        }
    }
}

bool EventAction::event(QEvent *event)
{
    /*动作事件类型*/
    if (event->type() == (static_cast<Application*>(qApp))->m_actionEvent) {
        EVENT_OBJECT actionEvent = *(static_cast<ActionEvent *>(event))->eventObj();

        for (quint8 i=0; i<m_eventDevStaus.count(); ++i) {
            quint32 eventIndex = m_eventDevStaus.at(i);
            if (m_eventObj[eventIndex].type == actionEvent.type &&\
                 m_eventObj[eventIndex].status == actionEvent.status) { //仪表状态类型匹配

                if (actionEvent.opeMode == Operation_Mode_Type_FallingEdge &&
                    (m_eventObj[eventIndex].opeMode == Operation_Mode_Type_FallingEdge ||
                     m_eventObj[eventIndex].opeMode == Operation_Mode_Type_Both)) { //下降沿
                    execAction(m_actionObj[eventIndex]);
                } else if (actionEvent.opeMode == Operation_Mode_Type_RisingEdge &&
                             (m_eventObj[eventIndex].opeMode == Operation_Mode_Type_RisingEdge ||
                             m_eventObj[eventIndex].opeMode == Operation_Mode_Type_Both)) {//上升沿
                    execAction(m_actionObj[eventIndex]);
                }
            }
        }

        return true;
    }

    return QObject::event(event);
}
