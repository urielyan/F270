#include <QPainter>
#include <QDebug>
#include "winoverviewchandelegate.h"
#include "./GlobalData/cfgGlobalDef.h"

WinOverviewChanDelegate::WinOverviewChanDelegate(QObject *parent) :
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
QStyleOptionViewItem WinOverviewChanDelegate::setOptions(const QStyleOptionViewItem &option,const QModelIndex &/*index*/) const
{
    QStyleOptionViewItem opt = option;
    opt.rect = option.rect.marginsAdded(m_margins); //边框缩进

    if(m_enable && option.state & QStyle::State_Sunken && option.state & QStyle::State_HasFocus){
        opt.rect.moveTo(opt.rect.x()+1, opt.rect.y()+1);
    }
    return opt;
}

/**
 * 功能
 *      当前单元格子元素布局
 */
void WinOverviewChanDelegate::doLayout(const QStyleOptionViewItem &option, QRectF &valRect, QRectF &unitRect, QRectF &tagRect) const
{
    valRect = option.rect;
    valRect.setTop(valRect.top()+valRect.height()/3);

    unitRect = valRect;
    unitRect.setRight(unitRect.right() - 10);
    unitRect.setTop(unitRect.center().y());

    tagRect = option.rect;
    tagRect.setRight(tagRect.right() - 10);
    tagRect.setHeight(tagRect.height()/3);

}

/**
 * 功能：
 *         绘制单元格背景
 */
void WinOverviewChanDelegate::drawBackground(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &/*index*/) const
{
    painter->save();
    /**********************背景*********************/
    QRectF rect(option.rect);
    painter->setPen(Qt::transparent);
    painter->setBrush(QBrush(getBackgroundColor(option,m_alarmStatus)));

    painter->drawRoundedRect(rect,5,5);

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
    painter->drawRoundedRect(rect, 5, 5);
    painter->restore();
}

/**
 * 功能：
 *      初始化报警参数
 */
void WinOverviewChanDelegate::initAlarmParamater(const QModelIndex &index) const
{
    m_alarmStatus = QVariant();
    m_alarmType = QVariant();
    quint32 value = SQL_Alarm_Status_Hold_Release_Ack;

    /*************************电平一*************************/
    QVariant alarmOnOff = index.data(ChannelAlarm1OnOff);
    if(alarmOnOff.isValid() && alarmOnOff.toUInt() != OFF){
        QVariant alarmStatusValue = index.data(ChannelAlarm1Status);
        QVariant alarmTypeValue = index.data(ChannelAlarm1Type);
        if(alarmTypeValue.isValid() && alarmTypeValue.isValid() && value >= alarmStatusValue.toUInt()){
            value = alarmStatusValue.toUInt();
            m_alarmStatus = alarmStatusValue;
            m_alarmType = alarmTypeValue;
        }
    }

    /*************************电平二*************************/
    alarmOnOff = index.data(ChannelAlarm2OnOff);
    if(alarmOnOff.isValid() && alarmOnOff.toUInt() != OFF){
        QVariant alarmStatusValue = index.data(ChannelAlarm2Status);
        QVariant alarmTypeValue = index.data(ChannelAlarm2Type);
        if(alarmTypeValue.isValid() && alarmTypeValue.isValid() && value >= alarmStatusValue.toUInt()){
            value = alarmStatusValue.toUInt();
            m_alarmStatus = alarmStatusValue;
            m_alarmType = alarmTypeValue;
        }
    }

    /*************************电平三*************************/
    alarmOnOff = index.data(ChannelAlarm3OnOff);
    if(alarmOnOff.isValid() && alarmOnOff.toUInt() != OFF){
        QVariant alarmStatusValue = index.data(ChannelAlarm3Status);
        QVariant alarmTypeValue = index.data(ChannelAlarm3Type);
        if(alarmTypeValue.isValid() && alarmTypeValue.isValid() && value >= alarmStatusValue.toUInt()){
            value = alarmStatusValue.toUInt();
            m_alarmStatus = alarmStatusValue;
            m_alarmType = alarmTypeValue;
        }
    }

     /*************************电平四*************************/
    alarmOnOff = index.data(ChannelAlarm4OnOff);
    if(alarmOnOff.isValid() && alarmOnOff.toUInt() != OFF){
        QVariant alarmStatusValue = index.data(ChannelAlarm4Status);
        QVariant alarmTypeValue = index.data(ChannelAlarm4Type);
        if(alarmTypeValue.isValid() && alarmTypeValue.isValid() && value >= alarmStatusValue.toUInt()){
            value = alarmStatusValue.toUInt();
            m_alarmStatus = alarmStatusValue;
            m_alarmType = alarmTypeValue;
        }
    }

}

/**
 * 功能：
 *      绘制通道标记
 */
void WinOverviewChanDelegate::drawTagRect(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index, const QRectF &tagRect) const
{
    qreal pixelSize = tagRect.height()/2;
    QVariant value = index.data(ChannelTag);
    if(pixelSize > WIN_MIN_FONT_SIZE && value.isValid()){
            painter->save();
            QFont font = option.font;
            font.setPixelSize(pixelSize);
            painter->setFont(font);
            painter->setPen(Qt::darkGray);
            painter->drawText(tagRect, Qt::AlignRight | Qt::AlignVCenter, value.toString());

            painter->restore();
    }
}

/**
 * 功能：
 *      绘制通道测量值单位
 */
void WinOverviewChanDelegate::drawUnitRect(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index, const QRectF &unitRect) const
{
    qreal pixelSize = unitRect.height()/3*2;
    QVariant value = index.data(ChannelUnit);
    if(pixelSize > WIN_MIN_FONT_SIZE && value.isValid()){
            painter->save();

            QFont font = option.font;
            font.setPixelSize(pixelSize);
            painter->setFont(font);
            painter->setPen(option.palette.foreground().color());
            painter->drawText(unitRect, Qt::AlignRight | Qt::AlignVCenter, value.toString());

            painter->restore();
    }
}

/**
 * 功能：
 *      绘制通道测量值
 */
void WinOverviewChanDelegate::drawMesureValRect(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index, const QRectF &valRect,QString measureVal) const
{
    QString text = measureVal;
    QColor pan = option.palette.foreground().color();
    if(text.isEmpty()){
        QVariant value = getChannelString(m_alarmType,index);
        if(value.isValid()){
            text = value.toString();
            if(m_alarmStatus.isValid() && (m_alarmStatus.toUInt() == SQL_Alarm_Status_Hold_Occur_Ack||m_alarmStatus.toUInt() == SQL_Alarm_Status_NonHold_Occur)){ //报警状态显示红色字体
                pan = QColor(Qt::red);
            }
        }else{
            return;
        }
    }

    QFont font = option.font;
    int pix = valRect.height()/3*2;
    if(pix <= 0)
        pix = 2;
    font.setPixelSize(pix);

    painter->save();
    painter->setFont(font);
    painter->setPen(pan);
    painter->drawText(valRect, Qt::AlignLeft | Qt::AlignVCenter,text);
    painter->restore();
}


/**
 * 功能：
 *      绘制按钮点击效果
 */
void WinOverviewChanDelegate::drawPressingRect(QPainter *painter,const QStyleOptionViewItem &option, const QModelIndex &/*index*/) const
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

/**
 * 功能：
 *      获取通道背景色
 */
QColor WinOverviewChanDelegate::getBackgroundColor(const QStyleOptionViewItem &option,QVariant alarmStatus) const
{
    QColor color = option.backgroundBrush.color();
    if(m_enable && alarmStatus.isValid()){
        switch(alarmStatus.toUInt()){
        case SQL_Alarm_Status_NonHold_Occur://非保持&发生
            color = QColor(Qt::red);
            break;
        case SQL_Alarm_Status_NonHold_Release://非保持&释放
            break;
        case SQL_Alarm_Status_Hold_Occur_NoAck://保持&发生&未解除
            if(option.state & QStyle::State_On ){
                color = QColor(Qt::red);
            }else{
                color = QColor(Qt::darkRed);
            }
            break;
        case SQL_Alarm_Status_Hold_Occur_Ack://保持&发生&已解除
            color = QColor(Qt::red);
            break;
        case SQL_Alarm_Status_Hold_Release_NoAck://保持&释放&未解除
            if(option.state & QStyle::State_On){
                color = QColor(QColor(64,64,64));
            }else{
                color = QColor(Qt::gray);
            }
            break;
        case SQL_Alarm_Status_Hold_Release_Ack://保持&释放&已解除
            break;
        default:
            break;
        }
    }
    return color;
}

/**
 * 功能：
 *      获取显示字符
 */
QVariant WinOverviewChanDelegate::getChannelString(QVariant alarmType,const QModelIndex &index) const
{
    QString text;
    QVariant chanStatus = index.data(ChannelStatus);
    if(!m_enable || !chanStatus.isValid())
        return QVariant();
    if(alarmType.isValid()){
        switch (alarmType.toUInt()) {
        case Alarm_Type_H://上限
            text =  tr("H");
            break;
        case Alarm_Type_h://差值上限， Delta运算时有效
            text =  tr("h");
            break;
        case Alarm_Type_L://下限
            text =  tr("L");
            break;
        case Alarm_Type_l://差值下限， Delta运算时有效
            text =  tr("l");
            break;
        case Alarm_Type_R://变化率上升限
            text =  tr("R");
            break;
        case Alarm_Type_r://变化率下升限
            text =  tr("r");
            break;
        case Alarm_Type_T://延迟上限
           text =  tr("T");
           break;
        case Alarm_Type_t://延迟下限
            text =  tr("t");
            break;
        default:
            break;
        }
    }

    if(!text.isEmpty()){
        text.append(" ");
    }

    switch (chanStatus.toUInt()) {
    case DATA_ERROR_OVER_POSITIVE:
        text += tr("+Over");
        break;
    case DATA_ERROR_OVER_NEGATIVE:
        text += tr("-Over");
        break;
    case DATA_ERROR_BURNOUT_POSITIVE:
    case DATA_ERROR_BURNOUT_NEGATIVE :
        text += tr("Burnout");
        break;
    case DATA_ERROR_NORMAL :
        text += QString::number(index.data(ChannelData).toReal(),'f',index.data(ChannelDataAccuracy).toUInt());
        break;
    default:
        return QVariant();
    }
    return text;
}

void	WinOverviewChanDelegate::paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
    Q_ASSERT(index.isValid());
    if( index.data(ChannelValidity).isValid()){
        painter->save();
        painter->setRenderHint(QPainter::Antialiasing);

        QRectF valRect, unitRect, tagRect;
        QString measureVal;
        m_enable = true;
        if(index.data(ChannelType).toUInt() != Channel_Type_DO){
            if(index.data(ChannelRangeType).toUInt() == OFF){
                m_enable = false;
            }else{
                initAlarmParamater(index);
            }
        }else{
            measureVal = index.data(ChannelDataAccuracy).toString().split('|').at(index.data(ChannelData).toUInt());
        }

        QStyleOptionViewItem opt = setOptions(option,index);

        doLayout(opt,valRect,unitRect,tagRect);
        drawBackground(painter, opt,index);
        drawTagRect(painter, opt,index,tagRect);
        if(m_enable){
            drawMesureValRect(painter, opt, index, valRect,measureVal);
            drawUnitRect(painter,opt,index,unitRect);
            drawPressingRect(painter, opt, index);
        }
        painter->restore();
    }
}


QSize	WinOverviewChanDelegate::sizeHint(const QStyleOptionViewItem & option, const QModelIndex & /*index*/) const
{
    return option.rect.size();
}

/*
 * 功能：
 *          去掉系统自带的toolTip
 */
bool WinOverviewChanDelegate::helpEvent(QHelpEvent */*event*/,
                           QAbstractItemView */*view*/,
                           const QStyleOptionViewItem &/*option*/,
                           const QModelIndex &/*index*/)
{
    return false;
}
