#include "winsummarydialog.h"
#include <QMessageBox>
#include "./Threads/threaddatasave.h"
#include "./Threads/threaddatacomm.h"
#include "./GlobalData/cfgGlobalDef_Type.h"
#include <QImage>
#include <QDebug>
#include <QModelIndex>
#include "./Threads/threaddatarecord.h"
#include "./WorkWin/wintrendgrouppanel.h"
#include "./WorkWin/wininfolistdialog.h"

void getImageAndStatus(const int type, QString &str);
QString getTimeFormate();
QString getSysTimeFormate(int width = DESKTOP_WIDTH, int height = DESKTOP_HEIGHT);


/*
 *
*/
WinSummaryDialog::WinSummaryDialog(Summary_Dialog_Type type, QModelIndex index, bool flag, QWidget */*parent*/)
    : WinDialog()
{
    p_query = new QSqlQuery(*ThreadDataRecord::instance()->getDatabaseConnect());
    ThreadDataRecord::instance()->putDatabaseConnect();
    QFrame *p_mainFrame = new QFrame(this);
    p_mainFrame->setObjectName("BackgrounFrame");
    p_mainFrame->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);

    WinStackedLayout *layout = new WinStackedLayout(this);
    layout->addWidget(p_mainFrame,Qt::AlignCenter);
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(0);

    m_closeBtn.setParent(p_mainFrame);
    m_closeBtn.setObjectName("CloseButton");
    m_closeBtn.setFocusPolicy(Qt::NoFocus);
    connect(&m_closeBtn, SIGNAL(clicked()),this,SLOT(close()));
    m_titleLabel.setParent(p_mainFrame);
    m_titleLabel.setObjectName("TitleLabel");

    QHBoxLayout * titleLayout = new QHBoxLayout();
    titleLayout->setContentsMargins(12,0,12,0);
    titleLayout->setSpacing(0);
    titleLayout->addWidget(&m_titleLabel,0,Qt::AlignVCenter);
    titleLayout->addSpacerItem(new QSpacerItem(40,59,QSizePolicy::Expanding,QSizePolicy::Fixed));
    titleLayout->addWidget(&m_closeBtn,0,Qt::AlignVCenter);

    m_toDispTrend.setParent(this);
    m_toDispTrend.setObjectName("m_toDispTrend");
    m_toDispTrend.setFocusPolicy(Qt::NoFocus);
    m_toEventTrend.setParent(this);
    m_toEventTrend.setObjectName("m_toEventTrend");
    m_toEventTrend.setFocusPolicy(Qt::NoFocus);

    p_contextLayout = new QVBoxLayout(p_mainFrame);
    p_contextLayout->setContentsMargins(0,0,0,20);
    p_contextLayout->setSpacing(15);

    p_contextLayout->addLayout(titleLayout);
    initData(type, index, flag);

    p_mainFrame->setStyleSheet("QFrame#BackgrounFrame{\
                               min-width:722px;\
                               max-width:722px;\
                               border: 1px solid #D3D3D3;\
                               background-color:rgb(255,255,255);\
                               background-image:url(:/relay/Image/relay/title_bg.png);\
                               background-repeat:repeat-x;\
                               background-position: top left;\
                               background-origin: content;\
                               border-top-left-radius:5px;\
                               border-top-right-radius:5px;\
                               }\
                        QPushButton#CloseButton{\
                            min-width:34px;\
                            max-width:34px;\
                            width:34px;\
                            height:34px;\
                            background-image:url(:/relay/Image/relay/close_normal.png);\
                            background-repeat: no-repeat;\
                            background-position:center;\
                            border:none;\
                        }\
                        QPushButton#CloseButton:pressed{\
                            background-image:url(:/relay/Image/relay/close_pressed.png);\
                        }\
                        QLabel#TitleLabel{\
                            color:rgb(255,255,255);\
                            font-size:32px;\
                        }\
                        QPushButton#m_toDispTrend,\
                        QPushButton#m_toEventTrend{\
                            width:124px;\
                            height:52px;\
                            font-size:16px;\
                            color:white;\
                            background-color:none;\
                            background-image:url(:/relay/Image/relay/button_normal.png);\
                            background-repeat:no-repeat;\
                            background-position:center;\
                            border:none;}\
                        ");
                        this->show();
}

WinSummaryDialog::~WinSummaryDialog()
{
    delete p_contextLayout;
    delete p_query;
}



/**
 * @brief WinSummaryDialog::initData
 * @param type : 弹出框类型（报警一览/信息一览/内存一览/modbus客户端/报表弹出框/数据保存）
 * @param index : 填充弹出框的数据模型索引
 * @param flag :    标志： 1) 该标志用在modbus客户端中，  flag = true ：表示界面为一行一行的显示数据   = false时表示界面是网格显示数据
 *                        2) 该标志用在数据保存弹出框时，flag = true 表示是保存显示数据，  = false表示保存事件数据
 */
void WinSummaryDialog::initData(Summary_Dialog_Type type, QModelIndex index, bool flag)
{
    switch (type) {
    case Summary_Alarm_Type:
    {
        WinSummaryAlarmDialog *alarmDlg = new WinSummaryAlarmDialog(index, this);
        alarmDlg->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
        p_contextLayout->addWidget(alarmDlg);
        setBtnAndTitleLabel(tr("Alarm summary"));
        m_time = index.model()->index(index.row(), 4).data().toLongLong();

    }
        break;
    case Summary_Mem_Type:
    {
        WinSummaryMemDialog *memDlg = new WinSummaryMemDialog(index, this);
        memDlg->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
        p_contextLayout->addWidget(memDlg);
        setBtnAndTitleLabel(tr("Memory summary"));
        m_time = index.model()->index(index.row(), 2).data().toLongLong();

    }
        break;
    case Summary_Message_Type:
    {
        WinSummaryMessageDialog *messageDlg = new WinSummaryMessageDialog(index, this);
        messageDlg->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
        p_contextLayout->addWidget(messageDlg);
        setBtnAndTitleLabel(tr("Message summary"));
        m_time = index.model()->index(index.row(), 1).data().toLongLong();
    }
        break;
    case Summary_ModbusClient_Type:
    {
        WinSummaryModbusClientDialog *modbusDlg = new WinSummaryModbusClientDialog(index, flag, this);
        modbusDlg->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
        p_contextLayout->addWidget(modbusDlg);
        QHBoxLayout * bottomLayout = new QHBoxLayout();
        bottomLayout->setContentsMargins(12,0,12,0);
        bottomLayout->setSpacing(0);

        bottomLayout->addWidget(&m_toDispTrend,0,Qt::AlignCenter);
        p_contextLayout->addLayout(bottomLayout);

        m_titleLabel.setText(tr("Modbus client"));
        m_toDispTrend.setText(tr("Refresh"));
        m_toEventTrend.hide();
        connect(&m_toDispTrend, SIGNAL(clicked()), this, SLOT(slotRefresh()));
    }
        break;
    case Summary_Report_Type:
    {
        WinSummaryReportDialog *reportDlg = new WinSummaryReportDialog(index, this);
        reportDlg->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
        p_contextLayout->addWidget(reportDlg);
        setBtnAndTitleLabel(tr("Report summary"));
        m_toEventTrend.hide();
        m_toDispTrend.hide();
    }
        break;
    default:
        break;
    }
}

/*
 * 跳转到显示趋势（报警一览/信息一览/内存一览 公用)
*/
void WinSummaryDialog::slotDisplayTrend()
{
    switchToTrend(true, "table_main_disp");
}

/*
 * 跳转到事件趋势（报警一览/信息一览/内存一览 公用)
*/
void WinSummaryDialog::slotEventTrend()
{
    switchToTrend(false, "table_main_event");
}

/*
 * 刷新，modbus专用
*/
void WinSummaryDialog::slotRefresh()
{
//    ThreadDataComm::instance()->tcpclient->handleCmd();
    QMessageBox::information(this,tr("Refresh"), tr("Refresh"));
}







/**
 * @brief WinSummaryDialog::switchToTrend  跳转到趋势
 * @param isDisp    true：跳转到显示趋势  false：跳转到事件趋势
 * @param tableName  数据库名称
 */
void WinSummaryDialog::switchToTrend(bool isDisp, QString tableName)
{
    p_query->clear();
    p_query->prepare(QString("select ID from %1 where StartTime <= ? and ? <= EndTime").arg(tableName));
    p_query->addBindValue(m_time);
    p_query->addBindValue(m_time);
    p_query->exec();

    if (p_query->next())
    {
        WinTrendGroupPanel * groupPanel = WinMainFrame::instance()->getTrendGroup();
        this->close();
        groupPanel->switchToHistoryReview(isDisp, p_query->value(0).toInt(), m_time);
        WinMainFrame::instance()->slotFrameChanged(Win_Trend_Panel);
    }
    else
    {
        //切换父窗体，然后阻塞，解决弹出错误提示框在本窗体后面的bug
        WinInfoListDialog::instance()->setParent(this);
        WinInfoListDialog::instance()->showMsg(532);
        WinInfoListDialog::instance()->exec();
        WinInfoListDialog::instance()->setParent(WinMainFrame::instance());
    }
}

/*
 * 在内存一览/报警一览/信息一览情况下设置跳转趋势按钮和跳转事件按钮样式和功能
*/
void WinSummaryDialog::setBtnAndTitleLabel(QString title)
{
    QHBoxLayout * bottomLayout = new QHBoxLayout();
    bottomLayout->setContentsMargins(12,0,12,0);
    bottomLayout->setSpacing(0);
    bottomLayout->addSpacerItem(new QSpacerItem(40,59,QSizePolicy::Expanding,QSizePolicy::Fixed));
    bottomLayout->addWidget(&m_toDispTrend,0,Qt::AlignVCenter);
    bottomLayout->addSpacerItem(new QSpacerItem(40,59,QSizePolicy::Expanding,QSizePolicy::Fixed));
    bottomLayout->addWidget(&m_toEventTrend,0,Qt::AlignVCenter);
    bottomLayout->addSpacerItem(new QSpacerItem(40,59,QSizePolicy::Expanding,QSizePolicy::Fixed));

    p_contextLayout->addLayout(bottomLayout);

    m_titleLabel.setText(title);
    m_toDispTrend.setText(tr("To display trend"));
    m_toEventTrend.setText(tr("To event trend"));
    connect(&m_toDispTrend, SIGNAL(clicked()), this, SLOT(slotDisplayTrend()));
    connect(&m_toEventTrend, SIGNAL(clicked()), this, SLOT(slotEventTrend()));
}







/*
 * 报警一览界面弹出框
*/
WinSummaryAlarmDialog::WinSummaryAlarmDialog(QModelIndex index, QWidget *parent) : QWidget(parent)
{
    QLabel *statusPromptLabel = new QLabel(this);
    statusPromptLabel->setObjectName("statusPromptLabel");
    statusPromptLabel->setText(tr("Status"));
    QLabel *statusLabel = new QLabel(this);
    statusLabel->setObjectName("statusLabel");

    QLabel *tagNoPromptLabel = new QLabel(this);
    tagNoPromptLabel->setObjectName("tagNoPromptLabel");
    tagNoPromptLabel->setText(tr("Tag Number"));
    QLabel *tagNoLabel = new QLabel(this);
    tagNoLabel->setObjectName("tagNoLabel");

    QLabel *tagPromptLabel = new QLabel(this);
    tagPromptLabel->setObjectName("tagPromptLabel");
    tagPromptLabel->setText(tr("Tag"));
    QLabel *tagLabel = new QLabel(this);
    tagLabel->setObjectName("tagLabel");

    QLabel *channelPromptLabel = new QLabel(this);
    channelPromptLabel->setObjectName("channelPromptLabel");
    channelPromptLabel->setText(tr("Channel number"));
    QLabel *channelLabel = new QLabel(this);
    channelLabel->setObjectName("channelLabel");

    QLabel *elecPromptLabel = new QLabel(this);
    elecPromptLabel->setObjectName("elecPromptLabel");
    elecPromptLabel->setText(tr("Electrical level"));
    QLabel *elecLabel = new QLabel(this);
    elecLabel->setObjectName("elecLabel");

    QLabel *typePromptLabel = new QLabel(this);
    typePromptLabel->setObjectName("typePromptLabel");
    typePromptLabel->setText(tr("Type"));
    QLabel *typeLabel = new QLabel(this);
    typeLabel->setObjectName("typeLabel");

    QLabel *timePromptLabel = new QLabel(this);
    timePromptLabel->setObjectName("timePromptLabel");
    timePromptLabel->setText(tr("Time"));
    QLabel *timeLabel = new QLabel(this);
    timeLabel->setObjectName("timeLabel");

    QFrame *line[4];
    for (int i = 0; i < 4; i++)
    {
        line[i] = new QFrame(this);
        line[i]->setObjectName("HLine");
        line[i]->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
        line[i]->setFrameShape(QFrame::HLine);
        line[i]->setFrameShadow(QFrame::Sunken);
    }

    QGridLayout *layout = new QGridLayout(this);
    layout->setContentsMargins(25,0,25,0);
    layout->setSpacing(15);
    layout->setColumnStretch(0,1);
    layout->setColumnStretch(1,1);
    layout->setColumnStretch(2,1);

    layout->addWidget(statusPromptLabel, 0, 0, 1, 1,Qt::AlignLeft);
    layout->addWidget(statusLabel, 0, 1, 1, 2,Qt::AlignLeft);
    layout->addWidget(line[0], 2, 0, 1, 3);

    layout->addWidget(tagNoPromptLabel, 3, 0, 1, 1,Qt::AlignLeft);
    layout->addWidget(tagNoLabel, 3, 1, 1, 2,Qt::AlignLeft);
    layout->addWidget(tagPromptLabel, 4, 0, 1, 1,Qt::AlignLeft);
    layout->addWidget(tagLabel, 4, 1, 1, 2,Qt::AlignLeft);
    layout->addWidget(line[1], 5, 0, 1, 3);

    layout->addWidget(channelPromptLabel, 6, 0, 1, 1,Qt::AlignLeft);
    layout->addWidget(channelLabel, 6, 1, 1, 2,Qt::AlignLeft);
    layout->addWidget(elecPromptLabel, 7, 0, 1, 1,Qt::AlignLeft);
    layout->addWidget(elecLabel, 7, 1, 1, 2,Qt::AlignLeft);
    layout->addWidget(typePromptLabel, 8, 0, 1, 1,Qt::AlignLeft);
    layout->addWidget(typeLabel, 8, 1, 1, 2,Qt::AlignLeft);
    layout->addWidget(line[2], 9, 0, 1, 3);

    layout->addWidget(timePromptLabel, 10, 0, 1, 1,Qt::AlignLeft);
    layout->addWidget(timeLabel, 10, 1, 1, 2,Qt::AlignLeft);
    layout->addWidget(line[3], 11, 0, 1, 3);

    this->setLayout(layout);

    int indexStatus = index.model()->index(index.row(), 0).data().toInt();
    statusLabel->setText(QString::number(indexStatus));

    QString tagNum = index.model()->index(index.row(), 1).data().toString();
    tagNoLabel->setText(tagNum);
    QString tagStr = index.model()->index(index.row(), 1).data().toString();
    tagLabel->setText(tagStr);

    QString indexChanel = index.model()->index(index.row(), 1).data().toString();
    channelLabel->setText(indexChanel);

    QString indexElec = index.model()->index(index.row(), 2).data().toString();
    elecLabel->setText(indexElec);

    int indexType = index.model()->index(index.row(), 3).data().toInt();
    QString typeName = getAlarmType(indexType);
    typeLabel->setText(typeName);

    QDateTime time;
    QString indexTime = index.model()->index(index.row(), 4).data().toString();
    QString timeValue =  time.fromMSecsSinceEpoch(indexTime.toLongLong()).toString(getTimeFormate());
    timeLabel->setText(timeValue);

    this->setStyleSheet("QLabel#statusPromptLabel,\
                        QLabel#tagNoPromptLabel,\
                        QLabel#tagPromptLabel,\
                        QLabel#channelPromptLabel,\
                        QLabel#elecPromptLabel,\
                        QLabel#typePromptLabel,\
                        QLabel#timePromptLabel{\
                            color:rgb(100,100,100);\
                            font-size:24px;\
                        }\
                        QLabel#statusLabel,\
                        QLabel#tagNoLabel,\
                        QLabel#tagLabel,\
                        QLabel#channelLabel,\
                        QLabel#elecLabel,\
                        QLabel#typeLabel,\
                        QLabel#timeLabel{\
                            color:black;\
                            font-size:24px;\
                        }");
}

//通过报警状态配置值得到状态值描述
QString WinSummaryAlarmDialog::getAlarmType(int type)
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
 * 信息一览弹出框详细内容
*/
WinSummaryMessageDialog::WinSummaryMessageDialog(QModelIndex index, QWidget *parent) : QWidget(parent)
{
    QLabel *messagePromptLable = new QLabel(this);
    messagePromptLable->setObjectName("messagePromptLable");
    messagePromptLable->setText(tr("Message"));
    QLabel *messageLabel = new QLabel(this);
    messageLabel->setObjectName("messageLabel");

    QLabel *timePromptLabel = new QLabel(this);
    timePromptLabel->setObjectName("timePromptLabel");
    timePromptLabel->setText(tr("Data time"));
    QLabel *timeLabel = new QLabel(this);
    timeLabel->setObjectName("timeLabel");

    QLabel *groupPromptLabel = new QLabel(this);
    groupPromptLabel->setObjectName("groupPromptLabel");
    groupPromptLabel->setText(tr("Group"));
    QLabel *groupLabel = new QLabel(this);
    groupLabel->setObjectName("groupLabel");

    QLabel *writePromptLabel = new QLabel(this);
    writePromptLabel->setObjectName("writePromptLabel");
    writePromptLabel->setText(tr("Write time"));
    QLabel *writelLabel = new QLabel(this);
    writelLabel->setObjectName("writelLabel");

    QFrame *line[2];
    for (int i = 0; i < 2; i++)
    {
        line[i] = new QFrame(this);
        line[i]->setObjectName("HLine");
        line[i]->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
        line[i]->setFrameShape(QFrame::HLine);
        line[i]->setFrameShadow(QFrame::Sunken);
    }

    QGridLayout *layout = new QGridLayout(this);
    layout->setContentsMargins(25,0,25,0);
    layout->setSpacing(15);
    layout->setColumnStretch(0,1);
    layout->setColumnStretch(1,1);
    layout->setColumnStretch(2,1);

    layout->addWidget(messagePromptLable, 0, 0, 1, 1,Qt::AlignLeft);
    layout->addWidget(messageLabel, 0, 1, 1, 2,Qt::AlignLeft);
    layout->addWidget(line[0], 2, 0, 1, 3);

    layout->addWidget(timePromptLabel, 3, 0, 1, 1,Qt::AlignLeft);
    layout->addWidget(timeLabel, 3, 1, 1, 2,Qt::AlignLeft);
    layout->addWidget(groupPromptLabel, 4, 0, 1, 1,Qt::AlignLeft);
    layout->addWidget(groupLabel, 4, 1, 1, 2,Qt::AlignLeft);
    layout->addWidget(writePromptLabel, 5, 0, 1, 1,Qt::AlignLeft);
    layout->addWidget(writelLabel, 5, 1, 1, 2,Qt::AlignLeft);
    layout->addWidget(line[1], 6, 0, 1, 3);

    this->setLayout(layout);

    QString indexMess = getMessageType(index.model()->index(index.row(), 0).data().toInt());
    messageLabel->setText(indexMess);

    QString indexTime = index.model()->index(index.row(), 1).data().toString();
    QString timeValue =  QDateTime::fromMSecsSinceEpoch(indexTime.toLongLong()).toString(getTimeFormate());
    timeLabel->setText(timeValue);
    writelLabel->setText(timeValue);

    int indexGroup = index.model()->index(index.row(), 2).data().toInt();
    groupLabel->setText(indexGroup == 0 ? tr("All") : QString::number(indexGroup));

    this->setStyleSheet("QLabel#messagePromptLable,\
                        QLabel#timePromptLabel,\
                        QLabel#groupPromptLabel,\
                        QLabel#writePromptLabel{\
                            color:rgb(100,100,100);\
                            font-size:24px;\
                        }\
                        QLabel#messageLabel,\
                        QLabel#timeLabel,\
                        QLabel#groupLabel,\
                        QLabel#writelLabel{\
                            color:black;\
                            font-size:24px;\
                        }");
}

QString WinSummaryMessageDialog::getMessageType(int type)
{
    if (type == Message_Type_Normal)
        return QString(QObject::tr("String message"));
    else if (type == Message_Type_Freehand)
        return QString("*" + QObject::tr("Freehand message") + "*");
    else
        return "";
}





/*
 * 内存一览弹出框详细内容
*/
WinSummaryMemDialog::WinSummaryMemDialog(QModelIndex index, QWidget *parent) : QWidget(parent)
{
    QLabel *startPromptLable = new QLabel(this);
    startPromptLable->setObjectName("startPromptLable");
    startPromptLable->setText(tr("Start time"));
    QLabel *startLabel = new QLabel(this);
    startLabel->setObjectName("startLabel");

    QLabel *stopPromptLabel = new QLabel(this);
    stopPromptLabel->setObjectName("stopPromptLabel");
    stopPromptLabel->setText(tr("Stop time"));
    QLabel *stopLabel = new QLabel(this);
    stopLabel->setObjectName("stopLabel");

    QLabel *filePromptLabel = new QLabel(this);
    filePromptLabel->setObjectName("filePromptLabel");
    filePromptLabel->setText(tr("File name"));
    QLabel *fileLabel = new QLabel(this);
    fileLabel->setObjectName("fileLabel");

    QFrame *line = new QFrame(this);
    line->setObjectName("HLine");
    line->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);

    QGridLayout *layout = new QGridLayout(this);
    layout->setContentsMargins(25,0,25,0);
    layout->setSpacing(15);
    layout->setColumnStretch(0,1);
    layout->setColumnStretch(1,1);
    layout->setColumnStretch(2,1);

    layout->addWidget(startPromptLable, 0, 0, 1, 1,Qt::AlignLeft);
    layout->addWidget(startLabel, 0, 1, 1, 2,Qt::AlignLeft);
    layout->addWidget(stopPromptLabel, 1, 0, 1, 1,Qt::AlignLeft);
    layout->addWidget(stopLabel, 1, 1, 1, 2,Qt::AlignLeft);
    layout->addWidget(filePromptLabel, 2, 0, 1, 1,Qt::AlignLeft);
    layout->addWidget(fileLabel, 2, 1, 1, 2,Qt::AlignLeft);
    layout->addWidget(line, 3, 0, 1, 3);

    this->setLayout(layout);
    QString timeFormate = getTimeFormate();

    int row = index.row();
    QString startTime = index.model()->index(row, 1).data().toString();
    startTime = QDateTime::fromMSecsSinceEpoch(startTime.toLongLong()).toString(timeFormate);
    startLabel->setText(startTime);

    QString stopTime = index.model()->index(row, 2).data().toString();
    stopTime = QDateTime::fromMSecsSinceEpoch(stopTime.toLongLong()).toString(timeFormate);
    stopLabel->setText(stopTime);

    QString fileName = index.model()->index(row, 5).data().toString();
    fileLabel->setText(fileName);

    this->setStyleSheet("QLabel#startPromptLable,\
                        QLabel#stopPromptLabel,\
                        QLabel#filePromptLabel{\
                            color:rgb(100,100,100);\
                            font-size:24px;\
                        }\
                        QLabel#startLabel,\
                        QLabel#stopLabel,\
                        QLabel#fileLabel{\
                            color:black;\
                            font-size:24px;\
                        }");
}




/*
 * modbus客户端一览弹出框详细内容
*/
WinSummaryModbusClientDialog::WinSummaryModbusClientDialog(QModelIndex index, bool flag, QWidget *parent) : QWidget(parent)
{
    QLabel *numPromptLabel = new QLabel(this);
    numPromptLabel->setObjectName("numPromptLabel");
    numPromptLabel->setText(tr("Number"));
    QLabel *numLabel = new QLabel(this);
    numLabel->setObjectName("numLabel");

    QLabel *cmdPromptLabel = new QLabel(this);
    cmdPromptLabel->setObjectName("cmdPromptLabel");
    cmdPromptLabel->setText(tr("Command"));
    QLabel *cmdLabel = new QLabel(this);
    cmdLabel->setObjectName("cmdLabel");

    QLabel *statusPromptLabel = new QLabel(this);
    statusPromptLabel->setObjectName("statusPromptLabel");
    statusPromptLabel->setText(tr("Status"));
    QLabel *statusLabel = new QLabel(this);
    statusLabel->setObjectName("statusLabel");

    QLabel *typePromptLabel = new QLabel(this);
    typePromptLabel->setObjectName("typePromptLabel");
    typePromptLabel->setText(tr("Type"));
    QLabel *typeLabel = new QLabel(this);
    typeLabel->setObjectName("typeLabel");

    QLabel *startChPromptLabel = new QLabel(this);
    startChPromptLabel->setObjectName("startChPromptLabel");
    startChPromptLabel->setText(tr("Start Channel"));
    QLabel *startChLabel = new QLabel(this);
    startChLabel->setObjectName("startChLabel");

    QLabel *stopChPromptLabel = new QLabel(this);
    stopChPromptLabel->setObjectName("stopChPromptLabel");
    stopChPromptLabel->setText(tr("Stop channel"));
    QLabel *stopChLabel = new QLabel(this);
    stopChLabel->setObjectName("stopChLabel");

    QLabel *serverPromptLabel = new QLabel(this);
    serverPromptLabel->setObjectName("serverPromptLabel");
    serverPromptLabel->setText(tr("Server Name"));
    QLabel *serverLabel = new QLabel(this);
    serverLabel->setObjectName("serverLabel");

    QLabel *registerPromptLabel = new QLabel(this);
    registerPromptLabel->setObjectName("registerPromptLabel");
    registerPromptLabel->setText(tr("Register"));
    QLabel *registerLabel = new QLabel(this);
    registerLabel->setObjectName("registerLabel");

    QLabel *unitPromptLabel = new QLabel(this);
    unitPromptLabel->setObjectName("unitPromptLabel");
    unitPromptLabel->setText(tr("Unit number"));
    QLabel *unitLabel = new QLabel(this);
    unitLabel->setObjectName("unitLabel");

    QFrame *line[3];
    for (int i = 0; i < 3; i++)
    {
        line[i] = new QFrame(this);
        line[i]->setObjectName("HLine");
        line[i]->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
        line[i]->setFrameShape(QFrame::HLine);
        line[i]->setFrameShadow(QFrame::Sunken);
    }

    QGridLayout *layout = new QGridLayout(this);
    layout->setContentsMargins(25,0,25,0);
    layout->setSpacing(15);
    layout->setColumnStretch(0,1);
    layout->setColumnStretch(1,1);
    layout->setColumnStretch(2,1);

    layout->addWidget(numPromptLabel, 0, 0, 1, 1,Qt::AlignLeft);
    layout->addWidget(numLabel, 0, 1, 1, 2,Qt::AlignLeft);
    layout->addWidget(cmdPromptLabel, 1, 0, 1, 1,Qt::AlignLeft);
    layout->addWidget(cmdLabel, 1, 1, 1, 2,Qt::AlignLeft);
    layout->addWidget(statusPromptLabel, 2, 0, 1, 1,Qt::AlignLeft);
    layout->addWidget(statusLabel, 2, 1, 1, 2,Qt::AlignLeft);
    layout->addWidget(typePromptLabel, 3, 0, 1, 1,Qt::AlignLeft);
    layout->addWidget(typeLabel, 3, 1, 1, 2,Qt::AlignLeft);
    layout->addWidget(line[0], 4, 0, 1, 3);

    layout->addWidget(startChPromptLabel, 5, 0, 1, 1,Qt::AlignLeft);
    layout->addWidget(startChLabel, 5, 1, 1, 2,Qt::AlignLeft);
    layout->addWidget(stopChPromptLabel, 6, 0, 1, 1,Qt::AlignLeft);
    layout->addWidget(stopChLabel, 6, 1, 1, 2,Qt::AlignLeft);
    layout->addWidget(line[1], 7, 0, 1, 3);

    layout->addWidget(serverPromptLabel, 8, 0, 1, 1,Qt::AlignLeft);
    layout->addWidget(serverLabel, 8, 1, 1, 2,Qt::AlignLeft);
    layout->addWidget(registerPromptLabel, 9, 0, 1, 1,Qt::AlignLeft);
    layout->addWidget(registerLabel, 9, 1, 1, 2,Qt::AlignLeft);
    layout->addWidget(unitPromptLabel, 10, 0, 1, 1,Qt::AlignLeft);
    layout->addWidget(unitLabel, 10, 1, 1, 2,Qt::AlignLeft);
    layout->addWidget(line[2], 11, 0, 1, 3);

    this->setLayout(layout);

    QString numStr, commandStr, statusStr, typeStr, channel, startChannel, stopChannel, serverName, registerStr, unitStr;
    if (flag)
    {
        numStr = index.model()->index(index.row(), 0).data().toString();
        commandStr = index.model()->index(index.row(), 1).data().toString() == "W" ? tr("write") : tr("read");
        getImageAndStatus(index.model()->index(index.row(), 2).data().toInt(), statusStr);

        channel = index.model()->index(index.row(), 3).data().toString();
        startChannel = channel.left(4);
        stopChannel = channel.right(4);

        serverName = index.model()->index(index.row(), 4).data().toString();
        registerStr = index.model()->index(index.row(), 5).data().toString();

        typeStr = getType(index.model()->index(index.row(), 6).data().toInt());
        unitStr = index.model()->index(index.row(), 7).data().toString();
    }
    else
    {
        QStringList modbus = index.model()->index(index.row(), index.column()).data().toString().split(",");
        numStr = modbus[0];
        commandStr = modbus[1] == "W" ? tr("write") : tr("read");
        getImageAndStatus(modbus[2].toInt(), statusStr);
        startChannel = modbus[3].left(4);
        stopChannel = modbus[3].right(4);
        serverName = modbus[4];
        registerStr = modbus[5];
        typeStr = getType(modbus[6].toInt());
        unitStr = modbus[7];
    }

    numLabel->setText(numStr);
    cmdLabel->setText(commandStr);
    statusLabel->setText(statusStr);
    typeLabel->setText(typeStr);
    startChLabel->setText(startChannel);
    stopChLabel->setText(stopChannel);
    serverLabel->setText(serverName);
    registerLabel->setText(registerStr);
    unitLabel->setText(unitStr);

    this->setStyleSheet("QLabel#numPromptLabel,\
                        QLabel#cmdPromptLabel,\
                        QLabel#statusPromptLabel,\
                        QLabel#typePromptLabel,\
                        QLabel#startChPromptLabel,\
                        QLabel#stopChPromptLabel,\
                        QLabel#serverPromptLabel,\
                        QLabel#registerPromptLabel, \
                        QLabel#unitPromptLabel{\
                            color:rgb(100,100,100);\
                            font-size:24px;\
                        }\
                        QLabel#numLabel,\
                        QLabel#cmdLabel,\
                        QLabel#statusLabel,\
                        QLabel#typeLabel,\
                        QLabel#startChLabel,\
                        QLabel#stopChLabel,\
                        QLabel#serverLabel,\
                        QLabel#registerLabel,\
                        QLabel#unitLabel{\
                            color:black;\
                            font-size:24px;\
                        }");
}

QString WinSummaryModbusClientDialog::getType(int type)
{
    switch (type) {
    case Register_Type_INT16:
        return "INIT16";
        break;
    case Register_Type_UINT16:
        return "UINIT16";
        break;
    case Register_Type_INT32B:
        return "INIT32_B";
        break;
    case Register_Type_INT32L:
        return "INIT32_L";
        break;
    case Register_Type_UINT32B:
        return "UINIT32_B";
        break;
    case Register_Type_UINT32L:
        return "UINIT32_L";
        break;
//    case Register_Type_FLOAT:
//        return "FLOAT";
//        break;
    case Register_Type_FLOATB:
        return "FLOAT_B";
        break;
    case Register_Type_FLOATL:
        return "FLOAT_L";
        break;
    case Register_Type_BIT:
        return "BIT";
        break;

    default:
        break;
    }
    return "";
}



/*
 * 报表一览弹出框详细内容
*/
WinSummaryReportDialog::WinSummaryReportDialog(QModelIndex index, QWidget *parent) : QWidget(parent)
{
    QLabel *tagNoPromptLabel = new QLabel(this);
    tagNoPromptLabel->setObjectName("tagNoPromptLabel");
    tagNoPromptLabel->setText(tr("Tag No"));
    QLabel *tagNoLabel = new QLabel(this);
    tagNoLabel->setObjectName("tagNoLabel");

    QLabel *tagPromptLabel = new QLabel(this);
    tagPromptLabel->setObjectName("tagPromptLabel");
    tagPromptLabel->setText(tr("Tag"));
    QLabel *tagLabel = new QLabel(this);
    tagLabel->setObjectName("tagLabel");

    QLabel *channelPromptLabel = new QLabel(this);
    channelPromptLabel->setObjectName("channelPromptLabel");
    channelPromptLabel->setText(tr("Channel"));
    QLabel *channelLabel = new QLabel(this);
    channelLabel->setObjectName("channelLabel");

    QString channelStr = index.model()->index(index.row(), 0).data().toString();
//    QString tagNoStr = index.model()->index(index.row(), 9).data().toString();
    QStringList tagStr = index.model()->index(index.row(), 8).data().toString().split("|");

    tagNoLabel->setText(tagStr.value(1));
    tagLabel->setText(tagStr.value(0));
    channelLabel->setText(channelStr);

    QGridLayout *layout = new QGridLayout(this);
    layout->setContentsMargins(25,0,25,0);
    layout->setSpacing(15);

    layout->addWidget(tagNoPromptLabel, 0, 0, 1, 1,Qt::AlignLeft);
    layout->addWidget(tagNoLabel, 0, 1, 1, 2,Qt::AlignLeft);
    layout->addWidget(tagPromptLabel, 1, 0, 1, 1,Qt::AlignLeft);
    layout->addWidget(tagLabel, 1, 1, 1, 2,Qt::AlignLeft);
    layout->addWidget(channelPromptLabel, 2, 0, 1, 1,Qt::AlignLeft);
    layout->addWidget(channelLabel, 2, 1, 1, 2,Qt::AlignLeft);

    this->setStyleSheet("QLabel#tagNoPromptLabel,\
                        QLabel#tagPromptLabel,\
                        QLabel#channelPromptLabel{\
                            color:rgb(100,100,100);\
                            font-size:24px;\
                        }\
                        QLabel#tagNoLabel,\
                        QLabel#tagLabel,\
                        QLabel#channelLabel{\
                            color:black;\
                            font-size:24px;\
                        }");
}




/*
 * 保存内存一览数据的弹出框
*/
WinSummarySaveDialog *WinSummarySaveDialog::instance()
{
    static WinSummarySaveDialog saveDlg;
    return &saveDlg;
}


WinSummarySaveDialog::WinSummarySaveDialog(QWidget */*parent*/) : WinDialog(), m_isSD(true)
{
    initSaveDialog();
}

void WinSummarySaveDialog::initSaveDialog()
{
    QFrame *p_mainFrame = new QFrame(this);
    p_mainFrame->setObjectName("BackgrounFrame");
    p_mainFrame->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);

    WinStackedLayout *layout = new WinStackedLayout(this);
    layout->addWidget(p_mainFrame,Qt::AlignCenter);
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(0);

    QPushButton *closeBtn = new QPushButton(p_mainFrame);
    closeBtn->setObjectName("CloseButton");
    closeBtn->setFocusPolicy(Qt::NoFocus);
    connect(closeBtn, SIGNAL(clicked()),this,SLOT(close()));
    QLabel *titleLabel = new QLabel(p_mainFrame);
    titleLabel->setObjectName("TitleLabel");
    titleLabel->setText(tr("Select save"));
    QHBoxLayout * titleLayout = new QHBoxLayout();
    titleLayout->setContentsMargins(12,0,12,0);
    titleLayout->setSpacing(0);
    titleLayout->addWidget(titleLabel,0,Qt::AlignVCenter);
    titleLayout->addSpacerItem(new QSpacerItem(40,59,QSizePolicy::Expanding,QSizePolicy::Fixed));
    titleLayout->addWidget(closeBtn,0,Qt::AlignVCenter);

    QLabel *saveLabel = new QLabel(this);
    saveLabel->setObjectName("saveLabel");
    saveLabel->setText(tr("Save to : "));

    m_USBBtn.setParent(p_mainFrame);
    m_USBBtn.setObjectName("USBButton");
    m_USBBtn.setFocusPolicy(Qt::NoFocus);
    m_USBBtn.setText(tr("USB"));
    m_USBBtn.setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    m_SDBtn.setParent(p_mainFrame);
    m_SDBtn.setObjectName("SDButton");
    m_SDBtn.setFocusPolicy(Qt::NoFocus);
    m_SDBtn.setText(tr("SD"));
    m_SDBtn.setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    QHBoxLayout * btnLayout = new QHBoxLayout();
    btnLayout->setContentsMargins(24,0,24,0);
    btnLayout->setSpacing(0);
    btnLayout->addWidget(saveLabel);
    btnLayout->addWidget(&m_USBBtn);
    btnLayout->addWidget(&m_SDBtn);
    connect(&m_SDBtn, SIGNAL(clicked()),this,SLOT(slotChooseBtnClicked()));
    connect(&m_USBBtn, SIGNAL(clicked()),this,SLOT(slotChooseBtnClicked()));

    m_bar.setParent(this);
    m_bar.setObjectName("bar");
    m_bar.setValue(0);
    QHBoxLayout *barLayout = new QHBoxLayout;
    barLayout->addSpacerItem(new QSpacerItem(40,40,QSizePolicy::Maximum,QSizePolicy::Expanding));
    barLayout->addWidget(&m_bar);
    barLayout->addSpacerItem(new QSpacerItem(40,40,QSizePolicy::Maximum,QSizePolicy::Expanding));
    m_bar.hide();

    m_cancleBtn.setParent(p_mainFrame);
    m_cancleBtn.setObjectName("CancelButton");
    m_cancleBtn.setText(tr("Cancel"));
    m_cancleBtn.setFocusPolicy(Qt::NoFocus);
    m_cancleBtn.setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    connect(&m_cancleBtn, SIGNAL(clicked()),this,SLOT(close()));

    m_okBtn.setParent(p_mainFrame);
    m_okBtn.setObjectName("OkButton");
    m_okBtn.setText(tr("OK"));
    m_okBtn.setFocusPolicy(Qt::NoFocus);
    m_okBtn.setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    connect(&m_okBtn, SIGNAL(clicked()),&m_bar,SLOT(show()));

    QHBoxLayout *bottomLayout = new QHBoxLayout;
    bottomLayout->setSpacing(10);
    bottomLayout->setContentsMargins(40,10,24,0);
    bottomLayout->addWidget(&m_cancleBtn,0, Qt::AlignLeft);
    bottomLayout->addStretch();
    bottomLayout->addWidget(&m_okBtn,1, Qt::AlignRight);

    QLabel *txtLabel = new QLabel(p_mainFrame);
    txtLabel->setObjectName("TextLabel");
    txtLabel->setText(tr("Save select data ? "));

    QVBoxLayout * contextLayout = new QVBoxLayout(p_mainFrame);
    contextLayout->setContentsMargins(0,0,0,20);
    contextLayout->setSpacing(30);
    contextLayout->addLayout(titleLayout);
    contextLayout->addWidget(txtLabel);
    contextLayout->addLayout(btnLayout);
    contextLayout->addLayout(barLayout);
    contextLayout->addLayout(bottomLayout);

    p_mainFrame->setStyleSheet("QFrame#BackgrounFrame{\
                        min-width:722px;\
                        max-width:722px;\
                        border: 1px solid #D3D3D3;\
                        background-color:rgb(255,255,255);\
                        background-image:url(:/relay/Image/relay/title_bg.png);\
                        background-repeat:repeat-x;\
                        background-position: top left;\
                        background-origin: content;\
                        border-top-left-radius:5px;\
                        border-top-right-radius:5px;\
                        }\
                        QPushButton#CloseButton{\
                        min-width:34px;\
                        max-width:34px;\
                        width:34px;\
                        height:34px;\
                        background-image:url(:/relay/Image/relay/close_normal.png);\
                        background-repeat: no-repeat;\
                        background-position:center;\
                        border:none;\
                        }\
                        QPushButton#CloseButton:pressed{\
                        background-image:url(:/relay/Image/relay/close_pressed.png);\
                        }\
                        QLabel#TitleLabel{\
                        color:rgb(255,255,255);\
                        font-size:32px;\
                        }\
                        QLabel#TextLabel,\
                        QLabel#saveLabel{\
                            color:black;\
                            margin-top:20px;\
                            margin-bottom:20px;\
                            margin-left:20px;\
                            font-size:28px;\
                            min-width:150px;\
                            }\
                        QPushButton#OkButton,\
                        QPushButton#CancelButton{\
                            background-color:rgb(0,0,0,0);\
                            max-width:150px;min-width:150px;\
                            border:1px solid gray;font-size:16px;\
                            min-height:50px;}\
                        QPushButton#USBButton,\
                        QPushButton#SDButton{\
                        min-height:100px;\
                        font-size:32px;\
                        border: 1px solid #D3D3D3;\
                        }\
                        QPushButton#SDButton:pressed,\
                        QPushButton#USBButton:pressed\
                        {background-color:rgb(0,187,255,100%);}\
                        QProgressBar::chunk {\
                            background-color: rgb(0,187,255);\
                            width: 10px;\
                            margin: 0.5px;}\
                        QProgressBar {\
                            max-width: 722px;\
                            border: 2px solid grey;\
                            border-radius: 5px;\
                            text-align: center;\
                            font: 18px;\
                        }A margin can be included to obtain more visible chunks.");
}

void WinSummarySaveDialog::slotChooseBtnClicked()
{
    QPushButton *btn = (QPushButton *)sender();
    if (btn->objectName() == tr("SDButton"))
    {
        m_isSD = true;
        m_SDBtn.setStyleSheet("QPushButton#SDButton{background-color:rgb(0,178,255,30%);}QPushButton#SDButton:pressed{background-color:rgb(0,178,255,100%);}");
        m_USBBtn.setStyleSheet("background-color:rgb(0,0,0,0%);");
    }
    else
    {
        m_isSD = false;
        m_USBBtn.setStyleSheet("QPushButton#USBButton{background-color:rgb(0,178,255,30%);}QPushButton#USBButton:pressed{background-color:rgb(0,178,255,100%);}");
        m_SDBtn.setStyleSheet("background-color:rgb(0,0,0,0%);");
    }
}


void WinSummarySaveDialog::showMsg(quint32 range)
{
    m_bar.setValue(0);
    setBarRange(range);
    setButtonStyle();
    this->show();
}

void WinSummarySaveDialog::setBarRange(quint32 range)
{
    m_bar.setRange(0, range);
}

// SD/USB存在时样式和不存在时样式
void WinSummarySaveDialog::setButtonStyle()
{
    if (!m_isSD)     //TODO  改成判断USB是否存在的接口函数
    {
        m_USBBtn.setStyleSheet("QPushButton#USBButton{background-color:rgb(0,178,255,30%);}QPushButton#SDButton:pressed{background-color:rgb(0,178,255,100%);}");
        m_USBBtn.setEnabled(true);
    }
    else
    {
        m_USBBtn.setStyleSheet("background-color:rgb(0,0,0,0%);");
        m_USBBtn.setEnabled(false);
    }

    if (m_isSD)     //TODO  改成判断SD是否存在的接口函数
    {
        m_SDBtn.setStyleSheet("QPushButton#SDButton{background-color:rgb(0,178,255,30%);}QPushButton#SDButton:pressed{background-color:rgb(0,178,255,100%);}");
        m_SDBtn.setEnabled(true);
    }
    else
    {
        m_SDBtn.setStyleSheet("QPushButton#SDButton{background-color:rgb(0,0,0,0%);}");
        m_SDBtn.setEnabled(false);
    }
}

void WinSummarySaveDialog::setBarValue(quint32 value)
{
    m_bar.setValue(value);
}




static QImage m_imgConnect(":/summary/Image/summary/log_device.png");
static QImage m_imgClose(":/summary/Image/summary/log_negative.png"); //hongse
static QImage m_imgBusy(":/summary/Image/summary/log_busy.png"); //chengse
static QImage m_imgAck(":/summary/Image/summary/log_ack.png");   //huangse

/*
 * modbus客户端日志界面图片
*/
void getImageAndStatus(const int type, QString &str)
{
    switch (type) {
    case ModbusStatus_Valid:    //已正常建立Modbus 通信。
        str = "Connect";
        break;

    case ModbusStatus_Start:    //Modbus 通信已开始。 START
        str = "START";
        break;
    case ModbusStatus_Stop:     //Modbus 通信已停止。 STOP
        str = "STOP";
        break;
    case ModbusStatus_Dropout:  //不能在指定通信周期内发送全部命令。请重新检查Modbus 命令数和波特率等设定。 DROPOUT
        str = "DROPOUT";
        break;

    case ModbusStatus_Invalid:   //不能执行命令。
        str = "INVALID";
        break;
    case ModbusStatus_Waiting:    //与外部设备的通信受阻，等待恢复。请检查与外部设备的连接。
        str = "WAITING";
        break;
    case ModbusStatus_CLosed:    //Modbus 通信已停止，与外部设备的连接已关闭。
        str = "CLOSED";
        break;
    case ModbusStatus_Resolving:  //正在建立与服务器/ 从机的连接( 地址解析中)。
        str = "RESOLVING";
        break;
    case ModbusStatus_Connecting:   //正在建立与服务器/ 从机的连接( 连接请求中)
        str = "CONNNECTING";
        break;
    case ModbusStatus_Unreach:    //网络中找不到外部设备。请检查Modbus 服务器的地址设定及以太网电缆的连接。
        str = "UNREACH";
        break;
    case ModbusStatus_Timeout:    //等待外部设备的响应超时。请检查与外部设备的连接
        str = "TIMEOUT";
        break;
    case ModbusStatus_Nodata:  //还未曾取得过数据。请检查通信设置。
        str = "NO_DATA";
        break;

    case ModbusStatus_ErrFc:    //Modbus 通信已停止，与外部设备的连接已关闭。
        str = "ERR_FC";
        break;
    case ModbusStatus_ErrAddr:  //正在建立与服务器/ 从机的连接( 地址解析中)。
        str = "ERR_ADDR";
        break;
    case ModbusStatus_ErrValue:   //正在建立与服务器/ 从机的连接( 连接请求中)
        str = "ERR_VALUE";
        break;
    case ModbusStatus_ErrDevice:    //网络中找不到外部设备。请检查Modbus 服务器的地址设定及以太网电缆的连接。
        str = "ERR_DEVICE";
        break;
    case ModbusStatus_Ack:    //等待外部设备的响应超时。请检查与外部设备的连接
        str = "ERR_ACK";
        break;
    case ModbusStatus_Busy:  //还未曾取得过数据。请检查通信设置。
        str = "ERR_BUSY";
        break;
    default:
        str = "Unknown Status";
        break;
    }
}


QString getTimeFormate()
{
    return QDateTime::currentDateTime().toString(CfgChannel::instance()->getSysDateFormat()).append(" hh:mm:ss");
}







QString getSysTimeFormate(int width, int height)
{
    QString fmt = "%1%4%2%4%3 %5";
    QString mm = "MM";
    QString deli = "/";
    QString ret = "";
    QString yy;

    if (width > DESKTOP_WIDTH * 0.60)
        yy = "yyyy";
    else if (width > DESKTOP_WIDTH * 0.35 && width <= DESKTOP_WIDTH * 0.60)
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
    if (width < DESKTOP_WIDTH * 0.35)
        ret = ret.right(8);
    else
    {
        if (height < DESKTOP_HEIGHT * 0.35)
            ret = ret.right(8);
    }

    return ret;
}
