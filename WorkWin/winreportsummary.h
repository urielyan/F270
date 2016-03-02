#ifndef WINREPORTSUMMARY_H
#define WINREPORTSUMMARY_H

#include <QScroller>
#include <QScrollBar>
#include <QMouseEvent>
#include <QWidget>
#include <QPushButton>
#include <QGridLayout>
#include <QTableView>
#include <QPushButton>
#include <QLabel>
#include "./Common/slidingscreen.h"

class WinReport;


class ReportSlidingScreen : public SlidingScreen
{
    Q_OBJECT
public:
    explicit ReportSlidingScreen(QGraphicsView *parent = 0, bool isEnabledXDir = true, bool isEnabledYDir = true);

    int updateFrame(bool indexAdd);

    int addWidgetItem(int widgetIndex, QWidget *pw);

signals:
    void sigMoveToPre();
    void sigMoveToNext();

public slots:

protected:
    void mouseReleaseEvent(QMouseEvent *event);

public:
    int             m_sceneCnt;
};


class ReportTableView : public QTableView
{
    Q_OBJECT
public:
    explicit ReportTableView(QWidget *parent = 0);

private:
    void showMsgDlg(QPoint pos);

protected:
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

public slots:
    void slotBackgroundChanged(quint32 ID);

public:
    bool m_isPress;
    QPoint m_pos;
    bool m_drag;

    QPoint m_point;
};

class WinReportSummary : public QWidget
{
    Q_OBJECT
public:
    WinReportSummary(QWidget * parent = 0);
    ~WinReportSummary();

private:
    void initReportSummaryUI();    //init MVC UI
    void signalAndSlotConnect();    //bind signal and slot

public slots:
    /* 上/下按钮按下和松开的信号的槽函数和与之相关的定时器槽函数 */
    void slotUpBtnPressed();
    void slotUpBtnRelease();
    void slotDownBtnPressed();
    void slotDownBtnRelease();
    void slotTimeOut();

public:
    QPushButton *           p_upBtn;
    QPushButton *           p_downBtn;
    QLabel *                p_headerLabel;
    QGridLayout *           p_gridLayout;

    ReportTableView *       p_view;
    int                     m_btnPress;   //上 下按钮哪一个被按下  -1:无按下; 0:上按钮被按下;  1:下按钮被按下

    QTimer *                p_timer;
};




#endif
