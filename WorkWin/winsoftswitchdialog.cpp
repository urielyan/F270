#include <QFrame>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "winsoftswitchdialog.h"
#include "./GlobalData/cfgchannel.h"
#include "./Common/eventaction.h"
#include "./Common/authorization.h"

WinSoftSwitchDialog::WinSoftSwitchDialog(quint32 swNum, QWidget */*parent*/) :
    WinDialog(),m_swNum(swNum)
{
    p_mainFrame = new QFrame(this);
    p_mainFrame->setObjectName("BackgroundFrame");
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
    titleLabel->setText(tr("Internal Switch Information"));
    QHBoxLayout * titleLayout = new QHBoxLayout();
    titleLayout->setContentsMargins(12,0,12,0);
    titleLayout->setSpacing(0);
    titleLayout->addWidget(titleLabel,0,Qt::AlignVCenter);
    titleLayout->addSpacerItem(new QSpacerItem(40,59,QSizePolicy::Expanding,QSizePolicy::Fixed));
    titleLayout->addWidget(closeBtn,0,Qt::AlignVCenter);

    QLabel*swNumPrompt = new QLabel(p_mainFrame);
    swNumPrompt->setObjectName("SwitchNumberPromptLabel");
    swNumPrompt->setText(tr("Internal Swtich No."));
    p_swNumber = new QLabel(p_mainFrame);
    p_swNumber->setObjectName("SwtichNumberLabel");
    p_swNumber->setText(CfgChannel::instance()->getDeviceSwitchName(swNum).toString());

    QFrame *line = new QFrame(p_mainFrame);
    line->setObjectName("HLine");
    line->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);

    QGridLayout *contentLayout = new QGridLayout();
    contentLayout->setSpacing(10);
    contentLayout->setContentsMargins(24,20,24,20);
    contentLayout->addWidget(swNumPrompt,0,0,1,2);
    contentLayout->addWidget(p_swNumber,0,2,1,3);
    contentLayout->addWidget(line,1,0,1,5);

    p_onBtn = new QPushButton(this);
    p_onBtn->setObjectName("ONButton");
    p_onBtn->setText(tr("On"));
    p_onBtn->setFocusPolicy(Qt::NoFocus);
    connect(p_onBtn,SIGNAL(clicked()),this,SLOT(slotOnButtonClicked()));
    p_offBtn = new QPushButton(this);
    p_offBtn->setObjectName("OFFButton");
    p_offBtn->setText(tr("Off"));
    p_offBtn->setFocusPolicy(Qt::NoFocus);
    connect(p_offBtn,SIGNAL(clicked()),this,SLOT(slotOffButtonClicked()));
    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout->setSpacing(15);
    btnLayout->setContentsMargins(25,0,25,25);
    btnLayout->addWidget(p_onBtn);
    btnLayout->addWidget(p_offBtn);
    p_swName = new QLabel(this);
    p_swName->setObjectName("SwitchName");
    p_swName->setText(tr("Internal Switch"));
    QVBoxLayout *swLayout = new QVBoxLayout;
    swLayout->setSpacing(20);
    swLayout->setMargin(0);
    swLayout->addWidget(p_swName,0,Qt::AlignHCenter);
    swLayout->addLayout(btnLayout);

    if(CfgChannel::instance()->getSysAlarmChangeConfig() && (CfgChannel::instance()->getDeviceSwitchMode(m_swNum).toUInt() == 1)){//手动
        p_onBtn->setVisible(true);
        p_offBtn->setVisible(true);
        p_swName->setVisible(true);
        if(Authorization::instance()->getLoginStatus() == Authorization::Auth_LoginStatus_Login && Authorization::instance()->getLoginedUser()->isLocked(Security_ModuleAuth_OutPutOpe)){
            p_onBtn->setEnabled(false);
            p_offBtn->setEnabled(false);
        }else{
            p_onBtn->setEnabled(true);
            p_offBtn->setEnabled(true);
        }
    }else{
        p_onBtn->setVisible(false);
        p_offBtn->setVisible(false);
        p_swName->setVisible(false);
    }

    QVBoxLayout *frameLayout = new QVBoxLayout(p_mainFrame);
    frameLayout->setSpacing(0);
    frameLayout->setContentsMargins(0,0,0,0);
    frameLayout->addLayout(titleLayout);
    frameLayout->addLayout(contentLayout);
    frameLayout->addLayout(swLayout);
    frameLayout->addStretch();

    p_mainFrame->setStyleSheet("QFrame#BackgroundFrame{\
                        min-width:722px;\
                        max-width:722px;\
                        min-height:300px;\
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
                        QLabel#SwitchNumberPromptLabel{\
                        color:rgb(100,100,100);\
                        font-size:24px;\
                        }\
                        QLabel#SwtichNumberLabel{\
                        color:black;\
                        font-size:24px;\
                        }\
                        QPushButton#ONButton,\
                        QPushButton#OFFButton{\
                        min-height:70px;\
                        font-size:32px;\
                        border: 1px solid #D3D3D3;\
                        background-color:rgb(0,0,0,0%);\
                        }\
                        QPushButton#ONButton:pressed,\
                        QPushButton#OFFButton:pressed{\
                        background-color:rgb(0,0,0,20%);\
                        }\
                        QLabel#SwitchName{\
                        font-size:28px;\
                        }");
}

void WinSoftSwitchDialog::slotOnButtonClicked()
{
    EventAction::instance()->setInterSW(m_swNum,true);
}

void WinSoftSwitchDialog::slotOffButtonClicked()
{
    EventAction::instance()->setInterSW(m_swNum,false);
}

