#include <QPainter>
#include <QStyleOptionViewItem>
#include <QModelIndex>
#include <QDebug>
#include <QPainter>
#include <QApplication>
#include <QLinearGradient>

#include "GlobalData/cfgGlobalDef.h"
#include "winbardelegate.h"

#define WIN_MIN_FONT_SIZE                  4                                                    //字体最小尺寸
#define WIN_MAX_FONT_SIZE                  32                                                 //字体最大尺寸
#define WIN_MIN_ALARM_SIZE               WIN_MIN_FONT_SIZE + 4           //警报灯最小尺寸
#define MARK_WIDTH 6
#define MARK_HEIGHT 5

WinBarDelegate::WinBarDelegate(QObject *parent) :
    QAbstractItemDelegate(parent),m_margins(-3,-6,-3,-6)
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
QStyleOptionViewItem WinBarDelegate::setOption(const QStyleOptionViewItem & option, const QModelIndex &/* index*/) const
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
 *          布局子元素
 * 参数：
 *          １、const QStyleOptionViewItem &option    样式
 *          ２、QRectF &topRect   顶部矩形区
 *          ３、 QRectF &bottomRect   底部矩形区
 *          ４、 QRectF &tagRect   通道标记矩形区
 *          ５、QRectF &valRect   测量值矩形区
 *          ６、QRectF &alarmRect  警报矩形区
 *          ７、qreal &alarmSpan  报警区间间隔
 */
void WinBarDelegate::doLayout(const QStyleOptionViewItem &option, QRectF &topRect, QRectF &bottomRect, QRectF &tagRect, QRectF &valRect, QRectF &alarmRect, qreal &alarmSpan) const
{
    /*****************顶部****************/
    topRect = option.rect;
    topRect.setHeight(topRect.height()/6);

    /*****************底部****************/
    bottomRect = option.rect;
    bottomRect.setTop(topRect.bottom()+2);

    /*****************通道标记****************/
    tagRect = topRect;
    tagRect.moveTop(tagRect.top() + tagRect.height()/5);
    tagRect.setHeight(topRect.height()/25*12);// 4/5 * 3/5

    /****************报警标记与间隔*****************/
    alarmSpan = topRect.width()/5;
    alarmRect = tagRect;
    alarmRect.setHeight(topRect.height()/25 *4);// 4/5 * 1/5
    alarmRect.moveBottom(topRect.bottom() - topRect.height()/25*4 + alarmRect.height()/2);
    alarmRect.setWidth(alarmRect.height());
    alarmRect.moveLeft(alarmRect.left() + alarmSpan - alarmRect.width()/2);


    /****************字符*****************/
    valRect = bottomRect;
    valRect.setHeight(valRect.height()/16);
    valRect.setRight(valRect.right() - valRect.width()/8);
}

/**
 * 功能：
 *          绘制棒图背景色
 */
void WinBarDelegate::drawBackground(QPainter *painter, const QStyleOptionViewItem &option ,const QModelIndex &/*index*/,const QRectF &topRect, const QRectF &bottomRect) const
{
    painter->save();

    /***************颜色卡*********************/
    QRectF colorRect(topRect);
    colorRect.setHeight(topRect.height()/5 * 3);
    painter->setPen(Qt::transparent);
    painter->setBrush(m_chanColor);
    painter->drawRoundedRect(colorRect,5,5);

    /****************顶部背景色********************/
    QRectF headRect(topRect);
    headRect.setTop(colorRect.center().y());
    painter->setBrush(option.backgroundBrush);
    painter->drawRect(headRect);

    /****************底部背景色********************/
    painter->setPen(QPen(option.palette.foreground().color(),1));
    painter->setBrush(option.backgroundBrush);
    painter->drawRect(bottomRect);

    painter->restore();
}

/**
 * 功能：
 *          绘制光照效果
 */
void WinBarDelegate::drawLightRect(QPainter *painter,const QStyleOptionViewItem &option,const QRectF &topRect, const QRectF &bottomRect) const
{
    painter->save();

    /****************光照效果*****************/
    QLinearGradient skinColor(option.rect.topLeft(),option.rect.topRight());
    skinColor.setColorAt(0,QColor(255,255,255,0));
    skinColor.setColorAt(0.09,QColor(255,255,255,0));
    skinColor.setColorAt(0.1,QColor(255,255,255,200));
    skinColor.setColorAt(0.13,QColor(255,255,255,200));
    skinColor.setColorAt(0.14,QColor(255,255,255,80));
    skinColor.setColorAt(0.50,QColor(0,0,0,20));
    skinColor.setColorAt(0.75,QColor(0,0,0,10));
    skinColor.setColorAt(0.76,QColor(255,255,255,30));
    skinColor.setColorAt(0.95,QColor(255,255,255,150));
    skinColor.setColorAt(0.96,QColor(255,255,255,0));
    skinColor.setColorAt(1,QColor(255,255,255,0));
    painter->setPen(QPen(QColor(200,230,255,0)));
    painter->setBrush(skinColor);
    painter->drawRect(option.rect);

    /*****************擦除头尾连接**********************/
    painter->setBrush(Qt::transparent);
    painter->setPen(QPen(QColor(64,64,64),1));
    painter->drawRect(bottomRect);
    painter->eraseRect(QRectF(topRect.bottomLeft(), bottomRect.topRight()));

    painter->restore();
}

void WinBarDelegate::drawTagRect(QPainter *painter, const QStyleOptionViewItem &option,const QModelIndex &index, const QRectF &tagRect) const
{
    /*******************通道标记**********************/
    int pixelSize = tagRect.height()/3*2;
    QString text = index.data(ChannelTag).toString();
    if(pixelSize < WIN_MIN_FONT_SIZE){
        return;
    }

    painter->save();

    QFont font = option.font;
    font.setPixelSize(pixelSize);
    painter->setFont(font);
    painter->setPen(QColor(64,64,64,255));

    QRectF textRect = painter->boundingRect(tagRect,Qt::AlignCenter,text);
    if(textRect.width() > tagRect.width()){
        pixelSize = pixelSize * tagRect.width()/textRect.width();
        if(pixelSize >= WIN_MIN_FONT_SIZE){
            font.setPixelSize(pixelSize);
            painter->setFont(font);
            painter->drawText(tagRect,Qt::AlignCenter,text);
        }
    }else{
        painter->drawText(tagRect,Qt::AlignCenter,text);
    }
    painter->restore();
}

/**
 * 功能：
 *          绘制棒图棒状色柱
 *
 */
void WinBarDelegate::drawColorBarRect(QPainter *painter,  const QModelIndex &index, const QRectF &bottomRect) const
{
    m_baseLine = bottomRect.bottom();

    /******************色柱********************/
    QVariant value = index.data(ChannelBarBase);
    if(!value.isValid()){
        return;
    }
    quint8 base = value.toUInt();
    QRectF barRect(bottomRect);

    if(m_rangeUpper <= m_rangeDowner){
        m_baseLine = bottomRect.center().y();
        return;
    }else if(m_mesureVal > m_rangeUpper){
        m_mesureVal = m_rangeUpper;
    }else if(m_mesureVal < m_rangeDowner){
        m_mesureVal = m_rangeDowner;
    }

    qreal rate = (m_mesureVal - m_rangeDowner)/(m_rangeUpper - m_rangeDowner);
    barRect.setTop(bottomRect.bottom() - (bottomRect.height()*rate));
    m_baseLine = barRect.top();

    if(base == 1){//中部
        barRect.setBottom(bottomRect.center().y());
    }else if(base == 2){//顶部
        barRect.setBottom(bottomRect.top());
    }//默认底部

    painter->save();
    painter->setPen(Qt::transparent);
    painter->setBrush(m_chanColor);
    painter->drawRect(barRect);

    painter->restore();
}

/**
 * 功能：
 *          绘制棒图彩色标尺带
 */
void WinBarDelegate::drawColorBandRect(QPainter *painter,const QModelIndex &index, const QRectF &bottomRect) const
{
    QVariant posValue = index.data(ChannelColorScalePos);
    if(!posValue.isValid() || posValue.toUInt() == 0){
        return;
    }
    uint pos = posValue.toUInt();

    qreal upper = index.data(ChannelColorScaleUpper).toFloat();
    qreal downer = index.data(ChannelColorScaleDowner).toFloat();
    if(upper == downer){
        return;
    }else if(upper < downer){
        qSwap(upper,downer);
    }

    QColor color = index.data(ChannelColorScaleColor).value<QColor>();
    int width = bottomRect.width()/10;
    if(width > 15){
        width = 15;
    }else if(width < 2){
        return;
    }

    QRectF bandRect(bottomRect);
    bandRect.setLeft(bandRect.right() - width);
    QRectF rect(bandRect);
    qreal upRate = (upper - m_rangeDowner)/(m_rangeUpper - m_rangeDowner);
    qreal downRate = (downer - m_rangeDowner)/(m_rangeUpper - m_rangeDowner);
    bandRect.setTop(bottomRect.bottom() - bottomRect.height()*upRate);
    bandRect.setBottom(bottomRect.bottom() - bottomRect.height()*downRate);
    painter->save();

    painter->setPen(Qt::transparent);
    painter->setBrush(QBrush(color));
    if(pos == 1){//内侧
        painter->drawRect(bandRect);
    }else{//外侧
        painter->drawRect(QRectF(rect.topLeft(), bandRect.topRight()));
        painter->drawRect(QRectF(bandRect.bottomLeft(), rect.bottomRight()));
    }

    painter->restore();
}

/**
 * 功能：
 *          绘制棒图刻度
 */
void WinBarDelegate::drawScaleRect(QPainter *painter,const QStyleOptionViewItem &/*option*/,const QModelIndex &index, const QRectF &bottomRect) const
{
    /*****************刻度*********************/
    QVariant value = index.data(ChannelBarDiv);
    if(value.isValid()){
        uint div = value.toUInt();
        uint center = div%2?0:div/2;
        qreal span = bottomRect.height()/div;
        qreal baseX = bottomRect.right();
        qreal baseY = bottomRect.top();
        qreal len = bottomRect.width()/10;

        painter->save();
        painter->setPen(QColor(64,64,64));
        painter->setBrush(Qt::transparent);

        for(uint i =1; i<div; ++i){
            if(i == center){
                painter->drawLine(baseX - 1.5*len , baseY + i*span, baseX, baseY + i*span);  //中线加长
            }else{
                painter->drawLine(baseX - len, baseY + i*span, baseX, baseY + i*span);
            }
        }

        painter->restore();
    }
}

/**
 * 功能：
 *          绘制棒图通道标记、测量值、测量单位、量程
 */
void WinBarDelegate::drawValueRect(QPainter *painter,const QStyleOptionViewItem &option, const QModelIndex &index, const QRectF &bottomRect, QRectF &valRect, QString measureVal) const
{
    int pixelSize = valRect.height();
    if(pixelSize > WIN_MAX_FONT_SIZE){
        valRect.setHeight(WIN_MAX_FONT_SIZE);
    }else if(pixelSize < WIN_MIN_FONT_SIZE){
        return;
    }
    QFont font = option.font;
    font.setPixelSize(pixelSize);
    int fontWidth = QFontMetrics(font).averageCharWidth()*8;
    if(fontWidth > valRect.width()){
        pixelSize = pixelSize * valRect.width()/fontWidth;
        if(pixelSize > WIN_MIN_FONT_SIZE){
            font.setPixelSize(pixelSize);
        }else{
            return;
        }
    }

    painter->save();
    painter->setFont(font);
    painter->setPen(option.palette.foreground().color());

//    QString upperString = QString::number(m_rangeUpper, 'f', m_dot);
//    QString lowerString = QString::number(m_rangeDowner,'f', m_dot);

//    QRectF upperRect = painter->boundingRect(bottomRect,Qt::AlignLeft|Qt::AlignTop,upperString);
//    QRectF lowerRect = painter->boundingRect(bottomRect,Qt::AlignLeft|Qt::AlignBottom,lowerString);
//    qreal width = bottomRect.width();
//    width = qMax(upperRect.width(),width);
//    width = qMax(lowerRect.width(),width);
//    if(width > bottomRect.width()){
//        pixelSize = pixelSize * bottomRect.width()/width;
//        if(pixelSize > WIN_MAX_FONT_SIZE){
//            pixelSize = WIN_MAX_FONT_SIZE;
//        }else if(pixelSize < WIN_MIN_FONT_SIZE){
//            painter->restore();
//            return;
//        }
//        font.setPixelSize(pixelSize);
//        painter->setFont(font);
//    }


    /****************量程******************/
    if(m_rangeUpper > m_rangeDowner){
        painter->drawText(bottomRect, Qt::AlignLeft|Qt::AlignTop, QString::number(m_rangeUpper, 'f', m_dot));
        painter->drawText(bottomRect, Qt::AlignLeft|Qt::AlignBottom, QString::number(m_rangeDowner,'f', m_dot));
    }

    /*******************测量值显示基准线****************/
    if(m_baseLine >= bottomRect.bottom() - valRect.height()*2){
        m_baseLine = bottomRect.bottom() - valRect.height()*2;
    }else if(m_baseLine <= bottomRect.top() + valRect.height()*3){
        m_baseLine = bottomRect.top() + valRect.height()*3;
    }

    QString text = measureVal;
    if(text.isEmpty()){

        /*********************测量单位****************/
        QVariant value = index.data(ChannelUnit);
        if(value.isValid()){
            valRect.moveTop(m_baseLine);
            painter->drawText(valRect, Qt::AlignRight|Qt::AlignVCenter, value.toString());
        }

        /**********************测量值*****************/
        value = index.data(ChannelStatus);
        if(value.isValid()){
            switch (value.toUInt()) {
            case DATA_ERROR_OVER_POSITIVE:
                text = tr("+Over");
                break;
            case DATA_ERROR_OVER_NEGATIVE:
                text = tr("-Over");
                break;
            case DATA_ERROR_BURNOUT_POSITIVE:
                text = tr("Burnout");
                break;
            case DATA_ERROR_BURNOUT_NEGATIVE :
                text = tr("Burnout");
                break;
            case DATA_ERROR_NORMAL :
                text = QString::number(m_mesureVal,'f',m_dot);
                break;
            default:
                break;
            }
        }
    }

    if(m_alert){ //报警状态显示红色字体
        painter->setPen(Qt::red);
    }

    if(!text.isEmpty()){
        valRect.setHeight(valRect.height()/4*5);
        valRect.moveBottom(m_baseLine);
//        QRectF textRect = painter->boundingRect(valRect,Qt::AlignCenter,text);
//        if(textRect.width() > valRect.width()){
//            pixelSize = pixelSize * valRect.width()/textRect.width();
//            if(pixelSize > WIN_MAX_FONT_SIZE){
//                pixelSize = WIN_MAX_FONT_SIZE;
//            }else  if(pixelSize < WIN_MIN_FONT_SIZE){
//                painter->restore();
//                return;
//            }
//            font.setPixelSize(pixelSize);
//            painter->setFont(font);
//        }
        painter->drawText(valRect,Qt::AlignCenter,text);
    }

    painter->restore();
}

/**
 * 功能：
 *          绘制棒图报警标记
 */
void WinBarDelegate::drawAlarmRect(QPainter *painter,const QStyleOptionViewItem &option, const QModelIndex &index,const QRectF &bottomRect, QRectF &alarmRect, qreal alarmSpan) const
{
    qreal alarmVal;
    QColor alarmColor;
    QVariant value1, value2,value3;
    uint alarmStatus, alarmType,markType = index.data(ChannelAlarmType).toUInt();
    QRectF alarm1Rect, alarm2Rect,alarm3Rect,alarm4Rect;
    QPointF markPoints[3] = {bottomRect.bottomRight()};
    bool  isMark = index.data(ChannelAlarmOnOff).toUInt() != 0;

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
            if( !isMark){
                drawAlarmElement(painter, option,alarm1Rect,alarmStatus, alarmType);
            }else{
                alarmColor = index.data(ChannelAlarm1Color).value<QColor>();
                alarmVal = index.data(ChannelAlarm1Value).toReal();
                markPoints[0].setY(bottomRect.bottom() - bottomRect.height()*((alarmVal - m_rangeDowner)/(m_rangeUpper - m_rangeDowner)));
                drawAlarmElement(painter, option,alarm1Rect,alarmStatus, alarmType, markType, alarmColor, markPoints);
            }
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
            if( !isMark){
                drawAlarmElement(painter, option,alarm2Rect,alarmStatus, alarmType);
            }else{
                alarmColor = index.data(ChannelAlarm2Color).value<QColor>();
                alarmVal = index.data(ChannelAlarm2Value).toReal();
                markPoints[0].setY(bottomRect.bottom() - bottomRect.height()*((alarmVal - m_rangeDowner)/(m_rangeUpper - m_rangeDowner)));
                drawAlarmElement(painter, option,alarm2Rect,alarmStatus, alarmType, markType, alarmColor, markPoints);
            }
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
            if( !isMark){
                drawAlarmElement(painter, option,alarm3Rect,alarmStatus, alarmType);
            }else{
                alarmColor = index.data(ChannelAlarm3Color).value<QColor>();
                alarmVal = index.data(ChannelAlarm3Value).toReal();
                markPoints[0].setY(bottomRect.bottom() - bottomRect.height()*((alarmVal - m_rangeDowner)/(m_rangeUpper - m_rangeDowner)));
                drawAlarmElement(painter,option, alarm3Rect,alarmStatus, alarmType, markType, alarmColor, markPoints);
            }
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
            if( !isMark){
                drawAlarmElement(painter, option,alarm4Rect,alarmStatus, alarmType);
            }else{
                alarmColor = index.data(ChannelAlarm4Color).value<QColor>();
                alarmVal = index.data(ChannelAlarm4Value).toReal();
                markPoints[0].setY(bottomRect.bottom() - bottomRect.height()*((alarmVal - m_rangeDowner)/(m_rangeUpper - m_rangeDowner)));
                drawAlarmElement(painter,option, alarm4Rect,alarmStatus, alarmType, markType, alarmColor, markPoints);
            }
        }
    }
}

/**
 * 功能：
 *          绘制报警标记子元素（带报警点）
 */
void WinBarDelegate::drawAlarmElement(QPainter *painter, const QStyleOptionViewItem &option, QRectF &alarmRect,  uint alarmStatus, uint alarmType, uint markType, QColor &alarmColor, QPointF *markPoints) const
{
    QString tipChar = "";
    switch (alarmType) {
    case Alarm_Type_H:
        tipChar = "H";
        markPoints[1].setX(markPoints[0].x()+MARK_WIDTH);
        markPoints[2].setX(markPoints[0].x()+MARK_WIDTH);
        markPoints[1].setY(markPoints[0].y());
        markPoints[2].setY(markPoints[0].y()-MARK_HEIGHT);
        break;
    case Alarm_Type_L:
        tipChar = "L";
        markPoints[1].setX(markPoints[0].x()+MARK_WIDTH);
        markPoints[2].setX(markPoints[0].x()+MARK_WIDTH);
        markPoints[1].setY(markPoints[0].y());
        markPoints[2].setY(markPoints[0].y()+MARK_HEIGHT);
        break;
    case Alarm_Type_R:
        tipChar = "R";
        markPoints[1].setX(markPoints[0].x()+MARK_WIDTH);
        markPoints[2].setX(markPoints[0].x()+MARK_WIDTH);
        markPoints[1].setY(markPoints[0].y());
        markPoints[2].setY(markPoints[0].y()-MARK_HEIGHT);
        break;
    case Alarm_Type_r:
        tipChar = "r";
        markPoints[1].setX(markPoints[0].x()+MARK_WIDTH);
        markPoints[2].setX(markPoints[0].x()+MARK_WIDTH);
        markPoints[1].setY(markPoints[0].y());
        markPoints[2].setY(markPoints[0].y()+MARK_HEIGHT);
        break;
    case Alarm_Type_T:
        tipChar = "T";
        markPoints[1].setX(markPoints[0].x()+MARK_WIDTH);
        markPoints[2].setX(markPoints[0].x()+MARK_WIDTH);
        markPoints[1].setY(markPoints[0].y());
        markPoints[2].setY(markPoints[0].y()-MARK_HEIGHT);
        break;
    case Alarm_Type_t:
        tipChar = "t";
        markPoints[1].setX(markPoints[0].x()+MARK_WIDTH);
        markPoints[2].setX(markPoints[0].x()+MARK_WIDTH);
        markPoints[1].setY(markPoints[0].y());
        markPoints[2].setY(markPoints[0].y()+MARK_HEIGHT);
        break;
    case Alarm_Type_h:
        tipChar = "h";
        markPoints[1].setX(markPoints[0].x()+MARK_WIDTH);
        markPoints[2].setX(markPoints[0].x()+MARK_WIDTH);
        markPoints[1].setY(markPoints[0].y());
        markPoints[2].setY(markPoints[0].y()-MARK_HEIGHT);
        break;
    case Alarm_Type_l:
        tipChar = "l";
        markPoints[1].setX(markPoints[0].x()+MARK_WIDTH);
        markPoints[2].setX(markPoints[0].x()+MARK_WIDTH);
        markPoints[1].setY(markPoints[0].y());
        markPoints[2].setY(markPoints[0].y()+MARK_HEIGHT);
        break;
    default:
        return;
    }

    QColor tipColor, markColor;
    switch(alarmStatus){
    case SQL_Alarm_Status_NonHold_Occur:
        tipColor = QColor(Qt::red);
        markColor = QColor(Qt::red);
        m_alert = true;
        break;
    case SQL_Alarm_Status_NonHold_Release:
        tipColor = QColor(0,0,0,0);
        markColor = QColor(Qt::green);
        break;
    case SQL_Alarm_Status_Hold_Occur_NoAck:
        if(option.state & QStyle::State_On){
            tipColor = QColor(Qt::red);
        }else{
            tipColor = QColor(Qt::darkRed);
        }
        markColor = QColor(Qt::red);
        m_alert  = true;
        break;
    case SQL_Alarm_Status_Hold_Occur_Ack:
        tipColor = QColor(Qt::red);
        markColor = QColor(Qt::red);
        m_alert  = true;
        break;
    case SQL_Alarm_Status_Hold_Release_NoAck:
        if(option.state & QStyle::State_On){
            tipColor = QColor(QColor(64,64,64));
        }else{
            tipColor = QColor(Qt::gray);
        }
        markColor = QColor(Qt::green);
        break;
    case SQL_Alarm_Status_Hold_Release_Ack:
        tipColor = QColor(0,0,0,0);
        markColor = QColor(Qt::green);
        break;
    default:
        return;
    }

    if(markType != 0){//类型Fixed
        //颜色用户设置
        markColor = alarmColor;
        //报警点为等腰三角形
        markPoints[1].setX(markPoints[0].x()+MARK_WIDTH);
        markPoints[2].setX(markPoints[0].x()+MARK_WIDTH);
        markPoints[1].setY(markPoints[0].y()-MARK_HEIGHT);
        markPoints[2].setY(markPoints[0].y()+MARK_HEIGHT);
    }

    painter->save();

    /****************报警标记*****************/
    painter->setBrush(QBrush(tipColor));
    if(alarmRect.width() >= WIN_MIN_ALARM_SIZE){
        QFont font = option.font;
        font.setPixelSize(alarmRect.height() - 4);
        painter->setFont(font);
        painter->setPen(Qt::transparent);
        painter->drawRoundedRect(alarmRect,3,3);
        painter->setPen(Qt::darkYellow);
        painter->drawText(alarmRect,  Qt::AlignCenter, tipChar);
    }else{
        painter->setPen(Qt::transparent);
        painter->drawRoundedRect(alarmRect,1,1);
    }

    /****************报警点标记*****************/
    painter->setPen(Qt::transparent);
    painter->setBrush(QBrush(markColor));
    painter->drawPolygon(markPoints,3);

    painter->restore();
}

/**
 * 功能：
 *          绘制报警标记子元素（无报警点）
 */
void WinBarDelegate::drawAlarmElement(QPainter *painter, const QStyleOptionViewItem &option, QRectF &alarmRect,  uint alarmStatus, uint alarmType) const
{
    QString tipChar = "";
    switch (alarmType) {
    case Alarm_Type_H:
        tipChar = "H";
        break;
    case Alarm_Type_L:
        tipChar = "L";
        break;
    case Alarm_Type_R:
        tipChar = "R";
        break;
    case Alarm_Type_r:
        tipChar = "r";
        break;
    case Alarm_Type_T:
        tipChar = "T";
        break;
    case Alarm_Type_t:
        tipChar = "t";
        break;
    case Alarm_Type_h:
        tipChar = "h";
        break;
    case Alarm_Type_l:
        tipChar = "l";
        break;
    default:
        return;
    }

    QColor tipColor;
    switch(alarmStatus){
    case SQL_Alarm_Status_NonHold_Occur:
        tipColor = QColor(Qt::red);
        m_alert = true;
        break;
    case SQL_Alarm_Status_NonHold_Release:
        tipColor = QColor(0,0,0,0);
        break;
    case SQL_Alarm_Status_Hold_Occur_NoAck:
        if(option.state & QStyle::State_On){
            tipColor = QColor(Qt::red);
        }else{
            tipColor = QColor(Qt::darkRed);
        }
        m_alert  = true;
        break;
    case SQL_Alarm_Status_Hold_Occur_Ack:
        tipColor = QColor(Qt::red);
        m_alert  = true;
        break;
    case SQL_Alarm_Status_Hold_Release_NoAck:
        if(option.state & QStyle::State_On){
            tipColor = QColor(QColor(64,64,64));
        }else{
            tipColor = QColor(Qt::gray);
        }
        break;
    case SQL_Alarm_Status_Hold_Release_Ack:
        tipColor = QColor(0,0,0,0);
        break;
    default:
        return;
    }

    painter->save();

    /****************报警标记*****************/
    painter->setBrush(QBrush(tipColor));
    if(alarmRect.width() >= WIN_MIN_ALARM_SIZE){
        QFont font = option.font;
        font.setPixelSize(alarmRect.height() - 4);
        painter->setFont(font);
        painter->setPen(Qt::transparent);
        painter->drawRoundedRect(alarmRect,3,3);
        painter->setPen(Qt::darkYellow);
        painter->drawText(alarmRect,  Qt::AlignCenter, tipChar);
    }else{
        painter->setPen(Qt::transparent);
        painter->drawRoundedRect(alarmRect,1,1);
    }

    painter->restore();
}

/**
 * 功能：
 *          绘制按钮按压效果
 */
void WinBarDelegate::drawPressingRect(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &/*index*/) const
{
    painter->save();

    /*********************按压效果*************************/
    if(option.state & QStyle::State_Sunken && option.state & QStyle::State_HasFocus){
        painter->setPen(QPen(QColor(0,0,255,100),3));
        painter->setBrush(QBrush(QColor(180,180,180,60)));
        painter->drawRoundedRect(option.rect,5,5);
    }

    painter->restore();
}

void WinBarDelegate::paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
    Q_ASSERT(index.isValid());
    if( index.data(ChannelValidity).isValid()){
        painter->save();
        painter->setRenderHint(QPainter::Antialiasing);

        QRectF valRect, topRect,bottomRect, tagRect, alarmRect;
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

        QStyleOptionViewItem opt = setOption(option,index);

        m_dot = index.data(ChannelDataAccuracy).toUInt();
        m_rangeUpper = index.data(ChannelRangeUpper).toReal();
        m_rangeDowner = index.data(ChannelRangeDowner).toReal();
        m_mesureVal = index.data(ChannelData).toReal();
        m_chanColor = index.data(ChannelColor).value<QColor>();

        doLayout(opt, topRect, bottomRect, tagRect, valRect, alarmRect, alarmSpan);
        drawBackground(painter, opt, index, topRect, bottomRect);
        if(m_enable){
            drawColorBarRect(painter,index,bottomRect);
            drawColorBandRect(painter,index,bottomRect);
            drawAlarmRect(painter,opt,index,bottomRect,alarmRect, alarmSpan);
            drawLightRect(painter,opt,topRect, bottomRect);
            drawScaleRect(painter,opt,index,bottomRect);
            drawValueRect(painter,opt, index,bottomRect, valRect,measureVal);
            drawTagRect(painter,opt,index,tagRect);
            drawPressingRect(painter,opt,index);
        }else{
            drawLightRect(painter,opt,topRect, bottomRect);
            drawTagRect(painter,opt,index,tagRect);
        }
        painter->restore();
    }
}

QSize WinBarDelegate::sizeHint(const QStyleOptionViewItem & option, const QModelIndex & /*index*/) const
{
    return option.rect.size();
}

/*
 * 功能：
 *      去掉系统自带的toolTip
 */
bool WinBarDelegate::helpEvent(QHelpEvent */*event*/,
                           QAbstractItemView */*view*/,
                           const QStyleOptionViewItem &/*option*/,
                           const QModelIndex &/*index*/)
{
    return false;
}


/*****************************************水平模式***********************************/
WinHBarDelegate::WinHBarDelegate(QObject *parent) :
    QAbstractItemDelegate(parent),m_margins(-6,-3,-6,-3)
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
QStyleOptionViewItem WinHBarDelegate::setOption(const QStyleOptionViewItem & option, const QModelIndex &/* index*/) const
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
 *          绘制棒图背景色
 */
void WinHBarDelegate::drawBackground(QPainter *painter, const QStyleOptionViewItem &option ,const QModelIndex &/*index*/) const
{
    painter->save();

    painter->setPen(QPen(option.palette.foreground().color(),1));
    painter->setBrush(option.backgroundBrush);
    painter->drawRect(option.rect);

    painter->restore();
}

/**
 * 功能：
 *          绘制光照效果
 */
void WinHBarDelegate::drawLightRect(QPainter *painter,const QStyleOptionViewItem &option) const
{
    painter->save();

    /****************光照效果*****************/
    QLinearGradient skinColor(option.rect.topLeft(),option.rect.bottomLeft());
//    skinColor.setColorAt(0,QColor(255,255,255,0));
//    skinColor.setColorAt(0.09,QColor(255,255,255,0));
//    skinColor.setColorAt(0.1,QColor(255,255,255,200));
//    skinColor.setColorAt(0.13,QColor(255,255,255,200));
//    skinColor.setColorAt(0.14,QColor(255,255,255,80));
//    skinColor.setColorAt(0.50,QColor(0,0,0,20));
//    skinColor.setColorAt(0.75,QColor(0,0,0,10));
//    skinColor.setColorAt(0.76,QColor(255,255,255,30));
//    skinColor.setColorAt(0.95,QColor(255,255,255,150));
//    skinColor.setColorAt(0.96,QColor(255,255,255,0));
//    skinColor.setColorAt(1,QColor(255,255,255,0));
    skinColor.setColorAt(0,QColor(255,255,255,170));
    skinColor.setColorAt(0.09,QColor(255,255,255,170));
    skinColor.setColorAt(0.10,QColor(255,255,255,50));
    skinColor.setColorAt(0.48,QColor(0,0,0,5));
    skinColor.setColorAt(0.49,QColor(100,100,100,20));
    skinColor.setColorAt(0.97,QColor(255,255,255,0));
    skinColor.setColorAt(0.98,QColor(255,255,255,50));
    skinColor.setColorAt(1,QColor(255,255,255,150));
    painter->setPen(QPen(QColor(200,230,255,0)));
    painter->setBrush(skinColor);
    painter->drawRect(option.rect);

    painter->restore();
}

/**
 * 功能：
 *          绘制棒图棒状色柱
 */
void WinHBarDelegate::drawColorBarRect(QPainter *painter,  const QModelIndex &index, const QRectF &leftRect) const
{
        /******************色柱********************/
        QRectF barRect(leftRect);
        quint8 base = index.data(ChannelBarBase).toUInt();

        if(m_rangeUpper <= m_rangeDowner){
            return;
        }else if(m_mesureVal > m_rangeUpper){
            m_mesureVal = m_rangeUpper;
        }else if(m_mesureVal < m_rangeDowner){
            m_mesureVal = m_rangeDowner;
        }

        qreal rate = (m_mesureVal - m_rangeDowner)/(m_rangeUpper - m_rangeDowner);
        barRect.setRight(leftRect.left() + (leftRect.width()*rate));

        if(base == 1){//中间
            barRect.setLeft(leftRect.center().x());
        }else if(base == 2){//右边
            barRect.setLeft(leftRect.right());
        }//默认左边

        painter->save();
        painter->setPen(Qt::transparent);
        painter->setBrush(m_chanColor);
        painter->drawRect(barRect);
        painter->restore();
}

/**
 * 功能：
 *          绘制棒图彩色标尺带
 */
void WinHBarDelegate::drawColorBandRect(QPainter *painter,const QModelIndex &index, const QRectF &leftRect) const
{
    QVariant posValue = index.data(ChannelColorScalePos);
    if(!posValue.isValid() || posValue.toUInt() == 0){
        return;
    }
    uint pos = posValue.toUInt();

    qreal upper = index.data(ChannelColorScaleUpper).toFloat();
    qreal downer = index.data(ChannelColorScaleDowner).toFloat();
    if(upper == downer){
        return;
    }else if(upper < downer){
        qSwap(upper,downer);
    }

    QColor color = index.data(ChannelColorScaleColor).value<QColor>();
    int height = leftRect.height()/10;
    if(height > 15){
        height = 15;
    }else if(height < 2){
        return;
    }

    QRectF bandRect(leftRect);
    bandRect.setTop(bandRect.bottom() - height);
    QRectF rect(bandRect);
    qreal upRate = (upper - m_rangeDowner)/(m_rangeUpper - m_rangeDowner);
    qreal downRate = (downer - m_rangeDowner)/(m_rangeUpper - m_rangeDowner);
    bandRect.setRight(leftRect.left() + leftRect.width()*upRate);
    bandRect.setLeft(leftRect.left() + leftRect.width()*downRate);

    painter->save();

    painter->setPen(Qt::transparent);
    painter->setBrush(QBrush(color));
    if(pos == 1){//内侧
        painter->drawRect(bandRect);
    }else{//外侧
        painter->drawRect(QRectF(rect.topLeft(), bandRect.bottomLeft()));
        painter->drawRect(QRectF(bandRect.topRight(), rect.bottomRight()));
    }

    painter->restore();
}
/**
 * 功能：
 *          绘制棒图刻度
 */
void WinHBarDelegate::drawScaleRect(QPainter *painter,const QStyleOptionViewItem &option, const QModelIndex &index) const
{
        /*****************刻度*********************/
        QRectF leftRect = option.rect;
        qreal len = leftRect.height()/10;
        QVariant value = index.data(ChannelBarDiv);
        if(value.isValid()){
            uint div = value.toUInt();
            uint center = div%2?0:div/2;
            qreal span = leftRect.width()/div;
            qreal baseX = leftRect.left();
            qreal baseY = leftRect.bottom();


            painter->save();
            painter->setPen(QColor(64,64,64));
            painter->setBrush(Qt::transparent);

            for(uint i =1; i<div; ++i){
                if(i == center){
                    painter->drawLine(baseX + i*span , baseY  - 1.5*len, baseX + i*span, baseY);  //中线加长
                }else{
                    painter->drawLine(baseX + i*span, baseY - len, baseX + i*span, baseY);
                }
            }

            painter->restore();
        }

        /*****************量程*********************/
        int pixelSize = (leftRect.height() - len)/3;
        if(pixelSize > WIN_MAX_FONT_SIZE){
            pixelSize = WIN_MAX_FONT_SIZE;
        }else{
            if(pixelSize < WIN_MIN_FONT_SIZE){
                pixelSize = (leftRect.height() - len)/2;
            }

            if(pixelSize < WIN_MIN_FONT_SIZE){
                pixelSize = leftRect.height() - len;
            }
        }

        if(pixelSize >= WIN_MIN_FONT_SIZE){
            QFont font = option.font;
            font.setPixelSize(pixelSize);

            painter->save();
            painter->setFont(font);
            painter->setPen(option.palette.foreground().color());
            painter->drawText(leftRect,QString::number(m_rangeDowner,'f',m_dot),Qt::AlignTop|Qt::AlignLeft);
            painter->drawText(leftRect,QString::number(m_rangeUpper,'f',m_dot),Qt::AlignTop|Qt::AlignRight);    
            painter->restore();
        }
}

/**
 * 功能：
 *          绘制棒图报警标记
 */
void WinHBarDelegate::drawMarkRect(QPainter *painter,const QModelIndex &index, const QRectF &leftRect) const
{
    QVariant value1,value2,value3;
    QPointF markPoints[3];
    quint32 alarmType,alarmStatus;
    qreal alarmVal;
    QColor alarmColor;
    quint32 markType = index.data(ChannelAlarmType).toUInt();
    markPoints[0] = leftRect.bottomLeft();
    if(index.data(ChannelAlarmOnOff).toUInt() != 0){

        /*********************电平一*******************/
        value3 = index.data(ChannelAlarm1OnOff);
        if(value3.isValid() && value3.toUInt() != OFF){
            value1 = index.data(ChannelAlarm1Type);
            value2 = index.data(ChannelAlarm1Status);
            if(value1.isValid() && value2.isValid()){
                alarmType = value1.toUInt();
                alarmStatus = value2.toUInt();
                alarmColor = index.data(ChannelAlarm1Color).value<QColor>();
                alarmVal = index.data(ChannelAlarm1Value).toReal();
                markPoints[0].setX(leftRect.left() +  leftRect.width()*((alarmVal - m_rangeDowner)/(m_rangeUpper - m_rangeDowner)));
                drawMarkElement(painter, alarmStatus, alarmType, markType, markPoints,alarmColor);
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
                alarmColor = index.data(ChannelAlarm2Color).value<QColor>();
                alarmVal = index.data(ChannelAlarm2Value).toReal();
                markPoints[0].setX(leftRect.left() +  leftRect.width()*((alarmVal - m_rangeDowner)/(m_rangeUpper - m_rangeDowner)));
                drawMarkElement(painter, alarmStatus, alarmType, markType, markPoints,alarmColor);
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
                alarmColor = index.data(ChannelAlarm3Color).value<QColor>();
                alarmVal = index.data(ChannelAlarm3Value).toReal();
                markPoints[0].setX(leftRect.left() + leftRect.width()*((alarmVal - m_rangeDowner)/(m_rangeUpper - m_rangeDowner)));
                drawMarkElement(painter, alarmStatus, alarmType, markType, markPoints,alarmColor);
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
                alarmColor = index.data(ChannelAlarm4Color).value<QColor>();
                alarmVal = index.data(ChannelAlarm4Value).toReal();
                markPoints[0].setX(leftRect.left() + leftRect.width()*((alarmVal - m_rangeDowner)/(m_rangeUpper - m_rangeDowner)));
                drawMarkElement(painter, alarmStatus, alarmType, markType, markPoints,alarmColor);
            }
        }
    }
}
/**
 * 功能：
 *          绘制报警标记子元素（带报警点）
 */
void WinHBarDelegate::drawMarkElement(QPainter *painter, uint alarmStatus, uint alarmType, uint markType, QPointF *markPoints, QColor &alarmColor) const
{
    painter->save();
    if(markType == 1){//类型Fixed
        markPoints[1].setX(markPoints[0].x()+MARK_WIDTH);
        markPoints[2].setX(markPoints[0].x()-MARK_WIDTH);
        markPoints[1].setY(markPoints[0].y()+MARK_HEIGHT);
        markPoints[2].setY(markPoints[0].y()+MARK_HEIGHT);
        painter->setBrush(QBrush(alarmColor));
    }else{
        if(alarmType == Alarm_Type_H || alarmType == Alarm_Type_R ||alarmType == Alarm_Type_T ||alarmType == Alarm_Type_h ){
            markPoints[1].setX(markPoints[0].x()-MARK_WIDTH);
            markPoints[2].setX(markPoints[0].x());
            markPoints[1].setY(markPoints[0].y()+MARK_HEIGHT);
            markPoints[2].setY(markPoints[0].y()+MARK_HEIGHT);
        }else{
            markPoints[1].setX(markPoints[0].x()+MARK_WIDTH);
            markPoints[2].setX(markPoints[0].x());
            markPoints[1].setY(markPoints[0].y()+MARK_HEIGHT);
            markPoints[2].setY(markPoints[0].y()+MARK_HEIGHT);
        }

        if(alarmStatus == SQL_Alarm_Status_NonHold_Occur || alarmStatus == SQL_Alarm_Status_Hold_Occur_NoAck|| alarmStatus == SQL_Alarm_Status_Hold_Occur_Ack){
            painter->setBrush(QBrush(QColor(Qt::red)));
        }else{
            painter->setBrush(QBrush(QColor(Qt::green)));
        }
    }

    /****************报警点标记*****************/
    painter->setPen(Qt::transparent);
    painter->drawPolygon(markPoints,3);

    painter->restore();
}

/**
 * 功能：
 *          绘制按钮按压效果
 */
void WinHBarDelegate::drawPressingRect(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &/*index*/) const
{
    painter->save();

    /*********************按压效果*************************/
    if(option.state & QStyle::State_Sunken && option.state & QStyle::State_HasFocus){
        painter->setPen(QPen(QColor(0,0,255,100),3));
        painter->setBrush(QBrush(QColor(180,180,180,60)));
        painter->drawRoundedRect(option.rect,5,5);
    }

    painter->restore();
}

void WinHBarDelegate::paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
    Q_ASSERT(index.isValid());
    if( index.data(ChannelValidity).isValid()){
        painter->save();
        painter->setRenderHint(QPainter::Antialiasing);

        m_alert = false;
        m_enable = true;
        if(index.data(ChannelType).toUInt() != Channel_Type_DO){
            if(index.data(ChannelRangeType).toUInt() == OFF){
                m_enable = false;
            }
        }

        QStyleOptionViewItem opt = setOption(option,index);
        m_dot = index.data(ChannelDataAccuracy).toUInt();
        m_rangeUpper = index.data(ChannelRangeUpper).toReal();
        m_rangeDowner = index.data(ChannelRangeDowner).toReal();
        m_mesureVal = index.data(ChannelData).toReal();
        m_chanColor = index.data(ChannelColor).value<QColor>();
        drawBackground(painter, opt, index);
        if(m_enable){
            drawColorBarRect(painter,index,opt.rect);
            drawColorBandRect(painter,index,opt.rect);
            drawLightRect(painter,opt);
            drawScaleRect(painter,opt,index);
            drawMarkRect(painter,index,opt.rect);
            drawPressingRect(painter,opt,index);
        }else{
            drawLightRect(painter,opt);
        }

        painter->restore();
    }
}

QSize WinHBarDelegate::sizeHint(const QStyleOptionViewItem & option, const QModelIndex & /*index*/) const
{
    return option.rect.size();
}

/*
 * 功能：
 *      去掉系统自带的toolTip
 */
bool WinHBarDelegate::helpEvent(QHelpEvent */*event*/,
                           QAbstractItemView */*view*/,
                           const QStyleOptionViewItem &/*option*/,
                           const QModelIndex &/*index*/)
{
    return false;
}
