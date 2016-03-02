/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：CfgGlobbalDef_Network_FTPClient.h
 * 概   要：网络--FTP客户端配置相关数据结构体、枚举定义
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

#ifndef CFGGLOBALDEF_NETWORK_FTPCLIENT_H
#define CFGGLOBALDEF_NETWORK_FTPCLIENT_H

/****************************************
 * FTP Client配置
 ****************************************/
typedef struct NETWORK_CONFIG_FTPCLIENT_STRUCT
{
    quint8 clientOnOff;                //FTP功能开关

    //传送文件，在FTP功能on时有效， 0-off, 1-on
    quint8 dataDisplayEvent;        //显示&事件数据
    quint8 dataReport;                  //报表数据
    quint8 dataManualSample;    //手动采样数据
    quint8 dataAlarm;                    //报警一览
    quint8 dataSnapshot;               //画面存储
    quint8 dataSetting;                   //设定文件

    //传送延迟
    quint8 delayDisplayEvent;      //显示&事件数据传送延迟，单位：min, 范围：0~120
    quint8 delayReport;                 //报表传送延迟，单位：min,范围：0~120

    //加密
    quint8 encryptionSSL;              //SSL传输加密，0-off, 1-on
    quint8 encryptionVerify;          //证书验证加密，0-off, 1-on
    quint8 RESERVED;

    //第一、二优先服务器设定
    quint16 serverPASVMode[2];       //PASV模式，0-off,1-on,默认:0
    quint16 serverPort[2];                   //端口号，1~65535, 默认：21
    uchar   serverName[2][64];          //FTP服务器名，最多64个半角字符
    uchar   serverUserName[2][32];  //用户名
    uchar   serverPasswd[2][32];        //密码
    uchar   serverDirectory[2][64];     //目录名
}NETWORK_CONFIG_FTPCLIENT ;


#endif // CFGGLOBALDEF_NETWORK_FTPCLIENT_H
