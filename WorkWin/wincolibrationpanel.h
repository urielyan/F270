#ifndef WINCOLIBRATIONPANEL_H
#define WINCOLIBRATIONPANEL_H
#include <QFrame>
#include <QPushButton>
#include "wincolibrationleftpanel.h"
#include "wincolibrationmainpanel.h"

//初始化模块顶部界面
class WinColibrationTopPanel : public QFrame
{
    Q_OBJECT
public:
    explicit WinColibrationTopPanel(QWidget *parent = 0);
};

//初始化模块底部界面
class WinColibrationBottomPanel : public QFrame
{
    Q_OBJECT
public:
    explicit WinColibrationBottomPanel(WinColibrationMainPanel *slider,QWidget *parent = 0);

signals:
    void sigChangeFrame(quint32 frameId);//切换显示画面

private slots:
    void slotQuitButtonClicked();
    void slotBackButtonClicked();

private:
    QHBoxLayout m_layout;
    QPushButton m_exeBtn;//执行按钮
    QPushButton m_quitBtn;//退出按钮
    QPushButton m_backBtn;//返回按钮
    WinColibrationMainPanel *p_slider;//滑屏模块
};

//初始化/校正模块界面类
class WinColibrationPanel : public QFrame
{
    Q_OBJECT
public:
    explicit WinColibrationPanel(QWidget *parent = 0);

signals:
    void sigChangeFrame(quint32 frameId);//切换显示画面


public slots:
    void slotAuthChanged();

private:
    WinColibrationMainPanel m_mainPanel;//右侧菜单
    WinColibrationTopPanel m_topPanel;//顶部标题界面
    WinColibrationLeftPanel m_leftPanel;//左侧导航菜单
    WinColibrationBottomPanel m_bottomPanel;//底部操作界面

    QHBoxLayout m_hLayout;
    QVBoxLayout m_vLayout;
};

#endif // WINCOLIBRATIONPANEL_H
