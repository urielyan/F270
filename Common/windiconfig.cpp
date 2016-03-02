/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：windiconfig.cpp
 * 概    要：DI-channel settings下存在item单例的所有配置项目，构建并展开选项菜单list。
 *
 * 当前版本：V1.0.0
 * 作    者：邢 俊 杰
 * 完成日期：2015-11-03
 *
 * 修改版本：
 * 修改说明：
 * 修改作者：
 *
 ********************************************************************/
#include "windiconfig.h"


WinDIRangeRangeType *WinDIRangeRangeType::instance()
{
    static WinDIRangeRangeType  instance;
    return &instance;
}

WinDIRangeRangeType::WinDIRangeRangeType(QWidget *parent)
    :WinChannelSelector(parent)
{
    m_map_RangeRangeType.insert(DI_Range_Type_Disabled, tr("Skip"));
    m_map_RangeRangeType.insert(DI_Range_Type_DI, tr("DI"));
    m_map_RangeRangeType.insert(DI_Range_Type_Pluse, tr("Pulse"));

    m_map_RangeRangeType_noPluse.insert(DI_Range_Type_Disabled, tr("Skip"));
    m_map_RangeRangeType_noPluse.insert(DI_Range_Type_DI, tr("DI"));

    slotResetMenus();
}

void WinDIRangeRangeType::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map.begin();
    for(;item != m_map.end();item++) {
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}

void WinDIRangeRangeType::initMenu()
{
    m_map.clear();
    //此处有问题，只有一个模块能被设置为远程输入 TODO
    ConfigObject* pObj = ConfigTreeObject::getConfigObject(MEASURE_DELEGATE_U_M_DIMODE_MODE);
    if(pObj) {
        DI_OpeMode mode = (DI_OpeMode)pObj->getData().toUInt();

        if(DI_OpeMode_Remote == mode) {
            m_map = m_map_RangeRangeType_noPluse;
        } else {
            m_map = m_map_RangeRangeType;
        }
    }

    slotResetMenus();
}


WinDIRangeRangeRange *WinDIRangeRangeRange::instance()
{
    static WinDIRangeRangeRange  instance;
    return &instance;
}

WinDIRangeRangeRange::WinDIRangeRangeRange(QWidget *parent)
    :WinChannelSelector(parent)
{
    m_map_RangeRangeRange.insert(0, "0");
    m_map_RangeRangeRange.insert(1, "1");

    slotResetMenus();
}

void WinDIRangeRangeRange::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map_RangeRangeRange.begin();
    for(;item != m_map_RangeRangeRange.end();item++) {
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}


WinDIRangeCalculation *WinDIRangeCalculation::instance()
{
    static WinDIRangeCalculation  instance;
    return &instance;
}

WinDIRangeCalculation::WinDIRangeCalculation(QWidget *parent)
    :WinChannelSelector(parent)
{
    m_map_RangeCalcu.insert(DI_Range_Calculate_Disabled, "Off");
    m_map_RangeCalcu.insert(DI_Range_Calculate_Delta, "Delta");
    m_map_RangeCalcu.insert(DI_Range_Calculate_Linear, tr("Linear scaling"));

    m_map_RangeCalcu_off.insert(DI_Range_Calculate_Disabled, "Off");

    slotResetMenus();
}

void WinDIRangeCalculation::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map.begin();
    for(;item != m_map.end();item++) {
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}

void WinDIRangeCalculation::initMenu()
{
    m_map.clear();

    ConfigObject* pObj = ConfigTreeObject::getConfigObject(DI_DELEGATE_RANG_RANGE_TYPE);
    if(pObj) {
        DI_Range_Type type = (DI_Range_Type)pObj->getData().toUInt();

        if(DI_Range_Type_Disabled == type) {

        } else if(DI_Range_Type_Pluse == type) {
            m_map = m_map_RangeCalcu_off;
        } else {
            m_map = m_map_RangeCalcu;
        }
    }

    slotResetMenus();
}


WinDIAlarmLvl1Type *WinDIAlarmLvl1Type::instance()
{
    static WinDIAlarmLvl1Type  instance;
    return &instance;
}

WinDIAlarmLvl1Type::WinDIAlarmLvl1Type(QWidget *parent)
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

void WinDIAlarmLvl1Type::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map.begin();
    for(;item != m_map.end();item++) {
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}

void WinDIAlarmLvl1Type::initMenu()
{
    m_map.clear();

    ConfigObject* pObj = ConfigTreeObject::getConfigObject(DI_DELEGATE_RANG_RANGE_CALCU);
    if(pObj) {
        DI_Range_Calculate calcu = (DI_Range_Calculate)pObj->getData().toUInt();
        if(calcu == DI_Range_Calculate_Delta) {
            m_map = m_map_AlarmLvl1Type_Delta;
        } else {
            m_map = m_map_AlarmLvl1Type;
        }
    }

    slotResetMenus();
}
