#include "cfgconfig_event.h"
#include "../Common/devboard.h"
#include "./GlobalData/configtreeobject.h"

CfgConfig_Event *CfgConfig_Event::instance()
{
    static CfgConfig_Event data;
    return &data;
}



CfgConfig_Event::CfgConfig_Event(QObject *parent) :
    QObject(parent)
{
}




QVariant CfgConfig_Event::getEventData(quint32 ID, quint32 group)
{
    switch (ID) {
    case ID_Event_EventAction_Switch:
        return p_eventConfig->eventActionSwitch[group];
        break;
    case ID_Event_Event_Type:
        return p_eventConfig->eventType[group];
        break;

    case ID_Event_Event_Num:
        return p_eventConfig->eventNumber[group];
        break;
    case ID_Event_Event_AlarmNum:
        return p_eventConfig->eventAlarmNumber[group];
        break;
    case ID_Event_Event_AlarmStatus:
        return p_eventConfig->eventStatus[group];
        break;
    case ID_Event_Event_Mode:
        return p_eventConfig->eventOpeMode[group];
        break;

    case ID_Event_Action_Type:
        return p_eventConfig->activeType[group];
        break;
    case ID_Event_Action_Num:
        return p_eventConfig->activeNo[group];
        break;
    case ID_Event_Action_Detail:
        return p_eventConfig->activeDeatil[group];
        break;
    case ID_Event_Action_GrpNum:
        return p_eventConfig->actionGroupNo[group];
        break;
    default:
        break;
    }
    return QVariant();
}


QString CfgConfig_Event::getEventNumObjValStr(quint32 num, Event_Type type)
{
    switch (type) {

    case Event_Type_UserFunc:
    case Event_Type_Timer:
    case Event_Type_Timer_Match:
        return QString::number(num);
        break;
    case Event_Type_Remote:
    case Event_Type_Relay:
    case Event_Type_Alarm_IO:
    {
        return CfgChannel::instance()->getChannelStr(num);
    }
        break;
    case Event_Type_Alarm_Math:
    {
        return CfgChannel::instance()->getChannelStr(num).replace("A", "0");
    }
        break;
    case Event_Type_Alarm_Comm:
    {
        return CfgChannel::instance()->getChannelStr(num).replace("C", "0");
    }
        break;
    default:
        return QString("%1").arg(num, 4, 10, QChar('0'));
        break;
    }
}



QString CfgConfig_Event::getEventObjStr(quint32 ID, quint32 /*group*/)
{
    switch (ID) {
    case ID_Event:
    case ID_Event_EventAction:
        return tr("Event action");
        break;
    case ID_Event_Num:
        return tr("Event action number");
        break;
    case ID_Event_EventAction_Switch:
        return tr("On/Off");
        break;
    case ID_Event_Event:
        return tr("Event");
        break;
    case ID_Event_Event_Type:
    case ID_Event_Action_Type:
        return tr("Type");
        break;
    case ID_Event_Event_Num:
    case ID_Event_Action_Num:
        return tr("Number");
        break;
    case ID_Event_Event_AlarmNum:
        return tr("Alarm number");
        break;
    case ID_Event_Event_AlarmStatus:
        return tr("Event details");
        break;
    case ID_Event_Event_Mode:
        return tr("Operation mode");
        break;

    case ID_Event_Action:
        return tr("Action");
        break;
    case ID_Event_Action_Detail:
        return tr("Detail");
        break;
    case ID_Event_Action_GrpNum:
        return tr("Group number");
        break;
    default:
        break;
    }
    return NULL;
}

QString CfgConfig_Event::getEventObjValStr(quint32 ID, quint32 /*group*/, ConfigObject *cfg)
{
    switch (ID) {
    case ID_Event_EventAction_Switch:
        return cfg->getData().toInt() == 0 ? tr("Off") : tr("On");
        break;
    case ID_Event_Event_Type:
        return getEventTypeObjValStr(cfg->getData().toInt());
        break;

    case ID_Event_Event_Num:
        return getEventNumObjValStr(cfg->getData().toUInt(), (Event_Type)ConfigTreeObject::getConfigData(EVENT_DELEGATE_EVENT_TYPE).toUInt());
        break;

    case ID_Event_Event_AlarmNum:
        return tr("Alarm Level %1").arg(cfg->getData().toString());
        break;
    case ID_Event_Event_AlarmStatus:
        return getEventStatusObjValStr(cfg->getData().toInt());
        break;
    case ID_Event_Event_Mode:
        return getEventOperationObjValStr(cfg->getData().toInt());
        break;

    case ID_Event_Action_Type:
        return getEventActionTypeObjValStr(cfg->getData().toInt());
        break;
    case ID_Event_Action_Num:
        return cfg->getData().toString();
        break;
    case ID_Event_Action_Detail:
        return getEventDetailTypeObjValStr(cfg->getData().toInt());
        break;
    case ID_Event_Action_GrpNum:
        return cfg->getData().toString();
        break;
    default:
        break;
    }
    return NULL;
}



QString CfgConfig_Event::getEventTypeObjValStr(quint32 type)
{
    switch (type) {
    case Event_Type_InterSwitch:
        return tr("Internal switch");
        break;
    case Event_Type_Remote:
        return tr("Remote");
        break;
    case Event_Type_Relay:
        return tr("Relay");
        break;
    case Event_Type_Alarm_IO:
        return tr("Alarm - I/O channel");
        break;
    case Event_Type_Alarm_Math:
        return tr("Alarm - Math channel");
        break;
    case Event_Type_Alarm_Comm:
        return tr("Alarm - Communication channel");
        break;
    case Event_Type_Alarm_All:
        return tr("Any alarm");
        break;

    case Event_Type_Timer:
        return tr("Timer");
        break;
    case Event_Type_Timer_Match:
        return tr("Match time timer");
        break;
    case Event_Type_UserFunc:
        return tr("User function key");
        break;
    case Event_Type_Status:
        return tr("Status");
        break;
    default:
        break;
    }
    return NULL;
}

QString CfgConfig_Event::getEventStatusObjValStr(quint32 type)
{
    switch (type) {
    case Device_Status_Type_Record:
        return tr("Record");
        break;
    case Device_Status_Type_Math:
        return tr("Computation");
        break;
    case Device_Status_Type_UserLock:
        return tr("User lock");
        break;
    case Device_Status_Type_UnderLogin:
        return tr("Under login");
        break;
    case Device_Status_Type_MemErr:
        return tr("Memory/Media error");
        break;
    case Device_Status_Type_MeasureErr:
        return tr("Measurement error");
        break;
    case Device_Status_Type_CommErr:
        return tr("Communication error");
        break;
    default:
        break;
    }
    return NULL;
}

QString CfgConfig_Event::getEventOperationObjValStr(quint32 type)
{
    switch (type) {
    case Operation_Mode_Type_RisingEdge:
        return tr("Rising edge");
        break;
    case Operation_Mode_Type_FallingEdge:
        return tr("Falling edge");
        break;
    case Operation_Mode_Type_Both:
        return tr("Rising / Falling edge");
        break;
    case Operation_Mode_Type_Edge:
        return tr("Edge");
        break;
    default:
        break;
    }
    return NULL;
}

QString CfgConfig_Event::getEventActionTypeObjValStr(quint32 type)
{
    quint32 mode = ConfigTreeObject::getConfigData(EVENT_DELEGATE_EVENT_MODE).toUInt();
    if (mode == Operation_Mode_Type_Both)
    {
        switch (type) {
        case Action_Type2_Record_Start_Stop:
            return tr("Recording start/stop");
            break;
        case Action_Type2_Computation_Start_Stop:
            return tr("Computation start/stop");
            break;
        case Action_Type2_SwitchRate_1_2:
            return tr("Switch the display rate 1/2");
            break;
        case Action_Type2_Flag_On_Off:
            return tr("Flag On/Off");
            break;
        default:
            break;
           }
    }
    switch (type) {
    case Action_Type1_Recorded:
        return tr("Record");
        break;
    case Action_Type1_Computation:
        return tr("Computation");
        break;
    case Action_Type1_SwitchRate:
        return tr("Switch the display rate");
        break;
    case Action_Type1_Flag:
        return tr("Flag");
        break;
    case Action_Type1_ManualSample:
        return tr("Manual sample");
        break;
    case Action_Type1_AlarmAck:
        return tr("AlarmAck");
        break;
    case Action_Type1_Snapshot:
        return tr("Snapshot");
        break;
    case Action_Type1_AdjustTime:
        return tr("Adjust the time");
        break;
    case Action_Type1_SaveDispData:
        return tr("Save display data");
        break;
    case Action_Type1_SaveEventData:
        return tr("Save event data");
        break;
    case Action_Type1_EventTrigger:
        return tr("Event trigger");
        break;
    case Action_Type1_Message:
        return tr("Message");
        break;
    case Action_Type1_SwitchGroup:
        return tr("Switch the display group");
        break;
    case Action_Type1_TimerReset:
        return tr("Reset the relative timer");
        break;
    case Action_Type1_LoadSetting:
        return tr("Load setting");
        break;
    case Action_Type1_HomeWin:
        return tr("Show the favorite display");
        break;
    default:
        break;
    }
    return NULL;
}

QString CfgConfig_Event::getEventDetailTypeObjValStr(quint32 type)
{
    switch (type) {
    case Action_Detail_Start:
        return tr("Start");
        break;
    case Action_Detail_Stop:
        return tr("Stop");
        break;
    case Action_Detail_Reset:
        return tr("Reset");
        break;
    case Action_Detail_AllGrp:
        return tr("All groups");
        break;
    case Action_Detail_SpecGrp:
        return tr("Specified group");
        break;
    default:
        break;
    }
    return NULL;
}





void CfgConfig_Event::setEventData(quint32 ID, quint32 group)
{
    switch (ID) {
    case ID_Event_EventAction_Switch:
        p_eventConfig->eventActionSwitch[group] = ConfigTreeObject::getConfigData(QString(EVENTCONFIG_GROUP_SW).arg(group)).toInt();
        break;
    case ID_Event_Event_Type:
        p_eventConfig->eventType[group] = (Event_Type)ConfigTreeObject::getConfigData(QString(EVENTCONFIG_EVENT_TYPE).arg(group)).toInt();
    case ID_Event_Event_Num:
        p_eventConfig->eventNumber[group] = ConfigTreeObject::getConfigData(QString(EVENTCONFIG_EVENT_NUMBER).arg(group)).toInt();
        break;
    case ID_Event_Event_AlarmNum:
        p_eventConfig->eventAlarmNumber[group] = ConfigTreeObject::getConfigData(QString(EVENTCONFIG_EVENT_ALARM).arg(group)).toInt();
        break;
    case ID_Event_Event_AlarmStatus:
        p_eventConfig->eventStatus[group] = (Device_Status_Type)ConfigTreeObject::getConfigData(QString(EVENTCONFIG_EVENT_STATUS).arg(group)).toInt();
        break;
    case ID_Event_Event_Mode:
        p_eventConfig->eventOpeMode[group] = (Operation_Mode_Type)ConfigTreeObject::getConfigData(QString(EVENTCONFIG_EVENT_MODE).arg(group)).toInt();
        break;

    case ID_Event_Action_Type:
        p_eventConfig->activeType[group] = (Action_Type)ConfigTreeObject::getConfigData(QString(EVENTCONFIG_ACTION_TYPE).arg(group)).toInt();
        break;
    case ID_Event_Action_Num:
        p_eventConfig->activeNo[group] = ConfigTreeObject::getConfigData(QString(EVENTCONFIG_ACTION_NUMBER).arg(group)).toInt();
        break;
    case ID_Event_Action_Detail:
        p_eventConfig->activeDeatil[group] = (Action_Detail)ConfigTreeObject::getConfigData(QString(EVENTCONFIG_ACTION_DETAIL).arg(group)).toInt();
        break;
    case ID_Event_Action_GrpNum:
        p_eventConfig->actionGroupNo[group] = ConfigTreeObject::getConfigData(QString(EVENTCONFIG_ACTION_GROUPNUM).arg(group)).toInt();
        break;
    default:
        break;
    }
}

