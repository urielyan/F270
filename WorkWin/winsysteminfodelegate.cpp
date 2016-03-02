#include <QPainter>
#include "winsysteminfodelegate.h"

WinSystemInfoDelegate::WinSystemInfoDelegate(QObject *parent) :
    QItemDelegate(parent)
{
}

void WinSystemInfoDelegate::paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
    Q_ASSERT(index.isValid());
    QVariant value = index.data(Qt::UserRole);

    if(value.isValid() && !value.isNull()){
        painter->save();
        int x = option.rect.center().x() - 6;
        int y = option.rect.center().y() - 6;
        if(index.column() == 0){
            if(value.toBool()){
                painter->setPen(QColor(0,0,0,0));
                painter->setBrush(Qt::green);
                painter->drawEllipse(option.rect.center(),8,8);

            }else{
                painter->fillRect(x,y,12,12,Qt::red);
            }
        }else{
            if(value.toBool()){
                painter->setPen(QColor(0,0,0,0));
                painter->setBrush(Qt::blue);
                painter->drawEllipse(option.rect.center(),8,8);

            }else{
                painter->fillRect(x,y,12,12,Qt::yellow);
            }
        }

        painter->restore();
    }else{
        QItemDelegate::paint(painter,option,index);
    }
}

bool WinSystemInfoDelegate::helpEvent(QHelpEvent */*event*/,
                   QAbstractItemView */*view*/,
                   const QStyleOptionViewItem &/*option*/,
                   const QModelIndex &/*index*/)
{
    return false;
}
