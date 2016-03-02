#include "configtreeobject_DI.h"
#include "cfgchannel.h"
#include "./GlobalData/globalDef.h"

#include "cfgGlobalDef.h"
#include "configtreeobject.h"

ConfigTreeObject_DI *ConfigTreeObject_DI::instance(QObject *parent)
{
    static ConfigTreeObject_DI data(parent);
    return &data;
}

ConfigTreeObject_DI::ConfigTreeObject_DI(QObject *parent):
    ConfigObject(parent)
{
    //DI channel settings代理树
    QFile fileDelegate(":/configtree/Image/configTree/DI/DI_SettingDelegate/delegate.xml");
    if(!fileDelegate.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file: DI_settingDelegate!";
    }else{
        XmlConfigParser::parseConfigXml(fileDelegate, static_cast<ConfigObject*>(parent), \
                                                                   (QObject*)this);
        fileDelegate.close();
    }
}

/*
 * 功能：
 * 	  DI配置窗体当前界面刷新
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
void ConfigTreeObject_DI::updateStrShowOn()
{
    if (CfgChannel::instance()->getExistChannelDI().isEmpty())
        return;
    else
        ConfigTreeObject::getConfigObject(DI_DELEGATE)->setHideFlag(false);

    updateStrShowRange();
    updateStrShowAlarm();
    updateStrShowDisplay();
}

void ConfigTreeObject_DI::updateStrShowRange()
{
    slotDIRange_RangeTypeChanged();
    slotDIRange_SpanLowerChanged();
    slotDIRange_SpanUpperChanged();
    slotDIRange_RefChannelChanged();

    slotDIRange_ScaleDecPosChanged();
    slotDIRange_ScaleLowerChanged();
    slotDIRange_ScaleUpperChanged();
    slotDIRange_ScaleUnitChanged();

    slotDIRange_CalculationChanged();
}

void ConfigTreeObject_DI::updateStrShowAlarm()
{
    slotDIAlarm_Level1TypeChanged();
    slotDIAlarm_Level1ValueChanged();
    slotDIAlarm_Level1HysterChanged();
    slotDIAlarm_Level1LogChanged();
    slotDIAlarm_Level1OutTypeChanged();
    slotDIAlarm_Level1OutNoChanged();
    slotDIAlarm_Level1OnoffChanged();

    slotDIAlarm_Level2TypeChanged();
    slotDIAlarm_Level2ValueChanged();
    slotDIAlarm_Level2HysterChanged();
    slotDIAlarm_Level2LogChanged();
    slotDIAlarm_Level2OutTypeChanged();
    slotDIAlarm_Level2OutNoChanged();
    slotDIAlarm_Level2OnoffChanged();

    slotDIAlarm_Level3TypeChanged();
    slotDIAlarm_Level3ValueChanged();
    slotDIAlarm_Level3HysterChanged();
    slotDIAlarm_Level3LogChanged();
    slotDIAlarm_Level3OutTypeChanged();
    slotDIAlarm_Level3OutNoChanged();
    slotDIAlarm_Level3OnoffChanged();

    slotDIAlarm_Level4TypeChanged();
    slotDIAlarm_Level4ValueChanged();
    slotDIAlarm_Level4HysterChanged();
    slotDIAlarm_Level4LogChanged();
    slotDIAlarm_Level4OutTypeChanged();
    slotDIAlarm_Level4OutNoChanged();
    slotDIAlarm_Level4OnoffChanged();

    slotDIAlarm_DelayHourChanged();
    slotDIAlarm_DelayMinChanged();
    slotDIAlarm_DelaySecChanged();
}

void ConfigTreeObject_DI::updateStrShowDisplay()
{
    slotDIDisp_ColorColorChanged();

    slotDIDisp_TagCharChanged();
    slotDIDisp_TagNoChanged();

    slotDIDisp_ZoneLowerChanged();
    slotDIDisp_ZoneUpperChanged();

    slotDIDisp_ScalePosChanged();
    slotDIDisp_ScaleDivChanged();

    slotDIDisp_BarBasePosChanged();
    slotDIDisp_BarDivChanged();

    slotDIDisp_AlarmIndicateChanged();
    slotDIDisp_AlarmMarkTypeChanged();
    slotDIDisp_AlarmAlarm1Changed();
    slotDIDisp_AlarmAlarm2Changed();
    slotDIDisp_AlarmAlarm3Changed();
    slotDIDisp_AlarmAlarm4Changed();

    slotDIDisp_DisVal_0_Changed();
    slotDIDisp_DisVal_1_Changed();
}

void ConfigTreeObject_DI::slotCreateDIsettingTree(QObject *parent)
{
    if (CfgChannel::instance()->getExistChannelDI().isEmpty())
        return;

    //DI channel settings配置树
    QFile file(":/configtree/Image/configTree/DI/DI_Setting/config.xml");
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file: DI_setting!";
    }else{
        XmlConfigParser::parseConfigXml(file, static_cast<ConfigObject*>(parent));
        file.close();
    }

    //开始通道、结束通道初始化--特殊情况，一般的object都在解析时初始化
    QString keyFirst = CfgChannel::instance()->getExistChannelDI().firstKey();
    quint32 valueFirst = CfgChannel::instance()->getExistChannelDI().value(keyFirst);

    quint32 ID_first = ConfigTreeObject::getConfigObject(DICONFIG_FIRSTCHAN)->getObjectID();
    quint32 ID_last = ConfigTreeObject::getConfigObject(DICONFIG_LASTCHAN)->getObjectID();

    ConfigTreeObject::getConfigObject(DICONFIG_FIRSTCHAN)->initData(keyFirst, valueFirst, ID_first);
    ConfigTreeObject::getConfigObject(DICONFIG_LASTCHAN)->initData(keyFirst, valueFirst, ID_last);

    ConfigTreeObject::getConfigObject(DICONFIG_FIRSTCHAN)->setStrShowOn(keyFirst);
    ConfigTreeObject::getConfigObject(DICONFIG_LASTCHAN)->setStrShowOn(keyFirst);

}

void ConfigTreeObject_DI::slotDIBoardInoutCfgTree(quint8 unit, quint8 module, quint8 chNum, bool in)
{
    quint32 chanCoder;
    ConfigObject *pConfigParentObj;
    ConfigObject *pConfigObj;
    QFile file;

    if(in) {   //板卡插入
        for(int i=1; i<=chNum; ++i) {
            chanCoder = CHANNEL_DI_CODER(unit, module, i);
            int index = CfgChannel::instance()->getExistChannelDI().values().indexOf(chanCoder);
            QString chanNum = CfgChannel::instance()->getExistChannelDI().keys().at(index);

            pConfigParentObj = ConfigTreeObject::getConfigObject(DICONFIG_RANG);
            pConfigObj = new ConfigObject(pConfigParentObj);
            pConfigObj->setObjectName(chanNum);
            file.setFileName(":/configtree/Image/configTree/DI/DI_Setting/config-Range.xml");
            if(!file.open(QFile::ReadOnly | QFile::Text)){
                qWarning()<< "Can't open the file: config-Range.xml";
            }else{
                XmlConfigParser::parseConfigXml(file, pConfigObj, 0, chanCoder);
                file.close();
            }

            pConfigParentObj = ConfigTreeObject::getConfigObject(DICONFIG_ALARM);
            pConfigObj = new ConfigObject(pConfigParentObj);
            pConfigObj->setObjectName(chanNum);
            file.setFileName(":/configtree/Image/configTree/DI/DI_Setting/config-Alarm.xml");
            if(!file.open(QFile::ReadOnly | QFile::Text)){
                qWarning()<< "Can't open the file: config-Alarm.xml";
            }else{
                XmlConfigParser::parseConfigXml(file, pConfigObj, 0, chanCoder);
                file.close();
            }

            pConfigParentObj = ConfigTreeObject::getConfigObject(DICONFIG_DISP);
            pConfigObj = new ConfigObject(pConfigParentObj);
            pConfigObj->setObjectName(chanNum);
            file.setFileName(":/configtree/Image/configTree/DI/DI_Setting/config-Display settings.xml");
            if(!file.open(QFile::ReadOnly | QFile::Text)){
                qWarning()<< "Can't open the file: config-Display settings.xml";
            }else{
                XmlConfigParser::parseConfigXml(file, pConfigObj, 0, chanCoder);
                file.close();
            }
        }
    } else {  //板卡拔出
        for(int i=1; i<=chNum; ++i) {
            chanCoder = CHANNEL_DI_CODER(unit, module, i);
            int index = CfgChannel::instance()->getExistChannelDI().values().indexOf(chanCoder);
            QString chanNum = CfgChannel::instance()->getExistChannelDI().keys().at(index);

            QString delPath;
            delPath = QString(DICONFIG_RANG) + ":" + QString(chanNum);
            ConfigTreeObject::destroyConfigTree(delPath);
            delPath = QString(DICONFIG_ALARM) + ":" + QString(chanNum);
            ConfigTreeObject::destroyConfigTree(delPath);
            delPath = QString(DICONFIG_DISP) + ":" + QString(chanNum);
            ConfigTreeObject::destroyConfigTree(delPath);
        }
    }
}

/*
 * 功能：
 * 	  DI配置窗体当前通道选择更改处理：修改所有代理配置对象，使它们
 *    根据当前选择的通道号指向相应的配置对象，同时刷新当前界面
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
void ConfigTreeObject_DI::slotDIDelegateChange()
{
    if (CfgChannel::instance()->getExistChannelDI().isEmpty())
        return;

    QString fistChan = ConfigTreeObject::getConfigObject(DICONFIG_FIRSTCHAN)->getData().toString();
    QString lastChan = ConfigTreeObject::getConfigObject(DICONFIG_LASTCHAN)->getData().toString();
    quint32 firstCoder = CfgChannel::instance()->getExistChannelDI().value(fistChan);
    quint32 lastCoder = CfgChannel::instance()->getExistChannelDI().value(lastChan);
    QFile delegateChange(":/configtree/Image/configTree/DI/DI_DelegateChange/delegateChange.xml");
    if(!delegateChange.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file: DI_settingDelegateChange!";
    }else{
        XmlConfigParser::parseDelegateChangeXml(delegateChange, firstCoder, lastCoder);
        delegateChange.close();
    }

    _curStatus = false;
    updateStrShowOn();
    _curStatus = true;
}

void ConfigTreeObject_DI::slotDIFirst_chanSelectchanged()
{
    QString chanNumF;
    QString chanNumL;
    ConfigObject *pObj = ConfigTreeObject::getConfigObject(DI_DELEGATE_FIRSTCHAN);
    ConfigObject *qObj = ConfigTreeObject::getConfigObject(DI_DELEGATE_LASTCHAN);

    if(pObj && qObj){
        chanNumF = pObj->getData().toString();
        chanNumL = qObj->getData().toString();

        pObj->setStrShowOn(chanNumF);
        qObj->changeData(chanNumF, false);
    }
}

void ConfigTreeObject_DI::slotDILast_chanSelectchanged()
{
    QString chanNumF;
    QString chanNumL;
    ConfigObject *pObj = ConfigTreeObject::getConfigObject(DI_DELEGATE_FIRSTCHAN);
    ConfigObject *qObj = ConfigTreeObject::getConfigObject(DI_DELEGATE_LASTCHAN);

    if(pObj && qObj){
        chanNumL = qObj->getData().toString();
        chanNumF = pObj->getData().toString();
        int value = QString::compare(chanNumF, chanNumL);
        if(value <= 0) {
            qObj->setStrShowOn(chanNumL);
        }
        else{
            qObj->setStrShowOn(chanNumL);
            pObj->changeData(chanNumL ,false);
        }
    }
}

void ConfigTreeObject_DI::slotDIRange_RangeTypeChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(DI_DELEGATE_RANG_RANGE_TYPE)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(DI_DELEGATE_RANG_RANGE_TYPE)->getChannelCoder();
    QFile file(XML_DI_RANGE_RANGE_TYPE);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:DIRange_RangeType!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
    }
}

void ConfigTreeObject_DI::slotDIRange_SpanLowerChanged()
{
    ConfigObject *rObj = ConfigTreeObject::getConfigObject(DI_DELEGATE_RANG_RANGE_LOWER);
    ConfigObject *sObj = ConfigTreeObject::getConfigObject(DI_DELEGATE_RANG_RANGE_UPPER);

    float value = rObj->getData().toFloat();    //Low
    float value_t = sObj->getData().toFloat();  //Up
    if(value == value_t) {
        if(value_t == (ConfigTreeObject::getConfigData(DI_DELEGATE_RANG_RANGE_MAX).toFloat())) {
            value_t = ConfigTreeObject::getConfigData(DI_DELEGATE_RANG_RANGE_MIN).toFloat();
        }else{
            value_t = ConfigTreeObject::getConfigData(DI_DELEGATE_RANG_RANGE_MAX).toFloat();
        }
        sObj->changeData(value_t, false);
    }

//    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(DI_DELEGATE_RANG_RANGE_LOWER);
    quint32 ID = ID_DI_Range_Range_DispMin;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, rObj);
    rObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_DI::slotDIRange_SpanUpperChanged()
{
    ConfigObject *rObj = ConfigTreeObject::getConfigObject(DI_DELEGATE_RANG_RANGE_UPPER);
    ConfigObject *sObj = ConfigTreeObject::getConfigObject(DI_DELEGATE_RANG_RANGE_LOWER);

    float value = rObj->getData().toFloat();    //Up
    float value_t = sObj->getData().toFloat();  //Low
    if(value == value_t) {
        if(value_t == (ConfigTreeObject::getConfigData(DI_DELEGATE_RANG_RANGE_MIN).toFloat())) {
            value_t = ConfigTreeObject::getConfigData(DI_DELEGATE_RANG_RANGE_MAX).toFloat();
        }else{
            value_t = ConfigTreeObject::getConfigData(DI_DELEGATE_RANG_RANGE_MIN).toFloat();
        }
        sObj->changeData(value_t, false);
    }

//    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(DI_DELEGATE_RANG_RANGE_UPPER);
    quint32 ID = ID_DI_Range_Range_DispMax;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, rObj);
    rObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_DI::slotDIRange_CalculationChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(DI_DELEGATE_RANG_RANGE_CALCU)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(DI_DELEGATE_RANG_RANGE_CALCU)->getChannelCoder();
    QFile file(XML_DI_RANGE_RANGE_CALCU);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:DIRange_RangeCalcu!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
    }
}

void ConfigTreeObject_DI::slotDIRange_RefChannelChanged()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(DI_DELEGATE_RANG_RANGE_REFER);
    quint32 ID = ID_DI_Range_Range_BasicChannel;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_DI::slotDIRange_ScaleDecPosChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(DI_DELEGATE_RANG_SCALE_DEC)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(DI_DELEGATE_RANG_SCALE_DEC)->getChannelCoder();
    QFile file(XML_DI_RANGE_SCALE_DEC);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:DIRange_ScaleDec!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
    }
}

void ConfigTreeObject_DI::slotDIRange_ScaleLowerChanged()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(DI_DELEGATE_RANG_SCALE_LOWER);
    quint32 ID = ID_DI_Range_Scale_Min;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_DI::slotDIRange_ScaleUpperChanged()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(DI_DELEGATE_RANG_SCALE_UPPER);
    quint32 ID = ID_DI_Range_Scale_Max;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_DI::slotDIRange_ScaleUnitChanged()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(DI_DELEGATE_RANG_SCALE_UNIT);
    quint32 ID = ID_DI_Range_Scale_Unit;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_DI::slotDIAlarm_Level1OnoffChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(DI_DELEGATE_ALARM_LVL1_ONOFF)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(DI_DELEGATE_ALARM_LVL1_ONOFF)->getChannelCoder();

    QFile file(XML_DI_ALARM_LVL1_ONOFF);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:DIAlarm_Lvl1Onoff!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
    }
}

void ConfigTreeObject_DI::slotDIAlarm_Level1TypeChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(DI_DELEGATE_ALARM_LVL1_TYPE)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(DI_DELEGATE_ALARM_LVL1_TYPE)->getChannelCoder();
    QFile file(XML_DI_ALARM_LVL1_TYPE);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:DIAlarm_Lvl1Type!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
    }
}

void ConfigTreeObject_DI::slotDIAlarm_Level1ValueChanged()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(DI_DELEGATE_ALARM_LVL1_VALUE);
    quint32 ID = ID_DI_Alarm_Level1_Value;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_DI::slotDIAlarm_Level1HysterChanged()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(DI_DELEGATE_ALARM_LVL1_HYST);
    quint32 ID = ID_DI_Alarm_Level1_Hysteresis;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_DI::slotDIAlarm_Level1LogChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(DI_DELEGATE_ALARM_LVL1_LOGG)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(DI_DELEGATE_ALARM_LVL1_LOGG)->getChannelCoder();
    QFile file(XML_DI_ALARM_LVL1_LOGG);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:DIAlarm_Lvl1Logg!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
    }
}

void ConfigTreeObject_DI::slotDIAlarm_Level1OutTypeChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(DI_DELEGATE_ALARM_LVL1_OUTTYPE)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(DI_DELEGATE_ALARM_LVL1_OUTTYPE)->getChannelCoder();
    QFile file(XML_DI_ALARM_LVL1_OUTTYPE);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:DIAlarm_Lvl1Outype!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
    }

    quint32 valueNo = value.toUInt();
    if(0 == valueNo) {

    } else if (1 == valueNo) {
        QMap<QString, quint32> curChanDO = CfgChannel::instance()->getExistChannelDO();
        if(!curChanDO.isEmpty()) {
            ConfigTreeObject::getConfigObject(DI_DELEGATE_ALARM_LVL1_OUTNO)->changeData(curChanDO.first(), false);
        }
    } else if (2 == valueNo) {
        ConfigTreeObject::getConfigObject(DI_DELEGATE_ALARM_LVL1_OUTNO)->changeData(1, false);
    }
}

void ConfigTreeObject_DI::slotDIAlarm_Level1OutNoChanged()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(DI_DELEGATE_ALARM_LVL1_OUTNO);
    quint32 ID = ID_DI_Alarm_Level1_OutputNo;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_DI::slotDIAlarm_Level2OnoffChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(DI_DELEGATE_ALARM_LVL2_ONOFF)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(DI_DELEGATE_ALARM_LVL2_ONOFF)->getChannelCoder();

    QFile file(XML_DI_ALARM_LVL2_ONOFF);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:DIAlarm_Lvl2Onoff!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
    }
}

void ConfigTreeObject_DI::slotDIAlarm_Level2TypeChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(DI_DELEGATE_ALARM_LVL2_TYPE)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(DI_DELEGATE_ALARM_LVL2_TYPE)->getChannelCoder();
    QFile file(XML_DI_ALARM_LVL2_TYPE);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:DIAlarm_Lvl2Type!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
    }
}

void ConfigTreeObject_DI::slotDIAlarm_Level2ValueChanged()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(DI_DELEGATE_ALARM_LVL2_VALUE);
    quint32 ID = ID_DI_Alarm_Level2_Value;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_DI::slotDIAlarm_Level2HysterChanged()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(DI_DELEGATE_ALARM_LVL2_HYST);
    quint32 ID = ID_DI_Alarm_Level2_Hysteresis;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_DI::slotDIAlarm_Level2LogChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(DI_DELEGATE_ALARM_LVL2_LOGG)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(DI_DELEGATE_ALARM_LVL2_LOGG)->getChannelCoder();
    QFile file(XML_DI_ALARM_LVL2_LOGG);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:DIAlarm_Lvl2Logg!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
    }
}

void ConfigTreeObject_DI::slotDIAlarm_Level2OutTypeChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(DI_DELEGATE_ALARM_LVL2_OUTTYPE)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(DI_DELEGATE_ALARM_LVL2_OUTTYPE)->getChannelCoder();
    QFile file(XML_DI_ALARM_LVL2_OUTTYPE);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:DIAlarm_Lvl2Outype!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
    }

    quint32 valueNo = value.toUInt();
    if(0 == valueNo) {

    } else if (1 == valueNo) {
        QMap<QString, quint32> curChanDO = CfgChannel::instance()->getExistChannelDO();
        if(!curChanDO.isEmpty()) {
            ConfigTreeObject::getConfigObject(DI_DELEGATE_ALARM_LVL2_OUTNO)->changeData(curChanDO.first(), false);
        }
    } else if (2 == valueNo) {
        ConfigTreeObject::getConfigObject(DI_DELEGATE_ALARM_LVL2_OUTNO)->changeData(1, false);
    }
}

void ConfigTreeObject_DI::slotDIAlarm_Level2OutNoChanged()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(DI_DELEGATE_ALARM_LVL2_OUTNO);
    quint32 ID = ID_DI_Alarm_Level2_OutputNo;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_DI::slotDIAlarm_Level3OnoffChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(DI_DELEGATE_ALARM_LVL3_ONOFF)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(DI_DELEGATE_ALARM_LVL3_ONOFF)->getChannelCoder();

    QFile file(XML_DI_ALARM_LVL3_ONOFF);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:DIAlarm_Lvl3Onoff!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
    }
}

void ConfigTreeObject_DI::slotDIAlarm_Level3TypeChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(DI_DELEGATE_ALARM_LVL3_TYPE)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(DI_DELEGATE_ALARM_LVL3_TYPE)->getChannelCoder();
    QFile file(XML_DI_ALARM_LVL3_TYPE);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:DIAlarm_Lvl3Type!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
    }
}

void ConfigTreeObject_DI::slotDIAlarm_Level3ValueChanged()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(DI_DELEGATE_ALARM_LVL3_VALUE);
    quint32 ID = ID_DI_Alarm_Level3_Value;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_DI::slotDIAlarm_Level3HysterChanged()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(DI_DELEGATE_ALARM_LVL3_HYST);
    quint32 ID = ID_DI_Alarm_Level3_Hysteresis;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_DI::slotDIAlarm_Level3LogChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(DI_DELEGATE_ALARM_LVL3_LOGG)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(DI_DELEGATE_ALARM_LVL3_LOGG)->getChannelCoder();
    QFile file(XML_DI_ALARM_LVL3_LOGG);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:DIAlarm_Lvl3Logg!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
    }
}

void ConfigTreeObject_DI::slotDIAlarm_Level3OutTypeChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(DI_DELEGATE_ALARM_LVL3_OUTTYPE)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(DI_DELEGATE_ALARM_LVL3_OUTTYPE)->getChannelCoder();
    QFile file(XML_DI_ALARM_LVL3_OUTTYPE);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:DIAlarm_Lvl3Outype!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
    }

    quint32 valueNo = value.toUInt();
    if(0 == valueNo) {

    } else if (1 == valueNo) {
        QMap<QString, quint32> curChanDO = CfgChannel::instance()->getExistChannelDO();
        if(!curChanDO.isEmpty()) {
            ConfigTreeObject::getConfigObject(DI_DELEGATE_ALARM_LVL3_OUTNO)->changeData(curChanDO.first(), false);
        }
    } else if (2 == valueNo) {
        ConfigTreeObject::getConfigObject(DI_DELEGATE_ALARM_LVL3_OUTNO)->changeData(1, false);
    }
}

void ConfigTreeObject_DI::slotDIAlarm_Level3OutNoChanged()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(DI_DELEGATE_ALARM_LVL3_OUTNO);
    quint32 ID = ID_DI_Alarm_Level3_OutputNo;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

//alarm level 4
void ConfigTreeObject_DI::slotDIAlarm_Level4OnoffChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(DI_DELEGATE_ALARM_LVL4_ONOFF)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(DI_DELEGATE_ALARM_LVL4_ONOFF)->getChannelCoder();

    QFile file(XML_DI_ALARM_LVL4_ONOFF);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:DIAlarm_Lvl4Onoff!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
    }
}

void ConfigTreeObject_DI::slotDIAlarm_Level4TypeChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(DI_DELEGATE_ALARM_LVL4_TYPE)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(DI_DELEGATE_ALARM_LVL4_TYPE)->getChannelCoder();
    QFile file(XML_DI_ALARM_LVL4_TYPE);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:DIAlarm_Lvl4Type!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
    }
}

void ConfigTreeObject_DI::slotDIAlarm_Level4ValueChanged()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(DI_DELEGATE_ALARM_LVL4_VALUE);
    quint32 ID = ID_DI_Alarm_Level4_Value;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_DI::slotDIAlarm_Level4HysterChanged()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(DI_DELEGATE_ALARM_LVL4_HYST);
    quint32 ID = ID_DI_Alarm_Level4_Hysteresis;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_DI::slotDIAlarm_Level4LogChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(DI_DELEGATE_ALARM_LVL4_LOGG)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(DI_DELEGATE_ALARM_LVL4_LOGG)->getChannelCoder();
    QFile file(XML_DI_ALARM_LVL4_LOGG);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:DIAlarm_Lvl4Logg!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
    }
}

void ConfigTreeObject_DI::slotDIAlarm_Level4OutTypeChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(DI_DELEGATE_ALARM_LVL4_OUTTYPE)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(DI_DELEGATE_ALARM_LVL4_OUTTYPE)->getChannelCoder();
    QFile file(XML_DI_ALARM_LVL4_OUTTYPE);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:DIAlarm_Lvl4Outype!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
    }

    quint32 valueNo = value.toUInt();
    if(0 == valueNo) {

    } else if (1 == valueNo) {
        QMap<QString, quint32> curChanDO = CfgChannel::instance()->getExistChannelDO();
        if(!curChanDO.isEmpty()) {
            ConfigTreeObject::getConfigObject(DI_DELEGATE_ALARM_LVL4_OUTNO)->changeData(curChanDO.first(), false);
        }
    } else if (2 == valueNo) {
        ConfigTreeObject::getConfigObject(DI_DELEGATE_ALARM_LVL4_OUTNO)->changeData(1, false);
    }
}

void ConfigTreeObject_DI::slotDIAlarm_Level4OutNoChanged()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(DI_DELEGATE_ALARM_LVL4_OUTNO);
    quint32 ID = ID_DI_Alarm_Level4_OutputNo;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_DI::slotDIAlarm_DelayHourChanged()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(DI_DELEGATE_ALARM_DELAY_HOUR);
    quint32 ID = ID_DI_Alarm_Leve_Delay_h;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_DI::slotDIAlarm_DelayMinChanged()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(DI_DELEGATE_ALARM_DELAY_MIN);
    quint32 ID = ID_DI_Alarm_Leve_Delay_m;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_DI::slotDIAlarm_DelaySecChanged()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(DI_DELEGATE_ALARM_DELAY_SEC);
    quint32 ID = ID_DI_Alarm_Leve_Delay_s;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_DI::slotDIDisp_ColorColorChanged()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(DI_DELEGATE_DISP_COLOR_COLOR);
    quint32 ID = ID_DI_Disp_Color_Value;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_DI::slotDIDisp_TagCharChanged()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(DI_DELEGATE_DISP_TAG_CHAR);
    quint32 ID = ID_DI_Disp_Tag_Char;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_DI::slotDIDisp_TagNoChanged()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(DI_DELEGATE_DISP_TAG_NO);
    quint32 ID = ID_DI_Disp_Tag_No;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_DI::slotDIDisp_ZoneLowerChanged()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(DI_DELEGATE_DISP_ZONE_LOWER);
    quint32 ID = ID_DI_Disp_Zone_Lower;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_DI::slotDIDisp_ZoneUpperChanged()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(DI_DELEGATE_DISP_ZONE_UPPER);
    quint32 ID = ID_DI_Disp_Zone_Upper;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_DI::slotDIDisp_ScalePosChanged()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(DI_DELEGATE_DISP_SCALE_POS);
    quint32 ID = ID_DI_Disp_Scale_Pos;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_DI::slotDIDisp_ScaleDivChanged()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(DI_DELEGATE_DISP_SCALE_DIV);
    quint32 ID = ID_DI_Disp_Scale_Div;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_DI::slotDIDisp_BarBasePosChanged()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(DI_DELEGATE_DISP_BAR_BASEPOS);
    quint32 ID = ID_DI_DISP_Bar_Pos;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_DI::slotDIDisp_BarDivChanged()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(DI_DELEGATE_DISP_BAR_DIVISION);
    quint32 ID = ID_DI_DISP_Bar_Div;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_DI::slotDIDisp_AlarmIndicateChanged()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(DI_DELEGATE_DISP_ALARM_INDIC);
    quint32 ID = ID_DI_DISP_AlarmMark_OnOff;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_DI::slotDIDisp_AlarmMarkTypeChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(DI_DELEGATE_DISP_ALARM_MARKTYPE)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(DI_DELEGATE_DISP_ALARM_MARKTYPE)->getChannelCoder();
    QFile file(XML_DI_DISP_ALARM_MARKTYPE);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:DIDisp_AlarmMarkType!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
    }
}

void ConfigTreeObject_DI::slotDIDisp_AlarmAlarm1Changed()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(DI_DELEGATE_DISP_ALARM_COLOR1);
    quint32 ID = ID_DI_DISP_AlarmMark_Color1;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_DI::slotDIDisp_AlarmAlarm2Changed()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(DI_DELEGATE_DISP_ALARM_COLOR2);
    quint32 ID = ID_DI_DISP_AlarmMark_Color2;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_DI::slotDIDisp_AlarmAlarm3Changed()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(DI_DELEGATE_DISP_ALARM_COLOR3);
    quint32 ID = ID_DI_DISP_AlarmMark_Color3;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_DI::slotDIDisp_AlarmAlarm4Changed()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(DI_DELEGATE_DISP_ALARM_COLOR4);
    quint32 ID = ID_DI_DISP_AlarmMark_Color4;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_DI::slotDIDisp_DisVal_0_Changed()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(DI_DELEGATE_DISP_DISPCHAR_0);
    quint32 ID = ID_DI_DISP_DI0_Str;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_DI::slotDIDisp_DisVal_1_Changed()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(DI_DELEGATE_DISP_DISPCHAR_1);
    quint32 ID = ID_DI_DISP_DI1_Str;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

