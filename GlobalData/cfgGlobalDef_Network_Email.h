/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：CfgGlobbalDef_Network_Email.h
 * 概   要：网络--Email配置相关数据结构体、枚举定义
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

#ifndef CFGGLOBALDEF_NETWORK_EMAIL_H
#define CFGGLOBALDEF_NETWORK_EMAIL_H

/****************************************
 * Email配置
 ****************************************/
typedef struct NETWORK_CONFIG_EMAIL_STRUCT
{
    //邮件标题
    uchar   recipientAddr[2][152];    //收信地址1/2字符串，最多150个半角字符
    uchar   senderAddr[64];              //发信地址，最多64个半角字符
    uchar   subject[32];                      //邮件主题，最多32个半角字符

    //邮件内容
    uchar   header[128];                    //标题，字符串，最多128个半角字符，中英文支持
    quint8    includeSrcURL;               //追加发信地址URL，WEB服务器有效时添加,1-on, 0-off

    //报表设定
    quint8 reportNotify;                   //报表通知，0-off, 1-收信地址1, 2-收信地址2, 3-收信地址1&2

    //定时设定
    quint8 timerNotify;                    //定时通知,1-on, 0-off
    quint8 timerAttachData;            //追加瞬时值,1-on, 0-off
    quint8 timerInterval[2];              //收信地址1&2发送间隔, 单位：h, 范围：1~24
    quint8 timerBaseHour[2];          //收信地址1&2发送基准时间 时，范围：0~23
    quint8 timerBaseMinute[2];      //收信地址1&2发送基准时间 分，范围:0~59
    quint8 RESERVED[2];

    //报警设定
    quint8 alarmNotify;                    //报警通知，0-off, 1-收信地址1, 2-收信地址2, 3-收信地址1&2, 默认：0
    quint8 alarmAttachData;            //追加瞬时值,1-on, 0-off
    quint8 alarmAttachTagCh;          //给主题添加标记/通道,1-on, 0-off
    quint8 alarmChannelNum;        //检测通道数
    quint32 alarmChannelItem[50];   //检测通道，最多50个通道，有效项由alarmChannelNum决定

    //系统设定
    quint8 systemMemFull;              //系统内存满通知，0-off, 1-收信地址1, 2-收信地址2, 3-收信地址1&2, 默认：0
    quint8 systemPowerFail;            //电源故障通知，0-off, 1-收信地址1, 2-收信地址2, 3-收信地址1&2, 默认：0
    quint8 systemError;                    //系统错误通知，0-off, 1-收信地址1, 2-收信地址2, 3-收信地址1&2, 默认：0
    quint8 systemUserLock;             //用户锁定通知，0-off, 1-收信地址1, 2-收信地址2, 3-收信地址1&2, 默认：0
}NETWORK_CONFIG_EMAIL ;

#endif // CFGGLOBALDEF_NETWORK_EMAIL_H
