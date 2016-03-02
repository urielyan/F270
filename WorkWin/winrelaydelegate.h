#ifndef WINRELAYDELEGATE_H
#define WINRELAYDELEGATE_H

#include <QAbstractItemDelegate>
#define WIN_MIN_FONT_SIZE                  8                                                    //字体最小尺寸
class WinRelayDelegate : public QAbstractItemDelegate
{
    Q_OBJECT
public:
    explicit WinRelayDelegate(QObject *parent = 0);

    virtual void	paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const;

    virtual QSize	sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index) const;

    bool helpEvent(QHelpEvent *event,
                               QAbstractItemView *view,
                               const QStyleOptionViewItem &option,
                               const QModelIndex &index);
signals:

public slots:

private:
    QStyleOptionViewItem setOptions(const QStyleOptionViewItem &option,const QModelIndex &index) const;
    void doLayout(const QStyleOptionViewItem & option, QRectF &valRect, QRectF &tagRect, QRectF &alarmRect) const;
    void drawBackground(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void drawPressingRect(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void drawTagRect(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index, const QRectF &tagRect) const;
    void drawAlarmRect(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index, const QRectF &alarmRect, const QRectF &valRect) const;

private:
    QMargins m_margins;//继电器单元间隔
};

#endif // WINRELAYDELEGATE_H
