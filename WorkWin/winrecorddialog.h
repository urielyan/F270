#ifndef WINRECORDDIALOG_H
#define WINRECORDDIALOG_H
#include <QFrame>
#include "windialog.h"

//有批处理的记录对话框
class WinRecordDialog : public WinDialog
{
    Q_OBJECT
public:
    WinRecordDialog();
    ~WinRecordDialog();

private:
    void initDialog();//初始化界面

private slots:
    void slotRecordKeyClicked(bool status); // true为记录开始状态， false为记录解除
    void slotFirstButtonClicked();
    void slotSecondButtonClicked();
    void slotThirdButtonClicked();

private:
    QFrame *p_mainFrame;//背景
    QPushButton *p_textBtn;

    QLabel *p_batchNumber;
    QLabel *p_lotNumber;
    QLabel *p_comment1;
    QLabel *p_comment2;
    QLabel *p_comment3;

    QPushButton *p_exitBtn;
    QPushButton *p_recBtn;
    QPushButton *p_resetBtn;
    QPushButton *p_recordBtn;
};

//无批处理的记录对话框
class WinRecordOtherDialog : public WinDialog
{
    Q_OBJECT
public:

    static WinRecordOtherDialog *instance();

    ~WinRecordOtherDialog();

    void setRecordStatus(bool recordStatus/*, int type*/);

private:

     WinRecordOtherDialog();

    void initDialog();//界面初始化

private slots:

    void slotRecordKeyClicked(bool status); // true为记录开始状态， false为记录解除

    void slotFirstButtonClicked();
    void slotSecondButtonClicked();
    void slotThirdButtonClicked();

private:
    QFrame *p_mainFrame;//背景
    QLabel *titleLabel;
    QLabel *txtLabel;
    QPushButton *firstBtn;
    QPushButton *secondBtn;
    QPushButton *thirdBtn;

    bool m_recordStatus;

};

#endif // WINRECORDDIALOG_H
