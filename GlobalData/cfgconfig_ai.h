#ifndef CFGCONFIG_AI_H
#define CFGCONFIG_AI_H

#include <QObject>
#include "cfgGlobalDef.h"
#include "configtreeobject.h"


#define AI_Start_ID                             10000
#define AI_End_ID                               14999

#define AI_Range_Start_ID                       10003
#define AI_Range_End_ID                         10999

#define AI_Alarm_Start_ID                       11000
#define AI_Alarm_End_ID                         11999

#define AI_Disp_Start_ID                        12000
#define AI_Disp_End_ID                          12999

#define AI_Correct_Start_ID                     13000
#define AI_Correct_End_ID                       13999

/* *********************************   AI通道宏定义,在10000-14999之间     *******************/
#define ID_AI                                       10000

#define ID_AI_Last_CH                               10001
#define ID_AI_First_CH                              10002

/*AI-Range  在10003-11000之间*/
#define ID_AI_Range                                 10003

#define ID_AI_Range_Range                           10007

#define ID_AI_Range_Range_Type                      10010
//#define ID_AI_Range_Range_Type_Volt                 10011
//#define ID_AI_Range_Range_Type_GS                   10012
//#define ID_AI_Range_Range_Type_TC                   10013
//#define ID_AI_Range_Range_Type_RTD                  10014
//#define ID_AI_Range_Range_Type_DI                   10015
//#define ID_AI_Range_Range_Type_Cur0_20mA            10016
//#define ID_AI_Range_Range_Type_GS4_20mA             10017
//#define ID_AI_Range_Range_Type_Skip                 10018

#define ID_AI_Range_Range_Range                     10020       //config
#define ID_AI_Range_Range_Min                       10021
#define ID_AI_Range_Range_Max                       10022

#define ID_AI_Range_Range_DispMin                   10030
#define ID_AI_Range_Range_DispMax                   10031
#define ID_AI_Range_Range_Calculate                 10032
#define ID_AI_Range_Range_BasicChannel              10033

#define ID_AI_Range_Scale                           10040
#define ID_AI_Range_Scale_Pos                       10041
#define ID_AI_Range_Scale_Min                       10042
#define ID_AI_Range_Scale_Max                       10043
#define ID_AI_Range_Scale_Unit                      10044

#define ID_AI_Range_LowCut                          10050
#define ID_AI_Range_LowCut_OnOff                    10051
#define ID_AI_Range_LowCut_Value                    10052
#define ID_AI_Range_LowCut_Output                   10053

#define ID_AI_Range_MoveAverange                    10060
#define ID_AI_Range_MoveAverange_OnOff              10061
#define ID_AI_Range_MoveAverange_Cnt                10062

#define ID_AI_Range_RJC                             10070
#define ID_AI_Range_RJC_Mode                        10071
#define ID_AI_Range_RJC_Temp                        10072

#define ID_AI_Range_Burnout                         10080
#define ID_AI_Range_Burnout_Mode                    10081

#define ID_AI_Range_Bias                            10090
#define ID_AI_Range_Bias_Value                      10091

//AI-Alarm 在11000-12000之间
#define ID_AI_Alarm                                 11000

#define ID_AI_Alarm_Level1                          11001
#define ID_AI_Alarm_Level1_OnOff                    11002
#define ID_AI_Alarm_Level1_Type                     11003
#define ID_AI_Alarm_Level1_Value                    11004
#define ID_AI_Alarm_Level1_Hysteresis               11005
#define ID_AI_Alarm_Level1_Logging                  11006
#define ID_AI_Alarm_Level1_OutputType               11007
#define ID_AI_Alarm_Level1_OutputNo                 11008

#define ID_AI_Alarm_Level2                          11011
#define ID_AI_Alarm_Level2_OnOff                    11012
#define ID_AI_Alarm_Level2_Type                     11013
#define ID_AI_Alarm_Level2_Value                    11014
#define ID_AI_Alarm_Level2_Hysteresis               11015
#define ID_AI_Alarm_Level2_Logging                  11016
#define ID_AI_Alarm_Level2_OutputType               11017
#define ID_AI_Alarm_Level2_OutputNo                 11018

#define ID_AI_Alarm_Level3                          11021
#define ID_AI_Alarm_Level3_OnOff                    11022
#define ID_AI_Alarm_Level3_Type                     11023
#define ID_AI_Alarm_Level3_Value                    11024
#define ID_AI_Alarm_Level3_Hysteresis               11025
#define ID_AI_Alarm_Level3_Logging                  11026
#define ID_AI_Alarm_Level3_OutputType               11027
#define ID_AI_Alarm_Level3_OutputNo                 11028

#define ID_AI_Alarm_Level4                          11031
#define ID_AI_Alarm_Level4_OnOff                    11032
#define ID_AI_Alarm_Level4_Type                     11033
#define ID_AI_Alarm_Level4_Value                    11034
#define ID_AI_Alarm_Level4_Hysteresis               11035
#define ID_AI_Alarm_Level4_Logging                  11036
#define ID_AI_Alarm_Level4_OutputType               11037
#define ID_AI_Alarm_Level4_OutputNo                 11038

#define ID_AI_Alarm_Leve_Delay                      11041
#define ID_AI_Alarm_Leve_Delay_s                    11042
#define ID_AI_Alarm_Leve_Delay_m                    11043
#define ID_AI_Alarm_Leve_Delay_h                    11044

//AI-Disp 在12000-13000之间
#define ID_AI_Disp                                  12000   //

#define ID_AI_Disp_Color                            12001   //group
#define ID_AI_Disp_Color_Value                      12002

#define ID_AI_Disp_Tag                              12010   //group
#define ID_AI_Disp_Tag_Char                         12011
#define ID_AI_Disp_Tag_No                           12012

#define ID_AI_Disp_Zone                             12020   //group
#define ID_AI_Disp_Zone_Upper                        12021
#define ID_AI_Disp_Zone_Lower                        12022

#define ID_AI_Disp_Scale                            12030
#define ID_AI_Disp_Scale_Pos                        12031
#define ID_AI_Disp_Scale_Div                        12032

#define ID_AI_DISP_Bar                              12040
#define ID_AI_DISP_Bar_Pos                           12041
#define ID_AI_DISP_Bar_Div                           12042

#define ID_AI_DISP_Partial                          12050
#define ID_AI_DISP_Partial_OnOff                     12051
#define ID_AI_DISP_Partial_Boundary                  12052
#define ID_AI_DISP_Partial_Pos                       12053

#define ID_AI_DISP_ColorScaleBand                    12060
#define ID_AI_DISP_ColorScaleBand_Color              12061
#define ID_AI_DISP_ColorScaleBand_PosLower           12062
#define ID_AI_DISP_ColorScaleBand_PosUpper           12063
#define ID_AI_DISP_ColorScaleBand_Mode               12064

#define ID_AI_DISP_AlarmMark                        12070
#define ID_AI_DISP_AlarmMark_OnOff                  12071
#define ID_AI_DISP_AlarmMark_Type                   12072
#define ID_AI_DISP_AlarmMark_Color1                 12073
#define ID_AI_DISP_AlarmMark_Color2                 12074
#define ID_AI_DISP_AlarmMark_Color3                 12075
#define ID_AI_DISP_AlarmMark_Color4                 12076

#define ID_AI_DISP_DI                               12080
#define ID_AI_DISP_DI0_Str                          12081
#define ID_AI_DISP_DI1_Str                          12082

//AI-Correct 在13000-14000之间
#define ID_AI_Correct                               13000   //title

#define ID_AI_Correct_Mode                          13010       //group
#define ID_AI_Correct_ModeVlaue                     13011
#define ID_AI_Correct_ModeCnt                       13012

#define ID_AI_Correct_Point1                        13020
#define ID_AI_Correct_Point1_In                     13021
#define ID_AI_Correct_Point1_Out                    13022
#define ID_AI_Correct_Point1_StartIn                13023

#define ID_AI_Correct_Point2                        13030
#define ID_AI_Correct_Point2_In                     13031
#define ID_AI_Correct_Point2_Out                    13032
#define ID_AI_Correct_Point2_StartIn                13033

#define ID_AI_Correct_Point3                        13040
#define ID_AI_Correct_Point3_In                     13041
#define ID_AI_Correct_Point3_Out                    13042
#define ID_AI_Correct_Point3_StartIn                13043

#define ID_AI_Correct_Point4                        13050
#define ID_AI_Correct_Point4_In                     13051
#define ID_AI_Correct_Point4_Out                    13052
#define ID_AI_Correct_Point4_StartIn                13053

#define ID_AI_Correct_Point5                        13060
#define ID_AI_Correct_Point5_In                     13061
#define ID_AI_Correct_Point5_Out                    13062
#define ID_AI_Correct_Point5_StartIn                13063

#define ID_AI_Correct_Point6                        13070
#define ID_AI_Correct_Point6_In                     13071
#define ID_AI_Correct_Point6_Out                    13072
#define ID_AI_Correct_Point6_StartIn                13073

#define ID_AI_Correct_Point7                        13080
#define ID_AI_Correct_Point7_In                     13081
#define ID_AI_Correct_Point7_Out                    13082
#define ID_AI_Correct_Point7_StartIn                13083

#define ID_AI_Correct_Point8                        13090
#define ID_AI_Correct_Point8_In                     13091
#define ID_AI_Correct_Point8_Out                    13092
#define ID_AI_Correct_Point8_StartIn                13093

#define ID_AI_Correct_Point9                        13100
#define ID_AI_Correct_Point9_In                     13101
#define ID_AI_Correct_Point9_Out                    13102
#define ID_AI_Correct_Point9_StartIn                13103

#define ID_AI_Correct_Point10                        13110
#define ID_AI_Correct_Point10_In                     13111
#define ID_AI_Correct_Point10_Out                    13112
#define ID_AI_Correct_Point10_StartIn                13113

#define ID_AI_Correct_Point11                        13120
#define ID_AI_Correct_Point11_In                     13121
#define ID_AI_Correct_Point11_Out                    13122
#define ID_AI_Correct_Point11_StartIn                13123

#define ID_AI_Correct_Point12                        13130
#define ID_AI_Correct_Point12_In                     13131
#define ID_AI_Correct_Point12_Out                    13132
#define ID_AI_Correct_Point12_StartIn                13133


class Cfgconfig_AI : public QObject
{
    Q_OBJECT
public:
    static Cfgconfig_AI * instance();
    QVariant getAIData(quint32 ID, quint32 group);
    void setAIData(quint32 ID, quint32 group);
    QString getAIObjStr(quint32 ID, quint32 group = 0);     //第二个参数先保留，目前没有用到
    QString getAIValueObjStr(quint32 ID, quint32 group, ConfigObject *cfg);

private:
    explicit Cfgconfig_AI(QObject *parent = 0);

    //得到AI通道的配置信息
//    QVariant getAIData(quint32 ID, quint32 group);
    QVariant getAIRangeData(quint32 ID, quint32 group);
    QVariant getAIAlarmData(quint32 ID, quint32 group);
    QVariant getAIDisplayData(quint32 ID, quint32 group);
    QVariant getAICrorectionData(quint32 ID, quint32 group);

    QVariant getAIRangeRangeRangeData(AI_CONFIG AiCfg);

    //得到AI通道的对象名称描述
    QString getAIRangeStr(quint32 ID, quint32 group = 0);
    QString getAIAlarmStr(quint32 ID, quint32 group = 0);
    QString getAIDisplayStr(quint32 ID, quint32 group = 0);
    QString getAICrorectionStr(quint32 ID, quint32 group = 0);

    //通过ID group, 配置对象 得到AI通道的对象的值描述
    QString getAIRangeValueObjStr(quint32 ID, quint32 group, ConfigObject *cfg);
    QString getAIAlarmValueObjStr(quint32 ID, quint32 group, ConfigObject *cfg);
    QString getAIDisplayValueObjStr(quint32 ID, quint32 group, ConfigObject *cfg);
    QString getAICrorectionValueObjStr(quint32 ID, quint32 group, ConfigObject *cfg);

    QString getAIRangeTypeValueObjStr(quint32 type);
    QString getAIRangeScaleUnitValObjStr(uchar *unit);      //unit
    QString getAIRangeScaleUnitValObjStr(QString unit);      //unit
    QString getAIRangeRangeValueObjStr(quint32 type, quint32 range);
    QString getAIRangeRangeBasicChannelValueObjStr(quint32 data);
    QString getAIAlarmChannelObjValStr(quint32 num);

    //得到AI配置值和单位的字符串
    QString getAIDataUnitObjStr(float data, quint32 group);     //得到传进数据的单位，并转化为带单位的字符串输出

    QString getAIRangeCalCulateValueObjStr(quint32 calculateType);
    QString getAiRangeBiasValueObjStr(quint32 data, quint32 type, quint32 range);

    QString getAIAlarmTypeValObjStr(quint32 type);
    //QString getAIAlarmLevelOutputType(AI_CONFIG AiCfg, int alarmNum);
    QString getAIAlarmLevelOutputType(quint32 outputType);

    QString getAIDisplayColorValueObjStr(quint32 x);

    //得到AI配置值和单位的字符串
    QString getAIDataUnitObjStr(float data);
    QString getAIRangScaleUnit(float data);
    quint32 getAIRangeDecCnt(QString calPath, QString posPath, QString rangeUpperPath);

    // 把char *类型转化为QString类型
    QVariant getCharToString(uchar *str);

    //把配置树上的配置写入配置结构体中
    void setAIRangeData(quint32 ID, quint32 group);
    void setAIAlarmData(quint32 ID, quint32 group);
    void setAIDisplayData(quint32 ID, quint32 group);
    void setAICrorectionData(quint32 ID, quint32 group);

    void setAIRangeConfigData(AI_CONFIG * AiCfg, QString chanNum);
    void setAIRangeScaleUnit(uchar *uData, QString str, int len = 0);

    quint32 setAIOutputNo(quint32 type, quint32 code);


    quint32 getAIChannelDataAccuracy(AI_CONFIG & AiCfg) const;
    int getFloatToInt(float data, int pos);   //把float类型的数据乘以10的精度次方后转换为int类型返回，该函数避免了用float类型直接乘法后数值有差异


signals:

public slots:

};

#endif // CFGCONFIG_AI_H
