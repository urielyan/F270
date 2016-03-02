#ifndef WINNETWORKINFO_H
#define WINNETWORKINFO_H

#include <QFrame>
#include <QLabel>
#include <QGridLayout>

//网络信息画面
class WinNetworkInfo : public QFrame
{
    Q_OBJECT
public:
    explicit WinNetworkInfo(QWidget *parent = 0);

public slots:
    void slotConfigChanged(quint32 ID);//系统配置更改时刷新显示

protected:
    void resizeEvent(QResizeEvent *);

private:
    //服务状态字符转换
    QString BoolToString(bool status){return status?tr("USE"):tr("NOT");}
    void setFrameConfig();
    void setNetConfig();

private:
    QVBoxLayout m_layout;//总体布局，必须放在所有布局之前

    QGridLayout m_layout1;
    QLabel m_ipLabel;
    QLabel m_ip;
    QLabel m_subnetLabel;
    QLabel m_subnet;
    QLabel m_gatewayLabel;
    QLabel m_gateway;

    QGridLayout m_layout2;
    QLabel m_macLabel;
    QLabel m_mac;

    QGridLayout m_layout3;
    QLabel m_dnsLabel;
    QLabel m_priDnsLabel;
    QLabel m_priDns;
    QLabel m_secDnsLabel;
    QLabel m_secDns;

    QGridLayout m_layout4;
    QLabel m_domainLabel;
    QLabel m_priDomainLabel;
    QLabel m_priDomain;
    QLabel m_secDomainLabel;
    QLabel m_secDomain;

     QGridLayout m_layout5;
     QLabel m_serverLabel;
     QLabel m_ftpLabel;
    QLabel m_ftp;
    QLabel m_webLabel;
    QLabel m_web;
    QLabel m_sntpLabel;
    QLabel m_sntp;
    QLabel m_modbusLabel;
    QLabel m_modbus;
    QLabel m_genLabel;
    QLabel m_gen;
    QLabel m_ethLabel;
    QLabel m_eth;
    QLabel m_darwinLabel;
    QLabel m_darwin;
};

#endif // WINNETWORKINFO_H
