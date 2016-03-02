/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：MessageListDelegate.cpp
 * 概   要：信息显示接口的代理
 *
 * 当前版本：V1.0.0
 * 作   者：葛 海 浪
 * 完成日期：2016-2-22
 *
 * 修改版本：
 * 修改说明：
 * 修改作者：
 *
 ********************************************************************/
#include "messagelistdelegate.h"
#include <QPainter>
MessageListDelegate::MessageListDelegate(QObject *parent) :
    QAbstractItemDelegate(parent)
{
}

void MessageListDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);
    QRect sequnceRect, messageRect;
    doLayout(option, sequnceRect, messageRect);
    drawBackGround(painter, option, index);
    drawSequnceRect(painter, option, index, sequnceRect);
    drawMessageRect(painter, option, index, messageRect);
    painter->restore();
}

QSize MessageListDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &/*index*/) const
{
    return option.rect.size();
}

bool MessageListDelegate::helpEvent(QHelpEvent */*event*/, QAbstractItemView */*view*/, const QStyleOptionViewItem &/*option*/, const QModelIndex &/*index*/)
{
    return false;
}

void MessageListDelegate::doLayout(const QStyleOptionViewItem &option, QRect &sequnceRect, QRect &messageRect) const
{
    /** draw Message Sequnce Rect **/
    sequnceRect = option.rect;
    sequnceRect.setWidth(option.rect.width() / 9);
    sequnceRect.moveLeft(option.rect.left());

    /** draw Message Information Rect **/
    messageRect = option.rect;
    messageRect.moveLeft(option.rect.left());
}

void MessageListDelegate::drawBackGround(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    painter->save();
    /** draw background **/
    QRect rect = option.rect;
    painter->setPen(QColor(202, 201, 200));
    if(index.data(MousePressRole).toBool()) {       // 鼠标下押状态
        painter->setBrush(QBrush(QColor(179, 231, 255)));
    } else {
        if((index.data(MouseClickedRole).toBool())) {   // 鼠标单击状态
            painter->setBrush(QBrush(QColor(220, 244, 255)));
        } else {        // 默认状态
            painter->setBrush(QBrush(QColor(248, 248, 248)));
        }
    }
    painter->drawRoundedRect(rect, 0, 0);
    /**  draw over **/
    painter->restore();
}

void MessageListDelegate::drawSequnceRect(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index, const QRect &rect) const
{
    painter->save();
    int pixSize = rect.height() * 0.25;
    QFont font = option.font;
    QTextOption txtAlign(Qt::AlignHCenter | Qt::AlignTop);
    font.setPixelSize(pixSize);
    painter->setFont(font);
    painter->drawText(rect, index.data(SequnceRole).toString(), txtAlign);
    painter->restore();
}

void MessageListDelegate::drawMessageRect(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index, const QRect &rect) const
{
    painter->save();
    int pixSize = rect.height() * 0.35;
    QFont font = option.font;
    QTextOption txtAlign(Qt::AlignCenter);
    font.setPixelSize(pixSize);
    painter->setFont(font);
    painter->drawText(rect, index.data(MessageRole).toString(), txtAlign);
    painter->restore();
}
