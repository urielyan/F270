#include "winroot_generalwin.h"
#include "winlogdialog.h"

#include <QPainter>
#include "WorkWin/wintrendgrouppanel.h"

WinRoot_GeneralWin *WinRoot_GeneralWin::instance()
{
    static WinRoot_GeneralWin data;
    return &data;
}

QList<GeneralMenu_Item *> WinRoot_GeneralWin::getGeneralWinItemList()
{
    return itemList;
}

WinRoot_GeneralWin::WinRoot_GeneralWin(QWidget *parent) :
    QWidget(parent)
{

    initItem();
//
    setMainLayout();
    getItemFlag();
    connectSgSlot();

}

WinRoot_GeneralWin::~WinRoot_GeneralWin()
{
    release();
}

/*
 * 功能：
 *  flag是一个32位的数，每一位对于list中button的显示与否,这个函数还需要到时再重写，现在先目前测试用
 * 参数：
 *  无
 * 返回值：
 *  无
 */
void WinRoot_GeneralWin::getItemFlag()
{
    slotControlItem(ID_NetWork_Modbus_BasicClientOnOff);
    slotControlItem(ID_Serial_Basic_Receiver_Mode);
    slotControlItem(ID_Report_Basic_Type_Value);
}

/*
 * 功能：
 *  初始化窗体,new出各个对象,将所有按键全部放入list中
 * 参数：
 *  无
 * 返回值：
 *  无
 */
void WinRoot_GeneralWin::initItem()
{
    /**
      * add by gehailang
      */
    this->setObjectName("WinrootGeneralWin");
    this->setStyleSheet("#WinrootGeneralWin{background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, "\
                        "stop: 0 rgb(0, 7, 164), stop: 1 rgb(0, 68, 249));"\
                        "border-width: 0px; border-style: solid;}");

    p_scrollArea = new QScrollArea;
    p_scrollArea->setFixedSize(DESKTOP_WIDTH-DESKTOP_WIDTH/20, DESKTOP_HEIGHT - DESKTOP_WIDTH/10);
    p_scrollArea->horizontalScrollBar()->setHidden(true);
    p_scrollArea->verticalScrollBar()->setHidden(true);
    p_scrollArea->setFrameShape(QFrame::NoFrame);
    p_scrollArea->setContentsMargins(50, 0 ,50,0);
    p_scrollArea->setStyleSheet(QString::fromUtf8("background-color:transparent;"));
    p_scrollArea->verticalScrollBar()->setStyleSheet("QScrollBar{background:rgb(0,72,254); width: 4px;}"
                                                                          "QScrollBar::handle{background:rgb(0,9,166); border:0px solid transparent; border-radius:2px;}"
                                                                          "QScrollBar::handle:hover{background:rgb(0,9,200);}"
                                                                          "QScrollBar::sub-line{background:transparent;}"
                                                                          "QScrollBar::add-line{background:transparent;}");

    p_mainWidget = new QWidget(p_scrollArea);
    p_mainWidget->setFixedWidth(DESKTOP_WIDTH-DESKTOP_WIDTH/20);

    m_vLayout.setSizeConstraint(QLayout::SetFixedSize);

    m_mainLayout.setContentsMargins(30,0,30,20);


    for(int i = WinItemID_Trend;i <= WinItemID_InternalSwitch;i++) {

        itemList.push_back(new GeneralMenu_Item(i));
    }
    while (itemList.size()%5 != 0)
        itemList.push_back(new GeneralMenu_Item(-1));
}


/*
 * 功能：
 *  绑定每个按键的点击信号和槽函数
 * 参数：
 *  无
 * 返回值：
 *  无
 */
void WinRoot_GeneralWin::connectSgSlot()
{
    connect(CfgChannel::instance(), SIGNAL(sigNormalConfigChanged(quint32)), this, SLOT(getItemFlag(/*quint32*/)));

    for(int i = 0;i < itemList.size();++i) {
        connect(itemList.value(i), SIGNAL(sgItemRelease(int)), this, SLOT(slotItemClicked(int)));
    }

}

/*
 * 功能：
 *  网格布局按键的添加,根据flag的值来决定每个按键的添加与否
 * 参数：
 *  无
 * 返回值：
 *  无
 */
void WinRoot_GeneralWin::setMainLayout()
{
    int k = 0;

    for (int i =0; i < (itemList.size()) / 5 ; ++i) {
        m_hLayoutList.append(new QHBoxLayout);
        if (DESKTOP_WIDTH < 900)
            m_hLayoutList.value(i)->setSpacing(p_scrollArea->width() / 20);
        else if (DESKTOP_WIDTH > 900 && DESKTOP_WIDTH < 1300)
            m_hLayoutList.value(i)->setSpacing(p_scrollArea->width() / 15);
        else
            m_hLayoutList.value(i)->setSpacing(p_scrollArea->width() / 10);
        m_vLayout.addLayout(m_hLayoutList.value(i));
        for (int j = 0; j < 5; ++j) {
            if (k < itemList.size()) {
                m_hLayoutList.value(i)->addWidget(itemList.value(k++));
            }
        }
    }


    p_mainWidget->setLayout(&m_vLayout);
    p_scrollArea->setWidget(p_mainWidget);

    m_mainLayout.addWidget(p_scrollArea);
    this->setLayout(&m_mainLayout);
}

/*
 * 功能：
 *  取消信号槽绑定，delete对象，在析构函数中使用
 * 参数：
 *  无
 * 返回值：
 *  无
 */
void WinRoot_GeneralWin::release()
{
    /**取消绑定**/
    while(!itemList.isEmpty()) {
        disconnect(itemList.value(0), SIGNAL(sgItemRelease(int)), this, SLOT(slotItemClicked(int)));
        delete itemList.value(0);
        itemList.pop_front();
    }

}

void WinRoot_GeneralWin::setItemHide(int ID, bool hide)
{
    int i = 0;

    for (; i < itemList.size(); ++i) {
        if (-1 == itemList.value(i)->getItemID()) {
            itemList.value(i)->deleteLater();
            itemList.removeAt(i--);
        }
    }

    i = 0;

    for (; i < itemList.size(); ++i) {
        if (ID == itemList.value(i)->getItemID())
            break;
    }
    if (i < itemList.size()) {
        if (hide) {
            disconnect(itemList.value(i), SIGNAL(sgItemRelease(int)), this, SLOT(slotItemClicked(int)));
            itemList.value(i)->deleteLater();
            itemList.removeAt(i);
        }
    }
    else {
        if (!hide) {
            itemList.append(new GeneralMenu_Item(ID));
            connect(itemList.last(), SIGNAL(sgItemRelease(int)), this, SLOT(slotItemClicked(int)));
        }
    }
    while (itemList.size()%5 != 0)
        itemList.push_back(new GeneralMenu_Item(-1));
}

//void WinRoot_GeneralWin::resizeEvent(QResizeEvent *event)
//{

//}

void WinRoot_GeneralWin::slotItemClicked(int id)
{
    switch (id) {
    case WinItemID_Trend:
        //该入口进入趋势模块时，始终切换至实时模式
        WinMainFrame::instance()->getTrendGroup()->switchToRealtimeReview();
        sigChangeFrame(Win_Trend_Panel);
        break;
    case WinItemID_Digital:
        sigChangeFrame(Win_Digital_Panel);
        break;
    case WinItemID_Bar:
        sigChangeFrame(Win_Bar_Panel);
        break;
    case WinItemID_Overview:
        sigChangeFrame(Win_Overview_Panel);
        break;
    case WinItemID_Alert:
        sigChangeFrame(Win_Alarm_Summary_Panel);
        break;
    case WinItemID_Information:
        sigChangeFrame(Win_Info_Summary_Panel);
        break;
    case WinItemID_Ram:
        sigChangeFrame(Win_Mem_Summary_Panel);
        break;
    case WinItemID_ReportForm:
        sigChangeFrame(Win_Report_Panel);
        break;
    case WinItemID_ModbusClient:
        sigChangeFrame(Win_Modbus_Client_Panel);
        break;
    case WinItemID_ModbusHost:

        break;
    case WinItemID_Journal:
    {
        WinLogDialog log;
        connect(&log, SIGNAL(sigChangeFrame(quint32)), this, SIGNAL(sigChangeFrame(quint32)));
        log.exec();
        break;
    }
    case WinItemID_MultiScreen:
        sigChangeFrame(Win_Multi_Panel);
        break;
//    case WinItemID_Setting:

//        break;
    case WinItemID_SaveLoading:
        sigChangeFrame(Win_Save_Load_Panel);
        break;
    case WinItemID_InternetInfo:
        sigChangeFrame(Win_Network_Panel);
        break;
    case WinItemID_SystemInfo:
        sigChangeFrame(Win_System_Info_Panel);
        break;
    case WinItemID_InitValueCorrection:
        sigChangeFrame(Win_Colibration_Panel);
        break;
//    case WinItemID_CustomScreen:

//        break;
    case WinItemID_InternalSwitch:
        sigChangeFrame(Win_Relay_SW_Panel);
        break;
    default:
        break;
    }
}

void WinRoot_GeneralWin::slotControlItem(/*WinItemID id, int visible*/quint32 ID)
{
    if (ID == ID_NetWork_Modbus_BasicClientOnOff) {
        if (DevConfigPtr->networkModbus.clientOnOff)
            setItemHide(WinItemID_ModbusClient, false);
        else
            setItemHide(WinItemID_ModbusClient, true);
    }
    else if (ID == ID_Serial_Basic_Receiver_Mode || ID == ID_Serial_Modbus_Basic_Server_OnOff) {
        if (DevConfigPtr->serialConfig.serialMode == Serial_Mode_Modbus_Master &&
                DevConfigPtr->serialConfig.clientOnOff)
            setItemHide(WinItemID_ModbusHost, false);
        else
            setItemHide(WinItemID_ModbusHost, true);
    }
    if (ID == ID_Report_Basic_Type_Value) {
        if (DevConfigPtr->reportConfig.reportType)
            setItemHide(WinItemID_ReportForm, false);
        else
            setItemHide(WinItemID_ReportForm, true);
    }
    else
        return;

    for (int i = 0; i < m_hLayoutList.size(); ++i) {
        m_hLayoutList.value(i)->deleteLater();
    }

    m_hLayoutList.clear();
    int k = 0;

    for (int i =0; i < (itemList.size()) / 5 ; ++i) {
        m_hLayoutList.append(new QHBoxLayout);
        if (DESKTOP_WIDTH < 900)
            m_hLayoutList.value(i)->setSpacing(p_scrollArea->width() / 20);
        else if (DESKTOP_WIDTH > 900 && DESKTOP_WIDTH < 1300)
            m_hLayoutList.value(i)->setSpacing(p_scrollArea->width() / 15);
        else
            m_hLayoutList.value(i)->setSpacing(p_scrollArea->width() / 10);
        m_vLayout.addLayout(m_hLayoutList.value(i));
        for (int j = 0; j < 5; ++j) {
            if (k < itemList.size()) {
                m_hLayoutList.value(i)->addWidget(itemList.value(k++));
            }
        }
    }
}
