/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：trenddigitaldelegate.h
 * 概   要：趋势数值浏览窗体的代理，绘制趋势数值浏览窗体的数值信息，工作于实时刷新
 * 和浏览历史数据两种模式下。
 *
 * 当前版本：V1.0.0
 * 作   者：王 宸 博
 * 完成日期：2015-6-15
 *
 * 修改版本：
 * 修改说明：
 * 修改作者：
 *
 ********************************************************************/
#ifndef TRENDDIGITALDELEGATE_H
#define TRENDDIGITALDELEGATE_H

#include <QAbstractItemDelegate>
#include "./GlobalData/cfgGlobalDef_Type.h"
#include "./GlobalData/cfgtrendgroup.h"
#include <QFont>
#include <QSizeF>

class TrendDigitalDelegate : public QAbstractItemDelegate
{
    Q_OBJECT
public:
    explicit TrendDigitalDelegate(QObject *parent = 0);

    virtual void	paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const;

    virtual QSize	sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index) const;

    bool helpEvent(QHelpEvent *event,
                               QAbstractItemView *view,
                               const QStyleOptionViewItem &option,
                               const QModelIndex &index);

    //设置通道项边界保留区
    void setMargins(QMargins &margins){m_margins = margins;}
    void setMargins(int left, int top, int right, int bottom){m_margins = QMargins(left,top,right,bottom);}

private:
    QStyleOptionViewItem setOptions(const QStyleOptionViewItem &option,const QModelIndex &index) const;
    void doLayout(const QStyleOptionViewItem & option, QRectF &valRect, QRectF &unitRect, QRectF &tagRect, QRectF &alarmRect, qreal &alarmSpan) const;
    void drawBackground(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void drawPressingRect(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void drawMesureValueRect(QPainter * painter, const QStyleOptionViewItem &option, const QModelIndex &index, const QRectF &rect,QString measureVal) const;
    void drawMesureValueRect(QPainter * painter, const QStyleOptionViewItem &option, const QModelIndex &index, const QRectF &rect) const;
    void drawChannelTagRect(QPainter * painter, const QStyleOptionViewItem &option, const QModelIndex &index,  const QRectF &rect) const;
    void drawChannelUnitRect(QPainter * painter, const QStyleOptionViewItem &option, const QModelIndex &index, const QRectF &rect)const;
    void drawAlarmRect(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index,  QRectF &alarmRect, qreal alarmSpan) const;
    void drawAlarmElement(QPainter *painter, const QStyleOptionViewItem &option, QRectF &alarmRect,  uint alarmStatus, uint alarmType) const;
    void drawHistoryAlarmRect(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index,  QRectF &alarmRect, qreal alarmSpan) const;
    void drawSelectionRect(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void calcFontSize(const QStyleOptionViewItem &option, QRectF &rect) const;
    //void setFontSize(QRectF &rect, const QStyleOptionViewItem &option, QPainter *painter ) const;  //计算字体大小
private:
    QMargins m_margins;

    mutable bool m_enable,m_alert, m_realTime;
    mutable qreal m_height, m_pixel;
};

#endif // TRENDDIGITALDELEGATE_H
