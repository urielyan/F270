#include "winnetworkconfig.h"
#include "../GlobalData/cfgchannel.h"

/**************modbus client************/
WinModbusClientInterval *WinModbusClientInterval::instance()
{
    static WinModbusClientInterval  instance;
    return &instance;
}

WinModbusClientInterval::WinModbusClientInterval(QWidget */*parent*/)
{
    m_map_interval.insert(NetworkModbusInterval_100ms,tr("100ms"));
    m_map_interval.insert(NetworkModbusInterval_200ms,tr("200ms"));
    m_map_interval.insert(NetworkModbusInterval_500ms,tr("500ms"));
    m_map_interval.insert(NetworkModbusInterval_1s,tr("1s"));
    m_map_interval.insert(NetworkModbusInterval_2s,tr("2s"));
    m_map_interval.insert(NetworkModbusInterval_5s,tr("5s"));
    m_map_interval.insert(NetworkModbusInterval_10s,tr("10s"));
    m_map_interval.insert(NetworkModbusInterval_20s,tr("20s"));
    m_map_interval.insert(NetworkModbusInterval_30s,tr("30s"));
    m_map_interval.insert(NetworkModbusInterval_1min,tr("1min"));
    slotResetMenus();
}

void WinModbusClientInterval::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map_interval.begin();
    for(;item != m_map_interval.end();item++){
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}

WinModbusClientConnectTime *WinModbusClientConnectTime::instance()
{
    static WinModbusClientConnectTime  instance;
    return &instance;
}

WinModbusClientConnectTime::WinModbusClientConnectTime(QWidget */*parent*/)
{
    for (int i = 1; i <= 10; ++i) {
        QString str = QString("%1s").arg(i);
        QByteArray ba = str.toLatin1();
        m_map_connectTime.insert(i, tr(ba.data()));
    }
    slotResetMenus();
}

void WinModbusClientConnectTime::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map_connectTime.begin();
    for(;item != m_map_connectTime.end();item++){
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}


WinModbusClientWaitTime *WinModbusClientWaitTime::instance()
{
    static WinModbusClientWaitTime  instance;
    return &instance;
}

WinModbusClientWaitTime::WinModbusClientWaitTime(QWidget */*parent*/)
{
    m_map_waitTime.insert(NetworkModbusWaitTime_off, tr("Off"));
    m_map_waitTime.insert(NetworkModbusWaitTime_5s, tr("5s"));
    m_map_waitTime.insert(NetworkModbusWaitTime_10s, tr("10s"));
    m_map_waitTime.insert(NetworkModbusWaitTime_30s, tr("30s"));
    m_map_waitTime.insert(NetworkModbusWaitTime_1min, tr("1min"));
    m_map_waitTime.insert(NetworkModbusWaitTime_2min, tr("2min"));
    m_map_waitTime.insert(NetworkModbusWaitTime_5min, tr("5min"));
    slotResetMenus();
}

void WinModbusClientWaitTime::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map_waitTime.begin();
    for(;item != m_map_waitTime.end();item++){
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}


WinModbusServerSettingNumber *WinModbusServerSettingNumber::instance()
{
    static WinModbusServerSettingNumber  instance;
    return &instance;
}

WinModbusServerSettingNumber::WinModbusServerSettingNumber(QWidget */*parent*/)
{
    for (int i = 0; i < 32 ;++i) {
        QString str = QString("%1").arg(i+1);
        QByteArray ba = str.toLatin1();
        m_map_number.insert(i, tr(ba.data()));
    }
    slotResetMenus();
}

void WinModbusServerSettingNumber::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map_number.begin();
    for(;item != m_map_number.end();item++){
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}

WinModbusServerDelayRes *WinModbusServerDelayRes::instance()
{
    static WinModbusServerDelayRes  instance;
    return &instance;
}

WinModbusServerDelayRes::WinModbusServerDelayRes(QWidget */*parent*/)
{
    m_map_DelayRes.insert(0,  tr("Off"));
    m_map_DelayRes.insert(10, tr("10ms"));
    m_map_DelayRes.insert(20, tr("20ms"));
    m_map_DelayRes.insert(50, tr("50ms"));

    slotResetMenus();
}

void WinModbusServerDelayRes::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map_DelayRes.begin();
    for(;item != m_map_DelayRes.end();item++){
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}

/******************SMTP******************************/
WinSMTPAuthentication *WinSMTPAuthentication::instance()
{
    static WinSMTPAuthentication  instance;
    return &instance;
}

WinSMTPAuthentication::WinSMTPAuthentication(QWidget */*parent*/)
{
    m_map_authentication.insert(SMTP_AUTH_OFF, tr("Off"));
    m_map_authentication.insert(SMTP_AUTH_SMTP, tr("SMTP Authentication"));
    m_map_authentication.insert(SMTP_AUTH_POP, tr("POP before SMTP"));
    m_map_authentication.insert(SMTP_AUTH_APOP, tr("APOP"));
    slotResetMenus();
}

void WinSMTPAuthentication::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map_authentication.begin();
    for(;item != m_map_authentication.end();item++){
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}

/******************E-mail******************************/

WinEMailAlarmNotification *WinEMailAlarmNotification::instance()
{
    static WinEMailAlarmNotification  instance;
    return &instance;
}

WinEMailAlarmNotification::WinEMailAlarmNotification(QWidget */*parent*/)
{
    m_map_alarmNotification.insert(EMAILAlarmNotify_OFF, tr("Off"));
    m_map_alarmNotification.insert(EMAILAlarmNotify_R1, tr("Recipient 1"));
    m_map_alarmNotification.insert(EMAILAlarmNotify_R2, tr("Recipient 2"));
    m_map_alarmNotification.insert(EMAILAlarmNotify_R1_R2, tr("Recipient 1 && Recipient 2"));
    slotResetMenus();
}

void WinEMailAlarmNotification::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map_alarmNotification.begin();
    for(;item != m_map_alarmNotification.end();item++){
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}


WinEMailChannelSet *WinEMailChannelSet::instance()
{
    static WinEMailChannelSet  instance;
    return &instance;
}

WinEMailChannelSet::WinEMailChannelSet(QWidget */*parent*/)
{
    m_map_channelSet.insert(EMAILSET_UpTo50, tr("Up to 50 channels"));
    m_map_channelSet.insert(EMAILSET_IO, tr("I/O channels"));
    m_map_channelSet.insert(EMAILSET_Math, tr("Math channels"));
    m_map_channelSet.insert(EMAILSET_Comm, tr("Communication channels"));
    slotResetMenus();
}

void WinEMailChannelSet::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map_channelSet.begin();
    for(;item != m_map_channelSet.end();item++){
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}


WinEMailInteral *WinEMailInteral::instance()
{
    static WinEMailInteral  instance;
    return &instance;
}

WinEMailInteral::WinEMailInteral(QWidget */*parent*/)
{
    m_map_interal.insert(EMAILInteral_1h,tr("1h"));
    m_map_interal.insert(EMAILInteral_2h,tr("2h"));
    m_map_interal.insert(EMAILInteral_3h,tr("3h"));
    m_map_interal.insert(EMAILInteral_4h,tr("4h"));
    m_map_interal.insert(EMAILInteral_6h,tr("6h"));
    m_map_interal.insert(EMAILInteral_8h,tr("8h"));
    m_map_interal.insert(EMAILInteral_12h,tr("12h"));
    m_map_interal.insert(EMAILInteral_24h,tr("24h"));
    slotResetMenus();
}

void WinEMailInteral::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map_interal.begin();
    for(;item != m_map_interal.end();item++){
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}


WinEMailTimeHour *WinEMailTimeHour::instance()
{
    static WinEMailTimeHour  instance;
    return &instance;
}

WinEMailTimeHour::WinEMailTimeHour(QWidget */*parent*/)
{
    for (int i = 0; i <= 23 ;++i) {
        QString str = QString("%1").arg(i);
        QByteArray ba = str.toLatin1();
        m_map_hour.insert(i,tr(ba.data()));
    }
    slotResetMenus();
}

void WinEMailTimeHour::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map_hour.begin();
    for(;item != m_map_hour.end();item++){
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}


WinEMailTimeMinute *WinEMailTimeMinute::instance()
{
    static WinEMailTimeMinute  instance;
    return &instance;
}

WinEMailTimeMinute::WinEMailTimeMinute(QWidget */*parent*/)
{
    for (int i = 0; i <= 59 ;++i) {
        QString str = QString("%1").arg(i);
        QByteArray ba = str.toLatin1();
        m_map_minute.insert(i,tr(ba.data()));
    }
    slotResetMenus();
}

void WinEMailTimeMinute::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map_minute.begin();
    for(;item != m_map_minute.end();item++){
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}

/**********************************SNTP****************************/
WinSNTPInterval *WinSNTPInterval::instance()
{
    static WinSNTPInterval  instance;
    return &instance;
}

WinSNTPInterval::WinSNTPInterval(QWidget */*parent*/)
{
    m_map_interval.insert(SNTPInterval_6h, tr("6h"));
    m_map_interval.insert(SNTPInterval12h, tr("12h"));
    m_map_interval.insert(SNTPInterval24h, tr("24h"));
    slotResetMenus();
}

void WinSNTPInterval::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map_interval.begin();
    for(;item != m_map_interval.end();item++){
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}


WinSNTPTimeout *WinSNTPTimeout::instance()
{
    static WinSNTPTimeout  instance;
    return &instance;
}

WinSNTPTimeout::WinSNTPTimeout(QWidget */*parent*/)
{
    m_map_timeout.insert(SNTPTimeout_10s, tr("10s"));
    m_map_timeout.insert(SNTPTimeout_30s, tr("30s"));
    m_map_timeout.insert(SNTPTimeout_90s, tr("90s"));
    slotResetMenus();
}

void WinSNTPTimeout::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map_timeout.begin();
    for(;item != m_map_timeout.end();item++){
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}


WinModbusCommandNumber *WinModbusCommandNumber::instance()
{
    static WinModbusCommandNumber  instance;
    return &instance;
}

WinModbusCommandNumber::WinModbusCommandNumber(QWidget */*parent*/)
{
    for (int i = 0; i < 100 ;++i) {
        QString str = QString("%1").arg(i+1);
        QByteArray ba = str.toLatin1();
        m_map_number.insert(i, tr(ba.data()));
    }
    slotResetMenus();
}

void WinModbusCommandNumber::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map_number.begin();
    for(;item != m_map_number.end();item++){
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}


WinModbusCommandType *WinModbusCommandType::instance()
{
    static WinModbusCommandType  instance;
    return &instance;
}

WinModbusCommandType::WinModbusCommandType(QWidget */*parent*/)
{
    m_map_Type.insert(NetworkModbusCommandType_Off, tr("Off"));
    m_map_Type.insert(NetworkModbusCommandType_Read, tr("Read"));
    m_map_Type.insert(NetworkModbusCommandType_Write, tr("Write"));
    slotResetMenus();
}

void WinModbusCommandType::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map_Type.begin();
    for(;item != m_map_Type.end();item++){
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}


WinModbusCommandDataType *WinModbusCommandDataType::instance()
{
    static WinModbusCommandDataType  instance;
    return &instance;
}

WinModbusCommandDataType::WinModbusCommandDataType(QWidget */*parent*/)
{
    m_map_Type.insert(NetworkModbusCommandDataType_Bit, tr("Bit"));
    m_map_Type.insert(NetworkModbusCommandDataType_Int16, tr("Int16"));
    m_map_Type.insert(NetworkModbusCommandDataType_UInt16, tr("UInt16"));
    m_map_Type.insert(NetworkModbusCommandDataType_Int32_B, tr("Int32_B"));
    m_map_Type.insert(NetworkModbusCommandDataType_Int32_L, tr("Int32_L"));
    m_map_Type.insert(NetworkModbusCommandDataType_UInt32_B, tr("UInt32_B"));
    m_map_Type.insert(NetworkModbusCommandDataType_UInt32_L, tr("UInt32_L"));
    m_map_Type.insert(NetworkModbusCommandDataType_Float_B, tr("Float_B"));
    m_map_Type.insert(NetworkModbusCommandDataType_Float_L, tr("Float_L"));
    slotResetMenus();
}

void WinModbusCommandDataType::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map_Type.begin();
    for(;item != m_map_Type.end();item++){
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}


WinModbusCommandChanelType *WinModbusCommandChanelType::instance()
{
    static WinModbusCommandChanelType  instance;
    return &instance;
}

WinModbusCommandChanelType::WinModbusCommandChanelType(QWidget */*parent*/)
{
    m_map_TypeW.insert(NetworkModbusCommandChanelType_IO, tr("I/O channel"));
    m_map_TypeW.insert(NetworkModbusCommandChanelType_COMM, tr("Communication channel"));
    m_map_TypeW.insert(NetworkModbusCommandChanelType_MATH, tr("Math channel"));

    m_map_TypeR.insert(NetworkModbusCommandChanelType_COMM, tr("Communication channel"));

    m_map_TypeHide.insert(NetworkModbusCommandChanelType_COMM, tr("Communication channel"));
    m_map_TypeHide.insert(NetworkModbusCommandChanelType_MATH, tr("Math channel"));

    slotResetMenus();
}

void WinModbusCommandChanelType::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map_Type.begin();
    for(;item != m_map_Type.end();item++){
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}

void WinModbusCommandChanelType::initMenu()
{
    QVariant value= ConfigTreeObject::getConfigObject(NETWORK_MODBUS_Deledate_COMMAND_TYPE)->getData();
    if(value != data){
        data = value;
        if(data.isValid()){
            switch(data.toUInt()){
            case NetworkModbusCommandType_Read:
                m_map_Type = m_map_TypeR;
                break;
            case NetworkModbusCommandType_Write:
                if (CfgChannel::instance()->getList_BoardIO().isEmpty())
                    m_map_Type = m_map_TypeHide;
                else
                    m_map_Type = m_map_TypeW;
                break;
            default:
                m_map_Type = m_map_TypeR;
                break;
            }
        }else{
            m_map_Type = m_map_TypeR;
        }
        slotResetMenus();
    }
}
