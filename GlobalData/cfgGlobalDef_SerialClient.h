/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：CfgGlobbalDef_Network_SerialClient.h
 * 概   要：串口通信配置相关数据结构体、枚举定义
 *
 * 当前版本：V1.0.0
 * 作   者：王 品
 * 完成日期：2015-6-9
 *
 * 修改版本：
 * 修改说明：
 * 修改作者：
 *
 ********************************************************************/
#include <QtGlobal>

#include "cfgGlobalDef_Type.h"

#ifndef CFGGLOBALDEF_SERIALCLIENT_H
#define CFGGLOBALDEF_SERIALCLIENT_H

#define COMM_CMD_SERIAL_NUMBER 100     //Modbus串口通信命令数

/* 串口工作模式定义*/
typedef enum   Serial_Mode_Enum{
    Serial_Mode_Off = 0,                        //不使用
    Serial_Mode_STD,                             //标准模式
    Serial_Mode_Modbus_Master,        //Modbus主机功能
    Serial_Mode_Modbus_Slave,           //Modbus从机功能
}Serial_Mode_Enum;

/* 串口波特率*/
typedef enum    Serial_BaudRate_Enum{
    Serial_BaudRate_1200 = 1200,
    Serial_BaudRate_2400 = 2400,
    Serial_BaudRate_4800 = 4800,
    Serial_BaudRate_9600 = 9600,
    Serial_BaudRate_19200 = 19200,
    Serial_BaudRate_38400 = 38400,
    Serial_BaudRate_57600 = 57600,
    Serial_BaudRate_115200 = 115200,
}Serial_BaudRate;

/* 串口校验*/
typedef enum    Serial_Parity_Enum{
    Serial_Parity_NoParity = 0,     //校验关闭
    Serial_Parity_EvenParity ,       //偶校验
    Serial_Parity_OddParity ,        //奇校验
}Serial_Parity;

/* 串口停止位*/
typedef enum    Serial_StopBits_Enum{
    Serial_StopBits_1 = 0,
    Serial_StopBits_2 ,
}Serial_StopBits;

/* 串口数据长度*/
typedef enum    Serial_DataLen_Enum{
    Serial_DataLen_7 = 0,     //校验关闭
    Serial_DataLen_8 ,       //偶校验
}Serial_DataLen;

/****************************************
 * Modbus Serial Client配置
 ****************************************/
typedef struct SERIAL_CONFIG_MODBUSCLIENT_STRUCT
{
    /*串口基本配置*/
    Serial_Mode_Enum    serialMode;     //串口工作模式
    quint32                       serialAddr;     //串口Modbus地址

    Serial_BaudRate         serialBaudrate; //波特率
    Serial_Parity                serialParity;      //校验
    Serial_StopBits            serialStopBit;   //停止位
    Serial_DataLen            serilaDataLen; //数据长度

    /*串口Modbus主机（客户端）功能*/
    quint8 clientOnOff;            //Modbus客户端功能开关
    quint8 RESERVED[3];

    //通信
    quint32 commInterval;       //采样间隔，单位:ms,范围：100ms/200ms/500ms/1s/2s/5s/10s/20s/30s/1min,默认：1s
    quint32 commTimeOut;     //通信超时，单位:ms,范围：100ms/200ms/250ms/500ms/1s/2s/5s/10s/1min,默认：1s
    quint32 commCmdInterval;    //命令间隔，单位：ms，范围：0(off)/5ms/10ms/20ms/50ms/100ms,默认：off

    //恢复动作
    quint32 recoveryCount;      //重试次数，范围：0(off)/1/2/3/4/5/10/20, 默认：1
    quint32 recoveryTime;       //通信恢复等待时间，单位:s，范围：0(off)/5s/10s/30s/1min/2min/5min,默认:5s

    /* 命令设定，最多100个命令*/
    quint8 cmdType[COMM_CMD_SERIAL_NUMBER];                   //命令类型：0-off, 1-读取， 2-写入
    quint8 cmdServeNo[COMM_CMD_SERIAL_NUMBER];            //命令对应的服务器(从机）号，范围:1~247,默认1
    Register_Type cmdRegType[COMM_CMD_SERIAL_NUMBER];  //寄存器类型
    quint32 cmdRegNo[COMM_CMD_SERIAL_NUMBER];                //寄存器号
    Channel_Type   cmdChType[COMM_CMD_SERIAL_NUMBER];   //通道类型，读取时：通信通道；写入时：输入输出通道/运算通道/通信通道
    quint32 cmdStartNo[COMM_CMD_SERIAL_NUMBER];              //与通道类型匹配的起始通道号
    quint32 cmdEndNo[COMM_CMD_SERIAL_NUMBER];                //与通道类型匹配的结束通道号
}SERIAL_CONFIG_MODBUSCLIENT ;

#endif // CFGGLOBALDEF_SERIALCLIENT_H
