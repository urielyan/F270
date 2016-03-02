#include "cfgconfig_ai.h"
#include <QVariant>
#include "cfgchannel.h"
#include "configtreeobject_AI.h"
#include "./Common/digitaloperation.h"

Cfgconfig_AI::Cfgconfig_AI(QObject *parent) :
    QObject(parent)
{
}

Cfgconfig_AI * Cfgconfig_AI::instance()
{
    static Cfgconfig_AI data;
    return  &data;
}


QVariant Cfgconfig_AI::getAIData(quint32 ID, quint32 group)
{
    //添加开始通道和结束通道
    if (ID == ID_AI_Last_CH || ID == ID_AI_First_CH)
        return QVariant();

    if (ID >= AI_Range_Start_ID && ID <= AI_Range_End_ID)
        return getAIRangeData(ID, group);
    else if (ID >= AI_Alarm_Start_ID && ID <= AI_Alarm_End_ID)
        return getAIAlarmData(ID, group);
    else if (ID >= AI_Disp_Start_ID  && ID <= AI_Disp_End_ID)
        return getAIDisplayData(ID, group);
    else
        return getAICrorectionData(ID, group);
}


//AI obj str
QString Cfgconfig_AI::getAIObjStr(quint32 ID, quint32 /*group*/)
{
    //    if (ID == ID_AI_Last_CH || ID == ID_AI_First_CH)
    //        return NULL;

    if (ID >= AI_Start_ID && ID <= AI_Range_End_ID)
        return getAIRangeStr(ID);
    else if (ID >= AI_Alarm_Start_ID && ID <= AI_Alarm_End_ID)
        return getAIAlarmStr(ID);
    else if (ID >= AI_Disp_Start_ID && ID <= AI_Disp_End_ID)
        return getAIDisplayStr(ID);
    else
        return getAICrorectionStr(ID);
}


/*
 * 通过ID，组和对象得到AI相对应对象配置值的值描述
 *       para : ID       配置对象的ID
 *              group    配置对象的group
 *              cfg      配置对象
 *  retrun :   对象的值描述
*/
QString Cfgconfig_AI::getAIValueObjStr(quint32 ID, quint32 group, ConfigObject * cfg)
{
    if (ID == ID_AI_First_CH || ID == ID_AI_Last_CH)
        return NULL;
    if (ID >= AI_Range_Start_ID && ID <= AI_Range_End_ID)
        return getAIRangeValueObjStr(ID, group, cfg);
    else if (ID >= AI_Alarm_Start_ID && ID <= AI_Alarm_End_ID)
        return getAIAlarmValueObjStr(ID, group, cfg);
    else if (ID >= AI_Disp_Start_ID && ID <= AI_Disp_End_ID)
        return getAIDisplayValueObjStr(ID, group, cfg);
    else
        return getAICrorectionValueObjStr(ID, group, cfg);
}

void Cfgconfig_AI::setAIData(quint32 ID, quint32 group)
{
    if (ID >= AI_Range_Start_ID && ID <= AI_Range_End_ID)
        setAIRangeData(ID, group);
    else if (ID >= AI_Alarm_Start_ID && ID <= AI_Alarm_End_ID)
        setAIAlarmData(ID, group);
    else if (ID >= AI_Disp_Start_ID  && ID <= AI_Disp_End_ID)
        setAIDisplayData(ID, group);
    else if (ID >= AI_Correct_Start_ID  && ID <= AI_Correct_End_ID)
        setAICrorectionData(ID, group);
}






//AI-Range
QVariant Cfgconfig_AI::getAIRangeData(quint32 ID, quint32 group)
{
    AI_CONFIG AiCfg = *CfgChannel::instance()->getAIConfig(group);
    switch (ID) {
    case ID_AI_Range_Range_Type:
        return AiCfg.rangeConfig.rangeType;
        break;
    case ID_AI_Range_Range_Range:
        return getAIRangeRangeRangeData(AiCfg);
        break;
    case ID_AI_Range_Range_Min:
        return AiCfg.rangeConfig.rangeMin;
        break;
    case ID_AI_Range_Range_Max:
        return AiCfg.rangeConfig.rangeMax;
        break;
    case ID_AI_Range_Range_DispMin:
        return AiCfg.rangeConfig.rangeDispMin;
        break;
    case ID_AI_Range_Range_DispMax:
        return AiCfg.rangeConfig.rangeDispMax;
        break;
    case ID_AI_Range_Range_Calculate:
        return AiCfg.rangeConfig.rangeCalculateType;
        break;

    case ID_AI_Range_Range_BasicChannel:    //通道号的字符串   quint32 --> QString
    {
        QList<quint32> list = CfgChannel::instance()->getEffectiveChannel();
        if (list.contains(AiCfg.rangeConfig.rangeBasicChannel))
            return AiCfg.rangeConfig.rangeBasicChannel;
        else
            return list.first();
    }
        break;

    case ID_AI_Range_Scale_Pos:
        return AiCfg.rangeConfig.rangeScale.scalePosition;
        break;
    case ID_AI_Range_Scale_Min:
        return AiCfg.rangeConfig.rangeScale.scaleMin;
        break;
    case ID_AI_Range_Scale_Max:
        return AiCfg.rangeConfig.rangeScale.scaleMax;
        break;
    case ID_AI_Range_Scale_Unit:
        return getAIRangeScaleUnitValObjStr(AiCfg.rangeConfig.rangeScale.scaleUnit);      //unit
        break;

    case ID_AI_Range_LowCut_OnOff:
        return AiCfg.rangeConfig.rangeLowCut.lowCutOnOff;
        break;
    case ID_AI_Range_LowCut_Output:
        return AiCfg.rangeConfig.rangeLowCut.lowCutOut;
        break;
    case ID_AI_Range_LowCut_Value:
        return AiCfg.rangeConfig.rangeLowCut.lowCutPos;
        break;

    case ID_AI_Range_MoveAverange_OnOff:
        return AiCfg.rangeConfig.rangeMovingAvg.avgOnOff;
        break;
    case ID_AI_Range_MoveAverange_Cnt:
        return AiCfg.rangeConfig.rangeMovingAvg.avgCount;
        break;

    case ID_AI_Range_RJC_Mode:
        return AiCfg.rangeConfig.rangeRJCCompensation.rjcCompMode;
        break;
    case ID_AI_Range_RJC_Temp:
        return AiCfg.rangeConfig.rangeRJCCompensation.rjcTemp;
        break;

    case ID_AI_Range_Burnout_Mode:
        return AiCfg.rangeConfig.rangeBurnout;
        break;

    case ID_AI_Range_Bias_Value:
        return AiCfg.rangeConfig.rangeBias;
        break;
    default:
        break;
    }
    return QVariant();
}

/*
 * 通过通道得到该通道配置类型
 *    return : 返回陪行类型的配置值
*/
QVariant Cfgconfig_AI::getAIRangeRangeRangeData(AI_CONFIG AiCfg)
{
    switch (AiCfg.rangeConfig.rangeType) {
    case AI_RANGE_Type_Vlot:        //直流电压
        return AiCfg.rangeConfig.config.rangeVolt;
        break;
    case AI_RANGE_Type_GS:      //GS（0.4-2V, 1-5V标准信号）
        return AiCfg.rangeConfig.config.rangeGS;
        break;
    case AI_RANGE_Type_TC:       //热电耦
        return AiCfg.rangeConfig.config.rangeTC;
        break;
    case AI_RANGE_Type_RTD:     //热电阻
        return AiCfg.rangeConfig.config.rangeRTD;
        break;
    case AI_RANGE_Type_DI:       //开关量
        return AiCfg.rangeConfig.config.rangeDI;
        break;
    case AI_RANGE_Type_Cur0_20mA:       //电流（0-20mA）
        return AiCfg.rangeConfig.config.rangeCur0_20mA;
        break;
    case AI_RANGE_Type_GS4_20mA:
        return AiCfg.rangeConfig.config.rangeGS4_20mA;
        break;
    default:
        break;
    }
    return QVariant();
}





//Ai-alarm
QVariant Cfgconfig_AI::getAIAlarmData(quint32 ID, quint32 group)
{
    AI_CONFIG AiCfg = *CfgChannel::instance()->getAIConfig(group);
    switch (ID) {
    case ID_AI_Alarm_Level1_OnOff:
        return AiCfg.alarmConfig.alarmOnOff[0];
        break;
    case ID_AI_Alarm_Level1_Type:
        return AiCfg.alarmConfig.alarmType[0];
        break;
    case ID_AI_Alarm_Level1_Value:
    {
        int pos = getAIChannelDataAccuracy(AiCfg);
        return getFloatToInt(AiCfg.alarmConfig.alarmValue[0], pos);
    }
        break;
    case ID_AI_Alarm_Level1_Hysteresis:
    {
        int pos = getAIChannelDataAccuracy(AiCfg);
        return getFloatToInt(AiCfg.alarmConfig.alarmHysteresis[0], pos);
    }
        break;
    case ID_AI_Alarm_Level1_Logging:
        return AiCfg.alarmConfig.alarmLogging[0];
        break;
    case ID_AI_Alarm_Level1_OutputType:
        return AiCfg.alarmConfig.alarmOutputType[0];
        break;
    case ID_AI_Alarm_Level1_OutputNo:
        return AiCfg.alarmConfig.alarmOuputNo[0];
        break;

    case ID_AI_Alarm_Level2_OnOff:
        return AiCfg.alarmConfig.alarmOnOff[1];
        break;
    case ID_AI_Alarm_Level2_Type:
        return AiCfg.alarmConfig.alarmType[1];
        break;
    case ID_AI_Alarm_Level2_Value:
    {
        int pos = getAIChannelDataAccuracy(AiCfg);
        return getFloatToInt(AiCfg.alarmConfig.alarmValue[1], pos);
    }
        break;
    case ID_AI_Alarm_Level2_Hysteresis:
    {
        int pos = getAIChannelDataAccuracy(AiCfg);
        return getFloatToInt(AiCfg.alarmConfig.alarmHysteresis[1], pos);
    }
        break;
    case ID_AI_Alarm_Level2_Logging:
        return AiCfg.alarmConfig.alarmLogging[1];
        break;
    case ID_AI_Alarm_Level2_OutputType:
        return AiCfg.alarmConfig.alarmOutputType[1];
        break;
    case ID_AI_Alarm_Level2_OutputNo:
        return AiCfg.alarmConfig.alarmOuputNo[1];
        break;

    case ID_AI_Alarm_Level3_OnOff:
        return AiCfg.alarmConfig.alarmOnOff[2];
        break;
    case ID_AI_Alarm_Level3_Type:
        return AiCfg.alarmConfig.alarmType[2];
        break;
    case ID_AI_Alarm_Level3_Value:
    {
        int pos = getAIChannelDataAccuracy(AiCfg);
        return getFloatToInt(AiCfg.alarmConfig.alarmValue[2], pos);
    }
        break;
    case ID_AI_Alarm_Level3_Hysteresis:
    {
        int pos = getAIChannelDataAccuracy(AiCfg);
        return getFloatToInt(AiCfg.alarmConfig.alarmHysteresis[2], pos);
    }
        break;
    case ID_AI_Alarm_Level3_Logging:
        return AiCfg.alarmConfig.alarmLogging[2];
        break;
    case ID_AI_Alarm_Level3_OutputType:
        return AiCfg.alarmConfig.alarmOutputType[2];
        break;
    case ID_AI_Alarm_Level3_OutputNo:
        return AiCfg.alarmConfig.alarmOuputNo[2];
        break;

    case ID_AI_Alarm_Level4_OnOff:
        return AiCfg.alarmConfig.alarmOnOff[3];
        break;
    case ID_AI_Alarm_Level4_Type:
        return AiCfg.alarmConfig.alarmType[3];
        break;
    case ID_AI_Alarm_Level4_Value:
    {
        int pos = getAIChannelDataAccuracy(AiCfg);
        return getFloatToInt(AiCfg.alarmConfig.alarmValue[3], pos);
    }
        break;
    case ID_AI_Alarm_Level4_Hysteresis:
    {
        int pos = getAIChannelDataAccuracy(AiCfg);
        return getFloatToInt(AiCfg.alarmConfig.alarmHysteresis[3], pos);
    }
    case ID_AI_Alarm_Level4_Logging:
        return AiCfg.alarmConfig.alarmLogging[3];
        break;
    case ID_AI_Alarm_Level4_OutputType:
        return AiCfg.alarmConfig.alarmOutputType[3];
        break;
    case ID_AI_Alarm_Level4_OutputNo:
        return AiCfg.alarmConfig.alarmOuputNo[3];
        break;

    case ID_AI_Alarm_Leve_Delay_s:
        return AiCfg.alarmConfig.alarmDelay[0];
        break;
    case ID_AI_Alarm_Leve_Delay_m:
        return AiCfg.alarmConfig.alarmDelay[1];
        break;
    case ID_AI_Alarm_Leve_Delay_h:
        return AiCfg.alarmConfig.alarmDelay[2];
        break;
    default:
        break;
    }
    return QVariant();
}

//AI-Display
QVariant Cfgconfig_AI::getAIDisplayData(quint32 ID, quint32 group)
{
    AI_CONFIG AiCfg = *CfgChannel::instance()->getAIConfig(group);
    switch (ID) {
    case ID_AI_Disp_Color_Value:
        return AiCfg.displayConfig.displayColor;
        break;
    case ID_AI_Disp_Tag_Char:
        return getCharToString(AiCfg.displayConfig.displayTagChar);
        break;
    case ID_AI_Disp_Tag_No:
        return getCharToString(AiCfg.displayConfig.displayTagNo);
        break;
    case ID_AI_Disp_Zone_Upper:
        return AiCfg.displayConfig.displayZoneUpper;
        break;
    case ID_AI_Disp_Zone_Lower:
        return AiCfg.displayConfig.displayZoneLower;
        break;
    case ID_AI_Disp_Scale_Pos:
        return AiCfg.displayConfig.displayScalePos;
        break;
    case ID_AI_Disp_Scale_Div:
        return AiCfg.displayConfig.displayScaleDiv;
        break;
    case ID_AI_DISP_Bar_Pos:
        return AiCfg.displayConfig.displayBarPos;
        break;
    case ID_AI_DISP_Bar_Div:
        return AiCfg.displayConfig.displayBarDiv;
        break;
    case ID_AI_DISP_Partial_Pos:
        return AiCfg.displayConfig.displayPartialPos;
        break;
    case ID_AI_DISP_Partial_Boundary:
    {
        int pos = getAIChannelDataAccuracy(AiCfg);
        return getFloatToInt(AiCfg.displayConfig.displayPartialBoundary, pos);
    }
        break;
    case ID_AI_DISP_Partial_OnOff:
        return AiCfg.displayConfig.displayPartialOnOff;
        break;
    case ID_AI_DISP_ColorScaleBand_Mode:
        return AiCfg.displayConfig.displayColorbandMode;
        break;
    case ID_AI_DISP_ColorScaleBand_PosUpper:
    {
        int pos = getAIChannelDataAccuracy(AiCfg);
        return getFloatToInt(AiCfg.displayConfig.displayColorbandPosUpper, pos);
    }
        break;
    case ID_AI_DISP_ColorScaleBand_PosLower:
    {
        int pos = getAIChannelDataAccuracy(AiCfg);
        return getFloatToInt(AiCfg.displayConfig.displayColorbandPosLower, pos);
    }
        break;
    case ID_AI_DISP_ColorScaleBand_Color:
        return AiCfg.displayConfig.displayColorbandColor;
        break;
    case ID_AI_DISP_AlarmMark_OnOff:
        return AiCfg.displayConfig.displayAlarmMarkOnOff;
        break;
    case ID_AI_DISP_AlarmMark_Type:
        return AiCfg.displayConfig.displayAlarmMarkType;
        break;
    case ID_AI_DISP_AlarmMark_Color1:
        return AiCfg.displayConfig.displayAlarmMarkColor[0];
        break;
    case ID_AI_DISP_AlarmMark_Color2:
        return AiCfg.displayConfig.displayAlarmMarkColor[1];
        break;
    case ID_AI_DISP_AlarmMark_Color3:
        return AiCfg.displayConfig.displayAlarmMarkColor[2];
        break;
    case ID_AI_DISP_AlarmMark_Color4:
        return AiCfg.displayConfig.displayAlarmMarkColor[3];
        break;
    case ID_AI_DISP_DI0_Str:
        return getCharToString(AiCfg.displayConfig.displayDI0);
        break;
    case ID_AI_DISP_DI1_Str:
        return getCharToString(AiCfg.displayConfig.displayDI1);
        break;
    default:
        break;
    }
    return QVariant();
}

//AI-crorection
QVariant Cfgconfig_AI::getAICrorectionData(quint32 ID, quint32 group)
{
    AI_CONFIG AiCfg = *CfgChannel::instance()->getAIConfig(group);
    switch (ID) {
    case ID_AI_Correct_ModeVlaue:
        return AiCfg.correctConfig.correctMode;
        break;
    case ID_AI_Correct_ModeCnt:
        return AiCfg.correctConfig.correctCount;
        break;
    case ID_AI_Correct_Point1_In:
    {
        int pos = getAIChannelDataAccuracy(AiCfg);
        return getFloatToInt(AiCfg.correctConfig.correctPointIn[0], pos);
    }
        break;
    case ID_AI_Correct_Point1_Out:
    {
        int pos = getAIChannelDataAccuracy(AiCfg);
        return getFloatToInt(AiCfg.correctConfig.correctPointOut[0], pos);
    }
        break;
    case ID_AI_Correct_Point2_In:
    {
        int pos = getAIChannelDataAccuracy(AiCfg);
        return getFloatToInt(AiCfg.correctConfig.correctPointIn[1], pos);
    }
        break;
    case ID_AI_Correct_Point2_Out:
    {
        int pos = getAIChannelDataAccuracy(AiCfg);
        return getFloatToInt(AiCfg.correctConfig.correctPointOut[1], pos);
    }
        break;
    case ID_AI_Correct_Point3_In:
    {
        int pos = getAIChannelDataAccuracy(AiCfg);
        return getFloatToInt(AiCfg.correctConfig.correctPointIn[2], pos);
    }
        break;
    case ID_AI_Correct_Point3_Out:
    {
        int pos = getAIChannelDataAccuracy(AiCfg);
        return getFloatToInt(AiCfg.correctConfig.correctPointOut[2], pos);
    }
        break;
    case ID_AI_Correct_Point4_In:
    {
        int pos = getAIChannelDataAccuracy(AiCfg);
        return getFloatToInt(AiCfg.correctConfig.correctPointIn[3], pos);
    }
        break;
    case ID_AI_Correct_Point4_Out:
    {
        int pos = getAIChannelDataAccuracy(AiCfg);
        return getFloatToInt(AiCfg.correctConfig.correctPointOut[3], pos);
    }
        break;
    case ID_AI_Correct_Point5_In:
    {
        int pos = getAIChannelDataAccuracy(AiCfg);
        return getFloatToInt(AiCfg.correctConfig.correctPointIn[4], pos);
    }
        break;
    case ID_AI_Correct_Point5_Out:
    {
        int pos = getAIChannelDataAccuracy(AiCfg);
        return getFloatToInt(AiCfg.correctConfig.correctPointOut[4], pos);
    }
        break;
    case ID_AI_Correct_Point6_In:
    {
        int pos = getAIChannelDataAccuracy(AiCfg);
        return getFloatToInt(AiCfg.correctConfig.correctPointIn[5], pos);
    }
        break;
    case ID_AI_Correct_Point6_Out:
    {
        int pos = getAIChannelDataAccuracy(AiCfg);
        return getFloatToInt(AiCfg.correctConfig.correctPointOut[5], pos);
    }
        break;
    case ID_AI_Correct_Point7_In:
    {
        int pos = getAIChannelDataAccuracy(AiCfg);
        return getFloatToInt(AiCfg.correctConfig.correctPointIn[6], pos);
    }
        break;
    case ID_AI_Correct_Point7_Out:
    {
        int pos = getAIChannelDataAccuracy(AiCfg);
        return getFloatToInt(AiCfg.correctConfig.correctPointOut[6], pos);
    }
        break;
    case ID_AI_Correct_Point8_In:
    {
        int pos = getAIChannelDataAccuracy(AiCfg);
        return getFloatToInt(AiCfg.correctConfig.correctPointIn[7], pos);
    }
        break;
    case ID_AI_Correct_Point8_Out:
    {
        int pos = getAIChannelDataAccuracy(AiCfg);
        return getFloatToInt(AiCfg.correctConfig.correctPointOut[7], pos);
    }
        break;
    case ID_AI_Correct_Point9_In:
    {
        int pos = getAIChannelDataAccuracy(AiCfg);
        return getFloatToInt(AiCfg.correctConfig.correctPointIn[8], pos);
    }
        break;
    case ID_AI_Correct_Point9_Out:
    {
        int pos = getAIChannelDataAccuracy(AiCfg);
        return getFloatToInt(AiCfg.correctConfig.correctPointOut[8], pos);
    }
        break;
    case ID_AI_Correct_Point10_In:
    {
        int pos = getAIChannelDataAccuracy(AiCfg);
        return getFloatToInt(AiCfg.correctConfig.correctPointIn[9], pos);
    }
        break;
    case ID_AI_Correct_Point10_Out:
    {
        int pos = getAIChannelDataAccuracy(AiCfg);
        return getFloatToInt(AiCfg.correctConfig.correctPointOut[9], pos);
    }
        break;
    case ID_AI_Correct_Point11_In:
    {
        int pos = getAIChannelDataAccuracy(AiCfg);
        return getFloatToInt(AiCfg.correctConfig.correctPointIn[10], pos);
    }
        break;
    case ID_AI_Correct_Point11_Out:
    {
        int pos = getAIChannelDataAccuracy(AiCfg);
        return getFloatToInt(AiCfg.correctConfig.correctPointOut[10], pos);
    }
        break;
    case ID_AI_Correct_Point12_In:
    {
        int pos = getAIChannelDataAccuracy(AiCfg);
        return getFloatToInt(AiCfg.correctConfig.correctPointIn[11], pos);
    }
        break;
    case ID_AI_Correct_Point12_Out:
    {
        int pos = getAIChannelDataAccuracy(AiCfg);
        return getFloatToInt(AiCfg.correctConfig.correctPointOut[11], pos);
    }
        break;
    default:
        break;
    }
    return QVariant();
}




//得到AI通道的对象名称描述
QString Cfgconfig_AI::getAIRangeStr(quint32 ID, quint32 /*group*/)
{
    switch (ID) {
    case ID_AI:
        return tr("AI Channel settings");
        break;
    case ID_AI_Last_CH:
        return tr("Last-CH");
        break;
    case ID_AI_First_CH:
        return tr("First-CH");
        break;
    case ID_AI_Range:
    case ID_AI_Range_Range:
    case ID_AI_Range_Range_Range:
        return tr("Range");
        break;
    case ID_AI_Range_Range_Type:
        return tr("Type");
        break;
    case ID_AI_Range_Range_Min:
        return tr("Range Lower");
        break;
    case ID_AI_Range_Range_Max:
        return tr("Range Upper");
        break;
    case ID_AI_Range_Range_DispMin:
        return tr("Span Lower");
        break;
    case ID_AI_Range_Range_DispMax:
        return tr("Span Upper");
        break;
    case ID_AI_Range_Range_Calculate:
        return tr("Calculation");
        break;
    case ID_AI_Range_Range_BasicChannel:
        return tr("Reference channel");
        break;

    case ID_AI_Range_Scale:
        return tr("Scale");
        break;
    case ID_AI_Range_Scale_Pos:
        return tr("Decimal place");
        break;
    case ID_AI_Range_Scale_Min:
        return tr("Scale Lower");
        break;
    case ID_AI_Range_Scale_Max:
        return tr("Scale Upper");
        break;
    case ID_AI_Range_Scale_Unit:
        return tr("Unit");
        break;

    case ID_AI_Range_LowCut:
        return tr("Low-cut");
        break;
    case ID_AI_Range_LowCut_OnOff:
    case ID_AI_Range_MoveAverange_OnOff:
        return tr("On/Off");
        break;
    case ID_AI_Range_LowCut_Value:
        return tr("Low-cut value");
        break;
    case ID_AI_Range_LowCut_Output:
        return tr("Low-cut output");
        break;

    case ID_AI_Range_MoveAverange:
        return tr("Moving average");
        break;
    case ID_AI_Range_MoveAverange_Cnt:
        return tr("Count");
        break;

    case ID_AI_Range_RJC:
        return tr("RJC");
        break;
    case ID_AI_Range_RJC_Mode:
        return tr("Mode");
        break;
    case ID_AI_Range_RJC_Temp:
        return tr("Temperature");
        break;
    case ID_AI_Range_Burnout:
        return tr("Burnout set");
        break;
    case ID_AI_Range_Burnout_Mode:
        return tr("Mode");
        break;
    case ID_AI_Range_Bias:
        return tr("Bias");
        break;
    case ID_AI_Range_Bias_Value:
        return tr("Value");
        break;
    default:
        break;
    }
    return NULL;
}


QString Cfgconfig_AI::getAIAlarmStr(quint32 ID, quint32 /*group*/)
{
    switch (ID) {
    case ID_AI_Alarm:
        return tr("Alarm");
        break;

    case ID_AI_Alarm_Level1:
        return QString(tr("Level ") + "1");
        break;
    case ID_AI_Alarm_Level2:
        return QString(tr("Level ") + "2");
        break;
    case ID_AI_Alarm_Level3:
        return QString(tr("Level ") + "3");
        break;
    case ID_AI_Alarm_Level4:
        return QString(tr("Level ") + "4");
        break;

        break;
    case ID_AI_Alarm_Level1_OnOff:
    case ID_AI_Alarm_Level2_OnOff:
    case ID_AI_Alarm_Level3_OnOff:
    case ID_AI_Alarm_Level4_OnOff:
        return tr("On/Off");
        break;
    case ID_AI_Alarm_Level1_Type:
    case ID_AI_Alarm_Level2_Type:
    case ID_AI_Alarm_Level3_Type:
    case ID_AI_Alarm_Level4_Type:
        return tr("Type");
        break;
    case ID_AI_Alarm_Level1_Value:
    case ID_AI_Alarm_Level2_Value:
    case ID_AI_Alarm_Level3_Value:
    case ID_AI_Alarm_Level4_Value:
        return tr("Value");
        break;
    case ID_AI_Alarm_Level1_Hysteresis:
    case ID_AI_Alarm_Level2_Hysteresis:
    case ID_AI_Alarm_Level3_Hysteresis:
    case ID_AI_Alarm_Level4_Hysteresis:
        return tr("Hysteresis");
        break;
    case ID_AI_Alarm_Level1_Logging:
    case ID_AI_Alarm_Level2_Logging:
    case ID_AI_Alarm_Level3_Logging:
    case ID_AI_Alarm_Level4_Logging:
        return tr("Logging");
        break;
    case ID_AI_Alarm_Level1_OutputType:
    case ID_AI_Alarm_Level2_OutputType:
    case ID_AI_Alarm_Level3_OutputType:
    case ID_AI_Alarm_Level4_OutputType:
        return tr("Output type");
        break;
    case ID_AI_Alarm_Level1_OutputNo:
    case ID_AI_Alarm_Level2_OutputNo:
    case ID_AI_Alarm_Level3_OutputNo:
    case ID_AI_Alarm_Level4_OutputNo:
        return tr("Output No.");
        break;
    case ID_AI_Alarm_Leve_Delay:                  //alarm-delay
        return tr("Alarm delay");
        break;
    case ID_AI_Alarm_Leve_Delay_s:
        return tr("Second");
    case ID_AI_Alarm_Leve_Delay_m:
        return tr("Minute");
    case ID_AI_Alarm_Leve_Delay_h:
        return tr("Hour");
        break;
    default:
        break;
    }
    return NULL;
}

QString Cfgconfig_AI::getAIDisplayStr(quint32 ID, quint32 /*group*/)
{
    switch (ID) {
    case ID_AI_Disp:
        return tr("Display settings");
        break;

    case ID_AI_Disp_Color:
    case ID_AI_Disp_Color_Value:
    case ID_AI_DISP_ColorScaleBand_Color:
        return tr("Color");
        break;

    case ID_AI_Disp_Tag:
        return tr("Tag");
        break;
    case ID_AI_Disp_Tag_Char:
        return tr("Characters");
        break;
    case ID_AI_Disp_Tag_No:
        return tr("No.");
        break;

    case ID_AI_Disp_Zone:
        return tr("Zone");
        break;
    case ID_AI_Disp_Zone_Upper:
        return tr("Upper");
        break;
    case ID_AI_Disp_Zone_Lower:
        return tr("Lower");
        break;

    case ID_AI_Disp_Scale:
        return tr("Scale");
        break;
    case ID_AI_Disp_Scale_Pos:
        return tr("Position");
        break;
    case ID_AI_Disp_Scale_Div:
        return tr("Division");
        break;

    case ID_AI_DISP_Bar:
        return tr("Bar graph");
        break;
    case ID_AI_DISP_Bar_Pos:
        return tr("Base position");
        break;
    case ID_AI_DISP_Bar_Div:
        return tr("Division");
        break;

    case ID_AI_DISP_Partial:
        return tr("Partial");
        break;
    case ID_AI_DISP_Partial_OnOff:
        return tr("On/Off");
        break;
    case ID_AI_DISP_Partial_Boundary:
        return tr("Boundary");
        break;
    case ID_AI_DISP_Partial_Pos:
        return tr("Expand");
        break;

    case ID_AI_DISP_ColorScaleBand:
        return tr("Color scale band");
        break;
    case ID_AI_DISP_ColorScaleBand_PosLower:
        return tr("Display position Lower");
        break;
    case ID_AI_DISP_ColorScaleBand_PosUpper:
        return tr("Display position Upper");
        break;
    case ID_AI_DISP_ColorScaleBand_Mode:
        return tr("Band area");
        break;

    case ID_AI_DISP_AlarmMark:              //报警标记分组
        return tr("Alarm point mark");
        break;
    case ID_AI_DISP_AlarmMark_OnOff:        //报警标记开关，0-0ff, 1-on
        return tr("Indicate on Scale");
        break;
    case ID_AI_DISP_AlarmMark_Type:     //报警标记类型
        return tr("Mark type");
        break;
    case ID_AI_DISP_AlarmMark_Color1:       //报警颜色， Byte2-Red, Byte1-Green, Byte0-Blue
        return tr("Alarm 1 color");
        break;
    case ID_AI_DISP_AlarmMark_Color2:
        return tr("Alarm 2 color");
        break;
    case ID_AI_DISP_AlarmMark_Color3:
        return tr("Alarm 3 color");
        break;
    case ID_AI_DISP_AlarmMark_Color4:
        return tr("Alarm 4 color");
        break;

    case ID_AI_DISP_DI:    //【DI】类型量程且运算off时有效，测量值为0/1时显示的字符串
        return tr("Display characters of each value");
        break;
    case ID_AI_DISP_DI0_Str:        //最多8个半角字符，中英文
        return tr("0");
        break;
    case ID_AI_DISP_DI1_Str:
        return tr("1");
        break;
    default:
        break;
    }
    return NULL;
}

QString Cfgconfig_AI::getAICrorectionStr(quint32 ID, quint32 /*group*/)
{

    switch (ID) {
    case ID_AI_Correct:
        return tr("Calibration correction");
        break;

    case ID_AI_Correct_Mode:
    case ID_AI_Correct_ModeVlaue:
        return tr("Mode");
        break;
    case ID_AI_Correct_ModeCnt:
        return tr("Number of set points");
        break;
    case ID_AI_Correct_Point1:
        return tr("1");
    case ID_AI_Correct_Point2:
        return tr("2");
    case ID_AI_Correct_Point3:
        return tr("3");
    case ID_AI_Correct_Point4:
        return tr("4");
    case ID_AI_Correct_Point5:
        return tr("5");
    case ID_AI_Correct_Point6:
        return tr("6");
    case ID_AI_Correct_Point7:
        return tr("7");
    case ID_AI_Correct_Point8:
        return tr("8");
    case ID_AI_Correct_Point9:
        return tr("9");
    case ID_AI_Correct_Point10:
        return tr("10");
    case ID_AI_Correct_Point11:
        return tr("11");
    case ID_AI_Correct_Point12:
        return tr("12");
        break;
    case ID_AI_Correct_Point1_In:
    case ID_AI_Correct_Point2_In:
    case ID_AI_Correct_Point3_In:
    case ID_AI_Correct_Point4_In:
    case ID_AI_Correct_Point5_In:
    case ID_AI_Correct_Point6_In:
    case ID_AI_Correct_Point7_In:
    case ID_AI_Correct_Point8_In:
    case ID_AI_Correct_Point9_In:
    case ID_AI_Correct_Point10_In:
    case ID_AI_Correct_Point11_In:
    case ID_AI_Correct_Point12_In:
        return tr("Linearizer input");
        break;
    case ID_AI_Correct_Point1_Out:
    case ID_AI_Correct_Point2_Out:
    case ID_AI_Correct_Point3_Out:
    case ID_AI_Correct_Point4_Out:
    case ID_AI_Correct_Point5_Out:
    case ID_AI_Correct_Point6_Out:
    case ID_AI_Correct_Point7_Out:
    case ID_AI_Correct_Point8_Out:
    case ID_AI_Correct_Point9_Out:
    case ID_AI_Correct_Point10_Out:
    case ID_AI_Correct_Point11_Out:
    case ID_AI_Correct_Point12_Out:
        return tr("Linearizer output");
        break;
    case ID_AI_Correct_Point1_StartIn:
    case ID_AI_Correct_Point2_StartIn:
    case ID_AI_Correct_Point3_StartIn:
    case ID_AI_Correct_Point4_StartIn:
    case ID_AI_Correct_Point5_StartIn:
    case ID_AI_Correct_Point6_StartIn:
    case ID_AI_Correct_Point7_StartIn:
    case ID_AI_Correct_Point8_StartIn:
    case ID_AI_Correct_Point9_StartIn:
    case ID_AI_Correct_Point10_StartIn:
    case ID_AI_Correct_Point11_StartIn:
    case ID_AI_Correct_Point12_StartIn:
        return tr("Execution of the input measurement");
        break;
    default:
        break;
    }
    return NULL;
}




/*
 * 通过ID，组和对象得到AI/Range相对应对象配置值的值描述
 *       para : ID       配置对象的ID
 *              group    配置对象的group
 *              cfg      配置对象
 *  retrun :   对象的值描述
*/
QString Cfgconfig_AI::getAIRangeValueObjStr(quint32 ID, quint32 /*group*/, ConfigObject *cfg)
{
    QString cfgData = cfg->getData().toString();
    switch (ID) {
    case ID_AI_Range_Range_Type:
        return getAIRangeTypeValueObjStr(cfgData.toInt());
        break;
    case ID_AI_Range_Range_Range:
    {
        quint32 type = ConfigTreeObject::getConfigData("Config:AI Delegate:Range:Range:Type").toInt();
        quint32 range = ConfigTreeObject::getConfigData("Config:AI Delegate:Range:Range:Range").toInt();
        return getAIRangeRangeValueObjStr(type, range);
    }
    case ID_AI_Range_Range_Min:
    case ID_AI_Range_Range_Max:
    case ID_AI_Range_Range_DispMin:
    case ID_AI_Range_Range_DispMax:
        return getAIDataUnitObjStr(cfgData.toFloat());
        break;

    case ID_AI_Range_Range_Calculate:
        return getAIRangeCalCulateValueObjStr(cfgData.toInt());
        break;
    case ID_AI_Range_Range_BasicChannel:
        return getAIRangeRangeBasicChannelValueObjStr(cfg->getData().toUInt());
        break;

    case ID_AI_Range_Scale_Min:
    case ID_AI_Range_Scale_Max:
    case ID_AI_Range_Bias_Value:
    {
        quint32 decCnt = getAIRangeDecCnt(AI_DELEGATE_RANGE_RANGE_CALCU,AI_DELEGATE_RANGE_SCALE_DECPOS,AI_DELEGATE_RANGE_RANGE_UPPER);
        float data = (float)cfg->getData().toInt() / ::pow10(decCnt);
        return getAIRangScaleUnit(data);
    }
        break;

    case ID_AI_Range_LowCut_OnOff:
    case ID_AI_Range_MoveAverange_OnOff:
        return cfgData.toInt() == 0 ? tr("Off") : tr("On");
        break;
    case ID_AI_Range_LowCut_Output:
        return cfgData.toInt() == 0 ? tr("Output 0%") : tr("Output linear");
        break;
    case ID_AI_Range_LowCut_Value:
        return QString("%1 %").arg(QString::number(cfgData.toFloat(), 'f', 1));
        break;
    case ID_AI_Range_RJC_Mode:
        return cfgData.toInt() == 0 ? tr("Internal") : tr("External");
        break;
    case ID_AI_Range_RJC_Temp:
        return QString("%1 °C").arg(cfgData);
        break;
    case ID_AI_Range_Burnout_Mode:
    {
        int mode = ConfigTreeObject::getConfigData("Config:AI Delegate:Range:Burnout set:Mode").toInt();
        if (mode == 0)
            return tr("Off");
        else if (mode == 1)
            return tr("Up");
        else if (mode == 2)
            return tr("Down");
        else
            return NULL;
    }
        break;

    case ID_AI_Range_Scale_Unit:
        return getAIRangeScaleUnitValObjStr(cfgData);
        break;

    default:
        break;
    }
    return cfgData;
}

/*
 * 通过量程类型和量程量程类型得到带单位的字符串
 *      para:   data    : 配置值
 *      return : 返回带单位的配置值字符串
*/
QString Cfgconfig_AI::getAIDataUnitObjStr(float data)
{
    quint32 type = ConfigTreeObject::getConfigData("Config:AI Delegate:Range:Range:Type").toInt();
    quint32 range = ConfigTreeObject::getConfigData("Config:AI Delegate:Range:Range:Range").toInt();
    if (type == AI_RANGE_Type_Disabled)
        return NULL;
    else if (type == AI_RANGE_Type_Vlot)
    {
        switch (range) {
        case AI_Range_Volt_20mV:
            return QString("%1 mV").arg(QString::number(data, 'f', 3));
            break;
        case AI_Range_Volt_60mV:
        case AI_Range_Volt_200mV:
            return QString("%1 mV").arg(QString::number(data, 'f', 2));
            break;
        case AI_Range_Volt_1V:
        case AI_Range_Volt_2V:
            return QString("%1 V").arg(QString::number(data, 'f', 4));
            break;
        case AI_Range_Volt_6V:
        case AI_Range_Volt_20V:
            return QString("%1 V").arg(QString::number(data, 'f', 3));
            break;
        case AI_Range_Volt_50V:
            return QString("%1 V").arg(QString::number(data, 'f', 2));
            break;
        default:
            break;
        }
    }
    else if (type == AI_RANGE_Type_GS)
    {
        switch (range) {
        case AI_Range_GS_Low:
        case AI_Range_GS_High:
            return QString("%1 V").arg(QString::number(data, 'f', 4));
        }
    }
    else if (type == AI_RANGE_Type_TC)
    {
        switch (range) {
        case AI_Range_TC_R:
        case AI_Range_TC_S:
        case AI_Range_TC_B:
        case AI_Range_TC_K:
        case AI_Range_TC_K_H:
        case AI_Range_TC_E:
        case AI_Range_TC_C:
        case AI_Range_TC_T:
        case AI_Range_TC_N:
        case AI_Range_TC_W:
        case AI_Range_TC_L:
        case AI_Range_TC_U:
        case AI_Range_TC_WRe3_25:
        case AI_Range_TC_PLATINEL:
        case AI_Range_TC_PR20_40:
            return QString("%1 ° C").arg(QString::number(data, 'f', 1));
            break;
        case AI_Range_TC_KpvsAu7Fe:
            return QString("%1 K").arg(QString::number(data, 'f', 1));
            break;
        case AI_Range_TC_NiNiMo:
        case AI_Range_TC_WWRe26:
        case AI_Range_TC_N14:
        case AI_Range_TC_XK:
            return QString("%1 ° C").arg(QString::number(data, 'f', 1));
            break;
        }
    }
    else if (type == AI_RANGE_Type_RTD)
    {
        switch (range) {
        case AI_Range_RTD_Pt100:
            return QString("%1 ° C").arg(QString::number(data, 'f', 1));
            break;
        case AI_Range_RTD_Pt100_H:
            return QString("%1 ° C").arg(QString::number(data, 'f', 2));
            break;
        case AI_Range_RTD_JPt100:
            return QString("%1 ° C").arg(QString::number(data, 'f', 1));
            break;
        case AI_Range_RTD_JPt100_H:
            return QString("%1 ° C").arg(QString::number(data, 'f', 2));
            break;
        case AI_Range_RTD_Cu10GE:
        case AI_Range_RTD_Cu10LN:
        case AI_Range_RTD_Cu10WEED:
        case AI_Range_RTD_Cu10BAILEY:
        case AI_Range_RTD_Cu10a392:
        case AI_Range_RTD_Cu10a393:
        case AI_Range_RTD_Cu25:
        case AI_Range_RTD_Cu53:
        case AI_Range_RTD_Cu100:
            return QString("%1 ° C").arg(QString::number(data, 'f', 1));
            break;
        case AI_Range_RTD_J263B:
            return QString("%1 K").arg(QString::number(data, 'f', 1));
            break;
        case AI_Range_RTD_Ni100SAMA:
        case AI_Range_RTD_Ni100DN:
        case AI_Range_RTD_Ni120:
        case AI_Range_RTD_Pt25:
        case AI_Range_RTD_Pt50:
        case AI_Range_RTD_Pt200WEED:
        case AI_Range_RTD_Cu10G:
        case AI_Range_RTD_Cu50G:
        case AI_Range_RTD_Cu100G:
        case AI_Range_RTD_Pt46G:
        case AI_Range_RTD_Pt100G:
            return QString("%1 ° C").arg(QString::number(data, 'f', 1));
            break;
        }
    }
    else if (type == AI_RANGE_Type_DI)
    {
        switch (range) {
        case AI_Range_DI_Level:
        case AI_Range_DI_Contact:
            return QString("%1").arg(data);
            break;
        }
    }
    else
    {
        return QString("%1 mA").arg(data);
    }
    return NULL;
}


QString Cfgconfig_AI::getAIRangScaleUnit(float data)
{
    QString unit = ConfigTreeObject::getConfigData(AI_DELEGATE_RANGE_SCALE_UNIT).toString();
    quint32 caltype = ConfigTreeObject::getConfigData(AI_DELEGATE_RANGE_RANGE_CALCU).toInt();
    quint32 decCnt = ConfigTreeObject::getConfigData(AI_DELEGATE_RANGE_SCALE_DECPOS).toInt();

    if (caltype == AI_Range_Calculate_Linear || caltype == AI_Range_Calculate_Sqrt)      //线性转换以获取测量值
        return QString::number(data, 'f', decCnt) + " " + unit;
    return getAIDataUnitObjStr(data);
}

/*
 * 返回小数点位置
 * 当标尺打开的时，小数点位置就是标尺小数点位置
 * 当标尺关闭的时候，小数点位置随着量程量程显示最大值和最小值变化
*/
quint32 Cfgconfig_AI::getAIRangeDecCnt(QString calPath, QString posPath, QString rangeUpperPath)
{
    quint32 decCnt;
    quint32 caltype = ConfigTreeObject::getConfigData(calPath).toInt();

    if (caltype == AI_Range_Calculate_Linear || caltype == AI_Range_Calculate_Sqrt)      //线性转换以获取测量值
    {
        decCnt= ConfigTreeObject::getConfigData(posPath).toInt();
    }
    else
    {
        ConfigObject *cfg = ConfigTreeObject::getConfigObject(rangeUpperPath);
        QString showOn = getAIValueObjStr(ID_AI_Range_Range_DispMax, 0, cfg);

        QStringList strList = showOn.split(" ");
        QStringList pointList = strList[0].split(".");
        if(pointList.size() >= 2) {
            decCnt = pointList.at(1).size();
        }
    }
    return decCnt;
}

/*
 * 通过ID，组和对象得到AI/Alarm相对应对象配置值的值描述
 *       para : ID       配置对象的ID
 *              group    配置对象的group
 *              cfg      配置对象
 *  retrun :   对象的值描述
*/
QString Cfgconfig_AI::getAIAlarmValueObjStr(quint32 ID, quint32 /*group*/, ConfigObject *cfg)
{
    QString cfgData = cfg->getData().toString();
    switch (ID) {
    case ID_AI_Alarm_Level1_OnOff:      //alarm-level1
    case ID_AI_Alarm_Level2_OnOff:
    case ID_AI_Alarm_Level3_OnOff:
    case ID_AI_Alarm_Level4_OnOff:
    case ID_AI_Alarm_Level1_Logging:
    case ID_AI_Alarm_Level2_Logging:
    case ID_AI_Alarm_Level3_Logging:
    case ID_AI_Alarm_Level4_Logging:
        return cfgData.toInt() == 0 ? tr("Off") : tr("On");
        break;
    case ID_AI_Alarm_Level1_Type:
    case ID_AI_Alarm_Level2_Type:
    case ID_AI_Alarm_Level3_Type:
    case ID_AI_Alarm_Level4_Type:
        return getAIAlarmTypeValObjStr(cfgData.toInt());
        break;

    case ID_AI_Alarm_Level1_Value:
    case ID_AI_Alarm_Level1_Hysteresis:
    case ID_AI_Alarm_Level2_Value:
    case ID_AI_Alarm_Level2_Hysteresis:
    case ID_AI_Alarm_Level3_Value:
    case ID_AI_Alarm_Level3_Hysteresis:
    case ID_AI_Alarm_Level4_Value:
    case ID_AI_Alarm_Level4_Hysteresis:
    {
        quint32 decCnt = getAIRangeDecCnt(AI_DELEGATE_RANGE_RANGE_CALCU,AI_DELEGATE_RANGE_SCALE_DECPOS,AI_DELEGATE_RANGE_RANGE_UPPER);
        float data = (float)cfg->getData().toInt() / ::pow10(decCnt);
        QString strShowOn = getAIRangScaleUnit(data);
        DigitalOperation digOpe;
        return digOpe.getAllowedData(ID, strShowOn, cfg);
    }
        break;

    case ID_AI_Alarm_Level1_OutputType:
    case ID_AI_Alarm_Level2_OutputType:
    case ID_AI_Alarm_Level3_OutputType:
    case ID_AI_Alarm_Level4_OutputType:
        return getAIAlarmLevelOutputType(cfgData.toInt());
        break;

    case ID_AI_Alarm_Level1_OutputNo:
    case ID_AI_Alarm_Level2_OutputNo:
    case ID_AI_Alarm_Level3_OutputNo:
    case ID_AI_Alarm_Level4_OutputNo:
        return getAIAlarmChannelObjValStr(cfg->getData().toUInt());
    default:
        break;
    }
    return cfgData;
}

/*
 * 通过ID，组和对象得到AI/Display相对应对象配置值的值描述
 *       para : ID       配置对象的ID
 *              group    配置对象的group
 *              cfg      配置对象
 *  retrun :   对象的值描述
*/
QString Cfgconfig_AI::getAIDisplayValueObjStr(quint32 ID, quint32 /*group*/, ConfigObject *cfg)
{
    QString cfgData = cfg->getData().toString();
    switch (ID) {
    case ID_AI_Disp_Color_Value:        //通道颜色，Byte2-Red, Byte1-Green, Byte0-Blue
    case ID_AI_DISP_ColorScaleBand_Color:       //彩色标尺带颜色，Byte2-Red, Byte1-Green, Byte0-Blue
    case ID_AI_DISP_AlarmMark_Color1:       //报警1-4颜色， Byte2-Red, Byte1-Green, Byte0-Blue
    case ID_AI_DISP_AlarmMark_Color2:
    case ID_AI_DISP_AlarmMark_Color3:
    case ID_AI_DISP_AlarmMark_Color4:
        return getAIDisplayColorValueObjStr(cfg->getData().toUInt());
        break;
    case ID_AI_Disp_Tag_Char:       //标记字符串，中英支持，最多32个半角字符
    case ID_AI_Disp_Tag_No:         //标记号码，符号，最多16个半角字符
        return QString(tr("%1").arg(cfgData));
        break;

    case ID_AI_Disp_Zone_Upper:       //显示区域上限，5-100,单位%
    case ID_AI_Disp_Zone_Lower:       //显示区域上限，0-95,单位%
    case ID_AI_DISP_Partial_Pos:       //部分压缩放大位置，1-99, 单位：%，默认50
        return QString("%1 ").arg(cfgData) + QString("%");
        break;
    case ID_AI_Disp_Scale_Pos:
    {
        int type = cfgData.toInt();
        if (type == 0) return tr("Off");
        return cfgData;
    }
        break;
    case ID_AI_Disp_Scale_Div:
    {
        int type = cfgData.toInt();
        if (type == 13) return "C10";
        return cfgData;
    }
        break;

    case ID_AI_DISP_Bar_Pos:      //棒图基准位置， 0-Lower, 1-Center, 2-Upper
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

    case ID_AI_DISP_Partial_OnOff:       //部分压缩放大开关,0-off, 1-on
    case ID_AI_DISP_AlarmMark_OnOff:       //报警标记开关，0-0ff, 1-on
        return cfgData.toInt() == 0 ? tr("Off") : tr("On");
        break;

    case ID_AI_DISP_ColorScaleBand_Mode:       //彩色标尺带模式， 0-off, 1-内侧， 2-外侧.
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

    case ID_AI_DISP_AlarmMark_Type:       //报警标记类型，0-报警， 1-固定
        return cfgData.toInt() == 0 ? tr("Alarm") : tr("Fixed");
        break;

    case ID_AI_DISP_ColorScaleBand_PosLower:       //彩色标尺带显示位置下限
    case ID_AI_DISP_ColorScaleBand_PosUpper:       //彩色标尺带显示位置上限
    case  ID_AI_DISP_Partial_Boundary:
    {
        quint32 decCnt = getAIRangeDecCnt(AI_DELEGATE_RANGE_RANGE_CALCU,AI_DELEGATE_RANGE_SCALE_DECPOS,AI_DELEGATE_RANGE_RANGE_UPPER);
        float data = (float)cfg->getData().toInt() / ::pow10(decCnt);
        QString strShowOn = getAIRangScaleUnit(data);
        DigitalOperation digOpe;
        return digOpe.getAllowedData(ID, strShowOn, cfg);
    }
        break;

    default:
        break;
    }
    return cfgData;
}

/*
 * 把从配置中得到的颜色配置值转换为界面label显示的样式字符串返回
*/
QString Cfgconfig_AI::getAIDisplayColorValueObjStr(quint32 x)
{
    //quint32 x = cfg->getData().toUInt();
    QColor colorRGBx(COLOR_RED(x), COLOR_GREEN(x), COLOR_BLUE(x));
    QString str = QString("QLabel {background-color: %1}").arg(colorRGBx.name());
    return str;
}

/*
 * 通过ID，组和对象得到AI/Correction相对应对象配置值的值描述
 *       para : ID       配置对象的ID
 *              group    配置对象的group
 *              cfg      配置对象
 *  retrun :   对象的值描述
*/
QString Cfgconfig_AI::getAICrorectionValueObjStr(quint32 ID, quint32 /*group*/, ConfigObject *cfg)
{
    QString cfgData = cfg->getData().toString();
    switch (ID) {
    case ID_AI_Correct_ModeVlaue:           //校准模式，0-off, 1-折线近似， 2-折线偏移
    {
        int modeValue = cfgData.toInt();
        if (modeValue == 0)
            return tr("Off");
        else if (modeValue == 1)
            return tr("Linearizer Approximation");
        else
            return tr("Linearizer Bias");
    }
        break;
    case ID_AI_Correct_Point1_In:             //折线输入点
    case ID_AI_Correct_Point2_In:
    case ID_AI_Correct_Point3_In:
    case ID_AI_Correct_Point4_In:
    case ID_AI_Correct_Point5_In:
    case ID_AI_Correct_Point6_In:
    case ID_AI_Correct_Point7_In:
    case ID_AI_Correct_Point8_In:
    case ID_AI_Correct_Point9_In:
    case ID_AI_Correct_Point10_In:
    case ID_AI_Correct_Point11_In:
    case ID_AI_Correct_Point12_In:

    case ID_AI_Correct_Point1_Out:
    case ID_AI_Correct_Point2_Out:
    case ID_AI_Correct_Point3_Out:
    case ID_AI_Correct_Point4_Out:
    case ID_AI_Correct_Point5_Out:
    case ID_AI_Correct_Point6_Out:
    case ID_AI_Correct_Point7_Out:
    case ID_AI_Correct_Point8_Out:
    case ID_AI_Correct_Point9_Out:
    case ID_AI_Correct_Point10_Out:
    case ID_AI_Correct_Point11_Out:
    case ID_AI_Correct_Point12_Out:
    {
        quint32 decCnt = getAIRangeDecCnt(AI_DELEGATE_RANGE_RANGE_CALCU,AI_DELEGATE_RANGE_SCALE_DECPOS,AI_DELEGATE_RANGE_RANGE_UPPER);
        float data = (float)cfg->getData().toInt() / ::pow10(decCnt);
        QString strShowOn = getAIRangScaleUnit(data);
        DigitalOperation digOpe;
        return digOpe.getAllowedData(ID, strShowOn, cfg);
    }
        break;
    default:
        break;
    }
    return cfgData;
}


/*
 * 得到AI/Range/Range中分割线单位
*/
QString Cfgconfig_AI::getAIRangeScaleUnitValObjStr(uchar *unit)
{
    QString unitStr((char *)unit);
    return tr(unitStr.toLatin1());
}

/*
 * 得到AI/Range/Range中分割线单位
*/
QString Cfgconfig_AI::getAIRangeScaleUnitValObjStr(QString unit)
{
    QStringList str;
    str << AI_DELEGATE_CALI_1_INPUT << AI_DELEGATE_CALI_1_OUTPUT
        << AI_DELEGATE_CALI_2_INPUT << AI_DELEGATE_CALI_2_OUTPUT
        << AI_DELEGATE_CALI_3_INPUT << AI_DELEGATE_CALI_3_OUTPUT
        << AI_DELEGATE_CALI_4_INPUT << AI_DELEGATE_CALI_4_OUTPUT
        << AI_DELEGATE_CALI_5_INPUT << AI_DELEGATE_CALI_5_OUTPUT
        << AI_DELEGATE_CALI_6_INPUT << AI_DELEGATE_CALI_6_OUTPUT
        << AI_DELEGATE_CALI_7_INPUT << AI_DELEGATE_CALI_7_OUTPUT
        << AI_DELEGATE_CALI_8_INPUT << AI_DELEGATE_CALI_8_OUTPUT
        << AI_DELEGATE_CALI_9_INPUT << AI_DELEGATE_CALI_9_OUTPUT
        << AI_DELEGATE_CALI_10_INPUT << AI_DELEGATE_CALI_10_OUTPUT
        << AI_DELEGATE_CALI_11_INPUT << AI_DELEGATE_CALI_11_OUTPUT
        << AI_DELEGATE_CALI_12_INPUT << AI_DELEGATE_CALI_12_OUTPUT
        << AI_DELEGATE_ALARM_LVL1_HYST << AI_DELEGATE_ALARM_LVL1_VALUE
        << AI_DELEGATE_ALARM_LVL2_HYST << AI_DELEGATE_ALARM_LVL2_VALUE
        << AI_DELEGATE_ALARM_LVL3_HYST << AI_DELEGATE_ALARM_LVL3_VALUE
        << AI_DELEGATE_ALARM_LVL4_HYST << AI_DELEGATE_ALARM_LVL4_VALUE
        << AI_DELEGATE_RANGE_SCALE_LOWER << AI_DELEGATE_RANGE_SCALE_UPPER
        << AI_DELEGATE_DISP_CSBAND_UPPER << AI_DELEGATE_DISP_CSBAND_LOWER
        << AI_DELEGATE_DISP_PART_BOUND << AI_DELEGATE_RANGE_BIAS_VALUE;

    for (int i = str.count() - 1; i >= 0; i--)
    {
        QString strShow = ConfigTreeObject::getConfigObject(str[i])->getStrShowON();
        QStringList on = strShow.split(" ");
        ConfigTreeObject::getConfigObject(str[i])->setStrShowOn(on[0] + " " + unit);
    }
    return unit;
}

/*
 * 得到AI/Range/Range中量程类型配置值描述
*/
QString Cfgconfig_AI::getAIRangeTypeValueObjStr(quint32 type)
{
    //AI_CONFIG AiCfg = *CfgChannel::instance()->getAIConfig(group);
    switch (type) {
    case AI_RANGE_Type_Disabled:
        return tr("Skip");
        break;
    case AI_RANGE_Type_Vlot:
        return tr("Volt");
        break;
    case AI_RANGE_Type_GS:
        return tr("GS");
        break;
    case AI_RANGE_Type_TC:
        return tr("TC");
        break;
    case AI_RANGE_Type_RTD:
        return tr("RTD");
        break;
    case AI_RANGE_Type_DI:
        return tr("DI");
        break;
    case AI_RANGE_Type_Cur0_20mA:
        return tr("Current(0-20mA)");
        break;
    case AI_RANGE_Type_GS4_20mA:
        return tr("GS(4-20mA)");
        break;
    default:
        break;
    }
    return NULL;
}

QString Cfgconfig_AI::getAIRangeRangeValueObjStr(quint32 type, quint32 range)
{
    if (type == AI_RANGE_Type_Disabled)
        return NULL;
    else if (type == AI_RANGE_Type_Vlot)
    {
        switch (range) {
        case AI_Range_Volt_20mV:
            return "20mV";
            break;
        case AI_Range_Volt_60mV:
            return "60mV";
            break;
        case AI_Range_Volt_200mV:
            return "200mV";
            break;
        case AI_Range_Volt_1V:
            return "1V";
            break;
        case AI_Range_Volt_2V:
            return "2V";
            break;
        case AI_Range_Volt_6V:
            return "6V";
            break;
        case AI_Range_Volt_20V:
            return "20V";
            break;
        case AI_Range_Volt_50V:
            return "50V";
            break;
        default:
            break;
        }
    }
    else if (type == AI_RANGE_Type_GS)
    {
        switch (range) {
        case AI_Range_GS_Low:
            return "0.4-2V";
            break;
        case AI_Range_GS_High:
            return "1-5V";
            break;
        default:
            break;
        }
    }
    else if (type == AI_RANGE_Type_TC)
    {
        switch (range) {
        case AI_Range_TC_R:
            return "R";
            break;
        case AI_Range_TC_S:
            return "S";
            break;
        case AI_Range_TC_B:
            return "B";
            break;
        case AI_Range_TC_K:
            return "K";
            break;
        case AI_Range_TC_K_H:
            return "K-H";
            break;
        case AI_Range_TC_E:
            return "E";
            break;
        case AI_Range_TC_C:
            return "C";
            break;
        case AI_Range_TC_T:
            return "T";
            break;
        case AI_Range_TC_N:
            return "N";
            break;
        case AI_Range_TC_W:
            return "W";
            break;
        case AI_Range_TC_L:
            return "L";
            break;
        case AI_Range_TC_U:
            return "U";
            break;
        case AI_Range_TC_WRe3_25:
            return "Wre3_25";
            break;
        case AI_Range_TC_PLATINEL:
            return "PLATINE";
            break;
        case AI_Range_TC_PR20_40:
            return "PR20_40";
            break;
        case AI_Range_TC_KpvsAu7Fe:
            return "KpvsAu7Fe";
            break;
        case AI_Range_TC_NiNiMo:
            return "NiNiMo";
            break;
        case AI_Range_TC_WWRe26:
            return "WWRe26";
            break;
        case AI_Range_TC_N14:
            return "N14";
            break;
        case AI_Range_TC_XK:
            return "XK";
            break;
        default:
            break;
        }
    }
    else if (type == AI_RANGE_Type_RTD)
    {
        switch (range) {
        case AI_Range_RTD_Pt100:
            return "Pt100";
            break;
        case AI_Range_RTD_Pt100_H:
            return "Pt100-H";
            break;
        case AI_Range_RTD_JPt100:
            return "JPt100";
            break;
        case AI_Range_RTD_JPt100_H:
            return "JPt100-H";
            break;
        case AI_Range_RTD_Cu10GE:
            return "Cu10GE";
            break;
        case AI_Range_RTD_Cu10LN:
            return "Cu10LN";
            break;
        case AI_Range_RTD_Cu10WEED:
            return "Cu10WEED";
            break;
        case AI_Range_RTD_Cu10BAILEY:
            return "Cu10BAILEY";
            break;
        case AI_Range_RTD_Cu10a392:
            return "Cu10a392";
            break;
        case AI_Range_RTD_Cu10a393:
            return "Cu10a393";
            break;
        case AI_Range_RTD_Cu25:
            return "Cu25";
            break;
        case AI_Range_RTD_Cu53:
            return "Cu53";
            break;
        case AI_Range_RTD_Cu100:
            return "Cu100";
            break;
        case AI_Range_RTD_J263B:
            return "J263B";
            break;
        case AI_Range_RTD_Ni100SAMA:
            return "Ni100SAMA";
            break;
        case AI_Range_RTD_Ni100DN:
            return "Ni100DN";
            break;
        case AI_Range_RTD_Ni120:
            return "Ni120";
            break;
        case AI_Range_RTD_Pt25:
            return "Pt25";
            break;
        case AI_Range_RTD_Pt50:
            return "Pt50";
            break;
        case AI_Range_RTD_Pt200WEED:
            return "Pt200WEED";
            break;
        case AI_Range_RTD_Cu10G:
            return "Cu10G";
            break;
        case AI_Range_RTD_Cu50G:
            return "Cu50G";
            break;
        case AI_Range_RTD_Cu100G:
            return "Cu100G";
            break;
        case AI_Range_RTD_Pt46G:
            return "Pt46G";
            break;
        case AI_Range_RTD_Pt100G:
            return "Pt100G";
            break;
        default:
            break;
        }

    }
    else if (type == AI_RANGE_Type_DI)
    {
        switch (range) {
        case AI_Range_DI_Level:
            return "Level";
            break;
        case AI_Range_DI_Contact:
            return "DI";
            break;
        default:
            break;
        }
    }
    else if (type == AI_RANGE_Type_Cur0_20mA)
    {
        return "Cur0_20mA";
    }
    else if (type == AI_RANGE_Type_GS4_20mA)
    {
        return "GS4_20mA";
    }

    return NULL;
}

QString Cfgconfig_AI::getAIRangeRangeBasicChannelValueObjStr(quint32 data)
{
    if (data == 0)
    {
        if (CfgChannel::instance()->getEffectiveChannel().size() != 0)
            data= CfgChannel::instance()->getEffectiveChannel().first();
        else
            return NULL;
    }
    int type = CHANNEL_TYPE(data);
    switch (type) {
    case Channel_Type_AI:
        return CfgChannel::instance()->getExistChannelAI().key(data);
        break;
    case Channel_Type_DI:
        return CfgChannel::instance()->getExistChannelDI().key(data);
        break;
    case Channel_Type_DO:
        return CfgChannel::instance()->getExistChannelDO().key(data);
        break;
    default:
        break;
    }
    return NULL;
}

//把32位通道号转化为相应的四位通道     quint32 --> QString
QString Cfgconfig_AI::getAIAlarmChannelObjValStr(quint32 num)
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
//            int index = CfgChannel::instance()->getExistChannelDO().values().indexOf(num);
//            if (index >= 0)
//                return CfgChannel::instance()->getExistChannelDO().keys().at(index);
//            else
//                return "0001";   //switch
        }
    }
}

/*
 * 通过值与组信息得到传入数据的的字符串(带单位)
*/
QString Cfgconfig_AI::getAIDataUnitObjStr(float data, quint32 group)
{
    AI_CONFIG AiCfg = *CfgChannel::instance()->getAIConfig(group);
    int type = AiCfg.rangeConfig.rangeType;
    if (type == AI_RANGE_Type_Disabled)
        return NULL;
    else if (type == AI_RANGE_Type_Vlot)
    {
        switch (AiCfg.rangeConfig.config.rangeVolt) {
        case AI_Range_Volt_20mV:
        case AI_Range_Volt_60mV:
        case AI_Range_Volt_200mV:
            return QString("%1 mV").arg(data);
            break;
        case AI_Range_Volt_1V:
        case AI_Range_Volt_2V:
        case AI_Range_Volt_6V:
        case AI_Range_Volt_20V:
        case AI_Range_Volt_50V:
            return QString("%1 V").arg(data);
        default:
            break;
        }
    }
    else if (type == AI_RANGE_Type_GS)
    {
        return QString("%1 V").arg(data);
    }
    else if (type == AI_RANGE_Type_TC || type == AI_RANGE_Type_RTD)
    {
        return QString("%1 °C").arg(data);
    }
    else if (type == AI_RANGE_Type_DI)
    {
        return QString("%1").arg(data);
    }
    else if (type == AI_RANGE_Type_Cur0_20mA || type == AI_RANGE_Type_GS4_20mA)
    {
        return QString("%1 mA").arg(data);
    }
    return NULL;
}

QString Cfgconfig_AI::getAIRangeCalCulateValueObjStr(quint32 calculateType)
{
    QString data;
    switch (calculateType) {
    case AI_Range_Calculate_Disabled:
        data = tr("Off");
        break;
    case AI_Range_Calculate_Delta:
        data = tr("Delta");
        break;
    case AI_Range_Calculate_Linear:
        data = tr("Linear scaling");
        break;
    case AI_Range_Calculate_Sqrt:
        data = tr("Sqrt root");
        break;
    case AI_Range_Calculate_Log:
        data =  tr("Log");
        break;
    case AI_Range_Calculate_LogApproach:
        data = tr("Log approach");
        break;
    case AI_Range_Calculate_LogLinear:
        data = tr("Log linear");
        break;
    default:
        break;
    }
    return data;
}

QString Cfgconfig_AI::getAiRangeBiasValueObjStr(quint32 data, quint32 type, quint32 range)
{
    if (type == AI_RANGE_Type_Disabled)
        return NULL;
    else if (type == AI_RANGE_Type_Vlot)
    {
        switch (range) {
        case AI_Range_Volt_20mV:
        case AI_Range_Volt_60mV:
        case AI_Range_Volt_200mV:
            return QString("%1 mV").arg(data);
            break;
        case AI_Range_Volt_1V:
        case AI_Range_Volt_2V:
        case AI_Range_Volt_6V:
        case AI_Range_Volt_20V:
        case AI_Range_Volt_50V:
            return QString("%1 V").arg(data);
        default:
            break;
        }
    }
    else if (type == AI_RANGE_Type_GS)
    {
        return QString("%1").arg(data);
    }
    else if (type == AI_RANGE_Type_TC || type == AI_RANGE_Type_RTD)
    {
        return QString("%1 °C").arg(data);
    }
    else if (type == AI_RANGE_Type_DI)
    {
        return QString("%1").arg(data);
    }
    else if (type == AI_RANGE_Type_Cur0_20mA || type == AI_RANGE_Type_GS4_20mA)
    {
        return QString("%1 mA").arg(data);
    }
    return NULL;
}


/*
 * 通过AI通道和报警等级号码得到相对应通道报警的描述
 *      AiCfg : AI通道
 *      alarmNum : 报警等级号码
*/
QString Cfgconfig_AI::getAIAlarmTypeValObjStr(quint32 type)
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
 * 通过报警输出类型配置得到报警输出类型的字符串描述
*/
QString Cfgconfig_AI::getAIAlarmLevelOutputType(quint32 outputType)
{
    if (outputType == 0)    //报警输出类型， 0-off, 1-Relay(继电器), 2-Switch(内部开关)
        return tr("Off");
    else if (outputType == 1)
        return tr("Relay");
    else
        return tr("Internal  switch");
}



/*
 * 把uchar *类型转化为QString类型
*/
QVariant Cfgconfig_AI::getCharToString(uchar *str)
{
    QString data((char *)str);
    return data;
}



/**************************************        save     **********************************************/
void Cfgconfig_AI::setAIRangeData(quint32 ID, quint32 group)
{
    int index = CfgChannel::instance()->getExistChannelAI().values().indexOf(group);
    QString chanNum = CfgChannel::instance()->getExistChannelAI().keys().at(index);

    AI_CONFIG *AiCfg = CfgChannel::instance()->getAIConfig(group);
    switch (ID) {
    case ID_AI_Range_Range_Type:
        AiCfg->rangeConfig.rangeType = AI_RANGE_Type(ConfigTreeObject::getConfigData(QString(AICONFIG_RANG_RANGE_TYPE).arg(chanNum)).toInt());
        break;
    case ID_AI_Range_Range_Range:
        setAIRangeConfigData(AiCfg, chanNum);
        break;
    case ID_AI_Range_Range_Min:
        AiCfg->rangeConfig.rangeMin = ConfigTreeObject::getConfigData(QString(AICONFIG_RANG_RANGE_MIN).arg(chanNum)).toFloat();
        break;
    case ID_AI_Range_Range_Max:
        AiCfg->rangeConfig.rangeMax = ConfigTreeObject::getConfigData(QString(AICONFIG_RANG_RANGE_MAX).arg(chanNum)).toFloat();
        break;
    case ID_AI_Range_Range_DispMin:
        AiCfg->rangeConfig.rangeDispMin = ConfigTreeObject::getConfigData(QString(AICONFIG_RANG_RANGE_LOWER).arg(chanNum)).toFloat();
        break;
    case ID_AI_Range_Range_DispMax:
        AiCfg->rangeConfig.rangeDispMax = ConfigTreeObject::getConfigData(QString(AICONFIG_RANG_RANGE_UPPER).arg(chanNum)).toFloat();
        break;

    case ID_AI_Range_Range_Calculate:
        AiCfg->rangeConfig.rangeCalculateType = (AI_Range_Calculate)ConfigTreeObject::getConfigData(QString(AICONFIG_RANG_RANGE_CALCU).arg(chanNum)).toInt();
        if (AiCfg->rangeConfig.rangeCalculateType != AI_Range_Calculate_Delta)
            break;
    case ID_AI_Range_Range_BasicChannel:        //通过字符串得到通道号  QString --> quint32
        AiCfg->rangeConfig.rangeBasicChannel = ConfigTreeObject::getConfigData(QString(AICONFIG_RANG_RANGE_REFER).arg(chanNum)).toUInt();
        break;
    case ID_AI_Range_Scale_Pos:      //小数点位置, 0/1/2/3/4/5
        AiCfg->rangeConfig.rangeScale.scalePosition = ConfigTreeObject::getConfigData(QString(AICONFIG_RANG_SCALE_DEC).arg(chanNum)).toInt();
        break;
    case ID_AI_Range_Scale_Min:
        AiCfg->rangeConfig.rangeScale.scaleMin = ConfigTreeObject::getConfigData(QString(AICONFIG_RANG_SCALE_LOWER).arg(chanNum)).toInt();
        break;
    case ID_AI_Range_Scale_Max:
        AiCfg->rangeConfig.rangeScale.scaleMax = ConfigTreeObject::getConfigData(QString(AICONFIG_RANG_SCALE_UPPER).arg(chanNum)).toInt();
        break;
    case ID_AI_Range_Scale_Unit:
        setAIRangeScaleUnit(AiCfg->rangeConfig.rangeScale.scaleUnit,
              ConfigTreeObject::getConfigData(QString(AICONFIG_RANG_SCALE_UNIT).arg(chanNum)).toString(), 8);
        break;
    case ID_AI_Range_LowCut_OnOff:
        AiCfg->rangeConfig.rangeLowCut.lowCutOnOff = ConfigTreeObject::getConfigData(QString(AICONFIG_RANGE_LOWCUT_ONOFF).arg(chanNum)).toUInt();
        break;
    case ID_AI_Range_LowCut_Value:
        AiCfg->rangeConfig.rangeLowCut.lowCutPos = ConfigTreeObject::getConfigData(QString(AICONFIG_RANGE_LOWCUT_VALUE).arg(chanNum)).toFloat();
        break;
    case ID_AI_Range_LowCut_Output:
        AiCfg->rangeConfig.rangeLowCut.lowCutOut = ConfigTreeObject::getConfigData(QString(AICONFIG_RANGE_LOWCUT_OUTPUT).arg(chanNum)).toInt();
        break;
    case ID_AI_Range_MoveAverange_OnOff:
        AiCfg->rangeConfig.rangeMovingAvg.avgOnOff = ConfigTreeObject::getConfigData(QString(AICONFIG_RANGE_MOVAVE_ONOFF).arg(chanNum)).toInt();
        break;
    case ID_AI_Range_MoveAverange_Cnt:
        AiCfg->rangeConfig.rangeMovingAvg.avgCount = ConfigTreeObject::getConfigData(QString(AICONFIG_RANGE_MOVAVE_COUNT).arg(chanNum)).toInt();
        break;
    case ID_AI_Range_RJC_Mode:
        AiCfg->rangeConfig.rangeRJCCompensation.rjcCompMode = ConfigTreeObject::getConfigData(QString(AICONFIG_RANGE_RJC_MODE).arg(chanNum)).toInt();
        break;
    case ID_AI_Range_RJC_Temp:
        AiCfg->rangeConfig.rangeRJCCompensation.rjcTemp = ConfigTreeObject::getConfigData(QString(AICONFIG_RANGE_RJC_TEMPER).arg(chanNum)).toFloat();
        break;
    case ID_AI_Range_Burnout_Mode:
        AiCfg->rangeConfig.rangeBurnout = AI_Range_Burnout(ConfigTreeObject::getConfigData(QString(AICONFIG_RANGE_BURN_MODE).arg(chanNum)).toInt());
        break;
    case ID_AI_Range_Bias_Value:
        AiCfg->rangeConfig.rangeBias = ConfigTreeObject::getConfigData(QString(AICONFIG_RANGE_BIAS_VALUE).arg(chanNum)).toInt();
        break;
    default:
        break;
    }
}

/*
 * save ai range config
*/
void Cfgconfig_AI::setAIRangeConfigData(AI_CONFIG *AiCfg, QString chanNum)
{
    QString path = AICONFIG_RANG_RANGE_RANGE;
    int type = ConfigTreeObject::getConfigData(QString(AICONFIG_RANG_RANGE_TYPE).arg(chanNum)).toInt();
    switch (type) {
    case AI_RANGE_Type_Disabled:
        break;
    case AI_RANGE_Type_Vlot:
        AiCfg->rangeConfig.config.rangeVolt = AI_Range_Volt(ConfigTreeObject::getConfigData(path.arg(chanNum)).toInt());
        break;
    case AI_RANGE_Type_GS:
        AiCfg->rangeConfig.config.rangeGS = AI_Range_GS(ConfigTreeObject::getConfigData(path.arg(chanNum)).toInt());
        break;
    case AI_RANGE_Type_TC:
        AiCfg->rangeConfig.config.rangeTC = AI_Range_TC(ConfigTreeObject::getConfigData(path.arg(chanNum)).toInt());
        break;
    case AI_RANGE_Type_RTD:
        AiCfg->rangeConfig.config.rangeRTD = AI_Range_RTD(ConfigTreeObject::getConfigData(path.arg(chanNum)).toInt());
        break;
    case AI_RANGE_Type_DI:
        AiCfg->rangeConfig.config.rangeDI = AI_Range_DI(ConfigTreeObject::getConfigData(path.arg(chanNum)).toInt());
        break;
    case AI_RANGE_Type_Cur0_20mA:
        AiCfg->rangeConfig.config.rangeCur0_20mA = AI_Range_Cur0_20mA(ConfigTreeObject::getConfigData(path.arg(chanNum)).toInt());
        break;
    case AI_RANGE_Type_GS4_20mA:
        AiCfg->rangeConfig.config.rangeGS4_20mA = AI_Range_GS4_20mA(ConfigTreeObject::getConfigData(path.arg(chanNum)).toInt());
        break;
    default:
        break;
    }
}

/*
 * save ai range scale unit
*/
void Cfgconfig_AI::setAIRangeScaleUnit(uchar * uData, QString str, int len)
{
    memset(uData, 0, len);
    QByteArray baUnit = str.toLocal8Bit();
    char* valueUnit = baUnit.data();
//    memcpy(uData, valueUnit, strlen(valueUnit));
    while (*valueUnit != '\0') {
        *uData++ = *valueUnit++;
    }
}


//save alarm data
void Cfgconfig_AI::setAIAlarmData(quint32 ID, quint32 group)
{
    int index = CfgChannel::instance()->getExistChannelAI().values().indexOf(group);
    QString chanNum = CfgChannel::instance()->getExistChannelAI().keys().at(index);
    AI_CONFIG *AiCfg = CfgChannel::instance()->getAIConfig(group);
    switch (ID) {
    case ID_AI_Alarm_Level1_OnOff:
        AiCfg->alarmConfig.alarmOnOff[0] = ConfigTreeObject::getConfigData(QString(AICONFIG_ALARM_LVL1_ONOFF).arg(chanNum)).toUInt();
        break;
    case ID_AI_Alarm_Level1_Type:
        AiCfg->alarmConfig.alarmType[0] = (Alarm_Type)ConfigTreeObject::getConfigData(QString(AICONFIG_ALARM_LVL1_TYPE).arg(chanNum)).toUInt();
        break;
    case ID_AI_Alarm_Level1_Value:
    {
        quint32 decCnt = getAIRangeDecCnt(QString(AICONFIG_RANG_RANGE_CALCU).arg(chanNum)\
                              ,QString(AICONFIG_RANG_SCALE_DEC).arg(chanNum),QString(AICONFIG_RANG_RANGE_UPPER).arg(chanNum));
        AiCfg->alarmConfig.alarmValue[0] = (float)ConfigTreeObject::getConfigData(QString(AICONFIG_ALARM_LVL1_VALUE).arg(chanNum)).toInt() / ::pow10(decCnt);
    }
        break;
    case ID_AI_Alarm_Level1_Hysteresis:
    {
        quint32 decCnt = getAIRangeDecCnt(QString(AICONFIG_RANG_RANGE_CALCU).arg(chanNum)\
                              ,QString(AICONFIG_RANG_SCALE_DEC).arg(chanNum),QString(AICONFIG_RANG_RANGE_UPPER).arg(chanNum));
        AiCfg->alarmConfig.alarmHysteresis[0] = (float)ConfigTreeObject::getConfigData(QString(AICONFIG_ALARM_LVL1_HYST).arg(chanNum)).toInt() / ::pow10(decCnt);
    }
        break;
    case ID_AI_Alarm_Level1_Logging:
        AiCfg->alarmConfig.alarmLogging[0] = ConfigTreeObject::getConfigData(QString(AICONFIG_ALARM_LVL1_LOGG).arg(chanNum)).toUInt();
        break;
    case ID_AI_Alarm_Level1_OutputType:
        AiCfg->alarmConfig.alarmOutputType[0] = ConfigTreeObject::getConfigData(QString(AICONFIG_ALARM_LVL1_OUTTYPE).arg(chanNum)).toUInt();
    case ID_AI_Alarm_Level1_OutputNo:
        AiCfg->alarmConfig.alarmOuputNo[0] = ConfigTreeObject::getConfigData(QString(AICONFIG_ALARM_LVL1_OUTNO).arg(chanNum)).toUInt();
        break;

    case ID_AI_Alarm_Level2_OnOff:
        AiCfg->alarmConfig.alarmOnOff[1] = ConfigTreeObject::getConfigData(QString(AICONFIG_ALARM_LVL2_ONOFF).arg(chanNum)).toUInt();
        break;
    case ID_AI_Alarm_Level2_Type:
        AiCfg->alarmConfig.alarmType[1] = (Alarm_Type)ConfigTreeObject::getConfigData(QString(AICONFIG_ALARM_LVL2_TYPE).arg(chanNum)).toUInt();
        break;
    case ID_AI_Alarm_Level2_Value:
    {
        quint32 decCnt = getAIRangeDecCnt(QString(AICONFIG_RANG_RANGE_CALCU).arg(chanNum)\
                              ,QString(AICONFIG_RANG_SCALE_DEC).arg(chanNum),QString(AICONFIG_RANG_RANGE_UPPER).arg(chanNum));
        AiCfg->alarmConfig.alarmValue[1] = (float)ConfigTreeObject::getConfigData(QString(AICONFIG_ALARM_LVL2_VALUE).arg(chanNum)).toInt() / ::pow10(decCnt);
    }
        break;
    case ID_AI_Alarm_Level2_Hysteresis:
    {
        quint32 decCnt = getAIRangeDecCnt(QString(AICONFIG_RANG_RANGE_CALCU).arg(chanNum)\
                              ,QString(AICONFIG_RANG_SCALE_DEC).arg(chanNum),QString(AICONFIG_RANG_RANGE_UPPER).arg(chanNum));
        AiCfg->alarmConfig.alarmHysteresis[1] = (float)ConfigTreeObject::getConfigData(QString(AICONFIG_ALARM_LVL2_HYST).arg(chanNum)).toInt() / ::pow10(decCnt);
    }
        break;
    case ID_AI_Alarm_Level2_Logging:
        AiCfg->alarmConfig.alarmLogging[1] = ConfigTreeObject::getConfigData(QString(AICONFIG_ALARM_LVL2_LOGG).arg(chanNum)).toUInt();
        break;
    case ID_AI_Alarm_Level2_OutputType:
        AiCfg->alarmConfig.alarmOutputType[1] = ConfigTreeObject::getConfigData(QString(AICONFIG_ALARM_LVL2_OUTTYPE).arg(chanNum)).toUInt();
    case ID_AI_Alarm_Level2_OutputNo:
        AiCfg->alarmConfig.alarmOuputNo[1] = ConfigTreeObject::getConfigData(QString(AICONFIG_ALARM_LVL2_OUTNO).arg(chanNum)).toUInt();
        break;

    case ID_AI_Alarm_Level3_OnOff:
        AiCfg->alarmConfig.alarmOnOff[2] = ConfigTreeObject::getConfigData(QString(AICONFIG_ALARM_LVL3_ONOFF).arg(chanNum)).toUInt();
        break;
    case ID_AI_Alarm_Level3_Type:
        AiCfg->alarmConfig.alarmType[2] = (Alarm_Type)ConfigTreeObject::getConfigData(QString(AICONFIG_ALARM_LVL3_TYPE).arg(chanNum)).toUInt();
        break;
    case ID_AI_Alarm_Level3_Value:
    {
        quint32 decCnt = getAIRangeDecCnt(QString(AICONFIG_RANG_RANGE_CALCU).arg(chanNum)\
                              ,QString(AICONFIG_RANG_SCALE_DEC).arg(chanNum),QString(AICONFIG_RANG_RANGE_UPPER).arg(chanNum));
        AiCfg->alarmConfig.alarmValue[2] = (float)ConfigTreeObject::getConfigData(QString(AICONFIG_ALARM_LVL3_VALUE).arg(chanNum)).toInt() / ::pow10(decCnt);
    }
        break;
    case ID_AI_Alarm_Level3_Hysteresis:
    {
        quint32 decCnt = getAIRangeDecCnt(QString(AICONFIG_RANG_RANGE_CALCU).arg(chanNum)\
                              ,QString(AICONFIG_RANG_SCALE_DEC).arg(chanNum),QString(AICONFIG_RANG_RANGE_UPPER).arg(chanNum));
        AiCfg->alarmConfig.alarmHysteresis[2] = (float)ConfigTreeObject::getConfigData(QString(AICONFIG_ALARM_LVL3_HYST).arg(chanNum)).toInt() / ::pow10(decCnt);
    }
        break;
    case ID_AI_Alarm_Level3_Logging:
        AiCfg->alarmConfig.alarmLogging[2] = ConfigTreeObject::getConfigData(QString(AICONFIG_ALARM_LVL3_LOGG).arg(chanNum)).toUInt();
        break;
    case ID_AI_Alarm_Level3_OutputType:
        AiCfg->alarmConfig.alarmOutputType[2] = ConfigTreeObject::getConfigData(QString(AICONFIG_ALARM_LVL3_OUTTYPE).arg(chanNum)).toUInt();
    case ID_AI_Alarm_Level3_OutputNo:
        AiCfg->alarmConfig.alarmOuputNo[2] = ConfigTreeObject::getConfigData(QString(AICONFIG_ALARM_LVL3_OUTNO).arg(chanNum)).toUInt();
        break;

    case ID_AI_Alarm_Level4_OnOff:
        AiCfg->alarmConfig.alarmOnOff[3] = ConfigTreeObject::getConfigData(QString(AICONFIG_ALARM_LVL4_ONOFF).arg(chanNum)).toUInt();
        break;
    case ID_AI_Alarm_Level4_Type:
        AiCfg->alarmConfig.alarmType[3] = (Alarm_Type)ConfigTreeObject::getConfigData(QString(AICONFIG_ALARM_LVL4_TYPE).arg(chanNum)).toUInt();
        break;
    case ID_AI_Alarm_Level4_Value:
    {
        quint32 decCnt = getAIRangeDecCnt(QString(AICONFIG_RANG_RANGE_CALCU).arg(chanNum)\
                              ,QString(AICONFIG_RANG_SCALE_DEC).arg(chanNum),QString(AICONFIG_RANG_RANGE_UPPER).arg(chanNum));
        AiCfg->alarmConfig.alarmValue[3] = (float)ConfigTreeObject::getConfigData(QString(AICONFIG_ALARM_LVL4_VALUE).arg(chanNum)).toInt() / ::pow10(decCnt);
    }
        break;
    case ID_AI_Alarm_Level4_Hysteresis:
    {
        quint32 decCnt = getAIRangeDecCnt(QString(AICONFIG_RANG_RANGE_CALCU).arg(chanNum)\
                              ,QString(AICONFIG_RANG_SCALE_DEC).arg(chanNum),QString(AICONFIG_RANG_RANGE_UPPER).arg(chanNum));
        AiCfg->alarmConfig.alarmHysteresis[3] = (float)ConfigTreeObject::getConfigData(QString(AICONFIG_ALARM_LVL4_HYST).arg(chanNum)).toInt() / ::pow10(decCnt);
    }
        break;
    case ID_AI_Alarm_Level4_Logging:
        AiCfg->alarmConfig.alarmLogging[3] = ConfigTreeObject::getConfigData(QString(AICONFIG_ALARM_LVL4_LOGG).arg(chanNum)).toUInt();
        break;
    case ID_AI_Alarm_Level4_OutputType:
        AiCfg->alarmConfig.alarmOutputType[3] = ConfigTreeObject::getConfigData(QString(AICONFIG_ALARM_LVL4_OUTTYPE).arg(chanNum)).toUInt();
    case ID_AI_Alarm_Level4_OutputNo:
        AiCfg->alarmConfig.alarmOuputNo[3] = ConfigTreeObject::getConfigData(QString(AICONFIG_ALARM_LVL4_OUTNO).arg(chanNum)).toUInt();
        break;

    case ID_AI_Alarm_Leve_Delay_s:
        AiCfg->alarmConfig.alarmDelay[0] = ConfigTreeObject::getConfigData(QString(AICONFIG_ALARM_DELAY_SEC).arg(chanNum)).toUInt();
        break;
    case ID_AI_Alarm_Leve_Delay_m:
        AiCfg->alarmConfig.alarmDelay[1] = ConfigTreeObject::getConfigData(QString(AICONFIG_ALARM_DELAY_MIN).arg(chanNum)).toUInt();
        break;
    case ID_AI_Alarm_Leve_Delay_h:
        AiCfg->alarmConfig.alarmDelay[2] = ConfigTreeObject::getConfigData(QString(AICONFIG_ALARM_DELAY_HOUR).arg(chanNum)).toUInt();
        break;
    default:
        break;
    }
}

//save display data
void Cfgconfig_AI::setAIDisplayData(quint32 ID, quint32 group)
{
    int index = CfgChannel::instance()->getExistChannelAI().values().indexOf(group);
    QString chanNum = CfgChannel::instance()->getExistChannelAI().keys().at(index);
    AI_CONFIG *AiCfg = CfgChannel::instance()->getAIConfig(group);
    switch (ID) {
    case ID_AI_Disp_Color_Value:
        AiCfg->displayConfig.displayColor = ConfigTreeObject::getConfigData(QString(AICONFIG_DISP_COLOR_COLOR).arg(chanNum)).toUInt();
        break;
    case ID_AI_Disp_Tag_Char:
        setAIRangeScaleUnit(AiCfg->displayConfig.displayTagChar, ConfigTreeObject::getConfigData(QString(AICONFIG_DISP_TAG_CHAR).arg(chanNum)).toString(), 32);
        break;
    case ID_AI_Disp_Tag_No:
        setAIRangeScaleUnit(AiCfg->displayConfig.displayTagNo, ConfigTreeObject::getConfigData(QString(AICONFIG_DISP_TAG_NO).arg(chanNum)).toString(), 16);
        break;
    case ID_AI_Disp_Zone_Upper:
        AiCfg->displayConfig.displayZoneUpper = ConfigTreeObject::getConfigData(QString(AICONFIG_DISP_ZONE_UPPER).arg(chanNum)).toUInt();
        break;
    case ID_AI_Disp_Zone_Lower:
        AiCfg->displayConfig.displayZoneLower = ConfigTreeObject::getConfigData(QString(AICONFIG_DISP_ZONE_LOWER).arg(chanNum)).toUInt();
        break;
    case ID_AI_Disp_Scale_Pos:
        AiCfg->displayConfig.displayScalePos = ConfigTreeObject::getConfigData(QString(AICONFIG_DISP_SCALE_POS).arg(chanNum)).toUInt();
        break;
    case ID_AI_Disp_Scale_Div:
        AiCfg->displayConfig.displayScaleDiv = ConfigTreeObject::getConfigData(QString(AICONFIG_DISP_SCALE_DIV).arg(chanNum)).toUInt();
        break;
    case ID_AI_DISP_Bar_Pos:
        AiCfg->displayConfig.displayBarPos = ConfigTreeObject::getConfigData(QString(AICONFIG_DISP_BAR_BASEPOS).arg(chanNum)).toUInt();
        break;
    case ID_AI_DISP_Bar_Div:
        AiCfg->displayConfig.displayBarDiv = ConfigTreeObject::getConfigData(QString(AICONFIG_DISP_BAR_DIVISION).arg(chanNum)).toUInt();
        break;
    case ID_AI_DISP_Partial_OnOff:
        AiCfg->displayConfig.displayPartialOnOff = ConfigTreeObject::getConfigData(QString(AICONFIG_DISP_PART_ONOFF).arg(chanNum)).toUInt();
        break;
    case ID_AI_DISP_Partial_Boundary:
    {
        quint32 decCnt = getAIRangeDecCnt(QString(AICONFIG_RANG_RANGE_CALCU).arg(chanNum)\
                              ,QString(AICONFIG_RANG_SCALE_DEC).arg(chanNum),QString(AICONFIG_RANG_RANGE_UPPER).arg(chanNum));
        AiCfg->displayConfig.displayPartialBoundary = (float)ConfigTreeObject::getConfigData(QString(AICONFIG_DISP_PART_BOUND).arg(chanNum)).toInt() / ::pow10(decCnt);
    }
        break;
    case ID_AI_DISP_Partial_Pos:
        AiCfg->displayConfig.displayPartialPos = ConfigTreeObject::getConfigData(QString(AICONFIG_DISP_PART_EXPAND).arg(chanNum)).toUInt();
        break;

    case ID_AI_DISP_ColorScaleBand_Color:
        AiCfg->displayConfig.displayColorbandColor = ConfigTreeObject::getConfigData(QString(AICONFIG_DISP_CSBAND_COLOR).arg(chanNum)).toUInt();
        break;
    case ID_AI_DISP_ColorScaleBand_PosLower:
    {
        quint32 decCnt = getAIRangeDecCnt(QString(AICONFIG_RANG_RANGE_CALCU).arg(chanNum)\
                              ,QString(AICONFIG_RANG_SCALE_DEC).arg(chanNum),QString(AICONFIG_RANG_RANGE_UPPER).arg(chanNum));
        AiCfg->displayConfig.displayColorbandPosLower = (float)ConfigTreeObject::getConfigData(QString(AICONFIG_DISP_CSBAND_LOWER).arg(chanNum)).toInt() / ::pow10(decCnt);
    }
        break;
    case ID_AI_DISP_ColorScaleBand_PosUpper:
    {
        quint32 decCnt = getAIRangeDecCnt(QString(AICONFIG_RANG_RANGE_CALCU).arg(chanNum)\
                              ,QString(AICONFIG_RANG_SCALE_DEC).arg(chanNum),QString(AICONFIG_RANG_RANGE_UPPER).arg(chanNum));
        AiCfg->displayConfig.displayColorbandPosUpper = (float)ConfigTreeObject::getConfigData(QString(AICONFIG_DISP_CSBAND_UPPER).arg(chanNum)).toInt() / ::pow10(decCnt);
    }
        break;
    case ID_AI_DISP_ColorScaleBand_Mode:
        AiCfg->displayConfig.displayColorbandMode = ConfigTreeObject::getConfigData(QString(AICONFIG_DISP_CSBAND_AREA).arg(chanNum)).toUInt();
        break;

    case ID_AI_DISP_AlarmMark_OnOff:
        AiCfg->displayConfig.displayAlarmMarkOnOff = ConfigTreeObject::getConfigData(QString(AICONFIG_DISP_ALARM_INDIC).arg(chanNum)).toUInt();
        break;
    case ID_AI_DISP_AlarmMark_Type:
        AiCfg->displayConfig.displayAlarmMarkType = ConfigTreeObject::getConfigData(QString(AICONFIG_DISP_ALARM_MARKTYPE).arg(chanNum)).toUInt();
        break;
    case ID_AI_DISP_AlarmMark_Color1:
        AiCfg->displayConfig.displayAlarmMarkColor[0] = ConfigTreeObject::getConfigData(QString(AICONFIG_DISP_ALARM_COLOR1).arg(chanNum)).toUInt();
        break;
    case ID_AI_DISP_AlarmMark_Color2:
        AiCfg->displayConfig.displayAlarmMarkColor[1] = ConfigTreeObject::getConfigData(QString(AICONFIG_DISP_ALARM_COLOR2).arg(chanNum)).toUInt();
        break;
    case ID_AI_DISP_AlarmMark_Color3:
        AiCfg->displayConfig.displayAlarmMarkColor[2] = ConfigTreeObject::getConfigData(QString(AICONFIG_DISP_ALARM_COLOR3).arg(chanNum)).toUInt();
        break;
    case ID_AI_DISP_AlarmMark_Color4:
        AiCfg->displayConfig.displayAlarmMarkColor[3] = ConfigTreeObject::getConfigData(QString(AICONFIG_DISP_ALARM_COLOR4).arg(chanNum)).toUInt();
        break;

    case ID_AI_DISP_DI0_Str:
        setAIRangeScaleUnit(AiCfg->displayConfig.displayDI0,ConfigTreeObject::getConfigData(QString(AICONFIG_DISP_DISPCHAR_0).arg(chanNum)).toString(), 8);
        break;
    case ID_AI_DISP_DI1_Str:
        setAIRangeScaleUnit(AiCfg->displayConfig.displayDI1, ConfigTreeObject::getConfigData(QString(AICONFIG_DISP_DISPCHAR_1).arg(chanNum)).toString(), 8);
        break;
    default:
        break;
    }
}

//save crorect data
void Cfgconfig_AI::setAICrorectionData(quint32 ID, quint32 group)
{
    int index = CfgChannel::instance()->getExistChannelAI().values().indexOf(group);
    QString chanNum = CfgChannel::instance()->getExistChannelAI().keys().at(index);
    AI_CONFIG *AiCfg = CfgChannel::instance()->getAIConfig(group);
    quint32 decCnt = getAIRangeDecCnt(QString(AICONFIG_RANG_RANGE_CALCU).arg(chanNum)\
                          ,QString(AICONFIG_RANG_SCALE_DEC).arg(chanNum),QString(AICONFIG_RANG_RANGE_UPPER).arg(chanNum));
    switch (ID) {
    case ID_AI_Correct_ModeVlaue:
        AiCfg->correctConfig.correctMode = ConfigTreeObject::getConfigData(QString(AICONFIG_CALI_MODE_MODE).arg(chanNum)).toUInt();
        break;
    case ID_AI_Correct_ModeCnt:
        AiCfg->correctConfig.correctCount = ConfigTreeObject::getConfigData(QString(AICONFIG_CALI_MODE_NUM).arg(chanNum)).toUInt();
        break;

    case ID_AI_Correct_Point1_In:
        AiCfg->correctConfig.correctPointIn[0] = (float)ConfigTreeObject::getConfigData(QString(AICONFIG_CALI_1_INPUT).arg(chanNum)).toInt() / ::pow10(decCnt);
        break;
    case ID_AI_Correct_Point1_Out:
        AiCfg->correctConfig.correctPointOut[0] = (float)ConfigTreeObject::getConfigData(QString(AICONFIG_CALI_1_OUTPUT).arg(chanNum)).toInt() / ::pow10(decCnt);
        break;

    case ID_AI_Correct_Point2_In:
        AiCfg->correctConfig.correctPointIn[1] = (float)ConfigTreeObject::getConfigData(QString(AICONFIG_CALI_2_INPUT).arg(chanNum)).toInt() / ::pow10(decCnt);
        break;
    case ID_AI_Correct_Point2_Out:
        AiCfg->correctConfig.correctPointOut[1] = (float)ConfigTreeObject::getConfigData(QString(AICONFIG_CALI_2_OUTPUT).arg(chanNum)).toInt() / ::pow10(decCnt);
        break;

    case ID_AI_Correct_Point3_In:
        AiCfg->correctConfig.correctPointIn[2] = (float)ConfigTreeObject::getConfigData(QString(AICONFIG_CALI_3_INPUT).arg(chanNum)).toInt() / ::pow10(decCnt);
        break;
    case ID_AI_Correct_Point3_Out:
        AiCfg->correctConfig.correctPointOut[2] = (float)ConfigTreeObject::getConfigData(QString(AICONFIG_CALI_3_OUTPUT).arg(chanNum)).toInt() / ::pow10(decCnt);
        break;

    case ID_AI_Correct_Point4_In:
        AiCfg->correctConfig.correctPointIn[3] = (float)ConfigTreeObject::getConfigData(QString(AICONFIG_CALI_4_INPUT).arg(chanNum)).toInt() / ::pow10(decCnt);
        break;
    case ID_AI_Correct_Point4_Out:
        AiCfg->correctConfig.correctPointOut[3] = (float)ConfigTreeObject::getConfigData(QString(AICONFIG_CALI_4_OUTPUT).arg(chanNum)).toInt() / ::pow10(decCnt);
        break;

    case ID_AI_Correct_Point5_In:
        AiCfg->correctConfig.correctPointIn[4] = (float)ConfigTreeObject::getConfigData(QString(AICONFIG_CALI_5_INPUT).arg(chanNum)).toInt() / ::pow10(decCnt);
        break;
    case ID_AI_Correct_Point5_Out:
        AiCfg->correctConfig.correctPointOut[4] = (float)ConfigTreeObject::getConfigData(QString(AICONFIG_CALI_5_OUTPUT).arg(chanNum)).toInt() / ::pow10(decCnt);
        break;

    case ID_AI_Correct_Point6_In:
        AiCfg->correctConfig.correctPointIn[5] = (float)ConfigTreeObject::getConfigData(QString(AICONFIG_CALI_6_INPUT).arg(chanNum)).toInt() / ::pow10(decCnt);
        break;
    case ID_AI_Correct_Point6_Out:
        AiCfg->correctConfig.correctPointOut[5] = (float)ConfigTreeObject::getConfigData(QString(AICONFIG_CALI_6_OUTPUT).arg(chanNum)).toInt() / ::pow10(decCnt);
        break;

    case ID_AI_Correct_Point7_In:
        AiCfg->correctConfig.correctPointIn[6] = (float)ConfigTreeObject::getConfigData(QString(AICONFIG_CALI_7_INPUT).arg(chanNum)).toInt() / ::pow10(decCnt);
        break;
    case ID_AI_Correct_Point7_Out:
        AiCfg->correctConfig.correctPointOut[6] = (float)ConfigTreeObject::getConfigData(QString(AICONFIG_CALI_7_OUTPUT).arg(chanNum)).toInt() / ::pow10(decCnt);
        break;

    case ID_AI_Correct_Point8_In:
        AiCfg->correctConfig.correctPointIn[7] = (float)ConfigTreeObject::getConfigData(QString(AICONFIG_CALI_8_INPUT).arg(chanNum)).toInt() / ::pow10(decCnt);
        break;
    case ID_AI_Correct_Point8_Out:
        AiCfg->correctConfig.correctPointOut[7] = (float)ConfigTreeObject::getConfigData(QString(AICONFIG_CALI_8_OUTPUT).arg(chanNum)).toInt() / ::pow10(decCnt);
        break;

    case ID_AI_Correct_Point9_In:
        AiCfg->correctConfig.correctPointIn[8] = (float)ConfigTreeObject::getConfigData(QString(AICONFIG_CALI_9_INPUT).arg(chanNum)).toInt() / ::pow10(decCnt);
        break;
    case ID_AI_Correct_Point9_Out:
        AiCfg->correctConfig.correctPointOut[8] = (float)ConfigTreeObject::getConfigData(QString(AICONFIG_CALI_9_OUTPUT).arg(chanNum)).toInt() / ::pow10(decCnt);
        break;

    case ID_AI_Correct_Point10_In:
        AiCfg->correctConfig.correctPointIn[9] = (float)ConfigTreeObject::getConfigData(QString(AICONFIG_CALI_10_INPUT).arg(chanNum)).toInt() / ::pow10(decCnt);
        break;
    case ID_AI_Correct_Point10_Out:
        AiCfg->correctConfig.correctPointOut[9] = (float)ConfigTreeObject::getConfigData(QString(AICONFIG_CALI_10_OUTPUT).arg(chanNum)).toInt() / ::pow10(decCnt);
        break;

    case ID_AI_Correct_Point11_In:
        AiCfg->correctConfig.correctPointIn[10] = (float)ConfigTreeObject::getConfigData(QString(AICONFIG_CALI_11_INPUT).arg(chanNum)).toInt() / ::pow10(decCnt);
        break;
    case ID_AI_Correct_Point11_Out:
        AiCfg->correctConfig.correctPointOut[10] = (float)ConfigTreeObject::getConfigData(QString(AICONFIG_CALI_11_OUTPUT).arg(chanNum)).toInt() / ::pow10(decCnt);
        break;

    case ID_AI_Correct_Point12_In:
        AiCfg->correctConfig.correctPointIn[11] = (float)ConfigTreeObject::getConfigData(QString(AICONFIG_CALI_12_INPUT).arg(chanNum)).toInt() / ::pow10(decCnt);
        break;
    case ID_AI_Correct_Point12_Out:
        AiCfg->correctConfig.correctPointOut[11] = (float)ConfigTreeObject::getConfigData(QString(AICONFIG_CALI_12_OUTPUT).arg(chanNum)).toInt() / ::pow10(decCnt);
        break;
    default:
        break;
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
quint32 Cfgconfig_AI::setAIOutputNo(quint32 type, quint32 code)
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






/*获取通道数值精度*/
quint32 Cfgconfig_AI::getAIChannelDataAccuracy(AI_CONFIG &AiCfg) const
{
//    AI_CONFIG AiCfg = *CfgChannel::instance()->getAIConfig(AiCfg);

    quint32 caltype = AiCfg.rangeConfig.rangeCalculateType;
    if (caltype == AI_Range_Calculate_Linear || caltype == AI_Range_Calculate_Sqrt)
        return AiCfg.rangeConfig.rangeScale.scalePosition;

    quint32 type = AiCfg.rangeConfig.rangeType;
    quint32 range;
    if (type == AI_RANGE_Type_Disabled) {
        return 0;
    }
    else if (type == AI_RANGE_Type_Vlot)
    {
        range = AiCfg.rangeConfig.config.rangeVolt;
        switch (range) {
        case AI_Range_Volt_60mV:
        case AI_Range_Volt_200mV:
        case AI_Range_Volt_50V:
            return 2;
            break;
        case AI_Range_Volt_20mV:
        case AI_Range_Volt_6V:
        case AI_Range_Volt_20V:
            return 3;
            break;
        case AI_Range_Volt_1V:
        case AI_Range_Volt_2V:
            return 4;
            break;
        default:
            break;
        }
    }
    else if (type == AI_RANGE_Type_GS)
    {
        return 4;
    }
    else if (type == AI_RANGE_Type_TC)
    {
        return 1;
    }
    else if (type == AI_RANGE_Type_RTD)
    {
        range = AiCfg.rangeConfig.config.rangeRTD;
        switch (range) {
        case AI_Range_RTD_Pt100:
        case AI_Range_RTD_JPt100:
        case AI_Range_RTD_Cu10GE:
        case AI_Range_RTD_Cu10LN:
        case AI_Range_RTD_Cu10WEED:
        case AI_Range_RTD_Cu10BAILEY:
        case AI_Range_RTD_Cu10a392:
        case AI_Range_RTD_Cu10a393:
        case AI_Range_RTD_Cu25:
        case AI_Range_RTD_Cu53:
        case AI_Range_RTD_Cu100:
        case AI_Range_RTD_J263B:
        case AI_Range_RTD_Ni100SAMA:
        case AI_Range_RTD_Ni100DN:
        case AI_Range_RTD_Ni120:
        case AI_Range_RTD_Pt25:
        case AI_Range_RTD_Pt50:
        case AI_Range_RTD_Pt200WEED:
        case AI_Range_RTD_Cu10G:
        case AI_Range_RTD_Cu50G:
        case AI_Range_RTD_Cu100G:
        case AI_Range_RTD_Pt46G:
        case AI_Range_RTD_Pt100G:
            return 1;
            break;
        case AI_Range_RTD_Pt100_H:
        case AI_Range_RTD_JPt100_H:
            return 2;
            break;
        default:
            break;
        }
    }
    else if (type == AI_RANGE_Type_DI)
    {
        return 0;
    }
    else if (type == AI_RANGE_Type_Cur0_20mA || type == AI_RANGE_Type_GS4_20mA)
    {
        return 2;
    }

    return 0;
}

/**
 * @brief 把float类型的数据乘以10的精度次方后转换为int类型返回，该函数避免了用float类型直接乘法后数值有差异
 * @param data  : float类型的数据
 * @param pos   : 精度
 * @return      : float类型的数据乘以10的精度次方后转换为int类型值
 */
int Cfgconfig_AI::getFloatToInt(float data, int pos)
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
