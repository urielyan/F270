#ifndef CFGCONFIG_DI_H
#define CFGCONFIG_DI_H

#include <QObject>
#include "configtreeobject.h"

//DI                15000 - 17999
#define DI_Start_ID                             15000
#define DI_End_ID                               17999

#define DI_Range_Start_ID                       15003
#define DI_Range_End_ID                         15999

#define DI_Alarm_Start_ID                       16000
#define DI_Alarm_End_ID                         16999

#define DI_Disp_Start_ID                        17000
#define DI_Disp_End_ID                          17999

/* ********************************* DI通道宏定义,在15000 - 17999之间 *******************/
#define ID_DI                                       15000

#define ID_DI_Last_CH                               15001
#define ID_DI_First_CH                              15002

/*DI-Range  在15003-16000之间*/
#define ID_DI_Range                                 15003

#define ID_DI_Range_Range                           15007

#define ID_DI_Range_Range_Type                      15010

#define ID_DI_Range_Range_Min                       15021
#define ID_DI_Range_Range_Max                       15022

#define ID_DI_Range_Range_DispMin                   15030
#define ID_DI_Range_Range_DispMax                   15031
#define ID_DI_Range_Range_Calculate                 15032
#define ID_DI_Range_Range_BasicChannel              15033

#define ID_DI_Range_Scale                           15040
#define ID_DI_Range_Scale_Pos                       15041
#define ID_DI_Range_Scale_Min                       15042
#define ID_DI_Range_Scale_Max                       15043
#define ID_DI_Range_Scale_Unit                      15044


//DI-Alarm 在16000-17000之间
#define ID_DI_Alarm                                 16000

#define ID_DI_Alarm_Level1                          16001
#define ID_DI_Alarm_Level1_OnOff                    16002
#define ID_DI_Alarm_Level1_Type                     16003
#define ID_DI_Alarm_Level1_Value                    16004
#define ID_DI_Alarm_Level1_Hysteresis               16005
#define ID_DI_Alarm_Level1_Logging                  16006
#define ID_DI_Alarm_Level1_OutputType               16007
#define ID_DI_Alarm_Level1_OutputNo                 16008

#define ID_DI_Alarm_Level2                          16011
#define ID_DI_Alarm_Level2_OnOff                    16012
#define ID_DI_Alarm_Level2_Type                     16013
#define ID_DI_Alarm_Level2_Value                    16014
#define ID_DI_Alarm_Level2_Hysteresis               16015
#define ID_DI_Alarm_Level2_Logging                  16016
#define ID_DI_Alarm_Level2_OutputType               16017
#define ID_DI_Alarm_Level2_OutputNo                 16018

#define ID_DI_Alarm_Level3                          16021
#define ID_DI_Alarm_Level3_OnOff                    16022
#define ID_DI_Alarm_Level3_Type                     16023
#define ID_DI_Alarm_Level3_Value                    16024
#define ID_DI_Alarm_Level3_Hysteresis               16025
#define ID_DI_Alarm_Level3_Logging                  16026
#define ID_DI_Alarm_Level3_OutputType               16027
#define ID_DI_Alarm_Level3_OutputNo                 16028

#define ID_DI_Alarm_Level4                          16031
#define ID_DI_Alarm_Level4_OnOff                    16032
#define ID_DI_Alarm_Level4_Type                     16033
#define ID_DI_Alarm_Level4_Value                    16034
#define ID_DI_Alarm_Level4_Hysteresis               16035
#define ID_DI_Alarm_Level4_Logging                  16036
#define ID_DI_Alarm_Level4_OutputType               16037
#define ID_DI_Alarm_Level4_OutputNo                 16038

#define ID_DI_Alarm_Leve_Delay                      16041
#define ID_DI_Alarm_Leve_Delay_s                    16042
#define ID_DI_Alarm_Leve_Delay_m                    16043
#define ID_DI_Alarm_Leve_Delay_h                    16044

//DI-Disp 在17000-18000之间
#define ID_DI_Disp                                  17000   //

#define ID_DI_Disp_Color                            17001   //group
#define ID_DI_Disp_Color_Value                      17002

#define ID_DI_Disp_Tag                              17010   //group
#define ID_DI_Disp_Tag_Char                         17011
#define ID_DI_Disp_Tag_No                           17012

#define ID_DI_Disp_Zone                             17020   //group
#define ID_DI_Disp_Zone_Upper                        17021
#define ID_DI_Disp_Zone_Lower                        17022

#define ID_DI_Disp_Scale                            17030
#define ID_DI_Disp_Scale_Pos                        17031
#define ID_DI_Disp_Scale_Div                        17032

#define ID_DI_DISP_Bar                              17040
#define ID_DI_DISP_Bar_Pos                          17041
#define ID_DI_DISP_Bar_Div                           17042

//#define ID_DI_DISP_Partial                          17050
//#define ID_DI_DISP_Partial_OnOff                     17051
//#define ID_DI_DISP_Partial_Boundary                  17052
//#define ID_DI_DISP_Partial_Pos                       17053

//#define ID_DI_DISP_ColorScaleBand                    17060
//#define ID_DI_DISP_ColorScaleBand_Color              17061
//#define ID_DI_DISP_ColorScaleBand_PosLower           17062
//#define ID_DI_DISP_ColorScaleBand_PosUpper           17063
//#define ID_DI_DISP_ColorScaleBand_Mode               17064

#define ID_DI_DISP_AlarmMark                        17070
#define ID_DI_DISP_AlarmMark_OnOff                  17071
#define ID_DI_DISP_AlarmMark_Type                   17072
#define ID_DI_DISP_AlarmMark_Color1                 17073
#define ID_DI_DISP_AlarmMark_Color2                 17074
#define ID_DI_DISP_AlarmMark_Color3                 17075
#define ID_DI_DISP_AlarmMark_Color4                 17076

#define ID_DI_DISP_DI                               17080
#define ID_DI_DISP_DI0_Str                          17081
#define ID_DI_DISP_DI1_Str                          17082

#define ID_DI_OpMode                                17090   //DI模块操作模式定义 0.标准   1.远程控制输入
#define ID_DI_OpMode_Value                          17091

class CfgConfig_DI : public QObject
{
    Q_OBJECT
public:
    static CfgConfig_DI *instance();
    QVariant getDIData(quint32 ID, quint32 group);
    void setDIData(quint32 ID, quint32 group);
    QString getDIObjStr(quint32 ID, quint32 group = 0);
    QString getDIValueObjStr(quint32 ID, quint32 group, ConfigObject *cfg);


private:
    explicit CfgConfig_DI(QObject *parent = 0);

    //get data
    QVariant getDIRangeData(quint32 ID, quint32 group);
    QVariant getDIAlarmData(quint32 ID, quint32 group);
    QVariant getDIDispData(quint32 ID, quint32 group);

    //get obj str
    QString getDIRangeObjStr(quint32 ID, quint32 group = 0);
    QString getDIAlarmObjStr(quint32 ID, quint32 group = 0);
    QString getDIDispObjStr(quint32 ID, quint32 group = 0);

    QString getDIRangeValueObjStr(quint32 ID, quint32 group, ConfigObject *cfg);
    QString getDIAlarmValueObjStr(quint32 ID, quint32 group, ConfigObject *cfg);
    QString getDIDispValueObjStr(quint32 ID, quint32 group, ConfigObject *cfg);

    QString getDIRangeTypeValueObjStr(quint32 type);
    QString getDIRangScaleUnit(int data);
    QString getDIRangeScaleUnitValObjStr(QString unit);
    QString getDIAlarmTypeValObjStr(quint32 type);

    void setDIRangeData(quint32 ID, quint32 group);
    void setDIAlarmData(quint32 ID, quint32 group);
    void setDIDispData(quint32 ID, quint32 group);


    QVariant getCharToString(uchar *str);
    QString getDIDisplayColorValueObjStr(quint32 x);
    void setStrToChar(uchar * uData, QString str, int len = 0);

    QString getDIRangeRangeBasicChannelValueObjStr(quint32 data);
    QString getDIAlarmChannelObjValStr(quint32 num);


signals:

public slots:

};

#endif // CFGCONFIG_DI_H
