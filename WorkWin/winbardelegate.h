#ifndef WINBARDELEGATE_H
#define WINBARDELEGATE_H

#include <QAbstractItemDelegate>

class WinBarDelegate : public QAbstractItemDelegate
{
    Q_OBJECT
public:
    explicit WinBarDelegate(QObject *parent = 0);
    virtual void	paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const;

    virtual QSize	sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index) const;

    bool helpEvent(QHelpEvent *event,
                               QAbstractItemView *view,
                               const QStyleOptionViewItem &option,
                               const QModelIndex &index);

private:
    QStyleOptionViewItem setOption(const QStyleOptionViewItem & option,const QModelIndex & index) const;
    void doLayout(const QStyleOptionViewItem &option, QRectF &topRect, QRectF &bottomRect, QRectF &tagRect, QRectF &valRect, QRectF &alarmRect, qreal &alarmSpan) const;
    void drawBackground(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index,const QRectF &topRect, const  QRectF &bottomRect) const;
    void drawLightRect(QPainter *painter, const QStyleOptionViewItem &option, const QRectF &topRect, const QRectF &bottomRect) const;
    void drawTagRect(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index,const QRectF &tagRect) const;
    void drawColorBarRect(QPainter *painter,  const QModelIndex &index, const QRectF &bottomRect) const;
    void drawColorBandRect(QPainter *painter,const QModelIndex &index, const QRectF &bottomRect) const;
    void drawScaleRect(QPainter *painter,const QStyleOptionViewItem &option,const QModelIndex &index, const QRectF &bottomRect) const;
    void drawValueRect(QPainter *painter,const QStyleOptionViewItem &option,const QModelIndex &index,  const QRectF &bottomRect, QRectF &valRect,QString measureVal) const;
    void drawAlarmRect(QPainter *painter,const QStyleOptionViewItem &option, const QModelIndex &index, const QRectF &bottomRect,QRectF &alarmRect, qreal alarmSpan) const;
    void drawAlarmElement(QPainter *painter, const QStyleOptionViewItem &option, QRectF &alarmRect,  uint alarmStatus, uint alarmType) const;
    void drawAlarmElement(QPainter *painter, const QStyleOptionViewItem &option, QRectF &alarmRect,  uint alarmStatus, uint alarmType, uint markType, QColor &alarmColor, QPointF *markPoints) const;
    void drawPressingRect(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:
    QMargins m_margins;//边缘间距
    mutable qreal m_baseLine;//基准点
    mutable qreal m_mesureVal;//测量值
    mutable qreal m_rangeUpper;//显示上限
    mutable qreal m_rangeDowner;//显示下限
    mutable uint m_dot;//测量值精度
    mutable QColor m_chanColor;//通道颜色
    mutable bool m_enable,m_alert;
};

/***********************水平模式******************************/
class WinHBarDelegate : public QAbstractItemDelegate
{
    Q_OBJECT
public:
    explicit WinHBarDelegate(QObject *parent = 0);
    virtual void	paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const;

    virtual QSize	sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index) const;

    bool helpEvent(QHelpEvent *event,
                               QAbstractItemView *view,
                               const QStyleOptionViewItem &option,
                               const QModelIndex &index);

private:
    QStyleOptionViewItem setOption(const QStyleOptionViewItem & option,const QModelIndex & index) const;
    void drawBackground(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void drawLightRect(QPainter *painter, const QStyleOptionViewItem &option) const;
    void drawColorBarRect(QPainter *painter,  const QModelIndex &index, const QRectF &leftRect) const;
    void drawColorBandRect(QPainter *painter,const QModelIndex &index, const QRectF &leftRect) const;
    void drawScaleRect(QPainter *painter,const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void drawMarkRect(QPainter *painter,const QModelIndex &index, const QRectF &leftRect) const;
    void drawMarkElement(QPainter *painter, uint alarmStatus, uint alarmType, uint markType, QPointF *markPoints, QColor &alarmColor) const;
    void drawPressingRect(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:
    QMargins m_margins;//边缘间距
    mutable qreal m_mesureVal;//测量值
    mutable qreal m_rangeUpper;//显示上限
    mutable qreal m_rangeDowner;//显示下限
    mutable uint m_dot;//测量值精度
    mutable QColor m_chanColor;//通道颜色
    mutable bool m_enable,m_alert;
};

#endif // WINBARDELEGATE_H
