#include "cfgconfig_math.h"
#include <QVariant>
#include "./GlobalData/configobject.h"
#include "./GlobalData/cfgchannel.h"
#include "globalDef.h"
#include "./Common/digitaloperation.h"



CfgConfig_Math *CfgConfig_Math::instance()
{
    static CfgConfig_Math data;
    return &data;
}



CfgConfig_Math::CfgConfig_Math(QObject *parent) :
    QObject(parent)
{
}




QVariant CfgConfig_Math::getMathData(quint32 ID, quint32 group)
{
    if (ID >= Math_Exp_Start_ID && ID <= Math_Exp_End_ID)
        return getMathExpData(ID, group);
    else if (ID >= Math_Alarm_Start_ID && ID <= Math_Alarm_End_ID)
        return getMathAlarmData(ID, group);
    else if (ID >= Math_Disp_Start_ID && ID <= Math_Disp_End_ID)
        return getMathDispData(ID, group);
    else if (ID >= Math_Const_Start_ID && ID <= Math_Const_End_ID)
        return getMathConstData(ID, group);
    else if (ID >= Math_Action_Start_ID && ID <= Math_Action_End_ID)
        return getMathActionData(ID, group);
    return QVariant();
}


QString CfgConfig_Math::getMathObjStr(quint32 ID, quint32 group)
{
    if (ID >= Math_Start_ID && ID <= Math_Exp_End_ID)
        return getMathExpObjStr(ID, group);
    else if (ID >= Math_Alarm_Start_ID && ID <= Math_Alarm_End_ID)
        return getMathAlarmObjStr(ID, group);
    else if (ID >= Math_Disp_Start_ID && ID <= Math_Disp_End_ID)
        return getMathDispObjStr(ID, group);
    else if (ID >= Math_Const_Start_ID && ID <= Math_Const_End_ID)
        return getMathConstObjStr(ID, group);
    else if (ID >= Math_Action_Start_ID && ID <= Math_Action_End_ID)
        return getMathActionObjStr(ID, group);
    return NULL;
}

QString CfgConfig_Math::getMathObjValStr(quint32 ID, quint32 group, ConfigObject *cfg)
{
    if (ID >= Math_Exp_Start_ID && ID <= Math_Exp_End_ID)
        return getMathExpObjValStr(ID, group, cfg);
    else if (ID >= Math_Alarm_Start_ID && ID <= Math_Alarm_End_ID)
        return getMathAlarmObjValStr(ID, group, cfg);
    else if (ID >= Math_Disp_Start_ID && ID <= Math_Disp_End_ID)
        return getMathDispObjValStr(ID, group, cfg);
    else if (ID >= Math_Const_Start_ID && ID <= Math_Const_End_ID)
        return getMathConstObjValStr(ID, group, cfg);
    else if (ID >= Math_Action_Start_ID && ID <= Math_Action_End_ID)
        return getMathActionObjValStr(ID, group, cfg);
    return NULL;
}


void CfgConfig_Math::setMathData(quint32 ID, quint32 group)
{
    if (ID >= Math_Exp_Start_ID && ID <= Math_Exp_End_ID)
        setMathExpData(ID, group);
    else if (ID >= Math_Alarm_Start_ID && ID <= Math_Alarm_End_ID)
        setMathAlarmData(ID, group);
    else if (ID >= Math_Disp_Start_ID && ID <= Math_Disp_End_ID)
        setMathDispData(ID, group);
    else if (ID >= Math_Const_Start_ID && ID <= Math_Const_End_ID)
        setMathConstData(ID, group);
    else if (ID >= Math_Action_Start_ID && ID <= Math_Action_End_ID)
        setMathActionData(ID, group);
}



/*********************************************   get data   **********************************************/


QVariant CfgConfig_Math::getMathExpData(quint32 ID, quint32 group)
{
    MATH_CONFIG MathCfg = *CfgChannel::instance()->getMathConfig(group);
    switch (ID) {
    case ID_Math_Exp_MathRange_Switch:
        return MathCfg.expressConfig.expSwtich;
        break;
    case ID_Math_Exp_MathRange_Pos:
        return MathCfg.expressConfig.expPos;
        break;
    case ID_Math_Exp_MathRange_Min:
        return MathCfg.expressConfig.expMin;
        break;
    case ID_Math_Exp_MathRange_Max:
        return MathCfg.expressConfig.expMax;
        break;
    case ID_Math_Exp_MathRange_Exp:
        return getCharToString(MathCfg.expressConfig.expExpression);
        break;
    case ID_Math_Exp_MathRange_Unit:
        return getCharToString(MathCfg.expressConfig.expUnit);
        break;

    case ID_Math_Exp_Tlog_Type:
        return MathCfg.expressConfig.expTlogType;
        break;
    case ID_Math_Exp_Tlog_NO:
        return MathCfg.expressConfig.expTlogNo;
        break;
    case ID_Math_Exp_Tlog_SumScale:
        return MathCfg.expressConfig.expTlogSumScale;
        break;
    case ID_Math_Exp_Tlog_Reset:
        return MathCfg.expressConfig.expTlogReset;
        break;

    case ID_Math_Exp_Roll_Switch:
        return MathCfg.expressConfig.expRollingAvgSwtich;
        break;
    case ID_Math_Exp_Roll_Interval:
        return getRollingAvgIntervalData(MathCfg.expressConfig.expRollingAvgInterval);
        break;
    case ID_Math_Exp_Roll_AvgNumber:
        return MathCfg.expressConfig.expRollingAvgNumber;
        break;


    default:
        break;
    }
    return QVariant();
}

QVariant CfgConfig_Math::getMathAlarmData(quint32 ID, quint32 group)
{
    MATH_CONFIG MathCfg = *CfgChannel::instance()->getMathConfig(group);

    switch (ID) {
    case ID_Math_Alarm_Level1_OnOff:
        return MathCfg.alarmConfig.alarmOnOff[0];
        break;
    case ID_Math_Alarm_Level1_Value:
        return MathCfg.alarmConfig.alarmValue[0];
        break;
    case ID_Math_Alarm_Level1_Hysteresis:
        return MathCfg.alarmConfig.alarmHysteresis[0];
        break;
    case ID_Math_Alarm_Level1_Logging:
        return MathCfg.alarmConfig.alarmLogging[0];
        break;
    case ID_Math_Alarm_Level1_OutputType:
        return MathCfg.alarmConfig.alarmOutputType[0];
        break;
    case ID_Math_Alarm_Level1_OutputNo:
        return MathCfg.alarmConfig.alarmOuputNo[0];
        break;

    case ID_Math_Alarm_Level2_OnOff:
        return MathCfg.alarmConfig.alarmOnOff[1];
        break;
    case ID_Math_Alarm_Level2_Value:
        return MathCfg.alarmConfig.alarmValue[1];
        break;
    case ID_Math_Alarm_Level2_Hysteresis:
        return MathCfg.alarmConfig.alarmHysteresis[1];
        break;
    case ID_Math_Alarm_Level2_Logging:
        return MathCfg.alarmConfig.alarmLogging[1];
        break;
    case ID_Math_Alarm_Level2_OutputType:
        return MathCfg.alarmConfig.alarmOutputType[1];
        break;
    case ID_Math_Alarm_Level2_OutputNo:
        return MathCfg.alarmConfig.alarmOuputNo[1];
        break;

    case ID_Math_Alarm_Level3_OnOff:
        return MathCfg.alarmConfig.alarmOnOff[2];
        break;
    case ID_Math_Alarm_Level3_Type:
        return MathCfg.alarmConfig.alarmType[2];
        break;
    case ID_Math_Alarm_Level3_Value:
        return MathCfg.alarmConfig.alarmValue[2];
        break;
    case ID_Math_Alarm_Level3_Hysteresis:
        return MathCfg.alarmConfig.alarmHysteresis[2];
        break;
    case ID_Math_Alarm_Level3_Logging:
        return MathCfg.alarmConfig.alarmLogging[2];
        break;
    case ID_Math_Alarm_Level3_OutputType:
        return MathCfg.alarmConfig.alarmOutputType[2];
        break;
    case ID_Math_Alarm_Level3_OutputNo:
        return MathCfg.alarmConfig.alarmOuputNo[2];
        break;

    case ID_Math_Alarm_Level4_OnOff:
        return MathCfg.alarmConfig.alarmOnOff[3];
        break;
    case ID_Math_Alarm_Level4_Type:
        return MathCfg.alarmConfig.alarmType[3];
        break;
    case ID_Math_Alarm_Level4_Value:
        return MathCfg.alarmConfig.alarmValue[3];
        break;
    case ID_Math_Alarm_Level4_Hysteresis:
        return MathCfg.alarmConfig.alarmHysteresis[3];
        break;
    case ID_Math_Alarm_Level4_Logging:
        return MathCfg.alarmConfig.alarmLogging[3];
        break;
    case ID_Math_Alarm_Level4_OutputType:
        return MathCfg.alarmConfig.alarmOutputType[3];
        break;
    case ID_Math_Alarm_Level4_OutputNo:
        return MathCfg.alarmConfig.alarmOuputNo[3];
        break;


    case ID_Math_Alarm_Leve_Delay_s:
        return MathCfg.alarmConfig.alarmDelay[0];
        break;
    case ID_Math_Alarm_Leve_Delay_m:
        return MathCfg.alarmConfig.alarmDelay[1];
        break;
    case ID_Math_Alarm_Leve_Delay_h:
        return MathCfg.alarmConfig.alarmDelay[2];
        break;
    default:
        break;
    }
    return QVariant();
}

QVariant CfgConfig_Math::getMathDispData(quint32 ID, quint32 group)
{
    MATH_CONFIG MathCfg = *CfgChannel::instance()->getMathConfig(group);
    switch (ID) {
    case ID_Math_Disp_Color_Value:
        return MathCfg.displayConfig.displayColor;
        break;
    case ID_Math_Disp_Tag_Char:
        return getCharToString(MathCfg.displayConfig.displayTagChar);
        break;
    case ID_Math_Disp_Tag_No:
        return getCharToString(MathCfg.displayConfig.displayTagNo);
        break;
    case ID_Math_Disp_Zone_Upper:
        return MathCfg.displayConfig.displayZoneUpper;
        break;
    case ID_Math_Disp_Zone_Lower:
        return MathCfg.displayConfig.dispalyZoneLower;
        break;
    case ID_Math_Disp_Scale_Pos:
        return MathCfg.displayConfig.displayScalePos;
        break;
    case ID_Math_Disp_Scale_Div:
        return MathCfg.displayConfig.displayScaleDiv;
        break;
    case ID_Math_DISP_Bar_Pos:
        return MathCfg.displayConfig.displayBarPos;
        break;
    case ID_Math_DISP_Bar_Div:
        return MathCfg.displayConfig.displayBarDiv;
        break;
    case ID_Math_DISP_Partial_Pos:
        return MathCfg.displayConfig.displayPartialPos;
        break;
    case ID_Math_DISP_Partial_Boundary:
        return getFloatToInt(MathCfg.displayConfig.displayPartialBoundary, MathCfg.expressConfig.expPos);
        break;
    case ID_Math_DISP_Partial_OnOff:
        return MathCfg.displayConfig.displayPartialOnOff;
        break;
    case ID_Math_DISP_ColorScaleBand_Mode:
        return MathCfg.displayConfig.displayColorbandMode;
        break;
    case ID_Math_DISP_ColorScaleBand_PosUpper:
        return getFloatToInt(MathCfg.displayConfig.displayColorbandPosUpper, MathCfg.expressConfig.expPos);
        break;
    case ID_Math_DISP_ColorScaleBand_PosLower:
        return getFloatToInt(MathCfg.displayConfig.displayColorbandPosLower, MathCfg.expressConfig.expPos);
        break;
    case ID_Math_DISP_ColorScaleBand_Color:
        return MathCfg.displayConfig.displayColorbandColor;
        break;
    case ID_Math_DISP_AlarmMark_OnOff:
        return MathCfg.displayConfig.displayAlarmMarkOnOff;
        break;
    case ID_Math_DISP_AlarmMark_Type:
        return MathCfg.displayConfig.displayAlarmMarkType;
        break;
    case ID_Math_DISP_AlarmMark_Color1:
        return MathCfg.displayConfig.displayAlarmMarkColor[0];
        break;
    case ID_Math_DISP_AlarmMark_Color2:
        return MathCfg.displayConfig.displayAlarmMarkColor[1];
        break;
    case ID_Math_DISP_AlarmMark_Color3:
        return MathCfg.displayConfig.displayAlarmMarkColor[2];
        break;
    case ID_Math_DISP_AlarmMark_Color4:
        return MathCfg.displayConfig.displayAlarmMarkColor[3];
        break;
    default:
        break;
    }
    return QVariant();
}


QVariant CfgConfig_Math::getMathConstData(quint32 ID, quint32 /*group*/)
{
    int id = ID % 100;          //ID是从21200-21299  当%100后就是对应数组的索引号
    return DevConfigPtr->mathConstK[id];
}

QVariant CfgConfig_Math::getMathActionData(quint32 ID, quint32 /*group*/)
{
    switch (ID) {
    case ID_Math_Action_ErrorVal:
        return DevConfigPtr->mathErrorVal;
        break;
    case ID_Math_Action_KeyAction:
        return DevConfigPtr->mathKeyAction;
        break;
    case ID_Math_Action_Over_SumAve:
        return DevConfigPtr->mathSumAve;
        break;
    case ID_Math_Action_Over_MaxMinPP:
        return DevConfigPtr->mathMaxMinPP;
        break;
    default:
        break;
    }
    return QVariant();
}


QVariant CfgConfig_Math::getRollingAvgIntervalData(quint32 data)
{
    switch (data) {
    case 1:
        return QString::number(Roll_Avg_Interval_1s);
        break;
    case 2:
        return QString::number(Roll_Avg_Interval_2s);
        break;
    case 3:
        return QString::number(Roll_Avg_Interval_3s);
        break;
    case 4:
        return QString::number(Roll_Avg_Interval_4s);
        break;
    case 5:
        return QString::number(Roll_Avg_Interval_5s);
        break;
    case 6:
        return QString::number(Roll_Avg_Interval_6s);
        break;
    case 10:
        return QString::number(Roll_Avg_Interval_10s);
        break;
    case 12:
        return QString::number(Roll_Avg_Interval_12s);
        break;
    case 15:
        return QString::number(Roll_Avg_Interval_15s);
        break;
    case 20:
        return QString::number(Roll_Avg_Interval_20s);
        break;
    case 30:
        return QString::number(Roll_Avg_Interval_30s);
        break;

    case 60:
        return QString::number(Roll_Avg_Interval_1min);
        break;
    case 120:
        return QString::number(Roll_Avg_Interval_2min);
        break;
    case 180:
        return QString::number(Roll_Avg_Interval_3min);
        break;
    case 240:
        return QString::number(Roll_Avg_Interval_4min);
        break;
    case 300:
        return QString::number(Roll_Avg_Interval_5min);
        break;
    case 360:
        return QString::number(Roll_Avg_Interval_6min);
        break;
    case 600:
        return QString::number(Roll_Avg_Interval_10min);
        break;
    case 720:
        return QString::number(Roll_Avg_Interval_12min);
        break;
    case 900:
        return QString::number(Roll_Avg_Interval_15min);
        break;
    case 1200:
        return QString::number(Roll_Avg_Interval_20min);
        break;
    case 1800:
        return QString::number(Roll_Avg_Interval_30min);
        break;
    case 3600:
        return QString::number(Roll_Avg_Interval_1h);
        break;
    default:
        break;
    }
    return QVariant();
}





/*********************************************   get obj str   **********************************************/

QString CfgConfig_Math::getMathExpObjStr(quint32 ID, quint32 /*group*/)
{
    switch (ID) {
    case ID_Math:
        return tr("Math channel settings");
        break;
    case ID_Math_FirstCH:
        return tr("First-CH");
        break;
    case ID_Math_LastCH:
        return tr("Last-CH");
        break;

    case ID_Math_Exp:
        return tr("Caculation expression");
        break;
    case ID_Math_Exp_MathRange:
        return tr("Math range");
        break;
    case ID_Math_Exp_MathRange_Switch:
        return tr("On/Off");
        break;
    case ID_Math_Exp_MathRange_Exp:
        return tr("Calculation expression");
        break;
    case ID_Math_Exp_MathRange_Pos:
        return tr("Decimal place");
        break;
    case ID_Math_Exp_MathRange_Min:
        return tr("Span Lower");
        break;
    case ID_Math_Exp_MathRange_Max:
        return tr("Span Upper");
        break;
    case ID_Math_Exp_MathRange_Unit:
        return tr("Unit");
        break;

    case ID_Math_Exp_Tlog:
        return tr("TLOG");
        break;
    case ID_Math_Exp_Tlog_Type:
        return tr("Timer type");
        break;
    case ID_Math_Exp_Tlog_NO:
        return tr("Timer No.");
        break;
    case ID_Math_Exp_Tlog_SumScale:
        return tr("Sum scale");
        break;
    case ID_Math_Exp_Tlog_Reset:
        return tr("Reset");
        break;

    case ID_Math_Exp_Roll:
        return tr("Rolling average");
        break;
    case ID_Math_Exp_Roll_Switch:       //移动平均开关,0-off, 1-on
        return tr("On/Off");
        break;
    case ID_Math_Exp_Roll_Interval:     //移动平均采样间隔，测量周期的整数倍，单位：S，默认10s
        return tr("Interval");
        break;
    case ID_Math_Exp_Roll_AvgNumber:        //移动平均采样数，移动平均时间 = 采样间隔 * 采样数
        return tr("Number of samples");
        break;
    default:
        break;
    }
    return NULL;
}

QString CfgConfig_Math::getMathAlarmObjStr(quint32 ID, quint32 /*group*/)
{
    switch (ID) {
    case ID_Math_Alarm:
        return tr("Alarm");
        break;

    case ID_Math_Alarm_Level1:
        return QString(tr("Level ") + "1");
        break;
    case ID_Math_Alarm_Level2:
        return QString(tr("Level ") + "2");
        break;
    case ID_Math_Alarm_Level3:
        return QString(tr("Level ") + "3");
        break;
    case ID_Math_Alarm_Level4:
        return QString(tr("Level ") + "4");
        break;

        break;
    case ID_Math_Alarm_Level1_OnOff:
    case ID_Math_Alarm_Level2_OnOff:
    case ID_Math_Alarm_Level3_OnOff:
    case ID_Math_Alarm_Level4_OnOff:
        return tr("On/Off");
        break;
    case ID_Math_Alarm_Level1_Type:
    case ID_Math_Alarm_Level2_Type:
    case ID_Math_Alarm_Level3_Type:
    case ID_Math_Alarm_Level4_Type:
        return tr("Type");
        break;
    case ID_Math_Alarm_Level1_Value:
    case ID_Math_Alarm_Level2_Value:
    case ID_Math_Alarm_Level3_Value:
    case ID_Math_Alarm_Level4_Value:
        return tr("Value");
        break;
    case ID_Math_Alarm_Level1_Hysteresis:
    case ID_Math_Alarm_Level2_Hysteresis:
    case ID_Math_Alarm_Level3_Hysteresis:
    case ID_Math_Alarm_Level4_Hysteresis:
        return tr("Hysteresis");
        break;
    case ID_Math_Alarm_Level1_Logging:
    case ID_Math_Alarm_Level2_Logging:
    case ID_Math_Alarm_Level3_Logging:
    case ID_Math_Alarm_Level4_Logging:
        return tr("Logging");
        break;
    case ID_Math_Alarm_Level1_OutputType:
    case ID_Math_Alarm_Level2_OutputType:
    case ID_Math_Alarm_Level3_OutputType:
    case ID_Math_Alarm_Level4_OutputType:
        return tr("Output type");
        break;
    case ID_Math_Alarm_Level1_OutputNo:
    case ID_Math_Alarm_Level2_OutputNo:
    case ID_Math_Alarm_Level3_OutputNo:
    case ID_Math_Alarm_Level4_OutputNo:
        return tr("Output No.");
        break;
    case ID_Math_Alarm_Leve_Delay:                  //alarm-delay
        return tr("Alarm delay");
        break;
    case ID_Math_Alarm_Leve_Delay_s:
        return tr("Second");
    case ID_Math_Alarm_Leve_Delay_m:
        return tr("Minute");
    case ID_Math_Alarm_Leve_Delay_h:
        return tr("Hour");
        break;
    default:
        break;
    }
    return NULL;
}

QString CfgConfig_Math::getMathDispObjStr(quint32 ID, quint32 /*group*/)
{
    switch (ID) {
    case ID_Math_Disp:
        return tr("Display settings");
        break;

    case ID_Math_Disp_Color:
    case ID_Math_Disp_Color_Value:
    case ID_Math_DISP_ColorScaleBand_Color:
        return tr("Color");
        break;

    case ID_Math_Disp_Tag:
        return tr("Tag");
        break;
    case ID_Math_Disp_Tag_Char:
        return tr("Characters");
        break;
    case ID_Math_Disp_Tag_No:
        return tr("No.");
        break;

    case ID_Math_Disp_Zone:
        return tr("Zone");
        break;
    case ID_Math_Disp_Zone_Upper:
        return tr("Upper");
        break;
    case ID_Math_Disp_Zone_Lower:
        return tr("Lower");
        break;

    case ID_Math_Disp_Scale:
        return tr("Scale");
        break;
    case ID_Math_Disp_Scale_Pos:
        return tr("Position");
        break;
    case ID_Math_Disp_Scale_Div:
        return tr("Division");
        break;

    case ID_Math_DISP_Bar:
        return tr("Bar graph");
        break;
    case ID_Math_DISP_Bar_Pos:
        return tr("Base position");
        break;
    case ID_Math_DISP_Bar_Div:
        return tr("Division");
        break;

    case ID_Math_DISP_Partial:
        return tr("Partial");
        break;
    case ID_Math_DISP_Partial_OnOff:
        return tr("On/Off");
        break;
    case ID_Math_DISP_Partial_Boundary:
        return tr("Boundary");
        break;
    case ID_Math_DISP_Partial_Pos:
        return tr("Expand");
        break;

    case ID_Math_DISP_ColorScaleBand:
        return tr("Color scale band");
        break;
    case ID_Math_DISP_ColorScaleBand_PosLower:
        return tr("Display position Lower");
        break;
    case ID_Math_DISP_ColorScaleBand_PosUpper:
        return tr("Display position Upper");
        break;
    case ID_Math_DISP_ColorScaleBand_Mode:
        return tr("Band area");
        break;

    case ID_Math_DISP_AlarmMark:              //报警标记分组
        return tr("Alarm point mark");
        break;
    case ID_Math_DISP_AlarmMark_OnOff:        //报警标记开关，0-0ff, 1-on
        return tr("Indicate on Scale");
        break;
    case ID_Math_DISP_AlarmMark_Type:     //报警标记类型
        return tr("Mark type");
        break;
    case ID_Math_DISP_AlarmMark_Color1:       //报警颜色， Byte2-Red, Byte1-Green, Byte0-Blue
        return tr("Alarm 1 color");
        break;
    case ID_Math_DISP_AlarmMark_Color2:
        return tr("Alarm 2 color");
        break;
    case ID_Math_DISP_AlarmMark_Color3:
        return tr("Alarm 3 color");
        break;
    case ID_Math_DISP_AlarmMark_Color4:
        return tr("Alarm 4 color");
        break;
    default:
        break;
    }
    return NULL;
}

// get the math constant obj str
QString CfgConfig_Math::getMathConstObjStr(quint32 ID, quint32 /*group*/)
{
    if (ID == ID_Math_Const)
        return tr("Constant");
    else if (ID == ID_Math_Const_NumVal)
        return tr("Constant number");
    else if (ID == ID_Math_Const_Num)
        return tr("Constant");
    else
    {
        int id = ID % 100;
        return "K" + QString("%1").arg(id + 1, 3, 10, QChar('0'));
    }
}

QString CfgConfig_Math::getMathActionObjStr(quint32 ID, quint32 /*group*/)
{
    switch (ID) {
    case ID_Math_Action:
        return tr("Math action settings");
        break;
    case ID_Math_Action_ErrorVal:
        return tr("Value on Error");
        break;
    case ID_Math_Action_KeyAction:
        return tr("START/STOP key action");
        break;
    case ID_Math_Action_Over:
        return tr("Value on Overflow");
        break;
    case ID_Math_Action_Over_SumAve:
        return tr("SUM, AVE");
        break;
    case ID_Math_Action_Over_MaxMinPP:
        return tr("MAX, MIN, P-P");
        break;
    default:
        break;
    }
    return NULL;
}




/*********************************************   get obj val str   **********************************************/
QString CfgConfig_Math::getMathExpObjValStr(quint32 ID, quint32 /*group*/, ConfigObject *cfg)
{
    switch (ID) {
    case ID_Math_Exp_MathRange_Switch:
        return cfg->getData().toInt() == 0 ? tr("Off") : tr("On");
        break;

    case ID_Math_Exp_MathRange_Min:
    case ID_Math_Exp_MathRange_Max:
    {
        int dec = ConfigTreeObject::getConfigData(MATH_DELEGATE_CALCULEXP_RANGE_DECIMAL).toInt();
        float data = (float)(cfg->getData().toInt() / ::pow10(dec));
        return getMathRangScaleUnit(data);
    }
        break;
    case ID_Math_Exp_MathRange_Unit:
        return getScaleUnitValObjStr(cfg->getData().toString());
        break;

    case ID_Math_Exp_Tlog_Type:
        return cfg->getData().toInt() == 0 ? tr("Timer") : tr("Match time timer");
        break;
    case ID_Math_Exp_Tlog_SumScale:
    {
        int type = cfg->getData().toInt();
        if (type == 0)
            return tr("Off");
        else if (type == 1000)
            return tr("sec");
        else if (type == 60000)
            return tr("min");
        else
            return tr("hour");
    }
        break;
    case ID_Math_Exp_Tlog_Reset:
    case ID_Math_Exp_Roll_Switch:
        return cfg->getData().toInt() == 0 ? tr("Off") : tr("On");
        break;
    case ID_Math_Exp_Roll_Interval:
    {
        int type = getRollingAvgInterObjValStr(cfg->getData().toInt()).toInt();
        if (type < 60)
            return QString::number(type) + " " + tr("s");
        else if (type >= 60 && type < 3600)
            return QString::number(type / 60) + " " + tr("min");
        else
            return "1 " + tr("h");
    }
        break;
    default:
        break;
    }
    return cfg->getData().toString();
}

QString CfgConfig_Math::getMathAlarmObjValStr(quint32 ID, quint32 /*group*/, ConfigObject *cfg)
{
    QString cfgData = cfg->getData().toString();
    switch (ID) {
    case ID_Math_Alarm_Level1_OnOff:      //alarm-level1
    case ID_Math_Alarm_Level2_OnOff:
    case ID_Math_Alarm_Level3_OnOff:
    case ID_Math_Alarm_Level4_OnOff:
    case ID_Math_Alarm_Level1_Logging:
    case ID_Math_Alarm_Level2_Logging:
    case ID_Math_Alarm_Level3_Logging:
    case ID_Math_Alarm_Level4_Logging:
        return cfgData.toInt() == 0 ? tr("Off") : tr("On");
        break;
    case ID_Math_Alarm_Level1_Type:
    case ID_Math_Alarm_Level2_Type:
    case ID_Math_Alarm_Level3_Type:
    case ID_Math_Alarm_Level4_Type:
        return getAlarmTypeValObjStr(cfgData.toInt());
        break;

    case ID_Math_Alarm_Level1_Value:
    case ID_Math_Alarm_Level1_Hysteresis:
    case ID_Math_Alarm_Level2_Value:
    case ID_Math_Alarm_Level2_Hysteresis:
    case ID_Math_Alarm_Level3_Value:
    case ID_Math_Alarm_Level3_Hysteresis:
    case ID_Math_Alarm_Level4_Value:
    case ID_Math_Alarm_Level4_Hysteresis:   
    {
        int dec = ConfigTreeObject::getConfigData(MATH_DELEGATE_CALCULEXP_RANGE_DECIMAL).toInt();
        float data = (float)cfg->getData().toInt() / ::pow10(dec);
        return getMathRangScaleUnit(data);
    }
        break;

    case ID_Math_Alarm_Level1_OutputType:
    case ID_Math_Alarm_Level2_OutputType:
    case ID_Math_Alarm_Level3_OutputType:
    case ID_Math_Alarm_Level4_OutputType:
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
    case ID_Math_Alarm_Level1_OutputNo:
    case ID_Math_Alarm_Level2_OutputNo:
    case ID_Math_Alarm_Level3_OutputNo:
    case ID_Math_Alarm_Level4_OutputNo:
        return getMathChannelObjValStr(cfg->getData().toUInt());
        break;
    default:
        break;
    }
    return cfgData;
}

QString CfgConfig_Math::getMathDispObjValStr(quint32 ID, quint32 /*group*/, ConfigObject *cfg)
{
    QString cfgData = cfg->getData().toString();
    switch (ID) {
    case ID_Math_Disp_Color_Value:        //通道颜色，Byte2-Red, Byte1-Green, Byte0-Blue
    case ID_Math_DISP_ColorScaleBand_Color:       //彩色标尺带颜色，Byte2-Red, Byte1-Green, Byte0-Blue
    case ID_Math_DISP_AlarmMark_Color1:       //报警1-4颜色， Byte2-Red, Byte1-Green, Byte0-Blue
    case ID_Math_DISP_AlarmMark_Color2:
    case ID_Math_DISP_AlarmMark_Color3:
    case ID_Math_DISP_AlarmMark_Color4:
        return getColorValueObjStr(cfg->getData().toUInt());
        break;
    case ID_Math_Disp_Tag_Char:       //标记字符串，中英支持，最多32个半角字符
    case ID_Math_Disp_Tag_No:         //标记号码，符号，最多16个半角字符
        return QString(tr("%1").arg(cfgData));
        break;

    case ID_Math_Disp_Zone_Upper:       //显示区域上限，5-100,单位%
    case ID_Math_Disp_Zone_Lower:       //显示区域上限，0-95,单位%
    case ID_Math_DISP_Partial_Pos:       //部分压缩放大位置，1-99, 单位：%，默认50
        return QString("%1 ").arg(cfgData) + QString("%");
        break;
    case ID_Math_Disp_Scale_Pos:
    {
        int type = cfgData.toInt();
        if (type == 0) return tr("Off");
        return cfgData;
    }
        break;

    case ID_Math_DISP_Bar_Pos:      //棒图基准位置， 0-Lower, 1-Center, 2-Upper
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

    case ID_Math_DISP_Partial_OnOff:       //部分压缩放大开关,0-off, 1-on
    case ID_Math_DISP_AlarmMark_OnOff:       //报警标记开关，0-0ff, 1-on
        return cfgData.toInt() == 0 ? tr("Off") : tr("On");
        break;

    case ID_Math_DISP_ColorScaleBand_Mode:       //彩色标尺带模式， 0-off, 1-内侧， 2-外侧.
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

    case ID_Math_DISP_AlarmMark_Type:       //报警标记类型，0-报警， 1-固定
        return cfgData.toInt() == 0 ? tr("Alarm") : tr("Fixed");
        break;

    case ID_Math_DISP_ColorScaleBand_PosLower:       //彩色标尺带显示位置下限
    case ID_Math_DISP_ColorScaleBand_PosUpper:       //彩色标尺带显示位置上限
    case ID_Math_DISP_Partial_Boundary:
    {
        int dec = ConfigTreeObject::getConfigData(MATH_DELEGATE_CALCULEXP_RANGE_DECIMAL).toInt();
        float data = (float)cfg->getData().toInt() / ::pow10(dec);
        QString strShowOn=  getMathRangScaleUnit(data);
        DigitalOperation digOpe;
        return digOpe.getAllowedData(ID, strShowOn, cfg);
    }
        break;

    case ID_Math_Disp_Scale_Div:
    {
        int data = cfg->getData().toInt();
        if (data == 13)
            return "C10";
    }
        break;

    default:
        break;
    }
    return cfgData;
}

QString CfgConfig_Math::getMathConstObjValStr(quint32 ID, quint32 /*group*/, ConfigObject *cfg)
{
    if (ID == ID_Math_Const_NumVal)
        return getMsgNumVal(cfg->getData().toInt());
    else
        return cfg->getData().toString().toUpper();
}


QString CfgConfig_Math::getMathActionObjValStr(quint32 ID, quint32 /*group*/, ConfigObject *cfg)
{
    switch (ID) {
    case ID_Math_Action_ErrorVal:   //运算错误显示值
        return cfg->getData().toInt() == 0 ? tr("+Over") : tr("-Over");
        break;
    case ID_Math_Action_KeyAction:   //START/STOP键动作设定
    {
        int type = cfg->getData().toInt();
        if (type == Math_Key_Action_Off)
            return tr("Off");
        else if (type == Math_Key_Action_Normal)
            return tr("Start / Stop");
        else
            return tr("Reset + Start / Stop");
    }
        break;

    case ID_Math_Action_Over_SumAve:         //SUM, AVE计算溢出处理
    case ID_Math_Action_Over_MaxMinPP:      //MAX, MIN, P-P计算溢出处理
    {
        int type = cfg->getData().toInt();
        if (type == Math_Over_Range_Error)
            return tr("Error");
        else if (type == Math_Over_Range_Limit)
            return tr("Limit");
        else if (type == Math_Over_Range_Skip)
            return tr("Skip");
        else
            return tr("Over");
    }
        break;
    default:
        break;
    }
    return NULL;
}


QString CfgConfig_Math::getMathRangScaleUnit(float data)
{
    QString unit = ConfigTreeObject::getConfigData(MATH_DELEGATE_CALCULEXP_RANGE_UNIT).toString();
    quint32 decCnt = ConfigTreeObject::getConfigData(MATH_DELEGATE_CALCULEXP_RANGE_DECIMAL).toInt();

    return QString::number(data, 'f', decCnt) + " " + unit;
}

QString CfgConfig_Math::getScaleUnitValObjStr(QString unit)
{
    QStringList str;
    str << MATH_DELEGATE_CALCULEXP_RANGE_LOWER << MATH_DELEGATE_CALCULEXP_RANGE_UPPER
        << MATH_DELEGATE_ALARM_LVL1_VALUE << MATH_DELEGATE_ALARM_LVL1_HYST
        << MATH_DELEGATE_ALARM_LVL2_VALUE << MATH_DELEGATE_ALARM_LVL2_HYST
        << MATH_DELEGATE_ALARM_LVL3_VALUE << MATH_DELEGATE_ALARM_LVL3_HYST
        << MATH_DELEGATE_ALARM_LVL4_VALUE << MATH_DELEGATE_ALARM_LVL4_HYST
        << MATH_DELEGATE_DISP_CSBAND_UPPER << MATH_DELEGATE_DISP_CSBAND_LOWER;

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
QString CfgConfig_Math::getAlarmTypeValObjStr(quint32 type)
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

QString CfgConfig_Math::getRollingAvgInterObjValStr(quint32 data)
{
    switch (data) {
    case Roll_Avg_Interval_1s:
        return QString::number(1);
        break;
    case Roll_Avg_Interval_2s:
        return QString::number(2);
        break;
    case Roll_Avg_Interval_3s:
        return QString::number(3);
        break;
    case Roll_Avg_Interval_4s:
        return QString::number(4);
        break;
    case Roll_Avg_Interval_5s:
        return QString::number(5);
        break;
    case Roll_Avg_Interval_6s:
        return QString::number(6);
        break;
    case Roll_Avg_Interval_10s:
        return QString::number(10);
        break;
    case Roll_Avg_Interval_12s:
        return QString::number(12);
        break;
    case Roll_Avg_Interval_15s:
        return QString::number(15);
        break;
    case Roll_Avg_Interval_20s:
        return QString::number(20);
        break;
    case Roll_Avg_Interval_30s:
        return QString::number(30);
        break;

    case Roll_Avg_Interval_1min:
        return QString::number(60);
        break;
    case Roll_Avg_Interval_2min:
        return QString::number(120);
        break;
    case Roll_Avg_Interval_3min:
        return QString::number(180);
        break;
    case Roll_Avg_Interval_4min:
        return QString::number(240);
        break;
    case Roll_Avg_Interval_5min:
        return QString::number(300);
        break;
    case Roll_Avg_Interval_6min:
        return QString::number(360);
        break;
    case Roll_Avg_Interval_10min:
        return QString::number(600);
        break;
    case Roll_Avg_Interval_15min:
        return QString::number(900);
        break;
    case Roll_Avg_Interval_20min:
        return QString::number(1200);
        break;
    case Roll_Avg_Interval_30min:
        return QString::number(1800);
        break;
    case Roll_Avg_Interval_1h:
        return QString::number(3600);
        break;
    default:
        break;
    }
    return NULL;
}

QString CfgConfig_Math::getMsgNumVal(quint32 type)
{
    return QString("K%1").arg(type, 2, 10, QChar('0')) + "1 -- " + \
            QString("K%1").arg(type + 1, 2, 10, QChar('0')) + "0";
}





void CfgConfig_Math::setMathExpData(quint32 ID, quint32 group)
{
    int index = CfgChannel::instance()->getExistChannelMath().values().indexOf(group);
    QString chanNum = CfgChannel::instance()->getExistChannelMath().keys().at(index);
    MATH_CONFIG * MathCfg = CfgChannel::instance()->getMathConfig(group);
    switch (ID) {
    case ID_Math_Exp_MathRange_Switch:
        MathCfg->expressConfig.expSwtich = ConfigTreeObject::getConfigData(QString(MATHCFG_CALCULEXP_RANGE_SWITCH).arg(chanNum)).toInt();
        break;
    case ID_Math_Exp_MathRange_Pos:
        MathCfg->expressConfig.expPos = ConfigTreeObject::getConfigData(QString(MATHCFG_CALCULEXP_RANGE_DECIMAL).arg(chanNum)).toInt();
        break;
    case ID_Math_Exp_MathRange_Min:
        MathCfg->expressConfig.expMin = ConfigTreeObject::getConfigData(QString(MATHCFG_CALCULEXP_RANGE_LOWER).arg(chanNum)).toInt();
        break;
    case ID_Math_Exp_MathRange_Max:
        MathCfg->expressConfig.expMax = ConfigTreeObject::getConfigData(QString(MATHCFG_CALCULEXP_RANGE_UPPER).arg(chanNum)).toInt();
        break;
    case ID_Math_Exp_MathRange_Exp:
        setStrToChar(MathCfg->expressConfig.expExpression, ConfigTreeObject::getConfigData(QString(MATHCFG_CALCULEXP_RANGE_CALCUEXP).arg(chanNum)).toString(), 124);
        break;
    case ID_Math_Exp_MathRange_Unit:
        setStrToChar(MathCfg->expressConfig.expUnit, ConfigTreeObject::getConfigData(QString(MATHCFG_CALCULEXP_RANGE_UNIT).arg(chanNum)).toString(), 8);
        break;

    case ID_Math_Exp_Tlog_Type:
        MathCfg->expressConfig.expTlogType = ConfigTreeObject::getConfigData(QString(MATHCFG_CALCULEXP_TLOG_TYPE).arg(chanNum)).toInt();
        break;
    case ID_Math_Exp_Tlog_NO:
        MathCfg->expressConfig.expTlogNo = ConfigTreeObject::getConfigData(QString(MATHCFG_CALCULEXP_TLOG_NO).arg(chanNum)).toInt();
        break;
    case ID_Math_Exp_Tlog_SumScale:
        MathCfg->expressConfig.expTlogSumScale = ConfigTreeObject::getConfigData(QString(MATHCFG_CALCULEXP_TLOG_SCALE).arg(chanNum)).toInt();
        break;
    case ID_Math_Exp_Tlog_Reset:
        MathCfg->expressConfig.expTlogReset = ConfigTreeObject::getConfigData(QString(MATHCFG_CALCULEXP_TLOG_RESET).arg(chanNum)).toInt();
        break;

    case ID_Math_Exp_Roll_Switch:
        MathCfg->expressConfig.expRollingAvgSwtich  = ConfigTreeObject::getConfigData(QString(MATHCFG_CALCULEXP_MOVAVE_SWITCH).arg(chanNum)).toInt();
        break;
    case ID_Math_Exp_Roll_Interval:
        MathCfg->expressConfig.expRollingAvgInterval = getRollingAvgInterObjValStr(ConfigTreeObject::getConfigData(QString(MATHCFG_CALCULEXP_MOVAVE_INTER).arg(chanNum)).toInt()).toInt();
        break;
    case ID_Math_Exp_Roll_AvgNumber:
        MathCfg->expressConfig.expRollingAvgNumber = ConfigTreeObject::getConfigData(QString(MATHCFG_CALCULEXP_MOVAVE_NUM).arg(chanNum)).toInt();
        break;
    default:
        break;
    }
}

void CfgConfig_Math::setMathAlarmData(quint32 ID, quint32 group)
{
    int index = CfgChannel::instance()->getExistChannelMath().values().indexOf(group);
    QString chanNum = CfgChannel::instance()->getExistChannelMath().keys().at(index);
    MATH_CONFIG *MathCfg = CfgChannel::instance()->getMathConfig(group);
    switch (ID) {
    case ID_Math_Alarm_Level1_OnOff:
        MathCfg->alarmConfig.alarmOnOff[0] = ConfigTreeObject::getConfigData(QString(MATHCFG_ALARM_LVL1_ONOFF).arg(chanNum)).toUInt();
        break;
    case ID_Math_Alarm_Level1_Type:
        MathCfg->alarmConfig.alarmType[0] = (Alarm_Type)ConfigTreeObject::getConfigData(QString(MATHCFG_ALARM_LVL1_TYPE).arg(chanNum)).toUInt();
        break;
    case ID_Math_Alarm_Level1_Value:
        MathCfg->alarmConfig.alarmValue[0] = ConfigTreeObject::getConfigData(QString(MATHCFG_ALARM_LVL1_VALUE).arg(chanNum)).toFloat();
        break;
    case ID_Math_Alarm_Level1_Hysteresis:
        MathCfg->alarmConfig.alarmHysteresis[0] = ConfigTreeObject::getConfigData(QString(MATHCFG_ALARM_LVL1_HYST).arg(chanNum)).toFloat();
        break;
    case ID_Math_Alarm_Level1_Logging:
        MathCfg->alarmConfig.alarmLogging[0] = ConfigTreeObject::getConfigData(QString(MATHCFG_ALARM_LVL1_LOGG).arg(chanNum)).toUInt();
        break;
    case ID_Math_Alarm_Level1_OutputType:
        MathCfg->alarmConfig.alarmOutputType[0] = ConfigTreeObject::getConfigData(QString(MATHCFG_ALARM_LVL1_OUTTYPE).arg(chanNum)).toUInt();
    case ID_Math_Alarm_Level1_OutputNo:
        MathCfg->alarmConfig.alarmOuputNo[0] = ConfigTreeObject::getConfigData(QString(MATHCFG_ALARM_LVL1_OUTNO).arg(chanNum)).toUInt();
        break;

    case ID_Math_Alarm_Level2_OnOff:
        MathCfg->alarmConfig.alarmOnOff[1] = ConfigTreeObject::getConfigData(QString(MATHCFG_ALARM_LVL2_ONOFF).arg(chanNum)).toUInt();
        break;
    case ID_Math_Alarm_Level2_Type:
        MathCfg->alarmConfig.alarmType[1] = (Alarm_Type)ConfigTreeObject::getConfigData(QString(MATHCFG_ALARM_LVL2_TYPE).arg(chanNum)).toUInt();
        break;
    case ID_Math_Alarm_Level2_Value:
        MathCfg->alarmConfig.alarmValue[1] = ConfigTreeObject::getConfigData(QString(MATHCFG_ALARM_LVL2_VALUE).arg(chanNum)).toFloat();
        break;
    case ID_Math_Alarm_Level2_Hysteresis:
        MathCfg->alarmConfig.alarmHysteresis[1] = ConfigTreeObject::getConfigData(QString(MATHCFG_ALARM_LVL2_HYST).arg(chanNum)).toFloat();
        break;
    case ID_Math_Alarm_Level2_Logging:
        MathCfg->alarmConfig.alarmLogging[1] = ConfigTreeObject::getConfigData(QString(MATHCFG_ALARM_LVL2_LOGG).arg(chanNum)).toUInt();
        break;
    case ID_Math_Alarm_Level2_OutputType:
        MathCfg->alarmConfig.alarmOutputType[1] = ConfigTreeObject::getConfigData(QString(MATHCFG_ALARM_LVL2_OUTTYPE).arg(chanNum)).toUInt();
    case ID_Math_Alarm_Level2_OutputNo:
        MathCfg->alarmConfig.alarmOuputNo[1] = ConfigTreeObject::getConfigData(QString(MATHCFG_ALARM_LVL2_OUTNO).arg(chanNum)).toUInt();
        break;

    case ID_Math_Alarm_Level3_OnOff:
        MathCfg->alarmConfig.alarmOnOff[2] = ConfigTreeObject::getConfigData(QString(MATHCFG_ALARM_LVL3_ONOFF).arg(chanNum)).toUInt();
        break;
    case ID_Math_Alarm_Level3_Type:
        MathCfg->alarmConfig.alarmType[2] = (Alarm_Type)ConfigTreeObject::getConfigData(QString(MATHCFG_ALARM_LVL3_TYPE).arg(chanNum)).toUInt();
        break;
    case ID_Math_Alarm_Level3_Value:
        MathCfg->alarmConfig.alarmValue[2] = ConfigTreeObject::getConfigData(QString(MATHCFG_ALARM_LVL3_VALUE).arg(chanNum)).toFloat();
        break;
    case ID_Math_Alarm_Level3_Hysteresis:
        MathCfg->alarmConfig.alarmHysteresis[2] = ConfigTreeObject::getConfigData(QString(MATHCFG_ALARM_LVL3_HYST).arg(chanNum)).toFloat();
        break;
    case ID_Math_Alarm_Level3_Logging:
        MathCfg->alarmConfig.alarmLogging[2] = ConfigTreeObject::getConfigData(QString(MATHCFG_ALARM_LVL3_LOGG).arg(chanNum)).toUInt();
        break;
    case ID_Math_Alarm_Level3_OutputType:
        MathCfg->alarmConfig.alarmOutputType[2] = ConfigTreeObject::getConfigData(QString(MATHCFG_ALARM_LVL3_OUTTYPE).arg(chanNum)).toUInt();
    case ID_Math_Alarm_Level3_OutputNo:
        MathCfg->alarmConfig.alarmOuputNo[2] = ConfigTreeObject::getConfigData(QString(MATHCFG_ALARM_LVL3_OUTNO).arg(chanNum)).toUInt();
        break;

    case ID_Math_Alarm_Level4_OnOff:
        MathCfg->alarmConfig.alarmOnOff[3] = ConfigTreeObject::getConfigData(QString(MATHCFG_ALARM_LVL4_ONOFF).arg(chanNum)).toUInt();
        break;
    case ID_Math_Alarm_Level4_Type:
        MathCfg->alarmConfig.alarmType[3] = (Alarm_Type)ConfigTreeObject::getConfigData(QString(MATHCFG_ALARM_LVL4_TYPE).arg(chanNum)).toUInt();
        break;
    case ID_Math_Alarm_Level4_Value:
        MathCfg->alarmConfig.alarmValue[3] = ConfigTreeObject::getConfigData(QString(MATHCFG_ALARM_LVL4_VALUE).arg(chanNum)).toFloat();
        break;
    case ID_Math_Alarm_Level4_Hysteresis:
        MathCfg->alarmConfig.alarmHysteresis[3] = ConfigTreeObject::getConfigData(QString(MATHCFG_ALARM_LVL4_HYST).arg(chanNum)).toFloat();
        break;
    case ID_Math_Alarm_Level4_Logging:
        MathCfg->alarmConfig.alarmLogging[3] = ConfigTreeObject::getConfigData(QString(MATHCFG_ALARM_LVL4_LOGG).arg(chanNum)).toUInt();
        break;
    case ID_Math_Alarm_Level4_OutputType:
        MathCfg->alarmConfig.alarmOutputType[3] = ConfigTreeObject::getConfigData(QString(MATHCFG_ALARM_LVL4_OUTTYPE).arg(chanNum)).toUInt();
    case ID_Math_Alarm_Level4_OutputNo:
        MathCfg->alarmConfig.alarmOuputNo[3] = ConfigTreeObject::getConfigData(QString(MATHCFG_ALARM_LVL3_OUTNO).arg(chanNum)).toUInt();
        break;

    case ID_Math_Alarm_Leve_Delay_s:
        MathCfg->alarmConfig.alarmDelay[0] = ConfigTreeObject::getConfigData(QString(MATHCFG_ALARM_DELAY_SEC).arg(chanNum)).toUInt();
        break;
    case ID_Math_Alarm_Leve_Delay_m:
        MathCfg->alarmConfig.alarmDelay[1] = ConfigTreeObject::getConfigData(QString(MATHCFG_ALARM_DELAY_MIN).arg(chanNum)).toUInt();
        break;
    case ID_Math_Alarm_Leve_Delay_h:
        MathCfg->alarmConfig.alarmDelay[2] = ConfigTreeObject::getConfigData(QString(MATHCFG_ALARM_DELAY_HOUR).arg(chanNum)).toUInt();
        break;
    default:
        break;
    }
}

void CfgConfig_Math::setMathDispData(quint32 ID, quint32 group)
{
    int index = CfgChannel::instance()->getExistChannelMath().values().indexOf(group);
    QString chanNum = CfgChannel::instance()->getExistChannelMath().keys().at(index);
    MATH_CONFIG *MathCfg = CfgChannel::instance()->getMathConfig(group);
    switch (ID) {
    case ID_Math_Disp_Color_Value:
        MathCfg->displayConfig.displayColor = ConfigTreeObject::getConfigData(QString(MATHCFG_DISP_COLOR_COLOR).arg(chanNum)).toUInt();
        break;
    case ID_Math_Disp_Tag_Char:
        setStrToChar(MathCfg->displayConfig.displayTagChar, ConfigTreeObject::getConfigData(QString(MATHCFG_DISP_TAG_CHAR).arg(chanNum)).toString(), 32);
        break;
    case ID_Math_Disp_Tag_No:
        setStrToChar(MathCfg->displayConfig.displayTagNo, ConfigTreeObject::getConfigData(QString(MATHCFG_DISP_TAG_NO).arg(chanNum)).toString(), 16);
        break;
    case ID_Math_Disp_Zone_Upper:
        MathCfg->displayConfig.displayZoneUpper = ConfigTreeObject::getConfigData(QString(MATHCFG_DISP_ZONE_UPPER).arg(chanNum)).toUInt();
        break;
    case ID_Math_Disp_Zone_Lower:
        MathCfg->displayConfig.dispalyZoneLower = ConfigTreeObject::getConfigData(QString(MATHCFG_DISP_ZONE_LOWER).arg(chanNum)).toUInt();
        break;
    case ID_Math_Disp_Scale_Pos:
        MathCfg->displayConfig.displayScalePos = ConfigTreeObject::getConfigData(QString(MATHCFG_DISP_SCALE_POS).arg(chanNum)).toUInt();
        break;
    case ID_Math_Disp_Scale_Div:
        MathCfg->displayConfig.displayScaleDiv = ConfigTreeObject::getConfigData(QString(MATHCFG_DISP_SCALE_DIV).arg(chanNum)).toUInt();
        break;
    case ID_Math_DISP_Bar_Pos:
        MathCfg->displayConfig.displayBarPos = ConfigTreeObject::getConfigData(QString(MATHCFG_DISP_BAR_BASEPOS).arg(chanNum)).toUInt();
        break;
    case ID_Math_DISP_Bar_Div:
        MathCfg->displayConfig.displayBarDiv = ConfigTreeObject::getConfigData(QString(MATHCFG_DISP_BAR_DIVISION).arg(chanNum)).toUInt();
        break;
    case ID_Math_DISP_Partial_OnOff:
        MathCfg->displayConfig.displayPartialOnOff = ConfigTreeObject::getConfigData(QString(MATHCFG_DISP_PART_ONOFF).arg(chanNum)).toUInt();
        break;
    case ID_Math_DISP_Partial_Boundary:
    {
        int decCnt= ConfigTreeObject::getConfigData(QString(MATHCFG_DISP_SCALE_POS).arg(chanNum)).toInt();
        MathCfg->displayConfig.displayPartialBoundary = (float)ConfigTreeObject::getConfigData(QString(MATHCFG_DISP_PART_BOUND).arg(chanNum)).toInt() / ::pow10(decCnt);
    }
        break;
    case ID_Math_DISP_Partial_Pos:
        MathCfg->displayConfig.displayPartialPos = ConfigTreeObject::getConfigData(QString(MATHCFG_DISP_PART_EXPAND).arg(chanNum)).toUInt();
        break;

    case ID_Math_DISP_ColorScaleBand_Color:
        MathCfg->displayConfig.displayColorbandColor = ConfigTreeObject::getConfigData(QString(MATHCFG_DISP_CSBAND_COLOR).arg(chanNum)).toUInt();
        break;
    case ID_Math_DISP_ColorScaleBand_PosLower:
    {
        int decCnt= ConfigTreeObject::getConfigData(QString(MATHCFG_DISP_SCALE_POS).arg(chanNum)).toInt();
        MathCfg->displayConfig.displayColorbandPosLower = (float)ConfigTreeObject::getConfigData(QString(MATHCFG_DISP_CSBAND_LOWER).arg(chanNum)).toInt() / ::pow10(decCnt);
    }
        break;
    case ID_Math_DISP_ColorScaleBand_PosUpper:
    {
        int decCnt= ConfigTreeObject::getConfigData(QString(MATHCFG_DISP_SCALE_POS).arg(chanNum)).toInt();
        MathCfg->displayConfig.displayColorbandPosUpper = (float)ConfigTreeObject::getConfigData(QString(MATHCFG_DISP_CSBAND_UPPER).arg(chanNum)).toInt() / ::pow10(decCnt);
    }
        break;
    case ID_Math_DISP_ColorScaleBand_Mode:
        MathCfg->displayConfig.displayColorbandMode = ConfigTreeObject::getConfigData(QString(MATHCFG_DISP_CSBAND_AREA).arg(chanNum)).toUInt();
        break;

    case ID_Math_DISP_AlarmMark_OnOff:
        MathCfg->displayConfig.displayAlarmMarkOnOff = ConfigTreeObject::getConfigData(QString(MATHCFG_DISP_ALARM_INDIC).arg(chanNum)).toUInt();
        break;
    case ID_Math_DISP_AlarmMark_Type:
        MathCfg->displayConfig.displayAlarmMarkType = ConfigTreeObject::getConfigData(QString(MATHCFG_DISP_ALARM_MARKTYPE).arg(chanNum)).toUInt();
        break;
    case ID_Math_DISP_AlarmMark_Color1:
        MathCfg->displayConfig.displayAlarmMarkColor[0] = ConfigTreeObject::getConfigData(QString(MATHCFG_DISP_ALARM_COLOR1).arg(chanNum)).toUInt();
        break;
    case ID_Math_DISP_AlarmMark_Color2:
        MathCfg->displayConfig.displayAlarmMarkColor[1] = ConfigTreeObject::getConfigData(QString(MATHCFG_DISP_ALARM_COLOR2).arg(chanNum)).toUInt();
        break;
    case ID_Math_DISP_AlarmMark_Color3:
        MathCfg->displayConfig.displayAlarmMarkColor[2] = ConfigTreeObject::getConfigData(QString(MATHCFG_DISP_ALARM_COLOR3).arg(chanNum)).toUInt();
        break;
    case ID_Math_DISP_AlarmMark_Color4:
        MathCfg->displayConfig.displayAlarmMarkColor[3] = ConfigTreeObject::getConfigData(QString(MATHCFG_DISP_ALARM_COLOR4).arg(chanNum)).toUInt();
        break;
    default:
        break;
    }
}


void CfgConfig_Math::setMathConstData(quint32 ID, quint32 /*group*/)
{
    int id = ID % 100;          //ID是从21200-21299  当%100后就是对应数组的索引号
    DevConfigPtr->mathConstK[id] = ConfigTreeObject::getConfigData(QString(MATHCFG_CONSTANTNUM_INDEX).arg(id + 1, 3, 10, QChar('0'))).toFloat();
}

void CfgConfig_Math::setMathActionData(quint32 ID, quint32 /*group*/)
{
    switch (ID) {
    case ID_Math_Action_ErrorVal:
        DevConfigPtr->mathErrorVal = (Math_Error_Value_Enum)ConfigTreeObject::getConfigData(QString(MATHCFG_MATHACTION_VALUEERROR)).toInt();
        break;
    case ID_Math_Action_KeyAction:
        DevConfigPtr->mathKeyAction = (Math_Key_Action)ConfigTreeObject::getConfigData(QString(MATHCFG_MATHACTION_KEYACTION)).toInt();
        break;
    case ID_Math_Action_Over_SumAve:
        DevConfigPtr->mathSumAve = (Math_Over_Range)ConfigTreeObject::getConfigData(QString(MATHCFG_MATHACTION_VALUE_SUM)).toInt();
        break;
    case ID_Math_Action_Over_MaxMinPP:
        DevConfigPtr->mathMaxMinPP = (Math_Over_Range)ConfigTreeObject::getConfigData(QString(MATHCFG_MATHACTION_VALUE_PP)).toInt();
        break;
    default:
        break;
    }
}





//把32位通道号转化为相应的四位通道     quint32 --> QString
QString CfgConfig_Math::getMathChannelObjValStr(quint32 num)
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
 *  QString ---> quint32  把四位的字符串描述转化为quint32
 *     参数
 *          type : output类型
 *          code : outputNo的真实树配置值
 *    返回值
 *      32 位的通道号
*/
quint32 CfgConfig_Math::setMathOutputNo(quint32 type, quint32 code)
{
    quint32 num;
    if (type == 0)
        num = 1;
    else if (type == 1)
    {
        if (CfgChannel::instance()->getExistChannelDO().isEmpty())
            num = 1;
        else
        {
            QString numStr = QString("%1").arg(code, 4, 10, QChar('0'));
            int index = CfgChannel::instance()->getExistChannelDO().keys().indexOf(numStr);
            num = CfgChannel::instance()->getExistChannelDO().values().at(index);
        }
    }
    else
        num = code;

    return num;
}



/*
 * 把uchar *类型转化为QString类型
*/
QVariant CfgConfig_Math::getCharToString(uchar *str)
{
    QString data((char *)str);
    return data;
}

//把QString类型转化为uchar *类型
void CfgConfig_Math::setStrToChar(uchar * uData, QString str, int len)
{
    memset(uData, 0, len);
    QByteArray baUnit = str.toLocal8Bit();
    char* valueUnit = baUnit.data();
    while (*valueUnit != '\0') {
        *uData++ = *valueUnit++;
    }
    //    memcpy(uData, valueUnit, strlen(valueUnit));
}


/*
 * 把从配置中得到的颜色配置值转换为界面label显示的样式字符串返回
*/
QString CfgConfig_Math::getColorValueObjStr(quint32 x)
{
    QColor colorRGBx(COLOR_RED(x), COLOR_GREEN(x), COLOR_BLUE(x));
    QString str = QString("QLabel {background-color: %1}").arg(colorRGBx.name());
    return str;
}

int CfgConfig_Math::getFloatToInt(float data, int pos)
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

