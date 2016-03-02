#include "configtreeobject_Comm.h"
#include "cfgchannel.h"
#include "./GlobalData/globalDef.h"

#include "cfgGlobalDef.h"

#include <QDebug>
#include <QStack>


ConfigTreeObject_Comm *ConfigTreeObject_Comm::instance(QObject *parent)
{
    static ConfigTreeObject_Comm data(parent);
    return &data;
}

ConfigTreeObject_Comm::ConfigTreeObject_Comm(QObject *parent):
      ConfigObject(parent)
{
    //Network settings代理树
    QFile fileDelegate(":/configtree/Image/configTree/Comm/Comm_SettingDelegate/delegate.xml");
    if(!fileDelegate.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file: Network_setting!";
    }else{
        XmlConfigParser::parseConfigXml(fileDelegate, static_cast<ConfigObject*>(parent), \
                                                                   (QObject*)this);
        fileDelegate.close();
    }
}

void ConfigTreeObject_Comm::updateStrShowOn()
{
    if (CfgChannel::instance()->getExistChannelComm().isEmpty())
        return;
    else
        ConfigTreeObject::getConfigObject(COMMChannel_Delegate)->setHideFlag(false);

    slotCommRangeSwitchChange();
    slotCommRangeDecimalChange();
    slotCommRangeLowerChange();
    slotCommRangeUpperChange();
    slotCommRangeUnitChange();
    slotCommRangePowerChange();
    slotCommRangeValueChange();
    slotCommRangeWatchDogSwitchChange();
    slotCommRangeWatchDogTimerChange();
    slotCommRangeWatchDogValueChange();

    slotCommAlarmL1SwitchChange();
    slotCommAlarmL1TypeChange();
    slotCommAlarmL1ValueChange();
    slotCommAlarmL1HysteresisChange();
    slotCommAlarmL1LoggingChange();
    slotCommAlarmL1OutTypeChange();
    slotCommAlarmL1OutNOChange();

    slotCommAlarmL2SwitchChange();
    slotCommAlarmL2TypeChange();
    slotCommAlarmL2ValueChange();
    slotCommAlarmL2HysteresisChange();
    slotCommAlarmL2LoggingChange();
    slotCommAlarmL2OutTypeChange();
    slotCommAlarmL2OutNOChange();

    slotCommAlarmL3SwitchChange();
    slotCommAlarmL3TypeChange();
    slotCommAlarmL3ValueChange();
    slotCommAlarmL3HysteresisChange();
    slotCommAlarmL3LoggingChange();
    slotCommAlarmL3OutTypeChange();
    slotCommAlarmL3OutNOChange();

    slotCommAlarmL4SwitchChange();
    slotCommAlarmL4TypeChange();
    slotCommAlarmL4ValueChange();
    slotCommAlarmL4HysteresisChange();
    slotCommAlarmL4LoggingChange();
    slotCommAlarmL4OutTypeChange();
    slotCommAlarmL4OutNOChange();

    slotCommAlarmDelayHourChange();
    slotCommAlarmDelayMinuteChange();
    slotCommAlarmDelaySecendChange();

    slotCommDisplayColorChange();
    slotCommDisplayTagCharactersChange();
    slotCommDisplayTagNoChange();
    slotCommDisplayZoneUpperChange();
    slotCommDisplayZoneLowerChange();

    slotCommDisplayScalePostionChange();
    slotCommDisplayScaleDivisionChange();
    slotCommDisplayBarPositionChange();
    slotCommDisplayBarDivisionChange();
    slotCommDisplayPartialOnOrOffChange();

    slotCommDisplayPartialExpandChange();
    slotCommDisplayPartialBoundaryChange();
    slotCommDisplayBandAreaChange();
    slotCommDisplayBandUpperChange();
    slotCommDisplayBandLowerChange();

    slotCommDisplayBandColorChange();
    slotCommDisplayMarkScaleChange();
    slotCommDisplayMarkTypeChange();
    slotCommDisplayMarkAlarm1Change();
    slotCommDisplayMarkAlarm2Change();

    slotCommDisplayMarkAlarm3Change();
    slotCommDisplayMarkAlarm4Change();

    slotCommAlarmDelayHideChange();
}

void ConfigTreeObject_Comm::resetPartialSettings()
{
    ConfigObject* pObj = ConfigTreeObject::getConfigObject(DISP_DELEGATE_TREND_PARTIAL_SWITCH);
    if(pObj) {
        QVariant valueSwitch = pObj->getData();
        if(valueSwitch.toUInt() == 1) {
            ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Diaplay_Partial)->setHideFlag(false);
            ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Diaplay_Partial_OnOrOff)->setHideFlag(false);
            ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Diaplay_Partial_OnOrOff)->changeData(0, false, false);
            ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Diaplay_Partial_Boundary)->setHideFlag(true);
            ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Diaplay_Partial_Expand)->setHideFlag(true);
        }
    }
}

void ConfigTreeObject_Comm::slotCreateCommsettingTree(QObject *parent)
{
    if (CfgChannel::instance()->getExistChannelComm().isEmpty())
        return;

    //Network setting 配置树
    QFile file(":/configtree/Image/configTree/Comm/Comm_Setting/config.xml");
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file: Network_setting!";
    }else{
        XmlConfigParser::parseConfigXml(file, static_cast<ConfigObject*>(parent));
        file.close();
    }

    //开始通道、结束通道初始化--特殊情况，一般的object都在解析时初始化
    QString keyFirst = CfgChannel::instance()->getExistChannelComm().firstKey();
    quint32 valueFirst = CfgChannel::instance()->getExistChannelComm().value(keyFirst);

    quint32 ID_first = ConfigTreeObject::getConfigObject(COMMChannel_Setting_FirstCH)->getObjectID();
    quint32 ID_last = ConfigTreeObject::getConfigObject(COMMChannel_Setting_LastCH)->getObjectID();

    ConfigTreeObject::getConfigObject(COMMChannel_Setting_FirstCH)->initData(keyFirst, valueFirst, ID_first);
    ConfigTreeObject::getConfigObject(COMMChannel_Setting_LastCH)->initData(keyFirst, valueFirst, ID_last);

    ConfigTreeObject::getConfigObject(COMMChannel_Setting_FirstCH)->setStrShowOn(keyFirst);
    ConfigTreeObject::getConfigObject(COMMChannel_Setting_LastCH)->setStrShowOn(keyFirst);

}

void ConfigTreeObject_Comm::slotCommDelegateChange()
{
    if (CfgChannel::instance()->getExistChannelComm().isEmpty())
        return;

    QString fistChan = ConfigTreeObject::getConfigObject(COMMChannel_Setting_FirstCH)->getData().toString();
    QString lastChan = ConfigTreeObject::getConfigObject(COMMChannel_Setting_LastCH)->getData().toString();
    quint32 firstCoder = CfgChannel::instance()->getExistChannelComm().value(fistChan);
    quint32 lastCoder = CfgChannel::instance()->getExistChannelComm().value(lastChan);

    QFile delegateChange(":/configtree/Image/configTree/Comm/Comm_DelegateChange/delegateChange.xml");
     if(!delegateChange.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file: AI_settingDelegateChange!";
     }else{
        XmlConfigParser::parseDelegateChangeXml(delegateChange, firstCoder, lastCoder);
        delegateChange.close();
     }

     _curStatus = false;
     updateStrShowOn();
     _curStatus = true;
}

void ConfigTreeObject_Comm::slotCommFirstCHChange()
{
    QString chanNumF;
    QString chanNumL;
    ConfigObject *pObj = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_FirstCH);
    ConfigObject *qObj = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_LastCH);

    if(pObj && qObj){
        chanNumF = pObj->getData().toString();
        chanNumL = qObj->getData().toString();

        pObj->setStrShowOn(chanNumF);
        qObj->changeData(chanNumF, false);
    }
}

void ConfigTreeObject_Comm::slotCommLastCHChange()
{
    QString chanNumF;
    QString chanNumL;
    QString tF,tL;

    ConfigObject *pObj = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_FirstCH);
    ConfigObject *qObj = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_LastCH);

    if(pObj && qObj){
        chanNumL = qObj->getData().toString();
        chanNumF = pObj->getData().toString();

        tF = chanNumF;
        tL = chanNumL;

        int value = QString::compare(tF.remove("C"), tL.remove("C"));

        if(value <= 0) {
            qObj->setStrShowOn(chanNumL);
        }
        else{
            qObj->setStrShowOn(chanNumL);
            pObj->changeData(chanNumL ,false);
        }
    }
}



void ConfigTreeObject_Comm::slotCommRangeSwitchChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Range_Range_OnOrOff)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Range_Range_OnOrOff)->getChannelCoder();

    QFile file(XML_COMMChannel_Range_Range_OnOrOff);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
     }
     resetPartialSettings();
}

void ConfigTreeObject_Comm::slotCommRangeDecimalChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Range_Range_Decimal)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Range_Range_Decimal)->getChannelCoder();

    QFile file(XML_COMMChannel_Range_Range_Decimal);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
     }

     ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Alarm_1_OnOrOff)->changeData(0, false);
     ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Alarm_2_OnOrOff)->changeData(0, false);
     ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Alarm_3_OnOrOff)->changeData(0, false);
     ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Alarm_4_OnOrOff)->changeData(0, false);

    slotCommAlarmDelayHideChange();
    slotCommRangeLowerChange();
    slotCommRangeUpperChange();
}

void ConfigTreeObject_Comm::slotCommRangeLowerChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Range_Range_Lower)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Range_Range_Lower)->getChannelCoder();

    QFile file(XML_COMMChannel_Range_Range_Lower);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
    }
    resetPartialSettings();
}

void ConfigTreeObject_Comm::slotCommRangeUpperChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Range_Range_Upper)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Range_Range_Upper)->getChannelCoder();

    QFile file(XML_COMMChannel_Range_Range_Upper);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
    }
    resetPartialSettings();
}

void ConfigTreeObject_Comm::slotCommRangeUnitChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Range_Range_Unit)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Range_Range_Unit)->getChannelCoder();

    QFile file(XML_COMMChannel_Range_Range_Unit);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
     }
}

void ConfigTreeObject_Comm::slotCommRangePowerChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Range_Power)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Range_Power)->getChannelCoder();

    QFile file(XML_COMMChannel_Range_Power);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
     }
}

void ConfigTreeObject_Comm::slotCommRangeValueChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Range_Value)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Range_Value)->getChannelCoder();

    QFile file(XML_COMMChannel_Range_Value);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
     }
}

void ConfigTreeObject_Comm::slotCommRangeWatchDogSwitchChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Range_WatchDog_OnOrOff)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Range_WatchDog_OnOrOff)->getChannelCoder();

    QFile file(XML_COMMChannel_Range_WatchDog_OnOrOff);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
     }
}

void ConfigTreeObject_Comm::slotCommRangeWatchDogTimerChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Range_WatchDog_Timer)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Range_WatchDog_Timer)->getChannelCoder();

    QFile file(XML_COMMChannel_Range_WatchDog_Timer);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
     }
}

void ConfigTreeObject_Comm::slotCommRangeWatchDogValueChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Range_WatchDog_Value)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Range_WatchDog_Value)->getChannelCoder();

    QFile file(XML_COMMChannel_Range_WatchDog_Value);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
     }
}

void ConfigTreeObject_Comm::slotCommAlarmL1SwitchChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Alarm_1_OnOrOff)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Alarm_1_OnOrOff)->getChannelCoder();

    QFile file(XML_COMMChannel_Alarm_1_OnOrOff);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
     }
}

void ConfigTreeObject_Comm::slotCommAlarmL1TypeChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Alarm_1_Type)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Alarm_1_Type)->getChannelCoder();

    QFile file(XML_COMMChannel_Alarm_1_Type);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
     }
}

void ConfigTreeObject_Comm::slotCommAlarmL1ValueChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Alarm_1_Value)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Alarm_1_Value)->getChannelCoder();

    QFile file(XML_COMMChannel_Alarm_1_Value);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
     }
}

void ConfigTreeObject_Comm::slotCommAlarmL1HysteresisChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Alarm_1_Hysteresis)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Alarm_1_Hysteresis)->getChannelCoder();

    QFile file(XML_COMMChannel_Alarm_1_Hysteresis);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
     }
}

void ConfigTreeObject_Comm::slotCommAlarmL1LoggingChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Alarm_1_Logging)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Alarm_1_Logging)->getChannelCoder();

    QFile file(XML_COMMChannel_Alarm_1_Logging);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
     }
}

void ConfigTreeObject_Comm::slotCommAlarmL1OutTypeChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Alarm_1_OutputType)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Alarm_1_OutputType)->getChannelCoder();

    QFile file(XML_COMMChannel_Alarm_1_OutputType);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
     }

     quint32 valueNo = value.toUInt();
     if(0 == valueNo) {

     } else if (1 == valueNo) {
         QMap<QString, quint32> curChanDO = CfgChannel::instance()->getExistChannelDO();
         if(!curChanDO.isEmpty()) {
             ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Alarm_1_OutputNo)->changeData(curChanDO.first(), false);
         }
     } else if (2 == valueNo) {
         ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Alarm_1_OutputNo)->changeData(1, false);
     }
}

void ConfigTreeObject_Comm::slotCommAlarmL1OutNOChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Alarm_1_OutputNo)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Alarm_1_OutputNo)->getChannelCoder();

    QFile file(XML_COMMChannel_Alarm_1_OutputNo);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
     }
}

void ConfigTreeObject_Comm::slotCommAlarmL2SwitchChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Alarm_2_OnOrOff)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Alarm_2_OnOrOff)->getChannelCoder();

    QFile file(XML_COMMChannel_Alarm_2_OnOrOff);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
     }
}

void ConfigTreeObject_Comm::slotCommAlarmL2TypeChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Alarm_2_Type)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Alarm_2_Type)->getChannelCoder();

    QFile file(XML_COMMChannel_Alarm_2_Type);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
     }
}

void ConfigTreeObject_Comm::slotCommAlarmL2ValueChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Alarm_2_Value)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Alarm_2_Value)->getChannelCoder();

    QFile file(XML_COMMChannel_Alarm_2_Value);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
     }
}

void ConfigTreeObject_Comm::slotCommAlarmL2HysteresisChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Alarm_2_Hysteresis)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Alarm_2_Hysteresis)->getChannelCoder();

    QFile file(XML_COMMChannel_Alarm_2_Hysteresis);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
     }
}

void ConfigTreeObject_Comm::slotCommAlarmL2LoggingChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Alarm_2_Logging)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Alarm_2_Logging)->getChannelCoder();

    QFile file(XML_COMMChannel_Alarm_2_Logging);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
     }
}

void ConfigTreeObject_Comm::slotCommAlarmL2OutTypeChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Alarm_2_OutputType)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Alarm_2_OutputType)->getChannelCoder();

    QFile file(XML_COMMChannel_Alarm_2_OutputType);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
     }

     quint32 valueNo = value.toUInt();
     if(0 == valueNo) {

     } else if (1 == valueNo) {
         QMap<QString, quint32> curChanDO = CfgChannel::instance()->getExistChannelDO();
         if(!curChanDO.isEmpty()) {
             ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Alarm_2_OutputNo)->changeData(curChanDO.first(), false);
         }
     } else if (2 == valueNo) {
         ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Alarm_2_OutputNo)->changeData(1, false);
     }
}

void ConfigTreeObject_Comm::slotCommAlarmL2OutNOChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Alarm_2_OutputNo)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Alarm_2_OutputNo)->getChannelCoder();

    QFile file(XML_COMMChannel_Alarm_2_OutputNo);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
     }
}

void ConfigTreeObject_Comm::slotCommAlarmL3SwitchChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Alarm_3_OnOrOff)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Alarm_3_OnOrOff)->getChannelCoder();

    QFile file(XML_COMMChannel_Alarm_3_OnOrOff);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
     }
}

void ConfigTreeObject_Comm::slotCommAlarmL3TypeChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Alarm_3_Type)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Alarm_3_Type)->getChannelCoder();

    QFile file(XML_COMMChannel_Alarm_3_Type);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
     }
}

void ConfigTreeObject_Comm::slotCommAlarmL3ValueChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Alarm_3_Value)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Alarm_3_Value)->getChannelCoder();

    QFile file(XML_COMMChannel_Alarm_3_Value);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
     }
}

void ConfigTreeObject_Comm::slotCommAlarmL3HysteresisChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Alarm_3_Hysteresis)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Alarm_3_Hysteresis)->getChannelCoder();

    QFile file(XML_COMMChannel_Alarm_3_Hysteresis);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
     }
}

void ConfigTreeObject_Comm::slotCommAlarmL3LoggingChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Alarm_3_Logging)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Alarm_3_Logging)->getChannelCoder();

    QFile file(XML_COMMChannel_Alarm_3_Logging);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
     }
}

void ConfigTreeObject_Comm::slotCommAlarmL3OutTypeChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Alarm_3_OutputType)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Alarm_3_OutputType)->getChannelCoder();

    QFile file(XML_COMMChannel_Alarm_3_OutputType);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
     }

     quint32 valueNo = value.toUInt();
     if(0 == valueNo) {

     } else if (1 == valueNo) {
         QMap<QString, quint32> curChanDO = CfgChannel::instance()->getExistChannelDO();
         if(!curChanDO.isEmpty()) {
             ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Alarm_3_OutputNo)->changeData(curChanDO.first(), false);
         }
     } else if (2 == valueNo) {
         ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Alarm_3_OutputNo)->changeData(1, false);
     }
}

void ConfigTreeObject_Comm::slotCommAlarmL3OutNOChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Alarm_3_OutputNo)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Alarm_3_OutputNo)->getChannelCoder();

    QFile file(XML_COMMChannel_Alarm_3_OutputNo);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
     }
}

void ConfigTreeObject_Comm::slotCommAlarmL4SwitchChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Alarm_4_OnOrOff)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Alarm_4_OnOrOff)->getChannelCoder();

    QFile file(XML_COMMChannel_Alarm_4_OnOrOff);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
     }
}

void ConfigTreeObject_Comm::slotCommAlarmL4TypeChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Alarm_4_Type)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Alarm_4_Type)->getChannelCoder();

    QFile file(XML_COMMChannel_Alarm_4_Type);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
     }
}

void ConfigTreeObject_Comm::slotCommAlarmL4ValueChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Alarm_4_Value)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Alarm_4_Value)->getChannelCoder();

    QFile file(XML_COMMChannel_Alarm_4_Value);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
     }
}

void ConfigTreeObject_Comm::slotCommAlarmL4HysteresisChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Alarm_4_Hysteresis)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Alarm_4_Hysteresis)->getChannelCoder();

    QFile file(XML_COMMChannel_Alarm_4_Hysteresis);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
     }
}

void ConfigTreeObject_Comm::slotCommAlarmL4LoggingChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Alarm_4_Logging)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Alarm_4_Logging)->getChannelCoder();

    QFile file(XML_COMMChannel_Alarm_4_Logging);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
     }
}

void ConfigTreeObject_Comm::slotCommAlarmL4OutTypeChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Alarm_4_OutputType)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Alarm_4_OutputType)->getChannelCoder();

    QFile file(XML_COMMChannel_Alarm_4_OutputType);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
     }

     quint32 valueNo = value.toUInt();
     if(0 == valueNo) {

     } else if (1 == valueNo) {
         QMap<QString, quint32> curChanDO = CfgChannel::instance()->getExistChannelDO();
         if(!curChanDO.isEmpty()) {
             ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Alarm_4_OutputNo)->changeData(curChanDO.first(), false);
         }
     } else if (2 == valueNo) {
         ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Alarm_4_OutputNo)->changeData(1, false);
     }
}

void ConfigTreeObject_Comm::slotCommAlarmL4OutNOChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Alarm_4_OutputNo)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Alarm_4_OutputNo)->getChannelCoder();

    QFile file(XML_COMMChannel_Alarm_4_OutputNo);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
     }
}

void ConfigTreeObject_Comm::slotCommAlarmDelayHourChange()
{
    QString num;

    ConfigObject *pObj = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Alarm_Delay_Hour);

    if(pObj){
        num = pObj->getData().toString();

        pObj->setStrShowOn(num);
    }
}

void ConfigTreeObject_Comm::slotCommAlarmDelayMinuteChange()
{
    QString num;

    ConfigObject *pObj = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Alarm_Delay_Minute);

    if(pObj){
        num = pObj->getData().toString();

        pObj->setStrShowOn(num);
    }
}

void ConfigTreeObject_Comm::slotCommAlarmDelaySecendChange()
{
    QString num;

    ConfigObject *pObj = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Alarm_Delay_Secend);

    if(pObj){
        num = pObj->getData().toString();

        pObj->setStrShowOn(num);
    }
}

void ConfigTreeObject_Comm::slotCommDisplayColorChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Diaplay_Color)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Diaplay_Color)->getChannelCoder();

    QFile file(XML_COMMChannel_Diaplay_Color);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
     }
}

void ConfigTreeObject_Comm::slotCommDisplayTagCharactersChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Diaplay_Tag_Characters)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Diaplay_Tag_Characters)->getChannelCoder();

    QFile file(XML_COMMChannel_Diaplay_Tag_Characters);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
     }
}

void ConfigTreeObject_Comm::slotCommDisplayTagNoChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Diaplay_Tag_No)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Diaplay_Tag_No)->getChannelCoder();

    QFile file(XML_COMMChannel_Diaplay_Tag_No);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
     }
}

void ConfigTreeObject_Comm::slotCommDisplayZoneUpperChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Diaplay_Zone_Upper)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Diaplay_Zone_Upper)->getChannelCoder();

    QFile file(XML_COMMChannel_Diaplay_Zone_Upper);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
     }
}

void ConfigTreeObject_Comm::slotCommDisplayZoneLowerChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Diaplay_Zone_Lower)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Diaplay_Zone_Lower)->getChannelCoder();

    QFile file(XML_COMMChannel_Diaplay_Zone_Lower);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
     }
}

void ConfigTreeObject_Comm::slotCommDisplayScalePostionChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Diaplay_Scale_Postion)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Diaplay_Scale_Postion)->getChannelCoder();

    QFile file(XML_COMMChannel_Diaplay_Scale_Postion);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
     }
}

void ConfigTreeObject_Comm::slotCommDisplayScaleDivisionChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Diaplay_Scale_Division)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Diaplay_Scale_Division)->getChannelCoder();

    QFile file(XML_COMMChannel_Diaplay_Scale_Division);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
     }
}

void ConfigTreeObject_Comm::slotCommDisplayBarPositionChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Diaplay_Bar_Position)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Diaplay_Bar_Division)->getChannelCoder();

    QFile file(XML_COMMChannel_Diaplay_Bar_Position);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
     }
}

void ConfigTreeObject_Comm::slotCommDisplayBarDivisionChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Diaplay_Bar_Division)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Diaplay_Bar_Division)->getChannelCoder();

    QFile file(XML_COMMChannel_Diaplay_Bar_Division);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
     }
}

void ConfigTreeObject_Comm::slotCommDisplayPartialOnOrOffChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Diaplay_Partial_OnOrOff)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Diaplay_Partial_OnOrOff)->getChannelCoder();

    QFile file(XML_COMMChannel_Diaplay_Partial_OnOrOff);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
     }
}

void ConfigTreeObject_Comm::slotCommDisplayPartialExpandChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Diaplay_Partial_Expand)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Diaplay_Partial_Expand)->getChannelCoder();

    QFile file(XML_COMMChannel_Diaplay_Partial_Expand);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
     }
}

void ConfigTreeObject_Comm::slotCommDisplayPartialBoundaryChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Diaplay_Partial_Boundary)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Diaplay_Partial_Boundary)->getChannelCoder();

    QFile file(XML_COMMChannel_Diaplay_Partial_Boundary);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
     }
}

void ConfigTreeObject_Comm::slotCommDisplayBandAreaChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Diaplay_Band_Area)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Diaplay_Band_Area)->getChannelCoder();

    QFile file(XML_COMMChannel_Diaplay_Band_Area);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
     }
}

void ConfigTreeObject_Comm::slotCommDisplayBandUpperChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Diaplay_Band_Upper)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Diaplay_Band_Upper)->getChannelCoder();

    QFile file(XML_COMMChannel_Diaplay_Band_Upper);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
     }
}

void ConfigTreeObject_Comm::slotCommDisplayBandLowerChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Diaplay_Band_Lower)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Diaplay_Band_Lower)->getChannelCoder();

    QFile file(XML_COMMChannel_Diaplay_Band_Lower);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
     }
}

void ConfigTreeObject_Comm::slotCommDisplayBandColorChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Diaplay_Band_Color)->getData();

    quint32 coder = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Diaplay_Band_Color)->getChannelCoder();

    QFile file(XML_COMMChannel_Diaplay_Band_Color);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
     }
}

void ConfigTreeObject_Comm::slotCommDisplayMarkScaleChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Diaplay_Mark_Scale)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Diaplay_Mark_Scale)->getChannelCoder();

    QFile file(XML_COMMChannel_Diaplay_Mark_Scale);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
     }
}

void ConfigTreeObject_Comm::slotCommDisplayMarkTypeChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Diaplay_Mark_Type)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Diaplay_Mark_Type)->getChannelCoder();

    QFile file(XML_COMMChannel_Diaplay_Mark_Type);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
     }
}

void ConfigTreeObject_Comm::slotCommDisplayMarkAlarm1Change()
{
    QVariant value = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Diaplay_Mark_Alarm1)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Diaplay_Mark_Alarm1)->getChannelCoder();

    QFile file(XML_COMMChannel_Diaplay_Mark_Alarm1);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
     }
}

void ConfigTreeObject_Comm::slotCommDisplayMarkAlarm2Change()
{
    QVariant value = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Diaplay_Mark_Alarm2)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Diaplay_Mark_Alarm2)->getChannelCoder();

    QFile file(XML_COMMChannel_Diaplay_Mark_Alarm2);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
     }
}

void ConfigTreeObject_Comm::slotCommDisplayMarkAlarm3Change()
{
    QVariant value = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Diaplay_Mark_Alarm3)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Diaplay_Mark_Alarm3)->getChannelCoder();

    QFile file(XML_COMMChannel_Diaplay_Mark_Alarm3);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
     }
}

void ConfigTreeObject_Comm::slotCommDisplayMarkAlarm4Change()
{
    QVariant value = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Diaplay_Mark_Alarm4)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Diaplay_Mark_Alarm4)->getChannelCoder();

    QFile file(XML_COMMChannel_Diaplay_Mark_Alarm4);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
     }
}

void ConfigTreeObject_Comm::slotCommDecimalAndUnitRelationChange()
{
    slotCommRangeLowerChange();
    slotCommRangeUpperChange();

    slotCommAlarmL1ValueChange();
    slotCommAlarmL1HysteresisChange();
    slotCommAlarmL2ValueChange();
    slotCommAlarmL2HysteresisChange();
    slotCommAlarmL3ValueChange();
    slotCommAlarmL3HysteresisChange();
    slotCommAlarmL4ValueChange();
    slotCommAlarmL4HysteresisChange();

    slotCommDisplayPartialBoundaryChange();
    slotCommDisplayBandUpperChange();

    slotCommDisplayBandLowerChange();

}

void ConfigTreeObject_Comm::slotCommAlarmDelayHideChange()
{
    QVariant value1 = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Alarm_1_OnOrOff)->getData();
    QVariant value2 = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Alarm_2_OnOrOff)->getData();
    QVariant value3 = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Alarm_3_OnOrOff)->getData();
    QVariant value4 = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Alarm_4_OnOrOff)->getData();

    if (value1.toInt() == 0 && value2.toInt() == 0 && value3.toInt() == 0 && value4.toInt() == 0) {
        ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Alarm_Delay)->setHideFlag(true);
        ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Alarm_Delay_Hour)->setHideFlag(true);
        ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Alarm_Delay_Minute)->setHideFlag(true);
        ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Alarm_Delay_Secend)->setHideFlag(true);
    }
    else {
        ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Alarm_Delay)->setHideFlag(false);
        ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Alarm_Delay_Hour)->setHideFlag(false);
        ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Alarm_Delay_Minute)->setHideFlag(false);
        ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Alarm_Delay_Secend)->setHideFlag(false);
    }
}






