#ifndef WINDIGITALDELEGATE_H
#define WINDIGITALDELEGATE_H

#include <QAbstractItemDelegate>

class WinDigitalDelegate : public QAbstractItemDelegate
{
    Q_OBJECT
public:
    explicit WinDigitalDelegate(QObject *parent = 0);

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
    void drawChannelTagRect(QPainter * painter, const QStyleOptionViewItem &option, const QModelIndex &index,  const QRectF &rect) const;
    void drawChannelUnitRect(QPainter * painter, const QStyleOptionViewItem &option, const QModelIndex &index, const QRectF &rect)const;
    void drawAlarmRect(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index,  QRectF &alarmRect, qreal alarmSpan) const;
    void drawAlarmElement(QPainter *painter, const QStyleOptionViewItem &option,QRectF &alarmRect,  uint alarmStatus, uint alarmType) const;

private:
    QMargins m_margins;//边界间距
    mutable bool m_enable,m_alert;
};

#endif // WINDIGITALDELEGATE_H
