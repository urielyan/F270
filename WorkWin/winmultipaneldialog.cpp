#include <QLabel>
#include <QFrame>
#include <QPushButton>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QButtonGroup>
#include <QVBoxLayout>
#include "winmultipaneldialog.h"
#include "winmainframe.h"
#include "./GlobalData/cfgGlobalDef.h"
#include "./GlobalData/cfgchannel.h"
#include "./Common/authorization.h"

WinMultiPanelDialog::WinMultiPanelDialog(quint32 frameIndex, int panelIndex,quint32 panelType,QVariant panelParam, WinMultiFrame *parent) :
    QDialog(WinMainFrame::instance()),p_multFrame(parent),m_panelIndex(panelIndex),m_frameIndex(frameIndex),m_panelType(panelType),m_panelParam(panelParam)
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setGeometry(0,0,WinMainFrame::instance()->width(),WinMainFrame::instance()->height());
    connect(this,SIGNAL(sigChangeFrame(quint32)),WinMainFrame::instance(),SLOT(slotFrameChanged(quint32)));

    QToolButton *jumpBtn = new QToolButton;
    jumpBtn->setObjectName("JumpButton");
    jumpBtn->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    jumpBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    jumpBtn->setFocusPolicy(Qt::NoFocus);
    jumpBtn->setText(tr("Expand"));
    jumpBtn->setFixedSize(76,76);
    jumpBtn->setIconSize(QSize(33,30));
    jumpBtn->setIcon(QIcon(":/multiPanel/Image/multiPanel/expand.png"));
    connect(jumpBtn,SIGNAL(clicked()),this,SLOT(slotJumpButtonClicked()),Qt::DirectConnection);

    QFrame *bgFrame = new QFrame;
    bgFrame->setObjectName("BackgrounFrame");
    bgFrame->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(bgFrame,0,Qt::AlignCenter);
    layout->addWidget(jumpBtn,0,Qt::AlignRight);
    layout->setContentsMargins(20,20,20,20);
    layout->setSpacing(0);

    /***********************对话框标题*************************/
    QPushButton *closeBtn = new QPushButton(bgFrame);
    closeBtn->setObjectName("CloseButton");
    closeBtn->setFocusPolicy(Qt::NoFocus);
    connect(closeBtn, SIGNAL(clicked()),this,SLOT(close()),Qt::DirectConnection);
    QLabel *titleLabel = new QLabel(bgFrame);
    titleLabel->setObjectName("TitleLabel");
    titleLabel->setText(tr("Multi Panel"));
    QHBoxLayout *titleLayout = new QHBoxLayout;
    titleLayout->setContentsMargins(12,0,12,0);
    titleLayout->setSpacing(0);
    titleLayout->addWidget(titleLabel,0,Qt::AlignVCenter);
    titleLayout->addSpacerItem(new QSpacerItem(40,59,QSizePolicy::Expanding,QSizePolicy::Fixed));
    titleLayout->addWidget(closeBtn,0,Qt::AlignVCenter);

    /************************导航栏****************************/
    p_lastBtn = new QPushButton(bgFrame);
    p_lastBtn->setObjectName("LastButton");
    p_lastBtn->setFocusPolicy(Qt::NoFocus);
    connect(p_lastBtn,SIGNAL(clicked()),this,SLOT(slotLastButtonClicked()));
    p_backBtn = new QPushButton(bgFrame);
    p_backBtn->setObjectName("BackButton");
    p_backBtn->setFocusPolicy(Qt::NoFocus);
    connect(p_backBtn,SIGNAL(clicked()),this,SLOT(slotBackButtonClicked()));
    p_nextBtn = new QPushButton(bgFrame);
    p_nextBtn->setObjectName("NextButton");
    p_nextBtn->setFocusPolicy(Qt::NoFocus);
    connect(p_nextBtn,SIGNAL(clicked()),this,SLOT(slotNextButtonClicked()));
    p_firstBtn = new QPushButton(bgFrame);
    p_firstBtn->setObjectName("FirstButton");
    p_firstBtn->setFocusPolicy(Qt::NoFocus);
    connect(p_firstBtn,SIGNAL(clicked()),this,SLOT(slotFirstButtonClicked()));
    p_nameLabel = new QLabel(bgFrame);
    p_nameLabel->setObjectName("NameLabel");
//    p_nameLabel->setWordWrap(true);
//    p_nameLabel->setText(WinMainFrame::getFrameName(m_panelType));
    QHBoxLayout *navbarLayout = new QHBoxLayout;
    navbarLayout->setContentsMargins(12,6,12,6);
    navbarLayout->setSpacing(6);
    navbarLayout ->addWidget(p_lastBtn,0,Qt::AlignLeft|Qt::AlignVCenter);
    navbarLayout ->addWidget(p_backBtn,0,Qt::AlignLeft|Qt::AlignVCenter);
    navbarLayout->addStretch();
    navbarLayout ->addWidget(p_nameLabel,0,Qt::AlignCenter);
    navbarLayout->addStretch();
    navbarLayout ->addWidget(p_nextBtn,0,Qt::AlignRight|Qt::AlignVCenter);
    navbarLayout ->addWidget(p_firstBtn,0,Qt::AlignRight|Qt::AlignVCenter);

    /***********************设置按钮************************/
    QIcon icon;
    QButtonGroup *btnGroup = new QButtonGroup(bgFrame);
    connect(btnGroup,SIGNAL(buttonClicked(int)),this,SLOT(slotPanelButtonClicked(int)));
    QToolButton *trendBtn = new QToolButton(bgFrame);
    trendBtn->setObjectName("TrendButton");
    trendBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    icon.addPixmap(QPixmap(":/multiPanel/Image/multiPanel/trend_e.png"),QIcon::Normal);
    icon.addPixmap(QPixmap(":/multiPanel/Image/multiPanel/trend_d.png"),QIcon::Disabled);
    trendBtn->setIcon(icon);
    trendBtn->setIconSize(QSize(60,50));
    trendBtn->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    trendBtn->setText(WinMainFrame::getFrameName(Win_Trend_Panel));
    trendBtn->setCheckable(true);
    btnGroup->addButton(trendBtn,Win_Trend_Panel);

    QToolButton *digitalBtn = new QToolButton(bgFrame);
    digitalBtn->setObjectName("DigitalButton");
    digitalBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    icon.addPixmap(QPixmap(":/multiPanel/Image/multiPanel/digital_e.png"),QIcon::Normal);
    icon.addPixmap(QPixmap(":/multiPanel/Image/multiPanel/digital_d.png"),QIcon::Disabled);
    digitalBtn->setIcon(icon);
    digitalBtn->setIconSize(QSize(60,50));
    digitalBtn->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    digitalBtn->setText(WinMainFrame::getFrameName(Win_Digital_Panel));
    digitalBtn->setCheckable(true);
    btnGroup->addButton(digitalBtn,Win_Digital_Panel);
    QToolButton *barBtn = new QToolButton(bgFrame);
    barBtn->setObjectName("BarButton");
    barBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    icon.addPixmap(QPixmap(":/multiPanel/Image/multiPanel/bar_e.png"),QIcon::Normal);
    icon.addPixmap(QPixmap(":/multiPanel/Image/multiPanel/bar_d.png"),QIcon::Disabled);
    barBtn->setIcon(icon);
    barBtn->setIconSize(QSize(60,50));
    barBtn->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    barBtn->setText(WinMainFrame::getFrameName(Win_Bar_Panel));
    barBtn->setCheckable(true);
    btnGroup->addButton(barBtn,Win_Bar_Panel);
    QToolButton *ovBtn = new QToolButton(bgFrame);
    ovBtn->setObjectName("OverviewButton");
    ovBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    icon.addPixmap(QPixmap(":/multiPanel/Image/multiPanel/ov_e.png"),QIcon::Normal);
    icon.addPixmap(QPixmap(":/multiPanel/Image/multiPanel/ov_d.png"),QIcon::Disabled);
    ovBtn->setIcon(icon);
    ovBtn->setIconSize(QSize(60,50));
    ovBtn->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    ovBtn->setText(WinMainFrame::getFrameName(Win_Overview_Panel));
    ovBtn->setCheckable(true);
    btnGroup->addButton(ovBtn,Win_Overview_Panel);
    QToolButton *alarmBtn = new QToolButton(bgFrame);
    alarmBtn->setObjectName("AlarmButton");
    alarmBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    icon.addPixmap(QPixmap(":/multiPanel/Image/multiPanel/alarm_e.png"),QIcon::Normal);
    icon.addPixmap(QPixmap(":/multiPanel/Image/multiPanel/alarm_d.png"),QIcon::Disabled);
    alarmBtn->setIcon(icon);
    alarmBtn->setIconSize(QSize(60,50));
    alarmBtn->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    alarmBtn->setText(WinMainFrame::getFrameName(Win_Alarm_Summary_Panel));
    alarmBtn->setCheckable(true);
    btnGroup->addButton(alarmBtn,Win_Alarm_Summary_Panel);
    QToolButton *msgBtn = new QToolButton(bgFrame);
    msgBtn->setObjectName("MessageButton");
    msgBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    icon.addPixmap(QPixmap(":/multiPanel/Image/multiPanel/msg_e.png"),QIcon::Normal);
    icon.addPixmap(QPixmap(":/multiPanel/Image/multiPanel/msg_d.png"),QIcon::Disabled);
    msgBtn->setIcon(icon);
    msgBtn->setIconSize(QSize(60,50));
    msgBtn->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    msgBtn->setText(WinMainFrame::getFrameName(Win_Info_Summary_Panel));
    msgBtn->setCheckable(true);
    btnGroup->addButton(msgBtn,Win_Info_Summary_Panel);
    QToolButton *memBtn = new QToolButton(bgFrame);
    memBtn->setObjectName("MemButton");
    memBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    icon.addPixmap(QPixmap(":/multiPanel/Image/multiPanel/mem_e.png"),QIcon::Normal);
    icon.addPixmap(QPixmap(":/multiPanel/Image/multiPanel/mem_d.png"),QIcon::Disabled);
    memBtn->setIcon(icon);
    memBtn->setIconSize(QSize(60,50));
    memBtn->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    memBtn->setText(WinMainFrame::getFrameName(Win_Mem_Summary_Panel));
    memBtn->setCheckable(true);
    btnGroup->addButton(memBtn,Win_Mem_Summary_Panel);
    QToolButton *reportBtn = new QToolButton(bgFrame);
    reportBtn->setObjectName("ReportButton");
    reportBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    icon.addPixmap(QPixmap(":/multiPanel/Image/multiPanel/report_e.png"),QIcon::Normal);
    icon.addPixmap(QPixmap(":/multiPanel/Image/multiPanel/report_d.png"),QIcon::Disabled);
    reportBtn->setIcon(icon);
    reportBtn->setIconSize(QSize(60,50));
    reportBtn->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    reportBtn->setText(WinMainFrame::getFrameName(Win_Report_Panel));
    reportBtn->setCheckable(true);
    btnGroup->addButton(reportBtn,Win_Report_Panel);
    QToolButton *modcBtn = new QToolButton(bgFrame);
    modcBtn->setObjectName("ModClientButton");
    modcBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    icon.addPixmap(QPixmap(":/multiPanel/Image/multiPanel/modc_e.png"),QIcon::Normal);
    icon.addPixmap(QPixmap(":/multiPanel/Image/multiPanel/modc_d.png"),QIcon::Disabled);
    modcBtn->setIcon(icon);
    modcBtn->setIconSize(QSize(60,50));
    modcBtn->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    modcBtn->setText(WinMainFrame::getFrameName(Win_Modbus_Client_Panel));
    modcBtn->setCheckable(true);
    btnGroup->addButton(modcBtn,Win_Modbus_Client_Panel);
    QToolButton *modsBtn = new QToolButton(bgFrame);
    modsBtn->setObjectName("ModServerButton");
    modsBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    icon.addPixmap(QPixmap(":/multiPanel/Image/multiPanel/modc_e.png"),QIcon::Normal);
    icon.addPixmap(QPixmap(":/multiPanel/Image/multiPanel/modc_d.png"),QIcon::Disabled);
    modsBtn->setIcon(icon);
    modsBtn->setIconSize(QSize(60,50));
    modsBtn->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    modsBtn->setText(WinMainFrame::getFrameName(Win_Modbus_Server_Panel));
    modsBtn->setCheckable(true);
    btnGroup->addButton(modsBtn,Win_Modbus_Server_Panel);
    QToolButton *relayBtn = new QToolButton(bgFrame);
    relayBtn->setObjectName("RelayButton");
    relayBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    icon.addPixmap(QPixmap(":/multiPanel/Image/multiPanel/sw_e.png"),QIcon::Normal);
    icon.addPixmap(QPixmap(":/multiPanel/Image/multiPanel/sw_d.png"),QIcon::Disabled);
    relayBtn->setIcon(icon);
    relayBtn->setIconSize(QSize(60,50));
    relayBtn->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    relayBtn->setText(WinMainFrame::getFrameName(Win_Relay_SW_Panel));
    relayBtn->setCheckable(true);
    btnGroup->addButton(relayBtn,Win_Relay_SW_Panel);
    QToolButton *opeBtn = new QToolButton(bgFrame);
    opeBtn->setObjectName("OperationButton");
    opeBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    icon.addPixmap(QPixmap(":/multiPanel/Image/multiPanel/ope_e.png"),QIcon::Normal);
    icon.addPixmap(QPixmap(":/multiPanel/Image/multiPanel/ope_d.png"),QIcon::Disabled);
    opeBtn->setIcon(icon);
    opeBtn->setIconSize(QSize(60,50));
    opeBtn->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    opeBtn->setText(WinMainFrame::getFrameName(Win_Event_Log_Panel));
    opeBtn->setCheckable(true);
    btnGroup->addButton(opeBtn,Win_Event_Log_Panel);
    QToolButton *errBtn = new QToolButton(bgFrame);
    errBtn->setObjectName("ErrorButton");
    errBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    icon.addPixmap(QPixmap(":/multiPanel/Image/multiPanel/error_e.png"),QIcon::Normal);
    icon.addPixmap(QPixmap(":/multiPanel/Image/multiPanel/error_d.png"),QIcon::Disabled);
    errBtn->setIcon(icon);
    errBtn->setIconSize(QSize(60,50));
    errBtn->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    errBtn->setText(WinMainFrame::getFrameName(Win_Error_Log_Panel));
    errBtn->setCheckable(true);
    btnGroup->addButton(errBtn,Win_Error_Log_Panel);
    QToolButton *commBtn = new QToolButton(bgFrame);
    commBtn->setObjectName("CommunicationButton");
    commBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    icon.addPixmap(QPixmap(":/multiPanel/Image/multiPanel/comm_e.png"),QIcon::Normal);
    icon.addPixmap(QPixmap(":/multiPanel/Image/multiPanel/comm_d.png"),QIcon::Disabled);
    commBtn->setIcon(icon);
    commBtn->setIconSize(QSize(60,50));
    commBtn->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    commBtn->setText(WinMainFrame::getFrameName(Win_General_Log_Panel));
    commBtn->setCheckable(true);
    btnGroup->addButton(commBtn,Win_General_Log_Panel);
    QToolButton *ftpBtn = new QToolButton(bgFrame);
    ftpBtn->setObjectName("FtpButton");
    ftpBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    icon.addPixmap(QPixmap(":/multiPanel/Image/multiPanel/FTP_e.png"),QIcon::Normal);
    icon.addPixmap(QPixmap(":/multiPanel/Image/multiPanel/FTP_d.png"),QIcon::Disabled);
    ftpBtn->setIcon(icon);
    ftpBtn->setIconSize(QSize(60,50));
    ftpBtn->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    ftpBtn->setText(WinMainFrame::getFrameName(Win_FTP_Log_Panel));
    ftpBtn->setCheckable(true);
    btnGroup->addButton(ftpBtn,Win_FTP_Log_Panel);
    QToolButton *webBtn = new QToolButton(bgFrame);
    webBtn->setObjectName("WebButton");
    webBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    icon.addPixmap(QPixmap(":/multiPanel/Image/multiPanel/WEB_e.png"),QIcon::Normal);
    icon.addPixmap(QPixmap(":/multiPanel/Image/multiPanel/WEB_d.png"),QIcon::Disabled);
    webBtn->setIcon(icon);
    webBtn->setIconSize(QSize(60,50));
    webBtn->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    webBtn->setText(WinMainFrame::getFrameName(Win_WEB_Log_Panel));
    webBtn->setCheckable(true);
    btnGroup->addButton(webBtn,Win_WEB_Log_Panel);
    QToolButton *emailBtn = new QToolButton(bgFrame);
    emailBtn->setObjectName("EmailButton");
    emailBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    icon.addPixmap(QPixmap(":/multiPanel/Image/multiPanel/EMAIL_e.png"),QIcon::Normal);
    icon.addPixmap(QPixmap(":/multiPanel/Image/multiPanel/EMAIL_d.png"),QIcon::Disabled);
    emailBtn->setIcon(icon);
    emailBtn->setIconSize(QSize(60,50));
    emailBtn->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    emailBtn->setText(WinMainFrame::getFrameName(Win_Email_Log_Panel));
    emailBtn->setCheckable(true);
    btnGroup->addButton(emailBtn,Win_Email_Log_Panel);
    QToolButton *mbBtn = new QToolButton(bgFrame);
    mbBtn->setObjectName("ModbusButton");
    mbBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    icon.addPixmap(QPixmap(":/multiPanel/Image/multiPanel/MOD_e.png"),QIcon::Normal);
    icon.addPixmap(QPixmap(":/multiPanel/Image/multiPanel/MOD_d.png"),QIcon::Disabled);
    mbBtn->setIcon(icon);
    mbBtn->setIconSize(QSize(60,50));
    mbBtn->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    mbBtn->setText(WinMainFrame::getFrameName(Win_Modbus_Log_Panel));
    mbBtn->setCheckable(true);
    btnGroup->addButton(mbBtn,Win_Modbus_Log_Panel);
    QToolButton *sntpBtn = new QToolButton(bgFrame);
    sntpBtn->setObjectName("SntpButton");
    sntpBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    icon.addPixmap(QPixmap(":/multiPanel/Image/multiPanel/SNTP_e.png"),QIcon::Normal);
    icon.addPixmap(QPixmap(":/multiPanel/Image/multiPanel/SNTP_d.png"),QIcon::Disabled);
    sntpBtn->setIcon(icon);
    sntpBtn->setIconSize(QSize(60,50));
    sntpBtn->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    sntpBtn->setText(WinMainFrame::getFrameName(Win_SNTP_Log_Panel));
    sntpBtn->setCheckable(true);
    btnGroup->addButton(sntpBtn,Win_SNTP_Log_Panel);
    QToolButton *dhcpBtn = new QToolButton(bgFrame);
    dhcpBtn->setObjectName("DhcpButton");
    dhcpBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    icon.addPixmap(QPixmap(":/multiPanel/Image/multiPanel/DHCP_e.png"),QIcon::Normal);
    icon.addPixmap(QPixmap(":/multiPanel/Image/multiPanel/DHCP_d.png"),QIcon::Disabled);
    dhcpBtn->setIcon(icon);
    dhcpBtn->setIconSize(QSize(60,50));
    dhcpBtn->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    dhcpBtn->setText(WinMainFrame::getFrameName(Win_DHCP_Log_Panel));
    dhcpBtn->setCheckable(true);
    btnGroup->addButton(dhcpBtn,Win_DHCP_Log_Panel);
    QToolButton *netBtn = new QToolButton(bgFrame);
    netBtn->setObjectName("NetworkButton");
    netBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    icon.addPixmap(QPixmap(":/multiPanel/Image/multiPanel/net_e.png"),QIcon::Normal);
    icon.addPixmap(QPixmap(":/multiPanel/Image/multiPanel/net_d.png"),QIcon::Disabled);
    netBtn->setIcon(icon);
    netBtn->setIconSize(QSize(60,50));
    netBtn->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    netBtn->setText(WinMainFrame::getFrameName(Win_Network_Panel));
    netBtn->setCheckable(true);
    btnGroup->addButton(netBtn,Win_Network_Panel);

    QGridLayout *contentLayout =new QGridLayout;
    contentLayout->setContentsMargins(12,12,12,12);
    contentLayout->setSpacing(0);
    contentLayout->addWidget(trendBtn,0,0,1,1);
    contentLayout->addWidget(digitalBtn,0,1,1,1);
    contentLayout->addWidget(barBtn,0,2,1,1);
    contentLayout->addWidget(ovBtn,0,3,1,1);
    contentLayout->addWidget(alarmBtn,0,4,1,1);
    contentLayout->addWidget(msgBtn,1,0,1,1);
    contentLayout->addWidget(memBtn,1,1,1,1);
    contentLayout->addWidget(reportBtn,1,2,1,1);
    contentLayout->addWidget(modcBtn,1,3,1,1);
    contentLayout->addWidget(modsBtn,1,4,1,1);
    contentLayout->addWidget(relayBtn,2,0,1,1);
    contentLayout->addWidget(opeBtn,2,1,1,1);
    contentLayout->addWidget(errBtn,2,2,1,1);
    contentLayout->addWidget(commBtn,2,3,1,1);
    contentLayout->addWidget(ftpBtn,2,4,1,1);
    contentLayout->addWidget(webBtn,3,0,1,1);
    contentLayout->addWidget(emailBtn,3,1,1,1);
    contentLayout->addWidget(mbBtn,3,2,1,1);
    contentLayout->addWidget(sntpBtn,3,3,1,1);
    contentLayout->addWidget(dhcpBtn,3,4,1,1);
    contentLayout->addWidget(netBtn,4,0,1,1);

    QVBoxLayout *frameLayout = new QVBoxLayout(bgFrame);
    frameLayout->setContentsMargins(0,0,0,0);
    frameLayout->setSpacing(0);
    frameLayout->addLayout(titleLayout);
    frameLayout->addLayout(navbarLayout);
    frameLayout->addLayout(contentLayout);
    bgFrame->setStyleSheet("QFrame#BackgrounFrame{\
                        min-width:722px;\
                        width:722px;\
                        border:none;\
                        background-color:rgb(255,255,255);\
                        background-image:url(:/summary/Image/summary/title.png);\
                        background-repeat:no-repeat;\
                        background-position: top left;\
                        background-origin: content;\
                        border-top-left-radius:5px;\
                        border-top-right-radius:5px;\
                        }\
                        QLabel#TitleLabel{\
                        color:rgb(255,255,255);\
                        font-size:32px;\
                        }\
                        QLabel#NameLabel{\
                        color:rgb(96,103,96);\
                        margin-left:25px;\
                        margin-right:25px;\
                        font-size:24px;\
                        }\
                        QPushButton#CloseButton{\
                        min-width:34px;\
                        max-width:34px;\
                        width:34px;\
                        height:34px;\
                        background-image:url(:/relay/Image/relay/close_normal.png);\
                        background-repeat: no-repeat;\
                        background-position:center;\
                        border:none;\
                        }\
                        QPushButton#CloseButton:pressed{\
                        background-image:url(:/relay/Image/relay/close_pressed.png);\
                        }\
                        QPushButton#LastButton{\
                        height:35px;\
                        width:50px;\
                        background-color:rgba(0,0,0,0%);\
                        border:1px solid grad;\
                        qproperty-iconSize: 32px;\
                        qproperty-icon:url(:/multiPanel/Image/multiPanel/last_e.png) active;\
                        qproperty-icon:url(:/multiPanel/Image/multiPanel/last_d.png) disable;\
                        }\
                        QPushButton#BackButton{\
                        height:35px;\
                        width:50px;\
                        background-color:rgba(0,0,0,0%);\
                        border:1px solid grad;\
                        qproperty-iconSize: 32px;\
                        qproperty-icon:url(:/multiPanel/Image/multiPanel/left_e.png) active;\
                        qproperty-icon:url(:/multiPanel/Image/multiPanel/left_d.png) disable;\
                        }\
                        QPushButton#NextButton{\
                        height:35px;\
                        width:50px;\
                        background-color:rgba(0,0,0,0%);\
                        border:1px solid grad;\
                        qproperty-iconSize: 32px;\
                        qproperty-icon:url(:/multiPanel/Image/multiPanel/right_e.png) active;\
                        qproperty-icon:url(:/multiPanel/Image/multiPanel/right_d.png) disable;\
                        }\
                        QPushButton#FirstButton{\
                        height:35px;\
                        width:50px;\
                        background-color:rgba(0,0,0,0%);\
                        border:1px solid grad;\
                        qproperty-iconSize: 32px;\
                        qproperty-icon:url(:/multiPanel/Image/multiPanel/first_e.png) active;\
                        qproperty-icon:url(:/multiPanel/Image/multiPanel/first_d.png) disable;\
                        }\
                        QPushButton#LastButton:pressed,\
                        QPushButton#BackButton:pressed,\
                        QPushButton#NextButton:pressed,\
                        QPushButton#FirstButton:pressed\
                        {\
                        background-color:rgba(0,0,0,20%);\
                        }\
                        QToolButton{\
                        background-color:rgba(0,0,0,0%);\
                        border:1px solid grad;\
                        color:#00B2EE;\
                        font-size:12px;\
                        }\
                        QToolButton:checked{\
                        background-color:rgba(0,0,0,40%);\
                        }\
                ");

    btnGroup->button(m_panelType)->setChecked(true);
    slotPanelButtonClicked(m_panelType);
    connect(this, SIGNAL(sigChangePanelType(int,quint32)),p_multFrame,SLOT(slotPanelTypeChanged(int,quint32)));
    connect(this,SIGNAL(sigChangePanelParam(int,QVariant)),p_multFrame,SLOT(slotPanelParamChanged(int,QVariant)));

    if(Authorization::instance()->getLoginStatus() == Authorization::Auth_LoginStatus_Login && Authorization::instance()->getLoginedUser()->isLocked(Security_ModuleAuth_TouchOpe)){
        p_lastBtn->setEnabled(false);
        p_backBtn->setEnabled(false);
        p_nextBtn->setEnabled(false);
        p_firstBtn->setEnabled(false);
        trendBtn->setEnabled(false);
        digitalBtn->setEnabled(false);
        barBtn->setEnabled(false);
        ovBtn->setEnabled(false);
        alarmBtn->setEnabled(false);
        msgBtn->setEnabled(false);
        memBtn->setEnabled(false);
        reportBtn->setEnabled(false);
        modcBtn->setEnabled(false);
        modsBtn->setEnabled(false);
        relayBtn->setEnabled(false);
        opeBtn->setEnabled(false);
        errBtn->setEnabled(false);
        commBtn->setEnabled(false);
        ftpBtn->setEnabled(false);
        webBtn->setEnabled(false);
        emailBtn->setEnabled(false);
        mbBtn->setEnabled(false);
        sntpBtn->setEnabled(false);
        dhcpBtn->setEnabled(false);
        netBtn->setEnabled(false);
    }
}

void WinMultiPanelDialog::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    QPainterPath path;
    path.setFillRule(Qt::OddEvenFill);
    path.addRect(e->rect());
    path.addRect(p_multFrame->getPanelRect(m_panelIndex));
    painter.setPen(Qt::transparent);
    painter.setBrush(QColor(0,0,0,180));
    painter.drawPath(path);
    QDialog::paintEvent(e);
}

/**
 * 功能：
 *      画面切换按钮被点击
 * 参数：
 *      int panelId　画面类型索引
 */
void WinMultiPanelDialog::slotPanelButtonClicked(int panelType)
{
    m_panelType = panelType;
    if(m_panelType == Win_Trend_Panel || m_panelType == Win_Bar_Panel  || m_panelType == Win_Digital_Panel ){
        p_lastBtn->setEnabled(true);
        p_backBtn->setEnabled(true);
        p_nextBtn->setEnabled(true);
        p_firstBtn->setEnabled(true);
        p_nameLabel->setText(CfgChannel::instance()->getGroupName(m_panelParam.toUInt()).toString());
    }else{
        p_lastBtn->setEnabled(false);
        p_backBtn->setEnabled(false);
        p_nextBtn->setEnabled(false);
        p_firstBtn->setEnabled(false);
        p_nameLabel->setText(WinMainFrame::getFrameName(m_panelType));
    }

    emit sigChangePanelType(m_panelIndex,m_panelType);
}

/**
 * 功能：
 *      前进
 */
void WinMultiPanelDialog::slotNextButtonClicked()
{
    const QList<quint32> &grpList = CfgChannel::instance()->getEffectiveGroup();

    quint32 count =  grpList.count();
    if(count < 1)
        return;

    quint32 index = CfgChannel::instance()->getEffectiveGroup().indexOf(m_panelParam.toUInt());
    index = (index + 1)%count;

    m_panelParam = grpList.value(index,0);
    QVariant value = CfgChannel::instance()->getGroupName(m_panelParam.toUInt());
    if(value.isValid()){
        p_nameLabel->setText(value.toString());
    }
    emit sigChangePanelParam(m_panelIndex,m_panelParam.toUInt());
}

/**
 * 功能：
 *      快进
 */
void WinMultiPanelDialog::slotFirstButtonClicked()
{
    const QList<quint32> &grpList = CfgChannel::instance()->getEffectiveGroup();

    quint32 count =  grpList.count();
    if(count < 1)
        return;

    quint32 index = CfgChannel::instance()->getEffectiveGroup().indexOf(m_panelParam.toUInt());
    if(count > 5){
        index = (index + 5)%count;
    }else if(count > 10){
        index = (index + 10)%count;
    }else{
        index = (index + 1)%count;
    }

    m_panelParam = grpList.value(index,0);
    QVariant value = CfgChannel::instance()->getGroupName(m_panelParam.toUInt());
    if(value.isValid()){
        p_nameLabel->setText(value.toString());
    }
    emit sigChangePanelParam(m_panelIndex,m_panelParam.toUInt());
}

/**
 * 功能：
 *      后退
 */
void WinMultiPanelDialog::slotBackButtonClicked()
{
    const QList<quint32> &grpList = CfgChannel::instance()->getEffectiveGroup();

    quint32 count =  grpList.count();
    if(count < 1)
        return;

    quint32 index = CfgChannel::instance()->getEffectiveGroup().indexOf(m_panelParam.toUInt()) + count;
    index = (index - 1)%count;

    m_panelParam = grpList.value(index,0);
    QVariant value = CfgChannel::instance()->getGroupName(m_panelParam.toUInt());
    if(value.isValid()){
        p_nameLabel->setText(value.toString());
    }
    emit sigChangePanelParam(m_panelIndex,m_panelParam.toUInt());
}

/**
 * 功能：
 *      快退
 */
void WinMultiPanelDialog::slotLastButtonClicked()
{
    const QList<quint32> &grpList = CfgChannel::instance()->getEffectiveGroup();

    quint32 count =  grpList.count();
    if(count < 1)
        return;

    quint32 index = CfgChannel::instance()->getEffectiveGroup().indexOf(m_panelParam.toUInt()) + count;
    if(count > 5){
        index = (index - 5)%count;
    }else if(count > 10){
        index = (index - 10)%count;
    }else{
        index = (index - 1)%count;
    }

    m_panelParam = grpList.value(index,0);
    QVariant value = CfgChannel::instance()->getGroupName(m_panelParam.toUInt());
    if(value.isValid()){
        p_nameLabel->setText(value.toString());
    }
    emit sigChangePanelParam(m_panelIndex,m_panelParam.toUInt());
}

/**
 * 功能：
 *      跳转到全屏模式
 */
void WinMultiPanelDialog::slotJumpButtonClicked()
{
    emit sigChangeFrame(m_panelType);
    this->close();
}
