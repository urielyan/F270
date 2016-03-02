/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：CfgGlobbalDef_EventAction.h
 * 概   要： 事件动作配置相关数据结构、枚举值定义，最多定义50个事件动作
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

#ifndef CFGGLOBALDEF_EVENTACTION_H
#define CFGGLOBALDEF_EVENTACTION_H

#define EVENT_ACTION_NUMBER 50          //事件动作数
#define EVENT_ACTION_NUMBER_PRIVATE 52  //事件动作内部定义个数，用于对齐

/* 事件类型定义*/
typedef enum  Event_Type_Enum{
    Event_Type_InterSwitch = 0,            //内部开关
    Event_Type_Remote,                         //远程控制输入DI
    Event_Type_Relay,                             //继电器
    Event_Type_Alarm_IO,                      //来自输入输出通道的报警
    Event_Type_Alarm_Math,                 //来自运算通道的报警
    Event_Type_Alarm_Comm,               //来自通信通道的报警
    Event_Type_Alarm_All,                      //任何报警
    Event_Type_Timer,                             //计时器
    Event_Type_Timer_Match,                //匹配计时器
    Event_Type_UserFunc,                      //用户功能键
    Event_Type_Status,                           //仪表状态
}Event_Type;

/* 操作模式类型定义*/
typedef enum Operation_Mode_Type_Enum{
    Operation_Mode_Type_RisingEdge = 0,      //上升沿
    Operation_Mode_Type_FallingEdge,           //下降沿
    Operation_Mode_Type_Both,                       //上升沿/下降沿
    Operation_Mode_Type_Edge,                     //边缘---【计时器/匹配计时器】模式才有效
}Operation_Mode_Type;

/* 动作类型定义-对应操作模式：上升沿\下降沿\边缘-*/
typedef enum Action_Type_Enum{
    Action_Type1_Recorded = 0,                      //记录
    Action_Type1_Computation,                      //运算
    Action_Type1_SwitchRate,                          //切换显示率
    Action_Type1_Flag,                                      //标志
    Action_Type1_ManualSample,                   //手动采样
    Action_Type1_AlarmAck,                             //解除报警
    Action_Type1_Snapshot,                             //画面存储
    Action_Type1_AdjustTime,                          //时间调整
    Action_Type1_SaveDispData,                      //保存显示数据
    Action_Type1_SaveEventData,                    //保存事件数据
    Action_Type1_EventTrigger,                        //事件触发
    Action_Type1_Message,                               //信息
    Action_Type1_SwitchGroup,                       //切换显示组
    Action_Type1_TimerReset,                          //计时器复位
    Action_Type1_LoadSetting,                        //读入设定数据
    Action_Type1_HomeWin,                            //显示home界面
//    Action_Type1_Relay,                                   //继电器
//    Action_Type1_InternalSwitch,                    //内部开关
}Action_Type;

/* 动作类型2动作on/off定义-对应操作模式：上升沿/下降沿*/
typedef enum Action_Type2_Enum{
    Action_Type2_Record_Start_Stop = 0,         //记录开始/停止
    Action_Type2_Computation_Start_Stop,    //运算开始/停止
    Action_Type2_SwitchRate_1_2,                    //切换显示率1/2
    Action_Type2_Flag_On_Off,                          //标志1-on, 0-off
//    Action_Type2_Relay_On_Off,                       //继电器on/off
//    Action_Type2_InternalSwitch_On_Off,        //内部开关on/off
}Action_Type2;

/* 动作细节定义*/
typedef enum Action_Detail_Enum{
    Action_Detail_Start = 0,            //开始
    Action_Detail_Stop,                  //停止
    Action_Detail_Reset,                 //复位
    Action_Detail_AllGrp,                //所有组
    Action_Detail_SpecGrp,            //指定组
//    Action_Detail_On,                     //on
//    Action_Detail_Off,                      //off
}Action_Detail;

/****************************************
 * 事件动作配置相关数据结构体定义
 ****************************************/
typedef struct EVENT_ACTION_CONFIG_STRUCT
{
    quint8 eventActionSwitch[EVENT_ACTION_NUMBER_PRIVATE];                     //事件动作开关，0-off, 1-on

    /*事件*/
    Event_Type   eventType[EVENT_ACTION_NUMBER_PRIVATE];                        //事件动作类型
    quint32 eventNumber[EVENT_ACTION_NUMBER_PRIVATE];                         //事件动作类型对应的号码，【所有报警】时无效
    quint8 eventAlarmNumber[EVENT_ACTION_NUMBER_PRIVATE];                  //报警号，范围：1~4，【报警-IO/报警-Math/报警-Comm】时有效
    Device_Status_Type  eventStatus[EVENT_ACTION_NUMBER_PRIVATE];          //仪表状态类型，在【仪表状态】类型时有效
    Operation_Mode_Type eventOpeMode[EVENT_ACTION_NUMBER_PRIVATE];    //操作模式

    /*动作*/
//    Action_Type1   activeType1[EVENT_ACTION_NUMBER_PRIVATE];        //动作类型1, 操作模式：上升沿\下降沿\边缘时有效
//    Action_Type2   activeType2[EVENT_ACTION_NUMBER_PRIVATE];        //动作类型2，操作模式：上升沿/下降沿时有效
    Action_Type     activeType[EVENT_ACTION_NUMBER_PRIVATE];          /*动作类型, 操作模式：上升沿\下降沿\边缘时按Action_Type
                                                                                                                            操作模式：上升沿/下降沿时有效按Action_Type2*/

    quint32 activeNo[EVENT_ACTION_NUMBER_PRIVATE];                  //动作号码，具体范围依赖于动作类型
    Action_Detail   activeDeatil[EVENT_ACTION_NUMBER_PRIVATE];   //动作细节，具体可分配值依赖于动作类型
    quint32 actionGroupNo[EVENT_ACTION_NUMBER_PRIVATE];       //组号码，【信息】类型且详细信息为【指定组】时有效
}EVENT_ACTION_CONFIG ;

#endif // CFGGLOBALDEF_EVENTACTION_H
