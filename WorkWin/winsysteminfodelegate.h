#ifndef WINSYSTEMINFODELEGATE_H
#define WINSYSTEMINFODELEGATE_H

#include <QItemDelegate>
//系统信息视图委托
class WinSystemInfoDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit WinSystemInfoDelegate(QObject *parent = 0);
    void paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const;

    bool helpEvent(QHelpEvent *event,
                   QAbstractItemView *view,
                   const QStyleOptionViewItem &option,
                   const QModelIndex &index);

};

#endif // WINSYSTEMINFODELEGATE_H
