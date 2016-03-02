/*********************************************************************
 * Copyright(c) 2015, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：winroot_sysconfig_display.h
 * 概   要：设置界面中显示设置中每个界面的显示
 *
 * 当前版本：V1.0.0
 * 作   者：邱洪利
 * 完成日期：2015-8-23
 *
 * 修改版本：
 * 修改说明：
 * 修改作者：
 *
 ********************************************************************/
#ifndef WINROOT_SYSCONFIG_DISPLAY_H
#define WINROOT_SYSCONFIG_DISPLAY_H

#include <QWidget>
#include "winwidget.h"
#include "./GlobalData/configtreeobject.h"
#include "./Common/sysconfigslidingscreen.h"
#include "./Common/softkeypad.h"
#include "./Common/winconfig.h"

class WinRoot_SysConfig_Display_Group_NUM;

/*basic*/

#define CONFIG_DISPLAY_BASIC_BASICCHANGEFROMMONITOR     "Config/Display/Basic/BasicChangeFromMonitor"
#define CONFIG_DISPLAY_BASIC_BASICFIRSTWEEKDAY          "Config/Display/Basic/BasicFirstWeekday"
#define CONFIG_DISPLAY_BASIC_BASICAUTOHOMEWINTIME       "Config/Display/Basic/BasicAutoHomeWinTime"
#define CONFIG_DISPLAY_BASIC_BASICGROUPCHANGETIME       "Config/Display/Basic/BasicGroupChangeTime"
#define CONFIG_DISPLAY_BASIC_BASICBACKGROUND            "Config/Display/Basic/BasicBackground"
#define CONFIG_DISPLAY_BASIC_BASICBACKLIGHTRESTORE      "Config/Display/Basic/BasicBacklightRestore"
#define CONFIG_DISPLAY_BASIC_BASICBACKLIGHTSAVERTIMER   "Config/Display/Basic/BasicBacklightSaverTime"
#define CONFIG_DISPLAY_BASIC_BASICBACKLIGHTSAVERMODER   "Config/Display/Basic/BasicBacklightSaverMode"
#define CONFIG_DISPLAY_BASIC_BASICLCDBRIGHTNESS         "Config/Display/Basic/BasicLCDBrightness"
#define CONFIG_DISPLAY_BASIC_BASICBARDIRECTION          "Config/Display/Basic/BasicBarDirection"

#define CONFIG_DISPLAY_BASIC                            "Config/Display/Basic"

/*trend*/
#define CONFIG_DISPLAY_TREND_TRENDMSGINTERVALCHANGE         "Config/Display/Trend/TrendMsgIntervalChange"
#define CONFIG_DISPLAY_TREND_TRENDMSGPOWERDOWN              "Config/Display/Trend/TrendMsgPowerDown"
#define CONFIG_DISPLAY_TREND_TRENDMSGWRITEMODE              "Config/Display/Trend/TrendMsgWriteMode"
#define CONFIG_DISPLAY_TREND_TRENDPARTIALONOFF              "Config/Display/Trend/TrendPartialOnOff"
#define CONFIG_DISPLAY_TREND_TRENDSCALENUMBER               "Config/Display/Trend/TrendScaleNumber"
#define CONFIG_DISPLAY_TREND_TRENDSCALETYPE                 "Config/Display/Trend/TrendScaleType"
#define CONFIG_DISPLAY_TREND_TRENDSCALEMODE                 "Config/Display/Trend/TrendScaleMode"
#define CONFIG_DISPLAY_TREND_TRENDGRID                      "Config/Display/Trend/TrendGrid"
#define CONFIG_DISPLAY_TREND_TRENDLINEWIDTH                 "Config/Display/Trend/TrendLineWidth"
#define CONFIG_DISPLAY_TREND_TRENDCLEAR                     "Config/Display/Trend/TrendClear"
#define CONFIG_DISPLAY_TREND_TRENDDISPLAYDIRECTION          "Config/Display/Trend/TrendDisplayDirection"
#define CONFIG_DISPLAY_TREND_TRENDINTERVALSECOND            "Config/Display/Trend/TrendIntervalSecond"
#define CONFIG_DISPLAY_TREND_TRENDINTERVALSECONDSWITCH      "Config/Display/Trend/TrendIntervalSecondSwitch"
#define CONFIG_DISPLAY_TREND_TRENDINTERVAL_S                "Config/Display/Trend/TrendInterval_s"


/*
 * @主界面设置栏显示设置的title界面      基本设置/组设定/信息/趋势
 *      path : 基本设置 "Config/Display/Basic"
 *              组设定  "Config/Display/Group"
 *              信息    "Config/Display/Message"
 *              趋势    "Config/Display/Trend"
 */
class WinRoot_SysConfig_Display : public QWidget
{
    Q_OBJECT
public:
    static WinRoot_SysConfig_Display *instance(); //
    void setSlidingScreen(SysConfigSlidingScreen *pSlidingScreen);

private:
    explicit WinRoot_SysConfig_Display(QWidget *parent = 0);
    void signalAndSlotConnect();

signals:

public slots:
    void slotDisplayBasicMenuClieked();
    void slotDisplayMessageMenuClieked();
    void slotDisplayGroupMenuClieked();
    void slotDisplayTrendMenuClieked();

private:
    WinConfig *                 p_displayConfig;
};




/*********************************  group  *******************************/

/*
 * @显示界面设置中的组设定界面
 */
class DisplayConfig_Group : public QWidget
{
    Q_OBJECT
public:
    static DisplayConfig_Group *instace();
    void initMenuMenu();

private:
    explicit DisplayConfig_Group(QWidget *parent = 0);

    void signalAndSlotConnect();       //信号和槽链接

signals:

public slots:
    void slotDisplayNumMenuClick();     //组选择
    void slotDisplaySwitch();       //组开关
    void slotShowSelectChannelWidget();    //选择组通道  maxChannelNum = 20
    void slotShowChangeGroupNameWidget();   //修改组名
    void slotGetInputStr();     //组改名时得到组键盘输入框文字
    void slotDisplayTripLineSwitch();    //分割线开关
    void slotDisplyTripLineColor();     //分割线颜色
    void slotDisplayTripLineWidth();    //分割线宽度
    void slotDisplayTripLinePos();      //分割线位置

private:
    MenuWidget *                p_groupNumSelectMenu;       //组选择
    MenuWidget *                p_groupChannelNumSelect;    //通道选择
    MenuWidget *                p_groupName;        //组名
    MenuWidget *                p_groupChannelNum;  //通道名称
    MenuWidget *                p_groupSwitch;      //通道开关
    MenuWidget *                p_groupTrip[4];     //分割线title
    MenuWidget *                p_groupTripLineSwitch[4];   //分割线开关
    MenuWidget *                p_tripLineColor[4];     //  groupnum/triplineNum/tripLinecolor
    MenuWidget *                p_tripLineWidth[4];     //分割线宽度
    MenuWidget *                p_tripLinePos[4];       //分割线位置
    MenuWidget *                p_groupSetingGroup;     //设置组title

    WinConfig *                 p_displaySetting;

    SoftKeyPad *                keyPad;     //键盘
};


///*
// * @ 组设置中组通道号码选择界面  1-50通道
// */
//class DisplayConfig_Group_NUM : public WinWidget
//{
//    Q_OBJECT
//public:
//    static DisplayConfig_Group_NUM *instance();

//private:
//    explicit DisplayConfig_Group_NUM(QWidget *parent = 0);
//    ~DisplayConfig_Group_NUM();

//    void initMenuMenu();
//    void signalAndSlotConnect();

//signals:

//public slots:
//    void slotChangeGroupNum();      //组号码

//private:
//    MenuWidget *                p_groupNumItem[50];
//};


///*
// * @ 组设置中的开关  显示界面只有On/Off两个menu
// */
//class DisplayConfig_Group_Switch :public WinWidget
//{
//    Q_OBJECT
//public:
//    static DisplayConfig_Group_Switch *instace();

//private:
//    explicit DisplayConfig_Group_Switch(QWidget *parent = 0);
//    ~DisplayConfig_Group_Switch();

//    void initMenuMenu();
//    void signalAndSlotConnect();
//    QString getPath();

//signals:

//public slots:
//    void slotSwitchSelected();      //开关On/Off

//private:
//    MenuWidget *                p_groupSwitch[2];
//};

///*
//** 组设定中分割线宽度选择界面  界面包含三个menu：粗/中/细
//*/
//class DisplayConfig_Group_TripLineWidth : public WinWidget
//{
//    Q_OBJECT
//public:
//    static DisplayConfig_Group_TripLineWidth *instace();

//private:
//    explicit DisplayConfig_Group_TripLineWidth(QWidget *parent = 0);
//    ~DisplayConfig_Group_TripLineWidth();

//    void initMenuMenu();
//    void signalAndSlotConnect();

//public slots:
//    void slotTripLineItemClicked();

//private:
//    MenuWidget *                 p_tripLineWidth[3];    //0 标准  1 粗  2 细
//};






///*********************************  display --> basic  *****************************/

///*
// * 显示设置的基本设置类
//*/
//class WinRoot_SysConfig_Display_Basic : public WinWidget
//{
//    Q_OBJECT
//public:
//    static WinRoot_SysConfig_Display_Basic *instace();
//    void backlightSaverMode();  //通过保护模式判断保护时间和背光还原是否显示

//private:
//    explicit WinRoot_SysConfig_Display_Basic(QWidget *parent = 0);
//    ~WinRoot_SysConfig_Display_Basic();

//    void initMenuMenu();
//    void signalAndSlotConnect();
//    QString getSaveModelStr();  //通过配置值得到保护的模式的描述
//    QString getSaveTime();  //通过保护时间的配置值，得到保护时间的描述
//    QString getGroupChangetime(); //通过组切换时间的配置值，得到组切换时间的描述

//public slots:
//    void slotChangeFromMonitor();
//    void slotFirstWeekDay();
//    void slotAutoHomeWinTime();
//    void slotGroupChangeTime();
//    void slotBackGround();
//    void slotBacklightRestore();
//    void slotBacklightSaverTime();
//    void slotBacklightSaveModeSwitch();
//    void slotLcdBrightnessChoose();
//    void slotbarDirection();

//private:
//    MenuWidget *            p_basicChangeFromMonitor;   //监控模式
//    MenuWidget *            p_BasicFirstWeekDay;        //一周的第一天
//    MenuWidget *            p_BasicAutoHomeWinTime;     //
//    MenuWidget *            p_basicGroupChangeTime;
//    MenuWidget *            p_basicBackground;      //背景颜色
//    MenuWidget *            p_basicBacklightRestore;    //
//    MenuWidget *            p_basicBacklightSaverTime;
//    MenuWidget *            p_basicBacklightSaverMode;
//    MenuWidget *            p_basicLCDBrightness;
//    MenuWidget *            p_basicBarDirection;

//    MenuWidget *            p_BarGroup;     //bar
//    MenuWidget *            p_Lcdgroup;
//    MenuWidget *            p_LcdProtectgroup;
//    MenuWidget *            p_monitor;      //监控
//    MenuWidget *            p_calendarDisplay;
//    MenuWidget *            p_changeFromMonitor;
//};


///*
// *  显示设置-->基本设置-->棒图方向 : vertical/horizontal
// *    配置值与配置值的描述对应关系 0 : vertical / 1 : horizontal
//*/
//class WinRoot_SysConfig_Display_BasicBarDirection : public WinWidget
//{
//    Q_OBJECT
//public:
//    static WinRoot_SysConfig_Display_BasicBarDirection *instace(MenuWidget *barMenu);

//private:
//    explicit WinRoot_SysConfig_Display_BasicBarDirection(MenuWidget * barMenu, QWidget *parent = 0);
//    ~WinRoot_SysConfig_Display_BasicBarDirection();

//    void initMenuMenu();
//    void signalAndSlotConnect();

//public slots:
//    void slotbarDirection();    //点击某一个方向按钮后的响应槽函数

//private:
//    MenuWidget *            p_direction[2]; //棒图方向 0 : vertical / 1: horizontal
//    MenuWidget *            p_barMenu;  //保存棒图方向配置值的对象menu

//};

///******************************** 显示设置-->基本设置-->LCD辉度 *********************/
//class WinRoot_SysConfig_Display_BasicLcdBrightness : public WinWidget
//{
//    Q_OBJECT
//public:
//    static WinRoot_SysConfig_Display_BasicLcdBrightness *instace(MenuWidget *lcdBrightnessMenu);

//private:
//    explicit WinRoot_SysConfig_Display_BasicLcdBrightness(MenuWidget * lcdBrightnessMenu, QWidget *parent = 0);
//    ~WinRoot_SysConfig_Display_BasicLcdBrightness();

//    void initMenuMenu();
//    void signalAndSlotConnect();
//    int getBrightnessValue(QString brightnessStr);  //把从界面上得到辉度的描述转化为辉度的配置值

//public slots:
//    void slotLcdBrightness();

//private:
//    MenuWidget *                p_lcdBrightnessMenu;  //上一级菜单的对象，保存LCD辉度配置的当前值
//    MenuWidget *                p_brightness[6];    //辉度值
//};

///*
// *  显示设置-->基本设置-->保护模式 : Off/变暗/熄灭
//*/
//class WinRoot_SysConfig_Display_BasicSaveMode : public WinWidget
//{
//    Q_OBJECT
//public:
//    static WinRoot_SysConfig_Display_BasicSaveMode *instace(MenuWidget *saveMenu);

//private:
//    explicit WinRoot_SysConfig_Display_BasicSaveMode(MenuWidget * saveMenu, QWidget *parent = 0);
//    ~WinRoot_SysConfig_Display_BasicSaveMode();

//    void initMenuMenu();
//    void signalAndSlotConnect();
//    int getSaveMode(QString saveStr);      //把从界面上获取配置字符串转化为保护模式的配置值

//public slots:
//    void slotSaveMode();    //从界面得到保护配置的描述，并把它转化为配置的值

//private:
//    MenuWidget *                p_saveMenu;     //该界面的上一级按钮对象，保存了当前的模式

//    MenuWidget *                p_saveMode[3];  //0: Off / 1: darkening / 2: go out
//};


///************************ 显示设置-->基本设置-->LCD保护时间 ********************/

////时间的配置值为秒  形式如： 1min : 60 / 2min : 120 / 5min : 300 .....
//class WinRoot_SysConfig_Display_BasicSaveTime : public WinWidget
//{
//    Q_OBJECT
//public:
//    static WinRoot_SysConfig_Display_BasicSaveTime *instace(MenuWidget *saveTimeMenu);

//private:
//    explicit WinRoot_SysConfig_Display_BasicSaveTime(MenuWidget * saveTimeMenu, QWidget *parent = 0);
//    ~WinRoot_SysConfig_Display_BasicSaveTime();

//    void initMenuMenu();
//    void signalAndSlotConnect();
//    int getSaveTime(QString saveTime);      //把从界面上获取保护时间配置字符串转化为保护时间的配置值

//public slots:
//    void slotSaveTime();    //从界面得到保护时间配置的描述，并把它转化为保护时间的值

//private:
//    MenuWidget *                p_saveTimeMenu;     //该界面的上一级按钮对象，保存了当前的时间的配置值

//    MenuWidget *                p_saveTime[6];
//};



///************************ 显示设置-->基本设置-->LCD 背光还原********************/

////背光还原操作与配置值的关系为  0 : 按键/触摸    1: 按键/触摸/警报
//class WinRoot_SysConfig_Display_BasicBacklightRestore : public WinWidget
//{
//    Q_OBJECT
//public:
//    static WinRoot_SysConfig_Display_BasicBacklightRestore *instace(MenuWidget *backlightRestoreMenu);

//private:
//    explicit WinRoot_SysConfig_Display_BasicBacklightRestore(MenuWidget * backlightRestoreMenu, QWidget *parent = 0);
//    ~WinRoot_SysConfig_Display_BasicBacklightRestore();

//    void initMenuMenu();
//    void signalAndSlotConnect();
//    int getSaveRestore(QString restoreStr);      //把从界面上获取背光还原字符串转化为背光还原的配置值

//public slots:
//    void slotRestore();    //从界面得到背光还原配置的描述，并把它转化为背光还原的值

//private:
//    MenuWidget *                p_backlightRestoreMenu;     //该界面的上一级按钮对象，保存了当前的背光还原的配置值

//    MenuWidget *                p_backlightRestore[2];
//};


///************************ 显示设置-->基本设置-->背景颜色********************/

////设置背景颜色   背景颜色与配置值之间的关系为   0:白    1:黑
//class WinRoot_SysConfig_Display_BasicBackground: public WinWidget
//{
//    Q_OBJECT
//public:
//    static WinRoot_SysConfig_Display_BasicBackground *instace(MenuWidget *backgroundMenu);

//private:
//    explicit WinRoot_SysConfig_Display_BasicBackground(MenuWidget * backgroundMenu, QWidget *parent = 0);
//    ~WinRoot_SysConfig_Display_BasicBackground();

//    void initMenuMenu();
//    void signalAndSlotConnect();
//    int getBackgroundValue(QString backgroundvalue);      //把从界面上获取背景颜色字符串转化为背景颜色的配置值

//public slots:
//    void slotBackground();    //从界面得到背景颜色配置的描述，并把它转化为背景颜色的值

//private:
//    MenuWidget *                p_backgroundMenu;     //该界面的上一级按钮对象，保存了当前的背景颜色的配置值

//    MenuWidget *                p_background[2];
//};


///************************ 显示设置-->基本设置-->组自动切换时间********************/

////组自动切换时间    5 : 5s    10: 10s  20: 20s  30: 30s  60: 1min
//class WinRoot_SysConfig_Display_BasicGroupChangeTime: public WinWidget
//{
//    Q_OBJECT
//public:
//    static WinRoot_SysConfig_Display_BasicGroupChangeTime *instace(MenuWidget *groupChangeTime);

//private:
//    explicit WinRoot_SysConfig_Display_BasicGroupChangeTime(MenuWidget * groupChangeTime, QWidget *parent = 0);
//    ~WinRoot_SysConfig_Display_BasicGroupChangeTime();

//    void initMenuMenu();
//    void signalAndSlotConnect();
//    int getGroupChnageTime(QString timeStr);      //把从界面上获取跳转至组自动切换时间字符串转化为跳转至组自动切换时间的配置值

//public slots:
//    void slotGroupChnageTime();    //从界面得到组自动切换时间配置的描述，并把它转化为组自动切换时间的值

//private:
//    MenuWidget *                p_groupChangeTimeMenu;     //该界面的上一级按钮对象，保存了当前的跳转至默认画面的配置值

//    MenuWidget *                p_groupChangeTime[5];
//};




///************************ 显示设置-->基本设置-->跳转至默认画面********************/

///*跳转至默认画面开关   关闭自动跳转    0: Off
//                    打开自动跳转    1: min    2: 2min  3: 5min   4: 10min  5: 20min  6: 30min  7: 1h
//*/
//class WinRoot_SysConfig_Display_BasicAutoHomeWinTime: public WinWidget
//{
//    Q_OBJECT
//public:
//    static WinRoot_SysConfig_Display_BasicAutoHomeWinTime *instace(MenuWidget *autoHomeWintime);

//private:
//    explicit WinRoot_SysConfig_Display_BasicAutoHomeWinTime(MenuWidget * autoHomeWintime, QWidget *parent = 0);
//    ~WinRoot_SysConfig_Display_BasicAutoHomeWinTime();

//    void initMenuMenu();
//    void signalAndSlotConnect();
//    int getAutoHomeWinTime(QString timeStr);      //把从界面上获取跳转至默认画面字符串转化为跳转至默认画面的配置值

//public slots:
//    void slotAutoHomeWinTime();    //从界面得到跳转至默认画面配置的描述，并把它转化为跳转至默认画面的值

//private:
//    MenuWidget *                p_autoHomeWintimeMenu;     //该界面的上一级按钮对象，保存了当前的跳转至默认画面的配置值

//    MenuWidget *                p_autoHomeWintime[8];
//};


///************************ 显示设置-->基本设置-->周的起始时间********************/
///*周的起始时间选择界面  关闭自动跳转    0: 星期日   1: 星期一
//*/
//class WinRoot_SysConfig_Display_BasicFirstWeekDay: public WinWidget
//{
//    Q_OBJECT
//public:
//    static WinRoot_SysConfig_Display_BasicFirstWeekDay *instace(MenuWidget *weekdayMenu);

//private:
//    explicit WinRoot_SysConfig_Display_BasicFirstWeekDay(MenuWidget * weekdayMenu, QWidget *parent = 0);
//    ~WinRoot_SysConfig_Display_BasicFirstWeekDay();

//    void initMenuMenu();
//    void signalAndSlotConnect();

//public slots:
//    void slotFirstWeekDay();    //从界面得到周的起始时间配置的描述，并把它转化为周的起始时间的值

//private:
//    MenuWidget *                p_firstWeekDayMenu;     //该界面的上一级按钮对象，保存了当前的周的起始时间的配置值

//    MenuWidget *                p_firstWeekDay[2];
//};


#endif // WINROOT_SYSCONFIG_DISPLAY_H
