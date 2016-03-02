/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：CfgGlobbalDef_Timer.h
 * 概   要：定时器/计时器配置相关数据结构体、枚举值定义；分别包括4个计时器和匹配
 * 计时器。
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

#ifndef CFGGLOBALDEF_TIMER_H
#define CFGGLOBALDEF_TIMER_H

/* 定时器类型定义*/
typedef enum    Timer_Type_Enum{
    Timer_Type_Off = 0,
    Timer_Type_Relative,
    Timer_Type_Absolute,
}Timer_Type;


/* 匹配计时器类型定义*/
typedef enum    Timer_Match_Type_Enum{
    Timer_Match_Type_Off = 0,
    Timer_Match_Type_Daily,
    Timer_Match_Type_Weekly,
    Timer_Match_Type_Monthly,
    Timer_Match_Type_Yearly,
}Timer_Match_Type;

/****************************************
 * 定时器/计时器配置结构体定义
 ****************************************/
typedef struct TIMER_CONFIG_STRUCT
{
    //计时器1~4
    Timer_Type timerType[4];         //计时器1-4类型
    quint8 timerDay[4];              //计时器1-4天数间隔，0-31
    quint8 timerHour[4];            //计时器1-4小时间隔，0-24
    quint8 timerMin[4];              //计时器1-4分钟间隔，0-60
    qint32 timerInter[4];            //时间间隔，绝对时间下使用，单位:min (1min - 24h)

    //匹配计时器1-4
    Timer_Match_Type    timerMatchType[4];      //匹配计时器1-4类型
    quint8 timerMatchMonth[4];                        //匹配计时器1-4匹配月，范围:1~12
    quint8 timerMatchDay[4];                             //匹配计时器1-4匹配天，范围：1~31
    quint8 timerMatchWeekDay[4];                    //匹配计时器1-4匹配星期几，范围：1~7
    quint8 timerMatchHour[4];                           //匹配计时器1-4匹配小时，范围：0~23
    quint8 timerMatchMinute[4];                        //匹配计时器1-4匹配分钟，范围：0~59
    quint8 timerMatchAction[4];                         //匹配计时器1-4动作，0-单次， 1-循环，其它：单次下已执行
}TIMER_CONFIG ;
#endif // CFGGLOBALDEF_TIMER_H
