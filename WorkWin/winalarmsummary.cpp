#include "winalarmsummary.h"
#include "../GlobalData/globalDef.h"
#include <QDebug>
#include <QDateTime>
#include <QHeaderView>
#include <QImage>
#include <QMessageBox>
#include "./WorkWin/winsummarydialog.h"
#include "./Common/authorization.h"
#include "./WorkWin/wininfolistdialog.h"



#define STATUS_COLUMN           0
#define TAG_COLUMN              1
#define ELEC_COLUMN             2
#define TYPE_COLUMN             3
#define TIME_COLUMN             4
#define CHANNEL_COLUMN          5

#define ALARM_MAIN_TABLE        "table_main_alarm"

WinAlarmSummary::WinAlarmSummary(QWidget *parent) :
    QWidget(parent), m_display(false), alarmCurrentRow(11)
{
    getTimeFormate();   //得到时间的格式
    getDisplayType();   //得到显示类型：仅显示正在报警/显示全部
    m_alarmSummary.setParent(this);
    p_alarmModel = new ModelSummaryAlarm(m_display);
    m_alarmSummary.p_view->setModel(p_alarmModel);
    setViewStyle();
    initFrameHeader();

    resetColumnSize();
    initHeaderData();
    initBtnShowAndHidden();
    signalAndSlotConnect();
}

WinAlarmSummary::~WinAlarmSummary()
{
    delete p_alarmModel;
}

/*
 * 初始化表头上竖线
*/
void WinAlarmSummary::initFrameHeader()
{
    for (int i = 0; i < 3; i++)
    {
        m_frameHeader[i].setParent(m_alarmSummary.p_view);
        m_frameHeader[i].setFrameShape(QFrame::VLine);
        m_frameHeader[i].setFrameShadow(QFrame::Sunken);
        m_frameHeader[i].hide();
    }
}

/*
 * view的表头和upBtn与downBtn的隐藏与显示
 */
void WinAlarmSummary::initBtnShowAndHidden()
{
    alarmSumRow = p_alarmModel->rowCount();
    if(alarmSumRow) {
        m_alarmSummary.p_upBtn->show();
        m_alarmSummary.p_downBtn->show();
    } else {
        m_alarmSummary.p_upBtn->hide();
        m_alarmSummary.p_downBtn->hide();
    }
    if (alarmSumRow < 11)
        alarmCurrentRow = alarmSumRow;
}

/*
 * 信号和槽的绑定
*/
void WinAlarmSummary::signalAndSlotConnect()
{
    connect(m_alarmSummary.p_view, SIGNAL(clicked(QModelIndex)), this, SLOT(slotDetailMessage(QModelIndex))); //当前行详细信息

    connect(m_alarmSummary.p_view->horizontalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(slotSectionSort(int)));
}


void WinAlarmSummary::slotSectionSort(int section)
{
    m_alarmSummary.p_view->setCurrentIndex(m_alarmSummary.p_view->model()->index(0, section));
}

/*
 * 设置当前行与总行数的信息
*/
void WinAlarmSummary::setCurrentRow(int current, int sumrow)
{
    if (current < 11)
        current = 11;
    if (sumrow < 11)
        current = sumrow;
    if (this->height() < DESKTOP_HEIGHT * 0.35)
        m_alarmSummary.p_headerLabel->setText(QString("%1/%2\t%3").arg(current, 4, 10, QChar('0'))
                                               .arg(sumrow, 4, 10, QChar('0'))
                                               .arg(tr("Alarm summary")));
    else
        m_alarmSummary.p_headerLabel->setText(QString("%1/%2").arg(current, 4, 10, QChar('0')).arg(sumrow, 4, 10, QChar('0')));
}


void WinAlarmSummary::initHeaderData()
{
    int width = this->width();
    int height = this->height();
    int heightHeader;
    QFont font;

    if (width > DESKTOP_WIDTH * 0.75)
    {
        setHeadDataString();
        m_alarmSummary.p_headerLabel->setGeometry(100, 0, 400, 20);
        if (height > DESKTOP_HEIGHT * 0.75)
        {
            font.setPixelSize(height / 15 * 0.4);
            heightHeader = 50;
        }
        else/* if (height > DESKTOP_HEIGHT * 0.33 && height <= DESKTOP_HEIGHT * 0.75)*/
        {
            font.setPixelSize(height / 12 * 0.5);
            heightHeader = 45;
        }
        ((AlarmSummaryDelegate *)m_alarmSummary.p_view->itemDelegate())->setTimeFormate(m_dateTimeFormate);
    }
    else if (width > DESKTOP_WIDTH * 0.35 && width <= DESKTOP_WIDTH * 0.75)
    {
        m_alarmSummary.p_headerLabel->setGeometry(width / 13, 0, 400, 20);

        if (height > DESKTOP_HEIGHT * 0.75)
        {
            font.setPixelSize(height / 22 * 0.5);
            setHeadDataString();
            //p_modelAlarm->setHeaderData(ELEC_COLUMN, Qt::Horizontal, tr("Lv"));
            heightHeader = 50;
        }
        else if (height > DESKTOP_HEIGHT * 0.35 && height <= DESKTOP_HEIGHT * 0.75)
        {
            font.setPixelSize(height / 12 * 0.5);
            setHeadDataString();
            heightHeader = 45;
        }
        else
        {
            font.setPixelSize(height / 12 * 0.8);
            setHeadDataNull();
            heightHeader = 20;
        }
        ((AlarmSummaryDelegate *)m_alarmSummary.p_view->itemDelegate())->setTimeFormate(m_dateTimeFormate);
    }
    else
    {
        m_alarmSummary.p_headerLabel->setGeometry(width / 13, 0, 400, 20);
        if (height > DESKTOP_HEIGHT * 0.35 && height <= DESKTOP_HEIGHT * 0.75)
        {
            font.setPixelSize(height / 15 * 0.6);
            setHeadDataString();
            p_alarmModel->setHeaderData(TAG_COLUMN, Qt::Horizontal, tr("Ch"));
            p_alarmModel->setHeaderData(ELEC_COLUMN, Qt::Horizontal, "");
            p_alarmModel->setHeaderData(TYPE_COLUMN, Qt::Horizontal, "");
            heightHeader = 45;
        }
        else
        {
            font.setPixelSize(height / 15);
            setHeadDataNull();
            heightHeader = 20;
        }
        ((AlarmSummaryDelegate *)m_alarmSummary.p_view->itemDelegate())->setTimeFormate(m_dateTimeFormate.split(" ").at(1));
    }

    m_alarmSummary.p_view->verticalHeader()->setDefaultSectionSize(height / 12);
    m_alarmSummary.p_view->horizontalHeader()->setFixedHeight(heightHeader);

    m_alarmSummary.p_view->horizontalHeader()->setFont(font);
    m_alarmSummary.p_headerLabel->setFont(font);
    m_alarmSummary.p_view->setFont(font);
}

void WinAlarmSummary::setHeadDataNull()
{
    m_alarmSummary.p_view->horizontalHeader()->setSortIndicatorShown(false);
    p_alarmModel->setHeaderData(STATUS_COLUMN, Qt::Horizontal, "");
    p_alarmModel->setHeaderData(TAG_COLUMN, Qt::Horizontal, "");
    p_alarmModel->setHeaderData(ELEC_COLUMN, Qt::Horizontal, "");
    p_alarmModel->setHeaderData(TYPE_COLUMN, Qt::Horizontal, "");
    p_alarmModel->setHeaderData(TIME_COLUMN, Qt::Horizontal, "");
}

void WinAlarmSummary::setHeadDataString()
{
    p_alarmModel->setHeaderData(STATUS_COLUMN, Qt::Horizontal, "");
    p_alarmModel->setHeaderData(TAG_COLUMN, Qt::Horizontal, tr("Channel"));
    p_alarmModel->setHeaderData(ELEC_COLUMN, Qt::Horizontal, tr("Level"));
    p_alarmModel->setHeaderData(TYPE_COLUMN, Qt::Horizontal, tr("Type"));
    p_alarmModel->setHeaderData(TIME_COLUMN, Qt::Horizontal, tr("Time"));
}


void WinAlarmSummary::resetColumnSize()
{
    if (this->width() > DESKTOP_WIDTH * 0.70)
    {
        m_alarmSummary.p_view->setColumnWidth(STATUS_COLUMN, 100);
        m_alarmSummary.p_view->setColumnWidth(ELEC_COLUMN, this->width() / 6);
        m_alarmSummary.p_downBtn->show();
        m_alarmSummary.p_upBtn->show();
        for (int i = 0; i < 3; i++)
            m_frameHeader[i].show();
    }
    else
    {
        m_alarmSummary.p_view->setColumnWidth(STATUS_COLUMN, this->width() / 13);
        m_alarmSummary.p_view->setColumnWidth(ELEC_COLUMN, this->width() / 8);
        m_alarmSummary.p_view->setColumnWidth(TYPE_COLUMN, this->width() / 8);
        m_alarmSummary.p_view->setColumnWidth(CHANNEL_COLUMN, this->width() / 10);
        m_alarmSummary.p_downBtn->hide();
        m_alarmSummary.p_upBtn->hide();
        for (int i = 0; i < 3; i++)
            m_frameHeader[i].hide();
    }
    m_alarmSummary.p_view->setColumnWidth(TAG_COLUMN, this->width() / 5);
    m_alarmSummary.p_view->setColumnWidth(TIME_COLUMN, this->width() / 4);
}



/*
 * 从配置信息中得到时间的格式
 *
 * 需要修改(修改成从相应的配置信息中得到时间的格式)
 */
void WinAlarmSummary::getTimeFormate()
{
    m_dateTimeFormate = QDateTime::currentDateTime().toString(CfgChannel::instance()->getSysDateFormat()).append(" hh:mm:ss.zzz");
}

/*
 * 得到显示界面的显示模式：仅显示正在报警的信息/全部显示
 */
void WinAlarmSummary::getDisplayType()
{
    m_display = true;
//    m_display = !m_display;
}

/*
 * 当配置信息的时间格式改变后，响应的槽函数
 * 绑定时间格式切换的信号
*/
void WinAlarmSummary::slotTimeFormateChangeed()
{
    getTimeFormate();
    int width = this->width();
    if (width > DESKTOP_WIDTH * 0.35)
        ((AlarmSummaryDelegate *)(m_alarmSummary.p_view->itemDelegate()))->setTimeFormate(m_dateTimeFormate);
    else
        ((AlarmSummaryDelegate *)(m_alarmSummary.p_view->itemDelegate()))->setTimeFormate(m_dateTimeFormate.split(" ").at(1));
}


/*
 * 绑定显示模式的信号 :最新1000条报警信息全部显示/仅显示正在报警的信息
*/
void WinAlarmSummary::slotDisplayType() //显示界面的显示模式 false 仅显示正在报警的信息   true全部显示
{
    p_alarmModel->init(m_display);

    alarmSumRow = p_alarmModel->rowCount();
    setCurrentRow(m_alarmSummary.p_headerLabel->text().split("/").at(0).toInt(), alarmSumRow);
}

void WinAlarmSummary::slotUpdateData()
{
    if (p_alarmModel->getMaxId() != p_alarmModel->m_maxId)
    {
        p_alarmModel->init(m_display);
        alarmSumRow = p_alarmModel->rowCount();
    }
    this->repaint();
}

/*
 * 显示点击行的详细信息
*/
void WinAlarmSummary::slotDetailMessage(QModelIndex index)
{
    if(Authorization::instance()->getLoginStatus() == Authorization::Auth_LoginStatus_Logout){
        WinInfoListDialog::instance()->showMsg(818);    //818 :请使用MENU 键登录。Press [MENU] key to login.
    }else{
        WinSummaryDialog dialog(Summary_Alarm_Type, index);
        dialog.exec();
    }
}


/*
 * 设置view的属性和代理
 */
void WinAlarmSummary::setViewStyle()
{
    /* 设置代理 */
    m_alarmSummary.p_view->setItemDelegate(new AlarmSummaryDelegate(m_alarmSummary.p_view));
    setviewColumnStyle();
}

/*
 * 设置行的隐藏
*/
void WinAlarmSummary::setviewColumnStyle()
{
    m_alarmSummary.p_view->setColumnHidden(CHANNEL_COLUMN, true);
    m_alarmSummary.p_view->sortByColumn(TIME_COLUMN, Qt::DescendingOrder);    //降序
}

/*
 * 重写resizeEvent函数，当界面大小小于一个值的时候，该view窗口会隐藏某列或者缩放某列
*/
void WinAlarmSummary::resizeEvent(QResizeEvent * event)
{
    m_alarmSummary.resize(QSize(this->size().width(), this->size().height()));

    resetColumnSize();
    initHeaderData();
    setVline();

    QWidget::resizeEvent(event);
}

void WinAlarmSummary::paintEvent(QPaintEvent *event)
{
    if (p_alarmModel->getMaxId() != p_alarmModel->m_maxId)
    {
        p_alarmModel->init(m_display);
        alarmSumRow = p_alarmModel->rowCount();
    }
    slotTimeFormateChangeed();
    getTimeFormate();
    int width = this->width();
    if (width > DESKTOP_WIDTH * 0.35)
        ((AlarmSummaryDelegate *)(m_alarmSummary.p_view->itemDelegate()))->setTimeFormate(m_dateTimeFormate);
    else
        ((AlarmSummaryDelegate *)(m_alarmSummary.p_view->itemDelegate()))->setTimeFormate(m_dateTimeFormate.split(" ").at(1));

    int currentrow = m_alarmSummary.p_view->rowAt(m_alarmSummary.p_view->y());
    setCurrentRow(currentrow + 12, alarmSumRow);
    QWidget::paintEvent(event);
}

/**
 * @brief line
 * @每个表头旁边的竖线
 */
void WinAlarmSummary::setVline()
{
    int widthLocal[3];
    widthLocal[0] = m_alarmSummary.p_view->columnWidth(TAG_COLUMN) + m_alarmSummary.p_view->columnWidth(STATUS_COLUMN);
    widthLocal[1] = widthLocal[0] + m_alarmSummary.p_view->columnWidth(ELEC_COLUMN);
    widthLocal[2] = widthLocal[1] + m_alarmSummary.p_view->columnWidth(TYPE_COLUMN);

    for (int i = 0; i < 3; i++)
        m_frameHeader[i].setGeometry(QRect(widthLocal[i] - 6, 6, 4, m_alarmSummary.p_view->horizontalHeader()->height() - 12 ));
}



/*****************************************************************************************
    自定义代理类的重新实现
        ：重写代理类，实现第一列图片
*****************************************************************************************/

/**
 * 通过报警类型得到报警的类型描述
 * @param type
 * @return : 报警类型描述
 */

//Alarm_Type_Off = 0,
//Alarm_Type_H,                                              //上限
//Alarm_Type_L ,                                              //下限
//Alarm_Type_R,                                              //变化率上升限
//Alarm_Type_r,                                               //变化率下升限
//Alarm_Type_T,                                              //延迟上限
//Alarm_Type_t,                                              //延迟下限
//Alarm_Type_h,                                             //差值上限， Delta运算时有效
//Alarm_Type_l,
QString getAlarmType(int type)
{
    switch (type) {
    case Alarm_Type_Off:
        return QObject::tr("Off");
        break;
    case Alarm_Type_H:
        return QObject::tr("H");
        break;
    case Alarm_Type_L:
        return QObject::tr("L");
        break;
    case Alarm_Type_R:
        return QObject::tr("R");
        break;
    case Alarm_Type_r:
        return QObject::tr("r");
        break;
    case Alarm_Type_T:
        return QObject::tr("T");
        break;
    case Alarm_Type_t:
        return QObject::tr("t");
        break;
    case Alarm_Type_h:
        return QObject::tr("h");
        break;
    case Alarm_Type_l:
        return QObject::tr("l");
        break;
    default:
        break;
    }
    return "";
}

/*
 * 根据报警类型，返回该报警类型需要显示的图片
*/
//非保持状态下	非保持状态下报警发生  SQL_Alarm_Status_NonHold_Occur	ON标记，不闪烁
//	非保持状态下报警释放  SQL_Alarm_Status_NonHold_Release	Off标记不闪烁
//	非保持状态下所有报警Off   SQL_Alarm_Status_NonHold_AllOff	ACK标记(不显示通道号)

//保持状态下	保持状态下报警发生，未解除ACK  SQL_Alarm_Status_Hold_Occur_NoAck	ON标记，闪烁
//	保持状态下报警发生，解除ACK    SQL_Alarm_Status_Hold_Occur_Ack	ON标记,不闪烁
//	保持状态下报警释放，未解除ACK  SQL_Alarm_Status_Hold_Release_NoAck	Off标记，闪烁
//	保持状态下报警释放，解除ACK    SQL_Alarm_Status_Hold_Release_Ack	Off标记，不闪烁
//	保持状态下全部解除ACK          SQL_Alarm_Status_Hold_Ack_All	ACK标记(不显示通道号)
//	保持状态下单个解除ACK          SQL_Alarm_Status_Hold_Ack_Single	ACK标记(显示通道号)

QImage AlarmSummaryDelegate::getAlarmImage(int type, bool isBlink) const
{
    switch (type) {
    case SQL_Alarm_Status_NonHold_Occur:
    case SQL_Alarm_Status_Hold_Occur_Ack:   //on
        return m_imageOn;
        break;
    case SQL_Alarm_Status_NonHold_Release:
    case SQL_Alarm_Status_Hold_Release_Ack://off
        return m_imageOff;
        break;
    case SQL_Alarm_Status_NonHold_AllOff:
    case SQL_Alarm_Status_Hold_Ack_All:
    case SQL_Alarm_Status_Hold_Ack_Single:
        return m_imageAck;
        break;
    case SQL_Alarm_Status_Hold_Occur_NoAck:                         //on and blink
    {
        if (isBlink)
            return m_imageOn;
        else
            return QImage();
    }
        break;
    case SQL_Alarm_Status_Hold_Release_NoAck:
    {
        if (isBlink)
            return m_imageOff;
        else
           return QImage();
    }
        break;
    default:
       return QImage();
        break;
    }
    return QImage();
}

void AlarmSummaryDelegate::setTimeFormate(QString timeFormate)
{
    m_timeFormate = timeFormate;
}

AlarmSummaryDelegate::AlarmSummaryDelegate(QTableView* tableView, QObject *parent) : QItemDelegate(parent)
{
    int gridHint = tableView->style()->styleHint(QStyle::SH_Table_GridLineColor, new QStyleOptionViewItemV4());
    QColor gridColor = static_cast<QRgb>(gridHint);
    pen = QPen(gridColor,0,tableView->gridStyle());
    view = tableView;

    m_imageOn.load(":/summary/Image/summary/on.png");
    m_imageOff.load(":/summary/Image/summary/OFF.png");
    m_imageAck.load(":/summary/Image/summary/ACK.png");
}


void AlarmSummaryDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option,const QModelIndex& index)const
{
    int x = option.rect.x();
    int y = option.rect.y();
    int width = option.rect.width();
    int height = option.rect.height();

    painter->save();
    if (index.column() == STATUS_COLUMN)
    {
        bool isBlink = false;
        if(option.state & QStyle::State_On){
            isBlink = true;
        }
        QImage image = getAlarmImage(index.data().toInt(), isBlink);
        if (image.width() != 0)
        {
            //if (height < image.height() * 2)
            image = image.scaled(height / 2, height / 2, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);  //
        }
        painter->drawImage(x + (height - image.width()) / 2, y + (height - image.height()) / 2 , image, 0, 0, image.width(), image.height());
    }
    else if (index.column() == TYPE_COLUMN)
    {
        QString type = getAlarmType(index.data().toInt());
        painter->drawText(x, y, width, height, Qt::AlignVCenter, type);
    }
    else if (index.column() == TIME_COLUMN)
    {
        QString timeValue = index.data().toString();
        timeValue = QDateTime::fromMSecsSinceEpoch(timeValue.toLongLong()).toString(m_timeFormate);
        painter->drawText(x, y, width, height, Qt::AlignVCenter, timeValue);
    }
    else
    {
        QString str = index.data().toString();
        painter->drawText(x, y, width, height, Qt::AlignVCenter, str);
    }
    QPen oldPen = painter->pen();
    painter->setPen(pen);
    if (index.row() == 0)
        painter->drawLine(option.rect.topLeft(),option.rect.topRight());
    painter->drawLine(option.rect.bottomLeft(),option.rect.bottomRight());
    painter->setPen(oldPen);
    painter->restore();
}

bool AlarmSummaryDelegate::helpEvent(QHelpEvent */*event*/,
                                     QAbstractItemView */*view*/,
                                     const QStyleOptionViewItem &/*option*/,
                                     const QModelIndex &/*index*/)
{
    return false;
}

//bool AlarmSummaryDelegate::editorEvent(QEvent *event, QAbstractItemModel *model,
//                              const QStyleOptionViewItem &option,const QModelIndex &index)
//{

//}


