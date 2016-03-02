/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：CfgGlobbalDef_Record.h
 * 概   要：记录条件配置相关数据结构、枚举值定义
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

#ifndef CFGGLOBALDEF_RECORD_H
#define CFGGLOBALDEF_RECORD_H

/* 记录模式定义*/
typedef enum Record_Mode_Enum{                  //默认值:Record_Mode_Display
    Record_Mode_Display = 0,                              //显示：记录显示数据
    Record_Mode_Event ,                                      //事件：记录事件数据
    Record_Mode_Both,                                        //显示+事件：记录显示+事件数据
} Record_Mode;

/*
 * 保存周期定义, 单位：min
 * 1.保存周期在记录模式为事件时无效
 * 2.保存周期最小值取决于显示数据的趋势周期和记录通道数
 */
typedef enum Display_Record_DataLength_Enum{
    Display_Record_DataLength_10min = 10,
    Display_Record_DataLength_20min = 20,
    Display_Record_DataLength_30min = 30,
    Display_Record_DataLength_1h = 60,
    Display_Record_DataLength_2h = 120,
    Display_Record_DataLength_3h = 180,
    Display_Record_DataLength_4h = 240,
    Display_Record_DataLength_6h = 360,
    Display_Record_DataLength_8h = 480,
    Display_Record_DataLength_12h = 720,
    Display_Record_DataLength_1day = 1440,
    Display_Record_DataLength_2day = 2880,
    Display_Record_DataLength_3day = 4320,
    Display_Record_DataLength_5day = 7200,
    Display_Record_DataLength_7day = 10080,
    Display_Record_DataLength_14day = 20160,
    Display_Record_DataLength_31day = 44640,
}Display_Record_DataLength;

/* 事件数据记录周期定义, 单位：ms*/
typedef enum Event_Record_Interval_Enum{
    Event_Record_Interval_100ms = 100,
    Event_Record_Interval_200ms = 200,
    Event_Record_Interval_500ms = 500,
    Event_Record_Interval_1s = 1000,
    Event_Record_Interval_2s = 2000,
    Event_Record_Interval_5s = 5000,
    Event_Record_Interval_10s = 10000,
    Event_Record_Interval_15s = 15000,
    Event_Record_Interval_20s = 20000,
    Event_Record_Interval_30s = 30000,
    Event_Record_Interval_1min = 60000,
    Event_Record_Interval_2min = 120000,
    Event_Record_Interval_5min = 300000,
    Event_Record_Interval_10min = 600000,
    Event_Record_Interval_15min = 900000,
    Event_Record_Interval_20min = 1200000,
    Event_Record_Interval_30min = 1800000,
}Event_Record_Interval;

/* 事件数据记录模式定义*/
typedef enum Event_Record_Mode_Enum{
    Event_Record_Mode_Free = 0,       //自由
    Event_Record_Mode_Single ,          //单次
    Event_Record_Mode_Loop ,           //循环
}Event_Record_Mode;

/* 事件数据记录数据长度定义, 单位：min*/
typedef enum Event_Record_DataLength_Enum{
    Event_Record_DataLength_10min = 10,
    Event_Record_DataLength_20min = 20,
    Event_Record_DataLength_30min = 30,
    Event_Record_DataLength_1h = 60,
    Event_Record_DataLength_2h = 120,
    Event_Record_DataLength_3h = 180,
    Event_Record_DataLength_6h = 360,
    Event_Record_DataLength_8h = 480,
    Event_Record_DataLength_12h = 720,
    Event_Record_DataLength_1day = 1440,
    Event_Record_DataLength_2day = 2880,
    Event_Record_DataLength_3day = 4320,
    Event_Record_DataLength_5day = 7200,
    Event_Record_DataLength_7day = 10080,
    Event_Record_DataLength_14day = 20160,
    Event_Record_DataLength_31day = 44640,
}Event_Record_DataLength;

/****************************************
 * 记录条件相关数据结构体定义
 ****************************************/
//typedef struct RECORD_CONFIG_STRUCT
//{
//    Record_Mode    recordMode;                                                          //记录模式

//    //显示数据
//    Display_Record_DataLength  recordDisplaySaveInterval;      //显示数据保存周期
//    quint32 recordDisplayChannelNum;            //显示数据记录通道数
//    quint32 recordDisplayChannelItem[500];   //显示数据记录通道号

//    //事件数据
//    Event_Record_Interval    recordEventSaveInterval;         //事件数据记录周期
//    Event_Record_Mode        recordEventSaveMode;            //事件数据记录模式
//    Event_Record_DataLength  recordEventSaveLength;      //事件数据保存长度
//    quint32 recordEventPrevTrigger;        //事件数据前置触发位置，单位：%，范围：0/5/25/50/75/95/100, 【自由】模式外有效
//    quint32 recordEventTriggerSrc;          //触发源操作，0-off, 1-on， 【自由】模式外有效
//    quint32 recordEventChannelNum;            //事件数据记录通道数
//    quint32 recordEventChannelItem[500];   //事件数据记录通道号

//    //确认画面
//    quint32 recordConfirmMsgBox;        //确认画面开关，0-off, 1-on
//    quint32 recordManualChannelNum;         //手动采样记录通道数
//    quint32 recordManualChannelItem[50];   //手动采样记录通道号
//}RECORD_CONFIG ;

#endif // CFGGLOBALDEF_RECORD_H
