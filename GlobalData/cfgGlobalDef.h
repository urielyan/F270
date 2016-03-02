/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：CfgGlobbalDef.h
 * 概   要：设备配置结构体、宏、枚举值定义
 *
 * 当前版本：V1.0.0
 * 作   者：王 品
 * 完成日期：2015-6-10
 *
 * 修改版本：
 * 修改说明：
 * 修改作者：
 *
 ********************************************************************/
#include <QtGlobal>

#include "GlobalData/globalDef.h"

#include "cfgGlobalDef_Measure.h"
#include "cfgGlobalDef_Record.h"
#include "cfgGlobalDef_Display.h"
#include "cfgGlobalDef_DataSave.h"
#include "cfgGlobalDef_Batch.h"
#include "cfgGlobalDef_Report.h"
#include "cfgGlobalDef_Timer.h"
#include "cfgGlobalDef_EventAction.h"
#include "cfgGlobalDef_Network_Basic.h"
#include "cfgGlobalDef_Network_FTPClient.h"
#include "cfgGlobalDef_Network_SMTPClient.h"
#include "cfgGlobalDef_Network_Email.h"
#include "cfgGlobalDef_Network_SNTPClient.h"
#include "cfgGlobalDef_Network_ModbusClient.h"
#include "cfgGlobalDef_Network_Server.h"
#include "cfgGlobalDef_SerialClient.h"
#include "cfgGlobalDef_SysEnvironment.h"
#include "cfgGlobalDef_Trend.h"
#include "cfgGlobalDef_Security.h"

#include "dataGlobalDef_SysStatus.h"
#include "dataGlobalDef_Record.h"

#include "cfgGlobalDef_AI.h"
#include "cfgGlobalDef_DI.h"
#include "cfgGlobalDef_DO.h"
#include "cfgGlobalDef_Math.h"
#include "cfgGlobalDef_Comm.h"

#ifndef CFGGLOBALDEF_H
#define CFGGLOBALDEF_H

#define TREND_SPACE 20                 //趋势模块上下空间像素
#define  SLIDING_TIME 500              //滑屏时间间隙

typedef struct DEVICE_CONFIG_STRUCT{
    quint32                     productMagic;
    /***********************测量配置**********************/
    Measure_Interval            measureInterval_ms;         //测量周期
    Measure_OverRange           measureOverRange;           //量程溢出判定

    /*********************记录配置************************/
    Record_Mode                 recordMode;                 //数据记录模式
    quint8                      recordConfirm;              //记录画面确认，0-off, 1-on
    //事件数据相关
    quint8                      eventSingleFlag;            //单次触发执行标记
    quint8                      eventTriggered;             //触发标记
    quint8                      eventRecordTriggerSrc;      //触发源操作，0-off, 1-on， 通过画面操作触发，【自由】模式外有效
    Event_Record_Interval       eventRecordInterval_ms;     //事件记录周期
    Event_Record_Mode           eventRecordMode;            //事件记录模式
    Event_Record_DataLength     eventRecordLength;          //事件记录数据长度
    quint16                     eventRecordPreTrigger;      //事件前置触发(%),范围:0/5/25/50/75/95/10, 【自由】模式外有效
    quint16                     eventRecordChNum;           //事件记录通道数，Max:500
    quint32                     eventRecordChItem[500];     //事件记录通道号
    //显示数据相关
    Display_Record_DataLength   displayRecordLength;        //显示记录数据长度
    quint32                     displayRecordChNum;         //显示记录通道数，Max:500
    quint32                     displayRecordChItem[500];   //显示记录通道号
    //手动采样
    quint32                     manualSampleChNum;          //手动采样通道数，Max:50
    quint32                     manualSampleChItem[50];     //手动采样通道号

    //组自动切换开关
    quint8                      trendAutoGroupChange;       //趋势组自动切换开关，0-off, 1-on
    quint8                      digitAutoGroupChange;       //数字组自动切换开关，0-off, 1-on
    quint8                      barAutoGroupChange;         //棒图组自动切换开关，0-off, 1-on
    quint8                      Reserved_Group;             //保留

    /***********************显示设置***********************/
    DISPLAY_GONFIG_TREND        displayTrend;                   //显示配置之趋势周期设置
    DISPLAY_GONFIG_GROUP        displayGroup[GROUP_NUM_MAX];    //显示配置之组设置
    DISPLAY_GONFIG_PRE_MESSAGE  displayMessage;                 //显示配置之预定义信息
    DISPLAY_GONFIG_BASIC        displayBasic;                   //显示配置之画面基本设定

    /********************数据保存设置***********************/
    DATASAVE_CONFIG             dataSaveConfig;                   //数据保存设置

    /********************批处理设置***********************/
    BATCH_CONFIG                batchConfig;                    //批处理相关设置

    /********************报表设置***********************/
    REPORT_CONFIG               reportConfig;                   //报表设置

    /********************计时器设置*********************/
    TIMER_CONFIG                timerConfig;                    //计时器设置

    /********************事件动作设置*********************/
    EVENT_ACTION_CONFIG         eventActiveConfig;              //事件动作配置

    /**********************网络设置*********************/
    NETWORK_CONFIG_BASIC            networkBasic;               //网络基本设置
    NETWORK_CONFIG_FTPCLIENT        networkFTP;                 //网络FTP客户端设置
    NETWORK_CONFIG_SMTPCLIENT       networkSTMP;                //SMTP客户端设置
    NETWORK_CONFIG_EMAIL            networkEmail;               //邮件相关设置
    NETWORK_CONFIG_SNTPCLIENT       networkSNTP;                //SNTP设定
    NETWORK_CONFIG_MODBUSCLIENT     networkModbus;              //Modbus客户端设定
    NETWORK_CONFIG_SERVER           networkServer;              //网络服务器相关设定

    /**********************串口设置*********************/
    SERIAL_CONFIG_MODBUSCLIENT      serialConfig;               //串口配置

    /*****************系统整体环境配置******************/
    SYS_CONFIG_ENVRIONMENT          systemConfig;               //系统整体配置

    /********************* 通道配置*********************/
    AI_CONFIG               mainUnitAI[MAIN_UNIT_MODULE_NUM][UNIT_CHAN_NUM_PRE];
    AI_CONFIG               extUnitAI[EXT_UNIT_NUM][EXT_UNIT_MODULE_NUM][UNIT_CHAN_NUM_PRE];
    DI_CONFIG               mainUnitDI[MAIN_UNIT_MODULE_NUM][UNIT_CHAN_NUM_PRE];
    DI_CONFIG               extUnitDI[EXT_UNIT_NUM][EXT_UNIT_MODULE_NUM][UNIT_CHAN_NUM_PRE];
    DO_CONFIG               mainUnitDO[MAIN_UNIT_MODULE_NUM][UNIT_CHAN_NUM_PRE];
    DO_CONFIG               extUnitDO[EXT_UNIT_NUM][EXT_UNIT_MODULE_NUM][UNIT_CHAN_NUM_PRE];
    MATH_CONFIG             chanMath[MAIN_UNIT_MATH_CHAN_NUM_PRE];
    COMM_CONFIG             chanComm[MAIN_UNIT_COMM_CHAN_NUM_PRE];

    //运算通道相关配置
    float                       mathConstK[100];        //运算式常数
    Math_Error_Value_Enum       mathErrorVal;           //运算错误显示值
    Math_Key_Action             mathKeyAction;          //START/STOP键动作设定
    Math_Over_Range             mathSumAve;             //SUM, AVE计算溢出处理
    Math_Over_Range             mathMaxMinPP;           //MAX, MIN, P-P计算溢出处理

    /********************系统状态**********************/
    SYSTEM_STATUS               sysStatus;              //设备状态

    /********************安全设定配置**********************/
    SECURITY_CONFIG             securityConfig;         //安全配置

    quint32                     DIRemoteModule;         //DI远程输入卡槽 16-8-8: skip-unit-module
}DEVICE_CONFIG;

#endif // CFGGLOBALDEF_H
