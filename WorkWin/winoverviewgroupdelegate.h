#ifndef WINOVERVIEWGROUPDELEGATE_H
#define WINOVERVIEWGROUPDELEGATE_H

#include <QAbstractItemDelegate>

class WinOverviewGroupDelegate : public QAbstractItemDelegate
{
    Q_OBJECT
public:
    explicit WinOverviewGroupDelegate(QObject *parent = 0);

    virtual void	paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const;

    virtual QSize	sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index) const;

    bool helpEvent(QHelpEvent *event,
                               QAbstractItemView *view,
                               const QStyleOptionViewItem &option,
                               const QModelIndex &index);

private:
    QStyleOptionViewItem setOptions(const QStyleOptionViewItem &option,const QModelIndex &index) const;
    void drawBackground(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void drawGroupNameRect(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void drawPressingRect(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QColor getBackgroundColor(const QStyleOptionViewItem &option, quint32 status) const;

private:
    QMargins m_margins;//通道间隔
};

#endif // WINOVERVIEWGROUPDELEGATE_H
