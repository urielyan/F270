#include "trendrulerwidget.h"
#include <QVector3D>
/*
 * 功能：
 * 	 标尺窗体构造
 * 参数：
 *       quint32 groupIndex, 组号
 *       QWidget *parent   父指针
 * 返回值：
 *       无
 */
TrendRulerWidget::TrendRulerWidget(quint32 groupIndex, QWidget *parent) :
    QWidget(parent),
    m_groupIndex(groupIndex)
{
    m_startPoint = -1;
    m_paintWidth = -1;
    m_largeScaleLen = 0;
    m_smallScaleLen = 0;
    m_rulerWidth = 0;
    m_rulerNum = 1;
    m_backAlpha = 0x10;
    p_paintParam = p_gCfgTrendGroup->getRealTimeTrendBuf();
    p_rulerMap = new QVector<QVector<quint32> >(10);
    m_scaleImageLoadFlag = false;
    setFocusPolicy(Qt::NoFocus);
}

/*
 * 功能：
 * 	 标尺窗体析构函数
 * 参数：
 *        无
 * 返回值：
 *       无
 */
TrendRulerWidget::~TrendRulerWidget()
{
    delete p_rulerMap;
}

/*
 * 功能：
 * 	 标尺窗体，设置标尺移动距离范围
 * 参数：
 *       int startPoint 范围开始点
 *       int endPoint   范围结束点
 * 返回值：
 *       无
 */
int TrendRulerWidget::setRulerMoveRange(int startPoint, int width)
{
    if (startPoint == width)
        return 1;
    m_startPoint = startPoint;
    m_paintWidth = width;
    return 0;
}


/*
 * 功能：
 * 	 标尺窗体，重写绘制函数, 绘制刻度绘制标识
 * 参数：
 *       QPaintEvent *e
 * 返回值：
 *       无
 */
void TrendRulerWidget::paintEvent(QPaintEvent *e)
{
    quint32 currentNum = p_gCfgTrendGroup->getCurrentChanNo();
    if (RealTimeRecord == p_gCfgTrendGroup->getCurrentWorkMode()) {
        p_paintParam = p_gCfgTrendGroup->getRealTimeTrendBuf();
    } else {
        p_paintParam = p_gCfgTrendGroup->getHistoryTrendBuf();
    }

    if (false == m_scaleImageLoadFlag) {
        //背景填充
        paintRulerBackGroup();
    } else {
        //标尺图像加载
        QPixmap backImage(m_filePath);
        QPainter painter(this);
        painter.drawPixmap(0,0,rect().width(), rect().height(), backImage);
    }

    //多标尺绘制
    int realPos = 0 ;
    for (quint8 i = 0; i < 10; ++i) {

        if (p_rulerMap->at(i).isEmpty())
            continue;

        //用户选择当前组内的通道时，将用户选择的通道最后绘制
        for (quint8 j = 0; j < p_rulerMap->at(i).length(); ++j) {
            if (p_rulerMap->at(i).last()!= currentNum &&
                    p_rulerMap->at(i).contains(currentNum)) {
                int index = p_rulerMap->at(i).indexOf(currentNum);
                QVector<quint32> vectorChnum = p_rulerMap->at(i);
                vectorChnum.replace(index, p_rulerMap->at(i).last());
                vectorChnum.replace(vectorChnum.length()-1, p_rulerMap->at(i).at(index));
                p_rulerMap->replace(i, vectorChnum);
            }
        }

        if (DevConfigPtr->displayTrend.trendScaleType == 0) {   //MarkMode
            //绘制当前通道刻度
            if (false == m_scaleImageLoadFlag)
                paintScale(realPos, p_rulerMap->at(i).last());

            //绘制所有标记
            for (quint8 j = 0; j < p_rulerMap->at(i).length(); ++j) {
                paintMark(realPos, p_rulerMap->at(i).at(j));
            }
        } else {    //BarMode
            //绘制当前通道的标尺棒图
            paintBar(realPos, p_rulerMap->at(i).last());
            //绘制当前通道刻度
            if (false == m_scaleImageLoadFlag)
                paintScale(realPos, p_rulerMap->at(i).last());
        }        
        realPos++;
    }

    QWidget::paintEvent(e);
}

void TrendRulerWidget::showEvent(QShowEvent *e)
{
    m_scaleImageLoadFlag = false;
    m_filePath = QString(TREND_SCALE_IMAGE_DIR) + QString::number(m_groupIndex);
    QFile fileObj(m_filePath);

    if (DevConfigPtr->displayGroup[m_groupIndex].groupScaleImage) {
        if (fileObj.exists()) {
            m_scaleImageLoadFlag = true;
        } else {
            //警告提示信号发射
            emit sigWarningScaleImageLose();
        }
    }

    QWidget::showEvent(e);
}

/*
 * 功能：
 * 	 标尺窗体，重写鼠标移动函数
 * 参数：
 *       QMouseEvent*e
 * 返回值：
 *       无
 */
void TrendRulerWidget::mouseMoveEvent(QMouseEvent *e)
{
    if (m_startPoint == m_paintWidth)  //如果没有设置范围则 不能移动标尺
        return;

    if (p_gCfgTrendGroup->getCurrentWorkMode() == RealTimeRecord) {
        p_gCfgTrendGroup->switchToHistoryRecordMode();
        p_gCfgTrendGroup->setWorkMode(BrowseHistory);
    }

    if (DevConfigPtr->displayTrend.trendDisplayDirection) { //垂直显示
        int posY = mapTo((QWidget *)parent(), e->pos()).y();
        if ( posY <= 0) {
            posY = 0;
            move( pos().x(), posY);
        } else if (posY >= (m_paintWidth)) {
            posY = m_paintWidth;
            move(pos().x(), posY);
        } else {
            move(pos().x(), posY);
        }

        int index = (posY) * SCREEN_DATA_COUNT_MAX/m_paintWidth;
        p_gCfgTrendGroup->setCurrentRulerPointDataIndex(index);

    } else {
        int posX = mapTo((QWidget *)parent(), e->pos()).x();
        if ( posX <= m_startPoint) {
            posX = m_startPoint;
            move(m_startPoint, pos().y());
        } else if (posX >= (m_startPoint + m_paintWidth)) {
            posX = m_startPoint + m_paintWidth;
            move(m_startPoint+m_paintWidth, pos().y());
        } else {
            move(posX, pos().y());
        }

        int index = (m_paintWidth - posX + m_startPoint) * SCREEN_DATA_COUNT_MAX/m_paintWidth;
        p_gCfgTrendGroup->setCurrentRulerPointDataIndex(index);
    }

    QWidget::mouseMoveEvent(e);
}

/*
 * 功能：
 * 	 标尺窗体，刻度绘制函数
 * 参数：
 *       int pos,                 标尺位置号 0～9
 *       quint32 chanNo  通道号
 * 返回值：
 *       无
 */
void TrendRulerWidget::paintScale(int pos, quint32 chanNo)
{
    int height = rect().height();
    QPainter painter(this);
    QFont textFont;

    if (DevConfigPtr->displayTrend.trendDisplayDirection) {
        painter.translate(QPointF(0,rect().height()));
        painter.rotate(-90);
        height =  rect().width();
    }

    CHANNEL_RULER t_ruler = p_paintParam->getRulerPaintItem()->value(chanNo); //取得通道标尺绘制参数对象
    QVector<QLineF> largeScaleLines, smallScaleLines;
    qreal rulerLen = (height-2*TREND_SPACE)*(t_ruler.displayZoneUpper-t_ruler.displayZoneLower)/100;
    qreal largescale = (qreal)(rulerLen) / (qreal)t_ruler.displayScaleDiv;
    qreal smallscale =  largescale/10; //每大格固定为10小格
    qreal rulerEndPosY = height-TREND_SPACE-((height-2*TREND_SPACE)*(t_ruler.displayZoneLower)/100); //相对qt坐标系来分 start 和 end
    qreal rulerStartPosY = height-TREND_SPACE-((height-2*TREND_SPACE)*(t_ruler.displayZoneUpper)/100);
    qreal rulerRangeTop = t_ruler.rangeTop;
    qreal rulerRangeBottom = t_ruler.rangeBottom;
    qreal rulerRangeStep = (t_ruler.rangeTop - t_ruler.rangeBottom) / t_ruler.displayScaleDiv;

    //标尺彩带绘制
    if (t_ruler.colorBandMode == 1) {           //内侧
        qreal bandStartPos = rulerStartPosY + rulerLen * ((t_ruler.rangeTop - t_ruler.colorBandPosUpper)/(t_ruler.rangeTop - t_ruler.rangeBottom));
        qreal bandEndPos =  rulerStartPosY + rulerLen * ((t_ruler.rangeTop - t_ruler.colorBandPosLower)/(t_ruler.rangeTop - t_ruler.rangeBottom));
        QColor bandColor(COLOR_RED(t_ruler.colorBandColor), COLOR_GREEN(t_ruler.colorBandColor), COLOR_BLUE(t_ruler.colorBandColor));
        painter.fillRect(pos*m_rulerWidth, bandStartPos, m_rulerWidth/4, bandEndPos-bandStartPos, bandColor);
    } else if (t_ruler.colorBandMode == 2) {//外侧
        qreal bandStartPos = rulerStartPosY + rulerLen * ((t_ruler.rangeTop - t_ruler.colorBandPosUpper)/(t_ruler.rangeTop - t_ruler.rangeBottom));
        qreal bandEndPos =  rulerStartPosY + rulerLen * ((t_ruler.rangeTop - t_ruler.colorBandPosLower)/(t_ruler.rangeTop - t_ruler.rangeBottom));
        QColor bandColor(COLOR_RED(t_ruler.colorBandColor), COLOR_GREEN(t_ruler.colorBandColor), COLOR_BLUE(t_ruler.colorBandColor));
        painter.fillRect(pos*m_rulerWidth, rulerStartPosY, m_rulerWidth/4, bandStartPos-rulerStartPosY, bandColor);
        painter.fillRect(pos*m_rulerWidth, bandEndPos, m_rulerWidth/4, rulerEndPosY-bandEndPos, bandColor);
    }

    //报警标记绘制
    if (t_ruler.alarmMark) {
        QColor alarmColor[4];
        if (t_ruler.alarmMode) {        //固定
            for (int i=0; i<4; ++i)
                alarmColor[i] = QColor(COLOR_RED(t_ruler.alarmColor[i]), COLOR_GREEN(t_ruler.alarmColor[i]), COLOR_BLUE(t_ruler.alarmColor[i]));
        } else {                                    //报警
            for (int i=0; i<4; ++i)
                alarmColor[i] = QColor(Qt::red);
        }
        for (int i=0; i<4; ++i) {
            if (t_ruler.alarmOnOff[i]) {
                qreal alarmMarkPos = rulerStartPosY + rulerLen * (t_ruler.rangeTop - t_ruler.alarmValue[i]) / (t_ruler.rangeTop - t_ruler.rangeBottom);
                QPainterPath path(QPointF(pos*m_rulerWidth+m_rulerWidth/2, alarmMarkPos));
                path.lineTo(QPointF(pos*m_rulerWidth+m_rulerWidth*3/4, alarmMarkPos));
                path.lineTo(QPointF(pos*m_rulerWidth+m_rulerWidth*3/4, alarmMarkPos-rulerLen/70));
                painter.fillPath(path, QBrush(alarmColor[i]));
            }
        }

    }

    textFont.setFixedPitch(m_rulerWidth/5);
    textFont.setBold(true);
    painter.setFont(textFont);
    painter.setPen(QColor(0x40, 0x40, 0x40));
    if (!t_ruler.displayPartialOnOff) {   //压缩功能未开启
        //绘制大刻度
        for (int i = 0; i<=t_ruler.displayScaleDiv; i++)
        {
            largeScaleLines.append(QLineF(QPointF(pos*m_rulerWidth,  rulerStartPosY+(largescale*i)+1),
                                          QPointF(m_largeScaleLen+pos*m_rulerWidth, rulerStartPosY+(largescale*i)+1)));
        }
        painter.drawLines(largeScaleLines);

        //绘制小刻度
        for (int i = 1; i<=10*t_ruler.displayScaleDiv; i++)
        {
            QLineF line(QPointF(pos*m_rulerWidth,  rulerStartPosY+(smallscale*i)+1),
                               QPointF(m_smallScaleLen+pos*m_rulerWidth, rulerStartPosY+(smallscale*i)+1));
            if (i % 5 == 0)
                line.setLength(m_smallScaleLen*3/2);
            smallScaleLines.append(line);
        }
        painter.drawLines(smallScaleLines);

        //绘制刻度信息
        for (int i = 0; i<=t_ruler.displayScaleDiv; i++)
        {
            if(DevConfigPtr->displayTrend.trendScaleMode == 0) //标准刻度
                painter.drawText(QPointF(pos*m_rulerWidth,  rulerStartPosY+(+largescale*i)+1), QString::number(rulerRangeTop-rulerRangeStep*i, 'f', 1));
            else  //详细刻度
                painter.drawText(QPointF(pos*m_rulerWidth,  rulerStartPosY+(+largescale*i)+1), QString::number(rulerRangeTop-rulerRangeStep*i, 'f', 3));
        }
    } else { //压缩功能开启
        //这里边界应该由数值输入框来限定，万一越界 暂定默认计算中间点为边界点
        if (t_ruler.displayPartialBoundary >= t_ruler.rangeTop || t_ruler.displayPartialPos <= t_ruler.rangeBottom)
            t_ruler.displayPartialBoundary = t_ruler.rangeTop - (t_ruler.rangeTop - t_ruler.rangeBottom) /2;
        qreal rulerPartialEndPosY = rulerStartPosY + (rulerEndPosY - rulerStartPosY) *(100-t_ruler.displayPartialPos)/100;
        qreal rulerPartialStartPosY =  rulerStartPosY;
        qreal rulerPartialLen = rulerLen*(100-t_ruler.displayPartialPos)/100;
        largescale = (qreal)(rulerPartialLen) / (qreal)t_ruler.displayScaleDiv;
        painter.drawText(QPointF(pos*m_rulerWidth,  rulerPartialStartPosY+1), QString::number(rulerRangeTop, 'f', 1)); //绘制量程上限

        //压缩功能开启下第一段刻度绘制
        for (int i = 0; i<=t_ruler.displayScaleDiv; i++)
        {
            if (i %2 != 0)
                largeScaleLines.append(QLineF(QPointF(pos*m_rulerWidth,  rulerPartialStartPosY+(largescale*i)+1),
                                              QPointF(m_smallScaleLen+pos*m_rulerWidth, rulerPartialStartPosY+(largescale*i)+1)));
            else
                largeScaleLines.append(QLineF(QPointF(pos*m_rulerWidth,  rulerPartialStartPosY+(largescale*i)+1),
                                              QPointF(m_largeScaleLen+pos*m_rulerWidth, rulerPartialStartPosY+(largescale*i)+1)));
        }
        painter.drawLines(largeScaleLines);
        //压缩功能开启下第二段刻度绘制
        rulerPartialStartPosY =  rulerPartialEndPosY;
        rulerPartialEndPosY = rulerEndPosY;
        rulerPartialLen = rulerLen*(t_ruler.displayPartialPos)/100;
        largescale = (qreal)(rulerPartialLen) / (qreal)t_ruler.displayScaleDiv;
        largeScaleLines.clear();
        painter.drawText(QPointF(pos*m_rulerWidth,  rulerPartialStartPosY+1), QString::number(t_ruler.displayPartialBoundary, 'f', 1)); //绘制压缩点
        for (int i = 0; i<=t_ruler.displayScaleDiv; i++)
        {
            if (i%2 != 0)
                largeScaleLines.append(QLineF(QPointF(pos*m_rulerWidth,  rulerPartialStartPosY+(largescale*i)+1),
                                              QPointF(m_smallScaleLen+pos*m_rulerWidth, rulerPartialStartPosY+(largescale*i)+1)));
            else
                largeScaleLines.append(QLineF(QPointF(pos*m_rulerWidth,  rulerPartialStartPosY+(largescale*i)+1),
                                              QPointF(m_largeScaleLen+pos*m_rulerWidth, rulerPartialStartPosY+(largescale*i)+1)));
        }
        painter.drawLines(largeScaleLines);
        painter.drawText(QPointF(pos*m_rulerWidth,  rulerEndPosY+1), QString::number(rulerRangeBottom, 'f', 1)); //绘制量程下限
    }
}

void TrendRulerWidget::paintRulerBackGroup()
{
    int width = rect().width();
    int height = rect().height();
    QPainter painter(this);
    QColor backColor = QColor(0x2f,0x2f,0xff,0x10);
    backColor.setAlpha(m_backAlpha);

    if (DevConfigPtr->displayTrend.trendDisplayDirection) {
        painter.translate(QPointF(0,rect().height()));
        painter.rotate(-90);
        width = rect().height();
        height =  rect().width();

        painter.fillRect(0, 0, width, height, QBrush(backColor));
        painter.setPen(QColor(0x40, 0x40, 0x40));
        painter.drawRect(1, 0, width, height);
    } else {
        painter.fillRect(0, 0, width, height, QBrush(backColor));
        painter.setPen(QColor(0x40, 0x40, 0x40));
        painter.drawRect(0, 0, width, height);
    }

}

/*
 * 功能：
 * 	 标尺窗体，通道标识绘制函数
 * 参数：
 *       int pos                 数据缓存中的位置点
 *       quint32 chanNo 通道号
 * 返回值：
 *       无
 */
void TrendRulerWidget::paintMark(int rulerPos, quint32 chanNo)
{
    //获取标记颜色参数
    CHANNEL_RULER t_ruler = p_paintParam->getRulerPaintItem()->value(chanNo);

    //标尺点数据
    QVector3D markPoint = p_gCfgTrendGroup->getCurrentRulerPointData(chanNo);
    if (markPoint.x() != markPoint.y()) {  //如果是显示数据则取平均值
        markPoint.setX((markPoint.x() + markPoint.y())/2);
        markPoint.setY(markPoint.x());
    }

//    int width = rect().width();
    int height = rect().height();
    QPainter painter(this);

    if (DevConfigPtr->displayTrend.trendDisplayDirection) {
        painter.translate(QPointF(0,rect().height()));
        painter.rotate(-90);
//        width = rect().height();
        height =  rect().width();
    }
    //参数计算
    qreal rulerLen = (height-2*TREND_SPACE)*(t_ruler.displayZoneUpper-t_ruler.displayZoneLower)/100;
    qreal rulerEndPosY = height-TREND_SPACE-((height-2*TREND_SPACE)*(t_ruler.displayZoneLower)/100);
    qreal rulerStartPosY = height-TREND_SPACE-((height-2*TREND_SPACE)*(t_ruler.displayZoneUpper)/100);
    qreal markHeight = (height-2*TREND_SPACE)/30;


    //计算出绘制坐标
    qreal markX = rulerPos*m_rulerWidth+m_smallScaleLen/2;  // 标识往后挪
    qreal markY = rulerEndPosY-((markPoint.x()-t_ruler.rangeBottom)*rulerLen/(t_ruler.rangeTop-t_ruler.rangeBottom));
    qreal rulerPartialEndPosY;
    if (t_ruler.displayPartialOnOff && markPoint.x() >= t_ruler.displayPartialBoundary) { //如果开启压缩功能坐标重新计算
        rulerPartialEndPosY = rulerStartPosY + rulerLen *(100-t_ruler.displayPartialPos)/100;
        markY = rulerPartialEndPosY - ((markPoint.x()-t_ruler.displayPartialBoundary)*rulerLen*(100-t_ruler.displayPartialPos)/100
                                /(t_ruler.rangeTop-t_ruler.displayPartialBoundary));
    } else if (t_ruler.displayPartialOnOff && markPoint.x() < t_ruler.displayPartialBoundary) { //如果数值小于缩放边界点
         markY = rulerEndPosY- ((markPoint.x()-t_ruler.rangeBottom)*rulerLen*t_ruler.displayPartialPos/100
                                        /(t_ruler.displayPartialBoundary-t_ruler.rangeBottom));
    }
    //绘制标记
    QPointF points[5] = { //点的顺序不能乱
        QPointF(markX, markY),
        QPointF(markX+m_rulerWidth*1/4, markY-markHeight/2),
        QPointF(markX+m_rulerWidth*2/3, markY-markHeight/2),
        QPointF(markX+m_rulerWidth*2/3, markY+markHeight/2),
        QPointF(markX+m_rulerWidth*1/4, markY+markHeight/2)
    };
    QString tag;
    int i = 0;
    if (t_ruler.displayTagNo[i] == '\0') {
        tag = CfgChannel::instance()->getChannelStr(chanNo);
    }else{
        while(t_ruler.displayTagNo[i] != '\0'){
            tag.append(QChar(t_ruler.displayTagNo[i]));
            i++;
        }
    }

    QFont font = painter.font();
    font.setPixelSize(markHeight/2);
    font.setBold(true);
    painter.setFont(font);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QColor(t_ruler.red, t_ruler.green, t_ruler.blue));
    painter.setBrush(QBrush(QColor(t_ruler.red, t_ruler.green, t_ruler.blue)));
    painter.drawPolygon(points, 5);
    painter.setPen(QColor("white"));
    if (DevConfigPtr->displayTrend.trendScaleNumber == 0)
        return;
    if (DevConfigPtr->displayTrend.trendScaleNumber == 3)
        tag.remove(0, 1);
    painter.drawText(QRectF(QPointF(markX+m_rulerWidth*1/8, markY-markHeight/2),QPointF(markX+m_rulerWidth*2/3, markY+markHeight/2)),Qt::AlignCenter|Qt::AlignLeft,
                     QString(tag));
}

/*
 * 功能：
 * 	 标尺窗体，通道棒图标记绘制函数
 * 参数：
 *       无
 * 返回值：
 *       无
 */
void TrendRulerWidget::paintBar(int rulerPos, quint32 chanNo)
{
    //获取标记颜色参数
    CHANNEL_RULER t_ruler = p_paintParam->getRulerPaintItem()->value(chanNo);

    //标尺点数据
    QVector3D markPoint = p_gCfgTrendGroup->getCurrentRulerPointData(chanNo);
    //如果是显示数据则取平均值
    if (markPoint.x() != markPoint.y()) {
        markPoint.setX((markPoint.x() + markPoint.y())/2);
        markPoint.setY(markPoint.x());
    }

//    int width = rect().width();
    int height = rect().height();
    QPainter painter(this);

    if (DevConfigPtr->displayTrend.trendDisplayDirection) {
        painter.translate(QPointF(0,rect().height()));
        painter.rotate(-90);
//        width = rect().height();
        height =  rect().width();
    }

    //参数计算
    qreal rulerLen = (height-2*TREND_SPACE)*(t_ruler.displayZoneUpper-t_ruler.displayZoneLower)/100;
    qreal rulerEndPosY = height-TREND_SPACE-((height-2*TREND_SPACE)*(t_ruler.displayZoneLower)/100);
    qreal rulerStartPosY = height-TREND_SPACE-((height-2*TREND_SPACE)*(t_ruler.displayZoneUpper)/100);

    //计算出绘制坐标
    qreal markX = rulerPos*m_rulerWidth+m_smallScaleLen/2;  // 标识往后挪
    qreal markY = rulerEndPosY-((markPoint.x()-t_ruler.rangeBottom)*rulerLen/(t_ruler.rangeTop-t_ruler.rangeBottom));
    qreal rulerPartialEndPosY;
    if (t_ruler.displayPartialOnOff && markPoint.x() >= t_ruler.displayPartialBoundary) { //如果开启压缩功能坐标重新计算
        rulerPartialEndPosY = rulerStartPosY + rulerLen *(100-t_ruler.displayPartialPos)/100;
        markY = rulerPartialEndPosY - ((markPoint.x()-t_ruler.displayPartialBoundary)*rulerLen*(100-t_ruler.displayPartialPos)/100
                                /(t_ruler.rangeTop-t_ruler.displayPartialBoundary));
    } else if (t_ruler.displayPartialOnOff && markPoint.x() < t_ruler.displayPartialBoundary) { //如果数值小于缩放边界点
         markY = rulerEndPosY- ((markPoint.x()-t_ruler.rangeBottom)*rulerLen*t_ruler.displayPartialPos/100
                                        /(t_ruler.displayPartialBoundary-t_ruler.rangeBottom));
    }
    //绘制标记
    QString tag;
    int i = 0;
    while(t_ruler.displayTagNo[i] != '\0'){
        tag.append(QChar(t_ruler.displayTagNo[i]));
        i++;
    }
    painter.setBrush(QBrush(QColor(t_ruler.red, t_ruler.green, t_ruler.blue)));
    painter.drawRect(QRectF(markX, markY, m_smallScaleLen ,height-markY));
}

/*
 * 功能：
 * 	 生成本组下的标尺地图
 * 参数：
 *       无
 * 返回值：
 *       qint8 返回当前要显示的标尺个数
 */
quint8 TrendRulerWidget::produceRulerMap()
{
    if (p_paintParam == nullptr) {
        return 1;
    }
    delete p_rulerMap;
    p_rulerMap = new QVector<QVector<quint32> >(10);

    qint32 chanIndex;
    quint8 count = 0;

    quint8 rulerPos;
    CHAN_INFO  chanInfor;
    quint16 chanType;
    quint32 chanNo;  //临时通道号
    //取组信息块中的通道号数值
    GROUP_INFO groupInfor;
     ::memset(&groupInfor, 0, sizeof(GROUP_INFO));

    for (quint32 i= 0; i< p_paintParam->getGroupInforHead().groupNum; ++i){
        groupInfor = p_paintParam->getGroupInfor(i);
        if (groupInfor.grpIndex == m_groupIndex)
            break;
    }

    //由组信息块中的通道号数组访问通道信息块并生成标尺信息向量
    for (int i = 0; i<groupInfor.groupInfo.groupChannelNum; ++i){
        chanNo = groupInfor.groupInfo.groupChannelItem[i]; //获取通道号
        chanIndex = p_paintParam->getChannelIndex(chanNo);
        if (chanIndex < 0)
            continue;
        chanType = CHANNEL_TYPE(chanNo);   //解析通道类型
        chanInfor =  p_paintParam->getChannelInfor(chanIndex);//获取通道信息块
        switch(chanType){
        case Channel_Type_AI:   //AI类型通道
            rulerPos = chanInfor.channelConfig.chanAI.displayConfig.displayScalePos-1;
            break;
        case Channel_Type_DI:   //DI类型通道
            rulerPos = chanInfor.channelConfig.chanDI.displayConfig.displayScalePos-1;
            break;
        case Channel_Type_DO:
            rulerPos = chanInfor.channelConfig.chanDO.displayConfig.displayScalePos-1;
            break;
        case Channel_Type_Math:
            rulerPos = chanInfor.channelConfig.chanMath.displayConfig.displayScalePos-1;
            break;
        case Channel_Type_Comm: //通信类型通道
            rulerPos = chanInfor.channelConfig.chanComm.displayConfig.displayScalePos-1;
            break;
        default:
            rulerPos = 0;
            break;
        }

        if (rulerPos>9) rulerPos = 0;   //无效配置，默认为0

        if (p_rulerMap->at(rulerPos).length() == 0){
            count++;
        }
        QVector<quint32> ruler = p_rulerMap->at(rulerPos);
        ruler.append(chanNo);
        p_rulerMap->operator [](rulerPos) = ruler;
    }

    m_rulerNum = count;
    if (m_rulerNum == 0)
        m_rulerNum = 1;

    return count;
}

/*
 * 功能：
 * 	 重新计算相关参数
 * 参数：
 *       无
 * 返回值：
 *       无
 */
void TrendRulerWidget::reCountTruleArg()
{
    if (DevConfigPtr->displayTrend.trendDisplayDirection) {
        m_rulerWidth = rect().height()/m_rulerNum;
    } else {
        m_rulerWidth = rect().width()/m_rulerNum;
    }
    m_largeScaleLen = m_rulerWidth/2 ;
    m_smallScaleLen = m_largeScaleLen/2;
}





