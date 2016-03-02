#ifndef WINRELAYANDSW_H
#define WINRELAYANDSW_H

#include <QWidget>
#include <QTableView>
#include <QFrame>
#include <QLabel>
#include <QStackedLayout>
#include <QStackedWidget>
#include <QPushButton>

#include "./Common/slidingscreen.h"
#include "./DataModel/modelrelaymodel.h"
#include "./DataModel/modelsoftswitchmodel.h"
#include "winrelaydelegate.h"
#include "winsoftswitchdelegate.h"

#define WIN_RELAY_FRAME 0
#define WIN_SW_FRAME 1
#define WIN_SIGNAL_FRAME 0
#define WIN_MULTI_FRAME 1

class WinRSTable;
class WinRSNavigationBar;
class WinRSSingleFrame;
class WinRSMultiFrame;
class WinRelayAndSW;

/**
 * DO通道和软件开关表格
 */
class WinRSTable : public QTableView
{
    Q_OBJECT
public:
    explicit WinRSTable(QWidget *parent = 0);

signals:
    void sigClicked(QModelIndex);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    QStyleOptionViewItem viewOptions() const;

private:
    bool m_pressed;
    QPoint m_point;//鼠标点击坐标
    QRect m_rect;//按压选中区域
};

/**
 * DO通道和软件开关导航栏
 */
class WinRSNavigationBar : public   QFrame
{
    Q_OBJECT
public:
    WinRSNavigationBar(QWidget *parent = 0);
    static quint32 getFormIndex(){return m_formIndex;}

signals:
    void sigShrinkButtonClick();//全部显示按钮点击
    void sigPreviousButtonClick();//前一帧按钮被点击
    void sigNextButtonClick();//后一帧按钮被点击

    void slotSwitchFrame(quint32 frameIndex);//切换当前帧

public slots:
    void slotFormNameChanged(quint32 formIndex);

private:
    static quint32 m_formIndex;//面板枚举（WIN_RELAY_FRAME/继电器，WIN_SW_FRAME/内部开关）
    QHBoxLayout m_horizontalLayout;
    QLabel m_formNameLabel;//面板名称
    QPushButton m_shrinkBtn;
    QPushButton m_prevBtn;
    QPushButton m_nextBtn;
};
/**
 * DO通道和软件开关单屏（二分屏）画面
 */
class WinRSSingleFrame : public QFrame
{
    Q_OBJECT
public:
    WinRSSingleFrame(QWidget *parent = 0);
    bool eventFilter(QObject *, QEvent *);

protected:
    void paintEvent(QPaintEvent *);

signals:
    void sigFrameClick(quint32 frameIndex);//继电器或内部开关面板按压时触发（继电器面板隐藏时无效）

private slots:
    void slotSwitchItemClicked(QModelIndex);//内部开关被点击

private:
    bool getDoStatus();//获取do模块是否有效

private:
    bool m_pressed;//按压标记
    QVBoxLayout m_layout;
    QFrame m_maskView;//按压状态下的半透明窗
    WinRSTable m_relayTable;//继电器面板
    WinRelayDelegate m_relayDelegate;
    ModelRelayModel m_relayModel;
    WinRSTable m_swTable;//内部开关面板
    WinSoftSwitchDelegate m_swDelegate;
    ModelSoftSwitchModel m_swModel;
    QLabel m_relayTitle;
    QLabel m_swTitle;
};

/**
 * DO通道和软件开关多屏（滑屏）画面
 */
class WinRSMultiFrame : public QFrame
{
    Q_OBJECT
public:
    WinRSMultiFrame(QWidget *parent = 0,quint32 frameIndex = WIN_SW_FRAME);
    void setCurrentFrame(quint32 frameIndex);

signals:
    void sigShrinkButtonClick();//全部显示按钮点击
    void slotSwitchFrame(quint32 frameIndex);//切换当前帧

private slots:
    void slotRelayItemClicked(QModelIndex);//继电器被点击
    void slotSwitchItemClicked(QModelIndex);//内部开关被点击

private:
    QVBoxLayout m_layout;
    SlidingScreen m_slinder;//滑屏模块
    WinRSNavigationBar m_navBar;//导航条
};

/**
 * 总画面
 */
class WinRelayAndSW : public QStackedWidget
{
    Q_OBJECT
public:
    explicit WinRelayAndSW(QWidget *parent = 0);
    void switchFrame(int fmIndex);
    static int getCurrentIndex(){return m_index;}

private slots:
    void slotZoomInFrame(quint32 frameIndex);//放大显示滑屏画面
    void slotZoomOutFrame();//缩小显示二分画面

private:
    static int m_index;

    WinRSSingleFrame m_sFrame;//单屏显示模式
    WinRSMultiFrame m_mFrame ;//多屏显示模式实例
};

#endif // WINRELAYANDSW_H
