#include "cfgconfig_network.h"

#include "./Common/winnetworkconfig.h"
#include "./GlobalData/cfgchannel.h"
#include "../Common/devboard.h"

CfgConfig_Network::CfgConfig_Network(QObject *parent) :
    QObject(parent)
{
}

CfgConfig_Network * CfgConfig_Network::instance()
{
    static CfgConfig_Network data;
    return &data;
}


/**********************************************************   network  ************************************************/

//network obj data
QVariant CfgConfig_Network::getNetWorkData(quint32 ID, quint32 group)
{
    if (ID >= NetWork_Basic_Start_ID && ID <= NetWork_Basic_End_ID)
        return getNetWorkBasicData(ID, group);
    else if (ID >= NetWork_FTP_Start_ID && ID <= NetWork_FTP_End_ID)
        return getNetWorkFTPData(ID, group);
    else if (ID >= NetWork_SMTP_Start_ID && ID <= NetWork_SMTP_End_ID)
        return getNetWorkSMTPData(ID, group);
    else if (ID >= NetWork_Email_Start_ID && ID <= NetWork_Email_End_ID)
        return getNetWorkEmailData(ID, group);
    else if (ID >= NetWork_SNTP_Start_ID && ID <= NetWork_SNTP_End_ID)
        return getNetWorkSNTPData(ID, group);
    else if (ID >= NetWork_Modbus_Start_ID && ID <= NetWork_Modbus_End_ID)
        return getNetWorkModbusData(ID, group);
    else if (ID >= NetWork_Server_Start_ID && ID <= NetWork_Server_End_ID)
        return getNetWorkServerData(ID, group);

    return QVariant();
}

QVariant CfgConfig_Network::getNetWorkBasicData(quint32 ID, quint32 /*group*/)
{
    NETWORK_CONFIG_BASIC netWork = p_deviceConfig->networkBasic;
    switch (ID) {
    case ID_NetWork_Basic_AutoIPSetting_Value:      //自动获取IP，0-off, 1-on
        return netWork.autoIP;
        break;
    case ID__NetWork_Basic_AutoDNSSetting_Vlaue:        //自动DNS，0-off, 1-on
        return netWork.autoDNS;
        break;
    case ID__NetWork_Basic_IPAddress_Value:      //ip address
        return getNetWorkBasicIpAddrData(netWork.ipAddress);
        break;
    case ID__NetWork_Basic_SubNetMask_Value:        //netmask
        return getNetWorkBasicIpAddrData(netWork.subnetMask);
        break;
    case ID__NetWork_Basic_DefaultGateway_Value:        //gateway
        return getNetWorkBasicIpAddrData(netWork.defaultGateway);
        break;
    case ID_NetWork_Basic_DNSSetting_First:             //DNS
        return getNetWorkBasicIpAddrData(netWork.DNSAddrFirst);
        break;
    case ID_NetWork_Basic_DNSSetting_Second:
        return getNetWorkBasicIpAddrData(netWork.DNSAddrSecond);
        break;
    default:
        break;
    }
    return QVariant();
}

/*
 * 通过地址指针得到地址数组，转化为ip地址类型(0.0.0.0)
*/
QVariant CfgConfig_Network::getNetWorkBasicIpAddrData(quint8 *addr)
{
    QString addrStr = QString("%1.%2.%3.%4").arg(addr[0]).arg(addr[1]).arg(addr[2]).arg(addr[3]);
    return addrStr;
}

//network obj str
QString CfgConfig_Network::getNetWorkObjStr(quint32 ID, quint32 /*group*/)
{
    if (ID >= NetWork_Basic_Start_ID && ID <= NetWork_Basic_End_ID)
        return getNetWorkBasicObjStr(ID);
    else if (ID >= NetWork_FTP_Start_ID && ID <= NetWork_FTP_End_ID)
        return getNetWorkFTPObjStr(ID);
    else if (ID >= NetWork_SMTP_Start_ID && ID <= NetWork_SMTP_End_ID)
        return getNetWorkSMTPObjStr(ID);
    else if (ID >= NetWork_Email_Start_ID && ID <= NetWork_Email_End_ID)
        return getNetWorkEmailObjStr(ID);
    else if (ID >= NetWork_SNTP_Start_ID && ID <= NetWork_SNTP_End_ID)
        return getNetWorkSNTPObjStr(ID);
    else if (ID >= NetWork_Modbus_Start_ID && ID <= NetWork_Modbus_End_ID)
        return getNetWorkModbusObjStr(ID);
    else if (ID >= NetWork_Server_Start_ID && ID <= NetWork_Server_End_ID)
        return getNetWorkServerObjStr(ID);

    return NULL;
}

QString CfgConfig_Network::getNetWorkBasicObjStr(quint32 ID, quint32 /*group*/)
{
    switch (ID) {
    case ID_NetWork:
        return tr("Communication (Ethernet) settings");
        break;

    case ID_NetWork_Basic:
        return tr("Basic settings");
        break;

    case ID_NetWork_Basic_AutoIPSetting:        //IP
        return tr("Automatic IP settings");
        break;
    case ID_NetWork_Basic_AutoIPSetting_Value:
        return tr("Obtain IP address automatically");
        break;

    case ID__NetWork_Basic_AutoDNSSetting:          //DNS
        return tr("Automatic DNS settings");
        break;
    case ID__NetWork_Basic_AutoDNSSetting_Vlaue:        //自动DNS，0-off, 1-on
        return tr("Obtain DNS address automatically");
        break;

    case ID__NetWork_Basic_IPAddress:          //IP
        return tr("IP Address");
        break;
    case ID__NetWork_Basic_IPAddress_Value:
        return tr("IP Address");
        break;
    case ID__NetWork_Basic_SubNetMask_Value:
        return tr("Subnet mask");
        break;
    case ID__NetWork_Basic_DefaultGateway_Value:
        return tr("Default gateway");
        break;

    case ID_NetWork_Basic_DNSSetting:
        return tr("DNS settings");
        break;
    case ID_NetWork_Basic_DNSSetting_First:
        return tr("Primary DNS server");
        break;
    case ID_NetWork_Basic_DNSSetting_Second:
        return tr("Second DNS server");
        break;
    default:
        break;
    }
    return NULL;
}


//network obj value str
QString CfgConfig_Network::getNetWorkValueObjStr(quint32 ID, quint32 group, ConfigObject *cfg)
{
    if (ID >= NetWork_Basic_Start_ID && ID <= NetWork_Basic_End_ID)
        return getNetWorkBasicValueObjStr(ID, group, cfg);
    else if (ID >= NetWork_FTP_Start_ID && ID <= NetWork_FTP_End_ID)
        return getNetWorkFTPValueObjStr(ID, group, cfg);
    else if (ID >= NetWork_SMTP_Start_ID && ID <= NetWork_SMTP_End_ID)
        return getNetWorkSMTPValueObjStr(ID, group, cfg);
    else if (ID >= NetWork_Email_Start_ID && ID <= NetWork_Email_End_ID)
        return getNetWorkEmailValueObjStr(ID, group, cfg);
    else if (ID >= NetWork_SNTP_Start_ID && ID <= NetWork_SNTP_End_ID)
        return getNetWorkSNTPValueObjStr(ID, group, cfg);
    else if (ID >= NetWork_Modbus_Start_ID && ID <= NetWork_Modbus_End_ID)
        return getNetWorkModbusValueObjStr(ID, group, cfg);
    else if (ID >= NetWork_Server_Start_ID && ID <= NetWork_Server_End_ID)
        return getNetWorkServerValueObjStr(ID, group, cfg);
    return NULL;
}


/*
 * 通过ID, group和配置对象 得到配置值描述
*/
QString CfgConfig_Network::getNetWorkBasicValueObjStr(quint32 ID, quint32 /*group*/, ConfigObject *cfg)
{
    QString cfgData = cfg->getData().toString();
    switch (ID) {
    case ID_NetWork_Basic_AutoIPSetting_Value:
    case ID__NetWork_Basic_AutoDNSSetting_Vlaue:        //自动DNS，0-off, 1-on
        return cfgData.toInt() == 0 ? tr("Off") : tr("On");
        break;
    default:
        break;
    }
    return cfgData;
}


/*
 * 通过ID, group和配置对象 得到有关smtp配置值描述
*/
QString CfgConfig_Network::getNetWorkSMTPValueObjStr(quint32 ID, quint32 /*group*/, ConfigObject *cfg)
{
    QString cfgData = cfg->getData().toString();
    switch (ID) {
    case ID_NetWork_SMTP_Client_OnOff:
    case ID_NetWork_SMTP_Client_Encry_SSL:        //证书验证加密，0-off, 1-on
    case ID_NetWork_SMTP_Client_Encry_Verify:
        return cfgData.toInt() == 0 ? tr("Off") : tr("On");
        break;
    case ID_NetWork_SMTP_Client_AuthType_Value:         //用户认证方式
        return getSMTPAuthType(cfgData.toInt());
        break;
    case ID_NetWork_SMTP_Client_ServerPwd:
    case ID_NetWork_SMTP_Client_POP3ServerPwd:
         return "****";
    default:
        break;
    }
    return cfgData;
}

/*
 * 通过SMTP认证方式配置值得到认证描述字符串
*/
QString CfgConfig_Network::getSMTPAuthType(quint32 type)
{
    switch (type) {
    case SMTP_Authentication_Type_Off:      //off
        return tr("Off");
        break;
    case SMTP_Authentication_Type_Smtp:     //SMTP认证
        return tr("SMTP authentication");
        break;
    case SMTP_Authentication_Type_Pop:      //POP before SMTP
        return tr("POP before SMTP");
        break;
    case SMTP_Authentication_Type_Apop:      //APOP
        return tr("APOP");
        break;
    default:
        break;
    }
    return NULL;
}

//network ftp

//ftp data
QVariant CfgConfig_Network::getNetWorkFTPData(quint32 ID, quint32 /*group*/)
{
    NETWORK_CONFIG_FTPCLIENT network = p_deviceConfig->networkFTP;
    switch (ID) {
    case ID_NetWork_FTP_Client_OnOff:   //FTP功能开关 传送文件，在FTP功能on时有效， 0-off, 1-on
        return network.clientOnOff;
        break;
    case ID_NetWork_FTP_Transfer_DataDispEvent:        //显示&事件数据
        return network.dataDisplayEvent;
        break;
    case ID_NetWork_FTP_Transfer_DataReport:  //报表数据
        return network.dataReport;
        break;
    case ID_NetWork_FTP_Transfer_DataManualSample:   //手动采样数据
        return network.dataManualSample;
        break;
    case ID_NetWork_FTP_Transfer_DataAlarm:   //报警一览
        return network.dataAlarm;
        break;
    case ID_NetWork_FTP_Transfer_DataSnapshot:   //画面存储
        return network.dataSnapshot;
        break;
    case ID_NetWork_FTP_Transfer_DataSetting:       //设定文件
        return network.dataSetting;
        break;

    case ID_NetWork_FTP_TransferWait_DispEvent:      //显示&事件数据传送延迟，单位：min, 范围：0~120
        return network.delayDisplayEvent;
        break;
    case ID_NetWork_FTP_TransferWait_Report:      //报表传送延迟，单位：min,范围：0~120
        return network.delayReport;
        break;

    case ID_NetWork_FTP_TransferEncry_SSL:     //SSL传输加密，0-off, 1-on
        return network.encryptionSSL;
        break;
    case ID_NetWork_FTP_TransferEncry_Verity:       //证书验证加密，0-off, 1-on
        return network.encryptionVerify;
        break;

    case ID_NetWork_FTP__ServerFirst_PASV:       //PASV模式，0-off,1-on,默认:0
        return network.serverPASVMode[0];
        break;
    case ID_NetWork_FTP__ServerFirst_Port:       //端口号，1~65535, 默认：21
        return network.serverPort[0];
        break;
    case ID_NetWork_FTP__ServerFirst_Name:       //FTP服务器名，最多64个半角字符
        return getCharToString(network.serverName[0]);
        break;
    case ID_NetWork_FTP__ServerFirst_userName:       //用户名
        return getCharToString(network.serverUserName[0]);
        break;
    case ID_NetWork_FTP__ServerFirst_Pwd:
        return getCharToString( network.serverPasswd[0]);
        break;
    case ID_NetWork_FTP__ServerFirst_Directory:     //目录名
        return getCharToString(network.serverDirectory[0]);
        break;

    case ID_NetWork_FTP__ServerSecond_PASV:       //PASV模式，0-off,1-on,默认:0
        return network.serverPASVMode[1];
        break;
    case ID_NetWork_FTP__ServerSecond_Port:       //端口号，1~65535, 默认：21
        return network.serverPort[1];
        break;
    case ID_NetWork_FTP__ServerSecond_Name:       //FTP服务器名，最多64个半角字符
        return getCharToString(network.serverName[1]);
        break;
    case ID_NetWork_FTP__ServerSecond_userName:       //用户名
        return getCharToString(network.serverUserName[1]);
        break;
    case ID_NetWork_FTP__ServerSecond_Pwd:
        return getCharToString(network.serverPasswd[1]);
        break;
    case ID_NetWork_FTP__ServerSecond_Directory:     //目录名
        return getCharToString(network.serverDirectory[1]);
        break;
    default:
        break;
    }
    return QVariant();
}

/*
 * 把char *类型转化为QString类型
*/
QVariant CfgConfig_Network::getCharToString(uchar *str)
{
    QString data((char *)str);
    return tr(data.toLatin1());
}

//smtp data
QVariant CfgConfig_Network::getNetWorkSMTPData(quint32 ID, quint32 /*group*/)
{
    NETWORK_CONFIG_SMTPCLIENT network = p_deviceConfig->networkSTMP;
    switch (ID) {
    case ID_NetWork_SMTP_Client_OnOff:   //SMTP功能开关 传送文件，在FTP功能on时有效， 0-off, 1-on
        return network.clientOnOff;
        break;
    case ID_NetWork_SMTP_Client_AuthType_Value:
        return network.authType;
        break;

    case ID_NetWork_SMTP_Client_Encry_SSL:
        return network.encryptionSSL;
        break;
    case ID_NetWork_SMTP_Client_Encry_Verify:
        return network.encryptionVerify;
        break;

    case ID_NetWork_SMTP_Client_Serverport:   //服务器端口号，1~65535,默认：25
        return network.SMTPServerPort;
        break;
    case ID_NetWork_SMTP_Client_ServerName:
        return getCharToString(network.SMTPServerName);
        break;
    case ID_NetWork_SMTP_Client_ServerUserName:       //用户名，最多32个半角字符,【SMTP】认证时有效
        return getCharToString(network.SMTPServerUserName);
        break;
    case ID_NetWork_SMTP_Client_ServerPwd:     //密码，最多32个半角字符,【SMTP】认证时有效
        return getCharToString(network.SMTPServerPasswd);
        break;

    case ID_NetWork_SMTP_Client_POP3ServerPort:
        return network.POP3ServePort;
        break;
    case ID_NetWork_SMTP_Client_POP3ServerName:
        return getCharToString(network.POP3ServerName);
        break;
    case ID_NetWork_SMTP_Client_POP3ServeruserName:
        return getCharToString(network.POP3ServerUserName);
    case ID_NetWork_SMTP_Client_POP3ServerPwd:
        return getCharToString(network.POP3ServerPasswd);
        break;
    default:
        break;
    }
    return QVariant();
}


/*
 * 通过ID, group和配置对象 得到配置值描述
*/
QString CfgConfig_Network::getNetWorkFTPValueObjStr(quint32 ID, quint32 /*group*/, ConfigObject *cfg)
{
    QString cfgData = cfg->getData().toString();
    switch (ID) {
    case ID_NetWork_FTP_Client_OnOff:                       //FTP功能开关 传送文件，在FTP功能on时有效， 0-off, 1-on
    case ID_NetWork_FTP_Transfer_DataDispEvent:             //显示&事件数据
    case ID_NetWork_FTP_Transfer_DataReport:                //报表数据
    case ID_NetWork_FTP_Transfer_DataManualSample:          //手动采样数据
    case ID_NetWork_FTP_Transfer_DataAlarm:                 //报警一览
    case ID_NetWork_FTP_Transfer_DataSnapshot:              //画面存储
    case ID_NetWork_FTP_Transfer_DataSetting:               //设定文件
    case ID_NetWork_FTP_TransferEncry_SSL:                  //SSL传输加密，0-off, 1-on
    case ID_NetWork_FTP_TransferEncry_Verity:               //证书验证加密，0-off, 1-on
    case ID_NetWork_FTP__ServerFirst_PASV:                  //PASV模式，0-off,1-on,默认:0
    case ID_NetWork_FTP__ServerSecond_PASV:                 //PASV模式，0-off,1-on,默认:0
        return cfgData.toUInt() == 0 ? tr("Off") : tr("On");
        break;

    case ID_NetWork_FTP_TransferWait_DispEvent:      //显示&事件数据传送延迟，单位：min, 范围：0~120
    case ID_NetWork_FTP_TransferWait_Report:      //报表传送延迟，单位：min,范围：0~120
        return QString::number(cfgData.toInt()) + tr(" min");
        break;
   case ID_NetWork_FTP__ServerFirst_Pwd:
   case ID_NetWork_FTP__ServerSecond_Pwd:
        return "****";
        break;
    default:
        break;
    }
    return cfgData;
}

//ftp obj str
QString CfgConfig_Network::getNetWorkFTPObjStr(quint32 ID, quint32 /*group*/)
{
    switch (ID) {
    case ID_NetWork_FTP:                                //title
        return tr("FTP client settings");
        break;
    case ID_NetWork_FTP_Client:                         //group
        return tr("FTP client function");
        break;
    case ID_NetWork_FTP_Client_OnOff:                         //normal
        return tr("On/Off");
        break;
    case ID_NetWork_FTP_Transfer:                         //group
        return tr("Transfer file");
        break;
    case ID_NetWork_FTP_Transfer_DataDispEvent:
        return tr("Display & Event data");
        break;
    case ID_NetWork_FTP_Transfer_DataReport:
        return tr("Report");
        break;
    case ID_NetWork_FTP_Transfer_DataManualSample:
        return tr("Manual sample data");
        break;
    case ID_NetWork_FTP_Transfer_DataAlarm:
        return tr("Alarm summary");
        break;
    case ID_NetWork_FTP_Transfer_DataSnapshot:
        return tr("Snap shot");
        break;
    case ID_NetWork_FTP_Transfer_DataSetting:
        return tr("Data setting");
        break;

    case ID_NetWork_FTP_TransferWait:                         //group
        return tr("Transfer wait time");
        break;
    case ID_NetWork_FTP_TransferWait_DispEvent:
        return tr("Display & Event data");
        break;
    case ID_NetWork_FTP_TransferWait_Report:
        return tr("Report");
        break;

    case ID_NetWork_FTP_TransferEncry:
    case ID_NetWork_FTP_TransferEncry_SSL:
        return tr("Encryption");
        break;
    case ID_NetWork_FTP_TransferEncry_Verity:
        return tr("Verification of certificate");
        break;

    case ID_NetWork_FTP__ServerFirst:
        return tr("FTP connection primary");
        break;

    case ID_NetWork_FTP__ServerFirst_PASV:
    case ID_NetWork_FTP__ServerSecond_PASV:
        return tr("PASV mode");
        break;
    case ID_NetWork_FTP__ServerFirst_Port:
    case ID_NetWork_FTP__ServerSecond_Port:
        return tr("Port number");
        break;
    case ID_NetWork_FTP__ServerFirst_Name:
    case ID_NetWork_FTP__ServerSecond_Name:
        return tr("FTP server name");
        break;
    case ID_NetWork_FTP__ServerFirst_userName:
    case ID_NetWork_FTP__ServerSecond_userName:
        return tr("User name");
        break;
    case ID_NetWork_FTP__ServerFirst_Pwd:
    case ID_NetWork_FTP__ServerSecond_Pwd:
        return tr("Password");
        break;
    case ID_NetWork_FTP__ServerFirst_Directory:
    case ID_NetWork_FTP__ServerSecond_Directory:
        return tr("Directory");
        break;

    case ID_NetWork_FTP__ServerSecond:
        return tr("FTP connection Secondary");
        break;
    default:
        break;
    }
    return NULL;
}

//network smtp
QString CfgConfig_Network::getNetWorkSMTPObjStr(quint32 ID, quint32 /*group*/)
{
    switch (ID) {
    case ID_NetWork_SMTP:                               //title
        return tr("SMTP client settings");
        break;

    case ID_NetWork_SMTP_Client:                        //group
        return tr("SMTP client function");
        break;
    case ID_NetWork_SMTP_Client_OnOff:
        return tr("On/Off");
        break;

    case ID_NetWork_SMTP_Client_AuthType:           //group
    case ID_NetWork_SMTP_Client_AuthType_Value:
        return tr("Authenctication");
        break;

    case ID_NetWork_SMTP_Client_Encry:
    case ID_NetWork_SMTP_Client_Encry_SSL:
        return tr("Encryption");
        break;
    case ID_NetWork_SMTP_Client_Encry_Verify:       //证书验证加密，0-off, 1-on
        return tr("Verification of certificate");
        break;

    case ID_NetWork_SMTP_Client_Server:
        return tr("SMTP server");
        break;
    case ID_NetWork_SMTP_Client_Serverport:
    case ID_NetWork_SMTP_Client_POP3ServerPort:         //POP3服务器端口号，1~65535,默认：110
        return tr("Port number");
        break;
    case ID_NetWork_SMTP_Client_ServerName:
        return tr("SMTP server name");
        break;
    case ID_NetWork_SMTP_Client_ServerUserName:
    case ID_NetWork_SMTP_Client_POP3ServeruserName:
        return tr("User name");
        break;
    case ID_NetWork_SMTP_Client_ServerPwd:
    case ID_NetWork_SMTP_Client_POP3ServerPwd:
        return tr("Password");
        break;

    case ID_NetWork_SMTP_Client_POP3Server:
        return tr("POP3 server");
        break;

    case ID_NetWork_SMTP_Client_POP3ServerName:
        return tr("POP3 server name");
        break;
    default:
        break;
    }
    return NULL;
}

//network email data
QVariant CfgConfig_Network::getNetWorkEmailData(quint32 ID, quint32 /*group*/)
{
    NETWORK_CONFIG_EMAIL network = p_deviceConfig->networkEmail;
    switch (ID) {
    case ID_NetWork_Email_MailHeader_RecipientAddr_1:   //收信地址1/2字符串，最多150个半角字符
        return getCharToString(network.recipientAddr[0]);
        break;
    case ID_NetWork_Email_MailHeader_RecipientAddr_2:   //收信地址1/2字符串，最多150个半角字符
        return getCharToString(network.recipientAddr[1]);
        break;

    case ID_NetWork_Email_MailHeader_SenderAddr:            //发信地址，最多64个半角字符
        return getCharToString(network.senderAddr);
        break;
    case ID_NetWork_Email_MailHeader_Subject:           //邮件主题，最多32个半角字符
        return getCharToString(network.subject);
        break;

    case ID_NetWork_Email_ContentHeader:                //标题
        return getCharToString(network.header);
        break;
    case ID_NetWork_Email_ContentIncludeSrcURL:                //追加发信地址URL，WEB服务器有效时添加,1-on, 0-off
        return network.includeSrcURL;
        break;

    case ID_NetWork_Email_ReportNotify:   //报表通知，0-off, 1-收信地址1, 2-收信地址2, 3-收信地址1&2
        return network.reportNotify;
        break;

    case ID_NetWork_Email_TimerNotify:       //定时通知,1-on, 0-off
        return network.timerNotify;
        break;
    case ID_NetWork_Email_TimerAttachData:       //追加瞬时值,1-on, 0-off
        return network.timerAttachData;
        break;

    case ID_NetWork_Email_TimerInterval_1:     //收信地址1&2发送间隔, 单位：h, 范围：1~24
        return getNetWorkEmailIntervalData(network.timerInterval[0]);
        break;
    case ID_NetWork_Email_TimerInterval_2:     //收信地址1&2发送间隔, 单位：h, 范围：1~24
        return getNetWorkEmailIntervalData(network.timerInterval[1]);
        break;
    case ID_NetWork_Email_TimerBaseHour_1:
        return network.timerBaseHour[0];
        break;
    case ID_NetWork_Email_TimerBaseHour_2:
        return network.timerBaseHour[1];
        break;
    case ID_NetWork_Email_TimerBaseMin_1:
        return network.timerBaseMinute[0];
        break;
    case ID_NetWork_Email_TimerBaseMin_2:
        return network.timerBaseMinute[1];
        break;

    case ID_NetWork_Email_AlarmNotify:      //报警通知，0-off, 1-收信地址1, 2-收信地址2, 3-收信地址1&2, 默认：0
        return network.alarmNotify;
        break;
    case ID_NetWork_Email_AlarmAttachData:                  //追加瞬时值,1-on, 0-off
        return network.alarmAttachData;
        break;
    case ID_NetWork_Email_AlarmAttachTagCh:         //给主题添加标记/通道,1-on, 0-off
        return network.alarmAttachTagCh;
        break;
    case ID_NetWork_Email_AlarmChannelNum:              //检测通道数
        return network.alarmChannelNum;
        break;

    case ID_NetWork_Email_AlarmChannelItem:
    {
        QList<QVariant> channelList;
        int cnt = network.alarmChannelNum;
        for (int i = 0; i < cnt; i++)
            channelList.append(network.alarmChannelItem[i]);
        return channelList;
    }
        break;

    case ID_NetWork_Email_SystemMemFull:   //系统内存满通知，0-off, 1-收信地址1, 2-收信地址2, 3-收信地址1&2, 默认：0
        return network.alarmNotify;
        break;
    case ID_NetWork_Email_SystemPowerFail:    //电源故障通知，0-off, 1-收信地址1, 2-收信地址2, 3-收信地址1&2, 默认：0
        return network.alarmAttachData;
        break;
    case ID_NetWork_Email_SystemError:        //系统错误通知，0-off, 1-收信地址1, 2-收信地址2, 3-收信地址1&2, 默认：0
        return network.alarmAttachTagCh;
        break;
    case ID_NetWork_Email_SystemUserLock:     //用户锁定通知，0-off, 1-收信地址1, 2-收信地址2, 3-收信地址1&2, 默认：0
        return network.alarmChannelNum;
        break;
    default:
        break;
    }
    return QVariant();
}

/*
 * 通过email的周期配置值，得到该配置值的描述
*/
QVariant CfgConfig_Network::getNetWorkEmailIntervalData(quint32 type)
{
    switch (type) {
    case 1:
        return EMAILInteral_1h;
        break;
    case 2:
        return EMAILInteral_2h;
        break;
    case 3:
        return EMAILInteral_3h;
        break;
    case 4:
        return EMAILInteral_4h;
        break;
    case 6:
        return EMAILInteral_6h;
        break;
    case 8:
        return EMAILInteral_8h;
        break;
    case 12:
        return EMAILInteral_12h;
        break;
    case 24:
        return EMAILInteral_24h;
        break;
    default:
        break;
    }
    return QVariant();
}

//network email obj str
QString CfgConfig_Network::getNetWorkEmailObjStr(quint32 ID, quint32 /*group*/)
{
    switch (ID) {
    case ID_NetWork_Email:
        return tr("E-mail settings");       //title
        break;

    case ID_NetWork_Email_MailHeader:       //group
        return tr("Mail header");
        break;
    case ID_NetWork_Email_MailHeader_RecipientAddr_1:   //收信地址1/2字符串，最多150个半角字符
        return tr("Recipient 1");
        break;
    case ID_NetWork_Email_MailHeader_RecipientAddr_2:
        return tr("Recipient 2");
        break;
    case ID_NetWork_Email_MailHeader_SenderAddr:        //发信地址，最多64个半角字符
        return tr("Sender");
        break;
    case ID_NetWork_Email_MailHeader_Subject:           //邮件主题，最多32个半角字符
        return tr("Subject");
        break;

    case ID_NetWork_Email_Content:              //邮件内容
        return tr("E-mail contents");
        break;
    case ID_NetWork_Email_ContentHeader:
        return tr("Header");
        break;
    case ID_NetWork_Email_ContentIncludeSrcURL:
        return tr("Include source URL");
        break;

    case ID_NetWork_Email_Report:         //报表设定
        return tr("Report settings");
        break;
    case ID_NetWork_Email_ReportNotify:     //报表通知，0-off, 1-收信地址1, 2-收信地址2, 3-收信地址1&2
        return tr("Report notification");
        break;

    case ID_NetWork_Email_Timer:         //定时设定
        return tr("Scheduled settings");
        break;
    case ID_NetWork_Email_TimerNotify:          //定时通知,1-on, 0-off
        return tr("Scheduled notification");
        break;
    case ID_NetWork_Email_TimerAttachData:         //追加瞬时值,1-on, 0-off
        return tr("Attach instanteneous data");
        break;
    case ID_NetWork_Email_TimerInterval_1:
        return tr("Interval (Recipient 1)");
        break;
    case ID_NetWork_Email_TimerInterval_2:
        return tr("Interval (Recipient 2)");
        break;
    case ID_NetWork_Email_TimerBaseHour_1:
        return tr("Ref. time hour (Recipient 1)");
        break;
    case ID_NetWork_Email_TimerBaseHour_2:
        return tr("Ref. time hour (Recipient 2)");
        break;
    case ID_NetWork_Email_TimerBaseMin_1:
        return tr("Ref. time minute (Recipient 1)");
        break;
    case ID_NetWork_Email_TimerBaseMin_2:
        return tr("Ref. time minute (Recipient 2)");
        break;

    case ID_NetWork_Email_Alarm:          //报警设定
        return tr("Alarm settings");
        break;
    case ID_NetWork_Email_AlarmNotify:              //报警通知，0-off, 1-收信地址1, 2-收信地址2, 3-收信地址1&2, 默认：0
        return tr("Alarm notification");
        break;
    case ID_NetWork_Email_AlarmAttachData:          //追加瞬时值,1-on, 0-off
        return tr("Attach instanstaneous data");
        break;
    case ID_NetWork_Email_AlarmAttachTagCh:             //给主题添加标记/通道,1-on, 0-off
        return tr("Include tag/ch in Subject");
        break;
    case ID_NetWork_Email_AlarmChannelNum:               //检测通道数
        return tr("Channel set");
        break;

    case ID_NetWork_Email_System:          //报警设定
        return tr("System settings");
        break;
    case ID_NetWork_Email_SystemMemFull:     //系统内存满通知，0-off, 1-收信地址1, 2-收信地址2, 3-收信地址1&2, 默认：0
        return tr("Memory full notification");
        break;
    case ID_NetWork_Email_SystemPowerFail:     //电源故障通知，0-off, 1-收信地址1, 2-收信地址2, 3-收信地址1&2, 默认：0
        return tr("power failure notification");
        break;
    case ID_NetWork_Email_SystemError:          //系统错误通知，0-off, 1-收信地址1, 2-收信地址2, 3-收信地址1&2, 默认：0
        return tr("System error notification");
        break;
    case ID_NetWork_Email_SystemUserLock:           //用户锁定通知，0-off, 1-收信地址1, 2-收信地址2, 3-收信地址1&2, 默认：0
        return tr("System uset lock notification");
        break;
    default:
        break;
    }
    return NULL;
}


//network email obj value str
QString CfgConfig_Network::getNetWorkEmailValueObjStr(quint32 ID, quint32 /*group*/, ConfigObject *cfg)
{
    QString cfgData = cfg->getData().toString();
    switch (ID) {
    case ID_NetWork_Email_ContentIncludeSrcURL:                //追加发信地址URL，WEB服务器有效时添加,1-on, 0-off
    case ID_NetWork_Email_TimerNotify:       //定时通知,1-on, 0-off
    case ID_NetWork_Email_TimerAttachData:       //追加瞬时值,1-on, 0-off
    case ID_NetWork_Email_AlarmAttachData:                  //追加瞬时值,1-on, 0-off
    case ID_NetWork_Email_AlarmAttachTagCh:         //给主题添加标记/通道,1-on, 0-off
        return cfgData.toInt() == 0 ? tr("Off") : tr("On");
        break;

    case ID_NetWork_Email_ReportNotify:   //报表通知，0-off, 1-收信地址1, 2-收信地址2, 3-收信地址1&2
    case ID_NetWork_Email_AlarmNotify:      //报警通知，0-off, 1-收信地址1, 2-收信地址2, 3-收信地址1&2, 默认：0
    case ID_NetWork_Email_SystemMemFull:   //系统内存满通知，0-off, 1-收信地址1, 2-收信地址2, 3-收信地址1&2, 默认：0
    case ID_NetWork_Email_SystemPowerFail:    //电源故障通知，0-off, 1-收信地址1, 2-收信地址2, 3-收信地址1&2, 默认：0
    case ID_NetWork_Email_SystemError:        //系统错误通知，0-off, 1-收信地址1, 2-收信地址2, 3-收信地址1&2, 默认：0
    case ID_NetWork_Email_SystemUserLock:     //用户锁定通知，0-off, 1-收信地址1, 2-收信地址2, 3-收信地址1&2, 默认：0
        return getNetWorkEmailSystemValObjStr(cfgData.toInt());
        break;

     case ID_NetWork_Email_TimerInterval_1:
     case ID_NetWork_Email_TimerInterval_2:
        return QString::number(getNetWorkEmailIntervalValObjStr(cfgData.toInt())) + " h";
        break;
    case ID_NetWork_Email_AlarmChannelNum:
        return cfgData + " / 50 CH";
    default:
        break;
    }
    return cfgData;
}

/*
 * 通过email的配置值，得到该配置值的描述
*/
QString CfgConfig_Network::getNetWorkEmailSystemValObjStr(quint32 type)
{
    switch (type) {
    case 0:
        return tr("Off");
        break;
    case 1:
        return tr("Recipient 1");
        break;
    case 2:
        return tr("Recipient 2");
        break;
    case 3:
        return tr("Recipient 1 & Recipient 2");
        break;
    default:
        break;
    }
    return NULL;
}

/*
 * 通过email的周期配置值，得到该配置值的描述
*/
quint32 CfgConfig_Network::getNetWorkEmailIntervalValObjStr(quint32 type)
{
    switch (type) {
    case EMAILInteral_1h:
        return 1;
        break;
    case EMAILInteral_2h:
        return 2;
        break;
    case EMAILInteral_3h:
        return 3;
        break;
    case EMAILInteral_4h:
        return 4;
        break;
    case EMAILInteral_6h:
        return 6;
        break;
    case EMAILInteral_8h:
        return 8;
        break;
    case EMAILInteral_12h:
        return 12;
        break;
    case EMAILInteral_24h:
        return 24;
        break;
    default:
        break;
    }
    return 0;
}


//sntp
//network sntp data
QVariant CfgConfig_Network::getNetWorkSNTPData(quint32 ID, quint32 /*group*/)
{
    NETWORK_CONFIG_SNTPCLIENT network = p_deviceConfig->networkSNTP;
    switch (ID) {
    case ID_NetWork_SNTP_ClientOnOff:       //SNTP功能开关，0-off, 1-on
        return network.clientOnOff;
        break;
    case ID_NetWork_SNTP_QueryInterval:     //查询间隔，单位:h, 范围：6/12/24h
    {
        int data = network.queryInterval;
        if (data == 6)
            return 0;
        else if (data == 12)
            return 1;
        else if (data == 24)
            return 2;
    }
        break;
    case ID_NetWork_SNTP_QueryTimeout:      //查询超时时间，单位：s,范围：10/30/90s
    {
        int data = network.queryTimeout;
        if (data == 10)
            return 0;
        else if (data == 30)
            return 1;
        else if (data == 90)
            return 2;
    }
        return network.queryTimeout;
        break;
    case ID_NetWork_SNTP_QueryAdjust:           //记录开始时，进行对时，0-off, 1-on
        return network.adjustAtStart;
        break;
    case ID_NetWork_SNTP_ServerPort:            //服务器端口号，1~65535,默认：123
        return network.SNTPServerPort;
        break;
    case ID_NetWork_SNTP_ServerName:        //服务器名，最多64个半角字符
        return getCharToString(network.SNTPServerName);
        break;
    default:
        break;
    }
    return QVariant();
}

//network sntp obj str
QString CfgConfig_Network::getNetWorkSNTPObjStr(quint32 ID, quint32 /*group*/)
{
    switch (ID) {
    case ID_NetWork_SNTP:
        return tr("SNTP client settings");
        break;
    case ID_NetWork_SNTP_Client:
        return tr("SNTP client function");
        break;
    case ID_NetWork_SNTP_ClientOnOff:           //SNTP功能开关，0-off, 1-on
        return tr("On/Off");
        break;
    case ID_NetWork_SNTP_Query:
        return tr("Query action");
        break;
    case ID_NetWork_SNTP_QueryInterval:     //查询间隔，单位:h, 范围：6/12/24h
        return tr("Interval");
        break;
    case ID_NetWork_SNTP_QueryTimeout:      //查询超时时间，单位：s,范围：10/30/90s
        return tr("Timeout");
        break;
    case ID_NetWork_SNTP_QueryAdjust:       //记录开始时，进行对时，0-off, 1-on
        return tr("Time adjust on start action");
        break;
    case ID_NetWork_SNTP_Server:
        return tr("SNTP server");
        break;
    case ID_NetWork_SNTP_ServerPort:        //服务器端口号，1~65535,默认：123
        return tr("SNTP server number");
        break;
    case ID_NetWork_SNTP_ServerName:        //服务器名，最多64个半角字符
        return tr("SNTP server name");
        break;
    default:
        break;
    }
    return NULL;
}



//network sntp obj value str
QString CfgConfig_Network::getNetWorkSNTPValueObjStr(quint32 ID, quint32 /*group*/, ConfigObject *cfg)
{
    QVariant cfgData = cfg->getData();
    switch (ID) {
    case ID_NetWork_SNTP_ClientOnOff:       //SNTP功能开关，0-off, 1-on
    case ID_NetWork_SNTP_QueryAdjust:           //记录开始时，进行对时，0-off, 1-on
        return cfgData.toInt() == 0 ? tr("Off") : tr("On");
        break;

    case ID_NetWork_SNTP_QueryInterval:     //查询间隔，单位:h, 范围：6/12/24h
    {
        int type = cfgData.toInt();
        if (type == 0)
            return tr("6 h");
        else if (type == 1)
            return tr("12 h");
        else if (type == 2)
            return tr("24 h");
    }
        break;
    case ID_NetWork_SNTP_QueryTimeout:      //查询超时时间，单位：s,范围：10/30/90s
    {
        int type = cfgData.toInt();
        if (type == 0)
            return tr("10 s");
        else if (type == 1)
            return tr("30 s");
        else if (type == 2)
            return tr("90 s");
    }
        break;
    default:
        break;
    }
    return cfgData.toString();
}

//modbus
//network modbus data
QVariant CfgConfig_Network::getNetWorkModbusData(quint32 ID, quint32 group)
{
    NETWORK_CONFIG_MODBUSCLIENT network = p_deviceConfig->networkModbus;
    switch (ID) {
    case ID_NetWork_Modbus_BasicClientOnOff:        //Modbus客户端功能开关，0-off, 1-on
        return network.clientOnOff;
        break;

    case ID_NetWork_Modbus_BasicCommInterval:  //采样间隔，单位:ms,范围：100ms/200ms/500ms/1s/2s/5s/10s/20s/30s/1min,默认：1s
        return getNetworkModbusBasicCommInterval(network.commInterval);
        break;
    case ID_NetWork_Modbus_BasicRecWaitTime: //通信恢复等待时间，单位:s，范围：0(off)/5s/10s/30s/1min/2min/5min,默认:2min
        return getNetworkModbusBasicRecTimeData(network.recoveryTime);
        break;
    case ID_NetWork_Modbus_BasicConnectOnOff: //保持连接开关，0-off, 1-on
        return network.connectHold;
        break;
    case ID_NetWork_Modbus_BasicConnectTime: //保持连接时间，单位：s,范围：1~10
        return network.connectTime;
        break;

    case ID_NetWork_Modbus_ServerNO:
        return group + 1;
    case ID_NetWork_Modbus_ServerName:        //服务器名，最多64个半角字符
        return getCharToString(network.serverName[group]);
        break;
    case ID_NetWork_Modbus_Serverport:        //服务器端口号，1~65535,默认：502
        return network.serverPort[group];
        break;

    case ID_NetWork_Modbus_CmdNO:
        return group + 1;
        break;
    case ID_NetWork_Modbus_CmdSettingType:      //命令类型：0-off, 1-读取， 2-写入
        return network.cmdType[group];
        break;
    case ID_NetWork_Modbus_CmdSettingServer:         //命令对应的服务器号，范围:1~32
        return network.cmdServeNo[group];
        break;
    case ID_NetWork_Modbus_CmdSettingUnitNO:         //单元号:1~255, Modbus网关功能时使用
        return network.cmdServeCell[group];
        break;
    case ID_NetWork_Modbus_CmdSettingRegType:         //寄存器类型
        return network.cmdRegType[group];
        break;
    case ID_NetWork_Modbus_CmdSettingRegNO:         //寄存器号
        return network.cmdRegNo[group];
        break;
    case ID_NetWork_Modbus_CmdSettingChType:        //通道类型，读取时：通信通道；写入时：输入输出通道/运算通道/通信通道
        return network.cmdChType[group];
        break;
    case ID_NetWork_Modbus_CmdSettingStartNO:         //与通道类型匹配的起始通道号
    {
        quint32 ch = network.cmdStartNo[group];
        if (ch <= 300)
            return CfgChannel::instance()->getExistChannelComm().first();
        else
            return ch;
    }
        break;
    case ID_NetWork_Modbus_CmdSettingEndNo:         //与通道类型匹配的结束通道号
    {
        quint32 ch = network.cmdEndNo[group];
        if (ch <= 300)
            return CfgChannel::instance()->getExistChannelComm().first();
        else
            return ch;
    }
        break;
    default:
        break;
    }
    return QVariant();
}

QVariant CfgConfig_Network::getNetworkModbusBasicCommInterval(quint32 data)
{
    switch (data) {
    case 100:
        return NetworkModbusInterval_100ms;
        break;
    case 200:
        return NetworkModbusInterval_200ms;
        break;
    case 500:
        return NetworkModbusInterval_500ms;
        break;
    case 1000:
        return NetworkModbusInterval_1s;
        break;
    case 2000:
        return NetworkModbusInterval_2s;
        break;
    case 5000:
        return NetworkModbusInterval_5s;
        break;
    case 10000:
        return NetworkModbusInterval_10s;
        break;
    case 20000:
        return NetworkModbusInterval_20s;
        break;
    case 30000:
        return NetworkModbusInterval_30s;
        break;
    case 60000:
        return NetworkModbusInterval_1min;
        break;
    default:
        break;
    }
    return QVariant();
}

QVariant CfgConfig_Network::getNetworkModbusBasicRecTimeData(quint32 data)
{
    switch (data) {
    case 0:
        return 0;
        break;
    case 5:
        return NetworkModbusWaitTime_5s;
        break;
    case 10:
        return NetworkModbusWaitTime_10s;
        break;
    case 30:
        return NetworkModbusWaitTime_30s;
        break;
    case 60:
        return NetworkModbusWaitTime_1min;
        break;
    case 120:
        return NetworkModbusWaitTime_2min;
        break;
    case 300:
        return NetworkModbusWaitTime_5min;
        break;

    default:
        break;
    }
    return QVariant();
}

//network modbus obj str
QString CfgConfig_Network::getNetWorkModbusObjStr(quint32 ID, quint32 /*group*/)
{
    switch (ID) {
    case ID_NetWork_Modbus:
        return tr("Modbus client settings");
        break;

    case ID_NetWork_Modbus_Basic:
        return tr("Basic settings");
        break;
    case ID_NetWork_Modbus_BasicClient:
        return tr("Modbus client function");
        break;
    case ID_NetWork_Modbus_BasicClientOnOff:
        return tr("On/Off");
        break;

    case ID_NetWork_Modbus_BasicComm:
        return tr("Communication");
        break;
    case ID_NetWork_Modbus_BasicCommInterval://采样间隔，单位:ms,范围：100ms/200ms/500ms/1s/2s/5s/10s/20s/30s/1min,默认：1s
        return tr("Interval");
        break;

    case ID_NetWork_Modbus_BasicRecovery:
        return tr("Recovery action");
        break;
    case ID_NetWork_Modbus_BasicRecWaitTime://通信恢复等待时间，单位:s，范围：0(off)/5s/10s/30s/1min/2min/5min,默认:2min
        return tr("Wait time");
        break;

    case ID_NetWork_Modbus_BasicConnect:
        return tr("Connection");
        break;
    case ID_NetWork_Modbus_BasicConnectOnOff://保持连接开关，0-off, 1-on
        return tr("Keep connection");
        break;
    case ID_NetWork_Modbus_BasicConnectTime://保持连接时间，单位：s,范围：1~10
        return tr("Connection timeout");
        break;
    case ID_NetWork_Modbus_Server:              //title
    case ID_NetWork_Modbus_ServerSetting:  //group
        return tr("Modbus server settings");
        break;
    case ID_NetWork_Modbus_ServerNO:
        return tr("Server number");
        break;

    case ID_NetWork_Modbus_ServerName:      //服务器名，最多64个半角字符
        return tr("Server name");
        break;
    case ID_NetWork_Modbus_Serverport:  //服务器端口号，1~65535,默认：502
        return tr("Port number");
        break;

    case ID_NetWork_Modbus_Cmd:         //title
    case ID_NetWork_Modbus_CmdSetting:           //group
        return tr("Command settings");
        break;
    case ID_NetWork_Modbus_CmdNO:
        return tr("Client command number");
        break;

    case ID_NetWork_Modbus_CmdSettingType:      //命令类型：0-off, 1-读取， 2-写入
        return tr("Type");
        break;
    case ID_NetWork_Modbus_CmdSettingServer:        //命令对应的服务器号，范围:1~32
        return tr("Server");
        break;
    case ID_NetWork_Modbus_CmdSettingUnitNO:        //单元号:1~255, Modbus网关功能时使用
        return tr("Unit No.");
        break;
    case ID_NetWork_Modbus_CmdSettingRegType:       //寄存器类型
        return tr("Data type");
        break;
    case ID_NetWork_Modbus_CmdSettingRegNO:     //寄存器号
        return tr("Register");
        break;
    case ID_NetWork_Modbus_CmdSettingChType:    //通道类型，读取时：通信通道；写入时：输入输出通道/运算通道/通信通道
        return tr("Channel type");
        break;
    case ID_NetWork_Modbus_CmdSettingStartNO:   //与通道类型匹配的起始通道号
        return tr("First-CH");
        break;
    case ID_NetWork_Modbus_CmdSettingEndNo: //与通道类型匹配的结束通道号
        return tr("Last-CH");
        break;
    default:
        break;
    }
    return NULL;
}

//network modbus obj val str
QString CfgConfig_Network::getNetWorkModbusValueObjStr(quint32 ID, quint32 /*group*/, ConfigObject *cfg)
{
    QString cfgData = cfg->getData().toString();
    switch (ID) {
    case ID_NetWork_Modbus_BasicClientOnOff:
    case ID_NetWork_Modbus_BasicConnectOnOff:
        return cfgData.toInt() == 0 ? tr("Off") : tr("On");
        break;
    case ID_NetWork_Modbus_BasicCommInterval:   //采样间隔，单位:ms,范围：100ms/200ms/500ms/1s/2s/5s/10s/20s/30s/1min,默认：1s
        return getNetWorkModbusBasicInterval(cfgData.toInt());
        break;
    case ID_NetWork_Modbus_BasicRecWaitTime:  //通信恢复等待时间，单位:s，范围：0(off)/5s/10s/30s/1min/2min/5min,默认:2min
        return getNetWorkModbusBasicRecTime(cfgData.toInt());
        break;
    case ID_NetWork_Modbus_BasicConnectTime: //保持连接时间，单位：s,范围：1~10
        return cfgData + " s";
        break;
    case ID_NetWork_Modbus_CmdSettingType:
    {
        int type = cfgData.toInt();
        if (type == 0)
            return tr("Off");
        else if (type == 1)
            return tr("Read");
        else if (type == 2)
            return tr("Write");
    }
        break;
    case ID_NetWork_Modbus_CmdSettingRegType:       //寄存器类型
        return getNetworkModbusRegTypeValStr(cfgData.toInt());
        break;
    case ID_NetWork_Modbus_CmdSettingChType:    //通道类型，读取时：通信通道；写入时：输入输出通道/运算通道/通信通道
    {
        int type = cfgData.toInt();
        if (type == 1 || type == 2 || type == 3)
            return tr("I/O channel");
        else if (type == 4)
            return tr("Communication channel");
        else if (type ==5)
            return tr("Math channel");
    }
        break;
    case ID_NetWork_Modbus_CmdSettingStartNO:
    case ID_NetWork_Modbus_CmdSettingEndNo:
        return getNetworkModbusChValObjStr(cfg->getData().toUInt());
        break;
    case ID_NetWork_Modbus_CmdSettingServer:
        return QString::number(cfg->getData().toInt());
        break;
    default:
        break;
    }
    return cfgData;
}

/*
 * 通过modbus配置的基本周期值来得到配置值描述的字符串
*/
QString CfgConfig_Network::getNetWorkModbusBasicInterval(quint32 type)
{
    switch (type) {
    case NetworkModbusInterval_100ms:
        return tr("100 ms");
        break;
    case NetworkModbusInterval_200ms:
        return tr("200 ms");
        break;
    case NetworkModbusInterval_500ms:
        return tr("500 ms");
        break;
    case NetworkModbusInterval_1s:
        return tr("1 s");
        break;
    case NetworkModbusInterval_2s:
        return tr("2 s");
        break;
    case NetworkModbusInterval_5s:
        return tr("5 s");
        break;
    case NetworkModbusInterval_10s:
        return tr("10 s");
        break;
    case NetworkModbusInterval_20s:
        return tr("20 s");
        break;
    case NetworkModbusInterval_30s:
        return tr("30 s");
        break;
    case NetworkModbusInterval_1min:
        return tr("1 min");
        break;
    default:
        break;
    }
    return NULL;
}

/*
 * 通过modbus配置的恢复周期值来得到配置值描述的字符串
*/
QString CfgConfig_Network::getNetWorkModbusBasicRecTime(quint32 type)
{
    switch (type) {
    case NetworkModbusWaitTime_off:
        return tr("Off");
        break;
    case NetworkModbusWaitTime_5s:
        return tr("5 s");
        break;
    case NetworkModbusWaitTime_10s:
        return tr("10 s");
        break;
    case NetworkModbusWaitTime_30s:
        return tr("30 s");
        break;
    case NetworkModbusWaitTime_1min:
        return tr("1 min");
        break;
    case NetworkModbusWaitTime_2min:
        return tr("2 min");
        break;
    case NetworkModbusWaitTime_5min:
        return tr("5 min");
        break;
    default:
        break;
    }
    return NULL;
}



/*
 * 通道通道编码得到界面上显示的相应通道编码的样式
 *          code是从底层配置结构体中获取，此时code是一个quint32类型的数字
*/
QString CfgConfig_Network::getNetworkModbusChValObjStr(quint32 code)
{
//    if (code == 0)
//        return "0001";
    int type = CHANNEL_TYPE(code);
    QString str;
    switch (type) {
    case Channel_Type_AI:
        return CfgChannel::instance()->getExistChannelAI().key(code);
        break;
    case Channel_Type_DI:
        return CfgChannel::instance()->getExistChannelDI().key(code);
        break;
    case Channel_Type_DO:
        return CfgChannel::instance()->getExistChannelDO().key(code);
        break;
    case Channel_Type_Comm:
    {
        str = CfgChannel::instance()->getExistChannelComm().key(code);
        str = "0" + str.right(3);
    }
        break;
    case Channel_Type_Math:
    {
        str = CfgChannel::instance()->getExistChannelMath().key(code);
        str = "0" + str.right(3);
    }
        break;
    default:
        break;
    }

    return str;
}


//network modbus reg type obj str
QString CfgConfig_Network::getNetworkModbusRegTypeValStr(quint32 type)
{
    switch (type) {
    case Register_Type_INT16:
        return "INT16";
        break;
    case Register_Type_UINT16:
        return "UINT16";
        break;
    case Register_Type_INT32B:
        return "INT32_B";
        break;
    case Register_Type_INT32L:
        return "INT32_L";
        break;
    case Register_Type_UINT32B:
        return "UINT32_B";
        break;
    case Register_Type_UINT32L:
        return "UINT32_L";
        break;
//    case Register_Type_FLOAT:
//        return "FLOAT";
//        break;
    case Register_Type_FLOATB:
        return "FLOAT_B";
        break;
    case Register_Type_FLOATL:
        return "FLOAT_L";
        break;
    case Register_Type_BIT:
        return "BIT";
        break;
    default:
        break;
    }
    return NULL;
}

//network server data
QVariant CfgConfig_Network::getNetWorkServerData(quint32 ID, quint32 /*group*/)
{
    NETWORK_CONFIG_SERVER network = p_deviceConfig->networkServer;
    switch (ID) {
    case ID_NetWork_Server_Modbus_ModbusServerLimit:    //Modbus服务连接限制开关
        return network.serverModbusLimit;
        break;
    case ID_NetWork_Server_Modbus_ModbusServerTimeOut:        //Modbus服务器延迟响应，单位：ms,范围：0(off)/10ms/20ms/50ms
    {
        int type = network.serverModbusTimeOut;
        if (type == 0)
            return 0;
        else if (type == 10)
            return 1;
        else if (type == 20)
            return 2;
        else if (type == 50)
            return 3;
    }

        break;

    case ID_NetWork_Server_Modbus_ModbusServerLimSwitch_1:        //限制开关
        return network.serverModbusLimitSwitch[0];
        break;
    case ID_NetWork_Server_Modbus_ModbusServerLimIP_1:
        return  getNetWorkBasicIpAddrData(network.serverModbusLimitIP[0]);
        break;
    case ID_NetWork_Server_Modbus_ModbusServerLimSwitch_2:        //限制开关
        return network.serverModbusLimitSwitch[1];
        break;
    case ID_NetWork_Server_Modbus_ModbusServerLimIP_2:
        return getNetWorkBasicIpAddrData(network.serverModbusLimitIP[1]);
        break;
    case ID_NetWork_Server_Modbus_ModbusServerLimSwitch_3:        //限制开关
        return network.serverModbusLimitSwitch[2];
        break;
    case ID_NetWork_Server_Modbus_ModbusServerLimIP_3:
        return getNetWorkBasicIpAddrData(network.serverModbusLimitIP[2]);
        break;
    case ID_NetWork_Server_Modbus_ModbusServerLimSwitch_4:        //限制开关
        return network.serverModbusLimitSwitch[3];
        break;
    case ID_NetWork_Server_Modbus_ModbusServerLimIP_4:
        return getNetWorkBasicIpAddrData(network.serverModbusLimitIP[3]);
        break;
    case ID_NetWork_Server_Modbus_ModbusServerLimSwitch_5:        //限制开关
        return network.serverModbusLimitSwitch[4];
        break;
    case ID_NetWork_Server_Modbus_ModbusServerLimIP_5:
        return getNetWorkBasicIpAddrData(network.serverModbusLimitIP[4]);
        break;
    case ID_NetWork_Server_Modbus_ModbusServerLimSwitch_6:        //限制开关
        return network.serverModbusLimitSwitch[5];
        break;
    case ID_NetWork_Server_Modbus_ModbusServerLimIP_6:
        return getNetWorkBasicIpAddrData(network.serverModbusLimitIP[5]);
        break;
    case ID_NetWork_Server_Modbus_ModbusServerLimSwitch_7:        //限制开关
        return network.serverModbusLimitSwitch[6];
        break;
    case ID_NetWork_Server_Modbus_ModbusServerLimIP_7:
        return getNetWorkBasicIpAddrData(network.serverModbusLimitIP[6]);
        break;
    case ID_NetWork_Server_Modbus_ModbusServerLimSwitch_8:        //限制开关
        return network.serverModbusLimitSwitch[7];
        break;
    case ID_NetWork_Server_Modbus_ModbusServerLimIP_8:
        return getNetWorkBasicIpAddrData(network.serverModbusLimitIP[7]);
        break;
    case ID_NetWork_Server_Modbus_ModbusServerLimSwitch_9:        //限制开关
        return network.serverModbusLimitSwitch[8];
        break;
    case ID_NetWork_Server_Modbus_ModbusServerLimIP_9:
        return getNetWorkBasicIpAddrData(network.serverModbusLimitIP[8]);
        break;
    case ID_NetWork_Server_Modbus_ModbusServerLimSwitch_10:        //限制开关
        return network.serverModbusLimitSwitch[9];
        break;
    case ID_NetWork_Server_Modbus_ModbusServerLimIP_10:
        return getNetWorkBasicIpAddrData(network.serverModbusLimitIP[9]);
        break;

    case ID_NetWork_Server_List_FTPOnOff:            //FTP服务器开关，0-off, 1-on
        return network.serverFTPOnOff;
        break;
    case ID_NetWork_Server_List_FTPEncrypt:         //FTP加密开关， 0-off, 1-on
        return network.serverFTPEncrypt;
        break;
    case ID_NetWork_Server_List_FTPConnectHole:         //FTP服务器保持有效功能，0-off, 1-on
        return network.serverFTPConnectHole;
        break;
    case ID_NetWork_Server_List_FTPPort:
        return network.serverFTPPort;
        break;

    case ID_NetWork_Server_List_HTTPOnOff:            //HTTP服务开关，0-off, 1-on
        return network.serverHTTPOnOff;
        break;
    case ID_NetWork_Server_List_HTTPEncrypt:         //HTTP加密开关， 0-off, 1-on
        return network.serverHTTPEncrypt;
        break;
    case ID_NetWork_Server_List_HTTPPort:
        return network.serverHTTPPort;
        break;

    case ID_NetWork_Server_List_SNTPOnOff:            //SNTP服务开关，0-off, 1-on
        return network.serverSNTPOnOff;
        break;
    case ID_NetWork_Server_List_SNTPPort:
        return network.serverSNTPPort;
        break;

    case ID_NetWork_Server_List_ModbusOnOff:        //Modbus服务开关，0-off, 1-on
        return network.serverModbusOnOff;
        break;
    case ID_NetWork_Server_List_ModbusPort:
        return network.serverModbusPort;
        break;
    default:
        break;
    }
    return QVariant();
}



//network server obj str
QString CfgConfig_Network::getNetWorkServerObjStr(quint32 ID, quint32 /*group = 0*/)
{
    switch (ID) {
    case ID_NetWork_Server:
        return tr("Server settings");
        break;

    case ID_NetWork_Server_Modbus:
        return tr("Allowed modbus clients");
        break;
    case ID_NetWork_Server_Modbus_ModbusServer:
        return tr("Modbus client connect limits function");
        break;
    case ID_NetWork_Server_Modbus_ModbusServerTimeOut:    //Modbus服务器延迟响应，单位：ms,范围：0(off)/10ms/20ms/50ms
        return tr("Modbus delay response");
        break;

    case ID_NetWork_Server_Modbus_ModbusServerLim_1:
        return "1";
        break;
    case ID_NetWork_Server_Modbus_ModbusServerLim_2:
        return "2";
        break;
    case ID_NetWork_Server_Modbus_ModbusServerLim_3:
        return "3";
        break;
    case ID_NetWork_Server_Modbus_ModbusServerLim_4:
        return "4";
        break;
    case ID_NetWork_Server_Modbus_ModbusServerLim_5:
        return "5";
        break;
    case ID_NetWork_Server_Modbus_ModbusServerLim_6:
        return "6";
        break;
    case ID_NetWork_Server_Modbus_ModbusServerLim_7:
        return "7";
        break;
    case ID_NetWork_Server_Modbus_ModbusServerLim_8:
        return "8";
        break;
    case ID_NetWork_Server_Modbus_ModbusServerLim_9:
        return "9";
        break;
    case ID_NetWork_Server_Modbus_ModbusServerLim_10:
        return "10";
        break;
    case ID_NetWork_Server_Modbus_ModbusServerLimit:    //Modbus服务连接限制开关
    case ID_NetWork_Server_Modbus_ModbusServerLimSwitch_1:
    case ID_NetWork_Server_Modbus_ModbusServerLimSwitch_2:
    case ID_NetWork_Server_Modbus_ModbusServerLimSwitch_3:
    case ID_NetWork_Server_Modbus_ModbusServerLimSwitch_4:
    case ID_NetWork_Server_Modbus_ModbusServerLimSwitch_5:
    case ID_NetWork_Server_Modbus_ModbusServerLimSwitch_6:
    case ID_NetWork_Server_Modbus_ModbusServerLimSwitch_7:
    case ID_NetWork_Server_Modbus_ModbusServerLimSwitch_8:
    case ID_NetWork_Server_Modbus_ModbusServerLimSwitch_9:
    case ID_NetWork_Server_Modbus_ModbusServerLimSwitch_10:
    case ID_NetWork_Server_List_FTPOnOff:       //FTP服务器开关，0-off, 1-on
    case ID_NetWork_Server_List_HTTPOnOff:
    case ID_NetWork_Server_List_SNTPOnOff:
    case ID_NetWork_Server_List_ModbusOnOff:
        return tr("On/Off");
        break;
    case ID_NetWork_Server_Modbus_ModbusServerLimIP_1:
    case ID_NetWork_Server_Modbus_ModbusServerLimIP_2:
    case ID_NetWork_Server_Modbus_ModbusServerLimIP_3:
    case ID_NetWork_Server_Modbus_ModbusServerLimIP_4:
    case ID_NetWork_Server_Modbus_ModbusServerLimIP_5:
    case ID_NetWork_Server_Modbus_ModbusServerLimIP_6:
    case ID_NetWork_Server_Modbus_ModbusServerLimIP_7:
    case ID_NetWork_Server_Modbus_ModbusServerLimIP_8:
    case ID_NetWork_Server_Modbus_ModbusServerLimIP_9:
    case ID_NetWork_Server_Modbus_ModbusServerLimIP_10:
        return tr("IP Address");
        break;

    case ID_NetWork_Server_List:
        return tr("Server list");
        break;

    case ID_NetWork_Server_List_FTP:
        return tr("FTP");
        break;
    case ID_NetWork_Server_List_HTTP:
        return tr("HTTP");
        break;
    case ID_NetWork_Server_List_SNTP:
        return tr("SNTP");
        break;
    case ID_NetWork_Server_List_Modbus:
        return tr("Modbus");
        break;
    case ID_NetWork_Server_List_FTPConnectHole:     //FTP服务器保持有效功能，0-off, 1-on
        return tr("Keep alive function");
        break;
    case ID_NetWork_Server_List_FTPEncrypt:      //FTP加密开关， 0-off, 1-on
    case ID_NetWork_Server_List_HTTPEncrypt:       //HTTP加密开关， 0-off, 1-on
        return tr("Encryption");
        break;
    case ID_NetWork_Server_List_FTPPort:
    case ID_NetWork_Server_List_HTTPPort:
    case ID_NetWork_Server_List_SNTPPort:
    case ID_NetWork_Server_List_ModbusPort:
        return tr("Port number");
        break;
    default:
        break;
    }
    return NULL;
}


//network server val obj str
QString CfgConfig_Network::getNetWorkServerValueObjStr(quint32 ID, quint32 /*group*/, ConfigObject *cfg)
{
    QString cfgData = cfg->getData().toString();
    switch (ID) {
    case ID_NetWork_Server_Modbus_ModbusServerLimit:    //Modbus服务连接限制开关
    case ID_NetWork_Server_Modbus_ModbusServerLimSwitch_1:
    case ID_NetWork_Server_Modbus_ModbusServerLimSwitch_2:
    case ID_NetWork_Server_Modbus_ModbusServerLimSwitch_3:
    case ID_NetWork_Server_Modbus_ModbusServerLimSwitch_4:
    case ID_NetWork_Server_Modbus_ModbusServerLimSwitch_5:
    case ID_NetWork_Server_Modbus_ModbusServerLimSwitch_6:
    case ID_NetWork_Server_Modbus_ModbusServerLimSwitch_7:
    case ID_NetWork_Server_Modbus_ModbusServerLimSwitch_8:
    case ID_NetWork_Server_Modbus_ModbusServerLimSwitch_9:
    case ID_NetWork_Server_Modbus_ModbusServerLimSwitch_10:
    case ID_NetWork_Server_List_FTPOnOff:
    case ID_NetWork_Server_List_HTTPOnOff:
    case ID_NetWork_Server_List_SNTPOnOff:
    case ID_NetWork_Server_List_ModbusOnOff:
    case ID_NetWork_Server_List_FTPEncrypt:
    case ID_NetWork_Server_List_HTTPEncrypt:
    case ID_NetWork_Server_List_FTPConnectHole: //FTP服务器保持有效功能，0-off, 1-on
        return cfgData.toInt() == 0 ? tr("Off") : tr("On");
        break;

    case ID_NetWork_Server_Modbus_ModbusServerTimeOut:   //Modbus服务器延迟响应，单位：ms,范围：0(off)/10ms/20ms/50ms
    {
        int type = cfgData.toInt();
        if (type == 0)
            return tr("Off");
        else
            return cfgData + "ms";
    }
        break;
    default:
        break;
    }
    return cfgData;
}

void CfgConfig_Network::setNetWorkBasicData(quint32 ID, quint32 /*group*/)
{
    NETWORK_CONFIG_BASIC * network = &p_deviceConfig->networkBasic;
    switch (ID) {
    case ID_NetWork_Basic_AutoIPSetting_Value:
        network->autoIP = ConfigTreeObject::getConfigData(Network_Basic_Settings_AutoIP).toInt();
        break;
    case ID__NetWork_Basic_AutoDNSSetting_Vlaue:
        network->autoDNS = ConfigTreeObject::getConfigData(Network_Basic_Settings_AutoDNS).toInt();
        break;

    case ID__NetWork_Basic_IPAddress_Value:
        setIPAddress(network->ipAddress, ConfigTreeObject::getConfigData(Network_Basic_Settings_IPAddr).toString());
        break;
    case ID__NetWork_Basic_SubNetMask_Value:
        setIPAddress(network->subnetMask, ConfigTreeObject::getConfigData(Network_Basic_Settings_IPMask).toString());
        break;
    case ID__NetWork_Basic_DefaultGateway_Value:
        setIPAddress(network->defaultGateway, ConfigTreeObject::getConfigData(Network_Basic_Settings_Gateway).toString());
        break;
    case ID_NetWork_Basic_DNSSetting_First:
        setIPAddress(network->DNSAddrFirst, ConfigTreeObject::getConfigData(Network_Basic_Settings_DNSPri).toString());
        break;
    case ID_NetWork_Basic_DNSSetting_Second:
        setIPAddress(network->DNSAddrSecond, ConfigTreeObject::getConfigData(Network_Basic_Settings_DNSSec).toString());
        break;
    default:
        break;
    }
}



//save network basic
void CfgConfig_Network::setNetWorkData(quint32 ID, quint32 group)
{
    if (ID >= NetWork_Basic_Start_ID && ID <= NetWork_Basic_End_ID)
        setNetWorkBasicData(ID, group);
    else if (ID >= NetWork_FTP_Start_ID && ID <= NetWork_FTP_End_ID)
        setNetWorkFTPData(ID, group);
    else if (ID >= NetWork_SMTP_Start_ID && ID <= NetWork_SMTP_End_ID)
        setNetWorkSMTPData(ID, group);
    else if (ID >= NetWork_Email_Start_ID && ID <= NetWork_Email_End_ID)
        setNetWorkEmailData(ID, group);
    else if (ID >= NetWork_Email_Start_ID && ID <= NetWork_Email_End_ID)
        setNetWorkEmailData(ID, group);
    else if (ID >= NetWork_SNTP_Start_ID && ID <= NetWork_SNTP_End_ID)
        setNetWorkSNTPData(ID, group);
    else if (ID >= NetWork_Modbus_Start_ID && ID <= NetWork_Modbus_End_ID)
        setNetWorkModbusData(ID, group);
    else if (ID >= NetWork_Server_Start_ID && ID <= NetWork_Server_End_ID)
        setNetWorkServerData(ID, group);
}

//save network ftp
void CfgConfig_Network::setNetWorkFTPData(quint32 ID, quint32 /*group*/)
{
    NETWORK_CONFIG_FTPCLIENT * network = &p_deviceConfig->networkFTP;
    switch (ID) {
    case ID_NetWork_FTP_Client_OnOff:
        network->clientOnOff = ConfigTreeObject::getConfigData(NETWORK_FTP_Settings_FUNCTION).toInt();
        break;
    case ID_NetWork_FTP_Transfer_DataDispEvent:
        network->dataDisplayEvent = ConfigTreeObject::getConfigData(NETWORK_FTP_Settings_TRANSFERFILE_DISPLAY).toInt();
        break;
    case ID_NetWork_FTP_Transfer_DataReport:
        network->dataReport = ConfigTreeObject::getConfigData(NETWORK_FTP_Settings_TRANSFERFILE_REPORT).toInt();
        break;
    case ID_NetWork_FTP_Transfer_DataManualSample:
        network->dataManualSample = ConfigTreeObject::getConfigData(NETWORK_FTP_Settings_TRANSFERFILE_DATA).toInt();
        break;
    case ID_NetWork_FTP_Transfer_DataAlarm:
        network->dataAlarm = ConfigTreeObject::getConfigData(NETWORK_FTP_Settings_TRANSFERFILE_ALARM).toInt();
        break;
    case ID_NetWork_FTP_Transfer_DataSnapshot:
        network->dataSnapshot = ConfigTreeObject::getConfigData(NETWORK_FTP_Settings_TRANSFERFILE_SNAP).toInt();
        break;
    case ID_NetWork_FTP_Transfer_DataSetting:
        network->dataSetting = ConfigTreeObject::getConfigData(NETWORK_FTP_Settings_TRANSFERFILE_DEVICE).toInt();
        break;

    case ID_NetWork_FTP_TransferWait_DispEvent:
        network->delayDisplayEvent = ConfigTreeObject::getConfigData(NETWORK_FTP_Settings_TRANSFERWAIT_DISPLAY).toInt();
        break;
    case ID_NetWork_FTP_TransferWait_Report:
        network->delayReport = ConfigTreeObject::getConfigData(NETWORK_FTP_Settings_TRANSFERWAIT_REPORT).toInt();
        break;

    case ID_NetWork_FTP_TransferEncry_SSL:
        network->encryptionSSL = ConfigTreeObject::getConfigData(NETWORK_FTP_Settings_ENCRYPTION_ENCRYPTION).toInt();
        break;
    case ID_NetWork_FTP_TransferEncry_Verity:
        network->encryptionVerify = ConfigTreeObject::getConfigData(NETWORK_FTP_Settings_ENCRYPTION_VERFICATION).toInt();
        break;

    case ID_NetWork_FTP__ServerFirst_PASV:       //PASV模式，0-off,1-on,默认:0
        network->serverPASVMode[0] = ConfigTreeObject::getConfigData(NETWORK_FTP_Settings_PRI_MODE).toInt();
        break;
    case ID_NetWork_FTP__ServerFirst_Port:      //端口号，1~65535, 默认：21
        network->serverPort[0] = ConfigTreeObject::getConfigData(NETWORK_FTP_Settings_PRI_PORT).toInt();
        break;
    case ID_NetWork_FTP__ServerFirst_Name:      //FTP服务器名，最多64个半角字符
        setStrToChar(network->serverName[0], ConfigTreeObject::getConfigData(NETWORK_FTP_Settings_PRI_SERVER).toString(), 64);
        break;
    case ID_NetWork_FTP__ServerFirst_userName:
        setStrToChar(network->serverUserName[0],ConfigTreeObject::getConfigData(NETWORK_FTP_Settings_PRI_NAME).toString(), 32);
        break;
    case ID_NetWork_FTP__ServerFirst_Pwd:
        setStrToChar(network->serverPasswd[0],ConfigTreeObject::getConfigData(NETWORK_FTP_Settings_PRI_PASSWD).toString(), 32);
        break;
    case ID_NetWork_FTP__ServerFirst_Directory:
       setStrToChar(network->serverDirectory[0],ConfigTreeObject::getConfigData(NETWORK_FTP_Settings_PRI_DIRECTION).toString(), 64);
        break;

    case ID_NetWork_FTP__ServerSecond_PASV:       //PASV模式，0-off,1-on,默认:0
        network->serverPASVMode[1] = ConfigTreeObject::getConfigData(NETWORK_FTP_Settings_SEC_MODE).toInt();
        break;
    case ID_NetWork_FTP__ServerSecond_Port:      //端口号，1~65535, 默认：21
        network->serverPort[1] = ConfigTreeObject::getConfigData(NETWORK_FTP_Settings_SEC_PORT).toInt();
        break;
    case ID_NetWork_FTP__ServerSecond_Name:      //FTP服务器名，最多64个半角字符
        setStrToChar(network->serverName[1], ConfigTreeObject::getConfigData(NETWORK_FTP_Settings_SEC_SERVER).toString(), 64);
        break;
    case ID_NetWork_FTP__ServerSecond_userName:
        setStrToChar(network->serverUserName[1],ConfigTreeObject::getConfigData(NETWORK_FTP_Settings_SEC_NAME).toString(), 32);
        break;
    case ID_NetWork_FTP__ServerSecond_Pwd:
        setStrToChar(network->serverPasswd[1],ConfigTreeObject::getConfigData(NETWORK_FTP_Settings_SEC_PASSWD).toString(), 32);
        break;
    case ID_NetWork_FTP__ServerSecond_Directory:
       setStrToChar(network->serverDirectory[1],ConfigTreeObject::getConfigData(NETWORK_FTP_Settings_SEC_DIRECTORY).toString(), 64);
        break;
    default:
        break;
    }
}

//save smtp data
void CfgConfig_Network::setNetWorkSMTPData(quint32 ID, quint32 /*group*/)
{
    NETWORK_CONFIG_SMTPCLIENT * network = &p_deviceConfig->networkSTMP;
    switch (ID) {
    case ID_NetWork_SMTP_Client_OnOff:
        network->clientOnOff = ConfigTreeObject::getConfigData(NETWORK_SMTP_Settings_FUNCTION).toInt();
        break;
    case ID_NetWork_SMTP_Client_AuthType_Value:
        network->authType = (SMTP_Authentication_Type)ConfigTreeObject::getConfigData(NETWORK_SMTP_Settings_AUTH).toInt();
        break;
    case ID_NetWork_SMTP_Client_Encry_SSL:
        network->encryptionSSL = ConfigTreeObject::getConfigData(NETWORK_SMTP_Settings_ENCRYPTION).toInt();
        break;
    case ID_NetWork_SMTP_Client_Encry_Verify:   //证书验证加密，0-off, 1-on
        network->encryptionVerify = ConfigTreeObject::getConfigData(NETWORK_SMTP_ENCRYPTION_Settings_VERIFICATION).toInt();
        break;

    case ID_NetWork_SMTP_Client_Serverport:      //服务器端口号，1~65535,默认：25
        network->SMTPServerPort = ConfigTreeObject::getConfigData(NETWORK_SMTP_SMTP_Settings_PORT).toInt();
        break;
    case ID_NetWork_SMTP_Client_ServerName:
        setStrToChar(network->SMTPServerName, ConfigTreeObject::getConfigData(NETWORK_SMTP_SMTP_Settings_SERVER).toString(), 64);
        break;
    case ID_NetWork_SMTP_Client_ServerUserName:
        setStrToChar(network->SMTPServerUserName, ConfigTreeObject::getConfigData(NETWORK_SMTP_SMTP_Settings_NAME).toString(), 32);
        break;
    case ID_NetWork_SMTP_Client_ServerPwd:
        setStrToChar(network->SMTPServerPasswd, ConfigTreeObject::getConfigData(NETWORK_SMTP_SMTP_Settings_PASSWD).toString(), 32);
        break;

    case ID_NetWork_SMTP_Client_POP3ServerPort:     //POP3服务器端口号，1~65535,默认：110
        network->POP3ServePort = ConfigTreeObject::getConfigData(NETWORK_SMTP_POP3_Settings_PORT).toInt();
        break;
    case ID_NetWork_SMTP_Client_POP3ServerName:
        setStrToChar(network->POP3ServerName, ConfigTreeObject::getConfigData(NETWORK_SMTP_POP3_Settings_SERVER).toString(), 64);
        break;
    case ID_NetWork_SMTP_Client_POP3ServeruserName:
        setStrToChar(network->POP3ServerUserName, ConfigTreeObject::getConfigData(NETWORK_SMTP_POP3_Settings_NAME).toString(), 32);
        break;
    case ID_NetWork_SMTP_Client_POP3ServerPwd:
        setStrToChar(network->POP3ServerPasswd, ConfigTreeObject::getConfigData(NETWORK_SMTP_POP3_Settings_PASSWD).toString(), 32);
        break;
    default:
        break;
    }
}

//save email data
void CfgConfig_Network::setNetWorkEmailData(quint32 ID, quint32 /*group*/)
{
    NETWORK_CONFIG_EMAIL * network = &p_deviceConfig->networkEmail;
    switch (ID) {
    case ID_NetWork_Email_MailHeader_RecipientAddr_1:       //收信地址1/2字符串，最多150个半角字符
        setStrToChar(network->recipientAddr[0], ConfigTreeObject::getConfigData(NETWORK_EMAIL_Settings_HEADER_R1).toString(), 152);
        break;
    case ID_NetWork_Email_MailHeader_RecipientAddr_2:
        setStrToChar(network->recipientAddr[1], ConfigTreeObject::getConfigData(NETWORK_EMAIL_Settings_HEADER_R2).toString(), 152);
        break;
    case ID_NetWork_Email_MailHeader_SenderAddr:
        setStrToChar(network->senderAddr, ConfigTreeObject::getConfigData(NETWORK_EMAIL_Settings_HEADER_SENDER).toString(), 64);
        break;
    case ID_NetWork_Email_MailHeader_Subject:
        setStrToChar(network->subject, ConfigTreeObject::getConfigData(NETWORK_EMAIL_Settings_HEADER_SUBJECT).toString(), 32);
        break;
    case ID_NetWork_Email_ContentHeader:
        setStrToChar(network->header, ConfigTreeObject::getConfigData(NETWORK_EMAIL_Settings_CONTENTS_HEADER).toString(), 128);
        break;
    case ID_NetWork_Email_ContentIncludeSrcURL:
        network->includeSrcURL = ConfigTreeObject::getConfigData(NETWORK_EMAIL_Settings_CONTENTS_URL).toInt();
        break;
    case ID_NetWork_Email_ReportNotify:     //报表通知，0-off, 1-收信地址1, 2-收信地址2, 3-收信地址1&2
        network->reportNotify = ConfigTreeObject::getConfigData(NETWORK_EMAIL_Settings_ALARM_REPORT).toInt();
        break;

    case ID_NetWork_Email_TimerNotify:       //定时通知,1-on, 0-off
        network->timerNotify = ConfigTreeObject::getConfigData(NETWORK_EMAIL_Settings_SCHEDULED_NOTIFICATION).toInt();
        break;
    case ID_NetWork_Email_TimerAttachData:    //追加瞬时值,1-on, 0-off
        network->timerAttachData = ConfigTreeObject::getConfigData(NETWORK_EMAIL_Settings_SCHEDULED_DATA).toInt();
        break;

    case ID_NetWork_Email_TimerInterval_1:
        network->timerInterval[0] = getNetWorkEmailIntervalValObjStr(ConfigTreeObject::getConfigData(NETWORK_EMAIL_Settings_SCHEDULED_R1_INTERAL).toInt());
        break;
    case ID_NetWork_Email_TimerBaseHour_1:
        network->timerBaseHour[0] = ConfigTreeObject::getConfigData(NETWORK_EMAIL_Settings_SCHEDULED_R1_TIMEH).toInt();
        break;
    case ID_NetWork_Email_TimerBaseMin_1:
        network->timerBaseMinute[0] = ConfigTreeObject::getConfigData(NETWORK_EMAIL_Settings_SCHEDULED_R1_TIMEM).toInt();
        break;
    case ID_NetWork_Email_TimerInterval_2:
        network->timerInterval[1] = getNetWorkEmailIntervalValObjStr(ConfigTreeObject::getConfigData(NETWORK_EMAIL_Settings_SCHEDULED_R2_INTERAL).toInt());
        break;
    case ID_NetWork_Email_TimerBaseHour_2:
        network->timerBaseHour[1] = ConfigTreeObject::getConfigData(NETWORK_EMAIL_Settings_SCHEDULED_R2_TIMEH).toInt();
        break;
    case ID_NetWork_Email_TimerBaseMin_2:
        network->timerBaseMinute[1] = ConfigTreeObject::getConfigData(NETWORK_EMAIL_Settings_SCHEDULED_R2_TIMEM).toInt();
        break;

    case ID_NetWork_Email_AlarmNotify:
        network->alarmNotify = ConfigTreeObject::getConfigData(NETWORK_EMAIL_Settings_ALARM_NOTIFICATION).toInt();
        break;
    case ID_NetWork_Email_AlarmAttachData:
        network->alarmAttachData = ConfigTreeObject::getConfigData(NETWORK_EMAIL_Settings_ALARM_DATA).toInt();
        break;
    case ID_NetWork_Email_AlarmAttachTagCh:
        network->alarmAttachTagCh = ConfigTreeObject::getConfigData(NETWORK_EMAIL_Settings_ALARM_SUBJECT).toInt();
        break;
    case ID_NetWork_Email_AlarmChannelNum:
        network->alarmChannelNum = ConfigTreeObject::getConfigData(NETWORK_EMAIL_Settings_ALARM_SET).toInt();
        break;
    case ID_NetWork_Email_AlarmChannelItem:
    {
        QList<QVariant> channelList = ConfigTreeObject::getConfigData(NETWORK_EMAIL_Settings_ALARM_CHANELLIST).toList();
        QList<QVariant>::iterator var;
        memset(network->alarmChannelItem, 0, 50 * sizeof(quint32)); //清空数组
        int i = 0;
        for(var = channelList.begin(); var != channelList.end(); var++)
            network->alarmChannelItem[i++] = (*var).toUInt();
    }
        break;

    case ID_NetWork_Email_SystemMemFull:
        network->systemMemFull = ConfigTreeObject::getConfigData(NETWORK_EMAIL_Settings_SYSTEM_MEMORY).toInt();
        break;
    case ID_NetWork_Email_SystemPowerFail:
        network->systemPowerFail = ConfigTreeObject::getConfigData(NETWORK_EMAIL_Settings_SYSTEM_POWER).toInt();
        break;
    case ID_NetWork_Email_SystemError:
        network->systemError = ConfigTreeObject::getConfigData(NETWORK_EMAIL_Settings_SYSTEM_ERROR).toInt();
        break;
    case ID_NetWork_Email_SystemUserLock:
        network->systemUserLock = ConfigTreeObject::getConfigData(NETWORK_EMAIL_Settings_SYSTEM_USER).toInt();
        break;
    default:
        break;
    }
}

//save sntp data
void CfgConfig_Network::setNetWorkSNTPData(quint32 ID, quint32 /*group*/)
{
    NETWORK_CONFIG_SNTPCLIENT * network = &p_deviceConfig->networkSNTP;
    switch (ID) {
    case ID_NetWork_SNTP_ClientOnOff:
        network->clientOnOff = ConfigTreeObject::getConfigData(NETWORK_SNTP_Settings_FUNCTION).toInt();
        break;
    case ID_NetWork_SNTP_QueryInterval:     //查询间隔，单位:h, 范围：6/12/24h
    {
        int type = ConfigTreeObject::getConfigData(NETWORK_SNTP_Settings_ACTION_INTERVAL).toInt();
        if (type == 0)
            network->queryInterval = 6;
        else if (type == 1)
            network->queryInterval = 12;
        else if (type == 2)
            network->queryInterval = 24;
    }
        break;
    case ID_NetWork_SNTP_QueryTimeout:
    {
        int type = ConfigTreeObject::getConfigData(NETWORK_SNTP_Settings_ACTION_TIMEOUT).toInt();
        if (type == 0)
            network->queryTimeout = 10;
        else if (type == 1)
            network->queryTimeout = 30;
        else if (type == 2)
            network->queryTimeout = 90;
    }
        break;
    case ID_NetWork_SNTP_QueryAdjust:
        network->adjustAtStart = ConfigTreeObject::getConfigData(NETWORK_SNTP_Settings_ACTION_TIME).toInt();
        break;
    case ID_NetWork_SNTP_ServerPort:
        network->SNTPServerPort = ConfigTreeObject::getConfigData(NETWORK_SNTP_Settings_SERVER_POER).toInt();
        break;
    case ID_NetWork_SNTP_ServerName:
        setStrToChar(network->SNTPServerName, ConfigTreeObject::getConfigData(NETWORK_SNTP_Settings_SERVER_NAME).toString(), 64);
        break;
    default:
        break;
    }
}

//save modbus data
void CfgConfig_Network::setNetWorkModbusData(quint32 ID, quint32 group)
{
    NETWORK_CONFIG_MODBUSCLIENT * network = &p_deviceConfig->networkModbus;
    switch (ID) {
    case ID_NetWork_Modbus_BasicClientOnOff:        //Modbus客户端功能开关，0-off, 1-on
        network->clientOnOff = ConfigTreeObject::getConfigData(NETWORK_MODBUS_Settings_BASIC_FUNCTION).toInt();
        break;
    case ID_NetWork_Modbus_BasicCommInterval:
        setNetWorkModbusBasicCommInterval(network->commInterval);
        break;
    case ID_NetWork_Modbus_BasicRecWaitTime:
        setNetWorkModbusBasicRecTime(network->recoveryTime);
        break;
    case ID_NetWork_Modbus_BasicConnectOnOff:   //保持连接开关，0-off, 1-on
        network->connectHold = ConfigTreeObject::getConfigData(NETWORK_MODBUS_Settings_BASIC_CONNECT).toInt();
        break;
    case ID_NetWork_Modbus_BasicConnectTime:
        network->connectTime = ConfigTreeObject::getConfigData(NETWORK_MODBUS_Settings_BASIC_CONNECT_TIMEOUT).toInt();
        break;

    case ID_NetWork_Modbus_ServerName:      //server
        setStrToChar(network->serverName[group], ConfigTreeObject::getConfigData(QString(NETWORK_MODBUS_Settings_SERVER_SERVERNAME).arg(group)).toString(), 64);
        break;
    case ID_NetWork_Modbus_Serverport:
        network->serverPort[group] = ConfigTreeObject::getConfigData(QString(NETWORK_MODBUS_Settings_SERVER_SERVERPORT).arg(group)).toInt();
        break;


    case ID_NetWork_Modbus_CmdSettingServer:
        network->cmdServeNo[group] = ConfigTreeObject::getConfigData(QString(NETWORK_MODBUS_Settings_COMMAND_SERVER).arg(group)).toInt();
        break;
    case ID_NetWork_Modbus_CmdSettingUnitNO:
        network->cmdServeCell[group] = ConfigTreeObject::getConfigData(QString(NETWORK_MODBUS_Settings_COMMAND_UNIT).arg(group)).toInt();
        break;
    case ID_NetWork_Modbus_CmdSettingRegType:
        network->cmdRegType[group] = (Register_Type)ConfigTreeObject::getConfigData(QString(NETWORK_MODBUS_Settings_COMMAND_DATATYPE).arg(group)).toInt();
        break;
    case ID_NetWork_Modbus_CmdSettingRegNO:
        network->cmdRegNo[group] = ConfigTreeObject::getConfigData(QString(NETWORK_MODBUS_Settings_COMMAND_REGISTER).arg(group)).toInt();
        break;

    case ID_NetWork_Modbus_CmdSettingType:      //cmd
        network->cmdType[group] = ConfigTreeObject::getConfigData(QString(NETWORK_MODBUS_Settings_COMMAND_TYPE).arg(group)).toUInt();
    case ID_NetWork_Modbus_CmdSettingChType:
        setModbusChType(group, network->cmdChType[group]);
    case ID_NetWork_Modbus_CmdSettingStartNO:
    case ID_NetWork_Modbus_CmdSettingEndNo:
    {
        network->cmdStartNo[group] = ConfigTreeObject::getConfigData(QString(NETWORK_MODBUS_Settings_COMMAND_FIRSTCH).arg(group)).toUInt();
        network->cmdEndNo[group] = ConfigTreeObject::getConfigData(QString(NETWORK_MODBUS_Settings_COMMAND_LASTCH).arg(group)).toUInt();
    }
        break;
    default:
        break;
    }
}

void CfgConfig_Network::setNetWorkModbusBasicCommInterval(quint32 &time)
{
    int type  = ConfigTreeObject::getConfigData(NETWORK_MODBUS_Settings_BASIC_INTERVAL).toInt();
    switch (type) {
    case NetworkModbusInterval_100ms:
        time = 100;
        break;
    case NetworkModbusInterval_200ms:
        time = 200;
        break;
    case NetworkModbusInterval_500ms:
        time = 500;
        break;
    case NetworkModbusInterval_1s:
        time = 1000;
        break;
    case NetworkModbusInterval_2s:
        time = 2000;
        break;
    case NetworkModbusInterval_5s:
        time = 5000;
        break;
    case NetworkModbusInterval_10s:
        time = 10000;
        break;
    case NetworkModbusInterval_20s:
        time = 20000;
        break;
    case NetworkModbusInterval_30s:
        time = 30;
        break;
    case NetworkModbusInterval_1min:
        time = 60000;
        break;
    default:
        break;
    }
}

void CfgConfig_Network::setNetWorkModbusBasicRecTime(quint32 &time)
{
    int type = ConfigTreeObject::getConfigData(NETWORK_MODBUS_Settings_BASIC_RECOVER).toInt();
    switch (type) {
    case NetworkModbusWaitTime_off:
        time = 0;
        break;
    case NetworkModbusWaitTime_5s:
        time = 5;
        break;
    case NetworkModbusWaitTime_10s:
        time = 10;
        break;
    case NetworkModbusWaitTime_30s:
       time = 30;
        break;
    case NetworkModbusWaitTime_1min:
        time = 60;
        break;
    case NetworkModbusWaitTime_2min:
        time = 120;
        break;
    case NetworkModbusWaitTime_5min:
        time = 300;
        break;
    default:
        break;
    }
}

//save server data
void CfgConfig_Network::setNetWorkServerData(quint32 ID, quint32 /*group*/)
{
    NETWORK_CONFIG_SERVER * network = &p_deviceConfig->networkServer;
    switch (ID) {
    case ID_NetWork_Server_Modbus_ModbusServerLimit:        //Modbus服务连接限制开关
        network->serverModbusLimit = ConfigTreeObject::getConfigData(NETWORK_SERVER_Settings_MODBUS_FUNCTIOM).toInt();
        break;
    case ID_NetWork_Server_Modbus_ModbusServerLimSwitch_1:        //限制开关，
        network->serverModbusLimitSwitch[0] = ConfigTreeObject::getConfigData(NETWORK_SERVER_Settings_MODBUS_1_SWITCH).toInt();
        break;
    case ID_NetWork_Server_Modbus_ModbusServerLimIP_1:         //限制IP, IPv4
        setIPAddress(network->serverModbusLimitIP[0], ConfigTreeObject::getConfigData(NETWORK_SERVER_Settings_MODBUS_1_IP).toString());
        break;
    case ID_NetWork_Server_Modbus_ModbusServerLimSwitch_2:        //限制开关，
        network->serverModbusLimitSwitch[1] = ConfigTreeObject::getConfigData(NETWORK_SERVER_Settings_MODBUS_2_SWITCH).toInt();
        break;
    case ID_NetWork_Server_Modbus_ModbusServerLimIP_2:         //限制IP, IPv4
        setIPAddress(network->serverModbusLimitIP[1], ConfigTreeObject::getConfigData(NETWORK_SERVER_Settings_MODBUS_2_IP).toString());
        break;
    case ID_NetWork_Server_Modbus_ModbusServerLimSwitch_3:        //限制开关，
        network->serverModbusLimitSwitch[2] = ConfigTreeObject::getConfigData(NETWORK_SERVER_Settings_MODBUS_3_SWITCH).toInt();
        break;
    case ID_NetWork_Server_Modbus_ModbusServerLimIP_3:         //限制IP, IPv4
        setIPAddress(network->serverModbusLimitIP[2], ConfigTreeObject::getConfigData(NETWORK_SERVER_Settings_MODBUS_3_IP).toString());
        break;
    case ID_NetWork_Server_Modbus_ModbusServerLimSwitch_4:        //限制开关，
        network->serverModbusLimitSwitch[3] = ConfigTreeObject::getConfigData(NETWORK_SERVER_Settings_MODBUS_4_SWITCH).toInt();
        break;
    case ID_NetWork_Server_Modbus_ModbusServerLimIP_4:         //限制IP, IPv4
        setIPAddress(network->serverModbusLimitIP[3], ConfigTreeObject::getConfigData(NETWORK_SERVER_Settings_MODBUS_4_IP).toString());
        break;
    case ID_NetWork_Server_Modbus_ModbusServerLimSwitch_5:        //限制开关，
        network->serverModbusLimitSwitch[4] = ConfigTreeObject::getConfigData(NETWORK_SERVER_Settings_MODBUS_5_SWITCH).toInt();
        break;
    case ID_NetWork_Server_Modbus_ModbusServerLimIP_5:         //限制IP, IPv4
        setIPAddress(network->serverModbusLimitIP[4], ConfigTreeObject::getConfigData(NETWORK_SERVER_Settings_MODBUS_5_IP).toString());
        break;
    case ID_NetWork_Server_Modbus_ModbusServerLimSwitch_6:        //限制开关，
        network->serverModbusLimitSwitch[5] = ConfigTreeObject::getConfigData(NETWORK_SERVER_Settings_MODBUS_6_SWITCH).toInt();
        break;
    case ID_NetWork_Server_Modbus_ModbusServerLimIP_6:         //限制IP, IPv4
        setIPAddress(network->serverModbusLimitIP[5], ConfigTreeObject::getConfigData(NETWORK_SERVER_Settings_MODBUS_6_IP).toString());
        break;
    case ID_NetWork_Server_Modbus_ModbusServerLimSwitch_7:        //限制开关，
        network->serverModbusLimitSwitch[6] = ConfigTreeObject::getConfigData(NETWORK_SERVER_Settings_MODBUS_7_SWITCH).toInt();
        break;
    case ID_NetWork_Server_Modbus_ModbusServerLimIP_7:         //限制IP, IPv4
        setIPAddress(network->serverModbusLimitIP[6], ConfigTreeObject::getConfigData(NETWORK_SERVER_Settings_MODBUS_7_IP).toString());
    case ID_NetWork_Server_Modbus_ModbusServerLimSwitch_8:        //限制开关，
        network->serverModbusLimitSwitch[7] = ConfigTreeObject::getConfigData(NETWORK_SERVER_Settings_MODBUS_8_SWITCH).toInt();
        break;
    case ID_NetWork_Server_Modbus_ModbusServerLimIP_8:         //限制IP, IPv4
        setIPAddress(network->serverModbusLimitIP[7], ConfigTreeObject::getConfigData(NETWORK_SERVER_Settings_MODBUS_8_IP).toString());
        break;
    case ID_NetWork_Server_Modbus_ModbusServerLimSwitch_9:        //限制开关，
        network->serverModbusLimitSwitch[8] = ConfigTreeObject::getConfigData(NETWORK_SERVER_Settings_MODBUS_9_SWITCH).toInt();
        break;
    case ID_NetWork_Server_Modbus_ModbusServerLimIP_9:         //限制IP, IPv4
        setIPAddress(network->serverModbusLimitIP[8], ConfigTreeObject::getConfigData(NETWORK_SERVER_Settings_MODBUS_9_IP).toString());
        break;
    case ID_NetWork_Server_Modbus_ModbusServerLimSwitch_10:        //限制开关，
        network->serverModbusLimitSwitch[9] = ConfigTreeObject::getConfigData(NETWORK_SERVER_Settings_MODBUS_10_SWITCH).toInt();
        break;
    case ID_NetWork_Server_Modbus_ModbusServerLimIP_10:         //限制IP, IPv4
        setIPAddress(network->serverModbusLimitIP[9], ConfigTreeObject::getConfigData(NETWORK_SERVER_Settings_MODBUS_10_IP).toString());
        break;

    case ID_NetWork_Server_List_FTPOnOff:         //FTP服务器开关，0-off, 1-on
        network->serverFTPOnOff = ConfigTreeObject::getConfigData(NETWORK_SERVER_Settings_SETTING_FTP_FUNCTION).toInt();
        break;
    case ID_NetWork_Server_List_FTPEncrypt:
        network->serverFTPEncrypt = ConfigTreeObject::getConfigData(NETWORK_SERVER_Settings_SETTING_FTP_ENCYPTION).toInt();
        break;
    case ID_NetWork_Server_List_FTPConnectHole:
        network->serverFTPConnectHole = ConfigTreeObject::getConfigData(NETWORK_SERVER_Settings_SETTING_FTP_KEEP).toInt();
        break;
    case ID_NetWork_Server_List_FTPPort:
        network->serverFTPPort = ConfigTreeObject::getConfigData(NETWORK_SERVER_Settings_SETTING_FTP_PORT).toInt();
        break;

    case ID_NetWork_Server_List_HTTPOnOff:              //http
        network->serverHTTPOnOff = ConfigTreeObject::getConfigData(NETWORK_SERVER_Settings_SETTING_HTTP_FUNCTION).toInt();
        break;
    case ID_NetWork_Server_List_HTTPEncrypt:
        network->serverHTTPEncrypt = ConfigTreeObject::getConfigData(NETWORK_SERVER_Settings_SETTING_HTTP_ENCYPTION).toInt();
        break;
    case ID_NetWork_Server_List_HTTPPort:
        network->serverHTTPPort = ConfigTreeObject::getConfigData(NETWORK_SERVER_Settings_SETTING_HTTP_PORT).toInt();
        break;

    case ID_NetWork_Server_List_SNTPOnOff:              //sntp
        network->serverSNTPOnOff = ConfigTreeObject::getConfigData(NETWORK_SERVER_Settings_SETTING_SNTP_FUNCTION).toInt();
        break;
    case ID_NetWork_Server_List_SNTPPort:
        network->serverSNTPPort = ConfigTreeObject::getConfigData(NETWORK_SERVER_Settings_SETTING_SNTP_PORT).toInt();
        break;

    case ID_NetWork_Server_List_ModbusOnOff:              //modbus
        network->serverModbusOnOff = ConfigTreeObject::getConfigData(NETWORK_SERVER_Settings_SETTING_MODBUS_FUNCTION).toInt();
        break;
    case ID_NetWork_Server_List_ModbusPort:
        network->serverModbusPort = ConfigTreeObject::getConfigData(NETWORK_SERVER_Settings_SETTING_MODBUS_PORT).toInt();
        break;
    case ID_NetWork_Server_Modbus_ModbusServerTimeOut:
        network->serverModbusTimeOut = ConfigTreeObject::getConfigData(NETWORK_SERVER_Settings_SETTING_MODBUS_DELAY).toInt();
        break;
    default:
        break;
    }
}


/*
 * 把字符串IP地址转化为数组形式存放
*/
void CfgConfig_Network::setIPAddress(quint8 *IP, QString IPStr)
{
    QStringList str = IPStr.split(".");
    for (int i = 0; i < str.size(); i++)
        *IP++ = str[i].toInt();
}

void CfgConfig_Network::setStrToChar(uchar *uData, QString str, int len)
{
    memset(uData, 0, len);
    QByteArray baUnit = str.toLocal8Bit();
    char* valueUnit = baUnit.data();
//    memcpy(uData, valueUnit, /*strlen(valueUnit)*/len);
    while (*valueUnit != '\0') {
        *uData++ = *valueUnit++;
    }
}


/*
 * 设置板卡类型
*/
void CfgConfig_Network::setModbusChType(quint32 group, Channel_Type &chType)
{
    Channel_Type type = (Channel_Type)ConfigTreeObject::getConfigData(QString(NETWORK_MODBUS_Settings_COMMAND_CHANELTYPE).arg(group)).toInt();
    if (type == Channel_Type_Off || type == Channel_Type_Comm || type == Channel_Type_Math)
        chType = type;
    else
    {
        quint32 channel = ConfigTreeObject::getConfigData(QString(NETWORK_MODBUS_Settings_COMMAND_FIRSTCH).arg(group)).toUInt();
        chType = (Channel_Type)CHANNEL_TYPE(channel);
    }
}

