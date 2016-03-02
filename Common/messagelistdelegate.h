/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：MessageListDelegate.h
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
#ifndef MESSAGELISTDELEGATE_H
#define MESSAGELISTDELEGATE_H

#include <QAbstractItemDelegate>

class MessageListDelegate : public QAbstractItemDelegate
{
    Q_OBJECT
public:
    explicit MessageListDelegate(QObject *parent = 0);
    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    virtual QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
    bool helpEvent(QHelpEvent *event, QAbstractItemView *view, const QStyleOptionViewItem &option, const QModelIndex &index);

private:
    /**
     *SequnceRole : 显示信息ID
     * MessageRole: 显示信息的内容
     * MousePressRole: 显示信息鼠标下押
     * MouseClickedRole:显示信息鼠标单击
     */
    enum {
        SequnceRole = 0,
        MessageRole,
        MousePressRole,
        MouseClickedRole
    };
    /** 视图布局  **/
    void doLayout(const QStyleOptionViewItem &option, QRect &sequnceRect, QRect &messageRect) const;
    /** 视图背景颜色  **/
    void drawBackGround(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex &index) const;
    /** 绘制信息ID号  **/
    void drawSequnceRect(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex &index, const QRect& rect) const;
    /** 绘制信息内容  **/
    void drawMessageRect(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex &index, const QRect& rect) const;

};

#endif // MESSAGELISTDELEGATE_H
