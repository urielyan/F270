#include "cfgconfig_record.h"
#include "./GlobalData/configtreeobject.h"

CfgConfig_Record *CfgConfig_Record::instance()
{
    static CfgConfig_Record data;
    return &data;
}



CfgConfig_Record::CfgConfig_Record(QObject *parent) :
    QObject(parent)
{
}




QVariant CfgConfig_Record::getRecordData(quint32 ID, quint32 group)
{
    if (ID >= Record_Basic_Start_ID && ID <= Record_Basic_End_ID)
        return getRecordBasicData(ID, group);
    else
        return getRecordChannelData(ID, group);
}

QString CfgConfig_Record::getRecordObjStr(quint32 ID, quint32 group)
{
    if (ID >= Record_Basic_Start_ID && ID <= Record_Basic_End_ID)
        return getRecordBasicObjStr(ID, group);
    else
        return getRecordChannelObjStr(ID, group);
}

QString CfgConfig_Record::getRecordObjValStr(quint32 ID, quint32 group, ConfigObject *cfg)
{
    if (ID >= Record_Basic_Start_ID && ID <= Record_Basic_End_ID)
        return getRecordBasicObjValStr(ID, group, cfg);
    else
        return getRecordChannelObjValStr(ID, group, cfg);
}


void CfgConfig_Record::setRecordData(quint32 ID, quint32 group)
{
    if (ID >= Record_Basic_Start_ID && ID <= Record_Basic_End_ID)
        setRecordBasicData(ID, group);
    else
        setRecordChannelData(ID, group);
}






QVariant CfgConfig_Record::getRecordBasicData(quint32 ID, quint32 /*group*/)
{
    switch (ID) {
    case ID_Record_Basic_Mode_Value:
        return p_recordConfig->recordMode;
        break;
    case ID_Record_Basic_Event_Interval:
        return p_recordConfig->eventRecordInterval_ms;
        break;
    case ID_Record_Basic_Event_Mode:
        return p_recordConfig->eventRecordMode;
        break;
    case ID_Record_Basic_Event_Len:
        return p_recordConfig->eventRecordLength;
        break;
    case ID_Record_Basic_Event_TriggeredSrc:
        return p_recordConfig->eventRecordTriggerSrc;
        break;
    case ID_Record_Basic_Event_TriggerPre:
        return p_recordConfig->eventRecordPreTrigger;
        break;

    case ID_Record_Basic_Disp_Interval:
        return p_recordConfig->displayRecordLength;
        break;

    case ID_Record_Basic_Confirm_OnOff:
        return p_recordConfig->recordConfirm;
        break;
    default:
        break;
    }
    return QVariant();
}

QVariant CfgConfig_Record::getRecordChannelData(quint32 ID, quint32 /*group*/)
{
    switch (ID) {
    case ID_Record_Basic_Event_ChNum:
        return p_recordConfig->eventRecordChNum;
        break;
    case ID_Record_Basic_Disp_ChNum:
        return p_recordConfig->displayRecordChNum;
        break;
    case ID_Record_Basic_Sample_ChNum:
        return p_recordConfig->manualSampleChNum;
        break;

    case ID_Record_Basic_Event_ChItem:
    {
        QList<QVariant> channelList;
        int cnt = p_recordConfig->eventRecordChNum;
        for (int i = 0; i < cnt; i++)
            channelList.append(p_recordConfig->eventRecordChItem[i]);
        return channelList;
    }
        break;
    case ID_Record_Basic_Disp_ChItem:
    {
        QList<QVariant> channelList;
        int cnt = p_recordConfig->displayRecordChNum;
        for (int i = 0; i < cnt; i++)
            channelList.append(p_recordConfig->displayRecordChItem[i]);
        return channelList;
    }
        break;
    case ID_Record_Basic_Sample_ChItem:
    {
        QList<QVariant> channelList;
        int cnt = p_recordConfig->manualSampleChNum;
        for (int i = 0; i < cnt; i++)
            channelList.append(p_recordConfig->manualSampleChItem[i]);
        return channelList;
    }
        break;
    default:
        break;
    }
    return QVariant();
}









QString CfgConfig_Record::getRecordBasicObjStr(quint32 ID, quint32 /*group*/)
{
    switch (ID) {
    case ID_Record:
        return tr("Recording settings");
        break;
    case ID_Record_Basic:
        return tr("Basic settings");
        break;
    case ID_Record_Basic_Mode:
        return tr("Recording mode");
        break;
    case ID_Record_Basic_Mode_Value:
        return tr("Mode");
        break;

    case ID_Record_Basic_Event:
        return tr("Event data");
        break;
    case ID_Record_Basic_Event_Interval:
        return tr("Recording interval");
        break;
    case ID_Record_Basic_Event_Mode:
        return tr("Recording mode");
        break;
    case ID_Record_Basic_Event_Len:
        return tr("Data length");
        break;
    case ID_Record_Basic_Event_SignaFlag:
        return tr("Signal flag");
        break;
    case ID_Record_Basic_Event_Triggered:
        return tr("Trigger");
        break;
    case ID_Record_Basic_Event_TriggeredSrc:
        return tr("Trigger source operation");
        break;
    case ID_Record_Basic_Event_TriggerPre:
        return tr("Pre-trigger");
        break;

    case ID_Record_Basic_Disp:
        return tr("Display data, Trend waveform");
        break;
    case ID_Record_Basic_Disp_Interval:
        return tr("Saving interval");
        break;
    case ID_Record_Basic_Confirm:
        return tr("Record confirmation action");
        break;
    case ID_Record_Basic_Confirm_OnOff:
        return tr("Confirmation screen");
        break;
    default:
        break;
    }
    return NULL;
}

QString CfgConfig_Record::getRecordChannelObjStr(quint32 ID, quint32 /*group*/)
{
    switch (ID) {
    case ID_Record_CH:
        return tr("Recording channel settings");
        break;
    case ID_Record_Basic_Event_ChNum:
        return tr("Event data");
        break;
    case ID_Record_Basic_Disp_ChNum:
        return tr("Display data, Trend waveform");
        break;
    case ID_Record_Basic_Sample_ChNum:
        return tr("Manual sample");
        break;
    default:
        break;
    }
    return NULL;
}





QString CfgConfig_Record::getRecordBasicObjValStr(quint32 ID, quint32 /*group*/, ConfigObject *cfg)
{
    switch (ID) {
    case ID_Record_Basic_Mode_Value:
    {
        int type = cfg->getData().toInt();
        if (type == Record_Mode_Display)
            return tr("Display");
        else if (type == Record_Mode_Event)
            return tr("Event");
        else
            return tr("Display + Event");
    }
        break;
    case ID_Record_Basic_Event_Interval:
    {
        int data = cfg->getData().toInt();
        if (data < 1000)
            return QString("%1 %2").arg(data).arg(tr("ms"));
        else if (data >= 1000 && data < 60000)
            return QString("%1 %2").arg(data / 1000).arg(tr("s"));
        else if (data >= 60000 && data < 3600000)
            return QString("%1 %2").arg(data / 60000).arg(tr("min"));
        else
            return QString("%1 %2").arg(data / 3600000).arg(tr("h"));
    }
        break;
    case ID_Record_Basic_Event_Mode:
    {
        int type = cfg->getData().toInt();
        if (type == Event_Record_Mode_Free)
            return tr("Free");
        else if (type == Event_Record_Mode_Single)
            return tr("Single");
        else
            return tr("Repeat");
    }
        break;
    case ID_Record_Basic_Event_Len:
    {
        int data = cfg->getData().toInt();
        if (data >= 0 && data < 60)
            return QString("%1 %2").arg(data).arg(tr("min"));
        else if (data >= 60 && data < 1440)
            return QString("%1 %2").arg(data / 60).arg(tr("h"));
        else
            return QString("%1 %2").arg(data / 1440).arg(tr("day"));
    }
        break;
    case ID_Record_Basic_Event_TriggeredSrc:
        return cfg->getData().toInt() == 0 ? tr("Off") : tr("On");
        break;
    case ID_Record_Basic_Event_TriggerPre:
        return cfg->getData().toString() + " %";
        break;

    case ID_Record_Basic_Disp_Interval:
    {
        int data = cfg->getData().toInt();
        if (data >= 0 && data < 60)
            return QString("%1 %2").arg(data).arg(tr("min"));
        else if (data >= 60 && data < 1440)
            return QString("%1 %2").arg(data / 60).arg(tr("h"));
        else
            return QString("%1 %2").arg(data / 1440).arg(tr("day"));
    }
        break;
    case ID_Record_Basic_Confirm_OnOff:
        return cfg->getData().toInt() == 0 ? tr("Off") : tr("On");
        break;
    default:
        break;
    }
    return cfg->getData().toString();
}

QString CfgConfig_Record::getRecordChannelObjValStr(quint32 ID, quint32 /*group*/, ConfigObject *cfg)
{
    int ch = cfg->getData().toInt();
    switch (ID) {
    case ID_Record_Basic_Event_ChNum:
    case ID_Record_Basic_Disp_ChNum:
        return QString("%1 / 500 CH").arg(ch);
        break;
    case ID_Record_Basic_Sample_ChNum:
        return QString("%1 / 50 CH").arg(ch);
        break;
    default:
        break;
    }
    return cfg->getData().toString();
}







void CfgConfig_Record::setRecordBasicData(quint32 ID, quint32 /*group*/)
{
    switch (ID) {
    case ID_Record_Basic_Mode_Value:
        p_recordConfig->recordMode = (Record_Mode)ConfigTreeObject::getConfigData(RECORDCONFIG_BASIC_MODE_VALUE).toInt();
        break;
    case ID_Record_Basic_Event_Interval:
        p_recordConfig->eventRecordInterval_ms = (Event_Record_Interval)ConfigTreeObject::getConfigData(RECORDCONFIG_BASIC_EVENT_INTERVAL).toInt();
        break;
    case ID_Record_Basic_Event_Mode:
        p_recordConfig->eventRecordMode = (Event_Record_Mode)ConfigTreeObject::getConfigData(RECORDCONFIG_BASIC_EVENT_MODE).toInt();
        break;
    case ID_Record_Basic_Event_Len:
        p_recordConfig->eventRecordLength = (Event_Record_DataLength)ConfigTreeObject::getConfigData(RECORDCONFIG_BASIC_EVENT_LENGTH).toInt();
        break;
    case ID_Record_Basic_Event_TriggeredSrc:
        p_recordConfig->eventRecordTriggerSrc = ConfigTreeObject::getConfigData(RECORDCONFIG_BASIC_EVENT_OPERATE).toInt();
        break;
    case ID_Record_Basic_Event_TriggerPre:
        p_recordConfig->eventRecordPreTrigger = ConfigTreeObject::getConfigData(RECORDCONFIG_BASIC_EVENT_TRIGGER).toInt();
        break;

    case ID_Record_Basic_Disp_Interval:
        p_recordConfig->displayRecordLength = (Display_Record_DataLength)ConfigTreeObject::getConfigData(RECORDCONFIG_BASIC_DISP_VALUE).toInt();
        break;

    case ID_Record_Basic_Confirm_OnOff:
        p_recordConfig->recordConfirm = ConfigTreeObject::getConfigData(RECORDCONFIG_BASIC_ACTION_SWITCH).toInt();
        break;
    default:
        break;
    }
}


void CfgConfig_Record::setRecordChannelData(quint32 ID, quint32 /*group*/)
{
    switch (ID) {
    case ID_Record_Basic_Event_ChNum:
        p_recordConfig->eventRecordChNum = ConfigTreeObject::getConfigData(RECORDCONFIG_CHANNEL_EVENT).toInt();
        break;
    case ID_Record_Basic_Disp_ChNum:
        p_recordConfig->displayRecordChNum = ConfigTreeObject::getConfigData(RECORDCONFIG_CHANNEL_DISP).toInt();
        break;
    case ID_Record_Basic_Sample_ChNum:
        p_recordConfig->manualSampleChNum = ConfigTreeObject::getConfigData(RECORDCONFIG_CHANNEL_MANUAL).toInt();
        break;

    case ID_Record_Basic_Event_ChItem:
    {
        QList<QVariant> channelList = ConfigTreeObject::getConfigData(RECORDCONFIG_CHANNEL_EVENT_CHANITEM).toList();
        QList<QVariant>::iterator var;
        memset(p_recordConfig->eventRecordChItem, 0, 500 * sizeof(quint32)); //清空数组
        int i = 0;
        for(var = channelList.begin(); var != channelList.end(); var++)
            p_recordConfig->eventRecordChItem[i++] = (*var).toUInt();
    }
        break;
    case ID_Record_Basic_Disp_ChItem:
    {
        QList<QVariant> channelList = ConfigTreeObject::getConfigData(RECORDCONFIG_CHANNEL_DISP_CHANITEM).toList();
        QList<QVariant>::iterator var;
        memset(p_recordConfig->displayRecordChItem, 0, 500 * sizeof(quint32)); //清空数组
        int i = 0;
        for(var = channelList.begin(); var != channelList.end(); var++)
            p_recordConfig->displayRecordChItem[i++] = (*var).toUInt();
    }
        break;
    case ID_Record_Basic_Sample_ChItem:
    {
        QList<QVariant> channelList = ConfigTreeObject::getConfigData(RECORDCONFIG_CHANNEL_MANUAL_CHANITEM).toList();
        QList<QVariant>::iterator var;
        memset(p_recordConfig->manualSampleChItem, 0, 50 * sizeof(quint32)); //清空数组
        int i = 0;
        for(var = channelList.begin(); var != channelList.end(); var++)
            p_recordConfig->manualSampleChItem[i++] = (*var).toUInt();
    }
        break;
    default:
        break;
    }
}

