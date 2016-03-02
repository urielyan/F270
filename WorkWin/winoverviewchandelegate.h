#ifndef WINOVERVIEWCHANDELEGATE_H
#define WINOVERVIEWCHANDELEGATE_H

#include <QAbstractItemDelegate>
#define WIN_MIN_FONT_SIZE                  8                                                    //字体最小尺寸

class WinOverviewChanDelegate : public QAbstractItemDelegate
{
    Q_OBJECT
public:
    explicit WinOverviewChanDelegate(QObject *parent = 0);

    virtual void	paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const;

    virtual QSize	sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index) const;

    bool helpEvent(QHelpEvent *event,
                               QAbstractItemView *view,
                               const QStyleOptionViewItem &option,
                               const QModelIndex &index);

private:
    QStyleOptionViewItem setOptions(const QStyleOptionViewItem &option,const QModelIndex &index) const;
    void doLayout(const QStyleOptionViewItem &option, QRectF &valRect, QRectF &unitRect, QRectF &tagRect) const;
    void drawBackground(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void drawTagRect(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index, const QRectF &tagRect) const;
    void drawUnitRect(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index, const QRectF &unitRect) const;
    void drawMesureValRect(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index, const QRectF &valRect,QString measureVal) const;
    void drawPressingRect(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QColor getBackgroundColor(const QStyleOptionViewItem &option,QVariant alarmStatus) const;
    QVariant getChannelString(QVariant alarmType,const QModelIndex &index) const;
    void  initAlarmParamater(const QModelIndex &index) const;

private:
    QMargins m_margins;
    mutable QVariant m_alarmStatus;
    mutable QVariant m_alarmType;
    mutable bool m_enable,m_alert;
};

#endif // WINOVERVIEWCHANDELEGATE_H
