/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：CfgGlobbalDef_DO.h
 * 概   要：数字板卡类型数字输出通道配置相关数据结构体、枚举值定义
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

#ifndef CFGGLOBALDEF_DO_H
#define CFGGLOBALDEF_DO_H

/****************************************
 * DO通道配置相关数据结构体定义
 ****************************************/
typedef struct DO_CONFIG_RANGE_STRUCT
{
    quint8 rangeType;                                                       //量程类型, 0-报警， 1-手动
    quint8 rangeMin;                                                         //范围下限：0/1，上下限值不能相同
    quint8 rangeMax;                                                        //范围上限：0/1，上下限值不能相同
    quint8 rangeDispMin;                                                 //范围下限：0/1，上下限值不能相同
    quint8 rangeDispMax;                                                //范围上限：0/1，上下限值不能相同
    quint8 RESERVED[3];
    uchar   rangeUnit[8];                                                    //单位，字符串，最多6个半角字符
}DO_CONFIG_RANGE;

/****************************************
 * DO通道动作相关数据结构体定义,
 * 量程类型为手动时无效！
 ****************************************/
typedef struct DO_CONFIG_ACTION_STRUCT
{
    quint8 actionEnergize;                                                //励磁/非励磁， 0-励磁， 1-非励磁
    quint8 actionMode;                                                     //动作，0-或， 1-与， 2-再故障再报警
    quint8 actionACK;                                                       //报警接触动作，0-标准，1-复位
    quint8 actionHold;                                                      //保持：0-保持，1-非保持 **再故障再报警时无效**
    quint32 actionDeactiveInterval;                                  //继电器非动作间隔，500ms/1s/2s**再故障再报警时有效**
}DO_CONFIG_ACTION;

/****************************************
 * DO通道显示相关数据结构体定义
 ****************************************/
typedef struct DO_CONFIG_DISPLAY_STRUCT
{
    quint32 displayColor;                                           //通道颜色，Byte2-Red, Byte1-Green, Byte0-Blue

    uchar   displayTagChar[32];                                 //标记字符串，中英支持，最多32个半角字符
    uchar   displayTagNo[16];                                    //标记号码，符号，最多16个半角字符

    quint8  displayZoneUpper;                                  //显示区域上限，5-100,单位%
    quint8  dispalyZoneLower;                                  //显示区域上限，0-95,单位%

    quint8  displayScalePos;                                     //标尺位置，1-10,默认1

    quint8  displayBarPos;                                        //棒图基准位置， 0-Lower, 1-Center, 2-Upper

    //0/1测量值对应显示字符串
    uchar   displayDI0[10];                                        //最多8个半角字符，中英文
    uchar   displayDI1[10];                                        //最多8个半角字符，中英文
}DO_CONFIG_DISPLAY ;

/****************************************
 * DI通道定义
 ****************************************/
typedef struct DO_CONFIG_STRUCT
{
    DO_CONFIG_RANGE             rangeConfig;
    DO_CONFIG_ACTION            activeConfig;
    DO_CONFIG_DISPLAY           displayConfig;
}DO_CONFIG;

#endif // CFGGLOBALDEF_DO_H
