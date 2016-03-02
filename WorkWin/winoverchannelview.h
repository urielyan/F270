#ifndef WINOVERCHANNELVIEW_H
#define WINOVERCHANNELVIEW_H

#include <QWidget>
#include <QTableView>
#include <QLabel>
#include <QFrame>
#include <QTimer>

#include "./Common/slidingscreen.h"

#define WIN_ALL_CHANNEL_FRAME_INDEX 1
#define WIN_ALL_GROUP_FRAME_INDEX 0
#define WIN_DIV_GROUP_FRAME_INDEX 2

class WinOverviewAllGroupPanel;
class WinOverviewAllChanPanel;
class WinOverviewNavigationBar ;
class WinOverviewDivGroupFrame;
class WinOverviewAllChannelFrame;
class WinOverviewTable;
class WinOverView;

/**
 * 总览图表格
 */
class WinOverviewTable : public QTableView
{
    Q_OBJECT
public:
    explicit WinOverviewTable(QWidget *parent = 0);

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
    QRect m_rect;
};

/**
 * 分组显示导航栏
 */
class WinOverviewNavigationBar : public QFrame
{
    Q_OBJECT
public:
    explicit WinOverviewNavigationBar(QWidget *parent = 0);

signals:
    void sigNextFrame();//下一帧画面
    void sigLastFrame();//上一帧画面
    void sigGoBack();//返回总览组画面
    void sigGoTrend();//转到趋势图

public slots:
    void slotGroupNameChanged(quint32 grpIndex);//组名变更

private:
    QLabel *p_groupNameLabel;
};

/**
 * 分组显示画面
 */
class WinOverviewDivGroupFrame : public QFrame
{
    Q_OBJECT
public:
    explicit WinOverviewDivGroupFrame(quint32 grpIndex=0, QWidget *parent = 0);
    ~WinOverviewDivGroupFrame();
    void setCurrentGroupFrame(quint32 grpIndex);
    void updateGroupConfig();

signals:
    void sigGoBack();
    void sigChangeFrame(quint32 frameId);//切换显示画面

public slots:
    void slotGoTrendButtonClicked();//跳转至趋势图
    void slotChannelClicked(QModelIndex);//通道被点击

private:
//    QMap<quint32,WinOverviewTable *> p_map_Frame;
    WinOverviewNavigationBar *p_navBar;
    SlidingScreen *p_chanPanel;
};

/**
 * 单个通道画面
 */
class WinOverviewAllChannelFrame : public WinOverviewTable
{
    Q_OBJECT
public:
    explicit  WinOverviewAllChannelFrame(quint32 frameIndex = 0, QWidget *parent = 0);

signals:
    void sigChangeFrame(quint32 frameId);//切换显示画面

private slots:
        void slotChannelClicked(QModelIndex);//通道被点击
};

/**
 * 全部通道画面
 */
class WinOverviewAllChanPanel : public SlidingScreen
{
    Q_OBJECT
public:
    explicit WinOverviewAllChanPanel(QGraphicsView *parent = 0);
//    ~WinOverviewAllChanPanel();
    void updateChannelConfig();

signals:
    void sigChangeFrame(quint32 frameId);//切换显示画面

private:
//    QMap<quint32,WinOverviewAllChannelFrame *> p_map_Frame;
};

/**
 * 全部分组画面
 */
class WinOverviewAllGroupPanel : public WinOverviewTable
{
    Q_OBJECT
public:
    explicit  WinOverviewAllGroupPanel(QWidget *parent = 0);

};

#endif // WINOVERCHANNELVIEW_H
