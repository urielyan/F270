#include "configtreeobject_EventAction.h"
#include "cfgchannel.h"
#include "./GlobalData/globalDef.h"

#include "cfgGlobalDef.h"
#include "configtreeobject.h"

ConfigTreeObject_EventAction *ConfigTreeObject_EventAction::instance(QObject *parent)
{
    static ConfigTreeObject_EventAction data(parent);
    return &data;
}

ConfigTreeObject_EventAction::ConfigTreeObject_EventAction(QObject *parent):
      ConfigObject(parent)
{
    //Event Action代理树
    QFile fileDelegate(":/configtree/Image/configTree/Event/Event_SettingDelegate/delegate.xml");
    if(!fileDelegate.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file: Event_settingDelegate!";
    }else{
        XmlConfigParser::parseConfigXml(fileDelegate, static_cast<ConfigObject*>(parent), \
                                                          (QObject*)this);
        fileDelegate.close();
    }
}

void ConfigTreeObject_EventAction::updateStrShowOn()
{
    slotEventAction_E_Typechanged();
    slotEventAction_E_Numberchanged();
    slotEventAction_E_Alarmlvlchanged();
    slotEventAction_E_Detailchanged();
    slotEventAction_E_Modechanged();

    slotEventAction_A_Typechanged();
    slotEventAction_A_Numberchanged();
    slotEventAction_A_Detailchanged();
    slotEventAction_A_GrpNochanged();

    slotEventAction_GrpSWchanged();
}

void ConfigTreeObject_EventAction::slotCreateEventsettingTree(QObject *parent)
{
    //Event Action配置树
    QFile file(":/configtree/Image/configTree/Event/Event_Setting/config.xml");
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file: Event_setting!";
    }else{
        XmlConfigParser::parseConfigXml(file, static_cast<ConfigObject*>(parent));
        file.close();
    }

    //Event Action初始化--特殊情况，一般的object都在解析时初始化
    quint32 ID_group = ConfigTreeObject::getConfigObject(EVENTCONFIG_GRPNUM)->getObjectID();
    ConfigTreeObject::getConfigObject(EVENTCONFIG_GRPNUM)->initData(0, 0, ID_group);
    ConfigTreeObject::getConfigObject(EVENTCONFIG_GRPNUM)->setStrShowOn("1");
}

void ConfigTreeObject_EventAction::slotEventDelegateChange()
{
    QFile delegateChange(":/configtree/Image/configTree/Event/Event_DelegateChange/delegateChange.xml");
    if(!delegateChange.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file: Event_settingDelegateChange!";
    }else{
        XmlConfigParser::parseDelegateChangeXml(delegateChange);
        delegateChange.close();
    }

    _curStatus = false;
    updateStrShowOn();
    _curStatus = true;
}

void ConfigTreeObject_EventAction::slotEventAction_GrpNumchanged()
{
    ConfigObject *pObj = ConfigTreeObject::getConfigObject(EVENT_DELEGATE_GRPNUM);
    if(pObj) {
        quint32 groupNum = pObj->getData().toUInt();
        pObj->setStrShowOn(QString("%1").arg(groupNum+1));
    }
}

void ConfigTreeObject_EventAction::slotEventAction_GrpSWchanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(EVENT_DELEGATE_GROUP_SW)->getData();

    QFile file(XML_EVENT_GROUP_SW);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:EventAction_GroupSW!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_EventAction::slotEventAction_E_Typechanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(EVENT_DELEGATE_EVENT_TYPE)->getData();

    QFile file(XML_EVENT_EVENT_TYPE);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:EventAction_EventType!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }

    QMap<quint32,QString> m_map_EventMode;
    m_map_EventMode.insert(Operation_Mode_Type_RisingEdge, tr("Rising edge"));
    m_map_EventMode.insert(Operation_Mode_Type_FallingEdge, tr("Falling edge"));
    m_map_EventMode.insert(Operation_Mode_Type_Both, tr("Rising / Falling edge"));
    m_map_EventMode.insert(Operation_Mode_Type_Edge, tr("Edge"));

    Event_Type valueCur = (Event_Type)value.toUInt();
    if ((Event_Type_Timer == valueCur) || (Event_Type_Timer_Match == valueCur)
            || (Event_Type_UserFunc == valueCur)) {
        m_map_EventMode.remove(Operation_Mode_Type_RisingEdge);
        m_map_EventMode.remove(Operation_Mode_Type_FallingEdge);
        m_map_EventMode.remove(Operation_Mode_Type_Both);
    } else {
        m_map_EventMode.remove(Operation_Mode_Type_Edge);
    }

    ConfigObject* pObj = ConfigTreeObject::getConfigObject(EVENT_DELEGATE_EVENT_MODE);
    if(pObj) {
        Operation_Mode_Type modeCur = (Operation_Mode_Type)pObj->getData().toUInt();
        if(!(m_map_EventMode.contains(modeCur))) {
            pObj->changeData(m_map_EventMode.firstKey(), false);
        }
    }

    ConfigObject* qObj = ConfigTreeObject::getConfigObject(EVENT_DELEGATE_EVENT_NUMBER);
    if(qObj) {
        switch(valueCur) {
        case Event_Type_InterSwitch:
        case Event_Type_Timer:
        case Event_Type_Timer_Match:
        case Event_Type_UserFunc:
            qObj->changeData(1, false);
            break;
        case Event_Type_Remote://need to modify TODO
            qObj->changeData(CfgChannel::instance()->getExistChannelDI().first(), false);
            break;
        case Event_Type_Relay:
            qObj->changeData(CfgChannel::instance()->getExistChannelDO().first(), false);
            break;
        case Event_Type_Alarm_IO:
            qObj->changeData(CfgChannel::instance()->getExistChannel().first(), false);
            break;
        case Event_Type_Alarm_Math:
            qObj->changeData(CfgChannel::instance()->getExistChannelMath().first(), false);
            break;
        case Event_Type_Alarm_Comm:
            qObj->changeData(CfgChannel::instance()->getExistChannelComm().first(), false);
            break;
        case Event_Type_Alarm_All:
        case Event_Type_Status:
            break;
        default:
            break;
        }
    }
}

void ConfigTreeObject_EventAction::slotEventAction_E_Numberchanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(EVENT_DELEGATE_EVENT_NUMBER)->getData();

    QFile file(XML_EVENT_EVENT_NUMBER);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:EventAction_EventNumber!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_EventAction::slotEventAction_E_Detailchanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(EVENT_DELEGATE_EVENT_STATUS)->getData();

    QFile file(XML_EVENT_EVENT_STATUS);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:EventAction_EventDetail!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_EventAction::slotEventAction_E_Alarmlvlchanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(EVENT_DELEGATE_EVENT_ALARM)->getData();

    QFile file(XML_EVENT_EVENT_ALARM);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:EventAction_EventAlarm!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_EventAction::slotEventAction_E_Modechanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(EVENT_DELEGATE_EVENT_MODE)->getData();

    QFile file(XML_EVENT_EVENT_MODE);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:EventAction_EventMode!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_EventAction::slotEventAction_A_Typechanged()
{
    QFile file;
    Operation_Mode_Type mode = (Operation_Mode_Type)ConfigTreeObject \
            ::getConfigObject(EVENT_DELEGATE_EVENT_MODE)->getData().toUInt();

    switch(mode) {
    case Operation_Mode_Type_RisingEdge:
    case Operation_Mode_Type_FallingEdge:
    case Operation_Mode_Type_Edge:
        file.setFileName(XML_EVENT_ACTION_TYPE);
        break;
    case Operation_Mode_Type_Both:
        file.setFileName(XML_EVENT_ACTION_TYPE_C);
        break;
    }

    QVariant value = ConfigTreeObject::getConfigObject(EVENT_DELEGATE_ACTION_TYPE)->getData();

    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:EventAction_ActionType!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }

    QMap<quint32,QString> m_map;

    Action_Type valueCur = (Action_Type)value.toUInt();
    if(Action_Type1_Recorded == valueCur) {
        m_map.insert(Action_Detail_Start, tr("Start"));
        m_map.insert(Action_Detail_Stop, tr("Stop"));
    } else if (Action_Type1_Computation == valueCur) {
        m_map.insert(Action_Detail_Start, tr("Start"));
        m_map.insert(Action_Detail_Stop, tr("Stop"));
        m_map.insert(Action_Detail_Reset, tr("Reset"));
    } else if (Action_Type1_Message == valueCur) {
        m_map.insert(Action_Detail_AllGrp, tr("All groups"));
        m_map.insert(Action_Detail_SpecGrp, tr("Specified group"));
    } else {
        return;
    }

    ConfigObject* pObj = ConfigTreeObject::getConfigObject(EVENT_DELEGATE_ACTION_DETAIL);
    if(pObj) {
        Action_Detail detailCur = (Action_Detail)pObj->getData().toUInt();
        if(!(m_map.contains(detailCur))) {
            pObj->changeData(m_map.firstKey(), false);
        }
    }

}

void ConfigTreeObject_EventAction::slotEventAction_A_Numberchanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(EVENT_DELEGATE_ACTION_NUMBER)->getData();

    QFile file(XML_EVENT_ACTION_NUMBER);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:EventAction_ActionNumber!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_EventAction::slotEventAction_A_Detailchanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(EVENT_DELEGATE_ACTION_DETAIL)->getData();

    QFile file(XML_EVENT_ACTION_DETAIL);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:EventAction_ActionDetail!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_EventAction::slotEventAction_A_GrpNochanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(EVENT_DELEGATE_ACTION_GROUPNUM)->getData();

    QFile file(XML_EVENT_ACTION_GROUPNUM);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:EventAction_ActionGrpNo!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

