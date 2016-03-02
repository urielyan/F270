/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：winaiconfig.cpp
 * 概    要：AI-channel settings下存在item单例的所有配置项目，构建并展开选项菜单list。
 *
 * 当前版本：V1.0.0
 * 作    者：邢 俊 杰
 * 完成日期：2015-9-22
 *
 * 修改版本：
 * 修改说明：
 * 修改作者：
 *
 ********************************************************************/
#include "winaiconfig.h"
#include "wininputconfig.h"
#include "devboard.h"

WinAIRangeTypeSelector *WinAIRangeTypeSelector::instance()
{
    static WinAIRangeTypeSelector  instance;
    return &instance;
}

WinAIRangeTypeSelector::WinAIRangeTypeSelector(QWidget *parent):
    WinChannelSelector(parent)
{
    m_map_RangeType.insert(AI_RANGE_Type_Disabled,tr("Skip"));
    m_map_RangeType.insert(AI_RANGE_Type_Vlot,tr("Volt"));
    m_map_RangeType.insert(AI_RANGE_Type_GS,tr("GS"));
    m_map_RangeType.insert(AI_RANGE_Type_TC,tr("TC"));
    m_map_RangeType.insert(AI_RANGE_Type_RTD,tr("RTD"));
    m_map_RangeType.insert(AI_RANGE_Type_DI,tr("DI"));
//    m_map_RangeType.insert(AI_RANGE_Type_Cur0_20mA,tr("Current(0-20mA)"));
//    m_map_RangeType.insert(AI_RANGE_Type_GS4_20mA,tr("GS(4-20mA)"));
    slotResetMenus();
}

void WinAIRangeTypeSelector::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map_RangeType.begin();
    for(;item != m_map_RangeType.end();item++){
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}


WinAIRangeRangeSelector *WinAIRangeRangeSelector::instance()
{
    static WinAIRangeRangeSelector  instance;
    return &instance;
}

WinAIRangeRangeSelector::WinAIRangeRangeSelector(QWidget *parent):
    WinChannelSelector(parent)
{
    m_map_Volt.insert(AI_Range_Volt_20mV,tr("20mV"));
    m_map_Volt.insert(AI_Range_Volt_60mV,tr("60mV"));
    m_map_Volt.insert(AI_Range_Volt_200mV,tr("200mV"));
    m_map_Volt.insert(AI_Range_Volt_1V,tr("1V"));
    m_map_Volt.insert(AI_Range_Volt_2V,tr("2V"));
    m_map_Volt.insert(AI_Range_Volt_6V,tr("6V"));
    m_map_Volt.insert(AI_Range_Volt_20V,tr("20V"));
    m_map_Volt.insert(AI_Range_Volt_50V,tr("50V"));

    m_map_GS.insert(AI_Range_GS_Low,tr("0.4-2V"));
    m_map_GS.insert(AI_Range_GS_High,tr("1-5V"));

    m_map_TC.insert(AI_Range_TC_R,tr("R"));
    m_map_TC.insert(AI_Range_TC_S,tr("S"));
    m_map_TC.insert(AI_Range_TC_B,tr("B"));
    m_map_TC.insert(AI_Range_TC_K,tr("K"));
    m_map_TC.insert(AI_Range_TC_K_H,tr("K-H"));
    m_map_TC.insert(AI_Range_TC_E,tr("E"));
    m_map_TC.insert(AI_Range_TC_C,tr("J"));
    m_map_TC.insert(AI_Range_TC_T,tr("T"));
    m_map_TC.insert(AI_Range_TC_N,tr("N"));
    m_map_TC.insert(AI_Range_TC_W,tr("W"));
    m_map_TC.insert(AI_Range_TC_L,tr("L"));
    m_map_TC.insert(AI_Range_TC_U,tr("U"));
    m_map_TC.insert(AI_Range_TC_WRe3_25,tr("WRe3-25"));
    m_map_TC.insert(AI_Range_TC_PLATINEL,tr("PLATINEL"));
    m_map_TC.insert(AI_Range_TC_PR20_40,tr("PR20-40"));
    m_map_TC.insert(AI_Range_TC_KpvsAu7Fe,tr("KpvsAu7Fe"));
    m_map_TC.insert(AI_Range_TC_NiNiMo,tr("NiNiMo"));
    m_map_TC.insert(AI_Range_TC_WWRe26,tr("WWRe26"));
    m_map_TC.insert(AI_Range_TC_N14,tr("N14"));
    m_map_TC.insert(AI_Range_TC_XK,tr("XK"));

    m_map_RTD.insert(AI_Range_RTD_Pt100,tr("Pt100"));
    m_map_RTD.insert(AI_Range_RTD_Pt100_H,tr("Pt100-H"));
    m_map_RTD.insert(AI_Range_RTD_JPt100,tr("JPt100"));
    m_map_RTD.insert(AI_Range_RTD_JPt100_H,tr("JPt100-H"));
    m_map_RTD.insert(AI_Range_RTD_Cu10GE,tr("Cu10GE"));
    m_map_RTD.insert(AI_Range_RTD_Cu10LN,tr("Cu10LN"));
    m_map_RTD.insert(AI_Range_RTD_Cu10WEED,tr("Cu10WEED"));
    m_map_RTD.insert(AI_Range_RTD_Cu10BAILEY,tr("Cu10BAILEY"));
    m_map_RTD.insert(AI_Range_RTD_Cu10a392,tr("Cu10a392"));
    m_map_RTD.insert(AI_Range_RTD_Cu10a393,tr("Cu10a393"));
    m_map_RTD.insert(AI_Range_RTD_Cu25,tr("Cu25"));
    m_map_RTD.insert(AI_Range_RTD_Cu53,tr("Cu53"));
    m_map_RTD.insert(AI_Range_RTD_Cu100,tr("Cu100"));
    m_map_RTD.insert(AI_Range_RTD_J263B,tr("J263B"));
    m_map_RTD.insert(AI_Range_RTD_Ni100SAMA,tr("Ni100SAMA"));
    m_map_RTD.insert(AI_Range_RTD_Ni100DN,tr("Ni100DN"));
    m_map_RTD.insert(AI_Range_RTD_Ni120,tr("Ni120"));
    m_map_RTD.insert(AI_Range_RTD_Pt25,tr("Pt25"));
    m_map_RTD.insert(AI_Range_RTD_Pt50,tr("Pt50"));
    m_map_RTD.insert(AI_Range_RTD_Pt200WEED,tr("Pt200WEED"));
    m_map_RTD.insert(AI_Range_RTD_Cu10G,tr("Cu10G"));
    m_map_RTD.insert(AI_Range_RTD_Cu50G,tr("Cu50G"));
    m_map_RTD.insert(AI_Range_RTD_Cu100G,tr("Cu100G"));
    m_map_RTD.insert(AI_Range_RTD_Pt46G,tr("Pt46G"));
    m_map_RTD.insert(AI_Range_RTD_Pt100G,tr("Pt100G"));

    m_map_DI.insert(AI_Range_DI_Level,tr("Level"));
    m_map_DI.insert(AI_Range_DI_Contact,tr("DI"));

    m_map_Cur0_20mA.insert(AI_Range_Cur0_20mA_Const,tr("0-20mA"));

    m_map_GS4_20mA.insert(AI_Range_GS4_20mA_Type,tr("4-20mA"));

    map = NULL;
    slotResetMenus();
}

void WinAIRangeRangeSelector::initMenu()
{
    QVariant value= ConfigTreeObject::instance()->getConfigObject(AI_DELEGATE_RANGE_RANGE_TYPE)->getData();
    if(value != data){
        data = value;
        if(data.isValid()){
            switch(data.toUInt()){
            case AI_RANGE_Type_Vlot:
                map = &m_map_Volt;
                break;
            case AI_RANGE_Type_GS:
                map = &m_map_GS;
                break;
            case AI_RANGE_Type_TC:
                map = &m_map_TC;
                break;
            case AI_RANGE_Type_RTD:
                map = &m_map_RTD;
                break;
            case AI_RANGE_Type_DI:
                map = &m_map_DI;
                break;
            case AI_RANGE_Type_Cur0_20mA:
                map = &m_map_Cur0_20mA;
                break;
            case AI_RANGE_Type_GS4_20mA:
                map = &m_map_GS4_20mA;
                break;
            default:
                map = NULL;
                break;
            }
        }else{
            map = NULL;
        }
        slotResetMenus();
    }
}

void WinAIRangeRangeSelector::reloadMenu()
{
    if(map){
        QMap<quint32,QString>::iterator item = map->begin();
        for(;item != map->end();item++){
            Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
            menu->setMenuData(item.key());
            addMenu(menu);
        }
    }
}


WinAIRangeCalculation *WinAIRangeCalculation::instance()
{
    static WinAIRangeCalculation  instance;
    return &instance;
}

WinAIRangeCalculation::WinAIRangeCalculation(QWidget *parent)
    :WinChannelSelector(parent)
{
    m_map_RangeCalcu_volt.insert(AI_Range_Calculate_Disabled, "Off");
    m_map_RangeCalcu_volt.insert(AI_Range_Calculate_Delta, "Delta");
    m_map_RangeCalcu_volt.insert(AI_Range_Calculate_Linear, tr("Linear scaling"));
    m_map_RangeCalcu_volt.insert(AI_Range_Calculate_Sqrt, tr("Square root"));

    m_map_RangeCalcu_gs.insert(AI_Range_Calculate_Linear, tr("Linear scaling"));
    m_map_RangeCalcu_gs.insert(AI_Range_Calculate_Sqrt, tr("Square root"));

    m_map_RangeCalcu_other.insert(AI_Range_Calculate_Disabled, "Off");
    m_map_RangeCalcu_other.insert(AI_Range_Calculate_Delta, "Delta");
    m_map_RangeCalcu_other.insert(AI_Range_Calculate_Linear, tr("Linear scaling"));

    slotResetMenus();
}

void WinAIRangeCalculation::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map.begin();
    for(;item != m_map.end();item++) {
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}

void WinAIRangeCalculation::initMenu()
{
    m_map.clear();

    ConfigObject* pObj = ConfigTreeObject::getConfigObject(AI_DELEGATE_RANGE_RANGE_TYPE);
    if(pObj) {
        AI_RANGE_Type type = (AI_RANGE_Type)pObj->getData().toUInt();
        if(AI_RANGE_Type_Disabled == type) {

        } else if(AI_RANGE_Type_Vlot == type) {
            m_map = m_map_RangeCalcu_volt;
        } else if (AI_RANGE_Type_GS == type) {
            m_map = m_map_RangeCalcu_gs;
        } else {
            m_map = m_map_RangeCalcu_other;
        }
    }

    slotResetMenus();
}


WinAIRangeScalePos *WinAIRangeScalePos::instance()
{
    static WinAIRangeScalePos  instance;
    return &instance;
}

WinAIRangeScalePos::WinAIRangeScalePos(QWidget *parent)
    :WinChannelSelector(parent)
{
    m_map_RangeScalePos.insert(0, "0");
    m_map_RangeScalePos.insert(1, "1");
    m_map_RangeScalePos.insert(2, "2");
    m_map_RangeScalePos.insert(3, "3");
    m_map_RangeScalePos.insert(4, "4");
    m_map_RangeScalePos.insert(5, "5");

    slotResetMenus();
}

void WinAIRangeScalePos::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map_RangeScalePos.begin();
    for(;item != m_map_RangeScalePos.end();item++) {
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}

WinAIRangeLowCutOutput *WinAIRangeLowCutOutput::instance()
{
    static WinAIRangeLowCutOutput  instance;
    return &instance;
}

WinAIRangeLowCutOutput::WinAIRangeLowCutOutput(QWidget *parent)
    :WinChannelSelector(parent)
{
    m_map_RangeLowCutOutput.insert(0, tr("Output 0%"));
    m_map_RangeLowCutOutput.insert(1, tr("Output linear"));

    slotResetMenus();
}

void WinAIRangeLowCutOutput::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map_RangeLowCutOutput.begin();
    for(;item != m_map_RangeLowCutOutput.end();item++) {
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}


WinAIRangeRJCMode *WinAIRangeRJCMode::instance()
{
    static WinAIRangeRJCMode  instance;
    return &instance;
}

WinAIRangeRJCMode::WinAIRangeRJCMode(QWidget *parent)
    :WinChannelSelector(parent)
{
        m_map_RangeRJCMode.insert(0, tr("Internal"));
        m_map_RangeRJCMode.insert(1, tr("External"));

        slotResetMenus();
}

void WinAIRangeRJCMode::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map_RangeRJCMode.begin();
    for(;item != m_map_RangeRJCMode.end();item++) {
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}


WinAIRangeBurnoutMode *WinAIRangeBurnoutMode::instance()
{
    static WinAIRangeBurnoutMode  instance;
    return &instance;
}

WinAIRangeBurnoutMode::WinAIRangeBurnoutMode(QWidget *parent)
    :WinChannelSelector(parent)
{
    m_map_RangeBurnoutMode.insert(AI_Range_Burnout_Off, "Off");
    m_map_RangeBurnoutMode.insert(AI_Range_Burnout_Up, tr("Up"));
    m_map_RangeBurnoutMode.insert(AI_Range_Burnout_Down, tr("Down"));

    slotResetMenus();
}

void WinAIRangeBurnoutMode::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map_RangeBurnoutMode.begin();
    for(;item != m_map_RangeBurnoutMode.end();item++) {
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}


WinAIAlarmLvl1Type *WinAIAlarmLvl1Type::instance()
{
    static WinAIAlarmLvl1Type  instance;
    return &instance;
}

WinAIAlarmLvl1Type::WinAIAlarmLvl1Type(QWidget *parent)
    :WinChannelSelector(parent)
{
    m_map_AlarmLvl1Type_Delta.insert(Alarm_Type_H, tr("H : High limit"));
    m_map_AlarmLvl1Type_Delta.insert(Alarm_Type_L, tr("L : Low limit"));
    m_map_AlarmLvl1Type_Delta.insert(Alarm_Type_R, tr("R : High limit on rate-of-change"));
    m_map_AlarmLvl1Type_Delta.insert(Alarm_Type_r, tr("r : Low limit on rate-of-change"));
    m_map_AlarmLvl1Type_Delta.insert(Alarm_Type_T, tr("T : Delay high limit"));
    m_map_AlarmLvl1Type_Delta.insert(Alarm_Type_t, tr("t : Delay low limit"));
    m_map_AlarmLvl1Type_Delta.insert(Alarm_Type_h, tr("h : Difference high limit"));
    m_map_AlarmLvl1Type_Delta.insert(Alarm_Type_l, tr("l : Difference low limit"));

    m_map_AlarmLvl1Type.insert(Alarm_Type_H, tr("H : High limit"));
    m_map_AlarmLvl1Type.insert(Alarm_Type_L, tr("L : Low limit"));
    m_map_AlarmLvl1Type.insert(Alarm_Type_R, tr("R : High limit on rate-of-change"));
    m_map_AlarmLvl1Type.insert(Alarm_Type_r, tr("r : Low limit on rate-of-change"));
    m_map_AlarmLvl1Type.insert(Alarm_Type_T, tr("T : Delay high limit"));
    m_map_AlarmLvl1Type.insert(Alarm_Type_t, tr("t : Delay low limit"));

    slotResetMenus();
}

void WinAIAlarmLvl1Type::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map.begin();
    for(;item != m_map.end();item++) {
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}

void WinAIAlarmLvl1Type::initMenu()
{
    m_map.clear();

    ConfigObject* pObj = ConfigTreeObject::getConfigObject(AI_DELEGATE_RANGE_RANGE_CALCU);
    if(pObj) {
        AI_Range_Calculate calcu = (AI_Range_Calculate)pObj->getData().toUInt();
        if(calcu == AI_Range_Calculate_Delta) {
            m_map = m_map_AlarmLvl1Type_Delta;
        } else {
            m_map = m_map_AlarmLvl1Type;
        }
    }

    slotResetMenus();
}


WinAIAlarmLvl1OutType *WinAIAlarmLvl1OutType::instance()
{
    static WinAIAlarmLvl1OutType  instance;
    return &instance;
}

WinAIAlarmLvl1OutType::WinAIAlarmLvl1OutType(QWidget *parent)
    :WinChannelSelector(parent)
{
    m_map_AlarmLvl1OutType.insert(0, "Off");

    m_map_AlarmLvl1OutType_Both.insert(0, "Off");
    m_map_AlarmLvl1OutType_Both.insert(1, tr("Relay"));
    m_map_AlarmLvl1OutType_Both.insert(2, tr("Internal Switch"));

    m_map_AlarmLvl1OutType_NoRelay.insert(0, "Off");
    m_map_AlarmLvl1OutType_NoRelay.insert(2, tr("Internal Switch"));

    m_map_AlarmLvl1OutType_NoSW.insert(0, "Off");
    m_map_AlarmLvl1OutType_NoSW.insert(1, tr("Relay"));

    slotResetMenus();
}

void WinAIAlarmLvl1OutType::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map.begin();
    for(;item != m_map.end();item++) {
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}

void WinAIAlarmLvl1OutType::initMenu()
{
    m_map.clear();

    bool flag_Alarm_DO = false;
    bool flag_Alarm_SW = false;

    quint8 DO_Alarm;
    if (CfgChannel::instance()->getExistChannelDO().isEmpty()) {
        //do nothing
    } else {
        foreach (QString chanNum, CfgChannel::instance()->getExistChannelDO().keys()) {
            DO_Alarm = ConfigTreeObject::getConfigData(QString(DOCONFIG_RANGE_RANGE_TYPE).arg(chanNum)).toUInt();
            if(0 == DO_Alarm) {
                flag_Alarm_DO = true;
                break;
            }
        }
    }

    quint8 SW_Alarm;
    for(quint8 index=0; index<INTERNAL_SWITCH_NUMBER; ++index) {
        SW_Alarm = ConfigTreeObject::getConfigData(QString(System_Setting_Internal_Type).arg(index)).toUInt();
        if (InterSwitch_Mode_Alarm == SW_Alarm) {
            flag_Alarm_SW = true;
            break;
        }
    }

    if((flag_Alarm_DO) && (flag_Alarm_SW)) {    //0 1 2
        m_map = m_map_AlarmLvl1OutType_Both;
    } else if ((flag_Alarm_DO) && (!flag_Alarm_SW)) {    //0 1
        m_map = m_map_AlarmLvl1OutType_NoSW;
    } else if ((!flag_Alarm_DO) && (flag_Alarm_SW)) {    //0 2
        m_map = m_map_AlarmLvl1OutType_NoRelay;
    } else if ((!flag_Alarm_DO) && (!flag_Alarm_SW)) {  //0
        m_map = m_map_AlarmLvl1OutType;
    }

    slotResetMenus();
}


WinAIAlarmDelayHour *WinAIAlarmDelayHour::instance()
{
    static WinAIAlarmDelayHour  instance;
    return &instance;
}

WinAIAlarmDelayHour::WinAIAlarmDelayHour(QWidget *parent)
    :WinChannelSelector(parent)
{
    for(int i=0;i<25;++i) {
        QString istr = QString("%1").arg(i);
        m_map_AlarmDelayHour.insert(i, istr);
    }

    slotResetMenus();
}

void WinAIAlarmDelayHour::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map_AlarmDelayHour.begin();
    for(;item != m_map_AlarmDelayHour.end();item++) {
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}


WinAIAlarmDelayMin *WinAIAlarmDelayMin::instance()
{
    static WinAIAlarmDelayMin  instance;
    return &instance;
}

WinAIAlarmDelayMin::WinAIAlarmDelayMin(QWidget *parent)
    :WinChannelSelector(parent)
{
    for(int i=0;i<60;++i) {
        QString istr = QString("%1").arg(i);
        m_map_AlarmDelayMin.insert(i, istr);
    }

    slotResetMenus();
}

void WinAIAlarmDelayMin::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map_AlarmDelayMin.begin();
    for(;item != m_map_AlarmDelayMin.end();item++) {
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}


WinAIAlarmDelaySec *WinAIAlarmDelaySec::instance()
{
    static WinAIAlarmDelaySec  instance;
    return &instance;
}

WinAIAlarmDelaySec::WinAIAlarmDelaySec(QWidget *parent)
    :WinChannelSelector(parent)
{
    for(int i=0;i<60;++i) {
        QString istr = QString("%1").arg(i);
        m_map_AlarmDelaySec.insert(i, istr);
    }

    slotResetMenus();
}

void WinAIAlarmDelaySec::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map_AlarmDelaySec.begin();
    for(;item != m_map_AlarmDelaySec.end();item++) {
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}


WinAIDisplayScalePos *WinAIDisplayScalePos::instance()
{
    static WinAIDisplayScalePos  instance;
    return &instance;
}

WinAIDisplayScalePos::WinAIDisplayScalePos(QWidget *parent)
    :WinChannelSelector(parent)
{
    m_map_DisplayScalePos.insert(0, tr("Off"));
    for(int i=1;i<11;++i) {
        QString istr = QString("%1").arg(i);
        m_map_DisplayScalePos.insert(i, istr);
    }

    slotResetMenus();
}

void WinAIDisplayScalePos::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map_DisplayScalePos.begin();
    for(;item != m_map_DisplayScalePos.end();item++) {
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}


WinAIDisplayScaleDiv *WinAIDisplayScaleDiv::instance()
{
    static WinAIDisplayScaleDiv  instance;
    return &instance;
}

WinAIDisplayScaleDiv::WinAIDisplayScaleDiv(QWidget *parent)
    :WinChannelSelector(parent)
{
    for(int i=4;i<13;++i) {
        QString istr = QString("%1").arg(i);
        m_map_DisplayScaleDiv.insert(i, istr);
    }
    m_map_DisplayScaleDiv.insert(13, "C10");

    slotResetMenus();
}

void WinAIDisplayScaleDiv::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map_DisplayScaleDiv.begin();
    for(;item != m_map_DisplayScaleDiv.end();item++) {
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}


WinAIDisplayBarPos *WinAIDisplayBarPos::instance()
{
    static WinAIDisplayBarPos  instance;
    return &instance;
}

WinAIDisplayBarPos::WinAIDisplayBarPos(QWidget *parent)
    :WinChannelSelector(parent)
{
    m_map_DisplayBarPos.insert(0, tr("Lower"));
    m_map_DisplayBarPos.insert(1, tr("Center"));
    m_map_DisplayBarPos.insert(2, tr("Upper"));

    slotResetMenus();
}

void WinAIDisplayBarPos::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map_DisplayBarPos.begin();
    for(;item != m_map_DisplayBarPos.end();item++) {
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}


WinAIDisplayBarDiv *WinAIDisplayBarDiv::instance()
{
    static WinAIDisplayBarDiv  instance;
    return &instance;
}

WinAIDisplayBarDiv::WinAIDisplayBarDiv(QWidget *parent)
    :WinChannelSelector(parent)
{
    for(int i=4;i<13;++i) {
        QString istr = QString("%1").arg(i);
        m_map_DisplayBarDiv.insert(i, istr);
    }

    slotResetMenus();
}

void WinAIDisplayBarDiv::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map_DisplayBarDiv.begin();
    for(;item != m_map_DisplayBarDiv.end();item++) {
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}


WinAIDisplayCsbandMode *WinAIDisplayCsbandMode::instance()
{
    static WinAIDisplayCsbandMode  instance;
    return &instance;
}

WinAIDisplayCsbandMode::WinAIDisplayCsbandMode(QWidget *parent)
    :WinChannelSelector(parent)
{
    m_map_DisplayCsbandMode.insert(0, tr("Off"));
    m_map_DisplayCsbandMode.insert(1, tr("In "));
    m_map_DisplayCsbandMode.insert(2, tr("Out"));

    m_map_Off.insert(0, tr("Off"));

    slotResetMenus();
}

void WinAIDisplayCsbandMode::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map.begin();
    for(;item != m_map.end();item++) {
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}

void WinAIDisplayCsbandMode::initMenu()
{
    m_map.clear();
    ConfigObject* pObj;

    switch (((MenuNormal*)p_pMenu)->getMenuID().toUInt()) {
        case ID_Comm_DISP_ColorScaleBand_Mode:
            pObj = ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Range_Range_OnOrOff);
            break;
        case ID_AI_DISP_ColorScaleBand_Mode:
            pObj = ConfigTreeObject::getConfigObject(AI_DELEGATE_RANGE_RANGE_TYPE);
            break;
        case ID_Math_DISP_ColorScaleBand_Mode:
            pObj = ConfigTreeObject::getConfigObject(MATH_DELEGATE_CALCULEXP_RANGE_SWITCH);
            break;
        default:
            break;
    }

    if(pObj) {
        quint32 value = pObj->getData().toUInt();
        switch (value) {
        case 0:
            m_map = m_map_Off;
            break;
        case 1:
            m_map = m_map_DisplayCsbandMode;
            break;
        default:
            m_map = m_map_DisplayCsbandMode;
            break;
        }
    } else {
        m_map = m_map_DisplayCsbandMode;
    }

    slotResetMenus();
}


WinAIDisplayAlarmType *WinAIDisplayAlarmType::instance()
{
    static WinAIDisplayAlarmType  instance;
    return &instance;
}

WinAIDisplayAlarmType::WinAIDisplayAlarmType(QWidget *parent)
    :WinChannelSelector(parent)
{
    m_map_DisplayAlarmType.insert(0, tr("Alarm"));
    m_map_DisplayAlarmType.insert(1, tr("Fixed"));

    slotResetMenus();
}

void WinAIDisplayAlarmType::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map_DisplayAlarmType.begin();
    for(;item != m_map_DisplayAlarmType.end();item++) {
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}


WinAICaliCorrectMode *WinAICaliCorrectMode::instance()
{
    static WinAICaliCorrectMode  instance;
    return &instance;
}

WinAICaliCorrectMode::WinAICaliCorrectMode(QWidget *parent)
    :WinChannelSelector(parent)
{
    m_map_CaliCorrectMode.insert(0, "Off");
    m_map_CaliCorrectMode.insert(1, tr("Linearizer Approximation"));
    m_map_CaliCorrectMode.insert(2, tr("Linearizer Bias"));

    m_map_CaliCorrectMode_Off.insert(0, "Off");

    slotResetMenus();
}

void WinAICaliCorrectMode::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map.begin();
    for(;item != m_map.end();item++) {
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}

void WinAICaliCorrectMode::initMenu()
{
    m_map.clear();

    ConfigObject* pObj = ConfigTreeObject::getConfigObject(AI_DELEGATE_RANGE_RANGE_TYPE);
    if(pObj) {
        AI_RANGE_Type type = (AI_RANGE_Type)pObj->getData().toUInt();
        if((type == AI_RANGE_Type_Disabled) || (type == AI_RANGE_Type_DI)) {
            m_map = m_map_CaliCorrectMode_Off;
        } else {
            m_map = m_map_CaliCorrectMode;
        }
    }

    slotResetMenus();
}


WinAICaliCorrectCount *WinAICaliCorrectCount::instance()
{
    static WinAICaliCorrectCount  instance;
    return &instance;
}

WinAICaliCorrectCount::WinAICaliCorrectCount(QWidget *parent)
    :WinChannelSelector(parent)
{
    for(int i=2;i<13;++i) {
        QString istr = QString("%1").arg(i);
        m_map_CaliCorrectCount.insert(i, istr);
    }

    slotResetMenus();
}

void WinAICaliCorrectCount::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map_CaliCorrectCount.begin();
    for(;item != m_map_CaliCorrectCount.end();item++) {
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}


//执行输入测量按钮对话框
WinCfgAICaliDialog::WinCfgAICaliDialog()
    :WinDialog()
{
    QFrame *p_mainFrame = new QFrame(this);
    p_mainFrame->setObjectName("BackgrounFrame");
    p_mainFrame->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);

    WinStackedLayout *layout = new WinStackedLayout(this);
    layout->addWidget(p_mainFrame,Qt::AlignCenter);
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(0);

    QPushButton *closeBtn = new QPushButton(p_mainFrame);
    closeBtn->setObjectName("CloseButton");
    closeBtn->setFocusPolicy(Qt::NoFocus);
    connect(closeBtn, SIGNAL(clicked()),this,SLOT(close()));

    QLabel *titleLabel = new QLabel(p_mainFrame);
    titleLabel->setObjectName("TitleLabel");
    titleLabel->setText(tr("Comfirmation"));

    QHBoxLayout *titleLayout = new QHBoxLayout();
    titleLayout->setContentsMargins(12,0,12,0);
    titleLayout->setSpacing(0);
    titleLayout->addWidget(titleLabel,0,Qt::AlignVCenter);
    titleLayout->addSpacerItem(new QSpacerItem(40,59,QSizePolicy::Expanding,QSizePolicy::Fixed));
    titleLayout->addWidget(closeBtn,0,Qt::AlignVCenter);

    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout->setContentsMargins(24,0,24,0);
    btnLayout->setSpacing(0);

    QPushButton *okBtn = new QPushButton();
    okBtn->setObjectName("OkButton");
    okBtn->setFocusPolicy(Qt::NoFocus);
    okBtn->setText(tr("OK"));
    connect(okBtn, SIGNAL(clicked()),this,SLOT(accept()));

    QPushButton *cancelBtn = new QPushButton();
    cancelBtn->setObjectName("CancelButton");
    cancelBtn->setFocusPolicy(Qt::NoFocus);
    cancelBtn->setText(tr("Cancel"));
    connect(cancelBtn, SIGNAL(clicked()),this,SLOT(reject()));

    btnLayout->addWidget(cancelBtn,0,Qt::AlignVCenter);
    btnLayout->addStretch();
    btnLayout->addWidget(okBtn,0,Qt::AlignVCenter);

    QVBoxLayout *contextLayout = new QVBoxLayout(p_mainFrame);
    contextLayout->setContentsMargins(0,0,0,20);
    contextLayout->setSpacing(0);
    QLabel *txtLabel = new QLabel(p_mainFrame);
    txtLabel->setObjectName("TextLabel");
    txtLabel->setText(tr("Perform the input measurement ? \nInput range must be saved prior to this."));
    contextLayout->addLayout(titleLayout);
    contextLayout->addWidget(txtLabel,0,Qt::AlignVCenter | Qt::AlignLeft);
    contextLayout->addLayout(btnLayout);

    p_mainFrame->setStyleSheet("QFrame#BackgrounFrame{\
                        min-width:722px;\
                        max-width:722px;\
                        border: 1px solid #D3D3D3;\
                        background-color:rgb(255,255,255);\
                        background-image:url(:/relay/Image/relay/title_bg.png);\
                        background-repeat:repeat-x;\
                        background-position: top left;\
                        background-origin: content;\
                        border-top-left-radius:5px;\
                        border-top-right-radius:5px;\
                        }\
                        QPushButton#CloseButton{\
                        min-width:34px;\
                        max-width:34px;\
                        width:34px;\
                        height:34px;\
                        background-image:url(:/relay/Image/relay/close_normal.png);\
                        background-repeat: no-repeat;\
                        background-position:center;\
                        border:none;\
                        }\
                        QPushButton#CloseButton:pressed{\
                        background-image:url(:/relay/Image/relay/close_pressed.png);\
                        }\
                        QLabel#TitleLabel{\
                        color:rgb(255,255,255);\
                        font-size:32px;\
                        }\
                        QLabel#TextLabel{\
                        color:black;\
                        margin-top:30px;\
                        margin-bottom:30px;\
                        margin-left:20px;\
                        font-size:28px;\
                        }\
                        QPushButton{\
                        width:124px;\
                        height:52px;\
                        font-size:24px;\
                        color:white;\
                        background-color:none;\
                        background-image:url(:/relay/Image/relay/button_normal.png);\
                        background-repeat:no-repeat;\
                        background-position:center;\
                        border:none;\
                        }\
                        QPushButton:pressed{\
                        background-image:url(:/relay/Image/relay/button_pressed.png);\
                        }");
}
