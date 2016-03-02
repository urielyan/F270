#ifndef WINMATHDIALOG_H
#define WINMATHDIALOG_H
#include <QFrame>
#include "windialog.h"

//运算操作对话框
class WinMathDialog : public WinDialog
{
    Q_OBJECT
public:
    WinMathDialog();
    ~WinMathDialog();

private:
    void initDialog();//初始化界面

private:
    QFrame *p_mainFrame;//背景
};

#endif // WINMATHDIALOG_H
