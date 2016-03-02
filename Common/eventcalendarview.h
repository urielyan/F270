/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：EventCalendarView.h
 * 概   要：日历事件接口中日历界面显示的视图
 *
 * 当前版本：V1.0.0
 * 作   者：葛 海 浪
 * 完成日期：2016-1-21
 *
 * 修改版本：
 * 修改说明：
 * 修改作者：
 *
 ********************************************************************/
#ifndef EVENTCALENDARVIEW_H
#define EVENTCALENDARVIEW_H

#include <QTableView>
#include <QModelIndex>
class EventCalendarView : public QTableView
{
    Q_OBJECT
public:
    EventCalendarView(QWidget *parent = 0);
    void setSelectStatus(QModelIndex index);

signals:
    void sigReleasedIndex(QModelIndex index);
protected:
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);

private:
    enum {
        CalendarDataRole = 0,
        CanledarActive,
        CalendarPressRole,
        CalendarClickedRole,
        CalendarShowRole,
        CalendarEventRole
    };
    QPoint m_point;
    QModelIndex m_currentIndex;
    QModelIndex m_moveIndex;
};

#endif // EVENTCALENDARVIEW_H
