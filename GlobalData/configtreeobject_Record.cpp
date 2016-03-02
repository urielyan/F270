#include "configtreeobject_Record.h"
#include "cfgchannel.h"
#include "./GlobalData/globalDef.h"

#include "cfgGlobalDef.h"
#include "configtreeobject.h"

ConfigTreeObject_Record *ConfigTreeObject_Record::instance(QObject *parent)
{
    static ConfigTreeObject_Record data(parent);
    return &data;
}

ConfigTreeObject_Record::ConfigTreeObject_Record(QObject *parent):
      ConfigObject(parent)
{
    //Recording settings代理树
    QFile fileDelegate(":/configtree/Image/configTree/Record/Record_SettingDelegate/delegate.xml");
    if(!fileDelegate.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file: Record_settingDelegate!";
    }else{
        XmlConfigParser::parseConfigXml(fileDelegate, static_cast<ConfigObject*>(parent), \
                                                          (QObject*)this);
        fileDelegate.close();
    }
}

void ConfigTreeObject_Record::updateStrShowOn()
{
    slotRecordBasicModeModeChanged();
    slotRecordBasicDispValueChanged();
    slotRecordBasicEventInterChanged();
    slotRecordBasicEventModeChanged();
    slotRecordBasicEventLenChanged();
    slotRecordBasicEventTrigChanged();
    slotRecordBasicEventOperaChanged();
    slotRecordBasicScreenSWChanged();

    slotRecordChannelDispChanged();
    slotRecordChannelEventChanged();
    slotRecordChannelSampleChanged();
}

void ConfigTreeObject_Record::slotCreateRecordsettingTree(QObject *parent)
{
    //Recording settings配置树
    QFile file(":/configtree/Image/configTree/Record/Record_Setting/config.xml");
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file: Record_setting!";
    }else{
        XmlConfigParser::parseConfigXml(file, static_cast<ConfigObject*>(parent));
        file.close();
    }
}

void ConfigTreeObject_Record::slotRecordDelegateChange()
{
    QFile delegateChange(":/configtree/Image/configTree/Record/Record_DelegateChange/delegateChange.xml");
    if(!delegateChange.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file: Record_DelegateChange!";
    }else{
        XmlConfigParser::parseDelegateChangeXml(delegateChange);
        delegateChange.close();
    }

    _curStatus = false;
    updateStrShowOn();
    _curStatus = true;
}

void ConfigTreeObject_Record::slotRecordBasicModeModeChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(RECORD_DELEGATE_BASIC_MODE_VALUE)->getData();

    QFile file(XML_RECORD_BASIC_MODE_VALUE);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:RecordBasic_ModeMode!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }

    quint32 valueCur = value.toUInt();
    ConfigObject *pObj = ConfigTreeObject::getConfigObject(DISP_DELEGATE_TREND_INTERVAL_SWITCH);
    quint32 sw = pObj->getData().toUInt();
    if(0 == sw) {
        if((0 == valueCur) || (2 == valueCur)) {
            ConfigTreeObject::getConfigObject(DISP_DELEGATE_TREND_INTERVAL_INTERVAL)->setHideFlag(false);
        } else if (1 == valueCur) { //event
            ConfigTreeObject::getConfigObject(DISP_DELEGATE_TREND_INTERVAL_INTERVAL)->setHideFlag(true);
        }
    }
}

void ConfigTreeObject_Record::slotRecordBasicDispValueChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(RECORD_DELEGATE_BASIC_DISP_VALUE)->getData();

    QFile file(XML_RECORD_BASIC_DISP_VALUE);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:RecordBasic_DispValue!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Record::slotRecordBasicEventInterChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(RECORD_DELEGATE_BASIC_EVENT_INTERVAL)->getData();

    QFile file(XML_RECORD_BASIC_EVENT_INTERVAL);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:RecordBasic_EventInterval!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }

    Event_Record_Interval valueCur = (Event_Record_Interval)value.toUInt();
    switch(valueCur) {
    case Event_Record_Interval_100ms:
    case Event_Record_Interval_200ms:
    case Event_Record_Interval_500ms:
    case Event_Record_Interval_1s:
    case Event_Record_Interval_2s:
    case Event_Record_Interval_5s:
    case Event_Record_Interval_10s:
    case Event_Record_Interval_15s:
    case Event_Record_Interval_20s:
        ConfigTreeObject::getConfigObject(RECORD_DELEGATE_BASIC_EVENT_LENGTH)->changeData(Event_Record_DataLength_10min, false);
        break;
    case Event_Record_Interval_30s:
    case Event_Record_Interval_1min:
    case Event_Record_Interval_2min:
    case Event_Record_Interval_5min:
    case Event_Record_Interval_10min:
    case Event_Record_Interval_15min:
    case Event_Record_Interval_20min:
    case Event_Record_Interval_30min:
        ConfigTreeObject::getConfigObject(RECORD_DELEGATE_BASIC_EVENT_LENGTH)->changeData(Event_Record_DataLength_1h, false);
        break;
    }
}

void ConfigTreeObject_Record::slotRecordBasicEventModeChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(RECORD_DELEGATE_BASIC_EVENT_MODE)->getData();

    QFile file(XML_RECORD_BASIC_EVENT_MODE);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:RecordBasic_EventMode!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Record::slotRecordBasicEventLenChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(RECORD_DELEGATE_BASIC_EVENT_LENGTH)->getData();

    QFile file(XML_RECORD_BASIC_EVENT_LENGTH);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:RecordBasic_EventLength!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Record::slotRecordBasicEventTrigChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(RECORD_DELEGATE_BASIC_EVENT_TRIGGER)->getData();

    QFile file(XML_RECORD_BASIC_EVENT_TRIGGER);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:RecordBasic_EventTrigger!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Record::slotRecordBasicEventOperaChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(RECORD_DELEGATE_BASIC_EVENT_OPERATE)->getData();

    QFile file(XML_RECORD_BASIC_EVENT_OPERATE);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:RecordBasic_EventOpera!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Record::slotRecordBasicScreenSWChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(RECORD_DELEGATE_BASIC_ACTION_SWITCH)->getData();

    QFile file(XML_RECORD_BASIC_ACTION_SWITCH);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:RecordBasic_ScreenSW!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Record::slotRecordChannelDispChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(RECORD_DELEGATE_CHANNEL_DISP)->getData();

    QFile file(XML_RECORD_CHANNEL_DISP);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:RecordChannel_Disp!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Record::slotRecordChannelEventChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(RECORD_DELEGATE_CHANNEL_EVENT)->getData();

    QFile file(XML_RECORD_CHANNEL_EVENT);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:RecordChannel_Event!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Record::slotRecordChannelSampleChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(RECORD_DELEGATE_CHANNEL_MANUAL)->getData();

    QFile file(XML_RECORD_CHANNEL_MANUAL);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:RecordChannel_Sample!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}
