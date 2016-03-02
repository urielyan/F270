#ifndef WINSTANDARDDIALOG_H
#define WINSTANDARDDIALOG_H

#include <QScrollArea>
#include "windialog.h"

class SoftKeyPad;
class DigitalOperation;

/***********************设置主画面对话框**********************************/
class WinStandardDialog : public WinDialog
{
    Q_OBJECT
public:
    explicit WinStandardDialog(QWidget *parent = 0);

signals:

public slots:

    void slotCloseClicked();
    void slotOKClicked();

};

/***********************定时器重置对话框**********************************/
class WinTimerRestartDialog : public WinDialog
{
    Q_OBJECT
public:
    explicit WinTimerRestartDialog(QWidget *parent = 0);

signals:

public slots:

    void slotButtonEnable();
     void slotCloseClicked();
     void slotOKClicked();

private:
     QPushButton *button1;
     QPushButton *button2;
     QPushButton *button3;
     QPushButton *button4;
     QPushButton *buttonAll;

};

/***********************设置时间日期对话框**********************************/
class SetDateTime;

class WinSetTimeDialog : public QWidget
{
    Q_OBJECT
public:
    static WinSetTimeDialog *instance();

    void show(/*bool sequeue*/);
private:

    explicit WinSetTimeDialog(QWidget *parent = 0);

signals:

public slots:

     void slotOKClicked();

private:
    SetDateTime *p_time;

};

/***********************批处理对话框**********************************/
class WinBatchDialog : public WinDialog
{
    Q_OBJECT
public:

    static WinBatchDialog* instance();

    void setUserDefineStyle();

    QString getBatchNumberStr() {return p_batchStr;}
    void setBatchNumberStr(QString str){p_batchStr = str;}
    QString getLotNumberStr() {return p_lotNumberStr;}
    void setLotNumberStr(QString str){p_lotNumberStr = str;}

    void setDialogType(int type);  //0.为批处理，1.为记录对话框

    void setRecordStatus(bool recoedStatus/*, int type*/);

    ~WinBatchDialog();
private:

    WinBatchDialog();

    void sigAndSlot();


private slots:

    void slotBatchNumberClicked();
    void slotLotBumberClicked();
    void slotComment1Clicked();
    void slotComment2Clicked();
    void slotComment3Clicked();

    void slotRecordKeyClicked(bool status); // true为记录开始状态， false为记录解除
    void slotFirstButtonClicked();
    void slotSecondButtonClicked();
    void slotThirdButtonClicked();
    void slotMoveToTextAraa();

private:
    void initDialog();//初始化界面



private:
    QFrame *p_mainFrame;//背景
    QLabel *titleLabel;
    QPushButton *p_textBtn;

    QPushButton *p_batchNumber;
    QPushButton *p_lotNumber;
    QPushButton *p_comment1;
    QPushButton *p_comment2;
    QPushButton *p_comment3;

    QLabel*lotNumberPrompt;

    QPushButton *p_exitBtn;
    QPushButton *p_recBtn;
    QPushButton *p_resetBtn;
    QPushButton *p_recordBtn;

    QString p_batchStr;
    QString p_lotNumberStr;
    QString p_comment1Str;
    QString p_comment2Str;
    QString p_comment3Str;

    bool m_recoedStatus;
};

/***********************批处理文本区域对话框对话框**********************************/
#include <QButtonGroup>
class WinTextAreaDialog : public WinDialog
{
    Q_OBJECT
public:
    static WinTextAreaDialog *instance();

    void setDialogType(int type);

private:
    explicit WinTextAreaDialog(QWidget *parent = 0);

    void initText();

signals:

private slots:

    void slotBtnClicked(int id);

    void slotCloseFromRecoed();

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);


private:
    QLabel*lotNumberPrompt;
    QLabel * p_lotNumber ;
    QLabel * p_batchNumber;

    QList<QLabel*> p_textTitleList;
    QList<QPushButton*> p_textCharacterList;
    QList<QLabel*> p_textTitleNumList;
    QList<QLabel*> p_textCharacterNumList;

    QButtonGroup m_btnGroup;

    QScrollArea *batchText;

    int p_beginY;
    int p_moveY;
    int m_dialogType;

};

/***********************Email对话框对话框**********************************/
class WinEMailDialog : public WinDialog
{
    Q_OBJECT
public:
    explicit WinEMailDialog(QWidget *parent = 0);

signals:

public slots:

};

/***********************FTP对话框对话框**********************************/
class WinFTPDialog : public WinDialog
{
    Q_OBJECT
public:
    explicit WinFTPDialog(QWidget *parent = 0);

signals:

public slots:

};

#include "../Common/englishinput.h"
class WinSoftKeypadDialog : public QWidget
{
    Q_OBJECT
public:
    static WinSoftKeypadDialog *instance();

    void show(QPushButton *widget, int ID, QWidget *parent);

    QString getSoftString();

private:
    WinSoftKeypadDialog(QWidget *parent = 0);
    ~WinSoftKeypadDialog();
signals:

    void sigFinishedInptu();

private slots:
    void slotGetKeyPadData();
    void slotCancelKeyPad();
private:
    SoftKeyPad *keyPad;
    EnglishInput *englishPad;
    QPushButton *button;
    int p_ID;
    QString m_getSoftStr;
    int m_usePadType;
};


class WinNumericKeypadDialog : public QWidget
{
    Q_OBJECT
public:
    static WinNumericKeypadDialog *instance();

    void show(QWidget *widget);

    void setID(quint32 ID);

private:
    WinNumericKeypadDialog(QWidget *parent = 0);
    ~WinNumericKeypadDialog();
private slots:
    void slotGetKeyPadData();
private:
    DigitalOperation *keyPad;
    QPushButton *button;
    quint32 m_ID;

};

/***********************ope对话框**********************************/
class WinOpeLoginDialog : public WinDialog
{
    Q_OBJECT
public:
    static WinOpeLoginDialog *instance();
private:

    WinOpeLoginDialog(QWidget *parent = 0);

    ~WinOpeLoginDialog();

signals :
    void sigOpeLoginSuccess();

private slots:

    void slotPasswdClicked();
    void slotJudgePasswd();

private:

    QPushButton *p_batchNumber;

};

/***********************login对话框**********************************/
class WinLoginDialog : public WinDialog
{
    Q_OBJECT
public:

    static WinLoginDialog *instance();
    bool getDialogStatus();
    void setDialogStatus(bool status);

    void initData();

private:

    WinLoginDialog(QWidget *parent = 0);

private slots:

    void slotUserClicked();
    void slotPasswdClicked();
    void slotGetSoftInput();
    void slotTestLogin();


private:
    QPushButton *p_batchNumber;
    QPushButton *p_comment1;

    int m_clieckBtnID;
    QString m_userName,m_passwd;
    bool m_dialogStatus;

};

/***********************change passwd对话框**********************************/
class WinChangePasswdDialog : public WinDialog
{
    Q_OBJECT
public:

    WinChangePasswdDialog(QWidget *parent = 0);

    ~WinChangePasswdDialog();

private slots:

    void slotOldPasswd();
    void slotNewPasswd();
    void slotNewPasswdAgain();

    void slotGetSoftInput();
    void slotTestLogin();

private:

    int m_clieckBtnID;
    QString m_oldPassed,m_newPasswd,m_newPasswdAgain;
};

//****************************透明窗体********************************/

class WinTransparentDialog : public QDialog
{
    Q_OBJECT
public:
    static WinTransparentDialog *instance();

    void setDialogStatus(bool status) {m_dialogStatus = status;}
    bool getDialogStatus() {return m_dialogStatus;}

private:
     WinTransparentDialog(QWidget *parent = WinMainFrame::instance());

     void mousePressEvent(QMouseEvent *event);

signals:
//    void sigChangeFrame(quint32 frameId);//画面切换

public slots:

private :
     bool m_dialogStatus;

};

#endif // WINSTANDARDDIALOG_H
