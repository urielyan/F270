/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：CfgGlobbalDef_COMM.h
 * 概   要：通信通道配置相关数据结构体、枚举值定义
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

#ifndef CFGGLOBALDEF_COMM_H
#define CFGGLOBALDEF_COMM_H

/****************************************
 * COMM通道量程配置相关数据结构体定义
 ****************************************/
typedef struct COMM_CONFIG_RANGE_STRUCT
{
    quint32 rangeSwtich;                                                //通信通道开关，0-off, 1-on, 默认0
    qint32 rangePos;                                                     //小数点位置：0/1/2/3/4/5,默认0
    qint32 rangeMin;                                                     //范围下限：-999999 ~ 999999, 默认0
    qint32 rangeMax;                                                    //范围上限：-999999 ~ 999999, 默认100
    uchar   rangeUnit[8];                                                 //单位，字符串，最多6个半角字符, 中英文支持

    quint32 rangePowerOnVal;                                      //电源接通时的值类型， 0-最终值，1-预设值，默认0
    float   rangePreVal;                                                  //预设值

    quint32  rangeTimerSwitch;                                     //监视时钟开关,0-off, 1-on
    quint32 rangeTimerInterval;                                   //监视器计时器时间，1-120s,默认30
    quint32 rangeTimerResetVal;                                 //计时器满值设定值类型，0-最终值，1-预设值。默认0
}COMM_CONFIG_RANGE;

/* COMM通道报警类型定义*/
//typedef enum COMM_Alarm_Type_Enum{                  //默认值:COMM__Alarm_Type_H
//    COMM_Alarm_Type_Off = 0,
//    COMM__Alarm_Type_H,                                             //上限
//    COMM__Alarm_Type_L ,                                              //下限
//    COMM__Alarm_Type_T,                                              //延迟上限
//    COMM__Alarm_Type_t,                                              //延迟下限
//} COMM_Alarm_Type;

/****************************************
 *  COMM通道报警相关数据结构体定义
 ****************************************/
typedef struct  COMM_CONFIG_ALARM_STRUCT
{
    quint8 alarmOnOff[4];                                          //报警开关, 0:off,  1:on
    quint8 alarmDelay[4];                                          //报警延迟时间，延迟上下限时有效，[0]-秒，[1]-分，[2]-时
    Alarm_Type alarmType[4];                                  //报警类型
    qint32 alarmValue[4];                                       //报警值，-999999 ~ 999999，忽略小数点
    quint32 alarmHysteresis[4];                               //滞后，上/下限时有效，0~100000
    quint8 alarmLogging[4];                                      //检出, 0-off, 1-on
    quint8 alarmOutputType[4];                              //报警输出类型， 0-off, 1-Relay(继电器), 2-Switch(内部开关)
    quint32 alarmOuputNo[4];                                  //报警输出通道， Relay/Switches
}COMM_CONFIG_ALARM ;

/****************************************
 * COMM通道显示相关数据结构体定义
 ****************************************/
typedef struct COMM_CONFIG_DISPLAY_STRUCT
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
    quint8  displayPartialOnOff;                              //部分压缩放大开关,0-off, 1-on
    quint8  displayPartialPos;                                   //部分压缩放大位置，1-99, 单位：%，默认50
    float  displayPartialBoundary;                         //部分压缩放大边界值

    // 棒图共用
    quint32  displayColorbandMode;                        //彩色标尺带模式， 0-off, 1-内侧， 2-外侧.
    float  displayColorbandPosUpper;                 //彩色标尺带显示位置上限
    float  displayColorbandPosLower;                 //彩色标尺带显示位置下限
    quint32 displayColorbandColor;                       //彩色标尺带颜色，Byte2-Red, Byte1-Green, Byte0-Blue

    // 棒图共用
    quint8  displayAlarmMarkOnOff;                       //报警标记开关，0-0ff, 1-on
    quint8  displayAlarmMarkType;                        //报警标记类型，0-报警， 1-固定
    quint8  RESERVED[2];
    quint32 displayAlarmMarkColor[4];                  //报警1-4颜色， Byte2-Red, Byte1-Green, Byte0-Blue
}COMM_CONFIG_DISPLAY ;

/****************************************
 * COMM通道定义
 ****************************************/
typedef struct COMM_CONFIG_STRUCT
{
    COMM_CONFIG_RANGE      rangeConfig;
    COMM_CONFIG_ALARM      alarmConfig;
    COMM_CONFIG_DISPLAY    displayConfig;
}COMM_CONFIG;

#endif // CFGGLOBALDEF_COMM_H
