#include <QPushButton>
#include <QLabel>
#include <QFrame>
#include <QString>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QStackedLayout>
#include <QDebug>
#include "winchannelinfodialog.h"
#include "winchannelalarmdialog.h"
#include "winchanneleditdialog.h"
#include "./GlobalData/cfgchannel.h"
#include "./GlobalData/cfgGlobalDef.h"
#include "./Common/eventaction.h"
#include "./Common/authorization.h"

WinChannelInfoDialog::WinChannelInfoDialog() :
    WinDialog(),m_chanNum(GROUP_NUM_MAX),m_alarmDialog(this),m_editDialog(this),m_spanDialog(this)
{
    connect(&m_alarmDialog,SIGNAL(finished(int)),this,SLOT(slotAlarmDialogClosed(int)));
    connect(&m_editDialog,SIGNAL(finished(int)),this,SLOT(slotSettingDialogClosed(int)));
    connect(&m_spanDialog,SIGNAL(finished(int)),this,SLOT(slotRangeDialogClosed(int)));

    initMainDialog();
}

WinChannelInfoDialog& WinChannelInfoDialog::instance()
{
    static WinChannelInfoDialog instance;
    return instance;
}

/*
 * 功能：
 *              初始化报警各个特征值
 */
void WinChannelInfoDialog::initAlarmCharacteristic()
{
    int dot = CfgChannel::instance()->getChannelDataAccuracy(m_chanNum).toInt();

    /***************************电平一*****************************/
    QVariant alarmOnOff = CfgChannel::instance()->getChannelAlarm1OnOff(m_chanNum);
    QVariant alarmType = CfgChannel::instance()->getChannelAlarm1Type(m_chanNum);
    QVariant alarmValue = CfgChannel::instance()->getChannelAlarm1Value(m_chanNum);
    QVariant alarmStatus = CfgChannel::instance()->getChannelAlarm1Status(m_chanNum);
    m_alarm1Char = tr("OFF");
    m_alarm1Status = SQL_Alarm_Status_NonHold_AllOff;
    m_alarm1OnOff = OFF;
    if(alarmOnOff.isValid()){
        m_alarm1OnOff = alarmOnOff.toUInt();
        if(ON == m_alarm1OnOff && alarmType.isValid() && alarmStatus.isValid()){
            m_alarm1Status = alarmStatus.toUInt();
            m_alarm1Char = getAlarmString(alarmType.toUInt(),alarmValue,dot,m_alarm1Char);
        }
    }

    /***************************电平二*****************************/
    alarmOnOff = CfgChannel::instance()->getChannelAlarm2OnOff(m_chanNum);
    alarmType = CfgChannel::instance()->getChannelAlarm2Type(m_chanNum);
    alarmValue = CfgChannel::instance()->getChannelAlarm2Value(m_chanNum);
    alarmStatus = CfgChannel::instance()->getChannelAlarm2Status(m_chanNum);
    m_alarm2Char = tr("OFF");
    m_alarm2Status = SQL_Alarm_Status_NonHold_AllOff;
    m_alarm2OnOff = OFF;
    if(alarmOnOff.isValid()){
        m_alarm2OnOff = alarmOnOff.toUInt();
        if(ON == m_alarm2OnOff && alarmType.isValid() && alarmStatus.isValid()){
            m_alarm2Status = alarmStatus.toUInt();
            m_alarm2Char = getAlarmString(alarmType.toUInt(),alarmValue,dot,m_alarm2Char);
        }
    }

    /***************************电平三*****************************/
    alarmOnOff = CfgChannel::instance()->getChannelAlarm3OnOff(m_chanNum);
    alarmType = CfgChannel::instance()->getChannelAlarm3Type(m_chanNum);
    alarmValue = CfgChannel::instance()->getChannelAlarm3Value(m_chanNum);
    alarmStatus = CfgChannel::instance()->getChannelAlarm3Status(m_chanNum);
    m_alarm3Char = tr("OFF");
    m_alarm3Status = SQL_Alarm_Status_NonHold_AllOff;
    m_alarm3OnOff = OFF;
    if(alarmOnOff.isValid()){
        m_alarm3OnOff = alarmOnOff.toUInt();
        if(ON == m_alarm3OnOff && alarmType.isValid() && alarmStatus.isValid()){
            m_alarm3Status = alarmStatus.toUInt();
            m_alarm3Char = getAlarmString(alarmType.toUInt(),alarmValue,dot,m_alarm3Char);
        }
    }

    /***************************电平四*****************************/
    alarmOnOff = CfgChannel::instance()->getChannelAlarm4OnOff(m_chanNum);
    alarmType = CfgChannel::instance()->getChannelAlarm4Type(m_chanNum);
    alarmValue = CfgChannel::instance()->getChannelAlarm4Value(m_chanNum);
    alarmStatus = CfgChannel::instance()->getChannelAlarm4Status(m_chanNum);
    m_alarm4Char = tr("OFF");
    m_alarm4Status = SQL_Alarm_Status_NonHold_AllOff;
    m_alarm4OnOff = OFF;
    if(alarmOnOff.isValid()){
        m_alarm4OnOff = alarmOnOff.toUInt();
        if(ON == m_alarm4OnOff && alarmType.isValid() && alarmStatus.isValid()){
            m_alarm4Status = alarmStatus.toUInt();
            m_alarm4Char = getAlarmString(alarmType.toUInt(),alarmValue,dot,m_alarm4Char);
        }
    }

    /****************************量程***************************/
    QVariant span= CfgChannel::instance()->getChannelRangeUpper(m_chanNum);
    if(span.isValid()){
        m_dispSpanUpper = QString::number(span.toReal(),'f',dot);
    }else{
        m_dispSpanUpper = "";
    }
    span = CfgChannel::instance()->getChannelRangeDowner(m_chanNum);
    if(span.isValid()){
        m_dispSpanLower = QString::number(span.toReal(),'f',dot);
    }else{
        m_dispSpanLower = "";
    }
}

/**
 * 功能：
 *      获取报警电平对应显示字符串
 * 参数值：
 *      １、quint32 alarmType　报警类型
 *      ２、QVariant value　报警值
 *      ３、int prec　数据精度
 * 返回值：
 *      QString：显示字符串
 */
QString WinChannelInfoDialog::getAlarmString(quint32 type, QVariant value, int prec,QString defStr)
{
    QString ret = defStr;
    switch(type){
    case Alarm_Type_H://上限
        ret = tr("H  %1").arg(QString::number(value.toReal(),'f',prec));
        break;
    case Alarm_Type_h://差值上限， Delta运算时有效
        ret = tr("h  %1").arg(QString::number(value.toReal(),'f',prec));
        break;
    case Alarm_Type_L://下限
        ret = tr("L  %1").arg(QString::number(value.toReal(),'f',prec));
        break;
    case Alarm_Type_l://差值下限， Delta运算时有效
        ret = tr("l  %1").arg(QString::number(value.toReal(),'f',prec));
        break;
    case Alarm_Type_R://变化率上升限
        ret = tr("R  %1").arg(QString::number(value.toReal(),'f',prec));
        break;
    case Alarm_Type_r://变化率下升限
        ret = tr("r  %1").arg(QString::number(value.toReal(),'f',prec));
        break;
    case Alarm_Type_T://延迟上限
        ret = tr("T  %1").arg(QString::number(value.toReal(),'f',prec));
        break;
    case Alarm_Type_t://延迟下限
        ret = tr("t  %1").arg(QString::number(value.toReal(),'f',prec));
        break;
    }

    return ret;
}

/*
 * 功能：
 *              初始化界面
 */
void WinChannelInfoDialog::initMainDialog()
{
    this->setLayout(&m_layout);
    m_layout.addWidget(&m_mainFrame,Qt::AlignCenter);
    m_layout.setContentsMargins(0,0,0,0);
    m_layout.setSpacing(0);

    m_mainFrame.setObjectName("BackgroundFrame");
    m_mainFrame.setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    m_mainFrame.setLayout(&m_mainFrameLayout);

    m_mainFrameLayout.setContentsMargins(0,0,0,0);
    m_mainFrameLayout.setSpacing(10);
    m_mainFrameLayout.addLayout(&m_titleLayout);
    m_mainFrameLayout.addLayout(&m_contentLayout);
    m_mainFrameLayout.addStretch();

    m_titleLayout.setContentsMargins(12,0,12,0);
    m_titleLayout.setSpacing(0);
    m_titleLayout.addWidget(&m_titleLabel,0,Qt::AlignVCenter);
    m_titleLayout.addSpacerItem(new QSpacerItem(40,59,QSizePolicy::Expanding,QSizePolicy::Fixed));
    m_titleLayout.addWidget(&m_closeBtn,0,Qt::AlignVCenter);
    m_closeBtn.setObjectName("CloseButton");
    m_closeBtn.setFocusPolicy(Qt::NoFocus);
    connect(&m_closeBtn, SIGNAL(clicked()),this,SLOT(close()));
    m_titleLabel.setObjectName("TitleLabel");
    m_titleLabel.setText(tr("Channel Information"));

    m_contentLayout.setContentsMargins(25,10,25,10);
    m_contentLayout.setSpacing(12);
    m_contentLayout.setColumnStretch(0,1);
    m_contentLayout.setColumnStretch(1,1);
    m_contentLayout.setColumnStretch(2,1);
    m_contentLayout.addWidget(&m_tagNumberPrompt,0,0,1,1,Qt::AlignLeft);
    m_contentLayout.addWidget(&m_tagNumber,0,1,1,1,Qt::AlignLeft);
    m_contentLayout.addWidget(&m_chanTagPrompt,1,0,1,1,Qt::AlignLeft);
    m_contentLayout.addWidget(&m_chanTag,1,1,1,1,Qt::AlignLeft);
    m_contentLayout.addWidget(&m_chanNumPrompt,2,0,1,1,Qt::AlignLeft);
    m_contentLayout.addWidget(&m_chanNumber,2,1,1,1,Qt::AlignLeft);
    m_contentLayout.addWidget(&m_line,3,0,1,3);
    m_contentLayout.addWidget(&m_chanAlarm1Prompt,4,0,1,1,Qt::AlignLeft);
    m_contentLayout.addWidget(&m_chanAlarm1,4,1,1,1,Qt::AlignLeft);
    m_contentLayout.addWidget(&m_chanAlarm2Prompt,5,0,1,1,Qt::AlignLeft);
    m_contentLayout.addWidget(&m_chanAlarm2,5,1,1,1,Qt::AlignLeft);
    m_contentLayout.addWidget(&m_chanAlarm3Prompt,6,0,1,1,Qt::AlignLeft);
    m_contentLayout.addWidget(&m_chanAlarm3,6,1,1,1,Qt::AlignLeft);
    m_contentLayout.addWidget(&m_chanAlarm4Prompt,7,0,1,1,Qt::AlignLeft);
    m_contentLayout.addWidget(&m_chanAlarm4,7,1,1,1,Qt::AlignLeft);
    m_contentLayout.addWidget(&m_settingBtn,4,2,2,1,Qt::AlignCenter);
    m_contentLayout.addWidget(&m_alarmBtn,6,2,2,1,Qt::AlignCenter);
    m_contentLayout.addWidget(&m_line2,8,0,1,3);
    m_contentLayout.addWidget(&m_chanRangeTopPrompt,9,0,1,1,Qt::AlignLeft);
    m_contentLayout.addWidget(&m_chanRangeTop,9,1,1,1,Qt::AlignLeft);
    m_contentLayout.addWidget(&m_chanRangeBottomPrompt,10,0,1,1,Qt::AlignLeft);
    m_contentLayout.addWidget(&m_chanRangeBottom,10,1,1,1,Qt::AlignLeft);
    m_contentLayout.addWidget(&m_rangeSettingBtn,9,2,2,1,Qt::AlignCenter);
    m_contentLayout.addWidget(&m_line3,11,0,1,3);
    m_contentLayout.addWidget(&m_unselectBtn,12,0,1,3);

    m_tagNumberPrompt.setObjectName("TagNumberPromptLabel");
    m_tagNumberPrompt.setText(tr("Tag No."));
    m_tagNumber.setObjectName("TagNumberLabel");
    m_chanTagPrompt.setObjectName("ChannelTagPromptLabel");
    m_chanTagPrompt.setText(tr("Tag"));
    m_chanTag.setObjectName("ChannelTagLabel");
    m_chanNumPrompt.setObjectName("ChannelNumberPromptLabel");
    m_chanNumPrompt.setText(tr("Channel No."));
    m_chanNumber.setObjectName("ChannelNumberLabel");
    m_line.setObjectName("HLine");
    m_line.setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    m_line.setFrameShape(QFrame::HLine);
    m_line.setFrameShadow(QFrame::Sunken);

    QIcon icon;
    m_alarmBtn.setObjectName("AlarmButton");
    m_alarmBtn.setText(tr("Alarm ACK"));
    m_alarmBtn.setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    connect(&m_alarmBtn,SIGNAL(clicked()),this,SLOT(slotAlarmButtonClicked()));
    icon.addPixmap(QPixmap(":/common/Image/common/alarm_btn_icon.png"),QIcon::Normal);
    m_alarmBtn.setIcon(icon);
    m_alarmBtn.setIconSize(QSize(32,32));
    m_alarmBtn.setFocusPolicy(Qt::NoFocus);

    m_settingBtn.setObjectName("SettingButton");
    m_settingBtn.setText(tr("Edit"));
    m_settingBtn.setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    connect(&m_settingBtn,SIGNAL(clicked()),this,SLOT(slotSettingButtonClicked()));
    icon.addPixmap(QPixmap(":/common/Image/common/setting_btn_icon.png"),QIcon::Normal);
    m_settingBtn.setIcon(icon);
    m_settingBtn.setIconSize(QSize(32,32));
    m_settingBtn.setFocusPolicy(Qt::NoFocus);

    m_chanAlarm1Prompt.setObjectName("ChannelAlarm1Prompt");
    m_chanAlarm1Prompt.setText(tr("Alarm lv1"));
    m_chanAlarm1.setObjectName("ChannelAlarm1");
    m_chanAlarm2Prompt.setObjectName("ChannelAlarm2Prompt");
    m_chanAlarm2Prompt.setText(tr("Alarm lv2"));
    m_chanAlarm2.setObjectName("ChannelAlarm2");
    m_chanAlarm3Prompt.setObjectName("ChannelAlarm3Prompt");
    m_chanAlarm3Prompt.setText(tr("Alarm lv3"));
    m_chanAlarm3.setObjectName("ChannelAlarm3");
    m_chanAlarm4Prompt.setObjectName("ChannelAlarm4Prompt");
    m_chanAlarm4Prompt.setText(tr("Alarm lv4"));
    m_chanAlarm4.setObjectName("ChannelAlarm4");

    m_line2.setObjectName("HLine2");
    m_line2.setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    m_line2.setFrameShape(QFrame::HLine);
    m_line2.setFrameShadow(QFrame::Sunken);
    m_line3.setObjectName("HLine3");
    m_line3.setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    m_line3.setFrameShape(QFrame::HLine);
    m_line3.setFrameShadow(QFrame::Sunken);

    m_chanRangeTopPrompt.setObjectName("ChannelRangeTopPrompt");
    m_chanRangeTopPrompt.setText(tr("Disp span upper"));
    m_chanRangeTop.setObjectName("ChannelRangeTop");
    m_chanRangeBottomPrompt.setObjectName("ChannelRangeBottomPrompt");
    m_chanRangeBottomPrompt.setText(tr("Disp span lower"));
    m_chanRangeBottom.setObjectName("ChannelRangeBottom");

    m_rangeSettingBtn.setObjectName("RangeButton");
    m_rangeSettingBtn.setText(tr("Edit"));
    m_rangeSettingBtn.setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    connect(&m_rangeSettingBtn,SIGNAL(clicked()),this,SLOT(slotRangeButtonClicked()));
    icon.addPixmap(QPixmap(":/common/Image/common/setting_btn_icon.png"),QIcon::Normal);
    m_rangeSettingBtn.setIcon(icon);
    m_rangeSettingBtn.setIconSize(QSize(32,32));
    m_rangeSettingBtn.setFocusPolicy(Qt::NoFocus);

    m_unselectBtn.setObjectName("UnselectButton");
    connect(&m_unselectBtn,SIGNAL(clicked()),this,SIGNAL(sigUnselectItem()));
    connect(&m_unselectBtn,SIGNAL(clicked()),this,SLOT(close()));
    m_unselectBtn.setText(tr("Unselect"));
    m_unselectBtn.setFocusPolicy(Qt::NoFocus);

    m_mainFrame.setStyleSheet("\
                    QFrame#BackgroundFrame{\
                        min-width:722px;\
                        max-width:722px;\
                        min-height:400px;\
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
                    QLabel#TagNumberPromptLabel,\
                    QLabel#ChannelTagPromptLabel,\
                    QLabel#ChannelNumberPromptLabel,\
                    QLabel#ChannelAlarm1Prompt,\
                    QLabel#ChannelAlarm2Prompt,\
                    QLabel#ChannelAlarm3Prompt,\
                    QLabel#ChannelAlarm4Prompt,\
                    QLabel#ChannelRangeTopPrompt,\
                    QLabel#ChannelRangeBottomPrompt{\
                        color:#404040;\
                        font-size:20px;\
                        margin-left:10px;\
                    }\
                    QLabel#TagNumberLabel,\
                    QLabel#ChannelTagLabel,\
                    QLabel#ChannelNumberLabel,\
                    QLabel#ChannelAlarm1,\
                    QLabel#ChannelAlarm2,\
                    QLabel#ChannelAlarm3,\
                    QLabel#ChannelAlarm4,\
                    QLabel#ChannelRangeTop,\
                    QLabel#ChannelRangeBottom{\
                        color:black;\
                        font-size:22px;\
                    }\
                    QToolButton#AlarmButton,\
                    QToolButton#SettingButton,\
                    QToolButton#RangeButton{\
                        width:95px;\
                        height:65px;\
                     }\
                     QPushButton#UnselectButton{\
                        height:45px;\
                        font-size:20px;\
                        border:1px solid black;\
                        background-color:white;\
                        color:black;\
                        margin-top:20px;\
                        margin-left:10px;\
                        margin-right:10px;\
                        margin-bottom:10px;\
                    }\
                    QPushButton#UnselectButton:pressed{\
                         border-color:green;\
                         background-color:darkGray;\
                   }");
}

/*
 * 功能：
 *              获取报警解除按钮状态
 *
 * 返回值：
 *              bool 按钮状态enable/true, disable/off
 */
bool WinChannelInfoDialog::getAlarmButtonStatus()
{
    if ((m_alarm1OnOff != OFF) && (m_alarm1Status == SQL_Alarm_Status_Hold_Occur_NoAck ||m_alarm1Status == SQL_Alarm_Status_Hold_Release_NoAck)){
        return true;
    }

    if((m_alarm2OnOff != OFF) && (m_alarm2Status == SQL_Alarm_Status_Hold_Occur_NoAck ||m_alarm2Status == SQL_Alarm_Status_Hold_Release_NoAck)){
        return true;
    }

    if((m_alarm3OnOff != OFF) && (m_alarm3Status == SQL_Alarm_Status_Hold_Occur_NoAck ||m_alarm3Status == SQL_Alarm_Status_Hold_Release_NoAck)){
        return true;
    }

    if((m_alarm4OnOff != OFF) && (m_alarm4Status == SQL_Alarm_Status_Hold_Occur_NoAck ||m_alarm4Status == SQL_Alarm_Status_Hold_Release_NoAck)){
        return true;
    }

    return false;
}

/*
 * 功能：
 *              获取报警设置按钮状态
 *
 * 返回值：
 *              bool 按钮状态enable/true, disable/off
 */
bool WinChannelInfoDialog::getSettingButtonStatus()
{
    if (m_alarm1OnOff != OFF){
        return true;
    }

    if(m_alarm2OnOff != OFF){
        return true;
    }

    if(m_alarm3OnOff != OFF){
        return true;
    }

    if(m_alarm4OnOff != OFF){
        return true;
    }

    return false;
}

/*
 * 功能：
 *              报警解除按钮被点击
 *
 */
void WinChannelInfoDialog::slotAlarmButtonClicked()
{
    this->setHidden(true);
    m_alarmDialog.exec();
}

/*
 * 功能：
 *              报警设置按钮被点击
 *
 */
void WinChannelInfoDialog::slotSettingButtonClicked()
{
    this->setHidden(true);
    m_editDialog.exec() ;
}

void WinChannelInfoDialog::slotRangeButtonClicked()
{
    this->setHidden(true);
    m_spanDialog.exec();
}

void WinChannelInfoDialog::slotAlarmDialogClosed(int result)
{
    if(result == QDialog::Rejected ||result == QDialog::Accepted){
        this->close();
    }else{
        updateCurrentChannelNumber();
        this->setHidden(false);
    }
}

void WinChannelInfoDialog::slotSettingDialogClosed(int result)
{
    if(result == QDialog::Accepted){
        updateCurrentChannelNumber();
        this->setHidden(false);
    }else{
        this->close();
    }
}

void WinChannelInfoDialog::slotRangeDialogClosed(int result)
{
    if(result == QDialog::Accepted){
        updateCurrentChannelNumber();
        this->setHidden(false);
    }else{
        this->close();
    }
}

/*
 * 功能：
 *              重置当前通道索引
 *
 */
void WinChannelInfoDialog::resetCurrentChannelNumber(quint32 chanNum,bool enableExt)
{
    m_chanNum = chanNum;
    m_extEnable = enableExt;
    initAlarmCharacteristic();
    m_alarmDialog.resetChannelNumber();
    m_editDialog.resetChannelNumber();
    m_spanDialog.resetChannelNumber();

    QVariant  value = CfgChannel::instance()->getChannelTagNumber(m_chanNum);
    if(value.isValid()){
        m_tagNumber.setText(value.toString());
    }else{
        m_tagNumber.setText("");
    }

    value = CfgChannel::instance()->getChannelTagName(m_chanNum);
    if(value.isValid()){
        m_chanTag.setText(value.toString());
    }else{
        m_chanTag.setText("");
    }

    value = CfgChannel::instance()->getChannelNumber(m_chanNum);
    if(value.isValid()){
        m_chanNumber.setText(value.toString());
    }else{
        m_chanNumber.setText("");
    }

    if(CfgChannel::instance()->getSysIndividualAlarmAckConfig()){
        m_alarmBtn.setVisible(true);
        if(getAlarmButtonStatus()){
                if(Authorization::instance()->getLoginStatus() == Authorization::Auth_LoginStatus_Login && Authorization::instance()->getLoginedUser()->isLocked(Security_ModuleAuth_ACK)){
                    m_alarmBtn.setEnabled(false);
                }else{
                    m_alarmBtn.setEnabled(true);
                }
        }
    }else{
        m_alarmBtn.setVisible(false);
    }

    if(CfgChannel::instance()->getSysAlarmChangeConfig()){
        m_settingBtn.setVisible(true);
        if(getSettingButtonStatus()){
            if(Authorization::instance()->getLoginStatus() == Authorization::Auth_LoginStatus_Login && Authorization::instance()->getLoginedUser()->isLocked(Security_ModuleAuth_Setting)){
                m_settingBtn.setEnabled(false);
            }else{
                m_settingBtn.setEnabled(true);
            }
        }else{
            m_settingBtn.setEnabled(false);
        }
    }else{
        m_settingBtn.setVisible(false);
    }

    m_chanAlarm1.setText(m_alarm1Char);
    m_chanAlarm2.setText(m_alarm2Char);
    m_chanAlarm3.setText(m_alarm3Char);
    m_chanAlarm4.setText(m_alarm4Char);

    if(m_extEnable){
        m_line2.setVisible(true);
        m_line3.setVisible(true);
        m_chanRangeTopPrompt.setVisible(true);
        m_chanRangeTop.setVisible(true);
        m_chanRangeBottomPrompt.setVisible(true);
        m_chanRangeBottom.setVisible(true);
        m_rangeSettingBtn.setVisible(true);
        m_unselectBtn.setVisible(true);
        m_chanRangeTop.setText(m_dispSpanUpper);
        m_chanRangeBottom.setText(m_dispSpanLower);
        if(Authorization::instance()->getLoginStatus() == Authorization::Auth_LoginStatus_Login && Authorization::instance()->getLoginedUser()->isLocked(Security_ModuleAuth_Setting)){
            m_rangeSettingBtn.setEnabled(false);
        }else{
            m_rangeSettingBtn.setEnabled(true);
        }
    }else{
        m_line2.setVisible(false);
        m_line3.setVisible(false);
        m_chanRangeTopPrompt.setVisible(false);
        m_chanRangeTop.setVisible(false);
        m_chanRangeBottomPrompt.setVisible(false);
        m_chanRangeBottom.setVisible(false);
        m_rangeSettingBtn.setVisible(false);
        m_unselectBtn.setVisible(false);
    }
}

void WinChannelInfoDialog::WinChannelInfoDialog::updateCurrentChannelNumber()//重置当前通道索引
{
    m_chanAlarm1.setText(m_alarm1Char);
    m_chanAlarm2.setText(m_alarm2Char);
    m_chanAlarm3.setText(m_alarm3Char);
    m_chanAlarm4.setText(m_alarm4Char);
    if(m_extEnable){
        m_chanRangeTop.setText(m_dispSpanUpper);
        m_chanRangeBottom.setText(m_dispSpanLower);
    }
}

WinDOChannelDialog::WinDOChannelDialog():
    WinDialog(),m_chanNum(GROUP_NUM_MAX)
{
    connect(&m_offBtn,SIGNAL(clicked()),this,SLOT(slotOnButtonClicked()));
    connect(&m_onBtn,SIGNAL(clicked()),this,SLOT(slotOffButtonClicked()));

    initMainDialog();
}

WinDOChannelDialog& WinDOChannelDialog::instance()
{
    static WinDOChannelDialog obj;
    return obj;
}

void WinDOChannelDialog::initMainDialog()
{
    this->setLayout(&m_layout);
    m_layout.addWidget(&m_mainFrame,Qt::AlignCenter);
    m_layout.setContentsMargins(0,0,0,0);
    m_layout.setSpacing(0);

    m_mainFrame.setObjectName("BackgroundFrame");
    m_mainFrame.setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    m_mainFrame.setLayout(&m_mainFrameLayout);

    m_mainFrameLayout.setContentsMargins(0,0,0,0);
    m_mainFrameLayout.setSpacing(10);
    m_mainFrameLayout.addLayout(&m_titleLayout);
    m_mainFrameLayout.addLayout(&m_contentLayout);
//    m_mainFrameLayout.addLayout(&m_relayLayout);
    m_mainFrameLayout.addStretch();

    m_titleLayout.setContentsMargins(12,0,12,0);
    m_titleLayout.setSpacing(0);
    m_titleLayout.addWidget(&m_titleLabel,0,Qt::AlignVCenter);
    m_titleLayout.addSpacerItem(new QSpacerItem(40,59,QSizePolicy::Expanding,QSizePolicy::Fixed));
    m_titleLayout.addWidget(&m_closeBtn,0,Qt::AlignVCenter);
    m_closeBtn.setObjectName("CloseButton");
    m_closeBtn.setFocusPolicy(Qt::NoFocus);
    connect(&m_closeBtn, SIGNAL(clicked()),this,SLOT(close()));
//    connect(&m_closeBtn, SIGNAL(clicked()),this,SIGNAL(sigUnselectItem()));
    m_titleLabel.setObjectName("TitleLabel");
    m_titleLabel.setText(tr("Channel Information"));

    m_contentLayout.setContentsMargins(25,10,25,10);
    m_contentLayout.setSpacing(12);
    m_contentLayout.setColumnStretch(0,1);
    m_contentLayout.setColumnStretch(1,1);
    m_contentLayout.setColumnStretch(2,1);
    m_contentLayout.addWidget(&m_tagNumberPrompt,0,0,1,1,Qt::AlignLeft);
    m_contentLayout.addWidget(&m_tagNumber,0,1,1,1,Qt::AlignLeft);
    m_contentLayout.addWidget(&m_chanTagPrompt,1,0,1,1,Qt::AlignLeft);
    m_contentLayout.addWidget(&m_chanTag,1,1,1,1,Qt::AlignLeft);
    m_contentLayout.addWidget(&m_chanNumPrompt,2,0,1,1,Qt::AlignLeft);
    m_contentLayout.addWidget(&m_chanNumber,2,1,1,1,Qt::AlignLeft);
    m_contentLayout.addWidget(&m_line,3,0,1,3);
    m_contentLayout.addWidget(&m_relayLabel,4,0,1,3);
    m_contentLayout.addLayout(&m_btnLayout,5,0,1,3);
    m_contentLayout.addWidget(&m_line2,6,0,1,3);
    m_contentLayout.addWidget(&m_unselectBtn,7,0,1,3);
    m_contentLayout.setRowMinimumHeight(4,30);
    m_contentLayout.setRowMinimumHeight(5,60);

    m_tagNumberPrompt.setObjectName("TagNumberPromptLabel");
    m_tagNumberPrompt.setText(tr("Tag No."));
    m_tagNumber.setObjectName("TagNumberLabel");
    m_chanTagPrompt.setObjectName("ChannelTagPromptLabel");
    m_chanTagPrompt.setText(tr("Tag"));
    m_chanTag.setObjectName("ChannelTagLabel");
    m_chanNumPrompt.setObjectName("ChannelNumberPromptLabel");
    m_chanNumPrompt.setText(tr("Channel No."));
    m_chanNumber.setObjectName("ChannelNumberLabel");
    m_line.setObjectName("HLine");
    m_line.setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    m_line.setFrameShape(QFrame::HLine);
    m_line.setFrameShadow(QFrame::Sunken);

    m_relayLabel.setObjectName("RelayName");
    m_relayLabel.setText(tr("Relay"));
    m_relayLabel.setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    m_relayLabel.setAlignment(Qt::AlignCenter);

    m_btnLayout.setSpacing(0);
    m_btnLayout.setContentsMargins(25,0,25,0);
    m_btnLayout.addWidget(&m_onBtn);
    m_btnLayout.addWidget(&m_offBtn);
    m_onBtn.setObjectName("ONButton");
    m_onBtn.setText(tr("On"));
    m_onBtn.setFocusPolicy(Qt::NoFocus);
    m_offBtn.setObjectName("OFFButton");
    m_offBtn.setText(tr("Off"));
    m_offBtn.setFocusPolicy(Qt::NoFocus);

    m_line2.setObjectName("HLine2");
    m_line2.setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    m_line2.setFrameShape(QFrame::HLine);
    m_line2.setFrameShadow(QFrame::Sunken);


    m_unselectBtn.setObjectName("UnselectButton");
    connect(&m_unselectBtn,SIGNAL(clicked()),this,SIGNAL(sigUnselectItem()));
    connect(&m_unselectBtn,SIGNAL(clicked()),this,SLOT(close()));
    m_unselectBtn.setText(tr("Unselect"));
    m_unselectBtn.setFocusPolicy(Qt::NoFocus);

    m_mainFrame.setStyleSheet("\
                    QFrame#BackgroundFrame{\
                        min-width:722px;\
                        max-width:722px;\
                        min-height:400px;\
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
                    QLabel#TagNumberPromptLabel,\
                    QLabel#ChannelTagPromptLabel,\
                    QLabel#ChannelNumberPromptLabel{\
                        color:#404040;\
                        font-size:20px;\
                        margin-left:10px;\
                    }\
                   QLabel#TagNumberLabel,\
                   QLabel#ChannelTagLabel,\
                   QLabel#ChannelNumberLabel{\
                       color:black;\
                       font-size:22px;\
                   }\
                   QPushButton#ONButton,\
                   QPushButton#OFFButton{\
                        min-height:60px;\
                        font-size:32px;\
                        border: 1px solid #D3D3D3;\
                        background-color:rgb(0,0,0,0%);\
                        margin-left:10px;\
                        margin-right:10px;\
                   }\
                  QPushButton#ONButton:pressed,\
                  QPushButton#OFFButton:pressed{\
                        background-color:rgb(0,0,0,20%);\
                   }\
                  QLabel#RelayName{\
                       font-size:28px;\
                   }\
                  QPushButton#UnselectButton{\
                     height:55px;\
                     font-size:20px;\
                     border:1px solid black;\
                     background-color:white;\
                     color:black;\
                     margin-top:20px;\
                     margin-left:10px;\
                     margin-right:10px;\
                     margin-bottom:10px;\
                  }\
                 QPushButton#UnselectButton:pressed{\
                      border-color:green;\
                      background-color:darkGray;\
                 }");
}

void WinDOChannelDialog::resetCurrentChannelNumber(quint32 chanNum,bool enableExt)
{
    m_chanNum = chanNum;

    QVariant  value = CfgChannel::instance()->getChannelTagNumber(m_chanNum);
    if(value.isValid()){
        m_tagNumber.setText(value.toString());
    }else{
        m_tagNumber.setText("");
    }

    value = CfgChannel::instance()->getChannelTagName(m_chanNum);
    if(value.isValid()){
        m_chanTag.setText(value.toString());
    }else{
        m_chanTag.setText("");
    }

    value = CfgChannel::instance()->getChannelNumber(m_chanNum);
    if(value.isValid()){
        m_chanNumber.setText(value.toString());
    }else{
        m_chanNumber.setText("");
    }

    if(CfgChannel::instance()->getSysAlarmChangeConfig() && (CfgChannel::instance()->getChannelRangeType(m_chanNum).toUInt() == 1)){//手动
        m_onBtn.setVisible(true);
        m_offBtn.setVisible(true);
        m_relayLabel.setVisible(true);
        if(Authorization::instance()->getLoginStatus() == Authorization::Auth_LoginStatus_Login && Authorization::instance()->getLoginedUser()->isLocked(Security_ModuleAuth_OutPutOpe)){
            m_onBtn.setEnabled(false);
            m_offBtn.setEnabled(false);
        }else{
            m_onBtn.setEnabled(true);
            m_offBtn.setEnabled(true);
        }
    }else{
        m_onBtn.setVisible(false);
        m_offBtn.setVisible(false);
        m_relayLabel.setVisible(false);
    }

    if(enableExt){
        m_line2.setVisible(true);
        m_unselectBtn.setVisible(true);
    }else{
        m_line2.setVisible(false);
        m_unselectBtn.setVisible(false);
    }
}

void WinDOChannelDialog::slotOnButtonClicked()
{
    //手动打开DO通道
    EventAction::instance()->setRelayDO(m_chanNum,true);
}

void WinDOChannelDialog::slotOffButtonClicked()
{
    //手动关闭DO通道
    EventAction::instance()->setRelayDO(m_chanNum,false);
}

