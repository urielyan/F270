#include "cfgconfig_display.h"
#include <QVariant>

CfgConfig_Display *CfgConfig_Display::instance()
{
    static CfgConfig_Display data;
    return &data;
}



CfgConfig_Display::CfgConfig_Display(QObject *parent) :
    QObject(parent)
{
}



QVariant CfgConfig_Display::getDispData(quint32 ID, quint32 group)
{
    if (ID >= Disp_Trend_Start_ID && ID <= Disp_Trend_End_ID)
        return getDispTrendData(ID, group);
    else if (ID >= Disp_Grp_Start_ID && ID <= Disp_Grp_End_ID)
        return getDispGrpData(ID, group);
    else if (ID >= Disp_Baisc_Start_ID && ID <= Disp_Baisc_End_ID)
        return getDispBasicData(ID, group);
    else
        return getDispMsgData(ID, group);
}

QString CfgConfig_Display::getDispObjStr(quint32 ID, quint32 group)
{
    if (ID >= Disp_Trend_Start_ID && ID <= Disp_Trend_End_ID)
        return getDispTrendObjStr(ID, group);
    else if (ID >= Disp_Grp_Start_ID && ID <= Disp_Grp_End_ID)
        return getDispGrpObjStr(ID, group);
    else if (ID >= Disp_Baisc_Start_ID && ID <= Disp_Baisc_End_ID)
        return getDispBasicObjStr(ID, group);
    else
        return getDispMsgObjStr(ID, group);
}

QString CfgConfig_Display::getDispObjValStr(quint32 ID, quint32 group, ConfigObject *cfg)
{
    if (ID >= Disp_Trend_Start_ID && ID <= Disp_Trend_End_ID)
        return getDispTrendObjValStr(ID, group, cfg);
    else if (ID >= Disp_Grp_Start_ID && ID <= Disp_Grp_End_ID)
        return getDispGrpObjValStr(ID, group, cfg);
    else if (ID >= Disp_Baisc_Start_ID && ID <= Disp_Baisc_End_ID)
        return getDispBasicObjValStr(ID, group, cfg);
    else
        return getDispMsgObjValStr(ID, group, cfg);
}

void CfgConfig_Display::setDispData(quint32 ID, quint32 group)
{
    if (ID >= Disp_Trend_Start_ID && ID <= Disp_Trend_End_ID)
        setDispTrendData(ID, group);
    else if (ID >= Disp_Grp_Start_ID && ID <= Disp_Grp_End_ID)
        setDispGrpData(ID, group);
    else if (ID >= Disp_Baisc_Start_ID && ID <= Disp_Baisc_End_ID)
        setDispBasicData(ID, group);
    else
        setDispMsgData(ID, group);
}








QVariant CfgConfig_Display::getDispBasicData(quint32 ID, quint32 /*group*/)
{
    DISPLAY_GONFIG_BASIC basic = p_deviceConfig->displayBasic;
    switch (ID) {
    case ID_Disp_Basic_Bar_Direction:
        return basic.basicBarDirection;
        break;

    case ID_Disp_Basic_LCD_Brightness: //LCD显示辉度，1-6,默认:3
        return basic.basicLCDBrightness;
        break;
    case ID_Disp_Basic_Backlight_Mode: //LCD屏保模式，0-off, 1-变暗，2-熄灭
        return basic.basicBacklightSaverMode;
        break;
    case ID_Disp_Basic_Backlight_Time:      //LCD屏保等待时间，单位：min, 范围：1min/2min/5min/10min/30min/1h
        return getDispBasicBackTimeData(basic.basicBacklightSaverTime);
        break;
    case ID_Disp_Basic_Backlight_Restore:      //LCD屏保恢复模式，0-按键/触摸/报警， 1-按键/触摸，默认：0
        return basic.basicBacklightRestore;
        break;
    case ID_Disp_Basic_Background:      //UI背景色，0-白色，1-黑色，默认：0
        return basic.basicBackground;
        break;

    case ID_Disp_Basic_Changetime:      //组自动切换时间，单位：s,范围：5s/10s/20s/30s/60s(1min),默认10s
        return getDispBasicScrollTimeData(basic.basicGroupChangeTime);
        break;
    case ID_Disp_Basic_Wintime:      //自动默认画面跳转时间，单位:min,范围：0(off)/1/2/5/10/20/30/1h,默认：0
        return getDispBasicWinTimeData(basic.basicAutoHomeWinTime);
        break;

    case ID_Disp_Basic_Calendar_Week:      //周起始日，0-星期日，1-星期一
        return basic.basicFirstWeekday;
        break;

    case ID_Disp_Basic_Change_Switch:      //从监视器修改各值，false-off, true-on
        return basic.basicChangeFromMonitor;
        break;
    default:
        break;
    }
    return QVariant();
}


QVariant CfgConfig_Display::getDispGrpData(quint32 ID, quint32 group)
{
    DISPLAY_GONFIG_GROUP dispGroup = p_deviceConfig->displayGroup[group];
    switch (ID) {
    case ID_Disp_Grp_Number:
        return group + 1;
        break;
    case ID_Disp_Grp_Set_Switch:         //组开关，0-off, 1-on
        return dispGroup.groupSwitch;
        break;
    case ID_Disp_Grp_Set_CHNumber:       //组通道数，最多20个通道
        return dispGroup.groupChannelNum;
        break;
    case ID_Disp_Grp_Set_Name:       //组名，最多16个半角字符，中英文支持
        return getCharToString(dispGroup.groupName);
        break;
    case ID_Disp_Grp_Set_CHItem:      //组通道号，有效通道号由groupChannelNum决定
    {
        QList<QVariant> channelList;
        int cnt = dispGroup.groupChannelNum;
        for (int i = 0; i < cnt; i++)
            channelList.append(dispGroup.groupChannelItem[i]);
        return channelList;
    }
        break;

    case ID_Disp_Grp_Set_ScaleImage_Switch:       //组标尺图像开关，0-off, 1-on
        return dispGroup.groupScaleImage;
        break;

    case ID_Disp_Grp_Set_Scale1_Switch:      //分割线1-4开关，0-off, 1-on, 默认false
        return dispGroup.groupTripLineSwitch[0];
        break;
    case ID_Disp_Grp_Set_Scale1_Pos:       //分割线1-4位置，单位:%, 0-100,默认50
        return dispGroup.groupTripLinePos[0];
        break;
    case ID_Disp_Grp_Set_Scale1_Width:       //分割线1-4线宽，0-标准， 1-粗， 2-细， 默认0
        return dispGroup.groupTripLineWidth[0];
        break;
    case ID_Disp_Grp_Set_Scale1_Color:        //分割线1-4颜色值，BYTE2-Red, BYTE1-Green, BYTE0-Blue
        return dispGroup.groupTripLineColor[0];
        break;

    case ID_Disp_Grp_Set_Scale2_Switch:      //分割线1-4开关，0-off, 1-on, 默认false
        return dispGroup.groupTripLineSwitch[1];
        break;
    case ID_Disp_Grp_Set_Scale2_Pos:       //分割线1-4位置，单位:%, 0-100,默认50
        return dispGroup.groupTripLinePos[1];
        break;
    case ID_Disp_Grp_Set_Scale2_Width:       //分割线1-4线宽，0-标准， 1-粗， 2-细， 默认0
        return dispGroup.groupTripLineWidth[1];
        break;
    case ID_Disp_Grp_Set_Scale2_Color:        //分割线1-4颜色值，BYTE2-Red, BYTE1-Green, BYTE0-Blue
        return dispGroup.groupTripLineColor[1];
        break;

    case ID_Disp_Grp_Set_Scale3_Switch:      //分割线1-4开关，0-off, 1-on, 默认false
        return dispGroup.groupTripLineSwitch[2];
        break;
    case ID_Disp_Grp_Set_Scale3_Pos:       //分割线1-4位置，单位:%, 0-100,默认50
        return dispGroup.groupTripLinePos[2];
        break;
    case ID_Disp_Grp_Set_Scale3_Width:       //分割线1-4线宽，0-标准， 1-粗， 2-细， 默认0
        return dispGroup.groupTripLineWidth[2];
        break;
    case ID_Disp_Grp_Set_Scale3_Color:        //分割线1-4颜色值，BYTE2-Red, BYTE1-Green, BYTE0-Blue
        return dispGroup.groupTripLineColor[2];
        break;

    case ID_Disp_Grp_Set_Scale4_Switch:      //分割线1-4开关，0-off, 1-on, 默认false
        return dispGroup.groupTripLineSwitch[3];
        break;
    case ID_Disp_Grp_Set_Scale4_Pos:       //分割线1-4位置，单位:%, 0-100,默认50
        return dispGroup.groupTripLinePos[3];
        break;
    case ID_Disp_Grp_Set_Scale4_Width:       //分割线1-4线宽，0-标准， 1-粗， 2-细， 默认0
        return dispGroup.groupTripLineWidth[3];
        break;
    case ID_Disp_Grp_Set_Scale4_Color:        //分割线1-4颜色值，BYTE2-Red, BYTE1-Green, BYTE0-Blue
        return dispGroup.groupTripLineColor[3];
        break;

    default:
        break;
    }
    return QVariant();
}

QVariant CfgConfig_Display::getDispMsgData(quint32 ID, quint32 /*group*/)
{
    DISPLAY_GONFIG_PRE_MESSAGE msg = p_deviceConfig->displayMessage;
    int id = ID % 100;
    return getCharToString(msg.preMessage[id]);
}

QVariant CfgConfig_Display::getDispTrendData(quint32 ID, quint32 /*group*/)
{
    DISPLAY_GONFIG_TREND & trend = p_deviceConfig->displayTrend;
    switch (ID) {
    case ID_Disp_Trend_Interval:    //趋势周期，最小值依赖于测量周期或事件记录周期
        return trend.trendInterval_s;
        break;
    case ID_Disp_Trend_SecondInterval:     //第二趋势周期
        return trend.trendIntervalSecond;
        break;
    case ID_Disp_Trend_SecondSwitch:     //第二趋势周期切换开关，0-off, 1-on
        return trend.trendIntervalSecondSwitch;
        break;

    case ID_Disp_Trend_Set_Clear:   //采样开始时清除波形，0-off, 1-on
        return trend.trendIntervalSecondSwitch;
        break;
    case ID_Disp_Trend_Set_Direction:   //趋势显示方向， 0-横， 1-纵
        return trend.trendDisplayDirection;
        break;
    case ID_Disp_Trend_Set_Line:    //趋势波形线宽， 0-标准， 1-粗， 2-细， 默认0
        return trend.trendLineWidth;
        break;
    case ID_Disp_Trend_Set_Grid:    //趋势栅格，0/4/5/6/7/8/9/10/11/12, 默认0(自动)
        return trend.trendGrid;
        break;

    case ID_Disp_Trend_Scale_Mode:    //标尺显示模式：0-标准，1-详细，默认：0
        return trend.trendScaleMode;
        break;
    case ID_Disp_Trend_Scale_Type:   //标尺显示方式：0-标记，1-棒图，默认：0
        return trend.trendScaleType;
        break;
    case ID_Disp_Trend_Scale_Number:    //当前值标记通道号，0/3/4,   默认3位
    {
        int type = trend.trendScaleNumber;
        if (type == 0) return 0;
        else if (type == 3) return 1;
        else return 2;
    }
        break;

    case ID_Disp_Trend_Partial_OnOff:     //部分压缩放大开关,0-off, 1-on
        return trend.trendPartialOnOff;
        break;

    case ID_Disp_Trend_Msg_WriteMode:    //趋势信息写入方式，0-共同，1-个别
        return trend.trendMsgWriteMode;
        break;
    case ID_Disp_Trend_Msg_PowerDown:   //停电信息开关，0-off, 1-on
        return trend.trendMsgPowerDown;
        break;
    case ID_Disp_Trend_Msg_IntervalChange:    //趋势周期变更信息写入开关，0-off, 1-on
        return trend.trendMsgIntervalChange;
        break;

    default:
        break;
    }
    return QVariant();
}

QVariant CfgConfig_Display::getDispBasicBackTimeData(quint8 &type)
{
    switch (type) {
    case 1:
        return 0;
        break;
    case 2:
        return 1;
        break;
    case 5:
        return 2;
        break;
    case 10:
        return 3;
        break;
    case 30:
        return 4;
        break;
    case 60:
        return 5;
        break;
    default:
        break;
    }
    return QVariant();
}

QVariant CfgConfig_Display::getDispBasicScrollTimeData(quint8 &type)
{
    switch (type) {
    case 5:
        return 0;
        break;
    case 10:
        return 1;
        break;
    case 20:
        return 2;
        break;
    case 30:
        return 3;
        break;
    case 60:
        return 4;
        break;
    default:
        break;
    }
    return QVariant();
}

QVariant CfgConfig_Display::getDispBasicWinTimeData(quint8 &type)
{
    switch (type) {
    case 0:
        return 0;
        break;
    case 1:
        return 1;
        break;
    case 2:
        return 2;
        break;
    case 5:
        return 3;
        break;
    case 10:
        return 4;
        break;
    case 20:
        return 5;
        break;
    case 30:
        return 6;
        break;
    case 60:
        return 7;
        break;
    default:
        break;
    }
    return QVariant();
}

QVariant CfgConfig_Display::getDispTrendSndInterData(quint32 type)
{
    switch (type) {
    case Trend_Interval_30s:
        return 0;
        break;
    case Trend_Interval_1min:
        return 1;
        break;
    case Trend_Interval_2min:
        return 2;
        break;
    case Trend_Interval_5min:
        return 3;
        break;
    case Trend_Interval_10min:
        return 4;
        break;
    case Trend_Interval_15min:
        return 5;
        break;
    case Trend_Interval_20min:
        return 6;
        break;
    case Trend_Interval_30min:
        return 7;
        break;
    case Trend_Interval_1h:
        return 8;
        break;
    case Trend_Interval_2h:
        return 9;
        break;
    case Trend_Interval_4h:
        return 10;
        break;
    case Trend_Interval_10h:
        return 11;
        break;
    default:
        break;
    }
    return QVariant();
}









QString CfgConfig_Display::getDispBasicObjStr(quint32 ID, quint32 /*group*/)
{
    switch (ID) {
    case ID_Disp_Basic:
        return tr("Screen display settings");
        break;
    case ID_Disp_Basic_Bar:
        return tr("Bar graph");
        break;
    case ID_Disp_Basic_Bar_Direction:
        return tr("Direction");
        break;

    case ID_Disp_Basic_LCD:
        return tr("LCD");
        break;
    case ID_Disp_Basic_LCD_Brightness:
        return tr("Brightness");
        break;

    case ID_Disp_Basic_Backlight:
        return tr("Backlight saver");
        break;
    case ID_Disp_Basic_Backlight_Mode:
        return tr("Mode");
        break;
    case ID_Disp_Basic_Backlight_Time:
        return tr("Saver time");
        break;
    case ID_Disp_Basic_Backlight_Restore:
        return tr("Restore");
        break;

    case ID_Disp_Basic_Monitor:
        return tr("Monitor");
        break;
    case ID_Disp_Basic_Background:
        return tr("Display background");
        break;
    case ID_Disp_Basic_Changetime:
        return tr("Scroll time");
        break;
    case ID_Disp_Basic_Wintime:
        return tr("Jump default display");
        break;

    case ID_Disp_Basic_Calendar:
        return tr("Calendar display");
        break;
    case ID_Disp_Basic_Calendar_Week:
        return tr("1st weekday");
        break;
    case ID_Disp_Basic_Change:
        return tr("Changing each value from monitoring");
        break;
    case ID_Disp_Basic_Change_Switch:
        return tr("On/Off");
        break;
    default:
        break;
    }
    return NULL;
}

QString CfgConfig_Display::getDispGrpObjStr(quint32 ID, quint32 /*group*/)
{
    switch (ID) {
    case ID_Disp_Grp:
    case ID_Disp_Grp_Set:
        return tr("Group settings");
        break;

    case ID_Disp_Grp_Number:
        return tr("Group number");
        break;

    case ID_Disp_Grp_Set_Switch:
    case ID_Disp_Grp_Set_ScaleImage_Switch:
    case ID_Disp_Grp_Set_Scale1_Switch:
    case ID_Disp_Grp_Set_Scale2_Switch:
    case ID_Disp_Grp_Set_Scale3_Switch:
    case ID_Disp_Grp_Set_Scale4_Switch:
        return tr("On/Off");
        break;
    case ID_Disp_Grp_Set_CHNumber:
        return tr("Channel set");
        break;
    case ID_Disp_Grp_Set_Name:
        return tr("Group name");
        break;

    case ID_Disp_Grp_Set_ScaleImage:
        return tr("Scale image");
        break;

    case ID_Disp_Grp_Set_Scale_1:
        return tr("Trip line 1");
        break;
    case ID_Disp_Grp_Set_Scale_2:
        return tr("Trip line 2");
        break;
    case ID_Disp_Grp_Set_Scale_3:
        return tr("Trip line 3");
        break;
    case ID_Disp_Grp_Set_Scale_4:
        return tr("Trip line 4");
        break;

    case ID_Disp_Grp_Set_Scale1_Pos:
    case ID_Disp_Grp_Set_Scale2_Pos:
    case ID_Disp_Grp_Set_Scale3_Pos:
    case ID_Disp_Grp_Set_Scale4_Pos:
        return tr("Position");
        break;
    case ID_Disp_Grp_Set_Scale1_Width:
    case ID_Disp_Grp_Set_Scale2_Width:
    case ID_Disp_Grp_Set_Scale3_Width:
    case ID_Disp_Grp_Set_Scale4_Width:
        return tr("Line width");
        break;
    case ID_Disp_Grp_Set_Scale1_Color:
    case ID_Disp_Grp_Set_Scale2_Color:
    case ID_Disp_Grp_Set_Scale3_Color:
    case ID_Disp_Grp_Set_Scale4_Color:
        return tr("Color");
        break;
    default:
        break;
    }
    return NULL;
}

QString CfgConfig_Display::getDispMsgObjStr(quint32 ID, quint32 /*group*/)
{

    if (ID == ID_Disp_Msg)
        return tr("Message setting");
    else if (ID == ID_Disp_Msg_GrpNum)
        return tr("Message number");
    else if (ID == ID_Disp_Msg_GrpNum_Msg)
        return tr("Message");
    else
    {
        int id = ID % 100;
        return tr("Message %1").arg(id + 1);
    }
}

QString CfgConfig_Display::getDispTrendObjStr(quint32 ID, quint32 /*group*/)
{
    switch(ID)
    {
    case ID_Disp:
        return tr("Display Settings");
        break;
    case ID_Disp_Trend:
        return tr("Trend Settings");
        break;

    case ID_Disp_Trend_Basic:
        return tr("Trend basic");
        break;
    case ID_Disp_Trend_Interval:
        return tr("Trend interval [/div]");
        break;
    case ID_Disp_Trend_SecondInterval:
        return tr("Second interval [/div]");
        break;
    case ID_Disp_Trend_SecondSwitch:
        return tr("Trend rate switching");
        break;

    case ID_Disp_Trend_Set:
        return tr("Trend settings");
        break;
    case ID_Disp_Trend_Set_Direction:
        return tr("Direction");
        break;
    case ID_Disp_Trend_Set_Clear:
        return tr("Trend clear");
        break;
    case ID_Disp_Trend_Set_Line:
        return tr("Trend line");
        break;
    case ID_Disp_Trend_Set_Grid:
        return tr("Grid");
        break;

    case ID_Disp_Trend_Scale:
        return tr("Scale");
        break;
    case ID_Disp_Trend_Scale_Mode:
        return tr("Digit");
        break;
    case ID_Disp_Trend_Scale_Type:
        return tr("Value indicator");
        break;
    case ID_Disp_Trend_Scale_Number:
        return tr("Digit of mark");
        break;

    case ID_Disp_Trend_Partial:
        return tr("Partial");
        break;
    case ID_Disp_Trend_Partial_OnOff:
        return tr("On/Off");
        break;

    case ID_Disp_Trend_Msg:
        return tr("Message");
        break;
    case ID_Disp_Trend_Msg_WriteMode:
        return tr("Write group");
        break;
    case ID_Disp_Trend_Msg_PowerDown:
        return tr("Power-fail message");
        break;
    case ID_Disp_Trend_Msg_IntervalChange:
        return tr("Change message");
        break;
    default:
        break;
    }
    return NULL;
}






/*******************************************************   get obj val str***************************************************************/

QString CfgConfig_Display::getDispBasicObjValStr(quint32 ID, quint32 /*group*/, ConfigObject *cfg)
{
    switch (ID) {
    case ID_Disp_Basic_Bar_Direction:
        return cfg->getData().toInt() == 0 ? tr("Horizontal") : tr("Vertical");
        break;

    case ID_Disp_Basic_LCD_Brightness:
        return tr("Brightness %1").arg(cfg->getData().toInt() / 20 + 1);
        break;

    case ID_Disp_Basic_Backlight_Mode:
    {
        int type = cfg->getData().toInt();
        if (type == 0)
            return tr("Off");
        else if (type == 1)
            return tr("Dimmer");
        else if (type == 2)
            return tr("Time-off");
    }
        break;

    case ID_Disp_Basic_Backlight_Time:
    {
        QString type = getDispBasicBackTimeObjValStr(cfg->getData().toInt());
        if (type.toInt() >= 60)
            return QString("%1 %2").arg(type.toInt() / 60).arg(tr("h"));
        else
            return type + " " + tr("min");
    }
        break;
    case ID_Disp_Basic_Backlight_Restore:
        return cfg->getData().toInt() == 0 ? tr("Key & Touch screen & Alarm") : tr("Key & Touch screen");
        break;

    case ID_Disp_Basic_Background:
        return cfg->getData().toInt() == 0 ? tr("White") : tr("Black");
        break;
    case ID_Disp_Basic_Changetime:
    {
        QString type = getDispBasicScrollTimeObjValStr(cfg->getData().toInt());
        if (type.toInt() >= 60)
            return QString("%1 %2").arg(type.toInt() / 60).arg(tr("h"));
        else
            return type + " " + tr("s");
    }
        break;
    case ID_Disp_Basic_Wintime:
    {
        int type = getDispBasicWinTimeObjValStr(cfg->getData().toInt()).toInt();
        if (type == 0)
            return tr("Off");
        else if (type >= 60)
            return QString::number(type / 60) + " " + tr("h");
        else
            return QString::number(type) + " " + tr("min");
    }
        break;

    case ID_Disp_Basic_Calendar_Week:
        return cfg->getData() == 0 ? tr("Sunday") : tr("Monday");
        break;

    case ID_Disp_Basic_Change_Switch:
        return cfg->getData() == 0 ? tr("Off") : tr("On");
        break;
    default:
        break;
    }
    return NULL;
}

QString CfgConfig_Display::getDispGrpObjValStr(quint32 ID, quint32 /*group*/, ConfigObject *cfg)
{
    switch (ID) {
    case ID_Disp_Grp_Number:
        return QString::number(cfg->getData().toInt() + 1);

    case ID_Disp_Grp_Set_Switch:
    case ID_Disp_Grp_Set_ScaleImage_Switch:
    case ID_Disp_Grp_Set_Scale1_Switch:
    case ID_Disp_Grp_Set_Scale2_Switch:
    case ID_Disp_Grp_Set_Scale3_Switch:
    case ID_Disp_Grp_Set_Scale4_Switch:
        return  cfg->getData().toInt() == 0 ? tr("Off") : tr("On");
        break;
    case ID_Disp_Grp_Set_CHNumber:
        return cfg->getData().toString() + " / " + "20 CH";
        break;

    case ID_Disp_Grp_Set_Scale1_Pos:
    case ID_Disp_Grp_Set_Scale2_Pos:
    case ID_Disp_Grp_Set_Scale3_Pos:
    case ID_Disp_Grp_Set_Scale4_Pos:
        return cfg->getData().toString() + " %";
        break;
    case ID_Disp_Grp_Set_Scale1_Width:
    case ID_Disp_Grp_Set_Scale2_Width:
    case ID_Disp_Grp_Set_Scale3_Width:
    case ID_Disp_Grp_Set_Scale4_Width:
    {
        int type = cfg->getData().toInt();
        if (type == 0)
            return tr("Normal");
        else if (type == 1)
            return tr("Thick");
        else
            return tr("Thin");
    }
        break;
    case ID_Disp_Grp_Set_Scale1_Color:
    case ID_Disp_Grp_Set_Scale2_Color:
    case ID_Disp_Grp_Set_Scale3_Color:
    case ID_Disp_Grp_Set_Scale4_Color:
        return getColorValueObjStr(cfg->getData().toInt());
        break;
    default:
        break;
    }
    return cfg->getData().toString();
}

QString CfgConfig_Display::getDispMsgObjValStr(quint32 ID, quint32 /*group*/, ConfigObject *cfg)
{
    if (ID == ID_Disp_Msg_GrpNum)
    {
        int type = cfg->getData().toInt();
        switch (type) {
        case 0:
            return "1 - 10";
            break;
        case 1:
            return "11 - 20";
            break;
        case 2:
            return "21 - 30";
            break;
        case 3:
            return "31 - 40";
            break;
        case 4:
            return "41 - 50";
            break;
        case 5:
            return "51 - 60";
            break;
        case 6:
            return "61 - 70";
            break;
        case 7:
            return "71 - 80";
            break;
        case 8:
            return "81 - 90";
            break;
        case 9:
            return "91 - 100";
            break;
        default:
            break;
        }
    }
    return cfg->getData().toString();
}


QString CfgConfig_Display::getDispTrendObjValStr(quint32 ID, quint32 /*group*/, ConfigObject *cfg)
{
    switch(ID)
    {
    case ID_Disp_Trend_Interval:
    case ID_Disp_Trend_SecondInterval:
    {
        int time = cfg->getData().toInt();
        if (time < 60)
            return QString::number(time) + " " + tr("s");
        else if (time >= 60 && time <= 1800)
            return QString::number(time / 60) + " " + tr("min");
        else
            return QString::number(time / 3600) + " " + tr("h");
    }
        break;
    case ID_Disp_Trend_SecondSwitch:
    case ID_Disp_Trend_Partial_OnOff:
    case ID_Disp_Trend_Set_Clear:
    case ID_Disp_Trend_Msg_PowerDown:
    case ID_Disp_Trend_Msg_IntervalChange:
        return cfg->getData().toInt() == 0 ? tr("Off") : tr("On");
        break;

    case ID_Disp_Trend_Set_Direction:
        return cfg->getData().toInt() == 0 ? tr("Horizontal") : tr("Verical");
        break;
    case ID_Disp_Trend_Set_Line:
    {
        int type = cfg->getData().toInt();
        if (type == 0)
            return tr("Normal");
        else if (type == 1)
            return tr("Thick");
        else
            return tr("Thin");
    }
        break;
    case ID_Disp_Trend_Set_Grid:
    {
        int type = cfg->getData().toInt();
        if (type == 0)
            return tr("Auto");
        else
            return QString::number(type);
    }
        break;

    case ID_Disp_Trend_Scale_Mode:
        return cfg->getData().toInt() == 0 ? tr("Normal") : tr("Fine");
        break;
    case ID_Disp_Trend_Scale_Type:
        return cfg->getData().toInt() == 0 ? tr("Mark") : tr("Bar graph");
        break;
    case ID_Disp_Trend_Scale_Number:
    {
        int type = cfg->getData().toInt();
        if (type == 0) return tr("0 digit");
        else if (type == 1) return tr("3 digit");
        else return tr("4 digit");
    }
        break;

    case ID_Disp_Trend_Msg_WriteMode:
        return cfg->getData().toInt() == 0 ? tr("Common") : tr("Separate");
        break;
    default:
        break;
    }
    return NULL;
}



QString CfgConfig_Display::getDispBasicBackTimeObjValStr(int type)
{
    switch (type) {
    case 0:
        return "1";
        break;
    case 1:
        return "2";
        break;
    case 2:
        return "5";
        break;
    case 3:
        return "10";
        break;
    case 4:
        return "30";
        break;
    case 5:
        return "60";
        break;
    default:
        break;
    }
    return NULL;
}

QString CfgConfig_Display::getDispBasicScrollTimeObjValStr(int type)
{
    switch (type) {
    case 0:
        return "5";
        break;
    case 1:
        return "10";
        break;
    case 2:
        return "20";
        break;
    case 3:
        return "30";
        break;
    case 4:
        return "60";
        break;
    default:
        break;
    }
    return NULL;
}


QString CfgConfig_Display::getDispBasicWinTimeObjValStr(int type)
{
    switch (type) {
    case 0:
        return "0";
        break;
    case 1:
        return "1";
        break;
    case 2:
        return "2";
        break;
    case 3:
        return "5";
        break;
    case 4:
        return "10";
        break;
    case 5:
        return "20";
        break;
    case 6:
        return "30";
        break;
    case 7:
        return "60";
        break;
    default:
        break;
    }
    return NULL;
}


QString CfgConfig_Display::getDispTrendSndInterObjValStr(int type)
{
    switch (type) {
    case 0:
        return QString::number(Trend_Interval_30s);
        break;
    case 1:
        return QString::number(Trend_Interval_1min);
        break;
    case 2:
        return QString::number(Trend_Interval_2min);
        break;
    case 3:
        return QString::number(Trend_Interval_5min);
        break;
    case 4:
        return QString::number(Trend_Interval_10min);
        break;
    case 5:
        return QString::number(Trend_Interval_15min);
        break;
    case 6:
        return QString::number(Trend_Interval_20min);
        break;
    case 7:
        return QString::number(Trend_Interval_30min);
        break;
    case 8:
        return QString::number(Trend_Interval_1h);
        break;
    case 9:
        return QString::number(Trend_Interval_2h);
        break;
    case 10:
        return QString::number(Trend_Interval_4h);
        break;
    case 11:
        return QString::number(Trend_Interval_10h);
        break;
    default:
        break;
    }
    return NULL;
}





/***********************************************    save    *******************************************/
void CfgConfig_Display::setDispBasicData(quint32 ID, quint32 /*group*/)
{
    switch (ID) {
    case ID_Disp_Basic_Bar_Direction:
        p_deviceConfig->displayBasic.basicBarDirection = ConfigTreeObject::getConfigData(QString(DISPCONFIG_SCREEN_BAR_DIRECTION)).toInt();
        break;

    case ID_Disp_Basic_LCD_Brightness: //LCD显示辉度，1-6,默认:3
        p_deviceConfig->displayBasic.basicLCDBrightness = ConfigTreeObject::getConfigData(QString(DISPCONFIG_SCREEN_LCD_SELECT)).toInt();
        break;
    case ID_Disp_Basic_Backlight_Mode: //LCD屏保模式，0-off, 1-变暗，2-熄灭
        p_deviceConfig->displayBasic.basicBacklightSaverMode = ConfigTreeObject::getConfigData(QString(DISPCONFIG_SCREEN_BACKLIGHT_MODE)).toInt();
        break;
    case ID_Disp_Basic_Backlight_Time:      //LCD屏保等待时间，单位：min, 范围：1min/2min/5min/10min/30min/1h
        p_deviceConfig->displayBasic.basicBacklightSaverTime = getDispBasicBackTimeObjValStr(\
                    ConfigTreeObject::getConfigData(QString(DISPCONFIG_SCREEN_BACKLIGHT_TIME)).toInt()).toInt();
        break;
    case ID_Disp_Basic_Backlight_Restore:      //LCD屏保恢复模式，0-按键/触摸/报警， 1-按键/触摸，默认：0
        p_deviceConfig->displayBasic.basicBacklightRestore = ConfigTreeObject::getConfigData(QString(DISPCONFIG_SCREEN_BACKLIGHT_RESTORE)).toInt();
        break;
    case ID_Disp_Basic_Background:      //UI背景色，0-白色，1-黑色，默认：0
        p_deviceConfig->displayBasic.basicBackground = ConfigTreeObject::getConfigData(QString(DISPCONFIG_SCREEN_MONITOR_BACK)).toInt();
        break;

    case ID_Disp_Basic_Changetime:      //组自动切换时间，单位：s,范围：5s/10s/20s/30s/60s(1min),默认10s
        p_deviceConfig->displayBasic.basicGroupChangeTime = getDispBasicScrollTimeObjValStr(\
                    ConfigTreeObject::getConfigData(QString(DISPCONFIG_SCREEN_MONITOR_SCROLL)).toInt()).toInt();
        break;
    case ID_Disp_Basic_Wintime:      //自动默认画面跳转时间，单位:min,范围：0(off)/1/2/5/10/20/30/1h,默认：0
        p_deviceConfig->displayBasic.basicAutoHomeWinTime = getDispBasicWinTimeObjValStr(\
                    ConfigTreeObject::getConfigData(QString(DISPCONFIG_SCREEN_MONITOR_JUMP)).toInt()).toInt();
        break;

    case ID_Disp_Basic_Calendar_Week:      //周起始日，0-星期日，1-星期一
        p_deviceConfig->displayBasic.basicFirstWeekday = ConfigTreeObject::getConfigData(QString(DISPCONFIG_SCREEN_CALENDAR_ITEM)).toInt();
        break;

    case ID_Disp_Basic_Change_Switch:      //从监视器修改各值，false-off, true-on
        p_deviceConfig->displayBasic.basicChangeFromMonitor = ConfigTreeObject::getConfigData(QString(DISPCONFIG_SCREEN_CHANGEVALUE_ONOFF)).toInt();
        break;
    default:
        break;
    }
}

void CfgConfig_Display::setDispGrpData(quint32 ID, quint32 group)
{
    switch (ID) {
    case ID_Disp_Grp_Set_Switch:         //组开关，0-off, 1-on
        p_deviceConfig->displayGroup[group].groupSwitch = ConfigTreeObject::getConfigData(QString(DISPCONFIG_GROUP_GROUP_SWITCH).arg(group)).toInt();
        break;
    case ID_Disp_Grp_Set_CHNumber:       //组通道数，最多20个通道
        p_deviceConfig->displayGroup[group].groupChannelNum  = ConfigTreeObject::getConfigData(QString(DISPCONFIG_GROUP_GROUP_CHANSET).arg(group)).toInt();
        break;
    case ID_Disp_Grp_Set_Name:       //组名，最多16个半角字符，中英文支持
        return setStrToChar(p_deviceConfig->displayGroup[group].groupName, \
                            ConfigTreeObject::getConfigData(QString(DISPCONFIG_GROUP_GROUP_GRPNAME).arg(group)).toString(), 20);
        break;
    case ID_Disp_Grp_Set_CHItem:      //组通道号，有效通道号由groupChannelNum决定
    {
        QList<QVariant> channelList = ConfigTreeObject::getConfigData(QString(DISPCONFIG_GROUP_GROUP_CHANITEM).arg(group)).toList();
        QList<QVariant>::iterator var;
        memset(p_deviceConfig->displayGroup[group].groupChannelItem, 0, 20 * sizeof(quint32)); //清空数组
        int i = 0;
        for(var = channelList.begin(); var != channelList.end(); var++)
            p_deviceConfig->displayGroup[group].groupChannelItem[i++] = (*var).toUInt();
    }
        break;

    case ID_Disp_Grp_Set_ScaleImage_Switch:       //组标尺图像开关，0-off, 1-on
        p_deviceConfig->displayGroup[group].groupScaleImage = ConfigTreeObject::getConfigData(QString(DISPCONFIG_GROUP_SCALEIMAGE_SWITCH).arg(group)).toInt();
        break;

    case ID_Disp_Grp_Set_Scale1_Switch:      //分割线1-4开关，0-off, 1-on, 默认false
        p_deviceConfig->displayGroup[group].groupTripLineSwitch[0] = ConfigTreeObject::getConfigData(QString(DISPCONFIG_GROUP_TRIPLINE1_SWITCH).arg(group)).toInt();
        break;
    case ID_Disp_Grp_Set_Scale1_Pos:       //分割线1-4位置，单位:%, 0-100,默认50
        p_deviceConfig->displayGroup[group].groupTripLinePos[0] = ConfigTreeObject::getConfigData(QString(DISPCONFIG_GROUP_TRIPLINE1_POS).arg(group)).toInt();
        break;
    case ID_Disp_Grp_Set_Scale1_Width:       //分割线1-4线宽，0-标准， 1-粗， 2-细， 默认0
        p_deviceConfig->displayGroup[group].groupTripLineWidth[0] = ConfigTreeObject::getConfigData(QString(DISPCONFIG_GROUP_TRIPLINE1_WIDTH).arg(group)).toInt();
        break;
    case ID_Disp_Grp_Set_Scale1_Color:        //分割线1-4颜色值，BYTE2-Red, BYTE1-Green, BYTE0-Blue
        p_deviceConfig->displayGroup[group].groupTripLineColor[0] = ConfigTreeObject::getConfigData(QString(DISPCONFIG_GROUP_TRIPLINE1_COLOR).arg(group)).toInt();
        break;

    case ID_Disp_Grp_Set_Scale2_Switch:      //分割线1-4开关，0-off, 1-on, 默认false
        p_deviceConfig->displayGroup[group].groupTripLineSwitch[1] = ConfigTreeObject::getConfigData(QString(DISPCONFIG_GROUP_TRIPLINE2_SWITCH).arg(group)).toInt();
        break;
    case ID_Disp_Grp_Set_Scale2_Pos:       //分割线1-4位置，单位:%, 0-100,默认50
        p_deviceConfig->displayGroup[group].groupTripLinePos[1] = ConfigTreeObject::getConfigData(QString(DISPCONFIG_GROUP_TRIPLINE2_POS).arg(group)).toInt();
        break;
    case ID_Disp_Grp_Set_Scale2_Width:       //分割线1-4线宽，0-标准， 1-粗， 2-细， 默认0
        p_deviceConfig->displayGroup[group].groupTripLineWidth[1] = ConfigTreeObject::getConfigData(QString(DISPCONFIG_GROUP_TRIPLINE2_WIDTH).arg(group)).toInt();
        break;
    case ID_Disp_Grp_Set_Scale2_Color:        //分割线1-4颜色值，BYTE2-Red, BYTE1-Green, BYTE0-Blue
        p_deviceConfig->displayGroup[group].groupTripLineColor[1] = ConfigTreeObject::getConfigData(QString(DISPCONFIG_GROUP_TRIPLINE2_COLOR).arg(group)).toInt();
        break;

    case ID_Disp_Grp_Set_Scale3_Switch:      //分割线1-4开关，0-off, 1-on, 默认false
        p_deviceConfig->displayGroup[group].groupTripLineSwitch[2] = ConfigTreeObject::getConfigData(QString(DISPCONFIG_GROUP_TRIPLINE3_SWITCH).arg(group)).toInt();
        break;
    case ID_Disp_Grp_Set_Scale3_Pos:       //分割线1-4位置，单位:%, 0-100,默认50
        p_deviceConfig->displayGroup[group].groupTripLinePos[2] = ConfigTreeObject::getConfigData(QString(DISPCONFIG_GROUP_TRIPLINE3_POS).arg(group)).toInt();
        break;
    case ID_Disp_Grp_Set_Scale3_Width:       //分割线1-4线宽，0-标准， 1-粗， 2-细， 默认0
        p_deviceConfig->displayGroup[group].groupTripLineWidth[2] = ConfigTreeObject::getConfigData(QString(DISPCONFIG_GROUP_TRIPLINE3_WIDTH).arg(group)).toInt();
        break;
    case ID_Disp_Grp_Set_Scale3_Color:        //分割线1-4颜色值，BYTE2-Red, BYTE1-Green, BYTE0-Blue
        p_deviceConfig->displayGroup[group].groupTripLineColor[2] = ConfigTreeObject::getConfigData(QString(DISPCONFIG_GROUP_TRIPLINE3_COLOR).arg(group)).toInt();
        break;

    case ID_Disp_Grp_Set_Scale4_Switch:      //分割线1-4开关，0-off, 1-on, 默认false
        p_deviceConfig->displayGroup[group].groupTripLineSwitch[3] = ConfigTreeObject::getConfigData(QString(DISPCONFIG_GROUP_TRIPLINE4_SWITCH).arg(group)).toInt();
        break;
    case ID_Disp_Grp_Set_Scale4_Pos:       //分割线1-4位置，单位:%, 0-100,默认50
        p_deviceConfig->displayGroup[group].groupTripLinePos[3] = ConfigTreeObject::getConfigData(QString(DISPCONFIG_GROUP_TRIPLINE4_POS).arg(group)).toInt();
        break;
    case ID_Disp_Grp_Set_Scale4_Width:       //分割线1-4线宽，0-标准， 1-粗， 2-细， 默认0
        p_deviceConfig->displayGroup[group].groupTripLineWidth[3] = ConfigTreeObject::getConfigData(QString(DISPCONFIG_GROUP_TRIPLINE4_WIDTH).arg(group)).toInt();
        break;
    case ID_Disp_Grp_Set_Scale4_Color:        //分割线1-4颜色值，BYTE2-Red, BYTE1-Green, BYTE0-Blue
        p_deviceConfig->displayGroup[group].groupTripLineColor[3] = ConfigTreeObject::getConfigData(QString(DISPCONFIG_GROUP_TRIPLINE4_COLOR).arg(group)).toInt();
        break;
    default:
        break;
    }
}

void CfgConfig_Display::setDispMsgData(quint32 ID, quint32 /*group*/)
{
    int id = ID % 100;
    setStrToChar(p_deviceConfig->displayMessage.preMessage[id], \
                 ConfigTreeObject::getConfigData(QString(DISPCONFIG_MESSAGE_MESSAGE_INDEX).arg(id + 1)).toString(), 32);
}

void CfgConfig_Display::setDispTrendData(quint32 ID, quint32 /*group*/)
{
    switch (ID) {
    case ID_Disp_Trend_Interval:    //趋势周期，最小值依赖于测量周期或事件记录周期
        p_deviceConfig->displayTrend.trendInterval_s = (Trend_Interval)ConfigTreeObject::getConfigData(DISPCONFIG_TREND_INTERVAL_INTERVAL).toInt();
        break;
    case ID_Disp_Trend_SecondInterval:     //第二趋势周期
        p_deviceConfig->displayTrend.trendIntervalSecond = (Trend_Interval)ConfigTreeObject::getConfigData(DISPCONFIG_TREND_INTERVAL_SECOND).toInt();
        break;
    case ID_Disp_Trend_SecondSwitch:     //第二趋势周期切换开关，0-off, 1-on
        p_deviceConfig->displayTrend.trendIntervalSecondSwitch = ConfigTreeObject::getConfigData(DISPCONFIG_TREND_INTERVAL_SWITCH).toInt();
        break;

    case ID_Disp_Trend_Set_Clear:   //采样开始时清除波形，0-off, 1-on
        p_deviceConfig->displayTrend.trendIntervalSecondSwitch = ConfigTreeObject::getConfigData(DISPCONFIG_TREND_BASIC_CLEAR).toInt();
        break;
    case ID_Disp_Trend_Set_Direction:   //趋势显示方向， 0-横， 1-纵
        p_deviceConfig->displayTrend.trendDisplayDirection = ConfigTreeObject::getConfigData(DISPCONFIG_TREND_BASIC_DIRECTION).toInt();
        break;
    case ID_Disp_Trend_Set_Line:    //趋势波形线宽， 0-标准， 1-粗， 2-细， 默认0
        p_deviceConfig->displayTrend.trendLineWidth = ConfigTreeObject::getConfigData(DISPCONFIG_TREND_BASIC_LINE).toInt();
        break;
    case ID_Disp_Trend_Set_Grid:    //趋势栅格，0/4/5/6/7/8/9/10/11/12, 默认0(自动)
        p_deviceConfig->displayTrend.trendGrid = ConfigTreeObject::getConfigData(DISPCONFIG_TREND_BASIC_GRID).toInt();
        break;

    case ID_Disp_Trend_Scale_Mode:    //标尺显示模式：0-标准，1-详细，默认：0
        p_deviceConfig->displayTrend.trendScaleMode = ConfigTreeObject::getConfigData(DISPCONFIG_TREND_SCALE_DIGIT).toInt();
        break;
    case ID_Disp_Trend_Scale_Type:   //标尺显示方式：0-标记，1-棒图，默认：0
        p_deviceConfig->displayTrend.trendScaleType = ConfigTreeObject::getConfigData(DISPCONFIG_TREND_SCALE_VALUE).toInt();
        break;
    case ID_Disp_Trend_Scale_Number:    //当前值标记通道号，0/3/4,   默认3位
    {
        int type = ConfigTreeObject::getConfigData(DISPCONFIG_TREND_SCALE_MARK).toInt();
        if (type == 0) p_deviceConfig->displayTrend.trendScaleNumber = 0;
        else if (type == 1) p_deviceConfig->displayTrend.trendScaleNumber = 3;
        else p_deviceConfig->displayTrend.trendScaleNumber = 4;
    }
        break;

    case ID_Disp_Trend_Partial_OnOff:     //部分压缩放大开关,0-off, 1-on
        p_deviceConfig->displayTrend.trendPartialOnOff = ConfigTreeObject::getConfigData(DISPCONFIG_TREND_PARTIAL_SWITCH).toInt();
        break;

    case ID_Disp_Trend_Msg_WriteMode:    //趋势信息写入方式，0-共同，1-个别
        p_deviceConfig->displayTrend.trendMsgWriteMode = ConfigTreeObject::getConfigData(DISPCONFIG_TREND_MESSAGE_WRITEGRP).toInt();
        break;
    case ID_Disp_Trend_Msg_PowerDown:   //停电信息开关，0-off, 1-on
        p_deviceConfig->displayTrend.trendMsgPowerDown = ConfigTreeObject::getConfigData(DISPCONFIG_TREND_MESSAGE_POWERMSG).toInt();
        break;
    case ID_Disp_Trend_Msg_IntervalChange:    //趋势周期变更信息写入开关，0-off, 1-on
        p_deviceConfig->displayTrend.trendMsgIntervalChange = ConfigTreeObject::getConfigData(DISPCONFIG_TREND_MESSAGE_CHANGMSG).toInt();
        break;
    default:
        break;
    }
}




/*
 * 把uchar *类型转化为QString类型
*/
QVariant CfgConfig_Display::getCharToString(uchar *str)
{
    QString data((char *)str);
    return data;
}

//把QString类型转化为uchar *类型
void CfgConfig_Display::setStrToChar(uchar * uData, QString str, int len)
{
    memset(uData, 0, len);
    QByteArray baUnit = str.toLocal8Bit();
    char* valueUnit = baUnit.data();
//    memcpy(uData, valueUnit, strlen(valueUnit));
    while (*valueUnit != '\0') {
        *uData++ = *valueUnit++;
    }
}

QString CfgConfig_Display::getColorValueObjStr(quint32 x)
{
    QColor colorRGBx(COLOR_RED(x), COLOR_GREEN(x), COLOR_BLUE(x));
    QString str = QString("QLabel {background-color: %1}").arg(colorRGBx.name());
    return str;
}
