#ifndef CONFIGTREEOBJECT_RECORD_H
#define CONFIGTREEOBJECT_RECORD_H

//Record 配置树路径
#define RECORDCONFIG_BASIC_MODE             "Config:Recording settings:Basic settings:Recording mode"
#define RECORDCONFIG_BASIC_MODE_VALUE       "Config:Recording settings:Basic settings:Recording mode:Mode"
#define RECORDCONFIG_BASIC_DISP             "Config:Recording settings:Basic settings:Display data, Trend waveform"
#define RECORDCONFIG_BASIC_DISP_VALUE       "Config:Recording settings:Basic settings:Display data, Trend waveform:Saving interval"
#define RECORDCONFIG_BASIC_EVENT            "Config:Recording settings:Basic settings:Event data"
#define RECORDCONFIG_BASIC_EVENT_INTERVAL   "Config:Recording settings:Basic settings:Event data:Recording interval"
#define RECORDCONFIG_BASIC_EVENT_MODE       "Config:Recording settings:Basic settings:Event data:Recording mode"
#define RECORDCONFIG_BASIC_EVENT_LENGTH     "Config:Recording settings:Basic settings:Event data:Data length"
#define RECORDCONFIG_BASIC_EVENT_TRIGGER    "Config:Recording settings:Basic settings:Event data:Pre-trigger"
#define RECORDCONFIG_BASIC_EVENT_OPERATE    "Config:Recording settings:Basic settings:Event data:Trigger source operation"
#define RECORDCONFIG_BASIC_ACTION           "Config:Recording settings:Basic settings:Record confirmation action"
#define RECORDCONFIG_BASIC_ACTION_SWITCH    "Config:Recording settings:Basic settings:Record confirmation action:Confirmation screen"

#define RECORDCONFIG_CHANNEL_DISP               "Config:Recording settings:Recording channel settings:Display data, Trend waveform"
#define RECORDCONFIG_CHANNEL_DISP_CHANITEM      "Config:Recording settings:Recording channel settings:Display data, Trend waveform Channel item"
#define RECORDCONFIG_CHANNEL_EVENT              "Config:Recording settings:Recording channel settings:Event data"
#define RECORDCONFIG_CHANNEL_EVENT_CHANITEM     "Config:Recording settings:Recording channel settings:Event data Channel item"
#define RECORDCONFIG_CHANNEL_MANUAL             "Config:Recording settings:Recording channel settings:Manual sample"
#define RECORDCONFIG_CHANNEL_MANUAL_CHANITEM    "Config:Recording settings:Recording channel settings:Manual sample Channel item"

//Record 代理树路径
#define RECORD_DELEGATE_BASIC_MODE             "Config:Recording Delegate:Basic settings:Recording mode"
#define RECORD_DELEGATE_BASIC_MODE_VALUE       "Config:Recording Delegate:Basic settings:Recording mode:Mode"
#define RECORD_DELEGATE_BASIC_DISP             "Config:Recording Delegate:Basic settings:Display data, Trend waveform"
#define RECORD_DELEGATE_BASIC_DISP_VALUE       "Config:Recording Delegate:Basic settings:Display data, Trend waveform:Saving interval"
#define RECORD_DELEGATE_BASIC_EVENT            "Config:Recording Delegate:Basic settings:Event data"
#define RECORD_DELEGATE_BASIC_EVENT_INTERVAL   "Config:Recording Delegate:Basic settings:Event data:Recording interval"
#define RECORD_DELEGATE_BASIC_EVENT_MODE       "Config:Recording Delegate:Basic settings:Event data:Recording mode"
#define RECORD_DELEGATE_BASIC_EVENT_LENGTH     "Config:Recording Delegate:Basic settings:Event data:Data length"
#define RECORD_DELEGATE_BASIC_EVENT_TRIGGER    "Config:Recording Delegate:Basic settings:Event data:Pre-trigger"
#define RECORD_DELEGATE_BASIC_EVENT_OPERATE    "Config:Recording Delegate:Basic settings:Event data:Trigger source operation"
#define RECORD_DELEGATE_BASIC_ACTION           "Config:Recording Delegate:Basic settings:Record confirmation action"
#define RECORD_DELEGATE_BASIC_ACTION_SWITCH    "Config:Recording Delegate:Basic settings:Record confirmation action:Confirmation screen"

#define RECORD_DELEGATE_CHANNEL_DISP            "Config:Recording Delegate:Recording channel settings:Display data, Trend waveform"
#define RECORD_DELEGATE_CHANNEL_DISP_CHANITEM   "Config:Recording Delegate:Recording channel settings:Display data, Trend waveform Channel item"
#define RECORD_DELEGATE_CHANNEL_EVENT           "Config:Recording Delegate:Recording channel settings:Event data"
#define RECORD_DELEGATE_CHANNEL_EVENT_CHANITEM  "Config:Recording Delegate:Recording channel settings:Event data Channel item"
#define RECORD_DELEGATE_CHANNEL_MANUAL          "Config:Recording Delegate:Recording channel settings:Manual sample"
#define RECORD_DELEGATE_CHANNEL_MANUAL_CHANITEM "Config:Recording Delegate:Recording channel settings:Manual sample Channel item"

//Record 值变更 XML文件路径
#define XML_RECORD_BASIC_MODE_VALUE     ":/configtree/Image/configTree/Record/Record_ValueChange/RecordBasic_ModeMode.xml"
#define XML_RECORD_BASIC_DISP_VALUE     ":/configtree/Image/configTree/Record/Record_ValueChange/RecordBasic_DispValue.xml"
#define XML_RECORD_BASIC_EVENT_INTERVAL ":/configtree/Image/configTree/Record/Record_ValueChange/RecordBasic_EventInterval.xml"
#define XML_RECORD_BASIC_EVENT_MODE     ":/configtree/Image/configTree/Record/Record_ValueChange/RecordBasic_EventMode.xml"
#define XML_RECORD_BASIC_EVENT_LENGTH   ":/configtree/Image/configTree/Record/Record_ValueChange/RecordBasic_EventLength.xml"
#define XML_RECORD_BASIC_EVENT_TRIGGER  ":/configtree/Image/configTree/Record/Record_ValueChange/RecordBasic_EventTrigger.xml"
#define XML_RECORD_BASIC_EVENT_OPERATE  ":/configtree/Image/configTree/Record/Record_ValueChange/RecordBasic_EventOpera.xml"
#define XML_RECORD_BASIC_ACTION_SWITCH  ":/configtree/Image/configTree/Record/Record_ValueChange/RecordBasic_ScreenSW.xml"
#define XML_RECORD_CHANNEL_DISP         ":/configtree/Image/configTree/Record/Record_ValueChange/RecordChannel_Disp.xml"
#define XML_RECORD_CHANNEL_EVENT        ":/configtree/Image/configTree/Record/Record_ValueChange/RecordChannel_Event.xml"
#define XML_RECORD_CHANNEL_MANUAL       ":/configtree/Image/configTree/Record/Record_ValueChange/RecordChannel_Sample.xml"

#include "configobject.h"

class ConfigTreeObject_Record : public ConfigObject
{
    Q_OBJECT
public:
    static ConfigTreeObject_Record *instance(QObject *parent);

signals:

private:
    explicit ConfigTreeObject_Record(QObject *parent = 0);
    void updateStrShowOn();

public slots:
    void slotCreateRecordsettingTree(QObject *parent = 0);

    void slotRecordDelegateChange();

public slots:
    void slotRecordBasicModeModeChanged();
    void slotRecordBasicDispValueChanged();
    void slotRecordBasicEventInterChanged();
    void slotRecordBasicEventModeChanged();
    void slotRecordBasicEventLenChanged();
    void slotRecordBasicEventTrigChanged();
    void slotRecordBasicEventOperaChanged();
    void slotRecordBasicScreenSWChanged();

    void slotRecordChannelDispChanged();
    void slotRecordChannelEventChanged();
    void slotRecordChannelSampleChanged();
};












#endif // CONFIGTREEOBJECT_RECORD_H
