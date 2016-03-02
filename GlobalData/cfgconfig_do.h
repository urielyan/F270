#ifndef CFGCONFIG_DO_H
#define CFGCONFIG_DO_H

#include "./GlobalData/configtreeobject.h"

//DO                18000 - 19999
#define DO_Start_ID                             18000
#define DO_End_ID                               19999

#define DO_Range_Start_ID                       18003
#define DO_Range_End_ID                         18999

#define DO_Disp_Start_ID                        19000
#define DO_Disp_End_ID                          19999

/* ***************************** DO通道宏定义,在18000 - 19999之间 ***********************/
#define ID_DO                                       18000

#define ID_DO_Last_CH                               18001
#define ID_DO_First_CH                              18002

/*DI-Range  在18003-19000之间*/
#define ID_DO_Range                                 18003

#define ID_DO_Range_Range                           18007

#define ID_DO_Range_Range_Type                      18010       //量程类型, 0-报警， 1-手动

#define ID_DO_Range_Range_Min                       18021       //范围下限：0/1，上下限值不能相同
#define ID_DO_Range_Range_Max                       18022

#define ID_DO_Range_Range_DispMin                   18030       //范围下限：0/1，上下限值不能相同
#define ID_DO_Range_Range_DispMax                   18031
#define ID_DO_Range_Range_Unit                      18032       //单位，字符串，最多6个半角字符

//action
#define ID_DO_Range_Action                          18040
#define ID_DO_Range_Action_Energize                 18041       //励磁/非励磁， 0-励磁， 1-非励磁
#define ID_DO_Range_Action_ActionMode               18042       //动作，0-或， 1-与， 2-再故障再报警
#define ID_DO_Range_Action_ACK                      18043       //报警接触动作，0-标准，1-复位
#define ID_DO_Range_Action_Hold                     18044       //保持：0-保持，1-非保持 **再故障再报警时无效**
#define ID_DO_Range_Action_Interval                 18045       //继电器非动作间隔，500ms/1s/2s**再故障再报警时有效**

//DO-Disp 在19000-20000之间
#define ID_DO_Disp                                  19000   //

#define ID_DO_Disp_Color                            19001   //group
#define ID_DO_Disp_Color_Value                      19002        //通道颜色，Byte2-Red, Byte1-Green, Byte0-Blue

#define ID_DO_Disp_Tag                              19010   //group
#define ID_DO_Disp_Tag_Char                         19011   //标记字符串，中英支持，最多32个半角字符
#define ID_DO_Disp_Tag_No                           19012   //标记号码，符号，最多16个半角字符

#define ID_DO_Disp_Zone                             19020   //group
#define ID_DO_Disp_Zone_Upper                        19021   //显示区域上限，5-100,单位%
#define ID_DO_Disp_Zone_Lower                        19022  //显示区域上限，0-95,单位%

#define ID_DO_Disp_Scale                            19030
#define ID_DO_Disp_Scale_Pos                        19031       //标尺位置，1-10,默认1
#define ID_DO_Disp_Scale_Div                        19032

#define ID_DO_DISP_Bar                              19040
#define ID_DO_DISP_Bar_Pos                          19041   //棒图基准位置， 0-Lower, 1-Center, 2-Upper
#define ID_DO_DISP_Bar_Div                          19042

#define ID_DO_DISP_DI                               19080       //最多8个半角字符，中英文
#define ID_DO_DISP_DI0_Str                          19081
#define ID_DO_DISP_DI1_Str                          19082

#include <QObject>

class CfgConfig_DO : public QObject
{
    Q_OBJECT
public:
    static CfgConfig_DO *instance();
    QVariant getDOData(quint32 ID, quint32 group);
    void setDOData(quint32 ID, quint32 group);
    QString getDOObjStr(quint32 ID, quint32 group = 0);
    QString getDOValueObjStr(quint32 ID, quint32 group, ConfigObject *cfg);

private:
    explicit CfgConfig_DO(QObject *parent = 0);

    QVariant getDORangeData(quint32 ID, quint32 group);
    QVariant getDODispData(quint32 ID, quint32 group);

    // obj str
    QString getDORangeObjStr(quint32 ID, quint32 group = 0);
    QString getDODispObjStr(quint32 ID, quint32 group = 0);

    //obj val str
    QString getDORangeObjValStr(quint32 ID, quint32 group, ConfigObject * cfg);
    QString getDODispObjValStr(quint32 ID, quint32 group,  ConfigObject * cfg);

    void setDORangeData(quint32 ID, quint32 group);
    void setDODispData(quint32 ID, quint32 group);

    // 把char *类型转化为QString类型
    QVariant getCharToString(uchar *str);
    QString getDODisplayColorValueObjStr(quint32 x);
    //把QString转化成uchar *
    void setStrToChar(uchar * uData, QString str, int len = 0);

signals:

public slots:

};

#endif // CFGCONFIG_DO_H
