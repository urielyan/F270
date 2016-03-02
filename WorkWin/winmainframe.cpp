#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDateTime>
#include <QScreen>
#include "winmainframe.h"
#include "winackalarmdialog.h"
#include "winmediadialog.h"
#include "winmathdialog.h"
#include "winrecorddialog.h"
#include "winstandarddialog.h"
#include "winsnapshotdialog.h"
#include "./GlobalData/globalDef.h"
#include "./GlobalData/cfgGlobalDef.h"
#include "./GlobalData/cfgchannel.h"
#include "./Common/eventaction.h"
#include "./Threads/threaddatasync.h"
#include "./Threads/threaddataprocess.h"
#include "./Common/authorization.h"

#include "wintrendgrouppanel.h"
#include "windigitalpanel.h"
#include "winbarpanel.h"
#include "winoverview.h"
#include "winalarmsummary.h"
#include "winmemsummary.h"
#include "winmessagesummary.h"
#include "winmodbusclient.h"
#include "winmultipanel.h"
#include "winrelayandsw.h"
#include "winreport.h"
#include "winnetworkinfo.h"
#include "winroot.h"
#include "winlog.h"
#include "winsaveload.h"
#include "wincolibrationpanel.h"
#include "winsysteminfo.h"

WinMainStatus::WinMainStatus(QWidget *parent) :
    QFrame(parent)
{
    //设置状态栏高度
    setFixedHeight(DESKTOP_HEIGHT/MAINWIN_STATUSBAR_HIGHT_SCALE);
    //setFixedHeight(65);
    setAutoFillBackground(true);
    setLayout(&m_headLayout);

    //状态栏整体布局
    m_headLayout.setSpacing(6);
    m_headLayout.setContentsMargins(9,0,9,0);
    m_headLayout.addWidget(&m_iconLabel,30,Qt::AlignVCenter);
    m_headLayout.addWidget(&m_titleLabel,0,Qt::AlignVCenter);
    m_headLayout.addSpacerItem(new QSpacerItem(40,40,QSizePolicy::Expanding,QSizePolicy::Fixed));
    m_headLayout.addWidget(&m_homeBtn,0,Qt::AlignVCenter);
    m_headLayout.addWidget(&m_funcBtn,0,Qt::AlignVCenter);
    m_headLayout.addWidget(&m_recordBtn,0,Qt::AlignVCenter);
    m_headLayout.addWidget(&m_alertBtn,0,Qt::AlignVCenter);
    m_headLayout.addWidget(&m_prtScBtn,0,Qt::AlignVCenter);
    m_headLayout.addWidget(&m_memBtn,0,Qt::AlignVCenter);
    m_headLayout.addWidget(&m_mathBtn,0,Qt::AlignVCenter);
    m_headLayout.addWidget(&m_statuesFrame,0,Qt::AlignVCenter);
    m_headLayout.addWidget(&m_timeFrame,0,Qt::AlignVCenter);

    //左侧标题
    m_iconLabel.setObjectName("IconLabel");
    m_iconLabel.setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    m_titleLabel.setObjectName("TitleLabel");

    //截屏按钮
    m_prtScBtn.setObjectName("PrintScreenButton");
    QPixmap icon(":/frame/Image/frame/prtsc_button.png");
    m_prtScBtn.setIcon(QIcon(icon));
    m_prtScBtn.setIconSize(QSize(32,27));
    m_prtScBtn.setFocusPolicy(Qt::NoFocus);
    connect(&m_prtScBtn,SIGNAL(clicked()),this,SLOT(slotPrintScreenButtonClicked()));

    //USB移除按钮
    m_memBtn.setObjectName("MemoryButton");
     icon = QPixmap(":/frame/Image/frame/mem_button.png");
    m_memBtn.setIcon(QIcon(icon));
    m_memBtn.setIconSize(icon.size());
    m_memBtn.setFocusPolicy(Qt::NoFocus);
    connect(&m_memBtn,SIGNAL(clicked()),this,SLOT(slotMemButtonClicked()));

    //运算按钮
    icon = QPixmap(":/frame/Image/frame/math_button.png");
    m_mathBtn.setIcon(QIcon(icon));
    m_mathBtn.setIconSize(icon.size());
    m_mathBtn.setObjectName("MathButton");
    m_mathBtn.setFocusPolicy(Qt::NoFocus);
    connect(&m_mathBtn,SIGNAL(clicked()),this,SLOT(slotMathButtonClicked()));

    //解除警报按钮
    icon = QPixmap(":/frame/Image/frame/alarm_button.png");
    m_alertBtn.setIcon(QIcon(icon));
    m_alertBtn.setIconSize(icon.size());
    m_alertBtn.setObjectName("AlertButton");
    m_alertBtn.setFocusPolicy(Qt::NoFocus);
    connect(&m_alertBtn,SIGNAL(clicked()),this,SLOT(slotAlertButtonClicked()));

    //记录按钮
    icon = QPixmap(":/frame/Image/frame/record_button.png");
    m_recordBtn.setIcon(QIcon(icon));
    m_recordBtn.setIconSize(icon.size());
    m_recordBtn.setObjectName("RecordButton");
    m_recordBtn.setFocusPolicy(Qt::NoFocus);
    connect(&m_recordBtn,SIGNAL(clicked()),this,SLOT(slotRecordButtonClicked()));

    //home按钮需要根据按压时间执行不同动作
    icon = QPixmap(":/frame/Image/frame/home_button.png");
    m_homeBtn.setIcon(QIcon(icon));
    m_homeBtn.setIconSize(icon.size());
    m_homeBtn.setObjectName("HomeButton");
    m_homeBtn.setFocusPolicy(Qt::NoFocus);
    connect(&m_homeBtn,SIGNAL(pressed()),this,SLOT(slotHomeButtonPressed()));
    connect(&m_homeBtn,SIGNAL(released()),this,SLOT(slotHomeButtonReleased()));

    //功能按钮根据不同模块控制不同功能
    icon = QPixmap(":/frame/Image/frame/fn_button.png");
    m_funcBtn.setIcon(QIcon(icon));
    m_funcBtn.setIconSize(icon.size());
    m_funcBtn.setObjectName("FunctionButton");
    m_funcBtn.setFocusPolicy(Qt::NoFocus);
    connect(&m_funcBtn,SIGNAL(clicked()),this,SLOT(slotFuncButtonClicked()));

    //右侧状态显示区
    m_statuesFrame.setObjectName("StatuesFrame");
    m_statuesFrame.setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    m_statuesFrame.setLayout(&m_stateLayout);
    m_status1.setObjectName("Statue1");
    m_status2.setObjectName("Statue2");
    m_status3.setObjectName("Statue3");
    m_status4.setObjectName("Statue4");
    m_stateLayout.setContentsMargins(6,3,6,3);
    m_stateLayout.addWidget(&m_status1);
    m_stateLayout.addWidget(&m_status2);
    m_stateLayout.addWidget(&m_status3);
    m_stateLayout.addWidget(&m_status4);

    //显示系统时间
    m_timeFrame.setObjectName("TimeFrame");
    m_timeFrame.setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    m_timeFrame.setLayout(&m_timeLayout);
    m_dateLabel.setObjectName("DateLabel");
    m_dateLabel.setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
    m_timeLabel.setObjectName("TimeLabel");
    m_timeLabel.setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
    m_timeLayout.setSpacing(0);
    m_timeLayout.setContentsMargins(6,2,6,2);
    m_timeLayout.addWidget(&m_dateLabel,0,Qt::AlignCenter);
    m_timeLayout.addWidget(&m_timeLabel,0,Qt::AlignCenter);

    connect(Authorization::instance(),SIGNAL(sigAuthUpdate()),this,SLOT(slotAuthorityChanged()));

    setStyleSheet("WinMainStatus{\
                    background-color:rgb(0,0,0);\
                    background-image:url(:/frame/Image/frame/status_bg.png);\
                    background-repeat: repeat-x;\
                    min-height:50px;\
                    }\
                    QPushButton{\
                    max-width:40px;\
                    max-height:40px;\
                    width:40px;\
                    height:40px;\
                    background-color:rgb(255,255,255,30);\
                    border:1px outset gray;\
                    border-radius:5px;\
                    }\
                    QPushButton:pressed{\
                    background-color:rgb(255,255,255,50);\
                    border-style:inset;\
                    }\
                    QLabel#TitleLabel{\
                    font-size:24px;\
                    color:rgb(255,255,255);\
                    }\
                    QLabel#IconLabel{\
                    min-width:24px;\
                    min-height:24px;\
                    background-position:center center;\
                    background-repeat: no-repeat;\
                    }\
                    QFrame#StatuesFrame{\
                    min-width:165px;\
                    min-height:40px;\
                    background-color:rgb(181,196,217);\
                    border:1px solid black;\
                    border-radius: 4px;\
                    }\
                    QLabel#Statue1{\
                    min-width:32px;\
                    max-width:32px;\
                    min-height:32px;\
                    max-height:32px;\
                    background-image:url(:/frame/Image/frame/Math_1.png);\
                    background-position:center center;\
                    background-repeat: no-repeat;\
                    }\
                    QLabel#Statue2{\
                    min-width:32px;\
                    max-width:32px;\
                    min-height:32px;\
                    max-height:32px;\
                    background-image:url(:/frame/Image/frame/USB_3.png);\
                    background-position:center center;\
                    background-repeat: no-repeat;\
                    }\
                    QLabel#Statue3{\
                    min-width:32px;\
                    max-width:32px;\
                    min-height:32px;\
                    max-height:32px;\
                    background-image:url(:/frame/Image/frame/sd_3.png);\
                    background-position:center center;\
                    background-repeat: no-repeat;\
                    }\
                    QLabel#Statue4{\
                    min-width:32px;\
                    max-width:32px;\
                    min-height:32px;\
                    max-height:32px;\
                    background-image:url(:/frame/Image/frame/red_off.png);\
                    background-position:center center;\
                    background-repeat: no-repeat;\
                    }\
                    QFrame#TimeFrame{\
                    min-width:110px;\
                    min-height:40px;\
                    background-image:url(:/frame/Image/frame/time_bg.png);\
                    background-repeat: repeat-x;\
                    border:1px solid black;\
                    border-radius: 4px;\
                    }\
                    QLabel#DateLabel,\
                    QLabel#TimeLabel{\
                    font-size:14px;\
                    }");

    //定时刷新
    slotTimeUpdate();
}

QPixmap WinMainStatus::getTitleIcon(quint32 frameId)
{
    switch(frameId)
    {
    case Win_Trend_Panel:                            //趋势图
        return QPixmap(":/frame/Image/frame/trend_icon.png");
    case Win_Digital_Panel:                           //数字图
        return QPixmap(":/frame/Image/frame/digital_icon.png");
    case Win_Bar_Panel:                                //棒图
        return QPixmap(":/frame/Image/frame/bar_icon.png");
    case Win_Overview_Panel:                      //总览图
        return QPixmap(":/frame/Image/frame/bar_icon.png");
    case Win_Alarm_Summary_Panel:          //报警一览
        return QPixmap(":/frame/Image/frame/alarm_icon.png");
    case Win_Info_Summary_Panel:             //信息一览
        return QPixmap(":/frame/Image/frame/msg.png");
    case Win_Mem_Summary_Panel:            //内存一览
        return QPixmap(":/frame/Image/frame/mem_summary.png");
    case Win_Report_Panel:                           //报表
        return QPixmap(":/frame/Image/frame/report_log.png");
    case Win_Modbus_Client_Panel:             //Modbus客户端
        return QPixmap(":/frame/Image/frame/mod_client.png");
    case Win_Modbus_Server_Panel:            //Modbus服务器
        return QPixmap(":/frame/Image/frame/mod_client.png");
    case Win_Relay_SW_Panel:                       //继电器和内部开关
        return QPixmap(":/frame/Image/frame/sw_relay.png");
    case Win_Event_Log_Panel:                       //事件
        return QPixmap(":/frame/Image/frame/event_log.png");
    case Win_Error_Log_Panel:                              //错误
        return QPixmap(":/frame/Image/frame/error_log.png");
    case Win_General_Log_Panel:                           //通信
        return QPixmap(":/frame/Image/frame/general_log.png");
    case Win_FTP_Log_Panel:                                //FTP
        return QPixmap(":/frame/Image/frame/ftp_log.png");
    case Win_WEB_Log_Panel:                              //WEB
        return QPixmap(":/frame/Image/frame/WEB_log.png");
    case Win_Email_Log_Panel:                            //Email
        return QPixmap(":/frame/Image/frame/EMAIL_log.png");
    case Win_Modbus_Log_Panel:                       //Modbus
        return QPixmap(":/frame/Image/frame/MOD_log.png");
    case Win_SNTP_Log_Panel:                            //SNTP
        return QPixmap(":/frame/Image/frame/SNTP_log.png");
    case Win_DHCP_Log_Panel:                           //DHCP
        return QPixmap(":/frame/Image/frame/DHCP_log.png");
    case Win_Network_Panel:                      //网络操作
        return QPixmap(":/frame/Image/frame/network.png");
    case Win_Multi_Panel:                             //多画面
        return QPixmap(":/frame/Image/frame/multi_icon.png");
    case Win_Root_Panel:
        return tr(":/frame/Image/frame/home.png");
    default:
        return QPixmap(":/frame/Image/frame/setting_icon.png");
    }
}

void WinMainStatus::slotTitleChanged(quint32 frameId)
{
    m_iconLabel.setPixmap(getTitleIcon(frameId));
    m_titleLabel.setText(WinMainFrame::getFrameName(frameId));
}

void WinMainStatus::slotTitleChanged(quint32 frameId,quint32 grpIndex)
{
    m_iconLabel.setPixmap(getTitleIcon(frameId));
    m_titleLabel.setText(WinMainFrame::getFrameName(frameId,grpIndex));
}

void WinMainStatus::slotPrintScreenButtonClicked()
{
    WinMainFrame::instance()->slotPrintSreen();
    WinSnapshotDialog().exec();
}

void WinMainStatus::slotAlertButtonClicked()
{
    WinAckAlarmDialog ackDialog;
    if(ackDialog.exec() == QDialog::Accepted){
        EventAction::instance()->ackAlarmAll();
    }
}

void WinMainStatus::slotMemButtonClicked()
{
    WinMediaDialog mediaDialog;
    mediaDialog.exec();
}

void WinMainStatus::slotMathButtonClicked()
{
    WinMathDialog mathDialog;
    mathDialog.exec();
}

void WinMainStatus::slotRecordButtonClicked()
{
    bool recordStatus = !ThreadDataSync::instance()->getRecordStatus();
    if (CfgChannel::instance()->getSysBatchSettingOnOff()) {
        WinBatchDialog::instance()->setDialogType(1);
        WinBatchDialog::instance()->setRecordStatus(recordStatus/*, type*/);
        WinBatchDialog::instance()->setUserDefineStyle();
        WinBatchDialog::instance()->exec();
    }
    else {
        WinRecordOtherDialog::instance()->setRecordStatus(recordStatus/*, type*/);
        WinRecordOtherDialog::instance()->exec();
    }
}

void WinMainStatus::slotHomeButtonPressed()
{
    m_time.restart();
}

void WinMainStatus::slotHomeButtonReleased()
{
    if(m_time.elapsed() < 1500){//按压小于1.5s直接跳转默认面板
        emit sigChangeFrame(WinMainFrame::getDefaultPanel());
    }else{//设置当前面板为home键默认跳转
        WinStandardDialog standartDialog;
        if(standartDialog.exec() == QDialog::Accepted){
            WinMainFrame::instance()->slotSaveCurrentPanel();
        }
    }
}

void WinMainStatus::slotAuthorityChanged()
{
    if(Authorization::instance()->getLoginStatus() == Authorization::Auth_LoginStatus_Logout){
        m_prtScBtn.setEnabled(false);
        m_memBtn.setEnabled(false);
        m_mathBtn.setEnabled(false);
        m_alertBtn.setEnabled(false);
        m_recordBtn.setEnabled(false);
        m_funcBtn.setEnabled(false);
    }else{
        m_memBtn.setEnabled(true);
        m_funcBtn.setEnabled(true);
        if(Authorization::instance()->getLoginStatus() == Authorization::Auth_LoginStatus_Off){
            m_prtScBtn.setEnabled(true);
            m_mathBtn.setEnabled(true);
            m_alertBtn.setEnabled(true);
            m_recordBtn.setEnabled(true);
        }else{
            m_prtScBtn.setEnabled(!Authorization::instance()->getLoginedUser()->isLocked(Security_ModuleAuth_Datasave));
            m_mathBtn.setEnabled(!Authorization::instance()->getLoginedUser()->isLocked(Security_ModuleAuth_Math));
            m_alertBtn.setEnabled(!Authorization::instance()->getLoginedUser()->isLocked(Security_ModuleAuth_ACK));
            m_recordBtn.setEnabled(!Authorization::instance()->getLoginedUser()->isLocked(Security_ModuleAuth_Record));
        }
    }
}

void WinMainStatus::slotFuncButtonClicked()
{
    WinMainFrame::instance()->slotShowDialog();
}

//更新状态区报警图标
void WinMainStatus::slotAlarmStateUpdate()
{
    QVariant value = CfgChannel::instance()->getSysAlarmStatus();
    if(!value.isValid()){
        m_status4.setVisible(false);
        return;
    }else{
        m_status4.setVisible(true);
    }

    QPixmap bg;
    bool blink = WinMainFrame::getBlinkStatus();

    switch(value.toUInt()){
    case SQL_Alarm_Status_NonHold_Occur://非保持发生　红灯不闪
    case SQL_Alarm_Status_Hold_Occur_Ack://保持发生解除　红灯不闪
        bg = QPixmap(":/frame/Image/frame/red_off.png");
        break;
    case SQL_Alarm_Status_Hold_Occur_NoAck://保持发生未解除　红灯闪
        if(blink){
            bg = QPixmap(":/frame/Image/frame/red_off.png");
        }else{
            bg = QPixmap(":/frame/Image/frame/red_on.png");
        }
        break;
    case SQL_Alarm_Status_Hold_Release_NoAck://保持释放未解除　灰灯闪
        if(blink){
            bg = QPixmap(":/frame/Image/frame/gray_off.png");
        }else{
            bg = QPixmap(":/frame/Image/frame/gray_on.png");
        }
        break;
    default:
        bg = QPixmap(32,32);//TODO 报警未发生图片
        bg.fill(QColor(181,196,217));
        break;
    }

    m_status4.setPixmap(bg);
}

void WinMainStatus::slotMathStateUpdate()
{
    if(ThreadDataSync::instance()->getRecordStatus()){
        m_status1.setVisible(true);
        static bool flag = false;
        flag = !flag;
        if(flag){
            m_status1.setPixmap(QPixmap(":/frame/Image/frame/Math_1.png"));
        }else{
            m_status1.setPixmap(QPixmap(":/frame/Image/frame/Math_2.png"));
        }
    }else{
        m_status1.setVisible(false);
    }
}

void WinMainStatus::slotMemStateUpdate()
{
    //TODO 热插拔设备插入或移除
}

void WinMainStatus::slotTimeUpdate()
{
    m_timeLabel.setText(QDateTime::currentDateTime().toString("hh:mm:ss"));
    m_dateLabel.setText(QDateTime::currentDateTime().toString(CfgChannel::instance()->getSysDateFormat()));
//    qDebug()<<CfgChannel::instance()->getSysDateFormat();
}

quint32 WinMainFrame::m_defaultPanel = Win_Root_Panel;
bool WinMainFrame::m_blink = false;

WinMainFrame *WinMainFrame::instance()
{
    static WinMainFrame data;
    return &data;
}

QString WinMainFrame::getFrameName(quint32 frameId)
{
    switch(frameId)
    {
    case Win_Trend_Panel:                            //趋势图
        return tr("Trend");
    case Win_Digital_Panel:                           //数字图
        return tr("Digital");
    case Win_Bar_Panel:                                //棒图
        return tr("Bar");
    case Win_Overview_Panel:                      //总览图
        return tr("Overview");
    case Win_Alarm_Summary_Panel:          //报警一览
        return tr("Alarm Summary");
    case Win_Info_Summary_Panel:             //信息一览
        return tr("Message Summary");
    case Win_Mem_Summary_Panel:            //内存一览
        return tr("Memory Summary");
    case Win_Report_Panel:                           //报表
        return tr("Report");
    case Win_Modbus_Client_Panel:             //Modbus客户端
        return tr("Modbus Client");
    case Win_Modbus_Server_Panel:            //Modbus服务器
        return tr("Modbus Server");
    case Win_Relay_SW_Panel:                       //继电器和内部开关
        return tr("Relay And Switch");
    case Win_Event_Log_Panel:                       //事件日志
        return tr("Event");
    case Win_Error_Log_Panel:                              //错误日志
        return tr("Error");
    case Win_General_Log_Panel:                           //通信日志
        return tr("General");
    case Win_FTP_Log_Panel:                                //FTP日志
        return tr("FTP");
    case Win_WEB_Log_Panel:                              //WEB日志
        return tr("WEB");
    case Win_Email_Log_Panel:                            //Email日志
        return tr("Email");
    case Win_Modbus_Log_Panel:                       //Modbus日志
        return tr("Modbus");
    case Win_SNTP_Log_Panel:                            //SNTP日志
        return tr("SNTP ");
    case Win_DHCP_Log_Panel:                           //DHCP日志
        return tr("DHCP");
    case Win_Network_Panel:                             //网络信息
        return tr("Network Information");
    case Win_Multi_Panel:                                   //多画面
        return tr("Multi Panel");
    case Win_Root_Panel:
        return tr("Home");
    case Win_Save_Load_Panel:
        return tr("Saveload");
    case Win_Colibration_Panel:
        return tr("Initialize Calibration");
    case Win_System_Info_Panel:
        return tr("System Information");
    default:
        return QString();
    }
}

QString WinMainFrame::getFrameName(quint32 frameId, quint32 grpIndex)
{
    if(frameId ==Win_Trend_Panel ||  frameId ==Win_Digital_Panel  || frameId ==Win_Bar_Panel ){
        QVariant value = CfgChannel::instance()->getGroupName(grpIndex);
        if(value.isValid()){
            return value.toString();
        }
    }
    return getFrameName(frameId);
}

WinTrendGroupPanel *WinMainFrame::getTrendGroup()
{
    return (WinTrendGroupPanel *)m_container[Win_Trend_Panel];
}

WinBarPanel *WinMainFrame::getBarPanel()
{
    return (WinBarPanel *)m_container[Win_Bar_Panel];
}

WinDigitalPanel *WinMainFrame::getDigitalPanel()
{
    return (WinDigitalPanel *)m_container[Win_Digital_Panel];
}

WinMainFrame::WinMainFrame(QWidget *parent) :
    QWidget(parent),m_timer(this)
{
    setWinLayout();
    initialize();
    m_timer.start(500);
}

void WinMainFrame::initialize()
{
    m_container.clear();

    //趋势图
    static WinTrendGroupPanel trendPanel;
    connect(&trendPanel,SIGNAL(sigCurrentIndex(quint32)),this,SLOT(slotFrameSlide(quint32)));
    m_container[Win_Trend_Panel] = &trendPanel;
    m_workWinStacked.addWidget(&trendPanel);

    //数字图
    static  WinDigitalPanel digitalPanel;
    connect(&digitalPanel,SIGNAL(sigCurrentIndex(quint32)),this,SLOT(slotFrameSlide(quint32)));
    m_container[Win_Digital_Panel] = &digitalPanel;
    m_workWinStacked.addWidget(&digitalPanel);

    //棒图
    static WinBarPanel barPanel;
    connect(&barPanel,SIGNAL(sigCurrentIndex(quint32)),this,SLOT(slotFrameSlide(quint32)));
    m_container[Win_Bar_Panel] = &barPanel;
    m_workWinStacked.addWidget(&barPanel);

    //多画面
    static WinMultiPanel multiPanel;
    connect(&multiPanel,SIGNAL(sigChangeFrame(quint32)),this,SLOT(slotFrameChanged(quint32)));
    m_container[Win_Multi_Panel] = &multiPanel;
    m_workWinStacked.addWidget(&multiPanel);

    //总览图
    static WinOverViewPanel overviewPanel;
    connect(&overviewPanel,SIGNAL(sigChangeFrame(quint32)),this,SLOT(slotFrameChanged(quint32)));
    m_container[Win_Overview_Panel] = &overviewPanel;
    m_workWinStacked.addWidget(&overviewPanel);

    //根配置菜单
    static WinRoot rootPanel;
    connect(&rootPanel,SIGNAL(sigChangeFrame(quint32)),this,SLOT(slotFrameChanged(quint32)));
    connect(CfgChannel::instance(), SIGNAL(sigNormalConfigChanged(quint32)), &rootPanel, SLOT(slotSecurityChange(quint32)));
    m_container[Win_Root_Panel] = &rootPanel;
    m_workWinStacked.addWidget(&rootPanel);

    //系统初始化与校验
    static WinColibrationPanel colibrationPanel;
    connect(&colibrationPanel,SIGNAL(sigChangeFrame(quint32)),this,SLOT(slotFrameChanged(quint32)));
    m_container[Win_Colibration_Panel] = &colibrationPanel;
    m_workWinStacked.addWidget(&colibrationPanel);

    //系统信息
    static WinSystemInfo systemInfoPanel;
    connect(&systemInfoPanel,SIGNAL(sigChangeFrame(quint32)),this,SLOT(slotFrameChanged(quint32)));
    m_container[Win_System_Info_Panel] = &systemInfoPanel;
    m_workWinStacked.addWidget(&systemInfoPanel);

    //文件加载与存储
    static winsaveload saveloadPanel;
    connect(&saveloadPanel,SIGNAL(sigChangeFrame(quint32)),this,SLOT(slotFrameChanged(quint32)));
    m_container[Win_Save_Load_Panel] = &saveloadPanel;
    m_workWinStacked.addWidget(&saveloadPanel);

    //报警一览
    static WinAlarmSummary alarmPanel;
    m_container[Win_Alarm_Summary_Panel] = &alarmPanel;
    m_workWinStacked.addWidget(&alarmPanel);

    //信息一览
    static WinMessageSummary msgPanel;
    m_container[Win_Info_Summary_Panel] = &msgPanel;
    m_workWinStacked.addWidget(&msgPanel);

    //内存一览
    static WinMemSummary memPanel;
    m_container[Win_Mem_Summary_Panel] = &memPanel;
    m_workWinStacked.addWidget(&memPanel);

    //报表
    static WinReport reportPanel;
    m_container[Win_Report_Panel] = &reportPanel;
    m_workWinStacked.addWidget(&reportPanel);

    //Modbus客户端
    static WinModbusClient modClient;
    m_container[Win_Modbus_Client_Panel] = &modClient;
    m_workWinStacked.addWidget(&modClient);

//    m_container[Win_Modbus_Server_Panel] = new ;

    //继电器与内部开关
    static WinRelayAndSW rasPanel;
    m_container[Win_Relay_SW_Panel] = &rasPanel;
    m_workWinStacked.addWidget(&rasPanel);

    //操作日志
    static WinOperationLog opeLogPanel;
    m_container[Win_Event_Log_Panel] = &opeLogPanel;
    m_workWinStacked.addWidget(&opeLogPanel);

    //错误日志
    static WinErrorLog errLogPanel;
    m_container[Win_Error_Log_Panel] = &errLogPanel;
    m_workWinStacked.addWidget(&errLogPanel);

    //通信日志
    static WinCommLog commLogPanel;
    m_container[Win_General_Log_Panel] = &commLogPanel;
    m_workWinStacked.addWidget(&commLogPanel);

    //FTP日志
    static WinFTPLog ftpLogPanel;
    m_container[Win_FTP_Log_Panel] = &ftpLogPanel;
    m_workWinStacked.addWidget(&ftpLogPanel);

    //WEB日志
    static WinWebLog webLogPanel;
    m_container[Win_WEB_Log_Panel] = &webLogPanel;
    m_workWinStacked.addWidget(&webLogPanel);

    //Email日志
    static WinEmailLog emailLogPanel;
    m_container[Win_Email_Log_Panel] = &emailLogPanel;
    m_workWinStacked.addWidget(&emailLogPanel);

    //modbus日志
    static WinModbusLog modLogPanel;
    m_container[Win_Modbus_Log_Panel] = &modLogPanel;
    m_workWinStacked.addWidget(&modLogPanel);

    //SNTP日志
    static WinSNTPLog sntpLogPanel;
    m_container[Win_SNTP_Log_Panel] = &sntpLogPanel;
    m_workWinStacked.addWidget(&sntpLogPanel);

    //DHCP日志
    static WinDHCPLog dhcpLogPanel;
    m_container[Win_DHCP_Log_Panel] = &dhcpLogPanel;
    m_workWinStacked.addWidget(&dhcpLogPanel);

    //网络信息
    static WinNetworkInfo networkPanel;
    m_container[Win_Network_Panel] = &networkPanel;
    m_workWinStacked.addWidget(&networkPanel);

    //设置默认显示界面
    slotFrameChanged(m_defaultPanel);

    //连接信号与槽
    connect(&m_statusBar,SIGNAL(sigChangeFrame(quint32)),this,SLOT(slotFrameChanged(quint32)));
    connect(&m_timer, SIGNAL(timeout()), &m_statusBar, SLOT(slotTimeUpdate()));
    connect(&m_timer,SIGNAL(timeout()),this,SIGNAL(sigHeartBeat500ms()));
    connect(&m_timer,SIGNAL(timeout()),this,SLOT(slotTimeout()));
    connect(this,SIGNAL(sigHeartBeat500ms()),&m_statusBar,SLOT(slotAlarmStateUpdate()));
    connect(Authorization::instance(), SIGNAL(sigAuthUpdate()), this, SLOT(slotAuthChange()));
    connect(this,SIGNAL(sigHeartBeat500ms()),&m_statusBar,SLOT(slotMathStateUpdate()));
}

void WinMainFrame::setWinLayout()
{
    setLayout(&m_mainLayout);
    setContentsMargins(0,0,0,0);
    setObjectName("MainWindows");

    m_mainLayout.setContentsMargins(0,0,0,0);
    m_mainLayout.setSpacing(0);

    m_mainLayout.addWidget(&m_statusBar);
    m_mainLayout.addWidget(&m_workWinStacked);

    setStyleSheet("#MainWindows{background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"\
                        "stop: 0 rgb(0, 0, 154), stop: 1 rgb(0, 72, 255));}");
}

void WinMainFrame::keyReleaseEvent(QKeyEvent *event)
{
    //T0D0 等待有能获取机器当前的记录状态的接口，在去掉这个testfalg使用接口控制按键的功能

    if(event->key() == Qt::Key_T && (event->modifiers() & Qt::AltModifier)){
        clickRecordButton(!ThreadDataSync::instance()->getRecordStatus());
    }
    else if (event->key() == Qt::Key_M && (event->modifiers() & Qt::AltModifier)) {
        if (Authorization::instance()->getLoginStatus() == Authorization::Auth_LoginStatus_Logout &&
                !Authorization::instance()->isApprovalOperation()) {
            WinLoginDialog::instance()->initData();
            WinLoginDialog::instance()->setDialogStatus(true);
            WinLoginDialog::instance()->exec();
        }
        else
            slotFrameChanged(m_defaultPanel);
    }
    return QWidget::keyReleaseEvent(event);
}

bool WinMainFrame::event(QEvent *e)
{
    if(e->type() == QEvent::KeyPress || e->type() ==QEvent::MouseMove ||e->type() ==QEvent::MouseButtonPress ||e->type() ==QEvent::TouchUpdate ||e->type() ==QEvent::Wheel){
        Authorization::instance()->resetTimer();
    }
    return QWidget::event(e);
}

void WinMainFrame::clickRecordButton(bool recordStatus)
{

    if(CfgChannel::instance()->getSysRecordConfirmScreen()){
        if (CfgChannel::instance()->getSysBatchSettingOnOff()) {
            WinBatchDialog::instance()->setDialogType(1);
            WinBatchDialog::instance()->setRecordStatus(recordStatus/*, type*/);
            WinBatchDialog::instance()->setUserDefineStyle();
            WinBatchDialog::instance()->exec();
        }
        else {
            WinRecordOtherDialog::instance()->setRecordStatus(recordStatus/*, type*/);
            WinRecordOtherDialog::instance()->exec();
        }
    }
    else {
        if (recordStatus) {
            ThreadDataSync::instance()->setRecordStatus(true);
            ThreadDataProcess::instance()->setMathStatus(Math_Status_Type_On);
        }
        else {
            ThreadDataSync::instance()->setRecordStatus(false);
            ThreadDataProcess::instance()->setMathStatus(Math_Status_Type_Off);
        }
    }

}

void WinMainFrame::slotFrameChanged()
{
    slotFrameChanged(m_defaultPanel);
}

void WinMainFrame::slotFrameChanged(quint32 frameId)
{
    m_currentFrameId = frameId;

    if(m_container.contains(frameId)){
        m_workWinStacked.setCurrentWidget(m_container[frameId]);
    }else{
        m_workWinStacked.setCurrentWidget(m_container[m_defaultPanel]);
    }

    m_statusBar.slotTitleChanged(frameId);
}

void WinMainFrame::slotGroupChanged(quint32 grpIndex)
{
    if(m_currentFrameId == Win_Trend_Panel){
        WinTrendGroupPanel *widget = (WinTrendGroupPanel *)m_container[Win_Trend_Panel];
        widget->setCurrentWidget(grpIndex);
    }else if(m_currentFrameId == Win_Digital_Panel){
        WinDigitalPanel *widget = (WinDigitalPanel *)m_container[Win_Digital_Panel];
        widget->setCurrentWidget(grpIndex);
    }else if(m_currentFrameId == Win_Bar_Panel){
        WinBarPanel *widget = (WinBarPanel *)m_container[Win_Bar_Panel];
        widget->setCurrentWidget(grpIndex);
    }
}

void WinMainFrame::slotPrintSreen()
{
    QScreen *screen = QGuiApplication::primaryScreen();
    QPixmap pix = screen->grabWindow(qApp->desktop()->winId());
    pix.save(QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss_zzz")+".jpg");
}

void WinMainFrame::slotSnapshot()
{
    m_statusBar.slotPrintScreenButtonClicked();
}

void WinMainFrame::slotShowDialog()
{
    if(m_currentFrameId == Win_Multi_Panel){
        WinMultiPanel *widget = (WinMultiPanel *)m_container[Win_Multi_Panel];
        widget->showConfigDialog();
    }
}

void WinMainFrame::slotFrameSlide(quint32 grpIndex)
{
    m_statusBar.slotTitleChanged(m_currentFrameId,grpIndex);
}

void WinMainFrame::slotTimeout()
{
    m_blink = !m_blink;
    this->update();
}

void WinMainFrame::slotAuthChange()
{
    if (Authorization::instance()->getLoginStatus() == 1 &&
            (Authorization::instance()->getLoginedUser()->getUserType() == 0 ||
             Authorization::instance()->getLoginedUser()->getUserType() == 1)) {
    }
    else if (Authorization::instance()->getLoginStatus() == 1 &&
             Authorization::instance()->getLoginedUser()->getUserType() == 2) {
        WinMainFrame::instance()->slotFrameChanged(); //跳转到主界面
//        slotUniversalClicked();
    }
    else if (Authorization::instance()->getLoginStatus() == 2) {
        WinMainFrame::instance()->slotFrameChanged(); //跳转到主界面
//        slotUniversalClicked();
        if (!Authorization::instance()->isApprovalOperation()) {
            WinTransparentDialog::instance()->setDialogStatus(true);
            WinTransparentDialog::instance()->exec();
        }
    }
}



