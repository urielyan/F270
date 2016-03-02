#include "winlog.h"
#include <QHeaderView>
#include <QDebug>
#include "winmessagesummary.h"
#include "./GlobalData/globalDef.h"
#include "./Common/logger.h"

QString spaceStr = "                                ";
extern void getImageAndStatus(const int type, QString &str);
extern QString getTimeFormate();
extern QString getSysTimeFormate(int width, int height);

WinLog::WinLog(QWidget *parent) : QWidget(parent)
{
    connect(CfgChannel::instance(), SIGNAL(sigNormalConfigChanged(quint32)), this, SLOT(slotDateTimeFormateChanged(quint32)));
}


/*********************************operation log***********************************/

#define Ope_Time_Column         0
#define Ope_Action_Column       1
#define Ope_Factor_Column       2
#define Ope_User_Column         3
WinOperationLog::WinOperationLog(QWidget * parent) : WinLog(parent)
{
    m_operationLog.setParent(this);
    p_operationModel = new ModelLog(Operation_Log);
    m_operationLog.p_view->setModel(p_operationModel);
    m_operationLog.p_view->setItemDelegate(new LogDelegate(m_operationLog.p_view, OperationLog));
    m_operationLog.p_view->setSortingEnabled(false);
    m_operationLog.p_view->setEditTriggers(false);

    initHeadVLine();
}

WinOperationLog::~WinOperationLog()
{
    delete p_operationModel;
}

void WinOperationLog::initHeadVLine()
{
    for (int i = 0; i < 3; i++)
    {
        m_operationHeadVFrame[i].setParent(m_operationLog.p_view);
        m_operationHeadVFrame[i].setFrameShape(QFrame::VLine);
        m_operationHeadVFrame[i].setFrameShadow(QFrame::Sunken);
        m_operationHeadVFrame[i].hide();
    }
}

void WinOperationLog::setVline()
{
    int widthLocal[3];
    widthLocal[0] = m_operationLog.p_view->columnWidth(0);
    widthLocal[1] = widthLocal[0] + m_operationLog.p_view->columnWidth(1);
    widthLocal[2] = widthLocal[1] + m_operationLog.p_view->columnWidth(2);

    for (int i = 0; i < 3; i++)
        m_operationHeadVFrame[i].setGeometry(QRect(widthLocal[i] - 6, 6, 4, m_operationLog.p_view->horizontalHeader()->height() - 12 ));
}


void WinOperationLog::resizeEvent(QResizeEvent */*event*/)
{
    m_operationLog.resize(this->width(), this->height());
    resetColumnWidth();
    initModelHeaderData();
    setVline();
}


void WinOperationLog::setCurrentRow(int current, int sumrow)
{
    if (current < 11)
        current = 11;
    if (sumrow < 11)
        current = sumrow;

    if (this->width() < DESKTOP_WIDTH * 0.35 || this->height() < DESKTOP_HEIGHT * 0.35)
        m_operationLog.p_headerLabel->setText(QString("%1/%2\t%3").arg(current, 4, 10, QChar('0'))
                                               .arg(sumrow, 4, 10, QChar('0'))
                                               .arg(tr("Event log")));
    else
        m_operationLog.p_headerLabel->setText(QString("%1/%2").arg(current, 4, 10, QChar('0')).arg(sumrow, 4, 10, QChar('0')));
}

/*
 * 当界面大小变化时，调整列宽，行高和列的显示隐藏
*/
void WinOperationLog::resetColumnWidth()
{
    int width = this->width();
    m_operationLog.p_upBtn->hide();
    m_operationLog.p_downBtn->hide();
    m_operationLog.p_view->setColumnHidden(3, false);

    m_operationLog.p_view->verticalHeader()->setDefaultSectionSize(m_operationLog.height() / 12);
    if (width > DESKTOP_WIDTH * 0.75)
    {
        m_operationLog.p_view->setColumnWidth(Ope_Time_Column, this->width() / 3);
        m_operationLog.p_view->setColumnWidth(1, this->width() / 6);
        m_operationLog.p_upBtn->show();
        m_operationLog.p_downBtn->show();
    }
    else if (width <= DESKTOP_WIDTH * 0.75 && width > DESKTOP_WIDTH * 0.35)
    {
        m_operationLog.p_view->setColumnWidth(0, width / 5);
        m_operationLog.p_view->setColumnWidth(1, width / 5);
        m_operationLog.p_view->setColumnWidth(2, width / 4);
    }
    else
    {
        m_operationLog.p_view->setColumnWidth(0, width / 3);
        m_operationLog.p_view->setColumnWidth(1, width / 3);
        m_operationLog.p_view->setColumnWidth(2, width / 3);
        m_operationLog.p_view->setColumnHidden(3, true);
    }
}


/*
 * view的表头和upBtn与p_downBtn的隐藏与显示
 */
void WinOperationLog::initModelHeaderData()
{
    int width = this->width();
    int height = this->height();
    int heightHeader;
    QFont font;

    m_operationLog.p_headerLabel->setGeometry(0, 0, 400, 25);
    for (int i = 0; i < 3; i++)
        m_operationHeadVFrame[i].hide();
    if (width > DESKTOP_WIDTH * 0.75)
    {
        m_operationLog.p_headerLabel->setGeometry(100, 0, 400, 25);
        setHeaderData();
        for (int i = 0; i < 3; i++)
            m_operationHeadVFrame[i].show();
        if (height > DESKTOP_HEIGHT * 0.75)
        {
            font.setPixelSize(height / 17 * 0.4);
            heightHeader = 50;
        }
        else/* if (height > DESKTOP_HEIGHT * 0.33 && height <= DESKTOP_HEIGHT * 0.75)*/
        {
            font.setPixelSize(height / 14 * 0.5);
            heightHeader = 45;
        }
        m_operationLog.p_headerLabel->setFixedHeight(heightHeader / 2);
    }
    else if (width > DESKTOP_WIDTH * 0.35 && width <= DESKTOP_WIDTH * 0.75)
    {
        if (height > DESKTOP_HEIGHT * 0.75)
        {
            font.setPixelSize(height / 24 * 0.5);
            setHeaderData();
            p_operationModel->setHeaderData(Ope_Time_Column, Qt::Horizontal, tr("Time"));
            heightHeader = 50;
            m_operationLog.p_headerLabel->setFixedHeight(heightHeader / 3);
        }
        else if (height > DESKTOP_HEIGHT * 0.35 && height <= DESKTOP_HEIGHT * 0.75)
        {
            font.setPixelSize(height / 14 * 0.5);
            setHeaderData();
            p_operationModel->setHeaderData(Ope_Time_Column, Qt::Horizontal, tr("Time"));
            heightHeader = 50;
            m_operationLog.p_headerLabel->setFixedHeight(heightHeader / 2);
        }
        else
        {
            font.setPixelSize(height / 14 * 0.8);
            setHeaderNull();
            heightHeader = 25;
            m_operationLog.p_view->verticalHeader()->setDefaultSectionSize(height / 8);
        }
    }
    else
    {
        setHeaderNull();
        heightHeader = 25;
        m_operationLog.p_headerLabel->setFixedHeight(heightHeader);
        m_operationLog.p_view->verticalHeader()->setDefaultSectionSize(height / 8);
        if (height > DESKTOP_HEIGHT * 0.35 && height <= DESKTOP_HEIGHT * 0.75)
            font.setPixelSize(height / 17 * 0.6);
        else
            font.setPixelSize(height / 17);
    }
    slotDateTimeFormateChanged(ID_System_Env_Date);
    m_operationLog.p_view->horizontalHeader()->setFont(font);
    m_operationLog.p_view->horizontalHeader()->setFixedHeight(heightHeader);
    m_operationLog.p_view->setFont(font);
    m_operationLog.p_headerLabel->setFont(font);
}

void WinOperationLog::setHeaderData()
{
    p_operationModel->setHeaderData(0, Qt::Horizontal, spaceStr + tr("Time"));
    p_operationModel->setHeaderData(1, Qt::Horizontal, tr("Action"));
    p_operationModel->setHeaderData(2, Qt::Horizontal, tr("Factor"));
    p_operationModel->setHeaderData(3, Qt::Horizontal, tr("User"));
}

void WinOperationLog::setHeaderNull()
{
    p_operationModel->setHeaderData(0, Qt::Horizontal, "");
    p_operationModel->setHeaderData(1, Qt::Horizontal, "");
    p_operationModel->setHeaderData(2, Qt::Horizontal, "");
    p_operationModel->setHeaderData(3, Qt::Horizontal, "");
}


void WinOperationLog::paintEvent(QPaintEvent *event)
{
    p_operationModel->logFileIsChanged(EVENT_LOG_PATH);
    int currentrow = m_operationLog.p_view->rowAt(m_operationLog.p_view->y());
    setCurrentRow(currentrow + 12, p_operationModel->rowCount());
    QWidget::paintEvent(event);
}

void WinOperationLog::slotDateTimeFormateChanged(quint32 ID)
{
    if (ID != ID_System_Env_Date)
        return ;

    QString timeFormate = getSysTimeFormate(this->width(), this->height());
    ((LogDelegate *)m_operationLog.p_view->itemDelegate())->setTimeFormateStr(timeFormate);
}




/*********************************error log***********************************/
WinErrorLog::WinErrorLog(QWidget * parent) : WinLog(parent)
{
    m_errorLog.setParent(this);

    p_errorModel = new ModelLog(Error_Log);
    m_errorLog.p_view->setModel(p_errorModel);
    m_errorLog.p_view->setItemDelegate(new LogDelegate(m_errorLog.p_view, ErrorLog));
    m_errorLog.p_view->setSortingEnabled(false);
    m_errorLog.p_view->setEditTriggers(false);
    m_errorLog.p_view->horizontalHeader()->setDefaultAlignment(Qt::AlignVCenter);

    initHeadVLine();
}

WinErrorLog::~WinErrorLog()
{
    delete p_errorModel;
}

void WinErrorLog::initHeadVLine()
{
    for (int i = 0; i < 2; i++)
    {
        m_erorHeadVFrame[i].setParent(m_errorLog.p_view);
        m_erorHeadVFrame[i].setFrameShape(QFrame::VLine);
        m_erorHeadVFrame[i].setFrameShadow(QFrame::Sunken);
        m_erorHeadVFrame[i].hide();
    }
}


void WinErrorLog::setVline()
{
    int widthLocal[2];
    widthLocal[0] = m_errorLog.p_view->columnWidth(0);
    widthLocal[1] = widthLocal[0] + m_errorLog.p_view->columnWidth(1);

    for (int i = 0; i < 2; i++)
        m_erorHeadVFrame[i].setGeometry(QRect(widthLocal[i] - 6, 6, 4, m_errorLog.p_view->horizontalHeader()->height() - 12 ));
}



void WinErrorLog::resizeEvent(QResizeEvent */*event*/)
{
    m_errorLog.resize(this->width(), this->height());
    resetColumnWidth();
    initModelHeaderData();
    setVline();
}

void WinErrorLog::setCurrentRow(int current, int sumrow)
{
    if (current < 11)
        current = 11;
    if (sumrow < 11)
        current = sumrow;
    if (this->width() < DESKTOP_WIDTH * 0.35 || this->height() < DESKTOP_HEIGHT * 0.35)
        m_errorLog.p_headerLabel->setText(QString("%1/%2\t%3").arg(current, 4, 10, QChar('0'))
                                               .arg(sumrow, 4, 10, QChar('0'))
                                               .arg(tr("Error log")));
    else
        m_errorLog.p_headerLabel->setText(QString("%1/%2").arg(current, 4, 10, QChar('0')).arg(sumrow, 4, 10, QChar('0')));
}


/*
 * 当界面大小变化时，调整列宽，行高和列的显示隐藏
*/
void WinErrorLog::resetColumnWidth()
{
    int width = this->width();
    m_errorLog.p_upBtn->hide();
    m_errorLog.p_downBtn->hide(); 

    m_errorLog.p_view->verticalHeader()->setDefaultSectionSize(m_errorLog.height() / 12);
    if (width > DESKTOP_WIDTH * 0.75)
    {
        m_errorLog.p_view->setColumnWidth(Ope_Time_Column, this->width() / 4);
        m_errorLog.p_upBtn->show();
        m_errorLog.p_downBtn->show();
    }
    else if (width <= DESKTOP_WIDTH * 0.75 && width > DESKTOP_WIDTH * 0.35)
    {
        m_errorLog.p_view->setColumnWidth(Ope_Time_Column, this->width() / 3);
        m_errorLog.p_view->setColumnWidth(1, width / 8);
    }
    else
    {
        m_errorLog.p_view->setColumnWidth(Ope_Time_Column, this->width() / 3);
        m_errorLog.p_view->setColumnWidth(0, width / 3);
        m_errorLog.p_view->setColumnWidth(1, width / 6);
    }
}


/*
 * view的表头和upBtn与p_downBtn的隐藏与显示
 */
void WinErrorLog::initModelHeaderData()
{
    int width = this->width();
    int height = this->height();
    int heightHeader;
    QFont font;
    m_errorLog.p_headerLabel->setGeometry(0, 0, 400, 25);
    for (int i = 0; i < 2; i++)
        m_erorHeadVFrame[i].hide();
    if (width > DESKTOP_WIDTH * 0.75)
    {
        m_errorLog.p_headerLabel->setGeometry(100, 0, 400, 25);
        setHeaderData();
        heightHeader = 45;
        m_errorLog.p_headerLabel->setFixedHeight(heightHeader / 2);
        for (int i = 0; i < 2; i++)
            m_erorHeadVFrame[i].show();
        if (height > DESKTOP_HEIGHT * 0.75)
        {
            font.setPixelSize(height / 17 * 0.4);
            heightHeader = 50;
        }
        else/* if (height > DESKTOP_HEIGHT * 0.33 && height <= DESKTOP_HEIGHT * 0.75)*/
        {
            font.setPixelSize(height / 14 * 0.5);
            heightHeader = 45;
        }
    }
    else if (width > DESKTOP_WIDTH * 0.35 && width <= DESKTOP_WIDTH * 0.75)
    {
        if (height > DESKTOP_HEIGHT * 0.75)
        {
            font.setPixelSize(height / 24 * 0.5);
            setHeaderData();
            p_errorModel->setHeaderData(Ope_Time_Column, Qt::Horizontal, tr("Time"));
            heightHeader = 50;
            m_errorLog.p_headerLabel->setFixedHeight(heightHeader / 3);
        }
        else if (height > DESKTOP_HEIGHT * 0.35 && height <= DESKTOP_HEIGHT * 0.75)
        {
            font.setPixelSize(height / 14 * 0.5);
            setHeaderData();
            p_errorModel->setHeaderData(Ope_Time_Column, Qt::Horizontal, tr("Time"));
            heightHeader = 50;
            m_errorLog.p_headerLabel->setFixedHeight(heightHeader / 2);
        }
        else
        {
            font.setPixelSize(height / 14 * 0.8);
            setHeaderNull();
            heightHeader = 25;
            m_errorLog.p_view->verticalHeader()->setDefaultSectionSize(height / 8);
        }
    }
    else
    {
        setHeaderNull();
        heightHeader = 25;
        m_errorLog.p_headerLabel->setFixedHeight(heightHeader);
        m_errorLog.p_view->verticalHeader()->setDefaultSectionSize(height / 8);
        if (height > DESKTOP_HEIGHT * 0.35 && height <= DESKTOP_HEIGHT * 0.75)
            font.setPixelSize(height / 17 * 0.6);
        else
            font.setPixelSize(height / 17);
    }
    slotDateTimeFormateChanged(ID_System_Env_Date);
    m_errorLog.p_view->horizontalHeader()->setFixedHeight(heightHeader);
    m_errorLog.p_view->horizontalHeader()->setFont(font);
    m_errorLog.p_view->setFont(font);
    m_errorLog.p_headerLabel->setFont(font);
}

void WinErrorLog::setHeaderData()
{
    p_errorModel->setHeaderData(0, Qt::Horizontal, spaceStr + tr("Time"));
    p_errorModel->setHeaderData(1, Qt::Horizontal, tr("No"));
    p_errorModel->setHeaderData(2, Qt::Horizontal, tr("Message"));
}

void WinErrorLog::setHeaderNull()
{
    p_errorModel->setHeaderData(0, Qt::Horizontal, "");
    p_errorModel->setHeaderData(1, Qt::Horizontal, "");
    p_errorModel->setHeaderData(2, Qt::Horizontal, "");
}


void WinErrorLog::paintEvent(QPaintEvent *event)
{
    p_errorModel->logFileIsChanged(ERROR_LOG_PATH);
    int currentrow = m_errorLog.p_view->rowAt(m_errorLog.p_view->y());
    setCurrentRow(currentrow + 12, p_errorModel->rowCount());
    QWidget::paintEvent(event);
}

void WinErrorLog::slotDateTimeFormateChanged(quint32 ID)
{
    if (ID != ID_System_Env_Date)
        return ;

    QString timeFormate = getSysTimeFormate(this->width(), this->height());
    ((LogDelegate *)m_errorLog.p_view->itemDelegate())->setTimeFormateStr(timeFormate);
}





/*********************************comm log***********************************/
WinCommLog::WinCommLog(QWidget * parent) : WinLog(parent)
{
    m_commLog.setParent(this);
    initHeadVLine();
    p_commModel = new ModelLog(Comm_Log);
    m_commLog.p_view->setModel(p_commModel);
    m_commLog.p_view->setItemDelegate(new LogDelegate(m_commLog.p_view, CommunicationLog));
    m_commLog.p_view->setSortingEnabled(false);
    m_commLog.p_view->setEditTriggers(false);
}

WinCommLog::~WinCommLog()
{
    delete p_commModel;
}

void WinCommLog::initHeadVLine()
{
    for (int i = 0; i < 5; i++)
    {
        m_commHeadVFrame[i].setParent(m_commLog.p_view);
        m_commHeadVFrame[i].setFrameShape(QFrame::VLine);
        m_commHeadVFrame[i].setFrameShadow(QFrame::Sunken);
        m_commHeadVFrame[i].hide();
    }
}


void WinCommLog::setVline()
{
    int widthLocal[5];
    widthLocal[0] = m_commLog.p_view->columnWidth(0);
    widthLocal[1] = widthLocal[0] + m_commLog.p_view->columnWidth(1);
    widthLocal[2] = widthLocal[1] + m_commLog.p_view->columnWidth(2);
    widthLocal[3] = widthLocal[2] + m_commLog.p_view->columnWidth(3);
    widthLocal[4] = widthLocal[3] + m_commLog.p_view->columnWidth(4);

    for (int i = 0; i < 5; i++)
        m_commHeadVFrame[i].setGeometry(QRect(widthLocal[i] - 6, 6, 4, m_commLog.p_view->horizontalHeader()->height() - 12));
}

void WinCommLog::resizeEvent(QResizeEvent */*event*/)
{
    m_commLog.resize(this->width(), this->height());
    resetColumnWidth();
    initModelHeaderData();
    setVline();
}

void WinCommLog::setCurrentRow(int current, int sumrow)
{
    if (current < 11)
        current = 11;
    if (sumrow < 11)
        current = sumrow;
    if (this->width() < DESKTOP_WIDTH * 0.35 || this->height() < DESKTOP_HEIGHT * 0.35)
        m_commLog.p_headerLabel->setText(QString("%1/%2\t%3").arg(current, 4, 10, QChar('0'))
                                               .arg(sumrow, 4, 10, QChar('0'))
                                               .arg(tr("General log")));
    else
        m_commLog.p_headerLabel->setText(QString("%1/%2").arg(current, 4, 10, QChar('0')).arg(sumrow, 4, 10, QChar('0')));
}


/*
 * 当界面大小变化时，调整列宽，行高和列的显示隐藏
*/
void WinCommLog::resetColumnWidth()
{
    int width = this->width();
    m_commLog.p_upBtn->hide();
    m_commLog.p_downBtn->hide();

    m_commLog.p_view->verticalHeader()->setDefaultSectionSize(m_commLog.height() / 12);
    m_commLog.p_view->setColumnWidth(0, width / 4);
    m_commLog.p_view->setColumnWidth(1, width / 10);
    m_commLog.p_view->setColumnWidth(2, width / 8);
    m_commLog.p_view->setColumnWidth(3, width / 8);
    m_commLog.p_view->setColumnWidth(4, width / 10);
    if (width > DESKTOP_WIDTH * 0.75)
    {
        m_commLog.p_upBtn->show();
        m_commLog.p_downBtn->show();
    }
}


/*
 * view的表头和upBtn与p_downBtn的隐藏与显示
 */
void WinCommLog::initModelHeaderData()
{
    int width = this->width();
    int height = this->height();
    int heightHeader;
    m_commLog.p_headerLabel->setGeometry(0, 0, 400, 25);
    QFont font;
    for (int i = 0; i < 5; i++)
        m_commHeadVFrame[i].hide();
    if (width > DESKTOP_WIDTH * 0.75)
    {
        m_commLog.p_headerLabel->setGeometry(100, 0, 400, 25);
        setHeaderData();
        for (int i = 0; i < 5; i++)
            m_commHeadVFrame[i].show();
        if (height > DESKTOP_HEIGHT * 0.75)
        {
            font.setPixelSize(height / 17 * 0.4);
            heightHeader = 50;
        }
        else/* if (height > DESKTOP_HEIGHT * 0.33 && height <= DESKTOP_HEIGHT * 0.75)*/
        {
            font.setPixelSize(height / 14 * 0.5);
            heightHeader = 45;
        }
        m_commLog.p_headerLabel->setFixedHeight(heightHeader / 2);
    }
    else if (width > DESKTOP_WIDTH * 0.35 && width <= DESKTOP_WIDTH * 0.75)
    {
        if (height > DESKTOP_HEIGHT * 0.75)
        {
            font.setPixelSize(height / 24 * 0.5);
            setHeaderData();
            p_commModel->setHeaderData(Ope_Time_Column, Qt::Horizontal, tr("Time"));
            heightHeader = 50;
            m_commLog.p_headerLabel->setFixedHeight(heightHeader / 3);
        }
        else if (height > DESKTOP_HEIGHT * 0.35 && height <= DESKTOP_HEIGHT * 0.75)
        {
            font.setPixelSize(height / 14 * 0.5);
            setHeaderData();
            p_commModel->setHeaderData(Ope_Time_Column, Qt::Horizontal, tr("Time"));
            heightHeader = 50;
            m_commLog.p_headerLabel->setFixedHeight(heightHeader / 2);
        }
        else
        {
            font.setPixelSize(height / 14 * 0.8);
            setHeaderNull();
            heightHeader = 25;
            m_commLog.p_view->verticalHeader()->setDefaultSectionSize(height / 8);
        }
    }
    else
    {
        setHeaderNull();
        heightHeader = 25;
        m_commLog.p_headerLabel->setFixedHeight(heightHeader);
        m_commLog.p_view->verticalHeader()->setDefaultSectionSize(height / 8);
        if (height > DESKTOP_HEIGHT * 0.35 && height <= DESKTOP_HEIGHT * 0.75)
            font.setPixelSize(height / 17 * 0.6);
        else
            font.setPixelSize(height / 17);
    }
    slotDateTimeFormateChanged(ID_System_Env_Date);
    m_commLog.p_view->horizontalHeader()->setFixedHeight(heightHeader);
    m_commLog.p_view->horizontalHeader()->setFont(font);
    m_commLog.p_view->setFont(font);
    m_commLog.p_headerLabel->setFont(font);
}

void WinCommLog::setHeaderData()
{
    p_commModel->setHeaderData(0, Qt::Horizontal, spaceStr + tr("Time"));
    p_commModel->setHeaderData(1, Qt::Horizontal, tr("No"));
    p_commModel->setHeaderData(2, Qt::Horizontal, tr("User"));
    p_commModel->setHeaderData(3, Qt::Horizontal, tr("Multi"));
    p_commModel->setHeaderData(4, Qt::Horizontal, tr("I/O"));
    p_commModel->setHeaderData(5, Qt::Horizontal, tr("Message"));
}

void WinCommLog::setHeaderNull()
{
    p_commModel->setHeaderData(0, Qt::Horizontal, "");
    p_commModel->setHeaderData(1, Qt::Horizontal, "");
    p_commModel->setHeaderData(2, Qt::Horizontal, "");
    p_commModel->setHeaderData(3, Qt::Horizontal, "");
    p_commModel->setHeaderData(4, Qt::Horizontal, "");
    p_commModel->setHeaderData(5, Qt::Horizontal, "");
}


void WinCommLog::paintEvent(QPaintEvent *event)
{
    p_commModel->logFileIsChanged(GENERAL_LOG_PATH);
    int currentrow = m_commLog.p_view->rowAt(m_commLog.p_view->y());
    setCurrentRow(currentrow + 12, p_commModel->rowCount());
    QWidget::paintEvent(event);
}

void WinCommLog::slotDateTimeFormateChanged(quint32 ID)
{
    if (ID != ID_System_Env_Date)
        return ;

    QString timeFormate = getSysTimeFormate(this->width(), this->height());
    ((LogDelegate *)m_commLog.p_view->itemDelegate())->setTimeFormateStr(timeFormate);
}





/*********************************FTP log***********************************/
WinFTPLog::WinFTPLog(QWidget * parent) : WinLog(parent)
{
    m_FTPLog.setParent(this);
    initHeadVLine();

    p_ftpModel = new ModelLog(FTP_Log);
    m_FTPLog.p_view->setModel(p_ftpModel);
    m_FTPLog.p_view->setItemDelegate(new LogDelegate(m_FTPLog.p_view, FTPLog));
    m_FTPLog.p_view->setSortingEnabled(false);
    m_FTPLog.p_view->setEditTriggers(false);
}

WinFTPLog::~WinFTPLog()
{
    delete p_ftpModel;
}

void WinFTPLog::initHeadVLine()
{
    for (int i = 0; i < 4; i++)
    {
        m_FTPHeadVFrame[i].setParent(m_FTPLog.p_view);
        m_FTPHeadVFrame[i].setFrameShape(QFrame::VLine);
        m_FTPHeadVFrame[i].setFrameShadow(QFrame::Sunken);
        m_FTPHeadVFrame[i].hide();
    }
}

void WinFTPLog::setVline()
{
    int widthLocal[4];
    widthLocal[0] = m_FTPLog.p_view->columnWidth(0);
    widthLocal[1] = widthLocal[0] + m_FTPLog.p_view->columnWidth(1);
    widthLocal[2] = widthLocal[1] + m_FTPLog.p_view->columnWidth(2);
    widthLocal[3] = widthLocal[2] + m_FTPLog.p_view->columnWidth(3);

    for (int i = 0; i < 4; i++)
        m_FTPHeadVFrame[i].setGeometry(QRect(widthLocal[i] - 6, 6, 4, m_FTPLog.p_view->horizontalHeader()->height() - 12 ));
}

void WinFTPLog::resizeEvent(QResizeEvent */*event*/)
{
    m_FTPLog.resize(this->width(), this->height());
    //setViewStyle();
    resetColumnWidth();
    initModelHeaderData();

    setVline();
}

void WinFTPLog::setCurrentRow(int current, int sumrow)
{
    if (current < 11)
        current = 11;
    if (sumrow < 11)
        current = sumrow;
    if (this->width() < DESKTOP_WIDTH * 0.35 || this->height() < DESKTOP_HEIGHT * 0.35)
        m_FTPLog.p_headerLabel->setText(QString("%1/%2\t%3").arg(current, 4, 10, QChar('0'))
                                               .arg(sumrow, 4, 10, QChar('0'))
                                               .arg(tr("General log")));
    else
        m_FTPLog.p_headerLabel->setText(QString("%1/%2").arg(current, 4, 10, QChar('0')).arg(sumrow, 4, 10, QChar('0')));
}


/*
 * 当界面大小变化时，调整列宽，行高和列的显示隐藏
*/
void WinFTPLog::resetColumnWidth()
{
    int width = this->width();
    m_FTPLog.p_upBtn->hide();
    m_FTPLog.p_downBtn->hide();

    m_FTPLog.p_view->verticalHeader()->setDefaultSectionSize(m_FTPLog.height() / 12);
    m_FTPLog.p_view->verticalHeader()->setDefaultSectionSize(m_FTPLog.height() / 12);
    m_FTPLog.p_view->setColumnWidth(0, this->width() / 4);
    m_FTPLog.p_view->setColumnWidth(1, this->width() / 8);
    m_FTPLog.p_view->setColumnWidth(2, this->width() / 6);
    m_FTPLog.p_view->setColumnWidth(3, this->width() / 8);
    if (width > DESKTOP_WIDTH * 0.75)
    {
        m_FTPLog.p_upBtn->show();
        m_FTPLog.p_downBtn->show();
    }
}

void WinFTPLog::slotDateTimeFormateChanged(quint32 ID)
{
    if (ID != ID_System_Env_Date)
        return ;

    QString timeFormate = getSysTimeFormate(this->width(), this->height());
    ((LogDelegate *)m_FTPLog.p_view->itemDelegate())->setTimeFormateStr(timeFormate);
}


/*
 * view的表头和upBtn与p_downBtn的隐藏与显示
 */
void WinFTPLog::initModelHeaderData()
{
    int width = this->width();
    int height = this->height();
    int heightHeader;
    QFont font;
    m_FTPLog.p_headerLabel->setGeometry(0, 0, 400, 25);
    for (int i = 0; i < 4; i++)
        m_FTPHeadVFrame[i].hide();
    if (width > DESKTOP_WIDTH * 0.75)
    {
        m_FTPLog.p_headerLabel->setGeometry(100, 0, 400, 25);
        setHeaderData();
        if (height > DESKTOP_HEIGHT * 0.75)
        {
            font.setPixelSize(height / 17 * 0.4);
            heightHeader = 50;
        }
        else/* if (height > DESKTOP_HEIGHT * 0.33 && height <= DESKTOP_HEIGHT * 0.75)*/
        {
            font.setPixelSize(height / 14 * 0.5);
            heightHeader = 45;
        }
        m_FTPLog.p_headerLabel->setFixedHeight(heightHeader / 2);
        for (int i = 0; i < 4; i++)
            m_FTPHeadVFrame[i].show();
    }
    else if (width > DESKTOP_WIDTH * 0.35 && width <= DESKTOP_WIDTH * 0.75)
    {
        if (height > DESKTOP_HEIGHT * 0.75)
        {
            font.setPixelSize(height / 24 * 0.5);
            setHeaderData();
            p_ftpModel->setHeaderData(Ope_Time_Column, Qt::Horizontal, tr("Time"));
            heightHeader = 50;
            m_FTPLog.p_headerLabel->setFixedHeight(heightHeader / 3);
        }
        else if (height > DESKTOP_HEIGHT * 0.35 && height <= DESKTOP_HEIGHT * 0.75)
        {
            font.setPixelSize(height / 14 * 0.5);
            setHeaderData();
            p_ftpModel->setHeaderData(Ope_Time_Column, Qt::Horizontal, tr("Time"));
            heightHeader = 50;
            m_FTPLog.p_headerLabel->setFixedHeight(heightHeader / 2);
        }
        else
        {
            font.setPixelSize(height / 14 * 0.8);
            setHeaderNull();
            heightHeader = 25;
            m_FTPLog.p_view->verticalHeader()->setDefaultSectionSize(height / 8);
        }
    }
    else
    {
        setHeaderNull();
        heightHeader = 25;
        m_FTPLog.p_headerLabel->setFixedHeight(heightHeader);
        m_FTPLog.p_view->verticalHeader()->setDefaultSectionSize(height / 8);
        if (height > DESKTOP_HEIGHT * 0.35 && height <= DESKTOP_HEIGHT * 0.75)
            font.setPixelSize(height / 14 * 0.6);
        else
            font.setPixelSize(height / 14);
    }
    slotDateTimeFormateChanged(ID_System_Env_Date);
    m_FTPLog.p_view->horizontalHeader()->setFixedHeight(heightHeader);
    m_FTPLog.p_view->horizontalHeader()->setFont(font);
    m_FTPLog.p_view->setFont(font);
    m_FTPLog.p_headerLabel->setFont(font);
}

void WinFTPLog::setHeaderData()
{
    p_ftpModel->setHeaderData(0, Qt::Horizontal, spaceStr + tr("Time"));
    p_ftpModel->setHeaderData(1, Qt::Horizontal, tr("No"));
    p_ftpModel->setHeaderData(2, Qt::Horizontal, tr("Code"));
    p_ftpModel->setHeaderData(3, Qt::Horizontal, tr("Flag"));
    p_ftpModel->setHeaderData(4, Qt::Horizontal, tr("File name"));
}

void WinFTPLog::setHeaderNull()
{
    p_ftpModel->setHeaderData(0, Qt::Horizontal, "");
    p_ftpModel->setHeaderData(1, Qt::Horizontal, "");
    p_ftpModel->setHeaderData(2, Qt::Horizontal, "");
    p_ftpModel->setHeaderData(3, Qt::Horizontal, "");
    p_ftpModel->setHeaderData(4, Qt::Horizontal, "");
}


void WinFTPLog::paintEvent(QPaintEvent *event)
{
    p_ftpModel->logFileIsChanged(FTP_LOG_PATH);
    int currentrow = m_FTPLog.p_view->rowAt(m_FTPLog.p_view->y());
    setCurrentRow(currentrow + 12, p_ftpModel->rowCount());
    QWidget::paintEvent(event);
}




/*********************************WEB log***********************************/
WinWebLog::WinWebLog(QWidget * parent) : WinLog(parent)
{
    m_WEBLog.setParent(this);
    initHeadVLine();

    p_webModel = new ModelLog(WEB_Log);
    m_WEBLog.p_view->setModel(p_webModel);
    m_WEBLog.p_view->setColumnHidden(2, true);
    m_WEBLog.p_view->setColumnHidden(5, true);
    m_WEBLog.p_view->setItemDelegate(new LogDelegate(m_WEBLog.p_view, WEBLog));
    m_WEBLog.p_view->setSortingEnabled(false);
    m_WEBLog.p_view->setEditTriggers(false);
}

WinWebLog::~WinWebLog()
{
    delete p_webModel;
}

void WinWebLog::initHeadVLine()
{
    for (int i = 0; i < 3; i++)
    {
        m_WEBHeadVFrame[i].setParent(m_WEBLog.p_view);
        m_WEBHeadVFrame[i].setFrameShape(QFrame::VLine);
        m_WEBHeadVFrame[i].setFrameShadow(QFrame::Sunken);
        m_WEBHeadVFrame[i].hide();
    }
}

void WinWebLog::setVline()
{
    int widthLocal[3];
    widthLocal[0] = m_WEBLog.p_view->columnWidth(0);
    widthLocal[1] = widthLocal[0] + m_WEBLog.p_view->columnWidth(1);
    widthLocal[2] = widthLocal[1] + m_WEBLog.p_view->columnWidth(3);

    for (int i = 0; i < 3; i++)
        m_WEBHeadVFrame[i].setGeometry(QRect(widthLocal[i] - 6, 6, 4, m_WEBLog.p_view->horizontalHeader()->height() - 12 ));
}


void WinWebLog::resizeEvent(QResizeEvent */*event*/)
{
    m_WEBLog.resize(this->width(), this->height());
    resetColumnWidth();
    initModelHeaderData();
    setVline();
}

void WinWebLog::setCurrentRow(int current, int sumrow)
{
    if (current < 11)
        current = 11;
    if (sumrow < 11)
        current = sumrow;
    if (this->width() < DESKTOP_WIDTH * 0.35 || this->height() < DESKTOP_HEIGHT * 0.35)
        m_WEBLog.p_headerLabel->setText(QString("%1/%2\t%3").arg(current, 4, 10, QChar('0'))
                                               .arg(sumrow, 4, 10, QChar('0'))
                                               .arg(tr("General log")));
    else
        m_WEBLog.p_headerLabel->setText(QString("%1/%2").arg(current, 4, 10, QChar('0')).arg(sumrow, 4, 10, QChar('0')));
}


/*
 * 当界面大小变化时，调整列宽，行高和列的显示隐藏
*/
void WinWebLog::resetColumnWidth()
{
    int width = this->width();
    m_WEBLog.p_upBtn->hide();
    m_WEBLog.p_downBtn->hide();

    m_WEBLog.p_view->verticalHeader()->setDefaultSectionSize(m_WEBLog.height() / 12);
    m_WEBLog.p_view->horizontalHeader()->setFixedHeight(50);
    m_WEBLog.p_view->setColumnWidth(0, this->width() / 4);
    m_WEBLog.p_view->setColumnWidth(1, this->width() / 4);
    m_WEBLog.p_view->setColumnWidth(3, this->width() / 3);
    m_WEBLog.p_view->setColumnHidden(3, false);
    if (width > DESKTOP_WIDTH * 0.75)
    {
        m_WEBLog.p_upBtn->show();
        m_WEBLog.p_downBtn->show();
    }
    else if (width < DESKTOP_WIDTH * 0.35)
    {
        m_WEBLog.p_view->setColumnHidden(3, true);
        m_WEBLog.p_view->setColumnWidth(1, this->width() / 3);
    }
}


/*
 * view的表头和upBtn与p_downBtn的隐藏与显示
 */
void WinWebLog::initModelHeaderData()
{
    int width = this->width();
    int height = this->height();
    int heightHeader;
    QFont font;
    m_WEBLog.p_headerLabel->setGeometry(0, 0, 400, 25);
    for (int i = 0; i < 3; i++)
        m_WEBHeadVFrame[i].hide();
    if (width > DESKTOP_WIDTH * 0.75)
    {
        m_WEBLog.p_headerLabel->setGeometry(100, 0, 400, 25);
        setHeaderData();
        if (height > DESKTOP_HEIGHT * 0.75)
        {
            font.setPixelSize(height / 17 * 0.4);
            heightHeader = 50;
        }
        else/* if (height > DESKTOP_HEIGHT * 0.33 && height <= DESKTOP_HEIGHT * 0.75)*/
        {
            font.setPixelSize(height / 14 * 0.5);
            heightHeader = 45;
        }
        m_WEBLog.p_headerLabel->setFixedHeight(heightHeader / 2);
        for (int i = 0; i < 3; i++)
            m_WEBHeadVFrame[i].show();
    }
    else if (width > DESKTOP_WIDTH * 0.35 && width <= DESKTOP_WIDTH * 0.75)
    {
        if (height > DESKTOP_HEIGHT * 0.75)
        {
            font.setPixelSize(height / 24 * 0.5);
            setHeaderData();
            p_webModel->setHeaderData(Ope_Time_Column, Qt::Horizontal, tr("Time"));
            heightHeader = 50;
            m_WEBLog.p_headerLabel->setFixedHeight(heightHeader / 3);
        }
        else if (height > DESKTOP_HEIGHT * 0.35 && height <= DESKTOP_HEIGHT * 0.75)
        {
            font.setPixelSize(height / 14 * 0.5);
            setHeaderData();
            p_webModel->setHeaderData(Ope_Time_Column, Qt::Horizontal, tr("Time"));
            heightHeader = 50;
            m_WEBLog.p_headerLabel->setFixedHeight(heightHeader / 2);
        }
        else
        {
            font.setPixelSize(height / 14 * 0.8);
            setHeaderNull();
            heightHeader = 25;
            m_WEBLog.p_view->verticalHeader()->setDefaultSectionSize(height / 5);
        }
    }
    else
    {
        setHeaderNull();
        heightHeader = 25;
        m_WEBLog.p_headerLabel->setFixedHeight(heightHeader);
        m_WEBLog.p_view->verticalHeader()->setDefaultSectionSize(height / 5);
        if (height > DESKTOP_HEIGHT * 0.35 && height <= DESKTOP_HEIGHT * 0.75)
            font.setPixelSize(height / 17 * 0.6);
        else
            font.setPixelSize(height / 17);
    }
    slotDateTimeFormateChanged(ID_System_Env_Date);
    m_WEBLog.p_view->horizontalHeader()->setFixedHeight(heightHeader);
    m_WEBLog.p_view->horizontalHeader()->setFont(font);
    m_WEBLog.p_view->setFont(font);
    m_WEBLog.p_headerLabel->setFont(font);
}

void WinWebLog::setHeaderData()
{
    p_webModel->setHeaderData(0, Qt::Horizontal, spaceStr + tr("Time"));
    p_webModel->setHeaderData(1, Qt::Horizontal, tr("Method") + "\n" + tr("IP Address"));
    p_webModel->setHeaderData(3, Qt::Horizontal, tr("URL"));
    p_webModel->setHeaderData(4, Qt::Horizontal, tr("Code") + "\n" + tr("Message"));
}

void WinWebLog::setHeaderNull()
{
    p_webModel->setHeaderData(0, Qt::Horizontal, "");
    p_webModel->setHeaderData(1, Qt::Horizontal, "");
    p_webModel->setHeaderData(3, Qt::Horizontal, "");
    p_webModel->setHeaderData(4, Qt::Horizontal, "");
}

void WinWebLog::paintEvent(QPaintEvent *event)
{
    p_webModel->logFileIsChanged(WEB_LOG_PATH);
    int currentrow = m_WEBLog.p_view->rowAt(m_WEBLog.p_view->y());
    setCurrentRow(currentrow + 12, p_webModel->rowCount());
    QWidget::paintEvent(event);
}

void WinWebLog::slotDateTimeFormateChanged(quint32 ID)
{
    if (ID != ID_System_Env_Date)
        return ;

    QString timeFormate = getSysTimeFormate(this->width(), this->height());
    ((LogDelegate *)m_WEBLog.p_view->itemDelegate())->setTimeFormateStr(timeFormate);
}




/*********************************email log***********************************/
WinEmailLog::WinEmailLog(QWidget * parent) : WinLog(parent)
{
    m_EmailLog.setParent(this);
    initHeadVLine();

    p_EmailModel = new ModelLog(EMail_Log);
    m_EmailLog.p_view->setModel(p_EmailModel);
    m_EmailLog.p_view->setColumnHidden(3, true);
    m_EmailLog.p_view->setColumnHidden(5, true);
    m_EmailLog.p_view->setItemDelegate(new LogDelegate(m_EmailLog.p_view, EmailLog));
    m_EmailLog.p_view->setSortingEnabled(false);
    m_EmailLog.p_view->setEditTriggers(false);
}

WinEmailLog::~WinEmailLog()
{
    delete p_EmailModel;
}

void WinEmailLog::initHeadVLine()
{
    for (int i = 0; i < 3; i++)
    {
        m_EmailHeadVFrame[i].setParent(m_EmailLog.p_view);
        m_EmailHeadVFrame[i].setFrameShape(QFrame::VLine);
        m_EmailHeadVFrame[i].setFrameShadow(QFrame::Sunken);
        m_EmailHeadVFrame[i].hide();
    }
}

void WinEmailLog::setVline()
{
    int widthLocal[3];
    widthLocal[0] = m_EmailLog.p_view->columnWidth(0);
    widthLocal[1] = widthLocal[0] + m_EmailLog.p_view->columnWidth(1);
    widthLocal[2] = widthLocal[1] + m_EmailLog.p_view->columnWidth(2);
    for (int i = 0; i < 3; i++)
        m_EmailHeadVFrame[i].setGeometry(QRect(widthLocal[i] - 6, 6, 4, m_EmailLog.p_view->horizontalHeader()->height() - 12 ));
}

void WinEmailLog::resizeEvent(QResizeEvent */*event*/)
{
    m_EmailLog.resize(this->width(), this->height());
    resetColumnWidth();
    initModelHeaderData();
    setVline();
}

void WinEmailLog::setCurrentRow(int current, int sumrow)
{
    if (current < 11)
        current = 11;
    if (sumrow < 11)
        current = sumrow;
    if (this->width() < DESKTOP_WIDTH * 0.35 || this->height() < DESKTOP_HEIGHT * 0.35)
        m_EmailLog.p_headerLabel->setText(QString("%1/%2\t%3").arg(current, 4, 10, QChar('0'))
                                               .arg(sumrow, 4, 10, QChar('0'))
                                               .arg(tr("Email log")));
    else
        m_EmailLog.p_headerLabel->setText(QString("%1/%2").arg(current, 4, 10, QChar('0')).arg(sumrow, 4, 10, QChar('0')));
}


/*
 * 当界面大小变化时，调整列宽，行高和列的显示隐藏
*/
void WinEmailLog::resetColumnWidth()
{
    int width = this->width();
    m_EmailLog.p_upBtn->hide();
    m_EmailLog.p_downBtn->hide();
    m_EmailLog.p_view->verticalHeader()->setDefaultSectionSize(this->height() / 12);
    m_EmailLog.p_view->setColumnWidth(0, width / 4);
    m_EmailLog.p_view->setColumnWidth(1, width / 6);
    m_EmailLog.p_view->setColumnWidth(2, width / 6);
    m_EmailLog.p_view->setColumnHidden(4, false);
    if (width > DESKTOP_WIDTH * 0.75)
    {
        m_EmailLog.p_upBtn->show();
        m_EmailLog.p_downBtn->show();
    }
    else if (width < DESKTOP_WIDTH * 0.35)
    {
        m_EmailLog.p_view->setColumnHidden(4, true);
    }
}


/*
 * view的表头和upBtn与p_downBtn的隐藏与显示
 */
void WinEmailLog::initModelHeaderData()
{
    int width = this->width();
    int height = this->height();
    int heightHeader;
    QFont font;

    m_EmailLog.p_headerLabel->setGeometry(0, 0, 400, 25);
    for (int i = 0; i < 3; i++)
        m_EmailHeadVFrame[i].hide();
    if (width > DESKTOP_WIDTH * 0.75)
    {
        m_EmailLog.p_headerLabel->setGeometry(100, 0, 400, 25);
        setHeaderData();
        if (height > DESKTOP_HEIGHT * 0.75)
        {
            font.setPixelSize(height / 17 * 0.4);
            heightHeader = 50;
        }
        else/* if (height > DESKTOP_HEIGHT * 0.33 && height <= DESKTOP_HEIGHT * 0.75)*/
        {
            font.setPixelSize(height / 14 * 0.5);
            heightHeader = 45;
        }
        m_EmailLog.p_headerLabel->setFixedHeight(heightHeader / 2);
        for (int i = 0; i < 3; i++)
            m_EmailHeadVFrame[i].show();
    }
    else if (width > DESKTOP_WIDTH * 0.35 && width <= DESKTOP_WIDTH * 0.75)
    {
        if (height > DESKTOP_HEIGHT * 0.75)
        {
            font.setPixelSize(height / 24 * 0.5);
            setHeaderData();
            p_EmailModel->setHeaderData(Ope_Time_Column, Qt::Horizontal, tr("Time"));
            heightHeader = 50;
            m_EmailLog.p_headerLabel->setFixedHeight(heightHeader / 3);
        }
        else if (height > DESKTOP_HEIGHT * 0.35 && height <= DESKTOP_HEIGHT * 0.75)
        {
            font.setPixelSize(height / 14 * 0.5);
            setHeaderData();
            p_EmailModel->setHeaderData(Ope_Time_Column, Qt::Horizontal, tr("Time"));
            heightHeader = 50;
            m_EmailLog.p_headerLabel->setFixedHeight(heightHeader / 2);
        }
        else
        {
            font.setPixelSize(height / 14 * 0.8);
            setHeaderNull();
            heightHeader = 25;
            m_EmailLog.p_view->verticalHeader()->setDefaultSectionSize(height / 5);
        }
    }
    else
    {
        setHeaderNull();
        heightHeader = 25;
        m_EmailLog.p_headerLabel->setFixedHeight(heightHeader);
        m_EmailLog.p_view->verticalHeader()->setDefaultSectionSize(height / 4);
        if (height > DESKTOP_HEIGHT * 0.35 && height <= DESKTOP_HEIGHT * 0.75)
            font.setPixelSize(height / 17 * 0.6);
        else
            font.setPixelSize(height / 17);
    }
    slotDateTimeFormateChanged(ID_System_Env_Date);
    m_EmailLog.p_view->horizontalHeader()->setFixedHeight(heightHeader);
    m_EmailLog.p_view->horizontalHeader()->setFont(font);
    m_EmailLog.p_view->setFont(font);
    m_EmailLog.p_headerLabel->setFont(font);
}

void WinEmailLog::setHeaderData()
{
    p_EmailModel->setHeaderData(0, Qt::Horizontal, spaceStr + tr("Time"));
    p_EmailModel->setHeaderData(1, Qt::Horizontal, tr("Type"));
    p_EmailModel->setHeaderData(2, Qt::Horizontal, tr("NO") + "\n" + tr("Code"));
    p_EmailModel->setHeaderData(4, Qt::Horizontal, tr("Recipient") + "/" + tr("Error"));
}

void WinEmailLog::setHeaderNull()
{
    p_EmailModel->setHeaderData(0, Qt::Horizontal, "");
    p_EmailModel->setHeaderData(1, Qt::Horizontal, "");
    p_EmailModel->setHeaderData(2, Qt::Horizontal, "");
    p_EmailModel->setHeaderData(3, Qt::Horizontal, "");
    p_EmailModel->setHeaderData(4, Qt::Horizontal, "");
    p_EmailModel->setHeaderData(5, Qt::Horizontal, "");
}


void WinEmailLog::paintEvent(QPaintEvent *event)
{
    p_EmailModel->logFileIsChanged(EMAIL_LOG_PATH);
    int currentrow = m_EmailLog.p_view->rowAt(m_EmailLog.p_view->y());
    setCurrentRow(currentrow + 12, p_EmailModel->rowCount());
    QWidget::paintEvent(event);
}

void WinEmailLog::slotDateTimeFormateChanged(quint32 ID)
{
    if (ID != ID_System_Env_Date)
        return ;

    QString timeFormate = getSysTimeFormate(this->width(), this->height());
    ((LogDelegate *)m_EmailLog.p_view->itemDelegate())->setTimeFormateStr(timeFormate);
}





/*********************************modbus log***********************************/
WinModbusLog::WinModbusLog(QWidget * parent) : WinLog(parent)
{
    m_modbusLog.setParent(this);
    initHeadVLine();

    p_modbusModel = new ModelLog(Modbus_Log);
    m_modbusLog.p_view->setModel(p_modbusModel);
    m_modbusLog.p_view->setItemDelegate(new LogDelegate(m_modbusLog.p_view, ModbusLog));
    m_modbusLog.p_view->setSortingEnabled(false);
    m_modbusLog.p_view->setEditTriggers(false);
}

WinModbusLog::~WinModbusLog()
{
    delete p_modbusModel;
}

void WinModbusLog::initHeadVLine()
{
    for (int i = 0; i < 3; i++)
    {
        m_modbusHeadVFrame[i].setParent(m_modbusLog.p_view);
        m_modbusHeadVFrame[i].setFrameShape(QFrame::VLine);
        m_modbusHeadVFrame[i].setFrameShadow(QFrame::Sunken);
        m_modbusHeadVFrame[i].hide();
    }
}

void WinModbusLog::setVline()
{
    int widthLocal[3];
    widthLocal[0] = m_modbusLog.p_view->columnWidth(0);
    widthLocal[1] = widthLocal[0] + m_modbusLog.p_view->columnWidth(1);
    widthLocal[2] = widthLocal[1] + m_modbusLog.p_view->columnWidth(2);

    for (int i = 0; i < 3; i++)
        m_modbusHeadVFrame[i].setGeometry(QRect(widthLocal[i] - 6, 6, 4, m_modbusLog.p_view->horizontalHeader()->height() - 12 ));
}


void WinModbusLog::resizeEvent(QResizeEvent */*event*/)
{
    m_modbusLog.resize(this->width(), this->height());
    resetColumnWidth();
    initModelHeaderData();
    setVline();
}

void WinModbusLog::setCurrentRow(int current, int sumrow)
{
    if (current < 11)
        current = 11;
    if (sumrow < 11)
        current = sumrow;
    if (this->width() < DESKTOP_WIDTH * 0.35 || this->height() < DESKTOP_HEIGHT * 0.35)
        m_modbusLog.p_headerLabel->setText(QString("%1/%2\t%3").arg(current, 4, 10, QChar('0'))
                                               .arg(sumrow, 4, 10, QChar('0'))
                                               .arg(tr("Modbus log")));
    else
        m_modbusLog.p_headerLabel->setText(QString("%1/%2").arg(current, 4, 10, QChar('0')).arg(sumrow, 4, 10, QChar('0')));
}


/*
 * 当界面大小变化时，调整列宽，行高和列的显示隐藏
*/
void WinModbusLog::resetColumnWidth()
{
    int width = this->width();
    m_modbusLog.p_upBtn->hide();
    m_modbusLog.p_downBtn->hide();

    m_modbusLog.p_view->verticalHeader()->setDefaultSectionSize(m_modbusLog.height() / 12);
    m_modbusLog.p_view->setColumnWidth(0, this->width() / 4);
    m_modbusLog.p_view->setColumnWidth(1, this->width() / 10);
    m_modbusLog.p_view->setColumnWidth(2, this->width() / 2);
    if (width > DESKTOP_WIDTH * 0.75)
    {
        m_modbusLog.p_upBtn->show();
        m_modbusLog.p_downBtn->show();
    }
}


/*
 * view的表头和upBtn与p_downBtn的隐藏与显示
 */
void WinModbusLog::initModelHeaderData()
{
    int width = this->width();
    int height = this->height();
    int heightHeader;
    QFont font;

    m_modbusLog.p_headerLabel->setGeometry(0, 0, 400, 25);
    for (int i = 0; i < 3; i++)
        m_modbusHeadVFrame[i].hide();
    if (width > DESKTOP_WIDTH * 0.75)
    {
        m_modbusLog.p_headerLabel->setGeometry(100, 0, 400, 25);
        setHeaderData();
        if (height > DESKTOP_HEIGHT * 0.75)
        {
            font.setPixelSize(height / 17 * 0.4);
            heightHeader = 50;
        }
        else/* if (height > DESKTOP_HEIGHT * 0.33 && height <= DESKTOP_HEIGHT * 0.75)*/
        {
            font.setPixelSize(height / 14 * 0.5);
            heightHeader = 45;
        }
        m_modbusLog.p_headerLabel->setFixedHeight(heightHeader / 2);
        for (int i = 0; i < 3; i++)
            m_modbusHeadVFrame[i].show();
    }
    else if (width > DESKTOP_WIDTH * 0.35 && width <= DESKTOP_WIDTH * 0.75)
    {
        if (height > DESKTOP_HEIGHT * 0.75)
        {
            font.setPixelSize(height / 24 * 0.5);
            setHeaderData();
            p_modbusModel->setHeaderData(Ope_Time_Column, Qt::Horizontal, tr("Time"));
            heightHeader = 50;
            m_modbusLog.p_headerLabel->setFixedHeight(heightHeader / 3);
        }
        else if (height > DESKTOP_HEIGHT * 0.35 && height <= DESKTOP_HEIGHT * 0.75)
        {
            font.setPixelSize(height / 14 * 0.5);
            setHeaderData();
            p_modbusModel->setHeaderData(Ope_Time_Column, Qt::Horizontal, tr("Time"));
            heightHeader = 50;
            m_modbusLog.p_headerLabel->setFixedHeight(heightHeader / 2);
        }
        else
        {
            font.setPixelSize(height / 14 * 0.8);
            setHeaderNull();
            heightHeader = 25;
            m_modbusLog.p_view->verticalHeader()->setDefaultSectionSize(height / 8);
        }
    }
    else
    {
        setHeaderNull();
        heightHeader = 25;
        m_modbusLog.p_headerLabel->setFixedHeight(heightHeader);
        m_modbusLog.p_view->verticalHeader()->setDefaultSectionSize(height / 5);
        if (height > DESKTOP_HEIGHT * 0.35 && height <= DESKTOP_HEIGHT * 0.75)
            font.setPixelSize(height / 17 * 0.6);
        else
            font.setPixelSize(height / 17);
    }
    slotDateTimeFormateChanged(ID_System_Env_Date);
    m_modbusLog.p_view->horizontalHeader()->setFixedHeight(heightHeader);
    m_modbusLog.p_view->horizontalHeader()->setFont(font);
    m_modbusLog.p_view->setFont(font);
    m_modbusLog.p_headerLabel->setFont(font);
}

void WinModbusLog::setHeaderData()
{
    p_modbusModel->setHeaderData(0, Qt::Horizontal, spaceStr + tr("Time"));
    p_modbusModel->setHeaderData(1, Qt::Horizontal, tr("Type"));
    p_modbusModel->setHeaderData(2, Qt::Horizontal, tr("Factor"));
    p_modbusModel->setHeaderData(3, Qt::Horizontal, tr("Command"));
}

void WinModbusLog::setHeaderNull()
{
    p_modbusModel->setHeaderData(0, Qt::Horizontal, "");
    p_modbusModel->setHeaderData(1, Qt::Horizontal, "");
    p_modbusModel->setHeaderData(2, Qt::Horizontal, "");
    p_modbusModel->setHeaderData(3, Qt::Horizontal, "");
}


void WinModbusLog::paintEvent(QPaintEvent *event)
{
    p_modbusModel->logFileIsChanged(MODBUS_LOG_PATH);
    int currentrow = m_modbusLog.p_view->rowAt(m_modbusLog.p_view->y());
    setCurrentRow(currentrow + 12, p_modbusModel->rowCount());
    QWidget::paintEvent(event);
}

void WinModbusLog::slotDateTimeFormateChanged(quint32 ID)
{
    if (ID != ID_System_Env_Date)
        return ;

    QString timeFormate = getSysTimeFormate(this->width(), this->height());
    ((LogDelegate *)m_modbusLog.p_view->itemDelegate())->setTimeFormateStr(timeFormate);
}





/***************************** sntp log**********************************/
WinSNTPLog::WinSNTPLog(QWidget * parent) : WinLog(parent)
{
    m_SNTPLog.setParent(this);
    initHeadVLine();

    p_sntpModel = new ModelLog(SNTP_Log);
    m_SNTPLog.p_view->setModel(p_sntpModel);
    m_SNTPLog.p_view->setItemDelegate(new LogDelegate(m_SNTPLog.p_view, SNTPLog));
    m_SNTPLog.p_view->setSortingEnabled(false);
    m_SNTPLog.p_view->setEditTriggers(false);
}

WinSNTPLog::~WinSNTPLog()
{
    delete p_sntpModel;
}

void WinSNTPLog::initHeadVLine()
{
    m_SNTPHeadVFrame.setParent(m_SNTPLog.p_view);
    m_SNTPHeadVFrame.setFrameShape(QFrame::VLine);
    m_SNTPHeadVFrame.setFrameShadow(QFrame::Sunken);
    m_SNTPHeadVFrame.hide();
}


void WinSNTPLog::setVline()
{
    int widthLocal;
    widthLocal = m_SNTPLog.p_view->columnWidth(0);
    m_SNTPHeadVFrame.setGeometry(QRect(widthLocal - 6, 6, 4, m_SNTPLog.p_view->horizontalHeader()->height() - 12 ));

}

void WinSNTPLog::resizeEvent(QResizeEvent */*event*/)
{
    m_SNTPLog.resize(this->width(), this->height());
    resetColumnWidth();
    initModelHeaderData();
    setVline();
}

void WinSNTPLog::setCurrentRow(int current, int sumrow)
{
    if (current < 11)
        current = 11;
    if (sumrow < 11)
        current = sumrow;
    if (this->width() < DESKTOP_WIDTH * 0.35 || this->height() < DESKTOP_HEIGHT * 0.35)
        m_SNTPLog.p_headerLabel->setText(QString("%1/%2\t%3").arg(current, 4, 10, QChar('0'))
                                               .arg(sumrow, 4, 10, QChar('0'))
                                               .arg(tr("SNTP log")));
    else
        m_SNTPLog.p_headerLabel->setText(QString("%1/%2").arg(current, 4, 10, QChar('0')).arg(sumrow, 4, 10, QChar('0')));
}


/*
 * 当界面大小变化时，调整列宽，行高和列的显示隐藏
*/
void WinSNTPLog::resetColumnWidth()
{
    int width = this->width();
    m_SNTPLog.p_upBtn->hide();
    m_SNTPLog.p_downBtn->hide();

    m_SNTPLog.p_view->verticalHeader()->setDefaultSectionSize(m_SNTPLog.height() / 12);
    m_SNTPLog.p_view->setColumnWidth(0, this->width() / 4);
    if (width > DESKTOP_WIDTH * 0.75)
    {
        m_SNTPLog.p_upBtn->show();
        m_SNTPLog.p_downBtn->show();
    }
}


/*
 * view的表头和upBtn与p_downBtn的隐藏与显示
 */
void WinSNTPLog::initModelHeaderData()
{
    int width = this->width();
    int height = this->height();
    int heightHeader;
    QFont font;

    m_SNTPLog.p_headerLabel->setGeometry(0, 0, 400, 25);
    m_SNTPHeadVFrame.hide();
    if (width > DESKTOP_WIDTH * 0.75)
    {
        m_SNTPLog.p_headerLabel->setGeometry(100, 0, 400, 25);
        setHeaderData();
        if (height > DESKTOP_HEIGHT * 0.75)
        {
            font.setPixelSize(height / 17 * 0.4);
            heightHeader = 50;
        }
        else/* if (height > DESKTOP_HEIGHT * 0.33 && height <= DESKTOP_HEIGHT * 0.75)*/
        {
            font.setPixelSize(height / 14 * 0.5);
            heightHeader = 45;
        }
        m_SNTPLog.p_headerLabel->setFixedHeight(heightHeader / 2);
        m_SNTPHeadVFrame.show();
    }
    else if (width > DESKTOP_WIDTH * 0.35 && width <= DESKTOP_WIDTH * 0.75)
    {
        if (height > DESKTOP_HEIGHT * 0.75)
        {
            font.setPixelSize(height / 24 * 0.5);
            setHeaderData();
            p_sntpModel->setHeaderData(Ope_Time_Column, Qt::Horizontal, tr("Time"));
            heightHeader = 50;
            m_SNTPLog.p_headerLabel->setFixedHeight(heightHeader / 3);
        }
        else if (height > DESKTOP_HEIGHT * 0.35 && height <= DESKTOP_HEIGHT * 0.75)
        {
            font.setPixelSize(height / 14 * 0.5);
            setHeaderData();
            p_sntpModel->setHeaderData(Ope_Time_Column, Qt::Horizontal, tr("Time"));
            heightHeader = 50;
            m_SNTPLog.p_headerLabel->setFixedHeight(heightHeader / 2);
        }
        else
        {
            font.setPixelSize(height / 14 * 0.8);
            setHeaderNull();
            heightHeader = 25;
            m_SNTPLog.p_view->verticalHeader()->setDefaultSectionSize(height / 8);
        }
    }
    else
    {
        setHeaderNull();
        heightHeader = 25;
        m_SNTPLog.p_headerLabel->setFixedHeight(heightHeader);
        m_SNTPLog.p_view->verticalHeader()->setDefaultSectionSize(height / 5);
        if (height > DESKTOP_HEIGHT * 0.35 && height <= DESKTOP_HEIGHT * 0.75)
            font.setPixelSize(height / 17 * 0.6);
        else
            font.setPixelSize(height / 17);
    }
    slotDateTimeFormateChanged(ID_System_Env_Date);
    m_SNTPLog.p_view->horizontalHeader()->setFixedHeight(heightHeader);
    m_SNTPLog.p_view->horizontalHeader()->setFont(font);
    m_SNTPLog.p_view->setFont(font);
    m_SNTPLog.p_headerLabel->setFont(font);
}

void WinSNTPLog::setHeaderData()
{
    p_sntpModel->setHeaderData(0, Qt::Horizontal, spaceStr + tr("Time"));
    p_sntpModel->setHeaderData(1, Qt::Horizontal, tr("Code"));
}

void WinSNTPLog::setHeaderNull()
{
    p_sntpModel->setHeaderData(0, Qt::Horizontal, "");
    p_sntpModel->setHeaderData(1, Qt::Horizontal, "");
}


void WinSNTPLog::paintEvent(QPaintEvent *event)
{
    p_sntpModel->logFileIsChanged(SNTP_LOG_PATH);
    int currentrow = m_SNTPLog.p_view->rowAt(m_SNTPLog.p_view->y());
    setCurrentRow(currentrow + 12, p_sntpModel->rowCount());
    QWidget::paintEvent(event);
}

void WinSNTPLog::slotDateTimeFormateChanged(quint32 ID)
{
    if (ID != ID_System_Env_Date)
        return ;

    QString timeFormate = getSysTimeFormate(this->width(), this->height());
    ((LogDelegate *)m_SNTPLog.p_view->itemDelegate())->setTimeFormateStr(timeFormate);
}



/*********************************DHCP log***********************************/
WinDHCPLog::WinDHCPLog(QWidget * parent) : WinLog(parent)
{
    m_DHCPLog.setParent(this);
    initHeadVLine();

    p_dhcpModel = new ModelLog(DHCP_Log);
    m_DHCPLog.p_view->setModel(p_dhcpModel);
    m_DHCPLog.p_view->setItemDelegate(new LogDelegate(m_DHCPLog.p_view, DHCPLog));
    m_DHCPLog.p_view->setSortingEnabled(false);
    m_DHCPLog.p_view->setEditTriggers(false);
}

WinDHCPLog::~WinDHCPLog()
{
    delete p_dhcpModel;
}

void WinDHCPLog::initHeadVLine()
{
    for (int i = 0; i < 2; i++)
    {
        m_DHCPHeadVFrame[i].setParent(m_DHCPLog.p_view);
        m_DHCPHeadVFrame[i].setFrameShape(QFrame::VLine);
        m_DHCPHeadVFrame[i].setFrameShadow(QFrame::Sunken);
        m_DHCPHeadVFrame[i].hide();
    }
}


void WinDHCPLog::setVline()
{
    int widthLocal[2];
    widthLocal[0] = m_DHCPLog.p_view->columnWidth(0);
    widthLocal[1] = widthLocal[0] + m_DHCPLog.p_view->columnWidth(1);

    for (int i = 0; i < 2; i++)
        m_DHCPHeadVFrame[i].setGeometry(QRect(widthLocal[i] - 6, 6, 4, m_DHCPLog.p_view->horizontalHeader()->height() - 12 ));
}

void WinDHCPLog::resizeEvent(QResizeEvent */*event*/)
{
    m_DHCPLog.resize(this->width(), this->height());
    resetColumnWidth();
    initModelHeaderData();
    setVline();
}

void WinDHCPLog::setCurrentRow(int current, int sumrow)
{
    if (current < 11)
        current = 11;
    if (sumrow < 11)
        current = sumrow;
    if (this->width() < DESKTOP_WIDTH * 0.35 || this->height() < DESKTOP_HEIGHT * 0.35)
        m_DHCPLog.p_headerLabel->setText(QString("%1/%2\t%3").arg(current, 4, 10, QChar('0'))
                                               .arg(sumrow, 4, 10, QChar('0'))
                                               .arg(tr("DHCP log")));
    else
        m_DHCPLog.p_headerLabel->setText(QString("%1/%2").arg(current, 4, 10, QChar('0')).arg(sumrow, 4, 10, QChar('0')));
}


/*
 * 当界面大小变化时，调整列宽，行高和列的显示隐藏
*/
void WinDHCPLog::resetColumnWidth()
{
    int width = this->width();
    m_DHCPLog.p_upBtn->hide();
    m_DHCPLog.p_downBtn->hide();

    m_DHCPLog.p_view->verticalHeader()->setDefaultSectionSize(m_DHCPLog.height() / 12);
    m_DHCPLog.p_view->setColumnWidth(0, this->width() / 4);
    m_DHCPLog.p_view->setColumnWidth(1, this->width() / 4);
    if (width > DESKTOP_WIDTH * 0.75)
    {
        m_DHCPLog.p_upBtn->show();
        m_DHCPLog.p_downBtn->show();
    }
}


/*
 * view的表头和upBtn与p_downBtn的隐藏与显示
 */
void WinDHCPLog::initModelHeaderData()
{
    int width = this->width();
    int height = this->height();
    int heightHeader;
    QFont font;

    m_DHCPLog.p_headerLabel->setGeometry(0, 0, 400, 25);
    m_DHCPHeadVFrame[0].hide();
    m_DHCPHeadVFrame[1].hide();
    if (width > DESKTOP_WIDTH * 0.75)
    {
        m_DHCPLog.p_headerLabel->setGeometry(100, 0, 400, 25);
        setHeaderData();
        if (height > DESKTOP_HEIGHT * 0.75)
        {
            font.setPixelSize(height / 17 * 0.4);
            heightHeader = 50;
        }
        else/* if (height > DESKTOP_HEIGHT * 0.33 && height <= DESKTOP_HEIGHT * 0.75)*/
        {
            font.setPixelSize(height / 14 * 0.5);
            heightHeader = 45;
        }
        m_DHCPLog.p_headerLabel->setFixedHeight(heightHeader / 2);
        m_DHCPHeadVFrame[0].show();
        m_DHCPHeadVFrame[1].show();
    }
    else if (width > DESKTOP_WIDTH * 0.35 && width <= DESKTOP_WIDTH * 0.75)
    {
        if (height > DESKTOP_HEIGHT * 0.75)
        {
            font.setPixelSize(height / 24 * 0.5);
            setHeaderData();
            p_dhcpModel->setHeaderData(Ope_Time_Column, Qt::Horizontal, tr("Time"));
            heightHeader = 50;
            m_DHCPLog.p_headerLabel->setFixedHeight(heightHeader / 3);
        }
        else if (height > DESKTOP_HEIGHT * 0.35 && height <= DESKTOP_HEIGHT * 0.75)
        {
            font.setPixelSize(height / 14 * 0.5);
            setHeaderData();
            p_dhcpModel->setHeaderData(Ope_Time_Column, Qt::Horizontal, tr("Time"));
            heightHeader = 50;
            m_DHCPLog.p_headerLabel->setFixedHeight(heightHeader / 2);
        }
        else
        {
            font.setPixelSize(height / 14 * 0.8);
            setHeaderNull();
            heightHeader = 25;
            m_DHCPLog.p_view->verticalHeader()->setDefaultSectionSize(height / 8);
        }
    }
    else
    {
        setHeaderNull();
        heightHeader = 25;
        m_DHCPLog.p_headerLabel->setFixedHeight(heightHeader);
        m_DHCPLog.p_view->verticalHeader()->setDefaultSectionSize(height / 5);
        if (height > DESKTOP_HEIGHT * 0.35 && height <= DESKTOP_HEIGHT * 0.75)
            font.setPixelSize(height / 17 * 0.6);
        else
            font.setPixelSize(height / 17);
    }
    slotDateTimeFormateChanged(ID_System_Env_Date);

    m_DHCPLog.p_view->horizontalHeader()->setFixedHeight(heightHeader);
    m_DHCPLog.p_view->horizontalHeader()->setFont(font);
    m_DHCPLog.p_view->setFont(font);
    m_DHCPLog.p_headerLabel->setFont(font);
}

void WinDHCPLog::setHeaderData()
{
    p_dhcpModel->setHeaderData(0, Qt::Horizontal, spaceStr + tr("Time"));
    p_dhcpModel->setHeaderData(1, Qt::Horizontal, tr("Kind"));
    p_dhcpModel->setHeaderData(2, Qt::Horizontal, tr("Message"));
}

void WinDHCPLog::setHeaderNull()
{
    p_dhcpModel->setHeaderData(0, Qt::Horizontal, "");
    p_dhcpModel->setHeaderData(1, Qt::Horizontal, "");
    p_dhcpModel->setHeaderData(2, Qt::Horizontal, "");
}


void WinDHCPLog::paintEvent(QPaintEvent *event)
{
    p_dhcpModel->logFileIsChanged(DHCP_LOG_PATH);
    int currentrow = m_DHCPLog.p_view->rowAt(m_DHCPLog.p_view->y());
    setCurrentRow(currentrow + 12, p_dhcpModel->rowCount());
    QWidget::paintEvent(event);
}

void WinDHCPLog::slotDateTimeFormateChanged(quint32 ID)
{
    if (ID != ID_System_Env_Date)
        return ;

    QString timeFormate = getSysTimeFormate(this->width(), this->height());
    ((LogDelegate *)m_DHCPLog.p_view->itemDelegate())->setTimeFormateStr(timeFormate);
}






/**********************************************************************************
 *
 * 代理类，实现不同日志的view显示样式不同
 *
 * ********************************************************************************/

/*
** 重写代理的构造函数
*/
LogDelegate::LogDelegate(QTableView *tableView, LogType type, QObject * parent) : QItemDelegate(parent), m_timeFormat(20)
{
    int gridHint = tableView->style()->styleHint(QStyle::SH_Table_GridLineColor, new QStyleOptionViewItemV4());
    QColor gridColor = static_cast<QRgb>(gridHint);
    pen = QPen(gridColor,0,tableView->gridStyle());
    view = tableView;
    logType = type;
    if (logType == ModbusLog)
    {
        m_imgConnect.load(":/summary/Image/summary/log_device.png");
        m_imgClose.load(":/summary/Image/summary/log_negative.png");
        m_imgBusy.load(":/summary/Image/summary/log_busy.png");
        m_imgAck.load(":/summary/Image/summary/log_ack.png");
    }
}


/*
** 代理模型的重画view界面的函数
*/
void LogDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    switch (logType) {
    case ModbusLog:
        drawModbus(painter, option, index);
        break;
    case EmailLog:
        drawEmail(painter, option, index);
        break;
    case WEBLog:
        drawWEB(painter, option, index);
        break;

    case ErrorLog:
    case OperationLog:
    case CommunicationLog:
    case FTPLog:
    case DHCPLog:
    case SNTPLog:
        drawLog(painter, option, index);
        break;

    default:
        QItemDelegate::paint(painter,option,index);
        break;
    }
    QPen oldPen = painter->pen();
    painter->setPen(pen);
    if (index.row() == 0)
        painter->drawLine(option.rect.topLeft(),option.rect.topRight());
    painter->drawLine(option.rect.bottomLeft(),option.rect.bottomRight());
    painter->setPen(oldPen);
}


void LogDelegate::drawLog(QPainter *painter, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const
{
    QString str = index.data().toString();
    if (index.column() == 0)
    {
        QString timeValue = index.data().toString();
        timeValue = QDateTime::fromMSecsSinceEpoch(timeValue.toLongLong()).toString(m_timeFormatStr);
        painter->drawText(option.rect, Qt::TextWordWrap | Qt::AlignLeft | Qt::AlignVCenter, timeValue);
    }
    else
        painter->drawText(option.rect, Qt::AlignLeft | Qt::AlignVCenter | Qt::ElideRight, str);
}

/**
 * @function : 重画modbus的第2列和第3列
 */
void LogDelegate::drawModbus(QPainter *painter, const QStyleOptionViewItem &option,
                             const QModelIndex &index) const
{
    QString str = index.data().toString();
    int x = option.rect.x();
    int y = option.rect.y();
    int width = option.rect.width();
    int height = option.rect.height();
    if (index.column() == 0)
    {
        QString timeValue = index.data().toString();
        timeValue = QDateTime::fromMSecsSinceEpoch(timeValue.toLongLong()).toString(m_timeFormatStr);
        painter->drawText(option.rect, Qt::TextWordWrap | Qt::AlignLeft | Qt::AlignVCenter, timeValue);
    }
    else if (index.column() == 2)
    {
        int type = str.toInt();
        QImage img;
        QString statusStr;
        getImageAndStatus(type, statusStr);
        if (type == 0)
            img = m_imgConnect;
        else if (type >= 0 && type < 20)
            img = m_imgBusy;
        else if (type >= 20 && type < 30)
            img = m_imgClose;
        else if (type >= 30 && type <= 35)
            img = m_imgAck;

        if (img.width() != 0)
        {
            img = img.scaled(height / 2, height / 2, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);  //
            painter->drawImage(x, y + (height - img.height()) / 2 , img, 0, 0, height, height);
        }
        painter->drawText(x + height / 2, y, width - height / 2, height, Qt::AlignVCenter, statusStr);
    }
    else
    {
        painter->drawText(x, y, width, height, Qt::AlignVCenter, str);
    }
}


/**
 * @function : 重画email的第2列
 */
void LogDelegate::drawEmail(QPainter *painter, const QStyleOptionViewItem &option,
                            const QModelIndex &index) const
{
    QString str = index.data().toString();
    int x = option.rect.x();
    int y = option.rect.y();
    int width = option.rect.width();
    int height = option.rect.height();
    if (index.column() == 0)
    {
        QString timeValue = index.data().toString();
        timeValue = QDateTime::fromMSecsSinceEpoch(timeValue.toLongLong()).toString(m_timeFormatStr);
        painter->drawText(option.rect, Qt::TextWordWrap | Qt::AlignLeft | Qt::AlignVCenter, timeValue);
    }
    else if (index.column() == 4)
    {
        str.append(" ").append(index.model()->index(index.row(), index.column() + 1).data().toString());
        painter->drawText(x, y, width, height, Qt::AlignVCenter, str);
    }
    else if (index.column() == 2)
    {
        QString str2 = index.model()->data(index.model()->index(index.row(), index.column() + 1)).toString();
        painter->drawText(x, y, width, height / 2 , Qt::AlignVCenter, str);
        painter->drawText(x, y + height / 2, width, height / 2 , Qt::AlignVCenter, str2);
    }
    else
    {
        painter->drawText(x, y, width, height, Qt::AlignVCenter, str);
    }
}


/**
 * @function : 重画web的第一列和第三列
 */
void LogDelegate::drawWEB(QPainter *painter, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const
{
    QString str = index.data().toString();
    int x = option.rect.x();
    int y = option.rect.y();
    int width = option.rect.width();
    int height = option.rect.height();

    if (index.column() == 0)
    {
        QString timeValue = index.data().toString();
        timeValue = QDateTime::fromMSecsSinceEpoch(timeValue.toLongLong()).toString(m_timeFormatStr);
        painter->drawText(option.rect, Qt::TextWordWrap | Qt::AlignLeft | Qt::AlignVCenter, timeValue);
    }
    else if (index.column() == 1 || index.column() == 4)
    {
        QString str2 = index.model()->data(index.model()->index(index.row(), index.column() + 1)).toString();
        painter->drawText(x, y, width, height / 2 , Qt::AlignVCenter, str);
        painter->drawText(x, y + height / 2, width, height / 2 , Qt::AlignVCenter, str2);
    }
    else
    {
        painter->drawText(x, y, width, height , Qt::AlignVCenter, str);
    }
}



//先自己定义，等配置信息定义后，要改成与配置相对应
enum MosbusType
{
    ModbusConnect = 0,
    ModbusClose,
    ModbusBusy,
    ModbusAck
};

/*
 * modbus客户端日志界面图片
*/
QImage LogDelegate::getImage(int imgType) const
{
    switch (imgType) {
    case ModbusConnect:
        return m_imgConnect;
        break;
    case ModbusClose:
        return m_imgClose;
        break;
    case ModbusBusy:
        return m_imgBusy;
        break;
    case ModbusAck:
        return m_imgAck;
        break;
    default:
        return QImage();
        break;
    }
}


void LogDelegate::setTimeFormateStr(QString timeFormateStr)
{
    m_timeFormatStr = timeFormateStr;
}

bool LogDelegate::helpEvent(QHelpEvent */*event*/, QAbstractItemView */*view*/, const QStyleOptionViewItem &/*option*/, const QModelIndex &/*index*/)
{
    return false;
}



