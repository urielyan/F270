/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：CfgGlobbalDef_Network_ModbusClient.h
 * 概   要：网络--Modbus客户端配置相关数据结构体、枚举定义
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

#ifndef CFGGLOBALDEF_NETWORK_MODBUSCLIENT_H
#define CFGGLOBALDEF_NETWORK_MODBUSCLIENT_H

#define COMM_CMD_NETWORK_NUMBER 100     //Modbus通信命令数

/****************************************
 * Modbus Client配置
 ****************************************/
typedef struct NETWORK_CONFIG_MODBUSCLIENT_STRUCT
{
    quint8 clientOnOff;            //Modbus客户端功能开关，0-off, 1-on

    //连接
    quint8 connectHold;         //保持连接开关，0-off, 1-on
    quint8 connectTime;         //保持连接时间，单位：s,范围：1~10
    quint8 RESERVED;

    //通信
    quint32 commInterval;       //采样间隔，单位:ms,范围：100ms/200ms/500ms/1s/2s/5s/10s/20s/30s/1min,默认：1s

    //恢复动作
    quint32 recoveryTime;       //通信恢复等待时间，单位:s，范围：0(off)/5s/10s/30s/1min/2min/5min,默认:2min

    /* Modbus服务器：32个服务器*/
    uchar   serverName[32][64];     //服务器名，最多64个半角字符
    quint16 serverPort[32];             //服务器端口号，1~65535,默认：502

    /* 命令设定，最多100个命令*/
    quint8 cmdType[COMM_CMD_NETWORK_NUMBER];                   //命令类型：0-off, 1-读取， 2-写入
    quint8 cmdServeNo[COMM_CMD_NETWORK_NUMBER];            //命令对应的服务器号，范围:1~32
    quint8 cmdServeCell[COMM_CMD_NETWORK_NUMBER];           //单元号:1~255, Modbus网关功能时使用
    Register_Type cmdRegType[COMM_CMD_NETWORK_NUMBER];  //寄存器类型
    quint32 cmdRegNo[COMM_CMD_NETWORK_NUMBER];                //寄存器号
    Channel_Type   cmdChType[COMM_CMD_NETWORK_NUMBER];   //通道类型，读取时：通信通道；写入时：输入输出通道/运算通道/通信通道
    quint32 cmdStartNo[COMM_CMD_NETWORK_NUMBER];              //与通道类型匹配的起始通道号
    quint32 cmdEndNo[COMM_CMD_NETWORK_NUMBER];                //与通道类型匹配的结束通道号
}NETWORK_CONFIG_MODBUSCLIENT ;

#endif // CFGGLOBALDEF_NETWORK_MODBUSCLIENT_H
