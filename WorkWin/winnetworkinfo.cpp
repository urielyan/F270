#include "GlobalData/cfgchannel.h"
#include "winnetworkinfo.h"
#include "./GlobalData/globalDef.h"
#include <QPaintEvent>

WinNetworkInfo::WinNetworkInfo(QWidget *parent) :
    QFrame(parent)
{
    setLayout(&m_layout);
    m_layout.setSpacing(15);
    m_layout.setContentsMargins(80,20,50,20);
    m_layout.addLayout(&m_layout1,3);
    m_layout.addLayout(&m_layout2,1);
    m_layout.addLayout(&m_layout3,3);
    m_layout.addLayout(&m_layout4,3);
    m_layout.addLayout(&m_layout5,8);

    m_layout1.setSpacing(0);
    m_layout1.setMargin(0);
    m_ipLabel.setText(tr("IP address"));
    m_layout1.addWidget(&m_ipLabel,0,0,1,1);
    m_layout1.addWidget(&m_ip,0,1,1,3);
    m_subnetLabel.setText(tr("Subnet mask"));
    m_layout1.addWidget(&m_subnetLabel,1,0,1,1);
    m_layout1.addWidget(&m_subnet,1,1,1,3);
    m_gatewayLabel.setText(tr("Default Gateway"));
    m_layout1.addWidget(&m_gatewayLabel,2,0,1,1);
    m_layout1.addWidget(&m_gateway,2,1,1,3);

    m_layout2.setSpacing(0);
    m_layout2.setMargin(0);
    m_macLabel.setText(tr("MAC address"));
    m_layout2.addWidget(&m_macLabel,0,0,1,1);
    m_layout2.addWidget(&m_mac,0,1,1,3);

    m_layout3.setSpacing(0);
    m_layout3.setMargin(0);
    m_dnsLabel.setText(tr("DNS server"));
    m_layout3.addWidget(&m_dnsLabel,0,0,1,1);
    m_priDnsLabel.setText(tr("Primary"));
    m_layout3.addWidget(&m_priDnsLabel,1,0,1,1);
    m_layout3.addWidget(&m_priDns,1,1,1,3);
    m_secDnsLabel.setText(tr("Secondary"));
    m_layout3.addWidget(&m_secDnsLabel,2,0,1,1);
    m_layout3.addWidget(&m_secDns,2,1,1,3);

    m_layout4.setSpacing(0);
    m_layout4.setMargin(0);
    m_domainLabel.setText(tr("Domain suffix"));
    m_layout4.addWidget(&m_domainLabel,0,0,1,1);
    m_priDomainLabel.setText(tr("Primary"));
    m_layout4.addWidget(&m_priDomainLabel,1,0,1,1);
    m_layout4.addWidget(&m_priDomain,1,1,1,3);
    m_secDomainLabel.setText(tr("Secondary"));
    m_layout4.addWidget(&m_secDomainLabel,2,0,1,1);
    m_layout4.addWidget(&m_secDomain,2,1,1,3);

    m_layout5.setSpacing(0);
    m_layout5.setMargin(0);
    m_serverLabel.setText(tr("Server"));
    m_layout5.addWidget(&m_serverLabel,0,0,1,1);
    m_ftpLabel.setText(tr("FTP"));
    m_layout5.addWidget(&m_ftpLabel,1,0,1,1);
    m_layout5.addWidget(&m_ftp,1,1,1,3);
    m_webLabel.setText(tr("WEB"));
    m_layout5.addWidget(&m_webLabel,2,0,1,1);
    m_layout5.addWidget(&m_web,2,1,1,3);
    m_sntpLabel.setText(tr("SNTP"));
    m_layout5.addWidget(&m_sntpLabel,3,0,1,1);
    m_layout5.addWidget(&m_sntp,3,1,1,3);
    m_modbusLabel.setText(tr("Modbus"));
    m_layout5.addWidget(&m_modbusLabel,4,0,1,1);
    m_layout5.addWidget(&m_modbus,4,1,1,3);
    m_genLabel.setText(tr("General"));
    m_layout5.addWidget(&m_genLabel,5,0,1,1);
    m_layout5.addWidget(&m_gen,5,1,1,3);
    m_ethLabel.setText(tr("EtherNet/IP"));
    m_layout5.addWidget(&m_ethLabel,6,0,1,1);
    m_layout5.addWidget(&m_eth,6,1,1,3);
    m_darwinLabel.setText(tr("DARWIN"));
    m_layout5.addWidget(&m_darwinLabel,7,0,1,1);
    m_layout5.addWidget(&m_darwin,7,1,1,3);

    setNetConfig();
    setFrameConfig();
    connect(CfgChannel::instance(),SIGNAL(sigNormalConfigChanged(quint32)),this,SLOT(slotConfigChanged(quint32)));
}

//系统配置变更时触发
void WinNetworkInfo::slotConfigChanged(quint32 ID)
{
    if((ID == ID_NetWork_Basic) || (ID == ID_NetWork_Server)){
        setNetConfig();
     }

    if(ID == ID_Disp_Basic_Background){
        setFrameConfig();
    }
}

void WinNetworkInfo::resizeEvent(QResizeEvent *e)
{
      setFrameConfig();
      QFrame::resizeEvent(e);
}

void WinNetworkInfo::setFrameConfig()
{
    QString style("QFrame{background-color:%1;color:%2;font-size:%3px;}");
    int h = height()/40;
    h = h > 5 ? h : 5;
    if(CfgChannel::instance()->getSysBackgroundColor()){
        style = style.arg("black","white").arg(h);
    }else{
        style = style.arg("white","black").arg(h);
    }
    setStyleSheet(style);
}

void WinNetworkInfo::setNetConfig()
{
    m_ip.setText(CfgChannel::instance()->getDeviceIPAddress());
    m_subnet.setText(CfgChannel::instance()->getDeviceSubnetMask());
    m_gateway.setText(CfgChannel::instance()->getDefaultGateway());
    m_mac.setText(CfgChannel::instance()->getDeviceMACAddress());

    m_priDns.setText(CfgChannel::instance()->getPrimaryDnsServer());
    m_secDns.setText(CfgChannel::instance()->getSecondaryDnsServer());

    m_ftp.setText(BoolToString(CfgChannel::instance()->getFTPServerStatus()));
    m_web.setText(BoolToString(CfgChannel::instance()->getWEBServerStatus()));
    m_sntp.setText(BoolToString(CfgChannel::instance()->getSNTPServerStatus()));
    m_modbus.setText(BoolToString(CfgChannel::instance()->getModbusStatus()));
}

