#include "winmodbusclient.h"
#include <QHeaderView>
#include "./GlobalData/globalDef.h"
#include "./WorkWin/winsummarydialog.h"
#include "DataModel/modelreport.h"
#include "./Common/authorization.h"
#include <QResizeEvent>
#include "./Common/wininputconfig.h"

#define MODBUSPATH "/home/qiu/share/modbus/modbus.txt"

extern void getImageAndStatus(const int type, QString &str);

#define NUM_COLUMN                      0
#define KIND_COLUMN                     1
#define STATUS_COLUMN                   2
#define CHANNEL_COLUMN                  3
#define SERVER_COLUMN                   4
#define REGISTER_COLUMN                 5
#define TYPE_COLUMN                     6
#define UNIT_COLUMN                     7


WinModbusClient::WinModbusClient(QWidget *parent) :
    QWidget(parent), m_timerStatus(false), m_status(true)
{
    initModbusUI();
    p_normalModel = new ModelModbusClient(Normal_Type);
    p_gridModel = new ModelModbusClient(Grid_Type);
    p_normalDelegate = new ModbusClientDelegate(m_modbusView.p_view);
    p_gridDelegate = new ModbusGridlayoutDelegate(m_modbusView.p_view);
    getModbusClientShowStatus();

    setViewStyle();
    initHeaderVline();
    settopLabelVlaue();
    signalAndSlotConnect();
}

WinModbusClient::~WinModbusClient()
{
    delete p_gridDelegate;
    delete p_normalDelegate;
    delete p_normalModel;
}

/*
 * 从配置信息中得到客户端显示界面的类型配置
 *      m_status 记录modbusClient的界面显示形式的状态
 *              true :一行一行的显示
 *              false:网格显示
 *  修改成从配置信息中得到显示状态
 */
void WinModbusClient::getModbusClientShowStatus()
{
    m_status = true;
    if (m_status)
    {
        m_modbusView.p_view->setModel(p_normalModel);
        m_modbusView.p_view->setItemDelegate(p_normalDelegate);
    }
    else
    {
        m_modbusView.p_view->setModel(p_gridModel);
        m_modbusView.p_view->setItemDelegate(p_gridDelegate);
    }
}


/*
 * 当modbus客户端的周期，自动恢复时间，连接时间改变时绑定的槽函数
*/
void WinModbusClient::slotModbusSettingChanged()
{
    settopLabelVlaue();
}


/*
 * 对外槽函数,绑定配置状态修改时对应的信号
*/
void WinModbusClient::slotShowModbusClient()
{
    getModbusClientShowStatus();

    resizeEvent(new QResizeEvent(this->size(), this->size()));
}

/*
 * 初始化客户端界面信息
*/
void WinModbusClient::initModbusUI()
{
    QGridLayout *p_mainLayout = new QGridLayout(this);
    p_mainLayout->setSpacing(0);
    p_mainLayout->setContentsMargins(0, 0, 0, 0);

    m_frame.setParent(this);
    m_frame.setObjectName(QStringLiteral("m_frame"));
    m_frame.setMinimumSize(QSize(0, 50));
    m_frame.setMaximumHeight(70);
    m_frame.setFrameShape(QFrame::NoFrame);
    m_frame.setFrameShadow(QFrame::Raised);
    m_frame.setLineWidth(0);
    m_frame.setStyleSheet("background:#ECECEC");

    QHBoxLayout * topLayout = new QHBoxLayout(&m_frame);
    topLayout->setSpacing(20);

    m_upBtn.setParent(&m_frame);
    m_upBtn.setObjectName("m_upBtn");
    m_upBtn.setMaximumSize(QSize(50, 16777215));
    m_upBtn.setIcon(QIcon(":/summary/Image/summary/up.png"));
    m_upBtn.setIconSize(QSize(45, 28));
    m_upBtn.setFlat(true);
    m_upBtn.setFocusPolicy(Qt::NoFocus);
    topLayout->addWidget(&m_upBtn);

    m_downBtn.setParent(&m_frame);
    m_downBtn.setObjectName("m_downBtn");
    m_downBtn.setMaximumSize(QSize(50, 16777215));
    m_downBtn.setIcon(QIcon(":/summary/Image/summary/down.png"));
    m_downBtn.setIconSize(QSize(40, 28));
    m_downBtn.setFlat(true);
    m_downBtn.setFocusPolicy(Qt::NoFocus);
    topLayout->addWidget(&m_downBtn);
    setStyleSheet("QPushButton {border: none;}");

    m_topLabel.setParent(&m_frame);
    m_topLabel.setObjectName("m_topLabel");
    m_topLabel.setMinimumSize(QSize(500, 40));
    m_topLabel.setStyleSheet("QLabel {color:rgb(100, 100, 100);font-size:16px;}");

    topLayout->addWidget(&m_topLabel);

    p_mainLayout->addWidget(&m_frame, 0, 0, 1, 1);

    m_modbusView.setParent(this);
    m_modbusView.p_view->setSortingEnabled(false);
    m_modbusView.p_view->setEditTriggers(false);
    m_modbusView.p_headerLabel->setGeometry(0, 0, 400, 20);

    p_mainLayout->addWidget(&m_modbusView, 2, 0, 1, 1);
}


void WinModbusClient::setViewStyle()
{
    if (m_status)
    {
        m_modbusView.p_view->setColumnHidden(KIND_COLUMN, true);
        m_modbusView.p_view->setColumnHidden(TYPE_COLUMN, true);
        m_modbusView.p_view->setColumnHidden(UNIT_COLUMN, true);
    }
    else
    {
        m_modbusView.p_view->horizontalHeader()->setFixedHeight(0);
        QFont font;
        if (this->width() > DESKTOP_WIDTH * 0.75 && this->height() > DESKTOP_HEIGHT * 0.75)
            font.setPixelSize(this->height() / 30);
        else
            font.setPixelSize(this->height() / 50);
        m_modbusView.p_view->setFont(font);
    }
}


void WinModbusClient::initHeaderVline()
{
    for (int i = 0; i < 4; i++)
    {
        m_vLine[i].setParent(m_modbusView.p_view);
        m_vLine[i].setFrameShape(QFrame::VLine);
        m_vLine[i].setFrameShadow(QFrame::Sunken);
        m_vLine[i].hide();
    }
}


/*
 * 界面改变大小函数
 */
void WinModbusClient::resizeEvent(QResizeEvent */*event*/)
{
    if (this->width() > DESKTOP_WIDTH * 0.80 && this->height() > DESKTOP_HEIGHT * 0.80)
       m_frame.show();
    else
        m_frame.hide();

    if (m_status)
    {
        m_modbusView.p_view->resize(this->width(), this->height());
        m_modbusView.p_view->setColumnHidden(KIND_COLUMN, true);
        m_modbusView.p_view->setColumnHidden(TYPE_COLUMN, true);
        m_modbusView.p_view->setColumnHidden(UNIT_COLUMN, true);
        m_modbusView.p_headerLabel->show();
        resetColumnWidth();
        initModelHeaderData();
        setVline();
    }
    else
    {
        for (int i = 0; i < 4; i++)
            m_vLine[i].hide();
//        m_modbusView.p_view->setColumnWidth(0, this->width() / 2 - 10);
        m_modbusView.p_view->horizontalHeader()->setFixedHeight(0);
        int col = p_gridModel->columnCount();
        for (int i = 0;i < col; i++)
             m_modbusView.p_view->setColumnWidth(i, this->width() / col);
        m_modbusView.p_view->setColumnHidden(1, false);
        int cnt = p_gridModel->rowCount();
        m_modbusView.p_view->verticalHeader()->setDefaultSectionSize(m_modbusView.height() / cnt);
        m_modbusView.p_headerLabel->hide();


        QFont font;
        if (this->width() > DESKTOP_WIDTH * 0.75 && this->height() > DESKTOP_HEIGHT * 0.75)
            font.setPixelSize(this->height() / 30);
        else
            font.setPixelSize(this->height() / 50);
        m_modbusView.p_view->setFont(font);
    }
}


void WinModbusClient::setVline()
{
    if (m_status)
    {
        int widthLocal[4];
        widthLocal[0] = m_modbusView.p_view->columnWidth(NUM_COLUMN);
        widthLocal[1] = widthLocal[0] + m_modbusView.p_view->columnWidth(Status_Column);
        widthLocal[2] = widthLocal[1] + m_modbusView.p_view->columnWidth(CHANNEL_COLUMN);
        widthLocal[3] = widthLocal[2] + m_modbusView.p_view->columnWidth(SERVER_COLUMN);
        int height = m_modbusView.p_view->horizontalHeader()->height() - 12;

        for (int i = 0; i < 4; i++)
            m_vLine[i].setGeometry(QRect(widthLocal[i] - 6, 6, 4, height));
    }
}

/*
 * 信号和槽函数的绑定
*/
void WinModbusClient::signalAndSlotConnect()
{
    connect(&m_upBtn, SIGNAL(pressed()), &m_modbusView, SLOT(slotUpBtnPressed()));
    connect(&m_upBtn, SIGNAL(released()), &m_modbusView, SLOT(slotUpBtnRelease()));

    connect(&m_downBtn, SIGNAL(pressed()), &m_modbusView, SLOT(slotDownBtnPressed()));
    connect(&m_downBtn, SIGNAL(released()), &m_modbusView, SLOT(slotDownBtnRelease()));

    connect(m_modbusView.p_view, SIGNAL(clicked(QModelIndex)), this, SLOT(slotDetailMessage(QModelIndex)));
    connect(CfgChannel::instance(), SIGNAL(sigNormalConfigChanged(quint32)), this, SLOT(settopLabelVlaue(quint32)));
}



/*
 * 获得当前配置值，并设置到响应的label中
*/
void WinModbusClient::settopLabelVlaue(quint32 ID)
{
    if (ID != ID_NetWork_Modbus_Basic)
        return ;
    quint32 cycleTime, waitTime, connectTime;
    QString cycle, wait, connect = "Off";
    cycleTime = DevConfigPtr->networkModbus.commInterval;
    waitTime = DevConfigPtr->networkModbus.recoveryTime;
    connectTime = DevConfigPtr->networkModbus.connectTime;

    if (waitTime == 0)
        wait = "Off";
    else if (waitTime > 0 && waitTime < 60)
        wait = QString("%1 s").arg(waitTime);
    else
        wait = QString("%1 min").arg(waitTime / 60);

    if (cycleTime < 1000)
        cycle = QString("%1 ms").arg(cycleTime);
    else if (cycleTime >= 1000 && cycleTime < 60000)
        cycle = QString("%1 s").arg(cycleTime / 1000);
    else
        cycle = QString("%1 s").arg(cycleTime / 60000);

    if(DevConfigPtr->networkModbus.connectHold != 0)
        connect = QString::number(connectTime) + " s";
    m_topLabel.setText(QString("%1 \t: %2\t\t%3 \t: %4\n%5 \t: %6\t\t%7 \t: %8")
                        .arg(tr("Read cycle")).arg(cycle)
                        .arg(tr("Auto recovery")).arg(connect)
                        .arg(tr("Connect time")).arg(wait)
                       .arg(tr("Loop time")).arg("1 ms"));
}

void WinModbusClient::slotTimeout()
{
    slotShowModbusClient();
}

void WinModbusClient::slotDetailMessage(QModelIndex index)
{
    if(Authorization::instance()->getLoginStatus() == Authorization::Auth_LoginStatus_Logout){
        WinInfoListDialog::instance()->showMsg(818);    //818 :请使用MENU 键登录。Press [MENU] key to login.
    }else {
        if (!index.data().isValid()) return ;
        WinSummaryDialog dlg(Summary_ModbusClient_Type, index, m_status, this);
        dlg.exec();
    }
}



/*
 * 当界面大小变化时，调整列宽，行高和列的显示隐藏
*/
void WinModbusClient::resetColumnWidth()
{
    int width = this->width();

    m_modbusView.p_view->setColumnWidth(NUM_COLUMN, 40);
    if (width < DESKTOP_WIDTH * 0.35)
    {
        m_modbusView.p_view->setColumnHidden(SERVER_COLUMN, true);
        m_modbusView.p_view->setColumnHidden(REGISTER_COLUMN, true);
        m_modbusView.p_view->setColumnWidth(STATUS_COLUMN, width / 2);
        m_modbusView.p_view->setColumnWidth(CHANNEL_COLUMN, width / 5);
    }
    else
    {
        m_modbusView.p_view->setColumnHidden(SERVER_COLUMN, false);
        m_modbusView.p_view->setColumnHidden(REGISTER_COLUMN, false);
        m_modbusView.p_view->setColumnWidth(STATUS_COLUMN, width / 4);
        m_modbusView.p_view->setColumnWidth(CHANNEL_COLUMN, width / 4);
        m_modbusView.p_view->setColumnWidth(SERVER_COLUMN, width / 4);
    }
    m_modbusView.p_view->verticalHeader()->setDefaultSectionSize(this->height() / 12);
}


/*
 * view的表头和upBtn与p_downBtn的隐藏与显示
 */
void WinModbusClient::initModelHeaderData()
{
    int width = this->width();
    int height = this->height();
    int heightHeader;
    QFont font;
    m_modbusView.p_headerLabel->setText("");
    for (int i = 0; i < 4; i++)
        m_vLine[i].hide();
    if (width > DESKTOP_WIDTH * 0.75)
    {
        setHeaderData();
        heightHeader = 45;
        for (int i = 0; i < 4; i++)
            m_vLine[i].show();
        font.setPixelSize(height / 13 * 0.4);
    }
    else if (width > DESKTOP_WIDTH * 0.35 && width <= DESKTOP_WIDTH * 0.75)
    {
        if (height > DESKTOP_HEIGHT * 0.75)
        {
            setHeaderData();
            p_normalModel->setHeaderData(CHANNEL_COLUMN, Qt::Horizontal, tr("Comm.Data") + "\n" + tr("First") + "      " + tr("Last"));
            font.setPixelSize(height / 20 * 0.5);
            heightHeader = 50;
        }
        else if (height > DESKTOP_HEIGHT * 0.35 && height <= DESKTOP_HEIGHT * 0.75)
        {
            setHeaderData();
            font.setPixelSize(height / 10 * 0.5);
            p_normalModel->setHeaderData(CHANNEL_COLUMN, Qt::Horizontal, tr("Comm.Data") + "\n" + tr("First") + "      " + tr("Last"));
            heightHeader = 40;
        }
        else
        {
            setHeaderNull();
            font.setPixelSize(height / 10 * 0.8);
            heightHeader = 20;
            m_modbusView.p_headerLabel->setText(tr("Modbus client"));
            m_modbusView.p_view->verticalHeader()->setDefaultSectionSize(height / 8);
        }
    }
    else
    {
        setHeaderNull();

        heightHeader = 20;
        m_modbusView.p_headerLabel->setFixedHeight(heightHeader);
        m_modbusView.p_headerLabel->setText(tr("Modbus client"));
        m_modbusView.p_view->verticalHeader()->setDefaultSectionSize(height / 8);
        if (height > DESKTOP_HEIGHT * 0.35 && height <= DESKTOP_HEIGHT * 0.75)
            font.setPixelSize(height / 13 * 0.6);
        else
            font.setPixelSize(height / 13);
    }
    m_modbusView.p_view->horizontalHeader()->setFont(font);
    m_modbusView.p_view->horizontalHeader()->setFixedHeight(heightHeader);
    m_modbusView.p_view->setFont(font);
}


void WinModbusClient::setHeaderData()
{
    p_normalModel->setHeaderData(NUM_COLUMN, Qt::Horizontal, tr("NO"));
    p_normalModel->setHeaderData(STATUS_COLUMN, Qt::Horizontal, tr("Status"));
    p_normalModel->setHeaderData(CHANNEL_COLUMN, Qt::Horizontal, "      " + tr("Comm.Data") + "\n" + tr("First") + "                " + tr("Last"));
    p_normalModel->setHeaderData(SERVER_COLUMN, Qt::Horizontal, tr("Server name"));
    p_normalModel->setHeaderData(REGISTER_COLUMN, Qt::Horizontal, tr("Register"));
}

void WinModbusClient::setHeaderNull()
{
    p_normalModel->setHeaderData(NUM_COLUMN, Qt::Horizontal, "");
    p_normalModel->setHeaderData(STATUS_COLUMN, Qt::Horizontal, "");
    p_normalModel->setHeaderData(CHANNEL_COLUMN, Qt::Horizontal, "");
    p_normalModel->setHeaderData(SERVER_COLUMN, Qt::Horizontal, "");
    p_normalModel->setHeaderData(REGISTER_COLUMN, Qt::Horizontal, "");
}


void WinModbusClient::paintEvent(QPaintEvent *event)
{
    if (!m_status)
    {
        int col = p_gridModel->columnCount();
        for (int i = 0;i < col; i++)
            m_modbusView.p_view->setColumnWidth(i, this->width() / col);
        int cnt = p_gridModel->rowCount();
        m_modbusView.p_view->verticalHeader()->setDefaultSectionSize(m_modbusView.height() / cnt);
    }
    QWidget::paintEvent(event);
}









/*************************************************************************************************
 *
 * 重写代理
 *
 * ************************************************************************************************/
bool ModbusClientDelegate::helpEvent(QHelpEvent */*event*/, QAbstractItemView */*view*/,
           const QStyleOptionViewItem &/*option*/,const QModelIndex &/*index*/)
{
    return false;
}


/*
** 重写代理的构造函数
*/
ModbusClientDelegate::ModbusClientDelegate(QTableView *tableView, QObject * parent) : QItemDelegate(parent)
{
    int gridHint = tableView->style()->styleHint(QStyle::SH_Table_GridLineColor, new QStyleOptionViewItemV4());
    QColor gridColor = static_cast<QRgb>(gridHint);
    pen = QPen(gridColor,0,tableView->gridStyle());
    view = tableView;
    m_imgConnect.load(":/summary/Image/summary/log_device.png");    //lanse
    m_imgClose.load(":/summary/Image/summary/log_negative.png"); //hongse
    m_imgBusy.load(":/summary/Image/summary/log_busy.png"); //chengse
    m_imgAck.load(":/summary/Image/summary/log_ack.png");   //huangse
}


/*
** 代理模型的重画view界面的函数
*/
void ModbusClientDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_ASSERT(index.isValid());
    QPen oldPen = painter->pen();
    painter->setPen(pen);
    if (index.row() == 0)
        painter->drawLine(option.rect.topLeft(),option.rect.topRight());
    painter->drawLine(option.rect.bottomLeft(),option.rect.bottomRight());
    painter->setPen(oldPen);

    QString str = index.model()->data(index, Qt::DisplayRole).toString();
    int x = option.rect.x();
    int y = option.rect.y();
    int width = option.rect.width();
    int height = option.rect.height();

    if (index.column() == NUM_COLUMN)
    {
        painter->drawText(option.rect, Qt::AlignCenter, str);
    }
    else if (index.column() == STATUS_COLUMN)
    {
        QString kind = index.model()->data(index.model()->index(index.row(), STATUS_COLUMN - 1)).toString();
        QString strStatus;
        QImage image;
        int type = str.toInt();
        if (type == 0)
            image = m_imgConnect;
        else if (type >= 0 && type < 20)
            image = m_imgBusy;
        else if (type >= 20 && type < 30)
            image = m_imgClose;
        else if (type >= 30 && type <= 35)
            image = m_imgAck;
        getImageAndStatus(str.toInt(), strStatus);

        if (image.width() != 0)
            image = image.scaled(height / 2, height / 2, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);  //
        painter->drawText(x, y, 20, height, Qt::AlignVCenter, kind);
        painter->drawImage(x + 20, y + (height - image.height()) / 2, image);
        painter->drawText(x + 60, y, width - height - 20, height, Qt::TextWordWrap | Qt::AlignVCenter, strStatus);
    }
    else if (index.column() == CHANNEL_COLUMN)
    {
        painter->drawText(x + 2, y, width / 2, height, Qt::AlignVCenter, str.left(4));
        painter->drawText(x + width / 2, y, width / 2, height, Qt::AlignVCenter, str.right(4));
    }
    else
    {
        painter->drawText(x, y, width, height, Qt::AlignVCenter, str);
    }
}



/*
 * 网格显示的代理
*/

#define WIN_MIN_FONT_SIZE                  8                                                    //字体最小尺寸
#define WIN_MIN_ALARM_SIZE               WIN_MIN_FONT_SIZE + 10          //警报灯最小尺寸

ModbusGridlayoutDelegate::ModbusGridlayoutDelegate(SummaryTableView *tableview, QWidget *parent)
    : QItemDelegate(parent), m_margins(-1,0,-6,-3)
{
    int gridHint = tableview->style()->styleHint(QStyle::SH_Table_GridLineColor, new QStyleOptionViewItemV4());
    QColor gridColor = static_cast<QRgb>(gridHint);
    pen = QPen(gridColor,0,tableview->gridStyle());
    view = tableview;
}


void ModbusGridlayoutDelegate::paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
    if (index.data(Qt::DisplayRole).isValid()) {
        painter->save();
        painter->setRenderHint(QPainter::Antialiasing);

        QStyleOptionViewItem opt = setOption(option,index);
        QRectF modbusRect;
        qreal moubusSpan;

        doLayout(option, modbusRect, moubusSpan);
        drawModbusRect(painter,opt,index,modbusRect);
        drawPressingRect(painter,opt,index);
        painter->restore();
    }
}


QSize ModbusGridlayoutDelegate::sizeHint(const QStyleOptionViewItem & option, const QModelIndex & /*index*/) const
{
    return option.rect.size();
}

void ModbusGridlayoutDelegate::doLayout(const QStyleOptionViewItem &option, QRectF &modbusRect, qreal &modbusSpan) const
{
    modbusRect = option.rect;
    modbusRect.setHeight(modbusRect.height() - 3);
    modbusRect.setWidth(modbusRect.width() - 3);
    modbusSpan = modbusRect.width();
}


QStyleOptionViewItem ModbusGridlayoutDelegate::setOption(const QStyleOptionViewItem & option, const QModelIndex &/* index*/) const
{
    QStyleOptionViewItem opt = option;
    opt.rect = option.rect.marginsAdded(m_margins); //边框缩进
    if(option.state & QStyle::State_Sunken && option.state & QStyle::State_HasFocus){
        opt.rect.moveTo(opt.rect.x()+1, opt.rect.y()+1);
    }
    return opt;
}

void ModbusGridlayoutDelegate::drawModbusRect(QPainter *painter, const QStyleOptionViewItem &/*option*/, const QModelIndex &index, QRectF &modbusRect) const
{
    painter->save();

    QString modbus;
    QVariant value1;
    value1 = index.data();
    if (value1.isValid())
    {
        modbus = value1.toString();
        QStringList modbusList = modbus.split(",");
        int type = modbusList[2].toInt();
        QColor modbusColor;
        if (type == 0)
            modbusColor = QColor(Qt::blue);
        else if (type >= 0 && type < 20)
            modbusColor = QColor(255, 128, 5);
        else if (type >= 20 && type < 30)
            modbusColor = QColor(Qt::red);
        else if (type >= 30 && type <= 35)
            modbusColor = QColor(Qt::yellow);

        painter->setBrush(QBrush(modbusColor));
        painter->drawRoundedRect(modbusRect,6,6);
        painter->drawText(modbusRect,  Qt::AlignCenter, modbusList[0]);
    }
    painter->restore();
}


/**
 * 去掉系统自带的toolTip
 * @return
 */
bool ModbusGridlayoutDelegate::helpEvent(QHelpEvent */*event*/,
                                         QAbstractItemView */*view*/,
                                         const QStyleOptionViewItem &/*option*/,
                                         const QModelIndex &/*index*/)
{
    return false;
}

/**
 * 功能：
 *          绘制按钮按压效果
 */
void ModbusGridlayoutDelegate::drawPressingRect(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &/*index*/) const
{
    painter->save();

    /*********************按压效果*************************/
    if(option.state & QStyle::State_Sunken && option.state & QStyle::State_HasFocus){
        painter->setPen(QPen(QColor(0,0,0,100),3));
        painter->setBrush(QBrush(QColor(180,180,180,60)));
        painter->drawRoundedRect(option.rect,6,6);
    }
    painter->restore();
}




