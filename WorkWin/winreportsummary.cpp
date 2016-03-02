#include "winreportsummary.h"
#include <QHeaderView>
#include <QTimer>
#include "winreport.h"
#include "./Common/authorization.h"
#include "./WorkWin/wininfolistdialog.h"
#include "winsummarydialog.h"

/*****************************************************************************
 *
 * 界面类
 *  主要包含界面的组件，组件的信号和槽函
 *
 * ****************************************************************************/

WinReportSummary::WinReportSummary(QWidget *parent) : QWidget(parent), m_btnPress(-1)
{
    initReportSummaryUI();

    signalAndSlotConnect();
}

WinReportSummary::~WinReportSummary()
{
    delete p_timer;
    delete p_downBtn;
    delete p_upBtn;
    delete p_view;
    delete p_gridLayout;
}


/*
 * 界面组件布局与属性
 */
void WinReportSummary::initReportSummaryUI()
{
    p_gridLayout = new QGridLayout(this);
    p_gridLayout->setSpacing(0);
    p_gridLayout->setContentsMargins(0, 0, 0, 0);

    p_view = new ReportTableView(this);
    p_view->setObjectName(QStringLiteral("p_view"));
    p_view->setBaseSize(QSize(0, 0));
    p_view->setShowGrid(false);   //设置不显示网格
    p_view->setSortingEnabled(true);
    p_view->horizontalHeader()->setStretchLastSection(true);  //最后一列自动补齐到界面最后面
    p_view->verticalHeader()->setStretchLastSection(false);
    p_view->resizeColumnsToContents();  //设置列宽随内容改变
    p_view->verticalHeader()->hide();    //隐藏列头
    p_view->horizontalHeader()->setDefaultAlignment(Qt::AlignVCenter);
    p_view->horizontalHeader()->setHighlightSections(false);
    p_view->setSelectionBehavior(QAbstractItemView::SelectRows);

    p_upBtn = new QPushButton(this);
    p_upBtn->setObjectName(QStringLiteral("p_upBtn"));
    p_upBtn->setGeometry(QRect(0, 5, 45, 28));
    p_upBtn->setMaximumSize(QSize(50, 16777215));
    p_upBtn->setIcon(QIcon(":/summary/Image/summary/up.png"));
    p_upBtn->setIconSize(QSize(45, 28));
    p_upBtn->setFocusPolicy(Qt::NoFocus);
    p_upBtn->hide();

    p_downBtn = new QPushButton(this);
    p_downBtn->setObjectName(QStringLiteral("p_downBtn"));
    p_downBtn->setGeometry(QRect(45, 5, 90, 28));
    p_downBtn->setMaximumSize(QSize(50, 16777215));
    p_downBtn->setIcon(QIcon(":/summary/Image/summary/down.png"));
    p_downBtn->setIconSize(QSize(45, 28));
    p_downBtn->setFocusPolicy(Qt::NoFocus);
    p_downBtn->hide();
    p_downBtn->setAutoRepeatInterval(200);

    p_headerLabel = new QLabel(this);
    p_headerLabel->setObjectName(QStringLiteral("p_headerLabel"));
    p_headerLabel->setGeometry(QRect(100, 0, 400, 20));

    setStyleSheet("QPushButton {border: none;}");

    p_gridLayout->addWidget(p_view, 0, 0, 1, 1);
}

/*
 * 信号和槽函数的绑定
 */
void WinReportSummary::signalAndSlotConnect()
{
    p_timer = new QTimer(this);
    connect(p_timer, SIGNAL(timeout()), this, SLOT(slotTimeOut()));

    connect(p_upBtn, SIGNAL(pressed()), this, SLOT(slotUpBtnPressed()));
    connect(p_upBtn, SIGNAL(released()), this, SLOT(slotUpBtnRelease()));

    connect(p_downBtn, SIGNAL(pressed()), this, SLOT(slotDownBtnPressed()));
    connect(p_downBtn, SIGNAL(released()), this, SLOT(slotDownBtnRelease()));
}

/*
 * upBtn被按下的槽函数
 *      按下后注册键盘key_up事件，并发送该事件，从而达到和按键盘key_up一样的效果
 */
void WinReportSummary::slotUpBtnPressed()
{
    m_btnPress = 0;
    p_view->verticalScrollBar()->setValue(p_view->verticalScrollBar()->value() - p_view->height() / 12);
    p_timer->start(50);
}


/*
 * upBtn松开
 */
void WinReportSummary::slotUpBtnRelease()
{
    p_timer->stop();
}

/*
 * p_downBtn被按下的槽函数
 *      按下后注册键盘key_down事件，并发送该事件，从而达到和按键盘key_down一样的效果
 */
void WinReportSummary::slotDownBtnPressed()
{
    m_btnPress = 1;
    p_view->verticalScrollBar()->setValue(p_view->verticalScrollBar()->value() + p_view->height() / 12);
    p_timer->start(50);
}

/*
 * p_downBtn松开
 */
void WinReportSummary::slotDownBtnRelease()
{
    p_timer->stop();
}


/*
 * 监控上/下按钮被按下的定时器槽函数(模拟键盘的上下键操作)
 *  btnPress : 默认为-1; 0 : 表示上按钮(upBtn)被按下; 1 : 表示下按钮(p_downBtn)被按下
 *
 */
void WinReportSummary::slotTimeOut()
{
    if (m_btnPress == 0)
    {
        emit p_upBtn->pressed();
    }
    else if (m_btnPress == 1)
    {
        emit p_downBtn->pressed();
    }
}





/*
 * 重写tableview,主要是为了在代理那边得到自定义的闪烁状态
 *      m_isBlink = true 显示图片
 *      m_isBlink = false 不显示图片
*/
ReportTableView::ReportTableView(QWidget *parent) : QTableView(parent),
    m_isPress(false), m_pos(0, 0), m_drag(false), m_point(0, 0)
{
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    this->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->setSelectionMode (QAbstractItemView::SingleSelection);
    this->setShowGrid(false);   //设置不显示网格
    this->setSortingEnabled(true);
    this->horizontalHeader()->setStretchLastSection(true);  //最后一列自动补齐到界面最后面
    this->verticalHeader()->setStretchLastSection(false);
    this->resizeColumnsToContents();  //设置列宽随内容改变
    this->verticalHeader()->hide();    //隐藏列头
    this->horizontalHeader()->setDefaultAlignment(Qt::AlignVCenter);
    this->horizontalHeader()->setHighlightSections(false);

    this->horizontalHeader()->setStyleSheet("QHeaderView::section {\
            color: black;\
            background-color: rgb(240, 240, 240);\
            border-bottom: 1px solid #f6f7fa;\
            border-radius:0px;\
            border-color: rgb(180, 180, 180);\
            }");
    this->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);

    /*
     * 设置滚动条属性
     */
    this->verticalScrollBar()->setStyleSheet("QScrollBar{background:lightgray; width: 4px;}"
         "QScrollBar::handle{background:green; border:0px solid transparent; border-radius:2px;}"
         "QScrollBar::handle:hover{background:gray;}"
         "QScrollBar::sub-line{background:transparent;}"
         "QScrollBar::add-line{background:transparent;}");
    connect(CfgChannel::instance(), SIGNAL(sigNormalConfigChanged(quint32)), this, SLOT(slotBackgroundChanged(quint32)));
    slotBackgroundChanged(ID_Disp_Basic_Background);
}



void ReportTableView::slotBackgroundChanged(quint32 ID)
{
    if (ID != ID_Disp_Basic_Background)
        return ;
    if (CfgChannel::instance()->getSysBackgroundColor()) {
          this->setStyleSheet("QTableView{background-color: rgb(0, 0, 0);color:#404040;\
                              selection-background-color:qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 rgb(0,187,255),\
                                                                             stop:1 rgb(0,187,255));\
              }");
      } else {
          this->setStyleSheet("QTableView{background-color: rgb(255, 255, 255);color:#404040;\
                              selection-background-color:qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 rgb(0,187,255),\
                                                                             stop:1 rgb(0,187,255));\
              }");
      }
}


void ReportTableView::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        m_pos = event->pos();
        m_point = event->pos();
        m_drag = true;
    }
    QTableView::mousePressEvent(event);
}


void ReportTableView::mouseMoveEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton || m_drag){
        int span =  m_pos.y() - event->y();
        this->verticalScrollBar()->setValue(this->verticalScrollBar()->value() + span);
        m_pos = event->pos();
    }else{
        m_drag = true;
        m_pos = event->pos();
    }
    QTableView::mouseMoveEvent(event);
}

void ReportTableView::mouseReleaseEvent(QMouseEvent *event)
{
    QPoint pos = event->pos();
    int rx = m_point.x() - pos.x();
    int ry = m_point.y() - pos.y();
    if(rx>-20 && rx<20 && ry>-20 && ry<20){
        showMsgDlg(pos);
    }
    m_pos = event->pos();
    m_point = event->pos();
    m_drag = false;

    QTableView::mouseReleaseEvent(event);
}

void ReportTableView::showMsgDlg(QPoint pos)
{
    if(Authorization::instance()->getLoginStatus() == Authorization::Auth_LoginStatus_Logout){
        WinInfoListDialog::instance()->showMsg(818);    //818 :请使用MENU 键登录。Press [MENU] key to login.
    }else{
        QModelIndex index = indexAt(pos);
        if (index.isValid()){
            WinSummaryDialog dlg(Summary_Report_Type, index);
            dlg.exec();
        }
    }
}







ReportSlidingScreen::ReportSlidingScreen(QGraphicsView *parent, bool isEnabledXDir, bool isEnabledYDir) :
    SlidingScreen(parent, isEnabledXDir, isEnabledYDir), m_sceneCnt(0)
{
}

/*
 * 功能：
 * 	  判断滑动距离滑动方向，并发射信号激活激活状态
 * 参数：
 * 返回值：
 */
void ReportSlidingScreen::mouseReleaseEvent(QMouseEvent *event)
{
    if (!m_enable || !m_enableSliding || p_indexBuf->length() == 1||m_slidingTimer.remainingTime() <= 0)
    {
        QGraphicsView::mouseReleaseEvent(event);
        return;
    }
    m_releasePos = event->pos();
    if ((qAbs(m_releasePos.x() - m_pressPos.x()) < qAbs(m_releasePos.y() - m_pressPos.y())) && m_isEnableYDir) //means y轴上的移动
    {
        if ((m_releasePos.y() - m_pressPos.y())>20)
        {
            if (m_isXDirection == false)
            {
                updateFrame(false);
                emit sigMoveDown();
            }
            else if (m_isXDirection == true)//如果是x轴和y轴方向切换 则发两次信号 即做两次状态切换。
            {
                emit sigMoveDown();
                updateFrame(false);
                emit sigMoveDown();
            }
            m_isXDirection = false;
            m_enableSliding = false;
        }
        else if ((m_releasePos.y() - m_pressPos.y())< -20)
        {
            if (m_isXDirection == false)
            {
                updateFrame(true);
                emit sigMoveUp();
            }
            else if (m_isXDirection == true) //如果是x轴和y轴方向切换 则发两次信号 即做两次状态切换。
            {
                emit sigMoveUp();            //第一次信号调整方向至水平方向无动画效果
                updateFrame(true);
                emit sigMoveUp();            //第二次信号移动窗体
            }
            m_isXDirection = false;
            m_enableSliding = false;
        }
    }
    else if ((qAbs(m_releasePos.x() - m_pressPos.x()) > qAbs(m_releasePos.y() - m_pressPos.y())) && m_isEnableXDir)//x轴上的移动
    {
        if ((m_releasePos.x() - m_pressPos.x())>20)
        {
            if (m_isXDirection == true)
            {
                updateFrame(false);
                emit sigMoveRight();
            }
            else if (m_isXDirection == false)//如果是x轴和y轴方向切换 则发两次信号 即做两次状态切换。
            {
                emit sigMoveRight();
                updateFrame(false);
                emit sigMoveRight();
            }
            emit sigMoveToPre();
            m_isXDirection = true;
            m_enableSliding = false;
        }
        else if ((m_releasePos.x() - m_pressPos.x())< -20)
        {
            if (m_isXDirection == true)
            {
                updateFrame(true);
                emit sigMoveLeft();
            }
            else if (m_isXDirection == false)//如果是x轴和y轴方向切换 则发两次信号 即做两次状态切换。
            {
                emit sigMoveLeft();
                updateFrame(true);
                emit sigMoveLeft();
            }
            emit sigMoveToNext();
            m_isXDirection = true;
            m_enableSliding = false;
        }
    }
    QGraphicsView::mouseReleaseEvent(event);
}

/*
 * 功能：
 * 	  计算下一个状态下三个frame各自需要存放的窗体指针，并装载之。
 * 参数：
 *    bool indexAdd  指明当前p_indexBuf内元素索引变化方向（递增or递减)
 * 返回值：
 * 	  int
 *    0 成功
 */
int ReportSlidingScreen::updateFrame(bool indexAdd)
{
    m_currentFrameIndexl = p_frame1->objectName().toInt();
    m_currentWidgetIndex = p_indexBuf->indexOf(m_layOut[m_currentFrameIndexl]->objectName().toInt());

    int newWidgetIndex = m_currentWidgetIndex;   //新的窗体的索引在p_indexBuf中的index号。
    int needUpdateFrameIndex = m_currentFrameIndexl;

    int step = 2;
    if (p_indexBuf->length() == 2)       //如果是2屏下则三屏动，show 2屏内容。
    {
        indexAdd = !indexAdd;
        step = 1;
    }

    if (indexAdd == true){  //indexAdd = true means 滑屏方向为 索引递增方向
        newWidgetIndex = (m_currentWidgetIndex+step) % p_indexBuf->length(); //后一位
        needUpdateFrameIndex = (m_currentFrameIndexl+2) % 3;
    }
    else if(indexAdd == false){ //indexAdd = false means 滑屏方向为 索引递减方向
        newWidgetIndex = (m_currentWidgetIndex+p_indexBuf->length()-step) % p_indexBuf->length(); // 前两位
        needUpdateFrameIndex = (m_currentFrameIndexl+1) % 3;
    }

    if (p_indexBuf->length() == 2)     //2屏模式下有两屏内容完全一样。
    {
        int otherFrameIndex = 3 - needUpdateFrameIndex - p_frame1->objectName().toInt();
        if (!m_layOut[otherFrameIndex]->objectName().isEmpty())
            m_layOut[otherFrameIndex]->removeWidget(m_layOut[otherFrameIndex]->currentWidget());
        m_layOut[otherFrameIndex]->addWidget(p_widgetBuf->at(p_indexBuf->at(newWidgetIndex)));
        m_layOut[otherFrameIndex]->setObjectName(QString::number(p_indexBuf->at(newWidgetIndex)));
    }
    m_layOut[needUpdateFrameIndex]->removeWidget(m_layOut[needUpdateFrameIndex]->currentWidget());
    m_layOut[needUpdateFrameIndex]->addWidget(p_widgetBuf->at(p_indexBuf->at(newWidgetIndex)));
    m_layOut[needUpdateFrameIndex]->setObjectName(QString::number(p_indexBuf->at(newWidgetIndex)));
    return 0;
}

int ReportSlidingScreen::addWidgetItem(int widgetIndex, QWidget *pw)
{
    m_sceneCnt++;
    return SlidingScreen::addWidgetItem(widgetIndex, pw);
}

