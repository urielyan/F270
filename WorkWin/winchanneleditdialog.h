#ifndef WINCHANNELEDITDIALOG_H
#define WINCHANNELEDITDIALOG_H

#include <QFrame>
#include "windialog.h"
#include "./Common/digitaloperation.h"

class WinChannelInfoDialog;
/*
 * 通道报警上下限编辑对话框
 *
 */
class WinChannelEditDialog : public WinDialog
{
    Q_OBJECT
public:
    explicit WinChannelEditDialog(WinChannelInfoDialog *parent);
    void resetChannelNumber();

private:
    void initAlarmDialog();//初始化界面


private slots:
    void slotAlarmLevel1Edit();
    void slotAlarmLevel2Edit();
    void slotAlarmLevel3Edit();
    void slotAlarmLevel4Edit();
    void slotDigitalPadFinish();

private:
    WinChannelInfoDialog *p_parent;//父对话框

    WinStackedLayout m_layout;
    QVBoxLayout m_alarmFrameLayout;
    QHBoxLayout m_titleLayout;
    QGridLayout m_contentLayout;

    QFrame m_alarmFrame;//背景
    QPushButton m_closeBtn;
    QLabel m_titleLabel;

    QPushButton m_alarm1Btn;
    QPushButton m_alarm2Btn;
    QPushButton m_alarm3Btn;
    QPushButton m_alarm4Btn;

    QPushButton m_cancelBtn;
    QLabel m_textLabel;

    DigitalOperation m_digitalPad;
    quint8 m_chanLevel;
};


/*
 * 通道量程上下限编辑对话框
 *
 */
class WinChannelSpanDialog : public WinDialog
{
    Q_OBJECT
public:
    explicit WinChannelSpanDialog(WinChannelInfoDialog *parent);
    void resetChannelNumber();

private:
    void initAlarmDialog();//初始化界面

private slots:
    void slotAlarmSpanUpperEdit();
    void slotAlarmSpanLowerEdit();
    void slotDigitalPadFinish();


private:
    WinChannelInfoDialog *p_parent;//父对话框

    WinStackedLayout m_layout;
    QVBoxLayout m_alarmFrameLayout;
    QHBoxLayout m_titleLayout;
    QHBoxLayout m_contentLayout;

    QFrame m_alarmFrame;//背景
    QPushButton m_closeBtn;
    QLabel m_titleLabel;

    QPushButton m_spanUpperBtn;
    QPushButton m_spanLowerBtn;

    QPushButton m_cancelBtn;
    QLabel m_textLabel;

    DigitalOperation m_digitalPad;
    quint8 m_span;
};
#endif // WINCHANNELEDITDIALOG_H
