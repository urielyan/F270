#include "winroot_generalmenu.h"
#include <QPainter>
#include <QPixmap>
#include <QDebug>
#include <QPalette>
#include <QString>
#include <sys/time.h>
#include <unistd.h>

#include "winstandarddialog.h"
#include "../Common/setdatetime.h"
#include "../Common/displaymessage.h"
#include "./Common/eventcalendar.h"
#include "./Common/printtest.h"
#include "winroot_generalwin.h"
#include "./Common/eventaction.h"

GeneralMenu_Item::GeneralMenu_Item(QWidget *parent) :
    QWidget(parent)
{
    initItem();
}

GeneralMenu_Item::GeneralMenu_Item( int ID) :
    m_id(ID),
    m_itemEnable(true)
{
    initItem();
//    setItemEnable(true);

    if (ID >= 0) {
        WinItemInfo tempInfo = getWinBtnInfo(ID);

        setItemText(tempInfo.name);
        setItemStyle(tempInfo.imagePress, tempInfo.imageRelease);
    }
    else {
        setItemVisible(false);
    }

//    getWinBtnInfo()
}

void GeneralMenu_Item::initItem()
{
    /***new出对象和进行布局**/
        vLayout = new QVBoxLayout;
        hLayout = new QHBoxLayout;
        button = new QPushButton;
        button->setFocusPolicy(Qt::NoFocus);
        hLayout->addStretch();
        hLayout->addWidget(button);
        hLayout->addStretch();
        vLayout->addLayout(hLayout);
        text = new QLabel;
        QPalette p;
        p.setColor(QPalette::WindowText, Qt::white);
        text->setPalette(p);

        text->setAlignment(Qt::AlignHCenter);
        vLayout->addWidget(text);

        setLayout(vLayout);

        connect(this->button,SIGNAL(released()), this, SLOT(slotItemRelease()));
}

/*
 * 功能：
 * 	  设置按键下方的文字
 * 参数：
 * 	  QString itemText:文字内容
 * 返回值：
 * 	  无
 */
void GeneralMenu_Item::setItemText(QString itemText)
{
    text->setText(itemText);
}

/**/
/*
 * 功能：
 * 	  设置整个对象的visible属性
 * 参数：
 * 	  bool visible：item的visible属性
 * 返回值：
 * 	  无
 */
void GeneralMenu_Item::setItemVisible(bool visible)
{
    button->setVisible(visible);
    text->setVisible(visible);
}

/*
 * 功能：
 * 	  设置button的样式
 * 参数：
 * 	  QString image ：button背景图片URL
 *    QString image_p ：鼠标移动到button上是的图片URL
 * 返回值：
 * 	  无
 */
void GeneralMenu_Item::setItemStyle(QString image, QString image_p)
{
    m_image = image;
    m_imageP = image_p;
    button->setStyleSheet("\
                          QPushButton{border:none;\
                                      min-height: 120px;\
                                      min-width: 120px;\
                                      background-image: url("+image+");\
                                      background-position:center;}"\
                          "QPushButton:hover{border:none;\
                                      min-height: 120px;\
                                      min-width: 120px;\
                                      background-image: url("+image_p+");\
                                      background-position:center;}");
}

void GeneralMenu_Item::setItemID(int id)
{
    m_id = id;
}

int GeneralMenu_Item::getItemID()
{
    return m_id;
}

void GeneralMenu_Item::setItemEnable(bool enable)
{
    if (m_id == -1)
        return;

    WinItemInfo tempInfo = getWinBtnInfo(m_id);

    m_itemEnable = enable;

    button->setEnabled(enable);
    text->setEnabled(enable);

    if (enable) {
        setItemStyle(tempInfo.imagePress, tempInfo.imageRelease);
    }
    else {
        setItemStyle(tempInfo.imageGray, tempInfo.imageRelease);
    }
}

bool GeneralMenu_Item::getItemEnable()
{
    return m_itemEnable;
}

void GeneralMenu_Item::slotItemRelease()
{
    emit sgItemRelease(m_id);
}

WinItemInfo GeneralMenu_Item::getWinBtnInfo(int ID)
{
    WinItemInfo map;

     QString tempBtnInfo[46][4] = {

         //通用菜单画面
         {
             "Buzzer aCK",
             ":/commonMenus/Image/commonMenus/buzzer.png",
             ":/commonMenus/Image/commonMenus/buzzer_p.png",
             ":/commonMenus/Image/commonMenus/buzzer_gray.png",
         },
         {
             "Manual sampling",
             ":/commonMenus/Image/commonMenus/manual_sampling.png",
             ":/commonMenus/Image/commonMenus/manual_sampling_p.png",
             ":/commonMenus/Image/commonMenus/manual_sampling_gray.png",
         },
         {
             "Saving data",
             ":/commonMenus/Image/commonMenus/event_data_save.png",
             ":/commonMenus/Image/commonMenus/event_data_save_p.png",
             ":/commonMenus/Image/commonMenus/event_data_save_gray.png",
         },
         {
             "Message",
             ":/commonMenus/Image/commonMenus/info.png",
             ":/commonMenus/Image/commonMenus/info_p.png",
             ":/commonMenus/Image/commonMenus/info_gray.png",
         },
         {
             "Free message",
             ":/commonMenus/Image/commonMenus/freeden_info.png",
             ":/commonMenus/Image/commonMenus/freeden_info_p.png",
            ":/commonMenus/Image/commonMenus/freeden_info_gray.png",
         },
         {
             "User function 1",
             ":/commonMenus/Image/commonMenus/user_function1.png",
             ":/commonMenus/Image/commonMenus/user_function1_p.png",
             ":/commonMenus/Image/commonMenus/user_function1_gray.png",
         },
         {
             "User function 2",
             ":/commonMenus/Image/commonMenus/user_function2.png",
             ":/commonMenus/Image/commonMenus/user_function2_p.png",
            ":/commonMenus/Image/commonMenus/user_function2_gray.png",
         },
         {
             "Time setting",
             ":/commonMenus/Image/commonMenus/time_data_setting.png",
             ":/commonMenus/Image/commonMenus/time_data_setting_p.png",
             ":/commonMenus/Image/commonMenus/time_data_setting_gray.png",
         },
         {
             "Test print",
             ":/commonMenus/Image/commonMenus/print_test.png",
             ":/commonMenus/Image/commonMenus/print_test_p.png",
             ":/commonMenus/Image/commonMenus/print_test_gray.png",
         },
         {
             "Show all data",
             ":/commonMenus/Image/commonMenus/show_all_data.png",
             ":/commonMenus/Image/commonMenus/show_all_data_p.png",
             ":/commonMenus/Image/commonMenus/show_all_data_gray.png",
         },
         {
             "Digital label",
             ":/commonMenus/Image/commonMenus/digital_label.png",
             ":/commonMenus/Image/commonMenus/digital_label_p.png",
             ":/commonMenus/Image/commonMenus/digital_label_gray.png",
         },
         {
             "Data retrieval",
             ":/commonMenus/Image/commonMenus/data_retrieval.png",
             ":/commonMenus/Image/commonMenus/data_retrieval_p.png",
             ":/commonMenus/Image/commonMenus/data_retrieval_gray.png",
         },
         {
             "Handwritten data",
             ":/commonMenus/Image/commonMenus/handwritten_info.png",
             ":/commonMenus/Image/commonMenus/handwritten_info_p.png",
             ":/commonMenus/Image/commonMenus/handwritten_info_gray.png",
         },
         {
             "FTP test",
             ":/commonMenus/Image/commonMenus/FTP_test.png",
             ":/commonMenus/Image/commonMenus/FTP_test_p.png",
             ":/commonMenus/Image/commonMenus/FTP_test_gray.png",
         },
         {
             "SNTP",
             ":/commonMenus/Image/commonMenus/time_adjust.png",
             ":/commonMenus/Image/commonMenus/time_adjust_p.png",
             ":/commonMenus/Image/commonMenus/time_adjust_gray.png",
         },
         {
             "MAIL",
             ":/commonMenus/Image/commonMenus/EMAL.png",
             ":/commonMenus/Image/commonMenus/EMAL_p.png",
             ":/commonMenus/Image/commonMenus/EMAL_gray.png",
         },
         {
             "Event trigger",
             ":/commonMenus/Image/commonMenus/event_trigger.png",
             ":/commonMenus/Image/commonMenus/event_trigger_p.png",
             ":/commonMenus/Image/commonMenus/event_trigger_gray.png",
         },
         {
             "Mail test",
             ":/commonMenus/Image/commonMenus/mail_test.png",
             ":/commonMenus/Image/commonMenus/mail_test_p.png",
             ":/commonMenus/Image/commonMenus/mail_test_gray.png",
         },
         {
             "Remove favourites",
             ":/commonMenus/Image/commonMenus/remove_favorites.png",
             ":/commonMenus/Image/commonMenus/remove_favorites_p.png",
             ":/commonMenus/Image/commonMenus/remove_favorites_gray.png",
         },
         {
             "Standard screen",
             ":/commonMenus/Image/commonMenus/standard_screen.png",
             ":/commonMenus/Image/commonMenus/standard_screen_p.png",
             ":/commonMenus/Image/commonMenus/standard_screen_gray.png",
         },
         {
             "Save display",
             ":/commonMenus/Image/commonMenus/save_display.png",
             ":/commonMenus/Image/commonMenus/save_display_p.png",
             ":/commonMenus/Image/commonMenus/save_display_gray.png",
         },
         {
             "Text area",
             ":/commonMenus/Image/commonMenus/text_area.png",
             ":/commonMenus/Image/commonMenus/text_area_p.png",
             ":/commonMenus/Image/commonMenus/text_area_gray.png",
         },
         {
             "Batch",
             ":/commonMenus/Image/commonMenus/batch.png",
             ":/commonMenus/Image/commonMenus/batch_p.png",
             ":/commonMenus/Image/commonMenus/batch_gray.png",
         },
         {
             "Reset timer",
             ":/commonMenus/Image/commonMenus/resetTimer.png",
             ":/commonMenus/Image/commonMenus/resetTimer_p.png",
             ":/commonMenus/Image/commonMenus/resetTimer_gray.png",
         },
         {
             "Screen storage",
             ":/commonMenus/Image/commonMenus/saveScreen.png",
             ":/commonMenus/Image/commonMenus/saveScreen_p.png",
             ":/commonMenus/Image/commonMenus/saveScreen_gray.png",
         },
         {
             "Save event",
             ":/commonMenus/Image/commonMenus/saveDisplayData.png",
             ":/commonMenus/Image/commonMenus/saveDisplayData_p.png",
             ":/commonMenus/Image/commonMenus/saveDisplayData_gray.png",
         },
         //T0D0

         {
             "Login",
             ":/commonMenus/Image/commonMenus/login.png",
             ":/commonMenus/Image/commonMenus/login_p.png",
             "",
         },
         {
             "Change password",
             ":/commonMenus/Image/commonMenus/changePasswd.png",
             ":/commonMenus/Image/commonMenus/changePasswd_p.png",
             "",
         },
         {
             "Ope lock",
             ":/commonMenus/Image/commonMenus/opeLuck .png",
             ":/commonMenus/Image/commonMenus/opeLuck_p.png",
             "",
         },



         //画面变更菜单
        {
            "Trend",
            ":/commonMenus/Image/commonMenus/trend.png",
            ":/commonMenus/Image/commonMenus/trend_p.png",
             ":/commonMenus/Image/commonMenus/trend_gray.png",
        },
        {
            "Digital",
            ":/commonMenus/Image/commonMenus/digital.png",
            ":/commonMenus/Image/commonMenus/digital_p.png",
             ":/commonMenus/Image/commonMenus/digital_gray.png",
        },
        {
            "Bar",
            ":/commonMenus/Image/commonMenus/bar.png",
            ":/commonMenus/Image/commonMenus/bar_p.png",
             ":/commonMenus/Image/commonMenus/bar_gray.png",
        },
        {
            "Overview",
            ":/commonMenus/Image/commonMenus/overView.png",
            ":/commonMenus/Image/commonMenus/overView_p.png",
             ":/commonMenus/Image/commonMenus/overView_gray.png",
        },
        {
            "Alarm summary",
            ":/commonMenus/Image/commonMenus/alert.png",
            ":/commonMenus/Image/commonMenus/alert_p.png",
             ":/commonMenus/Image/commonMenus/alert_gray.png",
        },
        {
            "Message summary",
            ":/commonMenus/Image/commonMenus/message.png",
            ":/commonMenus/Image/commonMenus/message_p.png",
             ":/commonMenus/Image/commonMenus/message_gray.png",
        },
        {
            "Memory summary",
            ":/commonMenus/Image/commonMenus/ram.png",
            ":/commonMenus/Image/commonMenus/ram_p.png",
             ":/commonMenus/Image/commonMenus/ram_gray.png",
        },
        {
            "Report",
            ":/commonMenus/Image/commonMenus/report_form.png",
            ":/commonMenus/Image/commonMenus/report_form_p.png",
             ":/commonMenus/Image/commonMenus/report_form_gray.png",
        },
        {
            "Modbus client",
            ":/commonMenus/Image/commonMenus/client.png",
            ":/commonMenus/Image/commonMenus/client_p.png",
             ":/commonMenus/Image/commonMenus/client_gray.png",
        },
        {
            "Modbus master",
            ":/commonMenus/Image/commonMenus/modbusHost.png",
            ":/commonMenus/Image/commonMenus/modbusHost_p.png",
             ":/commonMenus/Image/commonMenus/modbusHost_gray.png",
        },
        {
            "Log",
            ":/commonMenus/Image/commonMenus/journal.png",
            ":/commonMenus/Image/commonMenus/journal_p.png",
             ":/commonMenus/Image/commonMenus/journal_gray.png",
        },
        {
            "Multi panel",
            ":/commonMenus/Image/commonMenus/multi_screen.png",
            ":/commonMenus/Image/commonMenus/multi_screen_p.png",
             ":/commonMenus/Image/commonMenus/multi_screen_gray.png",
        },
        {
            "Save loading",
            ":/commonMenus/Image/commonMenus/save_loading.png",
            ":/commonMenus/Image/commonMenus/save_loading_p.png",
             ":/commonMenus/Image/commonMenus/save_loading_gray.png",
        },
        {
            "Network information",
            ":/commonMenus/Image/commonMenus/internet_info.png",
            ":/commonMenus/Image/commonMenus/internet_info_p.png",
             ":/commonMenus/Image/commonMenus/internet_info_gray.png",
        },
        {
            "System information",
            ":/commonMenus/Image/commonMenus/system_info.png",
            ":/commonMenus/Image/commonMenus/system_info_p.png",
             ":/commonMenus/Image/commonMenus/system_info_gray.png",
        },
        {
            "Initialize calibration",
            ":/commonMenus/Image/commonMenus/init_value_correction.png",
            ":/commonMenus/Image/commonMenus/init_value_correction_p.png",
             ":/commonMenus/Image/commonMenus/init_value_correction_gray.png",
        },
        {
            "Internal switch",
            ":/commonMenus/Image/commonMenus/internal_switch.png",
            ":/commonMenus/Image/commonMenus/internal_switch_p.png",
             ":/commonMenus/Image/commonMenus/internal_switch_gray.png",
        },
    };

    map.name = tempBtnInfo[ID][0];
    map.imagePress = tempBtnInfo[ID][1];
    map.imageRelease = tempBtnInfo[ID][2];
    map.imageGray = tempBtnInfo[ID][3];

    return map;
}


/*
 * 功能：
 * 	  窗体的构造函数
 * 参数：
 * 	  parent
 * 返回值：
 * 	  无
 */
WinRoot_GeneralMenu *WinRoot_GeneralMenu::instance()
{
    static WinRoot_GeneralMenu data;
    return &data;
}

WinRoot_GeneralMenu::WinRoot_GeneralMenu(QWidget *parent) :
    QWidget(parent)
{
    p_parent = parent;

    m_preAuthStatus = 0;
    m_preUserType = 3;

    initItem();

    setWidgetLayout();
    getItemFlag();

    connectSgSlot();
}

/*
 * 功能：
 * 	  析构函数，调用release函数，释放资源
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
WinRoot_GeneralMenu::~WinRoot_GeneralMenu()
{
    release();
}

/*
 * 功能：
 * 	  初始化窗体,new出各个对象,将所有按键全部放入list中
 * 参数：
 *   无
 * 返回值：
 * 	  无
 */
void WinRoot_GeneralMenu::initItem()
{
    /**
      * add by gehailang
      */
    this->setObjectName("WinrootGeneralMenu");
    this->setStyleSheet("#WinrootGeneralMenu{background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, "\
                        "stop: 0 rgb(0, 7, 164), stop: 1 rgb(0, 68, 249));"\
                        "border-width: 0px; border-style: solid;}");

//    p_frame = new QFrame(this);
//    p_frame->setFixedSize(this->width(), this->height());
//    p_frame->setFrameShape(QFrame::NoFrame);



    p_scrollArea = new QScrollArea(this);
    p_scrollArea->setFixedSize(DESKTOP_WIDTH-DESKTOP_WIDTH/20, DESKTOP_HEIGHT - DESKTOP_WIDTH/10);
    p_scrollArea->horizontalScrollBar()->setHidden(true);
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


    for(int i = MenuItemID_Buzzer;i <= MenuItemID_opeLuck;i++) {
//        itemList.value(i)->setItemID(i);
        itemList.push_back(new GeneralMenu_Item(i));
    }
    while (itemList.size()%5 != 0)
        itemList.push_back(new GeneralMenu_Item(-1));

}

/*
 * 功能：
 * 	  绑定每个按键的点击信号和槽函数
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
void WinRoot_GeneralMenu::connectSgSlot()
{
    connect(CfgChannel::instance(), SIGNAL(sigNormalConfigChanged(quint32)), this, SLOT(slotControlItem(quint32)));



    connect(Authorization::instance(), SIGNAL(sigAuthUpdate()), this, SLOT(slotAuthSettingChange()));

    for(int i = 0;i < itemList.size();++i) {
        connect(itemList.value(i), SIGNAL(sgItemRelease(int)), this, SLOT(slotItemClicked(int)));
    }
//    connect(WinOpeLoginDialog::instance(), SIGNAL(sigOpeLoginSuccess()), this, SLOT(slotOpeUnlockSuccess()));
}

void WinRoot_GeneralMenu::setWidgetLayout()
{
    int k = 0;

    for (int i =0; i < (itemList.size() + 5) / 5 ; ++i) {
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
 * 	  flag是一个64位的数，每一位对于list中button的显示与否,这个函数还需要到时再重写，现在先目前测试用
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
void WinRoot_GeneralMenu::getItemFlag()
{
    slotControlItem(ID_batch_Function_OnOff);
    slotControlItem(ID_NetWork_FTP_Client_OnOff);
    slotControlItem(ID_NetWork_SMTP_Client_OnOff);
    slotControlItem(ID_NetWork_SNTP_ClientOnOff);
    slotControlItem(ID_Timer_Timer_1_Type_Type);
    slotControlItem(ID_Security);
//    setSecurityItemStatus();
    //    slotSecurityChange();
}

int WinRoot_GeneralMenu::getPreAuthStatus()
{
    return m_preAuthStatus;
}

int WinRoot_GeneralMenu::getPreUserType()
{
    return m_preUserType;
}


/*
 * 功能：
 * 	  取消信号槽绑定，delete对象，在析构函数中使用
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
void WinRoot_GeneralMenu::release()
{
    /**取消绑定  delete各个对象，释放资源**/

    while(!itemList.isEmpty()) {
        disconnect(itemList.value(0), SIGNAL(sgItemRelease(int)), this, SLOT(slotItemClicked(int)));
        delete itemList.value(0);
        itemList.pop_front();
    }


}

void WinRoot_GeneralMenu::setItemHide(int ID, bool hide)
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

QList<MenuItemID> WinRoot_GeneralMenu::getLimitList(Security_ModuleAuth limitID)
{
    QList<MenuItemID> list;

    list.clear();

    switch (limitID) {
    case Security_ModuleAuth_Record:
        //发信号给记录对话框，关闭使能记录的一些按键
        break;
    case Security_ModuleAuth_Math:
        //发信号给记录对话框，关闭使能运算的一些按键
        break;
    case Security_ModuleAuth_Datasave:
    {
        //发信号给截屏关闭截屏按键
        list.append(MenuItemID_ManualSampling);
        list.append(MenuItemID_saveDisplay);
        list.append(MenuItemID_saveEvent);
        break;
    }
    case Security_ModuleAuth_Msg:
    {
        list.append(MenuItemID_Information);
        list.append(MenuItemID_FreeInformation);
        break;
    }
    case Security_ModuleAuth_Batch:
    {
        list.append(MenuItemID_batch);
        list.append(MenuItemID_textArea);
        break;
    }
    case Security_ModuleAuth_ACK:
    {
        //通知报警开关使能关闭
        break;
    }
    case Security_ModuleAuth_Comm:
    {
        list.append(MenuItemID_EMAL);
        list.append(MenuItemID_mailTest);
        list.append(MenuItemID_FTPTest);
        list.append(MenuItemID_PrintTest);
        break;
    }
    case Security_ModuleAuth_TouchOpe:
    {
        list.append(MenuItemID_standardScreen);
        list.append(MenuItemID_removeFavourites);
        list.append(MenuItemID_DigitalLabel);
        break;
    }
    case Security_ModuleAuth_Time:
    {
        list.append(MenuItemID_TimeSetting);
        list.append(MenuItemID_TimeAdjustment);
        break;
    }
    case Security_ModuleAuth_Setting:
    {
        //
        break;
    }
    case Security_ModuleAuth_EXTMedia:
    {
        list.append(WinItemID_SaveLoading);
        break;
    }
    case Security_ModuleAuth_SysOpe:
    {

        break;
    }
    case Security_ModuleAuth_OutPutOpe:
    {

        break;
    }
    default:
        break;
    }
    return list;
}

void WinRoot_GeneralMenu::setMenuItemGray(QList<qint8> limitList, bool gray)
{
    QList<MenuItemID> list;

    if (limitList.value(0) == -1) {
        for (int i = 0; i < itemList.size(); ++i) {
            if (itemList.value(i)->getItemID() != MenuItemID_login)
                itemList.value(i)->setItemEnable(true);
        }
        for (int i = 0; i < WinRoot_GeneralWin::instance()->getGeneralWinItemList().size(); ++i) {
            if (WinRoot_GeneralWin::instance()->getGeneralWinItemList().value(i)->getItemID() == WinItemID_SaveLoading ||
                    WinRoot_GeneralWin::instance()->getGeneralWinItemList().value(i)->getItemID() == WinItemID_InitValueCorrection)
                WinRoot_GeneralWin::instance()->getGeneralWinItemList().value(i)->setItemEnable(true);
        }
        return;
    }
    else if (limitList.value(0) == -2) {
        for (int i = Security_ModuleAuth_Record; i <= Security_ModuleAuth_OutPutOpe; ++i) {
            list = getLimitList((Security_ModuleAuth)i);
            for (int j = 0; j < list.size(); ++j) {
                if (list.value(j) <= MenuItemID_saveEvent) {
                    for (int k = 0; k < itemList.size(); ++k) {
                        if (list.value(j) == itemList.value(k)->getItemID()) {
                            itemList.value(k)->setItemEnable(false);
                        }
                    }
                }
                else {
                    QList<GeneralMenu_Item*> tempItemList = WinRoot_GeneralWin::instance()->getGeneralWinItemList();
                    for (int k = 0; k < tempItemList.size(); ++k) {
                        if (list.value(j) == tempItemList.value(k)->getItemID()) {
                            tempItemList.value(k)->setItemEnable(false);
                        }
                    }
                }
            }
        }
        return;
    }


    for (int i = 0; i < limitList.size(); ++i) {
        list.clear();
        list = getLimitList((Security_ModuleAuth)limitList.value(i));

        for (int j = 0; j < list.size(); ++j) {
            if (list.value(j) <= MenuItemID_saveEvent) {
                for (int k = 0; k < itemList.size(); ++k) {
                    if (list.value(j) == itemList.value(k)->getItemID()) {
                        itemList.value(k)->setItemEnable(gray);
                    }
                }
            }
            else {
                QList<GeneralMenu_Item*> tempItemList = WinRoot_GeneralWin::instance()->getGeneralWinItemList();
                for (int k = 0; k < tempItemList.size(); ++k) {
                    if (list.value(j) == tempItemList.value(k)->getItemID()) {
                        tempItemList.value(k)->setItemEnable(gray);
                    }
                }
            }
        }
    }
}

void WinRoot_GeneralMenu::setSecurityItemStatus()
{
    quint8 loginStatus = Authorization::instance()->getLoginStatus();

    if (loginStatus == 0) {
        m_preAuthStatus = 0;
        setItemHide(MenuItemID_login, true);
        setItemHide(MenuItemID_changePasswd, true);
        setItemHide(MenuItemID_opeLuck, true);
    }

    else if (loginStatus == 1) {
        m_preAuthStatus = 1;
        quint8 userType = Authorization::instance()->getLoginedUser()->getUserType();

        m_preUserType = userType;
        if (userType == 0 || userType == 1) {
            setItemHide(MenuItemID_login, false);
            setItemHide(MenuItemID_changePasswd, false);
            setItemHide(MenuItemID_opeLuck, true);

            int i = 0;
            for (; i < itemList.size(); ++i) {
                if (itemList.at(i)->getItemID() == MenuItemID_login)
                    break;
            }
            if (i == itemList.size())
                return;
            itemList.value(i)->setItemText("Logout");

        }
        else if (userType == 2) {
            setItemHide(MenuItemID_login, true);
            setItemHide(MenuItemID_changePasswd, true);
            setItemHide(MenuItemID_opeLuck, false);

            int i = 0;
            for (; i < itemList.size(); ++i) {
                if (itemList.at(i)->getItemID() == MenuItemID_opeLuck)
                    break;
            }
            if (i == itemList.size())
                return;

            if (Authorization::instance()->getLoginedUser()->getUserLevel() == 1) {
                itemList.value(i)->setItemStyle(":/commonMenus/Image/commonMenus/opeUnluck.png", \
                                                ":/commonMenus/Image/commonMenus/opeUnluck_p.png");
                itemList.value(i)->setItemText("Ope lock");
            }
            else if (Authorization::instance()->getLoginedUser()->getUserLevel() == 2){
                itemList.value(i)->setItemStyle(":/commonMenus/Image/commonMenus/opeLuck .png", \
                                                ":/commonMenus/Image/commonMenus/opeLuck_p.png");
                itemList.value(i)->setItemText("Ope unlock");
            }
        }

        updataItemAuthority();
    }
    else if (loginStatus == 2) {
        m_preAuthStatus = 2;
        setItemHide(MenuItemID_login, false);
        setItemHide(MenuItemID_changePasswd, false);
        setItemHide(MenuItemID_opeLuck, true);

        int i = 0;
        for (; i < itemList.size(); ++i) {
            if (itemList.at(i)->getItemID() == MenuItemID_login)
                break;
        }
        if (i == itemList.size())
            return;

        if (Authorization::instance()->isApprovalOperation()) { //无登录可操作

            itemList.value(i)->setItemText("Login");
        }
        else { //无登录不可操作，menu物理按键登陆
            itemList.value(i)->setItemText("Logout");
        }
        updataItemAuthority();
    }
}

void WinRoot_GeneralMenu::updataItemAuthority()
{


    QList<qint8> list; //-2为User模式，-1为admin 模式

    if ((Authorization::instance()->getLoginStatus() == 2 &&
         Authorization::instance()->isApprovalOperation())) {
        list.append(-2);
    }
    else if (Authorization::instance()->getLoginStatus() == 1) {

        quint8 userType = Authorization::instance()->getLoginedUser()->getUserType();

        if (userType == 0) {
            list.append(-1);
        }
        else if (userType == 1/* || userType == 2*/) {
            if (!Authorization::instance()->getLoginedUser()->isEnableAuth()) {
                for (int i = Security_ModuleAuth_Record; i <= Security_ModuleAuth_OutPutOpe; ++i) {
                    if (Authorization::instance()->getLoginedUser()->isLocked(i))
                        list.append(i);
                }
            }
            else {
                list.append(-1);
            }
        }
        else if (userType == 2) {
            if (Authorization::instance()->getLoginedUser()->getUserLevel() == 1) {
                list.append(-1);
            }
            else if (Authorization::instance()->getLoginedUser()->getUserLevel() == 2) {
                for (int i = Security_ModuleAuth_Record; i <= Security_ModuleAuth_OutPutOpe; ++i) {
                    if (Authorization::instance()->getLoginedUser()->isLocked(i))
                        list.append(i);
                }
            }
        }
    }

    setMenuItemGray(list, false);
}


void WinRoot_GeneralMenu::setWinParent(QWidget *parent)
{
    p_parent = parent;
}

void WinRoot_GeneralMenu::slotCloseSlide()
{

}

void WinRoot_GeneralMenu::slotSecurityChange()
{

    for (int i = 0;i < itemList.size(); ++i) {
        if (itemList.value(i)->getItemID() >= 0) {
            if (!itemList.value(i)->getItemEnable())
                itemList.value(i)->setItemEnable(true);
        }
    }
    for (int i = 0; i < WinRoot_GeneralWin::instance()->getGeneralWinItemList().size(); ++i) {
        if (!WinRoot_GeneralWin::instance()->getGeneralWinItemList().value(i)->getItemEnable())
            WinRoot_GeneralWin::instance()->getGeneralWinItemList().value(i)->setItemEnable(true);
    }

//    slotControlItem(ID_Security);
    setSecurityItemStatus();
}

void WinRoot_GeneralMenu::slotAuthSettingChange()
{
    slotControlItem(ID_Security);
}

void WinRoot_GeneralMenu::slotOpeUnlockSuccess()
{
    int i = 0;
    for (; i < itemList.size(); ++i) {
        if (itemList.at(i)->getItemID() == MenuItemID_opeLuck)
            break;
    }
    if (i == itemList.size())
        return;

    itemList.value(i)->setItemStyle(":/commonMenus/Image/commonMenus/opeUnluck.png", \
                                    ":/commonMenus/Image/commonMenus/opeUnluck_p.png");
        itemList.value(i)->setItemText("Ope lock");

}

/*
 * 功能：
 * 	  窗体大小改变事件，调整窗体的对象位置
 * 参数：
 * 	  event
 * 返回值：
 * 	  无
 */
void WinRoot_GeneralMenu::resizeEvent(QResizeEvent */*event*/)
{

}



/*
 * 功能：
 * 	  获取当前的位置
 * 参数：
 *
 * 返回值：
 *
 */
void WinRoot_GeneralMenu::mousePressEvent(QMouseEvent *event)
{
    m_beginY = event->y();
}

/*
 * 功能：
 * 	  属于鼠标来控制屏幕的上下滚动显示
 * 参数：
 *
 * 返回值：
 *
 */
void WinRoot_GeneralMenu::mouseMoveEvent(QMouseEvent *event)
{
    int d = event->y() - m_beginY;
    int value = p_scrollArea->verticalScrollBar()->value();
    p_scrollArea->verticalScrollBar()->setValue(value - d/10);
}

void WinRoot_GeneralMenu::mouseReleaseEvent(QMouseEvent */*event*/)
{
    QTimer::singleShot(300,this,SLOT(slotCloseSlide()));
//    slider_flag = false;
}



void WinRoot_GeneralMenu::slotControlItem(quint32 ID)
{
    if (ID == ID_batch_Function_OnOff) {
        if (DevConfigPtr->batchConfig.batchOnOff) {
            setItemHide(MenuItemID_batch, false);
            setItemHide(MenuItemID_textArea, false);
        }
        else {
            setItemHide(MenuItemID_batch, true);
            setItemHide(MenuItemID_textArea, true);
        }
        slotSecurityChange();
    }
    else if (ID == ID_NetWork_FTP_Client_OnOff) {
        if (DevConfigPtr->networkFTP.clientOnOff)
            setItemHide(MenuItemID_FTPTest, false);
        else
            setItemHide(MenuItemID_FTPTest, true);
        slotSecurityChange();
    }
    else if (ID == ID_NetWork_SMTP_Client_OnOff) {
        if (DevConfigPtr->networkSTMP.clientOnOff) {
            setItemHide(MenuItemID_EMAL, false);
            setItemHide(MenuItemID_mailTest, false);
        }
        else {
            setItemHide(MenuItemID_EMAL, true);
            setItemHide(MenuItemID_mailTest, true);
        }
        slotSecurityChange();
    }
    else if (ID == ID_NetWork_SNTP_ClientOnOff) {
        if (DevConfigPtr->networkSNTP.clientOnOff)
            setItemHide(MenuItemID_TimeAdjustment, false);
        else
            setItemHide(MenuItemID_TimeAdjustment, true);
        slotSecurityChange();
    }
    else if (ID == ID_Timer_Timer_1_Type_Type ||
             ID == ID_Timer_Timer_2_Type_Type ||
             ID == ID_Timer_Timer_3_Type_Type ||
             ID == ID_Timer_Timer_4_Type_Type) {
        for (int i = 0; i < 4; ++i) {
            if (DevConfigPtr->timerConfig.timerType[i]) {
                setItemHide(MenuItemID_resetTimer, false);
                break;
            }
            else
                setItemHide(MenuItemID_resetTimer, true);
        }
        slotSecurityChange();
    }
    else if (ID == ID_Security) {
        slotSecurityChange();
    }
    else
        return;

    for (int i = 0; i < m_hLayoutList.size(); ++i) {
        m_hLayoutList.value(i)->deleteLater();
    }

    m_hLayoutList.clear();
    int k = 0;

    for (int i =0; i < (itemList.size() + 5) / 5 ; ++i) {
        m_hLayoutList.append(new QHBoxLayout);
//        m_hLayoutList.value(i)->setSpacing(p_scrollArea->width() / 10);
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

//TODO,待删除
#include "../Threads/threaddataprocess.h"
#include "../Threads/threaddatasync.h"
#include "wintrendgrouppanel.h"
void WinRoot_GeneralMenu::slotItemClicked(int id)
{
    static bool testFlag = false;
    switch (id) {
    case MenuItemID_Buzzer:
        if (!testFlag) {
            testFlag = true;
            ThreadDataSync::instance()->setRecordStatus(true);
            ThreadDataProcess::instance()->setMathStatus(Math_Status_Type_On);
            ThreadDataRecord::instance()->slotSystemConfigChanged();
            WinMainFrame::instance()->getTrendGroup()->slotTrendConfigChanged(ID_Disp_Trend_Set_Direction);
        } else {
            testFlag = false;
            ThreadDataSync::instance()->setRecordStatus(false);
            ThreadDataProcess::instance()->setMathStatus(Math_Status_Type_Off);
        }

        break;
    case MenuItemID_ManualSampling:
    {

        break;
    }
    case MenuItemID_DataSave:
    {
        break;
    }
    case MenuItemID_Information:
    {
       DisPlayMessage *p_messageBar = new DisPlayMessage(WinMainFrame::instance());
       p_messageBar->show(false);
        break;
    }
    case MenuItemID_FreeInformation:
    {
        DisPlayMessage *p_messageBar = new DisPlayMessage(WinMainFrame::instance());
        p_messageBar->show(true);
        break;
    }
    case MenuItemID_UsrFunction1:
        EventAction::instance()->slotUserFunc(0);
        break;
    case MenuItemID_UsrFunction2:
        EventAction::instance()->slotUserFunc(1);
        break;
    case MenuItemID_TimeSetting:
    {
        WinSetTimeDialog::instance()->show(/*true*/);
        break;
    }
    case MenuItemID_PrintTest:
    {
        PrintTest* printest = new PrintTest;
        printest->setPrintAttribute();  // 获取打印相关的参数
        printest->PrinterImage(this);   // 打印当前画面
        break;
    }
    case MenuItemID_ShowData:

        break;
    case MenuItemID_DigitalLabel:

        break;
    case MenuItemID_DataRetriveal:
    {
        EventCalendar* p_calendar = new EventCalendar(WinMainFrame::instance());
        p_calendar->show();
        break;
    }
    case MenuItemID_HandwrittenData:
    {

        break;
    }
    case MenuItemID_FTPTest:
    {
        WinFTPDialog dialog;
        dialog.exec();
        break;
    }
    case MenuItemID_TimeAdjustment:
    {

        break;
    }
    case MenuItemID_EMAL:
    {
        WinEMailDialog dialog;
        dialog.exec();
        break;
    }
    case MenuItemID_eventTrigger:

        break;
    case MenuItemID_mailTest:

        break;
    case MenuItemID_removeFavourites:

        break;
    case MenuItemID_standardScreen:
    {
        WinStandardDialog dialog;
        dialog.exec();
        break;
    }
    case MenuItemID_saveDisplay:
    {

        break;
    }
    case MenuItemID_textArea:
    {
        WinTextAreaDialog::instance()->setDialogType(1);
        WinTextAreaDialog::instance()->exec();
        break;
    }
    case MenuItemID_batch:
    {
        WinBatchDialog::instance()->setDialogType(0);
        WinBatchDialog::instance()->setUserDefineStyle();
        WinBatchDialog::instance()->exec();
        break;
    }
//    case MenuItemID_saveAborted:

//        break;
    case MenuItemID_resetTimer:
    {
        WinTimerRestartDialog dialog;
        dialog.exec();
        break;
    }
//    case MenuItemID_matchTimer:

//        break;
//    case MenuItemID_pendingCertification:

//        break;
    case MenuItemID_screenStorage:

        break;
    case MenuItemID_saveEvent:

        break;
    case MenuItemID_login:
    {
        if (Authorization::instance()->isApprovalOperation()) {
            WinLoginDialog::instance()->initData();
            WinLoginDialog::instance()->setDialogStatus(true);
            WinLoginDialog::instance()->exec();
        }
        else {
            Authorization::instance()->logout();
            WinMainFrame::instance()->slotFrameChanged(); //跳转到主界面
        }
        break;
    }
    case MenuItemID_changePasswd:
    {
        WinChangePasswdDialog dialog;
        dialog.exec();
        break;
    }
    case MenuItemID_opeLuck:
    {

        int i = 0;
        for (; i < itemList.size(); ++i) {
            if (itemList.at(i)->getItemID() == MenuItemID_opeLuck)
                break;
        }
        if (i == itemList.size())
            return;

        if (Authorization::instance()->getLoginStatus() == 1 &&
                Authorization::instance()->getLoginedUser()->getUserType() == 2 &&
                Authorization::instance()->getLoginedUser()->getUserLevel() == 2) {

            WinOpeLoginDialog::instance()->exec();

        }
        else if (Authorization::instance()->getLoginStatus() == 1 &&
                Authorization::instance()->getLoginedUser()->getUserType() == 2 &&
                Authorization::instance()->getLoginedUser()->getUserLevel() == 1) {
            Authorization::instance()->logout();

            itemList.value(i)->setItemStyle(":/commonMenus/Image/commonMenus/opeLuck .png", \
                                            ":/commonMenus/Image/commonMenus/opeLuck_p.png");
            itemList.value(i)->setItemText("Ope unlock");

            WinMainFrame::instance()->slotFrameChanged(); //跳转到主界面
        }
        break;
    }
    default:
        break;
    }
}
