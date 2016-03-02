/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：windoconfig.cpp
 * 概    要：DO-channel settings下存在item单例的所有配置项目，构建并展开选项菜单list。
 *
 * 当前版本：V1.0.0
 * 作    者：邢 俊 杰
 * 完成日期：2015-10-28
 *
 * 修改版本：
 * 修改说明：
 * 修改作者：
 *
 ********************************************************************/
#include "windoconfig.h"

WinDORangeRangeType *WinDORangeRangeType::instance()
{
    static WinDORangeRangeType  instance;
    return &instance;
}

WinDORangeRangeType::WinDORangeRangeType(QWidget *parent)
    :WinChannelSelector(parent)
{
    m_map_RangeRangeType.insert(0, tr("Alarm"));
    m_map_RangeRangeType.insert(1, tr("Manual"));

    slotResetMenus();
}

void WinDORangeRangeType::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map_RangeRangeType.begin();
    for(;item != m_map_RangeRangeType.end();item++) {
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}


WinDORangeRangeRange *WinDORangeRangeRange::instance()
{
    static WinDORangeRangeRange  instance;
    return &instance;
}

WinDORangeRangeRange::WinDORangeRangeRange(QWidget *parent)
    :WinChannelSelector(parent)
{
    m_map_RangeRangeRange.insert(0, "0");
    m_map_RangeRangeRange.insert(1, "1");

    slotResetMenus();
}

void WinDORangeRangeRange::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map_RangeRangeRange.begin();
    for(;item != m_map_RangeRangeRange.end();item++) {
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}


WinDORangeActionEnergize *WinDORangeActionEnergize::instance()
{
    static WinDORangeActionEnergize  instance;
    return &instance;
}

WinDORangeActionEnergize::WinDORangeActionEnergize(QWidget *parent)
    :WinChannelSelector(parent)
{
    m_map_RangeActionEnergize.insert(0, tr("Energize"));
    m_map_RangeActionEnergize.insert(1, tr("De-energize"));

    slotResetMenus();
}

void WinDORangeActionEnergize::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map_RangeActionEnergize.begin();
    for(;item != m_map_RangeActionEnergize.end();item++) {
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}


WinDORangeActionAction *WinDORangeActionAction::instance()
{
    static WinDORangeActionAction  instance;
    return &instance;
}

WinDORangeActionAction::WinDORangeActionAction(QWidget *parent)
    :WinChannelSelector(parent)
{
    m_map_RangeActionAction.insert(0, tr("Or"));
    m_map_RangeActionAction.insert(1, tr("And"));
    m_map_RangeActionAction.insert(2, tr("Reflash"));

    slotResetMenus();
}

void WinDORangeActionAction::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map_RangeActionAction.begin();
    for(;item != m_map_RangeActionAction.end();item++) {
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}


WinDORangeActionHold *WinDORangeActionHold::instance()
{
    static WinDORangeActionHold  instance;
    return &instance;
}

WinDORangeActionHold::WinDORangeActionHold(QWidget *parent)
    :WinChannelSelector(parent)
{
    m_map_RangeActionHold.insert(0, tr("Hold"));
    m_map_RangeActionHold.insert(1, tr("Nonhold"));

    slotResetMenus();
}

void WinDORangeActionHold::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map_RangeActionHold.begin();
    for(;item != m_map_RangeActionHold.end();item++) {
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}


WinDORangeActionACK *WinDORangeActionACK::instance()
{
    static WinDORangeActionACK  instance;
    return &instance;
}

WinDORangeActionACK::WinDORangeActionACK(QWidget *parent):
    WinChannelSelector(parent)
{
    m_map_RangeActionACK.insert(0, tr("Normal"));
    m_map_RangeActionACK.insert(1, tr("Reset"));

    m_map_Reset.insert(1, tr("Reset"));

    slotResetMenus();
}

void WinDORangeActionACK::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map.begin();
    for(;item != m_map.end();item++) {
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}

void WinDORangeActionACK::initMenu()
{
    m_map.clear();
    QVariant value = ConfigTreeObject::getConfigObject(System_Delegate_Alarm_AlarmACK)->getData();

    switch (value.toInt()) {
    case 0:
        m_map = m_map_RangeActionACK;
        break;
    case 1:
        m_map = m_map_Reset;
        break;
    default:
        break;
    }

    slotResetMenus();
}


WinDORangeActionInterval *WinDORangeActionInterval::instance()
{
    static WinDORangeActionInterval  instance;
    return &instance;
}

WinDORangeActionInterval::WinDORangeActionInterval(QWidget *parent)
    :WinChannelSelector(parent)
{
    m_map_RangeActionInterval.insert(0, tr("500 ms"));
    m_map_RangeActionInterval.insert(1, tr("1 s"));
    m_map_RangeActionInterval.insert(2, tr("2 s"));

    slotResetMenus();
}

void WinDORangeActionInterval::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map_RangeActionInterval.begin();
    for(;item != m_map_RangeActionInterval.end();item++) {
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}
