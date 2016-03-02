#include "trendoverviewwidget.h"
#include <QDateTime>
#include <QRectF>
#include <QVector3D>

#define MOVE_DATA_COUNT_STEP 10

/*
 * 功能：
 * 	 趋势总览窗体构造函数
 * 参数：
 * 	  QWidget *parent
 * 返回值：
 *       无
 */
TrendOverviewWidget::TrendOverviewWidget(quint32 groupIndex, QWidget *parent) :
    QWidget(parent),
    m_groupIndex(groupIndex)
{
    initialize();
    setUserDefineStyle();
    setUserDefineContent();
    setWinLayout();
    sigAndSlotConnect();
}

/*
 * 功能：
 * 	 趋势总览窗体析构函数
 * 参数：
 * 	  无
 * 返回值：
 *       无
 */
TrendOverviewWidget::~TrendOverviewWidget()
{
//    release();
}

/*
 * 功能：
 * 	 设置组号
 * 参数：
 *       quint32 groupIndex 组号
 * 返回值：
 *       无
 */
void TrendOverviewWidget::setGroupIndex(quint32 groupIndex)
{
    m_groupIndex = groupIndex;
}

/*
 * 功能：
 * 	 趋势总览窗体数据对象初始化函数
 * 参数：
 * 	  无
 * 返回值：
 *       无
 */
void TrendOverviewWidget::initialize()
{
    m_paintHeight = m_paintWidth = 0;
    m_moveValidFlag = false;
    p_label_dateStart = new QLabel();
    p_label_dateEnd = new QLabel();

    p_pButton_close = new QPushButton();
    p_pButton_toRecordEnd = new QPushButton();
    p_pButton_toRecordStart = new QPushButton();
    p_pButton_toLeft = new QPushButton();
    p_pButton_toRight = new QPushButton();
    p_pButton_toLeftBig = new QPushButton();
    p_pButton_toRightBig = new QPushButton();

    p_gLayout_main = new QGridLayout();
    p_gLayout_control = new QGridLayout();

    p_pButton_toRecordEnd->setSizePolicy(QSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred));
    p_pButton_toRecordStart->setSizePolicy(QSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred));
    p_pButton_toLeft->setSizePolicy(QSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred));
    p_pButton_toRight->setSizePolicy(QSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred));
    p_pButton_toLeftBig->setSizePolicy(QSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred));
    p_pButton_toRightBig->setSizePolicy(QSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred));

    p_overViewDataBuf = p_gCfgTrendGroup->getOverviewTrendBuf();
}

/*
 * 功能：
 * 	 趋势总览窗体所开辟堆栈空间释放函数
 * 参数：
 * 	  无
 * 返回值：
 *       无
 */
void TrendOverviewWidget::release()
{
    delete p_label_dateStart;
    delete p_label_dateEnd;
    delete p_pButton_close;

    delete p_pButton_toRecordEnd;
    delete p_pButton_toRecordStart;
    delete p_pButton_toLeft;
    delete p_pButton_toRight;
    delete p_pButton_toLeftBig;
    delete p_pButton_toRightBig;

    delete p_gLayout_control;
    delete p_gLayout_main;
}

/*
 * 功能：
 * 	 趋势总览窗体风格设置
 * 参数：
 * 	  无
 * 返回值：
 *       无
 */
void TrendOverviewWidget::setUserDefineStyle()
{
    setStyleSheet("QLabel {border:none;}");
    p_gLayout_control->setContentsMargins(0,0,0,0);
    p_gLayout_control->setSpacing(0);
    p_gLayout_control->setMargin(0);
}

/*
 * 功能：
 * 	 趋势总览窗体内容设置
 * 参数：
 * 	  无
 * 返回值：
 *       无
 */
void TrendOverviewWidget::setUserDefineContent()
{
    QPixmap icon;
    icon.load(":/calendar/Image/calendar/close.png");
    p_pButton_close->setIcon(QIcon(icon));
    p_pButton_close->setIconSize(icon.size());

    icon.load(":/trend/Image/trend/left_e.png");
    p_pButton_toLeft->setIcon(QIcon(icon));
    p_pButton_toLeft->setIconSize(icon.size());

    icon.load(":/trend/Image/trend/right_e.png");
    p_pButton_toRight->setIcon(QIcon(icon));
    p_pButton_toRight->setIconSize(icon.size());

    icon.load(":/trend/Image/trend/qright_e.png");
    p_pButton_toRightBig->setIcon(QIcon(icon));
    p_pButton_toRightBig->setIconSize(icon.size());

    icon.load(":/trend/Image/trend/qleft_e.png");
    p_pButton_toLeftBig->setIcon(QIcon(icon));
    p_pButton_toLeftBig->setIconSize(icon.size());

    icon.load(":/trend/Image/trend/toStart_e.png");
    p_pButton_toRecordEnd->setIcon(QIcon(icon));
    p_pButton_toRecordEnd->setIconSize(icon.size());

    icon.load(":/trend/Image/trend/toEnd_e.png");
    p_pButton_toRecordStart->setIcon(QIcon(icon));
    p_pButton_toRecordStart->setIconSize(icon.size());
}

/*
 * 功能：
 * 	 趋势总览窗体布局设置
 * 参数：
 * 	  无
 * 返回值：
 *       无
 */
void TrendOverviewWidget::setWinLayout()
{
    p_gLayout_control->addWidget(p_pButton_toRecordStart,0,0,2,1);
    p_gLayout_control->addWidget(p_pButton_toLeftBig,0,1,2,1);
    p_gLayout_control->addWidget(p_pButton_toLeft, 0,2,2,1);
    p_gLayout_control->addWidget(p_label_dateStart, 0,3,1,1);
    p_gLayout_control->addWidget(p_label_dateEnd,   1,3,1,1);
    p_gLayout_control->addWidget(p_pButton_toRight,0,4,2,1);
    p_gLayout_control->addWidget(p_pButton_toRightBig, 0,5,2,1);
    p_gLayout_control->addWidget(p_pButton_toRecordEnd, 0,6,2,1);

    p_gLayout_main->setColumnStretch(0, 1);
    p_gLayout_main->setRowStretch(0, 1);
    p_gLayout_main->addLayout(p_gLayout_control, 8,0, 1, 1);
    p_gLayout_main->addWidget(p_pButton_close, 0, 10,1,1);

    setLayout(p_gLayout_main);
    p_gLayout_main->setAlignment(p_pButton_close, Qt::AlignTop);

    p_gLayout_control->setContentsMargins(0,0,0,0);
    p_gLayout_main->setContentsMargins(0,0,0,0);
}

/*
 * 功能：
 * 	 趋势总览窗体信号槽连接函数
 * 参数：
 * 	  无
 * 返回值：
 *       无
 */
void TrendOverviewWidget::sigAndSlotConnect()
{
    connect(p_pButton_close, SIGNAL(clicked(bool)), this, SIGNAL(sigHide()));
    connect(p_pButton_toLeft, SIGNAL(clicked()), this, SLOT(slotCurrentFrameMoveToLeft()));
    connect(p_pButton_toLeftBig, SIGNAL(clicked()), this, SLOT(slotCurrentFrameMoveBigToLeft()));
    connect(p_pButton_toRightBig, SIGNAL(clicked()), this, SLOT(slotCurrentFrameMoveBigToRight()));
    connect(p_pButton_toRight, SIGNAL(clicked()), this, SLOT(slotCurrentFrameMoveToRight()));
    connect(p_pButton_toRecordEnd, SIGNAL(clicked()), this, SLOT(slotMoveToRecordEnd()));
    connect(p_pButton_toRecordStart, SIGNAL(clicked()), this, SLOT(slotMoveToRecordStart()));
}

/*
 * 功能：
 * 	 趋势总览窗体趋势绘制函数
 * 参数：
 * 	  无
 * 返回值：
 *       无
 */
void TrendOverviewWidget::paintOverviewTrend()
{
    qint32 groupIndex, chanIndex;       //组和通道在数组中的索引
    groupIndex = p_overViewDataBuf->getGroupIndex(m_groupIndex);
    if (groupIndex < 0)
        return;
    GROUP_INFO group = p_overViewDataBuf->getGroupInfor(groupIndex);
    quint32 chanNo;
    CHANNEL_RULER t_ruler ;
   qreal rulerLen;
   qreal rulerEndPosY;//相对qt坐标系来分 start 和 end
   qreal rulerStartPosY;
   qreal rulerPartialEndPosY;  //开启压缩模式时的中间点
   QMap<quint32,QPointF> prePoint;  //保存前一个点的数值
   QVector3D data;

    QPen trendPen;
    trendPen.setStyle(Qt::SolidLine);
    trendPen.setWidth(2);
    QPainter painter(this);
    QPointF t_point1, t_point2;
    int validDataLen = p_overViewDataBuf->getBufValidLen()-1;
    qreal paintCount = p_overViewDataBuf->getBufLen()-1;

    for (int i = validDataLen; i>=0;--i){  //按点遍历
        //根据组号取出通道数据
        TREND_POINT point = p_overViewDataBuf->getDataBuf().at(i); //取一点数据
        if (point.valuePoint.isEmpty() )
            continue;

        for (int j = 0; j<group.groupInfo.groupChannelNum; ++j){  //按组内通道号遍历
            chanNo = group.groupInfo.groupChannelItem[j]; //取通道号
            chanIndex = p_overViewDataBuf->getChannelIndex(chanNo);
            if (chanIndex < 0)  //如果返回值为默认值则跳出本次循环
                continue;

            t_ruler = p_overViewDataBuf->getRulerPaintItem()->value(chanNo);
            rulerLen = m_paintHeight;
            rulerEndPosY = m_paintHeight;
            rulerStartPosY = 0;
            data = point.valuePoint.value(chanNo);

            //未压缩情况下正常绘制
            t_point1.setX((m_dataPointGap*(paintCount))); //从屏幕最右边开始画 如果未开启压缩模式
            t_point1.setY(rulerEndPosY-((data.x()-t_ruler.rangeBottom)*rulerLen
                                        /(t_ruler.rangeTop-t_ruler.rangeBottom)));
            t_point2.setX((m_dataPointGap*(paintCount)));
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
            //绘制
            trendPen.setColor(QColor(t_ruler.red,t_ruler.green,t_ruler.blue)); //设置通道号颜色
            painter.setPen(trendPen);
            if (paintCount == p_overViewDataBuf->getBufLen()-1) { //means 当前记录的为第一个点
                painter.drawLine(t_point1, t_point2);
            } else {
                painter.drawLine(prePoint.value(chanNo), t_point1);
            }
            prePoint.insert(chanNo, t_point1);
        }
        paintCount--;
    }
}

/*
 * 功能：
 * 	 计算趋势绘制框架大小，并绘制背景颜色
 * 参数：
 * 	  无
 * 返回值：
 *       无
 */
void TrendOverviewWidget::paintBackground()
{
    QPainter painter(this);
    painter.fillRect(m_paintBackgroundRectF, QBrush(QColor(200,200,200)));
}

/*
 * 功能：
 *          绘制当前趋势画面所在区域框
 */
void TrendOverviewWidget::paintCurrentTrendFrame()
{
    QPainter  painter(this);
    QPen pen;
    pen.setColor("black");
    pen.setWidth(2);
    painter.setPen(pen);
    painter.setBrush(QColor(255,255,255));
    painter.drawRect(m_frameRectF);
}

void TrendOverviewWidget::countPaintParameter()
{
    //总览趋势背景相关参数计算。
    m_paintHeight = rect().height() - 2*p_label_dateStart->height();
    m_paintWidth = rect().width() - p_pButton_close->width();
    m_dataPointGap = (qreal)((qreal)m_paintWidth/(qreal)p_overViewDataBuf->getBufLen());
    m_paintBackgroundRectF.setTopLeft(QPointF(0,0));               //背景绘制框赋值
    m_paintBackgroundRectF.setWidth(m_paintWidth);
    m_paintBackgroundRectF.setHeight(m_paintHeight);

    qint32 curRightPointIndex = p_overViewDataBuf->getRecordPointIndex();
    qint32 valiDataPointIndex = p_overViewDataBuf->getBufValidLen() - 1;
    qreal width, height;
    if (curRightPointIndex>=0 && curRightPointIndex<=valiDataPointIndex) {
        if (curRightPointIndex>(SCREEN_DATA_COUNT_MAX+1)) {
            width = m_dataPointGap * SCREEN_DATA_COUNT_MAX;
        } else {
            width = m_dataPointGap * curRightPointIndex;
        }
        height = m_paintHeight - 4;
        QPointF topLeft;
        topLeft.setX(m_paintWidth-(m_dataPointGap *(valiDataPointIndex-curRightPointIndex)) - width);
        topLeft.setY(2);
        m_frameRectF.setTopLeft(topLeft);
        m_frameRectF.setWidth(width);
        m_frameRectF.setHeight(height);
    }
}

void TrendOverviewWidget::slotCurrentFrameMoveToLeft()
{
    p_gCfgTrendGroup->moveHistoryDataCursor(-MOVE_DATA_COUNT_STEP);
}

void TrendOverviewWidget::slotCurrentFrameMoveToRight()
{
    p_gCfgTrendGroup->moveHistoryDataCursor(MOVE_DATA_COUNT_STEP);
}

void TrendOverviewWidget::slotCurrentFrameMoveBigToLeft()
{
    p_gCfgTrendGroup->moveHistoryDataCursor(-MOVE_DATA_COUNT_STEP << 2);
}

void TrendOverviewWidget::slotCurrentFrameMoveBigToRight()
{
    p_gCfgTrendGroup->moveHistoryDataCursor(MOVE_DATA_COUNT_STEP << 2);
}

void TrendOverviewWidget::slotMoveToRecordStart()
{
    p_gCfgTrendGroup->moveToBufLeft();
}

void TrendOverviewWidget::slotMoveToRecordEnd()
{
    p_gCfgTrendGroup->moveToBufRight();
}

/*
 * 功能：
 * 	 重写绘制函数
 * 参数：
 * 	  QPaintEvent *e
 * 返回值：
 *       无
 */
void TrendOverviewWidget::paintEvent(QPaintEvent *e)
{
    if (p_overViewDataBuf->isEmpty()) {
        m_startTimePoint = 0;
        m_endTimePoint = 0;
    } else {
        m_startTimePoint = p_overViewDataBuf->getDataBuf().at(0).timePoint;
        m_endTimePoint = p_overViewDataBuf->getDataBuf().at(p_overViewDataBuf->getBufValidLen()-1).timePoint;
    }
    p_label_dateEnd->setText(QDateTime::fromMSecsSinceEpoch(m_endTimePoint).toString(QString("yyyy/MM/dd  hh:mm:ss:zzz")));
    p_label_dateStart->setText(QDateTime::fromMSecsSinceEpoch(m_startTimePoint).toString(QString("yyyy/MM/dd  hh:mm:ss:zzz")));

    paintBackground();
    paintCurrentTrendFrame();
    paintOverviewTrend();
    QWidget::paintEvent(e);
}

void TrendOverviewWidget::showEvent(QShowEvent *e)
{
    countPaintParameter();
    QWidget::showEvent(e);
}

void TrendOverviewWidget::mousePressEvent(QMouseEvent *e)
{
    QWidget::mousePressEvent(e);
}

void TrendOverviewWidget::mouseMoveEvent(QMouseEvent *e)
{
    QPointF curPoint = e->localPos();

    if (m_moveValidFlag) {
         qint32 xdiff = curPoint.x() - m_prePoint.x();
         qint32 dataCount = xdiff/m_dataPointGap;
         p_gCfgTrendGroup->moveHistoryDataCursor(dataCount);
         m_prePoint = curPoint;
    }

    QWidget::mouseMoveEvent(e);
}


