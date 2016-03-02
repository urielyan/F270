/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：CfgGlobbalDef_DI.h
 * 概   要：数字板卡类型数字输入通道配置相关数据结构体、枚举值定义
 *
 * 当前版本：V1.0.0
 * 作   者：王 品
 * 完成日期：2015-6-4
 *
 * 修改版本：
 * 修改说明：
 * 修改作者：
 *
 ********************************************************************/
#include <QtGlobal>
#include "cfgGlobalDef_Type.h"

#ifndef CFGGLOBALDEF_DI_H
#define CFGGLOBALDEF_DI_H

/* DI模块操作模式定义*/
typedef enum DI_OpeMode_Enum {
    DI_OpeMode_STD = 0,              //标准
    DI_OpeMode_Remote,              //远程控制输入
}DI_OpeMode;

/* DI通道量程类型定义*/
typedef enum DI_Range_Type_Enum{                                  //默认值:DI_Range_Type_Disabled
    DI_Range_Type_Disabled = 0,                                            //不使用，即不测量
    DI_Range_Type_DI,                                                             //将接点或电压输入显示为对应显示范围的0%和100%
    DI_Range_Type_Pluse,                                                        //脉冲计数
} DI_Range_Type;

/* DI通道运算方式定义*/
typedef enum DI_Range_Calculate_Enum{             //默认值:DI_Range_Calculate_Disabled
    DI_Range_Calculate_Disabled = 0,                       //off
    DI_Range_Calculate_Delta,                                  //与基准通道测量值的差值，作为该通道的测量值
    DI_Range_Calculate_Linear,                                //线性转换以获取测量值
} DI_Range_Calculate;

/* 标尺数据结构定义*/
typedef struct DI_RANGE_SCALE_STRUCT
{
    quint32    scalePosition;                                          //小数点位置, 0/1/2/3/4/5，默认2
    qint32      scaleMin;                                                  //标尺下限，-999999 ~ 999999，默认0, 即0.00
    qint32      scaleMax;                                                 //标尺上限 , -999999 ~ 999999, 默认10000,即100.00
    uchar       scaleUnit[8];                                            //单位，字符串，最多6个半角字符
}DI_RANGE_SCALE;

/****************************************
 * DI通道量程配置相关数据结构体定义
 ****************************************/
typedef struct DI_CONFIG_RANGE_STRUCT
{
    DI_Range_Type type;                                                    //量程类型
    qint32  rangeMin;                                                         //范围下限
                                                                                           //【DI 】: 0/1, Delta运算时：-1 ~ 1, 默认0
                                                                                           //【脉冲】：0 ~ 999999, 默认0
    qint32  rangeMax;                                                        //范围上限
                                                                                           //【DI】：0/1, Delta运算时：-1~1,默认1
                                                                                           //【脉冲】：0 ~ 999999, 默认1250
    qint32  rangeDispMin;
    qint32  rangeDispMax;

    DI_Range_Calculate    calculateType;                         //通道运算类型
    quint32    basicChannel;                                             //基础通道索引值，Delta运算方式时使用
    DI_RANGE_SCALE    scale;                                           //标尺配置，在【线性缩放】运算方式时有效
}DI_CONFIG_RANGE;

/* DI通道报警类型定义*/
//typedef enum DI_Alarm_Type_Enum{                  //默认值:DI_Alarm_Type_H
//    DI_Alarm_Type_Off = 0,
//    DI_Alarm_Type_H,                                              //上限
//    DI_Alarm_Type_L ,                                              //下限
//    DI_Alarm_Type_R,                                              //变化率上升限
//    DI_Alarm_Type_r,                                               //变化率下升限
//    DI_Alarm_Type_T,                                              //延迟上限
//    DI_Alarm_Type_t,                                              //延迟下限
//    DI_Alarm_Type_h,                                             //差值上限， Delta运算时有效
//    DI_Alarm_Type_l,                                              //差值下限， Delta运算时有效
//} DI_Alarm_Type;

/****************************************
 * DI通道报警相关数据结构体定义
 ****************************************/
typedef struct DI_CONFIG_ALARM_STRUCT
{
    quint8 alarmOnOff[4];                                     //报警开关, 0:off,  1:on
    quint8 alarmDelay[4];                                       //报警延迟时间，延迟上下限时有效，[0]-秒，[1]-分，[2]-时
    Alarm_Type   alarmType[4];                              //报警类型
    qint8 alarmValue[4];                                         //报警值, H/L/T/t: 0/1, R/r: 固定1, h/l: -1/0/1
    qint8 alarmHysteresis[4];                                 //滞后，固定为0
    quint8 alarmLogging[4];                                //检出， 0-off, 1-on
    qint8 alarmOutputType[4];                             //报警输出类型, 0-off, 1-Relay(继电器), 2-Switch(内部开关)
    quint32 alarmOuputNo[4];                              //报警输出通道， Relay/Switches
}DI_CONFIG_ALARM ;

/****************************************
 * DI通道显示相关数据结构体定义
 ****************************************/
typedef struct DI_CONFIG_DISPLAY_STRUCT
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

    // 棒图共用
    quint8  displayAlarmMarkOnOff;                    //报警标记开关，0-0ff, 1-on
    quint8  displayAlarmMarkType;                        //报警标记类型，0-报警， 1-固定
    quint32 displayAlarmMarkColor[4];                  //报警1-4颜色， Byte2-Red, Byte1-Green, Byte0-Blue

    //0/1测量值对应显示字符串
    uchar   displayDI0[10];                                        //最多8个半角字符，中英文
    uchar   displayDI1[10];                                        //最多8个半角字符，中英文
}DI_CONFIG_DISPLAY ;

/****************************************
 * DI通道定义
 ****************************************/
typedef struct DI_CONFIG_STRUCT
{
    DI_CONFIG_RANGE             rangeConfig;
    DI_CONFIG_ALARM             alarmConfig;
    DI_CONFIG_DISPLAY           displayConfig;
}DI_CONFIG;

#endif // CFGGLOBALDEF_DI_H
