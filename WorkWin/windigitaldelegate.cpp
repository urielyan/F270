#include <QPainter>
#include <QStyleOptionViewItem>
#include <QModelIndex>
#include <QDebug>
#include <QPainter>
#include <QApplication>
#include <QLinearGradient>

#include "GlobalData/cfgGlobalDef.h"
#include "windigitaldelegate.h"

#define WIN_MIN_FONT_SIZE                  4                                                    //字体最小尺寸
#define WIN_MIN_ALARM_SIZE               WIN_MIN_FONT_SIZE + 4          //警报灯最小尺寸

WinDigitalDelegate::WinDigitalDelegate(QObject *parent) :
    QAbstractItemDelegate(parent),m_margins(-6,-6,-6,-6)
{
}
/**
 * 功能
 *      当前单元格子元素布局
 */
void WinDigitalDelegate::doLayout(const QStyleOptionViewItem & option, QRectF &valRect, QRectF &unitRect, QRectF &tagRect, QRectF &alarmRect, qreal &alarmSpan) const
{
    /***********测量值***********/
    valRect = option.rect;
    valRect.setHeight(valRect.height()/3);
    valRect.moveTop(valRect.bottom());
    valRect.setRight(valRect.right() - valRect.width()/18- 3);

    /************标记**********/
    tagRect = valRect;
    tagRect.moveBottom(tagRect.top());
    tagRect.setTop(tagRect.top() + option.rect.height()/10);

    /************单位***********/
    unitRect = valRect;
    unitRect.moveTop(unitRect.bottom());
    unitRect.setRight(unitRect.right() - 15);

    /*************报警标记**********/
    alarmRect = unitRect;
    alarmRect.setHeight(alarmRect.height()/3);
    alarmRect.moveTop(alarmRect.bottom());
    alarmRect.setLeft(alarmRect.center().x() - 4*alarmRect.height());
    alarmRect.setWidth(alarmRect.height());
    alarmSpan = alarmRect.width() * 2;
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
QStyleOptionViewItem WinDigitalDelegate::setOptions(const QStyleOptionViewItem &option,const QModelIndex &/*index*/) const
{
    QStyleOptionViewItem opt = option;
    opt.rect = option.rect.marginsAdded(m_margins); //边框缩进
    if(m_enable && option.state & QStyle::State_Sunken && option.state & QStyle::State_HasFocus){
        opt.rect.moveTo(opt.rect.x()+1, opt.rect.y()+1);
    }
    return opt;
}

/**
 * 功能：
 *         绘制单元格背景
 */
void WinDigitalDelegate::drawBackground(QPainter *painter,const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    painter->save();

    QPainterPath path;
    path.addRoundedRect(option.rect,5,5);
    painter->setClipPath(path);//倒角

    /***************绘制背景色***************/
    QRectF rect(option.rect);
    painter->setPen(Qt::transparent);
    painter->setBrush(option.backgroundBrush);
    painter->drawRect(rect);

    /*****************绘制色卡******************/
    QRectF colorRect(rect);
    colorRect.setLeft(rect.right()-rect.width()/18);
    painter->setBrush(QBrush(index.data(ChannelColor).value<QColor>()));
    painter->drawRect(colorRect);

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
    painter->drawRect(rect);

    painter->restore();
}

/**
 * 功能：
 *      绘制按钮点击效果
 */
void WinDigitalDelegate::drawPressingRect(QPainter *painter,const QStyleOptionViewItem &option, const QModelIndex &/*index*/) const
{
    painter->save();

    /*********************按压效果**********************/
    if(option.state & QStyle::State_Sunken && option.state & QStyle::State_HasFocus){
        painter->setPen(QPen(QColor(0,0,255,100),3));
        painter->setBrush(QBrush(QColor(180,180,180,50)));
        painter->drawRoundedRect(option.rect,5,5);
    }

    painter->restore();
}

/**
 * 功能：
 *          绘制通道测量值
 * 参数：
 *          １、 painter  画刷
 *          ２、 option   风格
 *          ３、index 单元格索引
 *          ４、rect  测量值矩形区
 *          ４、isAlert   报警状态
 */
void WinDigitalDelegate::drawMesureValueRect(QPainter * painter, const QStyleOptionViewItem &option, const QModelIndex &index, const QRectF &rect,QString measureVal) const
{
    int pixelSize = rect.height();
    if(pixelSize < WIN_MIN_FONT_SIZE)
        return;

    painter->save();

    QFont font = option.font;
    font.setPixelSize(pixelSize);
    painter->setFont(font);

    QString text = measureVal;
    if(text.isEmpty()){
        QVariant value = index.data(ChannelStatus);
        if(value.isValid()){
            switch (value.toUInt()) {
            case DATA_ERROR_OVER_POSITIVE:
                text = tr("+Over");
                break;
            case DATA_ERROR_OVER_NEGATIVE:
                text = tr("-Over");
                break;
            case DATA_ERROR_BURNOUT_POSITIVE:
            case DATA_ERROR_BURNOUT_NEGATIVE :
                text = tr("Burnout");
                break;
            case DATA_ERROR_NORMAL :
                text = QString::number(index.data(ChannelData).toReal(),'f',index.data(ChannelDataAccuracy).toInt());
                break;
            default:
                break;
            }
        }
    }

    if(m_alert){ //报警状态显示红色字体
        painter->setPen(Qt::red);
    }else{
        painter->setPen(option.palette.foreground().color());
    }

    if(!text.isEmpty()){
        QRectF textRect = painter->boundingRect(rect,Qt::AlignVCenter,text);
        if(textRect.width() > rect.width()){
            pixelSize = pixelSize * rect.width()/textRect.width();
            if(pixelSize >= WIN_MIN_FONT_SIZE){
                font.setPixelSize(pixelSize);
                painter->setFont(font);
                painter->drawText(rect,Qt::AlignVCenter,text);
            }
        }else{
            painter->drawText(rect,Qt::AlignVCenter,text);
        }
    }

    painter->restore();
}

/**
 * 功能：
 *      绘制通道标记
 */
void WinDigitalDelegate::drawChannelTagRect(QPainter * painter, const QStyleOptionViewItem &option,const QModelIndex &index, const QRectF &rect) const
{
    int pixelSize = rect.height()/3 * 2;
    if(pixelSize < WIN_MIN_FONT_SIZE)
        return;

    painter->save();

    QFont font = option.font;
    font.setPixelSize(pixelSize);
    painter->setFont(font);
    painter->setPen(QColor(64,64,64));
    painter->drawText(rect,Qt::AlignRight,index.data(ChannelTag).toString());//Qt::AlignVCenter|

    painter->restore();
}

/**
 * 功能：
 *      绘制通道测量值单位
 */
void WinDigitalDelegate::drawChannelUnitRect(QPainter * painter, const QStyleOptionViewItem &option, const QModelIndex &index,const QRectF &rect)const
{
    int pixelSize = rect.height()/2;
    if(pixelSize < WIN_MIN_FONT_SIZE)
        return;

    painter->save();

    QFont font = option.font;
    font.setPixelSize(pixelSize);
    painter->setFont(font);
    painter->setPen(option.palette.foreground().color());
    QString text = "";
    QVariant value = index.data(ChannelUnit);
    if(value.isValid()){
        text = value.toString();
    }
    if(!text.isEmpty()){
        painter->drawText(rect,Qt::AlignRight,text);//Qt::AlignVCenter|
    }

    painter->restore();
}

/**
 * 功能：
 *          绘制通道报警标记
 * 参数：
 *          １、painter   画刷
 *          ２、option    风格
 *          ３、index 单元格索引
 *          ４、alarmRect 报警区
 *          ５、alarmSpan 报警区间隔
 */
void WinDigitalDelegate::drawAlarmRect(QPainter *painter,const QStyleOptionViewItem &option, const QModelIndex &index, QRectF &alarmRect, qreal alarmSpan) const
{
    uint alarmStatus, alarmType;
    QRectF alarm1Rect, alarm2Rect,alarm3Rect,alarm4Rect;
    QVariant value1, value2,value3;

    alarm1Rect = alarm2Rect = alarm3Rect = alarm4Rect = alarmRect;
    alarm2Rect.moveLeft(alarmRect.left() + alarmSpan);
    alarm3Rect.moveLeft(alarmRect.left() + alarmSpan * 2);
    alarm4Rect.moveLeft(alarmRect.left() + alarmSpan * 3);

    /*********************电平一*******************/
    value3 = index.data(ChannelAlarm1OnOff);
    if(value3.isValid() && value3.toUInt() != OFF){
        value1 = index.data(ChannelAlarm1Type);
        value2 = index.data(ChannelAlarm1Status);
        if(value1.isValid() && value2.isValid()){
            alarmType = value1.toUInt();
            alarmStatus = value2.toUInt();
            drawAlarmElement(painter, option,alarm1Rect,alarmStatus, alarmType);
        }
    }

    /*********************电平二*******************/
    value3 = index.data(ChannelAlarm2OnOff);
    if(value3.isValid() && value3.toUInt() != OFF){
        value1 = index.data(ChannelAlarm2Type);
        value2 = index.data(ChannelAlarm2Status);
        if(value1.isValid() && value2.isValid()){
            alarmType = value1.toUInt();
            alarmStatus = value2.toUInt();
            drawAlarmElement(painter, option,alarm2Rect,alarmStatus, alarmType);
        }
    }

    /*********************电平三*******************/
    value3 = index.data(ChannelAlarm3OnOff);
    if(value3.isValid() && value3.toUInt() != OFF){
        value1 = index.data(ChannelAlarm3Type);
        value2 = index.data(ChannelAlarm3Status);
        if(value1.isValid() && value2.isValid()){
            alarmType = value1.toUInt();
            alarmStatus = value2.toUInt();
            drawAlarmElement(painter, option,alarm3Rect,alarmStatus, alarmType);
        }
    }


    /*********************电平四*******************/
    value3 = index.data(ChannelAlarm4OnOff);
    if(value3.isValid() && value3.toUInt() != OFF){
        value1 = index.data(ChannelAlarm4Type);
        value2 = index.data(ChannelAlarm4Status);
        if(value1.isValid() && value2.isValid()){
            alarmType = value1.toUInt();
            alarmStatus = value2.toUInt();
            drawAlarmElement(painter, option,alarm4Rect,alarmStatus, alarmType);
        }
    }
}

/**
 * 功能：
 *      绘制报警标记子元素
 */
void WinDigitalDelegate::drawAlarmElement(QPainter *painter, const QStyleOptionViewItem &option,QRectF &alarmRect,  uint alarmStatus, uint alarmType) const
{
    QString alarmChar = "";
    switch (alarmType) {
    case Alarm_Type_H:
        alarmChar = "H";
        break;
    case Alarm_Type_L:
        alarmChar = "L";
        break;
    case Alarm_Type_R:
        alarmChar = "R";
        break;
    case Alarm_Type_r:
        alarmChar = "r";
        break;
    case Alarm_Type_T:
        alarmChar = "T";
        break;
    case Alarm_Type_t:
        alarmChar = "t";
        break;
    case Alarm_Type_h:
        alarmChar = "h";
        break;
    case Alarm_Type_l:
        alarmChar = "l";
        break;
    default:
        return;
    }

    QColor alarmColor;
    switch(alarmStatus){
    case SQL_Alarm_Status_NonHold_Occur:
        alarmColor = QColor(Qt::red);
        m_alert = true;
        break;
    case SQL_Alarm_Status_NonHold_Release:
        alarmColor = QColor(0,0,0,0);
        break;
    case SQL_Alarm_Status_Hold_Occur_NoAck:
        if(option.state & QStyle::State_On){
            alarmColor = QColor(Qt::red);
        }else{
            alarmColor = QColor(Qt::darkRed);
        }
        m_alert  = true;
        break;
    case SQL_Alarm_Status_Hold_Occur_Ack:
        alarmColor = QColor(Qt::red);
        m_alert  = true;
        break;
    case SQL_Alarm_Status_Hold_Release_NoAck:
        if(option.state & QStyle::State_On){
            alarmColor = QColor(64,64,64);
        }else{
            alarmColor = QColor(Qt::gray);
        }
        break;
    case SQL_Alarm_Status_Hold_Release_Ack:
        alarmColor = QColor(0,0,0,0);
        break;
    default:
        return;
    }

    painter->save();
    painter->setBrush(QBrush(alarmColor));
    if(alarmRect.width() >= WIN_MIN_ALARM_SIZE){
        QFont font = option.font;
        font.setPixelSize(alarmRect.height() - 4);
        painter->setFont(font);
        painter->setPen(Qt::transparent);
        painter->drawRoundedRect(alarmRect,3,3);
        painter->setPen(Qt::darkYellow);
        painter->drawText(alarmRect,  Qt::AlignCenter, alarmChar);
    }else{
        painter->setPen(option.palette.foreground().color());
        painter->drawRoundedRect(alarmRect,1,1);
    }
    painter->restore();
}

void	WinDigitalDelegate::paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
    Q_ASSERT(index.isValid());
    QVariant value = index.data(ChannelValidity);
    if(value.isValid()){
        painter->save();
        painter->setRenderHint(QPainter::Antialiasing);//反锯齿

        QRectF valRect, unitRect, tagRect, alarmRect;
        qreal alarmSpan;
        QString measureVal;
        m_alert = false;
        m_enable = true;
        if(index.data(ChannelType).toUInt() != Channel_Type_DO){
            if(index.data(ChannelRangeType).toUInt() == OFF){
                m_enable = false;
            }
        }else{
            measureVal = index.data(ChannelDataAccuracy).toString().split('|').at(index.data(ChannelData).toUInt());
        }

        QStyleOptionViewItem opt = setOptions(option,index);

        doLayout(opt, valRect, unitRect, tagRect, alarmRect, alarmSpan);
        drawBackground(painter, opt, index);
        drawChannelTagRect(painter, opt, index, tagRect);
        if(m_enable){//通道量程类型非OFF
            drawAlarmRect(painter,opt,index,alarmRect,alarmSpan);
            drawMesureValueRect(painter, opt, index, valRect,measureVal);
            drawChannelUnitRect(painter, opt, index, unitRect);
            drawPressingRect(painter, opt, index);
        }

        painter->restore();
    }
}

QSize	WinDigitalDelegate::sizeHint(const QStyleOptionViewItem & option, const QModelIndex & /*index*/) const
{
    return option.rect.size();
}

/**
 * @brief WinDigitalDelegate::helpEvent 去掉系统自带的toolTip
 * @return
 */
bool WinDigitalDelegate::helpEvent(QHelpEvent */*event*/,
                           QAbstractItemView */*view*/,
                           const QStyleOptionViewItem &/*option*/,
                           const QModelIndex &/*index*/)
{
    return false;
}
