/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：CfgGlobbalDef_Network_SMTPClient.h
 * 概   要：网络--SMTP客户端配置相关数据结构体、枚举定义
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

#ifndef CFGGLOBALDEF_NETWORK_SMTPCLIENT_H
#define CFGGLOBALDEF_NETWORK_SMTPCLIENT_H

typedef enum SMTP_Authentication_Type_Enum{ //默认：SMTP_Authentication_Type_Off
    SMTP_Authentication_Type_Off = 0,                   //off
    SMTP_Authentication_Type_Smtp,                     //SMTP认证
    SMTP_Authentication_Type_Pop,                       //POP before SMTP
    SMTP_Authentication_Type_Apop,                     //APOP
}SMTP_Authentication_Type;

/****************************************
 * SMTP Client配置
 ****************************************/
typedef struct NETWORK_CONFIG_SMTPCLIENT_STRUCT
{
    quint8 clientOnOff;                //SMTP功能开关，0-off, 1-on

    //加密
    quint8 encryptionSSL;              //SSL传输加密，0-off, 1-on
    quint8 encryptionVerify;          //证书验证加密，0-off, 1-on
    quint8 RESERVED;

    //用户认证
    SMTP_Authentication_Type    authType;   //用户认证方式

    //SMTP服务器
    quint32 SMTPServerPort;                    //服务器端口号，1~65535,默认：25
    uchar   SMTPServerName[64];            //服务器名，最多64个半角字符
    uchar   SMTPServerUserName[32];    //用户名，最多32个半角字符,【SMTP】认证时有效
    uchar   SMTPServerPasswd[32];         //密码，最多32个半角字符,【SMTP】认证时有效

    //POP3服务器
    quint32 POP3ServePort;                      //POP3服务器端口号，1~65535,默认：110
    uchar   POP3ServerName[64];            //服务器名，最多64个半角字符
    uchar   POP3ServerUserName[32];    //用户名，最多32个半角字符,【POP before STMP】/【APOP】时有效
    uchar   POP3ServerPasswd[32];         //密码，最多32个半角字符,【POP before STMP】/【APOP】时有效
}NETWORK_CONFIG_SMTPCLIENT ;

#endif // CFGGLOBALDEF_NETWORK_SMTPCLIENT_H
