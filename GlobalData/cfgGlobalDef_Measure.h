/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：CfgGlobbalDef_Display.h
 * 概   要：画面显示条件配置相关数据结构、枚举值定义
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

#ifndef CFGGLOBALDEF_MEASURE_H
#define CFGGLOBALDEF_MEASURE_H

/*
 * 测量周期值定义
 * 1. 使用电磁继电器方式的模拟输入模块时，不可使用100ms/200ms/500ms
 * 2. 使用低耐压继电器方式的模拟输入模块时，不可使用100ms/200ms
 * 3. 电磁继电器方式的模拟输入模块，默认使用2s
 */
typedef enum Measure_Interval_Enum{                  //默认值:Measure_Interval_1s
    Measure_Interval_100ms = 100,
    Measure_Interval_200ms = 200,
    Measure_Interval_500ms = 500,
    Measure_Interval_1s = 1000,
    Measure_Interval_2s = 2000,
    Measure_Interval_5s = 5000,
} Measure_Interval;

/* 量程溢出值判定方式定义*/
typedef enum Measure_OverRange_Enum{
    Measure_OverRange_Free = 0, //自由：小于“量程范围”的-5%为负溢出，大于105%为正溢出
    Measure_OverRange_Over,      //溢出：小于“设定范围”的-5%为负溢出，大于105%为正溢出
}Measure_OverRange;

#endif // CFGGLOBALDEF_MEASURE_H
