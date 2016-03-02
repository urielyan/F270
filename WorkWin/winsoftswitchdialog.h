#ifndef WINSOFTSWITCHDIALOG_H
#define WINSOFTSWITCHDIALOG_H
#include <QFrame>
#include "windialog.h"

//内部开关对话框
class WinSoftSwitchDialog : public WinDialog
{
    Q_OBJECT
public:
    explicit WinSoftSwitchDialog(quint32 swNum, QWidget *parent = 0);

signals:

public slots:
    void slotOnButtonClicked();//手动开启内部开关
    void slotOffButtonClicked();//手动关闭内部开关

private:
    quint32 m_swNum;//内部开关编号
    QFrame *p_mainFrame;//背景
    QLabel *p_swNumber;//显示编号
    QLabel *p_swName;//内部开关名
    QPushButton *p_onBtn;//手动打开按钮
    QPushButton *p_offBtn;//手动关闭按钮
};

#endif // WINSOFTSWITCHDIALOG_H
