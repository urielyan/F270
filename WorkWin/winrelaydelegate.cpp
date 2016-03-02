#include <QPainter>
#include <QColor>
#include <QDebug>

#include "./GlobalData/cfgGlobalDef.h"
#include "winrelaydelegate.h"

WinRelayDelegate::WinRelayDelegate(QObject *parent) :
    QAbstractItemDelegate(parent),m_margins(-1,-2,-1,-2)
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
QStyleOptionViewItem WinRelayDelegate::setOptions(const QStyleOptionViewItem &option,const QModelIndex &/*index*/) const
{
    QStyleOptionViewItem opt = option;
    opt.rect = option.rect.marginsAdded(m_margins); //边框缩进
//    opt.font.setFamily(opt.font.defaultFamily());//设置默认字体
    if(option.state & QStyle::State_Sunken && option.state & QStyle::State_HasFocus){
        opt.rect.moveTo(opt.rect.x()+1, opt.rect.y()+1);
    }
    return opt;
}

/**
 * 功能：
 *      绘制单元格背景
 */
void WinRelayDelegate::drawBackground(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &/*index*/) const
{
    painter->save();

    /***************绘制背景色***************/
    QRectF rect(option.rect);
    painter->setPen(Qt::transparent);
    painter->setBrush(option.backgroundBrush);
    painter->drawRoundedRect(rect,5,5);

    /****************绘制光照效果****************/
    QLinearGradient skinColor(rect.topLeft(),rect.bottomLeft());
    skinColor.setColorAt(0,QColor(255,255,255,170));
    skinColor.setColorAt(0.09,QColor(255,255,255,170));
    skinColor.setColorAt(0.10,QColor(255,255,255,50));
    skinColor.setColorAt(0.48,QColor(0,0,0,5));
    skinColor.setColorAt(0.49,QColor(100,100,100,20));
    skinColor.setColorAt(0.97,QColor(255,255,255,0));
    skinColor.setColorAt(0.98,QColor(255,255,255,50));
    skinColor.setColorAt(1,QColor(255,255,255,150));
    painter->setPen(Qt::transparent);
    painter->setBrush(QBrush(skinColor));
    painter->drawRoundedRect(rect, 5, 5);

    painter->restore();
}

/**
 * 功能：
 *      布局单元格子元素
 */
void WinRelayDelegate::doLayout(const QStyleOptionViewItem & option, QRectF &valRect, QRectF &tagRect, QRectF &alarmRect) const
{
    tagRect = option.rect;
    tagRect.setHeight(tagRect.height()/2);

    valRect = tagRect;
    valRect.moveTop(valRect.bottom());
    valRect.adjust(valRect.width()/5,valRect.height()/8,-valRect.width()/5,-valRect.height()/8);

    alarmRect = valRect;
    alarmRect.setWidth(alarmRect.height());
//    alarmRect.moveLeft(alarmRect.right());
}

/**
 * 功能：
 *      绘制Ｄｏ通道标记
 */
void WinRelayDelegate::drawTagRect(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index, const QRectF &tagRect) const
{
    QVariant value = index.data(ChannelTag);
    int pixelSize = tagRect.height()/2;
    if(pixelSize > WIN_MIN_FONT_SIZE && value.isValid()){
        painter->save();
        QFont font = option.font;
        font.setPixelSize(pixelSize);
        painter->setFont(font);
        painter->drawText(tagRect, value.toString());
        painter->restore();
    }
}

/**
 * 功能：
 *      绘制报警标记
 */
void WinRelayDelegate::drawAlarmRect(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index, const QRectF &alarmRect, const QRectF &valRect) const
{
    QVariant value = index.data(ChannelData);
    if(value.isValid()){
        painter->save();
        QFont font = option.font;
        font.setPixelSize(valRect.height()/4*3);
        painter->setFont(font);
        painter->setPen(option.palette.foreground().color());
        painter->drawText(valRect, Qt::AlignRight|Qt::AlignVCenter, index.data(ChannelDataAccuracy).toString().split('|').at(value.toInt()));
        painter->setPen(QColor(0,0,0,0));
        qreal cx = 0.445 * alarmRect.width(), cy=0.527955 * alarmRect.height(), fx = 0.404545 * alarmRect.width(), fy=0.381*alarmRect.height();
        QRadialGradient light;
        light.setCenter(alarmRect.x() + cx, alarmRect.y() + cy);
        light.setFocalPoint(alarmRect.x() + fx, alarmRect.y() + fy);
        light.setColorAt(0,QColor(255,255,255,255));
        light.setRadius(alarmRect.height() * 0.88);
        if(value.toInt() == 0){
            light.setColorAt(1,QColor(150, 150, 150, 223));
        }else{
            light.setColorAt(1,QColor(255, 0, 0, 223));
        }
        painter->setBrush(light);
        painter->drawRoundedRect(alarmRect,5,5);
        painter->restore();
    }
}

/**
 * 功能：
 *      绘制按钮点击效果
 */
void WinRelayDelegate::drawPressingRect(QPainter *painter,const QStyleOptionViewItem &option, const QModelIndex &/*index*/) const
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

void	WinRelayDelegate::paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
    Q_ASSERT(index.isValid());
    if( index.data(ChannelValidity).isValid()){
        painter->save();
        painter->setRenderHint(QPainter::Antialiasing);
        QStyleOptionViewItem opt = setOptions(option,index);
        QRectF valRect, tagRect, alarmRect;
        doLayout(opt,valRect,tagRect,alarmRect);
        drawBackground(painter,opt,index);
        drawTagRect(painter,opt,index,tagRect);
        drawAlarmRect(painter,opt,index,alarmRect,valRect);
        drawPressingRect(painter,opt,index);

        painter->restore();
    }
}

QSize	WinRelayDelegate::sizeHint(const QStyleOptionViewItem & option, const QModelIndex &/* index*/) const
{
    return option.rect.size();
}

bool WinRelayDelegate::helpEvent(QHelpEvent */*event*/,QAbstractItemView */*view*/,const QStyleOptionViewItem &/*option*/,const QModelIndex &/*index*/)
{
    return false;
}
