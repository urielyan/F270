#ifndef WINCHANNELINFODIALOG_H
#define WINCHANNELINFODIALOG_H

#include <QFrame>
#include <QToolButton>
#include "windialog.h"
#include "winchanneleditdialog.h"
#include "winchannelalarmdialog.h"
/*
 *通道信息显示对话框
 *
 */
class WinChannelInfoDialog : public WinDialog
{
    Q_OBJECT
    friend class WinChannelAlarmDialog;
    friend class WinChannelEditDialog;
    friend class WinChannelSpanDialog;

public:
    static WinChannelInfoDialog& instance();
    void resetCurrentChannelNumber(quint32 chanNum,bool enableExt=false);//重置当前通道索引

signals:
    void sigUnselectItem();

private slots:
    void slotAlarmButtonClicked();//报警解除按钮被点击
    void slotSettingButtonClicked();//报警设置按钮被点击
    void slotRangeButtonClicked();//量程设置按钮被点击
    void slotAlarmDialogClosed(int result);
    void slotSettingDialogClosed(int result);
    void slotRangeDialogClosed(int result);

private:
    explicit WinChannelInfoDialog();
    void updateCurrentChannelNumber();//更新当前通道索引
    void initAlarmCharacteristic();//初始化报警特征值
    void initMainDialog();//初始化对话框
    bool getAlarmButtonStatus();//获取解除报警按钮显示状态
    bool getSettingButtonStatus();//获取编辑报警按钮显示状态
    QString getAlarmString(quint32 type, QVariant value, int prec, QString defStr);

private:
    quint32 m_chanNum;//通道索引
    bool m_extEnable;

    WinStackedLayout m_layout;
    QVBoxLayout m_mainFrameLayout;
    QHBoxLayout m_titleLayout;
    QGridLayout m_contentLayout;

    QFrame m_mainFrame;//背景
    QPushButton m_closeBtn;
    QLabel m_titleLabel;

    QLabel m_tagNumberPrompt;
    QLabel m_tagNumber;//标签号码
    QLabel m_chanTagPrompt;
    QLabel m_chanTag;//通道标签
    QLabel m_chanNumPrompt;
    QLabel m_chanNumber;//通道号

    QFrame m_line;

    QToolButton m_alarmBtn;//报警解除对话框按钮
    QToolButton m_settingBtn;//报警编辑对话框按钮

    QLabel m_chanAlarm1Prompt;
    QLabel m_chanAlarm1;//报警电平一
    QLabel m_chanAlarm2Prompt;
    QLabel m_chanAlarm2;//报警电平二
    QLabel m_chanAlarm3Prompt;
    QLabel m_chanAlarm3;//报警电平三
    QLabel m_chanAlarm4Prompt;
    QLabel m_chanAlarm4;//报警电平四

    QFrame m_line2;
    QFrame m_line3;
    QLabel m_chanRangeTopPrompt;
    QLabel m_chanRangeTop;//报警电平一
    QLabel m_chanRangeBottomPrompt;
    QLabel m_chanRangeBottom;//报警电平二

    QToolButton m_rangeSettingBtn;
    QPushButton m_unselectBtn;


    QString m_alarm1Char;//报警一类型字符/H/L/R/r/... ...
    quint32 m_alarm1Status;//报警一状态/保持/非保持状态
    quint32 m_alarm1OnOff;//报警一电平开关
    QString m_alarm2Char;//报警二类型字符/H/L/R/r/... ...
    quint32 m_alarm2Status;//报警二状态/保持/非保持状态
    quint32 m_alarm2OnOff;//报警二电平开关
    QString m_alarm3Char;//报警三类型字符/H/L/R/r/... ...
    quint32 m_alarm3Status;//报警三状态/保持/非保持状态
    quint32 m_alarm3OnOff;//报警三电平开关
    QString m_alarm4Char;//报警四类型字符/H/L/R/r/... ...
    quint32 m_alarm4Status;//报警四状态/保持/非保持状态
    quint32 m_alarm4OnOff;//报警四电平开关
    QString m_dispSpanUpper;//量程上限
    QString m_dispSpanLower;//量程下限

    WinChannelAlarmDialog m_alarmDialog;
    WinChannelEditDialog m_editDialog;
    WinChannelSpanDialog m_spanDialog;
};

class WinDOChannelDialog : public WinDialog
{
    Q_OBJECT
public:
    static WinDOChannelDialog& instance();
    void resetCurrentChannelNumber(quint32 chanNum,bool enableExt=false);//重置当前通道索引

signals:
    void sigUnselectItem();

private slots:
    void slotOnButtonClicked();//手动开启内部开关
    void slotOffButtonClicked();//手动关闭内部开关

private:
    void initMainDialog();//初始化对话框
    WinDOChannelDialog();

private:
    quint32 m_chanNum;//通道索引

    WinStackedLayout m_layout;
    QVBoxLayout m_mainFrameLayout;
    QHBoxLayout m_titleLayout;
    QGridLayout m_contentLayout;
    QHBoxLayout m_btnLayout;


    QFrame m_mainFrame;//背景
    QPushButton m_closeBtn;
    QLabel m_titleLabel;

    QLabel m_tagNumberPrompt;
    QLabel m_tagNumber;//标签号码
    QLabel m_chanTagPrompt;
    QLabel m_chanTag;//通道标签
    QLabel m_chanNumPrompt;
    QLabel m_chanNumber;//通道号

    QFrame m_line;

    QPushButton m_offBtn;//继电器关闭按钮
    QPushButton m_onBtn;//继电器打开按钮
    QLabel m_relayLabel;

    QFrame m_line2;

    QPushButton m_unselectBtn;
};

#endif // WINCHANNELINFODIALOG_H
