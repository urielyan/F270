/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：WinMainFrame.h
 * 概   要：主窗体类，垂直布局，包括WinMainStatus和QStackedWidget，
 *      ----WinMainStatus类对象用于系统状态显示
 *      ----QStackedWidget类对象作为各个窗体的堆栈容器
 *
 *
 * 当前版本：V1.0.0
 * 作   者：王 品
 * 完成日期：2015-6-30
 *
 * 修改版本：
 * 修改说明：
 * 修改作者：
 *
 ********************************************************************/
#ifndef WINMAINFRAME_H
#define WINMAINFRAME_H
#include <QFrame>
#include <QWidget>
#include <QStackedWidget>
#include <QLayout>
#include <QPushButton>
#include <QLabel>
#include <QTimer>
#include <QTime>
#include <QMap>

//class WinMainStatus;
//class WinMainFrame;
class WinTrendGroupPanel;
class WinBarPanel;
class WinDigitalPanel;

//状态栏
class WinMainStatus : public QFrame
{
    Q_OBJECT
public:
    explicit WinMainStatus(QWidget *parent = 0);

signals:
        void sigChangeFrame(quint32 frameID);//切换窗体信号

private:
    QPixmap getTitleIcon(quint32 frameId);//获取左侧图标

public slots:
    void slotTitleChanged(quint32 frameId);
    void slotTitleChanged(quint32 frameId,quint32 grpIndex);//标题更新
    void slotPrintScreenButtonClicked();//截屏按钮被点击
    void slotAlertButtonClicked();//报警管理按钮被点击
    void slotMemButtonClicked();//热插拔管理按钮被点击
    void slotMathButtonClicked();//运算按钮被点击
    void slotRecordButtonClicked();//记录按钮被点击
    void slotFuncButtonClicked();//多功能按钮（对应横河Context菜单）
    void slotAlarmStateUpdate();//报警状态更新
    void slotMathStateUpdate();//运算状态更新
    void slotMemStateUpdate();//移动存储介质状态更新
    void slotTimeUpdate();//时间更新

private slots:
    void slotHomeButtonPressed();//home按钮按下
    void slotHomeButtonReleased();//home按钮抬起
    void slotAuthorityChanged();

private:
    QLabel m_iconLabel;//左侧图标
    QLabel m_titleLabel;//左侧标题

    QPushButton m_prtScBtn;//截屏
    QPushButton m_memBtn;//扩展存储
    QPushButton m_mathBtn;//运算
    QPushButton m_alertBtn;//报警解除
    QPushButton m_recordBtn;//记录
    QPushButton m_homeBtn;//主页
    QPushButton m_funcBtn;//功能键

    QHBoxLayout m_stateLayout;
    QFrame m_statuesFrame;
    QLabel m_status1;//四种状态
    QLabel m_status2;
    QLabel m_status3;
    QLabel m_status4;

    QVBoxLayout m_timeLayout;
    QFrame m_timeFrame;
    QLabel m_dateLabel;//日期
    QLabel m_timeLabel;//时间
    QTime m_time;//home按钮计时

    QHBoxLayout m_headLayout;
};

//工作区
class WinMainFrame : public QWidget
{
    Q_OBJECT
public:
    static WinMainFrame *instance();
    static void setDefaultPanel(quint32 frameId){m_defaultPanel = frameId;}
    static quint32 getDefaultPanel(){return m_defaultPanel;}
    static QString getFrameName(quint32);
    static QString getFrameName(quint32 frameId, quint32 grpIndex);
    static bool getBlinkStatus(){return m_blink;}

    WinTrendGroupPanel *getTrendGroup();
    WinBarPanel *getBarPanel();
    WinDigitalPanel *getDigitalPanel();
    int getStatusBarHeight(){ return m_statusBar.height();}

protected:
    void keyReleaseEvent(QKeyEvent * event);
    bool event(QEvent *);

private:
    explicit WinMainFrame(QWidget *parent = 0);
     void initialize();
     void setWinLayout();
     void clickRecordButton(bool recordStatus);

signals:
    void sigHeartBeat500ms();

public slots:
     void slotFrameChanged(quint32 frameId);//切换当前显示窗体
     void slotFrameChanged();//重载,切换当前窗体到默认画面
     void slotGroupChanged(quint32 grpIndex = 0);//切换当前窗体显示组
     void slotPrintSreen();//截屏
     void slotSnapshot();//快照(带截图反馈效果)
     void slotShowDialog();
     void slotSaveCurrentPanel(){m_defaultPanel = m_currentFrameId;}//设置当前窗体为默认窗体

private slots:
     void slotFrameSlide(quint32 grpIndex);
     void slotTimeout();
     void slotAuthChange();

private:
    WinMainStatus   m_statusBar;
    QStackedWidget  m_workWinStacked;
    QVBoxLayout m_mainLayout;
    QMap<int, QWidget *>m_container;

    QTimer m_timer;                //刷新定时器，500ms刷新一次窗体
    static bool m_blink;            //闪烁效果切换标记
    quint32 m_currentFrameId; //当前工作面板
    static quint32 m_defaultPanel;//home键默认跳转面板ID
};

#endif // WINMAINFRAME_H
