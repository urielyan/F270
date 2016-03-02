#include <QPainter>
#include "winoverviewgroupdelegate.h"
#include "./GlobalData/cfgGlobalDef.h"

WinOverviewGroupDelegate::WinOverviewGroupDelegate(QObject *parent) :
    QAbstractItemDelegate(parent),m_margins(-2,-2,-2,-2)
{
}

/**
 * 功能：
 *          设置单元格样式
 * 参数：
 *          １、const QStyleOptionViewItem & option　当前默认样式
 *          ２、const QModelIndex & index　单元格索引
 * 返回值：
 *          QStyleOptionViewItem：样式
 */
QStyleOptionViewItem WinOverviewGroupDelegate::setOptions(const QStyleOptionViewItem &option,const QModelIndex &/*index*/) const
{
    QStyleOptionViewItem opt = option;
    opt.rect = option.rect.marginsAdded(m_margins); //边框缩进

    if(option.state & QStyle::State_Sunken && option.state & QStyle::State_HasFocus){
        opt.rect.moveTo(opt.rect.x()+1, opt.rect.y()+1);
    }
    return opt;
}

/**
 * 功能：
 *         绘制单元格背景
 */
void WinOverviewGroupDelegate::drawBackground(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    painter->save();
    QPainterPath path;
    path.addRoundedRect(option.rect,5,5);
    painter->setClipPath(path);
    /***************绘制背景色***************/
    QRectF rect(option.rect);
    painter->setPen(Qt::transparent);

    QVariant value = index.data(ChannelData);
    if(value.isValid()){
        painter->setBrush(QBrush(getBackgroundColor(option, value.toUInt())));//报警状态背景
    }else{
        painter->setBrush(option.backgroundBrush);
    }
    painter->drawRect(rect);


    /*******************三角标记********************/
    painter->setPen(Qt::transparent);
    painter->setBrush(QBrush(QColor(255,142,0)));
    QPointF points[3];
    points[0] = rect.topRight();
    points[1] = QPointF(rect.right(), rect.top() + rect.height()/5);
    points[2] = QPointF(rect.right() - rect.height()/5, rect.top());
    painter->drawPolygon(points,3);

    /****************绘制光泽效果****************/
    QLinearGradient skinColor(rect.topLeft(),rect.bottomLeft());
    skinColor.setColorAt(0,QColor(255,255,255,160));
    skinColor.setColorAt(0.04,QColor(255,255,255,160));
    skinColor.setColorAt(0.05,QColor(255,255,255,100));
    skinColor.setColorAt(0.48,QColor(0,0,0,5));
    skinColor.setColorAt(0.49,QColor(100,100,100,20));
    skinColor.setColorAt(0.97,QColor(255,255,255,0));
    skinColor.setColorAt(0.98,QColor(255,255,255,50));
    skinColor.setColorAt(1,QColor(255,255,255,150));
    painter->setPen(Qt::transparent);
    painter->setBrush(QBrush(skinColor));
    painter->drawRect(rect);

    painter->restore();
}

/**
 * 功能：
 *      根据报警状态获取组背景
 */
QColor WinOverviewGroupDelegate::getBackgroundColor(const QStyleOptionViewItem &option, quint32 status) const
{
    bool isBlink = (option.state & QStyle::State_On);
    QColor color = option.backgroundBrush.color();
    switch(status){
    case SQL_Alarm_Status_NonHold_Occur:
        color = QColor(Qt::red);
        break;
    case SQL_Alarm_Status_NonHold_Release:
        break;
    case SQL_Alarm_Status_Hold_Occur_NoAck:
        if(isBlink ){
            color = QColor(Qt::red);
        }else{
            color = QColor(Qt::darkRed);
        }
        break;
    case SQL_Alarm_Status_Hold_Occur_Ack:
        color = QColor(Qt::red);
        break;
    case SQL_Alarm_Status_Hold_Release_NoAck:
        if(isBlink){
            color = QColor(QColor(64,64,64));
        }else{
            color = QColor(Qt::gray);
        }
        break;
    case SQL_Alarm_Status_Hold_Release_Ack:
        break;
    default:
        break;
    }
    return color;
}

/**
 *功能：
 *      绘制显示组名称
 */
void WinOverviewGroupDelegate::drawGroupNameRect(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    /*********************组名**********************/
    QVariant value =  index.data(ChannelTag);
    if(value.isValid()){
            painter->save();

            QFont font =option.font;
            font.setPixelSize(option.rect.height()/3);
            painter->setFont(font);
            painter->setPen(QPen(option.palette.foreground().color(), 1));
            painter->drawText(option.rect,Qt::AlignCenter, value.toString());

            painter->restore();
    }
}

/**
 * 功能：
 *      绘制按钮点击效果
 */
void WinOverviewGroupDelegate::drawPressingRect(QPainter *painter,const QStyleOptionViewItem &option, const QModelIndex &/*index*/) const
{
    painter->save();

    /*********************按压效果**********************/
    QRectF rect(option.rect);
    if(option.state & QStyle::State_Sunken && option.state & QStyle::State_HasFocus){
        painter->setPen(QPen(QColor(0,0,255,100),3));
        painter->setBrush(QBrush(QColor(180,180,180,50)));
        painter->drawRoundedRect(rect,6,6);
    }

    painter->restore();
}

void WinOverviewGroupDelegate::paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
    Q_ASSERT(index.isValid());
    if( index.data(ChannelValidity).isValid()){
        painter->save();
        painter->setRenderHint(QPainter::Antialiasing);

        QStyleOptionViewItem opt = setOptions(option,index);
        drawBackground(painter, opt, index);
        drawGroupNameRect(painter,opt,index);
        drawPressingRect(painter,opt,index);

        painter->restore();
    }
}

QSize WinOverviewGroupDelegate::sizeHint(const QStyleOptionViewItem & option, const QModelIndex & /*index*/) const
{
    return option.rect.size();
}

/*
 * 功能：
 *          去掉系统自带的toolTip
 */
bool WinOverviewGroupDelegate::helpEvent(QHelpEvent */*event*/,
                           QAbstractItemView */*view*/,
                           const QStyleOptionViewItem &/*option*/,
                           const QModelIndex &/*index*/)
{
    return false;
}
