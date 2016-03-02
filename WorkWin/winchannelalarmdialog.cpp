#include <QStylePainter>
#include "winchannelinfodialog.h"
#include "winchannelalarmdialog.h"
#include "./Common/eventaction.h"


WinChannelAlarmDialog::WinChannelAlarmDialog(WinChannelInfoDialog *parent) :
    WinDialog(),p_parent(parent)
{
    initAlarmDialog();
    connect(WinMainFrame::instance(),SIGNAL(sigHeartBeat500ms()),this,SLOT(slotReflush()));
}

/*
 * 功能：
 *              初始化界面
 *
 */
void WinChannelAlarmDialog::initAlarmDialog()
{
    setLayout(&m_layout);
    m_layout.addWidget(&m_alarmFrame,Qt::AlignCenter);
    m_layout.setContentsMargins(0,0,0,0);
    m_layout.setSpacing(0);

    m_alarmFrame.setObjectName("AlarmSettingFrame");
    m_alarmFrame.setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    m_alarmFrame.setLayout(&m_alarmFrameLayout);

    m_alarmFrameLayout.setContentsMargins(0,0,0,0);
    m_alarmFrameLayout.setSpacing(25);
    m_alarmFrameLayout.addLayout(&m_titleLayout);
    m_alarmFrameLayout.addWidget(&m_textLabel);
    m_alarmFrameLayout.addLayout(&m_contentLayout);
    m_alarmFrameLayout.addLayout(&m_buttonLayout);

    m_titleLayout.setContentsMargins(12,0,12,0);
    m_titleLayout.setSpacing(0);
    m_titleLayout.addWidget(&m_titleLabel,0,Qt::AlignVCenter);
    m_titleLayout.addSpacerItem(new QSpacerItem(40,59,QSizePolicy::Expanding,QSizePolicy::Fixed));
    m_titleLayout.addWidget(&m_closeBtn,0,Qt::AlignVCenter);
    m_closeBtn.setObjectName("CloseButton");
    m_closeBtn.setFocusPolicy(Qt::NoFocus);
    connect(&m_closeBtn, SIGNAL(clicked()),this,SLOT(close()));
    m_titleLabel.setObjectName("TitleLabel");
    m_titleLabel.setText(tr("Individual alarm ACK"));

    m_textLabel.setObjectName("TextLabel");
    m_textLabel.setText(tr("Select item then press \" OK\" to proceed."));

    m_contentLayout.setSpacing(0);
    m_contentLayout.setContentsMargins(25,0,25,0);
    m_contentLayout.addWidget(&m_alarm1Btn,0,0,1,1);
    m_btnGrp.addButton(&m_alarm1Btn,0);
    m_contentLayout.addWidget(&m_alarm2Btn,0,1,1,1);
    m_btnGrp.addButton(&m_alarm2Btn,1);
    m_contentLayout.addWidget(&m_alarm3Btn,1,0,1,1);
    m_btnGrp.addButton(&m_alarm3Btn,2);
    m_contentLayout.addWidget(&m_alarm4Btn,1,1,1,1);
    m_btnGrp.addButton(&m_alarm4Btn,3);

    m_alarm1Btn.setObjectName("Alarm1Button");
    m_alarm1Btn.setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    m_alarm1Btn.setFocusPolicy(Qt::NoFocus);
    m_alarm1Btn.setCheckable(true);

    m_alarm2Btn.setObjectName("Alarm2Button");
    m_alarm2Btn.setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    m_alarm2Btn.setFocusPolicy(Qt::NoFocus);
    m_alarm2Btn.setCheckable(true);

    m_alarm3Btn.setObjectName("Alarm3Button");
    m_alarm3Btn.setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    m_alarm3Btn.setFocusPolicy(Qt::NoFocus);
    m_alarm3Btn.setCheckable(true);

    m_alarm4Btn.setObjectName("Alarm4Button");
    m_alarm4Btn.setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    m_alarm4Btn.setFocusPolicy(Qt::NoFocus);
    m_alarm4Btn.setCheckable(true);

    m_buttonLayout.setContentsMargins(25,0,25,20);
    m_buttonLayout.setSpacing(0);
    m_buttonLayout.addWidget(&m_okBtn,0,Qt::AlignVCenter);
    m_buttonLayout.addStretch();
    m_buttonLayout.addWidget(&m_cancelBtn,0,Qt::AlignVCenter);
    m_okBtn.setObjectName("OkButton");
    m_okBtn.setText(tr("OK"));
    connect(&m_okBtn,SIGNAL(clicked()),this,SLOT(slotOkButtonClicked()));
    m_cancelBtn.setObjectName("CancelButton");
    m_cancelBtn.setText(tr("Cancel"));
    connect(&m_cancelBtn,SIGNAL(clicked()),this,SLOT(slotCancelButtonClicked()));

    m_alarmFrame.setStyleSheet("\
                   QFrame#AlarmSettingFrame{\
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
                   QPushButton#OkButton,\
                   QPushButton#CancelButton{\
                        width:124px;\
                        height:52px;\
                        font-size:24px;\
                        color:white;\
                        background-color:none;\
                        background-image:url(:/relay/Image/relay/button_normal.png);\
                        background-repeat:no-repeat;\
                        background-position:center;\
                        border:none;\
                    }\
                   QPushButton#OkButton:pressed,\
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
                   QPushButton#Alarm1Button:checked,\
                   QPushButton#Alarm2Button:checked,\
                   QPushButton#Alarm3Button:checked,\
                   QPushButton#Alarm4Button:checked{\
                        background-color:rgb(0,0,0,20%);\
                   }");
}

void WinChannelAlarmDialog::resetChannelNumber()
{
    bool checked = false;
    m_alarm1Btn.setText(p_parent->m_alarm1Char);
    if(p_parent->m_alarm1OnOff != OFF  && (p_parent->m_alarm1Status == SQL_Alarm_Status_Hold_Occur_NoAck ||p_parent->m_alarm1Status == SQL_Alarm_Status_Hold_Release_NoAck)){
        checked = true;
        m_alarm1Btn.setEnabled(true);
        m_alarm1Btn.setChecked(true);
    }else{
        m_alarm1Btn.setEnabled(false);
    }

    m_alarm2Btn.setText(p_parent->m_alarm2Char);
    if(p_parent->m_alarm2OnOff != OFF  && (p_parent->m_alarm2Status == SQL_Alarm_Status_Hold_Occur_NoAck ||p_parent->m_alarm2Status == SQL_Alarm_Status_Hold_Release_NoAck)){
        m_alarm2Btn.setEnabled(true);
        if(!checked){
            checked = true;
            m_alarm2Btn.setChecked(true);
        }
    }else{
        m_alarm2Btn.setEnabled(false);
    }

    m_alarm3Btn.setText(p_parent->m_alarm3Char);
    if(p_parent->m_alarm3OnOff != OFF  && (p_parent->m_alarm3Status == SQL_Alarm_Status_Hold_Occur_NoAck ||p_parent->m_alarm3Status == SQL_Alarm_Status_Hold_Release_NoAck)){
        m_alarm3Btn.setEnabled(true);
        if(!checked){
            checked = true;
            m_alarm3Btn.setChecked(true);
        }
    }else{
        m_alarm3Btn.setEnabled(false);
    }

    m_alarm4Btn.setText(p_parent->m_alarm4Char);
    if(p_parent->m_alarm4OnOff != OFF  && (p_parent->m_alarm4Status == SQL_Alarm_Status_Hold_Occur_NoAck ||p_parent->m_alarm4Status == SQL_Alarm_Status_Hold_Release_NoAck)){
        m_alarm4Btn.setEnabled(true);
        if(!checked){
            checked = true;
            m_alarm4Btn.setChecked(true);
        }
    }else{
        m_alarm4Btn.setEnabled(false);
    }
}

/*
 * 功能：
 *              定时刷新界面
 *
 */
void WinChannelAlarmDialog::slotReflush()
{
    bool blink = WinMainFrame::getBlinkStatus();
    if(m_alarm1Btn.isEnabled()){
        if((p_parent->m_alarm1Status == SQL_Alarm_Status_Hold_Occur_NoAck) &&blink ){
            m_alarm1Btn.setStyleSheet("color:red");
        }else if((p_parent->m_alarm1Status == SQL_Alarm_Status_Hold_Release_NoAck) && blink){
            m_alarm1Btn.setStyleSheet("color:gray");
        }else{
            m_alarm1Btn.setStyleSheet("color:black");
        }
    }
    if(m_alarm2Btn.isEnabled()){
        if((p_parent->m_alarm2Status == SQL_Alarm_Status_Hold_Occur_NoAck)  && blink ){
            m_alarm2Btn.setStyleSheet("color:red");
        }else if((p_parent->m_alarm2Status == SQL_Alarm_Status_Hold_Release_NoAck) && blink){
            m_alarm2Btn.setStyleSheet("color:gray");
        }else{
            m_alarm2Btn.setStyleSheet("color:black");
        }
    }
    if(m_alarm3Btn.isEnabled()){
        if(p_parent->m_alarm3Status == SQL_Alarm_Status_Hold_Occur_NoAck &&blink ){
            m_alarm2Btn.setStyleSheet("color:red");
        }else if(p_parent->m_alarm3Status == SQL_Alarm_Status_Hold_Release_NoAck && blink){
            m_alarm3Btn.setStyleSheet("color:gray");
        }else{
            m_alarm3Btn.setStyleSheet("color:black");
        }
    }
    if(m_alarm4Btn.isEnabled()){
        if(p_parent->m_alarm4Status == SQL_Alarm_Status_Hold_Occur_NoAck &&blink ){
            m_alarm4Btn.setStyleSheet("color:red");
        }else if(p_parent->m_alarm4Status == SQL_Alarm_Status_Hold_Release_NoAck && blink){
            m_alarm4Btn.setStyleSheet("color:gray");
        }else{
            m_alarm4Btn.setStyleSheet("color:black");
        }
    }
}

/*
 *功能：
 *          确认按钮被点击
 *
 */
void WinChannelAlarmDialog::slotOkButtonClicked()
{
    EventAction::instance()->ackAlarmSingle(p_parent->m_chanNum,m_btnGrp.checkedId());
    accept();
}

/*
 *功能：
 *          取消按钮被点击
 *
 */
void WinChannelAlarmDialog::slotCancelButtonClicked()
{
    done(2);
}
