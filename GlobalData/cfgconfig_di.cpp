#include "cfgconfig_di.h"
#include <QVariant>
#include "./GlobalData/cfgchannel.h"

CfgConfig_DI *CfgConfig_DI::instance()
{
    static CfgConfig_DI data;
    return &data;
}


CfgConfig_DI::CfgConfig_DI(QObject *parent) :
    QObject(parent)
{
}


/*
 * 通过ID和 group得到DI的初始配置值并返回
*/
QVariant CfgConfig_DI::getDIData(quint32 ID, quint32 group)
{
    if (ID >= DI_Range_Start_ID && ID <= DI_Range_End_ID)
        return getDIRangeData(ID, group);
    else if (ID >= DI_Alarm_Start_ID && ID <= DI_Alarm_End_ID)
        return getDIAlarmData(ID, group);
    else if (ID >= DI_Disp_Start_ID && ID <= DI_Disp_End_ID)
        return getDIDispData(ID, group);
    return QVariant();
}


QString CfgConfig_DI::getDIObjStr(quint32 ID, quint32 group)
{
    if (ID >= DI_Start_ID && ID <= DI_Range_End_ID)
        return getDIRangeObjStr(ID, group);
    else if (ID >= DI_Alarm_Start_ID && ID <= DI_Alarm_End_ID)
        return getDIAlarmObjStr(ID, group);
    else if (ID >= DI_Disp_Start_ID && ID <= DI_Disp_End_ID)
        return getDIDispObjStr(ID, group);
    return NULL;
}


QString CfgConfig_DI::getDIValueObjStr(quint32 ID, quint32 group, ConfigObject *cfg)
{
    if (ID >= DI_Range_Start_ID && ID <= DI_Range_End_ID)
        return getDIRangeValueObjStr(ID, group, cfg);
    else if (ID >= DI_Alarm_Start_ID && ID <= DI_Alarm_End_ID)
        return getDIAlarmValueObjStr(ID, group, cfg);
    else if (ID >= DI_Disp_Start_ID && ID <= DI_Disp_End_ID)
        return getDIDispValueObjStr(ID, group, cfg);
    return NULL;
}

void CfgConfig_DI::setDIData(quint32 ID, quint32 group)
{
    if (ID >= DI_Range_Start_ID && ID <= DI_Range_End_ID)
        setDIRangeData(ID, group);
    else if (ID >= DI_Alarm_Start_ID && ID <= DI_Alarm_End_ID)
        setDIAlarmData(ID, group);
    else if (ID >= DI_Disp_Start_ID && ID <= DI_Disp_End_ID)
        setDIDispData(ID, group);
}





/*
 * 通过ID和 group得到DI-Range的初始配置值并返回
*/
QVariant CfgConfig_DI::getDIRangeData(quint32 ID, quint32 group)
{
    DI_CONFIG DiCfg = *CfgChannel::instance()->getDIConfig(group);
    switch (ID) {
    case ID_DI_Range_Range_Type:    //量程类型
        return DiCfg.rangeConfig.type;
        break;

    case ID_DI_Range_Range_Min:     //范围下限
        return DiCfg.rangeConfig.rangeMin;
        break;
    case ID_DI_Range_Range_Max:     //范围上限
        return DiCfg.rangeConfig.rangeMax;
        break;
    case ID_DI_Range_Range_DispMin:
        return DiCfg.rangeConfig.rangeDispMin;
        break;
    case ID_DI_Range_Range_DispMax:
        return DiCfg.rangeConfig.rangeDispMax;
        break;

    case ID_DI_Range_Range_Calculate:       //通道运算类型
        return DiCfg.rangeConfig.calculateType;
        break;
    case ID_DI_Range_Range_BasicChannel:         //基础通道索引值，Delta运算方式时使用
    {
        QList<quint32> list = CfgChannel::instance()->getEffectiveChannel();
        if (list.contains(DiCfg.rangeConfig.basicChannel))
            return DiCfg.rangeConfig.basicChannel;
        else
            return list.first();
    }
        break;
    //scale
    case ID_DI_Range_Scale_Pos:       //小数点位置, 0/1/2/3/4/5，默认2
        return DiCfg.rangeConfig.scale.scalePosition;
        break;
    case ID_DI_Range_Scale_Min:        //标尺下限，-999999 ~ 999999，默认0, 即0.00
        return DiCfg.rangeConfig.scale.scaleMin;
        break;
    case ID_DI_Range_Scale_Max:       //标尺上限 , -999999 ~ 999999, 默认10000,即100.00
        return DiCfg.rangeConfig.scale.scaleMax;
        break;
    case ID_DI_Range_Scale_Unit:         //单位，字符串，最多6个半角字符
        return getCharToString(DiCfg.rangeConfig.scale.scaleUnit);
        break;
    default:
        break;
    }
    return QVariant();
}



/*
 * 通过ID和 group得到DI-Alarm的初始配置值并返回
*/
QVariant CfgConfig_DI::getDIAlarmData(quint32 ID, quint32 group)
{
    DI_CONFIG DiCfg = *CfgChannel::instance()->getDIConfig(group);
    switch (ID) {
    case ID_DI_Alarm_Level1_OnOff:
        return DiCfg.alarmConfig.alarmOnOff[0];
        break;
    case ID_DI_Alarm_Level1_Value:
        return DiCfg.alarmConfig.alarmValue[0];
        break;
    case ID_DI_Alarm_Level1_Hysteresis:
        return DiCfg.alarmConfig.alarmHysteresis[0];
        break;
    case ID_DI_Alarm_Level1_Logging:
        return DiCfg.alarmConfig.alarmLogging[0];
        break;
    case ID_DI_Alarm_Level1_OutputType:
        return DiCfg.alarmConfig.alarmOutputType[0];
        break;
    case ID_DI_Alarm_Level1_OutputNo:
        return DiCfg.alarmConfig.alarmOuputNo[0];
        break;

    case ID_DI_Alarm_Level2_OnOff:
        return DiCfg.alarmConfig.alarmOnOff[1];
        break;
    case ID_DI_Alarm_Level2_Value:
        return DiCfg.alarmConfig.alarmValue[1];
        break;
    case ID_DI_Alarm_Level2_Hysteresis:
        return DiCfg.alarmConfig.alarmHysteresis[1];
        break;
    case ID_DI_Alarm_Level2_Logging:
        return DiCfg.alarmConfig.alarmLogging[1];
        break;
    case ID_DI_Alarm_Level2_OutputType:
        return DiCfg.alarmConfig.alarmOutputType[1];
        break;
    case ID_DI_Alarm_Level2_OutputNo:
        return DiCfg.alarmConfig.alarmOuputNo[1];
        break;

    case ID_DI_Alarm_Level3_OnOff:
        return DiCfg.alarmConfig.alarmOnOff[2];
        break;
    case ID_DI_Alarm_Level3_Type:
        return DiCfg.alarmConfig.alarmType[2];
        break;
    case ID_DI_Alarm_Level3_Value:
        return DiCfg.alarmConfig.alarmValue[2];
        break;
    case ID_DI_Alarm_Level3_Hysteresis:
        return DiCfg.alarmConfig.alarmHysteresis[2];
        break;
    case ID_DI_Alarm_Level3_Logging:
        return DiCfg.alarmConfig.alarmLogging[2];
        break;
    case ID_DI_Alarm_Level3_OutputType:
        return DiCfg.alarmConfig.alarmOutputType[2];
        break;
    case ID_DI_Alarm_Level3_OutputNo:
        return DiCfg.alarmConfig.alarmOuputNo[2];
        break;

    case ID_DI_Alarm_Level4_OnOff:
        return DiCfg.alarmConfig.alarmOnOff[3];
        break;
    case ID_DI_Alarm_Level4_Type:
        return DiCfg.alarmConfig.alarmType[3];
        break;
    case ID_DI_Alarm_Level4_Value:
        return DiCfg.alarmConfig.alarmValue[3];
        break;
    case ID_DI_Alarm_Level4_Hysteresis:
        return DiCfg.alarmConfig.alarmHysteresis[3];
        break;
    case ID_DI_Alarm_Level4_Logging:
        return DiCfg.alarmConfig.alarmLogging[3];
        break;
    case ID_DI_Alarm_Level4_OutputType:
        return DiCfg.alarmConfig.alarmOutputType[3];
        break;
    case ID_DI_Alarm_Level4_OutputNo:
        return DiCfg.alarmConfig.alarmOuputNo[3];
        break;


    case ID_DI_Alarm_Leve_Delay_s:
        return DiCfg.alarmConfig.alarmDelay[0];
        break;
    case ID_DI_Alarm_Leve_Delay_m:
        return DiCfg.alarmConfig.alarmDelay[1];
        break;
    case ID_DI_Alarm_Leve_Delay_h:
        return DiCfg.alarmConfig.alarmDelay[2];
        break;
    default:
        break;
    }
    return QVariant();
}


/*
 * 通过ID和 group得到DI-Display的初始配置值并返回
*/
QVariant CfgConfig_DI::getDIDispData(quint32 ID, quint32 group)
{
    DI_CONFIG DiCfg = *CfgChannel::instance()->getDIConfig(group);
    switch (ID) {
    case ID_DI_Disp_Color_Value:        //通道颜色，Byte2-Red, Byte1-Green, Byte0-Blue
        return DiCfg.displayConfig.displayColor;
        break;
    case ID_DI_Disp_Tag_Char:       //标记字符串，中英支持，最多32个半角字符
        return getCharToString(DiCfg.displayConfig.displayTagChar);
        break;
    case ID_DI_Disp_Tag_No:         //标记号码，符号，最多16个半角字符
        return getCharToString(DiCfg.displayConfig.displayTagNo);
        break;
    case ID_DI_Disp_Zone_Upper:     //显示区域上限，5-100,单位%
        return DiCfg.displayConfig.displayZoneUpper;
        break;
    case ID_DI_Disp_Zone_Lower:
        return DiCfg.displayConfig.dispalyZoneLower;
        break;
    case ID_DI_Disp_Scale_Pos:           //标尺位置，1-10,默认1
        return DiCfg.displayConfig.displayScalePos;
        break;
    case ID_DI_Disp_Scale_Div:           //标尺分割，4-13, 13:C10
        return DiCfg.displayConfig.displayScaleDiv;
        break;
    case ID_DI_DISP_Bar_Pos:        //棒图基准位置， 0-Lower, 1-Center, 2-Upper
        return DiCfg.displayConfig.displayBarPos;
        break;
    case ID_DI_DISP_Bar_Div:        //棒图分割数，4-12, 默认10
        return DiCfg.displayConfig.displayBarDiv;
        break;

    case ID_DI_DISP_AlarmMark_OnOff:         //报警标记开关，0-0ff, 1-on
        return DiCfg.displayConfig.displayAlarmMarkOnOff;
        break;
    case ID_DI_DISP_AlarmMark_Type:      //报警标记类型，0-报警， 1-固定
        return DiCfg.displayConfig.displayAlarmMarkType;
        break;
    case ID_DI_DISP_AlarmMark_Color1:       //报警1-4颜色， Byte2-Red, Byte1-Green, Byte0-Blue
        return DiCfg.displayConfig.displayAlarmMarkColor[0];
        break;
    case ID_DI_DISP_AlarmMark_Color2:
        return DiCfg.displayConfig.displayAlarmMarkColor[1];
        break;
    case ID_DI_DISP_AlarmMark_Color3:
        return DiCfg.displayConfig.displayAlarmMarkColor[2];
        break;
    case ID_DI_DISP_AlarmMark_Color4:
        return DiCfg.displayConfig.displayAlarmMarkColor[3];
        break;
    case ID_DI_DISP_DI0_Str:
        return getCharToString(DiCfg.displayConfig.displayDI0);
        break;
    case ID_DI_DISP_DI1_Str:
        return getCharToString(DiCfg.displayConfig.displayDI1);
        break;

    default:
        break;
    }
    return QVariant();
}




/*
 * 通过ID和 group得到DI-Range界面显示的左值
*/
QString CfgConfig_DI::getDIRangeObjStr(quint32 ID, quint32 /*group*/)
{
    switch (ID) {
    case ID_DI:
        return tr("DI Channel settings");
        break;
    case ID_DI_Last_CH:
        return tr("Last-CH");
        break;
    case ID_DI_First_CH:
        return tr("First-CH");
        break;
    case ID_DI_Range:
    case ID_DI_Range_Range:
        return tr("Range");
        break;
    case ID_DI_Range_Range_Type:
        return tr("Type");
        break;
    case ID_DI_Range_Range_Min:
        return tr("Range Lower");
        break;
    case ID_DI_Range_Range_Max:
        return tr("Range Upper");
        break;
    case ID_DI_Range_Range_DispMin:
        return tr("Span Lower");
        break;
    case ID_DI_Range_Range_DispMax:
        return tr("Span Upper");
        break;
    case ID_DI_Range_Range_Calculate:
        return tr("Calculation");
        break;
    case ID_DI_Range_Range_BasicChannel:
        return tr("Reference channel");
        break;

    case ID_DI_Range_Scale:
        return tr("Scale");
        break;
    case ID_DI_Range_Scale_Pos:
        return tr("Decimal place");
        break;
    case ID_DI_Range_Scale_Min:
        return tr("Scale Lower");
        break;
    case ID_DI_Range_Scale_Max:
        return tr("Scale Upper");
        break;
    case ID_DI_Range_Scale_Unit:
        return tr("Unit");
        break;
    default:
        break;
    }
    return NULL;
}

/*
 * 通过ID和 group得到DI-Alarm界面显示的左值
*/
QString CfgConfig_DI::getDIAlarmObjStr(quint32 ID, quint32 /*group*/)
{
    switch (ID) {
    case ID_DI_Alarm:
        return tr("Alarm");
        break;

    case ID_DI_Alarm_Level1:
        return QString(tr("Level") + " 1");
        break;
    case ID_DI_Alarm_Level2:
        return QString(tr("Level") + " 2");
        break;
    case ID_DI_Alarm_Level3:
        return QString(tr("Level") + " 3");
        break;
    case ID_DI_Alarm_Level4:
        return QString(tr("Level") + " 4");
        break;

        break;
    case ID_DI_Alarm_Level1_OnOff:
    case ID_DI_Alarm_Level2_OnOff:
    case ID_DI_Alarm_Level3_OnOff:
    case ID_DI_Alarm_Level4_OnOff:
        return tr("On/Off");
        break;
    case ID_DI_Alarm_Level1_Type:
    case ID_DI_Alarm_Level2_Type:
    case ID_DI_Alarm_Level3_Type:
    case ID_DI_Alarm_Level4_Type:
        return tr("Type");
        break;
    case ID_DI_Alarm_Level1_Value:
    case ID_DI_Alarm_Level2_Value:
    case ID_DI_Alarm_Level3_Value:
    case ID_DI_Alarm_Level4_Value:
        return tr("Value");
        break;
    case ID_DI_Alarm_Level1_Hysteresis:
    case ID_DI_Alarm_Level2_Hysteresis:
    case ID_DI_Alarm_Level3_Hysteresis:
    case ID_DI_Alarm_Level4_Hysteresis:
        return tr("Hysteresis");
        break;
    case ID_DI_Alarm_Level1_Logging:
    case ID_DI_Alarm_Level2_Logging:
    case ID_DI_Alarm_Level3_Logging:
    case ID_DI_Alarm_Level4_Logging:
        return tr("Logging");
        break;
    case ID_DI_Alarm_Level1_OutputType:
    case ID_DI_Alarm_Level2_OutputType:
    case ID_DI_Alarm_Level3_OutputType:
    case ID_DI_Alarm_Level4_OutputType:
        return tr("Output type");
        break;
    case ID_DI_Alarm_Level1_OutputNo:
    case ID_DI_Alarm_Level2_OutputNo:
    case ID_DI_Alarm_Level3_OutputNo:
    case ID_DI_Alarm_Level4_OutputNo:
        return tr("Output No.");
        break;
    case ID_DI_Alarm_Leve_Delay:                  //alarm-delay
        return tr("Alarm delay");
        break;
    case ID_DI_Alarm_Leve_Delay_s:
        return tr("Second");
    case ID_DI_Alarm_Leve_Delay_m:
        return tr("Minute");
    case ID_DI_Alarm_Leve_Delay_h:
        return tr("Hour");
        break;
    default:
        break;
    }
    return NULL;
}

/*
 * 通过ID和 group得到DI-Disp界面显示的左值
*/
QString CfgConfig_DI::getDIDispObjStr(quint32 ID, quint32 /*group*/)
{
    switch (ID) {
    case ID_DI_Disp:
        return tr("Display settings");
        break;

    case ID_DI_Disp_Color:
    case ID_DI_Disp_Color_Value:
        return tr("Color");
        break;

    case ID_DI_Disp_Tag:
        return tr("Tag");
        break;
    case ID_DI_Disp_Tag_Char:
        return tr("Characters");
        break;
    case ID_DI_Disp_Tag_No:
        return tr("No.");
        break;

    case ID_DI_Disp_Zone:
        return tr("Zone");
        break;
    case ID_DI_Disp_Zone_Upper:
        return tr("Upper");
        break;
    case ID_DI_Disp_Zone_Lower:
        return tr("Lower");
        break;

    case ID_DI_Disp_Scale:
        return tr("Scale");
        break;
    case ID_DI_Disp_Scale_Pos:
        return tr("Position");
        break;

    case ID_DI_DISP_Bar:
        return tr("Bar graph");
        break;
    case ID_DI_DISP_Bar_Pos:
        return tr("Base position");
        break;
    case ID_DI_DISP_Bar_Div:
    case ID_DI_Disp_Scale_Div:
        return tr("Division");
        break;

    case ID_DI_DISP_AlarmMark:              //报警标记分组
        return tr("Alarm point mark");
        break;
    case ID_DI_DISP_AlarmMark_OnOff:        //报警标记开关，0-0ff, 1-on
        return tr("Indicate on Scale");
        break;
    case ID_DI_DISP_AlarmMark_Type:     //报警标记类型
        return tr("Mark type");
        break;
    case ID_DI_DISP_AlarmMark_Color1:       //报警颜色， Byte2-Red, Byte1-Green, Byte0-Blue
        return tr("Alarm 1 color");
        break;
    case ID_DI_DISP_AlarmMark_Color2:
        return tr("Alarm 2 color");
        break;
    case ID_DI_DISP_AlarmMark_Color3:
        return tr("Alarm 3 color");
        break;
    case ID_DI_DISP_AlarmMark_Color4:
        return tr("Alarm 4 color");
        break;

    case ID_DI_DISP_DI:    //【DI】类型量程且运算off时有效，测量值为0/1时显示的字符串
        return tr("Display characters of each value");
        break;
    case ID_DI_DISP_DI0_Str:        //最多8个半角字符，中英文
        return tr("0");
        break;
    case ID_DI_DISP_DI1_Str:
        return tr("1");
        break;

    case ID_DI_OpMode:       //DI模块操作模式定义 0.标准   1.远程控制输入
        return tr("Operation mode");
        break;
    case ID_DI_OpMode_Value:
        return tr("Operation mode");
        break;
    default:
        break;
    }
    return NULL;
}





QString CfgConfig_DI::getDIRangeValueObjStr(quint32 ID, quint32 /*group*/, ConfigObject *cfg)
{
    QString cfgData = cfg->getData().toString();
    switch (ID) {
    case ID_DI_Range_Range_Type:
        return getDIRangeTypeValueObjStr(cfgData.toInt());
        break;
    case ID_DI_Range_Range_Min:
    case ID_DI_Range_Range_Max:
    case ID_DI_Range_Range_DispMin:
    case ID_DI_Range_Range_DispMax:
        return cfgData;
        break;

    case ID_DI_Range_Range_Calculate:
    {
        int type = cfgData.toInt();
        if (type == 0)
            return tr("Off");
        else if (type == 1)
            return tr("Delta");
        else if (type == 2)
            return tr("Linear scaling");
    }
        break;
    case ID_DI_Range_Range_BasicChannel:
        return getDIRangeRangeBasicChannelValueObjStr(cfg->getData().toUInt());
        break;

    case ID_DI_Range_Scale_Min:
    case ID_DI_Range_Scale_Max:
        return getDIRangScaleUnit(cfg->getData().toInt());
        break;
        break;

    case ID_DI_Range_Scale_Unit:
        return getDIRangeScaleUnitValObjStr(cfgData);
        break;
    default:
        break;
    }
    return cfgData;
}


QString CfgConfig_DI::getDIAlarmValueObjStr(quint32 ID, quint32 /*group*/, ConfigObject *cfg)
{
    QString cfgData = cfg->getData().toString();
    switch (ID) {
    case ID_DI_Alarm_Level1_OnOff:      //alarm-level1
    case ID_DI_Alarm_Level2_OnOff:
    case ID_DI_Alarm_Level3_OnOff:
    case ID_DI_Alarm_Level4_OnOff:
    case ID_DI_Alarm_Level1_Logging:
    case ID_DI_Alarm_Level2_Logging:
    case ID_DI_Alarm_Level3_Logging:
    case ID_DI_Alarm_Level4_Logging:
        return cfgData.toInt() == 0 ? tr("Off") : tr("On");
        break;
    case ID_DI_Alarm_Level1_Type:
    case ID_DI_Alarm_Level2_Type:
    case ID_DI_Alarm_Level3_Type:
    case ID_DI_Alarm_Level4_Type:
        return getDIAlarmTypeValObjStr(cfgData.toInt());
        break;

    case ID_DI_Alarm_Level1_Value:
    case ID_DI_Alarm_Level2_Value:
    case ID_DI_Alarm_Level3_Value:
    case ID_DI_Alarm_Level4_Value:
        return getDIRangScaleUnit(cfg->getData().toInt());
        break;

    case ID_DI_Alarm_Level1_OutputType:
    case ID_DI_Alarm_Level2_OutputType:
    case ID_DI_Alarm_Level3_OutputType:
    case ID_DI_Alarm_Level4_OutputType:
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
    case ID_DI_Alarm_Level1_OutputNo:
    case ID_DI_Alarm_Level2_OutputNo:
    case ID_DI_Alarm_Level3_OutputNo:
    case ID_DI_Alarm_Level4_OutputNo:
        return getDIAlarmChannelObjValStr(cfg->getData().toUInt());
        break;
    default:
        break;
    }

    return cfgData;
}


/*
 * 通过ID,group和cfg得到DI-Disp界面显示的左值
*/
QString CfgConfig_DI::getDIDispValueObjStr(quint32 ID, quint32 /*group*/, ConfigObject *cfg)
{
    QString cfgData = cfg->getData().toString();
    switch (ID) {
    case ID_DI_Disp_Color_Value:        //通道颜色，Byte2-Red, Byte1-Green, Byte0-Blue
//    case ID_DI_DISP_ColorScaleBand_Color:       //彩色标尺带颜色，Byte2-Red, Byte1-Green, Byte0-Blue
    case ID_DI_DISP_AlarmMark_Color1:       //报警1-4颜色， Byte2-Red, Byte1-Green, Byte0-Blue
    case ID_DI_DISP_AlarmMark_Color2:
    case ID_DI_DISP_AlarmMark_Color3:
    case ID_DI_DISP_AlarmMark_Color4:
        return getDIDisplayColorValueObjStr(cfg->getData().toUInt());
        break;
    case ID_DI_Disp_Tag_Char:       //标记字符串，中英支持，最多32个半角字符
    case ID_DI_Disp_Tag_No:         //标记号码，符号，最多16个半角字符
        return QString(tr("%1").arg(cfgData));
        break;

    case ID_DI_Disp_Zone_Upper:       //显示区域上限，5-100,单位%
    case ID_DI_Disp_Zone_Lower:       //显示区域上限，0-95,单位%
//    case ID_DI_DISP_Partial_Pos:       //部分压缩放大位置，1-99, 单位：%，默认50
        return QString("%1 ").arg(cfgData) + QString("%");
        break;
    case ID_DI_Disp_Scale_Pos:
    {
        int type = cfgData.toInt();
        if (type == 0) return tr("Off");
        return cfgData;
    }
        break;

    case ID_DI_DISP_Bar_Pos:      //棒图基准位置， 0-Lower, 1-Center, 2-Upper
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

    case ID_DI_DISP_AlarmMark_Type:       //报警标记类型，0-报警， 1-固定
        return cfgData.toInt() == 0 ? tr("Alarm") : tr("Fixed");
        break;
    case ID_DI_DISP_AlarmMark_OnOff:
        return cfgData.toInt() == 0 ? tr("Off") : tr("On");
        break;
    default:
        break;
    }
    return cfgData;
}


QString CfgConfig_DI::getDIRangScaleUnit(int data)
{
    quint32 decCnt;
    QString unit = ConfigTreeObject::getConfigData(DI_DELEGATE_RANG_SCALE_UNIT).toString();
    quint32 caltype = ConfigTreeObject::getConfigData(DI_DELEGATE_RANG_RANGE_CALCU).toInt();
//    decCnt = ConfigTreeObject::getConfigData(DI_DELEGATE_RANG_SCALE_DEC).toInt();

    if (caltype == DI_Range_Calculate_Linear)      //线性转换以获取测量值
    {
        decCnt= ConfigTreeObject::getConfigData(DI_DELEGATE_RANG_SCALE_DEC).toInt();
        float dataF = (float)(data/ ::pow10(decCnt));
        return QString::number(dataF, 'f', decCnt) + " " + unit;
    }
    else
    {
        return QString::number(data);
//        ConfigObject *cfg = ConfigTreeObject::getConfigObject(DI_DELEGATE_RANG_RANGE_LOWER);
//        QString showOn = getDIRangeValueObjStr(ID_DI_Range_Range_DispMin, 0, cfg);

//        QStringList strList = showOn.split(" ");
//        QStringList pointList = strList[0].split(".");
//        if(pointList.size() >= 2) {
//            decCnt = pointList.at(1).size();
//        }
//        else
//            decCnt = 0;
//        float dataF = (float)(data/ ::pow10(decCnt));
//        return QString::number(dataF, 'f', decCnt);
    }
}

QString CfgConfig_DI::getDIRangeScaleUnitValObjStr(QString unit)
{
    QStringList str;
    str << DI_DELEGATE_RANG_SCALE_LOWER << DI_DELEGATE_RANG_SCALE_UPPER
        << DI_DELEGATE_ALARM_LVL1_VALUE << DI_DELEGATE_ALARM_LVL1_HYST
        << DI_DELEGATE_ALARM_LVL2_VALUE << DI_DELEGATE_ALARM_LVL2_HYST
        << DI_DELEGATE_ALARM_LVL3_VALUE << DI_DELEGATE_ALARM_LVL3_HYST
        << DI_DELEGATE_ALARM_LVL4_VALUE << DI_DELEGATE_ALARM_LVL4_HYST;

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
QString CfgConfig_DI::getDIAlarmTypeValObjStr(quint32 type)
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






void CfgConfig_DI::setDIRangeData(quint32 ID, quint32 group)
{
    int index = CfgChannel::instance()->getExistChannelDI().values().indexOf(group);
    QString chanNum = CfgChannel::instance()->getExistChannelDI().keys().at(index);
    DI_CONFIG *DiCfg = CfgChannel::instance()->getDIConfig(group);
    switch (ID) {
    case ID_DI_Range_Range_Type:
        DiCfg->rangeConfig.type = DI_Range_Type(ConfigTreeObject::getConfigData\
                              (QString(DICONFIG_RANG_RANGE_TYPE).arg(chanNum)).toInt());
        break;
    case ID_DI_Range_Range_Min:
        DiCfg->rangeConfig.rangeMin = ConfigTreeObject::getConfigData(QString(DICONFIG_RANG_RANGE_MIN).arg(chanNum)).toFloat();
        break;
    case ID_DI_Range_Range_Max:
        DiCfg->rangeConfig.rangeMax = ConfigTreeObject::getConfigData(QString(DICONFIG_RANG_RANGE_MAX).arg(chanNum)).toFloat();
        break;
    case ID_DI_Range_Range_DispMin:
        DiCfg->rangeConfig.rangeDispMin = ConfigTreeObject::getConfigData(QString(DICONFIG_RANG_RANGE_LOWER).arg(chanNum)).toFloat();
        break;
    case ID_DI_Range_Range_DispMax:
        DiCfg->rangeConfig.rangeDispMax = ConfigTreeObject::getConfigData(QString(DICONFIG_RANG_RANGE_UPPER).arg(chanNum)).toFloat();
        break;

    case ID_DI_Range_Range_Calculate:
        DiCfg->rangeConfig.calculateType = DI_Range_Calculate(ConfigTreeObject::getConfigData(QString(DICONFIG_RANG_RANGE_CALCU).arg(chanNum)).toInt());
        if (DiCfg->rangeConfig.calculateType != DI_Range_Calculate_Delta)
            break;
    case ID_DI_Range_Range_BasicChannel:
        DiCfg->rangeConfig.basicChannel = ConfigTreeObject::getConfigData(QString(DICONFIG_RANG_RANGE_REFER).arg(chanNum)).toUInt();
        break;
    case ID_DI_Range_Scale_Pos:      //小数点位置, 0/1/2/3/4/5
        DiCfg->rangeConfig.scale.scalePosition = ConfigTreeObject::getConfigData(QString(DICONFIG_RANG_SCALE_DEC).arg(chanNum)).toInt();
        break;
    case ID_DI_Range_Scale_Min:
        DiCfg->rangeConfig.scale.scaleMin = ConfigTreeObject::getConfigData(QString(DICONFIG_RANG_SCALE_LOWER).arg(chanNum)).toInt();
        break;
    case ID_DI_Range_Scale_Max:
        DiCfg->rangeConfig.scale.scaleMax = ConfigTreeObject::getConfigData(QString(DICONFIG_RANG_SCALE_UPPER).arg(chanNum)).toInt();
        break;
    case ID_DI_Range_Scale_Unit:
        setStrToChar(DiCfg->rangeConfig.scale.scaleUnit, ConfigTreeObject::getConfigData(QString(DICONFIG_RANG_SCALE_UNIT).arg(chanNum)).toString(), 8);
        break;
    default:
        break;
    }
}

void CfgConfig_DI::setDIAlarmData(quint32 ID, quint32 group)
{
    int index = CfgChannel::instance()->getExistChannelDI().values().indexOf(group);
    QString chanNum = CfgChannel::instance()->getExistChannelDI().keys().at(index);
    DI_CONFIG *DiCfg = CfgChannel::instance()->getDIConfig(group);
    switch (ID) {
    case ID_DI_Alarm_Level1_OnOff:
        DiCfg->alarmConfig.alarmOnOff[0] = ConfigTreeObject::getConfigData(QString(DICONFIG_ALARM_LVL1_ONOFF).arg(chanNum)).toUInt();
        break;
    case ID_DI_Alarm_Level1_Type:
        DiCfg->alarmConfig.alarmType[0] = (Alarm_Type)ConfigTreeObject::getConfigData(QString(DICONFIG_ALARM_LVL1_TYPE).arg(chanNum)).toUInt();
        break;
    case ID_DI_Alarm_Level1_Value:
        DiCfg->alarmConfig.alarmValue[0] = ConfigTreeObject::getConfigData(QString(DICONFIG_ALARM_LVL1_VALUE).arg(chanNum)).toInt();
        break;
    case ID_DI_Alarm_Level1_Hysteresis:
        DiCfg->alarmConfig.alarmHysteresis[0] = ConfigTreeObject::getConfigData(QString(DICONFIG_ALARM_LVL1_HYST).arg(chanNum)).toInt();
        break;
    case ID_DI_Alarm_Level1_Logging:
        DiCfg->alarmConfig.alarmLogging[0] = ConfigTreeObject::getConfigData(QString(DICONFIG_ALARM_LVL1_LOGG).arg(chanNum)).toUInt();
        break;
    case ID_DI_Alarm_Level1_OutputType:
        DiCfg->alarmConfig.alarmOutputType[0] = ConfigTreeObject::getConfigData(QString(DICONFIG_ALARM_LVL1_OUTTYPE).arg(chanNum)).toUInt();
    case ID_DI_Alarm_Level1_OutputNo:
        DiCfg->alarmConfig.alarmOuputNo[0] = ConfigTreeObject::getConfigData(QString(DICONFIG_ALARM_LVL1_OUTNO).arg(chanNum)).toUInt();
        break;

    case ID_DI_Alarm_Level2_OnOff:
        DiCfg->alarmConfig.alarmOnOff[1] = ConfigTreeObject::getConfigData(QString(DICONFIG_ALARM_LVL2_ONOFF).arg(chanNum)).toUInt();
        break;
    case ID_DI_Alarm_Level2_Type:
        DiCfg->alarmConfig.alarmType[1] = (Alarm_Type)ConfigTreeObject::getConfigData(QString(DICONFIG_ALARM_LVL2_TYPE).arg(chanNum)).toUInt();
        break;
    case ID_DI_Alarm_Level2_Value:
        DiCfg->alarmConfig.alarmValue[1] = ConfigTreeObject::getConfigData(QString(DICONFIG_ALARM_LVL2_VALUE).arg(chanNum)).toInt();
        break;
    case ID_DI_Alarm_Level2_Hysteresis:
        DiCfg->alarmConfig.alarmHysteresis[1] = ConfigTreeObject::getConfigData(QString(DICONFIG_ALARM_LVL2_HYST).arg(chanNum)).toInt();
        break;
    case ID_DI_Alarm_Level2_Logging:
        DiCfg->alarmConfig.alarmLogging[1] = ConfigTreeObject::getConfigData(QString(DICONFIG_ALARM_LVL2_LOGG).arg(chanNum)).toUInt();
        break;
    case ID_DI_Alarm_Level2_OutputType:
        DiCfg->alarmConfig.alarmOutputType[1] = ConfigTreeObject::getConfigData(QString(DICONFIG_ALARM_LVL2_OUTTYPE).arg(chanNum)).toUInt();
    case ID_DI_Alarm_Level2_OutputNo:
        DiCfg->alarmConfig.alarmOuputNo[1] = ConfigTreeObject::getConfigData(QString(DICONFIG_ALARM_LVL2_OUTNO).arg(chanNum)).toUInt();
        break;

    case ID_DI_Alarm_Level3_OnOff:
        DiCfg->alarmConfig.alarmOnOff[2] = ConfigTreeObject::getConfigData(QString(DICONFIG_ALARM_LVL3_ONOFF).arg(chanNum)).toUInt();
        break;
    case ID_DI_Alarm_Level3_Type:
        DiCfg->alarmConfig.alarmType[2] = (Alarm_Type)ConfigTreeObject::getConfigData(QString(DICONFIG_ALARM_LVL3_TYPE).arg(chanNum)).toUInt();
        break;
    case ID_DI_Alarm_Level3_Value:
        DiCfg->alarmConfig.alarmValue[2] = ConfigTreeObject::getConfigData(QString(DICONFIG_ALARM_LVL3_VALUE).arg(chanNum)).toInt();
        break;
    case ID_DI_Alarm_Level3_Hysteresis:
        DiCfg->alarmConfig.alarmHysteresis[2] = ConfigTreeObject::getConfigData(QString(DICONFIG_ALARM_LVL3_HYST).arg(chanNum)).toInt();
        break;
    case ID_DI_Alarm_Level3_Logging:
        DiCfg->alarmConfig.alarmLogging[2] = ConfigTreeObject::getConfigData(QString(DICONFIG_ALARM_LVL3_LOGG).arg(chanNum)).toUInt();
        break;
    case ID_DI_Alarm_Level3_OutputType:
        DiCfg->alarmConfig.alarmOutputType[2] = ConfigTreeObject::getConfigData(QString(DICONFIG_ALARM_LVL3_OUTTYPE).arg(chanNum)).toUInt();
    case ID_DI_Alarm_Level3_OutputNo:
        DiCfg->alarmConfig.alarmOuputNo[2] = ConfigTreeObject::getConfigData(QString(DICONFIG_ALARM_LVL3_OUTNO).arg(chanNum)).toUInt();
        break;

    case ID_DI_Alarm_Level4_OnOff:
        DiCfg->alarmConfig.alarmOnOff[3] = ConfigTreeObject::getConfigData(QString(DICONFIG_ALARM_LVL4_ONOFF).arg(chanNum)).toUInt();
        break;
    case ID_DI_Alarm_Level4_Type:
        DiCfg->alarmConfig.alarmType[3] = (Alarm_Type)ConfigTreeObject::getConfigData(QString(DICONFIG_ALARM_LVL4_TYPE).arg(chanNum)).toUInt();
        break;
    case ID_DI_Alarm_Level4_Value:
        DiCfg->alarmConfig.alarmValue[3] = ConfigTreeObject::getConfigData(QString(DICONFIG_ALARM_LVL4_VALUE).arg(chanNum)).toInt();
        break;
    case ID_DI_Alarm_Level4_Hysteresis:
        DiCfg->alarmConfig.alarmHysteresis[3] = ConfigTreeObject::getConfigData(QString(DICONFIG_ALARM_LVL4_HYST).arg(chanNum)).toInt();
        break;
    case ID_DI_Alarm_Level4_Logging:
        DiCfg->alarmConfig.alarmLogging[3] = ConfigTreeObject::getConfigData(QString(DICONFIG_ALARM_LVL4_LOGG).arg(chanNum)).toUInt();
        break;
    case ID_DI_Alarm_Level4_OutputType:
        DiCfg->alarmConfig.alarmOutputType[3] = ConfigTreeObject::getConfigData(QString(DICONFIG_ALARM_LVL4_OUTTYPE).arg(chanNum)).toUInt();
    case ID_DI_Alarm_Level4_OutputNo:
        DiCfg->alarmConfig.alarmOuputNo[3] = ConfigTreeObject::getConfigData(QString(DICONFIG_ALARM_LVL4_OUTNO).arg(chanNum)).toUInt();
        break;

    case ID_DI_Alarm_Leve_Delay_s:
        DiCfg->alarmConfig.alarmDelay[0] = ConfigTreeObject::getConfigData(QString(DICONFIG_ALARM_DELAY_SEC).arg(chanNum)).toUInt();
        break;
    case ID_DI_Alarm_Leve_Delay_m:
        DiCfg->alarmConfig.alarmDelay[1] = ConfigTreeObject::getConfigData(QString(DICONFIG_ALARM_DELAY_MIN).arg(chanNum)).toUInt();
        break;
    case ID_DI_Alarm_Leve_Delay_h:
        DiCfg->alarmConfig.alarmDelay[2] = ConfigTreeObject::getConfigData(QString(DICONFIG_ALARM_DELAY_HOUR).arg(chanNum)).toUInt();
        break;
    default:
        break;
    }
}

void CfgConfig_DI::setDIDispData(quint32 ID, quint32 group)
{
    int index = CfgChannel::instance()->getExistChannelDI().values().indexOf(group);
    QString chanNum = CfgChannel::instance()->getExistChannelDI().keys().at(index);
    DI_CONFIG *DiCfg = CfgChannel::instance()->getDIConfig(group);
    switch (ID) {
    case ID_DI_Disp_Color_Value:
        DiCfg->displayConfig.displayColor = ConfigTreeObject::getConfigData(QString(DICONFIG_DISP_COLOR_COLOR).arg(chanNum)).toUInt();
        break;
    case ID_DI_Disp_Tag_Char:
        setStrToChar(DiCfg->displayConfig.displayTagChar, ConfigTreeObject::getConfigData(QString(DICONFIG_DISP_TAG_CHAR).arg(chanNum)).toString(), 32);
        break;
    case ID_DI_Disp_Tag_No:
        setStrToChar(DiCfg->displayConfig.displayTagNo, ConfigTreeObject::getConfigData(QString(DICONFIG_DISP_TAG_NO).arg(chanNum)).toString(), 16);
        break;
    case ID_DI_Disp_Zone_Upper:
        DiCfg->displayConfig.displayZoneUpper = ConfigTreeObject::getConfigData(QString(DICONFIG_DISP_ZONE_UPPER).arg(chanNum)).toUInt();
        break;
    case ID_DI_Disp_Zone_Lower:
        DiCfg->displayConfig.dispalyZoneLower= ConfigTreeObject::getConfigData(QString(DICONFIG_DISP_ZONE_LOWER).arg(chanNum)).toUInt();
        break;
    case ID_DI_Disp_Scale_Pos:
        DiCfg->displayConfig.displayScalePos = ConfigTreeObject::getConfigData(QString(DICONFIG_DISP_SCALE_POS).arg(chanNum)).toUInt();
        break;
    case ID_DI_Disp_Scale_Div:
        DiCfg->displayConfig.displayScaleDiv = ConfigTreeObject::getConfigData(QString(DICONFIG_DISP_SCALE_DIV).arg(chanNum)).toUInt();
        break;
    case ID_DI_DISP_Bar_Pos:
        DiCfg->displayConfig.displayBarPos = ConfigTreeObject::getConfigData(QString(DICONFIG_DISP_BAR_BASEPOS).arg(chanNum)).toUInt();
        break;
    case ID_DI_DISP_Bar_Div:
        DiCfg->displayConfig.displayBarDiv = ConfigTreeObject::getConfigData(QString(DICONFIG_DISP_BAR_DIVISION).arg(chanNum)).toUInt();
        break;

    case ID_DI_DISP_AlarmMark_OnOff:
        DiCfg->displayConfig.displayAlarmMarkOnOff = ConfigTreeObject::getConfigData(QString(DICONFIG_DISP_ALARM_INDIC).arg(chanNum)).toUInt();
        break;
    case ID_DI_DISP_AlarmMark_Type:
        DiCfg->displayConfig.displayAlarmMarkType = ConfigTreeObject::getConfigData(QString(DICONFIG_DISP_ALARM_MARKTYPE).arg(chanNum)).toUInt();
        break;
    case ID_DI_DISP_AlarmMark_Color1:
        DiCfg->displayConfig.displayAlarmMarkColor[0] = ConfigTreeObject::getConfigData(QString(DICONFIG_DISP_ALARM_COLOR1).arg(chanNum)).toUInt();
        break;
    case ID_DI_DISP_AlarmMark_Color2:
        DiCfg->displayConfig.displayAlarmMarkColor[1] = ConfigTreeObject::getConfigData(QString(DICONFIG_DISP_ALARM_COLOR2).arg(chanNum)).toUInt();
        break;
    case ID_DI_DISP_AlarmMark_Color3:
        DiCfg->displayConfig.displayAlarmMarkColor[2] = ConfigTreeObject::getConfigData(QString(DICONFIG_DISP_ALARM_COLOR3).arg(chanNum)).toUInt();
        break;
    case ID_DI_DISP_AlarmMark_Color4:
        DiCfg->displayConfig.displayAlarmMarkColor[3] = ConfigTreeObject::getConfigData(QString(DICONFIG_DISP_ALARM_COLOR4).arg(chanNum)).toUInt();
        break;

    case ID_DI_DISP_DI0_Str:
        setStrToChar(DiCfg->displayConfig.displayDI0,ConfigTreeObject::getConfigData(QString(DICONFIG_DISP_DISPCHAR_0).arg(chanNum)).toString(), 8);
        break;
    case ID_DI_DISP_DI1_Str:
        setStrToChar(DiCfg->displayConfig.displayDI1, ConfigTreeObject::getConfigData(QString(DICONFIG_DISP_DISPCHAR_1).arg(chanNum)).toString(), 8);
        break;
    default:
        break;
    }
}


//通道量程类型值得到类型的描述
QString CfgConfig_DI::getDIRangeTypeValueObjStr(quint32 type)
{
    switch (type) {
    case DI_Range_Type_Disabled:        //不使用，即不测量
        return tr("Skip");
        break;
    case DI_Range_Type_DI:        //将接点或电压输入显示为对应显示范围的0%和100%
        return tr("DI");
        break;
    case DI_Range_Type_Pluse:        //脉冲计数
        return tr("Pluse");
        break;
    default:
        break;
    }
    return NULL;
}





/*
 * 把uchar *类型转化为QString类型
*/
QVariant CfgConfig_DI::getCharToString(uchar *str)
{
//    char * valueChar = NULL;
//    while(*str != '\0')
//        *valueChar++ = *str++;
//    QString data(valueChar);
//    return tr(data.toLatin1());
    QString data((char *)str);
    return data;
}

/*
 * 把从配置中得到的颜色配置值转换为界面label显示的样式字符串返回
*/
QString CfgConfig_DI::getDIDisplayColorValueObjStr(quint32 x)
{
    QColor colorRGBx(COLOR_RED(x), COLOR_GREEN(x), COLOR_BLUE(x));
    QString str = QString("QLabel {background-color: %1}").arg(colorRGBx.name());
    return str;
}

//把QString类型转化为uchar *类型
void CfgConfig_DI::setStrToChar(uchar * uData, QString str, int len)
{
    memset(uData, 0, len);
    QByteArray baUnit = str.toLocal8Bit();
    char* valueUnit = baUnit.data();
//    memcpy(uData, valueUnit, strlen(valueUnit));
    while (*valueUnit != '\0') {
        *uData++ = *valueUnit++;
    }
}

QString CfgConfig_DI::getDIRangeRangeBasicChannelValueObjStr(quint32 data)
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
QString CfgConfig_DI::getDIAlarmChannelObjValStr(quint32 num)
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




