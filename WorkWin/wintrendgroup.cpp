#include "wintrendgroup.h"
#include <QApplication>
#include <QMouseEvent>
#include "../WorkWin/wininfolistdialog.h"
#include "./WorkWin/winchannelinfodialog.h"

/*
 * 功能：
 * 	 趋势组显示窗体构造函数
 * 参数：
 * 	  quint32 index,            组号
 *       QWidget *parent       父窗体指针
 * 返回值：
 *       无
 */
WinTrendGroup::WinTrendGroup(quint32 index, QWidget *parent) :
    QWidget(parent),
    m_grpIndex(index)
{
    initialize();
    setUserDefineStyle();
    setWinLayout();
    sigAndSlotConnect();
}

/*
 * 功能：
 * 	 趋势组显示窗体设置组号
 * 参数：
 * 	  quint32 groupIndex,            组号
 * 返回值：
 *       无
 */
void WinTrendGroup::setGroupIndex(quint32 groupIndex)
{
    m_grpIndex = groupIndex;
    p_detailTrend->setGroupIndex(groupIndex);
    p_ruler->setGroupIndex(groupIndex);
    p_overviewWidget->setGroupIndex(groupIndex);
    p_digitalGroup->resetGroupIndex(groupIndex);
    emit sigChangeFrame(groupIndex);
}

void WinTrendGroup::resetTrendAllGroup()
{
    quint8 rulerNum = p_ruler->produceRulerMap();

    if (rulerNum != 0) {
        p_boxLayout->removeWidget(p_detailTrend);
        p_boxLayout->removeWidget(p_ruler);
        p_boxLayout->addWidget(p_detailTrend, 10);
        p_boxLayout->addWidget(p_ruler, rulerNum);
    }
    if (DevConfigPtr->displayTrend.trendDisplayDirection) {
        p_ruler->setRulerMoveRange(p_detailTrend->y(), p_detailTrend->height());
        p_boxLayout->setDirection(QBoxLayout::BottomToTop);
    } else {
        p_ruler->setRulerMoveRange(p_detailTrend->x(), p_detailTrend->width());
        p_boxLayout->setDirection(QBoxLayout::LeftToRight);
    }
    p_detailTrend->reCountDetailArg();
    p_ruler->reCountTruleArg();
}

/*
 * 功能：
 *     供事件模块调用，用来设置字符信息输入
 * 参数：
 *     int pos ,   字符信息的位置（详细参考横河设备）
 *     QString msg  字符信息的内容
 * 返回值：
 *       无
 */
void WinTrendGroup::setCharMsg(int pos, QString msg)
{
    p_detailTrend->slotDealCharMsg(QString::number(pos%10)+msg);
}

/*
 * 功能：
 *     当趋势组信息窗体被滑动，当前组发生变更时，消息输入工具需要重新载入
 * 参数：
 *     MessageBar *p, 消息输入工具指针
 * 返回值：
 *       无
 */
void WinTrendGroup::loadMessageBar(DisPlayMessage *p)
{
    p_messageBar = p;
    disconnect(p_messageBar,0,0,0);
    connect(p_messageBar, SIGNAL(sigSendMessage(QString)), p_detailTrend, SLOT(slotDealCharMsg(QString)));
}

/*
 * 功能：
 *     当趋势组信息窗体被滑动，当前组发生变更时，手写消息输入工具需要重新载入
 * 参数：
 *     HandWritePad *p, 手写消息输入工具指针
 * 返回值：
 *       无
 */
void WinTrendGroup::loadHandWritePad(HandWritePad *p)
{
    p_handWriting = p;
    disconnect(p_handWriting, 0,0,0);
    connect(p_handWriting, SIGNAL(sigHWMsg(QImage*,QString&,quint8, QPoint&)),
            p_detailTrend, SLOT(slotDealHWMsg(QImage*,QString&,quint8, QPoint&)));
}


/*
 * 功能：
 *     当趋势组相关变量初始化
 * 参数：
 *      无
 * 返回值：
 *       无
 */
void WinTrendGroup::initialize()
{
    p_detailTrend = new TrendDetailWidget(m_grpIndex);
    p_digitalGroup = new TrendDigitalGroup(m_grpIndex);
    p_overviewWidget = new TrendOverviewWidget(m_grpIndex);
    p_handWriting =  nullptr;
    p_messageBar = nullptr;
    p_ruler = new TrendRulerWidget(m_grpIndex);
    p_vLayout = new QVBoxLayout();
    p_boxLayout = new QBoxLayout(QBoxLayout::LeftToRight);
    p_gLayout = new QGridLayout();
    setMouseTracking(false);

    p_detailTrend->installEventFilter(this);
    p_digitalGroup->viewport()->installEventFilter(this);
    p_overviewWidget->installEventFilter(this);
    p_ruler->installEventFilter(this);

    m_isResponseModeSwitch = true;
    m_changedFlag = 2;
}

/*
 * 功能：
 *     当趋势组相关风格设定
 * 参数：
 *      无
 * 返回值：
 *       无
 */
void WinTrendGroup::setUserDefineStyle()
{
    p_overviewWidget->hide();
}

/*
 * 功能：
 *     当趋势组相关布局设定
 * 参数：
 *      无
 * 返回值：
 *       无
 */
void WinTrendGroup::setWinLayout()
{
    p_boxLayout->addWidget(p_detailTrend,10);
    p_boxLayout->addWidget(p_ruler,1);
    p_vLayout->addWidget(p_overviewWidget, 1);
    p_vLayout->addLayout(p_boxLayout, 2);
    p_gLayout->addLayout(p_vLayout, 0, 0);
    p_gLayout->addWidget(p_digitalGroup,0, 1);

    p_gLayout->setContentsMargins(0,0,0,0);
    p_gLayout->setSpacing(0);
    p_gLayout->setMargin(0);
    p_boxLayout->setContentsMargins(0,0,0,0);
    p_boxLayout->setSpacing(0);
    p_boxLayout->setMargin(0);
    p_vLayout->setContentsMargins(0,0,0,0);
    p_vLayout->setSpacing(0);
    p_vLayout->setMargin(0);

    setLayout(p_gLayout);
}

/*
 * 功能：
 *     当趋势组相关信号槽联接
 * 参数：
 *      无
 * 返回值：
 *       无
 */
void WinTrendGroup::sigAndSlotConnect()
{
    connect(p_overviewWidget, SIGNAL(sigHide()), this, SIGNAL(sigOverViewHide()));
    connect(p_detailTrend, SIGNAL(sigOverviewTrend()), this, SIGNAL(sigOverViewShow()));

    connect(p_detailTrend, SIGNAL(sigInforHand()), this, SLOT(slotShowHandWritingWidget()));
    connect(p_detailTrend, SIGNAL(sigInforChar()), this, SLOT(slotShowCharInfor()));

    connect(p_digitalGroup, SIGNAL(sigShowDigitalItemInfor(QModelIndex)), this, SLOT(slotDigitalItemInfor(QModelIndex)));

    connect(p_gCfgTrendGroup, SIGNAL(sigHistoryPointerChanged()), this, SLOT(slotHistoryPointerChanged()));

    connect(p_ruler, SIGNAL(sigWarningScaleImageLose()), this, SLOT(slotshowWarningDialog()));
}

/*
 * 功能：
 *     当趋势组总览窗体隐藏函数
 *        m_hideFlag 用来解决因为总览窗体隐藏，而导致标尺产生移动事件的Bug。
 * 参数：
 *      无
 * 返回值：
 *       无
 */
void WinTrendGroup::slotHideOverViewTrend()
{
    if (p_gCfgTrendGroup->getCurrentWorkMode()==BrowseHistory &&
        ((DevConfigPtr->displayTrend.trendDisplayDirection==0 && p_ruler->pos().x() >= (p_detailTrend->x() + p_detailTrend->width())) ||
         (DevConfigPtr->displayTrend.trendDisplayDirection && p_ruler->pos().y() <= (p_ruler->height()>>4))) &&
        p_gCfgTrendGroup->isSameIDofRealAndHistory() &&
            p_gCfgTrendGroup->getHistoryTrendBuf()->getRecordPointIndex() ==
            (p_gCfgTrendGroup->getHistoryTrendBuf()->getBufValidLen()-1)) {
        p_gCfgTrendGroup->setWorkMode(RealTimeRecord);
    }
    p_overviewWidget->hide();
}

/*
 * 功能：
 *     趋势总览窗体显示
 * 参数：
 *      无
 * 返回值：
 *       无
 */
void WinTrendGroup::slotShowQverViewTrend()
{
    p_gCfgTrendGroup->startOverviewTrend();
    p_overviewWidget->show();
}

/*
 * 功能：
 *     显示趋势手写信息输入工具窗体
 * 参数：
 *      无
 * 返回值：
 *       无
 */
void WinTrendGroup::slotShowHandWritingWidget()
{
    if (p_handWriting->isHidden()) {  //手写输入界面开启时，需要调整趋势模块布局与工作模式
        p_overviewWidget->hide();
        p_gCfgTrendGroup->setWorkMode(BrowseHistory);
        dragDigitalWidget(TrendDigitalGroup::RIGHT);
        p_handWriting->reSetdrawImage(p_detailTrend->width(),
                                      p_detailTrend->height(),p_detailTrend->pos());
        p_handWriting->showFullScreen();
    }
}

/*
 * 功能：
 *     显示趋势信息输入工具窗体
 * 参数：
 *      无
 * 返回值：
 *       无
 */
void WinTrendGroup::slotShowCharInfor()
{
    p_messageBar->show(false);
}

void WinTrendGroup::slotHistoryPointerChanged()
{
    if (p_overviewWidget->isVisible())
        p_overviewWidget->countPaintParameter();

    this->update();
}

void WinTrendGroup::slotshowWarningDialog()
{
    //TODO，警告number待调整
//    static WinInfoListDialog warningWin(800, this);
//    warningWin.show();

//    WinInfoListDialog::instance()->showMsg(800);
}

/*
 * 功能：
 *     该函数响应趋势数值模块中的数值item被双击后的动作
 *     用来show出通道信息窗体
 * 参数：
 *      quint32 no， 被双击的趋势数值item所标识的通道号。
 * 返回值：
 *       无
 */
void WinTrendGroup::slotDigitalItemInfor(QModelIndex index)
{
    QVariant value = index.data(ChannelValidity);
    if(value.isValid()){
        quint32 chanNum = value.toUInt();
        if(CHANNEL_TYPE(chanNum) == Channel_Type_DO){
            WinDOChannelDialog::instance().resetCurrentChannelNumber(chanNum,true);
            connect(&WinDOChannelDialog::instance(),SIGNAL(sigUnselectItem()),p_digitalGroup,SLOT(slotChannelUnselect()));
            WinDOChannelDialog::instance().show();
        }else{
            value = index.data(ChannelRangeType);
            if(value.isValid() && value.toUInt() != OFF){
                WinChannelInfoDialog::instance().resetCurrentChannelNumber(chanNum,true);
                connect(&WinChannelInfoDialog::instance(),SIGNAL(sigUnselectItem()),p_digitalGroup,SLOT(slotChannelUnselect()));
                WinChannelInfoDialog::instance().show();
            }
        }
    }
}

/*
 * 功能：
 *     趋势模块中数值模块被拖拽后，重新布局函数。
 * 参数：
 *      TrendDigitalGroup::Position state   标明趋势数值窗体位于总布局中的位置，上下左右
 * 返回值：
 *       无
 */
void WinTrendGroup::dragDigitalWidget(TrendDigitalGroup::Position state)
{
    p_gLayout->removeItem(p_vLayout);
    p_gLayout->removeWidget(p_digitalGroup);

    switch (state)
    {
    case TrendDigitalGroup::LEFT:   //left
        p_gLayout->setColumnMinimumWidth(0, 0);
        p_gLayout->setColumnMinimumWidth(1, rect().width()*0.8);
        p_gLayout->setRowMinimumHeight(0, 0);
        p_gLayout->setRowMinimumHeight(1,0);

        p_gLayout->addWidget(p_digitalGroup, 0, 0);
        p_gLayout->addLayout(p_vLayout, 0, 1);
        p_digitalGroup->resize(rect().width()*0.2, rect().height());
        p_digitalGroup->changePos(TrendDigitalGroup::LEFT);
        break;
    case TrendDigitalGroup::DOWN:   //down
        p_gLayout->setColumnMinimumWidth(0, 0);
        p_gLayout->setColumnMinimumWidth(1, 0);
        p_gLayout->setRowMinimumHeight(0, rect().height()*0.85);
        p_gLayout->setRowMinimumHeight(1,0);

        p_gLayout->addLayout(p_vLayout, 0, 0);
        p_gLayout->addWidget(p_digitalGroup, 1, 0);
        p_digitalGroup->resize(rect().width(), rect().height()*0.15);
        p_digitalGroup->changePos(TrendDigitalGroup::DOWN);

        break;
    case TrendDigitalGroup::RIGHT:   //right
        p_gLayout->setColumnMinimumWidth(0, rect().width()*0.8);
        p_gLayout->setColumnMinimumWidth(1, 0);
        p_gLayout->setRowMinimumHeight(0, 0);
        p_gLayout->setRowMinimumHeight(1,0);

        p_gLayout->addLayout(p_vLayout, 0, 0);
        p_gLayout->addWidget(p_digitalGroup, 0, 1);
        p_digitalGroup->resize(rect().width()*0.2, rect().height());
        p_digitalGroup->changePos(TrendDigitalGroup::RIGHT);
        break;
    case TrendDigitalGroup::UP:   //up
        p_gLayout->setColumnMinimumWidth(0, 0);
        p_gLayout->setColumnMinimumWidth(1, 0);
        p_gLayout->setRowMinimumHeight(0, 0);
        p_gLayout->setRowMinimumHeight(1, rect().height()*0.85);

        p_digitalGroup->resize(rect().width(), rect().height()*0.15);
        p_digitalGroup->changePos(TrendDigitalGroup::UP);
        p_gLayout->addLayout(p_vLayout, 1, 0);
        p_gLayout->addWidget(p_digitalGroup, 0, 0);
        break;
    default:
        break;
    }

    if (DevConfigPtr->displayTrend.trendDisplayDirection) {
        p_ruler->setRulerMoveRange(p_detailTrend->y(), p_detailTrend->height());
    } else {
        p_ruler->setRulerMoveRange(p_detailTrend->x(), p_detailTrend->width());
    }
}

/*
 * 功能：
 *     趋势模块重载resizeEvent处理函数。
 *     发生resize事件时，需重新布局趋势组窗体
 * 参数：
 *      QResizeEvent *e
 * 返回值：
 *       无
 */
void WinTrendGroup::resizeEvent(QResizeEvent *e)
{
    dragDigitalWidget(p_digitalGroup->getPosition());
    QWidget::resizeEvent(e);
}

/*
 * 功能：
 *     趋势模块重载mouseMoveEvent处理函数。
 *     判断用户拖拽趋势数值窗体移动到的方向
 * 参数：
 *      QMouseEvent *e
 * 返回值：
 *       无
 */
void WinTrendGroup::mouseMoveEvent(QMouseEvent *e)
{
    if (p_digitalGroup->isPress())
    {
        if (e->pos().x() < rect().width()*0.25) //left
        {
            dragDigitalWidget(TrendDigitalGroup::LEFT);
        }
        else if (e->pos().x() > rect().width()*0.75) //right
        {
            dragDigitalWidget(TrendDigitalGroup::RIGHT);
        }
        else if (e->pos().y() < rect().height()*0.25) //top
        {
            dragDigitalWidget(TrendDigitalGroup::UP);
        }
        else if (e->pos().y() > rect().height() * 0.75)//down
        {
            dragDigitalWidget(TrendDigitalGroup::DOWN);
        }
    }
    QWidget::mouseMoveEvent(e);
}

/*
 * 功能：
 *     趋势组窗体事件过滤处理函数重载
 *     趋势组窗体中所有的用户界面动作所引起的模式切换在该函数中处理
 * 参数：
 *      QMouseEvent *e
 * 返回值：
 *       无
 */
bool WinTrendGroup::eventFilter(QObject *object, QEvent *event)
{
    static QPointF prePos, curPos;
    TrendWorkMode workMode = p_gCfgTrendGroup->getCurrentWorkMode();
    //如果实时更新模式，同时是standby状态 则 不响应任何事件。
    if (workMode== RealTimeRecord && p_gCfgTrendGroup->getCurrentRTState() == StandBy)
        return false;

    if (m_isResponseModeSwitch == true) {
        if (event->type() == QEvent::MouseButtonPress) {
            if (object == p_detailTrend) {
                if (workMode == RealTimeRecord) {
                    p_gCfgTrendGroup->switchToHistoryRecordMode();
                    p_gCfgTrendGroup->setWorkMode(BrowseHistory);
                    p_digitalGroup->update();
                }

                prePos = (dynamic_cast<QMouseEvent*>(event))->localPos();
                return false;
            }

            if (object == p_overviewWidget) {
                QPointF pressPoint = (dynamic_cast<QMouseEvent*>(event))->localPos();
                if (p_overviewWidget->getCurDataFrameRect().contains(pressPoint)) {
                    p_overviewWidget->setMoveFlag(true);
                    p_overviewWidget->setPressPoint(pressPoint);
                }
            }

            if (object == p_ruler) {
                p_detailTrend->slotPBTimerOut();
                p_ruler->setBackgroundAlpha(0x80);
            }

            if (object == p_ruler || object == p_overviewWidget)
                return true;
        }
        if (event->type() == QEvent::MouseButtonRelease) {
            if (object == p_detailTrend || object == p_ruler) {
                if (workMode == BrowseHistory &&  p_overviewWidget->isHidden() &&
                    p_gCfgTrendGroup->isSameIDofRealAndHistory() &&
                    p_gCfgTrendGroup->getHistoryTrendBuf()->getRecordPointIndex() ==
                    (p_gCfgTrendGroup->getHistoryTrendBuf()->getBufValidLen()-1) &&
                    ((DevConfigPtr->displayTrend.trendDisplayDirection==0 && p_ruler->pos().x() >= (p_detailTrend->x() + p_detailTrend->width())) ||
                     (DevConfigPtr->displayTrend.trendDisplayDirection && p_ruler->pos().y() <= (p_ruler->height()>>4)))) {
                    p_gCfgTrendGroup->setWorkMode(RealTimeRecord);
                }

                p_digitalGroup->update();
            }

            if (object == p_ruler) {
                p_ruler->setBackgroundAlpha(0x10);
            }

            if (object == p_overviewWidget) {
                p_overviewWidget->setMoveFlag(false);
            }
            return false;
        }
        if (event->type() == QEvent::MouseMove) {
            if (object == p_detailTrend) {  //如果趋势绘制页面被拖拽 则 标尺需要绘制需要及时更新
                curPos = (dynamic_cast<QMouseEvent *>(event))->localPos();
                qreal xdiff = curPos.x() - prePos.x();
                qreal ydiff = curPos.y() - prePos.y();
                if (DevConfigPtr->displayTrend.trendDisplayDirection) { //垂直显示
                    if (qAbs(xdiff) < qAbs(ydiff)) {
                        p_gCfgTrendGroup->moveHistoryDataCursor(ydiff/p_detailTrend->getRecordPaper()->getPointGap());
                    }
                } else {    //水平显示
                    if (qAbs(xdiff) > qAbs(ydiff)) {
                        p_gCfgTrendGroup->moveHistoryDataCursor(-xdiff/p_detailTrend->getRecordPaper()->getPointGap());
                    }
                }
                prePos = curPos;
                p_ruler->update();
                p_digitalGroup->update();
                return false;
            }
        }
        if (event->type() == QEvent::Move){
            if (object == p_ruler) {
                p_digitalGroup->update();
                p_ruler->update();
            }
            return false;
        }
    }
    return QWidget::eventFilter(object, event);
}

//由于多屏下趋势与趋势模块时两个不同界面共享同一份数据，故而所有绘制相关
//参数在界面切换时需要重新计算，showEvent函数与resizeEvent不适用。
void WinTrendGroup::paintEvent(QPaintEvent *e)
{
    if (m_changedFlag != 0) {   //第一次显示坐标有问题，以第2次为准
        m_changedFlag--;
        resetTrendAllGroup();
    }

    QPainter painter(this);
    if (DevConfigPtr->displayBasic.basicBackground) {
        painter.fillRect(this->rect(), QBrush(Qt::black));
    } else {
        painter.fillRect(this->rect(), QBrush(Qt::white));
    }

    QWidget::paintEvent(e);
}
