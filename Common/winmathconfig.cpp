#include "winmathconfig.h"



WinMathActionError *WinMathActionError::instance()
{
    static WinMathActionError  instance;
    return &instance;
}

WinMathActionError::WinMathActionError(QWidget *parent)
    :WinChannelSelector(parent)
{
    m_map_ActionError.insert(Math_Error_Value_Positive, tr("+Over"));
    m_map_ActionError.insert(Math_Error_Value_Negative, tr("-Over"));

    slotResetMenus();
}

void WinMathActionError::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map_ActionError.begin();
    for(;item != m_map_ActionError.end();item++) {
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}


WinMathActionAction *WinMathActionAction::instance()
{
    static WinMathActionAction  instance;
    return &instance;
}

WinMathActionAction::WinMathActionAction(QWidget *parent)
    :WinChannelSelector(parent)
{
    m_map_ActionAction.insert(Math_Key_Action_Off, tr("Off"));
    m_map_ActionAction.insert(Math_Key_Action_Normal, tr("Start / Stop"));
    m_map_ActionAction.insert(Math_Key_Action_Reset, tr("Reset + Start / Stop"));

    slotResetMenus();
}

void WinMathActionAction::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map_ActionAction.begin();
    for(;item != m_map_ActionAction.end();item++) {
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}


WinMathActionSumave *WinMathActionSumave::instance()
{
    static WinMathActionSumave  instance;
    return &instance;
}

WinMathActionSumave::WinMathActionSumave(QWidget *parent)
    :WinChannelSelector(parent)
{
    m_map_ActionSumave.insert(Math_Over_Range_Error, tr("Error"));
    m_map_ActionSumave.insert(Math_Over_Range_Limit, tr("Limit"));
    m_map_ActionSumave.insert(Math_Over_Range_Skip, tr("Skip"));

    slotResetMenus();
}

void WinMathActionSumave::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map_ActionSumave.begin();
    for(;item != m_map_ActionSumave.end();item++) {
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}


WinMathActionPP *WinMathActionPP::instance()
{
    static WinMathActionPP  instance;
    return &instance;
}

WinMathActionPP::WinMathActionPP(QWidget *parent)
    :WinChannelSelector(parent)
{
    m_map_ActionPP.insert(Math_Over_Range_Skip, tr("Skip"));
    m_map_ActionPP.insert(Math_Over_Range_Over, tr("Over"));

    slotResetMenus();
}

void WinMathActionPP::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map_ActionPP.begin();
    for(;item != m_map_ActionPP.end();item++) {
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}


WinMathExpTLOGType *WinMathExpTLOGType::instance()
{
    static WinMathExpTLOGType  instance;
    return &instance;
}

WinMathExpTLOGType::WinMathExpTLOGType(QWidget *parent)
    :WinChannelSelector(parent)
{
    m_map_ExpTLOGType.insert(0, tr("Timer"));
    m_map_ExpTLOGType.insert(1, tr("Match time timer"));

    slotResetMenus();
}

void WinMathExpTLOGType::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map_ExpTLOGType.begin();
    for(;item != m_map_ExpTLOGType.end();item++) {
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}


WinMathExpTLOGNo *WinMathExpTLOGNo::instance()
{
    static WinMathExpTLOGNo  instance;
    return &instance;
}

WinMathExpTLOGNo::WinMathExpTLOGNo(QWidget *parent)
    :WinChannelSelector(parent)
{
    m_map_ExpTLOGNo.insert(1, tr("1"));
    m_map_ExpTLOGNo.insert(2, tr("2"));
    m_map_ExpTLOGNo.insert(3, tr("3"));
    m_map_ExpTLOGNo.insert(4, tr("4"));

    slotResetMenus();
}

void WinMathExpTLOGNo::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map_ExpTLOGNo.begin();
    for(;item != m_map_ExpTLOGNo.end();item++) {
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}


WinMathExpTLOGSum *WinMathExpTLOGSum::instance()
{
    static WinMathExpTLOGSum  instance;
    return &instance;
}

WinMathExpTLOGSum::WinMathExpTLOGSum(QWidget *parent)
    :WinChannelSelector(parent)
{
    m_map_ExpTLOGSum.insert(0, tr("Off"));
    m_map_ExpTLOGSum.insert(1000, tr("sec"));
    m_map_ExpTLOGSum.insert(60000, tr("min"));
    m_map_ExpTLOGSum.insert(3600000, tr("hour"));

    slotResetMenus();
}

void WinMathExpTLOGSum::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map_ExpTLOGSum.begin();
    for(;item != m_map_ExpTLOGSum.end();item++) {
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}


WinMathExpRollInter *WinMathExpRollInter::instance()
{
    static WinMathExpRollInter  instance;
    return &instance;
}

WinMathExpRollInter::WinMathExpRollInter(QWidget *parent)
    :WinChannelSelector(parent)
{
    m_map_ExpRollInter.insert(0, tr("1s"));
    m_map_ExpRollInter.insert(1, tr("2s"));
    m_map_ExpRollInter.insert(2, tr("3s"));
    m_map_ExpRollInter.insert(3, tr("4s"));
    m_map_ExpRollInter.insert(4, tr("5s"));
    m_map_ExpRollInter.insert(5, tr("6s"));
    m_map_ExpRollInter.insert(6, tr("10s"));
    m_map_ExpRollInter.insert(7, tr("12s"));
    m_map_ExpRollInter.insert(8, tr("15s"));
    m_map_ExpRollInter.insert(9, tr("20s"));
    m_map_ExpRollInter.insert(10, tr("30s"));
    m_map_ExpRollInter.insert(11, tr("1min"));
    m_map_ExpRollInter.insert(12, tr("2min"));
    m_map_ExpRollInter.insert(13, tr("3min"));
    m_map_ExpRollInter.insert(14, tr("4min"));
    m_map_ExpRollInter.insert(15, tr("5min"));
    m_map_ExpRollInter.insert(16, tr("6min"));
    m_map_ExpRollInter.insert(17, tr("10min"));
    m_map_ExpRollInter.insert(18, tr("12min"));
    m_map_ExpRollInter.insert(19, tr("15min"));
    m_map_ExpRollInter.insert(20, tr("20min"));
    m_map_ExpRollInter.insert(21, tr("30min"));
    m_map_ExpRollInter.insert(22, tr("1h"));

    slotResetMenus();
}

void WinMathExpRollInter::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map_ExpRollInter.begin();
    for(;item != m_map_ExpRollInter.end();item++) {
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}


WinMathConstKNum *WinMathConstKNum::instance()
{
    static WinMathConstKNum  instance;
    return &instance;
}

WinMathConstKNum::WinMathConstKNum(QWidget *parent)
    :WinChannelSelector(parent)
{
    m_map_ConstKNum.insert(0, tr("K001 - K010"));
    m_map_ConstKNum.insert(1, tr("K011 - K020"));
    m_map_ConstKNum.insert(2, tr("K021 - K030"));
    m_map_ConstKNum.insert(3, tr("K031 - K040"));
    m_map_ConstKNum.insert(4, tr("K041 - K050"));
    m_map_ConstKNum.insert(5, tr("K051 - K060"));
    m_map_ConstKNum.insert(6, tr("K061 - K070"));
    m_map_ConstKNum.insert(7, tr("K071 - K080"));
    m_map_ConstKNum.insert(8, tr("K081 - K090"));
    m_map_ConstKNum.insert(9, tr("K091 - K100"));

    slotResetMenus();
}

void WinMathConstKNum::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map_ConstKNum.begin();
    for(;item != m_map_ConstKNum.end();item++) {
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}


WinMathAlarmLvl1Type *WinMathAlarmLvl1Type::instance()
{
    static WinMathAlarmLvl1Type  instance;
    return &instance;
}

WinMathAlarmLvl1Type::WinMathAlarmLvl1Type(QWidget *parent)
    :WinChannelSelector(parent)
{
    m_map_AlarmLvl1Type.insert(Alarm_Type_H, tr("H : High limit"));
    m_map_AlarmLvl1Type.insert(Alarm_Type_L, tr("L : Low limit"));
    m_map_AlarmLvl1Type.insert(Alarm_Type_T, tr("T : Delay high limit"));
    m_map_AlarmLvl1Type.insert(Alarm_Type_t, tr("t : Delay low limit"));

    slotResetMenus();
}

void WinMathAlarmLvl1Type::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map_AlarmLvl1Type.begin();
    for(;item != m_map_AlarmLvl1Type.end();item++) {
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}
