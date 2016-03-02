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
#ifndef CHANNELDELEDATA_H
#define CHANNELDELEDATA_H

#include <QAbstractItemDelegate>

class ChannelDeledata : public QAbstractItemDelegate
{
    Q_OBJECT
public:
    explicit ChannelDeledata(QObject *parent = 0);
    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    virtual QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
    bool helpEvent(QHelpEvent *event, QAbstractItemView *view, const QStyleOptionViewItem &option, const QModelIndex &index);
    void setChannelNums(int nums);          /** 设定要绘制的通道的数目 **/

signals:

public slots:

private:
    void doLayout(const QStyleOptionViewItem &option, QRect &codeRect, QRect &numberRect) const;
    void drawBackground(QPainter* painter, const QStyleOptionViewItem& option,  const QModelIndex &index) const;
    void drawChannelNames(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex &index, const QRect & rect) const;
    void drawChannelNums(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex &index, const QRect & rect) const;
private:
    enum {
        ChannelNameRole = 0,
        ChannelPressRole,
        ChannelClickedRole,
        ChannelSequnceRole
    };
    int m_channelNums;           // 行 ，列

};

#endif // CHANNELDELEDATA_H
