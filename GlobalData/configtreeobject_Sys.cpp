#include "configtreeobject_Sys.h"
#include "cfgchannel.h"
#include "./GlobalData/globalDef.h"

#include "cfgGlobalDef.h"
#include "./WorkWin/wininfolistdialog.h"

ConfigTreeObject_Sys *ConfigTreeObject_Sys::instance(QObject *parent)
{
    static ConfigTreeObject_Sys data(parent);
    return &data;
}

ConfigTreeObject_Sys::ConfigTreeObject_Sys(QObject *parent)
{
    //System settings代理树
    QFile fileDelegate(":/configtree/Image/configTree/System/System_Delegate/delegate.xml");
    if(!fileDelegate.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file: System_setting!";
    }else{
        XmlConfigParser::parseConfigXml(fileDelegate, static_cast<ConfigObject*>(parent), \
                                                                   (QObject*)this);
        fileDelegate.close();
    }
}

void ConfigTreeObject_Sys::updateStrShowOn()
{
    slotsystemEnvironmentLanguageChange();
    slotsystemEnvironmentTemperatureChange();
    slotsystemEnvironmentDecimalChange();
    slotsystemEnvironmentDateDataChange();
    slotsystemEnvironmentDateDelimiterChange();

    slotsystemEnvironmentDateMonthChange();
    slotsystemAlarmRateDecreaseChange();
    slotsystemAlarmRateIncreaseChange();
    slotsystemAlarmIndicatorChange();
    slotsystemAlarmAlarmackChange();

    slotsystemTimeHourChange();
    slotsystemTimeMinuteChange();
//    slotsystemInternalFirstChange();
//    slotsystemInternalLastChange();
    slotsystemInternalTypeChange();
    slotsystemInternalAndorChange();

    slotsystemRelayFailChange();
    slotsystemRelayStatusChange();
    slotsystemRelayMeasurementChange();
    slotsystemRelayCommunicationChange();
    slotsystemRelayRecordChange();

    slotsystemRelayAlarmChange();
    slotsystemPrinterIpChange();
    slotsystemPrinterPaperChange();
    slotsystemPrinterPageChange();
    slotsystemPrinterResolutionChange();

    slotsystemPrinterNumberChange();
    slotsystemSoundTouchChange();
    slotsystemSoundWarningChange();
    slotsystemSoundLedChange();
    slotsystemInstrumentTagChange();

    slotsystemInstrumentNoChange();
    slotsystemFileFileChange();
}

void ConfigTreeObject_Sys::slotCreateSystemsettingTree(QObject *parent)
{
    //System setting 配置树
    QFile file(":/configtree/Image/configTree/System/System_Setting/config.xml");
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file: System_setting!";
    }else{
        XmlConfigParser::parseConfigXml(file, static_cast<ConfigObject*>(parent));
        file.close();
    }

    quint32 ID_first = ConfigTreeObject::getConfigObject(System_Setting_Internal_First)->getObjectID();
    ConfigTreeObject::getConfigObject(System_Setting_Internal_First)->initData(0, 0, ID_first);
    ConfigTreeObject::getConfigObject(System_Setting_Internal_First)->setStrShowOn("1");

    quint32 ID_last = ConfigTreeObject::getConfigObject(System_Setting_Internal_Last)->getObjectID();
    ConfigTreeObject::getConfigObject(System_Setting_Internal_Last)->initData(0, 0, ID_last);
    ConfigTreeObject::getConfigObject(System_Setting_Internal_Last)->setStrShowOn("1");
}

void ConfigTreeObject_Sys::slotSystemDelegateChange()
{
    QFile delegateChange(":/configtree/Image/configTree/System/System_DelegateChange/delegateChange.xml");
    if(!delegateChange.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file: AI_settingDelegateChange!";
    }else{
        XmlConfigParser::parseDelegateChangeXml(delegateChange);
        delegateChange.close();
    }

    _curStatus = false;
    updateStrShowOn();
    _curStatus = true;
}

void ConfigTreeObject_Sys::slotsystemEnvironmentLanguageChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(System_Delegate_Environment_Language)->getData();

    QFile file(XML_System_Setting_Environment_Language);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }

}

void ConfigTreeObject_Sys::slotsystemEnvironmentTemperatureChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(System_Delegate_Environment_Temperature)->getData();

    QFile file(XML_System_Setting_Environment_Temperature);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Sys::slotsystemEnvironmentDecimalChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(System_Delegate_Environment_Decimal)->getData();

    QFile file(XML_System_Setting_Environment_Decimal);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Sys::slotsystemEnvironmentDateDataChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(System_Delegate_Environment_Date_Data)->getData();

    QFile file(XML_System_Setting_Environment_Date_Data);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Sys::slotsystemEnvironmentDateDelimiterChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(System_Delegate_Environment_Date_Delimiter)->getData();

    QFile file(XML_System_Setting_Environment_Date_Delimiter);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Sys::slotsystemEnvironmentDateMonthChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(System_Delegate_Environment_Date_Month)->getData();

    QFile file(XML_System_Setting_Environment_Date_Month);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Sys::slotsystemAlarmRateDecreaseChange()
{
    QString num;

    ConfigObject *pObj = ConfigTreeObject::getConfigObject(System_Delegate_Alarm_Rate_Decrease);


    if(pObj){
        num = pObj->getData().toString();

        pObj->setStrShowOn(QString("%1").arg(num.toInt()+1));
    }
}

void ConfigTreeObject_Sys::slotsystemAlarmRateIncreaseChange()
{
    QString num;

    ConfigObject *pObj = ConfigTreeObject::getConfigObject(System_Delegate_Alarm_Rate_Increase);


    if(pObj){
        num = pObj->getData().toString();

        pObj->setStrShowOn(QString("%1").arg(num.toInt()+1));
    }
}

void ConfigTreeObject_Sys::slotsystemAlarmIndicatorChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(System_Delegate_Alarm_Indicator)->getData();

    QFile file(XML_System_Setting_Alarm_Indicator);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Sys::slotsystemAlarmAlarmackChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(System_Delegate_Alarm_AlarmACK)->getData();

    QFile file(XML_System_Setting_Alarm_AlarmACK);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }

     ConfigObject *pObj;

     pObj = ConfigTreeObject::getConfigObject(System_Delegate_Alarm_Indicator);
     if(pObj && value.toInt() == 1) {
        pObj->changeData(Alarm_Indicator_Hold, false);//解除单个报警为为[On]时，指示器的设定自动变为[保持]。
     }

     pObj = ConfigTreeObject::getConfigObject(DO_DELEGATE_RANGE_ACTION_RELAYACK);
     if(pObj && value.toInt() == 1) {
        pObj->changeData(1, false); //解除单个报警为[On]时，所有DO通道的报警解除时继电器动作的设定变为[复位]。
     }

     if(value.toInt() == 1) {
         WinInfoListDialog::instance()->showMsg(865);
     }

}

void ConfigTreeObject_Sys::slotsystemTimeHourChange()
{
    QString num;

    ConfigObject *pObj = ConfigTreeObject::getConfigObject(System_Delegate_Time_Hour);


    if(pObj){
        num = pObj->getData().toString();

        pObj->setStrShowOn(QString("%1").arg(num.toInt()-13));
    }
}

void ConfigTreeObject_Sys::slotsystemTimeMinuteChange()
{
    QString num;

    ConfigObject *pObj = ConfigTreeObject::getConfigObject(System_Delegate_Time_Minute);


    if(pObj){
        num = pObj->getData().toString();

        pObj->setStrShowOn(QString("%1").arg(num.toInt()));
    }
}

void ConfigTreeObject_Sys::slotsystemInternalFirstChange()
{
    QString num,numq;

    ConfigObject *pObj = ConfigTreeObject::getConfigObject(System_Delegate_Internal_First);
    ConfigObject *qObj = ConfigTreeObject::getConfigObject(System_Delegate_Internal_Last);

    if(pObj){
        num = pObj->getData().toString();
        numq = qObj->getData().toString();
        pObj->setStrShowOn(QString("%1").arg(num.toInt()+1));
        if (numq.toInt() < num.toInt()) {
            qObj->changeData(num,false);
            slotsystemInternalLastChange();
        }
    }
}

void ConfigTreeObject_Sys::slotsystemInternalLastChange()
{
    QString num,numq;

    ConfigObject *pObj = ConfigTreeObject::getConfigObject(System_Delegate_Internal_Last);
    ConfigObject *qObj = ConfigTreeObject::getConfigObject(System_Delegate_Internal_First);

    if(pObj){
        num = pObj->getData().toString();
        numq = qObj->getData().toString();

        pObj->setStrShowOn(QString("%1").arg(num.toInt()+1));

        if (numq.toInt() > num.toInt()) {
            qObj->changeData(num,false);
            slotsystemInternalFirstChange();
        }
    }
}

void ConfigTreeObject_Sys::slotsystemInternalTypeChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(System_Delegate_Internal_Type)->getData();

    QFile file(XML_System_Setting_Internal_Type);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Sys::slotsystemInternalAndorChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(System_Delegate_Internal_AndOr)->getData();

    QFile file(XML_System_Setting_Internal_AndOr);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Sys::slotsystemRelayFailChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(System_Delegate_Relay_Fail)->getData();

    QFile file(XML_System_Setting_Relay_Fail);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Sys::slotsystemRelayStatusChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(System_Delegate_Relay_status)->getData();

    QFile file(XML_System_Setting_Relay_status);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Sys::slotsystemRelayMeasurementChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(System_Delegate_Relay_Measurement)->getData();

    QFile file(XML_System_Setting_Relay_Measurement);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Sys::slotsystemRelayCommunicationChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(System_Delegate_Relay_Communication)->getData();

    QFile file(XML_System_Setting_Relay_Communication);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Sys::slotsystemRelayRecordChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(System_Delegate_Relay_Record)->getData();

    QFile file(XML_System_Setting_Relay_Record);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Sys::slotsystemRelayAlarmChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(System_Delegate_Relay_Alarm)->getData();

    QFile file(XML_System_Setting_Relay_Alarm);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Sys::slotsystemPrinterIpChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(System_Delegate_Printer_IP)->getData();

    QFile file(XML_System_Setting_Printer_IP);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Sys::slotsystemPrinterPaperChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(System_Delegate_Printer_Paper)->getData();

    QFile file(XML_System_Setting_Printer_Paper);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Sys::slotsystemPrinterPageChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(System_Delegate_Printer_Page)->getData();

    QFile file(XML_System_Setting_Printer_Page);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Sys::slotsystemPrinterResolutionChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(System_Delegate_Printer_Resolution)->getData();

    QFile file(XML_System_Setting_Printer_Resolution);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Sys::slotsystemPrinterNumberChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(System_Delegate_Printer_Number)->getData();

    QFile file(XML_System_Setting_Printer_Number);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Sys::slotsystemSoundTouchChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(System_Delegate_Sound_Touch)->getData();

    QFile file(XML_System_Setting_Sound_Touch);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Sys::slotsystemSoundWarningChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(System_Delegate_Sound_Warning)->getData();

    QFile file(XML_System_Setting_Sound_Warning);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Sys::slotsystemSoundLedChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(System_Delegate_Sound_LED)->getData();

    QFile file(XML_System_Setting_Sound_LED);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Sys::slotsystemInstrumentTagChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(System_Delegate_Instrument_tag)->getData();

    QFile file(XML_System_Setting_Instrument_tag);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Sys::slotsystemInstrumentNoChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(System_Delegate_Instrument_No)->getData();

    QFile file(XML_System_Setting_Instrument_No);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Sys::slotsystemFileFileChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(System_Delegate_File_File)->getData();

    QFile file(XML_System_Setting_File_File);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}
