#include "trendrecordpaper.h"
#include <QModelIndex>
#include <QDateTime>
#include <QString>
#include <QPair>
#include <QVector3D>

#include "../Threads/threaddatasync.h"

/*
 * 功能：
 * 	 详细趋势绘制窗体构造函数
 * 参数：
 *       quint32 groupIndex 组号
 * 	  QWidget *parent
 *       int index
 * 返回值：
 *       无
 */
TrendRecordPaper::TrendRecordPaper(quint32 groupIndex, QWidget *parent) :
    QWidget(parent),m_groupIndex(groupIndex)
{
    m_diff = 0;
    m_pixelGap = 1;
    p_paintParam = p_gCfgTrendGroup->getRealTimeTrendBuf();
}

/*
 * 功能：
 * 	 详细趋势绘制窗体析构函数
 * 参数：
 *       无
 * 返回值：
 *       无
 */
TrendRecordPaper::~TrendRecordPaper()
{
}

/*
 * 功能：
 * 	 设置组号
 * 参数：
 *       quint32 groupIndex 组号
 * 返回值：
 *       无
 */
void TrendRecordPaper::setGroupIndex(quint32 groupIndex)
{
    m_groupIndex = groupIndex;
}

/*
 * 功能：
 * 	 处理字符信息
 * 参数：
 *       QString &msg  字符信息
 * 返回值：
 *       无
 */
void TrendRecordPaper::dealCharMsg(QString &msg, quint8 msgGrp)
{
    if (p_gCfgTrendGroup->getCurrentRTState() != Running)
        return;

    if (msg.isEmpty()) return;

    //取消息
    QString charMsg = msg;
    int index = p_paintParam->getBufValidLen()-1;

    //写缓存
    QString msgIndex(charMsg.at(0));
    charMsg.remove(0, 1);
    TREND_POINT point = p_paintParam->getDataBuf().value(index);
    QPair<quint8, QString> pair(msgGrp, charMsg);
    point.msgInfor.insert(msgIndex.toUInt(), pair);
    p_paintParam->getDataBuf().replace(index, point);

    //写数据库
    QByteArray charData( msg.toLatin1());
    MSG_DATA  msgData;
    msgData.msgGrp = msgGrp;
    msgData.msgSize = charData.length();
    msgData.pMsgData = charData.data();
    msgData.msgType = Message_Type_Normal;
    msgData.msgTime = p_paintParam->getDataBuf().at(index).timePoint;

    ThreadDataRecord::instance()->newMessageData(&msgData);
}

void TrendRecordPaper::dealHWMsg(QImage *image, QString &imageDir, quint8 msgGr, QPoint &rightBottom)
{
    if (p_gCfgTrendGroup->getCurrentRTState() != Running)
        return;
    //考虑到需要在趋势刚开始绘制时来记录手写信息，
    //缓存中记录索引号应为图片的右下角对应的缓存索引号。
    int paintPointNumGapToRight;
    if (DevConfigPtr->displayTrend.trendDisplayDirection)
        paintPointNumGapToRight = SCREEN_DATA_COUNT_MAX - rightBottom.y()/ m_pixelGap;
    else
        paintPointNumGapToRight = SCREEN_DATA_COUNT_MAX - rightBottom.x()/ m_pixelGap;

    int rightPointNum = p_paintParam->getBufValidLen()-1;
    if (rightPointNum < paintPointNumGapToRight)
        return;

    //取消息
    QImage *hwImage = new QImage();
    *hwImage = *image;
    int hwIndex = rightPointNum - paintPointNumGapToRight;
    //写缓存
    QString dir(imageDir);
    dir.remove(0, dir.indexOf('-')+1);
    HAND_MSG handMsg;
    handMsg.imageYPos = dir.toDouble();
    handMsg.msgGroupInfor = msgGr;
    handMsg.msgImage = hwImage;
    QImage *pImage = p_gCfgTrendGroup->getRealTimeTrendBuf()->getMssgBuf().value(hwIndex).msgImage;
    if (pImage != nullptr) {
        delete pImage;
    }
    p_gCfgTrendGroup->getRealTimeTrendBuf()->getMssgBuf().replace(hwIndex, handMsg);

    //写数据库
    QByteArray charData(imageDir.toLatin1());
    MSG_DATA  msgData;
    msgData.msgGrp = msgGr;
    msgData.msgSize = charData.length();
    msgData.pMsgData = charData.data();
    msgData.msgType = Message_Type_Freehand;
    msgData.msgTime = p_paintParam->getDataBuf().at(hwIndex).timePoint;
    ThreadDataRecord::instance()->newMessageData(&msgData);
}


/*
 * 功能：
 * 	 重写绘制函数 绘制网格背景 和趋势图
 * 参数：
 *       QPaintEvent *e
 * 返回值：
 *       无
 */
void TrendRecordPaper::paintEvent(QPaintEvent *e)
{
    TrendWorkMode mode = p_gCfgTrendGroup->getCurrentWorkMode(); //工作模式切换相关
    if (mode == RealTimeRecord){
        p_paintParam = p_gCfgTrendGroup->getRealTimeTrendBuf();
    } else {
        p_paintParam = p_gCfgTrendGroup->getHistoryTrendBuf();
    }

    if (DevConfigPtr->displayTrend.trendDisplayDirection) {
        m_pixelGap = (qreal)this->rect().height()/(qreal)SCREEN_DATA_COUNT_MAX;
    } else {
        m_pixelGap = (qreal)this->rect().width()/(qreal)SCREEN_DATA_COUNT_MAX;
    }

    //绘制手写信息图片
    p_paintParam->getDataLock();
    paintBackGround();
    paintTimeInfor();
    paintTrend();
    p_paintParam->putDataLock();

    QWidget::paintEvent(e);
}

void TrendRecordPaper::paintTimeInfor()
{
    QPainter painter(this);
    QFont font;
    font.setBold(true);
    font.setPixelSize(rect().height()/40);
    painter.setFont(font);
    painter.setPen(QColor(0x40, 0x40, 0x40));

    //时间DIV标志
    QString timeDiv;
    qint64 timeDiv_s = m_screenTimeDiv/1000;
    if (timeDiv_s <= Trend_Interval_30s) {
        timeDiv = QString("%1 Sec/Div").arg(timeDiv_s);
    } else if (timeDiv_s <= Trend_Interval_30min) {
        timeDiv = QString("%1 Min/Div").arg(timeDiv_s/60);
    } else {
        timeDiv = QString("%1 Hour/Div").arg(timeDiv_s/3600);
    }
    painter.drawText(QPointF(rect().width()*0.8,40.0), timeDiv);

    //当前时间
    if (p_gCfgTrendGroup->getCurrentWorkMode() == BrowseHistory){
        font.setPixelSize(rect().height()/28);
        painter.setFont(font);

        QString timePoint(QDateTime::fromMSecsSinceEpoch(m_rightPointTime).toString(QString("yyyy/MM/dd  hh:mm:ss:zzz")));
        painter.drawText(QPointF(0.0,40.0), timePoint);
    }
}

/*
 * 功能：
 * 	 实时模式背景网格绘制
 */
void TrendRecordPaper::paintBackGround()
{
    QPen backPen;
    backPen.setStyle(Qt::SolidLine);
    backPen.setColor(Qt::black);
    //参数设置
    int width = rect().width();
    int height = rect().height();
    QPainter painter(this);

    if (DevConfigPtr->displayTrend.trendDisplayDirection) {
        painter.translate(QPointF(0,rect().height()));
        painter.rotate(-90);
        width = rect().height();
        height =  rect().width();
    }
    painter.setPen(backPen);
    //背景绘制
    if (DevConfigPtr->displayBasic.basicBackground) {
        painter.fillRect(0, 0, width, height, QBrush(Qt::black));
    } else {
        painter.fillRect(0, 0, width, height, QBrush(Qt::white));
    }

    painter.setPen(QColor(0x40, 0x40, 0x40));

    //水平网格线
    quint8 screenVerDiv;

    if (DevConfigPtr->displayTrend.trendGrid == 0) {
        quint32 currentNum = p_gCfgTrendGroup->getCurrentChanNo();
        if (p_paintParam->getRulerPaintItem()->keys().contains(currentNum)) {
            screenVerDiv = p_paintParam->getRulerPaintItem()->value(currentNum).displayScaleDiv;
        } else if (p_paintParam->getRulerPaintItem()->isEmpty()) {
            screenVerDiv = DevConfigPtr->displayTrend.trendGrid;
        } else {
            screenVerDiv = p_paintParam->getRulerPaintItem()->first().displayScaleDiv;
        }
    } else {
        screenVerDiv = DevConfigPtr->displayTrend.trendGrid;
    }

    if (screenVerDiv<4 || screenVerDiv>12)
        screenVerDiv = SCREEN_VERTICAL_DIV;

    for (int i=0; i<=screenVerDiv; i++){
        painter.drawLine(QPoint(0, TOP_SPACE+((height-TOP_SPACE-BOTTOM_SPACE)*i/screenVerDiv)),
                                      QPoint(width, TOP_SPACE+((height-TOP_SPACE-BOTTOM_SPACE)*i/screenVerDiv)));
    }

    //垂直网格线
    qint64 timeDiff;
    qreal vlinsDiff, vlinsGap;

    m_screenTimeDiv = (qint64)p_paintParam->getTrendPeriodTarget() * 1000;
    m_screenDiv = p_paintParam->getTrendPeriodTarget()>Trend_Interval_10s ? 20 : 12;
    m_recordStartTime = p_paintParam->getRecordStartTime();  //记录开始时间

    vlinsGap = (qreal)width /m_screenDiv;    //水平像素每格
    if (!p_paintParam->isEmpty()) {


        if (p_gCfgTrendGroup->getCurrentWorkMode() == RealTimeRecord) {
            m_rightIndex = p_paintParam->getBufValidLen() - 1;
        } else {
            m_rightIndex = p_paintParam->getRecordPointIndex();
        }
        m_trendPeriodMin = p_paintParam->getTrendPeriodMin();
        m_trendPeriodMax = p_paintParam->getTrendPeriodMax();
        m_trendPeriodTarget = p_paintParam->getTrendPeriodTarget();
        if (m_trendPeriodTarget != m_trendPeriodMin) {
            m_scale = (qreal)m_trendPeriodMax/(qreal)m_trendPeriodMin;
            m_targetIndex = m_rightIndex * m_trendPeriodMin / m_trendPeriodMax;
        } else {
            m_scale = 1.0;
            m_targetIndex = m_rightIndex;
        }
        m_rightPointTime = p_paintParam->getDataBuf().at((int)(m_targetIndex*m_scale)).timePoint;

        timeDiff = (m_rightPointTime-m_recordStartTime) % m_screenTimeDiv;
        vlinsDiff = -(qreal)timeDiff * vlinsGap/(qreal)m_screenTimeDiv;
    } else {
        vlinsDiff = 0;
    }

    for (qint32 i=0; i<(m_screenDiv+1); ++i) {
        qreal xPosition = vlinsDiff+i*vlinsGap;
        painter.drawLine(QPointF(xPosition, TOP_SPACE), QPointF(xPosition, height-BOTTOM_SPACE));
    }

    //组分割线
    for (int i=0; i<4; ++i) {
        if (DevConfigPtr->displayGroup[m_groupIndex].groupTripLineSwitch[i]) {
            QPen tripLinePen;
            tripLinePen.setStyle(Qt::DashLine);
            switch (DevConfigPtr->displayGroup[m_groupIndex].groupTripLineWidth[i]) {
            case 1:
                tripLinePen.setWidth(4);
                break;
            case 2:
                tripLinePen.setWidth(6);
                break;
            default:
                tripLinePen.setWidth(2);
                break;
            }
            int red = (DevConfigPtr->displayGroup[m_groupIndex].groupTripLineColor[i]>>16) & 0xFF;
            int green = (DevConfigPtr->displayGroup[m_groupIndex].groupTripLineColor[i]>>8) & 0xFF;
            int blue = (DevConfigPtr->displayGroup[m_groupIndex].groupTripLineColor[i]) & 0xFF;
            tripLinePen.setColor(QColor(red, green, blue));
            painter.setPen(tripLinePen);
            qreal posY = (height-TOP_SPACE-BOTTOM_SPACE) * (100 - DevConfigPtr->displayGroup[m_groupIndex].groupTripLinePos[i]) /100.0 + BOTTOM_SPACE;
            painter.drawLine(QPointF(0, posY), QPointF(width, posY));
        }
    }
}

/*
 * 功能：
 * 	 绘制实时趋势图
 */
void TrendRecordPaper::paintTrend()
{
    qint32 groupIndex = p_paintParam->getGroupIndex(m_groupIndex);
    if (groupIndex < 0)
        return;

    GROUP_INFO group = p_paintParam->getGroupInfor(groupIndex);
    quint32 chanNo;
    qreal rulerLen;
    qreal rulerEndPosY;//相对qt坐标系来分 start 和 end
    qreal rulerStartPosY;
    qreal rulerPartialEndPosY;  //开启压缩模式时的中间点
    QMap<quint32,QPointF> prePoint;  //保存前一个点的数值
    QVector3D data;


    QPainter painter(this);

    int height = rect().height();

    if (DevConfigPtr->displayTrend.trendDisplayDirection) {
        painter.translate(QPointF(0,rect().height()));
        painter.rotate(-90);
        height =  rect().width();
    }

    QPointF t_point1, t_point2;
    int paintCount = SCREEN_DATA_COUNT_MAX+1;   //一屏数据总个数

    while (paintCount>0 && m_targetIndex>=0 && !p_paintParam->isEmpty()) {
            int index = m_targetIndex * m_scale;
            const TREND_POINT & point = p_paintParam->getDataBuf().at(index);
            const HAND_MSG &pMsg = p_paintParam->getMssgBuf().at(index);
            m_targetIndex--;
            paintCount--;

            //没有记录数据
            if (point.valuePoint.isEmpty()) continue;

            //绘制time标记
            qint64 curTime = point.timePoint;
            qreal curPointX = m_pixelGap*paintCount;
            if (((curTime - m_recordStartTime) % (m_screenTimeDiv<<1)) == 0) {
                QPen timeTagPen;
                timeTagPen.setColor(QColor(0x40, 0x40, 0x40));
                timeTagPen.setWidth(20);
                painter.setPen(timeTagPen);

                painter.drawText(QPointF(curPointX,height-BOTTOM_SPACE),
                                 QDateTime::fromMSecsSinceEpoch(curTime).toString(QString("hh:mm:ss")));
            }

            //趋势数据点绘制
            for (quint32 j = 0; j<group.groupInfo.groupChannelNum; ++j) {//按组内通道号遍历
                chanNo = group.groupInfo.groupChannelItem[j]; //取通道号
                qint32 chanIndex = p_paintParam->getChannelIndex(chanNo);
                if (chanIndex<0) continue;

                const CHANNEL_RULER t_ruler = p_paintParam->getRulerPaintItem()->value(chanNo);
                rulerLen = (height-2*TREND_SPACE)*(t_ruler.displayZoneUpper-t_ruler.displayZoneLower)/100;
                rulerEndPosY = height-TREND_SPACE-((height-2*TREND_SPACE)*(t_ruler.displayZoneLower)/100);
                rulerStartPosY = height-TREND_SPACE-((height-2*TREND_SPACE)*(t_ruler.displayZoneUpper)/100);
                data = point.valuePoint.value(chanNo);

                //未压缩情况下正常绘制
                t_point1.setX(curPointX); //从屏幕最右边开始画 如果未开启压缩模式
                t_point1.setY(rulerEndPosY-((data.x()-t_ruler.rangeBottom)*rulerLen
                                            /(t_ruler.rangeTop-t_ruler.rangeBottom)));
                t_point2.setX(curPointX);
                t_point2.setY(rulerEndPosY-((data.y()-t_ruler.rangeBottom)*rulerLen
                                            /(t_ruler.rangeTop-t_ruler.rangeBottom)));
                //压缩情况下绘制
                if (t_ruler.displayPartialOnOff && data.y() >= t_ruler.displayPartialBoundary) { //如果开启压缩功能坐标重新计算
                    rulerPartialEndPosY = rulerStartPosY + rulerLen *(100-t_ruler.displayPartialPos)/100;
                    t_point1.setY(rulerPartialEndPosY - ((data.x()-t_ruler.displayPartialBoundary)*rulerLen*(100-t_ruler.displayPartialPos)/100
                                                         /(t_ruler.rangeTop-t_ruler.displayPartialBoundary)));
                    t_point2.setY(rulerPartialEndPosY - ((data.y()-t_ruler.displayPartialBoundary)*rulerLen*(100-t_ruler.displayPartialPos)/100
                                                         /(t_ruler.rangeTop-t_ruler.displayPartialBoundary)));
                } else if (t_ruler.displayPartialOnOff && data.y() < t_ruler.displayPartialBoundary) { //如果数值小于缩放边界点
                    t_point1.setY(rulerEndPosY- ((data.x()-t_ruler.rangeBottom)*rulerLen*t_ruler.displayPartialPos/100
                                                 /(t_ruler.displayPartialBoundary-t_ruler.rangeBottom)));
                    t_point2.setY(rulerEndPosY- ((data.y()-t_ruler.rangeBottom)*rulerLen*t_ruler.displayPartialPos/100
                                                 /(t_ruler.displayPartialBoundary-t_ruler.rangeBottom)));
                }
                //绘制趋势
                QPen trendPen;
                trendPen.setStyle(Qt::SolidLine);
                trendPen.setColor(QColor(t_ruler.red,t_ruler.green,t_ruler.blue)); //设置通道号颜色
                //设置趋势波形粗细度
                switch (DevConfigPtr->displayTrend.trendLineWidth ) {
                case 0:  //标准
                    trendPen.setWidth(2);
                    break;
                case 1:  //粗
                    trendPen.setWidth(4);
                    break;
                case 2: //细
                    trendPen.setWidth(1);
                    break;
                default:
                    break;
                }
                painter.setPen(trendPen);
                if (paintCount == SCREEN_DATA_COUNT_MAX) { //means 当前记录的为第一个点
                    painter.drawLine(t_point1, t_point2);
                } else {
                    painter.drawLine(prePoint.value(chanNo), t_point1);
                }
                prePoint.insert(chanNo, t_point1);
            }

            //绘制char消息
            if (!point.msgInfor.isEmpty())
                paintCharInfor(&point, t_point1.x(), TREND_SPACE, height-TREND_SPACE);

            //绘制手写信息
            if (pMsg.msgImage != nullptr &&
                (pMsg.msgGroupInfor==0 || pMsg.msgGroupInfor == (m_groupIndex+1))) {
                qreal leftTopX = paintCount * m_pixelGap - pMsg.msgImage->width();
                qreal leftTopY = pMsg.imageYPos;
                painter.drawImage(QPointF(leftTopX, leftTopY), *(pMsg.msgImage));
            }
    }
}

/*
 * 功能：
 * 	 绘制字符信息
 * 参数：
 *       int index  字符信息位于缓存中的索引
 * 返回值：
 *       无
 */
void TrendRecordPaper::paintCharInfor(const TREND_POINT *data, qreal x, qreal rulerStart, qreal rulerEnd)
{
    QMap<quint32, QPair<quint8, QString>> msg = data->msgInfor;
    qreal y = rulerEnd;
    QPen inforPen;
    QPainter painter(this);
    QFont font;
    font.setPixelSize(rect().height()/40);

    QString time(QDateTime::fromMSecsSinceEpoch(data->timePoint).toString(QString("yyyy/MM/dd  hh:mm:ss")));

    inforPen.setStyle(Qt::SolidLine);
    inforPen.setWidth(10);

    for (quint32 i = 0; i<10; ++i) {
        switch (i) {
        case 0:
            inforPen.setColor(QColor(255, 0, 0));
            break;
        case 1:
            inforPen.setColor(QColor(0, 0, 0));
            break;
        case 2:
            inforPen.setColor(QColor(0, 0, 255));
            break;
        case 3:
            inforPen.setColor(QColor(128, 128, 192));
            break;
        case 4:
            inforPen.setColor(QColor(0, 255, 128));
            break;
        case 5:
            inforPen.setColor(QColor(128, 128, 64));
            break;
        case 6:
            inforPen.setColor(QColor(105, 33, 158));
            break;
        case 7:
            inforPen.setColor(QColor(255, 128, 0));
            break;
        case 8:
            inforPen.setColor(QColor(64, 0, 0));
            break;
        case 9:
            inforPen.setColor(QColor(255, 128, 255));
            break;
        default:
            break;
        }
        painter.setPen(inforPen);
        painter.setFont(font);
        y = ((rulerEnd-rulerStart) / 10) * (i+1);
        if (!msg.value(i).second.isEmpty() &&
             (msg.value(i).first==0 || msg.value(i).first == (m_groupIndex+1))) {
            time = time +QString(" ")+ msg.value(i).second;
            painter.drawPoint(QPointF(x, y));
            x = x + font.pixelSize();
            painter.drawText(QPointF(x,y), time);
        }
    }
}

/*
 * 功能：
 * 	 绘制手写信息，手写信息工作前提之一是，趋势模块界面为初始化界面
 * 参数：
 *       int index  字符信息位于缓存中的索引
 * 返回值：
 *       无
 */
void TrendRecordPaper::paintHandInfor()
{
    qreal paintCount = SCREEN_DATA_COUNT_MAX+1;
    int bufLen = p_paintParam->getHMsgBufValidLen()-1;

    qreal leftTopX, leftTopY;
    QPainter painter(this);
    if (DevConfigPtr->displayTrend.trendDisplayDirection) {
        painter.translate(QPointF(0,rect().height()));
        painter.rotate(-90);
    }
    leftTopX = leftTopY = 0;
    for (int i = bufLen-1; i>=0; --i){
        const HAND_MSG &pMsg = p_paintParam->getMssgBuf().at(i);
        if (pMsg.msgImage == nullptr ||
                (pMsg.msgGroupInfor != 0 && pMsg.msgGroupInfor != m_groupIndex+1)){
            paintCount--;
            continue;
        }
        leftTopX = ((paintCount)*m_pixelGap)-pMsg.msgImage->width();
        leftTopY = pMsg.imageYPos;
        painter.drawImage(QPoint(leftTopX, leftTopY), *(pMsg.msgImage));
        paintCount--;
    }
}


/*
 * 功能：
 * 	 重新设置趋势绘制时，通道的绘制先后次序
 * 参数：
 *       TrendWorkMode workMode 工作模式
 * 返回值：
 *       无
 */
void TrendRecordPaper::resetChNum(GROUP_INFO &groupInfor)
{
    quint8 count = groupInfor.groupInfo.groupChannelNum-1;
    quint32 currentChNum = p_gCfgTrendGroup->getCurrentChanNo();
    for (int i = count; i>=0; --i) {
        if (currentChNum == groupInfor.groupInfo.groupChannelItem[i]) {
            groupInfor.groupInfo.groupChannelItem[i] = groupInfor.groupInfo.groupChannelItem[count];
            groupInfor.groupInfo.groupChannelItem[count] = currentChNum;
        }
    }
}




