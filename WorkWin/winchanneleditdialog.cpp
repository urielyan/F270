#include "winchanneleditdialog.h"
#include "winchannelinfodialog.h"
#include "./GlobalData/cfgGlobalDef.h"

WinChannelEditDialog::WinChannelEditDialog(WinChannelInfoDialog *parent) :
    WinDialog(),p_parent(parent),m_digitalPad(WinMainFrame::instance()),m_chanLevel(0)
{
    connect(&m_digitalPad,SIGNAL(sigInputOver()),this,SLOT(slotDigitalPadFinish()));
    initAlarmDialog();
}

/*
 * 功能：
 *              初始化界面
 *
 */
void WinChannelEditDialog::initAlarmDialog()
{
    this->setLayout(&m_layout);
    m_layout.addWidget(&m_alarmFrame,Qt::AlignCenter);
    m_layout.setContentsMargins(0,0,0,0);
    m_layout.setSpacing(0);

    m_alarmFrame.setObjectName("BackgroundFrame");
    m_alarmFrame.setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    m_alarmFrame.setLayout(&m_alarmFrameLayout);

    m_alarmFrameLayout.setContentsMargins(0,0,0,0);
    m_alarmFrameLayout.setSpacing(20);
    m_alarmFrameLayout.addLayout(&m_titleLayout);
    m_alarmFrameLayout.addWidget(&m_textLabel);
    m_alarmFrameLayout.addLayout(&m_contentLayout);
    m_alarmFrameLayout.addWidget(&m_cancelBtn,0,Qt::AlignRight);

    m_titleLayout.setContentsMargins(12,0,12,0);
    m_titleLayout.setSpacing(0);
    m_titleLayout.addWidget(&m_titleLabel,0,Qt::AlignVCenter);
    m_titleLayout.addSpacerItem(new QSpacerItem(40,59,QSizePolicy::Expanding,QSizePolicy::Fixed));
    m_titleLayout.addWidget(&m_closeBtn,0,Qt::AlignVCenter);
    m_closeBtn.setObjectName("CloseButton");
    m_closeBtn.setFocusPolicy(Qt::NoFocus);
    connect(&m_closeBtn, SIGNAL(clicked()),this,SLOT(close()));
    m_titleLabel.setObjectName("TitleLabel");
    m_titleLabel.setText(tr("Edit alarm"));

    m_textLabel.setObjectName("TextLabel");
    m_textLabel.setText(tr("Touch the alarm to edit."));

    m_contentLayout.setSpacing(0);
    m_contentLayout.setContentsMargins(25,0,25,0);
    m_contentLayout.addWidget(&m_alarm1Btn,0,0,1,1);
    m_contentLayout.addWidget(&m_alarm2Btn,0,1,1,1);
    m_contentLayout.addWidget(&m_alarm3Btn,1,0,1,1);
    m_contentLayout.addWidget(&m_alarm4Btn,1,1,1,1);

    m_alarm1Btn.setObjectName("Alarm1Button");
    m_alarm1Btn.setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    m_alarm1Btn.setFocusPolicy(Qt::NoFocus);
    connect(&m_alarm1Btn,SIGNAL(clicked()),this,SLOT(slotAlarmLevel1Edit()));

    m_alarm2Btn.setObjectName("Alarm2Button");
    m_alarm2Btn.setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    m_alarm2Btn.setFocusPolicy(Qt::NoFocus);
    connect(&m_alarm2Btn,SIGNAL(clicked()),this,SLOT(slotAlarmLevel2Edit()));

    m_alarm3Btn.setObjectName("Alarm3Button");
    m_alarm3Btn.setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    m_alarm3Btn.setFocusPolicy(Qt::NoFocus);
    connect(&m_alarm3Btn,SIGNAL(clicked()),this,SLOT(slotAlarmLevel3Edit()));

    m_alarm4Btn.setObjectName("Alarm4Button");
    m_alarm4Btn.setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    m_alarm4Btn.setFocusPolicy(Qt::NoFocus);
    connect(&m_alarm4Btn,SIGNAL(clicked()),this,SLOT(slotAlarmLevel4Edit()));

    m_cancelBtn.setObjectName("CancelButton");
    m_cancelBtn.setText(tr("Cancel"));
    connect(&m_cancelBtn,SIGNAL(clicked()),this,SLOT(accept()));

    m_alarmFrame.setStyleSheet("\
                    QFrame#BackgroundFrame{\
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
                   QPushButton#CancelButton{\
                        width:124px;\
                        height:52px;\
                        font-size:24px;\
                        color:white;\
                        margin-bottom:30px;\
                        margin-right:20px;\
                        background-color:none;\
                        background-image:url(:/relay/Image/relay/button_normal.png);\
                        background-repeat:no-repeat;\
                        background-position:center;\
                        border:none;\
                   }\
                  QPushButton#CancelButton:pressed{\
                        background-image:url(:/relay/Image/relay/button_pressed.png);\
                  }\
                  QLabel#TextLabel{\
                        color:black;\
                        margin-left:20px;\
                        font-size:28px;\
                   }\
                   QPushButton#Alarm1Button,\
                   QPushButton#Alarm2Button,\
                   QPushButton#Alarm3Button,\
                   QPushButton#Alarm4Button{\
                        min-height:100px;\
                        font-size:32px;\
                        border: 1px solid #D3D3D3;\
                        background-color:rgb(0,0,0,0%);\
                   }\
                   QPushButton#Alarm1Button:pressed,\
                   QPushButton#Alarm2Button:pressed,\
                   QPushButton#Alarm3Button:pressed,\
                   QPushButton#Alarm4Button:pressed{\
                        background-color:rgb(0,0,0,20%);\
                   }");
}

void WinChannelEditDialog::resetChannelNumber()
{
    m_alarm1Btn.setText(p_parent->m_alarm1Char);
    m_alarm1Btn.setEnabled(p_parent->m_alarm1OnOff != OFF);

    m_alarm2Btn.setText(p_parent->m_alarm2Char);
    m_alarm2Btn.setEnabled(p_parent->m_alarm2OnOff != OFF);

    m_alarm3Btn.setText(p_parent->m_alarm3Char);
    m_alarm3Btn.setEnabled(p_parent->m_alarm3OnOff != OFF);

    m_alarm4Btn.setText(p_parent->m_alarm4Char);
    m_alarm4Btn.setEnabled(p_parent->m_alarm4OnOff != OFF);
}

void WinChannelEditDialog::slotAlarmLevel1Edit()
{
    this->hide();
    m_chanLevel = 0;
    m_digitalPad.show(p_parent->m_chanNum,m_chanLevel);
}

void WinChannelEditDialog::slotAlarmLevel2Edit()
{
    this->hide();
    m_chanLevel = 1;
    m_digitalPad.show(p_parent->m_chanNum,m_chanLevel);
}

void WinChannelEditDialog::slotAlarmLevel3Edit()
{
    this->hide();
    m_chanLevel = 2;
    m_digitalPad.show(p_parent->m_chanNum,m_chanLevel);
}

void WinChannelEditDialog::slotAlarmLevel4Edit()
{
    this->hide();
    m_chanLevel = 3;
    m_digitalPad.show(p_parent->m_chanNum,m_chanLevel);
}

void WinChannelEditDialog::slotDigitalPadFinish()
{
    m_digitalPad.setCfgvalue(p_parent->m_chanNum,m_chanLevel);
    p_parent->initAlarmCharacteristic();
    resetChannelNumber();
    m_digitalPad.hide();
    this->show();
}


WinChannelSpanDialog::WinChannelSpanDialog(WinChannelInfoDialog *parent) :
    WinDialog(),p_parent(parent),m_digitalPad(WinMainFrame::instance())
{
    connect(&m_digitalPad,SIGNAL(sigInputOver()),this,SLOT(slotDigitalPadFinish()));
    initAlarmDialog();
}

/*
 * 功能：
 *              初始化界面
 *
 */
void WinChannelSpanDialog::initAlarmDialog()
{
    this->setLayout(&m_layout);
    m_layout.addWidget(&m_alarmFrame,Qt::AlignCenter);
    m_layout.setContentsMargins(0,0,0,0);
    m_layout.setSpacing(0);

    m_alarmFrame.setObjectName("BackgroundFrame");
    m_alarmFrame.setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    m_alarmFrame.setLayout(&m_alarmFrameLayout);

    m_alarmFrameLayout.setContentsMargins(0,0,0,0);
    m_alarmFrameLayout.setSpacing(20);
    m_alarmFrameLayout.addLayout(&m_titleLayout);
    m_alarmFrameLayout.addWidget(&m_textLabel);
    m_alarmFrameLayout.addLayout(&m_contentLayout);
    m_alarmFrameLayout.addWidget(&m_cancelBtn,0,Qt::AlignRight);

    m_titleLayout.setContentsMargins(12,0,12,0);
    m_titleLayout.setSpacing(0);
    m_titleLayout.addWidget(&m_titleLabel,0,Qt::AlignVCenter);
    m_titleLayout.addSpacerItem(new QSpacerItem(40,59,QSizePolicy::Expanding,QSizePolicy::Fixed));
    m_titleLayout.addWidget(&m_closeBtn,0,Qt::AlignVCenter);
    m_closeBtn.setObjectName("CloseButton");
    m_closeBtn.setFocusPolicy(Qt::NoFocus);
    connect(&m_closeBtn, SIGNAL(clicked()),this,SLOT(close()));
    m_titleLabel.setObjectName("TitleLabel");
    m_titleLabel.setText(tr("Edit disp span"));

    m_textLabel.setObjectName("TextLabel");
    m_textLabel.setText(tr("Touch the span to edit."));

    m_contentLayout.setSpacing(0);
    m_contentLayout.setContentsMargins(25,0,25,0);
    m_contentLayout.addWidget(&m_spanUpperBtn);
    m_contentLayout.addWidget(&m_spanLowerBtn);


    m_spanUpperBtn.setObjectName("SpanUpperButton");
    m_spanUpperBtn.setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    m_spanUpperBtn.setFocusPolicy(Qt::NoFocus);
    connect(&m_spanUpperBtn,SIGNAL(clicked()),this,SLOT(slotAlarmSpanUpperEdit()));

    m_spanLowerBtn.setObjectName("SpanLowerButton");
    m_spanLowerBtn.setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    m_spanLowerBtn.setFocusPolicy(Qt::NoFocus);
    connect(&m_spanLowerBtn,SIGNAL(clicked()),this,SLOT(slotAlarmSpanLowerEdit()));

    m_cancelBtn.setObjectName("CancelButton");
    m_cancelBtn.setText(tr("Cancel"));
    connect(&m_cancelBtn,SIGNAL(clicked()),this,SLOT(accept()));

    m_alarmFrame.setStyleSheet("\
                    QFrame#BackgroundFrame{\
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
                   QPushButton#CancelButton{\
                        width:124px;\
                        height:52px;\
                        font-size:24px;\
                        color:white;\
                        margin-bottom:30px;\
                        margin-right:20px;\
                        background-color:none;\
                        background-image:url(:/relay/Image/relay/button_normal.png);\
                        background-repeat:no-repeat;\
                        background-position:center;\
                        border:none;\
                   }\
                  QPushButton#CancelButton:pressed{\
                        background-image:url(:/relay/Image/relay/button_pressed.png);\
                  }\
                  QLabel#TextLabel{\
                        color:black;\
                        margin-left:20px;\
                        font-size:28px;\
                   }\
                   QPushButton#SpanUpperButton,\
                   QPushButton#SpanLowerButton{\
                        min-height:100px;\
                        font-size:32px;\
                        border: 1px solid #D3D3D3;\
                        background-color:rgb(0,0,0,0%);\
                   }\
                   QPushButton#SpanUpperButton:pressed,\
                   QPushButton#SpanUpperButton:pressed{\
                        background-color:rgb(0,0,0,20%);\
                   }");
}

void WinChannelSpanDialog::resetChannelNumber()
{
    m_spanUpperBtn.setText(p_parent->m_dispSpanUpper);
    m_spanLowerBtn.setText(p_parent->m_dispSpanLower);
}

void WinChannelSpanDialog::slotAlarmSpanUpperEdit()
{
    this->hide();
    m_span = 4;
    m_digitalPad.show(p_parent->m_chanNum,m_span);
}

void WinChannelSpanDialog::slotAlarmSpanLowerEdit()
{
    this->hide();
    m_span = 5;
    m_digitalPad.show(p_parent->m_chanNum,m_span);
}


void WinChannelSpanDialog::slotDigitalPadFinish()
{
    m_digitalPad.setCfgvalue(p_parent->m_chanNum,m_span);
    p_parent->initAlarmCharacteristic();
    resetChannelNumber();
    m_digitalPad.hide();
    this->show();
}
