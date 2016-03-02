#include "winmemsummary.h"
#include <QHeaderView>
#include <QDateTime>
#include "./Threads/threaddatarecord.h"
#include "./WorkWin/winsummarydialog.h"
#include "./WorkWin/wininfolistdialog.h"
#include "./Common/authorization.h"
#include "./DataModel/modelsummarymem.h"
#include "../GlobalData/globalDef.h"

QMap<int, bool> s_displayStatusMap;     //记录显示数据的选中状态map  int: 表中主键id,    bool : 选中状态
QMap<int, bool> s_eventStatusMap;       //记录事件数据的选中状态map  int: 表中主键id,    bool : 选中状态

/* 标志是displayBtn被选中还是eventBtn被选中 false:  true: */
bool                s_displayOrEventChange;

#define STARTTIME_COLUMN        1
#define TIME_COLUMN             2
#define STATUS_COLUMN           3
#define DATACOUNT_COLUMN        4
#define FILENAME_COLUMN         5

#define MAX_COUNT   1000

#define DISP_MAIN_TABLE         "table_Main_Disp"     //显示数据主表
#define EVENT_MAIN_TABLE        "table_Main_Event"  //事件数据主表
#define MANUAL_MAIN_TABLE       "table_Main_Manual"   //手动采样数据主表
#define REPORT_MAIN_TABLE       "table_Main_Report" //报表数据主表
#define MANUAL_MAIN_TABLE       "table_Main_Manual"   //手动采样数据主表

static const int maxManualCount = 400;
static const int maxReportCount = 800;

WinMemSummary::WinMemSummary(QWidget *parent) :
    QWidget(parent), m_isSave(false), m_manualCnt(0), m_reportCnt(0)
{
    m_timeFormateStr = getSysDateFormate();
    initMemSummaryUI();
    initModel();
    initCheckMap();

    m_memView.p_view->setModel(p_modelDisp);
    showView();

    getManualDataInfo();
    getReportDataInfo();
    setSampleLabelData();
    signalAndSlotConnect();
}

WinMemSummary::~WinMemSummary()
{
    s_displayStatusMap.clear();
    s_eventStatusMap.clear();
    delete p_modelDisp;
    delete p_modelEvent;
}


/*
 * 内存一览界面组件添加函数
*/
void WinMemSummary::initMemSummaryUI()
{
    s_displayOrEventChange = false;

    QGridLayout *gridLayout = new QGridLayout(this);
    gridLayout->setSpacing(0);
    gridLayout->setContentsMargins(0, 0, 0, 0);

    m_topFrame.setParent(this);
    m_topFrame.setMinimumSize(QSize(0, 35));
    m_topFrame.setMaximumHeight(70);
    m_topFrame.setFrameShape(QFrame::StyledPanel);
    m_topFrame.setFrameShadow(QFrame::Raised);
    m_topFrame.setLineWidth(0);
    m_topFrame.setStyleSheet("background:#ECECEC");

    QHBoxLayout * hTopFrameLayout = new QHBoxLayout(&m_topFrame);
    hTopFrameLayout->setSpacing(0);

    QPushButton *upBtn = new QPushButton(&m_topFrame);
    upBtn->setMaximumSize(QSize(50, 16777215));
    upBtn->setIcon(QIcon(":/summary/Image/summary/up.png"));
    upBtn->setIconSize(QSize(45, 25));
    upBtn->setFocusPolicy(Qt::NoFocus);
    hTopFrameLayout->addWidget(upBtn);

    QPushButton *downBtn = new QPushButton(&m_topFrame);
    downBtn->setMaximumSize(QSize(50, 16777215));
    downBtn->setIcon(QIcon(":/summary/Image/summary/down.png"));
    downBtn->setIconSize(QSize(45, 25));
    downBtn->setFocusPolicy(Qt::NoFocus);
    hTopFrameLayout->addWidget(downBtn);

    hTopFrameLayout->addItem(new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding));
    m_sampleLabel.setParent(&m_topFrame);
    m_sampleLabel.setObjectName("m_sampleLabel");
    hTopFrameLayout->addWidget(&m_sampleLabel);
    hTopFrameLayout->addItem(new QSpacerItem(40, 40, QSizePolicy::Minimum, QSizePolicy::Expanding));

    m_toolBtn.setParent(&m_topFrame);
    m_toolBtn.setObjectName("toolBtn");
    m_toolBtn.setMaximumSize(QSize(160, 40));

    m_toolBtn.setStyleSheet("background-color: rgb(255, 255, 255);");
    m_toolBtn.setFocusPolicy(Qt::NoFocus);

    m_displayRadioBtn.setParent(&m_toolBtn);
    m_displayRadioBtn.setGeometry(QRect(10, 0, 117, 16));
    m_displayRadioBtn.setChecked(!s_displayOrEventChange);
    m_displayRadioBtn.setFocusPolicy(Qt::NoFocus);
    m_eventRadionBtn.setParent(&m_toolBtn);
    m_eventRadionBtn.setGeometry(QRect(10, 20, 117, 16));
    m_eventRadionBtn.setFocusPolicy(Qt::NoFocus);

    hTopFrameLayout->addWidget(&m_toolBtn);

    gridLayout->addWidget(&m_topFrame, 0, 0, 2, 1);

    m_memView.setParent(this);
    m_memView.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    gridLayout->addWidget(&m_memView, 2, 0, 1, 1);

    connect(upBtn, SIGNAL(pressed()), &m_memView, SLOT(slotUpBtnPressed()));
    connect(upBtn, SIGNAL(released()), &m_memView, SLOT(slotUpBtnRelease()));
    connect(downBtn, SIGNAL(pressed()), &m_memView, SLOT(slotDownBtnPressed()));
    connect(downBtn, SIGNAL(released()), &m_memView, SLOT(slotDownBtnRelease()));

    for (int i = 0; i < 3; i++)
    {
        m_frameLine[i].setParent(m_memView.p_view);
        m_frameLine[i].setFrameShape(QFrame::VLine);
        m_frameLine[i].setFrameShadow(QFrame::Sunken);
        m_frameLine[i].hide();
    }

    m_btnBox.setParent(this);
    m_btnBox.setObjectName("m_btnBox");
    m_btnBox.setGeometry((DESKTOP_WIDTH - 280) / 2, DESKTOP_HEIGHT - 120, 280, 120);
    m_btnBox.hide();
    QHBoxLayout *btnLayout = new QHBoxLayout(&m_btnBox);
    btnLayout->setSpacing(1);
    btnLayout->setContentsMargins(0,0,0,0);

    m_saveBtn.setParent(&m_btnBox);
    m_saveBtn.setObjectName("m_saveBtn");
    m_saveBtn.setText(QString("\n\n\n\n") + tr("Select save"));
    m_saveBtn.setFocusPolicy(Qt::NoFocus);
    m_saveBtn.setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    m_cancleBtn.setParent(&m_btnBox);
    m_cancleBtn.setObjectName("m_cancleBtn");
    m_cancleBtn.setText(QString("\n\n\n\n") + tr("Uselect"));
    m_cancleBtn.setFocusPolicy(Qt::NoFocus);
    m_cancleBtn.setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    btnLayout->addWidget(&m_saveBtn);
    btnLayout->addWidget(&m_cancleBtn);
    m_btnBox.setLayout(btnLayout);

    m_displayRadioBtn.setText(tr("display data"));
    m_eventRadionBtn.setText("event data");

    setStyleSheet("QToolButton#toolBtn{\
                  border-top-right-radius:5px;\
                  border-bottom-right-radius:5px;\
                  background-color:#0000ff;\
                  height:20px;\
                  outline:none;\
                  }\
                  QToolButton#toolBtn:press{\
                  border-top-right-radius:5px;\
                  border-bottom-right-radius:5px;\
                  background-color:#00ff00;\
                  height:20px;\
                  outline:none;\
                  }\
                  QPushButton {border: none;}\
                  QLabel#m_sampleLabel{color:rgb(100, 100, 100);font-size:16px;}\
                  QFrame#m_btnBox{background-color:rgb(100, 100, 100, 100);}\
                  QPushButton#m_saveBtn{\
                      font-size:20px;\
                      color:#404040;\
                      background-image:url(:/summary/Image/summary/save.png);\
                      background-repeat:no-repeat;\
                      background-position:center;}\
                  QPushButton#m_cancleBtn{\
                      font-size:20px;\
                      color:#404040;\
                      background-image:url(:/summary/Image/summary/cancle.png);\
                      background-repeat:no-repeat;\
                      background-position:center;}\
                  QPushButton#m_saveBtn:pressed,\
                  QPushButton#m_cancleBtn:pressed{\
                      background-color:rgb(0,187,255,100%);\
                  }");
}

/*
 * 设置显示手动采样数据和事件数据的数目和时间
*/
void WinMemSummary::setSampleLabelData()
{
    m_sampleLabel.setText(QString("%7: (%1/%2)\t%3\n%8: (%4/%5)\t\t%6").arg(m_manualCnt, 3, 10, QChar('0'))\
                         .arg(maxManualCount, 3, 10, QChar('0')).arg(m_manualTime).arg(m_reportCnt, 3, 10, QChar('0'))\
                         .arg(maxReportCount, 3, 10, QChar('0')).arg(m_reportTime).arg(tr("Manual sampling data")).\
                           arg(tr("report data")));
}



void WinMemSummary::getManualDataInfo()
{
    QSqlQuery sampleQuery(QString("select ID, LastTime from %1 order by ID desc limit 1").arg(MANUAL_MAIN_TABLE), *ThreadDataRecord::instance()->getDatabaseConnect());
    sampleQuery.exec();
    ThreadDataRecord::instance()->putDatabaseConnect();
    if (sampleQuery.next()) {
        m_manualCnt = sampleQuery.value(0).toInt();
        m_manualTime = QDateTime::fromMSecsSinceEpoch(sampleQuery.value(1).toLongLong()).toString("yyyy/MM/dd hh:mm:ss");
    }
}

void WinMemSummary::getReportDataInfo()
{
    QSqlQuery reportQuery(QString("select ID, EndTime from %1 order by ID desc limit 1").arg(REPORT_MAIN_TABLE), \
                          *ThreadDataRecord::instance()->getDatabaseConnect());
    reportQuery.exec();
    ThreadDataRecord::instance()->putDatabaseConnect();
    if (reportQuery.next()) {
        m_reportCnt = reportQuery.value(0).toInt();
        m_reportTime = QDateTime::fromMSecsSinceEpoch(reportQuery.value(1).toLongLong()).toString("yyyy/MM/dd hh:mm:ss");
    }
}

QString WinMemSummary::getSysDateFormate()
{
    QString fmt = "%1%4%2%4%3 %5";
    QString mm = "MM";
    QString deli = "/";
    QString ret = "";
    QString yy;
    int width = this->width();

    if (width > DESKTOP_WIDTH * 0.75)
        yy = "yyyy";
    else if (width > DESKTOP_WIDTH * 0.35 && width <= DESKTOP_WIDTH * 0.75)
        yy = "yy";
    else
        yy = "";

    if(DevConfigPtr->systemConfig.monthIndicator == Month_Indicator_Character){
        mm = "MMM";
    }

    if(DevConfigPtr->systemConfig.delimiterType == Delimiter_Type_Point){
        deli = ".";
    }else if(DevConfigPtr->systemConfig.delimiterType == Delimiter_Type_Hyphen){
        deli = "-";
    }

    if(DevConfigPtr->systemConfig.dateFormat == Date_Format_YMD){
        ret = fmt.arg(yy,mm,"dd",deli, "hh:mm:ss");
    }else if(DevConfigPtr->systemConfig.dateFormat == Date_Format_MDY){
        ret = fmt.arg(mm, yy,"dd",deli, "hh:mm:ss");
    }else{
        ret = fmt.arg("dd",mm, yy,deli, "hh:mm:ss");
    }

    return ret;
}

quint32 WinMemSummary::getManualMaxID()
{
    QSqlQuery sampleQuery(QString("select ID from %1 order by ID desc limit 1").arg(MANUAL_MAIN_TABLE), *ThreadDataRecord::instance()->getDatabaseConnect());
    sampleQuery.exec();
    ThreadDataRecord::instance()->putDatabaseConnect();

    if (sampleQuery.next())
        return sampleQuery.value(0).toUInt();
    else
        return 0;
}

quint32 WinMemSummary::getReportMaxID()
{
    QSqlQuery reportQuery(QString("select ID from %1 order by ID desc limit 1").arg(REPORT_MAIN_TABLE), \
                          *ThreadDataRecord::instance()->getDatabaseConnect());
    reportQuery.exec();
    ThreadDataRecord::instance()->putDatabaseConnect();

    if (reportQuery.next())
        return reportQuery.value(0).toUInt();
    else
        return 0;
}





/* 绑定信号和槽 */
void WinMemSummary::signalAndSlotConnect()
{
    /* 切换显示界面的信号和槽函数 */
    connect(&m_toolBtn, SIGNAL(clicked()), this, SLOT(slotRadioBtnChange()));
    connect(&m_displayRadioBtn, SIGNAL(clicked()), this, SLOT(slotRadioBtnChange()));
    connect(&m_eventRadionBtn, SIGNAL(clicked()), this, SLOT(slotRadioBtnChange()));

    /* 点击当前行，显示当前行的详细信息  */
    connect(m_memView.p_view, SIGNAL(clicked(QModelIndex)), this, SLOT(slotDetailMessage(QModelIndex)));

    /* 用按钮模拟事件(键盘key_up/key_down)的信号槽函数 */
    //某列前面的checkBox被选中时，点击保存和取消的信号槽连接
    connect(&m_saveBtn, SIGNAL(clicked()), this, SLOT(slotSaveBtnClicked()));
    connect(&m_cancleBtn, SIGNAL(clicked()), this, SLOT(slotCancleBtnClicked()));

    connect(m_memView.p_view->horizontalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(slotSectionSort(int)));
    connect(&m_saveTime, SIGNAL(timeout()), this, SLOT(slotSave()));
 }

/*
 * 点击表头排序的信号和槽函数
*/
void WinMemSummary::slotSectionSort(int section)
{
    m_memView.p_view->setCurrentIndex(m_memView.p_view->model()->index(0, section));
}

void WinMemSummary::slotSaveData()
{
    m_isSave = true;
    WinSummarySaveDialog::instance()->m_okBtn.setEnabled(false);
    WinSummarySaveDialog::instance()->m_cancleBtn.setEnabled(false);
    WinInfoListDialog::instance()->showMsg(803);
    m_selectCnt = m_saveList.size();
    m_saveTime.start(1000);
}

void WinMemSummary::slotSave()
{
    if (s_displayOrEventChange)
        ThreadDataSave::instance()->saveDispData(m_saveList.first());
    else
        ThreadDataSave::instance()->saveEventData(m_saveList.first());
    m_saveList.removeFirst();
    WinSummarySaveDialog::instance()->setBarValue(m_selectCnt - m_saveList.size());
    if (m_saveList.size() == 0)
    {
        m_saveTime.stop();
        WinInfoListDialog::instance()->showMsg(806);
        WinInfoListDialog::instance()->exec();  //功能是阻塞进度保存窗体，让进度保存窗体和806窗体一起关闭
        WinSummarySaveDialog::instance()->m_bar.hide();
        WinSummarySaveDialog::instance()->m_okBtn.setEnabled(true);
        WinSummarySaveDialog::instance()->m_cancleBtn.setEnabled(true);
        m_isSave = false;
        WinSummarySaveDialog::instance()->close();
    }
}

/*
** 初始化显示数据的model和事件数据的model
** para 	: 无
** return : 无
*/
void WinMemSummary::initModel()
{
    p_modelEvent = new ModelSummaryMem(Event_Summary_Type);
    m_eventSumRow = p_modelEvent->rowCount();
    p_modelDisp = new ModelSummaryMem(Display_Summary_Type);
    m_displaySumRow = p_modelDisp->rowCount();
}

void WinMemSummary::initCheckMap()
{
    for (int i = 0; i < m_displaySumRow; i++)
        s_displayStatusMap.insert(p_modelDisp->index(i, 0).data().toInt(), false);

    for (int i = 0; i < m_eventSumRow; i++)
        s_eventStatusMap.insert(p_modelEvent->index(i, 0).data().toInt(), false);
}


/*
 *
*/
void WinMemSummary::setCurrentRow(int current, int sumrow)
{
    if (current < 11)
        current = 11;
    if (sumrow < 11)
        current = sumrow;

    if (this->height() < DESKTOP_HEIGHT * 0.35)
        m_memView.p_headerLabel->setText(QString("%1/%2\t%3").arg(current, 4, 10, QChar('0'))
                                               .arg(sumrow, 4, 10, QChar('0'))
                                               .arg(tr("Memory summary")));
    else
        m_memView.p_headerLabel->setText(QString("%1/%2").arg(current, 4, 10, QChar('0')).arg(sumrow, 4, 10, QChar('0')));
}

/*
** 显示某行的详细信息和保存取消按钮的显示与隐藏
*/
void WinMemSummary::slotDetailMessage(QModelIndex index)
{
    if(Authorization::instance()->getLoginStatus() == Authorization::Auth_LoginStatus_Logout){
        WinInfoListDialog::instance()->showMsg(818);    //818 :请使用MENU 键登录。Press [MENU] key to login.
    }else{
        if (index.column() != 0)    //点击的不是第一列 显示点击列的详细信息
        {
            WinSummaryDialog dlg(Summary_Mem_Type, index, true);
            dlg.exec();
        }
        else if (index.column() == 0)       //点击第一列 有选中，显示保存取消按钮，没选中，保存取消按钮隐藏
        {
            showShortcutKey();
        }
    }
}

/*
** 显示 显示数据或者事件信息的槽函数
*/
void WinMemSummary::slotRadioBtnChange()
{
    //点击显示数据和事件数据所在的toolBtn后，按钮状态的切换
    s_displayOrEventChange = !s_displayOrEventChange;
    m_displayRadioBtn.setChecked(!s_displayOrEventChange);
    m_eventRadionBtn.setChecked(s_displayOrEventChange);

    if (!s_displayOrEventChange)      //
    {
        m_memView.p_view->setModel(p_modelDisp);
        setCurrentRow(0, m_displaySumRow);
        setHeaderData(*p_modelDisp);
        p_modelDisp->setHeaderData(TIME_COLUMN, Qt::Horizontal, tr("Stop time\nStart time"));
    }
    else
    {
        m_memView.p_view->setModel(p_modelEvent);
        setCurrentRow(0, m_eventSumRow);
        setHeaderData(*p_modelEvent);
        p_modelEvent->setHeaderData(TIME_COLUMN, Qt::Horizontal, tr("Stop time\nStart time"));
    }
    m_memView.p_view->setCurrentIndex(m_memView.p_view->model()->index(0, 0));

    slotCancleBtnClicked();     //当前列为第一列取消所有选中
}



/*
 *  当数据库中新添加行的时候，再次查询数据库时，相对应的选中标记map也需要添加与删除
*/
void WinMemSummary::mapChanged(bool flag)
{
    if (!flag)
    {
        int lastKey = s_displayStatusMap.lastKey();
        int maxId = /*getMaxId(DISP_MAIN_TABLE);*/p_modelDisp->getMaxId();
        for (int i = lastKey + 1; i <= maxId; i++)
        {
            if (i > MAX_COUNT)  //如果显示的的数据大于规定的显示最大条数，删除最小记录的选中标记，添加最大记录的选中标记
            {
                s_displayStatusMap.remove(i - MAX_COUNT);
                s_displayStatusMap.insert(i, false);
            }
            else    //否则直接添加最大数据的选中标记
            {
                s_displayStatusMap.insert(i, false);
            }
        }
    }
    else
    {
        int lastKey = s_eventStatusMap.lastKey();
        int maxId = /*getMaxId(EVENT_MAIN_TABLE);*/p_modelEvent->getMaxId();
        for (int i = lastKey + 1; i <= maxId; i++)
        {
            {
                if (i > MAX_COUNT)
                {
                    s_eventStatusMap.remove(i - MAX_COUNT);
                    s_eventStatusMap.insert(i, false);
                }
                else
                {
                    s_eventStatusMap.insert(i, false);
                }
            }
        }
    }
}


/*
 * 重写resizeEvent函数，当界面大小小于一个值的时候，该view窗口会隐藏或者缩放某列
*/
void WinMemSummary::resizeEvent(QResizeEvent * /*event*/)
{
    m_timeFormateStr = getSysDateFormate();
    setviewColumnStyle();
    initModelHeaderData();

    if (this->width() < DESKTOP_WIDTH ) {
        m_topFrame.hide();
    } else {
        m_topFrame.show();
    }
    setVline(m_memView.p_view);
}

/**
 * @brief line
 * @每个表头旁边的竖线
 */
void WinMemSummary::setVline(QTableView *view)
{
    int widthLocal[3];
    widthLocal[0] = view->columnWidth(0) + view->columnWidth(2);
    widthLocal[1] = widthLocal[0] + view->columnWidth(3);
    widthLocal[2] = widthLocal[1] + view->columnWidth(4);
    int height = view->horizontalHeader()->height() - 12;

    for (int i = 0; i < 3; i++)
        m_frameLine[i].setGeometry(QRect(widthLocal[i] - 6, 6, 4, height));
}


/*
** 保存选中
*/
void WinMemSummary::slotSaveBtnClicked()
{
    if (m_isSave)   //如果当前数据正在保存，则弹出正在保存窗体，错误代码211
    {
         WinInfoListDialog::instance()->showMsg(211);
    }
    else
    {
        QMap<int, bool> map;
        m_saveList.clear();
        if(!s_displayOrEventChange)
            map = s_displayStatusMap;
        else
            map = s_eventStatusMap;

        QMap<int, bool>::const_iterator i = map.constBegin();
        while (i != map.constEnd())
        {
            if (i.value())
                m_saveList.append(i.key());
            i++;
        }

        WinSummarySaveDialog::instance()->showMsg(m_saveList.size());
        connect(&WinSummarySaveDialog::instance()->m_okBtn, SIGNAL(clicked()), this, SLOT(slotSaveData()));
    }
}


void WinMemSummary::paintEvent(QPaintEvent *event)
{
    m_timeFormateStr = getSysDateFormate();
    if (this->width() > DESKTOP_WIDTH * 0.35)
        ((MemSummaryDelegate *)m_memView.p_view->itemDelegate())->setTimeFormate(m_timeFormateStr);
    else
        ((MemSummaryDelegate *)m_memView.p_view->itemDelegate())->setTimeFormate(m_timeFormateStr.right(8));
    int currentrow = m_memView.p_view->rowAt(m_memView.p_view->y());
    if (!s_displayOrEventChange)
    {
        if (p_modelDisp->getMaxId() != p_modelDisp->m_maxId)
        {
            p_modelDisp->init();
            if (s_displayStatusMap.size() == 0)
                initCheckMap();
            else
                mapChanged(s_displayOrEventChange);
            m_displaySumRow = p_modelDisp->rowCount();
        }
        setCurrentRow(currentrow + 12, m_displaySumRow);
    }
    else
    {
        if (p_modelEvent->getMaxId() != p_modelEvent->m_maxId)
        {
            p_modelEvent->init();
            if (s_eventStatusMap.size() == 0)
                initCheckMap();
            else
                mapChanged(s_displayOrEventChange);
            m_eventSumRow = p_modelEvent->rowCount();
        }
        setCurrentRow(currentrow + 12, m_eventSumRow);
    }
    if (m_reportCnt != getReportMaxID())
    {
        getReportDataInfo();
        setSampleLabelData();
    }
    if (m_manualCnt != getManualMaxID())
    {
        getManualDataInfo();
        setSampleLabelData();
    }

    QWidget::paintEvent(event);
}

/*
** 取消所有选中
**      取消后选中标记去掉
*/
void WinMemSummary::slotCancleBtnClicked()
{
    if (m_isSave)   //如果当前数据正在保存，则弹出正在保存窗体，错误代码211
    {
         WinInfoListDialog::instance()->showMsg(211);
         return ;
    }
    if(s_displayOrEventChange)
    {
        qFill(s_eventStatusMap, false);

//        QMap<int, bool>::const_iterator i = s_eventStatusMap.constBegin();
//        while (i != s_eventStatusMap.constEnd())
//        {
//            if (i.value())
//                s_eventStatusMap[i.key()] = false;
//            i++;
//        }
    }
    else
    {
        qFill(s_displayStatusMap, false);
//        QMap<int, bool>::const_iterator i = s_displayStatusMap.constBegin();
//        while (i != s_displayStatusMap.constEnd())
//        {
//            if (i.value())
//                s_displayStatusMap[i.key()] = false;
//            i++;
//        }
    }
    m_btnBox.hide();   //隐藏按钮
}

/*
** 快捷键的显示和隐藏
*/
void WinMemSummary::showShortcutKey()
{
    if (!s_displayOrEventChange)
    {
        if (s_displayStatusMap.values().contains(true))
            m_btnBox.show();
        else
            m_btnBox.hide();
    }
    else
    {
        if (s_eventStatusMap.values().contains(true))
            m_btnBox.show();
        else
            m_btnBox.hide();
    }
}

/*
** 显示显示数据或者事件数据的view函数
*/
void WinMemSummary::showView()
{
    m_memView.p_view->setItemDelegate(new MemSummaryDelegate(m_memView.p_view));
    m_memView.p_view->sortByColumn(TIME_COLUMN, Qt::DescendingOrder);    //降序
    m_memView.p_view->setColumnHidden(STARTTIME_COLUMN, true);
    m_memView.p_view->setColumnWidth(0, 85);
}

/*
 * 隐藏view中的列
*/
void WinMemSummary::setviewColumnStyle()
{
    int width = this->width();
    int height = this->height();
    m_memView.p_view->setColumnWidth(TIME_COLUMN, width / 4);
    if (width < DESKTOP_WIDTH * 0.35)
    {
        m_memView.p_view->setColumnHidden(FILENAME_COLUMN, true);
        m_memView.p_view->setColumnWidth(STATUS_COLUMN, width / 3);
    }
    else
    {
        if (height > DESKTOP_HEIGHT * 0.75)
            m_memView.p_view->setColumnWidth(TIME_COLUMN, width / 3);
        m_memView.p_view->setColumnHidden(FILENAME_COLUMN, false);
        m_memView.p_view->setColumnWidth(DATACOUNT_COLUMN, width / 9);
        m_memView.p_view->setColumnWidth(STATUS_COLUMN, width / 5);
    }
}


/*
 * view的表头和upBtn与p_downBtn的隐藏与显示
 */
void WinMemSummary::initModelHeaderData()
{
    int width = this->width();
    int height = this->height();
    int heightHeader;
    m_memView.p_view->horizontalHeader()->setSortIndicatorShown(false);
    m_memView.p_view->verticalHeader()->setDefaultSectionSize(height / 13);
    m_memView.p_headerLabel->setGeometry(0, 0, 400, 20);
    QFont font, fontLabel;

    if (width > DESKTOP_WIDTH * 0.75)
    {
        setHeaderData(*p_modelDisp);
        setHeaderData(*p_modelEvent);
        if (height > DESKTOP_HEIGHT * 0.75)
        {
            font.setPixelSize(height / 15 * 0.4);
            fontLabel.setPixelSize(height / 22 * 0.4);
            p_modelDisp->setHeaderData(TIME_COLUMN, Qt::Horizontal, tr("Stop time\nStart time"));
            p_modelEvent->setHeaderData(TIME_COLUMN, Qt::Horizontal, tr("Stop time\nStart time"));
            m_memView.p_headerLabel->setGeometry(0, 0, 85, 20);
            heightHeader = 50;
            m_memView.p_headerLabel->setFixedHeight(heightHeader);
            m_memView.p_view->horizontalHeader()->setSortIndicatorShown(true);
            for (int i = 0; i < 3; i++)
                m_frameLine[i].show();
        }
        else/* if (height > DESKTOP_HEIGHT * 0.33 && height <= DESKTOP_HEIGHT * 0.75)*/
        {
            font.setPixelSize(height / 12 * 0.5);
            fontLabel.setPixelSize(height / 12 * 0.5);
            heightHeader = 45;
            m_memView.p_view->setColumnHidden(0, true);
            m_memView.p_headerLabel->setFixedHeight(heightHeader / 2);
        }
        m_memView.p_view->horizontalHeader()->setFont(font);
        ((MemSummaryDelegate *)m_memView.p_view->itemDelegate())->setTimeFormate(m_timeFormateStr);
    }
    else if (width > DESKTOP_WIDTH * 0.35 && width <= DESKTOP_WIDTH * 0.75)
    {
        if (height > DESKTOP_HEIGHT * 0.75)
        {
            font.setPixelSize(height / 22 * 0.5);
            fontLabel.setPixelSize(height / 18 * 0.5);
            setHeaderData(*p_modelDisp);
            setHeaderData(*p_modelEvent);
            heightHeader = 50;
            m_memView.p_headerLabel->setFixedHeight(heightHeader / 3);
            m_memView.p_view->horizontalHeader()->setFont(font);
        }
        else if (height > DESKTOP_HEIGHT * 0.35 && height <= DESKTOP_HEIGHT * 0.75)
        {
            font.setPixelSize(height / 12 * 0.5);
            fontLabel.setPixelSize(height / 12 * 0.5);
            setHeaderData(*p_modelDisp);
            setHeaderData(*p_modelEvent);
            heightHeader = 45;
            m_memView.p_headerLabel->setFixedHeight(heightHeader / 2);
            m_memView.p_view->horizontalHeader()->setFont(font);
        }
        else
        {
            font.setPixelSize(height / 12 * 0.8);
            fontLabel.setPixelSize(height / 12 * 0.8);
            setHeaderNull(*p_modelDisp);
            setHeaderNull(*p_modelEvent);
            heightHeader = 20;
            m_memView.p_view->verticalHeader()->setDefaultSectionSize(height / 8);
            m_memView.p_headerLabel->setFixedHeight(heightHeader);
        }
        m_memView.p_view->setColumnHidden(0, true);
        ((MemSummaryDelegate *)m_memView.p_view->itemDelegate())->setTimeFormate(m_timeFormateStr);
    }
    else
    {
        if (height > DESKTOP_HEIGHT * 0.35 && height <= DESKTOP_HEIGHT * 0.75)
        {
            m_memView.p_headerLabel->setGeometry(0, 0, 400, 20);
            font.setPixelSize(height / 15 * 0.6);
            fontLabel.setPixelSize(height / 15 * 0.6);
            setHeaderData(*p_modelDisp);
            setHeaderData(*p_modelEvent);
            heightHeader = 45;
            m_memView.p_headerLabel->setFixedHeight(heightHeader / 3);
            m_memView.p_view->horizontalHeader()->setFont(font);
        }
        else
        {
            m_memView.p_headerLabel->setGeometry(width / 13, 0, 400, 20);
            font.setPixelSize(height / 15);
            fontLabel.setPixelSize(height / 15);
            setHeaderNull(*p_modelDisp);
            setHeaderNull(*p_modelEvent);
            heightHeader = 20;
            m_memView.p_headerLabel->setFixedHeight(heightHeader);
        }
        m_memView.p_view->verticalHeader()->setDefaultSectionSize(height / 8);
        m_memView.p_view->setColumnHidden(0, true);
        ((MemSummaryDelegate *)m_memView.p_view->itemDelegate())->setTimeFormate(m_timeFormateStr.right(8));
    }

    m_memView.p_view->horizontalHeader()->setFixedHeight(heightHeader);
    m_memView.p_view->setFont(font);
    m_memView.p_headerLabel->setFont(fontLabel);
}

void WinMemSummary::setHeaderData(ModelSummaryMem &model)
{
    model.setHeaderData(0, Qt::Horizontal, "");
    model.setHeaderData(TIME_COLUMN, Qt::Horizontal, tr("Time"));
    model.setHeaderData(STATUS_COLUMN, Qt::Horizontal, tr("Factor"));
    model.setHeaderData(DATACOUNT_COLUMN, Qt::Horizontal, tr("Data"));
    model.setHeaderData(FILENAME_COLUMN, Qt::Horizontal, tr("File name"));
}

void WinMemSummary::setHeaderNull(ModelSummaryMem &model)
{
    model.setHeaderData(0, Qt::Horizontal, "");
    model.setHeaderData(TIME_COLUMN, Qt::Horizontal, tr(""));
    model.setHeaderData(STATUS_COLUMN, Qt::Horizontal, tr(""));
    model.setHeaderData(DATACOUNT_COLUMN, Qt::Horizontal, tr(""));
    model.setHeaderData(FILENAME_COLUMN, Qt::Horizontal, tr(""));
}





/*****************************************************************************************
	自定义代理类的重新实现
		：实现了第一列显示checkBox按钮
		：实现了第二列显示两行信息(上一行：停止时间， 下一行：开始时间)
*****************************************************************************************/
/*
** 画checkBox的函数
*/
static QRect CheckBoxRect(const QStyleOptionViewItem &view_item_style_options)
{
    QStyleOptionButton check_box_style_option;
    QRect check_box_rect = QApplication::style()->subElementRect(
        QStyle::SE_CheckBoxIndicator,
        &check_box_style_option);

    QPoint check_box_point(view_item_style_options.rect.x() +
                           view_item_style_options.rect.width() / 2 -
                           check_box_rect.width() / 2,
                           view_item_style_options.rect.y() +
                           view_item_style_options.rect.height() / 2 -
                           check_box_rect.height() / 2);
    return QRect(check_box_point, check_box_rect.size());
}

/*
** 把从数据库中读取的整型值转换为相对应的字符串
*/
const QString getRecordStatus(int record_status)
{
    if (record_status == SQL_Record_Status_ManualKey)
        return QString(QObject::tr("Manual key"));
    else if (record_status == SQL_Record_Status_Trigger)
        return QString(QObject::tr("Trigger"));
    else if (record_status == SQL_Record_Status_Recording)
        return QString(QObject::tr("Recording"));
    else if (record_status == SQL_Record_Status_AutoSplitter)
        return QString(QObject::tr("Auto splitter"));
    else if (record_status == SQL_Record_Status_PowerOff)
        return QString(QObject::tr("Power off"));
    else
        return "";
}

MemSummaryDelegate::MemSummaryDelegate(QTableView* tableView, QObject *parent) : QItemDelegate(parent)
{
    int gridHint = tableView->style()->styleHint(QStyle::SH_Table_GridLineColor, new QStyleOptionViewItemV4());
    QColor gridColor = static_cast<QRgb>(gridHint);
    pen = QPen(gridColor,0,tableView->gridStyle());

    view = tableView;
}

void MemSummaryDelegate::setTimeFormate(QString timeFormate)
{
    m_timeFormate = timeFormate;
}

/*
** 重绘制view的代理类重绘制函数
**      实现第一列checkBox
**      实现第二列停止时间和开始时间显示在一个item上
*/
void MemSummaryDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option,const QModelIndex& index)const
{
    painter->save();
    if (index.column() == 0)
    {
        bool checked;
        if (!s_displayOrEventChange)
            checked = s_displayStatusMap[index.data().toInt()];
        else
            checked = s_eventStatusMap[index.data().toInt()];
        QStyleOptionButton check_box_style_option;
        if (index.model()->data(index.model()->index(index.row(), 3)).toInt() != SQL_Record_Status_Recording)
        {
            check_box_style_option.state |= QStyle::State_Enabled;
            if(checked)
            {
                check_box_style_option.state |= QStyle::State_On;
            }
            else
            {
                check_box_style_option.state |= QStyle::State_Off;
            }
        }else
        {
            check_box_style_option.state |= QStyle::State_ReadOnly;
        }
        check_box_style_option.rect = CheckBoxRect(option);
        QApplication::style()->drawControl(QStyle::CE_CheckBox,&check_box_style_option,painter);
        QPen oldPen = painter->pen();
        painter->setPen(pen);
        painter->drawLine(option.rect.topRight(),option.rect.bottomRight());
        painter->setPen(oldPen);
    }
    else if (index.column() == TIME_COLUMN)
    {
        QString startTimeVlaue = index.model()->data(index.model()->index(index.row(), STARTTIME_COLUMN)).toString();
        startTimeVlaue = QDateTime::fromMSecsSinceEpoch(startTimeVlaue.toLongLong()).toString(m_timeFormate);
        QString stopTimevalue = index.data().toString();
        stopTimevalue = QDateTime::fromMSecsSinceEpoch(stopTimevalue.toLongLong()).toString(m_timeFormate);
        QRect rect = option.rect;
        int pixelSize = rect.height();

        painter->save();
        QFont font =option.font;
        if (pixelSize * 13 > DESKTOP_HEIGHT)
            font.setPixelSize(pixelSize / 4);
        else if (pixelSize * 13 < DESKTOP_HEIGHT && pixelSize * 13 > DESKTOP_HEIGHT / 2)
            font.setPixelSize(pixelSize / 3);
        else
            font.setPixelSize(pixelSize / 2);
        painter->setFont(font);
        painter->drawText(rect.x(), rect.y() + rect.height() / 2, rect.width(), rect.height() / 2 , Qt::AlignVCenter, startTimeVlaue);
        painter->drawText(rect.x(), rect.y(), rect.width(), rect.height() / 2 , Qt::AlignVCenter, stopTimevalue);
        painter->restore();
    }
    else if (index.column() == STATUS_COLUMN )
    {
        QRect rect = option.rect;
        int statu = index.data().toInt();
        QString stuatStr = getRecordStatus(statu);
        painter->drawText(rect.x(), rect.y(), rect.width(), rect.height(), Qt::AlignVCenter, stuatStr);
    }
    else if (index.column() == DATACOUNT_COLUMN || index.column() == FILENAME_COLUMN)
    {
        QString str =index.data().toString();
        painter->drawText(option.rect, Qt::TextWordWrap | Qt::AlignLeft | Qt::AlignVCenter, str);
    }

    QPen oldPen = painter->pen();
    painter->setPen(pen);
    if (index.row() == 0)
        painter->drawLine(option.rect.topLeft(),option.rect.topRight());
    painter->drawLine(option.rect.bottomLeft(),option.rect.bottomRight());  //底部线
    painter->setPen(oldPen);
    painter->restore();
}

/*
 * 代理类的编辑事件，实现第一列被点击时复选框能够实现勾选或者取消勾选
*/
bool MemSummaryDelegate::editorEvent(QEvent *event, QAbstractItemModel *model,\
                              const QStyleOptionViewItem &option,const QModelIndex &index)
{
    if (index.column() != 0)
          return QItemDelegate::editorEvent(event, model, option, index);

      if (event->type() == QEvent::MouseButtonPress) {
          int id = index.data().toInt();
          if (index.model()->data(index.model()->index(index.row(), 3), Qt::DisplayRole).toInt() != SQL_Record_Status_Recording)
          {
            if (!s_displayOrEventChange)
                s_displayStatusMap[id] = !s_displayStatusMap.value(id);
            else
              s_eventStatusMap[id] = !s_eventStatusMap.value(id);
          }
          //bool checked = statusmap.value(row);
          //qDebug() << checked << "edit";
          //bool checked = index.model()->data(index, Qt::DisplayRole).toBool();
          //return model->setData(index, !checked, Qt::CheckStateRole);
      }

      return true;
}


bool MemSummaryDelegate::helpEvent(QHelpEvent */*event*/,
                                     QAbstractItemView */*view*/,
                                     const QStyleOptionViewItem &/*option*/,
                                     const QModelIndex &/*index*/)
{
    return false;
}


