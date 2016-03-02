#include "wineventconfig.h"


WinEventGrpNum *WinEventGrpNum::instance()
{
    static WinEventGrpNum  instance;
    return &instance;
}

WinEventGrpNum::WinEventGrpNum(QWidget *parent)
    :WinChannelSelector(parent)
{
    for(int i=0;i<EVENT_ACTION_NUMBER;++i) {
        QString istr = QString("%1").arg(i+1);
        m_map_GrpNum.insert(i, istr);
    }

    slotResetMenus();
}

void WinEventGrpNum::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map_GrpNum.begin();
    for(;item != m_map_GrpNum.end();item++) {
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}


WinEventEventType *WinEventEventType::instance()
{
    static WinEventEventType  instance;
    return &instance;
}

WinEventEventType::WinEventEventType(QWidget *parent)
    :WinChannelSelector(parent)
{
    m_map_EventType.insert(Event_Type_InterSwitch, tr("Internal switch"));
    m_map_EventType.insert(Event_Type_Remote, tr("Remote"));
    m_map_EventType.insert(Event_Type_Relay, tr("Relay"));
    m_map_EventType.insert(Event_Type_Alarm_IO, tr("Alarm - I/O channel"));
    m_map_EventType.insert(Event_Type_Alarm_Math, tr("Alarm - Math channel"));
    m_map_EventType.insert(Event_Type_Alarm_Comm, tr("Alarm - Communication channel"));
    m_map_EventType.insert(Event_Type_Alarm_All, tr("Any alarm"));
    m_map_EventType.insert(Event_Type_Timer, tr("Timer"));
    m_map_EventType.insert(Event_Type_Timer_Match, tr("Match time timer"));
    m_map_EventType.insert(Event_Type_UserFunc, tr("User function key"));
    m_map_EventType.insert(Event_Type_Status, tr("Status"));

    slotResetMenus();
}

void WinEventEventType::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map.begin();
    for(;item != m_map.end();item++) {
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}

void WinEventEventType::initMenu()
{
    m_map.clear();
    m_map = m_map_EventType;

    if(CfgChannel::instance()->getList_BoardIO().isEmpty()) {
        m_map.remove(Event_Type_Alarm_IO);
    }

    if(CfgChannel::instance()->getExistChannelDI().isEmpty()) {
        m_map.remove(Event_Type_Remote);
    } else {
        //TODO  DI模块的动作模式为[远程控制输入]时显示。
    }

    if(CfgChannel::instance()->getExistChannelDO().isEmpty()) {
        m_map.remove(Event_Type_Relay);
    }

    slotResetMenus();
}


WinEventEventAlarmlevel *WinEventEventAlarmlevel::instance()
{
    static WinEventEventAlarmlevel  instance;
    return &instance;
}

WinEventEventAlarmlevel::WinEventEventAlarmlevel(QWidget *parent)
    :WinChannelSelector(parent)
{
    m_map_EventAlarmlevel.insert(1, tr("Alarm Level 1"));
    m_map_EventAlarmlevel.insert(2, tr("Alarm Level 2"));
    m_map_EventAlarmlevel.insert(3, tr("Alarm Level 3"));
    m_map_EventAlarmlevel.insert(4, tr("Alarm Level 4"));

    slotResetMenus();
}

void WinEventEventAlarmlevel::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map_EventAlarmlevel.begin();
    for(;item != m_map_EventAlarmlevel.end();item++) {
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}


WinEventEventDetails *WinEventEventDetails::instance()
{
    static WinEventEventDetails  instance;
    return &instance;
}

WinEventEventDetails::WinEventEventDetails(QWidget *parent)
    :WinChannelSelector(parent)
{
    m_map_EventDetails.insert(Device_Status_Type_Record, tr("Record"));
    m_map_EventDetails.insert(Device_Status_Type_Math, tr("Computation"));
    m_map_EventDetails.insert(Device_Status_Type_UserLock, tr("User lock"));
    m_map_EventDetails.insert(Device_Status_Type_UnderLogin, tr("Under login"));
    m_map_EventDetails.insert(Device_Status_Type_MemErr, tr("Memory/Media error"));
    m_map_EventDetails.insert(Device_Status_Type_MeasureErr, tr("Measurement error"));
    m_map_EventDetails.insert(Device_Status_Type_CommErr, tr("Communication error"));

    slotResetMenus();
}

void WinEventEventDetails::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map_EventDetails.begin();
    for(;item != m_map_EventDetails.end();item++) {
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}


WinEventEventMode *WinEventEventMode::instance()
{
    static WinEventEventMode  instance;
    return &instance;
}

WinEventEventMode::WinEventEventMode(QWidget *parent)
    :WinChannelSelector(parent)
{
    m_map_EventMode.insert(Operation_Mode_Type_RisingEdge, tr("Rising edge"));
    m_map_EventMode.insert(Operation_Mode_Type_FallingEdge, tr("Falling edge"));
    m_map_EventMode.insert(Operation_Mode_Type_Both, tr("Rising / Falling edge"));
    m_map_EventMode.insert(Operation_Mode_Type_Edge, tr("Edge"));

    slotResetMenus();
}

void WinEventEventMode::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map.begin();
    for(;item != m_map.end();item++) {
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}

void WinEventEventMode::initMenu()
{
    m_map.clear();
    m_map = m_map_EventMode;

    ConfigObject* pObj = ConfigTreeObject::getConfigObject(EVENT_DELEGATE_EVENT_TYPE);
    if(pObj) {
        Event_Type type = (Event_Type)pObj->getData().toUInt();
        if ((Event_Type_Timer == type) || (Event_Type_Timer_Match == type)
                || (Event_Type_UserFunc == type)) {
            m_map.remove(Operation_Mode_Type_RisingEdge);
            m_map.remove(Operation_Mode_Type_FallingEdge);
            m_map.remove(Operation_Mode_Type_Both);
        } else {
            m_map.remove(Operation_Mode_Type_Edge);
        }
    }

    slotResetMenus();
}


WinEventActionType *WinEventActionType::instance()
{
    static WinEventActionType  instance;
    return &instance;
}

WinEventActionType::WinEventActionType(QWidget *parent)
    :WinChannelSelector(parent)
{
    m_map_ActionType.insert(Action_Type1_Recorded, tr("Record"));
    m_map_ActionType.insert(Action_Type1_Computation, tr("Computation"));
    m_map_ActionType.insert(Action_Type1_SwitchRate, tr("Switch the display rate"));
    m_map_ActionType.insert(Action_Type1_Flag, tr("Flag"));
    m_map_ActionType.insert(Action_Type1_ManualSample, tr("Manual sample"));
    m_map_ActionType.insert(Action_Type1_AlarmAck, tr("AlarmAck"));
    m_map_ActionType.insert(Action_Type1_Snapshot, tr("Snapshot"));
//    m_map_ActionType.insert(Action_Type1_AdjustTime, tr("Adjust the time"));
    m_map_ActionType.insert(Action_Type1_SaveDispData, tr("save display data"));
    m_map_ActionType.insert(Action_Type1_SaveEventData, tr("Save event data"));
    m_map_ActionType.insert(Action_Type1_EventTrigger, tr("Event trigger"));
    m_map_ActionType.insert(Action_Type1_Message, tr("Message"));
    m_map_ActionType.insert(Action_Type1_SwitchGroup, tr("Switch the display group"));
    m_map_ActionType.insert(Action_Type1_TimerReset, tr("Reset the relative timer"));
//    m_map_ActionType.insert(Action_Type1_LoadSetting, tr("Load Settings"));
    m_map_ActionType.insert(Action_Type1_HomeWin, tr("Show the favorite display"));

    m_map_ActionType_Both.insert(Action_Type2_Record_Start_Stop, tr("Recording start/stop"));
    m_map_ActionType_Both.insert(Action_Type2_Computation_Start_Stop, tr("Computation start/stop"));
    m_map_ActionType_Both.insert(Action_Type2_SwitchRate_1_2, tr("Switch the display rate 1/2"));
    m_map_ActionType_Both.insert(Action_Type2_Flag_On_Off, tr("Flag On/Off"));

    slotResetMenus();
}

void WinEventActionType::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map.begin();
    for(;item != m_map.end();item++) {
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}

void WinEventActionType::initMenu()
{
    m_map.clear();

    ConfigObject* pObj = ConfigTreeObject::getConfigObject(EVENT_DELEGATE_EVENT_MODE);
    if(pObj) {
        Operation_Mode_Type mode = (Operation_Mode_Type)pObj->getData().toUInt();
        if(mode == Operation_Mode_Type_Both) {
            m_map = m_map_ActionType_Both;
        } else {
            m_map = m_map_ActionType;
        }
    }

    slotResetMenus();
}


WinEventActionDetail *WinEventActionDetail::instance()
{
    static WinEventActionDetail  instance;
    return &instance;
}

WinEventActionDetail::WinEventActionDetail(QWidget *parent)
    :WinChannelSelector(parent)
{
    m_map_ActionDetail_Record.insert(Action_Detail_Start, tr("Start"));
    m_map_ActionDetail_Record.insert(Action_Detail_Stop, tr("Stop"));

    m_map_ActionDetail_Math.insert(Action_Detail_Start, tr("Start"));
    m_map_ActionDetail_Math.insert(Action_Detail_Stop, tr("Stop"));
    m_map_ActionDetail_Math.insert(Action_Detail_Reset, tr("Reset"));

    m_map_ActionDetail_Msg.insert(Action_Detail_AllGrp, tr("All groups"));
    m_map_ActionDetail_Msg.insert(Action_Detail_SpecGrp, tr("Specified group"));

    slotResetMenus();
}

void WinEventActionDetail::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map.begin();
    for(;item != m_map.end();item++) {
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}

void WinEventActionDetail::initMenu()
{
    m_map.clear();

    ConfigObject* pObj = ConfigTreeObject::getConfigObject(EVENT_DELEGATE_ACTION_TYPE);
    if(pObj) {
        Action_Type type = (Action_Type)pObj->getData().toUInt();
        if(Action_Type1_Recorded == type) {
            m_map = m_map_ActionDetail_Record;
        } else if (Action_Type1_Computation == type) {
            m_map = m_map_ActionDetail_Math;
        } else if (Action_Type1_Message == type) {
            m_map = m_map_ActionDetail_Msg;
        }
    }

    slotResetMenus();
}
