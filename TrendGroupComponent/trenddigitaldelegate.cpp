#include <QPainter>
#include <QStyleOptionViewItem>
#include <QModelIndex>
#include <QDebug>
#include <QPainter>
#include <QApplication>
#include <QLinearGradient>

#include "GlobalData/cfgGlobalDef.h"
#include "trenddigitaldelegate.h"

extern CfgTrendGroup *p_gCfgTrendGroup;

#define WIN_MIN_FONT_SIZE                  4                                                    //字体最小尺寸
//#define WIN_MAX_FONT_SIZE                 100                                                  //字体最大尺寸
#define WIN_MIN_ALARM_SIZE               WIN_MIN_FONT_SIZE + 4          //警报灯最小尺寸

TrendDigitalDelegate::TrendDigitalDelegate(QObject *parent) :
    QAbstractItemDelegate(parent),m_margins(-1,-1,-1,-1),m_height(0)
{
}

void TrendDigitalDelegate::doLayout(const QStyleOptionViewItem & option, QRectF &valRect, QRectF &unitRect, QRectF &tagRect, QRectF &alarmRect, qreal &alarmSpan) const
{
    if(m_realTime){
        /***********测量值***********/
        valRect = option.rect;
        valRect.setHeight(valRect.height()/3);
        valRect.moveTop(valRect.bottom());
        valRect.setRight(valRect.right() - valRect.width()/18 - 3);

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
        alarmSpan = alarmRect.width() * 1.5;
        if(alarmRect.left() < option.rect.left())
            alarmRect.moveLeft(option.rect.left());
    }else{
        /************标记**********/
        tagRect = option.rect;
        tagRect.setHeight(tagRect.height()/4);
        tagRect.setRight(tagRect.right() - tagRect.width()/18- 3);
        tagRect.setTop(tagRect.top() + option.rect.height()/10);

        /***********测量值***********/
        valRect = option.rect;
        valRect.setHeight(valRect.height()/2);
        valRect.moveTop(tagRect.bottom());
        valRect.setRight(tagRect.right());

        /**********记录高度字体*********/
        if(m_height != valRect.height()){
            m_height = valRect.height();
            calcFontSize(option,valRect);
        }

        /************单位***********/
        unitRect = option.rect;
        unitRect.setHeight(unitRect.height()/4);
        unitRect.moveTop(valRect.bottom());
        unitRect.setRight(valRect.right() - option.rect.height()/10);

        /*************报警标记**********/
        alarmRect = unitRect;
        alarmRect.setHeight(alarmRect.height()/2);
        alarmRect.moveCenter(unitRect.center());
        alarmRect.setLeft(alarmRect.center().x() - 4*alarmRect.height());
        alarmRect.setWidth(alarmRect.height());
        alarmSpan = alarmRect.width() * 1.5;
        if(alarmRect.left() < option.rect.left())
            alarmRect.moveLeft(option.rect.left());
    }
}


QStyleOptionViewItem TrendDigitalDelegate::setOptions(const QStyleOptionViewItem &option,const QModelIndex &/*index*/) const
{
    QStyleOptionViewItem opt = option;
    opt.rect = option.rect.marginsAdded(m_margins); //边框缩进
    if(m_enable && option.state & QStyle::State_Sunken && option.state & QStyle::State_HasFocus){
        opt.rect.moveTo(opt.rect.x()+1, opt.rect.y()+1);
    }
    return opt;
}

void TrendDigitalDelegate::drawBackground(QPainter *painter,const QStyleOptionViewItem &option, const QModelIndex &index) const
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
//    QLinearGradient skinColor(rect.topLeft(),rect.bottomLeft());
//    skinColor.setColorAt(0,QColor(255,255,255,170));
//    skinColor.setColorAt(0.09,QColor(255,255,255,170));
//    skinColor.setColorAt(0.10,QColor(255,255,255,50));
//    skinColor.setColorAt(0.48,QColor(0,0,0,5));
//    skinColor.setColorAt(0.49,QColor(100,100,100,20));
//    skinColor.setColorAt(0.97,QColor(255,255,255,0));
//    skinColor.setColorAt(0.98,QColor(255,255,255,50));
//    skinColor.setColorAt(1,QColor(255,255,255,150));
//    painter->setPen(Qt::transparent);
//    painter->setBrush(QBrush(skinColor));
//    painter->drawRect(rect);

    painter->restore();
}

void TrendDigitalDelegate::drawPressingRect(QPainter *painter,const QStyleOptionViewItem &option, const QModelIndex &/*index*/) const
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

void TrendDigitalDelegate::drawSelectionRect(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &/*index*/) const
{
    painter->save();

    /*********************选择框**********************/
    QRectF rect(option.rect);
    if(option.state & QStyle::State_Selected){
        painter->setPen(QPen(QColor(0,255,255,100),1));
        painter->setBrush(QBrush(QColor(93,71,139,150)));
        painter->drawRoundedRect(rect,6,6);
    }

    painter->restore();
}

/*
 * 记录模块
 *
 */
void TrendDigitalDelegate::drawMesureValueRect(QPainter * painter, const QStyleOptionViewItem &option, const QModelIndex &index, const QRectF &rect) const
{
    if(m_pixel < WIN_MIN_FONT_SIZE)
        return;

    painter->save();

    QFont font = option.font;
    font.setPixelSize(m_pixel);
    painter->setFont(font);
    painter->setPen(option.palette.foreground().color());

    uint dot = index.data(ChannelDataAccuracy).toUInt();
    QLineF data1 = index.data(ChannelData).toLineF();

    QString textP1 = QString::number(data1.p1().x(),'f',dot);   //当前值
    QString textP2 = QString::number(data1.p1().y(),'f',dot);
    QString textMax = QString::number(data1.p2().x(), 'f', dot);
    QString textMin = QString::number(data1.p2().y(), 'f', dot);
    QRectF rect1(rect.x(), rect.y(), rect.width()/2, rect.height()/2);// = painter->boundingRect(rect,Qt::AlignVCenter,textP1);
    QRectF rect2(rect.x(), rect.y()+rect1.height(), rect.width()/2, rect.height()/2);// = painter->boundingRect(rect,Qt::AlignVCenter,textP2);
    QRectF rect3(rect.x()+rect1.width(), rect1.y(), rect1.width(), rect1.height());// = painter->boundingRect(rect,Qt::AlignVCenter,textMax);
    QRectF rect4(rect3.x(), rect2.y(), rect2.width(),rect2.height());// = painter->boundingRect(rect,Qt::AlignVCenter,textMin);

//    qreal textLen = qMax(rect1.width(),rect2.width());
//    textLen = qMax(textLen,rect3.width());
//    textLen = qMax(textLen,rect4.width());
//    if(textLen*2.1 > rect.width()){
//        pixelSize = pixelSize * rect.width()/2.1*textLen;
//    }

//    if(pixelSize < WIN_MIN_FONT_SIZE){
//        painter->restore();
//        return;
//    }
//    rect1.setRect();
//    rect2.setRect();
//    rect3.setRect();
//    rect4.setRect();

    painter->drawText(rect3,Qt::AlignVCenter,textMax);
    painter->drawText(rect4,Qt::AlignVCenter,textMin);

    if(m_alert){
        painter->setPen(Qt::red);
    }

    painter->drawText(rect1,Qt::AlignVCenter,textP1);
    painter->drawText(rect2,Qt::AlignVCenter,textP2);

    painter->restore();
}


/*
 * 实时模块
 *
 */
void TrendDigitalDelegate::drawMesureValueRect(QPainter * painter, const QStyleOptionViewItem &option, const QModelIndex &index, const QRectF &rect,  QString measureVal) const
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
            if(pixelSize < WIN_MIN_FONT_SIZE){
                painter->restore();
                return;
            }
            font.setPixelSize(pixelSize);
            painter->setFont(font);
        }
            painter->drawText(rect,Qt::AlignVCenter,text);
    }

    painter->restore();
}

void TrendDigitalDelegate::drawChannelTagRect(QPainter * painter, const QStyleOptionViewItem &option,const QModelIndex &index, const QRectF &rect) const
{
    int pixelSize = rect.height();
    if(m_realTime){
        pixelSize = rect.height()/3 * 2;
    }

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

void TrendDigitalDelegate::drawChannelUnitRect(QPainter * painter, const QStyleOptionViewItem &option, const QModelIndex &index,const QRectF &rect)const
{
    int pixelSize = rect.height()/2;
    if(pixelSize < WIN_MIN_FONT_SIZE)
        return;

    painter->save();

    QFont font = option.font;
    font.setPixelSize(pixelSize);
    painter->setFont(font);
    painter->setPen(option.palette.foreground().color());

    QVariant value = index.data(ChannelUnit);
    if(value.isValid()){
        QString text = value.toString();
        if(!text.isEmpty()){
            painter->drawText(rect,Qt::AlignRight,text);//Qt::AlignVCenter|
        }
    }

    painter->restore();
}

void TrendDigitalDelegate::drawAlarmRect(QPainter *painter,const QStyleOptionViewItem &option, const QModelIndex &index, QRectF &alarmRect, qreal alarmSpan) const
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
            drawAlarmElement(painter,option, alarm1Rect,alarmStatus, alarmType);
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
            drawAlarmElement(painter,option, alarm3Rect,alarmStatus, alarmType);
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

void TrendDigitalDelegate::drawAlarmElement(QPainter *painter, const QStyleOptionViewItem &option, QRectF &alarmRect,  uint alarmStatus, uint alarmType) const
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
            alarmColor = QColor(QColor(64,64,64));
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

void TrendDigitalDelegate::drawHistoryAlarmRect(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index,  QRectF &alarmRect, qreal alarmSpan) const
{
    QVariant value = index.data(ChannelStatus);

    if(value.isValid()){
        quint16 alarmStatus = GET_DATA_ALARM(value.toUInt());
        QRectF alarm1Rect, alarm2Rect,alarm3Rect,alarm4Rect;
        alarm1Rect = alarm2Rect = alarm3Rect = alarm4Rect = alarmRect;
        alarm2Rect.moveLeft(alarmRect.left() + alarmSpan);
        alarm3Rect.moveLeft(alarmRect.left() + alarmSpan * 2);
        alarm4Rect.moveLeft(alarmRect.left() + alarmSpan * 3);
        m_alert = (alarmStatus != 0);

        painter->save();
        painter->setPen(QPen(option.palette.foreground().color(),1));
        painter->setBrush(Qt::red);
        int corner = (alarmRect.width() < WIN_MIN_ALARM_SIZE) ?1:3;

        /*********************电平一*******************/
        if(alarmStatus & (1 << DATA_STATUS_ALARM1_BITSOFF))
            painter->drawRoundedRect(alarm1Rect,corner,corner);

        /*********************电平二*******************/
        if(alarmStatus & (1 << DATA_STATUS_ALARM2_BITSOFF))
            painter->drawRoundedRect(alarm2Rect,corner,corner);

        /*********************电平三*******************/
        if(alarmStatus & (1 << DATA_STATUS_ALARM3_BITSOFF))
            painter->drawRoundedRect(alarm3Rect,corner,corner);

        /*********************电平四*******************/
        if(alarmStatus & (1 << DATA_STATUS_ALARM4_BITSOFF))
            painter->drawRoundedRect(alarm4Rect,corner,corner);

        painter->restore();
    }
}

void TrendDigitalDelegate::calcFontSize(const QStyleOptionViewItem &option, QRectF &rect) const
{
    m_pixel = rect.height()/2;
    if(m_pixel > WIN_MIN_FONT_SIZE){
        QFont font = option.font;
        font.setPixelSize(m_pixel);

        qreal width = QFontMetrics(font).averageCharWidth()*8;
        if(width > rect.width()/2){
            m_pixel = m_pixel * (rect.width()/2/width);
        }
    }
}
//void TrendDigitalDelegate::setFontSize(QRectF &rect, const QStyleOptionViewItem &option, QPainter *painter) const
//{
//    m_rtModePixel = rect.height()- 3;
//    qreal fontShowWidth = rect.width()/2;
//    int pixelSize = rect.height()/2;

//    painter->save();
//    QFont font = option.font;
//    font.setPixelSize(pixelSize);
//    painter->setFont(font);

//    qreal fontWidth = painter->fontMetrics().width('0')*6;// fontMetrics().width('a')*6;
//    while (fontWidth > fontShowWidth) {  //应该还有更好的办法
//        pixelSize--;
//        font.setPixelSize(pixelSize);
//        fontWidth = painter->fontMetrics().width('0')*6;
//        painter->setFont(font);
//    }
//    m_hModePixel = pixelSize;
//    painter->restore();
//}

void	TrendDigitalDelegate::paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
    Q_ASSERT(index.isValid());
    QVariant value = index.data(ChannelValidity);
    if( value.isValid() && value.toInt() > 0){
        painter->save();
        painter->setRenderHint(QPainter::Antialiasing);

        QRectF valRect, unitRect, tagRect, alarmRect;
        qreal alarmSpan;
        QString measureVal;
        m_alert = false;
        m_enable = true;
        m_realTime = p_gCfgTrendGroup->getCurrentWorkMode() == RealTimeRecord;
        if(index.data(ChannelType).toUInt() != Channel_Type_DO){
            if(index.data(ChannelRangeType).toUInt() == OFF){
                m_enable = false;
            }
        }else{
            if(m_realTime)
                measureVal = index.data(ChannelDataAccuracy).toString().split('|').at(index.data(ChannelData).toUInt());
        }

        QStyleOptionViewItem opt = setOptions(option,index);

        doLayout(opt, valRect, unitRect, tagRect, alarmRect, alarmSpan);
        drawBackground(painter, opt, index);
        drawChannelTagRect(painter, opt, index, tagRect);

        if(m_enable){
            if(m_realTime){
                drawAlarmRect(painter,opt,index,alarmRect,alarmSpan);
                drawMesureValueRect(painter, opt, index, valRect,measureVal);
            }else{
                drawHistoryAlarmRect(painter,opt,index,alarmRect,alarmSpan);
                drawMesureValueRect(painter, opt, index, valRect);
            }
            drawChannelUnitRect(painter, opt, index, unitRect);
            drawPressingRect(painter, opt, index);
            drawSelectionRect(painter, opt, index);
        }

        painter->restore();
    }
}

QSize	TrendDigitalDelegate::sizeHint(const QStyleOptionViewItem & option, const QModelIndex & /*index*/) const
{
    return option.rect.size();
}

/**
 * @brief TrendDigitalDelegate::helpEvent 去掉系统自带的toolTip
 * @return
 */
bool TrendDigitalDelegate::helpEvent(QHelpEvent */*event*/,
                           QAbstractItemView */*view*/,
                           const QStyleOptionViewItem &/*option*/,
                           const QModelIndex &/*index*/)
{
    return false;
}

