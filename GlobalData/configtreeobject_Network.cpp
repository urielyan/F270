#include "configtreeobject_Network.h"
#include "cfgchannel.h"
#include "../Common/winnetworkconfig.h"
#include "../Common/xmlconfigparser.h"
#include "cfgGlobalDef.h"
#include "Common/devboard.h"
#include "cfgconfig_network.h"


ConfigTreeObject_Network *ConfigTreeObject_Network::instance(QObject *parent)
{
    static ConfigTreeObject_Network data(parent);
    return &data;
}

ConfigTreeObject_Network::ConfigTreeObject_Network(QObject *parent):
      ConfigObject(parent)
{
    //Network settings代理树
    QFile fileDelegate(":/configtree/Image/configTree/Network/Network_SettingDelegate/delegate.xml");
    if(!fileDelegate.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file: Network_setting!";
    }else{
        XmlConfigParser::parseConfigXml(fileDelegate, static_cast<ConfigObject*>(parent), \
                                                                   (QObject*)this);
        fileDelegate.close();
    }
}

void ConfigTreeObject_Network::updateStrShowOn()
{
    updateStrShowOn_basic();
    updateStrShowOn_FTP();
    updateStrShowOn_SMTP();
    updateStrShowOn_EMAIL();
    updateStrShowOn_SNTP();
    updateStrShowOn_Server();
    updateStrShowOn_Modbus();
}

void ConfigTreeObject_Network::updateStrShowOn_basic()
{
    slotBasicAutoIPChange();
    slotBasicAutoDNSChange();
    slotBasicIPAddrChange();
    slotBasicIPMaskChange();
    slotBasicIPGatewayChange();
    slotBasicDNSPriChange();
    slotBasicDNSSecChange();

}

void ConfigTreeObject_Network::updateStrShowOn_FTP()
{
    slotFTPFunctionChange();
    slotFTPTransferFileDisplayChange();
    slotFTPTransferFileReportChange();
    slotFTPTransferFileDataChange();
    slotFTPTransferFileAlarmChange();
    slotFTPTransferFileSnapChange();
    slotFTPTransferFileDeviceChange();
    slotFTPEncrptionChange();
    slotFTPEncrptionVerficationChange();
    slotFTPTransferWaitDisplayChange();
    slotFTPTransferWaitReportChange();
    slotFTPPriPortChange();
    slotFTPPriModeChange();
    slotFTPPriServerChange();
    slotFTPPriNameChange();
    slotFTPPriPasswdChange();
    slotFTPPriDirectionChange();
    slotFTPSecPortChange();
    slotFTPSecModeChange();
    slotFTPSecServerChange();
    slotFTPSecNameChange();
    slotFTPSecPasswdChange();
    slotFTPSecDirectionChange();
}

void ConfigTreeObject_Network::updateStrShowOn_SMTP()
{
    slotSMTPFunctionChange();
    slotSMTPAuthChange();
    slotSMTPEncryptionChange();
    slotSMTPEncryptionVerifcationChange();
    slotSMTPSMTPPortChange();
    slotSMTPSMTPServerChange();
    slotSMTPSMTPNameChange();
    slotSMTPSMTPPasswdChange();
    slotSMTPPOP3PortChange();
    slotSMTPPOP3ServerChange();
    slotSMTPPOP3NameChange();
    slotSMTPPOP3PasswdChange();
}

void ConfigTreeObject_Network::updateStrShowOn_EMAIL()
{
    slotEMAILHeaderR1Change();
    slotEMAILHeaderR2Change();
    slotEMAILHeaderSenderChange();
    slotEMAILHeaderSubjectChange();

    slotEMAILContentsHeaderChange();
    slotEMAILContentsURLChange();

    slotEMAILAlarmNotificationChange();
    slotEMAILAlarmDataChange();
    slotEMAILAlarmSubjectChange();
    slotEMAILAlarmSetChange();
    slotEMAILAlarmReportChange();

    slotEMAILScheduledNotificationChange();
    slotEMAILScheduledDataChange();
    slotEMAILScheduledR1InteralChange();
    slotEMAILScheduledR1TimeHChange();
    slotEMAILScheduledR1TimeMChange();
    slotEMAILScheduledR2InteralChange();
    slotEMAILScheduledR2TimeHChange();
    slotEMAILScheduledR2TimeMChange();

    slotEMAILSystemMemoryChange();
    slotEMAILSystemPowerChange();
    slotEMAILSystemErrorChange();
    slotEMAILUserChange();
}

void ConfigTreeObject_Network::updateStrShowOn_SNTP()
{
    slotSNTPFunctionChange();
    slotSNTPServerPortChange();
    slotSNTPServerNameChange();
    slotSNTPActionIntervalChange();
    slotSNTPActionTimeoutChange();
    slotSNTPActionTimeChange();
}

void ConfigTreeObject_Network::updateStrShowOn_Server()
{
    slotServerModbusFunctionChange();

    slotServerModbus1SwitchChange();
    slotServerModbus1IPChange();
    slotServerModbus2SwitchChange();
    slotServerModbus2IPChange();
    slotServerModbus3SwitchChange();
    slotServerModbus3IPChange();
    slotServerModbus4SwitchChange();
    slotServerModbus4IPChange();
    slotServerModbus5SwitchChange();
    slotServerModbus5IPChange();
    slotServerModbus6SwitchChange();
    slotServerModbus6IPChange();
    slotServerModbus7SwitchChange();
    slotServerModbus7IPChange();
    slotServerModbus8SwitchChange();
    slotServerModbus8IPChange();
    slotServerModbus9SwitchChange();
    slotServerModbus9IPChange();
    slotServerModbus10SwitchChange();
    slotServerModbus10IPChange();

    slotServerSettingFTPFunctionChange();
    slotServerSettingFTPEncyptionChange();
    slotServerSettingFTPPortChange();
    slotServerSettingFTPKeepChange();

    slotServerSettingHTTPFunctionChange();
    slotServerSettingHTTPEncyptionChange();
    slotServerSettingHTTPPortChange();

    slotServerSettingSNTPFunctionChange();
    slotServerSettingSNTPPortChange();

    slotServerSettingModbusFunctionChange();
    slotServerSettingModbusPortChange();
    slotServerSettingModbusDelayChange();
}

void ConfigTreeObject_Network::updateStrShowOn_Modbus()
{
    slotNetworkModbusBasicFunctionChange();
    slotNetworkModbusBasicInteralChange();
    slotNetworkModbusBasicWaitTimeChange();
    slotNetworkModbusBasicConnectChange();
    slotNetworkModbusBasicTimeoutChange();

    //slotNetworkModbusServerNumberChange();
    slotNetworkModbusServerServerNameChange();
    slotNetworkModbusServerServerPortChange();

    //slotNetworkModbusCommandNumberChange();
    slotNetworkModbusCommandTypeChange();
    slotNetworkModbusCommandServerChange();
    slotNetworkModbusCommandUnitChange();
    slotNetworkModbusCommandDataTypeChange();
    slotNetworkModbusCommandRegisterChange();
    slotNetworkModbusCommandChanelTypeChange();
    slotNetworkModbusCommandFirstCHChange();
    slotNetworkModbusCommandLastCHChange();

}

void ConfigTreeObject_Network::slotCreateNetworksettingTree(QObject *parent)
{
    //Network setting 配置树
    QFile file(":/configtree/Image/configTree/Network/Network_Setting/config.xml");
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file: Network_setting!";
    }else{
        XmlConfigParser::parseConfigXml(file, static_cast<ConfigObject*>(parent));
        file.close();
    }

    quint32 ID_server = ConfigTreeObject::getConfigObject(Modbus_ServerNum_Setting)->getObjectID();
    quint32 ID_command = ConfigTreeObject::getConfigObject(Modbus_CommandNum_Setting)->getObjectID();

    ConfigTreeObject::getConfigObject(Modbus_ServerNum_Setting)->initData(0, 0, ID_server);
    ConfigTreeObject::getConfigObject(Modbus_CommandNum_Setting)->initData(0, 0, ID_command);

    ConfigTreeObject::getConfigObject(Modbus_ServerNum_Setting)->setStrShowOn("1");
    ConfigTreeObject::getConfigObject(Modbus_CommandNum_Setting)->setStrShowOn("1");

    m_cmdNumChannelTypeFlag = false;
    m_cmdNumDataTypeFlag = false;
}

void ConfigTreeObject_Network::slotNetworkDelegateChange()
{
    QFile delegateChange(":/configtree/Image/configTree/Network/Network_DelegateChange/delegateChange.xml");
     if(!delegateChange.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file: AI_settingDelegateChange!";
     }else{
        XmlConfigParser::parseDelegateChangeXml(delegateChange);
        delegateChange.close();
     }

     _curStatus = false;
     updateStrShowOn();
     _curStatus = true;
}

void ConfigTreeObject_Network::slotBasicAutoIPChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(Network_Basic_Deledate_AutoIP)->getData();

    QFile file(XML_Network_Basic_AutoIP);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Network::slotBasicAutoDNSChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(Network_Basic_Deledate_AutoDNS)->getData();

    QFile file(XML_Network_Basic_AutoDNS);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Network::slotBasicIPAddrChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(Network_Basic_Deledate_IPAddr)->getData();

    QFile file(XML_Network_Basic_IPAddr);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }

    ConfigTreeObject::getConfigObject(Network_Basic_Deledate_IPMask)->changeData("0.0.0.0", false);
    ConfigTreeObject::getConfigObject(Network_Basic_Deledate_Gateway)->changeData("0.0.0.0", false);
}

void ConfigTreeObject_Network::slotBasicIPMaskChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(Network_Basic_Deledate_IPMask)->getData();

    QFile file(XML_Network_Basic_IPMask);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Network::slotBasicIPGatewayChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(Network_Basic_Deledate_Gateway)->getData();

    QFile file(XML_Network_Basic_IPGateway);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Network::slotBasicDNSPriChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(Network_Basic_Deledate_DNSPri)->getData();

    QFile file(XML_Network_Basic_DNSPri);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Network::slotBasicDNSSecChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(Network_Basic_Deledate_DNSSec)->getData();

    QFile file(XML_Network_Basic_DNSSec);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Network::slotFTPFunctionChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(NETWORK_FTP_Deledate_FUNCTION)->getData();

    QFile file(XML_Network_FTP_FUNCTION);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Network::slotFTPTransferFileDisplayChange()
{

    QVariant value = ConfigTreeObject::getConfigObject(NETWORK_FTP_Deledate_TRANSFERFILE_DISPLAY)->getData();

    QFile file(XML_Network_FTP_TRANSFERFILE_DISPLAY);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Network::slotFTPTransferFileReportChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(NETWORK_FTP_Deledate_TRANSFERFILE_REPORT)->getData();

    QFile file(XML_Network_FTP_TRANSFERFILE_REPORT);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Network::slotFTPTransferFileDataChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(NETWORK_FTP_Deledate_TRANSFERFILE_DATA)->getData();

    QFile file(XML_Network_FTP_TRANSFERFILE_DATA);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Network::slotFTPTransferFileAlarmChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(NETWORK_FTP_Deledate_TRANSFERFILE_ALARM)->getData();

    QFile file(XML_Network_FTP_TRANSFERFILE_ALARM);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Network::slotFTPTransferFileSnapChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(NETWORK_FTP_Deledate_TRANSFERFILE_SNAP)->getData();

    QFile file(XML_Network_FTP_TRANSFERFILE_SNAP);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Network::slotFTPTransferFileDeviceChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(NETWORK_FTP_Deledate_TRANSFERFILE_DEVICE)->getData();

    QFile file(XML_Network_FTP_TRANSFERFILE_DEVICE);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Network::slotFTPEncrptionChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(NETWORK_FTP_Deledate_ENCRYPTION_ENCRYPTION)->getData();

    QFile file(XML_Network_FTP_ENCRYPTION_ENCRYPTION);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Network::slotFTPEncrptionVerficationChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(NETWORK_FTP_Deledate_ENCRYPTION_VERFICATION)->getData();

    QFile file(XML_Network_FTP_ENCRYPTION_VERFICATION);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Network::slotFTPTransferWaitDisplayChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(NETWORK_FTP_Deledate_TRANSFERWAIT_DISPLAY)->getData();

    QFile file(XML_Network_FTP_TRANSFERWAIT_DISPLAY);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Network::slotFTPTransferWaitReportChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(NETWORK_FTP_Deledate_TRANSFERWAIT_REPORT)->getData();

    QFile file(XML_Network_FTP_TRANSFERWAIT_REPORT);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Network::slotFTPPriPortChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(NETWORK_FTP_Deledate_PRI_PORT)->getData();

    QFile file(XML_Network_FTP_PRI_PORT);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Network::slotFTPPriModeChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(NETWORK_FTP_Deledate_PRI_MODE)->getData();

    QFile file(XML_Network_FTP_PRI_MODE);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Network::slotFTPPriServerChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(NETWORK_FTP_Deledate_PRI_SERVER)->getData();

    QFile file(XML_Network_FTP_PRI_SERVER);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Network::slotFTPPriNameChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(NETWORK_FTP_Deledate_PRI_NAME)->getData();

    QFile file(XML_Network_FTP_PRI_NAME);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Network::slotFTPPriPasswdChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(NETWORK_FTP_Deledate_PRI_PASSWD)->getData();

    QFile file(XML_Network_FTP_PRI_PASSWD);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Network::slotFTPPriDirectionChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(NETWORK_FTP_Deledate_PRI_DIRECTION)->getData();

    QFile file(XML_Network_FTP_PRI_DIRECTION);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Network::slotFTPSecPortChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(NETWORK_FTP_Deledate_SEC_PORT)->getData();

    QFile file(XML_Network_FTP_SEC_PORT);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Network::slotFTPSecModeChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(NETWORK_FTP_Deledate_SEC_MODE)->getData();

    QFile file(XML_Network_FTP_SEC_MODE);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Network::slotFTPSecServerChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(NETWORK_FTP_Deledate_SEC_SERVER)->getData();

    QFile file(XML_Network_FTP_SEC_SERVER);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Network::slotFTPSecNameChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(NETWORK_FTP_Deledate_SEC_NAME)->getData();

    QFile file(XML_Network_FTP_SEC_NAME);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Network::slotFTPSecPasswdChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(NETWORK_FTP_Deledate_SEC_PASSWD)->getData();

    QFile file(XML_Network_FTP_SEC_PASSWD);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Network::slotFTPSecDirectionChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(NETWORK_FTP_Deledate_SEC_DIRECTORY)->getData();

    QFile file(XML_Network_FTP_SEC_DIRECTORY);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Network::slotSMTPFunctionChange()
{

    QVariant value = ConfigTreeObject::getConfigObject(NETWORK_SMTP_Deledate_FUNCTION)->getData();

    QFile file(XML_Network_SMTP_FUNCTION);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Network::slotSMTPAuthChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(NETWORK_SMTP_Deledate_AUTH)->getData();

    QFile file(XML_Network_SMTP_AUTH);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Network::slotSMTPEncryptionChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(NETWORK_SMTP_Deledate_ENCRYPTION)->getData();

    QFile file(XML_Network_SMTP_ENCRYPTION);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Network::slotSMTPEncryptionVerifcationChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(NETWORK_SMTP_ENCRYPTION_Deledate_VERIFICATION)->getData();

    QFile file(XML_Network_SMTP_VERFICATION);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Network::slotSMTPSMTPPortChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(NETWORK_SMTP_SMTP_Deledate_PORT)->getData();

    QFile file(XML_Network_SMTP_SMTP_PORT);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Network::slotSMTPSMTPServerChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(NETWORK_SMTP_SMTP_Deledate_SERVER)->getData();

    QFile file(XML_Network_SMTP_SMTP_SERVER);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Network::slotSMTPSMTPNameChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(NETWORK_SMTP_SMTP_Deledate_NAME)->getData();

    QFile file(XML_Network_SMTP_SMTP_USER);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Network::slotSMTPSMTPPasswdChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(NETWORK_SMTP_SMTP_Deledate_PASSWD)->getData();

    QFile file(XML_Network_SMTP_SMTP_PASSWD);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Network::slotSMTPPOP3PortChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(NETWORK_SMTP_POP3_Deledate_PORT)->getData();

    QFile file(XML_Network_SMTP_POP3_PORT);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Network::slotSMTPPOP3ServerChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(NETWORK_SMTP_POP3_Deledate_SERVER)->getData();

    QFile file(XML_Network_SMTP_POP3_SERVER);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Network::slotSMTPPOP3NameChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(NETWORK_SMTP_POP3_Deledate_NAME)->getData();

    QFile file(XML_Network_SMTP_POP3_USER);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Network::slotSMTPPOP3PasswdChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(NETWORK_SMTP_POP3_Deledate_PASSWD)->getData();

    QFile file(XML_Network_SMTP_POP3_PASSWD);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Network::slotEMAILHeaderR1Change()
{
    QVariant value = ConfigTreeObject::getConfigObject(NETWORK_EMAIL_Deledate_HEADER_R1)->getData();

    QFile file(XML_Network_EMAIL_HEADER_R1);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Network::slotEMAILHeaderR2Change()
{
    QVariant value = ConfigTreeObject::getConfigObject(NETWORK_EMAIL_Deledate_HEADER_R2)->getData();

    QFile file(XML_Network_EMAIL_HEADER_R2);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Network::slotEMAILHeaderSenderChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(NETWORK_EMAIL_Deledate_HEADER_SENDER)->getData();

    QFile file(XML_Network_EMAIL_HEADER_SENDER);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Network::slotEMAILHeaderSubjectChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(NETWORK_EMAIL_Deledate_HEADER_SUBJECT)->getData();

    QFile file(XML_Network_EMAIL_HEADER_SUBJECT);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Network::slotEMAILContentsHeaderChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(NETWORK_EMAIL_Deledate_CONTENTS_HEADER)->getData();

    QFile file(XML_Network_EMAIL_CONTENTS_HEADER);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Network::slotEMAILContentsURLChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(NETWORK_EMAIL_Deledate_CONTENTS_URL)->getData();

    QFile file(XML_Network_EMAIL_CONTENTS_URL);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Network::slotEMAILAlarmNotificationChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(NETWORK_EMAIL_Deledate_ALARM_NOTIFICATION)->getData();

    QFile file(XML_Network_EMAIL_ALARM_NOITIFICATION);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Network::slotEMAILAlarmDataChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(NETWORK_EMAIL_Deledate_ALARM_DATA)->getData();

    QFile file(XML_Network_EMAIL_ALARM_ATTACH);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Network::slotEMAILAlarmSubjectChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(NETWORK_EMAIL_Deledate_ALARM_SUBJECT)->getData();

    QFile file(XML_Network_EMAIL_ALARM_INCLUDE);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Network::slotEMAILAlarmSetChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(NETWORK_EMAIL_Deledate_ALARM_SET)->getData();

    QFile file(XML_Network_EMAIL_ALARM_CHANELSET);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Network::slotEMAILAlarmReportChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(NETWORK_EMAIL_Deledate_ALARM_REPORT)->getData();

    QFile file(XML_Network_EMAIL_REPORT_NOITIFICATION);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Network::slotEMAILScheduledNotificationChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(NETWORK_EMAIL_Deledate_SCHEDULED_NOTIFICATION)->getData();

    QFile file(XML_Network_EMAIL_SCHEEDULED_NOITIFICATION);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Network::slotEMAILScheduledDataChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(NETWORK_EMAIL_Deledate_SCHEDULED_DATA)->getData();

    QFile file(XML_Network_EMAIL_SCHEEDULED_ATTACH);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Network::slotEMAILScheduledR1InteralChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(NETWORK_EMAIL_Deledate_SCHEDULED_R1_INTERAL)->getData();

    QFile file(XML_Network_EMAIL_SCHEEDULED_R1_INTERVAL);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Network::slotEMAILScheduledR1TimeHChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(NETWORK_EMAIL_Deledate_SCHEDULED_R1_TIMEH)->getData();

    QFile file(XML_Network_EMAIL_SCHEEDULED_R1_HOUR);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Network::slotEMAILScheduledR1TimeMChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(NETWORK_EMAIL_Deledate_SCHEDULED_R1_TIMEM)->getData();

    QFile file(XML_Network_EMAIL_SCHEEDULED_R1_MINUTE);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Network::slotEMAILScheduledR2InteralChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(NETWORK_EMAIL_Deledate_SCHEDULED_R2_INTERAL)->getData();

    QFile file(XML_Network_EMAIL_SCHEEDULED_R2_INTERVAL);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Network::slotEMAILScheduledR2TimeHChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(NETWORK_EMAIL_Deledate_SCHEDULED_R2_TIMEH)->getData();

    QFile file(XML_Network_EMAIL_SCHEEDULED_R2_HOUR);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Network::slotEMAILScheduledR2TimeMChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(NETWORK_EMAIL_Deledate_SCHEDULED_R2_TIMEM)->getData();

    QFile file(XML_Network_EMAIL_SCHEEDULED_R2_MINUTE);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Network::slotEMAILSystemMemoryChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(NETWORK_EMAIL_Deledate_SYSTEM_MEMORY)->getData();

    QFile file(XML_Network_EMAIL_SYSTEM_MEMORY);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Network::slotEMAILSystemPowerChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(NETWORK_EMAIL_Deledate_SYSTEM_POWER)->getData();

    QFile file(XML_Network_EMAIL_SYSTEM_POWER);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Network::slotEMAILSystemErrorChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(NETWORK_EMAIL_Deledate_SYSTEM_ERROR)->getData();

    QFile file(XML_Network_EMAIL_SYSTEM_ERROR);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Network::slotEMAILUserChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(NETWORK_EMAIL_Deledate_SYSTEM_USER)->getData();

    QFile file(XML_Network_EMAIL_SYSTEM_NOITIFICATION);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Network::slotSNTPFunctionChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(NETWORK_SNTP_Deledate_FUNCTION)->getData();

    QFile file(XML_Network_SNTP_FUNCTION);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Network::slotSNTPServerPortChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(NETWORK_SNTP_Deledate_SERVER_POER)->getData();

    QFile file(XML_Network_SNTP_NUMBER);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Network::slotSNTPServerNameChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(NETWORK_SNTP_Deledate_SERVER_NAME)->getData();

    QFile file(XML_Network_SNTP_SERVER);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Network::slotSNTPActionIntervalChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(NETWORK_SNTP_Deledate_ACTION_INTERVAL)->getData();

    QFile file(XML_Network_SNTP_INTERVAL);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Network::slotSNTPActionTimeoutChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(NETWORK_SNTP_Deledate_ACTION_TIMEOUT)->getData();

    QFile file(XML_Network_SNTP_TIMEOUT);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Network::slotSNTPActionTimeChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(NETWORK_SNTP_Deledate_ACTION_TIME)->getData();

    QFile file(XML_Network_SNTP_TIMEADJUST);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Network::slotServerModbusFunctionChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(NETWORK_SERVER_Deledate_MODBUS_FUNCTIOM)->getData();

    QFile file(XML_Network_Server_Modbus_FUNCTION);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Network::slotServerModbus1SwitchChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(NETWORK_SERVER_Deledate_MODBUS_1_SWITCH)->getData();

    QFile file(XML_Network_Server_Modbus_1_SWTICH);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Network::slotServerModbus1IPChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(NETWORK_SERVER_Deledate_MODBUS_1_IP)->getData();

    QFile file(XML_Network_Server_Modbus_1_IP);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Network::slotServerModbus2SwitchChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(NETWORK_SERVER_Deledate_MODBUS_2_SWITCH)->getData();

    QFile file(XML_Network_Server_Modbus_2_SWTICH);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Network::slotServerModbus2IPChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(NETWORK_SERVER_Deledate_MODBUS_2_IP)->getData();

    QFile file(XML_Network_Server_Modbus_2_IP);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Network::slotServerModbus3SwitchChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(NETWORK_SERVER_Deledate_MODBUS_3_SWITCH)->getData();

    QFile file(XML_Network_Server_Modbus_3_SWTICH);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Network::slotServerModbus3IPChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(NETWORK_SERVER_Deledate_MODBUS_3_IP)->getData();

    QFile file(XML_Network_Server_Modbus_3_IP);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Network::slotServerModbus4SwitchChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(NETWORK_SERVER_Deledate_MODBUS_4_SWITCH)->getData();

    QFile file(XML_Network_Server_Modbus_4_SWTICH);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Network::slotServerModbus4IPChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(NETWORK_SERVER_Deledate_MODBUS_4_IP)->getData();

    QFile file(XML_Network_Server_Modbus_4_IP);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Network::slotServerModbus5SwitchChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(NETWORK_SERVER_Deledate_MODBUS_5_SWITCH)->getData();

    QFile file(XML_Network_Server_Modbus_5_SWTICH);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Network::slotServerModbus5IPChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(NETWORK_SERVER_Deledate_MODBUS_5_IP)->getData();

    QFile file(XML_Network_Server_Modbus_5_IP);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Network::slotServerModbus6SwitchChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(NETWORK_SERVER_Deledate_MODBUS_6_SWITCH)->getData();

    QFile file(XML_Network_Server_Modbus_6_SWTICH);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Network::slotServerModbus6IPChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(NETWORK_SERVER_Deledate_MODBUS_6_IP)->getData();

    QFile file(XML_Network_Server_Modbus_6_IP);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Network::slotServerModbus7SwitchChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(NETWORK_SERVER_Deledate_MODBUS_7_SWITCH)->getData();

    QFile file(XML_Network_Server_Modbus_7_SWTICH);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Network::slotServerModbus7IPChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(NETWORK_SERVER_Deledate_MODBUS_7_IP)->getData();

    QFile file(XML_Network_Server_Modbus_7_IP);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Network::slotServerModbus8SwitchChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(NETWORK_SERVER_Deledate_MODBUS_8_SWITCH)->getData();

    QFile file(XML_Network_Server_Modbus_8_SWTICH);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Network::slotServerModbus8IPChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(NETWORK_SERVER_Deledate_MODBUS_8_IP)->getData();

    QFile file(XML_Network_Server_Modbus_8_IP);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Network::slotServerModbus9SwitchChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(NETWORK_SERVER_Deledate_MODBUS_9_SWITCH)->getData();

    QFile file(XML_Network_Server_Modbus_9_SWTICH);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Network::slotServerModbus9IPChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(NETWORK_SERVER_Deledate_MODBUS_9_IP)->getData();

    QFile file(XML_Network_Server_Modbus_9_IP);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Network::slotServerModbus10SwitchChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(NETWORK_SERVER_Deledate_MODBUS_10_SWITCH)->getData();

    QFile file(XML_Network_Server_Modbus_10_SWTICH);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Network::slotServerModbus10IPChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(NETWORK_SERVER_Deledate_MODBUS_10_IP)->getData();

    QFile file(XML_Network_Server_Modbus_10_IP);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Network::slotServerSettingFTPFunctionChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(NETWORK_SERVER_Deledate_SETTING_FTP_FUNCTION)->getData();

    QFile file(XML_Network_Server_List_FTP_SWITCH);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Network::slotServerSettingFTPEncyptionChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(NETWORK_SERVER_Deledate_SETTING_FTP_ENCYPTION)->getData();

    QFile file(XML_Network_Server_List_FTP_ENCRYPTION);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Network::slotServerSettingFTPPortChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(NETWORK_SERVER_Deledate_SETTING_FTP_PORT)->getData();

    QFile file(XML_Network_Server_List_FTP_PORT);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Network::slotServerSettingFTPKeepChange()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(NETWORK_SERVER_Deledate_SETTING_FTP_KEEP);
    quint32 ID = ID_NetWork_Server_List_FTPConnectHole;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_Network::slotServerSettingHTTPFunctionChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(NETWORK_SERVER_Deledate_SETTING_HTTP_FUNCTION)->getData();

    QFile file(XML_Network_Server_List_HTTP_SWITCH);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Network::slotServerSettingHTTPEncyptionChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(NETWORK_SERVER_Deledate_SETTING_HTTP_ENCYPTION)->getData();

    QFile file(XML_Network_Server_List_HTTP_ENCRYPTION);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Network::slotServerSettingHTTPPortChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(NETWORK_SERVER_Deledate_SETTING_HTTP_PORT)->getData();

    QFile file(XML_Network_Server_List_HTTP_PORT);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Network::slotServerSettingSNTPFunctionChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(NETWORK_SERVER_Deledate_SETTING_SNTP_FUNCTION)->getData();

    QFile file(XML_Network_Server_List_SNTP_SWITCH);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Network::slotServerSettingSNTPPortChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(NETWORK_SERVER_Deledate_SETTING_SNTP_PORT)->getData();

    QFile file(XML_Network_Server_List_SNTP_PORT);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Network::slotServerSettingModbusFunctionChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(NETWORK_SERVER_Deledate_SETTING_MODBUS_FUNCTION)->getData();

    QFile file(XML_Network_Server_List_MODBUS_SWITCH);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Network::slotServerSettingModbusPortChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(NETWORK_SERVER_Deledate_SETTING_MODBUS_PORT)->getData();

    QFile file(XML_Network_Server_List_MODBUS_PORT);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Network::slotServerSettingModbusDelayChange()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(NETWORK_SERVER_Deledate_SETTING_MODBUS_DELAY);
    quint32 ID = ID_NetWork_Server_Modbus_ModbusServerTimeOut;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_Network::slotNetworkModbusBasicFunctionChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(NETWORK_MODBUS_Deledate_BASIC_FUNCTION)->getData();

    QFile file(XML_Network_Modbus_BASIC_FUNCTION);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Network::slotNetworkModbusBasicInteralChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(NETWORK_MODBUS_Deledate_BASIC_INTERVAL)->getData();

    QFile file(XML_Network_Modbus_BASIC_INTERVAL);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Network::slotNetworkModbusBasicWaitTimeChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(NETWORK_MODBUS_Deledate_BASIC_RECOVER)->getData();

    QFile file(XML_Network_Modbus_BASIC_RECOVER);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Network::slotNetworkModbusBasicConnectChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(NETWORK_MODBUS_Deledate_BASIC_CONNECT)->getData();

    QFile file(XML_Network_Modbus_BASIC_CONNECT);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Network::slotNetworkModbusBasicTimeoutChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(NETWORK_MODBUS_Deledate_BASIC_CONNECT_TIMEOUT)->getData();

    QFile file(XML_Network_Modbus_BASIC_CONNECT_TIMEOUT);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Network::slotNetworkModbusServerNumberChange()
{
    ConfigObject *pObj = ConfigTreeObject::getConfigObject(NETWORK_MODBUS_Deledate_SERVER_NUMBER);

    if(pObj){
        quint32 num = pObj->getData().toUInt();
        pObj->setStrShowOn(QString("%1").arg(num+1));
    }
}

void ConfigTreeObject_Network::slotNetworkModbusServerServerNameChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(NETWORK_MODBUS_Deledate_SERVER_SERVERNAME)->getData();

    QFile file(XML_Network_Modbus_SERVER_SERVERNAME);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Network::slotNetworkModbusServerServerPortChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(NETWORK_MODBUS_Deledate_SERVER_SERVERPORT)->getData();

    QFile file(XML_Network_Modbus_SERVER_SERVERPORT);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Network::slotNetworkModbusCommandNumberChange()
{
    ConfigObject *pObj = ConfigTreeObject::getConfigObject(NETWORK_MODBUS_Deledate_COMMAND_NUMBER);

    if(pObj){
        quint32 num = pObj->getData().toUInt();
        pObj->setStrShowOn(QString("%1").arg(num+1));
    }
}

void ConfigTreeObject_Network::slotNetworkModbusCommandTypeChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(NETWORK_MODBUS_Deledate_COMMAND_TYPE)->getData();

    QFile file(XML_Network_Modbus_COMMAND_TYPE);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Network::slotNetworkModbusCommandServerChange()
{
    ConfigObject *pObj = ConfigTreeObject::getConfigObject(NETWORK_MODBUS_Deledate_COMMAND_SERVER);

    if(pObj){
        quint32 num = pObj->getData().toUInt();
        pObj->setStrShowOn(QString("%1").arg(num+1));
    }
}

void ConfigTreeObject_Network::slotNetworkModbusCommandUnitChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(NETWORK_MODBUS_Deledate_COMMAND_UNIT)->getData();

    QFile file(XML_Network_Modbus_COMMAND_UNIT);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Network::slotNetworkModbusCommandDataTypeChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(NETWORK_MODBUS_Deledate_COMMAND_DATATYPE)->getData();
    int data = ConfigTreeObject::getConfigObject(NETWORK_MODBUS_Deledate_COMMAND_REGISTER)->getData().toInt();

    switch (value.toInt()) {
    case NetworkModbusCommandDataType_Bit:
        if (data > 10000)
            ConfigTreeObject::getConfigObject(NETWORK_MODBUS_Deledate_COMMAND_REGISTER)->changeData("1",false);
        break;
    default:
    {
        QVariant type = ConfigTreeObject::getConfigObject(NETWORK_MODBUS_Deledate_COMMAND_TYPE)->getData();
        switch (type.toInt()) {
        case NetworkModbusCommandType_Read:
            if (data < 30001 || (data > 39999 && data < 300001) || (data > 399999))
                ConfigTreeObject::getConfigObject(NETWORK_MODBUS_Deledate_COMMAND_REGISTER)->changeData("30001",false);
            break;
        case NetworkModbusCommandType_Write:
            if (data < 40001 || (data > 49999 && data < 400001) || (data > 499999))
                ConfigTreeObject::getConfigObject(NETWORK_MODBUS_Deledate_COMMAND_REGISTER)->changeData("40001",false);
            break;
        default:
            break;
        }
        break;
    }
    }

    QFile file(XML_Network_Modbus_COMMAND_DATATYPE);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Network::slotNetworkModbusCommandRegisterChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(NETWORK_MODBUS_Deledate_COMMAND_REGISTER)->getData();

    QFile file(XML_Network_Modbus_COMMAND_REGISTER);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Network::slotNetworkModbusCommandChanelTypeChange()
{   
    quint32 type = ConfigTreeObject::getConfigObject(NETWORK_MODBUS_Deledate_COMMAND_TYPE)->getData().toUInt();

    QVariant value = ConfigTreeObject::getConfigObject(NETWORK_MODBUS_Deledate_COMMAND_CHANELTYPE)->getData();
    quint32 valueF = ConfigTreeObject::getConfigObject(NETWORK_MODBUS_Deledate_COMMAND_FIRSTCH)->getData().toUInt();
    quint32 valueL = ConfigTreeObject::getConfigObject(NETWORK_MODBUS_Deledate_COMMAND_LASTCH)->getData().toUInt();
    //QString样式转的int,非map对应的quint32
    int StrvalueF = CfgConfig_Network::instance()->getNetworkModbusChValObjStr(valueF).toInt();
    int StrvalueL = CfgConfig_Network::instance()->getNetworkModbusChValObjStr(valueL).toInt();

    QFile file;
    if (value.toString() == "0") {
        ConfigTreeObject::getConfigObject(NETWORK_MODBUS_Deledate_COMMAND_CHANELTYPE)->changeData(NetworkModbusCommandChanelType_COMM, false);
        return;
    }

    switch (type) {
    case NetworkModbusCommandType_Read:
    {
        ConfigTreeObject::getConfigObject(NETWORK_MODBUS_Deledate_COMMAND_CHANELTYPE)\
                ->changeData(NetworkModbusCommandChanelType_COMM, false);
        file.setFileName(XML_Network_Modbus_COMMAND_CHANELTYPE_R);

        //first channel
        if (StrvalueF > 300)
            ConfigTreeObject::getConfigObject(NETWORK_MODBUS_Deledate_COMMAND_FIRSTCH)\
                    ->changeData(CfgChannel::instance()->getExistChannelComm().first(), false);
        //last channel
        if (StrvalueL > 300)
            ConfigTreeObject::getConfigObject(NETWORK_MODBUS_Deledate_COMMAND_LASTCH)\
                    ->changeData(CfgChannel::instance()->getExistChannelComm().first(), false);

        break;
    }
    case NetworkModbusCommandType_Write:
    {
        QList<DevBoard *> boardList = CfgChannel::instance()->getList_BoardIO();
        //wait ....modify
        if (boardList.isEmpty()) {
            file.setFileName(XML_Network_Modbus_COMMAND_CHANELTYPE_HIDEIO);
        } else {
            file.setFileName(XML_Network_Modbus_COMMAND_CHANELTYPE_W);
        }
        //first channel
        switch (value.toInt()) {
        case Channel_Type_Comm:
        {
            //first channel
            if (StrvalueF > 300)
                ConfigTreeObject::getConfigObject(NETWORK_MODBUS_Deledate_COMMAND_FIRSTCH)\
                        ->changeData(CfgChannel::instance()->getExistChannelComm().first(), false);
            //last channel
            if (StrvalueL > 300)
                ConfigTreeObject::getConfigObject(NETWORK_MODBUS_Deledate_COMMAND_LASTCH)\
                        ->changeData(CfgChannel::instance()->getExistChannelComm().first(), false);
            break;
        }
        case Channel_Type_Math:
        {
            //first channel
            if (StrvalueF > 100)
                ConfigTreeObject::getConfigObject(NETWORK_MODBUS_Deledate_COMMAND_FIRSTCH)\
                        ->changeData(CfgChannel::instance()->getExistChannelMath().first(), false);
            //last channel
            if (StrvalueL > 100)
                ConfigTreeObject::getConfigObject(NETWORK_MODBUS_Deledate_COMMAND_LASTCH)\
                        ->changeData(CfgChannel::instance()->getExistChannelMath().first(), false);
            break;
        }
        case Channel_Type_AI:
        case Channel_Type_DI:
        case Channel_Type_DO:
        {
            quint32 codeF, codeL;
            int tempModule = 0;
            int moduleMin = boardList.value(0)->getBoardModule();
            moduleMin = boardList.value(0)->getBoardModule();
            for (int i = 0; i < boardList.size(); ++i) {
                if (boardList.value(i)->getBoardModule() < moduleMin) {
                    tempModule = i;
                    moduleMin = boardList.value(i)->getBoardModule();
                }
            }

            switch (boardList.value(tempModule)->getBoardType()) {
            case STATUS_BOARD_TYPE_AI:
                codeF = CHANNEL_AI_CODER(CHANNEL_UNIT(valueF), CHANNEL_MODULE(valueF), CHANNEL_NUM(valueF));
                codeL = CHANNEL_AI_CODER(CHANNEL_UNIT(valueL), CHANNEL_MODULE(valueL), CHANNEL_NUM(valueL));
                break;
            case STATUS_BOARD_TYPE_DI:
                codeF = CHANNEL_DI_CODER(CHANNEL_UNIT(valueF), CHANNEL_MODULE(valueF), CHANNEL_NUM(valueF));
                codeL = CHANNEL_DI_CODER(CHANNEL_UNIT(valueL), CHANNEL_MODULE(valueL), CHANNEL_NUM(valueL));
                break;
            case STATUS_BOARD_TYPE_DO:
                codeF = CHANNEL_DO_CODER(CHANNEL_UNIT(valueF), CHANNEL_MODULE(valueF), CHANNEL_NUM(valueF));
                codeL = CHANNEL_DO_CODER(CHANNEL_UNIT(valueL), CHANNEL_MODULE(valueL), CHANNEL_NUM(valueL));
                break;
            default:
                break;
            }
            ConfigTreeObject::getConfigObject(NETWORK_MODBUS_Deledate_COMMAND_FIRSTCH)\
                    ->changeData(codeF, false);
            ConfigTreeObject::getConfigObject(NETWORK_MODBUS_Deledate_COMMAND_LASTCH)\
                    ->changeData(codeL, false);
            break;
        }
        default:
            break;
        }
        break;
    }
    default:
        return;
    }

    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Range!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Network::slotNetworkModbusCommandFirstCHChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(NETWORK_MODBUS_Deledate_COMMAND_FIRSTCH)->getData();

    QFile file(XML_Network_Modbus_COMMAND_FIRSTCH);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Network::slotNetworkModbusCommandLastCHChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(NETWORK_MODBUS_Deledate_COMMAND_LASTCH)->getData();

    QVariant value_f = ConfigTreeObject::getConfigObject(NETWORK_MODBUS_Deledate_COMMAND_FIRSTCH)->getData();

    if (value.toInt() < value_f.toInt()) {
        ConfigTreeObject::getConfigObject(NETWORK_MODBUS_Deledate_COMMAND_LASTCH)->changeData(value_f.toInt(), false);
        return;
    }

    QFile file(XML_Network_Modbus_COMMAND_LASTCH);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Network::slotNetworkModbusCommandTypeRelationChange()
{
    slotNetworkModbusCommandChanelTypeChange();
    slotNetworkModbusCommandDataTypeChange();
    slotNetworkModbusCommandFirstCHChange();
    slotNetworkModbusCommandLastCHChange();
}

void ConfigTreeObject_Network::slotNetworkModbusCommandChanelTypeRelationChange()
{
    slotNetworkModbusCommandFirstCHChange();
    slotNetworkModbusCommandLastCHChange();
}
