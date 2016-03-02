#ifndef CFGCONFIG_MEASURE_H
#define CFGCONFIG_MEASURE_H

#include <QObject>
#include "./GlobalData/configobject.h"
#include "./GlobalData/globalDef.h"


/****************************************  Measure 21500 - 21600  *************************************************/

#define Measure_Start_ID                                    21500
#define Measure_End_ID                                      21599

#define Measure_Unit_Start_ID                               30000
#define Measure_Unit_End_ID                                 40000

//measure
#define ID_Measure                                          21500

#define ID_Measure_Scan                                     21510   //group
#define ID_Measure_Scan_Interval                            21511       //测量周期

#define ID_Measure_OverRange                                21520   //量程溢出判定
#define ID_Measure_OverRange_Value                          21521

#define ID_Measure_Select                                   21530       //选择单元



/****************************************  Measure-unit 30000 - 32000 *************************************************/
#define Measure_Select_Unit_Start_ID                        30000
#define Measure_Select_Unit_End_ID                          30199

#define ID_Measure_Select_MainUnit                          30000
#define ID_Measure_Select_ExtUnit1                          30001
#define ID_Measure_Select_ExtUnit2                          30002
#define ID_Measure_Select_ExtUnit3                          30003
#define ID_Measure_Select_ExtUnit4                          30004
#define ID_Measure_Select_ExtUnit5                          30005
#define ID_Measure_Select_ExtUnit6                          30006


#define ID_Measure_Select_MainUnit_Mod0                     30010
#define ID_Measure_Select_MainUnit_Mod1                     30011
#define ID_Measure_Select_MainUnit_Mod2                     30012
#define ID_Measure_Select_MainUnit_Mod3                     30013
#define ID_Measure_Select_MainUnit_Mod4                     30014
#define ID_Measure_Select_MainUnit_Mod5                     30015
#define ID_Measure_Select_MainUnit_Mod6                     30016
#define ID_Measure_Select_MainUnit_Mod7                     30017
#define ID_Measure_Select_MainUnit_Mod8                     30018
#define ID_Measure_Select_MainUnit_Mod9                     30019

#define ID_Measure_Select_ExtUnit1_Mod0                     30020
#define ID_Measure_Select_ExtUnit1_Mod1                     30021
#define ID_Measure_Select_ExtUnit1_Mod2                     30022
#define ID_Measure_Select_ExtUnit1_Mod3                     30023
#define ID_Measure_Select_ExtUnit1_Mod4                     30024
#define ID_Measure_Select_ExtUnit1_Mod5                     30025

#define ID_Measure_Select_ExtUnit2_Mod0                     30030
#define ID_Measure_Select_ExtUnit2_Mod1                     30031
#define ID_Measure_Select_ExtUnit2_Mod2                     30032
#define ID_Measure_Select_ExtUnit2_Mod3                     30033
#define ID_Measure_Select_ExtUnit2_Mod4                     30034
#define ID_Measure_Select_ExtUnit2_Mod5                     30035

#define ID_Measure_Select_ExtUnit3_Mod0                     30040
#define ID_Measure_Select_ExtUnit3_Mod1                     30041
#define ID_Measure_Select_ExtUnit3_Mod2                     30042
#define ID_Measure_Select_ExtUnit3_Mod3                     30043
#define ID_Measure_Select_ExtUnit3_Mod4                     30044
#define ID_Measure_Select_ExtUnit3_Mod5                     30045

#define ID_Measure_Select_ExtUnit4_Mod0                     30050
#define ID_Measure_Select_ExtUnit4_Mod1                     30051
#define ID_Measure_Select_ExtUnit4_Mod2                     30052
#define ID_Measure_Select_ExtUnit4_Mod3                     30053
#define ID_Measure_Select_ExtUnit4_Mod4                     30054
#define ID_Measure_Select_ExtUnit4_Mod5                     30055

#define ID_Measure_Select_ExtUnit5_Mod0                     30060
#define ID_Measure_Select_ExtUnit5_Mod1                     30061
#define ID_Measure_Select_ExtUnit5_Mod2                     30062
#define ID_Measure_Select_ExtUnit5_Mod3                     30063
#define ID_Measure_Select_ExtUnit5_Mod4                     30064
#define ID_Measure_Select_ExtUnit5_Mod5                     30065

#define ID_Measure_Select_ExtUnit6_Mod0                     30070
#define ID_Measure_Select_ExtUnit6_Mod1                     30071
#define ID_Measure_Select_ExtUnit6_Mod2                     30072
#define ID_Measure_Select_ExtUnit6_Mod3                     30073
#define ID_Measure_Select_ExtUnit6_Mod4                     30074
#define ID_Measure_Select_ExtUnit6_Mod5                     30075




#define ID_Measure_AI_Unit_Mode                             30101
#define ID_Measure_AI_Unit_Mode_Value                       30102

#define ID_Measure_AI_Unit_AD                               30103
#define ID_Measure_AI_Unit_AD_Value                         30104

#define ID_Measure_AI_Unit_Gneral                           30105
#define ID_Measure_AI_Unit_Gneral_Low                       30106
#define ID_Measure_AI_Unit_Gneral_Upper                     30107

#define ID_Measure_DI_Unit_Mode                             30111
#define ID_Measure_DI_Unit_Mode_Value                       30112

class CfgConfig_Measure : public QObject
{
    Q_OBJECT
public:
    static CfgConfig_Measure *instance();

    QVariant getMeasureData(quint32 ID, quint32 group);
    void setMeasureData(quint32 ID, quint32);

    void setMeasureDIModeData(quint32 module);    //该接口保存DI为远程模式下的Module号

    QString getMeasureObjStr(quint32 ID, quint32 group = 0);
    QString getMeasureObjValStr(quint32 ID, quint32 group, ConfigObject *cfg);

    void setDeviceMeasureConfig(DEVICE_CONFIG & deviceConfig) { p_deviceConfig = &deviceConfig;}


public:
    explicit CfgConfig_Measure(QObject *parent = 0);

signals:

public slots:

private:
   DEVICE_CONFIG *                   p_deviceConfig;

};

#endif // CFGCONFIG_MEASURE_H
