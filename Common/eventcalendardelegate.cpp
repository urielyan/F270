/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：EventCalendarDelegate.cpp
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
#include "eventcalendardelegate.h"
#include <QPainter>
EventCalendarDelegate::EventCalendarDelegate(QObject *parent) :
    QAbstractItemDelegate(parent)
{
}

void EventCalendarDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);
    QRect dataName, eventName;
    doLayout(option, dataName, eventName);
    drawBackground(painter, option, index);
    drawDataName(painter, option, index, dataName);
    drawEventName(painter, option, index, eventName);
    painter->restore();
}

QSize EventCalendarDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &/*index*/) const
{
    return option.rect.size();
}

bool EventCalendarDelegate::helpEvent(QHelpEvent */*event*/, QAbstractItemView */*view*/, const QStyleOptionViewItem &/*option*/, const QModelIndex &/*index*/)
{
    return false;
}

void EventCalendarDelegate::doLayout(const QStyleOptionViewItem &option, QRect &dataRect, QRect &eventRect) const
{
    /** 绘制日期显示区域  **/
    dataRect = option.rect;
    dataRect.setHeight(option.rect.height() / 2);
    dataRect.moveTop(option.rect.top());
    /** 绘制事件显示区域  **/
    eventRect = dataRect;
    eventRect.setHeight(option.rect.height() / 2);
    eventRect.moveTop(dataRect.bottom());
}

void EventCalendarDelegate::drawBackground(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    painter->save();
    /** 绘制背景色  **/
    QRect rect(option.rect);
    painter->setPen(QColor(200, 200, 200));
    if(index.data(CanledarActive).toInt() ==0) {
        if(index.data(CalendarPressRole).toBool()) {
            painter->setBrush(QBrush(QColor(179, 231, 255)));
        } else if(index.data(CalendarClickedRole).toBool()) {
            painter->setBrush(QBrush(QColor(220, 244, 255)));
        } else {
            painter->setBrush(QBrush(QColor(250, 250, 250)));
        }
    } else {
        if(index.data(CalendarPressRole).toBool()) {
            painter->setBrush(QBrush(QColor(179, 231, 255)));
        } else if(index.data(CalendarClickedRole).toBool()) {
            painter->setBrush(QBrush(QColor(220, 244, 255)));
        } else {
            painter->setBrush(QBrush(QColor(230, 230, 230)));
        }
    }
    painter->drawRoundedRect(rect, 0, 0);
    painter->restore();
}

void EventCalendarDelegate::drawDataName(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index, const QRect &rect) const
{
    painter->save();
    int pixSize = rect.height() * 0.6;
    QFont font = option.font;
    QTextOption txtAlign(Qt::AlignBottom | Qt::AlignHCenter);
    font.setPixelSize(pixSize);
    painter->setFont(font);
    painter->drawText(rect, QString("%1").arg(index.data(CalendarDataRole).toInt()), txtAlign);
    painter->restore();
}

void EventCalendarDelegate::drawEventName(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index, const QRect &rect) const
{
    painter->save();
    int pixSize = rect.height() * 0.4;
    QFont font = option.font;
    QTextOption txtAlign(Qt::AlignCenter);
    font.setPixelSize(pixSize);
    painter->setFont(font);
    if(index.data(CalendarDispRole).toBool() && index.data(CalendarEventRole).toBool()) {
        painter->drawText(rect, QString("Disp\nEvent"), txtAlign);
    } else if(index.data(CalendarDispRole).toBool()) {
        painter->drawText(rect, QString("Disp"), txtAlign);
    } else if(index.data(CalendarEventRole).toBool()) {
        painter->drawText(rect, QString("Event"), txtAlign);
    } else {
        painter->drawText(rect, QString(""), txtAlign);
    }
    painter->restore();
}
