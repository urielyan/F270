/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：EventCalendarDelegate.h
 * 概   要：日历事件接口中日历界面显示的代理
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
#ifndef EVENTCALENDARDELEGATE_H
#define EVENTCALENDARDELEGATE_H

#include <QAbstractItemDelegate>

class EventCalendarDelegate : public QAbstractItemDelegate
{
public:
    EventCalendarDelegate(QObject *parent = 0);
    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    virtual QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
    bool helpEvent(QHelpEvent *event, QAbstractItemView *view, const QStyleOptionViewItem &option, const QModelIndex &index);

private:
    enum {
        CalendarDataRole = 0,
        CanledarActive,
        CalendarPressRole,
        CalendarClickedRole,
        CalendarDispRole,
        CalendarEventRole
    };
    void doLayout(const QStyleOptionViewItem &option, QRect &dataRect, QRect &eventRect) const;
    void drawBackground(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex &index) const;
    void drawDataName(QPainter *painter, const QStyleOptionViewItem& option, const QModelIndex &index, const QRect& rect) const;
    void drawEventName(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex &index, const QRect& rect) const;

};

#endif // EVENTCALENDARDELEGATE_H
