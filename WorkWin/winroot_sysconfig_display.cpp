#include "winroot_sysconfig_display.h"
#include <QDebug>
#include "./Common/handwritepad.h"

enum SwitchType     //开关类型
{
    GroupSwitch = 0,    //组开关
    TripLineSwitch1,     //分割线1开关
    TripLineSwitch2,       //分割行线2开关
    TripLineSwitch3,
    TripLineSwitch4
};

SwitchType s_type;

static int groupNum = 1, preGroupNum = 1;       //当前通道号和前一个通道号
static QString tripLineNum = "TripLine1";     //记录当前的tripLineNum

static QList<int> s_channelNumList;       //选中通道的obj对象

static SysConfigSlidingScreen * _pSlidingScreen;       //滑屏操作类

/**
 * start  WinRoot_SysConfig_Display
 */

/*
 * @设置显示功能的耳机菜单
 * @设置的功能有 基本设置/组设置/信息设置/趋势设置
 *      para : sysConfigSlidingScreen 切换屏幕效果类的对象
 *
 */


#define CONFIG_DISPLAY_BASIC                            "Config:Display settings:Screen display settings"
#define CONFIG_DISPLAY_MESSAGE                          "Config:Display settings:Message settings"
#define CONFIG_DISPLAY_GROUP                            "Config:Display settings:Group settings"
#define CONFIG_DISPLAY_TREND                                "Config:Display settings:Trend settings"

WinRoot_SysConfig_Display *WinRoot_SysConfig_Display::instance()
{
    static WinRoot_SysConfig_Display config_Display;
    return &config_Display;
}

WinRoot_SysConfig_Display::WinRoot_SysConfig_Display(QWidget *parent) :
    QWidget(parent)
{
    QList<QPair<QString, MenuType> > menuList;
    menuList.append(qMakePair(QString(CONFIG_DISPLAY_BASIC), MenuType_title)); //基本设置按钮
    menuList.append(qMakePair(QString(CONFIG_DISPLAY_GROUP), MenuType_title)); //组信息设置按钮
    menuList.append(qMakePair(QString(CONFIG_DISPLAY_MESSAGE), MenuType_title)); //信息设置按钮
    menuList.append(qMakePair(QString(CONFIG_DISPLAY_TREND), MenuType_title)); //趋势设置按钮
    p_displayConfig =  new WinConfig(menuList);

    p_displayConfig->getMenu(CONFIG_DISPLAY_BASIC)->setMenuIcon(QPixmap(":/setting/Image/setting/change.png"), \
                                                                                                                        QPixmap(":/setting/Image/setting/title.png"));
    p_displayConfig->getMenu(CONFIG_DISPLAY_MESSAGE)->setMenuIcon(QPixmap(":/setting/Image/setting/change.png"), \
                                                                                                                        QPixmap(":/setting/Image/setting/title.png"));
    p_displayConfig->getMenu(CONFIG_DISPLAY_GROUP)->setMenuIcon(QPixmap(":/setting/Image/setting/change.png"), \
                                                                                                                        QPixmap(":/setting/Image/setting/title.png"));
    p_displayConfig->getMenu(CONFIG_DISPLAY_TREND)->setMenuIcon(QPixmap(":/setting/Image/setting/change.png"), \
                                                                                                                        QPixmap(":/setting/Image/setting/title.png"));


//    p_displayConfig->getMenu(CONFIG_DISPLAY_BASIC)->setMenuTitleHintIcon(QPixmap(":/setting/Image/setting/title.png"));
//    p_displayConfig->getMenu(CONFIG_DISPLAY_MESSAGE)->setMenuTitleHintIcon(QPixmap(":/setting/Image/setting/title.png"));
//    p_displayConfig->getMenu(CONFIG_DISPLAY_GROUP)->setMenuTitleHintIcon(QPixmap(":/setting/Image/setting/title.png"));
//    p_displayConfig->getMenu(CONFIG_DISPLAY_TREND)->setMenuTitleHintIcon(QPixmap(":/setting/Image/setting/title.png"));

//    p_displayConfig->getMenu(CONFIG_DISPLAY_BASIC)->setMenuTitleIcon(QPixmap(":/setting/Image/setting/change.png"));
//    p_displayConfig->getMenu(CONFIG_DISPLAY_MESSAGE)->setMenuTitleIcon(QPixmap(":/setting/Image/setting/change.png"));
//    p_displayConfig->getMenu(CONFIG_DISPLAY_GROUP)->setMenuTitleIcon(QPixmap(":/setting/Image/setting/change.png"));
//    p_displayConfig->getMenu(CONFIG_DISPLAY_TREND)->setMenuTitleIcon(QPixmap(":/setting/Image/setting/change.png"));

    QVBoxLayout *pLayout = new QVBoxLayout;
    pLayout->addWidget(p_displayConfig);
    this->setLayout(pLayout);

    signalAndSlotConnect();
}

void WinRoot_SysConfig_Display::setSlidingScreen(SysConfigSlidingScreen *pSlidingScreen)
{
    _pSlidingScreen = pSlidingScreen;
}


void WinRoot_SysConfig_Display::signalAndSlotConnect()
{
    connect(p_displayConfig->getMenu(CONFIG_DISPLAY_BASIC), SIGNAL(clicked()), this, SLOT(slotDisplayBasicMenuClieked()));
    connect(p_displayConfig->getMenu(CONFIG_DISPLAY_MESSAGE), SIGNAL(clicked()), this, SLOT(slotDisplayMessageMenuClieked()));
    connect(p_displayConfig->getMenu(CONFIG_DISPLAY_GROUP), SIGNAL(clicked()), this, SLOT(slotDisplayGroupMenuClieked()));
    connect(p_displayConfig->getMenu(CONFIG_DISPLAY_TREND), SIGNAL(clicked()), this, SLOT(slotDisplayTrendMenuClieked()));
}

/*
 * 点击组按钮时响应的槽函数
 * 出现下一级菜单(组功能设置菜单)
 */
void WinRoot_SysConfig_Display::slotDisplayGroupMenuClieked()
{
    _pSlidingScreen->setNextWidget(DisplayConfig_Group::instace());
    _pSlidingScreen->moveToNextWidget();
}

/*
 * 点击基本设置时响应的槽函数
*/
void WinRoot_SysConfig_Display::slotDisplayBasicMenuClieked()
{
//    _pSlidingScreen->setNextWidget(WinRoot_SysConfig_Display_Basic::instace());
//    _pSlidingScreen->moveToNextWidget();
}

/*
 * 点击信息设置时响应的槽函数
*/
void WinRoot_SysConfig_Display::slotDisplayMessageMenuClieked()
{
    qDebug() << "message";
}

/*
 * 点击基趋势设置时响应的槽函数
*/
void WinRoot_SysConfig_Display::slotDisplayTrendMenuClieked()
{
    qDebug() << "trend";
}

/**
 * end  WinRoot_SysConfig_Display
 */



/********************************** group ************************************/

/**
 * start  DisplayConfig_Group
 */
/***********************************************************************
 *
 * 组功能设置菜单界面
 *
 * *******************************************************************/
#define CONFIG_DISPLAY_GROUP_NUM      "Config:Display settings:Group number"
#define DISPLAY_GROUP_SETTING         "Config:Display settings:Group settings"
#define DISPLAY_GROUP_SWITCH          "Config:Display settings:Group settings:%1:Group settings:On/Off"
#define DISPLAY_GROUP_NAME            "Config:Display settings:Group settings:%1:Group settings:Group name"
#define DISPLAY_GROUP_SET             "Config:Display settings:Group settings:%1:Group settings:Group set"
#define DISPLAY_GROUP_TRIPLINE        "Config:Display settings:Group settings:%1:Trip line%2"
#define DISPLAY_GROUP_TRIPLINESWITCH  "Config:Display settings:Group settings:%1:Trip line%2:On/Off"
#define DISPLAY_GROUP_TRIPLINE_COLOR  "Config:Display settings:Group settings:50:Trip line4:Color"
#define DISPLAY_GROUP_TRIPLINE_WIDTH  "Config:Display settings:Group settings:50:Trip line4:Line width"
#define DISPLAY_GROUP_TRIPLINE_POS    "Config:Display settings:Group settings:50:Trip line4:Position"
DisplayConfig_Group * DisplayConfig_Group::instace()
{
    static DisplayConfig_Group Config_Display_Group;
    return & Config_Display_Group;
}


DisplayConfig_Group::DisplayConfig_Group(QWidget *parent)
    : QWidget(parent)
{
    initMenuMenu();
    QVBoxLayout *pLayout = new QVBoxLayout;
    pLayout->addWidget(p_displaySetting);
    this->setLayout(pLayout);

    signalAndSlotConnect();
}

/*
 * 不同的组通道显示不同的菜单界面
 */
void DisplayConfig_Group::initMenuMenu()
{
    QList<QPair<QString, MenuType> > menuList;
    menuList.append(qMakePair(QString(CONFIG_DISPLAY_GROUP_NUM), MenuType_normal));
    menuList.append(qMakePair(QString(DISPLAY_GROUP_SETTING), MenuType_group));

    //QString channelNum = ConfigTreeObject::instance()->getConfigObject(CONFIG_DISPLAY_GROUP_NUM)->getData().toString();
    QString channelNum = "1";   //测试用
    menuList.append(qMakePair(QString(DISPLAY_GROUP_SWITCH).arg(channelNum), MenuType_normal));
    if (ConfigTreeObject::instance()->getConfigData(QString(DISPLAY_GROUP_SWITCH).arg(channelNum)).toBool())
    {
        menuList.append(qMakePair(QString(DISPLAY_GROUP_NAME).arg(channelNum), MenuType_normal));
        menuList.append(qMakePair(QString(DISPLAY_GROUP_SET).arg(channelNum), MenuType_normal));

        for (int i =0; i < 4; i++)
        {
            menuList.append(qMakePair(QString(DISPLAY_GROUP_TRIPLINE).arg(channelNum).arg(i + 1), MenuType_group));
            menuList.append(qMakePair(QString(DISPLAY_GROUP_TRIPLINESWITCH).arg(channelNum).arg(i + 1), MenuType_normal));
            if (ConfigTreeObject::instance()->getConfigData(QString(DISPLAY_GROUP_TRIPLINESWITCH).arg(channelNum).arg(i + 1)).toInt())
            {
                menuList.append(qMakePair(QString(DISPLAY_GROUP_TRIPLINE_COLOR).arg(channelNum).arg(i + 1), MenuType_normal));
                menuList.append(qMakePair(QString(DISPLAY_GROUP_TRIPLINE_POS).arg(channelNum).arg(i + 1), MenuType_normal));
                menuList.append(qMakePair(QString(DISPLAY_GROUP_TRIPLINE_WIDTH).arg(channelNum).arg(i + 1), MenuType_normal));
            }
        }
    }

    p_displaySetting =  new WinConfig(menuList);
}

void DisplayConfig_Group::signalAndSlotConnect()
{
    connect(p_displaySetting->getMenu(CONFIG_DISPLAY_GROUP_NUM), SIGNAL(clicked()), this, SLOT(slotDisplayBasicMenuClieked()));
}

/*
 * 组通道选择界面的槽函数
 *      设置SysConfigSlidingScreen的下一个界面为通道选择界面，并移动到下一界面
 */
void DisplayConfig_Group::slotDisplayNumMenuClick()
{
//    _pSlidingScreen->setNextWidget(DisplayConfig_Group_NUM::instance());
//    _pSlidingScreen->moveToNextWidget();
}

/*
 * 组开关响应的槽函数
*/
void DisplayConfig_Group::slotDisplaySwitch()
{
}

/*
** 分割线开关响应函数
*/
void DisplayConfig_Group::slotDisplayTripLineSwitch()
{
    MenuWidget *menu = static_cast<MenuWidget *>(sender());
    QString LineNum = menu->p_obj->parent()->objectName();
    //qDebug() << menu->p_obj->parent()->objectName();
    if (LineNum == "TripLine1")
        s_type = TripLineSwitch1;
    else if (LineNum == "TripLine2")
        s_type = TripLineSwitch2;
    else if (LineNum == "TripLine3")
        s_type = TripLineSwitch3;
    else if (LineNum == "TripLine4")
        s_type = TripLineSwitch4;

//    _pSlidingScreen->setNextWidget(DisplayConfig_Group_Switch::instace());
//    _pSlidingScreen->moveToNextWidget();
}

/*
 * 分割线宽度响应函数
*/
void DisplayConfig_Group::slotDisplayTripLineWidth()    //分割线宽度
{
    MenuWidget *menu = static_cast<MenuWidget *>(sender()); //得到点击对象
    tripLineNum = menu->p_obj->parent()->objectName();      //得到点击对象的父对象，通过父对象得到点击的是第几个分割线

//    _pSlidingScreen->setNextWidget(DisplayConfig_Group_TripLineWidth::instace());
//    _pSlidingScreen->moveToNextWidget();
}

/*
 * 分割线颜色设定响应的槽函数
*/
void DisplayConfig_Group::slotDisplyTripLineColor()     //分割线颜色
{
    MenuWidget *menu = static_cast<MenuWidget *>(sender());
    tripLineNum = menu->p_obj->parent()->objectName();

    //选择颜色的弹出框

//    QString path = QString(CONFIG_DISPLAY_GROUP_NUM).append(QString::number(groupNum)) + "/" + tripLineNum + "/GroupTripLineColor";
//    ConfigTreeObject::instance()->getConfigObject(path)->changeData(changeData);


    qDebug() << "color";
}

/*
 * 分割线位置响应的槽函数
*/
void DisplayConfig_Group::slotDisplayTripLinePos()      //分割线位置
{
    MenuWidget *menu = static_cast<MenuWidget *>(sender());
    tripLineNum = menu->p_obj->parent()->objectName();

    //选择位置的弹出框

    qDebug() << "pos";
}


/*
 * 选择通道响应的槽函数
 *  需要添加点击时的弹出框     1-20
 *
*/
void DisplayConfig_Group::slotShowSelectChannelWidget()
{
    QString path = QString(CONFIG_DISPLAY_GROUP_NUM).append(QString::number(groupNum));

    s_channelNumList.clear();
    for (int i = 0; i < 20; i++)
    {
        if (ConfigTreeObject::instance()->getConfigData(path + QString("/GroupChannelNum%1").arg(i + 1)).toInt() != 0)
        {
            s_channelNumList.append(ConfigTreeObject::instance()->getConfigData( \
                                   path + QString("/GroupChannelNum%1").arg(i + 1)).toInt());
        }
    }
    int count = s_channelNumList.count();

//   QList<int> channelNumList =
//    int cnt = channelNumList.count();
//    if (cnt != count)
//    {
//        s_channelNumList.clear();
//        for(int i = 0; i < 20; i++)
//        {
//            if(i < cnt) {
//                s_channelNumList.append(channelNumList.at(i));
//                ConfigTreeObject::instance()->getConfigObject(path + QString("/GroupChannelNum%1").arg(i + 1))->changeData(i/*channelNumList.at(i)*/);
//            } else {
//                ConfigTreeObject::instance()->getConfigObject(path + QString("/GroupChannelNum%1").arg(i + 1))->changeData(0);
//            }
//        }
//        p_groupChannelNum->p_obj->changeData(cnt);
//    } else {
//        bool changed = false;
//        for (int i = 0; i < cnt; i++)
//        {
//            if (s_channelNumList.at(i) != channelNumList.at(i)) {
//                changed = true;
//                break;
//            }
//        }
//        if (changed) {
//            s_channelNumList.clear();
//            for (int i = 0; i < cnt; i++)
//            {
//                s_channelNumList.append(channelNumList.at(i));
//                ConfigTreeObject::instance()->getConfigObject(path + QString("/GroupChannelNum%1").arg(i + 1))->changeData(i/*channelNumList.at(i)*/);
//            }
//            p_groupChannelNum->p_obj->changeData(0);
//            p_groupChannelNum->p_obj->changeData(cnt);
//        }
//    }
    //p_groupChannelNum->setMenuMenu(p_groupChannelNum->p_obj->getData().toString() + " / 20");
}

/*
 * @改变组名称的槽函数
 *  点击组名menu得到当前组名，把把当前组名设置进键盘的输入框
 *  绑定键盘确定按钮信号的和槽函数
*/
void DisplayConfig_Group::slotShowChangeGroupNameWidget()
{
    //把当前的groupName设置进虚拟键盘的输入框
    keyPad = new SoftKeyPad();
    keyPad->show(p_groupName->p_obj->getData().toString());
    //keyPad->resize(DESKTOP_WIDTH, DESKTOP_HEIGHT);
    connect(keyPad, SIGNAL(sigOkPressSignal()), this, SLOT(slotGetInputStr()));
    //从虚拟键盘输入框得到当前值
}

/*
 * @键盘点击确定后的槽函数
 *  得到键盘的输入框文本
 *  改变menu对象的值，关闭键盘
*/
void DisplayConfig_Group::slotGetInputStr()
{
    p_groupName->p_obj->changeData(keyPad->getInputStr());
    p_groupName->setMenuMenu(p_groupName->p_obj->getData().toString());
    keyPad->close();
}



/**
 * end  DisplayConfig_Group
 */





///**
// * start  DisplayConfig_Group_NUM
// */

///***********************************************************************************
//**
//** 选择组号码界面  num = 50
//**
// * ********************************************************************************/
//DisplayConfig_Group_NUM *DisplayConfig_Group_NUM::instance()
//{
//    static DisplayConfig_Group_NUM Config_Display_Group_Num;
//    return & Config_Display_Group_Num;
//}

//DisplayConfig_Group_NUM::DisplayConfig_Group_NUM(QWidget *parent)
//    : WinWidget(parent)
//{
//    initMenuMenu();
//    signalAndSlotConnect();
//}

//DisplayConfig_Group_NUM::~DisplayConfig_Group_NUM()
//{
//    for (int i = 0; i < 50; i++)
//    {
//        delete p_groupNumItem[i];
//        p_groupNumItem[i] = NULL;
//    }
//}

///*
// * 添加组选择界面menu
// */
//void DisplayConfig_Group_NUM::initMenuMenu()
//{
//    for(int i = 0; i < 50; i++)
//    {
//        p_groupNumItem[i] = new MenuWidget(IsItem, QString(CONFIG_DISPLAY_GROUP_NUM).append(QString::number(i + 1)));
//        this->addMenu(p_groupNumItem[i]);
//    }

//    this->p_vlLayout->addStretch();
//}

///*
// * 绑定组界面menu信号槽
//*/
//void DisplayConfig_Group_NUM::signalAndSlotConnect()
//{
//    for ( int i = 0; i < 50; i++)
//    {
//        connect(p_groupNumItem[i], SIGNAL(clicked()), this, SLOT(slotChangeGroupNum()));
//    }
//}

///*
// * 点击某通道号的响应槽函数
// *      得到点击menu的对象，并得到对象中的值(也就是通道号)，
// *      设置全局当前通道号groupNum，
// *      设置当前窗体的上一级窗体，并移动到上一级窗体
// */
//void DisplayConfig_Group_NUM::slotChangeGroupNum()
//{
//    MenuWidget * menu = static_cast<MenuWidget *>(sender());
//    groupNum = menu->p_textLabel->text().toInt();

//    if (preGroupNum != groupNum)
//        DisplayConfig_Group::instace()->initMenuMenu();

//    _pSlidingScreen->setPreWidget(DisplayConfig_Group::instace());
//    _pSlidingScreen->moveToPreWidget();
//}

///**
// * end  DisplayConfig_Group_NUM
// */


///*
// * @ 组设置中的开关  显示界面只有On/Off两个menu
// */
//DisplayConfig_Group_Switch *DisplayConfig_Group_Switch::instace()
//{
//    static DisplayConfig_Group_Switch config_Group_switch;
//    return &config_Group_switch;
//}

//DisplayConfig_Group_Switch::DisplayConfig_Group_Switch(QWidget *parent)
//    :WinWidget(parent)
//{
//    initMenuMenu();
//    signalAndSlotConnect();
//}

//DisplayConfig_Group_Switch::~DisplayConfig_Group_Switch()
//{
//    delete p_groupSwitch[0];
//    p_groupSwitch[0] = NULL;

//    delete p_groupSwitch[1];
//    p_groupSwitch[1] = NULL;
//}


//void DisplayConfig_Group_Switch::initMenuMenu()
//{
//    //QString switchpath = QString(CONFIG_DISPLAY_GROUP_NUM).append(QString::number(groupNum).append("/GroupSwitch"));
//    p_groupSwitch[0] = new MenuWidget(IsItem, true);
//    p_groupSwitch[1] = new MenuWidget(IsItem, false);

//    this->addMenu(p_groupSwitch[0]);
//    this->addMenu(p_groupSwitch[1]);

//    this->p_vlLayout->addStretch();
//}

//void DisplayConfig_Group_Switch::signalAndSlotConnect()
//{
//    for (int i = 0; i < 2; i++)
//    {
//        connect(p_groupSwitch[i], SIGNAL(clicked()), this, SLOT(slotSwitchSelected()));
//    }
//}

///*
// * 点击menu时响应的槽函数
//*/
//void DisplayConfig_Group_Switch::slotSwitchSelected()
//{
//    MenuWidget * menu = static_cast<MenuWidget *>(sender());    //得到点击的对象
//    QString path = getPath();   //得到路径

//    ConfigTreeObject::instance()->getConfigObject(path)->changeData(menu->p_textLabel->text() == "On" ? true : false);
//    if (ConfigTreeObject::instance()->getConfigObject(path)->getChangedFlag())
//        DisplayConfig_Group::instace()->initMenuMenu();
//    _pSlidingScreen->setPreWidget(DisplayConfig_Group::instace());
//    _pSlidingScreen->moveToPreWidget();
//}

///*
// * 得到开关的路径      如： 组开关  分割线1路径....
//*/
//QString DisplayConfig_Group_Switch::getPath()
//{
//    QString path = QString(CONFIG_DISPLAY_GROUP_NUM).append(QString::number(groupNum));
//    switch (s_type) {
//    case GroupSwitch:
//        return (path + "/GroupSwitch");
//        break;
//    case TripLineSwitch1:
//        return (QString(path + "/TripLine%1/GroupTripLineSwitch").arg(1));
//        break;
//    case TripLineSwitch2:
//        return (QString(path + "/TripLine%1/GroupTripLineSwitch").arg(2));
//        break;
//    case TripLineSwitch3:
//        return (QString(path + "/TripLine%1/GroupTripLineSwitch").arg(3));
//        break;
//    case TripLineSwitch4:
//        return (QString(path + "/TripLine%1/GroupTripLineSwitch").arg(4));
//        break;
//    default:
//        break;
//    }
//}




///*
// * 分割线宽度
//*/
//DisplayConfig_Group_TripLineWidth *DisplayConfig_Group_TripLineWidth::instace()
//{
//    static DisplayConfig_Group_TripLineWidth config_TripLineWidth;
//    return & config_TripLineWidth;
//}

//DisplayConfig_Group_TripLineWidth::DisplayConfig_Group_TripLineWidth(QWidget *parent)
//    :WinWidget(parent)
//{
//    initMenuMenu();
//    signalAndSlotConnect();
//}

//DisplayConfig_Group_TripLineWidth::~DisplayConfig_Group_TripLineWidth()
//{
//    for (int i = 0; i < 3; i++)
//    {
//        delete p_tripLineWidth[i];
//        p_tripLineWidth[i] = NULL;
//    }
//}

///*
// * 界面
//*/
//void DisplayConfig_Group_TripLineWidth::initMenuMenu()
//{
//    p_tripLineWidth[0] = new MenuWidget(tr("标准"));
//    p_tripLineWidth[1] = new MenuWidget(tr("粗"));
//    p_tripLineWidth[2] = new MenuWidget(tr("细"));

//    this->addMenu(p_tripLineWidth[1]);
//    this->addMenu(p_tripLineWidth[0]);
//    this->addMenu(p_tripLineWidth[2]);

//    this->p_vlLayout->addStretch();
//}

//void DisplayConfig_Group_TripLineWidth::signalAndSlotConnect()
//{
//    for (int i = 0; i < 3; i++)
//    {
//        connect(p_tripLineWidth[i], SIGNAL(clicked()), this, SLOT(slotTripLineItemClicked()));
//    }
//}

///*
// * 响应点击函数
//*/
//void DisplayConfig_Group_TripLineWidth::slotTripLineItemClicked()
//{
//    MenuWidget *menu = static_cast<MenuWidget *>(sender());     //得到点击的对象
//    QString path = QString(CONFIG_DISPLAY_GROUP_NUM).append(QString::number(groupNum)) + "/" + tripLineNum + "/GroupTripLineWidth";
//    int changeData;
//    if (menu->p_textLabel->text() == "标准")
//        changeData = 0;
//    else if (menu->p_textLabel->text() == "粗")
//        changeData = 1;
//    else
//        changeData = 2;
//    ConfigTreeObject::instance()->getConfigObject(path)->changeData(changeData);
//    if (ConfigTreeObject::instance()->getConfigObject(path)->getChangedFlag())
//        DisplayConfig_Group::instace()->initMenuMenu();
//    _pSlidingScreen->setPreWidget(DisplayConfig_Group::instace());
//    _pSlidingScreen->moveToPreWidget();
//}




///********************************** basic ******************************/

///*
// * 当点击基本设置后出现的界面
//*/
//WinRoot_SysConfig_Display_Basic *WinRoot_SysConfig_Display_Basic::instace()
//{
//    static WinRoot_SysConfig_Display_Basic config_Display_Basic;
//    return &config_Display_Basic;
//}

//WinRoot_SysConfig_Display_Basic::WinRoot_SysConfig_Display_Basic(QWidget *parent)
//    : WinWidget(parent)
//{
//    initMenuMenu();
//    backlightSaverMode();
//    signalAndSlotConnect();

//    this->p_vlLayout->addStretch();
//}

//WinRoot_SysConfig_Display_Basic::~WinRoot_SysConfig_Display_Basic()
//{
//    delete p_basicBackground;
//    delete p_basicGroupChangeTime;
//    delete p_BasicAutoHomeWinTime;
//    delete p_BasicFirstWeekDay;
//    delete p_basicChangeFromMonitor;
//    delete p_basicBacklightSaverTime;

//    delete p_basicBacklightRestore;
//    delete p_basicBacklightSaverMode;
//    delete p_basicLCDBrightness;
//    delete p_basicBarDirection;
//    delete p_changeFromMonitor;

//    delete p_calendarDisplay;
//    delete p_monitor;
//    delete p_LcdProtectgroup;
//    delete p_Lcdgroup;
//    delete p_BarGroup;
//}

//void WinRoot_SysConfig_Display_Basic::initMenuMenu()
//{
//    int i = 0;
//    p_BarGroup = new MenuWidget(IsGroup);
//    p_BarGroup->setMenuGroup(tr("Bar graph"));
//    p_Lcdgroup = new MenuWidget(IsGroup);
//    p_Lcdgroup->setMenuGroup(tr("LCD"));
//    p_LcdProtectgroup = new MenuWidget(IsGroup);
//    p_LcdProtectgroup->setMenuGroup(tr("LCD protect"));

//    p_monitor = new MenuWidget(IsGroup);
//    p_monitor->setMenuGroup(tr("monitor"));

//    p_calendarDisplay = new MenuWidget(IsGroup);
//    p_calendarDisplay->setMenuGroup(tr("calendar display"));
//    p_changeFromMonitor = new MenuWidget(IsGroup);
//    p_changeFromMonitor->setMenuGroup(tr("Change from monitor"));

//    p_basicBarDirection = new MenuWidget(IsMenu, QString(CONFIG_DISPLAY_BASIC_BASICBARDIRECTION), 2, \
//           ConfigTreeObject::instance()->getConfigData(CONFIG_DISPLAY_BASIC_BASICBARDIRECTION).toInt() == 0 ? "Vertical" : "horizontal");

//    p_basicLCDBrightness = new MenuWidget(IsMenu, QString(CONFIG_DISPLAY_BASIC_BASICLCDBRIGHTNESS), 2, \
//           "Brightness " + ConfigTreeObject::instance()->getConfigData(CONFIG_DISPLAY_BASIC_BASICLCDBRIGHTNESS).toString());

//    QString saverData = getSaveModelStr();
//    p_basicBacklightSaverMode = new MenuWidget(IsMenu, QString(CONFIG_DISPLAY_BASIC_BASICBACKLIGHTSAVERMODER), 2, saverData);
//    QString saveTime = getSaveTime();
//    p_basicBacklightSaverTime = new MenuWidget(IsMenu, QString(CONFIG_DISPLAY_BASIC_BASICBACKLIGHTSAVERTIMER), 2, saveTime);
//    QString restoreStr = ConfigTreeObject::instance()->getConfigData(CONFIG_DISPLAY_BASIC_BASICBACKLIGHTRESTORE).\
//            toInt() == 0 ? tr("press key or touch") : tr("press key or touch or alarm");
//    p_basicBacklightRestore = new MenuWidget(IsMenu, QString(CONFIG_DISPLAY_BASIC_BASICBACKLIGHTRESTORE), 2, restoreStr);

//    QString background = ConfigTreeObject::instance()->getConfigData(CONFIG_DISPLAY_BASIC_BASICBACKGROUND).toInt() == 0 ?
//                tr("white") : tr("black");
//    p_basicBackground = new MenuWidget(IsMenu, QString(CONFIG_DISPLAY_BASIC_BASICBACKGROUND), 2, background);

//    QString groupchangeTime = getGroupChangetime();
//    p_basicGroupChangeTime = new MenuWidget(IsMenu, QString(CONFIG_DISPLAY_BASIC_BASICGROUPCHANGETIME), 2, groupchangeTime);
//    p_BasicAutoHomeWinTime = new MenuWidget(IsMenu, QString(CONFIG_DISPLAY_BASIC_BASICAUTOHOMEWINTIME), 1);

//    p_basicChangeFromMonitor = new MenuWidget(IsMenu, QString(CONFIG_DISPLAY_BASIC_BASICCHANGEFROMMONITOR), 1);   //监控模式改变
//    p_BasicFirstWeekDay= new MenuWidget(IsMenu, QString(CONFIG_DISPLAY_BASIC_BASICFIRSTWEEKDAY));        //


//    this->insertMenu(i++, p_BarGroup);  //title
//    this->insertMenu(i++, p_basicBarDirection);
//    this->insertMenu(i++, p_Lcdgroup);
//    this->insertMenu(i++, p_basicLCDBrightness);

//    this->insertMenu(i++, p_LcdProtectgroup);
//    this->insertMenu(i++, p_basicBacklightSaverMode);   //
//    this->insertMenu(i++, p_basicBacklightSaverTime);
//    this->insertMenu(i++, p_basicBacklightRestore);


//    this->insertMenu(i++, p_monitor);
//    this->insertMenu(i++, p_basicBackground);   //
//    this->insertMenu(i++, p_basicGroupChangeTime);
//    this->insertMenu(i++, p_BasicAutoHomeWinTime);

//    this->insertMenu(i++, p_calendarDisplay);
//    this->insertMenu(i++, p_BasicFirstWeekDay); //日历显示
//    this->insertMenu(i++, p_changeFromMonitor);
//    this->insertMenu(i++, p_basicChangeFromMonitor);    //监视器更改值
//}

///*
// * 通过保护模式判断保护时间和背光还原是否显示
//*/
//void WinRoot_SysConfig_Display_Basic::backlightSaverMode()
//{
//    if (p_basicBacklightSaverMode->p_obj->getData().toInt() == 0)
//    {
//        p_basicBacklightRestore->hide();    //背光还原
//        p_basicBacklightSaverTime->hide();  //保护时间
//    }
//    else
//    {
//        p_basicBacklightRestore->show();
//        p_basicBacklightSaverTime->show();
//    }
//}

///*
// * 得到保护的模式
// *      return : 不同保护模式对应的字符串
//*/
//QString WinRoot_SysConfig_Display_Basic::getSaveModelStr()
//{
//    int data = ConfigTreeObject::instance()->getConfigData(CONFIG_DISPLAY_BASIC_BASICBACKLIGHTSAVERMODER).toInt();
//    if (data == 0)
//        return tr("Off");
//    else if (data == 1)
//        return tr("darkening");  //变暗
//    else
//        return tr("go out");    //熄灭
//}

///*
// * 通过保护时间的配置值，得到保护时间的描述
// *  return: 保护时间的描述
//*/
//QString WinRoot_SysConfig_Display_Basic::getSaveTime()
//{
//    int time = ConfigTreeObject::instance()->getConfigData(CONFIG_DISPLAY_BASIC_BASICBACKLIGHTSAVERTIMER).toInt() / 60;
//    if (time == 60)
//        return "1h";
//    else
//        return QString::number(time) + "min";
//}

///*
// * 通过组切换时间的配置值，得到组切换时间的描述
// *      return : 组切换时间的描述
//*/
//QString WinRoot_SysConfig_Display_Basic::getGroupChangetime()
//{
//    int time = ConfigTreeObject::instance()->getConfigData(CONFIG_DISPLAY_BASIC_BASICGROUPCHANGETIME).toInt();
//    if (time == 5)
//        return "5s";
//    else if (time == 10)
//        return "10s";
//    else if (time == 20)
//        return "20s";
//    else if (time == 30)
//        return "30s";
//    else
//        return "1min";
//}


//void WinRoot_SysConfig_Display_Basic::signalAndSlotConnect()
//{
//    connect(p_basicLCDBrightness, SIGNAL(clicked()), this, SLOT(slotLcdBrightnessChoose()));
//    connect(p_basicBacklightSaverMode, SIGNAL(clicked()), this, SLOT(slotBacklightSaveModeSwitch()));
//    connect(p_basicChangeFromMonitor, SIGNAL(clicked()), this, SLOT(slotChangeFromMonitor()));
//    connect(p_basicBarDirection, SIGNAL(clicked()), this, SLOT(slotbarDirection()));
//    connect(p_BasicFirstWeekDay, SIGNAL(clicked()), this, SLOT(slotFirstWeekDay()));
//    connect(p_BasicAutoHomeWinTime, SIGNAL(clicked()), this, SLOT(slotAutoHomeWinTime()));
//    connect(p_basicGroupChangeTime, SIGNAL(clicked()), this, SLOT(slotGroupChangeTime()));
//    connect(p_basicBackground, SIGNAL(clicked()), this, SLOT(slotBackGround()));
//    connect(p_basicBacklightRestore, SIGNAL(clicked()), this, SLOT(slotBacklightRestore()));
//    connect(p_basicBacklightSaverTime, SIGNAL(clicked()), this, SLOT(slotBacklightSaverTime()));
//}

////辉度
//void WinRoot_SysConfig_Display_Basic::slotLcdBrightnessChoose()
//{
//    _pSlidingScreen->setNextWidget(WinRoot_SysConfig_Display_BasicLcdBrightness::instace(p_basicLCDBrightness));
//    _pSlidingScreen->moveToNextWidget();
//}

////保护模式
//void WinRoot_SysConfig_Display_Basic::slotBacklightSaveModeSwitch()
//{
//    _pSlidingScreen->setNextWidget(WinRoot_SysConfig_Display_BasicSaveMode::instace(p_basicBacklightSaverMode));
//    _pSlidingScreen->moveToNextWidget();
//}

////
//void WinRoot_SysConfig_Display_Basic::slotChangeFromMonitor()
//{

//}

////日历每周的第一天
//void WinRoot_SysConfig_Display_Basic::slotFirstWeekDay()
//{
//    _pSlidingScreen->setNextWidget(WinRoot_SysConfig_Display_BasicFirstWeekDay::instace(p_BasicFirstWeekDay));
//    _pSlidingScreen->moveToNextWidget();
//}

////跳转至默认画面
//void WinRoot_SysConfig_Display_Basic::slotAutoHomeWinTime()
//{
//    _pSlidingScreen->setNextWidget(WinRoot_SysConfig_Display_BasicAutoHomeWinTime::instace(p_BasicAutoHomeWinTime));
//    _pSlidingScreen->moveToNextWidget();
//}

////
//void WinRoot_SysConfig_Display_Basic::slotGroupChangeTime()
//{
//    _pSlidingScreen->setNextWidget(WinRoot_SysConfig_Display_BasicGroupChangeTime::instace(p_basicGroupChangeTime));
//    _pSlidingScreen->moveToNextWidget();
//}

////背景
//void WinRoot_SysConfig_Display_Basic::slotBackGround()
//{
//    _pSlidingScreen->setNextWidget(WinRoot_SysConfig_Display_BasicBackground::instace(p_basicBackground));
//    _pSlidingScreen->moveToNextWidget();
//}

////背光还原
//void WinRoot_SysConfig_Display_Basic::slotBacklightRestore()
//{
//    _pSlidingScreen->setNextWidget(WinRoot_SysConfig_Display_BasicBacklightRestore::instace(p_basicBacklightRestore));
//    _pSlidingScreen->moveToNextWidget();
//}

////背光保护时间
//void WinRoot_SysConfig_Display_Basic::slotBacklightSaverTime()
//{
//    _pSlidingScreen->setNextWidget(WinRoot_SysConfig_Display_BasicSaveTime::instace(p_basicBacklightSaverTime));
//    _pSlidingScreen->moveToNextWidget();
//}

////棒图方向
//void WinRoot_SysConfig_Display_Basic::slotbarDirection()
//{
//    _pSlidingScreen->setNextWidget(WinRoot_SysConfig_Display_BasicBarDirection::instace(p_basicBarDirection));
//    _pSlidingScreen->moveToNextWidget();
//}




///********************************** basic --> bar direction **********************************/
///*
// * 棒图方向的选择的界面
// *   para : barMenu 棒图方向选择的上一级对象，该对象保存了棒图的方向
//*/
//WinRoot_SysConfig_Display_BasicBarDirection *WinRoot_SysConfig_Display_BasicBarDirection::instace(MenuWidget *barMenu)
//{
//    static WinRoot_SysConfig_Display_BasicBarDirection config_disp_basicBarDirection(barMenu);
//    return &config_disp_basicBarDirection;
//}

//WinRoot_SysConfig_Display_BasicBarDirection::WinRoot_SysConfig_Display_BasicBarDirection(MenuWidget *barMenu, QWidget *parent)
//    : WinWidget(parent)
//{
//    p_barMenu = barMenu;
//    initMenuMenu();
//    signalAndSlotConnect();

//    this->p_vlLayout->addStretch();
//}

//WinRoot_SysConfig_Display_BasicBarDirection::~WinRoot_SysConfig_Display_BasicBarDirection()
//{
//    delete p_direction[1];
//    delete p_direction[0];
//}

///*
// * vertical / horizontal
//*/
//void WinRoot_SysConfig_Display_BasicBarDirection::initMenuMenu()
//{
//    p_direction[0] = new MenuWidget(tr("vertical"));
//    p_direction[1] = new MenuWidget(tr("horizontal"));

//    this->addMenu(p_direction[0]);
//    this->addMenu(p_direction[1]);
//}

//void WinRoot_SysConfig_Display_BasicBarDirection::signalAndSlotConnect()
//{
//    connect(p_direction[0], SIGNAL(clicked()), this, SLOT(slotbarDirection()));
//    connect(p_direction[1], SIGNAL(clicked()), this, SLOT(slotbarDirection()));
//}

///*
// * 点击某一个方向按钮后的响应槽函数
//*/
//void WinRoot_SysConfig_Display_BasicBarDirection::slotbarDirection()
//{
//    MenuWidget * menu = static_cast<MenuWidget *>(sender());
//    int changeData = menu->p_textLabel->text() == tr("vertical") ? 0 : 1;

//    p_barMenu->p_obj->changeData(changeData);   //上一级对象改变
//    if (p_barMenu->p_obj->getChangedFlag())     //判断上一级对象是否改变，并设置相应的改变数据在界面上的显示
//        p_barMenu->setMenuMenu(menu->p_textLabel->text());

//    _pSlidingScreen->setPreWidget(WinRoot_SysConfig_Display_Basic::instace());
//    _pSlidingScreen->moveToPreWidget();
//}





///******************************** 显示设置-->基本设置-->LCD辉度 *********************/

///**
// * @设置LCD的辉度
// * @param lcdBrightnessMenu : 上一级菜单的对象，保存LCD辉度配置的当前值
// */
//WinRoot_SysConfig_Display_BasicLcdBrightness *WinRoot_SysConfig_Display_BasicLcdBrightness::instace(MenuWidget *lcdBrightnessMenu)
//{
//    static WinRoot_SysConfig_Display_BasicLcdBrightness lcdBrightness(lcdBrightnessMenu);
//    return &lcdBrightness;
//}


//WinRoot_SysConfig_Display_BasicLcdBrightness::WinRoot_SysConfig_Display_BasicLcdBrightness(MenuWidget * lcdBrightnessMenu, QWidget *parent)
//    :WinWidget(parent)
//{
//    p_lcdBrightnessMenu = lcdBrightnessMenu;
//    initMenuMenu();
//    signalAndSlotConnect();
//    this->p_vlLayout->addStretch();
//}

//WinRoot_SysConfig_Display_BasicLcdBrightness::~WinRoot_SysConfig_Display_BasicLcdBrightness()
//{
//    for (int i = 0; i < 6; i++)
//    {
//        delete p_brightness[i];
//        p_brightness[i] = NULL;
//    }
//}

///* 配置值与配置值的描述对应关系
// *      1: 辉度1 / 2: 辉度2 / 3: 辉度3 / 4: 辉度4 / 5: 辉度5 / 6: 辉度6
//*/
//void WinRoot_SysConfig_Display_BasicLcdBrightness::initMenuMenu()
//{
//    for (int i = 0; i < 6; i++)
//    {
//        p_brightness[i] = new MenuWidget(tr("brightness") + " " + QString::number(i + 1));
//        this->addMenu(p_brightness[i]);
//    }
//}

//void WinRoot_SysConfig_Display_BasicLcdBrightness::signalAndSlotConnect()
//{
//    for (int i = 0; i < 6; i++)
//        connect(p_brightness[i], SIGNAL(clicked()), this, SLOT(slotLcdBrightness()));
//}

///*
// * 从界面选择辉度值的描述，转化为辉度配置值并进行相应的配置，滑动到上一级菜单
//*/
//void WinRoot_SysConfig_Display_BasicLcdBrightness::slotLcdBrightness()
//{
//    MenuWidget *menu = static_cast<MenuWidget *>(sender());

//    int brightnessValue = getBrightnessValue(menu->p_textLabel->text());
//    p_lcdBrightnessMenu->p_obj->changeData(brightnessValue);
//    if (p_lcdBrightnessMenu->p_obj->getChangedFlag())
//        p_lcdBrightnessMenu->setMenuMenu(menu->p_textLabel->text());

//    _pSlidingScreen->setPreWidget(WinRoot_SysConfig_Display_Basic::instace());
//    _pSlidingScreen->moveToPreWidget();
//}

///*
// * 把从界面上得到辉度的描述转化为辉度的配置值
// *      para : brightnessStr  界面上得到辉度的描述
// *      return : 辉度的配置值
//*/
//int WinRoot_SysConfig_Display_BasicLcdBrightness::getBrightnessValue(QString brightnessStr)
//{
//    QString bri = tr("brightness") + " ";
//    if (brightnessStr == bri + QString::number(6))
//        return 6;
//    else if (brightnessStr == bri + QString::number(5))
//        return 5;
//    else if (brightnessStr == bri + QString::number(4))
//        return 4;
//    else if (brightnessStr == bri + QString::number(3))
//        return 3;
//    else if (brightnessStr == bri + QString::number(2))
//        return 2;
//    else
//        return 1;
//}





///********************************* basic-->brightness ********************/
///*
// * 保护模式选择界面
// *      para: saveMenu 选择保护模式的上一级按钮对象,保存了当前的保护模式
//*/
//WinRoot_SysConfig_Display_BasicSaveMode *WinRoot_SysConfig_Display_BasicSaveMode::instace(MenuWidget *saveMenu)
//{
//    static WinRoot_SysConfig_Display_BasicSaveMode config_Disp_BasicSaveMode(saveMenu);
//    return &config_Disp_BasicSaveMode;
//}

//WinRoot_SysConfig_Display_BasicSaveMode::WinRoot_SysConfig_Display_BasicSaveMode(MenuWidget * saveMenu, QWidget *parent)
//    :WinWidget(parent)
//{
//    p_saveMenu = saveMenu;
//    initMenuMenu();
//    signalAndSlotConnect();
//    p_vlLayout->addStretch();
//}

//WinRoot_SysConfig_Display_BasicSaveMode::~WinRoot_SysConfig_Display_BasicSaveMode()
//{
//    delete p_saveMode[0];
//    delete p_saveMode[1];
//    delete p_saveMode[2];
//}

///*
// * 0: Off / 1: darkening(变暗) / 2: go out(熄灭)
//*/
//void WinRoot_SysConfig_Display_BasicSaveMode::initMenuMenu()
//{
//    p_saveMode[0] = new MenuWidget("Off");
//    p_saveMode[1] = new MenuWidget("darkening");
//    p_saveMode[2] = new MenuWidget("go out");

//    this->addMenu(p_saveMode[0]);
//    this->addMenu(p_saveMode[1]);
//    this->addMenu(p_saveMode[2]);
//}

//void WinRoot_SysConfig_Display_BasicSaveMode::signalAndSlotConnect()
//{
//    for (int i = 0; i < 3; i++)
//    {
//        connect(p_saveMode[i], SIGNAL(clicked()), this, SLOT(slotSaveMode()));
//    }
//}

///*
// * 从界面得到保护配置的描述，并把它转化为配置的值
//*/
//void WinRoot_SysConfig_Display_BasicSaveMode::slotSaveMode()
//{
//    MenuWidget * menu = static_cast<MenuWidget *>(sender());

//    int changeData = getSaveMode(menu->p_textLabel->text());
//    p_saveMenu->p_obj->changeData(changeData);

//    if (p_saveMenu->p_obj->getChangedFlag())
//    {
//        p_saveMenu->setMenuMenu(menu->p_textLabel->text());
//        //保护模式的配置值变类了之后，界面显示配置信息的menu才会跟着变
//        WinRoot_SysConfig_Display_Basic::instace()->backlightSaverMode();
//    }

//    _pSlidingScreen->setPreWidget(WinRoot_SysConfig_Display_Basic::instace());
//    _pSlidingScreen->moveToPreWidget();
//}

///*
// * 把从界面上获取配置字符串转化为保护模式的配置值
// *      return : 返回保护模式的配置值
//*/
//int WinRoot_SysConfig_Display_BasicSaveMode::getSaveMode(QString saveStr)
//{
//    if (saveStr == "Off")
//        return 0;
//    else if (saveStr == "darkening")
//        return 1;
//    else
//        return 2;
//}




///******************************basic --> brightsavetime ***********************************/
//WinRoot_SysConfig_Display_BasicSaveTime *WinRoot_SysConfig_Display_BasicSaveTime::instace(MenuWidget *saveTimeMenu)
//{
//    static WinRoot_SysConfig_Display_BasicSaveTime basicSaveTime(saveTimeMenu);
//    return &basicSaveTime;
//}

//WinRoot_SysConfig_Display_BasicSaveTime::WinRoot_SysConfig_Display_BasicSaveTime(MenuWidget * saveTimeMenu, QWidget *parent)
//    : WinWidget(parent)
//{
//    p_saveTimeMenu = saveTimeMenu;
//    initMenuMenu();
//    signalAndSlotConnect();
//    this->p_vlLayout->addStretch();
//}

//WinRoot_SysConfig_Display_BasicSaveTime::~WinRoot_SysConfig_Display_BasicSaveTime()
//{
//    for (int  i = 0; i < 6; i++)
//    {
//        delete p_saveTime[i];
//        p_saveTime[i] = NULL;
//    }
//}

//void WinRoot_SysConfig_Display_BasicSaveTime::initMenuMenu()
//{
//    p_saveTime[0] = new MenuWidget("1 min");
//    p_saveTime[1] = new MenuWidget("2 min");
//    p_saveTime[2] = new MenuWidget("5 min");
//    p_saveTime[3] = new MenuWidget("10 min");
//    p_saveTime[4] = new MenuWidget("30 min");
//    p_saveTime[5] = new MenuWidget("1 h");
//    for (int i = 0; i < 6; i++)
//    {
//        this->addMenu(p_saveTime[i]);
//    }
//}

//void WinRoot_SysConfig_Display_BasicSaveTime::signalAndSlotConnect()
//{
//    for (int i = 0; i < 6; i++)
//        connect(p_saveTime[i], SIGNAL(clicked()), this, SLOT(slotSaveTime()));
//}

////从界面得到保护时间配置的描述，并把它转化为保护时间的值
//void WinRoot_SysConfig_Display_BasicSaveTime::slotSaveTime()
//{
//    MenuWidget *menu = static_cast<MenuWidget *>(sender());
//    int time = getSaveTime(menu->p_textLabel->text());

//    p_saveTimeMenu->p_obj->changeData(time);
//    if (p_saveTimeMenu->p_obj->getChangedFlag())
//        p_saveTimeMenu->setMenuMenu(menu->p_textLabel->text());

//    _pSlidingScreen->setPreWidget(WinRoot_SysConfig_Display_Basic::instace());
//    _pSlidingScreen->moveToPreWidget();
//}

////把从界面上获取保护时间配置字符串转化为保护时间的配置值
//int WinRoot_SysConfig_Display_BasicSaveTime::getSaveTime(QString saveTime)
//{
//    if (saveTime == "1 h")
//        return 3600;
//    else if (saveTime == "30 min")
//        return 1800;
//    else if (saveTime == "10 min")
//        return 600;
//    else if (saveTime == "5 min")
//        return 300;
//    else if (saveTime == "2 min")
//        return 120;
//    else
//        return 60;
//}


///************************ 显示设置-->基本设置-->LCD 背光还原********************/
////背光还原操作与配置值的关系为  0 : 按键/触摸    1: 按键/触摸/警报
///*
// * para : backlightRestoreMenu 上一级menu对象，当切换背光还原配置时，便于修改menu上的显示设置
//*/
//WinRoot_SysConfig_Display_BasicBacklightRestore *WinRoot_SysConfig_Display_BasicBacklightRestore::instace(MenuWidget *backlightRestoreMenu)
//{
//    static WinRoot_SysConfig_Display_BasicBacklightRestore backlightRestore(backlightRestoreMenu);
//    return &backlightRestore;
//}

//WinRoot_SysConfig_Display_BasicBacklightRestore::WinRoot_SysConfig_Display_BasicBacklightRestore(MenuWidget * backlightRestoreMenu, QWidget *parent)
//    :WinWidget(parent)
//{
//    p_backlightRestoreMenu = backlightRestoreMenu;
//    initMenuMenu();
//    signalAndSlotConnect();
//    this->p_vlLayout->addStretch();
//}

//WinRoot_SysConfig_Display_BasicBacklightRestore::~WinRoot_SysConfig_Display_BasicBacklightRestore()
//{
//    delete p_backlightRestore[0];
//    delete p_backlightRestore[1];
//}

//void WinRoot_SysConfig_Display_BasicBacklightRestore::initMenuMenu()
//{
//    p_backlightRestore[0] = new MenuWidget(tr("press key or touch"));
//    p_backlightRestore[1] = new MenuWidget(tr("press key or touch or alarm"));

//    this->addMenu(p_backlightRestore[0]);
//    this->addMenu(p_backlightRestore[1]);
//}

//void WinRoot_SysConfig_Display_BasicBacklightRestore::signalAndSlotConnect()
//{
//    connect(p_backlightRestore[0], SIGNAL(clicked()), this, SLOT(slotRestore()));
//    connect(p_backlightRestore[1], SIGNAL(clicked()), this, SLOT(slotRestore()));
//}

////把从界面上获取保护时间配置字符串转化为保护时间的配置值
//int WinRoot_SysConfig_Display_BasicBacklightRestore::getSaveRestore(QString restoreStr)
//{
//    if (restoreStr == tr("press key or touch"))
//        return 0;
//    else
//        return 1;
//}

////从界面得到保护时间配置的描述，并把它转化为保护时间的值
//void WinRoot_SysConfig_Display_BasicBacklightRestore::slotRestore()
//{
//    MenuWidget *menu = static_cast<MenuWidget *>(sender());
//    int restoreValue = getSaveRestore(menu->p_textLabel->text());

//    p_backlightRestoreMenu->p_obj->changeData(restoreValue);
//    if (p_backlightRestoreMenu->p_obj->getChangedFlag())
//        p_backlightRestoreMenu->setMenuMenu(menu->p_textLabel->text());

//    _pSlidingScreen->setPreWidget(WinRoot_SysConfig_Display_Basic::instace());
//    _pSlidingScreen->moveToPreWidget();
//}



///************************ 显示设置-->基本设置-->背景颜色********************/

////设置背景颜色   背景颜色与配置值之间的关系为   0:白    1:黑

//WinRoot_SysConfig_Display_BasicBackground *WinRoot_SysConfig_Display_BasicBackground::instace(MenuWidget *backgroundMenu)
//{
//    static WinRoot_SysConfig_Display_BasicBackground background(backgroundMenu);
//    return &background;
//}

//WinRoot_SysConfig_Display_BasicBackground::WinRoot_SysConfig_Display_BasicBackground(MenuWidget * backgroundMenu, QWidget *parent)
//{
//    p_backgroundMenu = backgroundMenu;
//    initMenuMenu();
//    signalAndSlotConnect();
//    this->p_vlLayout->addStretch();
//}

//WinRoot_SysConfig_Display_BasicBackground::~WinRoot_SysConfig_Display_BasicBackground()
//{
//    delete p_background[1];
//    delete p_background[0];
//}

//void WinRoot_SysConfig_Display_BasicBackground::initMenuMenu()
//{
//    p_background[0] = new MenuWidget(tr("white"));
//    p_background[1] = new MenuWidget(tr("black"));

//    this->addMenu(p_background[0]);
//    this->addMenu(p_background[1]);
//}

//void WinRoot_SysConfig_Display_BasicBackground::signalAndSlotConnect()
//{
//    connect(p_background[0], SIGNAL(clicked()), this, SLOT(slotBackground()));
//    connect(p_background[1], SIGNAL(clicked()), this, SLOT(slotBackground()));
//}
////把从界面上获取背景颜色字符串转化为背景颜色的配置值
//int WinRoot_SysConfig_Display_BasicBackground::getBackgroundValue(QString backgroundvalue)
//{
//    if (backgroundvalue == tr("white"))
//        return 0;
//    else
//        return 1;
//}

////从界面得到背景颜色配置的描述，并把它转化为背景颜色的值
//void WinRoot_SysConfig_Display_BasicBackground::slotBackground()
//{
//    MenuWidget *menu = static_cast<MenuWidget *>(sender());
//    int backgroundvalue = getBackgroundValue(menu->p_textLabel->text());

//    p_backgroundMenu->p_obj->changeData(backgroundvalue);
//    if (p_backgroundMenu->p_obj->getChangedFlag())
//        p_backgroundMenu->setMenuMenu(menu->p_textLabel->text());

//    _pSlidingScreen->setPreWidget(WinRoot_SysConfig_Display_Basic::instace());
//    _pSlidingScreen->moveToPreWidget();
//}




///************************ 显示设置-->基本设置-->组自动切换时间********************/

////组自动切换时间    5 : 5s    10: 10s  20: 20s  30: 30s  60: 1min
//WinRoot_SysConfig_Display_BasicGroupChangeTime *WinRoot_SysConfig_Display_BasicGroupChangeTime::instace(MenuWidget *groupChangeTime)
//{
//    static WinRoot_SysConfig_Display_BasicGroupChangeTime changeTime(groupChangeTime);
//    return &changeTime;
//}

//WinRoot_SysConfig_Display_BasicGroupChangeTime::WinRoot_SysConfig_Display_BasicGroupChangeTime(MenuWidget * groupChangeTime, QWidget *parent)
//{
//    p_groupChangeTimeMenu = groupChangeTime;
//    initMenuMenu();
//    signalAndSlotConnect();

//    this->p_vlLayout->addStretch();
//}

//WinRoot_SysConfig_Display_BasicGroupChangeTime::~WinRoot_SysConfig_Display_BasicGroupChangeTime()
//{
//    for (int i = 5; i > 0; i++)
//    {
//        delete p_groupChangeTime[i - 1];
//        p_groupChangeTime[i - 1]= NULL;
//    }
//}

//void WinRoot_SysConfig_Display_BasicGroupChangeTime::initMenuMenu()
//{
//    p_groupChangeTime[0] = new MenuWidget(tr("5 s"));
//    p_groupChangeTime[1] = new MenuWidget(tr("10 s"));
//    p_groupChangeTime[2] = new MenuWidget(tr("20 s"));
//    p_groupChangeTime[3] = new MenuWidget(tr("30 s"));
//    p_groupChangeTime[4] = new MenuWidget(tr("1 min"));

//    for (int i = 0; i < 5; i++)
//        this->addMenu(p_groupChangeTime[i]);

//}

//void WinRoot_SysConfig_Display_BasicGroupChangeTime::signalAndSlotConnect()
//{
//    for (int i = 0; i < 5; i++)
//        connect(p_groupChangeTime[i], SIGNAL(clicked()), this, SLOT(slotGroupChnageTime()));
//}

////把从界面上获取跳转至组自动切换时间字符串转化为跳转至组自动切换时间的配置值
//int WinRoot_SysConfig_Display_BasicGroupChangeTime::getGroupChnageTime(QString timeStr)
//{}

////从界面得到组自动切换时间配置的描述，并把它转化为组自动切换时间的值
//void WinRoot_SysConfig_Display_BasicGroupChangeTime::slotGroupChnageTime()
//{
//    MenuWidget *menu = static_cast<MenuWidget *>(sender());
//    int backgroundvalue = getGroupChnageTime(menu->p_textLabel->text());

//    p_groupChangeTimeMenu->p_obj->changeData(backgroundvalue);
//    if (p_groupChangeTimeMenu->p_obj->getChangedFlag())
//        p_groupChangeTimeMenu->setMenuMenu(menu->p_textLabel->text());

//    _pSlidingScreen->setPreWidget(WinRoot_SysConfig_Display_Basic::instace());
//    _pSlidingScreen->moveToPreWidget();
//}





///************************ 显示设置-->基本设置-->跳转至默认画面********************/

///*跳转至默认画面开关   关闭自动跳转    0: Off
//                    打开自动跳转    1: 60    2: 120  3: 300  4: 600  5: 1200  6: 1800  7: 3600
//*/
//WinRoot_SysConfig_Display_BasicAutoHomeWinTime *WinRoot_SysConfig_Display_BasicAutoHomeWinTime::instace(MenuWidget *autoHomeWintime)
//{
//    static WinRoot_SysConfig_Display_BasicAutoHomeWinTime autoTime(autoHomeWintime);
//    return &autoTime;
//}

//WinRoot_SysConfig_Display_BasicAutoHomeWinTime::WinRoot_SysConfig_Display_BasicAutoHomeWinTime(MenuWidget * autoHomeWintime, QWidget *parent)
//    :WinWidget(parent)
//{
//    p_autoHomeWintimeMenu = autoHomeWintime;
//    initMenuMenu();
//    signalAndSlotConnect();
//    this->p_vlLayout->addStretch();
//}

//WinRoot_SysConfig_Display_BasicAutoHomeWinTime::~WinRoot_SysConfig_Display_BasicAutoHomeWinTime()
//{
//    for (int i = 7; i > 0; i++)
//    {
//        delete p_autoHomeWintime[i - 1];
//        p_autoHomeWintime[i - 1] = NULL;
//    }
//}

//void WinRoot_SysConfig_Display_BasicAutoHomeWinTime::initMenuMenu()
//{
//    p_autoHomeWintime[0] = new MenuWidget(tr("Off"));
//    p_autoHomeWintime[1] = new MenuWidget(tr("1 min"));
//    p_autoHomeWintime[2] = new MenuWidget(tr("2 min"));
//    p_autoHomeWintime[3] = new MenuWidget(tr("5 min"));
//    p_autoHomeWintime[4] = new MenuWidget(tr("10 min"));
//    p_autoHomeWintime[5] = new MenuWidget(tr("20 min"));
//    p_autoHomeWintime[6] = new MenuWidget(tr("30 min"));
//    p_autoHomeWintime[7] = new MenuWidget(tr("1 h"));

//    for (int i = 0; i < 8; i++)
//        this->addMenu(p_autoHomeWintime[i]);
//}

//void WinRoot_SysConfig_Display_BasicAutoHomeWinTime::signalAndSlotConnect()
//{
//    for (int i = 0; i < 8; i++)
//        connect(p_autoHomeWintime[i], SIGNAL(clicked()), this, SLOT(slotAutoHomeWinTime()));
//}

////把从界面上获取跳转至默认画面字符串转化为跳转至默认画面的配置值
//int WinRoot_SysConfig_Display_BasicAutoHomeWinTime::getAutoHomeWinTime(QString timeStr)
//{
//    if (timeStr == tr("Off"))
//        return 0;
//    else if (timeStr == "1 min")
//        return 60;
//    else if (timeStr == "2 min")
//        return 120;
//    else if (timeStr == "5 min")
//        return 300;
//    else if (timeStr == "10 min")
//        return 600;
//    else if (timeStr == "20 min")
//        return 1200;
//    else if (timeStr == "30 min")
//        return 1800;
//    else if (timeStr == "1h")
//        return 3600;
//}


////从界面得到跳转至默认画面配置的描述，并把它转化为跳转至默认画面的值
//void WinRoot_SysConfig_Display_BasicAutoHomeWinTime::slotAutoHomeWinTime()
//{
//    MenuWidget *menu = static_cast<MenuWidget *>(sender());
//    int time = getAutoHomeWinTime(menu->p_textLabel->text());

//    p_autoHomeWintimeMenu->p_obj->changeData(time);
//    if (p_autoHomeWintimeMenu->p_obj->getChangedFlag())
//        p_autoHomeWintimeMenu->setMenuMenu(menu->p_textLabel->text());

//    _pSlidingScreen->setPreWidget(WinRoot_SysConfig_Display_Basic::instace());
//    _pSlidingScreen->moveToPreWidget();
//}


///************************ 显示设置-->基本设置-->周的起始时间********************/
///*周的起始时间选择界面  关闭自动跳转    0: 星期日   1: 星期一
//*/
//WinRoot_SysConfig_Display_BasicFirstWeekDay *WinRoot_SysConfig_Display_BasicFirstWeekDay::instace(MenuWidget *weekdayMenu)
//{
//    static WinRoot_SysConfig_Display_BasicFirstWeekDay weekDay(weekdayMenu);
//    return &weekDay;
//}

//WinRoot_SysConfig_Display_BasicFirstWeekDay::WinRoot_SysConfig_Display_BasicFirstWeekDay(MenuWidget * weekdayMenu, QWidget *parent)
//    :WinWidget(parent)
//{
//    p_firstWeekDayMenu = weekdayMenu;
//    initMenuMenu();
//    signalAndSlotConnect();
//    this->p_vlLayout->addStretch();
//}

//WinRoot_SysConfig_Display_BasicFirstWeekDay::~WinRoot_SysConfig_Display_BasicFirstWeekDay()
//{
//    delete p_firstWeekDay[1];
//    delete p_firstWeekDay[0];
//}

//void WinRoot_SysConfig_Display_BasicFirstWeekDay::initMenuMenu()
//{
//    p_firstWeekDay[0] = new MenuWidget(tr("Sunday"));
//    p_firstWeekDay[1] = new MenuWidget(tr("Monday"));

//    this->addMenu(p_firstWeekDay[0]);
//    this->addMenu(p_firstWeekDay[1]);
//}

//void WinRoot_SysConfig_Display_BasicFirstWeekDay::signalAndSlotConnect()
//{
//    connect(p_firstWeekDay[0], SIGNAL(clicked()), this, SLOT(slotFirstWeekDay()));
//    connect(p_firstWeekDay[1], SIGNAL(clicked()), this, SLOT(slotFirstWeekDay()));
//}

////从界面得到周的起始时间配置的描述，并把它转化为周的起始时间的值
//void WinRoot_SysConfig_Display_BasicFirstWeekDay::slotFirstWeekDay()
//{
//    MenuWidget *menu = static_cast<MenuWidget *>(sender());
//    int time = menu->p_textLabel->text() == tr("Sunday") ? 0 : 1;

//    p_firstWeekDayMenu->p_obj->changeData(time);
//    if (p_firstWeekDayMenu->p_obj->getChangedFlag())
//        p_firstWeekDayMenu->setMenuMenu(menu->p_textLabel->text());

//    _pSlidingScreen->setPreWidget(WinRoot_SysConfig_Display_Basic::instace());
//    _pSlidingScreen->moveToPreWidget();
//}

