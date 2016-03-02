#ifndef CONFIGTREEOBJECT_EVENTACTION_H
#define CONFIGTREEOBJECT_EVENTACTION_H

//EventAction 配置树路径
#define EVENTCONFIG_GRPNUM      "Config:Event action:Event action number"

#define EVENTCONFIG_GROUP       "Config:Event action:%1:Event action"
#define EVENTCONFIG_GROUP_SW    "Config:Event action:%1:Event action:On/Off"

#define EVENTCONFIG_EVENT           "Config:Event action:%1:Event"
#define EVENTCONFIG_EVENT_TYPE      "Config:Event action:%1:Event:Type"
#define EVENTCONFIG_EVENT_NUMBER    "Config:Event action:%1:Event:Number"
#define EVENTCONFIG_EVENT_ALARM     "Config:Event action:%1:Event:Alarm level"
#define EVENTCONFIG_EVENT_STATUS    "Config:Event action:%1:Event:Event details"
#define EVENTCONFIG_EVENT_MODE      "Config:Event action:%1:Event:Operation mode"

#define EVENTCONFIG_ACTION          "Config:Event action:%1:Action"
#define EVENTCONFIG_ACTION_TYPE     "Config:Event action:%1:Action:Type"
#define EVENTCONFIG_ACTION_NUMBER   "Config:Event action:%1:Action:Number"
#define EVENTCONFIG_ACTION_DETAIL   "Config:Event action:%1:Action:Detail"
#define EVENTCONFIG_ACTION_GROUPNUM "Config:Event action:%1:Action:Group number"

//EventAction 代理树路径
#define EVENT_DELEGATE      "Config:Event Delegate"
#define EVENT_DELEGATE_GRPNUM      "Config:Event Delegate:Event action number"

#define EVENT_DELEGATE_GROUP       "Config:Event Delegate:Event action"
#define EVENT_DELEGATE_GROUP_SW    "Config:Event Delegate:Event action:On/Off"

#define EVENT_DELEGATE_EVENT           "Config:Event Delegate:Event"
#define EVENT_DELEGATE_EVENT_TYPE      "Config:Event Delegate:Event:Type"
#define EVENT_DELEGATE_EVENT_NUMBER    "Config:Event Delegate:Event:Number"
#define EVENT_DELEGATE_EVENT_ALARM     "Config:Event Delegate:Event:Alarm level"
#define EVENT_DELEGATE_EVENT_STATUS    "Config:Event Delegate:Event:Event details"
#define EVENT_DELEGATE_EVENT_MODE      "Config:Event Delegate:Event:Operation mode"

#define EVENT_DELEGATE_ACTION          "Config:Event Delegate:Action"
#define EVENT_DELEGATE_ACTION_TYPE     "Config:Event Delegate:Action:Type"
#define EVENT_DELEGATE_ACTION_NUMBER   "Config:Event Delegate:Action:Number"
#define EVENT_DELEGATE_ACTION_DETAIL   "Config:Event Delegate:Action:Detail"
#define EVENT_DELEGATE_ACTION_GROUPNUM "Config:Event Delegate:Action:Group number"

//EventAction 值变更 XML文件路径
#define XML_EVENT_GROUP_SW          ":/configtree/Image/configTree/Event/Event_ValueChange/EventAction_GroupSW.xml"
#define XML_EVENT_EVENT_TYPE        ":/configtree/Image/configTree/Event/Event_ValueChange/EventAction_EventType.xml"
#define XML_EVENT_EVENT_NUMBER      ":/configtree/Image/configTree/Event/Event_ValueChange/EventAction_EventNumber.xml"
#define XML_EVENT_EVENT_ALARM       ":/configtree/Image/configTree/Event/Event_ValueChange/EventAction_EventAlarm.xml"
#define XML_EVENT_EVENT_STATUS      ":/configtree/Image/configTree/Event/Event_ValueChange/EventAction_EventDetail.xml"
#define XML_EVENT_EVENT_MODE        ":/configtree/Image/configTree/Event/Event_ValueChange/EventAction_EventMode.xml"
#define XML_EVENT_ACTION_TYPE       ":/configtree/Image/configTree/Event/Event_ValueChange/EventAction_ActionType.xml"
#define XML_EVENT_ACTION_TYPE_C     ":/configtree/Image/configTree/Event/Event_ValueChange/EventAction_ActionType_c.xml"
#define XML_EVENT_ACTION_NUMBER     ":/configtree/Image/configTree/Event/Event_ValueChange/EventAction_ActionNumber.xml"
#define XML_EVENT_ACTION_DETAIL     ":/configtree/Image/configTree/Event/Event_ValueChange/EventAction_ActionDetail.xml"
#define XML_EVENT_ACTION_GROUPNUM   ":/configtree/Image/configTree/Event/Event_ValueChange/EventAction_ActionGrpNo.xml"

#include "configobject.h"

class ConfigTreeObject_EventAction : public ConfigObject
{
    Q_OBJECT
public:
    static ConfigTreeObject_EventAction *instance(QObject *parent);

signals:

private:
    explicit ConfigTreeObject_EventAction(QObject *parent = 0);
    void updateStrShowOn();

public slots:
    void slotCreateEventsettingTree(QObject *parent = 0);

    void slotEventDelegateChange();

public slots:
    void slotEventAction_GrpNumchanged();

    void slotEventAction_GrpSWchanged();

    void slotEventAction_E_Typechanged();
    void slotEventAction_E_Numberchanged();
    void slotEventAction_E_Detailchanged();
    void slotEventAction_E_Alarmlvlchanged();
    void slotEventAction_E_Modechanged();

    void slotEventAction_A_Typechanged();
    void slotEventAction_A_Numberchanged();
    void slotEventAction_A_Detailchanged();
    void slotEventAction_A_GrpNochanged();
};

#endif // CONFIGTREEOBJECT_EVENTACTION_H
