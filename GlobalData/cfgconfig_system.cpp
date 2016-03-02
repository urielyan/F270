#include "cfgconfig_system.h"
#include "./GlobalData/configtreeobject.h"

CfgConfig_System *CfgConfig_System::instance()
{
    static CfgConfig_System data;
    return &data;
}

CfgConfig_System::CfgConfig_System(QObject *parent) :
    QObject(parent)
{
//    p_envConfig = &DevConfigPtr->systemConfig;
}





QVariant CfgConfig_System::getSystemData(quint32 ID, quint32 group)
{
    if (ID >= System_Env_Start_ID && ID <= System_Env_End_ID)
        return getSystemEnvData(ID, group);
    else if (ID >= System_Alarm_Start_ID && ID <= System_Alarm_End_ID)
        return getSystemAlarmData(ID, group);
    else if (ID >= System_Time_Start_ID && ID <= System_Time_End_ID)
        return getSystemTimeData(ID, group);
    else if (ID >= System_Snd_Start_ID && ID <= System_Snd_End_ID)
        return getSystemSndData(ID, group);
    else if (ID >= System_Relay_Start_ID && ID <= System_Relay_End_ID)
        return getSystemRelayData(ID, group);
    else if (ID >= System_Switch_Start_ID && ID <= System_Switch_End_ID)
        return getSystemSwitchData(ID, group);
    else if (ID >= System_Printer_Start_ID && ID <= System_Printer_End_ID)
        return getSystemPrintData(ID, group);
    else
        return getSystemDevAndComData(ID, group);
}

QString CfgConfig_System::getSystemObjStr(quint32 ID, quint32 group)
{
    if (ID >= System_Env_Start_ID && ID <= System_Env_End_ID)
        return getSystemEnvObjStr(ID, group);
    else if (ID >= System_Alarm_Start_ID && ID <= System_Alarm_End_ID)
        return getSystemAlarmObjStr(ID, group);
    else if (ID >= System_Time_Start_ID && ID <= System_Time_End_ID)
        return getSystemTimeObjStr(ID, group);
    else if (ID >= System_Snd_Start_ID && ID <= System_Snd_End_ID)
        return getSystemSndObjStr(ID, group);
    else if (ID >= System_Relay_Start_ID && ID <= System_Relay_End_ID)
        return getSystemRelayObjStr(ID, group);
    else if (ID >= System_Switch_Start_ID && ID <= System_Switch_End_ID)
        return getSystemSwitchObjStr(ID, group);
    else if (ID >= System_Printer_Start_ID && ID <= System_Printer_End_ID)
        return getSystemPrintObjStr(ID, group);
    else
        return getSystemDevAndComObjStr(ID, group);
}

QString CfgConfig_System::getSystemObjValStr(quint32 ID, quint32 group, ConfigObject *cfg)
{
    if (ID >= System_Env_Start_ID && ID <= System_Env_End_ID)
        return getSystemEnvObjValStr(ID, group, cfg);
    else if (ID >= System_Alarm_Start_ID && ID <= System_Alarm_End_ID)
        return getSystemAlarmObjValStr(ID, group, cfg);
    else if (ID >= System_Time_Start_ID && ID <= System_Time_End_ID)
        return getSystemTimeObjValStr(ID, group, cfg);
    else if (ID >= System_Snd_Start_ID && ID <= System_Snd_End_ID)
        return getSystemSndObjValStr(ID, group, cfg);
    else if (ID >= System_Relay_Start_ID && ID <= System_Relay_End_ID)
        return getSystemRelayObjValStr(ID, group, cfg);
    else if (ID >= System_Switch_Start_ID && ID <= System_Switch_End_ID)
        return getSystemSwitchObjValStr(ID, group, cfg);
    else if (ID >= System_Printer_Start_ID && ID <= System_Printer_End_ID)
        return getSystemPrintObjValStr(ID, group, cfg);
    else
        return getSystemDevAndComObjValStr(ID, group, cfg);
}

void CfgConfig_System::setSystemData(quint32 ID, quint32 group)
{
    if (ID >= System_Env_Start_ID && ID <= System_Env_End_ID)
        setSystemEnvData(ID, group);
    else if (ID >= System_Alarm_Start_ID && ID <= System_Alarm_End_ID)
        setSystemAlarmData(ID, group);
    else if (ID >= System_Time_Start_ID && ID <= System_Time_End_ID)
        setSystemTimeData(ID, group);
    else if (ID >= System_Snd_Start_ID && ID <= System_Snd_End_ID)
        setSystemSndData(ID, group);
    else if (ID >= System_Relay_Start_ID && ID <= System_Relay_End_ID)
        setSystemRelayData(ID, group);
    else if (ID >= System_Switch_Start_ID && ID <= System_Switch_End_ID)
        setSystemSwitchData(ID, group);
    else if (ID >= System_Printer_Start_ID && ID <= System_Printer_End_ID)
        setSystemPrintData(ID, group);
    else
        setSystemDevAndComData(ID, group);
}






QVariant CfgConfig_System::getSystemEnvData(quint32 ID, quint32 /*group*/)
{
    switch (ID) {
    case ID_System_Env_Language:
        return p_envConfig->languageType;
        break;
    case ID_System_Env_Temp:
        return p_envConfig->temperatureUnit;
        break;
    case ID_System_Env_PointType:
        return p_envConfig->decimalPointType;
        break;

    case ID_System_Env_Date_Format:
        return p_envConfig->dateFormat;
        break;
    case ID_System_Env_Date_delimiter:
        return p_envConfig->delimiterType;
        break;
    case ID_System_Env_Date_Mon:
        return p_envConfig->monthIndicator;
        break;
    default:
        break;
    }
    return QVariant();
}

QVariant CfgConfig_System::getSystemAlarmData(quint32 ID, quint32 /*group*/)
{
    switch (ID) {
    case ID_System_Alarm_Rate_Dec:
        return p_envConfig->rateDecNum;
        break;

    case ID_System_Alarm_Rate_Inc:
        return p_envConfig->rateIncNum;
        break;
    case ID_System_Alarm_Ind_Value:
        return p_envConfig->alramIndicator;
        break;
    case ID_System_Alarm_ACK_Switch:
        return p_envConfig->individualAlarmAck;
        break;
    default:
        break;
    }
    return QVariant();
}

QVariant CfgConfig_System::getSystemTimeData(quint32 ID, quint32 /*group*/)
{
    switch (ID) {
    case ID_System_Time_H:
        return p_envConfig->diffGMTHour + 13;
        break;
    case ID_System_Time_m:
        return p_envConfig->diffGMTMin;
        break;
    default:
        break;
    }
    return QVariant();
}

QVariant CfgConfig_System::getSystemSndData(quint32 ID, quint32 /*group*/)
{
    switch (ID) {
    case ID_System_Snd_Snd_Touch:
        return p_envConfig->touchSnd;
        break;
    case ID_System_Snd_Snd_Warning:
        return p_envConfig->warningSnd;
        break;
    case ID_System_Snd_LED_LED:
        return p_envConfig->menuKeyLED;
        break;
    default:
        break;
    }
    return QVariant();
}


QVariant CfgConfig_System::getSystemRelayData(quint32 ID, quint32 /*group*/)
{
    switch (ID) {
    case ID_System_Relay_OutputMode:
        return p_envConfig->relayOutMode;
        break;
    case ID_System_Relay_MemMediaStatus:
        return p_envConfig->memMediaStatus;
        break;
    case ID_System_Relay_Measure:
        return p_envConfig->measureError;
        break;
    case ID_System_Relay_Comm:
        return p_envConfig->commError;
        break;
    case ID_System_Relay_Record:
        return p_envConfig->recordStop;
        break;
    case ID_System_Relay_Alarm:
        return p_envConfig->alarm;
        break;
    default:
        break;
    }
    return QVariant();
}

QVariant CfgConfig_System::getSystemSwitchData(quint32 ID, quint32 group)
{
    switch (ID) {
    case ID_System_Switch_Switch_Mode:
        return p_envConfig->interSwitchMode[group];
        break;
    case ID_System_Switch_Switch_Operate:
        return p_envConfig->interSwitchOperate[group];
        break;
    default:
        break;
    }
    return QVariant();
}


QVariant CfgConfig_System::getSystemPrintData(quint32 ID, quint32 /*group*/)
{
    switch (ID) {
    case ID_System_Printer_IP:
    {
        QString addrStr = QString("%1.%2.%3.%4")
                .arg(p_envConfig->printerIP[0])
                .arg(p_envConfig->printerIP[1])
                .arg(p_envConfig->printerIP[2])
                .arg(p_envConfig->printerIP[3]);
        return addrStr;
    }
        break;
    case ID_System_Printer_Size:
        return p_envConfig->printerPaperSize;
        break;
    case ID_System_Printer_Orientation:
        return p_envConfig->printerOrientation;
        break;
    case ID_System_Printer_DPI:
        return p_envConfig->printerDPI;
        break;
    case ID_System_Printer_Num:
        return p_envConfig->pageNumber;
        break;
    default:
        break;
    }
    return QVariant();
}

QVariant CfgConfig_System::getSystemDevAndComData(quint32 ID, quint32 /*group*/)
{
    switch (ID) {
    case ID_System_Device_Tag:
        return getCharToString(p_envConfig->deviceTag);
        break;
    case ID_System_Device_TagNo:
        return getCharToString(p_envConfig->deviceTagNo);
        break;
    case ID_System_Comment_Comment:
        return getCharToString(p_envConfig->settingComment);
        break;
    default:
        break;
    }
    return QVariant();
}








QString CfgConfig_System::getSystemEnvObjStr(quint32 ID, quint32 /*group*/)
{
    switch (ID) {
    case ID_System:
        return tr("System settings");
        break;
    case ID_System_Env:
        return tr("Environment (Language) settings");
        break;

    case ID_System_Env_Language:
        return tr("Language");
        break;
    case ID_System_Env_Temp:
        return tr("Temperature");
        break;
    case ID_System_Env_PointType:
        return tr("Decimal Point Type");
        break;
    case ID_System_Env_Date:
    case ID_System_Env_Date_Format:
        return tr("Date format");
        break;
    case ID_System_Env_Date_delimiter:
        return tr("Delimiter");
        break;
    case ID_System_Env_Date_Mon:
        return tr("Month indicator");
        break;
    default:
        break;
    }
    return NULL;
}

QString CfgConfig_System::getSystemAlarmObjStr(quint32 ID, quint32 /*group*/)
{
    switch (ID) {
    case ID_System_Alarm:
        return tr("Alarm basic settings");
        break;
    case ID_System_Alarm_Rate:
        return tr("Rate of change");
        break;

    case ID_System_Alarm_Rate_Dec:
        return tr("Decrease");
        break;
    case ID_System_Alarm_Rate_Inc:
        return tr("Increase");
        break;
    case ID_System_Alarm_Ind:
        return tr("Indicator");
        break;
    case ID_System_Alarm_Ind_Value:
        return tr("Hold/Nonhold");
        break;
    case ID_System_Alarm_ACK:
        return tr("AlarmACK");
        break;
    case ID_System_Alarm_ACK_Switch:
        return tr("Individual alarm ACK");
        break;
    default:
        break;
    }
    return NULL;
}

QString CfgConfig_System::getSystemTimeObjStr(quint32 ID, quint32 /*group*/)
{
    switch (ID) {
    case ID_System_Time:
        return tr("Time basic settings");
        break;
    case ID_System_Time_Zone:
        return tr("Time zone");
        break;
    case ID_System_Time_H:
        return tr("Hour");
        break;
    case ID_System_Time_m:
        return tr("Minute");
        break;
    default:
        break;
    }
    return NULL;
}

QString CfgConfig_System::getSystemSndObjStr(quint32 ID, quint32 /*group*/)
{
    switch (ID) {
    case ID_System_Snd:
        return tr("Sound, LED");
        break;
    case ID_System_Snd_Snd:
        return tr("Sound");
        break;
    case ID_System_Snd_Snd_Touch:
        return tr("Touch");
        break;
    case ID_System_Snd_Snd_Warning:
        return tr("Warning");
        break;

    case ID_System_Snd_LED:
        return tr("LED");
        break;
    case ID_System_Snd_LED_LED:
        return tr("MENU key LED");
        break;
    default:
        break;
    }
    return NULL;
}

QString CfgConfig_System::getSystemRelayObjStr(quint32 ID, quint32 /*group*/)
{
    switch (ID) {
    case ID_System_Relay:
        return tr("Status relay");
        break;
    case ID_System_Relay_OutputMode:        //继电器输出模式
        return tr("Fail relay");
        break;
    case ID_System_Relay_MemMediaStatus:
        return tr("Memory/Media status");
        break;
    case ID_System_Relay_Measure:
        return tr("Mesurement error");
        break;
    case ID_System_Relay_Comm:
        return tr("Communication error");
        break;
    case ID_System_Relay_Record:
        return tr("Record stop");
        break;
    case ID_System_Relay_Alarm:
        return tr("Alarm");
        break;
    default:
        break;
    }
    return NULL;
}

QString CfgConfig_System::getSystemSwitchObjStr(quint32 ID, quint32 /*group*/)
{
    switch (ID) {
    case ID_System_Switch:
        return tr("Internal switch settings");
        break;
    case ID_System_Switch_FirNum:
        return tr("First number");
        break;
    case ID_System_Switch_LastNum:
        return tr("Last number");
        break;
    case ID_System_Switch_Switch:
        return tr("Internal switch");
        break;
    case ID_System_Switch_Switch_Mode:
        return tr("Type");
        break;
    case ID_System_Switch_Switch_Operate:
        return tr("And/Or");
        break;
    default:
        break;
    }
    return NULL;
}

QString CfgConfig_System::getSystemPrintObjStr(quint32 ID, quint32 /*group*/)
{
    switch (ID) {
    case ID_System_Printer:
        return tr("Printer settings");
        break;
    case ID_System_Printer_IP:
        return tr("IP Address");
        break;
    case ID_System_Printer_Size:
        return tr("Paper size");
        break;
    case ID_System_Printer_Orientation:
        return tr("Page orientation");
        break;
    case ID_System_Printer_DPI:
        return tr("Resolution (dpi)");
        break;
    case ID_System_Printer_Num:
        return tr("Number of copies");
        break;
    default:
        break;
    }
    return NULL;
}

QString CfgConfig_System::getSystemDevAndComObjStr(quint32 ID, quint32 /*group*/)
{
    switch (ID) {
    case ID_System_Device:
    case ID_System_Device_Tag:
        return tr("Instruments tag");
        break;
    case ID_System_Device_TagNo:
        return tr("Instruments tag No.");
        break;
    case ID_System_Comment:
        return tr("Settings file");
        break;
    case ID_System_Comment_Comment:
        return tr("Settings file comment");
        break;
    default:
        break;
    }
    return NULL;
}






QString CfgConfig_System::getSystemEnvObjValStr(quint32 ID, quint32 /*group*/, ConfigObject *cfg)
{
    switch (ID) {
    case ID_System_Env_Language:
        return getSystemEnvLagObjValStr(cfg->getData().toInt());
        break;
    case ID_System_Env_Temp:
        return cfg->getData().toInt() == 0 ? "°C" : "°F";
        break;
    case ID_System_Env_PointType:
        return cfg->getData().toInt() == 0 ? tr("Point") : tr("Comma");
        break;

    case ID_System_Env_Date_Format:
    {
        int type = cfg->getData().toInt();
        if (type == Date_Format_YMD)
            return tr("Year Month Day");
        else if (type == Date_Format_MDY)
            return tr("Month Day Year");
        else
            return tr("Day Month Year");
    }
        break;
    case ID_System_Env_Date_delimiter:
    {
        int type = cfg->getData().toInt();
        if (type == Delimiter_Type_Slash)
            return tr("Slash (/)");
        else if (type == Delimiter_Type_Point)
            return tr("Point (.)");
        else
            return tr("hyphen (-)");
    }
        break;
    case ID_System_Env_Date_Mon:
        return cfg->getData().toInt() == 0 ? tr("Numerical") : tr("Characters");
        break;
    default:
        break;
    }
    return NULL;
}

QString CfgConfig_System::getSystemAlarmObjValStr(quint32 ID, quint32 /*group*/, ConfigObject *cfg)
{
    switch (ID) {
    case ID_System_Alarm_Ind_Value:
        return cfg->getData().toInt() == 0 ? tr("Nonhold") : tr("Hold");
        break;
    case ID_System_Alarm_ACK_Switch:
        return cfg->getData().toInt() == 0 ? tr("Off") : tr("On");
        break;
    default:
        break;
    }
    return cfg->getData().toString();
}

QString CfgConfig_System::getSystemTimeObjValStr(quint32 /*ID*/, quint32 /*group*/, ConfigObject *cfg)
{
    return cfg->getData().toString();
}

QString CfgConfig_System::getSystemSndObjValStr(quint32 ID, quint32 /*group*/, ConfigObject *cfg)
{
    switch (ID) {
    case ID_System_Snd_Snd_Touch:
    case ID_System_Snd_Snd_Warning:
        return cfg->getData().toInt() == 0 ? tr("Off") : tr("On");
        break;
    case ID_System_Snd_LED_LED:
        return cfg->getData().toInt() == 0 ? tr("Off") : tr("Alarm");
        break;
    default:
        break;
    }
    return NULL;
}


QString CfgConfig_System::getSystemRelayObjValStr(quint32 ID, quint32 /*group*/, ConfigObject *cfg)
{
    switch (ID) {
    case ID_System_Relay_OutputMode:
        return cfg->getData().toInt() == 0 ? tr("Fail") : tr("Status");
        break;
    case ID_System_Relay_MemMediaStatus:
    case ID_System_Relay_Measure:
    case ID_System_Relay_Comm:
    case ID_System_Relay_Record:
    case ID_System_Relay_Alarm:
        return cfg->getData().toInt() == 0 ? tr("Off") : tr("On");
        break;
    default:
        break;
    }
    return NULL;
}

QString CfgConfig_System::getSystemSwitchObjValStr(quint32 ID, quint32 /*group*/, ConfigObject *cfg)
{
    switch (ID) {
    case ID_System_Switch_Switch_Mode:
        return cfg->getData().toInt() == 0 ? tr("Alarm") : tr("Manual");
        break;
    case ID_System_Switch_Switch_Operate:
        return cfg->getData().toInt() == 0 ? tr("AND") : tr("OR");
        break;
    default:
        break;
    }
    return NULL;
}


QString CfgConfig_System::getSystemPrintObjValStr(quint32 ID, quint32 /*group*/, ConfigObject *cfg)
{
    switch (ID) {
    case ID_System_Printer_Size:
    {
        int type = cfg->getData().toInt();
        if (type == Printer_Paper_Size_A4)
            return "A4";
        else if (type == Printer_Paper_Size_A3)
            return "A3";
        else
            return tr("Letter");
    }
        break;
    case ID_System_Printer_Orientation:
        return cfg->getData().toInt() == 0 ? tr("Vertical") : tr("Horizontal");
        break;
    case ID_System_Printer_DPI:
        return cfg->getData().toInt() == 0 ? tr("300 dpi") : tr("600 dpi");
        break;
    default:
        break;
    }
    return cfg->getData().toString();
}

QString CfgConfig_System::getSystemDevAndComObjValStr(quint32 /*ID*/, quint32 /*group*/, ConfigObject *cfg)
{
    return cfg->getData().toString();
}


QString CfgConfig_System::getSystemEnvLagObjValStr(quint32 type)
{
    switch (type) {
    case Language_Type_English:
        return tr("English");
        break;
    case Language_Type_Japanese:
        return tr("Japanese");
        break;
    case Language_Type_German:
        return tr("German");
        break;
    case Language_Type_French:
        return tr("French");
        break;
    case Language_Type_Russian:
        return tr("Russian");
        break;
    case Language_Type_Chinese:
        return tr("Chinese");
        break;
    case Language_Type_Korean:
        return tr("Korean");
        break;
    default:
        break;
    }
    return NULL;
}






void CfgConfig_System::setSystemEnvData(quint32 ID, quint32 /*group*/)
{
    switch (ID) {
    case ID_System_Env_Language:
        p_envConfig->languageType = (Language_Type)ConfigTreeObject::getConfigData(QString(System_Setting_Environment_Language)).toInt();
        break;
    case ID_System_Env_Temp:
        p_envConfig->temperatureUnit = (Temperature_Unit)ConfigTreeObject::getConfigData(QString(System_Setting_Environment_Temperature)).toInt();
        break;
    case ID_System_Env_PointType:
        p_envConfig->decimalPointType = (Decimal_Point_Type)ConfigTreeObject::getConfigData(QString(System_Setting_Environment_Decimal)).toInt();
        break;

    case ID_System_Env_Date_Format:
        p_envConfig->dateFormat = (Date_Format)ConfigTreeObject::getConfigData(QString(System_Setting_Environment_Date_Data)).toInt();
        break;
    case ID_System_Env_Date_delimiter:
        p_envConfig->delimiterType = (Delimiter_Type)ConfigTreeObject::getConfigData(QString(System_Setting_Environment_Date_Delimiter)).toInt();
        break;
    case ID_System_Env_Date_Mon:
        p_envConfig->monthIndicator = (Month_Indicator)ConfigTreeObject::getConfigData(QString(System_Setting_Environment_Date_Month)).toInt();
        break;
    default:
        break;
    }
}

void CfgConfig_System::setSystemAlarmData(quint32 ID, quint32 /*group*/)
{
    switch (ID) {
    case ID_System_Alarm_Rate_Dec:
        p_envConfig->rateDecNum = ConfigTreeObject::getConfigData(QString(System_Setting_Alarm_Rate_Decrease)).toInt();
        break;

    case ID_System_Alarm_Rate_Inc:
        p_envConfig->rateIncNum = ConfigTreeObject::getConfigData(QString(System_Setting_Alarm_Rate_Increase)).toInt();
        break;
    case ID_System_Alarm_Ind_Value:
        p_envConfig->alramIndicator = (Alarm_Indicator)ConfigTreeObject::getConfigData(QString(System_Setting_Alarm_Indicator)).toInt();
        break;
    case ID_System_Alarm_ACK_Switch:
        p_envConfig->individualAlarmAck = ConfigTreeObject::getConfigData(QString(System_Setting_Alarm_AlarmACK)).toInt();
        break;
    default:
        break;
    }
}

void CfgConfig_System::setSystemTimeData(quint32 ID, quint32 /*group*/)
{
    switch (ID) {
    case ID_System_Time_H:
        p_envConfig->diffGMTHour = ConfigTreeObject::getConfigData(QString(System_Setting_Time_Hour)).toInt() - 13;
        break;
    case ID_System_Time_m:
        p_envConfig->diffGMTMin = ConfigTreeObject::getConfigData(QString(System_Setting_Time_Minute)).toInt();
        break;
    default:
        break;
    }
}

void CfgConfig_System::setSystemSndData(quint32 ID, quint32 /*group*/)
{
    switch (ID) {
    case ID_System_Snd_Snd_Touch:
        p_envConfig->touchSnd  = ConfigTreeObject::getConfigData(QString(System_Setting_Sound_Touch)).toInt();
        break;
    case ID_System_Snd_Snd_Warning:
        p_envConfig->warningSnd  = ConfigTreeObject::getConfigData(QString(System_Setting_Sound_Warning)).toInt();
        break;
    case ID_System_Snd_LED_LED:
        p_envConfig->menuKeyLED = ConfigTreeObject::getConfigData(QString(System_Setting_Sound_LED)).toInt();
        break;
    default:
        break;
    }
}

void CfgConfig_System::setSystemRelayData(quint32 ID, quint32 /*group*/)
{
    switch (ID) {
    case ID_System_Relay_OutputMode:
        p_envConfig->relayOutMode = (Relay_Output_Mode)ConfigTreeObject::getConfigData(QString(System_Setting_Relay_Fail)).toInt();
        break;
    case ID_System_Relay_MemMediaStatus:
        p_envConfig->memMediaStatus = ConfigTreeObject::getConfigData(QString(System_Setting_Relay_status)).toInt();
        break;
    case ID_System_Relay_Measure:
        p_envConfig->measureError = ConfigTreeObject::getConfigData(QString(System_Setting_Relay_Measurement)).toInt();
        break;
    case ID_System_Relay_Comm:
        p_envConfig->commError = ConfigTreeObject::getConfigData(QString(System_Setting_Relay_Communication)).toInt();
        break;
    case ID_System_Relay_Record:
        p_envConfig->recordStop = ConfigTreeObject::getConfigData(QString(System_Setting_Relay_Record)).toInt();
        break;
    case ID_System_Relay_Alarm:
        p_envConfig->alarm = ConfigTreeObject::getConfigData(QString(System_Setting_Relay_Record)).toInt();
        break;
    default:
        break;
    }
}

void CfgConfig_System::setSystemSwitchData(quint32 ID, quint32 group)
{
    switch (ID) {
    case ID_System_Switch_Switch_Mode:
        p_envConfig->interSwitchMode[group] = (InterSwitch_Mode)ConfigTreeObject::getConfigData(QString(System_Setting_Internal_Type).arg(group)).toInt();
        break;
    case ID_System_Switch_Switch_Operate:
        p_envConfig->interSwitchOperate[group] = (InterSwitch_Operate)ConfigTreeObject::getConfigData(QString(System_Setting_Internal_AndOr).arg(group)).toInt();
        break;
    default:
        break;
    }
}

void CfgConfig_System::setSystemPrintData(quint32 ID, quint32 /*group*/)
{
    switch (ID) {
    case ID_System_Printer_IP:
    {
        QString IPStr = ConfigTreeObject::getConfigData(QString(System_Setting_Printer_IP)).toString();
        QStringList str = IPStr.split(".");
        for (int i = 0; i < str.size(); i++)
            p_envConfig->printerIP[i] = str[i].toInt();
    }
        break;
    case ID_System_Printer_Size:
        p_envConfig->printerPaperSize = (Printer_Paper_Size)ConfigTreeObject::getConfigData(QString(System_Setting_Printer_Paper)).toInt();
        break;
    case ID_System_Printer_Orientation:
        p_envConfig->printerOrientation = (Printer_Orientation)ConfigTreeObject::getConfigData(QString(System_Setting_Printer_Page)).toInt();
        break;
    case ID_System_Printer_DPI:
        p_envConfig->printerDPI = (Printer_Resolution_DPI)ConfigTreeObject::getConfigData(QString(System_Setting_Printer_Resolution)).toInt();
        break;
    case ID_System_Printer_Num:
        p_envConfig->pageNumber = ConfigTreeObject::getConfigData(QString(System_Setting_Printer_Number)).toInt();
        break;
    default:
        break;
    }
}

void CfgConfig_System::setSystemDevAndComData(quint32 ID, quint32 /*group*/)
{
    switch (ID) {
    case ID_System_Device_Tag:
        setStrToChar(p_envConfig->deviceTag, ConfigTreeObject::getConfigData(QString(System_Setting_Instrument_tag)).toString(), 32);
        break;
    case ID_System_Device_TagNo:
        setStrToChar(p_envConfig->deviceTagNo, ConfigTreeObject::getConfigData(QString(System_Setting_Instrument_No)).toString(), 16);
        break;
    case ID_System_Comment_Comment:
        setStrToChar(p_envConfig->settingComment, ConfigTreeObject::getConfigData(QString(System_Setting_File_File)).toString(), 52);
        break;
    default:
        break;
    }
}




/*
 * 把uchar *类型转化为QString类型
*/
QVariant CfgConfig_System::getCharToString(uchar *str)
{
//    char * valueChar = NULL;
//    while(*str != '\0')
//        *valueChar++ = *str++;
//    QString data(valueChar);
//    return tr(data.toLatin1());
    QString data((char *)str);
    return data;
}

//把QString类型转化为uchar *类型
void CfgConfig_System::setStrToChar(uchar * uData, QString str, int len)
{
    memset(uData, 0, len);
    QByteArray baUnit = str.toLocal8Bit();
    char* valueUnit = baUnit.data();
//    memcpy(uData, valueUnit, strlen(valueUnit));
    while (*valueUnit != '\0') {
        *uData++ = *valueUnit++;
    }
}


