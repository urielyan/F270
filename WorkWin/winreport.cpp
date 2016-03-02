#include "winreport.h"
#include <QDebug>
#include "Threads/threaddatarecord.h"
#include <QHeaderView>
#include "./WorkWin/winsummarydialog.h"
#include "./Common/authorization.h"
#include "./WorkWin/wininfolistdialog.h"
#define REPORT_MAIN_TABLE   "table_Main_Report" //报表数据主表

extern QString spaceStr;
extern QString getTimeFormate();

WinReport::WinReport(QWidget *parent, bool isSingle) :
    QWidget(parent), m_isSingle(isSingle), m_screen(0, true, false), m_index(0)
{
    initReportUI();

    m_reportSummary[0].p_view->setModel(&m_reportModel[0]);
    m_reportSummary[1].p_view->setModel(&m_reportModel[1]);
    m_index = m_reportModel[0].currentIndex;

    setColumnHiden();
    signalAndSlotConnect();
    setViewStyle();
}

WinReport::~WinReport()
{

}

/*
 * 初始化报表数据的界面
*/
void WinReport::initReportUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    m_topFrame.setParent(this);
    m_topFrame.setObjectName(QStringLiteral("m_topFrame"));
    m_topFrame.setMinimumSize(QSize(0, 50));
    m_topFrame.setMaximumHeight(55);
    m_topFrame.setFrameShape(QFrame::StyledPanel);
    m_topFrame.setFrameShadow(QFrame::Raised);
    m_topFrame.setLineWidth(0);
    m_topFrame.setStyleSheet("background:#ECECEC");

    QHBoxLayout *toplayout = new QHBoxLayout(&m_topFrame);
    toplayout->setSpacing(0);
    toplayout->setContentsMargins(0, 0, 0, 0);

    m_leftBtn_2.setParent(&m_topFrame);
    m_leftBtn_2.setObjectName("m_leftBtn_2");
    m_leftBtn_2.setIcon(QIcon(":/summary/Image/summary/left_2.png"));
    m_leftBtn_2.setIconSize(QSize(45, 28));
    m_leftBtn_2.setFocusPolicy(Qt::NoFocus);
    toplayout->addWidget(&m_leftBtn_2);

    m_leftBtn.setParent(&m_topFrame);
    m_leftBtn.setObjectName("m_leftBtn");
    m_leftBtn.setIcon(QIcon(":/summary/Image/summary/left.png"));
    m_leftBtn.setIconSize(QSize(45, 28));
    m_leftBtn.setFocusPolicy(Qt::NoFocus);
    toplayout->addWidget(&m_leftBtn);

    toplayout->addSpacerItem(new QSpacerItem(20,59,QSizePolicy::Fixed,QSizePolicy::Expanding));
    m_typeLabel.setParent(&m_topFrame);
    m_typeLabel.setObjectName("m_typeLabel");
    toplayout->addWidget(&m_typeLabel);

    m_timeLabel.setParent(&m_topFrame);
    m_timeLabel.setObjectName("m_timeLabel");
    toplayout->addWidget(&m_timeLabel);
    toplayout->addSpacerItem(new QSpacerItem(20,59,QSizePolicy::Fixed,QSizePolicy::Expanding));

    m_rightBtn.setParent(&m_topFrame);
    m_rightBtn.setObjectName("m_rightBtn");
    m_rightBtn.setIcon(QIcon(":/summary/Image/summary/right.png"));
    m_rightBtn.setIconSize(QSize(45, 28));
    m_rightBtn.setFocusPolicy(Qt::NoFocus);
    toplayout->addWidget(&m_rightBtn);

    m_rightBtn_2.setParent(&m_topFrame);
    m_rightBtn_2.setObjectName("m_rightBtn_2");
    m_rightBtn_2.setIcon(QIcon(":/summary/Image/summary/right_2.png"));
    m_rightBtn_2.setIconSize(QSize(45, 28));
    m_rightBtn_2.setFocusPolicy(Qt::NoFocus);
    toplayout->addWidget(&m_rightBtn_2);

    mainLayout->addWidget(&m_topFrame);


    if (!m_isSingle)
    {
        mainLayout->addWidget(&m_screen);
        m_screen.addWidgetItem(0, &m_reportSummary[0]);
        if (m_reportModel[0].m_maxId > 1)
            m_screen.addWidgetItem(1, &m_reportSummary[1]);
        m_screen.setCurrentWidget(0);
    }
    else
        mainLayout->addWidget(&m_reportSummary[0]);

    this->setStyleSheet("QLabel#m_timeLabel{color:rgb(100, 100, 100);font-size:18px;}\
                        QPushButton{border: none;max-width : 50px;}");

    for(int i = 0; i < 4; i++)
    {
        m_frameLine[i].setParent(m_reportSummary[0].p_view);
        m_frameLine[i].setFrameShape(QFrame::VLine);
        m_frameLine[i].setFrameShadow(QFrame::Sunken);
        m_frameLine[i].hide();
    }
}

/*
 * 设置顶部显示报表的类型，开始时间和结束时间
*/
void WinReport::setTopFrameValue(ModelReport *currentModel)
{
    ModelReport * reportModel;
    if (!m_isSingle)
    {
        if (currentModel == NULL)
            reportModel = &m_reportModel[m_screen.getCurrentWidget()];
        else
            reportModel = currentModel;
    }
    else
        reportModel = &m_reportModel[0];
    QSqlQuery query(QString("select ReportInfo, StartTime, EndTime from %1 where ID = %2")\
                                .arg(REPORT_MAIN_TABLE).arg(reportModel->currentIndex),\
                                *ThreadDataRecord::instance()->getDatabaseConnect());
    query.exec();
    ThreadDataRecord::instance()->putDatabaseConnect();
    SQL_REPORT_INFO reportinfo;
    QString startTime = NULL, endTime = NULL;
    QString timeFormate = getTimeFormate();
    if (query.next())
    {
        memcpy(&reportinfo, query.value(0).toByteArray(), sizeof(SQL_REPORT_INFO));
        startTime = QDateTime::fromMSecsSinceEpoch(query.value(1).toLongLong()).toString(timeFormate);
        endTime = QDateTime::fromMSecsSinceEpoch(query.value(2).toLongLong()).toString(timeFormate);
    }
    m_typeLabel.setText(QString(" %1/%2 \n %3: %4")\
                         .arg(m_index, 4, 10, QChar('0'))\
                         .arg(reportModel->m_maxId, 4, 10, QChar('0'))\
                         .arg(tr("Type")).arg(getReoprtType((int)reportinfo.reportInfo.reportType)));
    m_timeLabel.setText(QString("%1 :\t%2\n%3 :\t%4")\
                         .arg(tr("Start time")).arg(startTime)\
                         .arg(tr("Time up")).arg(endTime));
}

/*
 * 信号和槽的连接
 */
void WinReport::signalAndSlotConnect()
{
    connect(&m_leftBtn, SIGNAL(clicked()), this, SLOT(slotDisplayPreviousReport()));
    connect(&m_rightBtn, SIGNAL(clicked()), this, SLOT(slotDisplayNextReport()));
    connect(&m_leftBtn_2, SIGNAL(clicked()), this, SLOT(slotDisplayPreviousTenReport()));
    connect(&m_rightBtn_2, SIGNAL(clicked()), this, SLOT(slotDisplayNextTenReport()));

    connect(&m_screen, SIGNAL(sigMoveToPre()), this, SLOT(slotMoveToPre()));
    connect(&m_screen, SIGNAL(sigMoveToNext()), this, SLOT(slotMoveToNext()));
}



/*
 * 设置当前行与总行数的信息
*/
void WinReport::setCurrentRow(int current, int sumrow)
{
    if (current < 11)
        current = 11;
    if (sumrow < 11)
        current = sumrow;

    WinReportSummary *reportSummary;
    if (m_isSingle)
            reportSummary = &m_reportSummary[0];
    else
        reportSummary = (WinReportSummary *)m_screen.getGroupWidget(m_screen.getCurrentWidget());

    if (this->width() < DESKTOP_WIDTH * 0.35 || this->height() < DESKTOP_HEIGHT * 0.35)
        reportSummary->p_headerLabel->setText(QString("%1/%2\t%3").arg(current, 4, 10, QChar('0'))
                                               .arg(sumrow, 4, 10, QChar('0'))
                                               .arg(tr("Report summary")));
    else
        reportSummary->p_headerLabel->setText(QString("%1/%2").arg(current, 4, 10, QChar('0')).arg(sumrow, 4, 10, QChar('0')));
}

void WinReport::initHeaderData()
{
    for (int i = 0; i < 2; i++){
        m_reportModel[i].setHeaderData(Channel_Column, Qt::Horizontal, spaceStr + tr("Channel"));
        m_reportModel[i].setHeaderData(Unit_Column, Qt::Horizontal, tr("Unit"));
        m_reportModel[i].setHeaderData(Status_Column, Qt::Horizontal, tr("Status"));
    }
}

void WinReport::showHeaaderVLine()
{
    WinReportSummary *reportSummary = &m_reportSummary[0]/*(WinReportSummary *)m_screen.getGroupWidget(m_screen.getCurrentWidget())*/;
    for (int i = 0; i < 4; i++)
        m_frameLine[i].show();
    if (reportSummary->p_view->columnWidth(Ave_Column) == 0 && reportSummary->p_view->columnWidth(Min_Column) == 0)
        m_frameLine[2].hide();
    if (reportSummary->p_view->columnWidth(Sum_Column) == 0)
        m_frameLine[3].hide();
}

/*
 * 显示前一条报表信息
 */
void WinReport::slotDisplayPreviousReport()
{
    ModelReport *reportModel;
    if (m_isSingle)
        reportModel = &m_reportModel[0];
    else
        reportModel = &m_reportModel[m_screen.getCurrentWidget()];
    if (reportModel->m_maxId == 0) return ;
    if (reportModel->currentIndex - 1 == 0)
        reportModel->init(reportModel->m_maxId);
    else
        reportModel->init(reportModel->currentIndex - 1);

    m_index = reportModel->currentIndex;

    setTopFrameValue();
    setColumnHiden();
//    resizeEvent(new QResizeEvent(this->size(), this->size()));
    resetColumnWidth();
    initModelHeaderData();
    setVline();
}

/*
 * 显示后一条报表信息
 */
void WinReport::slotDisplayNextReport()
{
    ModelReport *reportModel;
    if (m_isSingle)
        reportModel = &m_reportModel[0];
    else
        reportModel = &m_reportModel[m_screen.getCurrentWidget()];
    if (reportModel->m_maxId == 0) return ;
    if (reportModel->currentIndex + 1 > reportModel->m_maxId)
        reportModel->init(1);
    else
        reportModel->init(reportModel->currentIndex + 1);

    m_index = reportModel->currentIndex;

    setTopFrameValue();
    setColumnHiden();
//    resizeEvent(new QResizeEvent(this->size(), this->size()));
    resetColumnWidth();
    initModelHeaderData();
    setVline();
}

/*
 * 显示当前报表的前10个报表的信息
 */
void WinReport::slotDisplayPreviousTenReport()
{
    ModelReport *reportModel;
    if (m_isSingle)
        reportModel = &m_reportModel[0];
    else
        reportModel = &m_reportModel[m_screen.getCurrentWidget()];
    if (reportModel->m_maxId <= 10)
        return ;
    if (reportModel->currentIndex + 10 > reportModel->m_maxId)
        reportModel->init(reportModel->currentIndex + 10 - reportModel->m_maxId);
    else
        reportModel->init(reportModel->currentIndex + 10);

    m_index = reportModel->currentIndex;

    setTopFrameValue();
    setColumnHiden();
//    resizeEvent(new QResizeEvent(this->size(), this->size()));
    resetColumnWidth();
    initModelHeaderData();
    setVline();
}

/*
 * 显示当前报表的后10个报表的信息
 */
void WinReport::slotDisplayNextTenReport()
{
    ModelReport *reportModel;
    if (m_isSingle)
        reportModel = &m_reportModel[0];
    else
        reportModel = &m_reportModel[m_screen.getCurrentWidget()];
    if (reportModel->m_maxId <= 10)
        return ;
    if (reportModel->currentIndex - 10 <= 0)
        reportModel->init(reportModel->m_maxId - reportModel->currentIndex);
    else
        reportModel->init(reportModel->currentIndex - 10);

    m_index = reportModel->currentIndex;

    setTopFrameValue();
    setColumnHiden();
//    resizeEvent(new QResizeEvent(this->size(), this->size()));
    resetColumnWidth();
    initModelHeaderData();
    setVline();
}

/*
 * 设置某列的隐藏，用在切换左右显示报表时
*/
void WinReport::setColumnHiden(ModelReport *currentModel, WinReportSummary *currentSummary)
{
    ModelReport *reportModel;
    WinReportSummary *reportSummary;

    if (m_isSingle)
    {
        reportModel = &m_reportModel[0];
        reportSummary = &m_reportSummary[0];
    }
    else
    {
        if (currentModel == NULL)
            reportModel = &m_reportModel[m_screen.getCurrentWidget()];
        else
            reportModel = currentModel;
        if (currentSummary == NULL)
            reportSummary = (WinReportSummary *)m_screen.getGroupWidget(m_screen.getCurrentWidget());
        else
            reportSummary = currentSummary;
    }

    if (reportModel->index(0, Ave_Column).data().toString() == NULL \
            && reportModel->index(0, Max_Column).data().toString() == NULL)
        reportSummary->p_view->setColumnHidden(Ave_Column, true);
    else
        reportSummary->p_view->setColumnHidden(Ave_Column, false);

    if (reportModel->index(0, Min_Column).data().toString() == NULL \
            && reportModel->index(0, Inst_Column).data().toString() == NULL)
        reportSummary->p_view->setColumnHidden(Min_Column, true);
    else
        reportSummary->p_view->setColumnHidden(Min_Column, false);

    if (reportModel->index(0, Sum_Column).data().toString() == NULL)
        reportSummary->p_view->setColumnHidden(Sum_Column, true);
    else
        reportSummary->p_view->setColumnHidden(Sum_Column, false);

}


/*
 * 重新绘制界面的大小 和定义view的属性
 */
void WinReport::resizeEvent(QResizeEvent *event)
{
//    WinReportSummary *reportSummary = (WinReportSummary *)m_screen.getGroupWidget(m_screen.getCurrentWidget());
//    reportSummary->resize(this->width(), this->height());

    resetColumnWidth();
    initModelHeaderData();
    setVline();
    QWidget::resizeEvent(event);
}

/**
 * @brief line
 * @每个表头旁边的竖线
 */
void WinReport::setVline()
{
    WinReportSummary *reportSummary;
    if (m_isSingle)
        reportSummary = &m_reportSummary[0];
    else
        reportSummary = (WinReportSummary *)m_screen.getGroupWidget(m_screen.getCurrentWidget());
    int widthLocal[4];
    widthLocal[0] = reportSummary->p_view->columnWidth(Channel_Column);
    widthLocal[1] = widthLocal[0] + reportSummary->p_view->columnWidth(Unit_Column);
    widthLocal[2] = widthLocal[1] + reportSummary->p_view->columnWidth(Status_Column);
    widthLocal[3] = widthLocal[2] + reportSummary->p_view->columnWidth(Ave_Column)\
            + reportSummary->p_view->columnWidth(Min_Column);

    for(int i = 0; i < 4; i++)
    {
        m_frameLine[i].setParent(reportSummary);
        m_frameLine[i].setGeometry(QRect(widthLocal[i] - 10, 6 , 6, reportSummary->p_view->horizontalHeader()->height() - 12));
    }
}

/**
 * @brief getReoprtType
 * @param typeStr
 * @return 报表的类型
 */
QString WinReport::getReoprtType(int type)
{
    switch (type) {
    case Report_Type_Hourly_Daily:
        return tr("Hourly + Daily");
        break;
    case Report_Type_Daily_Weekly:
        return tr("Daily + Weekly");
        break;
    case Report_Type_Daily_Monthly:
        return tr("Daily + Monthly");
        break;
    case Report_Type_Batch:
        return tr("Batch");
        break;
    case Report_Type_Daily_Custom:
        return tr("Custom");
        break;
    default:
        break;
    }
    return NULL;
}

/*
** 设置view样式
 */
void WinReport::setViewStyle()
{
    for (int i = 0; i < 2; i++)
    {
        m_reportSummary[i].p_view->setItemDelegate(new ReportDelegate(m_reportSummary[i].p_view));
        m_reportSummary[i].p_view->setColumnHidden(4, true);
        m_reportSummary[i].p_view->setColumnHidden(6, true);
        m_reportSummary[i].p_view->setSortingEnabled(false);
        m_reportSummary[i].p_view->setColumnHidden(8, true);
    }

    setTopFrameValue();
//    setVline();
}

/*
 * 当界面大小变化时，调整列宽，行高和列的显示隐藏
*/
void WinReport::resetColumnWidth()
{
    int width = this->width();
    m_topFrame.hide();

    for (int i = 0; i < 2; i++)
    {
        m_reportSummary[i].p_upBtn->hide();
        m_reportSummary[i].p_downBtn->hide();
        m_reportSummary[i].p_view->verticalHeader()->setDefaultSectionSize(this->height() / 12);
        m_reportSummary[i].p_view->setColumnWidth(Channel_Column, width / 6);
        m_reportSummary[i].p_view->setColumnWidth(Unit_Column, width / 8);
        m_reportSummary[i].p_view->setColumnWidth(Status_Column, width / 8);
        m_reportSummary[i].p_view->setColumnWidth(Ave_Column, width / 6);
        m_reportSummary[i].p_view->setColumnWidth(Min_Column, width / 6);
        m_reportSummary[i].p_view->setColumnHidden(Unit_Column, false);
        m_reportSummary[i].p_view->setColumnHidden(Status_Column, false);

        if (width > DESKTOP_WIDTH * 0.75)
        {
            m_reportSummary[i].p_view->setColumnWidth(Channel_Column, width / 4);
            m_topFrame.show();
            m_reportSummary[i].p_upBtn->show();
            m_reportSummary[i].p_downBtn->show();
        }
        else if (width < DESKTOP_WIDTH * 0.35)
        {
            m_reportSummary[i].p_view->setColumnHidden(Unit_Column, true);
            m_reportSummary[i].p_view->setColumnHidden(Status_Column, true);
        }
    }
}

void WinReport::slotMoveToNext()
{
    int index = m_screen.getCurrentWidget();
    ModelReport *reportModel;
    WinReportSummary *reportSummary;
    if (m_isSingle)
    {
        reportModel = &m_reportModel[0];
        reportSummary = &m_reportSummary[0];
    }
    else
    {
        if (index == 0)
        {
            reportModel = &m_reportModel[1];
            reportSummary = &m_reportSummary[1];
        }
        else
        {
            reportModel = &m_reportModel[0];
            reportSummary = &m_reportSummary[0];
        }
    }
    if (m_index == reportModel->getMaxId())
        m_index = 0;
    reportModel->init(m_index + 1);
    m_index = reportModel->currentIndex;

    setTopFrameValue(reportModel);
    setColumnHiden(reportModel, reportSummary);

    resetColumnWidth();
    initModelHeaderData(reportModel, reportSummary);
}

void WinReport::slotMoveToPre()
{
    int index = m_screen.getCurrentWidget();
    ModelReport *reportModel;
    WinReportSummary *reportSummary;
    if (m_isSingle)
    {
        reportModel = &m_reportModel[0];
        reportSummary = &m_reportSummary[0];
    }
    else
    {
        if (index == 0)
        {
            reportModel = &m_reportModel[1];
            reportSummary = &m_reportSummary[1];
        }
        else
        {
            reportModel = &m_reportModel[0];
            reportSummary = &m_reportSummary[0];
        }
    }
    if (m_index == 1)
        m_index = reportModel->getMaxId() + 1;
    reportModel->init(m_index - 1);
    m_index = reportModel->currentIndex;

    setTopFrameValue(reportModel);
    setColumnHiden(reportModel, reportSummary);

    resetColumnWidth();
    initModelHeaderData(reportModel, reportSummary);
}




/*
 * view的表头和upBtn与p_downBtn的隐藏与显示
 */
void WinReport::initModelHeaderData(ModelReport *currentModel, WinReportSummary *currentSummary)
{
    ModelReport *reportModel;
    WinReportSummary *reportSummary;
    if (m_isSingle)
    {
        reportModel = &m_reportModel[0];
        reportSummary = &m_reportSummary[0];
    }
    else
    {
        if (currentModel == NULL)
            reportModel = &m_reportModel[m_screen.getCurrentWidget()];
        else
            reportModel = currentModel;
        if (currentSummary == NULL)
            reportSummary = (WinReportSummary *)m_screen.getGroupWidget(m_screen.getCurrentWidget());
        else
            reportSummary = currentSummary;
    }

    int width = this->width();
    int height = this->height();
    int heightHeader;
    QFont font;

    reportSummary->p_headerLabel->setGeometry(0, 0, 400, 20);
    for (int i = 0; i < 4; i++)
        m_frameLine[i].hide();
    if (width > DESKTOP_WIDTH * 0.75)
    {
        reportSummary->p_headerLabel->setGeometry(100, 0, 400, 20);
        setHeaderData(currentModel, currentSummary);
        showHeaaderVLine();
        if (height > DESKTOP_HEIGHT * 0.75)
        {
            font.setPixelSize(height / 14 * 0.4);
            heightHeader = 50;
        }
        else
        {
            font.setPixelSize(height / 11 * 0.5);
            heightHeader = 45;
        }
    }
    else if (width > DESKTOP_WIDTH * 0.35 && width <= DESKTOP_WIDTH * 0.75)
    {
        reportSummary->p_headerLabel->setGeometry(width / 13, 0, 400, 20);
        if (height > DESKTOP_HEIGHT * 0.75)
        {
            font.setPixelSize(height / 21 * 0.5);
            setHeaderData(currentModel, currentSummary);
            reportModel->setHeaderData(Channel_Column, Qt::Horizontal, tr("Channel"));
            heightHeader = 50;
        }
        else if (height > DESKTOP_HEIGHT * 0.35 && height <= DESKTOP_HEIGHT * 0.75)
        {
            font.setPixelSize(height / 11 * 0.5);
            setHeaderData(currentModel, currentSummary);
            reportModel->setHeaderData(Channel_Column, Qt::Horizontal, tr("Channel"));
            heightHeader = 45;
        }
        else
        {
            font.setPixelSize(height / 11 * 0.8);
            setHeaderNull();
            heightHeader = 20;
            reportSummary->p_view->verticalHeader()->setDefaultSectionSize(height / 8);
        }
    }
    else
    {
        reportSummary->p_headerLabel->setGeometry(width / 13, 0, 400, 20);
        setHeaderNull();
        reportSummary->p_view->verticalHeader()->setDefaultSectionSize(height / 8);
        if (height > DESKTOP_HEIGHT * 0.35 && height <= DESKTOP_HEIGHT * 0.75)
        {
            font.setPixelSize(height / 14 * 0.6);
            heightHeader = 45;
        }
        else
        {
            font.setPixelSize(height / 14);
            heightHeader = 20;
        }
    }
    m_reportSummary[0].p_view->horizontalHeader()->setFixedHeight(heightHeader);
    m_reportSummary[1].p_view->horizontalHeader()->setFixedHeight(heightHeader);
    reportSummary->p_headerLabel->setFont(font);
    reportSummary->p_view->setFont(font);
}

void WinReport::setHeaderData(ModelReport *currentModel, WinReportSummary *currentSummary)
{
    ModelReport *reportModel;
    WinReportSummary *reportSummary;

    if (m_isSingle)
    {
        reportModel = &m_reportModel[0];
        reportSummary = &m_reportSummary[0];
    }
    else
    {
        if (currentModel == NULL)
            reportModel = &m_reportModel[m_screen.getCurrentWidget()];
        else
            reportModel = currentModel;
        if (currentSummary == NULL)
            reportSummary = (WinReportSummary *)m_screen.getGroupWidget(m_screen.getCurrentWidget());
        else
            reportSummary = currentSummary;
    }

    reportModel->setHeaderData(Channel_Column, Qt::Horizontal, spaceStr + tr("Channel"));
    reportModel->setHeaderData(Unit_Column, Qt::Horizontal, tr("Unit"));
    reportModel->setHeaderData(Status_Column, Qt::Horizontal, tr("Status"));
    if (reportSummary->p_view->model()->rowCount() == 0)
        return ;

    if (reportSummary->p_view->model()->index(0, Ave_Column).data().toString() == NULL)
    {
        if (reportSummary->p_view->model()->index(0, Max_Column).data().toString() == NULL)
            reportModel->setHeaderData(Ave_Column, Qt::Horizontal, "");
        else
            reportModel->setHeaderData(Ave_Column, Qt::Horizontal, tr("Max"));
    }
    else
    {
        if (reportSummary->p_view->model()->index(0, Max_Column).data().toString() == NULL)
            reportModel->setHeaderData(Ave_Column, Qt::Horizontal, tr("Ave"));
        else
            reportModel->setHeaderData(Ave_Column, Qt::Horizontal, QString(tr("Ave")) + "\n" + tr("Max"));
    }

    if (reportSummary->p_view->model()->index(0, Min_Column).data().toString() == NULL)
    {
        if (reportSummary->p_view->model()->index(0, Inst_Column).data().toString() == NULL)
            reportModel->setHeaderData(Min_Column, Qt::Horizontal, "");
        else
            reportModel->setHeaderData(Min_Column, Qt::Horizontal, tr("Inst"));
    }
    else
    {
        if (reportSummary->p_view->model()->index(0, Inst_Column).data().toString() == NULL)
            reportModel->setHeaderData(Min_Column, Qt::Horizontal, tr("Min"));
        else
            reportModel->setHeaderData(Min_Column, Qt::Horizontal, QString(tr("Min")) + "\n" + tr("Inst"));
    }

    if (reportSummary->p_view->model()->index(0, Sum_Column).data().toString() == NULL)
        reportModel->setHeaderData(Sum_Column, Qt::Horizontal, "");
    else
        reportModel->setHeaderData(Sum_Column, Qt::Horizontal, tr("Sum"));
}

void WinReport::setHeaderNull()
{
    for (int i = 0; i < 2; i++)
    {
        m_reportModel[i].setHeaderData(Channel_Column, Qt::Horizontal, "");
        m_reportModel[i].setHeaderData(Unit_Column, Qt::Horizontal, "");
        m_reportModel[i].setHeaderData(Status_Column, Qt::Horizontal, "");
        m_reportModel[i].setHeaderData(Ave_Column, Qt::Horizontal, "");
        m_reportModel[i].setHeaderData(Min_Column, Qt::Horizontal, "");
        m_reportModel[i].setHeaderData(Unit_Column, Qt::Horizontal, "");
        m_reportModel[i].setHeaderData(Max_Column, Qt::Horizontal, "");
        m_reportModel[i].setHeaderData(Sum_Column, Qt::Horizontal, "");
    }
}


void WinReport::paintEvent(QPaintEvent *event)
{
    ModelReport *reportModel;
    WinReportSummary *reportSummary;
    if (m_isSingle)
    {
        reportModel = &m_reportModel[0];
        reportSummary = &m_reportSummary[0];
    }
    else
    {
        reportModel = &m_reportModel[m_screen.getCurrentWidget()];
        reportSummary = (WinReportSummary *)m_screen.getGroupWidget(m_screen.getCurrentWidget());
    }
    quint32 maxId = reportModel->getMaxId();
    if (maxId != reportModel->m_maxId)
    {
        reportModel->m_maxId = maxId;
        setTopFrameValue();
    }
    if (m_screen.m_sceneCnt == 1 && reportModel->m_maxId >= 2)  //
    {
        slotDisplayNextReport();
        slotDisplayNextReport();
        m_screen.addWidgetItem(1, &m_reportSummary[1]);
    }

    int currentrow = reportSummary->p_view->rowAt(reportSummary->p_view->y());
    setCurrentRow(currentrow + 11, reportModel->rowCount());
    QWidget::paintEvent(event);
}



/*************************************************************************************
 *
 * 重写代理类
 *  实现第3列...
 *  实现第4,5列在一个item中，上下两行显示数据
 *  实现所有列表格只显示下边横线，竖线竖不显示
 *
 * **********************************************************************************/
ReportDelegate::ReportDelegate(QTableView *tableView, QObject * parent) : QItemDelegate(parent)
{
    int gridHint = tableView->style()->styleHint(QStyle::SH_Table_GridLineColor, new QStyleOptionViewItemV4());
    QColor gridColor = static_cast<QRgb>(gridHint);
    pen = QPen(gridColor,0,tableView->gridStyle());
    view = tableView;
}

/*
** 代理模型的重画view界面的函数
**  实现第3列...
**  实现第4,5列在一个item中，上下两行显示数据
**  实现所有列表格只显示下边横线，竖线竖不显示
*/
void ReportDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{

    int x = option.rect.x();
    int y = option.rect.y();
    int width = option.rect.width();
    int height = option.rect.height();

    if (index.column() == Ave_Column || index.column() == Min_Column)
    {
        int pixelSize = option.rect.height();
        //RealNumberNotation   TODO  里面包含科学计数法  setRealNumberNotation
        QVariant str = index.model()->data(index, Qt::DisplayRole);
        QVariant strNext = index.model()->index(index.row(), index.column() + 1).data();

        painter->save();
        QFont font =option.font;
        if (pixelSize * 13 > DESKTOP_HEIGHT)
            font.setPixelSize(pixelSize / 5);
        else if (pixelSize * 13 < DESKTOP_HEIGHT && pixelSize * 13 > DESKTOP_HEIGHT / 2)
            font.setPixelSize(pixelSize / 3);
        else
            font.setPixelSize(pixelSize / 2);
        painter->setFont(font);
        if (str.toString() == NULL)
        {
            if (strNext.toString() != NULL)
                painter->drawText(x, y, width, height , Qt::AlignVCenter, QString::number(strNext.toReal()));
        }
        else
        {
            if (strNext.toString() != NULL)
            {
                painter->drawText(x, y, width, height / 2 , Qt::AlignVCenter, QString::number(str.toReal()));
                painter->drawText(x, y + height / 2, width, height / 2 , Qt::AlignVCenter,QString::number(strNext.toReal()));
            }
            else
                painter->drawText(x, y, width, height , Qt::AlignVCenter, QString::number(str.toReal()));
        }
        painter->restore();
    }
    else if (index.column() == Sum_Column)
    {
        QVariant str = index.model()->data(index, Qt::DisplayRole);
        if (str.toString() != NULL)
            painter->drawText(x, y, width, height , Qt::AlignVCenter, QString::number(str.toReal()));
    }
    else
    {
        QString str = index.model()->data(index, Qt::DisplayRole).toString();
        painter->drawText(x, y, width, height, Qt::AlignVCenter, str);
    }

    QPen oldPen = painter->pen();
    painter->setPen(pen);
    if (index.row() == 0)
        painter->drawLine(option.rect.topLeft(),option.rect.topRight());
    painter->drawLine(option.rect.bottomLeft(),option.rect.bottomRight());
    painter->setPen(oldPen);
}

bool ReportDelegate::helpEvent(QHelpEvent */*event*/,
                                     QAbstractItemView */*view*/,
                                     const QStyleOptionViewItem &/*option*/,
                                     const QModelIndex &/*index*/)
{
    return false;
}




