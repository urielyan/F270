/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：EventListDelegate.cpp
 * 概   要：日历事件接口中事件一览显示的代理
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
#include "eventlistdelegate.h"
#include <QPainter>
EventListDelegate::EventListDelegate(QObject *parent) :
    QAbstractItemDelegate(parent)
{
}

void EventListDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);
    QRect beginTime, endTime;
    doLayout(option, beginTime, endTime);
    drawBackground(painter, option, index);
    drawBeginTime(painter, option, index, beginTime);
    drawEndTime(painter, option, index, endTime);
    painter->restore();
}

QSize EventListDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &/*index*/) const
{
    return option.rect.size();
}

bool EventListDelegate::helpEvent(QHelpEvent */*event*/, QAbstractItemView */*view*/, const QStyleOptionViewItem &/*option*/, const QModelIndex &/*index*/)
{
    return false;
}

void EventListDelegate::doLayout(const QStyleOptionViewItem &option, QRect &dataRect, QRect &eventRect) const
{
    /** draw begin time area **/
    dataRect = option.rect;
    dataRect.setWidth(option.rect.width() / 2);
    dataRect.moveLeft(option.rect.left());

    /** draw end time area **/
    eventRect = dataRect;
    eventRect.moveLeft(dataRect.right());

}

void EventListDelegate::drawBackground(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    painter->save();
    /** draw background color   **/
    QRect rect = option.rect;
    painter->setPen(QColor(202, 201, 200));
    if(index.data(MousePressRole).toBool()) {
        painter->setBrush(QBrush(QColor(179, 231, 255)));
    } else {
        if(index.data(MouseClickedRole).toBool()) {
            painter->setBrush(QBrush(QColor(220, 244, 255)));
        } else {
            painter->setBrush(QBrush(QColor(248, 248, 248)));
        }
    }
    painter->drawRoundedRect(rect, 0, 0);
    painter->restore();
}

void EventListDelegate::drawBeginTime(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index, const QRect &rect) const
{
    painter->save();
    int pixSize = rect.height() * 0.3;
    QFont font = option.font;
    QTextOption txtAliagn(Qt::AlignCenter);
    font.setPixelSize(pixSize);
    painter->setFont(font);

    painter->drawText(rect, index.data(BeginTimeStrRole).toString(), txtAliagn);
    painter->restore();
}

void EventListDelegate::drawEndTime(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index, const QRect &rect) const
{
    painter->save();
    int pixSize = rect.height() * 0.3;
    QFont font = option.font;
    QTextOption txtAliagn(Qt::AlignCenter);
    font.setPixelSize(pixSize);
    painter->setFont(font);

    painter->drawText(rect, index.data(EndTimeStrRole).toString(), txtAliagn);
    painter->restore();
}
