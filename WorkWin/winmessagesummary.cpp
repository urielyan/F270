#include "winmessagesummary.h"
#include "winmemsummary.h"

#include <QDateTime>
#include <QDebug>
#include <QHeaderView>
#include <QMessageBox>
#include <QScrollBar>
#include "./Threads/threaddatarecord.h"
#include "windialog.h"
#include "./WorkWin/winsummarydialog.h"
#include <QScroller>
#include <QEasingCurve>
#include "./Common/authorization.h"
#include "./WorkWin/wininfolistdialog.h"

#define TYPE_COLUMN             0
#define TIME_COLUMN             1
#define GROUP_COLUMN            2

#define MAX_COUNT               450


extern QString spaceStr;
extern QString getTimeFormate();

/*
 * 信息一览类
 */
WinMessageSummary::WinMessageSummary(QWidget *parent) :
    QWidget(parent)
{
    m_messageSummary.setParent(this);

    p_modelMessage = new ModelSummaryMessage();
    setViewStyle();
    sumRow = p_modelMessage->rowCount();

    initFrameHeader();
    resetColumnSize();
    initModelHeaderData();
    m_messageSummary.p_view->setModel(p_modelMessage);

    signalAndSlotConnect();
}

WinMessageSummary::~WinMessageSummary()
{
    delete p_modelMessage;
}

/**
 * @brief WinMessageSummary::initFrameHeader
 */
void WinMessageSummary::initFrameHeader()
{
    for (int i = 0; i < 2; i++)
    {
        m_frameHeader[i].setParent(m_messageSummary.p_view);
        m_frameHeader[i].setFrameShape(QFrame::VLine);
        m_frameHeader[i].setFrameShadow(QFrame::Sunken);
        m_frameHeader[i].hide();
    }
}

/*
 * 信号和槽函数的绑定
 */
void WinMessageSummary::signalAndSlotConnect()
{
    connect(m_messageSummary.p_view, SIGNAL(clicked(QModelIndex)), this, SLOT(slotDetailMessage(QModelIndex)));
    connect(m_messageSummary.p_view->horizontalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(slotSectionSort(int)));
}

/*
 * 重写界面大小变化函数，实现主界面大小的改变，MVC模型相应的变化
 */
void WinMessageSummary::resizeEvent(QResizeEvent */*event*/)
{
    m_messageSummary.resize(QSize(this->width(), this->height()));
    resetColumnSize();
    initModelHeaderData();
    /* 设置每行高度 */
    m_messageSummary.p_view->verticalHeader()->setDefaultSectionSize(this->height() / 12);

    setVline();
}



/**
 * @brief line
 * @每个表头旁边的竖线
 */
void WinMessageSummary::setVline()
{
    int widthLocal[2];
    widthLocal[0] = m_messageSummary.p_view->columnWidth(TYPE_COLUMN);
    widthLocal[1] = widthLocal[0] + m_messageSummary.p_view->columnWidth(TIME_COLUMN);

    for (int i = 0; i < 2; i++)
    {
        m_frameHeader[i].setGeometry(QRect(widthLocal[i] - 6, 6, 4, m_messageSummary.p_view->horizontalHeader()->height() - 12 ));
    }
}



/*
 * 点击某行，显示该行相信信息的槽函数
 *  need to modify
 */
void WinMessageSummary::slotDetailMessage(QModelIndex index)
{
    if(Authorization::instance()->getLoginStatus() == Authorization::Auth_LoginStatus_Logout){
        WinInfoListDialog::instance()->showMsg(818);    //818 :请使用MENU 键登录。Press [MENU] key to login.
    }else{
        WinSummaryDialog dialog(Summary_Message_Type, index, true, this);
        dialog.exec();
    }
}

void WinMessageSummary::slotSectionSort(int section)
{
    m_messageSummary.p_view->setCurrentIndex(m_messageSummary.p_view->model()->index(0, section));
}



/*
 * 设置view的属性和代理
 */
void WinMessageSummary::setViewStyle()
{
    m_messageSummary.p_view->setItemDelegate(new MessageSummaryDelegate(m_messageSummary.p_view));
}



/*
 * view的表头和upBtn与p_downBtn的隐藏与显示
 */
void WinMessageSummary::initModelHeaderData()
{
    int width = this->width();
    int height = this->height();
    int heightHeader;
    QFont font;

    if (width > DESKTOP_WIDTH * 0.75)
    {
        setHeadDataString();
        m_messageSummary.p_headerLabel->setGeometry(100, 0, 400, 20);
        if (height > DESKTOP_HEIGHT * 0.75)
        {
            font.setPixelSize(height / 16 * 0.4);
            heightHeader = 50;
        }
        else/* if (height > DESKTOP_HEIGHT * 0.33 && height <= DESKTOP_HEIGHT * 0.75)*/
        {
            font.setPixelSize(height / 13 * 0.5);
            heightHeader = 45;
        }
        p_modelMessage->setHeaderData(GROUP_COLUMN, Qt::Horizontal, tr("Group"));
        ((MessageSummaryDelegate *)m_messageSummary.p_view->itemDelegate())->setTimeFormate(getTimeFormate());
    }
    else if (width > DESKTOP_WIDTH * 0.35 && width <= DESKTOP_WIDTH * 0.75)
    {
        m_messageSummary.p_headerLabel->setGeometry(width / 13, 0, 400, 20);

        if (height > DESKTOP_HEIGHT * 0.75)
        {
            font.setPixelSize(height / 23 * 0.5);
            setHeadDataString();
            p_modelMessage->setHeaderData(TYPE_COLUMN, Qt::Horizontal, tr("Message"));
            m_messageSummary.p_headerLabel->setGeometry(0, 0, 400, 20);
            heightHeader = 50;
        }
        else if (height > DESKTOP_HEIGHT * 0.35 && height <= DESKTOP_HEIGHT * 0.75)
        {
            font.setPixelSize(height / 13 * 0.5);
            setHeadDataString();
            p_modelMessage->setHeaderData(TYPE_COLUMN, Qt::Horizontal, tr("Message"));
            m_messageSummary.p_headerLabel->setGeometry(0, 0, 400, 20);
            heightHeader = 45;
        }
        else
        {
            font.setPixelSize(height / 12 * 0.8);
            setHeadDataNull();
            heightHeader = 20;
        }
        ((MessageSummaryDelegate *)m_messageSummary.p_view->itemDelegate())->setTimeFormate(getTimeFormate());
    }
    else
    {
        m_messageSummary.p_headerLabel->setGeometry(width / 13, 0, 400, 20);
        if (height > DESKTOP_HEIGHT * 0.35 && height <= DESKTOP_HEIGHT * 0.75)
        {
            font.setPixelSize(height / 16 * 0.6);
            setHeadDataString();
            p_modelMessage->setHeaderData(TYPE_COLUMN, Qt::Horizontal, tr("Message"));
            m_messageSummary.p_headerLabel->setGeometry(0, 0, 400, 20);
            heightHeader = 45;
        }
        else
        {
            font.setPixelSize(height / 15);
            setHeadDataNull();
            heightHeader = 20;
        }
        ((MessageSummaryDelegate *)m_messageSummary.p_view->itemDelegate())->setTimeFormate(getTimeFormate().right(8));
    }

    m_messageSummary.p_view->verticalHeader()->setDefaultSectionSize(height / 13);
    m_messageSummary.p_view->horizontalHeader()->setFixedHeight(heightHeader);
    m_messageSummary.p_view->horizontalHeader()->setFont(font);
    m_messageSummary.p_headerLabel->setFont(font);
    m_messageSummary.p_view->setFont(font);
}


void WinMessageSummary::setHeadDataNull()
{
    m_messageSummary.p_view->horizontalHeader()->setSortIndicatorShown(false);
    p_modelMessage->setHeaderData(GROUP_COLUMN, Qt::Horizontal, "");
    p_modelMessage->setHeaderData(TYPE_COLUMN, Qt::Horizontal, "");
    p_modelMessage->setHeaderData(TIME_COLUMN, Qt::Horizontal, "");
}

void WinMessageSummary::setHeadDataString()
{
    p_modelMessage->setHeaderData(GROUP_COLUMN, Qt::Horizontal, tr("Grp"));
    p_modelMessage->setHeaderData(TYPE_COLUMN, Qt::Horizontal, spaceStr + tr("Message"));
    p_modelMessage->setHeaderData(TIME_COLUMN, Qt::Horizontal, tr("Data time"));
}

void WinMessageSummary::resetColumnSize()
{
    if (this->width() > DESKTOP_WIDTH * 0.70)
    {
        m_messageSummary.p_view->setColumnWidth(TYPE_COLUMN, this->width() / 3);
        m_messageSummary.p_view->setColumnWidth(TIME_COLUMN, this->width() / 3);
        m_messageSummary.p_downBtn->show();
        m_messageSummary.p_upBtn->show();
        for (int i = 0; i < 2; i++)
            m_frameHeader[i].show();
    }
    else if (this->width() > DESKTOP_WIDTH * 0.35 && this->width() <= DESKTOP_WIDTH * 0.70 )
    {
        m_messageSummary.p_view->setColumnWidth(TYPE_COLUMN, this->width() / 12 * 5);
        m_messageSummary.p_view->setColumnWidth(TIME_COLUMN, this->width() / 12 * 5);
        m_messageSummary.p_downBtn->hide();
        m_messageSummary.p_upBtn->hide();
        for (int i = 0; i < 2; i++)
            m_frameHeader[i].hide();
    }
    else
    {
        m_messageSummary.p_view->setColumnWidth(TYPE_COLUMN, this->width() / 2);
        m_messageSummary.p_view->setColumnWidth(TIME_COLUMN, this->width() / 3);
        m_messageSummary.p_downBtn->hide();
        m_messageSummary.p_upBtn->hide();
        for (int i = 0; i < 2; i++)
            m_frameHeader[i].hide();
    }
}


void WinMessageSummary::paintEvent(QPaintEvent *event)
{
    if (this->width() > DESKTOP_WIDTH * 0.35)
        ((MessageSummaryDelegate *)(m_messageSummary.p_view->itemDelegate()))->setTimeFormate(getTimeFormate());
    else
        ((MessageSummaryDelegate *)(m_messageSummary.p_view->itemDelegate()))->setTimeFormate(getTimeFormate().right(8));
    if (p_modelMessage->getMaxId() != p_modelMessage->m_maxId)
    {
        p_modelMessage->init();
        sumRow = p_modelMessage->rowCount();
    }
    int currentrow = m_messageSummary.p_view->rowAt(m_messageSummary.p_view->y());
    setCurrentRow(currentrow + 12, sumRow);
    QWidget::paintEvent(event);
}


/*
 * 设置当前行与总行数的信息
*/
void WinMessageSummary::setCurrentRow(int current, int sumrow)
{
    if (current < 11)
        current = 11;
    if (sumrow < 11)
        current = sumrow;
    if (this->height() < DESKTOP_HEIGHT * 0.35)
        m_messageSummary.p_headerLabel->setText(QString("%1/%2\t%3").arg(current, 4, 10, QChar('0'))
                                               .arg(sumrow, 4, 10, QChar('0'))
                                               .arg(tr("Message summary")));
    else
        m_messageSummary.p_headerLabel->setText(QString("%1/%2").arg(current, 4, 10, QChar('0')).arg(sumrow, 4, 10, QChar('0')));
}



/*****************************************************************************
 *
 * 界面类
 *  主要包含界面的组件，组件的信号和槽函
 *
 * ****************************************************************************/

SummaryUI::SummaryUI(QWidget *parent) : QWidget(parent)
{
    initMessageSummaryUI();

    signalAndSlotConnect();
}

SummaryUI::~SummaryUI()
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
void SummaryUI::initMessageSummaryUI()
{
    p_gridLayout = new QGridLayout(this);
    p_gridLayout->setSpacing(0);
    p_gridLayout->setObjectName(QStringLiteral("p_gridLayout"));
    p_gridLayout->setContentsMargins(0, 0, 0, 0);

    p_view = new SummaryTableView(this);
    p_view->setObjectName(QStringLiteral("p_view"));

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

    p_headerLabel = new QLabel(this);
    p_headerLabel->setObjectName(QStringLiteral("p_headerLabel"));
    p_headerLabel->setGeometry(QRect(100, 0, 400, 20));

    setStyleSheet("QPushButton {border: none;}");

    p_gridLayout->addWidget(p_view, 0, 0, 1, 1);
}

/*
 * 信号和槽函数的绑定
 */
void SummaryUI::signalAndSlotConnect()
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
void SummaryUI::slotUpBtnPressed()
{
    m_btnPress = 0;
    p_view->verticalScrollBar()->setValue(p_view->verticalScrollBar()->value() - p_view->height() / 12);
    p_timer->start(50);
}


/*
 * upBtn松开
 */
void SummaryUI::slotUpBtnRelease()
{
    p_timer->stop();
}

/*
 * p_downBtn被按下的槽函数
 *      按下后注册键盘key_down事件，并发送该事件，从而达到和按键盘key_down一样的效果
 */
void SummaryUI::slotDownBtnPressed()
{
    m_btnPress = 1;
    p_view->verticalScrollBar()->setValue(p_view->verticalScrollBar()->value() + p_view->height() / 12);
    p_timer->start(50);
}

/*
 * p_downBtn松开
 */
void SummaryUI::slotDownBtnRelease()
{
    p_timer->stop();
}


/*
 * 监控上/下按钮被按下的定时器槽函数(模拟键盘的上下键操作)
 *  btnPress : 默认为-1; 0 : 表示上按钮(upBtn)被按下; 1 : 表示下按钮(p_downBtn)被按下
 */
void SummaryUI::slotTimeOut()
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



/*************************************************************************************
 *
 * 重写代理类
 *  实现第1列部分行蓝色显示
 *  实现第2列在一个item中，上下两行显示数据时间和写入时间
 *  实现所有列表格只显示下边横线，竖线竖不显示
 *
 * **********************************************************************************/
/*
** 得到信息类型的字符串
*/
QString getMessageType(QString str)
{
    int type = str.toInt();
    if (type == Message_Type_Normal)
        return QString(QObject::tr("String message"));
    else if (type == Message_Type_Freehand)
        return QString("*" + QObject::tr("Freehand message") + "*");
    else
        return "";
}


/*
** 重写代理的构造函数
*/
MessageSummaryDelegate::MessageSummaryDelegate(QTableView *tableView, QObject * parent) : QItemDelegate(parent)
{
    int gridHint = tableView->style()->styleHint(QStyle::SH_Table_GridLineColor, new QStyleOptionViewItemV4());
    QColor gridColor = static_cast<QRgb>(gridHint);
    pen = QPen(gridColor,0,tableView->gridStyle());
    view = tableView;
}

/*
** 代理模型的重画view界面的函数
**  实现第1列部分行蓝色显示
**  实现第2列在一个item中，上下两行显示数据时间和写入时间
**  实现所有列表格只显示下边横线，竖线竖不显示
*/
void MessageSummaryDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    int x = option.rect.x();
    int y = option.rect.y();
    int width = option.rect.width();
    int height = option.rect.height();

    painter->save();
    if (index.column() == TYPE_COLUMN)
    {
        QString str = index.model()->data(index, Qt::DisplayRole).toString();
        QString messageType = getMessageType(str);
        if(str == "1")
        {
            QColor color(Qt::blue);
            QPen oldPen = painter->pen();
            QPen newPen;
            newPen = QPen(color);
            painter->setPen(newPen);
            painter->drawText(x + 5, y, width, height, Qt::AlignVCenter, messageType);
            painter->setPen(oldPen);
        }
        else
        {
            painter->drawText(x + 5, y, width, height, Qt::AlignVCenter, messageType);
        }
    }

    else if (index.column() == TIME_COLUMN)
    {
        QDateTime time;
        QString dataTime = index.model()->index(index.row(), TIME_COLUMN).data().toString();
        dataTime = time.fromMSecsSinceEpoch(dataTime.toLongLong()).toString(m_timeFormate);
        painter->drawText(x, y, width, height , Qt::AlignVCenter, dataTime);
    }
    else if (index.column() == GROUP_COLUMN)
    {
        QString group = index.model()->index(index.row(), GROUP_COLUMN).data().toString();
        painter->drawText(x, y, width, height , Qt::AlignVCenter, group == "0" ? QString(QObject::tr("All")) : group);
    }

    QPen oldPen = painter->pen();
    painter->setPen(pen);
    if (index.row() == 0)
        painter->drawLine(option.rect.topLeft(),option.rect.topRight());
    painter->drawLine(option.rect.bottomLeft(),option.rect.bottomRight());
    painter->setPen(oldPen);
    painter->restore();
}

/*
 * 去掉鼠标悬浮在个item上面时的tooltip
*/
bool MessageSummaryDelegate::helpEvent(QHelpEvent */*event*/, QAbstractItemView */*view*/, const QStyleOptionViewItem &/*option*/, const QModelIndex &/*index*/)
{
    return false;
}

void MessageSummaryDelegate::setTimeFormate(QString timeFormate)
{
    m_timeFormate = timeFormate;
}




/*
 * 重写tableview,主要是为了在代理那边得到自定义的闪烁状态
 *      m_isBlink = true 显示图片
 *      m_isBlink = false 不显示图片
*/
SummaryTableView::SummaryTableView(QWidget *parent) : QTableView(parent), m_isBlink(false), m_isPress(false)
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

//    this->setTextElideMode(Qt::ElideRight);
//    this->setWordWrap(true);
    this->horizontalHeader()->setStyleSheet("QHeaderView::section {\
            color: black;\
            background-color: rgb(240, 240, 240);\
            border-bottom: 1px solid #f6f7fa;\
            border-radius:0px;\
            border-color: rgb(180, 180, 180);\
            }");

    /*
     * 设置滚动条属性
     */
    this->verticalScrollBar()->setStyleSheet("QScrollBar{background:lightgray; width: 4px;}"
         "QScrollBar::handle{background:blue; border:0px solid transparent; border-radius:2px;}"
         "QScrollBar::handle:hover{background:gray;}"
         "QScrollBar::sub-line{background:transparent;}"
         "QScrollBar::add-line{background:transparent;}");


    /*
     * 添加滑屏操作
    */
    this->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);

    QScroller *scroller = QScroller::scroller(this->viewport());

    QScrollerProperties prop = scroller->scrollerProperties();

    prop.setScrollMetric(QScrollerProperties::AxisLockThreshold, 0.66);
    prop.setScrollMetric(QScrollerProperties::ScrollingCurve, QEasingCurve(QEasingCurve::OutExpo));
    prop.setScrollMetric(QScrollerProperties::DecelerationFactor, 0.05);
    prop.setScrollMetric(QScrollerProperties::MaximumVelocity, 0);
    prop.setScrollMetric(QScrollerProperties::OvershootDragResistanceFactor, 0.33);
    prop.setScrollMetric(QScrollerProperties::OvershootScrollDistanceFactor, 0.33);
    prop.setScrollMetric(QScrollerProperties::SnapPositionRatio, 0.93);
    prop.setScrollMetric(QScrollerProperties::DragStartDistance, 0.001);
    prop.setScrollMetric(QScrollerProperties::SnapPositionRatio,0.5);

    scroller->setScrollerProperties(prop);

    scroller->grabGesture(this, QScroller::TouchGesture);
    scroller->grabGesture(this, QScroller::LeftMouseButtonGesture);
    connect(CfgChannel::instance(), SIGNAL(sigNormalConfigChanged(quint32)), this, SLOT(slotBackgroundChanged(quint32)));
    slotBackgroundChanged(ID_Disp_Basic_Background);
}


/**
 * 功能:
 *      添加自定义状态值
 */
QStyleOptionViewItem SummaryTableView::viewOptions() const
{
    QStyleOptionViewItem option = QTableView::viewOptions();

    if(m_isPress){
        option.state |= QStyle::State_Sunken;
    }

    if(WinMainFrame::getBlinkStatus()){
        option.state |= QStyle::State_On;   //显示图片标记
    }else{
        option.state |= QStyle::State_Off;  //不显示图片标记
    }

    return option;
}

void SummaryTableView::mousePressEvent(QMouseEvent *event)
{
    m_point = event->pos();
    QModelIndex index = indexAt(m_point);
    if(event->button() == Qt::LeftButton && index.isValid()){
        m_rect = visualRect(index);
        m_isPress = true;
        update(index);
    }
    QTableView::mousePressEvent(event);
}

void SummaryTableView::mouseReleaseEvent(QMouseEvent *event)
{
    m_isPress = false;
    QPoint pos = event->pos();
    QModelIndex index = indexAt(pos);
    update(index);
//    int rx = m_point.x() - pos.x();
//    int ry = m_point.y() - pos.y();
//    if(rx>-20 && rx<20 && ry>-20 && ry<20){
//        qDebug() << "hehe";
//    }
    QTableView::mouseReleaseEvent(event);
}

void SummaryTableView::paintEvent(QPaintEvent *event)
{
    QTableView::paintEvent(event);
}

void SummaryTableView::slotBackgroundChanged(quint32 ID)
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




