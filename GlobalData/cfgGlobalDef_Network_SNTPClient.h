/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：CfgGlobbalDef_Network_SNTPClient.h
 * 概   要：网络--SNTP客户端配置相关数据结构体、枚举定义
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

#ifndef CFGGLOBALDEF_NETWORK_SNTPCLIENT_H
#define CFGGLOBALDEF_NETWORK_SNTPCLIENT_H

/****************************************
 * SNTP Client配置
 ****************************************/
typedef struct NETWORK_CONFIG_SNTPCLIENT_STRUCT
{
    quint8 clientOnOff;                               //SNTP功能开关，0-off, 1-on

    //查询操作
    quint8 queryInterval;                         //查询间隔，单位:h, 范围：6/12/24h
    quint8 queryTimeout;                        //查询超时时间，单位：s,范围：10/30/90s
    quint8 adjustAtStart;                          //记录开始时，进行对时，0-off, 1-on

    //SNTP服务器
    quint32 SNTPServerPort;                    //服务器端口号，1~65535,默认：123
    uchar   SNTPServerName[64];            //服务器名，最多64个半角字符


}NETWORK_CONFIG_SNTPCLIENT ;

#endif // CFGGLOBALDEF_NETWORK_SNTPCLIENT_H
