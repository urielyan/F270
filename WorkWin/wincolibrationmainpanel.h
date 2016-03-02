#ifndef WINCOLIBRATIONMAINPANEL_H
#define WINCOLIBRATIONMAINPANEL_H

#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QVariant>
#include <QVBoxLayout>
#include <QDialog>
#include <QTimer>
#include  "wincolibrationbutton.h"
#include "Common/sysconfigslidingscreen.h"

class WinNormalButton;
class WinColibrationMainPanel;
class WinColibrationRightPanel;

/*
 * 触摸屏校验对话框
 */
class WinColibration : public QDialog
{
    Q_OBJECT
public:
    explicit WinColibration(QWidget *parent = 0, int step = 0);

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);

private:
    void capturePoint();
    bool getActualPos();
    bool checkActualPos();
    bool savePos(QPoint min, QPoint max);
    bool setColibrationPos();
    QString getString();

private slots:
    void slotTimeout();

private:
    bool m_enCalib;//开启校正模式
    bool m_pressed;//按压屏幕标记
    bool m_busy;//忙等标记
    bool m_isValid;//非法标记
    int m_step;//当前校正进度
    //QString m_tip;//提示字符
    QPoint m_pos;//当前按压坐标
    QPoint m_origin[5];//引导坐标点
    QPoint m_actual[4];//校正坐标点
};

class WinColibrationMainPanel : public SysConfigSlidingScreen
{
    Q_OBJECT
public:
    explicit WinColibrationMainPanel(QGraphicsView *parent = 0);
    ~WinColibrationMainPanel();

public slots:
    void slotExcute();
    void slotSwitchFrame(int btnId);

private:
    WinColibrationRightPanel *p_panel[6];
    int m_currentId;
};

class WinColibrationRightPanel : public QFrame
{
    Q_OBJECT
public:
    explicit WinColibrationRightPanel(QWidget *parent = 0);

    void addWidget(QWidget *widget);
    virtual void execute() = 0;

private:
    QVBoxLayout m_layout;
};

/*
 * On/Off窗体
 */
class WinOnOffPanel : public WinColibrationRightPanel
{
    Q_OBJECT
public:
    static WinOnOffPanel*Instance();
    void setSlidingScreen(SysConfigSlidingScreen *slider){p_slider = slider;}
    void setColibrationButton(WinColibrationButton *btn){p_btn = btn;}

private:
    explicit WinOnOffPanel();
    void execute(){}

private slots:
    void slotOnButtonClicked();
    void slotOffButtonClicked();

private:
    SysConfigSlidingScreen *p_slider;
    WinColibrationButton *p_btn;
    WinColibrationButton *p_onBtn;
    WinColibrationButton *p_offBtn;
};

class WinColibrationInitPanel : public WinColibrationRightPanel
{
    Q_OBJECT
public:
    explicit WinColibrationInitPanel(SysConfigSlidingScreen *slider,QWidget *parent = 0);

    enum GroupButtonID{
        Initialize_All_Button = 0,
        Secrity_Setting_Button,
        Other_Setting_Button,
        Internal_Data_Button
    };

    void execute();

private slots:
    void slotButtonClicked(int);
    void slotDataChanged(QVariant data);

private:
    SysConfigSlidingScreen *p_slider;
    QButtonGroup *p_btnGrp;
    WinColibrationButton *p_btns[4];
};

class WinColibrationReconfigPanel : public WinColibrationRightPanel
{
    Q_OBJECT
public:
    explicit WinColibrationReconfigPanel(SysConfigSlidingScreen *slider,QWidget *parent = 0);
    void execute();

private:
    SysConfigSlidingScreen *p_slider;
};

class WinColibrationTSCPanel : public WinColibrationRightPanel
{
    Q_OBJECT
public:
    explicit WinColibrationTSCPanel(SysConfigSlidingScreen *slider,QWidget *parent = 0);
    void execute(){}

private slots:
    void slotColibrationClicked();

private:
    SysConfigSlidingScreen *p_slider;
};

class WinColibrationADCPanel : public WinColibrationRightPanel
{
    Q_OBJECT
public:
    explicit WinColibrationADCPanel(SysConfigSlidingScreen *slider,QWidget *parent = 0);
    void execute(){}

private slots:
    void slotECButtonClicked();
    void slotECValueChanged(QVariant);
    void slotPasswdButtonClicked();

private:
    SysConfigSlidingScreen *p_slider;
    WinColibrationButton *p_ecBtn;
    WinColibrationButton *p_passwdBtn;
};

class WinColibrationECPanel : public WinColibrationRightPanel
{
    Q_OBJECT
public:
    explicit WinColibrationECPanel(SysConfigSlidingScreen *slider,QWidget *parent = 0);
    void execute(){}

private slots:
    void slotButtonClicked();

private:
    SysConfigSlidingScreen *p_slider;
    WinColibrationButton *p_eBtn;
};

class WinColibrationUpdataPanel : public WinColibrationRightPanel
{
    Q_OBJECT
public:
    explicit WinColibrationUpdataPanel(SysConfigSlidingScreen *slider,QWidget *parent = 0);
    void execute(){}

private:
    SysConfigSlidingScreen *p_slider;
};
#endif // WINCOLIBRATIONMAINPANEL_H
