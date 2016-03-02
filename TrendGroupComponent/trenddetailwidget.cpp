#include "trenddetailwidget.h"
#include <QDateTime>

/*
 * 功能：
 * 	 构造函数
 * 参数：
 * 	  int groupIndex 趋势详细浏览窗体组号
 *       QWidget *parent 窗体父指针
 * 返回值：
 *       无
 */
TrendDetailWidget::TrendDetailWidget(int groupIndex, QWidget *parent) :
    QWidget(parent),
    m_groupIndex(groupIndex)
{
    initialize();
    setUserDefineContent();
    setWinLayout();
    sigAndSlotConnect();
}

/*
 * 功能：
 * 	 析构函数
 * 参数：
 * 	  无
 * 返回值：
 *       无
 */
TrendDetailWidget::~TrendDetailWidget()
{
    release();
}

/*
 * 功能：
 * 	 public 接口，用于设置组号
 * 参数：
 * 	  quint32 groupIndex 组号
 * 返回值：
 *       无
 */
void TrendDetailWidget::setGroupIndex(quint32 groupIndex)
{
    m_groupIndex = groupIndex;
    p_recordPaper2->setGroupIndex(m_groupIndex);
}

/*
 * 功能：
 * 	 重新计算详细浏览趋势模块相关参数
 * 参数：
 * 	  无
 * 返回值：
 *      无
 */
void TrendDetailWidget::reCountDetailArg()
{
     setUserDefineStyle();
}

/*
 * 功能：
 * 	 接收界面上传递进来的字符信息，并处理之
 * 参数：
 * 	  QString msg
 * 返回值：
 *      无
 */
void TrendDetailWidget::slotDealCharMsg(QString msg)
{
    quint8 msgGr;
    if (DevConfigPtr->displayTrend.trendMsgWriteMode)
        msgGr = m_groupIndex+1;
    else
        msgGr = 0;

    p_recordPaper2->dealCharMsg(msg, msgGr);
}

void TrendDetailWidget::slotDealHWMsg(QImage *image, QString &dir, quint8 msgGr, QPoint &rightBottom)
{
    if (msgGr != 0)
        msgGr = m_groupIndex+1;
    p_recordPaper2->dealHWMsg(image, dir, msgGr, rightBottom);
}

/*
 * 功能：
 * 	 初始化函数，初始化窗体数据成员
 * 参数：
 * 	  无
 * 返回值：
 *      无
 */
void TrendDetailWidget::initialize()
{
    p_pb_overviewTrend = new QPushButton();
    p_pb_inforChar = new QPushButton();
    p_pb_inforHand = new QPushButton();
    p_pb_moveRight = new QPushButton();
    p_pb_moveLeftStep = new CustomButton();
    p_pb_moveRightStep = new CustomButton();
    p_pb_moveUp = new QPushButton();
    p_pb_moveUpStep = new CustomButton();
    p_pb_moveDownStep = new CustomButton();
    p_qt_pbTimer = new QTimer();
    p_qt_pbTimer->setSingleShot(true);

    p_recordPaper2 = new TrendRecordPaper(m_groupIndex, 0);

    p_pb_overviewTrend->setObjectName(QString("vertically"));
    p_pb_inforChar->setObjectName(QString("vertically"));
    p_pb_inforHand->setObjectName(QString("vertically"));

    slotPBTimerOut();
}

/*
 * 功能：
 * 	 释放函数，释放本窗体堆栈中所开辟的空间
 * 参数：
 * 	  无
 * 返回值：
 *      无
 */
void TrendDetailWidget::release()
{
    delete p_pb_overviewTrend;
    delete p_pb_inforChar;
    delete p_pb_inforHand;
    delete p_pb_moveRight;
    delete p_pb_moveLeftStep;
    delete p_pb_moveRightStep;
    delete p_pb_moveUp;
    delete p_pb_moveUpStep;
    delete p_pb_moveDownStep;

    delete p_qt_pbTimer;

    delete p_recordPaper2;
}

/*
 * 功能：
 * 	 设置风格函数，设置悬浮按钮大小位置，绘制窗体大小位置，手写透明层大小绘制
 * 参数：
 * 	  无
 * 返回值：
 *       无
 */
void TrendDetailWidget::setUserDefineStyle()
{
    int width = rect().width() /10;
    int height = rect().height() /10;
    p_pb_moveDownStep->resize(height, width);
    p_pb_moveUpStep->resize(height, width);
    p_pb_moveUp->resize(height, width);
    p_pb_moveLeftStep->resize(width, height);
    p_pb_moveRightStep->resize(width, height);
    p_pb_moveRight->resize(width,height);
    p_pb_overviewTrend->resize(width,height);
    p_pb_inforChar->resize(width,height);
    p_pb_inforHand->resize(width,height);

    p_pb_overviewTrend->move(0, rect().height()/3);
    p_pb_inforChar->move(0, rect().height()/3+p_pb_overviewTrend->size().height());
    p_pb_inforHand->move(0, rect().height()/3+p_pb_overviewTrend->size().height()+p_pb_inforChar->size().height());

    p_pb_moveLeftStep->move(rect().width()/6, rect().height()-p_pb_moveLeftStep->height());
    p_pb_moveRightStep->move(rect().width()/2,  rect().height()-p_pb_moveRightStep->height());
    p_pb_moveRight->move(rect().width() - p_pb_moveRight->width(), rect().height()-p_pb_moveRight->height());

    p_pb_moveUp->move(rect().width() - p_pb_moveUp->width(), 0);
    p_pb_moveUpStep->move(rect().width() - p_pb_moveUpStep->width(), rect().height()/2-p_pb_moveUpStep->height());
    p_pb_moveDownStep->move(rect().width() - p_pb_moveDownStep->width(), rect().height()*2/3);

    p_pb_moveLeftStep->setFlat(true);
    p_pb_moveRightStep->setFlat(true);
    p_pb_moveRight->setFlat(true);
    p_pb_moveUp->setFlat(true);
    p_pb_moveUpStep->setFlat(true);
    p_pb_moveDownStep->setFlat(true);

    setStyleSheet(QString("QPushButton#vertically{background-color:rgba(255,255,255,180);\
                                                                                       outline:none}\
                                            QLabel{background-color: white;\
                                                          border-width: 2px;\
                                                          border-style: solid;\
                                                          border-color: black;}"));

    p_recordPaper2->resize(rect().width(), rect().height());
}

/*
 * 功能：
 * 	 内容设置，控件text（）设置
 * 参数：
 * 	  无
 * 返回值：
 *       无
 */
void TrendDetailWidget::setUserDefineContent()
{
    QPixmap icon;
    icon.load(":/trend/Image/trend/overView.png");
    p_pb_overviewTrend->setIcon(QIcon(icon));
    p_pb_overviewTrend->setIconSize(icon.size());

    icon.load(":/trend/Image/trend/charInfor.png");
    p_pb_inforChar->setIcon(QIcon(icon));
    p_pb_inforChar->setIconSize(icon.size());

    icon.load(":/trend/Image/trend/handleInfor.png");
    p_pb_inforHand->setIcon(QIcon(icon));
    p_pb_inforHand->setIconSize(icon.size());

    icon.load(":/trend/Image/trend/toLeft_ee.png");
    p_pb_moveLeftStep->setIcon(QIcon(icon));
    p_pb_moveLeftStep->setIconSize(icon.size());

    icon.load(":/trend/Image/trend/toRight_ee.png");
    p_pb_moveRightStep->setIcon(QIcon(icon));
    p_pb_moveRightStep->setIconSize(icon.size());

    icon.load(":/trend/Image/trend/toStart_ee.png");
    p_pb_moveRight->setIcon(QIcon(icon));
    p_pb_moveRight->setIconSize(icon.size());

    icon.load(":/trend/Image/trend/toStart_up.png");
    p_pb_moveUp->setIcon(QIcon(icon));
    p_pb_moveUp->setIconSize(icon.size());

    icon.load(":/trend/Image/trend/toUpper.png");
    p_pb_moveUpStep->setIcon(QIcon(icon));
    p_pb_moveUpStep->setIconSize(icon.size());

    icon.load(":/trend/Image/trend/toDown.png");
    p_pb_moveDownStep->setIcon(QIcon(icon));
    p_pb_moveDownStep->setIconSize(icon.size());
}

/*
 * 功能：
 * 	 窗体控件布局设置函数
 * 参数：
 * 	  无
 * 返回值：
 *      无
 */
void TrendDetailWidget::setWinLayout()
{
    m_hlayout_ruler.addWidget(p_recordPaper2);
    m_hlayout_ruler.setContentsMargins(0,0,0,0);
    setLayout(&m_hlayout_ruler);
    p_pb_overviewTrend->setParent(this);
    p_pb_inforChar->setParent(this);
    p_pb_inforHand->setParent(this);
    p_pb_moveRight->setParent(this);
    p_pb_moveLeftStep->setParent(this);
    p_pb_moveRightStep->setParent(this);
    p_pb_moveUp->setParent(this);
    p_pb_moveUpStep->setParent(this);
    p_pb_moveDownStep->setParent(this);
}

/*
 * 功能：
 * 	 窗体中信号槽连接函数
 * 参数：
 * 	  无
 * 返回值：
 *      无
 */
void TrendDetailWidget::sigAndSlotConnect()
{
    connect(p_qt_pbTimer, SIGNAL(timeout()), this, SLOT(slotPBTimerOut()));

    connect(p_pb_overviewTrend, SIGNAL(clicked()), this, SLOT(slotPBOverviewTrendIsClicked()));
    connect(p_pb_inforChar, SIGNAL(clicked()), this, SLOT(slotPBInforCharIsClicked()));
    connect(p_pb_inforHand, SIGNAL(clicked()), this, SLOT(slotPBInforHandIsClicked()));
    connect(p_pb_moveLeftStep, SIGNAL(clicked(quint32)), this, SLOT(slotPBMoveLeftStepIsClicked(quint32)));
    connect(p_pb_moveRightStep, SIGNAL(clicked(quint32)), this, SLOT(slotPBMoveRightStepIsClicked(quint32)));
    connect(p_pb_moveRight, SIGNAL(clicked()), this, SLOT(slotPBMoveRightIsClicked()));
    connect(p_pb_moveDownStep, SIGNAL(clicked(quint32)), this, SLOT(slotPBMoveLeftStepIsClicked(quint32)));
    connect(p_pb_moveUpStep, SIGNAL(clicked(quint32)), this, SLOT(slotPBMoveRightStepIsClicked(quint32)));
    connect(p_pb_moveUp, SIGNAL(clicked()), this, SLOT(slotPBMoveRightIsClicked()));

    connect(p_pb_overviewTrend, SIGNAL(pressed()), this, SLOT(slotPBOverviewTrendIsPressed()));
    connect(p_pb_overviewTrend, SIGNAL(released()), this, SLOT(slotPBOverviewTrendIsReleased()));
    connect(p_pb_inforHand, SIGNAL(pressed()), this, SLOT(slotPBInforHandIsPressed()));
    connect(p_pb_inforHand, SIGNAL(released()), this, SLOT(slotPBInforHandIsReleased()));
    connect(p_pb_inforChar, SIGNAL(pressed()), this, SLOT(slotPBInforCharIsPressed()));
    connect(p_pb_inforChar, SIGNAL(released()), this, SLOT(slotPBInforCharIsReleased()));
}

/*
 * 功能：
 * 	 悬浮控件显示函数，鼠标按下时显示控件，同时计时器开始计时
 * 参数：
 * 	  无
 * 返回值：
 *       无
 */
void TrendDetailWidget::showControlPB()
{
    p_pb_overviewTrend->show();
    p_pb_inforChar->show();
    p_pb_inforHand->show();
    if (DevConfigPtr->displayTrend.trendDisplayDirection) {
        p_pb_moveUp->show();
        p_pb_moveUpStep->show();
        p_pb_moveDownStep->show();
    } else {
        p_pb_moveLeftStep->show();
        p_pb_moveRightStep->show();
        p_pb_moveRight->show();
    }
    p_qt_pbTimer->start(PB_Show_Time);
}

/*
 * 功能：
 * 	 计时timeout槽
 * 参数：
 * 	  无
 * 返回值:
 *       无
 */
void TrendDetailWidget::slotPBTimerOut()
{
    if (p_qt_pbTimer->isActive())
        p_qt_pbTimer->stop();
    p_pb_overviewTrend->hide();
    p_pb_inforChar->hide();
    p_pb_inforHand->hide();
    p_pb_moveLeftStep->hide();
    p_pb_moveRightStep->hide();
    p_pb_moveRight->hide();
    p_pb_moveUp->hide();
    p_pb_moveUpStep->hide();
    p_pb_moveDownStep->hide();
}

/*
 * 功能：
 * 	 总览窗体激活按钮响应槽
 * 参数：
 * 	  无
 * 返回值：
 *       无
 */
void TrendDetailWidget::slotPBOverviewTrendIsClicked()
{
    if (p_gCfgTrendGroup->getCurrentRTState() == StandBy)
        return;
    emit(sigOverviewTrend());
    p_qt_pbTimer->start(PB_Show_Time);
}

void TrendDetailWidget::slotPBOverviewTrendIsPressed()
{
    QPixmap icon;
    icon.load(":/trend/Image/trend/overView_press.png");
    p_pb_overviewTrend->setIcon(QIcon(icon));
    p_pb_overviewTrend->setIconSize(icon.size());
}

void TrendDetailWidget::slotPBOverviewTrendIsReleased()
{
    QPixmap icon;
    icon.load(":/trend/Image/trend/overView.png");
    p_pb_overviewTrend->setIcon(QIcon(icon));
    p_pb_overviewTrend->setIconSize(icon.size());
}

/*
 * 功能：
 * 	 手写信息窗体激活槽
 * 参数：
 * 	  无
 * 返回值：
 *       无
 */
void TrendDetailWidget::slotPBInforHandIsClicked()
{
    emit(sigInforHand());
    p_qt_pbTimer->start(PB_Show_Time);
}

void TrendDetailWidget::slotPBInforHandIsPressed()
{
    QPixmap icon;
    icon.load(":/trend/Image/trend/handleInfor_press.png");
    p_pb_inforHand->setIcon(QIcon(icon));
    p_pb_inforHand->setIconSize(icon.size());
}

void TrendDetailWidget::slotPBInforHandIsReleased()
{
    QPixmap icon;
    icon.load(":/trend/Image/trend/handleInfor.png");
    p_pb_inforHand->setIcon(QIcon(icon));
    p_pb_inforHand->setIconSize(icon.size());
}

/*
 * 功能：
 * 	 信息输入按钮响应槽
 * 参数：
 * 	  无
 * 返回值：
 *       无
 */
void TrendDetailWidget::slotPBInforCharIsClicked()
{
    emit(sigInforChar());
    p_qt_pbTimer->start(PB_Show_Time);
}

void TrendDetailWidget::slotPBInforCharIsPressed()
{
    QPixmap icon;
    icon.load(":/trend/Image/trend/charInfor_press.png");
    p_pb_inforChar->setIcon(QIcon(icon));
    p_pb_inforChar->setIconSize(icon.size());
}

void TrendDetailWidget::slotPBInforCharIsReleased()
{
    QPixmap icon;
    icon.load(":/trend/Image/trend/charInfor.png");
    p_pb_inforChar->setIcon(QIcon(icon));
    p_pb_inforChar->setIconSize(icon.size());
}

/*
 * 功能：
 * 	 趋势图形移动至结束点按钮响应槽
 * 参数：
 * 	  无
 * 返回值：
 *       无
 */
void TrendDetailWidget::slotPBMoveRightIsClicked()
{
    p_qt_pbTimer->start(PB_Show_Time);

    if (p_gCfgTrendGroup->getCurrentWorkMode() == BrowseHistory) {
        if (p_gCfgTrendGroup->isSameIDofRealAndHistory()) {
            p_gCfgTrendGroup->setWorkMode(RealTimeRecord);
        } else {
            p_gCfgTrendGroup->moveToBufRight();
        }
    }
}

/*
 * 功能：
 * 	 趋势图形向左移动按钮响应槽
 * 参数：
 * 	  无
 * 返回值：
 *       无
 */
void TrendDetailWidget::slotPBMoveLeftStepIsClicked(quint32 points)
{
    p_qt_pbTimer->start(PB_Show_Time);
    if (p_gCfgTrendGroup->getCurrentWorkMode() == RealTimeRecord) {
        p_gCfgTrendGroup->setWorkMode(BrowseHistory);
        p_gCfgTrendGroup->switchToHistoryRecordMode();
        return;
    } else {
        p_gCfgTrendGroup->moveHistoryDataCursor(-points);
    }
}

/*
 * 功能：
 * 	 趋势图形向右移动按钮响应槽
 * 参数：
 * 	  无
 * 返回值：
 *       无
 */
void TrendDetailWidget::slotPBMoveRightStepIsClicked(quint32 points)
{
    p_qt_pbTimer->start(PB_Show_Time);
    if (p_gCfgTrendGroup->getCurrentWorkMode() == RealTimeRecord) {
        return;
    } else {
        p_gCfgTrendGroup->moveHistoryDataCursor(points);
    }
}

/*
 * 功能：
 * 	 重写resizeEvent函数
 * 参数：
 * 	  QResizeEvent *e
 * 返回值：
 *       无
 */
void TrendDetailWidget::resizeEvent(QResizeEvent *e)
{
    setUserDefineStyle();
    QWidget::resizeEvent(e);
}


/*
 * 功能：
 * 	 重写鼠标按下事件函数，显示悬浮控件，记录鼠标按下坐标，窗体有鼠标按下
 * 参数：
 * 	  QMouseEvent *event
 * 返回值：
 *       无
 */
void TrendDetailWidget::mousePressEvent(QMouseEvent */*event*/)
{
    if (p_pb_inforChar->isHidden())
        showControlPB();
}

