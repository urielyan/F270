#ifndef WINCHANNELALARMDIALOG_H
#define WINCHANNELALARMDIALOG_H

#include <QFrame>
#include <QButtonGroup>
#include "windialog.h"

class WinChannelInfoDialog;
/*
 * 接触单个通道报警对话框
 *
 */
class WinChannelAlarmDialog : public WinDialog
{
    Q_OBJECT
public:
    explicit WinChannelAlarmDialog(WinChannelInfoDialog *parent);
    void resetChannelNumber();

public slots:
    void slotReflush();//定时刷新，用于绘制保持模式下报警闪烁效果

private slots:
    void slotOkButtonClicked();
    void slotCancelButtonClicked();

private:
    void initAlarmDialog();//初始化界面


private:
    WinChannelInfoDialog *p_parent;//父对话框

    WinStackedLayout m_layout;
    QVBoxLayout m_alarmFrameLayout;
    QHBoxLayout m_titleLayout;
    QGridLayout m_contentLayout;
    QHBoxLayout m_buttonLayout;

    QFrame m_alarmFrame;//背景
    QPushButton m_closeBtn;
    QLabel m_titleLabel;

    QLabel m_textLabel;
    QPushButton m_alarm1Btn;//报警电平1按钮
    QPushButton m_alarm2Btn;//报警电平2按钮
    QPushButton m_alarm3Btn;//报警电平3按钮
    QPushButton m_alarm4Btn;//报警电平4按钮
    QButtonGroup m_btnGrp;//报警电平按钮组

    QPushButton m_okBtn;
    QPushButton m_cancelBtn;
};

#endif // WINCHANNELALARMDIALOG_H
