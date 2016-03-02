#ifndef CFGCONFIG_NETWORK_H
#define CFGCONFIG_NETWORK_H

#include <QObject>
#include "cfgGlobalDef.h"
#include "configtreeobject.h"

//network
#define NetWork_Start_ID                        20000
#define NetWork_End_ID                          20999

#define NetWork_Basic_Start_ID                 20000
#define NetWork_Basic_End_ID                   20099

#define NetWork_FTP_Start_ID                   20100
#define NetWork_FTP_End_ID                     20199

#define NetWork_SMTP_Start_ID                   20200
#define NetWork_SMTP_End_ID                     20299

#define NetWork_Email_Start_ID                  20300
#define NetWork_Email_End_ID                    20399

#define NetWork_SNTP_Start_ID                  20400
#define NetWork_SNTP_End_ID                    20449

#define NetWork_Modbus_Start_ID                  20450
#define NetWork_Modbus_End_ID                    20499

#define NetWork_Server_Start_ID                  20500
#define NetWork_Server_End_ID                    20599

//
/********************              NetWork  20000 - 21000          ********************/

#define ID_NetWork                                  20000

//network basic         20001 - 20100
#define ID_NetWork_Basic                            20001     //title

#define ID_NetWork_Basic_AutoIPSetting              20010   //group
#define ID_NetWork_Basic_AutoIPSetting_Value        20011           //自动获取IP，0-off, 1-on

#define ID__NetWork_Basic_AutoDNSSetting            20020
#define ID__NetWork_Basic_AutoDNSSetting_Vlaue      20021           //自动DNS，0-off, 1-on

//#define ID__NetWork_Basic_Reserved                //保留字段，保留20030-20040值

#define ID__NetWork_Basic_IPAddress                 20050       //ip address
#define ID__NetWork_Basic_IPAddress_Value           20051
#define ID__NetWork_Basic_SubNetMask_Value          20052       //netmask
#define ID__NetWork_Basic_DefaultGateway_Value      20053           //gateway

#define ID_NetWork_Basic_DNSSetting                 20060       //DNS
#define ID_NetWork_Basic_DNSSetting_First           20061
#define ID_NetWork_Basic_DNSSetting_Second          20062

//network FTP         20100 - 20200
#define ID_NetWork_FTP                              20100

#define ID_NetWork_FTP_Client                       20101
#define ID_NetWork_FTP_Client_OnOff                 20102   //FTP功能开关 传送文件，在FTP功能on时有效， 0-off, 1-on

#define ID_NetWork_FTP_Transfer                     20110       //在FTP功能on时有效， 0-off, 1-on
#define ID_NetWork_FTP_Transfer_DataDispEvent       20111   //显示&事件数据
#define ID_NetWork_FTP_Transfer_DataReport          20112   //报表数据
#define ID_NetWork_FTP_Transfer_DataManualSample    20113   //手动采样数据
#define ID_NetWork_FTP_Transfer_DataAlarm           20114   //报警一览
#define ID_NetWork_FTP_Transfer_DataSnapshot        20115   //画面存储
#define ID_NetWork_FTP_Transfer_DataSetting         20116   //设定文件

#define ID_NetWork_FTP_TransferWait                 20120       //传送延迟
#define ID_NetWork_FTP_TransferWait_DispEvent       20121       //显示&事件数据传送延迟，单位：min, 范围：0~120
#define ID_NetWork_FTP_TransferWait_Report          20122       //报表传送延迟，单位：min,范围：0~120

#define ID_NetWork_FTP_TransferEncry                20130      //加密
#define ID_NetWork_FTP_TransferEncry_SSL            20131       //SSL传输加密，0-off, 1-on
#define ID_NetWork_FTP_TransferEncry_Verity         20132       //证书验证加密，0-off, 1-on
#define ID_NetWork_FTP_TransferEncry_Reserved

#define ID_NetWork_FTP__ServerFirst                 20140       //第一、二优先服务器设定
#define ID_NetWork_FTP__ServerFirst_PASV            20141       //PASV模式，0-off,1-on,默认:0
#define ID_NetWork_FTP__ServerFirst_Port            20142       //端口号，1~65535, 默认：21
#define ID_NetWork_FTP__ServerFirst_Name            20143       //FTP服务器名，最多64个半角字符
#define ID_NetWork_FTP__ServerFirst_userName        20144       //用户名
#define ID_NetWork_FTP__ServerFirst_Pwd             20145       //密码
#define ID_NetWork_FTP__ServerFirst_Directory       20146        //目录名

#define ID_NetWork_FTP__ServerSecond                 20150
#define ID_NetWork_FTP__ServerSecond_PASV            20151
#define ID_NetWork_FTP__ServerSecond_Port            20152
#define ID_NetWork_FTP__ServerSecond_Name            20153
#define ID_NetWork_FTP__ServerSecond_userName        20154
#define ID_NetWork_FTP__ServerSecond_Pwd             20155
#define ID_NetWork_FTP__ServerSecond_Directory       20156



//network SMTP  20200 - 20300
#define ID_NetWork_SMTP                             20200

#define ID_NetWork_SMTP_Client                      20201
#define ID_NetWork_SMTP_Client_OnOff                20202       //SMTP功能开关，0-off, 1-on

#define ID_NetWork_SMTP_Client_AuthType             20210
#define ID_NetWork_SMTP_Client_AuthType_Value       20211       //用户认证方式

#define ID_NetWork_SMTP_Client_Encry                20220
#define ID_NetWork_SMTP_Client_Encry_SSL            20221   //SSL传输加密，0-off, 1-on
#define ID_NetWork_SMTP_Client_Encry_Verify         20222        //证书验证加密，0-off, 1-on

#define ID_NetWork_SMTP_Client_Server               20230       //服务器端
#define ID_NetWork_SMTP_Client_Serverport           20231       //服务器端口号，1~65535,默认：25
#define ID_NetWork_SMTP_Client_ServerName           20232       //服务器名，最多64个半角字符
#define ID_NetWork_SMTP_Client_ServerUserName       20233       //用户名，最多32个半角字符,【SMTP】认证时有效
#define ID_NetWork_SMTP_Client_ServerPwd            20234       //密码，最多32个半角字符,【SMTP】认证时有效

#define ID_NetWork_SMTP_Client_POP3Server           20240
#define ID_NetWork_SMTP_Client_POP3ServerPort       20241        //POP3服务器端口号，1~65535,默认：110
#define ID_NetWork_SMTP_Client_POP3ServerName       20242
#define ID_NetWork_SMTP_Client_POP3ServeruserName   20243
#define ID_NetWork_SMTP_Client_POP3ServerPwd        20244

//network email   20300 - 20399
#define ID_NetWork_Email                            20300
//邮件标题
#define ID_NetWork_Email_MailHeader                     20301
#define ID_NetWork_Email_MailHeader_RecipientAddr_1     20302       //收信地址1/2字符串，最多150个半角字符
#define ID_NetWork_Email_MailHeader_RecipientAddr_2     20303
#define ID_NetWork_Email_MailHeader_SenderAddr          20304       //发信地址，最多64个半角字符
#define ID_NetWork_Email_MailHeader_Subject             20305      //邮件主题，最多32个半角字符

//邮件内容
#define ID_NetWork_Email_Content                        20310
#define ID_NetWork_Email_ContentHeader                  20311
#define ID_NetWork_Email_ContentIncludeSrcURL           20312

//报表设定
#define ID_NetWork_Email_Report                         20315
#define ID_NetWork_Email_ReportNotify                   20316   //报表通知，0-off, 1-收信地址1, 2-收信地址2, 3-收信地址1&2

//定时设定
#define ID_NetWork_Email_Timer                           20320       //group
#define ID_NetWork_Email_TimerNotify                     20321       //定时通知,1-on, 0-off
#define ID_NetWork_Email_TimerAttachData                 20322       //追加瞬时值,1-on, 0-off
#define ID_NetWork_Email_TimerInterval_1                 20323
#define ID_NetWork_Email_TimerInterval_2                 20324
#define ID_NetWork_Email_TimerBaseHour_1                 20325
#define ID_NetWork_Email_TimerBaseHour_2                 20326
#define ID_NetWork_Email_TimerBaseMin_1                  20327
#define ID_NetWork_Email_TimerBaseMin_2                  20328
#define ID_NetWork_Email_TimerReserved_1                 20329
#define ID_NetWork_Email_TimerReserved_2                 20330

//报警设定
#define ID_NetWork_Email_Alarm                          20331
#define ID_NetWork_Email_AlarmNotify                    20332   //报警通知，0-off, 1-收信地址1, 2-收信地址2, 3-收信地址1&2, 默认：0
#define ID_NetWork_Email_AlarmAttachData                20333   //追加瞬时值,1-on, 0-off
#define ID_NetWork_Email_AlarmAttachTagCh               20334   //给主题添加标记/通道,1-on, 0-off
#define ID_NetWork_Email_AlarmChannelNum                20335   //检测通道数
#define ID_NetWork_Email_AlarmChannelItem               20336       //在界面不显示该选项 预留 20335 - 20389

//系统设定
#define ID_NetWork_Email_System                         20390
#define ID_NetWork_Email_SystemMemFull                  20391   //系统内存满通知，0-off, 1-收信地址1, 2-收信地址2, 3-收信地址1&2, 默认：0
#define ID_NetWork_Email_SystemPowerFail                20392 //电源故障通知，0-off, 1-收信地址1, 2-收信地址2, 3-收信地址1&2, 默认：0
#define ID_NetWork_Email_SystemError                    20393 //系统错误通知，0-off, 1-收信地址1, 2-收信地址2, 3-收信地址1&2, 默认：0
#define ID_NetWork_Email_SystemUserLock                 20394 //用户锁定通知，0-off, 1-收信地址1, 2-收信地址2, 3-收信地址1&2, 默认：0


//email sntp    20400 - 20450
#define ID_NetWork_SNTP                                 20400

#define ID_NetWork_SNTP_Client                          20401
#define ID_NetWork_SNTP_ClientOnOff                     20402       //SNTP功能开关，0-off, 1-on

#define ID_NetWork_SNTP_Query                           20410
#define ID_NetWork_SNTP_QueryInterval                   20411       //查询间隔，单位:h, 范围：6/12/24h
#define ID_NetWork_SNTP_QueryTimeout                    20412       //查询超时时间，单位：s,范围：10/30/90s
#define ID_NetWork_SNTP_QueryAdjust                     20413       //记录开始时，进行对时，0-off, 1-on

#define ID_NetWork_SNTP_Server                          20420
#define ID_NetWork_SNTP_ServerPort                      20421        //服务器端口号，1~65535,默认：123
#define ID_NetWork_SNTP_ServerName                      20422       //服务器名，最多64个半角字符


//network modbus    20450 - 20500
#define ID_NetWork_Modbus                               20450

//network modbus basic
#define ID_NetWork_Modbus_Basic                         20451

#define ID_NetWork_Modbus_BasicClient                   20452
#define ID_NetWork_Modbus_BasicClientOnOff              20453       //Modbus客户端功能开关，0-off, 1-on

#define ID_NetWork_Modbus_BasicComm                     20454
#define ID_NetWork_Modbus_BasicCommInterval             20455   //采样间隔，单位:ms,范围：100ms/200ms/500ms/1s/2s/5s/10s/20s/30s/1min,默认：1s

#define ID_NetWork_Modbus_BasicRecovery                 20456
#define ID_NetWork_Modbus_BasicRecWaitTime              20457   //通信恢复等待时间，单位:s，范围：0(off)/5s/10s/30s/1min/2min/5min,默认:2min

#define ID_NetWork_Modbus_BasicConnect                  20458
#define ID_NetWork_Modbus_BasicConnectOnOff             20459       //保持连接开关，0-off, 1-on
#define ID_NetWork_Modbus_BasicConnectTime              20460       //保持连接时间，单位：s,范围：1~10

//network modbus server
#define ID_NetWork_Modbus_Server                        20461   //title

#define ID_NetWork_Modbus_ServerNO                      20462
#define ID_NetWork_Modbus_ServerSetting                 20463   //group
#define ID_NetWork_Modbus_ServerName                    20464   //服务器名，最多64个半角字符
#define ID_NetWork_Modbus_Serverport                    20465   //服务器端口号，1~65535,默认：502

//network modbus cmd
#define ID_NetWork_Modbus_Cmd                           20470
#define ID_NetWork_Modbus_CmdNO                         20471

#define ID_NetWork_Modbus_CmdSetting                    20472
#define ID_NetWork_Modbus_CmdSettingType                20473       //命令类型：0-off, 1-读取， 2-写入
#define ID_NetWork_Modbus_CmdSettingServer              20474       //命令对应的服务器号，范围:1~32
#define ID_NetWork_Modbus_CmdSettingUnitNO              20475       //单元号:1~255, Modbus网关功能时使用
#define ID_NetWork_Modbus_CmdSettingRegType             20476       //寄存器类型
#define ID_NetWork_Modbus_CmdSettingRegNO               20477       //寄存器号
#define ID_NetWork_Modbus_CmdSettingChType              20478       //通道类型，读取时：通信通道；写入时：输入输出通道/运算通道/通信通道
#define ID_NetWork_Modbus_CmdSettingStartNO             20479       //与通道类型匹配的起始通道号
#define ID_NetWork_Modbus_CmdSettingEndNo               20480       //与通道类型匹配的结束通道号

//network server setting
#define ID_NetWork_Server                               20500

#define ID_NetWork_Server_Modbus                        20501

#define ID_NetWork_Server_Modbus_ModbusServer             20502
#define ID_NetWork_Server_Modbus_ModbusServerLimit        20503    //Modbus服务连接限制开关


#define ID_NetWork_Server_Modbus_ModbusServerLim_1        20505
#define ID_NetWork_Server_Modbus_ModbusServerLimSwitch_1  20506     //限制开关，
#define ID_NetWork_Server_Modbus_ModbusServerLimIP_1      20507      //限制IP, IPv4

#define ID_NetWork_Server_Modbus_ModbusServerLim_2        20510     //限制开关，
#define ID_NetWork_Server_Modbus_ModbusServerLimSwitch_2  20511
#define ID_NetWork_Server_Modbus_ModbusServerLimIP_2      20512

#define ID_NetWork_Server_Modbus_ModbusServerLim_3        20515     //限制开关，
#define ID_NetWork_Server_Modbus_ModbusServerLimSwitch_3  20516
#define ID_NetWork_Server_Modbus_ModbusServerLimIP_3      20517

#define ID_NetWork_Server_Modbus_ModbusServerLim_4        20520     //限制开关，
#define ID_NetWork_Server_Modbus_ModbusServerLimSwitch_4  20521
#define ID_NetWork_Server_Modbus_ModbusServerLimIP_4      20522

#define ID_NetWork_Server_Modbus_ModbusServerLim_5        20525     //限制开关，
#define ID_NetWork_Server_Modbus_ModbusServerLimSwitch_5  20526
#define ID_NetWork_Server_Modbus_ModbusServerLimIP_5      20527

#define ID_NetWork_Server_Modbus_ModbusServerLim_6        20530     //限制开关，
#define ID_NetWork_Server_Modbus_ModbusServerLimSwitch_6  20531
#define ID_NetWork_Server_Modbus_ModbusServerLimIP_6      20532

#define ID_NetWork_Server_Modbus_ModbusServerLim_7        20535     //限制开关，
#define ID_NetWork_Server_Modbus_ModbusServerLimSwitch_7  20536
#define ID_NetWork_Server_Modbus_ModbusServerLimIP_7      20537

#define ID_NetWork_Server_Modbus_ModbusServerLim_8        20540     //限制开关，
#define ID_NetWork_Server_Modbus_ModbusServerLimSwitch_8  20541
#define ID_NetWork_Server_Modbus_ModbusServerLimIP_8      20542

#define ID_NetWork_Server_Modbus_ModbusServerLim_9        20545     //限制开关，
#define ID_NetWork_Server_Modbus_ModbusServerLimSwitch_9  20546
#define ID_NetWork_Server_Modbus_ModbusServerLimIP_9      20547

#define ID_NetWork_Server_Modbus_ModbusServerLim_10        20550     //限制开关，
#define ID_NetWork_Server_Modbus_ModbusServerLimSwitch_10  20551
#define ID_NetWork_Server_Modbus_ModbusServerLimIP_10      20552

#define ID_NetWork_Server_List                              20560       //server list

#define ID_NetWork_Server_List_FTP                          20561
#define ID_NetWork_Server_List_FTPOnOff                     20562       //FTP服务器开关，0-off, 1-on
#define ID_NetWork_Server_List_FTPEncrypt                   20563    //FTP加密开关， 0-off, 1-on
#define ID_NetWork_Server_List_FTPConnectHole               20564   //FTP服务器保持有效功能，0-off, 1-on
#define ID_NetWork_Server_List_FTPPort                      20565

#define ID_NetWork_Server_List_HTTP                         20570
#define ID_NetWork_Server_List_HTTPOnOff                    20571       //HTTP服务开关，0-off, 1-on
#define ID_NetWork_Server_List_HTTPEncrypt                  20572       //HTTP加密开关， 0-off, 1-on
#define ID_NetWork_Server_List_HTTPPort                     20573

#define ID_NetWork_Server_List_SNTP                         20575
#define ID_NetWork_Server_List_SNTPOnOff                    20576       //SNTP服务开关，0-off, 1-on
#define ID_NetWork_Server_List_SNTPPort                     20577

#define ID_NetWork_Server_List_Modbus                         20580
#define ID_NetWork_Server_List_ModbusOnOff                    20581       //Modbus服务开关，0-off, 1-on
#define ID_NetWork_Server_List_ModbusPort                     20582
#define ID_NetWork_Server_Modbus_ModbusServerTimeOut      20504   //Modbus服务器延迟响应，单位：ms,范围：0(off)/10ms/20ms/50ms

class CfgConfig_Network : public QObject
{
    Q_OBJECT
public:
    static CfgConfig_Network * instance();
    QVariant getNetWorkData(quint32 ID, quint32 group);
    void setNetWorkData(quint32 ID, quint32 group);

    //network obj str
    QString getNetWorkObjStr(quint32 ID, quint32 group = 0);     //第二个参数先保留，目前没有用到
    //network obj value str
    QString getNetWorkValueObjStr(quint32 ID, quint32 group, ConfigObject *cfg);

    void setDeviceNetworkConfig(DEVICE_CONFIG  &deviceConfig) {p_deviceConfig = &deviceConfig;}

    QString getNetworkModbusChValObjStr(quint32 code);

private:
    explicit CfgConfig_Network(QObject *parent = 0);

    /********************************************* netWork *******************************************/

    //network baisc
    QVariant getNetWorkBasicData(quint32 ID, quint32 group);
    QVariant getNetWorkBasicIpAddrData(quint8 *addr);
    //network ftp
    QVariant getNetWorkFTPData(quint32 ID, quint32 group);
    //network smtp
    QVariant getNetWorkSMTPData(quint32 ID, quint32 group);
    //network email
    QVariant getNetWorkEmailData(quint32 ID, quint32 group);
    QVariant getNetWorkEmailIntervalData(quint32 type);
    //network sntp
    QVariant getNetWorkSNTPData(quint32 ID, quint32 group);
    //network modbus
    QVariant getNetWorkModbusData(quint32 ID, quint32 group);
    QVariant getNetworkModbusBasicCommInterval(quint32 data);
    QVariant getNetworkModbusBasicRecTimeData(quint32 data);
    //network server
    QVariant getNetWorkServerData(quint32 ID, quint32 group);

    QVariant getCharToString(uchar *str); //把char *类型转化为QVariant类型


    //network basic
    QString getNetWorkBasicObjStr(quint32 ID, quint32 group = 0);     //第二个参数先保留，目前没有用到
    //network ftp
    QString getNetWorkFTPObjStr(quint32 ID, quint32 group = 0);
    //network smtp
    QString getNetWorkSMTPObjStr(quint32 ID, quint32 group = 0);
    //network email
    QString getNetWorkEmailObjStr(quint32 ID, quint32 group = 0);
    //network sntp
    QString getNetWorkSNTPObjStr(quint32 ID, quint32 group = 0);
    //network modbus
    QString getNetWorkModbusObjStr(quint32 ID, quint32 group = 0);
    //network server
    QString getNetWorkServerObjStr(quint32 ID, quint32 group = 0);


    //network baisc
    QString getNetWorkBasicValueObjStr(quint32 ID, quint32 group, ConfigObject *cfg);
    //network ftp
    QString getNetWorkFTPValueObjStr(quint32 ID, quint32 group, ConfigObject *cfg);
    //network smtp
    QString getNetWorkSMTPValueObjStr(quint32 ID, quint32 group, ConfigObject *cfg);
    QString getSMTPAuthType(quint32 type);
    //network email
    QString getNetWorkEmailValueObjStr(quint32 ID, quint32 group, ConfigObject *cfg);
    QString getNetWorkEmailSystemValObjStr(quint32 type);
    quint32 getNetWorkEmailIntervalValObjStr(quint32 type);

    //network sntp
    QString getNetWorkSNTPValueObjStr(quint32 ID, quint32 group, ConfigObject *cfg);
    //network modbus
    QString getNetWorkModbusValueObjStr(quint32 ID, quint32 group, ConfigObject *cfg);
    QString getNetworkModbusRegTypeValStr(quint32 type);
    QString getNetWorkModbusBasicInterval(quint32 type);
    QString getNetWorkModbusBasicRecTime(quint32 type);


    //network server
    QString getNetWorkServerValueObjStr(quint32 ID, quint32 group, ConfigObject *cfg);


    void setNetWorkBasicData(quint32 ID, quint32 group);
    void setNetWorkFTPData(quint32 ID, quint32 group);
    void setNetWorkSMTPData(quint32 ID, quint32 group);
    void setNetWorkEmailData(quint32 ID, quint32 group);
    void setNetWorkSNTPData(quint32 ID, quint32 group);
    void setNetWorkModbusData(quint32 ID, quint32 group);
    void setNetWorkModbusBasicCommInterval(quint32 & time);
    void setNetWorkModbusBasicRecTime(quint32 & time);

    void setNetWorkServerData(quint32 ID, quint32 group);

    void setIPAddress(quint8 * IP, QString IPStr);
    void setStrToChar(uchar *uData, QString str, int len = 0);

    void setModbusChType(quint32 group, Channel_Type &chType);

signals:

public slots:

private:
    DEVICE_CONFIG *p_deviceConfig;
};

#endif // CFGCONFIG_NETWORK_H
