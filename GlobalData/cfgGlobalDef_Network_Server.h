/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：CfgGlobbalDef_Network_Server.h
 * 概   要：网络服务器配置相关数据结构体、枚举定义
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

#ifndef CFGGLOBALDEF_NETWORK_SERVER_H
#define CFGGLOBALDEF_NETWORK_SERVER_H

#define MODBUS_CONNECT_LIMIT_MAX    10      //Modbus服务连接限制IP个数

/****************************************
 * Server配置:FTP, HTTP, SNTP, Modbus
 ****************************************/
typedef struct NETWORK_CONFIG_SERVER_STRUCT
{
    //FTP Server
    quint8 serverFTPOnOff;              //FTP服务器开关，0-off, 1-on
    quint8 serverFTPEncrypt;            //FTP加密开关， 0-off, 1-on
    quint8 serverFTPConnectHole;   //FTP服务器保持有效功能，0-off, 1-on

    //HTTP Server
    quint8 serverHTTPOnOff;     //HTTP服务开关，0-off, 1-on
    quint8 serverHTTPEncrypt;  //HTTP加密开关， 0-off, 1-on

    //SNTP Server
    quint8 serverSNTPOnOff;     //SNTP服务开关，0-off, 1-on

    //Modbus Server
    quint8 serverModbusOnOff;     //Modbus服务开关
    quint8 serverModbusTimeOut; //Modbus服务器延迟响应，单位：ms,范围：0(off)/10ms/20ms/50ms
    quint8 serverModbusLimit;       //Modbus服务连接限制开关
    quint8 serverModbusLimitSwitch[MODBUS_CONNECT_LIMIT_MAX];       //限制开关，
    quint8 RESERVED;
    quint8 serverModbusLimitIP[MODBUS_CONNECT_LIMIT_MAX][4];           //限制IP, IPv4

    quint16 serverFTPPort;           //FTP服务端口号, 默认21
    quint16 serverHTTPPort;         //HTTP服务端口号, 默认80
    quint16 serverSNTPPort;         //SNTP服务端口号, 默认123
    quint16 serverModbusPort;         //Modbus服务端口号, 默认502
}NETWORK_CONFIG_SERVER ;

#endif // CFGGLOBALDEF_NETWORK_SERVER_H
