#include "winlogdialog.h"
#include <QButtonGroup>
#include <QToolButton>
#include <QGridLayout>


WinLogDialog::WinLogDialog(QWidget */*parent*/)
    :WinDialog()
{
    QFrame *p_mainFrame = new QFrame(this);
    p_mainFrame->setObjectName("BackgrounFrame");
    p_mainFrame->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);

    WinStackedLayout *layout = new WinStackedLayout(this);
    layout->addWidget(p_mainFrame,Qt::AlignCenter);
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(0);

    QPushButton *closeBtn = new QPushButton(p_mainFrame);
    closeBtn->setObjectName("CloseButton");
    closeBtn->setFocusPolicy(Qt::NoFocus);
    connect(closeBtn, SIGNAL(clicked()),this,SLOT(close()));
    QLabel *titleLabel = new QLabel(p_mainFrame);
    titleLabel->setObjectName("TitleLabel");
    titleLabel->setText(tr("Log select"));
    QHBoxLayout * titleLayout = new QHBoxLayout();
    titleLayout->setContentsMargins(12,0,12,0);
    titleLayout->setSpacing(0);
    titleLayout->addWidget(titleLabel,0,Qt::AlignVCenter);
    titleLayout->addSpacerItem(new QSpacerItem(40,59,QSizePolicy::Expanding,QSizePolicy::Fixed));
    titleLayout->addWidget(closeBtn,0,Qt::AlignVCenter);

    m_gLayout = new QGridLayout();
    m_gLayout->setContentsMargins(12,12,12,12);
    m_gLayout->setSpacing(0);

    QIcon icon;
//    btnGroup = new QButtonGroup(bgFrame);


    QToolButton *opeBtn = new QToolButton();
    opeBtn->setObjectName("OperationButton");
    opeBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    icon.addPixmap(QPixmap(":/multiPanel/Image/multiPanel/ope_e.png"),QIcon::Normal);
    icon.addPixmap(QPixmap(":/multiPanel/Image/multiPanel/ope_d.png"),QIcon::Disabled);
    opeBtn->setIcon(icon);
    opeBtn->setIconSize(QSize(60,50));
    opeBtn->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    opeBtn->setText(WinMainFrame::getFrameName(Win_Event_Log_Panel));
    opeBtn->setCheckable(false);

    connect(opeBtn, SIGNAL(clicked()), this, SLOT(slotOpeClicked()));
//    connect(btnGroup,SIGNAL(buttonClicked(int)),this,SLOT(slotButtonClicked(int)));
//    btnGroup->addButton(opeBtn,Win_Event_Log_Panel);
    QToolButton *errBtn = new QToolButton();
    errBtn->setObjectName("ErrorButton");
    errBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    icon.addPixmap(QPixmap(":/multiPanel/Image/multiPanel/error_e.png"),QIcon::Normal);
    icon.addPixmap(QPixmap(":/multiPanel/Image/multiPanel/error_d.png"),QIcon::Disabled);
    errBtn->setIcon(icon);
    errBtn->setIconSize(QSize(60,50));
    errBtn->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    errBtn->setText(WinMainFrame::getFrameName(Win_Error_Log_Panel));
    errBtn->setCheckable(true);
    connect(errBtn, SIGNAL(clicked()), this, SLOT(slotErrClicked()));
//    btnGroup->addButton(errBtn,Win_Error_Log_Panel);
    QToolButton *commBtn = new QToolButton();
    commBtn->setObjectName("CommunicationButton");
    commBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    icon.addPixmap(QPixmap(":/multiPanel/Image/multiPanel/comm_e.png"),QIcon::Normal);
    icon.addPixmap(QPixmap(":/multiPanel/Image/multiPanel/comm_d.png"),QIcon::Disabled);
    commBtn->setIcon(icon);
    commBtn->setIconSize(QSize(60,50));
    commBtn->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    commBtn->setText(WinMainFrame::getFrameName(Win_General_Log_Panel));
    commBtn->setCheckable(true);
    connect(commBtn, SIGNAL(clicked()), this, SLOT(slotCommClicked()));
//    btnGroup->addButton(commBtn,Win_General_Log_Panel);
    QToolButton *ftpBtn = new QToolButton();
    ftpBtn->setObjectName("FtpButton");
    ftpBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    icon.addPixmap(QPixmap(":/multiPanel/Image/multiPanel/FTP_e.png"),QIcon::Normal);
    icon.addPixmap(QPixmap(":/multiPanel/Image/multiPanel/FTP_d.png"),QIcon::Disabled);
    ftpBtn->setIcon(icon);
    ftpBtn->setIconSize(QSize(60,50));
    ftpBtn->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    ftpBtn->setText(WinMainFrame::getFrameName(Win_FTP_Log_Panel));
    ftpBtn->setCheckable(true);
    connect(ftpBtn, SIGNAL(clicked()), this, SLOT(slotFtpClicked()));
//    btnGroup->addButton(ftpBtn,Win_FTP_Log_Panel);
    QToolButton *webBtn = new QToolButton();
    webBtn->setObjectName("WebButton");
    webBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    icon.addPixmap(QPixmap(":/multiPanel/Image/multiPanel/WEB_e.png"),QIcon::Normal);
    icon.addPixmap(QPixmap(":/multiPanel/Image/multiPanel/WEB_d.png"),QIcon::Disabled);
    webBtn->setIcon(icon);
    webBtn->setIconSize(QSize(60,50));
    webBtn->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    webBtn->setText(WinMainFrame::getFrameName(Win_WEB_Log_Panel));
    webBtn->setCheckable(true);
    connect(webBtn, SIGNAL(clicked()), this, SLOT(slotWebClicked()));
//    btnGroup->addButton(webBtn,Win_WEB_Log_Panel);
    QToolButton *emailBtn = new QToolButton();
    emailBtn->setObjectName("EmailButton");
    emailBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    icon.addPixmap(QPixmap(":/multiPanel/Image/multiPanel/EMAIL_e.png"),QIcon::Normal);
    icon.addPixmap(QPixmap(":/multiPanel/Image/multiPanel/EMAIL_d.png"),QIcon::Disabled);
    emailBtn->setIcon(icon);
    emailBtn->setIconSize(QSize(60,50));
    emailBtn->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    emailBtn->setText(WinMainFrame::getFrameName(Win_Email_Log_Panel));
    emailBtn->setCheckable(true);
    connect(emailBtn, SIGNAL(clicked()), this, SLOT(slotEmailClicked()));
//    btnGroup->addButton(emailBtn,Win_Email_Log_Panel);
    QToolButton *mbBtn = new QToolButton();
    mbBtn->setObjectName("ModbusButton");
    mbBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    icon.addPixmap(QPixmap(":/multiPanel/Image/multiPanel/MOD_e.png"),QIcon::Normal);
    icon.addPixmap(QPixmap(":/multiPanel/Image/multiPanel/MOD_d.png"),QIcon::Disabled);
    mbBtn->setIcon(icon);
    mbBtn->setIconSize(QSize(60,50));
    mbBtn->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    mbBtn->setText(WinMainFrame::getFrameName(Win_Modbus_Log_Panel));
    mbBtn->setCheckable(true);
    connect(mbBtn, SIGNAL(clicked()), this, SLOT(slotMbClicked()));
//    btnGroup->addButton(mbBtn,Win_Modbus_Log_Panel);
    QToolButton *sntpBtn = new QToolButton();
    sntpBtn->setObjectName("SntpButton");
    sntpBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    icon.addPixmap(QPixmap(":/multiPanel/Image/multiPanel/SNTP_e.png"),QIcon::Normal);
    icon.addPixmap(QPixmap(":/multiPanel/Image/multiPanel/SNTP_d.png"),QIcon::Disabled);
    sntpBtn->setIcon(icon);
    sntpBtn->setIconSize(QSize(60,50));
    sntpBtn->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    sntpBtn->setText(WinMainFrame::getFrameName(Win_SNTP_Log_Panel));
    sntpBtn->setCheckable(true);
    connect(sntpBtn, SIGNAL(clicked()), this, SLOT(slotSntpClicked()));
//    btnGroup->addButton(sntpBtn,Win_SNTP_Log_Panel);
    QToolButton *dhcpBtn = new QToolButton();
    dhcpBtn->setObjectName("DhcpButton");
    dhcpBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    icon.addPixmap(QPixmap(":/multiPanel/Image/multiPanel/DHCP_e.png"),QIcon::Normal);
    icon.addPixmap(QPixmap(":/multiPanel/Image/multiPanel/DHCP_d.png"),QIcon::Disabled);
    dhcpBtn->setIcon(icon);
    dhcpBtn->setIconSize(QSize(60,50));
    dhcpBtn->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    dhcpBtn->setText(WinMainFrame::getFrameName(Win_DHCP_Log_Panel));
    dhcpBtn->setCheckable(true);
    connect(dhcpBtn, SIGNAL(clicked()), this, SLOT(slotDhcpClicked()));
//    btnGroup->addButton(dhcpBtn,Win_DHCP_Log_Panel);

    m_gLayout->addWidget(opeBtn, 0, 0, 1, 1);
    m_gLayout->addWidget(errBtn, 0, 1, 1, 1);
    m_gLayout->addWidget(commBtn, 0, 2, 1, 1);
    m_gLayout->addWidget(ftpBtn, 1, 0, 1, 1);
    m_gLayout->addWidget(webBtn, 1, 1, 1, 1);
    m_gLayout->addWidget(emailBtn, 1, 2, 1, 1);
    m_gLayout->addWidget(mbBtn, 2, 0, 1, 1);
    m_gLayout->addWidget(sntpBtn, 2, 1, 1, 1);
    m_gLayout->addWidget(dhcpBtn, 2, 2, 1, 1);

    QLabel *txtLabel = new QLabel(p_mainFrame);
    txtLabel->setObjectName("TextLabel");
    txtLabel->setText(tr("Touch screen to diaplay"));

    QVBoxLayout * contextLayout = new QVBoxLayout(p_mainFrame);
    contextLayout->setContentsMargins(0,0,0,20);
    contextLayout->setSpacing(0);

    contextLayout->addLayout(titleLayout);
    contextLayout->addWidget(txtLabel);
    contextLayout->addLayout(m_gLayout);

    p_mainFrame->setStyleSheet("QFrame#BackgrounFrame{\
                               min-width:722px;\
                               max-width:722px;\
                               border: 1px solid #D3D3D3;\
                               background-color:rgb(255,255,255);\
                               background-image:url(:/relay/Image/relay/title_bg.png);\
                               background-repeat:repeat-x;\
                               background-position: top left;\
                               background-origin: content;\
                               border-top-left-radius:5px;\
                               border-top-right-radius:5px;\
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
                        QLabel#TitleLabel{\
                        color:rgb(255,255,255);\
                        font-size:32px;\
                        }\
                        QLabel#TextLabel{\
                        color:black;\
                        margin-top:30px;\
                        margin-bottom:30px;\
                        margin-left:20px;\
                        font-size:28px;\
                        }\
                        QToolButton{\
                        background-color:rgba(0,0,0,0%);\
                        border:1px solid grad;\
                        color:rgb(127,200,235);\
                        font-size:12px;\
                        }\
                ");

}


void WinLogDialog::slotOpeClicked()
{
    emit sigChangeFrame(Win_Event_Log_Panel);
    this->close();
}

void WinLogDialog::slotErrClicked()
{
    emit sigChangeFrame(Win_Error_Log_Panel);
    this->close();
}

void WinLogDialog::slotCommClicked()
{
    emit sigChangeFrame(Win_General_Log_Panel);
    this->close();
}

void WinLogDialog::slotFtpClicked()
{
    emit sigChangeFrame(Win_FTP_Log_Panel);
    this->close();
}

void WinLogDialog::slotWebClicked()
{
    emit sigChangeFrame(Win_WEB_Log_Panel);
    this->close();
}

void WinLogDialog::slotEmailClicked()
{
    emit sigChangeFrame(Win_Email_Log_Panel);
    this->close();
}

void WinLogDialog::slotMbClicked()
{
    emit sigChangeFrame(Win_Modbus_Log_Panel);
    this->close();
}

void WinLogDialog::slotSntpClicked()
{
    emit sigChangeFrame(Win_SNTP_Log_Panel);
    this->close();
}

void WinLogDialog::slotDhcpClicked()
{
    emit sigChangeFrame(Win_DHCP_Log_Panel);
    this->close();
}

void WinLogDialog::slotCloseDialogClicked()
{

    this->close();
}
