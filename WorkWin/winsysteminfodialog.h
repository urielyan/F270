#ifndef WINSYSTEMINFODIALOG_H
#define WINSYSTEMINFODIALOG_H

#include <QFrame>
#include "windialog.h"
//系统信息模块对话框
class WinSystemInfoDialog : public WinDialog
{
        Q_OBJECT
public:
    WinSystemInfoDialog();
    ~WinSystemInfoDialog();

private:
    void initDialog();//初始化界面

private:
    QFrame *p_mainFrame;//背景
};

#endif // WINSYSTEMINFODIALOG_H
