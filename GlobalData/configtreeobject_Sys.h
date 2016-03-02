#ifndef CONFIGTREEOBJECT_SYS_H
#define CONFIGTREEOBJECT_SYS_H

#include "configobject.h"

//Setting path
#define System_Setting_File_File        "Config:System settings:Settings file:Setting file comment"

#define System_Setting_Instrument_No    "Config:System settings:Instrument tag:Instruments tag No."
#define System_Setting_Instrument_tag   "Config:System settings:Instrument tag:Instruments tag"

#define System_Setting_Sound_LED        "Config:System settings:Sound, LED:LED:MENU key LED"
#define System_Setting_Sound_Warning    "Config:System settings:Sound, LED:Sound:Warning"
#define System_Setting_Sound_Touch      "Config:System settings:Sound, LED:Sound:Touch"

#define System_Setting_Printer_Number       "Config:System settings:Printer settings:Number of copies"
#define System_Setting_Printer_Resolution   "Config:System settings:Printer settings:Resolution (dpi)"
#define System_Setting_Printer_Page         "Config:System settings:Printer settings:Page orientation"
#define System_Setting_Printer_Paper        "Config:System settings:Printer settings:Paper size"
#define System_Setting_Printer_IP           "Config:System settings:Printer settings:IP Address"

#define System_Setting_Relay_Alarm          "Config:System settings:Status relay:Alarm"
#define System_Setting_Relay_Record         "Config:System settings:Status relay:Record stop"
#define System_Setting_Relay_Communication  "Config:System settings:Status relay:Communication error"
#define System_Setting_Relay_Measurement    "Config:System settings:Status relay:Measurement error"
#define System_Setting_Relay_status         "Config:System settings:Status relay:Memory/Media status"
#define System_Setting_Relay_Fail           "Config:System settings:Status relay:Fail relay"

#define System_Setting_Internal_AndOr       "Config:System settings:Internal switch settings:%1:Internal switch:And/Or"
#define System_Setting_Internal_Type        "Config:System settings:Internal switch settings:%1:Internal switch:Type"
#define System_Setting_Internal_Last        "Config:System settings:Internal switch settings:Last number"
#define System_Setting_Internal_First       "Config:System settings:Internal switch settings:First number"

#define System_Setting_Time_Minute          "Config:System settings:Time basic settings:Time Zone:Minute"
#define System_Setting_Time_Hour            "Config:System settings:Time basic settings:Time Zone:Hour"

#define System_Setting_Alarm_AlarmACK       "Config:System settings:Alarm basic settings:AlarmACK:Individual alarm ACK"
#define System_Setting_Alarm_Indicator      "Config:System settings:Alarm basic settings:Indicator:Hold/Nonhold"
#define System_Setting_Alarm_Rate_Increase  "Config:System settings:Alarm basic settings:Rate of change:Increase"
#define System_Setting_Alarm_Rate_Decrease  "Config:System settings:Alarm basic settings:Rate of change:Decrease"

#define System_Setting_Environment_Date_Month       "Config:System settings:Environment(Lanuage) settings:Date format:Month indicator"
#define System_Setting_Environment_Date_Delimiter   "Config:System settings:Environment(Lanuage) settings:Date format:Delimiter"
#define System_Setting_Environment_Date_Data        "Config:System settings:Environment(Lanuage) settings:Date format:Date format"
#define System_Setting_Environment_Decimal          "Config:System settings:Environment(Lanuage) settings:Decimal Point Type"
#define System_Setting_Environment_Temperature      "Config:System settings:Environment(Lanuage) settings:Temperature"
#define System_Setting_Environment_Language         "Config:System settings:Environment(Lanuage) settings:Language"

//Delegate path
#define System_Delegate_File_File       "Config:System Delegate:Settings file:Setting file comment"

#define System_Delegate_Instrument_No   "Config:System Delegate:Instrument tag:Instruments tag No."
#define System_Delegate_Instrument_tag  "Config:System Delegate:Instrument tag:Instruments tag"

#define System_Delegate_Sound_LED       "Config:System Delegate:Sound, LED:LED:MENU key LED"
#define System_Delegate_Sound_Warning   "Config:System Delegate:Sound, LED:Sound:Warning"
#define System_Delegate_Sound_Touch     "Config:System Delegate:Sound, LED:Sound:Touch"

#define System_Delegate_Printer_Number      "Config:System Delegate:Printer settings:Number of copies"
#define System_Delegate_Printer_Resolution  "Config:System Delegate:Printer settings:Resolution (dpi)"
#define System_Delegate_Printer_Page        "Config:System Delegate:Printer settings:Page orientation"
#define System_Delegate_Printer_Paper       "Config:System Delegate:Printer settings:Paper size"
#define System_Delegate_Printer_IP          "Config:System Delegate:Printer settings:IP Address"

#define System_Delegate_Relay_Alarm         "Config:System Delegate:Status relay:Alarm"
#define System_Delegate_Relay_Record        "Config:System Delegate:Status relay:Record stop"
#define System_Delegate_Relay_Communication "Config:System Delegate:Status relay:Communication error"
#define System_Delegate_Relay_Measurement   "Config:System Delegate:Status relay:Measurement error"
#define System_Delegate_Relay_status        "Config:System Delegate:Status relay:Memory/Media status"
#define System_Delegate_Relay_Fail          "Config:System Delegate:Status relay:Fail relay"

#define System_Delegate_Internal_AndOr      "Config:System Delegate:Internal switch settings:Internal switch:And/Or"
#define System_Delegate_Internal_Type       "Config:System Delegate:Internal switch settings:Internal switch:Type"
#define System_Delegate_Internal_Last       "Config:System Delegate:Internal switch settings:Last number"
#define System_Delegate_Internal_First      "Config:System Delegate:Internal switch settings:First number"

#define System_Delegate_Time_Minute         "Config:System Delegate:Time basic settings:Time Zone:Minute"
#define System_Delegate_Time_Hour           "Config:System Delegate:Time basic settings:Time Zone:Hour"

#define System_Delegate_Alarm_AlarmACK      "Config:System Delegate:Alarm basic settings:AlarmACK:Individual alarm ACK"
#define System_Delegate_Alarm_Indicator     "Config:System Delegate:Alarm basic settings:Indicator:Hold/Nonhold"
#define System_Delegate_Alarm_Rate_Increase "Config:System Delegate:Alarm basic settings:Rate of change:Increase"
#define System_Delegate_Alarm_Rate_Decrease "Config:System Delegate:Alarm basic settings:Rate of change:Decrease"

#define System_Delegate_Environment_Date            "Config:System Delegate:Environment(Lanuage) settings:Date format"
#define System_Delegate_Environment_Date_Month      "Config:System Delegate:Environment(Lanuage) settings:Date format:Month indicator"
#define System_Delegate_Environment_Date_Delimiter  "Config:System Delegate:Environment(Lanuage) settings:Date format:Delimiter"
#define System_Delegate_Environment_Date_Data       "Config:System Delegate:Environment(Lanuage) settings:Date format:Date format"
#define System_Delegate_Environment_Decimal         "Config:System Delegate:Environment(Lanuage) settings:Decimal Point Type"
#define System_Delegate_Environment_Temperature     "Config:System Delegate:Environment(Lanuage) settings:Temperature"
#define System_Delegate_Environment_Language        "Config:System Delegate:Environment(Lanuage) settings:Language"

//xml path
#define XML_System_Setting_File_File        ":/configtree/Image/configTree/System/System_ValueChange/System_File.xml"

#define XML_System_Setting_Instrument_No    ":/configtree/Image/configTree/System/System_ValueChange/System_Tag_No.xml"
#define XML_System_Setting_Instrument_tag   ":/configtree/Image/configTree/System/System_ValueChange/System_Tag_Tag.xml"

#define XML_System_Setting_Sound_LED        ":/configtree/Image/configTree/System/System_ValueChange/System_Sound_Menu.xml"
#define XML_System_Setting_Sound_Warning    ":/configtree/Image/configTree/System/System_ValueChange/System_Sound_Warning.xml"
#define XML_System_Setting_Sound_Touch      ":/configtree/Image/configTree/System/System_ValueChange/System_Sound_Touch.xml"

#define XML_System_Setting_Printer_Number       ":/configtree/Image/configTree/System/System_ValueChange/System_Printer_Number.xml"
#define XML_System_Setting_Printer_Resolution   ":/configtree/Image/configTree/System/System_ValueChange/System_Printer_Resolution.xml"
#define XML_System_Setting_Printer_Page         ":/configtree/Image/configTree/System/System_ValueChange/System_Printer_Page.xml"
#define XML_System_Setting_Printer_Paper        ":/configtree/Image/configTree/System/System_ValueChange/System_Printer_Paper.xml"
#define XML_System_Setting_Printer_IP           ":/configtree/Image/configTree/System/System_ValueChange/System_Printer_IP.xml"

#define XML_System_Setting_Relay_Alarm          ":/configtree/Image/configTree/System/System_ValueChange/System_Relay_Alarm.xml"
#define XML_System_Setting_Relay_Record         ":/configtree/Image/configTree/System/System_ValueChange/System_Relay_Record.xml"
#define XML_System_Setting_Relay_Communication  ":/configtree/Image/configTree/System/System_ValueChange/System_Relay_Communication.xml"
#define XML_System_Setting_Relay_Measurement    ":/configtree/Image/configTree/System/System_ValueChange/System_Relay_Measurement.xml"
#define XML_System_Setting_Relay_status         ":/configtree/Image/configTree/System/System_ValueChange/System_Relay_Memory.xml"
#define XML_System_Setting_Relay_Fail           ":/configtree/Image/configTree/System/System_ValueChange/System_Relay_Fail.xml"

#define XML_System_Setting_Internal_AndOr   ":/configtree/Image/configTree/System/System_ValueChange/System_Interval_AndOr.xml"
#define XML_System_Setting_Internal_Type    ":/configtree/Image/configTree/System/System_ValueChange/System_Interval_Type.xml"

#define XML_System_Setting_Alarm_AlarmACK   ":/configtree/Image/configTree/System/System_ValueChange/System_Alarm_ACK.xml"
#define XML_System_Setting_Alarm_Indicator  ":/configtree/Image/configTree/System/System_ValueChange/System_Alarm_Hold.xml"

#define XML_System_Setting_Environment_Date_Month       ":/configtree/Image/configTree/System/System_ValueChange/System_Environment_Month.xml"
#define XML_System_Setting_Environment_Date_Delimiter   ":/configtree/Image/configTree/System/System_ValueChange/System_Environment_Delimiter.xml"
#define XML_System_Setting_Environment_Date_Data        ":/configtree/Image/configTree/System/System_ValueChange/System_Environment_Data.xml"
#define XML_System_Setting_Environment_Decimal          ":/configtree/Image/configTree/System/System_ValueChange/System_Environment_Decimal.xml"
#define XML_System_Setting_Environment_Temperature      ":/configtree/Image/configTree/System/System_ValueChange/System_Environment_Temperature.xml"
#define XML_System_Setting_Environment_Language         ":/configtree/Image/configTree/System/System_ValueChange/System_Environment_Language.xml"

class ConfigTreeObject_Sys : public ConfigObject
{
    Q_OBJECT
public:
    static ConfigTreeObject_Sys *instance(QObject *parent);

signals:

private:
    explicit ConfigTreeObject_Sys(QObject *parent = 0);

    void updateStrShowOn();
public slots:
    void slotCreateSystemsettingTree(QObject *parent = 0);

    void slotSystemDelegateChange();

    void slotsystemEnvironmentLanguageChange();
    void slotsystemEnvironmentTemperatureChange();
    void slotsystemEnvironmentDecimalChange();
    void slotsystemEnvironmentDateDataChange();
    void slotsystemEnvironmentDateDelimiterChange();

    void slotsystemEnvironmentDateMonthChange();
    void slotsystemAlarmRateDecreaseChange();
    void slotsystemAlarmRateIncreaseChange();
    void slotsystemAlarmIndicatorChange();
    void slotsystemAlarmAlarmackChange();

    void slotsystemTimeHourChange();
    void slotsystemTimeMinuteChange();
    void slotsystemInternalFirstChange();
    void slotsystemInternalLastChange();
    void slotsystemInternalTypeChange();
    void slotsystemInternalAndorChange();

    void slotsystemRelayFailChange();
    void slotsystemRelayStatusChange();
    void slotsystemRelayMeasurementChange();
    void slotsystemRelayCommunicationChange();
    void slotsystemRelayRecordChange();

    void slotsystemRelayAlarmChange();
    void slotsystemPrinterIpChange();
    void slotsystemPrinterPaperChange();
    void slotsystemPrinterPageChange();
    void slotsystemPrinterResolutionChange();

    void slotsystemPrinterNumberChange();
    void slotsystemSoundTouchChange();
    void slotsystemSoundWarningChange();
    void slotsystemSoundLedChange();
    void slotsystemInstrumentTagChange();

    void slotsystemInstrumentNoChange();
    void slotsystemFileFileChange();


private:


};



















#endif // CONFIGTREEOBJECT_SYS_H
