#ifndef CFGCONFIG_COMM_H
#define CFGCONFIG_COMM_H

#include <QObject>
#include "./GlobalData/configobject.h"

/****************************************  Comm 22500 - 22700  *************************************************/
#define Comm_Start_ID                                   22500
#define Comm_End_ID                                     22699

#define Comm_Range_Start_ID                             22503
#define Comm_Range_End_ID                               22549

#define Comm_Alarm_Start_ID                             22550
#define Comm_Alarm_End_ID                               22599

#define Comm_Disp_Start_ID                              22600
#define Comm_Disp_End_ID                                22699

//Comm
#define ID_Comm                                         22500

#define ID_Comm_First_CH                                22501
#define ID_Comm_Last_CH                                 22502

#define ID_Comm_Range                                   22503   //title

#define ID_Comm_Range_Range                             22510   //group
#define ID_Comm_Range_Range_Switch                      22511   //通信通道开关，0-off, 1-on, 默认0
#define ID_Comm_Range_Range_Pos                         22512   //小数点位置：0/1/2/3/4/5,默认0
#define ID_Comm_Range_Range_Min                         22513   //范围下限：-999999 ~ 999999, 默认0
#define ID_Comm_Range_Range_Max                         22514    //范围上限：-999999 ~ 999999, 默认100
#define ID_Comm_Range_Range_Unit                        22515   //单位，字符串，最多6个半角字符, 中英文支持

#define ID_Comm_Range_PowerOn                           22520
#define ID_Comm_Range_PowerOn_Val                       22521    //电源接通时的值类型， 0-最终值，1-预设值，默认0

#define ID_Comm_Range_Pre                               22530   //预设值
#define ID_Comm_Range_Pre_Val                           22531

#define ID_Comm_Range_Timer                             22540   //监视时钟开关
#define ID_Comm_Range_Timer_Switch                      22541       //监视时钟开关,0-off, 1-on
#define ID_Comm_Range_Timer_Interval                    22542   //监视器计时器时间，1-120s,默认30
#define ID_Comm_Range_Timer_ResetVal                    22543    //计时器满值设定值类型，0-最终值，1-预设值。默认0

//alarm
#define ID_Comm_Alarm                                   22550       //title

#define ID_Comm_Alarm_Level1                            22551
#define ID_Comm_Alarm_Level1_OnOff                      22552   //报警开关, 0:off,  1:on
#define ID_Comm_Alarm_Level1_Type                       22553   //报警类型
#define ID_Comm_Alarm_Level1_Value                      22554   //报警值，-999999 ~ 999999，忽略小数点
#define ID_Comm_Alarm_Level1_Hysteresis                 22555   //滞后，上/下限时有效，0~100000
#define ID_Comm_Alarm_Level1_Logging                    22556   //检出, 0-off, 1-on
#define ID_Comm_Alarm_Level1_OutputType                 22557   //报警输出类型， 0-off, 1-Relay(继电器), 2-Switch(内部开关)
#define ID_Comm_Alarm_Level1_OutputNo                   22558   //报警输出通道， Relay/Switches

#define ID_Comm_Alarm_Level2                            22561
#define ID_Comm_Alarm_Level2_OnOff                      22562   //报警开关, 0:off,  1:on
#define ID_Comm_Alarm_Level2_Type                       22563   //报警类型
#define ID_Comm_Alarm_Level2_Value                      22564   //报警值，-999999 ~ 999999，忽略小数点
#define ID_Comm_Alarm_Level2_Hysteresis                 22565   //滞后，上/下限时有效，0~100000
#define ID_Comm_Alarm_Level2_Logging                    22566   //检出, 0-off, 1-on
#define ID_Comm_Alarm_Level2_OutputType                 22567   //报警输出类型， 0-off, 1-Relay(继电器), 2-Switch(内部开关)
#define ID_Comm_Alarm_Level2_OutputNo                   22568   //报警输出通道， Relay/Switches


#define ID_Comm_Alarm_Level3                            22571
#define ID_Comm_Alarm_Level3_OnOff                      22572   //报警开关, 0:off,  1:on
#define ID_Comm_Alarm_Level3_Type                       22573   //报警类型
#define ID_Comm_Alarm_Level3_Value                      22574   //报警值，-999999 ~ 999999，忽略小数点
#define ID_Comm_Alarm_Level3_Hysteresis                 22575   //滞后，上/下限时有效，0~100000
#define ID_Comm_Alarm_Level3_Logging                    22576   //检出, 0-off, 1-on
#define ID_Comm_Alarm_Level3_OutputType                 22577   //报警输出类型， 0-off, 1-Relay(继电器), 2-Switch(内部开关)
#define ID_Comm_Alarm_Level3_OutputNo                   22578   //报警输出通道， Relay/Switches


#define ID_Comm_Alarm_Level4                            22581
#define ID_Comm_Alarm_Level4_OnOff                      22582   //报警开关, 0:off,  1:on
#define ID_Comm_Alarm_Level4_Type                       22583   //报警类型
#define ID_Comm_Alarm_Level4_Value                      22584   //报警值，-999999 ~ 999999，忽略小数点
#define ID_Comm_Alarm_Level4_Hysteresis                 22585   //滞后，上/下限时有效，0~100000
#define ID_Comm_Alarm_Level4_Logging                    22586   //检出, 0-off, 1-on
#define ID_Comm_Alarm_Level4_OutputType                 22587   //报警输出类型， 0-off, 1-Relay(继电器), 2-Switch(内部开关)
#define ID_Comm_Alarm_Level4_OutputNo                   22588   //报警输出通道， Relay/Switches

#define ID_Comm_Alarm_Leve_Delay                        22591   //报警延迟时间，延迟上下限时有效，[0]-秒，[1]-分，[2]-时
#define ID_Comm_Alarm_Leve_Delay_s                      22592
#define ID_Comm_Alarm_Leve_Delay_m                      22593
#define ID_Comm_Alarm_Leve_Delay_h                      22594

//disp
#define ID_Comm_Disp                                    22660 //

#define ID_Comm_Disp_Color                              22661 //group
#define ID_Comm_Disp_Color_Value                        22662 //通道颜色，Byte2-Red, Byte1-Green, Byte0-Blue

#define ID_Comm_Disp_Tag                                22670 //group
#define ID_Comm_Disp_Tag_Char                           22671 //标记字符串，中英支持，最多32个半角字符
#define ID_Comm_Disp_Tag_No                             22672 //标记号码，符号，最多16个半角字符

#define ID_Comm_Disp_Zone                               22680 //group
#define ID_Comm_Disp_Zone_Upper                         22681 //显示区域上限，5-100,单位%
#define ID_Comm_Disp_Zone_Lower                         22682 //显示区域上限，0-95,单位%

#define ID_Comm_Disp_Scale                              22690
#define ID_Comm_Disp_Scale_Pos                          22691 //标尺位置，1-10,默认1
#define ID_Comm_Disp_Scale_Div                          22692 //标尺分割，4-13, 13:C10

#define ID_Comm_DISP_Bar                                22600
#define ID_Comm_DISP_Bar_Pos                            22601   //棒图基准位置， 0-Lower, 1-Center, 2-Upper
#define ID_Comm_DISP_Bar_Div                            22602       //棒图分割数，4-12, 默认10

#define ID_Comm_DISP_Partial                            22610
#define ID_Comm_DISP_Partial_OnOff                      22611   //部分压缩放大位置，1-99, 单位：%，默认50
#define ID_Comm_DISP_Partial_Boundary                   22612   //部分压缩放大边界值
#define ID_Comm_DISP_Partial_Pos                        22613    //部分压缩放大开关,0-off, 1-on

#define ID_Comm_DISP_ColorScaleBand                     22620
#define ID_Comm_DISP_ColorScaleBand_Color               22621   //彩色标尺带模式， 0-off, 1-内侧， 2-外侧.
#define ID_Comm_DISP_ColorScaleBand_PosLower            22622   //彩色标尺带显示位置下限
#define ID_Comm_DISP_ColorScaleBand_PosUpper            22623   //彩色标尺带显示位置上限
#define ID_Comm_DISP_ColorScaleBand_Mode                22624   //彩色标尺带颜色，Byte2-Red, Byte1-Green, Byte0-Blue

#define ID_Comm_DISP_AlarmMark                          22630
#define ID_Comm_DISP_AlarmMark_OnOff                    22631   //报警标记开关，0-0ff, 1-on
#define ID_Comm_DISP_AlarmMark_Type                     22632     //报警标记类型，0-报警， 1-固定
#define ID_Comm_DISP_AlarmMark_Color1                   22633   //报警1-4颜色， Byte2-Red, Byte1-Green, Byte0-Blue
#define ID_Comm_DISP_AlarmMark_Color2                   22634
#define ID_Comm_DISP_AlarmMark_Color3                   22635
#define ID_Comm_DISP_AlarmMark_Color4                   22636


class CfgConfig_Comm : public QObject
{
    Q_OBJECT
public:
    static CfgConfig_Comm * instance();

    QVariant getCommData(quint32 ID, quint32 group);
    void setCommData(quint32 ID, quint32 group);

    QString getCommObjStr(quint32 ID, quint32 group = 0);
    QString getCommObjValStr(quint32 ID, quint32 group, ConfigObject *cfg);

private:
    explicit CfgConfig_Comm(QObject *parent = 0);

    QVariant getCommRangeData(quint32 ID, quint32 group);
    QVariant getCommAlarmData(quint32 ID, quint32 group);
    QVariant getCommDispData(quint32 ID, quint32 group);



    QString getCommRangeObjStr(quint32 ID, quint32 group = 0);
    QString getCommAlarmObjStr(quint32 ID, quint32 group = 0);
    QString getCommDispObjStr(quint32 ID, quint32 group = 0);

    QString getCommRangeObjValStr(quint32 ID, quint32 group, ConfigObject * cfg);
    QString getCommAlarmObjValStr(quint32 ID, quint32 group, ConfigObject * cfg);
    QString getCommDispObjValStr(quint32 ID, quint32 group, ConfigObject * cfg);

    QString getCommRangScaleUnit(float data);
    QString getCommRangeScaleUnitValObjStr(QString unit);
    QString getCommAlarmTypeValObjStr(quint32 type);
    QString getCommChannelObjValStr(quint32 num);

    QString getCommDisplayColorValueObjStr(quint32 x);

    void setCommRangeData(quint32 ID, quint32 group);
    void setCommAlarmData(quint32 ID, quint32 group);
    void setCommDispData(quint32 ID, quint32 group);


    QVariant getCharToString(uchar *str);
    void setStrToChar(uchar * uData, QString str, int len);

    int getFloatToInt(float data, int pos);

signals:

public slots:

private:


};

#endif // CFGCONFIG_COMM_H
