#include "cfgconfig_comm.h"
#include "GlobalData/cfgchannel.h"
#include <QVariant>
#include "./GlobalData/configtreeobject_Comm.h"
#include "./GlobalData/configtreeobject.h"
#include "./Common/digitaloperation.h"

CfgConfig_Comm *CfgConfig_Comm::instance()
{
    static CfgConfig_Comm data;
    return &data;
}


CfgConfig_Comm::CfgConfig_Comm(QObject *parent)
    : QObject(parent)
{
}





QVariant CfgConfig_Comm::getCommData(quint32 ID, quint32 group)
{
    if (ID >= Comm_Range_Start_ID && ID <= Comm_Range_End_ID)
        return getCommRangeData(ID, group);
    else if (ID >= Comm_Alarm_Start_ID && ID <= Comm_Alarm_End_ID)
        return getCommAlarmData(ID, group);
    else if (ID >= Comm_Disp_Start_ID && ID <= Comm_Disp_End_ID)
        return getCommDispData(ID, group);
    return QVariant();

}

QString CfgConfig_Comm::getCommObjStr(quint32 ID, quint32 group)
{
    if (ID >= Comm_Start_ID && ID <= Comm_Range_End_ID)
        return getCommRangeObjStr(ID, group);
    else if (ID >= Comm_Alarm_Start_ID && ID <= Comm_Alarm_End_ID)
        return getCommAlarmObjStr(ID, group);
    else if (ID >= Comm_Disp_Start_ID && ID <= Comm_Disp_End_ID)
        return getCommDispObjStr(ID, group);
    return NULL;
}

QString CfgConfig_Comm::getCommObjValStr(quint32 ID, quint32 group, ConfigObject * cfg)
{
    if (ID >= Comm_Range_Start_ID && ID <= Comm_Range_End_ID)
        return getCommRangeObjValStr(ID, group, cfg);
    else if (ID >= Comm_Alarm_Start_ID && ID <= Comm_Alarm_End_ID)
        return getCommAlarmObjValStr(ID, group, cfg);
    else if (ID >= Comm_Disp_Start_ID && ID <= Comm_Disp_End_ID)
        return getCommDispObjValStr(ID, group, cfg);
    return NULL;
}

void CfgConfig_Comm::setCommData(quint32 ID, quint32 group)
{
    if (ID >= Comm_Range_Start_ID && ID <= Comm_Range_End_ID)
        setCommRangeData(ID, group);
    else if (ID >= Comm_Alarm_Start_ID && ID <= Comm_Alarm_End_ID)
        setCommAlarmData(ID, group);
    else if (ID >= Comm_Disp_Start_ID && ID <= Comm_Disp_End_ID)
        setCommDispData(ID, group);
}




/*******************************************  get comm data *************************************************/

QVariant CfgConfig_Comm::getCommRangeData(quint32 ID, quint32 group)
{
    COMM_CONFIG CommCfg = *CfgChannel::instance()->getCommConfig(group);

    switch (ID) {
    case ID_Comm_Range_Range_Switch:    //量程类型
        return CommCfg.rangeConfig.rangeSwtich;
        break;

    case ID_Comm_Range_Range_Min:     //范围下限
        return CommCfg.rangeConfig.rangeMin;
        break;
    case ID_Comm_Range_Range_Max:     //范围上限
        return CommCfg.rangeConfig.rangeMax;
        break;

    case ID_Comm_Range_Range_Pos:       //小数点位置, 0/1/2/3/4/5，默认0
        return CommCfg.rangeConfig.rangePos;
        break;
    case ID_Comm_Range_Range_Unit:         //单位，字符串，最多6个半角字符
        return getCharToString(CommCfg.rangeConfig.rangeUnit);
        break;


    case ID_Comm_Range_PowerOn_Val:      //电源接通时的值类型， 0-最终值，1-预设值，默认0
        return CommCfg.rangeConfig.rangePowerOnVal;
        break;
    case ID_Comm_Range_Pre_Val:           //预设值
        return CommCfg.rangeConfig.rangePreVal;
        break;

    case ID_Comm_Range_Timer_Switch:           //监视时钟开关,0-off, 1-on
        return CommCfg.rangeConfig.rangeTimerSwitch;
        break;
    case ID_Comm_Range_Timer_Interval:         //监视器计时器时间，1-120s,默认30
        return CommCfg.rangeConfig.rangeTimerInterval;
        break;
    case ID_Comm_Range_Timer_ResetVal:         //计时器满值设定值类型，0-最终值，1-预设值。默认0
        return CommCfg.rangeConfig.rangeTimerResetVal;
        break;
    default:
        break;
    }
    return QVariant();
}

QVariant CfgConfig_Comm::getCommAlarmData(quint32 ID, quint32 group)
{
    COMM_CONFIG CommCfg = *CfgChannel::instance()->getCommConfig(group);

    switch (ID) {
    case ID_Comm_Alarm_Level1_OnOff:
        return CommCfg.alarmConfig.alarmOnOff[0];
        break;
    case ID_Comm_Alarm_Level1_Value:
        return CommCfg.alarmConfig.alarmValue[0];
        break;
    case ID_Comm_Alarm_Level1_Hysteresis:
        return CommCfg.alarmConfig.alarmHysteresis[0];
        break;
    case ID_Comm_Alarm_Level1_Logging:
        return CommCfg.alarmConfig.alarmLogging[0];
        break;
    case ID_Comm_Alarm_Level1_OutputType:
        return CommCfg.alarmConfig.alarmOutputType[0];
        break;
    case ID_Comm_Alarm_Level1_OutputNo:
        return CommCfg.alarmConfig.alarmOuputNo[0];
        break;

    case ID_Comm_Alarm_Level2_OnOff:
        return CommCfg.alarmConfig.alarmOnOff[1];
        break;
    case ID_Comm_Alarm_Level2_Value:
        return CommCfg.alarmConfig.alarmValue[1];
        break;
    case ID_Comm_Alarm_Level2_Hysteresis:
        return CommCfg.alarmConfig.alarmHysteresis[1];
        break;
    case ID_Comm_Alarm_Level2_Logging:
        return CommCfg.alarmConfig.alarmLogging[1];
        break;
    case ID_Comm_Alarm_Level2_OutputType:
        return CommCfg.alarmConfig.alarmOutputType[1];
        break;
    case ID_Comm_Alarm_Level2_OutputNo:
        return CommCfg.alarmConfig.alarmOuputNo[1];
        break;

    case ID_Comm_Alarm_Level3_OnOff:
        return CommCfg.alarmConfig.alarmOnOff[2];
        break;
    case ID_Comm_Alarm_Level3_Type:
        return CommCfg.alarmConfig.alarmType[2];
        break;
    case ID_Comm_Alarm_Level3_Value:
        return CommCfg.alarmConfig.alarmValue[2];
        break;
    case ID_Comm_Alarm_Level3_Hysteresis:
        return CommCfg.alarmConfig.alarmHysteresis[2];
        break;
    case ID_Comm_Alarm_Level3_Logging:
        return CommCfg.alarmConfig.alarmLogging[2];
        break;
    case ID_Comm_Alarm_Level3_OutputType:
        return CommCfg.alarmConfig.alarmOutputType[2];
        break;
    case ID_Comm_Alarm_Level3_OutputNo:
        return CommCfg.alarmConfig.alarmOuputNo[2];
        break;

    case ID_Comm_Alarm_Level4_OnOff:
        return CommCfg.alarmConfig.alarmOnOff[3];
        break;
    case ID_Comm_Alarm_Level4_Type:
        return CommCfg.alarmConfig.alarmType[3];
        break;
    case ID_Comm_Alarm_Level4_Value:
        return CommCfg.alarmConfig.alarmValue[3];
        break;
    case ID_Comm_Alarm_Level4_Hysteresis:
        return CommCfg.alarmConfig.alarmHysteresis[3];
        break;
    case ID_Comm_Alarm_Level4_Logging:
        return CommCfg.alarmConfig.alarmLogging[3];
        break;
    case ID_Comm_Alarm_Level4_OutputType:
        return CommCfg.alarmConfig.alarmOutputType[3];
        break;
    case ID_Comm_Alarm_Level4_OutputNo:
        return CommCfg.alarmConfig.alarmOuputNo[3];
        break;


    case ID_Comm_Alarm_Leve_Delay_s:
        return CommCfg.alarmConfig.alarmDelay[0];
        break;
    case ID_Comm_Alarm_Leve_Delay_m:
        return CommCfg.alarmConfig.alarmDelay[1];
        break;
    case ID_Comm_Alarm_Leve_Delay_h:
        return CommCfg.alarmConfig.alarmDelay[2];
        break;
    default:
        break;
    }
    return QVariant();
}

QVariant CfgConfig_Comm::getCommDispData(quint32 ID, quint32 group)
{
    COMM_CONFIG CommCfg = *CfgChannel::instance()->getCommConfig(group);
    switch (ID) {
    case ID_Comm_Disp_Color_Value:        //通道颜色，Byte2-Red, Byte1-Green, Byte0-Blue
        return CommCfg.displayConfig.displayColor;
        break;
    case ID_Comm_Disp_Tag_Char:       //标记字符串，中英支持，最多32个半角字符
        return getCharToString(CommCfg.displayConfig.displayTagChar);
        break;
    case ID_Comm_Disp_Tag_No:         //标记号码，符号，最多16个半角字符
        return getCharToString(CommCfg.displayConfig.displayTagNo);
        break;
    case ID_Comm_Disp_Zone_Upper:     //显示区域上限，5-100,单位%
        return CommCfg.displayConfig.displayZoneUpper;
        break;
    case ID_Comm_Disp_Zone_Lower:
        return CommCfg.displayConfig.dispalyZoneLower;
        break;
    case ID_Comm_Disp_Scale_Pos:           //标尺位置，1-10,默认1
        return CommCfg.displayConfig.displayScalePos;
        break;
    case ID_Comm_Disp_Scale_Div:           //标尺分割，4-13, 13:C10
        return CommCfg.displayConfig.displayScaleDiv;
        break;
    case ID_Comm_DISP_Bar_Pos:        //棒图基准位置， 0-Lower, 1-Center, 2-Upper
        return CommCfg.displayConfig.displayBarPos;
        break;
    case ID_Comm_DISP_Bar_Div:        //棒图分割数，4-12, 默认10
        return CommCfg.displayConfig.displayBarDiv;
        break;

    case ID_Comm_DISP_Partial_Pos:
        return CommCfg.displayConfig.displayPartialPos;
        break;
    case ID_Comm_DISP_Partial_Boundary:
        return getFloatToInt(CommCfg.displayConfig.displayPartialBoundary, CommCfg.rangeConfig.rangePos);
        break;
    case ID_Comm_DISP_Partial_OnOff:
        return CommCfg.displayConfig.displayPartialOnOff;
        break;
    case ID_Comm_DISP_ColorScaleBand_Mode:
        return CommCfg.displayConfig.displayColorbandMode;
        break;
    case ID_Comm_DISP_ColorScaleBand_PosUpper:
        return getFloatToInt(CommCfg.displayConfig.displayColorbandPosUpper, CommCfg.rangeConfig.rangePos);
        break;
    case ID_Comm_DISP_ColorScaleBand_PosLower:
        return getFloatToInt(CommCfg.displayConfig.displayColorbandPosLower, CommCfg.rangeConfig.rangePos);
        break;
    case ID_Comm_DISP_ColorScaleBand_Color:
        return CommCfg.displayConfig.displayColorbandColor;
        break;

    case ID_Comm_DISP_AlarmMark_OnOff:         //报警标记开关，0-0ff, 1-on
        return CommCfg.displayConfig.displayAlarmMarkOnOff;
        break;
    case ID_Comm_DISP_AlarmMark_Type:      //报警标记类型，0-报警， 1-固定
        return CommCfg.displayConfig.displayAlarmMarkType;
        break;
    case ID_Comm_DISP_AlarmMark_Color1:       //报警1-4颜色， Byte2-Red, Byte1-Green, Byte0-Blue
        return CommCfg.displayConfig.displayAlarmMarkColor[0];
        break;
    case ID_Comm_DISP_AlarmMark_Color2:
        return CommCfg.displayConfig.displayAlarmMarkColor[1];
        break;
    case ID_Comm_DISP_AlarmMark_Color3:
        return CommCfg.displayConfig.displayAlarmMarkColor[2];
        break;
    case ID_Comm_DISP_AlarmMark_Color4:
        return CommCfg.displayConfig.displayAlarmMarkColor[3];
        break;
    default:
        break;
    }
    return QVariant();
}



/*******************************************  get comm obj str *************************************************/
QString CfgConfig_Comm::getCommRangeObjStr(quint32 ID, quint32 /*group*/)
{
    switch (ID) {
    case ID_Comm:
        return tr("Communication channel settings");
        break;
    case ID_Comm_Last_CH:
        return tr("Last-CH");
        break;
    case ID_Comm_First_CH:
        return tr("First-CH");
        break;
    case ID_Comm_Range:
    case ID_Comm_Range_Range:
        return tr("On/Off, Span");
        break;
    case ID_Comm_Range_Range_Switch:
        return tr("On/Off");
        break;
    case ID_Comm_Range_Range_Min:
        return tr("Span Lower");
        break;
    case ID_Comm_Range_Range_Max:
        return tr("Span Upper");
        break;
    case ID_Comm_Range_Range_Pos:
        return tr("Decimal place");
        break;
    case ID_Comm_Range_Range_Unit:
        return tr("Unit");
        break;
    case ID_Comm_Range_PowerOn:
        return tr("At power on");
        break;
    case ID_Comm_Range_PowerOn_Val:
        return tr("Value at power on");
        break;

    case ID_Comm_Range_Pre_Val:
    case ID_Comm_Range_Pre:
        return tr("Preset value");
        break;
    case ID_Comm_Range_Timer:
        return tr("Watchdog timer");
        break;
    case ID_Comm_Range_Timer_Switch:
        return tr("On/Off");
        break;
    case ID_Comm_Range_Timer_Interval:
        return tr("Timer");
        break;
    case ID_Comm_Range_Timer_ResetVal:
        return tr("Value at timer-expired");
        break;
    default:
        break;
    }
    return NULL;
}

QString CfgConfig_Comm::getCommAlarmObjStr(quint32 ID, quint32 /*group*/)
{
    switch (ID) {
    case ID_Comm_Alarm:
        return tr("Alarm");
        break;

    case ID_Comm_Alarm_Level1:
        return QString(tr("Level") + " 1");
        break;
    case ID_Comm_Alarm_Level2:
        return QString(tr("Level") + " 2");
        break;
    case ID_Comm_Alarm_Level3:
        return QString(tr("Level") + " 3");
        break;
    case ID_Comm_Alarm_Level4:
        return QString(tr("Level") + " 4");
        break;

        break;
    case ID_Comm_Alarm_Level1_OnOff:
    case ID_Comm_Alarm_Level2_OnOff:
    case ID_Comm_Alarm_Level3_OnOff:
    case ID_Comm_Alarm_Level4_OnOff:
        return tr("On/Off");
        break;
    case ID_Comm_Alarm_Level1_Type:
    case ID_Comm_Alarm_Level2_Type:
    case ID_Comm_Alarm_Level3_Type:
    case ID_Comm_Alarm_Level4_Type:
        return tr("Type");
        break;
    case ID_Comm_Alarm_Level1_Value:
    case ID_Comm_Alarm_Level2_Value:
    case ID_Comm_Alarm_Level3_Value:
    case ID_Comm_Alarm_Level4_Value:
        return tr("Value");
        break;
    case ID_Comm_Alarm_Level1_Hysteresis:
    case ID_Comm_Alarm_Level2_Hysteresis:
    case ID_Comm_Alarm_Level3_Hysteresis:
    case ID_Comm_Alarm_Level4_Hysteresis:
        return tr("Hysteresis");
        break;
    case ID_Comm_Alarm_Level1_Logging:
    case ID_Comm_Alarm_Level2_Logging:
    case ID_Comm_Alarm_Level3_Logging:
    case ID_Comm_Alarm_Level4_Logging:
        return tr("Logging");
        break;
    case ID_Comm_Alarm_Level1_OutputType:
    case ID_Comm_Alarm_Level2_OutputType:
    case ID_Comm_Alarm_Level3_OutputType:
    case ID_Comm_Alarm_Level4_OutputType:
        return tr("Output type");
        break;
    case ID_Comm_Alarm_Level1_OutputNo:
    case ID_Comm_Alarm_Level2_OutputNo:
    case ID_Comm_Alarm_Level3_OutputNo:
    case ID_Comm_Alarm_Level4_OutputNo:
        return tr("Output No.");
        break;
    case ID_Comm_Alarm_Leve_Delay:                  //alarm-delay
        return tr("Alarm delay");
        break;
    case ID_Comm_Alarm_Leve_Delay_s:
        return tr("Second");
    case ID_Comm_Alarm_Leve_Delay_m:
        return tr("Minute");
    case ID_Comm_Alarm_Leve_Delay_h:
        return tr("Hour");
        break;
    default:
        break;
    }
    return NULL;
}

QString CfgConfig_Comm::getCommDispObjStr(quint32 ID, quint32 /*group*/)
{
    switch (ID) {
    case ID_Comm_Disp:
        return tr("Display settings");
        break;

    case ID_Comm_Disp_Color:
    case ID_Comm_Disp_Color_Value:
    case ID_Comm_DISP_ColorScaleBand_Color:
        return tr("Color");
        break;

    case ID_Comm_Disp_Tag:
        return tr("Tag");
        break;
    case ID_Comm_Disp_Tag_Char:
        return tr("Characters");
        break;
    case ID_Comm_Disp_Tag_No:
        return tr("No.");
        break;

    case ID_Comm_Disp_Zone:
        return tr("Zone");
        break;
    case ID_Comm_Disp_Zone_Upper:
        return tr("Upper");
        break;
    case ID_Comm_Disp_Zone_Lower:
        return tr("Lower");
        break;

    case ID_Comm_Disp_Scale:
        return tr("Scale");
        break;
    case ID_Comm_Disp_Scale_Pos:
        return tr("Position");
        break;
    case ID_Comm_Disp_Scale_Div:
        return tr("Division");
        break;

    case ID_Comm_DISP_Bar:
        return tr("Bar graph");
        break;
    case ID_Comm_DISP_Bar_Pos:
        return tr("Base position");
        break;
    case ID_Comm_DISP_Bar_Div:
        return tr("Division");
        break;

    case ID_Comm_DISP_Partial:
        return tr("Partial");
        break;
    case ID_Comm_DISP_Partial_OnOff:
        return tr("On/Off");
        break;
    case ID_Comm_DISP_Partial_Boundary:
        return tr("Boundary");
        break;
    case ID_Comm_DISP_Partial_Pos:
        return tr("Expand");
        break;

    case ID_Comm_DISP_ColorScaleBand:
        return tr("Color scale band");
        break;
    case ID_Comm_DISP_ColorScaleBand_PosLower:
        return tr("Display position Lower");
        break;
    case ID_Comm_DISP_ColorScaleBand_PosUpper:
        return tr("Display position Upper");
        break;
    case ID_Comm_DISP_ColorScaleBand_Mode:
        return tr("Band area");
        break;

    case ID_Comm_DISP_AlarmMark:              //报警标记分组
        return tr("Alarm point mark");
        break;
    case ID_Comm_DISP_AlarmMark_OnOff:        //报警标记开关，0-0ff, 1-on
        return tr("Indicate on Scale");
        break;
    case ID_Comm_DISP_AlarmMark_Type:     //报警标记类型
        return tr("Mark type");
        break;
    case ID_Comm_DISP_AlarmMark_Color1:       //报警颜色， Byte2-Red, Byte1-Green, Byte0-Blue
        return tr("Alarm 1 color");
        break;
    case ID_Comm_DISP_AlarmMark_Color2:
        return tr("Alarm 2 color");
        break;
    case ID_Comm_DISP_AlarmMark_Color3:
        return tr("Alarm 3 color");
        break;
    case ID_Comm_DISP_AlarmMark_Color4:
        return tr("Alarm 4 color");
        break;
    default:
        break;
    }
    return NULL;
}



/*******************************************  get comm obj value str *************************************************/

QString CfgConfig_Comm::getCommRangeObjValStr(quint32 ID, quint32 /*group*/, ConfigObject *cfg)
{
    switch (ID) {
    case ID_Comm_Range_Range_Switch:
    case ID_Comm_Range_Timer_Switch:
        return cfg->getData().toInt() == 0 ? tr("Off") : tr("On");
        break;
    case ID_Comm_Range_Range_Min:
    case ID_Comm_Range_Range_Max:
    {
        int dec = ConfigTreeObject::getConfigData(COMMChannel_Delegate_Range_Range_Decimal).toInt();
        float data = (float)cfg->getData().toInt() / ::pow10(dec);
        return getCommRangScaleUnit(data);
    }

    case ID_Comm_Range_Range_Unit:
        return getCommRangeScaleUnitValObjStr(cfg->getData().toString());
        break;

    case ID_Comm_Range_PowerOn_Val:     //电源接通时的值类型， 0-最终值，1-预设值，默认0
    case ID_Comm_Range_Timer_ResetVal:  //计时器满值设定值类型，0-最终值，1-预设值。默认0
        return cfg->getData().toInt() == 0 ? tr("Last value") : tr("Preset value");
        break;
    case ID_Comm_Range_Timer_Interval:     //预设值
        return cfg->getData().toString() + " " + tr("s");
        break;
    default:
        break;
    }
    return cfg->getData().toString();
}

QString CfgConfig_Comm::getCommAlarmObjValStr(quint32 ID, quint32 /*group*/, ConfigObject *cfg)
{
    QString cfgData = cfg->getData().toString();
    switch (ID) {
    case ID_Comm_Alarm_Level1_OnOff:      //alarm-level1
    case ID_Comm_Alarm_Level2_OnOff:
    case ID_Comm_Alarm_Level3_OnOff:
    case ID_Comm_Alarm_Level4_OnOff:
    case ID_Comm_Alarm_Level1_Logging:
    case ID_Comm_Alarm_Level2_Logging:
    case ID_Comm_Alarm_Level3_Logging:
    case ID_Comm_Alarm_Level4_Logging:
        return cfgData.toInt() == 0 ? tr("Off") : tr("On");
        break;
    case ID_Comm_Alarm_Level1_Type:
    case ID_Comm_Alarm_Level2_Type:
    case ID_Comm_Alarm_Level3_Type:
    case ID_Comm_Alarm_Level4_Type:
        return getCommAlarmTypeValObjStr(cfgData.toInt());
        break;

    case ID_Comm_Alarm_Level1_OutputNo:
    case ID_Comm_Alarm_Level2_OutputNo:
    case ID_Comm_Alarm_Level3_OutputNo:
    case ID_Comm_Alarm_Level4_OutputNo:
        return getCommChannelObjValStr(cfg->getData().toUInt());
        break;

    case ID_Comm_Alarm_Level1_Value:
    case ID_Comm_Alarm_Level1_Hysteresis:
    case ID_Comm_Alarm_Level2_Value:
    case ID_Comm_Alarm_Level2_Hysteresis:
    case ID_Comm_Alarm_Level3_Value:
    case ID_Comm_Alarm_Level3_Hysteresis:
    case ID_Comm_Alarm_Level4_Value:
    case ID_Comm_Alarm_Level4_Hysteresis:
    {
        int dec = ConfigTreeObject::getConfigData(COMMChannel_Delegate_Range_Range_Decimal).toInt();
        float data = (float)cfg->getData().toInt() / ::pow10(dec);
        return getCommRangScaleUnit(data);
    }
        break;

    case ID_Comm_Alarm_Level1_OutputType:
    case ID_Comm_Alarm_Level2_OutputType:
    case ID_Comm_Alarm_Level3_OutputType:
    case ID_Comm_Alarm_Level4_OutputType:
    {
        int outputType = cfgData.toInt();
        if (outputType == 0)    //报警输出类型， 0-off, 1-Relay(继电器), 2-Switch(内部开关)
            return tr("Off");
        else if (outputType == 1)
            return tr("Relay");
        else
            return tr("Internal switch");
    }
        break;
    default:
        break;
    }
    return cfgData;
}

QString CfgConfig_Comm::getCommDispObjValStr(quint32 ID, quint32 /*group*/, ConfigObject *cfg)
{
    QString cfgData = cfg->getData().toString();
    switch (ID) {
    case ID_Comm_Disp_Color_Value:        //通道颜色，Byte2-Red, Byte1-Green, Byte0-Blue
    case ID_Comm_DISP_ColorScaleBand_Color:       //彩色标尺带颜色，Byte2-Red, Byte1-Green, Byte0-Blue
    case ID_Comm_DISP_AlarmMark_Color1:       //报警1-4颜色， Byte2-Red, Byte1-Green, Byte0-Blue
    case ID_Comm_DISP_AlarmMark_Color2:
    case ID_Comm_DISP_AlarmMark_Color3:
    case ID_Comm_DISP_AlarmMark_Color4:
        return getCommDisplayColorValueObjStr(cfg->getData().toUInt());
        break;
    case ID_Comm_Disp_Tag_Char:       //标记字符串，中英支持，最多32个半角字符
    case ID_Comm_Disp_Tag_No:         //标记号码，符号，最多16个半角字符
        return QString(tr("%1").arg(cfgData));
        break;

    case ID_Comm_Disp_Zone_Upper:       //显示区域上限，5-100,单位%
    case ID_Comm_Disp_Zone_Lower:       //显示区域上限，0-95,单位%
    case ID_Comm_DISP_Partial_Pos:       //部分压缩放大位置，1-99, 单位：%，默认50
        return QString("%1 ").arg(cfgData) + QString("%");
        break;
    case ID_Comm_Disp_Scale_Pos:
    {
        int type = cfgData.toInt();
        if (type == 0) return tr("Off");
        return cfgData;
    }
        break;

    case ID_Comm_DISP_Bar_Pos:      //棒图基准位置， 0-Lower, 1-Center, 2-Upper
    {
        int pos = cfgData.toInt();
        if (pos == 0)
            return tr("Lower");
        else if (pos == 1)
            return tr("Center");
        else
            return tr("Upper");
    }
        break;

    case ID_Comm_DISP_Partial_OnOff:       //部分压缩放大开关,0-off, 1-on
    case ID_Comm_DISP_AlarmMark_OnOff:       //报警标记开关，0-0ff, 1-on
        return cfgData.toInt() == 0 ? tr("Off") : tr("On");
        break;

    case ID_Comm_DISP_ColorScaleBand_Mode:       //彩色标尺带模式， 0-off, 1-内侧， 2-外侧.
    {
        int pos = cfgData.toInt();
        if (pos == 0)
            return tr("Off");
        else if (pos == 1)
            return tr("In");
        else
            return tr("Out");
    }
        break;

    case ID_Comm_DISP_AlarmMark_Type:       //报警标记类型，0-报警， 1-固定
        return cfgData.toInt() == 0 ? tr("Alarm") : tr("Fixed");
        break;

    case ID_Comm_DISP_ColorScaleBand_PosLower:       //彩色标尺带显示位置下限
    case ID_Comm_DISP_ColorScaleBand_PosUpper:       //彩色标尺带显示位置上限
    case ID_Comm_DISP_Partial_Boundary:
    {
        int dec = ConfigTreeObject::getConfigData(COMMChannel_Delegate_Range_Range_Decimal).toInt();
        float data = (float)cfg->getData().toInt() / ::pow10(dec);
        QString strShowOn=  getCommRangScaleUnit(data);
        DigitalOperation digOpe;
        return digOpe.getAllowedData(ID, strShowOn, cfg);
    }
        break;

    case ID_Comm_Disp_Scale_Div:
    {
        int data = cfg->getData().toUInt();
        if (data == 13)
            return "C10";
        else
            return QString::number(data);
    }
        break;
    default:
        break;
    }
    return cfgData;
}


QString CfgConfig_Comm::getCommRangScaleUnit(float data)
{
    QString unit = ConfigTreeObject::getConfigData(COMMChannel_Delegate_Range_Range_Unit).toString();
    quint32 decCnt = ConfigTreeObject::getConfigData(COMMChannel_Delegate_Range_Range_Decimal).toInt();

    return QString::number(data, 'f', decCnt) + " " + unit;
}

QString CfgConfig_Comm::getCommRangeScaleUnitValObjStr(QString unit)
{
    QStringList str;
    str << COMMChannel_Delegate_Alarm_1_Value << COMMChannel_Delegate_Alarm_1_Hysteresis
        << COMMChannel_Delegate_Alarm_2_Value << COMMChannel_Delegate_Alarm_2_Hysteresis
        << COMMChannel_Delegate_Alarm_3_Value << COMMChannel_Delegate_Alarm_3_Hysteresis
        << COMMChannel_Delegate_Alarm_4_Value << COMMChannel_Delegate_Alarm_4_Hysteresis
        <<COMMChannel_Delegate_Range_Range_Upper << COMMChannel_Delegate_Range_Range_Lower
       <<COMMChannel_Delegate_Diaplay_Band_Lower << COMMChannel_Delegate_Diaplay_Band_Upper
      <<COMMChannel_Delegate_Diaplay_Partial_Boundary;

    for (int i = str.count() - 1; i >= 0; i--)
    {
        QString strShow = ConfigTreeObject::getConfigObject(str[i])->getStrShowON();
        QStringList on = strShow.split(" ");
        ConfigTreeObject::getConfigObject(str[i])->setStrShowOn(on[0] + " " + unit);
    }

    return unit;
}

/*
 * 通过DI通道和报警等级号码得到相对应通道报警的描述
*/
QString CfgConfig_Comm::getCommAlarmTypeValObjStr(quint32 type)
{
    switch (type) {
    case Alarm_Type_Off:
        return tr("Off");
        break;
    case Alarm_Type_H:                                  //上限
        return tr("H : High limit");
        break;
    case Alarm_Type_L:                                  //下限
        return tr("L : Low limit");
        break;
    case Alarm_Type_R:                                                  //变化率上升限
        return tr("R : High limit on rate-of-change");
        break;
    case Alarm_Type_r:                                                   //变化率下升限
        return tr("r : Low limit on rate-of-change");
        break;
    case Alarm_Type_T:                                                  //延迟上限
        return tr("T : Delay hight limit");
        break;
    case Alarm_Type_t:                                                  //延迟下限
        return tr("t : Delay low limit");
        break;
    case Alarm_Type_h:                                                  //差值上限， Delta运算时有效
        return tr("t : Difference hight limit");
        break;
    case Alarm_Type_l:
        return tr("l : Difference low limit");
        break;
    default:
        break;
    }
    return NULL;
}

/*
 * 把从配置中得到的颜色配置值转换为界面label显示的样式字符串返回
*/
QString CfgConfig_Comm::getCommDisplayColorValueObjStr(quint32 x)
{
    QColor colorRGBx(COLOR_RED(x), COLOR_GREEN(x), COLOR_BLUE(x));
    QString str = QString("QLabel {background-color: %1}").arg(colorRGBx.name());
    return str;
}



/*******************************************  save *************************************************/

void CfgConfig_Comm::setCommRangeData(quint32 ID, quint32 group)
{
    int index = CfgChannel::instance()->getExistChannelComm().values().indexOf(group);
    QString chanNum = CfgChannel::instance()->getExistChannelComm().keys().at(index);

    COMM_CONFIG *CommCfg = CfgChannel::instance()->getCommConfig(group);
    switch (ID) {
    case ID_Comm_Range_Range_Switch:
        CommCfg->rangeConfig.rangeSwtich = ConfigTreeObject::getConfigData(QString(COMMChannel_Setting_Range_Range_OnOrOff).arg(chanNum)).toInt();
        break;
    case ID_Comm_Range_Range_Pos:
        CommCfg->rangeConfig.rangePos = ConfigTreeObject::getConfigData(QString(COMMChannel_Setting_Range_Range_Decimal).arg(chanNum)).toInt();
        break;
    case ID_Comm_Range_Range_Min:
        CommCfg->rangeConfig.rangeMin = ConfigTreeObject::getConfigData(QString(COMMChannel_Setting_Range_Range_Lower).arg(chanNum)).toInt();
        break;
    case ID_Comm_Range_Range_Max:
        CommCfg->rangeConfig.rangeMax = ConfigTreeObject::getConfigData(QString(COMMChannel_Setting_Range_Range_Upper).arg(chanNum)).toInt();
        break;

    case ID_Comm_Range_Range_Unit:
        setStrToChar(CommCfg->rangeConfig.rangeUnit,ConfigTreeObject::getConfigData(QString(COMMChannel_Setting_Range_Range_Unit).arg(chanNum)).toString(), 8);
        break;
    case ID_Comm_Range_PowerOn_Val:
        CommCfg->rangeConfig.rangePowerOnVal = ConfigTreeObject::getConfigData(QString(COMMChannel_Setting_Range_Power).arg(chanNum)).toUInt();
        break;
    case ID_Comm_Range_Pre_Val:
        CommCfg->rangeConfig.rangePreVal = ConfigTreeObject::getConfigData\
                (QString(COMMChannel_Setting_Range_Value).arg(chanNum)).toFloat();
        break;
    case ID_Comm_Range_Timer_Switch:
        CommCfg->rangeConfig.rangeTimerSwitch = ConfigTreeObject::getConfigData(QString(COMMChannel_Setting_Range_WatchDog_OnOrOff).arg(chanNum)).toInt();
        break;
    case ID_Comm_Range_Timer_Interval:
        CommCfg->rangeConfig.rangeTimerInterval = ConfigTreeObject::getConfigData(QString(COMMChannel_Setting_Range_WatchDog_Timer).arg(chanNum)).toInt();
        break;
    case ID_Comm_Range_Timer_ResetVal:
        CommCfg->rangeConfig.rangeTimerResetVal = ConfigTreeObject::getConfigData(QString(COMMChannel_Setting_Range_WatchDog_Value).arg(chanNum)).toInt();
        break;
    default:
        break;
    }
}



void CfgConfig_Comm::setCommAlarmData(quint32 ID, quint32 group)
{
    int index = CfgChannel::instance()->getExistChannelComm().values().indexOf(group);
    QString chanNum = CfgChannel::instance()->getExistChannelComm().keys().at(index);
    COMM_CONFIG * CommCfg = CfgChannel::instance()->getCommConfig(group);
    switch (ID) {
    case ID_Comm_Alarm_Level1_OnOff:
        CommCfg->alarmConfig.alarmOnOff[0] = ConfigTreeObject::getConfigData(QString(COMMChannel_Setting_Alarm_1_OnOrOff).arg(chanNum)).toUInt();
        break;
    case ID_Comm_Alarm_Level1_Type:
        CommCfg->alarmConfig.alarmType[0] = (Alarm_Type)ConfigTreeObject::getConfigData(QString(COMMChannel_Setting_Alarm_1_Type).arg(chanNum)).toUInt();
        break;
    case ID_Comm_Alarm_Level1_Value:
        CommCfg->alarmConfig.alarmValue[0] = ConfigTreeObject::getConfigData(QString(COMMChannel_Setting_Alarm_1_Value).arg(chanNum)).toInt();
        break;
    case ID_Comm_Alarm_Level1_Hysteresis:
        CommCfg->alarmConfig.alarmHysteresis[0] = ConfigTreeObject::getConfigData(QString(COMMChannel_Setting_Alarm_1_Hysteresis).arg(chanNum)).toInt();
        break;
    case ID_Comm_Alarm_Level1_Logging:
        CommCfg->alarmConfig.alarmLogging[0] = ConfigTreeObject::getConfigData(QString(COMMChannel_Setting_Alarm_1_Logging).arg(chanNum)).toUInt();
    case ID_Comm_Alarm_Level1_OutputType:
        CommCfg->alarmConfig.alarmOutputType[0] = ConfigTreeObject::getConfigData(QString(COMMChannel_Setting_Alarm_1_OutputType).arg(chanNum)).toUInt();
        break;
    case ID_Comm_Alarm_Level1_OutputNo:
        CommCfg->alarmConfig.alarmOuputNo[0] = ConfigTreeObject::getConfigData(QString(COMMChannel_Setting_Alarm_1_OutputNo).arg(chanNum)).toUInt();
        break;

    case ID_Comm_Alarm_Level2_OnOff:
        CommCfg->alarmConfig.alarmOnOff[1] = ConfigTreeObject::getConfigData(QString(COMMChannel_Setting_Alarm_2_OnOrOff).arg(chanNum)).toUInt();
        break;
    case ID_Comm_Alarm_Level2_Type:
        CommCfg->alarmConfig.alarmType[1] = (Alarm_Type)ConfigTreeObject::getConfigData(QString(COMMChannel_Setting_Alarm_2_Type).arg(chanNum)).toUInt();
        break;
    case ID_Comm_Alarm_Level2_Value:
        CommCfg->alarmConfig.alarmValue[1] = ConfigTreeObject::getConfigData(QString(COMMChannel_Setting_Alarm_2_Value).arg(chanNum)).toInt();
        break;
    case ID_Comm_Alarm_Level2_Hysteresis:
        CommCfg->alarmConfig.alarmHysteresis[1] = ConfigTreeObject::getConfigData(QString(COMMChannel_Setting_Alarm_2_Hysteresis).arg(chanNum)).toInt();
        break;
    case ID_Comm_Alarm_Level2_Logging:
        CommCfg->alarmConfig.alarmLogging[1] = ConfigTreeObject::getConfigData(QString(COMMChannel_Setting_Alarm_2_Logging).arg(chanNum)).toUInt();
        break;
    case ID_Comm_Alarm_Level2_OutputType:
        CommCfg->alarmConfig.alarmOutputType[1] = ConfigTreeObject::getConfigData(QString(COMMChannel_Setting_Alarm_2_OutputType).arg(chanNum)).toUInt();
    case ID_Comm_Alarm_Level2_OutputNo:
        CommCfg->alarmConfig.alarmOuputNo[1] = ConfigTreeObject::getConfigData(QString(COMMChannel_Setting_Alarm_2_OutputNo).arg(chanNum)).toUInt();
        break;

    case ID_Comm_Alarm_Level3_OnOff:
        CommCfg->alarmConfig.alarmOnOff[2] = ConfigTreeObject::getConfigData(QString(COMMChannel_Setting_Alarm_3_OnOrOff).arg(chanNum)).toUInt();
        break;
    case ID_Comm_Alarm_Level3_Type:
        CommCfg->alarmConfig.alarmType[2] = (Alarm_Type)ConfigTreeObject::getConfigData(QString(COMMChannel_Setting_Alarm_3_Type).arg(chanNum)).toUInt();
        break;
    case ID_Comm_Alarm_Level3_Value:
        CommCfg->alarmConfig.alarmValue[2] = ConfigTreeObject::getConfigData(QString(COMMChannel_Setting_Alarm_3_Value).arg(chanNum)).toInt();
        break;
    case ID_Comm_Alarm_Level3_Hysteresis:
        CommCfg->alarmConfig.alarmHysteresis[2] = ConfigTreeObject::getConfigData(QString(COMMChannel_Setting_Alarm_3_Hysteresis).arg(chanNum)).toInt();
        break;
    case ID_Comm_Alarm_Level3_Logging:
        CommCfg->alarmConfig.alarmLogging[2] = ConfigTreeObject::getConfigData(QString(COMMChannel_Setting_Alarm_3_Logging).arg(chanNum)).toUInt();
        break;
    case ID_Comm_Alarm_Level3_OutputType:
        CommCfg->alarmConfig.alarmOutputType[2] = ConfigTreeObject::getConfigData(QString(COMMChannel_Setting_Alarm_3_OutputType).arg(chanNum)).toUInt();
    case ID_Comm_Alarm_Level3_OutputNo:
        CommCfg->alarmConfig.alarmOuputNo[2] = ConfigTreeObject::getConfigData(QString(COMMChannel_Setting_Alarm_3_OutputNo).arg(chanNum)).toUInt();
        break;

    case ID_Comm_Alarm_Level4_OnOff:
        CommCfg->alarmConfig.alarmOnOff[3] = ConfigTreeObject::getConfigData(QString(COMMChannel_Setting_Alarm_4_OnOrOff).arg(chanNum)).toUInt();
        break;
    case ID_Comm_Alarm_Level4_Type:
        CommCfg->alarmConfig.alarmType[3] = (Alarm_Type)ConfigTreeObject::getConfigData(QString(COMMChannel_Setting_Alarm_4_Type).arg(chanNum)).toUInt();
        break;
    case ID_Comm_Alarm_Level4_Value:
        CommCfg->alarmConfig.alarmValue[3] = ConfigTreeObject::getConfigData(QString(COMMChannel_Setting_Alarm_4_Value).arg(chanNum)).toInt();
        break;
    case ID_Comm_Alarm_Level4_Hysteresis:
        CommCfg->alarmConfig.alarmHysteresis[3] = ConfigTreeObject::getConfigData(QString(COMMChannel_Setting_Alarm_4_Hysteresis).arg(chanNum)).toInt();
        break;
    case ID_Comm_Alarm_Level4_Logging:
        CommCfg->alarmConfig.alarmLogging[3] = ConfigTreeObject::getConfigData(QString(COMMChannel_Setting_Alarm_4_Logging).arg(chanNum)).toUInt();
        break;
    case ID_Comm_Alarm_Level4_OutputType:
        CommCfg->alarmConfig.alarmOutputType[3] = ConfigTreeObject::getConfigData(QString(COMMChannel_Setting_Alarm_4_OutputType).arg(chanNum)).toUInt();
    case ID_Comm_Alarm_Level4_OutputNo:
        CommCfg->alarmConfig.alarmOuputNo[3] = ConfigTreeObject::getConfigData(QString(COMMChannel_Setting_Alarm_4_OutputNo).arg(chanNum)).toUInt();
        break;
    case ID_Comm_Alarm_Leve_Delay_s:
        CommCfg->alarmConfig.alarmDelay[0] = ConfigTreeObject::getConfigData(QString(COMMChannel_Setting_Alarm_Delay_Secend).arg(chanNum)).toUInt();
        break;
    case ID_Comm_Alarm_Leve_Delay_m:
        CommCfg->alarmConfig.alarmDelay[1] = ConfigTreeObject::getConfigData(QString(COMMChannel_Setting_Alarm_Delay_Minute).arg(chanNum)).toUInt();
        break;
    case ID_Comm_Alarm_Leve_Delay_h:
        CommCfg->alarmConfig.alarmDelay[2] = ConfigTreeObject::getConfigData(QString(COMMChannel_Setting_Alarm_Delay_Hour).arg(chanNum)).toUInt();
        break;
    default:
        break;
    }
}

void CfgConfig_Comm::setCommDispData(quint32 ID, quint32 group)
{
    int index = CfgChannel::instance()->getExistChannelComm().values().indexOf(group);
    QString chanNum = CfgChannel::instance()->getExistChannelComm().keys().at(index);
    COMM_CONFIG *CommCfg = CfgChannel::instance()->getCommConfig(group);
    switch (ID) {
    case ID_Comm_Disp_Color_Value:
        CommCfg->displayConfig.displayColor = ConfigTreeObject::getConfigData(QString(COMMChannel_Setting_Diaplay_Color).arg(chanNum)).toUInt();
        break;
    case ID_Comm_Disp_Tag_Char:
        setStrToChar(CommCfg->displayConfig.displayTagChar, ConfigTreeObject::getConfigData(QString(COMMChannel_Setting_Diaplay_Tag_Characters).arg(chanNum)).toString(), 32);
        break;
    case ID_Comm_Disp_Tag_No:
        setStrToChar(CommCfg->displayConfig.displayTagNo, ConfigTreeObject::getConfigData(QString(COMMChannel_Setting_Diaplay_Tag_No).arg(chanNum)).toString(), 16);
        break;
    case ID_Comm_Disp_Zone_Upper:
        CommCfg->displayConfig.displayZoneUpper = ConfigTreeObject::getConfigData(QString(COMMChannel_Setting_Diaplay_Zone_Upper).arg(chanNum)).toUInt();
        break;
    case ID_Comm_Disp_Zone_Lower:
        CommCfg->displayConfig.dispalyZoneLower= ConfigTreeObject::getConfigData(QString(COMMChannel_Setting_Diaplay_Zone_Lower).arg(chanNum)).toUInt();
        break;
    case ID_Comm_Disp_Scale_Pos:
        CommCfg->displayConfig.displayScalePos = ConfigTreeObject::getConfigData(QString(COMMChannel_Setting_Diaplay_Scale_Postion).arg(chanNum)).toUInt();
        break;
    case ID_Comm_Disp_Scale_Div:
        CommCfg->displayConfig.displayScaleDiv = ConfigTreeObject::getConfigData(QString(COMMChannel_Setting_Diaplay_Scale_Division).arg(chanNum)).toUInt();
        break;
    case ID_Comm_DISP_Bar_Pos:
        CommCfg->displayConfig.displayBarPos = ConfigTreeObject::getConfigData(QString(COMMChannel_Setting_Diaplay_Bar_Position).arg(chanNum)).toUInt();
        break;
    case ID_Comm_DISP_Bar_Div:
        CommCfg->displayConfig.displayBarDiv = ConfigTreeObject::getConfigData(QString(COMMChannel_Setting_Diaplay_Bar_Division).arg(chanNum)).toUInt();
        break;

    case ID_Comm_DISP_Partial_OnOff:
        CommCfg->displayConfig.displayPartialOnOff = ConfigTreeObject::getConfigData(QString(COMMChannel_Setting_Diaplay_Partial_OnOrOff).arg(chanNum)).toUInt();
        break;
    case ID_Comm_DISP_Partial_Boundary:
    {
        int decCnt= ConfigTreeObject::getConfigData(QString(COMMChannel_Setting_Range_Range_Decimal).arg(chanNum)).toInt();
        CommCfg->displayConfig.displayPartialBoundary = (float)ConfigTreeObject::getConfigData(QString(COMMChannel_Setting_Diaplay_Partial_Boundary).arg(chanNum)).toInt() / ::pow10(decCnt);
    }
        break;
    case ID_Comm_DISP_Partial_Pos:
        CommCfg->displayConfig.displayPartialPos = ConfigTreeObject::getConfigData(QString(COMMChannel_Setting_Diaplay_Partial_Expand).arg(chanNum)).toUInt();
        break;

    case ID_Comm_DISP_ColorScaleBand_Color:
        CommCfg->displayConfig.displayColorbandColor = ConfigTreeObject::getConfigData(QString(COMMChannel_Setting_Diaplay_Band_Color).arg(chanNum)).toUInt();
        break;
    case ID_Comm_DISP_ColorScaleBand_PosLower:
    {
        int decCnt= ConfigTreeObject::getConfigData(QString(COMMChannel_Setting_Range_Range_Decimal).arg(chanNum)).toInt();
        CommCfg->displayConfig.displayColorbandPosLower = (float)ConfigTreeObject::getConfigData(QString(COMMChannel_Setting_Diaplay_Band_Lower).arg(chanNum)).toInt() / ::pow10(decCnt);
    }
        break;
    case ID_Comm_DISP_ColorScaleBand_PosUpper:
    {
        int decCnt= ConfigTreeObject::getConfigData(QString(COMMChannel_Setting_Range_Range_Decimal).arg(chanNum)).toInt();
        CommCfg->displayConfig.displayColorbandPosUpper = (float)ConfigTreeObject::getConfigData(QString(COMMChannel_Setting_Diaplay_Band_Upper).arg(chanNum)).toInt() / ::pow10(decCnt);
    }
        break;
    case ID_Comm_DISP_ColorScaleBand_Mode:
        CommCfg->displayConfig.displayColorbandMode = ConfigTreeObject::getConfigData(QString(COMMChannel_Setting_Diaplay_Band_Area).arg(chanNum)).toUInt();
        break;

    case ID_Comm_DISP_AlarmMark_OnOff:
        CommCfg->displayConfig.displayAlarmMarkOnOff = ConfigTreeObject::getConfigData(QString(COMMChannel_Setting_Diaplay_Mark_Scale).arg(chanNum)).toUInt();
        break;
    case ID_Comm_DISP_AlarmMark_Type:
        CommCfg->displayConfig.displayAlarmMarkType = ConfigTreeObject::getConfigData(QString(COMMChannel_Setting_Diaplay_Mark_Type).arg(chanNum)).toUInt();
        break;
    case ID_Comm_DISP_AlarmMark_Color1:
        CommCfg->displayConfig.displayAlarmMarkColor[0] = ConfigTreeObject::getConfigData(QString(COMMChannel_Setting_Diaplay_Mark_Alarm1).arg(chanNum)).toUInt();
        break;
    case ID_Comm_DISP_AlarmMark_Color2:
        CommCfg->displayConfig.displayAlarmMarkColor[1] = ConfigTreeObject::getConfigData(QString(COMMChannel_Setting_Diaplay_Mark_Alarm2).arg(chanNum)).toUInt();
        break;
    case ID_Comm_DISP_AlarmMark_Color3:
        CommCfg->displayConfig.displayAlarmMarkColor[2] = ConfigTreeObject::getConfigData(QString(COMMChannel_Setting_Diaplay_Mark_Alarm3).arg(chanNum)).toUInt();
        break;
    case ID_Comm_DISP_AlarmMark_Color4:
        CommCfg->displayConfig.displayAlarmMarkColor[3] = ConfigTreeObject::getConfigData(QString(COMMChannel_Setting_Diaplay_Mark_Alarm4).arg(chanNum)).toUInt();
        break;
    default:
        break;
    }
}





//把32位通道号转化为相应的四位通道     quint32 --> QString
QString CfgConfig_Comm::getCommChannelObjValStr(quint32 num)
{
    if (num == 0)
        return "0001";

    if (num >= 1 && num <= 100)
        return QString("%1").arg(num, 4, 10, QChar('0'));
    else
    {
        if (CfgChannel::instance()->getExistChannelDO().isEmpty())
            return "0001";  //switch
        else
        {
            return CfgChannel::instance()->getExistChannelDO().key(num);
        }
    }
}



/*
 * 把uchar *类型转化为QString类型
*/
QVariant CfgConfig_Comm::getCharToString(uchar *str)
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
void CfgConfig_Comm::setStrToChar(uchar * uData, QString str, int len)
{
    memset(uData, 0, len);
    QByteArray baUnit = str.toLocal8Bit();
    char* valueUnit = baUnit.data();
    while (*valueUnit != '\0') {
        *uData++ = *valueUnit++;
    }
//    memcpy(uData, valueUnit, strlen(valueUnit));
}


/**
 * @brief 把float类型的数据乘以10的精度次方后转换为int类型返回，该函数避免了用float类型直接乘法后数值有差异
 * @param data  : float类型的数据
 * @param pos   : 精度
 * @return      : float类型的数据乘以10的精度次方后转换为int类型值
 */
int CfgConfig_Comm::getFloatToInt(float data, int pos)
{
    QString dataStr = QString::number(data);
    QStringList dataList = dataStr.split(".");
    if (dataList.count() == 1)
    {
        if (pos == 0)
            return dataStr.toInt();
        else
            return QString(dataList[0]).append("%1").arg(0, pos, 10, QChar('0')).toInt();
    }
    else
    {
        int cnt = dataList[1].count();
        if (pos - cnt <= 0)
            return QString("%1%2").arg(dataList[0]).arg(dataList[1].left(pos)).toInt();
        else
            return QString("%1%2%3").arg(dataList[0]).arg(dataList[1]).arg(0, pos - cnt, 10, QChar('0')).toInt();
    }
}

