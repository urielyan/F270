/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：CfgGlobbalDef_Math.h
 * 概   要：运算通道配置相关数据结构体、枚举值定义
 *
 * 当前版本：V1.0.0
 * 作   者：王 品
 * 完成日期：2015-6-8
 *
 * 修改版本：
 * 修改说明：
 * 修改作者：
 *
 ********************************************************************/
#include <QtGlobal>
#include "cfgGlobalDef_Type.h"

#ifndef CFGGLOBALDEF_MATH_H
#define CFGGLOBALDEF_MATH_H

//运算溢出值显示定义
typedef enum Math_Error_Value_Enum {
    Math_Error_Value_Positive = 0,          //正溢出
    Math_Error_Value_Negative,              //负溢出
}Math_Error_Value_Enum;

//START/STOP键操作定义
typedef enum Math_Key_Action_Enum {
    Math_Key_Action_Off = 0,          //按下START/STOP键也不进行运算的停止和结束
    Math_Key_Action_Normal,         //按下START/STOP键运算开始或停止
    Math_Key_Action_Reset,            //按下START/STOP键运算开始或停止，开始前复位
}Math_Key_Action;

//运算溢出值显示定义
typedef enum Math_Over_Range_Enum {
    Math_Over_Range_Error = 0,      //错误：将溢出结果设定为运算错误
    Math_Over_Range_Limit,             //限制：用限制值替换溢出数据
    Math_Over_Range_Skip,              //不使用：放弃溢出数据，继续运算
    Math_Over_Range_Over,             //溢出：使用溢出数据进行运算
}Math_Over_Range;

/****************************************
 * MATH通道运算配置相关数据结构体定义
 ****************************************/
typedef struct MATH_CONFIG_EXP_STRUCT
{
    quint32 expSwtich;                                                         //运算通道开关
    quint32 expPos;                                                         //小数点位置：0/1/2/3/4/5,默认2
    qint32 expMin;                                                        //范围下限：-999999 ~ 999999, 默认0
    qint32 expMax;                                                       //范围上限：-999999 ~ 999999, 默认10000
    uchar   expUnit[8];                                                    //单位，字符串，最多6个半角字符
    uchar   expExpression[124];                                     //运算式，最多120个半角字符

    quint32 expTlogType;                                                //TLOG计时器类型， 0-计时器， 1-匹配计时器，默认0
    quint32 expTlogNo;                                                   //TLOG计时器号码，1~4
    quint32 expTlogSumScale;                                        //累计单位，off:0, 秒:1000， 分:60000， 时:3600000,单位ms
    quint32 expTlogReset;                                                //复位开关,0-off, 1-on

    quint32 expRollingAvgSwtich;                                   //移动平均开关,0-off, 1-on
    quint32 expRollingAvgInterval;                                 //移动平均采样间隔，测量周期的整数倍，单位：S，默认10s
    quint32 expRollingAvgNumber;                                //移动平均采样数，移动平均时间 = 采样间隔 * 采样数
}MATH_CONFIG_EXP;

/* MATH通道报警类型定义*/
//typedef enum MATH_Alarm_Type_Enum{                  //默认值:MATH_Alarm_Type_H
//    MATH_Alarm_Type_Off = 0,
//    MATH_Alarm_Type_H,                                              //上限
//    MATH_Alarm_Type_L ,                                              //下限
//    MATH_Alarm_Type_T,                                              //延迟上限
//    MATH_Alarm_Type_t,                                              //延迟下限
//} MATH_Alarm_Type;

/****************************************
 * MATH通道报警相关数据结构体定义
 ****************************************/
typedef struct MATH_CONFIG_ALARM_STRUCT
{
    quint8 alarmOnOff[4];                                          //报警开关, 0-off, 1-on
    quint8 alarmDelay[4];                                          //报警延迟时间，延迟上下限时有效，[0]-秒，[1]-分，[2]-时
    Alarm_Type   alarmType[4];                                  //报警类型
    qint32   alarmValue[4];                                            //报警值，-999999 ~ 999999，忽略小数点
    quint32   alarmHysteresis[4];                               //滞后，上/下限时有效，0~100000
    quint8 alarmLogging[4];                                      //检出, 0-off, 1-on
    quint8 alarmOutputType[4];                                //报警输出类型， 0-off, 1-Relay(继电器), 2-Switch(内部开关)
    quint32 alarmOuputNo[4];                                   //报警输出通道， Relay/Switches
}MATH_CONFIG_ALARM ;

/****************************************
 * MATH通道显示相关数据结构体定义
 ****************************************/
typedef struct MATH_CONFIG_DISPLAY_STRUCT
{
    quint32 displayColor;                                           //通道颜色，Byte2-Red, Byte1-Green, Byte0-Blue

    uchar   displayTagChar[32];                                 //标记字符串，中英支持，最多32个半角字符
    uchar   displayTagNo[16];                                    //标记号码，符号，最多16个半角字符

    quint8  displayZoneUpper;                                  //显示区域上限，5-100,单位%
    quint8  dispalyZoneLower;                                  //显示区域上限，0-95,单位%

    quint8  displayScalePos;                                     //标尺位置，1-10,默认1
    quint8  displayScaleDiv;                                      //标尺分割，4-13, 13:C10

    quint8  displayBarPos;                                        //棒图基准位置， 0-Lower, 1-Center, 2-Upper
    quint8  displayBarDiv;                                         //棒图分割数，4-12, 默认10

    //趋势设定的部分压缩放大功能打开时有效
    quint8  displayPartialOnOff;                                  //部分压缩放大开关
    quint8  displayPartialPos;                                   //部分压缩放大位置，1-99, 单位：%，默认50
    float  displayPartialBoundary;                           //部分压缩放大边界值

    // 棒图共用
    quint32  displayColorbandMode;                        //彩色标尺带模式， 0-off, 1-内侧， 2-外侧.
    float  displayColorbandPosUpper;                 //彩色标尺带显示位置上限
    float  displayColorbandPosLower;                 //彩色标尺带显示位置下限
    quint32 displayColorbandColor;                       //彩色标尺带颜色，Byte2-Red, Byte1-Green, Byte0-Blue

    // 棒图共用
    quint8  displayAlarmMarkOnOff;                         //报警标记开关，0-0ff, 1-on
    quint8  displayAlarmMarkType;                        //报警标记类型，0-报警， 1-固定
    quint8  RESERVED[2];
    quint32 displayAlarmMarkColor[4];                  //报警1-4颜色， Byte2-Red, Byte1-Green, Byte0-Blue
}MATH_CONFIG_DISPLAY ;

/****************************************
 * MATH通道定义
 ****************************************/
typedef struct MATH_CONFIG_STRUCT
{
    MATH_CONFIG_EXP              expressConfig;
    MATH_CONFIG_ALARM        alarmConfig;
    MATH_CONFIG_DISPLAY      displayConfig;
}MATH_CONFIG;

#endif // CFGGLOBALDEF_MATH_H
