#include "configtreeobject_Measure.h"
#include "cfgchannel.h"
#include "./GlobalData/globalDef.h"

#include "cfgGlobalDef.h"
#include "configtreeobject.h"
#include "./Common/devboard.h"

ConfigTreeObject_Measure *ConfigTreeObject_Measure::instance(QObject *parent)
{
    static ConfigTreeObject_Measure data(parent);
    return &data;
}

ConfigTreeObject_Measure::ConfigTreeObject_Measure(QObject *parent):
      ConfigObject(parent)
{
    //Measure settings代理树
    QFile fileDelegate(":/configtree/Image/configTree/Measure/Measure_SettingDelegate/delegate.xml");
    if(!fileDelegate.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file: Measure_settingDelegate!";
    }else{
        XmlConfigParser::parseConfigXml(fileDelegate, static_cast<ConfigObject*>(parent), \
                                                          (QObject*)this);
        fileDelegate.close();
    }
}

void ConfigTreeObject_Measure::updateStrShowOn()
{
    slotMeasureIntervalChanged();
    slotMeasureOverRangeChanged();
}

void ConfigTreeObject_Measure::updateModuleStrShowOn_AI()
{
    slotMeasureADmodechanged();
    slotMeasureBurnLowchanged();
    slotMeasureBurnUpchanged();
}

void ConfigTreeObject_Measure::updateModuleStrShowOn_DI()
{
    slotMeasureDIOperModechanged();
}

void ConfigTreeObject_Measure::initUnitModuleShowOn()
{
    if (CfgChannel::instance()->getList_BoardIO().isEmpty()) {
        //一张板卡都没有时--SelectUnit Group Hide;
        ConfigTreeObject::getConfigObject(MEASURE_DELEGATE_SELECT)->setHideFlag(true);
        return;
    } else {
        //存在板卡时初始化全部单元模块隐藏--All Unit setHide;
        ConfigTreeObject::getConfigObject(MEASURE_DELEGATE_SELECT)->setHideFlag(false);
        //Main Unit
        ConfigTreeObject::getConfigObject(MEASURE_DELEGATE_SELECT_MAIN)->setHideFlag(true);
        //EXT Unit 1~6
        for(quint8 i=0;i<6;++i) {
            ConfigTreeObject::getConfigObject(QString(MEASURE_DELEGATE_SELECT_EXTGRP).arg(i+1))\
                    ->setHideFlag(true);
        }
    }

    quint8 unitNum;             //板卡单元号
    quint8 moduleNum;           //板卡模块号
    quint8 typeNum;             //板卡类型：1-AI，2-DI
    //Exist Unit显示
    foreach (DevBoard *pBoard, CfgChannel::instance()->getList_BoardIO()) {
        unitNum = pBoard->getBoardUnit();
        switch(unitNum) {
        case 0: //Main Unit--10 Module
            ConfigTreeObject::getConfigObject(MEASURE_DELEGATE_SELECT_MAIN)->setHideFlag(false);
            for(quint8 i=0;i<10;++i) {
                ConfigObject* pExistMObj = ConfigTreeObject::getConfigObject(\
                            QString(MEASURE_DELEGATE_SELECT_MAIN_MODULE).arg(i));
                if(pExistMObj)
                    pExistMObj->setHideFlag(true);
            }
            break;
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6: //EXT Unit x--0~5 Module
            ConfigTreeObject::getConfigObject(QString(MEASURE_DELEGATE_SELECT_EXTGRP).arg(unitNum))\
                    ->setHideFlag(false);
            for(quint8 i=0;i<6;++i) {
                ConfigObject* pExistMObj = ConfigTreeObject::getConfigObject\
                        (QString(MEASURE_DELEGATE_SELECT_EXT_MODULE).arg(unitNum).arg(unitNum).arg(i));
                if(pExistMObj)
                    pExistMObj->setHideFlag(true);
            }
            break;
        default:
            ConfigTreeObject::getConfigObject(MEASURE_DELEGATE_SELECT)->setHideFlag(true);
            break;
        }
    }

    //Exist Module显示
    foreach (DevBoard *pBoard, CfgChannel::instance()->getList_BoardIO()) {
        unitNum = pBoard->getBoardUnit();
        moduleNum = pBoard->getBoardModule();
        typeNum = pBoard->getBoardType();
        switch(unitNum) {
        case 0: //Main Unit--10 Module
            for(quint8 i=0;i<10;++i) {      //Module Exist
                if((moduleNum == i) && \
                    ((STATUS_BOARD_TYPE_AI == typeNum) ||\
                     (STATUS_BOARD_TYPE_DI == typeNum) ||\
                     (STATUS_BOARD_TYPE_DIO == typeNum))) {
                    ConfigObject* pExistMObj = ConfigTreeObject::getConfigObject(\
                                QString(MEASURE_DELEGATE_SELECT_MAIN_MODULE).arg(i));
                    if(pExistMObj)
                        pExistMObj->setHideFlag(false);
                }
            }
            break;
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6: //EXT Unit x--0~5 Module
            for(quint8 i=0;i<6;++i) {   //Module Exist
                if((moduleNum == i) && \
                    ((STATUS_BOARD_TYPE_AI == typeNum) ||\
                     (STATUS_BOARD_TYPE_DI == typeNum) ||\
                     (STATUS_BOARD_TYPE_DIO == typeNum))) {
                    ConfigObject* pExistMObj = ConfigTreeObject::getConfigObject\
                            (QString(MEASURE_DELEGATE_SELECT_EXT_MODULE).arg(unitNum).arg(unitNum).arg(i));
                    if(pExistMObj)
                        pExistMObj->setHideFlag(false);
                }
            }
            break;
        default:
            break;
        }
    }
}

void ConfigTreeObject_Measure::slotCreateMeasuresettingTree(QObject *parent)
{
    //Measure settings配置树
    QFile file(":/configtree/Image/configTree/Measure/Measure_Setting/config.xml");
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file: Measure_setting!";
    }else{
        XmlConfigParser::parseConfigXml(file, static_cast<ConfigObject*>(parent));
        file.close();
    }
}

void ConfigTreeObject_Measure::slotCreateMeasuresettingTree_Unit()
{
    QString unitStr;
    QString moduleStr;
    QString path;
    QFile file;

    foreach (DevBoard *pBoard, CfgChannel::instance()->getList_BoardIO()) {
        quint8 unitNum = pBoard->getBoardUnit();                //板卡单元号
        quint8 moduleNum = pBoard->getBoardModule();            //板卡模块号
        quint8 typeNum = pBoard->getBoardType();                //板卡类型：1-AI，2-DI，4-DIO

        unitStr = (unitNum == 0 ? QString("Main Unit"):QString("EXT Unit %1").arg(unitNum));
        moduleStr = QString("Unit %1 Module %2").arg(unitNum).arg(moduleNum);
        path = QString(MEASURECONFIG_U_M).arg(unitStr).arg(moduleStr);
        ConfigObject* pUnitModuleObj = ConfigTreeObject::getConfigObject(QString(path));

        switch(typeNum) {
        case STATUS_BOARD_TYPE_AI: {
            file.setFileName(":/configtree/Image/configTree/Measure/Measure_Setting/config-normal-AI.xml");
            if(!file.open(QFile::ReadOnly | QFile::Text)){
                qWarning()<< "Can't open the file: config-normal-AI!";
            }else{
                XmlConfigParser::parseConfigXml(file, static_cast<ConfigObject*>(pUnitModuleObj));
                file.close();
            }

            ConfigTreeObject::getConfigObject(QString(MEASURECONFIG_U_M_ADTIME_VALUE).arg(unitStr).arg(moduleStr))\
                    ->initData(pBoard->getADMode(), 0, ID_Measure_AI_Unit_AD_Value);
            ConfigTreeObject::getConfigObject(QString(MEASURECONFIG_U_M_GENER_LOWER).arg(unitStr).arg(moduleStr))\
                    ->initData(pBoard->getBurnoutDownLimit(), 0, ID_Measure_AI_Unit_Gneral_Low);
            ConfigTreeObject::getConfigObject(QString(MEASURECONFIG_U_M_GENER_UPPER).arg(unitStr).arg(moduleStr))\
                    ->initData(pBoard->getBurnoutUpperLimit(), 0, ID_Measure_AI_Unit_Gneral_Upper);
        }
            break;
        case STATUS_BOARD_TYPE_DI:
        case STATUS_BOARD_TYPE_DIO: {
            file.setFileName(":/configtree/Image/configTree/Measure/Measure_Setting/config-normal-DI.xml");
            if(!file.open(QFile::ReadOnly | QFile::Text)){
                qWarning()<< "Can't open the file: config-normal-DI!";
            }else{
                XmlConfigParser::parseConfigXml(file, static_cast<ConfigObject*>(pUnitModuleObj));
                file.close();
            }

            quint32 RemoteModule = CfgConfig::instance()->getConfigData(ID_Measure_DI_Unit_Mode_Value, 0).toUInt();
            if((unitNum == REMOTE_UNIT(RemoteModule)) && (moduleNum == REMOTE_MODULE(RemoteModule))) {
                ConfigTreeObject::getConfigObject(QString(MEASURECONFIG_U_M_DIMODE_MODE).arg(unitStr).arg(moduleStr))
                        ->initData(DI_OpeMode_Remote, 0, ID_Measure_DI_Unit_Mode_Value);
            } else {
                ConfigTreeObject::getConfigObject(QString(MEASURECONFIG_U_M_DIMODE_MODE).arg(unitStr).arg(moduleStr))
                        ->initData(DI_OpeMode_STD, 0, ID_Measure_DI_Unit_Mode_Value);
            }
        }
            break;
        default:
            break;
        }
    }
}

void ConfigTreeObject_Measure::slotMeasureBoardInoutCfgTree_Unit(quint8 unit, quint8 module, quint8 type, bool in)
{
    QString unitStr;
    QString moduleStr;
    QString path;
    QFile file;

    if(in) {    //板卡插入
        foreach (DevBoard *pBoard, CfgChannel::instance()->getList_BoardIO()) {
            if((pBoard->getBoardUnit() == unit) && (pBoard->getBoardModule() == module)) {
                unitStr = (unit == 0 ? QString("Main Unit"):QString("EXT Unit %1").arg(unit));
                moduleStr = QString("Unit %1 Module %2").arg(unit).arg(module);
                path = QString(MEASURECONFIG_U_M).arg(unitStr).arg(moduleStr);
                ConfigObject* pUnitModuleObj = ConfigTreeObject::getConfigObject(QString(path));

                switch(type) {
                case STATUS_BOARD_TYPE_AI: {
                    file.setFileName(":/configtree/Image/configTree/Measure/Measure_Setting/config-normal-AI.xml");
                    if(!file.open(QFile::ReadOnly | QFile::Text)){
                        qWarning()<< "Can't open the file: config-normal-AI!";
                    }else{
                        XmlConfigParser::parseConfigXml(file, static_cast<ConfigObject*>(pUnitModuleObj));
                        file.close();
                    }

                    ConfigTreeObject::getConfigObject(QString(MEASURECONFIG_U_M_ADTIME_VALUE).arg(unitStr).arg(moduleStr))\
                            ->initData(pBoard->getADMode(), 0, ID_Measure_AI_Unit_AD_Value);
                    ConfigTreeObject::getConfigObject(QString(MEASURECONFIG_U_M_GENER_LOWER).arg(unitStr).arg(moduleStr))\
                            ->initData(pBoard->getBurnoutDownLimit(), 0, ID_Measure_AI_Unit_Gneral_Low);
                    ConfigTreeObject::getConfigObject(QString(MEASURECONFIG_U_M_GENER_UPPER).arg(unitStr).arg(moduleStr))\
                            ->initData(pBoard->getBurnoutUpperLimit(), 0, ID_Measure_AI_Unit_Gneral_Upper);
                }
                    break;
                case STATUS_BOARD_TYPE_DI:
                case STATUS_BOARD_TYPE_DIO: {
                    file.setFileName(":/configtree/Image/configTree/Measure/Measure_Setting/config-normal-DI.xml");
                    if(!file.open(QFile::ReadOnly | QFile::Text)){
                        qWarning()<< "Can't open the file: config-normal-DI!";
                    }else{
                        XmlConfigParser::parseConfigXml(file, static_cast<ConfigObject*>(pUnitModuleObj));
                        file.close();
                    }

                    quint32 RemoteModule = CfgConfig::instance()->getConfigData(ID_Measure_DI_Unit_Mode_Value, 0).toUInt();
                    if((unit == REMOTE_UNIT(RemoteModule)) && (module == REMOTE_MODULE(RemoteModule))) {
                        ConfigTreeObject::getConfigObject(QString(MEASURECONFIG_U_M_DIMODE_MODE).arg(unitStr).arg(moduleStr))
                                ->initData(DI_OpeMode_Remote, 0, ID_Measure_DI_Unit_Mode_Value);
                    } else {
                        ConfigTreeObject::getConfigObject(QString(MEASURECONFIG_U_M_DIMODE_MODE).arg(unitStr).arg(moduleStr))
                                ->initData(DI_OpeMode_STD, 0, ID_Measure_DI_Unit_Mode_Value);
                    }
                }
                    break;
                default:
                    break;
                }
            }
        }
    } else {    //板卡拔出
        unitStr = (unit == 0 ? QString("Main Unit"):QString("EXT Unit %1").arg(unit));
        moduleStr = QString("Unit %1 Module %2").arg(unit).arg(module);
        switch(type) {
        case STATUS_BOARD_TYPE_AI: {
            path = QString(MEASURECONFIG_U_M_ADTIME).arg(unitStr).arg(moduleStr);
            ConfigTreeObject::destroyConfigTree(path);

            path = QString(MEASURECONFIG_U_M_GENER).arg(unitStr).arg(moduleStr);
            ConfigTreeObject::destroyConfigTree(path);
        }
            break;
        case STATUS_BOARD_TYPE_DI:
        case STATUS_BOARD_TYPE_DIO: {
            path = QString(MEASURECONFIG_U_M_DIMODE).arg(unitStr).arg(moduleStr);
            ConfigTreeObject::destroyConfigTree(path);
        }
            break;
        default:
            break;
        }
    }
}

void ConfigTreeObject_Measure::slotMeasureDelegateChange()
{
    QFile delegateChange(":/configtree/Image/configTree/Measure/Measure_DelegateChange/delegateChange.xml");
    if(!delegateChange.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file: Measure_DelegateChange!";
    }else{
        XmlConfigParser::parseDelegateChangeXml(delegateChange);
        delegateChange.close();
    }

    _curStatus = false;
    updateStrShowOn();
    _curStatus = true;
}

void ConfigTreeObject_Measure::slotMeasureDelegateChange_Unit(Menu *menu)
{
    if(menu == NULL) {    //上电时Measure界面根据实际配置显示or隐藏
        initUnitModuleShowOn();
        return;
    }

    QString ModuleName = menu->getMenuName().toString();
    QStringList nameList = ModuleName.split(" ");
    quint32 unit = nameList[1].toUInt();
    quint32 module = nameList[3].toUInt();
    if (unit == 0) {
        Q_ASSERT(module<MAIN_UNIT_MODULE_NUM);
    } else {
        Q_ASSERT(module<EXT_UNIT_MODULE_NUM);
    }

    quint16 boardStatus = CfgConfig::instance()->getBoardStatus(unit, module);
    quint8 type = STATUS_BOARD_TYPE(boardStatus);
    QFile delegateChange;
    if(STATUS_BOARD_TYPE_AI == type) {
        ConfigTreeObject::getConfigObject(MEASURE_DELEGATE_U_M_DIMODE)->setHideFlag(true);

        delegateChange.setFileName(":/configtree/Image/configTree/Measure/Measure_SettingDelegate/module-delegate-AI.xml");
        if(!delegateChange.open(QFile::ReadOnly | QFile::Text)){
            qWarning()<< "Can't open the file: module-delegate-AI!";
        }else{
            XmlConfigParser::parseDelegateChangeXml(delegateChange, unit, module);
            delegateChange.close();
        }

        updateModuleStrShowOn_AI();
    } else if ((STATUS_BOARD_TYPE_DI == type) || (STATUS_BOARD_TYPE_DIO == type)) {
        ConfigTreeObject::getConfigObject(MEASURE_DELEGATE_U_M_ADTIME)->setHideFlag(true);
        ConfigTreeObject::getConfigObject(MEASURE_DELEGATE_U_M_GENER)->setHideFlag(true);

        delegateChange.setFileName(":/configtree/Image/configTree/Measure/Measure_SettingDelegate/module-delegate-DI.xml");
        if(!delegateChange.open(QFile::ReadOnly | QFile::Text)){
            qWarning()<< "Can't open the file: module-delegate-DI!";
        }else{
            XmlConfigParser::parseDelegateChangeXml(delegateChange, unit, module);
            delegateChange.close();
        }

        updateModuleStrShowOn_DI();
    }
}

void ConfigTreeObject_Measure::slotMeasureIntervalChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(MEASURE_DELEGATE_INTERVAL_INTERVAL)->getData();

    QFile file(XML_MEASURE_INTERVAL_INTERVAL);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:MeasureInter_value!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }

    QMap<quint32,QString> m_map_trend;
    m_map_trend.insert(Trend_Interval_5s, tr("5s"));
    m_map_trend.insert(Trend_Interval_10s, tr("10s"));
    m_map_trend.insert(Trend_Interval_15s, tr("15s"));
    m_map_trend.insert(Trend_Interval_30s, tr("30s"));
    m_map_trend.insert(Trend_Interval_1min, tr("1min"));
    m_map_trend.insert(Trend_Interval_2min, tr("2min"));
    m_map_trend.insert(Trend_Interval_5min, tr("5min"));
    m_map_trend.insert(Trend_Interval_10min, tr("10min"));
    m_map_trend.insert(Trend_Interval_15min, tr("15min"));
    m_map_trend.insert(Trend_Interval_20min, tr("20min"));
    m_map_trend.insert(Trend_Interval_30min, tr("30min"));
    m_map_trend.insert(Trend_Interval_1h, tr("1h"));
    m_map_trend.insert(Trend_Interval_2h, tr("2h"));
    m_map_trend.insert(Trend_Interval_4h, tr("4h"));
    m_map_trend.insert(Trend_Interval_10h, tr("10h"));

    QMap<quint32,QString> m_map_event;//事件记录周期是测量周期的整数倍，且最小值>=测量周期
    m_map_event.insert(Event_Record_Interval_100ms, tr("100ms"));
    m_map_event.insert(Event_Record_Interval_200ms, tr("200ms"));
    m_map_event.insert(Event_Record_Interval_500ms, tr("500ms"));
    m_map_event.insert(Event_Record_Interval_1s, tr("1s"));
    m_map_event.insert(Event_Record_Interval_2s, tr("2s"));
    m_map_event.insert(Event_Record_Interval_5s, tr("5s"));
    m_map_event.insert(Event_Record_Interval_10s, tr("10s"));
    m_map_event.insert(Event_Record_Interval_15s, tr("15s"));
    m_map_event.insert(Event_Record_Interval_20s, tr("20s"));
    m_map_event.insert(Event_Record_Interval_30s, tr("30s"));
    m_map_event.insert(Event_Record_Interval_1min, tr("1min"));
    m_map_event.insert(Event_Record_Interval_2min, tr("2min"));
    m_map_event.insert(Event_Record_Interval_5min, tr("5min"));
    m_map_event.insert(Event_Record_Interval_10min, tr("10min"));
    m_map_event.insert(Event_Record_Interval_15min, tr("15min"));
    m_map_event.insert(Event_Record_Interval_20min, tr("20min"));
    m_map_event.insert(Event_Record_Interval_30min, tr("30min"));

    Measure_Interval valueCur = (Measure_Interval)value.toUInt();
    switch(valueCur) {
    case Measure_Interval_100ms:
        break;
    case Measure_Interval_200ms:
        m_map_trend.remove(Trend_Interval_5s);
        m_map_trend.remove(Trend_Interval_15s);

        m_map_event.remove(Event_Record_Interval_100ms);
        break;
    case Measure_Interval_500ms:
        m_map_trend.remove(Trend_Interval_5s);
        m_map_trend.remove(Trend_Interval_10s);

        m_map_event.remove(Event_Record_Interval_100ms);
        m_map_event.remove(Event_Record_Interval_200ms);
        break;
    case Measure_Interval_1s:
        m_map_trend.remove(Trend_Interval_5s);
        m_map_trend.remove(Trend_Interval_10s);
        m_map_trend.remove(Trend_Interval_15s);

        m_map_event.remove(Event_Record_Interval_100ms);
        m_map_event.remove(Event_Record_Interval_200ms);
        m_map_event.remove(Event_Record_Interval_500ms);
        break;
    case Measure_Interval_2s:
        m_map_trend.remove(Trend_Interval_5s);
        m_map_trend.remove(Trend_Interval_10s);
        m_map_trend.remove(Trend_Interval_15s);
        m_map_trend.remove(Trend_Interval_30s);

        m_map_event.remove(Event_Record_Interval_100ms);
        m_map_event.remove(Event_Record_Interval_200ms);
        m_map_event.remove(Event_Record_Interval_500ms);
        m_map_event.remove(Event_Record_Interval_1s);
        m_map_event.remove(Event_Record_Interval_5s);
        m_map_event.remove(Event_Record_Interval_15s);
        break;
    case Measure_Interval_5s:
        m_map_trend.remove(Trend_Interval_5s);
        m_map_trend.remove(Trend_Interval_10s);
        m_map_trend.remove(Trend_Interval_15s);
        m_map_trend.remove(Trend_Interval_30s);
        m_map_trend.remove(Trend_Interval_1min);
        m_map_trend.remove(Trend_Interval_2min);

        m_map_event.remove(Event_Record_Interval_100ms);
        m_map_event.remove(Event_Record_Interval_200ms);
        m_map_event.remove(Event_Record_Interval_500ms);
        m_map_event.remove(Event_Record_Interval_1s);
        m_map_event.remove(Event_Record_Interval_2s);
        break;
    default:
        break;
    }

    ConfigObject* pObj = ConfigTreeObject::getConfigObject(DISP_DELEGATE_TREND_INTERVAL_INTERVAL);
    ConfigObject* qObj = ConfigTreeObject::getConfigObject(DISP_DELEGATE_TREND_INTERVAL_SECOND);
    if(pObj && qObj) {
        quint32 first = pObj->getData().toUInt();
        if(first < m_map_trend.firstKey()) {
            pObj->changeData(m_map_trend.firstKey(), false);
        } else {
            if(!(m_map_trend.contains(first))) {
                pObj->changeData(m_map_trend.firstKey(), false);
            }
        }
        quint32 second = qObj->getData().toUInt();
        if(second < m_map_trend.firstKey()) {
            qObj->changeData(m_map_trend.firstKey(), false);
        } else {
            if(!(m_map_trend.contains(second))) {
                qObj->changeData(m_map_trend.firstKey(), false);
            }
        }
    }

    ConfigObject* rObj = ConfigTreeObject::getConfigObject(RECORD_DELEGATE_BASIC_EVENT_INTERVAL);
    if(rObj) {
        quint32 eventvalue = rObj->getData().toUInt();
        if(eventvalue < m_map_event.firstKey()) {
            rObj->changeData(m_map_event.firstKey(), false);
        } else {
            if(!(m_map_event.contains(eventvalue))) {
                rObj->changeData(m_map_event.firstKey(), false);
            }
        }
    }
}

void ConfigTreeObject_Measure::slotMeasureOverRangeChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(MEASURE_DELEGATE_OVERANG_VALUE)->getData();

    QFile file(XML_MEASURE_OVERANG_VALUE);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:MeasureRange_value!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Measure::slotMeasureADmodechanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(MEASURE_DELEGATE_U_M_ADTIME_VALUE)->getData();

    QFile file(XML_MEASURE_ADMODE);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Measure_ADmode!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Measure::slotMeasureBurnLowchanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(MEASURE_DELEGATE_U_M_GENER_LOWER)->getData();

    QFile file(XML_MEASURE_BURNLOW);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Measure_BurnLow!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Measure::slotMeasureBurnUpchanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(MEASURE_DELEGATE_U_M_GENER_UPPER)->getData();

    QFile file(XML_MEASURE_BURNUP);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Measure_BurnUp!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Measure::slotMeasureDIOperModechanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(MEASURE_DELEGATE_U_M_DIMODE_MODE)->getData();

    QFile file(XML_MEASURE_DIMODE);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Measure_DImode!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}
