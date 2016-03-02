#include "cfgconfig_timer.h"
#include "./GlobalData/configtreeobject.h"

static QString monthList[12] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};


CfgConfig_Timer *CfgConfig_Timer::instance()
{
    static CfgConfig_Timer data;
    return &data;
}



CfgConfig_Timer::CfgConfig_Timer(QObject *parent) :
    QObject(parent)
{
}




QVariant CfgConfig_Timer::getTimerData(quint32 ID, quint32 group)
{
    if (ID >= Timer_Timer_Start_ID && ID <= Timer_Timer_End_ID)
        return getTimerTimerData(ID, group);
    else
        return getTimerMatchData(ID, group);
}


QString CfgConfig_Timer::getTimerObjStr(quint32 ID, quint32 group)
{
    if (ID >= Timer_Timer_Start_ID && ID <= Timer_Timer_End_ID)
        return getTimerTimerObjStr(ID, group);
    else
        return getTimerMatchObjStr(ID, group);
}

QString CfgConfig_Timer::getTimerObjValStr(quint32 ID, quint32 group, ConfigObject *cfg)
{
    if (ID >= Timer_Timer_Start_ID && ID <= Timer_Timer_End_ID)
        return getTimerTimerObjValStr(ID, group, cfg);
    else
        return getTimerMatchObjValStr(ID, group, cfg);
}

void CfgConfig_Timer::setTimerData(quint32 ID, quint32 group)
{
    if (ID >= Timer_Timer_Start_ID && ID <= Timer_Timer_End_ID)
        setTimerTimerData(ID, group);
    else
        setTimerMatchData(ID, group);
}




QVariant CfgConfig_Timer::getTimerTimerData(quint32 ID, quint32 /*group*/)
{
    switch (ID) {
    case ID_Timer_Timer_1_Type_Type:
        return p_timerConfig->timerType[0];
        break;
    case ID_Timer_Timer_1_Interval_D:
        return p_timerConfig->timerDay[0];
        break;
    case ID_Timer_Timer_1_Interval_H:
        return p_timerConfig->timerHour[0];
        break;
    case ID_Timer_Timer_1_Interval_M:
        return p_timerConfig->timerMin[0];
        break;
    case ID_Timer_Timer_1_Interval_Interval:
        return p_timerConfig->timerInter[0];
        break;

    case ID_Timer_Timer_2_Type_Type:
        return p_timerConfig->timerType[1];
        break;
    case ID_Timer_Timer_2_Interval_D:
        return p_timerConfig->timerDay[1];
        break;
    case ID_Timer_Timer_2_Interval_H:
        return p_timerConfig->timerHour[1];
        break;
    case ID_Timer_Timer_2_Interval_M:
        return p_timerConfig->timerMin[1];
        break;
    case ID_Timer_Timer_2_Interval_Interval:
        return p_timerConfig->timerInter[1];
        break;

    case ID_Timer_Timer_3_Type_Type:
        return p_timerConfig->timerType[2];
        break;
    case ID_Timer_Timer_3_Interval_D:
        return p_timerConfig->timerDay[2];
        break;
    case ID_Timer_Timer_3_Interval_H:
        return p_timerConfig->timerHour[2];
        break;
    case ID_Timer_Timer_3_Interval_M:
        return p_timerConfig->timerMin[2];
        break;
    case ID_Timer_Timer_3_Interval_Interval:
        return p_timerConfig->timerInter[2];
        break;

    case ID_Timer_Timer_4_Type_Type:
        return p_timerConfig->timerType[3];
        break;
    case ID_Timer_Timer_4_Interval_D:
        return p_timerConfig->timerDay[3];
        break;
    case ID_Timer_Timer_4_Interval_H:
        return p_timerConfig->timerHour[3];
        break;
    case ID_Timer_Timer_4_Interval_M:
        return p_timerConfig->timerMin[3];
        break;
    case ID_Timer_Timer_4_Interval_Interval:
        return p_timerConfig->timerInter[3];
        break;
    default:
        break;
    }
    return QVariant();
}

QVariant CfgConfig_Timer::getTimerMatchData(quint32 ID, quint32 /*group*/)
{
    switch (ID) {
    case ID_Timer_Match_1_Type_Type:
        return p_timerConfig->timerMatchType[0];
        break;
    case ID_Timer_Match_1_Condition_M:
        return p_timerConfig->timerMatchMonth[0];
        break;
    case ID_Timer_Match_1_Condition_D:
        return p_timerConfig->timerMatchDay[0];
        break;
    case ID_Timer_Match_1_Condition_W:
        return p_timerConfig->timerMatchWeekDay[0];
        break;
    case ID_Timer_Match_1_Condition_H:
        return p_timerConfig->timerMatchHour[0];
        break;
    case ID_Timer_Match_1_Condition_Min:
        return p_timerConfig->timerMatchMinute[0];
        break;
    case ID_Timer_Match_1_Action:
        return p_timerConfig->timerMatchAction[0];
        break;

    case ID_Timer_Match_2_Type_Type:
        return p_timerConfig->timerMatchType[1];
        break;
    case ID_Timer_Match_2_Condition_M:
        return p_timerConfig->timerMatchMonth[1];
        break;
    case ID_Timer_Match_2_Condition_D:
        return p_timerConfig->timerMatchDay[1];
        break;
    case ID_Timer_Match_2_Condition_W:
        return p_timerConfig->timerMatchWeekDay[1];
        break;
    case ID_Timer_Match_2_Condition_H:
        return p_timerConfig->timerMatchHour[1];
        break;
    case ID_Timer_Match_2_Condition_Min:
        return p_timerConfig->timerMatchMinute[1];
        break;
    case ID_Timer_Match_2_Action:
        return p_timerConfig->timerMatchAction[1];
        break;

    case ID_Timer_Match_3_Type_Type:
        return p_timerConfig->timerMatchType[2];
        break;
    case ID_Timer_Match_3_Condition_M:
        return p_timerConfig->timerMatchMonth[2];
        break;
    case ID_Timer_Match_3_Condition_D:
        return p_timerConfig->timerMatchDay[2];
        break;
    case ID_Timer_Match_3_Condition_W:
        return p_timerConfig->timerMatchWeekDay[2];
        break;
    case ID_Timer_Match_3_Condition_H:
        return p_timerConfig->timerMatchHour[2];
        break;
    case ID_Timer_Match_3_Condition_Min:
        return p_timerConfig->timerMatchMinute[2];
        break;
    case ID_Timer_Match_3_Action:
        return p_timerConfig->timerMatchAction[2];
        break;

    case ID_Timer_Match_4_Type_Type:
        return p_timerConfig->timerMatchType[3];
        break;
    case ID_Timer_Match_4_Condition_M:
        return p_timerConfig->timerMatchMonth[3];
        break;
    case ID_Timer_Match_4_Condition_D:
        return p_timerConfig->timerMatchDay[3];
        break;
    case ID_Timer_Match_4_Condition_W:
        return p_timerConfig->timerMatchWeekDay[3];
        break;
    case ID_Timer_Match_4_Condition_H:
        return p_timerConfig->timerMatchHour[3];
        break;
    case ID_Timer_Match_4_Condition_Min:
        return p_timerConfig->timerMatchMinute[3];
        break;
    case ID_Timer_Match_4_Action:
        return p_timerConfig->timerMatchAction[3];
        break;

    default:
        break;
    }
    return QVariant();
}








QString CfgConfig_Timer::getTimerTimerObjStr(quint32 ID, quint32 /*group*/)
{
    switch (ID) {
    case ID_Timer:
        return tr("Timer settings");
        break;
    case ID_Timer_Timer:
        return tr("Timer");
        break;
    case ID_Timer_Timer_Referencetime:
        return tr("Reference time");
        break;

    case ID_Timer_Timer_1:
        return tr("Timer 1");
        break;
    case ID_Timer_Timer_2:
        return tr("Timer 2");
        break;
    case ID_Timer_Timer_3:
        return tr("Timer 3");
        break;
    case ID_Timer_Timer_4:
        return tr("Timer 4");
        break;
    case ID_Timer_Timer_1_Type:
    case ID_Timer_Timer_1_Type_Type:
    case ID_Timer_Timer_2_Type:
    case ID_Timer_Timer_2_Type_Type:
    case ID_Timer_Timer_3_Type:
    case ID_Timer_Timer_3_Type_Type:
    case ID_Timer_Timer_4_Type:
    case ID_Timer_Timer_4_Type_Type:
        return tr("Type");
        break;

    case ID_Timer_Timer_1_Interval:
    case ID_Timer_Timer_1_Interval_Interval:
    case ID_Timer_Timer_2_Interval:
    case ID_Timer_Timer_2_Interval_Interval:
    case ID_Timer_Timer_3_Interval:
    case ID_Timer_Timer_3_Interval_Interval:
    case ID_Timer_Timer_4_Interval:
    case ID_Timer_Timer_4_Interval_Interval:
        return tr("Interval");
        break;
    case ID_Timer_Timer_1_Interval_D:
    case ID_Timer_Timer_2_Interval_D:
    case ID_Timer_Timer_3_Interval_D:
    case ID_Timer_Timer_4_Interval_D:
        return tr("Day");
        break;
    case ID_Timer_Timer_1_Interval_H:
    case ID_Timer_Timer_2_Interval_H:
    case ID_Timer_Timer_3_Interval_H:
    case ID_Timer_Timer_4_Interval_H:
        return tr("Hour");
        break;
    case ID_Timer_Timer_1_Interval_M:
    case ID_Timer_Timer_2_Interval_M:
    case ID_Timer_Timer_3_Interval_M:
    case ID_Timer_Timer_4_Interval_M:
        return tr("Minute");
        break;

    default:
        break;
    }
    return NULL;
}

QString CfgConfig_Timer::getTimerMatchObjStr(quint32 ID, quint32 /*group*/)
{
    switch (ID) {
    case ID_Timer_Match:
        return tr("Match time timer");
        break;

    case ID_Timer_Match_1:
        return tr("Match time timer 1");
        break;
    case ID_Timer_Match_2:
        return tr("Match time timer 2");
        break;
    case ID_Timer_Match_3:
        return tr("Match time timer 3");
        break;
    case ID_Timer_Match_4:
        return tr("Match time timer 4");
        break;
    case ID_Timer_Match_1_Type:
    case ID_Timer_Match_1_Type_Type:
    case ID_Timer_Match_2_Type:
    case ID_Timer_Match_2_Type_Type:
    case ID_Timer_Match_3_Type:
    case ID_Timer_Match_3_Type_Type:
    case ID_Timer_Match_4_Type:
    case ID_Timer_Match_4_Type_Type:
        return tr("Type");
        break;
    case ID_Timer_Match_1_Condition:
    case ID_Timer_Match_2_Condition:
    case ID_Timer_Match_3_Condition:
    case ID_Timer_Match_4_Condition:
        return tr("Timer match condition");
        break;
    case ID_Timer_Match_1_Condition_M:
    case ID_Timer_Match_2_Condition_M:
    case ID_Timer_Match_3_Condition_M:
    case ID_Timer_Match_4_Condition_M:
        return tr("Month");
        break;
    case ID_Timer_Match_1_Condition_D:
    case ID_Timer_Match_2_Condition_D:
    case ID_Timer_Match_3_Condition_D:
    case ID_Timer_Match_4_Condition_D:
        return tr("Day");
        break;
    case ID_Timer_Match_1_Condition_W:
    case ID_Timer_Match_2_Condition_W:
    case ID_Timer_Match_3_Condition_W:
    case ID_Timer_Match_4_Condition_W:
        return tr("Day of week");
        break;
    case ID_Timer_Match_1_Condition_H:
    case ID_Timer_Match_2_Condition_H:
    case ID_Timer_Match_3_Condition_H:
    case ID_Timer_Match_4_Condition_H:
        return tr("Hour");
        break;
    case ID_Timer_Match_1_Condition_Min:
    case ID_Timer_Match_2_Condition_Min:
    case ID_Timer_Match_3_Condition_Min:
    case ID_Timer_Match_4_Condition_Min:
        return tr("Minute");
        break;
    case ID_Timer_Match_1_Action:
    case ID_Timer_Match_2_Action:
    case ID_Timer_Match_3_Action:
    case ID_Timer_Match_4_Action:
    case ID_Timer_Match_Action:
        return tr("Timer action");
        break;
    default:
        break;
    }
    return NULL;
}








QString CfgConfig_Timer::getTimerTimerObjValStr(quint32 ID, quint32 /*group*/, ConfigObject *cfg)
{
    switch (ID) {
    case ID_Timer_Timer_1_Type_Type:
    case ID_Timer_Timer_2_Type_Type:
    case ID_Timer_Timer_3_Type_Type:
    case ID_Timer_Timer_4_Type_Type:
    {
        int type = cfg->getData().toInt();
        if (type == Timer_Type_Off)
            return tr("Off");
        else if (type == Timer_Type_Relative)
            return tr("Relative timer");
        else if (type == Timer_Type_Absolute)
            return tr("Absolute timer");
    }
        break;
    case ID_Timer_Timer_1_Interval_Interval:
    case ID_Timer_Timer_2_Interval_Interval:
    case ID_Timer_Timer_3_Interval_Interval:
    case ID_Timer_Timer_4_Interval_Interval:
    {
        int data = cfg->getData().toInt();
        if (data >= 0 && data < 60)
            return QString("%1 %2").arg(data).arg(tr("min"));
        else if (data >= 60 && data <= 1440)
            return QString("%1 %2").arg(data / 60).arg(tr("h"));
    }
        break;
    default:
        break;
    }
    return cfg->getData().toString();
}

QString CfgConfig_Timer::getTimerMatchObjValStr(quint32 ID, quint32 /*group*/, ConfigObject *cfg)
{
    switch (ID) {
    case ID_Timer_Match_1_Type_Type:
    case ID_Timer_Match_2_Type_Type:
    case ID_Timer_Match_3_Type_Type:
    case ID_Timer_Match_4_Type_Type:
    {
        int type = cfg->getData().toInt();
        if (type == Timer_Match_Type_Off)
            return tr("Off");
        else if (type == Timer_Match_Type_Daily)
            return tr("Day");
        else if (type == Timer_Match_Type_Weekly)
            return tr("Week");
        else if (type == Timer_Match_Type_Monthly)
            return tr("Month");
        else if (type == Timer_Match_Type_Yearly)
            return tr("Year");
    }
        break;
    case ID_Timer_Match_1_Condition_M:
    case ID_Timer_Match_2_Condition_M:
    case ID_Timer_Match_3_Condition_M:
    case ID_Timer_Match_4_Condition_M:
    {
        if (cfg->getData().toUInt() <= 12)
            return QString(tr("%1").arg(monthList[cfg->getData().toInt() - 1]));
    }
        break;

    case ID_Timer_Match_1_Condition_W:
    case ID_Timer_Match_2_Condition_W:
    case ID_Timer_Match_3_Condition_W:
    case ID_Timer_Match_4_Condition_W:
        return getTimerWeekObjValStr(cfg->getData().toInt());
        break;

    case ID_Timer_Match_1_Action:
    case ID_Timer_Match_2_Action:
    case ID_Timer_Match_3_Action:
    case ID_Timer_Match_4_Action:
        return cfg->getData().toInt() == 0 ? tr("Single") : tr("Repeat");
        break;

    default:
        break;
    }
    return cfg->getData().toString();
}


QString CfgConfig_Timer::getTimerWeekObjValStr(int type)
{
    switch (type) {
    case 0:
        return tr("Sunday");
        break;
    case 1:
        return tr("Monday");
        break;
    case 2:
        return tr("Tuesday");
        break;
    case 3:
        return tr("Wednesday");
        break;
    case 4:
        return tr("Thursday");
        break;
    case 5:
        return tr("Friday");
        break;
    case 6:
        return tr("Satuary");
        break;
    default:
        break;
    }
    return NULL;
}








void CfgConfig_Timer::setTimerTimerData(quint32 ID, quint32 /*group*/)
{
    switch (ID) {
    case ID_Timer_Timer_1_Type_Type:
        p_timerConfig->timerType[0] = (Timer_Type)ConfigTreeObject::getConfigData(Config_Timer_Setting_Timer1_Type).toInt();
        break;
    case ID_Timer_Timer_1_Interval_D:
        p_timerConfig->timerDay[0] = ConfigTreeObject::getConfigData(Config_Timer_Setting_Timer1_Day).toInt();
        break;
    case ID_Timer_Timer_1_Interval_H:
        p_timerConfig->timerHour[0] = ConfigTreeObject::getConfigData(Config_Timer_Setting_Timer1_Hour).toInt();
        break;
    case ID_Timer_Timer_1_Interval_M:
        p_timerConfig->timerMin[0] = ConfigTreeObject::getConfigData(Config_Timer_Setting_Timer1_Minute).toInt();
        break;
    case ID_Timer_Timer_1_Interval_Interval:
        p_timerConfig->timerInter[0] = ConfigTreeObject::getConfigData(Config_Timer_Setting_Timer1_Interval).toInt();
        break;

    case ID_Timer_Timer_2_Type_Type:
        p_timerConfig->timerType[1] = (Timer_Type)ConfigTreeObject::getConfigData(Config_Timer_Setting_Timer2_Type).toInt();
        break;
    case ID_Timer_Timer_2_Interval_D:
        p_timerConfig->timerDay[1] = ConfigTreeObject::getConfigData(Config_Timer_Setting_Timer2_Day).toInt();
        break;
    case ID_Timer_Timer_2_Interval_H:
        p_timerConfig->timerHour[1] = ConfigTreeObject::getConfigData(Config_Timer_Setting_Timer2_Hour).toInt();
        break;
    case ID_Timer_Timer_2_Interval_M:
        p_timerConfig->timerMin[1] = ConfigTreeObject::getConfigData(Config_Timer_Setting_Timer2_Minute).toInt();
        break;
    case ID_Timer_Timer_2_Interval_Interval:
        p_timerConfig->timerInter[1] = ConfigTreeObject::getConfigData(Config_Timer_Setting_Timer2_Interval).toInt();
        break;

    case ID_Timer_Timer_3_Type_Type:
        p_timerConfig->timerType[2] = (Timer_Type)ConfigTreeObject::getConfigData(Config_Timer_Setting_Timer3_Type).toInt();
        break;
    case ID_Timer_Timer_3_Interval_D:
        p_timerConfig->timerDay[2] = ConfigTreeObject::getConfigData(Config_Timer_Setting_Timer3_Day).toInt();
        break;
    case ID_Timer_Timer_3_Interval_H:
        p_timerConfig->timerHour[2] = ConfigTreeObject::getConfigData(Config_Timer_Setting_Timer3_Hour).toInt();
        break;
    case ID_Timer_Timer_3_Interval_M:
        p_timerConfig->timerMin[2] = ConfigTreeObject::getConfigData(Config_Timer_Setting_Timer3_Minute).toInt();
        break;
    case ID_Timer_Timer_3_Interval_Interval:
        p_timerConfig->timerInter[2] = ConfigTreeObject::getConfigData(Config_Timer_Setting_Timer3_Interval).toInt();
        break;

    case ID_Timer_Timer_4_Type_Type:
        p_timerConfig->timerType[3] = (Timer_Type)ConfigTreeObject::getConfigData(Config_Timer_Setting_Timer4_Type).toInt();
        break;
    case ID_Timer_Timer_4_Interval_D:
        p_timerConfig->timerDay[3] = ConfigTreeObject::getConfigData(Config_Timer_Setting_Timer4_Day).toInt();
        break;
    case ID_Timer_Timer_4_Interval_H:
        p_timerConfig->timerHour[3] = ConfigTreeObject::getConfigData(Config_Timer_Setting_Timer4_Hour).toInt();
        break;
    case ID_Timer_Timer_4_Interval_M:
        p_timerConfig->timerMin[3] = ConfigTreeObject::getConfigData(Config_Timer_Setting_Timer4_Minute).toInt();
        break;
    case ID_Timer_Timer_4_Interval_Interval:
        p_timerConfig->timerInter[3] = ConfigTreeObject::getConfigData(Config_Timer_Setting_Timer4_Interval).toInt();
        break;

    default:
        break;
    }
}

void CfgConfig_Timer::setTimerMatchData(quint32 ID, quint32 /*group*/)
{
    switch (ID) {
    case ID_Timer_Match_1_Type_Type:
        p_timerConfig->timerMatchType[0] = (Timer_Match_Type)ConfigTreeObject::getConfigData(Config_Timer_Setting_Match_Timer1_Type).toInt();
        break;
    case ID_Timer_Match_1_Condition_M:
        p_timerConfig->timerMatchMonth[0] = ConfigTreeObject::getConfigData(Config_Timer_Setting_Match_Timer1_Month).toInt();
        break;
    case ID_Timer_Match_1_Condition_D:
        p_timerConfig->timerMatchDay[0] = ConfigTreeObject::getConfigData(Config_Timer_Setting_Match_Timer1_Day).toInt();
        break;
    case ID_Timer_Match_1_Condition_W:
        p_timerConfig->timerMatchWeekDay[0] = ConfigTreeObject::getConfigData(Config_Timer_Setting_Match_Timer1_Week).toInt();
        break;
    case ID_Timer_Match_1_Condition_H:
        p_timerConfig->timerMatchHour[0] = ConfigTreeObject::getConfigData(Config_Timer_Setting_Match_Timer1_Hour).toInt();
        break;
    case ID_Timer_Match_1_Condition_Min:
        p_timerConfig->timerMatchMinute[0] = ConfigTreeObject::getConfigData(Config_Timer_Setting_Match_Timer1_Minute).toInt();
        break;
    case ID_Timer_Match_1_Action:
        p_timerConfig->timerMatchAction[0] = ConfigTreeObject::getConfigData(Config_Timer_Setting_Match_Timer1_Action).toInt();

    case ID_Timer_Match_2_Type_Type:
        p_timerConfig->timerMatchType[1] = (Timer_Match_Type)ConfigTreeObject::getConfigData(Config_Timer_Setting_Match_Timer2_Type).toInt();
        break;
    case ID_Timer_Match_2_Condition_M:
        p_timerConfig->timerMatchMonth[1] = ConfigTreeObject::getConfigData(Config_Timer_Setting_Match_Timer2_Month).toInt();
        break;
    case ID_Timer_Match_2_Condition_D:
        p_timerConfig->timerMatchDay[1] = ConfigTreeObject::getConfigData(Config_Timer_Setting_Match_Timer2_Day).toInt();
        break;
    case ID_Timer_Match_2_Condition_W:
        p_timerConfig->timerMatchWeekDay[1] = ConfigTreeObject::getConfigData(Config_Timer_Setting_Match_Timer2_Week).toInt();
        break;
    case ID_Timer_Match_2_Condition_H:
        p_timerConfig->timerMatchHour[1] = ConfigTreeObject::getConfigData(Config_Timer_Setting_Match_Timer2_Hour).toInt();
        break;
    case ID_Timer_Match_2_Condition_Min:
        p_timerConfig->timerMatchMinute[1] = ConfigTreeObject::getConfigData(Config_Timer_Setting_Match_Timer2_Minute).toInt();
        break;
    case ID_Timer_Match_2_Action:
        p_timerConfig->timerMatchAction[2] = ConfigTreeObject::getConfigData(Config_Timer_Setting_Match_Timer2_Action).toInt();

    case ID_Timer_Match_3_Type_Type:
        p_timerConfig->timerMatchType[2] = (Timer_Match_Type)ConfigTreeObject::getConfigData(Config_Timer_Setting_Match_Timer3_Type).toInt();
        break;
    case ID_Timer_Match_3_Condition_M:
        p_timerConfig->timerMatchMonth[2] = ConfigTreeObject::getConfigData(Config_Timer_Setting_Match_Timer3_Month).toInt();
        break;
    case ID_Timer_Match_3_Condition_D:
        p_timerConfig->timerMatchDay[2] = ConfigTreeObject::getConfigData(Config_Timer_Setting_Match_Timer3_Day).toInt();
        break;
    case ID_Timer_Match_3_Condition_W:
        p_timerConfig->timerMatchWeekDay[2] = ConfigTreeObject::getConfigData(Config_Timer_Setting_Match_Timer3_Week).toInt();
        break;
    case ID_Timer_Match_3_Condition_H:
        p_timerConfig->timerMatchHour[2] = ConfigTreeObject::getConfigData(Config_Timer_Setting_Match_Timer3_Hour).toInt();
        break;
    case ID_Timer_Match_3_Condition_Min:
        p_timerConfig->timerMatchMinute[2] = ConfigTreeObject::getConfigData(Config_Timer_Setting_Match_Timer3_Minute).toInt();
        break;
    case ID_Timer_Match_3_Action:
        p_timerConfig->timerMatchAction[2] = ConfigTreeObject::getConfigData(Config_Timer_Setting_Match_Timer3_Action).toInt();

    case ID_Timer_Match_4_Type_Type:
        p_timerConfig->timerMatchType[3] = (Timer_Match_Type)ConfigTreeObject::getConfigData(Config_Timer_Setting_Match_Timer4_Type).toInt();
        break;
    case ID_Timer_Match_4_Condition_M:
        p_timerConfig->timerMatchMonth[3] = ConfigTreeObject::getConfigData(Config_Timer_Setting_Match_Timer4_Month).toInt();
        break;
    case ID_Timer_Match_4_Condition_D:
        p_timerConfig->timerMatchDay[3] = ConfigTreeObject::getConfigData(Config_Timer_Setting_Match_Timer4_Day).toInt();
        break;
    case ID_Timer_Match_4_Condition_W:
        p_timerConfig->timerMatchWeekDay[3] = ConfigTreeObject::getConfigData(Config_Timer_Setting_Match_Timer4_Week).toInt();
        break;
    case ID_Timer_Match_4_Condition_H:
        p_timerConfig->timerMatchHour[3] = ConfigTreeObject::getConfigData(Config_Timer_Setting_Match_Timer4_Hour).toInt();
        break;
    case ID_Timer_Match_4_Condition_Min:
        p_timerConfig->timerMatchMinute[3] = ConfigTreeObject::getConfigData(Config_Timer_Setting_Match_Timer4_Minute).toInt();
        break;
    case ID_Timer_Match_4_Action:
        p_timerConfig->timerMatchAction[3] = ConfigTreeObject::getConfigData(Config_Timer_Setting_Match_Timer4_Action).toInt();
        break;
    default:
        break;
    }
}

