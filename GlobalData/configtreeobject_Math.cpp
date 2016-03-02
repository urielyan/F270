#include "configtreeobject_Math.h"
#include "cfgchannel.h"
#include "./GlobalData/globalDef.h"

#include "cfgGlobalDef.h"
#include "configtreeobject.h"
#include "../Common/winmenu.h"

ConfigTreeObject_Math *ConfigTreeObject_Math::instance(QObject *parent)
{
    static ConfigTreeObject_Math data(parent);
    return &data;
}


ConfigTreeObject_Math::ConfigTreeObject_Math(QObject *parent):
      ConfigObject(parent)
{
    //Math channel settings代理树
    QFile fileDelegate(":/configtree/Image/configTree/Math/Math_SettingDelegate/delegate.xml");
    if(!fileDelegate.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file: Math_settingDelegate!";
    }else{
        XmlConfigParser::parseConfigXml(fileDelegate, static_cast<ConfigObject*>(parent), \
                                                          (QObject*)this);
        fileDelegate.close();
    }
}

void ConfigTreeObject_Math::updateStrShowOn()
{
    if (CfgChannel::instance()->getExistChannelMath().isEmpty())
        return;
    else
        ConfigTreeObject::getConfigObject(MATH_DELEGATE)->setHideFlag(false);

    updateStrShowExp();
    updateStrShowAlarm();
    updateStrShowDisplay();
    updateStrShowConstK();
    updateStrShowAction();
}

void ConfigTreeObject_Math::updateStrShowExp()
{
    slotMathExp_RangeExpChanged();
    slotMathExp_RangeDecChanged();
    slotMathExp_RangeLowerChanged();
    slotMathExp_RangeUpperChanged();
    slotMathExp_RangeUnitChanged();
    slotMathExp_RangeSwitchChanged();

    slotMathExp_TLOGTypeChanged();
    slotMathExp_TLOGNoChanged();
    slotMathExp_TLOGSumChanged();
    slotMathExp_TLOGResetChanged();

    slotMathExp_RollSwitchChanged();
    slotMathExp_RollIntervalChanged();
    slotMathExp_RollSampleChanged();
}

void ConfigTreeObject_Math::updateStrShowAlarm()
{
    slotMathAlarm_Level1TypeChanged();
    slotMathAlarm_Level1ValueChanged();
    slotMathAlarm_Level1HysterChanged();
    slotMathAlarm_Level1LoggChanged();
    slotMathAlarm_Level1OutTypeChanged();
    slotMathAlarm_Level1OutNoChanged();
    slotMathAlarm_Level1OnoffChanged();

    slotMathAlarm_Level2TypeChanged();
    slotMathAlarm_Level2ValueChanged();
    slotMathAlarm_Level2HysterChanged();
    slotMathAlarm_Level2LoggChanged();
    slotMathAlarm_Level2OutTypeChanged();
    slotMathAlarm_Level2OutNoChanged();
    slotMathAlarm_Level2OnoffChanged();

    slotMathAlarm_Level3TypeChanged();
    slotMathAlarm_Level3ValueChanged();
    slotMathAlarm_Level3HysterChanged();
    slotMathAlarm_Level3LoggChanged();
    slotMathAlarm_Level3OutTypeChanged();
    slotMathAlarm_Level3OutNoChanged();
    slotMathAlarm_Level3OnoffChanged();

    slotMathAlarm_Level4TypeChanged();
    slotMathAlarm_Level4ValueChanged();
    slotMathAlarm_Level4HysterChanged();
    slotMathAlarm_Level4LoggChanged();
    slotMathAlarm_Level4OutTypeChanged();
    slotMathAlarm_Level4OutNoChanged();
    slotMathAlarm_Level4OnoffChanged();

    slotMathAlarm_DelayHourChanged();
    slotMathAlarm_DelayMinChanged();
    slotMathAlarm_DelaySecChanged();

}

void ConfigTreeObject_Math::updateStrShowDisplay()
{
    slotMathDisp_ColorColorChanged();

    slotMathDisp_TagCharChanged();
    slotMathDisp_TagNoChanged();

    slotMathDisp_ZoneLowerChanged();
    slotMathDisp_ZoneUpperChanged();

    slotMathDisp_ScalePosChanged();
    slotMathDisp_ScaleDivChanged();

    slotMathDisp_BarBasePosChanged();
    slotMathDisp_BarDivChanged();

    slotMathDisp_PartOnoffChanged();
    slotMathDisp_PartExpandChanged();
    slotMathDisp_PartBoundChanged();

    slotMathDisp_CsbandBandareaChanged();
    slotMathDisp_CsbandDispLowChanged();
    slotMathDisp_CsbandDispUpChanged();
    slotMathDisp_CsbandColorChanged();

    slotMathDisp_AlarmIndicateChanged();
    slotMathDisp_AlarmMarkTypeChanged();
    slotMathDisp_AlarmAlarm1Changed();
    slotMathDisp_AlarmAlarm2Changed();
    slotMathDisp_AlarmAlarm3Changed();
    slotMathDisp_AlarmAlarm4Changed();
}

void ConfigTreeObject_Math::updateStrShowConstK()
{
    slotMathConstantKChanged();
    updateAllConstKInfoMenu();
}

void ConfigTreeObject_Math::updateStrShowAction()
{
    slotMathAction_ValueErrorChanged();
    slotMathAction_KeyActionChanged();
    slotMathAction_ValueSumChanged();
    slotMathAction_ValuePPChanged();
}

void ConfigTreeObject_Math::updateAllConstKInfoMenu()
{
    ConfigObject *pConfigObj;
    QString valueStr;
    for(int i=0;i<100;++i) {
        pConfigObj = ConfigTreeObject::getConfigObject(QString(MATH_DELEGATE_CONSTANTNUM_INDEX).arg(i+1, 3, 10, QChar('0')));
        valueStr = pConfigObj->getData().toString().toUpper();
        pConfigObj->setStrShowOn(valueStr);
    }
}

void ConfigTreeObject_Math::resetPartialSettings()
{
    ConfigObject* pObj = ConfigTreeObject::getConfigObject(DISP_DELEGATE_TREND_PARTIAL_SWITCH);
    if(pObj) {
        QVariant valueSwitch = pObj->getData();
        if(valueSwitch.toUInt() == 1) {
            ConfigTreeObject::getConfigObject(MATH_DELEGATE_DISP_PART)->setHideFlag(false);
            ConfigTreeObject::getConfigObject(MATH_DELEGATE_DISP_PART_ONOFF)->setHideFlag(false);
            ConfigTreeObject::getConfigObject(MATH_DELEGATE_DISP_PART_ONOFF)->changeData(0, false, false);
            ConfigTreeObject::getConfigObject(MATH_DELEGATE_DISP_PART_BOUND)->setHideFlag(true);
            ConfigTreeObject::getConfigObject(MATH_DELEGATE_DISP_PART_EXPAND)->setHideFlag(true);
        }
    }
}

void ConfigTreeObject_Math::slotCreateMathsettingTree(QObject *parent)
{
    if (CfgChannel::instance()->getExistChannelMath().isEmpty())
        return;

    //Math channel settings
    QFile file(":/configtree/Image/configTree/Math/Math_Setting/config.xml");
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file: Math_setting!";
    }else{
        XmlConfigParser::parseConfigXml(file, static_cast<ConfigObject*>(parent));
        file.close();
    }

    //开始通道、结束通道初始化--特殊情况，一般的object都在解析时初始化
    QString keyFirst = CfgChannel::instance()->getExistChannelMath().firstKey();
    quint32 valueFirst = CfgChannel::instance()->getExistChannelMath().value(keyFirst);

    quint32 ID_first = ConfigTreeObject::getConfigObject(MATHCFG_FIRSTCHAN)->getObjectID();
    quint32 ID_last = ConfigTreeObject::getConfigObject(MATHCFG_LASTCHAN)->getObjectID();

    ConfigTreeObject::getConfigObject(MATHCFG_FIRSTCHAN)->initData(keyFirst, valueFirst, ID_first);
    ConfigTreeObject::getConfigObject(MATHCFG_LASTCHAN)->initData(keyFirst, valueFirst, ID_last);

    ConfigTreeObject::getConfigObject(MATHCFG_FIRSTCHAN)->setStrShowOn(keyFirst);
    ConfigTreeObject::getConfigObject(MATHCFG_LASTCHAN)->setStrShowOn(keyFirst);

    quint32 ID_const = ConfigTreeObject::getConfigObject(MATHCFG_CONSTANTNUM)->getObjectID();
    ConfigTreeObject::getConfigObject(MATHCFG_CONSTANTNUM)->initData(0, 0, ID_const);
    ConfigTreeObject::getConfigObject(MATHCFG_CONSTANTNUM)->setStrShowOn("K001 - K010");

}

void ConfigTreeObject_Math::slotMathDelegateChange()
{
    if (CfgChannel::instance()->getExistChannelMath().isEmpty())
        return;

    QString fistChan = ConfigTreeObject::getConfigObject(MATHCFG_FIRSTCHAN)->getData().toString();
    QString lastChan = ConfigTreeObject::getConfigObject(MATHCFG_LASTCHAN)->getData().toString();
    quint32 firstCoder = CfgChannel::instance()->getExistChannelMath().value(fistChan);
    quint32 lastCoder = CfgChannel::instance()->getExistChannelMath().value(lastChan);
    QFile delegateChange(":/configtree/Image/configTree/Math/Math_DelegateChange/delegateChange.xml");
     if(!delegateChange.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file: Math_settingDelegateChange!";
     }else{
        XmlConfigParser::parseDelegateChangeXml(delegateChange, firstCoder, lastCoder);
        delegateChange.close();
     }

     _curStatus = false;
     updateStrShowOn();
     _curStatus = true;
}

void ConfigTreeObject_Math::slotMathFirst_chanSelectchanged()
{
    QString chanNumF;
    QString chanNumL;
    ConfigObject *pObj = ConfigTreeObject::getConfigObject(MATH_DELEGATE_FIRSTCHAN);
    ConfigObject *qObj = ConfigTreeObject::getConfigObject(MATH_DELEGATE_LASTCHAN);

    if(pObj && qObj){
        chanNumF = pObj->getData().toString();
        chanNumL = qObj->getData().toString();

        pObj->setStrShowOn(chanNumF);
        qObj->changeData(chanNumF, false);
    }
}

void ConfigTreeObject_Math::slotMathLast_chanSelectchanged()
{
    QString chanNumF;
    QString chanNumL;
    ConfigObject *pObj = ConfigTreeObject::getConfigObject(MATH_DELEGATE_FIRSTCHAN);
    ConfigObject *qObj = ConfigTreeObject::getConfigObject(MATH_DELEGATE_LASTCHAN);

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

void ConfigTreeObject_Math::slotMathConstantKChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(MATH_DELEGATE_CONSTANTNUM)->getData();

    QFile file(XML_MATH_CONSTANTNUM);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:MathConstK_Num!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Math::slotMathConstantKItemChanged()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    QString strName = menu->objectName();
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(QString(MATH_DELEGATE_CONSTANTNUM_X).arg(strName));
    QString valueStr = pConfigObj->getData().toString().toUpper();
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_Math::slotMathExp_RangeSwitchChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(MATH_DELEGATE_CALCULEXP_RANGE_SWITCH)->getData();

    QFile file(XML_MATH_CALCULEXP_RANGE_SWITCH);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:MathExp_RangeSwitch!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Math::slotMathExp_RangeExpChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(MATH_DELEGATE_CALCULEXP_RANGE_CALCUEXP)->getData();

    QFile file(XML_MATH_CALCULEXP_RANGE_CALCUEXP);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:MathExp_RangeExp!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
    resetPartialSettings();
}

void ConfigTreeObject_Math::slotMathExp_RangeDecChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(MATH_DELEGATE_CALCULEXP_RANGE_DECIMAL)->getData();

    QFile file(XML_MATH_CALCULEXP_RANGE_DECIMAL);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:MathExp_RangeDec!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Math::slotMathExp_RangeLowerChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(MATH_DELEGATE_CALCULEXP_RANGE_LOWER)->getData();

    QFile file(XML_MATH_CALCULEXP_RANGE_LOWER);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:MathExp_RangeLower!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
    resetPartialSettings();
}

void ConfigTreeObject_Math::slotMathExp_RangeUpperChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(MATH_DELEGATE_CALCULEXP_RANGE_UPPER)->getData();

    QFile file(XML_MATH_CALCULEXP_RANGE_UPPER);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:MathExp_RangeUpper!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
    resetPartialSettings();
}

void ConfigTreeObject_Math::slotMathExp_RangeUnitChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(MATH_DELEGATE_CALCULEXP_RANGE_UNIT)->getData();

    QFile file(XML_MATH_CALCULEXP_RANGE_UNIT);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:MathExp_RangeUnit!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Math::slotMathExp_TLOGTypeChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(MATH_DELEGATE_CALCULEXP_TLOG_TYPE)->getData();

    QFile file(XML_MATH_CALCULEXP_TLOG_TYPE);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:MathExp_TLOGType!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Math::slotMathExp_TLOGNoChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(MATH_DELEGATE_CALCULEXP_TLOG_NO)->getData();

    QFile file(XML_MATH_CALCULEXP_TLOG_NO);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:MathExp_TLOGNo!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Math::slotMathExp_TLOGSumChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(MATH_DELEGATE_CALCULEXP_TLOG_SCALE)->getData();

    QFile file(XML_MATH_CALCULEXP_TLOG_SCALE);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:MathExp_TLOGSum!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Math::slotMathExp_TLOGResetChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(MATH_DELEGATE_CALCULEXP_TLOG_RESET)->getData();

    QFile file(XML_MATH_CALCULEXP_TLOG_RESET);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:MathExp_TLOGReset!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Math::slotMathExp_RollSwitchChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(MATH_DELEGATE_CALCULEXP_MOVAVE_SWITCH)->getData();

    QFile file(XML_MATH_CALCULEXP_MOVAVE_SWITCH);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:MathExp_RollSwitch!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Math::slotMathExp_RollIntervalChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(MATH_DELEGATE_CALCULEXP_MOVAVE_INTER)->getData();

    QFile file(XML_MATH_CALCULEXP_MOVAVE_INTER);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:MathExp_RollInterval!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Math::slotMathExp_RollSampleChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(MATH_DELEGATE_CALCULEXP_MOVAVE_NUM)->getData();

    QFile file(XML_MATH_CALCULEXP_MOVAVE_NUM);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:MathExp_RollSamples!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Math::slotMathAlarm_Level1OnoffChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(MATH_DELEGATE_ALARM_LVL1_ONOFF)->getData();

    QFile file(XML_MATH_ALARM_LVL1_ONOFF);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:MathAlarm_Lvl1Onoff!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Math::slotMathAlarm_Level1TypeChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(MATH_DELEGATE_ALARM_LVL1_TYPE)->getData();

    QFile file(XML_MATH_ALARM_LVL1_TYPE);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:MathAlarm_Lvl1Type!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Math::slotMathAlarm_Level1ValueChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(MATH_DELEGATE_ALARM_LVL1_VALUE)->getData();

    QFile file(XML_MATH_ALARM_LVL1_VALUE);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:MathAlarm_Lvl1Value!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Math::slotMathAlarm_Level1HysterChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(MATH_DELEGATE_ALARM_LVL1_HYST)->getData();

    QFile file(XML_MATH_ALARM_LVL1_HYST);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:MathAlarm_Lvl1Hyst!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Math::slotMathAlarm_Level1LoggChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(MATH_DELEGATE_ALARM_LVL1_LOGG)->getData();

    QFile file(XML_MATH_ALARM_LVL1_LOGG);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:MathAlarm_Lvl1Logg!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Math::slotMathAlarm_Level1OutTypeChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(MATH_DELEGATE_ALARM_LVL1_OUTTYPE)->getData();

    QFile file(XML_MATH_ALARM_LVL1_OUTTYPE);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:MathAlarm_Lvl1Output!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }

    quint32 valueNo = value.toUInt();
    if(0 == valueNo) {

    } else if (1 == valueNo) {
        QMap<QString, quint32> curChanDO = CfgChannel::instance()->getExistChannelDO();
        if(!curChanDO.isEmpty()) {
            ConfigTreeObject::getConfigObject(MATH_DELEGATE_ALARM_LVL1_OUTNO)->changeData(curChanDO.first(), false);
        }
    } else if (2 == valueNo) {
        ConfigTreeObject::getConfigObject(MATH_DELEGATE_ALARM_LVL1_OUTNO)->changeData(1, false);
    }
}

void ConfigTreeObject_Math::slotMathAlarm_Level1OutNoChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(MATH_DELEGATE_ALARM_LVL1_OUTNO)->getData();

    QFile file(XML_MATH_ALARM_LVL1_OUTNO);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:MathAlarm_Lvl1OutNo!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Math::slotMathAlarm_Level2OnoffChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(MATH_DELEGATE_ALARM_LVL2_ONOFF)->getData();

    QFile file(XML_MATH_ALARM_LVL2_ONOFF);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:MathAlarm_Lvl2Onoff!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Math::slotMathAlarm_Level2TypeChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(MATH_DELEGATE_ALARM_LVL2_TYPE)->getData();

    QFile file(XML_MATH_ALARM_LVL2_TYPE);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:MathAlarm_Lvl2Type!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Math::slotMathAlarm_Level2ValueChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(MATH_DELEGATE_ALARM_LVL2_VALUE)->getData();

    QFile file(XML_MATH_ALARM_LVL2_VALUE);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:MathAlarm_Lvl2Value!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Math::slotMathAlarm_Level2HysterChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(MATH_DELEGATE_ALARM_LVL2_HYST)->getData();

    QFile file(XML_MATH_ALARM_LVL2_HYST);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:MathAlarm_Lvl2Hyst!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Math::slotMathAlarm_Level2LoggChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(MATH_DELEGATE_ALARM_LVL2_LOGG)->getData();

    QFile file(XML_MATH_ALARM_LVL2_LOGG);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:MathAlarm_Lvl2Logg!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Math::slotMathAlarm_Level2OutTypeChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(MATH_DELEGATE_ALARM_LVL2_OUTTYPE)->getData();

    QFile file(XML_MATH_ALARM_LVL2_OUTTYPE);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:MathAlarm_Lvl2Output!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }

    quint32 valueNo = value.toUInt();
    if(0 == valueNo) {

    } else if (1 == valueNo) {
        QMap<QString, quint32> curChanDO = CfgChannel::instance()->getExistChannelDO();
        if(!curChanDO.isEmpty()) {
            ConfigTreeObject::getConfigObject(MATH_DELEGATE_ALARM_LVL2_OUTNO)->changeData(curChanDO.first(), false);
        }
    } else if (2 == valueNo) {
        ConfigTreeObject::getConfigObject(MATH_DELEGATE_ALARM_LVL2_OUTNO)->changeData(1, false);
    }
}

void ConfigTreeObject_Math::slotMathAlarm_Level2OutNoChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(MATH_DELEGATE_ALARM_LVL2_OUTNO)->getData();

    QFile file(XML_MATH_ALARM_LVL2_OUTNO);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:MathAlarm_Lvl2OutNo!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Math::slotMathAlarm_Level3OnoffChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(MATH_DELEGATE_ALARM_LVL3_ONOFF)->getData();

    QFile file(XML_MATH_ALARM_LVL3_ONOFF);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:MathAlarm_Lvl3Onoff!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Math::slotMathAlarm_Level3TypeChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(MATH_DELEGATE_ALARM_LVL3_TYPE)->getData();

    QFile file(XML_MATH_ALARM_LVL3_TYPE);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:MathAlarm_Lvl3Type!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Math::slotMathAlarm_Level3ValueChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(MATH_DELEGATE_ALARM_LVL3_VALUE)->getData();

    QFile file(XML_MATH_ALARM_LVL3_VALUE);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:MathAlarm_Lvl3Value!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Math::slotMathAlarm_Level3HysterChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(MATH_DELEGATE_ALARM_LVL3_HYST)->getData();

    QFile file(XML_MATH_ALARM_LVL3_HYST);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:MathAlarm_Lvl3Hyst!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Math::slotMathAlarm_Level3LoggChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(MATH_DELEGATE_ALARM_LVL3_LOGG)->getData();

    QFile file(XML_MATH_ALARM_LVL3_LOGG);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:MathAlarm_Lvl3Logg!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Math::slotMathAlarm_Level3OutTypeChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(MATH_DELEGATE_ALARM_LVL3_OUTTYPE)->getData();

    QFile file(XML_MATH_ALARM_LVL3_OUTTYPE);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:MathAlarm_Lvl3Output!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }

    quint32 valueNo = value.toUInt();
    if(0 == valueNo) {

    } else if (1 == valueNo) {
        QMap<QString, quint32> curChanDO = CfgChannel::instance()->getExistChannelDO();
        if(!curChanDO.isEmpty()) {
            ConfigTreeObject::getConfigObject(MATH_DELEGATE_ALARM_LVL3_OUTNO)->changeData(curChanDO.first(), false);
        }
    } else if (2 == valueNo) {
        ConfigTreeObject::getConfigObject(MATH_DELEGATE_ALARM_LVL3_OUTNO)->changeData(1, false);
    }
}

void ConfigTreeObject_Math::slotMathAlarm_Level3OutNoChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(MATH_DELEGATE_ALARM_LVL3_OUTNO)->getData();

    QFile file(XML_MATH_ALARM_LVL3_OUTNO);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:MathAlarm_Lvl3OutNo!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Math::slotMathAlarm_Level4OnoffChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(MATH_DELEGATE_ALARM_LVL4_ONOFF)->getData();

    QFile file(XML_MATH_ALARM_LVL4_ONOFF);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:MathAlarm_Lvl4Onoff!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Math::slotMathAlarm_Level4TypeChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(MATH_DELEGATE_ALARM_LVL4_TYPE)->getData();

    QFile file(XML_MATH_ALARM_LVL4_TYPE);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:MathAlarm_Lvl4Type!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Math::slotMathAlarm_Level4ValueChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(MATH_DELEGATE_ALARM_LVL4_VALUE)->getData();

    QFile file(XML_MATH_ALARM_LVL4_VALUE);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:MathAlarm_Lvl4Value!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Math::slotMathAlarm_Level4HysterChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(MATH_DELEGATE_ALARM_LVL4_HYST)->getData();

    QFile file(XML_MATH_ALARM_LVL4_HYST);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:MathAlarm_Lvl4Hyst!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Math::slotMathAlarm_Level4LoggChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(MATH_DELEGATE_ALARM_LVL4_LOGG)->getData();

    QFile file(XML_MATH_ALARM_LVL4_LOGG);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:MathAlarm_Lvl4Logg!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Math::slotMathAlarm_Level4OutTypeChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(MATH_DELEGATE_ALARM_LVL4_OUTTYPE)->getData();

    QFile file(XML_MATH_ALARM_LVL4_OUTTYPE);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:MathAlarm_Lvl4Output!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }

    quint32 valueNo = value.toUInt();
    if(0 == valueNo) {

    } else if (1 == valueNo) {
        QMap<QString, quint32> curChanDO = CfgChannel::instance()->getExistChannelDO();
        if(!curChanDO.isEmpty()) {
            ConfigTreeObject::getConfigObject(MATH_DELEGATE_ALARM_LVL4_OUTNO)->changeData(curChanDO.first(), false);
        }
    } else if (2 == valueNo) {
        ConfigTreeObject::getConfigObject(MATH_DELEGATE_ALARM_LVL4_OUTNO)->changeData(1, false);
    }
}

void ConfigTreeObject_Math::slotMathAlarm_Level4OutNoChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(MATH_DELEGATE_ALARM_LVL4_OUTNO)->getData();

    QFile file(XML_MATH_ALARM_LVL4_OUTNO);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:MathAlarm_Lvl4OutNo!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Math::slotMathAlarm_DelayHourChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(MATH_DELEGATE_ALARM_DELAY_HOUR)->getData();

    QFile file(XML_MATH_ALARM_DELAY_HOUR);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:MathAlarm_DelayHour!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Math::slotMathAlarm_DelayMinChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(MATH_DELEGATE_ALARM_DELAY_MIN)->getData();

    QFile file(XML_MATH_ALARM_DELAY_MIN);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:MathAlarm_DelayMin!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Math::slotMathAlarm_DelaySecChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(MATH_DELEGATE_ALARM_DELAY_SEC)->getData();

    QFile file(XML_MATH_ALARM_DELAY_SEC);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:MathAlarm_DelaySec!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Math::slotMathDisp_ColorColorChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(MATH_DELEGATE_DISP_COLOR_COLOR)->getData();

    QFile file(XML_MATH_DISP_COLOR_COLOR);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:MathDisp_ColorColor!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Math::slotMathDisp_TagCharChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(MATH_DELEGATE_DISP_TAG_CHAR)->getData();

    QFile file(XML_MATH_DISP_TAG_CHAR);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:MathDisp_TagChar!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Math::slotMathDisp_TagNoChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(MATH_DELEGATE_DISP_TAG_NO)->getData();

    QFile file(XML_MATH_DISP_TAG_NO);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:MathDisp_TagNo!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Math::slotMathDisp_ZoneLowerChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(MATH_DELEGATE_DISP_ZONE_LOWER)->getData();

    QFile file(XML_MATH_DISP_ZONE_LOWER);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:MathDisp_ZoneLower!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Math::slotMathDisp_ZoneUpperChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(MATH_DELEGATE_DISP_ZONE_UPPER)->getData();

    QFile file(XML_MATH_DISP_ZONE_UPPER);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:MathDisp_ZoneUpper!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Math::slotMathDisp_ScalePosChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(MATH_DELEGATE_DISP_SCALE_POS)->getData();

    QFile file(XML_MATH_DISP_SCALE_POS);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:MathDisp_ScalePos!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Math::slotMathDisp_ScaleDivChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(MATH_DELEGATE_DISP_SCALE_DIV)->getData();

    QFile file(XML_MATH_DISP_SCALE_DIV);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:MathDisp_ScaleDiv!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Math::slotMathDisp_BarBasePosChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(MATH_DELEGATE_DISP_BAR_BASEPOS)->getData();

    QFile file(XML_MATH_DISP_BAR_BASEPOS);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:MathDisp_BarBase!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Math::slotMathDisp_BarDivChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(MATH_DELEGATE_DISP_BAR_DIVISION)->getData();

    QFile file(XML_MATH_DISP_BAR_DIVISION);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:MathDisp_BarDiv!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Math::slotMathDisp_PartOnoffChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(MATH_DELEGATE_DISP_PART_ONOFF)->getData();

    QFile file(XML_MATH_DISP_PART_ONOFF);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:MathDisp_PartOnoff!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Math::slotMathDisp_PartExpandChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(MATH_DELEGATE_DISP_PART_EXPAND)->getData();

    QFile file(XML_MATH_DISP_PART_EXPAND);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:MathDisp_PartExpand!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Math::slotMathDisp_PartBoundChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(MATH_DELEGATE_DISP_PART_BOUND)->getData();

    QFile file(XML_MATH_DISP_PART_BOUND);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:MathDisp_PartBoundry!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Math::slotMathDisp_CsbandBandareaChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(MATH_DELEGATE_DISP_CSBAND_AREA)->getData();

    QFile file(XML_MATH_DISP_CSBAND_AREA);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:MathDisp_CsbandArea!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Math::slotMathDisp_CsbandDispLowChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(MATH_DELEGATE_DISP_CSBAND_LOWER)->getData();

    QFile file(XML_MATH_DISP_CSBAND_LOWER);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:MathDisp_CsbandLower!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Math::slotMathDisp_CsbandDispUpChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(MATH_DELEGATE_DISP_CSBAND_UPPER)->getData();

    QFile file(XML_MATH_DISP_CSBAND_UPPER);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:MathDisp_CsbandUpper!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Math::slotMathDisp_CsbandColorChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(MATH_DELEGATE_DISP_CSBAND_COLOR)->getData();

    QFile file(XML_MATH_DISP_CSBAND_COLOR);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:MathDisp_CsbandColor!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Math::slotMathDisp_AlarmIndicateChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(MATH_DELEGATE_DISP_ALARM_INDIC)->getData();

    QFile file(XML_MATH_DISP_ALARM_INDIC);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:MathDisp_AlarmIndic!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Math::slotMathDisp_AlarmMarkTypeChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(MATH_DELEGATE_DISP_ALARM_MARKTYPE)->getData();

    QFile file(XML_MATH_DISP_ALARM_MARKTYPE);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:MathDisp_AlarmType!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Math::slotMathDisp_AlarmAlarm1Changed()
{
    QVariant value = ConfigTreeObject::getConfigObject(MATH_DELEGATE_DISP_ALARM_COLOR1)->getData();

    QFile file(XML_MATH_DISP_ALARM_COLOR1);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:MathDisp_AlarmColor1!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Math::slotMathDisp_AlarmAlarm2Changed()
{
    QVariant value = ConfigTreeObject::getConfigObject(MATH_DELEGATE_DISP_ALARM_COLOR2)->getData();

    QFile file(XML_MATH_DISP_ALARM_COLOR2);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:MathDisp_AlarmColor2!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Math::slotMathDisp_AlarmAlarm3Changed()
{
    QVariant value = ConfigTreeObject::getConfigObject(MATH_DELEGATE_DISP_ALARM_COLOR3)->getData();

    QFile file(XML_MATH_DISP_ALARM_COLOR3);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:MathDisp_AlarmColor3!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Math::slotMathDisp_AlarmAlarm4Changed()
{
    QVariant value = ConfigTreeObject::getConfigObject(MATH_DELEGATE_DISP_ALARM_COLOR4)->getData();

    QFile file(XML_MATH_DISP_ALARM_COLOR4);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:MathDisp_AlarmColor4!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Math::slotMathAction_ValueErrorChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(MATH_DELEGATE_MATHACTION_VALUEERROR)->getData();

    QFile file(XML_MATH_MATHACTION_VALUEERROR);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:MathAction_ValueErr!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Math::slotMathAction_KeyActionChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(MATH_DELEGATE_MATHACTION_KEYACTION)->getData();

    QFile file(XML_MATH_MATHACTION_KEYACTION);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:MathAction_KeyAction!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Math::slotMathAction_ValueSumChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(MATH_DELEGATE_MATHACTION_VALUE_SUM)->getData();

    QFile file(XML_MATH_MATHACTION_VALUE_SUM);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:MathAction_OverSum!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Math::slotMathAction_ValuePPChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(MATH_DELEGATE_MATHACTION_VALUE_PP)->getData();

    QFile file(XML_MATH_MATHACTION_VALUE_PP);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:MathAction_OverPP!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}
