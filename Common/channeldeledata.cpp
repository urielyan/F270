/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：channeldeledata.h
 * 概   要：channelchoice的代理类
 *
 * 当前版本：V1.0.0
 * 作   者：葛 海 浪
 * 完成日期：2016-1-5
 *
 * 修改版本：
 * 修改说明：
 * 修改作者：
 *
 ********************************************************************/
#include "channeldeledata.h"
#include <QStyleOptionViewItem>
#include <QTextOption>
#include <QDebug>
#include <QPainter>
ChannelDeledata::ChannelDeledata(QObject *parent) :
    QAbstractItemDelegate(parent)
  , m_channelNums(0)
{
}
void ChannelDeledata::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if((index.row() * 10 + index.column()) < m_channelNums) {
        painter->save();
        painter->setRenderHint(QPainter::Antialiasing);
        QRect channelName, channelNum;
        doLayout(option, channelName, channelNum);
        drawBackground(painter, option, index);
        drawChannelNames(painter, option, index, channelName);
        drawChannelNums(painter, option, index, channelNum);
        painter->restore();
    }
}

QSize ChannelDeledata::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &/*index*/) const
{
    return option.rect.size();
}

bool ChannelDeledata::helpEvent(QHelpEvent */*event*/, QAbstractItemView */*view*/, const QStyleOptionViewItem &/*option*/, const QModelIndex &/*index*/)
{
    return false;
}

void ChannelDeledata::setChannelNums(int nums)
{
    m_channelNums = nums;
}

void ChannelDeledata::doLayout(const QStyleOptionViewItem &option, QRect &codeRect, QRect &numberRect) const
{
    codeRect = option.rect;
    codeRect.setHeight(option.rect.height() / 2);
    codeRect.moveTop(option.rect.top() + option.rect.height() / 4);
    numberRect = codeRect;
    numberRect.setHeight(codeRect.height() / 2);
    numberRect.moveTop(codeRect.bottom());
}

void ChannelDeledata::drawBackground(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    painter->save();

    /***************绘制背景色***************/
    QRect rect(option.rect);
    rect.adjust(3, 3, -3, -3);
    painter->setPen(QColor(214, 215, 216));
    if(index.data(ChannelPressRole).toBool()) {
        painter->setBrush(QBrush(QColor(150, 255, 255)));
    } else{
        if(index.data(ChannelClickedRole).toBool()) {
            painter->setBrush(QBrush(QColor(150, 255, 255, 100)));
        } else {
            painter->setBrush(QBrush(QColor(230, 230, 232)));
        }
    }
    painter->drawRoundedRect(rect, 0, 0);
    painter->restore();
}

void ChannelDeledata::drawChannelNames(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index, const QRect &rect) const
{
    QVariant value = index.data(ChannelNameRole);
    int pixSize = rect.height() * 0.6;
    if(value.isValid()) {
        painter->save();
        QFont font = option.font;
        QTextOption txtAliagn(Qt::AlignCenter);
        font.setPixelSize(pixSize);
        painter->setFont(font);
        painter->drawText(rect, value.toString(), txtAliagn);
        painter->restore();
    }
}

void ChannelDeledata::drawChannelNums(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index, const QRect &rect) const
{
    QVariant value = index.data(ChannelClickedRole);
    QVariant valueID = index.data(ChannelSequnceRole);
    int pixSize = rect.height() * 0.8;
    if(value.isValid() && value.toBool() && valueID.toInt()) {
        painter->save();
        QFont font = option.font;
        QTextOption txtAliagn(Qt::AlignCenter);
        font.setPixelSize(pixSize);
        painter->setFont(font);
        painter->drawText(rect, valueID.toString(), txtAliagn);
        painter->restore();
    }
}
