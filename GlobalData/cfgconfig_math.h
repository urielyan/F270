#ifndef CFGCONFIG_MATH_H
#define CFGCONFIG_MATH_H

#include <QObject>

class ConfigObject;

/******************************************** math 21000 - 21299*******************************************/
#define Math_Start_ID                               21000
#define Math_End_ID                                 21299

#define Math_Exp_Start_ID                           21003
#define Math_Exp_End_ID                             21030

#define Math_Alarm_Start_ID                         21040
#define Math_Alarm_End_ID                           21099

#define Math_Disp_Start_ID                          21100
#define Math_Disp_End_ID                            21179

#define Math_Action_Start_ID                         21180
#define Math_Action_End_ID                           21189

#define Math_Const_Start_ID                         21190
#define Math_Const_End_ID                           21299


//math
#define ID_Math                                     21000
#define ID_Math_FirstCH                             21001
#define ID_Math_LastCH                              21002

//math exp 21000 - 21030
#define ID_Math_Exp                                 21003       //title

#define  ID_Math_Exp_MathRange                      21004       //group
#define  ID_Math_Exp_MathRange_Switch               21005       //运算通道开关
#define  ID_Math_Exp_MathRange_Exp                  21006   //运算式，最多120个半角字符
#define  ID_Math_Exp_MathRange_Pos                  21007   //小数点位置：0/1/2/3/4/5,默认2
#define  ID_Math_Exp_MathRange_Min                  21008   //范围下限：-999999 ~ 999999, 默认0
#define  ID_Math_Exp_MathRange_Max                  21009   //范围上限：-999999 ~ 999999, 默认10000
#define  ID_Math_Exp_MathRange_Unit                 21010   //单位，字符串，最多6个半角字符

#define  ID_Math_Exp_Tlog                           21011       //group
#define  ID_Math_Exp_Tlog_Type                      21012   //TLOG计时器类型， 0-计时器， 1-匹配计时器，默认0
#define  ID_Math_Exp_Tlog_NO                        21013     //TLOG计时器号码，1~4
#define  ID_Math_Exp_Tlog_SumScale                  21014       //累计单位，off:0, 秒:1000， 分:60000， 时:3600000,单位ms
#define  ID_Math_Exp_Tlog_Reset                     21015       //复位开关,0-off, 1-on

#define  ID_Math_Exp_Roll                           21020       //group
#define  ID_Math_Exp_Roll_Switch                    21021       //移动平均开关,0-off, 1-on
#define  ID_Math_Exp_Roll_Interval                  21022       //移动平均采样间隔，测量周期的整数倍，单位：S，默认10s
#define  ID_Math_Exp_Roll_AvgNumber                 21023       //移动平均采样数，移动平均时间 = 采样间隔 * 采样数

//Math-Alarm 在21040-22090之间
#define ID_Math_Alarm                               21040

#define ID_Math_Alarm_Level1                        21041
#define ID_Math_Alarm_Level1_OnOff                  21042       //报警开关, 0-off, 1-on
#define ID_Math_Alarm_Level1_Type                   21043        //报警类型
#define ID_Math_Alarm_Level1_Value                  21044       //报警值，-999999 ~ 999999，忽略小数点
#define ID_Math_Alarm_Level1_Hysteresis             21045       //滞后，上/下限时有效，0~100000
#define ID_Math_Alarm_Level1_Logging                21046       //检出, 0-off, 1-on
#define ID_Math_Alarm_Level1_OutputType             21047       //报警输出类型， 0-off, 1-Relay(继电器), 2-Switch(内部开关)
#define ID_Math_Alarm_Level1_OutputNo               21048       //报警输出通道， Relay/Switches

#define ID_Math_Alarm_Level2                        21051
#define ID_Math_Alarm_Level2_OnOff                  21052
#define ID_Math_Alarm_Level2_Type                   21053
#define ID_Math_Alarm_Level2_Value                  21054
#define ID_Math_Alarm_Level2_Hysteresis             21055
#define ID_Math_Alarm_Level2_Logging                21056
#define ID_Math_Alarm_Level2_OutputType             21057
#define ID_Math_Alarm_Level2_OutputNo               21058

#define ID_Math_Alarm_Level3                        21061
#define ID_Math_Alarm_Level3_OnOff                  21062
#define ID_Math_Alarm_Level3_Type                   21063
#define ID_Math_Alarm_Level3_Value                  21064
#define ID_Math_Alarm_Level3_Hysteresis             21065
#define ID_Math_Alarm_Level3_Logging                21066
#define ID_Math_Alarm_Level3_OutputType             21067
#define ID_Math_Alarm_Level3_OutputNo               21068

#define ID_Math_Alarm_Level4                        21071
#define ID_Math_Alarm_Level4_OnOff                  21072
#define ID_Math_Alarm_Level4_Type                   21073
#define ID_Math_Alarm_Level4_Value                  21074
#define ID_Math_Alarm_Level4_Hysteresis             21075
#define ID_Math_Alarm_Level4_Logging                21076
#define ID_Math_Alarm_Level4_OutputType             21077
#define ID_Math_Alarm_Level4_OutputNo               21078

#define ID_Math_Alarm_Leve_Delay                    21081       //报警延迟时间，延迟上下限时有效，[0]-秒，[1]-分，[2]-时
#define ID_Math_Alarm_Leve_Delay_s                  21082
#define ID_Math_Alarm_Leve_Delay_m                  21083
#define ID_Math_Alarm_Leve_Delay_h                  21084

//
//AI-Disp 在21100-21200之间
#define ID_Math_Disp                                  21100   //

#define ID_Math_Disp_Color                            21101   //group
#define ID_Math_Disp_Color_Value                      21102     //通道颜色，Byte2-Red, Byte1-Green, Byte0-Blue

#define ID_Math_Disp_Tag                              21110   //group
#define ID_Math_Disp_Tag_Char                         21111     //标记字符串，中英支持，最多32个半角字符
#define ID_Math_Disp_Tag_No                           21112     //标记号码，符号，最多16个半角字符

#define ID_Math_Disp_Zone                             21120   //group
#define ID_Math_Disp_Zone_Upper                       21121     //显示区域上限，5-100,单位%
#define ID_Math_Disp_Zone_Lower                       21122     //显示区域上限，0-95,单位%

#define ID_Math_Disp_Scale                            21130
#define ID_Math_Disp_Scale_Pos                        21131     //标尺位置，1-10,默认1
#define ID_Math_Disp_Scale_Div                        21132     //标尺分割，4-13, 13:C10

#define ID_Math_DISP_Bar                              21140
#define ID_Math_DISP_Bar_Pos                          21141      //棒图基准位置， 0-Lower, 1-Center, 2-Upper
#define ID_Math_DISP_Bar_Div                          21142      //棒图分割数，4-12, 默认10

#define ID_Math_DISP_Partial                          21150
#define ID_Math_DISP_Partial_OnOff                    21151     //部分压缩放大开关
#define ID_Math_DISP_Partial_Boundary                 21152     //部分压缩放大边界值
#define ID_Math_DISP_Partial_Pos                      21153     //部分压缩放大位置，1-99, 单位：%，默认50

#define ID_Math_DISP_ColorScaleBand                    21160
#define ID_Math_DISP_ColorScaleBand_Color              21161        //彩色标尺带颜色，Byte2-Red, Byte1-Green, Byte0-Blue
#define ID_Math_DISP_ColorScaleBand_PosLower           21162        //彩色标尺带显示位置上限
#define ID_Math_DISP_ColorScaleBand_PosUpper           21163        //彩色标尺带显示位置下限
#define ID_Math_DISP_ColorScaleBand_Mode               21164        //彩色标尺带模式， 0-off, 1-内侧， 2-外侧.

#define ID_Math_DISP_AlarmMark                         21170
#define ID_Math_DISP_AlarmMark_OnOff                   21171     //报警标记开关，0-0ff, 1-on
#define ID_Math_DISP_AlarmMark_Type                    21172      //报警标记类型，0-报警， 1-固定
#define ID_Math_DISP_AlarmMark_Color1                  21173      //报警1-4颜色， Byte2-Red, Byte1-Green, Byte0-Blue
#define ID_Math_DISP_AlarmMark_Color2                  21174
#define ID_Math_DISP_AlarmMark_Color3                  21175
#define ID_Math_DISP_AlarmMark_Color4                  21176


//math action
#define ID_Math_Action                                  21180   //title

#define ID_Math_Action_ErrorVal                         21181   //运算错误显示值
#define ID_Math_Action_KeyAction                        21182   //START/STOP键动作设定

#define ID_Math_Action_Over                             21183
#define ID_Math_Action_Over_SumAve                      21184   //SUM, AVE计算溢出处理
#define ID_Math_Action_Over_MaxMinPP                    21185   //MAX, MIN, P-P计算溢出处理


//math const
#define ID_Math_Const                                   21190
#define ID_Math_Const_NumVal                            21191   //运算式常数Number

#define ID_Math_Const_Num                               21192   //group

#define ID_Math_Const_Num_1                             21200       //运算式常数
#define ID_Math_Const_Num_2                             21201
#define ID_Math_Const_Num_3                             21202
#define ID_Math_Const_Num_4                             21203
#define ID_Math_Const_Num_5                             21204
#define ID_Math_Const_Num_6                             21205
#define ID_Math_Const_Num_7                             21206
#define ID_Math_Const_Num_8                             21207
#define ID_Math_Const_Num_9                             21208
#define ID_Math_Const_Num_10                            21209

#define ID_Math_Const_Num_11                             21210       //运算式常数
#define ID_Math_Const_Num_12                             21211
#define ID_Math_Const_Num_13                             21212
#define ID_Math_Const_Num_14                             21213
#define ID_Math_Const_Num_15                             21214
#define ID_Math_Const_Num_16                             21215
#define ID_Math_Const_Num_17                             21216
#define ID_Math_Const_Num_18                             21217
#define ID_Math_Const_Num_19                             21218
#define ID_Math_Const_Num_20                             21219

#define ID_Math_Const_Num_21                             21220       //运算式常数
#define ID_Math_Const_Num_22                             21221
#define ID_Math_Const_Num_23                             21222
#define ID_Math_Const_Num_24                             21223
#define ID_Math_Const_Num_25                             21224
#define ID_Math_Const_Num_26                             21225
#define ID_Math_Const_Num_27                             21226
#define ID_Math_Const_Num_28                             21227
#define ID_Math_Const_Num_29                             21228
#define ID_Math_Const_Num_30                             21229

#define ID_Math_Const_Num_31                             21230       //运算式常数
#define ID_Math_Const_Num_32                             21231
#define ID_Math_Const_Num_33                             21232
#define ID_Math_Const_Num_34                             21233
#define ID_Math_Const_Num_35                             21234
#define ID_Math_Const_Num_36                             21235
#define ID_Math_Const_Num_37                             21236
#define ID_Math_Const_Num_38                             21237
#define ID_Math_Const_Num_39                             21238
#define ID_Math_Const_Num_40                             21239

#define ID_Math_Const_Num_41                             21240       //运算式常数
#define ID_Math_Const_Num_42                             21241
#define ID_Math_Const_Num_43                             21242
#define ID_Math_Const_Num_44                             21243
#define ID_Math_Const_Num_45                             21244
#define ID_Math_Const_Num_46                             21245
#define ID_Math_Const_Num_47                             21246
#define ID_Math_Const_Num_48                             21247
#define ID_Math_Const_Num_49                             21248
#define ID_Math_Const_Num_50                             21249

#define ID_Math_Const_Num_51                             21250       //运算式常数
#define ID_Math_Const_Num_52                             21251
#define ID_Math_Const_Num_53                             21252
#define ID_Math_Const_Num_54                             21253
#define ID_Math_Const_Num_55                             21254
#define ID_Math_Const_Num_56                             21255
#define ID_Math_Const_Num_57                             21256
#define ID_Math_Const_Num_58                             21257
#define ID_Math_Const_Num_59                             21258
#define ID_Math_Const_Num_60                             21259


#define ID_Math_Const_Num_61                             21260       //运算式常数
#define ID_Math_Const_Num_62                             21261
#define ID_Math_Const_Num_63                             21262
#define ID_Math_Const_Num_64                             21263
#define ID_Math_Const_Num_65                             21264
#define ID_Math_Const_Num_66                             21265
#define ID_Math_Const_Num_67                             21266
#define ID_Math_Const_Num_68                             21267
#define ID_Math_Const_Num_69                             21268
#define ID_Math_Const_Num_70                             21269

#define ID_Math_Const_Num_71                             21270       //运算式常数
#define ID_Math_Const_Num_72                             21271
#define ID_Math_Const_Num_73                             21272
#define ID_Math_Const_Num_74                             21273
#define ID_Math_Const_Num_75                             21274
#define ID_Math_Const_Num_76                             21275
#define ID_Math_Const_Num_77                             21276
#define ID_Math_Const_Num_78                             21277
#define ID_Math_Const_Num_79                             21278
#define ID_Math_Const_Num_80                             21279

#define ID_Math_Const_Num_81                             21280       //运算式常数
#define ID_Math_Const_Num_82                             21281
#define ID_Math_Const_Num_83                             21282
#define ID_Math_Const_Num_84                             21283
#define ID_Math_Const_Num_85                             21284
#define ID_Math_Const_Num_86                             21285
#define ID_Math_Const_Num_87                             21286
#define ID_Math_Const_Num_88                             21287
#define ID_Math_Const_Num_89                             21288
#define ID_Math_Const_Num_90                             21289

#define ID_Math_Const_Num_91                             21290       //运算式常数
#define ID_Math_Const_Num_92                             21291
#define ID_Math_Const_Num_93                             21292
#define ID_Math_Const_Num_94                             21293
#define ID_Math_Const_Num_95                             21294
#define ID_Math_Const_Num_96                             21295
#define ID_Math_Const_Num_97                             21296
#define ID_Math_Const_Num_98                             21297
#define ID_Math_Const_Num_99                             21298
#define ID_Math_Const_Num_100                            21299

//移动平均采样间隔
typedef enum Roll_Avg_Interval_Enum
{
    Roll_Avg_Interval_1s = 0,
    Roll_Avg_Interval_2s,
    Roll_Avg_Interval_3s,
    Roll_Avg_Interval_4s,
    Roll_Avg_Interval_5s,
    Roll_Avg_Interval_6s,
    Roll_Avg_Interval_10s,
    Roll_Avg_Interval_12s,
    Roll_Avg_Interval_15s,
    Roll_Avg_Interval_20s,
    Roll_Avg_Interval_30s,
    Roll_Avg_Interval_1min,
    Roll_Avg_Interval_2min,
    Roll_Avg_Interval_3min,
    Roll_Avg_Interval_4min,
    Roll_Avg_Interval_5min,
    Roll_Avg_Interval_6min,
    Roll_Avg_Interval_10min,
    Roll_Avg_Interval_12min,
    Roll_Avg_Interval_15min,
    Roll_Avg_Interval_20min,
    Roll_Avg_Interval_30min,
    Roll_Avg_Interval_1h
}Roll_Avg_Interval;


class CfgConfig_Math : public QObject
{
    Q_OBJECT
public:
    static CfgConfig_Math * instance();

    QVariant getMathData(quint32 ID, quint32 group);
    void setMathData(quint32 ID, quint32 group);

    QString getMathObjStr(quint32 ID, quint32 group = 0);
    QString getMathObjValStr(quint32 ID, quint32 group, ConfigObject *cfg);

private:
    explicit CfgConfig_Math(QObject *parent = 0);

    QVariant getMathExpData(quint32 ID, quint32 group);
    QVariant getMathAlarmData(quint32 ID, quint32 group);
    QVariant getMathDispData(quint32 ID, quint32 group);
    QVariant getMathConstData(quint32 ID, quint32 group);
    QVariant getMathActionData(quint32 ID, quint32 group);

    QVariant getRollingAvgIntervalData(quint32 data);

    QString getMathExpObjStr(quint32 ID, quint32 group = 0);
    QString getMathAlarmObjStr(quint32 ID, quint32 group = 0);
    QString getMathDispObjStr(quint32 ID, quint32 group = 0);
    QString getMathConstObjStr(quint32 ID, quint32 group = 0);
    QString getMathActionObjStr(quint32 ID, quint32 group = 0);

    QString getMathExpObjValStr(quint32 ID, quint32 group, ConfigObject *cfg);
    QString getMathAlarmObjValStr(quint32 ID, quint32 group, ConfigObject *cfg);
    QString getMathDispObjValStr(quint32 ID, quint32 group, ConfigObject *cfg);
    QString getMathConstObjValStr(quint32 ID, quint32 group, ConfigObject *cfg);
    QString getMathActionObjValStr(quint32 ID, quint32 group, ConfigObject *cfg);

    QString getMathRangScaleUnit(float data);
    QString getScaleUnitValObjStr(QString unit);
    QString getAlarmTypeValObjStr(quint32 type);
    QString getRollingAvgInterObjValStr(quint32 data);

    QString getColorValueObjStr(quint32 x);
    QString getMsgNumVal(quint32 type);

    void setMathExpData(quint32 ID, quint32 group);
    void setMathAlarmData(quint32 ID, quint32 group);
    void setMathDispData(quint32 ID, quint32 group);
    void setMathConstData(quint32 ID, quint32 group);
    void setMathActionData(quint32 ID, quint32 group);

    quint32 setMathOutputNo(quint32 type, quint32 code);
    QString getMathChannelObjValStr(quint32 num);


    QVariant getCharToString(uchar *str);
    void setStrToChar(uchar * uData, QString str, int len);

    int getFloatToInt(float data, int pos);

signals:

public slots:

};

#endif // CFGCONFIG_MATH_H
