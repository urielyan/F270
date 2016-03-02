#ifndef WINMENUGROUP_H
#define WINMENUGROUP_H

#include <QScrollArea>
#include <QXmlStreamReader>
#include "winmenu.h"
#include "sysconfigslidingscreen.h"
#include "winchannelselector.h"
#include "winnetworkconfig.h"
#include "winserialconfig.h"
#include "wininputconfig.h"
#include "winaiconfig.h"
#include "windiconfig.h"
#include "windoconfig.h"
#include "wincommchannelconfig.h"
#include "windispconfig.h"
#include "winsystemconfig.h"
#include "winmathconfig.h"
#include "wineventconfig.h"
#include "winmeasureconfig.h"

class WinMenuGroup : public QScrollArea
{
    Q_OBJECT
public:
    WinMenuGroup(QString xml, SysConfigSlidingScreen *scene, QWidget *parent = 0);
    WinMenuGroup(SysConfigSlidingScreen *scene, QWidget *parent = 0);
    void parseXml(QFile &xmlFile);
    void setParentMenu(Menu *parentMenu);
    Menu* parentMenu();
    SysConfigSlidingScreen *getSlidingScreen(){return p_group_Scene;}

protected:
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void resizeEvent(QResizeEvent *);
	
signals:

public slots:
    void slotParentMenuClicked();
    void slotAIMenuClicked();
    void slotDIMenuClicked();
    void slotDOMenuClicked();
    void slotMathMenuClicked();
    void slotCommMenuClicked();

    //common input
    void slotNumericKeypadClicked();    //number
    void slotSoftKeypadClicked();       //En/Ch
    void slotSelectColorClicked();      //color
    void slotEnglishInputClicked();     //En
    void slotNetworkIPNumericKeypadClicked();   //IP set
    void slotChannelSelectClicked();     //Channel Select
    void slotCalculationKeypadClicked();        //Math Calculation
    void slotSwitchClicked();           // On/off

    //AI item slot 由对应menu菜单点击触发，见对应xml文件
    void slotAIRangeRangeTypeMenuClicked();
    void slotAIRangeRangeRangeMenuClicked();
    void slotAIRangeRangeCalculationMenuClicked();
    void slotAIRangeScalePosMenuClicked();
    void slotAIRangeLowCutOutputMenuClicked();
    void slotAIRangeRJCModeMenuClicked();
    void slotAIRangeBurnoutModeMenuClicked();

    void slotAIAlarmLvl1TypeMenuClicked();
    void slotAIAlarmLvl1OutTypeMenuClicked();
    void slotAIAlarmDelayHourMenuClicked();
    void slotAIAlarmDelayMinMenuClicked();
    void slotAIAlarmDelaySecMenuClicked();

    void slotAIDisplayScalePosMenuClicked();
    void slotAIDisplayScaleDivMenuClicked();
    void slotAIDisplayBarPosMenuClicked();
    void slotAIDisplayBarDivMenuClicked();
    void slotAIDisplayCsbandModeMenuClicked();
    void slotAIDisplayAlarmTypeMenuClicked();

    void slotAICaliCorrectModeMenuClicked();
    void slotAICaliCorrectCountMenuClicked();
    void slotAICaliCorrectExecuMenuClicked();

    //DI item slot  部分Item单例与AI复用
    void slotDIRangeRangeTypeMenuClicked();
    void slotDIRangeRangeRangeMenuClicked();
    void slotDIRangeRangeCalculationMenuClicked();
    void slotDIAlarmLvl1TypeMenuClicked();

    //DO item slot  部分Item单例与AI复用
    void slotDORangeRangeTypeMenuClicked();
    void slotDORangeRangeRangeMenuClicked();
    void slotDORangeActionEnergizeMenuClicked();
    void slotDORangeActionActionMenuClicked();
    void slotDORangeActionHoldMenuClicked();
    void slotDORangeActionACKMenuClicked();
    void slotDORangeActionIntervalMenuClicked();

    //Math item slot  部分Item单例与AI复用
    void slotMathActionErrorMenuClicked();
    void slotMathActionActionMenuClicked();
    void slotMathActionSumaveMenuClicked();
    void slotMathActionPPMenuClicked();

    void slotMathExpTLOGTypeMenuClicked();
    void slotMathExpTLOGNoMenuClicked();
    void slotMathExpTLOGSumMenuClicked();
    void slotMathExpRollInterMenuClicked();
    void slotMathAlarmLvl1TypeMenuClicked();
    void slotMathConstKNumMenuClicked();

    //Comm item slot  部分Item单例与AI复用
    void slotCommChannelChooseClicked();
    void slotCommChannelValuePowerClicked();
    void slotCommChannelDecimalPlaceClicked();
    void slotCommChannelAlarmTypeClicked();

    //Display item slot
    void slotDispGrpNumMenuClicked();
    void slotDispTrendTrendIntervalMenuClicked();
    void slotDispTrendSecondIntervalMenuClicked();
    void slotDispTrendTrendDirectionMenuClicked();
    void slotDispTrendTrendLineMenuClicked();
    void slotDispTrendTrendGridMenuClicked();
    void slotDispTrendScaleDigitMenuClicked();
    void slotDispTrendScaleValueMenuClicked();
    void slotDispTrendScaleMarkMenuClicked();
    void slotDispTrendMsgWriteMenuClicked();
    void slotDispMsgNumMenuClicked();
    void slotDispScreenLCDValueMenuClicked();
    void slotDispScreenLightModeMenuClicked();
    void slotDispScreenLightTimeMenuClicked();
    void slotDispScreenLightRestoreMenuClicked();
    void slotDispScreenMonitorBackMenuClicked();
    void slotDispScreenMonitorScrollMenuClicked();
    void slotDispScreenMonitorJumpMenuClicked();
    void slotDispScreenCalendar1stMenuClicked();

    //Event-action item slot
    void slotEventGrpNumMenuClicked();
    void slotEventEventTypeMenuClicked();
    void slotEventEventAlarmlevelMenuClicked();
    void slotEventEventDetailsMenuClicked();
    void slotEventEventModeMenuClicked();
    void slotEventActionTypeMenuClicked();
    void slotEventActionDetailMenuClicked();

    //Data-save item slot
    void slotDatasaveDataStrMenuClicked();
    void slotDatasaveFormatMenuClicked();

    //Measure item slot
    void slotMeasureIntervalMenuClicked();
    void slotMeasureOverRangeMenuClicked();
    void slotMeasureUnitModuleMenuClicked();
    void slotMeasureAIModeMenuClicked();
    void slotMeasureADTimeMenuClicked();
    void slotMeasureDIModeMenuClicked();

    //Record item slot
    void slotRecordBasicModeModeMenuClicked();
    void slotRecordBasicDispValueMenuClicked();
    void slotRecordBasicEventValueMenuClicked();
    void slotRecordBasicEventModeMenuClicked();
    void slotRecordBasicEventLenthMenuClicked();
    void slotRecordBasicEventTriggerMenuClicked();

    /******network********/
        /******modbus basic****/
    void slotModbusCLientIntervalClicked();
    void slotModbusClientWaitTimeClicked();
    void slotModbusClientConnectTimeClicked();
        /******modbus server****/
    void slotModbusServerSettingNumberClicked();
    void slotModbusDelayResponseMenuClicked();
        /******modbus command****/
    void slotModbusCommandNumberClicked();
    void slotModbusCommandTypeClicked();
    void slotModbusCommandDataTypeClicked();
    void slotModbusCommandChanelTypeClicked();
        /*******SMTP************/
    void slotSMTPAuthenticationCLicked();
        /*******E-mail************/
    void slotEMailAlarmNotificationCLicked();
    void slotEMailChannelSetCLicked();
    void slotEMailInteralCLicked();
    void slotEMailTimeHourCLicked();
    void slotEMailTimeMinuteCLicked();

        /*******SNTP************/
    void slotSNTPIntervalCLicked();
    void slotSNTPTimeoutCLicked();

    /*****************Serial**************/
    void slotSerialBasicReceiveFunctionClicked();

    void slotSerialBasicBaudClicked();
    void slotSerialBasicParityClicked();
    void slotSerialBasicStopClicked();
    void slotSerialBasicLengthClicked();

    void slotSerialModbusTimeoutClicked();
    void slotSerialModbusGapClicked();
    void slotSerialModbusRetryClicked();
    void slotSerialModbusCommandChanelTypeClicked();

    /***************System********************/
    void slotSystemEnvironmentLanguageClicked();
    void slotSystemEnvironmentTemperatureClicked();
    void slotSystemEnvironmentDecimalClicked();
    void slotSystemEnvironmentDataClicked();
    void slotSystemEnvironmentDelimiterClicked();
    void slotSystemEnvironmentMonthClicked();
    void slotSystemAlarmHoldClicked();
    void slotSystemTimeHourClicked();
    void slotSystemIntervalTypeClicked();
    void slotSystemIntervalANDORClicked();
    void slotSystemRelayClicked();
    void slotSystemCommonClicked();

    //Batch
    void slotBatchCommonClicked();

    //Report
    void slotReportCommClicked();

    //Timer
    void slotTimerTimerCommClicked();
    void slotTimerMatchTimerCommClicked();

    //Security
    void slotSecurityBasicTouchClicked();
    void slotSecurityBasicCommClicked();
    void slotSecurityBasicAutoClicked();
    void slotSecurityPropertyClicked();
    void slotSecurityUserNumClicked();
    void slotSecurityUserLvlClicked();
    void slotSecurityUserModeClicked();
    void slotSecurityUserInitClicked();
    void slotSecurityAuthNumClicked();

private:
    WinMenuGroup(QString xml, Menu *parentMenu, SysConfigSlidingScreen *scene, int depth,QWidget *parent = 0);
    WinMenuGroup(Menu *parentMenu, SysConfigSlidingScreen *scene, int depth,QWidget *parent = 0);
    void setGroupStyleSheet();
    void parseElement(QXmlStreamReader &xmlReader);
    Menu* createMenuGroup(QXmlStreamAttributes &attrs);
    Menu* createMenuTitle(QXmlStreamAttributes &attrs);
    Menu* createMenuClass(QXmlStreamAttributes &attrs);
    Menu* createMenuNormal(QXmlStreamAttributes &attrs);
    Menu* createMenuItem(QXmlStreamAttributes &attrs);
    WinMenuGroup* createSubConfig(QString xml,Menu* menu);


private:
    const int m_depth;
    Menu * p_parentMenu;
    int m_pos;
    bool m_drag;

    QVBoxLayout *p_vLayout;
    SysConfigSlidingScreen *p_group_Scene;
};

#endif // WINMENUGROUP_H

