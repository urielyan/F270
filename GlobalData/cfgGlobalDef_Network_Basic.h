/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：CfgGlobbalDef_Network_Basic.h
 * 概   要：网络基本配置相关数据结构体、枚举定义
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

#ifndef CFGGLOBALDEF_NETWORK_BASIC_H
#define CFGGLOBALDEF_NETWORK_BASIC_H

/****************************************
 * 网络配置基本设定
 ****************************************/
typedef struct NETWORK_CONFIG_BASIC_STRUCT
{
    quint8 autoIP;                    //自动获取IP，0-off, 1-on
    quint8 autoDNS;                //自动DNS，0-off, 1-on
    quint8 RESERVED[2];

    quint8 ipAddress[4];           //IPv4地址
    quint8 subnetMask[4];       //子网掩码
    quint8 defaultGateway[4]; //默认网关

    quint8 DNSAddrFirst[4];     //第一优先DNS服务器
    quint8 DNSAddrSecond[4];//第二优先DNS服务器
}NETWORK_CONFIG_BASIC ;

#endif // CFGGLOBALDEF_NETWORK_BASIC_H
