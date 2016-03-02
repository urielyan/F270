#ifndef WINMEDIADIALOG_H
#define WINMEDIADIALOG_H

#include <QFrame>
#include "windialog.h"

//可移动存储介质管理对话框
class WinMediaDialog : public WinDialog
{
    Q_OBJECT
public:
    explicit WinMediaDialog();
    ~WinMediaDialog();

private:
    void initDialog();//界面初始化

private:
    QFrame *p_mainFrame;//背景
};

#endif // WINMEDIADIALOG_H
