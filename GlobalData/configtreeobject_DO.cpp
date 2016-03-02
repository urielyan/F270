#include "configtreeobject_DO.h"
#include "cfgchannel.h"
#include "./GlobalData/globalDef.h"

#include "cfgGlobalDef.h"
#include "configtreeobject.h"

ConfigTreeObject_DO *ConfigTreeObject_DO::instance(QObject *parent)
{
    static ConfigTreeObject_DO data(parent);
    return &data;
}

ConfigTreeObject_DO::ConfigTreeObject_DO(QObject *parent):
      ConfigObject(parent)
{
    //DO channel settings代理树
    QFile fileDelegate(":/configtree/Image/configTree/DO/DO_SettingDelegate/delegate.xml");
    if(!fileDelegate.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file: DO_settingDelegate!";
    }else{
        XmlConfigParser::parseConfigXml(fileDelegate, static_cast<ConfigObject*>(parent), \
                                                                   (QObject*)this);
        fileDelegate.close();
    }
}

/*
 * 功能：
 * 	  DO配置窗体当前界面刷新
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
void ConfigTreeObject_DO::updateStrShowOn()
{
    if (CfgChannel::instance()->getExistChannelDO().isEmpty())
        return;
    else
        ConfigTreeObject::getConfigObject(DO_DELEGATE)->setHideFlag(false);

    updateStrShowRange();
    updateStrShowDisplay();
}

void ConfigTreeObject_DO::updateStrShowRange()
{
    slotDORange_RangeTypeChanged();
    slotDORange_RangeUpperChanged();
    slotDORange_RangeLowerChanged();
    slotDORange_RangeUnitChanged();

    slotDORange_ActionTypeChanged();
    slotDORange_ActionActionChanged();
    slotDORange_ActionRelayAckChanged();
    slotDORange_ActionHoldChanged();
    slotDORange_ActionRelayInChanged();
}

void ConfigTreeObject_DO::updateStrShowDisplay()
{
    slotDODisp_ColorColorChanged();

    slotDODisp_TagCharChanged();
    slotDODisp_TagNoChanged();

    slotDODisp_ZoneUpperChanged();
    slotDODisp_ZoneLowerChanged();

    slotDODisp_ScalePosChanged();

    slotDODisp_BarPosChanged();

    slotDODisp_DischarVal0Changed();
    slotDODisp_DischarVal1Changed();
}

void ConfigTreeObject_DO::slotCreateDOsettingTree(QObject *parent)
{
    if (CfgChannel::instance()->getExistChannelDO().isEmpty())
        return;

    //DO channel settings配置树
    QFile file(":/configtree/Image/configTree/DO/DO_Setting/config.xml");
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file: DO_setting!";
    }else{
        XmlConfigParser::parseConfigXml(file, static_cast<ConfigObject*>(parent));
        file.close();
    }

    //开始通道、结束通道初始化--特殊情况，一般的object都在解析时初始化
    QString keyFirst = CfgChannel::instance()->getExistChannelDO().firstKey();
    quint32 valueFirst = CfgChannel::instance()->getExistChannelDO().value(keyFirst);

    quint32 ID_first = ConfigTreeObject::getConfigObject(DOCONFIG_FIRSTCHAN)->getObjectID();
    quint32 ID_last = ConfigTreeObject::getConfigObject(DOCONFIG_LASTCHAN)->getObjectID();

    ConfigTreeObject::getConfigObject(DOCONFIG_FIRSTCHAN)->initData(keyFirst, valueFirst, ID_first);
    ConfigTreeObject::getConfigObject(DOCONFIG_LASTCHAN)->initData(keyFirst, valueFirst, ID_last);

    ConfigTreeObject::getConfigObject(DOCONFIG_FIRSTCHAN)->setStrShowOn(keyFirst);
    ConfigTreeObject::getConfigObject(DOCONFIG_LASTCHAN)->setStrShowOn(keyFirst);

}

void ConfigTreeObject_DO::slotDOBoardInoutCfgTree(quint8 unit, quint8 module, quint8 chNum, bool in)
{
    quint32 chanCoder;
    ConfigObject *pConfigParentObj;
    ConfigObject *pConfigObj;
    QFile file;

    if(in) {   //板卡插入
        for(int i=1; i<=chNum; ++i) {
            chanCoder = CHANNEL_DO_CODER(unit, module, i);
            int index = CfgChannel::instance()->getExistChannelDO().values().indexOf(chanCoder);
            QString chanNum = CfgChannel::instance()->getExistChannelDO().keys().at(index);

            pConfigParentObj = ConfigTreeObject::getConfigObject(DOCONFIG_RANGE);
            pConfigObj = new ConfigObject(pConfigParentObj);
            pConfigObj->setObjectName(chanNum);
            file.setFileName(":/configtree/Image/configTree/DO/DO_Setting/config-Range.xml");
            if(!file.open(QFile::ReadOnly | QFile::Text)){
                qWarning()<< "Can't open the file: config-Range.xml";
            }else{
                XmlConfigParser::parseConfigXml(file, pConfigObj, 0, chanCoder);
                file.close();
            }

            pConfigParentObj = ConfigTreeObject::getConfigObject(DOCONFIG_DISP);
            pConfigObj = new ConfigObject(pConfigParentObj);
            pConfigObj->setObjectName(chanNum);
            file.setFileName(":/configtree/Image/configTree/DO/DO_Setting/config-Display settings.xml");
            if(!file.open(QFile::ReadOnly | QFile::Text)){
                qWarning()<< "Can't open the file: config-Display settings.xml";
            }else{
                XmlConfigParser::parseConfigXml(file, pConfigObj, 0, chanCoder);
                file.close();
            }
        }
    } else {  //板卡拔出
        for(int i=1; i<=chNum; ++i) {
            chanCoder = CHANNEL_DO_CODER(unit, module, i);
            int index = CfgChannel::instance()->getExistChannelDO().values().indexOf(chanCoder);
            QString chanNum = CfgChannel::instance()->getExistChannelDO().keys().at(index);

            QString delPath;
            delPath = QString(DOCONFIG_RANGE) + ":" + QString(chanNum);
            ConfigTreeObject::destroyConfigTree(delPath);
            delPath = QString(DOCONFIG_DISP) + ":" + QString(chanNum);
            ConfigTreeObject::destroyConfigTree(delPath);
        }
    }
}

/*
 * 功能：
 * 	  DO配置窗体当前通道选择更改处理：修改所有代理配置对象，使它们
 *    根据当前选择的通道号指向相应的配置对象，同时刷新当前界面
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
void ConfigTreeObject_DO::slotDODelegateChange()
{
    if (CfgChannel::instance()->getExistChannelDO().isEmpty())
        return;

    QString fistChan = ConfigTreeObject::getConfigObject(DOCONFIG_FIRSTCHAN)->getData().toString();
    QString lastChan = ConfigTreeObject::getConfigObject(DOCONFIG_LASTCHAN)->getData().toString();
    quint32 firstCoder = CfgChannel::instance()->getExistChannelDO().value(fistChan);
    quint32 lastCoder = CfgChannel::instance()->getExistChannelDO().value(lastChan);
    QFile delegateChange(":/configtree/Image/configTree/DO/DO_DelegateChange/delegateChange.xml");
    if(!delegateChange.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file: DO_settingDelegateChange!";
    }else{
        XmlConfigParser::parseDelegateChangeXml(delegateChange, firstCoder, lastCoder);
        delegateChange.close();
    }

    _curStatus = false;
    updateStrShowOn();
    _curStatus = true;
}

/*
 * 功能：
 * 	  DO起始通道变更槽函数，其他配置项目类似。
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
void ConfigTreeObject_DO::slotDOFirst_chanSelectchanged()
{
    QString chanNumF;
    QString chanNumL;
    ConfigObject *pObj = ConfigTreeObject::getConfigObject(DO_DELEGATE_FIRSTCHAN);
    ConfigObject *qObj = ConfigTreeObject::getConfigObject(DO_DELEGATE_LASTCHAN);

    if(pObj && qObj){
        chanNumF = pObj->getData().toString();
        chanNumL = qObj->getData().toString();

        pObj->setStrShowOn(chanNumF);
        qObj->changeData(chanNumF, false);
    }
}

void ConfigTreeObject_DO::slotDOLast_chanSelectchanged()
{
    QString chanNumF;
    QString chanNumL;
    ConfigObject *pObj = ConfigTreeObject::getConfigObject(DO_DELEGATE_FIRSTCHAN);
    ConfigObject *qObj = ConfigTreeObject::getConfigObject(DO_DELEGATE_LASTCHAN);

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

void ConfigTreeObject_DO::slotDORange_RangeTypeChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(DO_DELEGATE_RANGE_RANGE_TYPE)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(DO_DELEGATE_RANGE_RANGE_TYPE)->getChannelCoder();
    QFile file(XML_DO_RANGE_RANGE_TYPE);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:DORange_RangeType!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
    }
}

void ConfigTreeObject_DO::slotDORange_RangeUpperChanged()
{
    ConfigObject *rObj = ConfigTreeObject::getConfigObject(DO_DELEGATE_RANGE_RANGE_UPPER);
    ConfigObject *sObj = ConfigTreeObject::getConfigObject(DO_DELEGATE_RANGE_RANGE_LOWER);

    float value = rObj->getData().toFloat();    //Up
    float value_t = sObj->getData().toFloat();  //Low
    if(value == value_t) {
        if(value_t == (ConfigTreeObject::getConfigData(DO_DELEGATE_RANGE_RANGE_MIN).toFloat())) {
            value_t = ConfigTreeObject::getConfigData(DO_DELEGATE_RANGE_RANGE_MAX).toFloat();
        }else{
            value_t = ConfigTreeObject::getConfigData(DO_DELEGATE_RANGE_RANGE_MIN).toFloat();
        }
        sObj->changeData(value_t, false);
    }

    QVariant valueNow = rObj->getData();
    quint32 coder = rObj->getChannelCoder();
    QFile file(XML_DO_RANGE_RANGE_UPPER);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:DORange_RangeUpper!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, valueNow, coder);
        file.close();
    }
}

void ConfigTreeObject_DO::slotDORange_RangeLowerChanged()
{
    ConfigObject *rObj = ConfigTreeObject::getConfigObject(DO_DELEGATE_RANGE_RANGE_LOWER);
    ConfigObject *sObj = ConfigTreeObject::getConfigObject(DO_DELEGATE_RANGE_RANGE_UPPER);

    float value = rObj->getData().toFloat();    //Low
    float value_t = sObj->getData().toFloat();  //Up
    if(value == value_t) {
        if(value_t == (ConfigTreeObject::getConfigData(DO_DELEGATE_RANGE_RANGE_MAX).toFloat())) {
            value_t = ConfigTreeObject::getConfigData(DO_DELEGATE_RANGE_RANGE_MIN).toFloat();
        }else{
            value_t = ConfigTreeObject::getConfigData(DO_DELEGATE_RANGE_RANGE_MAX).toFloat();
        }
        sObj->changeData(value_t, false);
    }

    QVariant valueNow = rObj->getData();
    quint32 coder = rObj->getChannelCoder();
    QFile file(XML_DO_RANGE_RANGE_LOWER);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:DORange_RangeLower!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, valueNow, coder);
        file.close();
    }
}

void ConfigTreeObject_DO::slotDORange_RangeUnitChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(DO_DELEGATE_RANGE_RANGE_UNIT)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(DO_DELEGATE_RANGE_RANGE_UNIT)->getChannelCoder();
    QFile file(XML_DO_RANGE_RANGE_UNIT);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:DORange_RangeUnit!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
    }
}

void ConfigTreeObject_DO::slotDORange_ActionTypeChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(DO_DELEGATE_RANGE_ACTION_ENERGIZE)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(DO_DELEGATE_RANGE_ACTION_ENERGIZE)->getChannelCoder();
    QFile file(XML_DO_RANGE_ACTION_ENERGIZE);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:DORange_ActionEnergize!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
    }
}

void ConfigTreeObject_DO::slotDORange_ActionActionChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(DO_DELEGATE_RANGE_ACTION_ACTION)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(DO_DELEGATE_RANGE_ACTION_ACTION)->getChannelCoder();
    QFile file(XML_DO_RANGE_ACTION_ACTION);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:DORange_ActionAction!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
    }
}

void ConfigTreeObject_DO::slotDORange_ActionRelayAckChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(DO_DELEGATE_RANGE_ACTION_RELAYACK)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(DO_DELEGATE_RANGE_ACTION_RELAYACK)->getChannelCoder();
    QFile file(XML_DO_RANGE_ACTION_RELAYACK);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:DORange_ActionRelayAck!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
    }
}

void ConfigTreeObject_DO::slotDORange_ActionHoldChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(DO_DELEGATE_RANGE_ACTION_HOLD)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(DO_DELEGATE_RANGE_ACTION_HOLD)->getChannelCoder();
    QFile file(XML_DO_RANGE_ACTION_HOLD);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:DORange_ActionHold!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
    }
}

void ConfigTreeObject_DO::slotDORange_ActionRelayInChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(DO_DELEGATE_RANGE_ACTION_INTERVAL)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(DO_DELEGATE_RANGE_ACTION_INTERVAL)->getChannelCoder();
    QFile file(XML_DO_RANGE_ACTION_INTERVAL);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:DORange_ActionInterval!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
    }
}

void ConfigTreeObject_DO::slotDODisp_ColorColorChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(DO_DELEGATE_DISP_COLOR_COLOR)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(DO_DELEGATE_DISP_COLOR_COLOR)->getChannelCoder();
    QFile file(XML_DO_DISP_COLOR_COLOR);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:DODisp_ColorColor!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
    }
}

void ConfigTreeObject_DO::slotDODisp_TagCharChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(DO_DELEGATE_DISP_TAG_CHAR)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(DO_DELEGATE_DISP_TAG_CHAR)->getChannelCoder();
    QFile file(XML_DO_DISP_TAG_CHAR);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:DODisp_TagChar!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
    }
}

void ConfigTreeObject_DO::slotDODisp_TagNoChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(DO_DELEGATE_DISP_TAG_NO)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(DO_DELEGATE_DISP_TAG_NO)->getChannelCoder();
    QFile file(XML_DO_DISP_TAG_NO);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:DODisp_TagNo!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
    }
}

void ConfigTreeObject_DO::slotDODisp_ZoneUpperChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(DO_DELEGATE_DISP_ZONE_UPPER)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(DO_DELEGATE_DISP_ZONE_UPPER)->getChannelCoder();
    QFile file(XML_DO_DISP_ZONE_UPPER);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:DODisp_ZoneUpper!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
    }
}

void ConfigTreeObject_DO::slotDODisp_ZoneLowerChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(DO_DELEGATE_DISP_ZONE_LOWER)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(DO_DELEGATE_DISP_ZONE_LOWER)->getChannelCoder();
    QFile file(XML_DO_DISP_ZONE_LOWER);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:DODisp_ZoneLower!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
    }
}

void ConfigTreeObject_DO::slotDODisp_ScalePosChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(DO_DELEGATE_DISP_SCALE_POS)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(DO_DELEGATE_DISP_SCALE_POS)->getChannelCoder();
    QFile file(XML_DO_DISP_SCALE_POS);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:DODisp_ScalePos!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
    }
}

void ConfigTreeObject_DO::slotDODisp_BarPosChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(DO_DELEGATE_DISP_BAR_POS)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(DO_DELEGATE_DISP_BAR_POS)->getChannelCoder();
    QFile file(XML_DO_DISP_BAR_POS);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:DODisp_BarPos!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
    }
}

void ConfigTreeObject_DO::slotDODisp_DischarVal0Changed()
{
    QVariant value = ConfigTreeObject::getConfigObject(DO_DELEGATE_DISP_DISCHAR_0)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(DO_DELEGATE_DISP_DISCHAR_0)->getChannelCoder();
    QFile file(XML_DO_DISP_DISCHAR_0);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:DODisp_DispChar0!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
    }
}

void ConfigTreeObject_DO::slotDODisp_DischarVal1Changed()
{
    QVariant value = ConfigTreeObject::getConfigObject(DO_DELEGATE_DISP_DISCHAR_1)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(DO_DELEGATE_DISP_DISCHAR_1)->getChannelCoder();
    QFile file(XML_DO_DISP_DISCHAR_1);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:DODisp_DispChar1!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
    }
}

