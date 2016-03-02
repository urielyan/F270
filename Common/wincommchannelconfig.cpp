#include "wincommchannelconfig.h"


WinCommChannelChoose *WinCommChannelChoose::instance()
{
    static WinCommChannelChoose  instance;
    return &instance;
}

WinCommChannelChoose::WinCommChannelChoose(QWidget */*parent*/)
{
    QString str;
    for (int i = 0; i < 300; ++i) {
        str = QString("C%1").arg(i+1,3,10,QChar('0'));
        QByteArray ba = str.toLatin1();
        m_map.insert(i, tr(ba.data()));
    }
    slotResetMenus();
}

void WinCommChannelChoose::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map.begin();
    for(;item != m_map.end();item++){
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}


WinCommChannelDecimalPlace *WinCommChannelDecimalPlace::instance()
{
    static WinCommChannelDecimalPlace  instance;
    return &instance;
}

WinCommChannelDecimalPlace::WinCommChannelDecimalPlace(QWidget */*parent*/)
{
    for (int i = 0; i <= 5; ++i) {
        m_map.insert(i, QString("%1").arg(i));
    }
    slotResetMenus();
}

void WinCommChannelDecimalPlace::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map.begin();
    for(;item != m_map.end();item++){
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}


WinCommChannelAlarmType *WinCommChannelAlarmType::instance()
{
    static WinCommChannelAlarmType  instance;
    return &instance;
}

WinCommChannelAlarmType::WinCommChannelAlarmType(QWidget */*parent*/)
{

    m_map.insert(WinCommChannelAlarmType_H,tr("H: High limit"));
    m_map.insert(WinCommChannelAlarmType_L,tr("L: Low limit"));
    m_map.insert(WinCommChannelAlarmType_T,tr("T: Delay high limit"));
    m_map.insert(WinCommChannelAlarmType_t,tr("t: Delay low limit"));
    slotResetMenus();
}

void WinCommChannelAlarmType::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map.begin();
    for(;item != m_map.end();item++){
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}


WinCommChannelValuePower *WinCommChannelValuePower::instance()
{
    static WinCommChannelValuePower  instance;
    return &instance;
}

WinCommChannelValuePower::WinCommChannelValuePower(QWidget */*parent*/)
{
    m_map.insert(CommChannelValuePower_Last,tr("Last value"));
    m_map.insert(CommChannelValuePower_Preset,tr("Preset value"));
    slotResetMenus();
}

void WinCommChannelValuePower::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map.begin();
    for(;item != m_map.end();item++){
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}
