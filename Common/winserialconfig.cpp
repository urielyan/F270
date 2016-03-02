#include "winserialconfig.h"

WinSerialBasicReceiveFunction *WinSerialBasicReceiveFunction::instance()
{
    static WinSerialBasicReceiveFunction  instance;
    return &instance;
}

WinSerialBasicReceiveFunction::WinSerialBasicReceiveFunction(QWidget */*parent*/)
{
//    m_list_function.append(tr("Off"));
//    m_list_function.append(tr("Normal"));
//    m_list_function.append(tr("Modbus master (/MC option)"));
//    m_list_function.append(tr("Modbus slave"));
//    m_list_function.append(tr("Bar-code"));
//    m_list_function.append(tr("DARWIN"));
    m_map_function.insert(SerialBasicFunction_off, tr("Off"));
    m_map_function.insert(SerialBasicFunction_Normal, tr("Normal"));
    m_map_function.insert(SerialBasicFunction_ModbusMaster, tr("Modbus master"));
    m_map_function.insert(SerialBasicFunction_ModbusSlave, tr("Modbus slave"));
    slotResetMenus();
}

void WinSerialBasicReceiveFunction::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map_function.begin();
    for(;item != m_map_function.end();item++){
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}

WinSerialBasicBaudFunction *WinSerialBasicBaudFunction::instance()
{
    static WinSerialBasicBaudFunction  instance;
    return &instance;
}

WinSerialBasicBaudFunction::WinSerialBasicBaudFunction(QWidget */*parent*/)
{
    m_map_buad.insert(SerialBasicBuad_1200, tr("1200"));
    m_map_buad.insert(SerialBasicBuad_2400, tr("2400"));
    m_map_buad.insert(SerialBasicBuad_4800, tr("4800"));
    m_map_buad.insert(SerialBasicBuad_9600, tr("9600"));
    m_map_buad.insert(SerialBasicBuad_19200, tr("19200"));
    m_map_buad.insert(SerialBasicBuad_38400, tr("38400"));
    m_map_buad.insert(SerialBasicBuad_57600, tr("57600"));
    m_map_buad.insert(SerialBasicBuad_115200, tr("115200"));
    slotResetMenus();
}

void WinSerialBasicBaudFunction::reloadMenu()
{
//    foreach(QString str, m_list_baud){
//        addMenu(MenuFactory::instance()->createMenu(str, MenuType_item));
//    }
    QMap<quint32,QString>::iterator item = m_map_buad.begin();
    for(;item != m_map_buad.end();item++){
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}

WinSerialBasicParityFunction *WinSerialBasicParityFunction::instance()
{
    static WinSerialBasicParityFunction  instance;
    return &instance;
}

WinSerialBasicParityFunction::WinSerialBasicParityFunction(QWidget */*parent*/)
{
//    m_list_parity.append(tr("None"));
//    m_list_parity.append(tr("Even"));
//    m_list_parity.append(tr("Odd"));
    m_map_parity.insert(SerialBasicParity_None, tr("None"));
    m_map_parity.insert(SerialBasicParity_Even, tr("Even"));
    m_map_parity.insert(SerialBasicParity_Odd, tr("Odd"));
    slotResetMenus();
}

void WinSerialBasicParityFunction::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map_parity.begin();
    for(;item != m_map_parity.end();item++){
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}

WinSerialBasicStopFunction *WinSerialBasicStopFunction::instance()
{
    static WinSerialBasicStopFunction  instance;
    return &instance;
}

WinSerialBasicStopFunction::WinSerialBasicStopFunction(QWidget */*parent*/)
{
//    m_list_stop.append(tr("1bit"));
//    m_list_stop.append(tr("2bit"));
    m_map_stop.insert(SerialBasicStop_1bit, tr("1bit"));
    m_map_stop.insert(SerialBasicStop_2bit, tr("2bit"));
    slotResetMenus();
}

void WinSerialBasicStopFunction::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map_stop.begin();
    for(;item != m_map_stop.end();item++){
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}

WinSerialBasicLengthFunction *WinSerialBasicLengthFunction::instance()
{
    static WinSerialBasicLengthFunction  instance;
    return &instance;
}

WinSerialBasicLengthFunction::WinSerialBasicLengthFunction(QWidget */*parent*/)
{
//    m_list_length.append(tr("7bit"));
//    m_list_length.append(tr("8bit"));
    m_map_length.insert(SerialBasicDataLength_7bit, tr("7bit"));
    m_map_length.insert(SerialBasicDataLength_8bit, tr("8bit"));
    slotResetMenus();
}

void WinSerialBasicLengthFunction::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map_length.begin();
    for(;item != m_map_length.end();item++){
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}


WinSerialModbusTimeout *WinSerialModbusTimeout::instance()
{
    static WinSerialModbusTimeout  instance;
    return &instance;
}

WinSerialModbusTimeout::WinSerialModbusTimeout(QWidget */*parent*/)
{
//    m_list_timeout.append(tr("100ms"));
//    m_list_timeout.append(tr("200ms"));
//    m_list_timeout.append(tr("250ms"));
//    m_list_timeout.append(tr("500ms"));
//    m_list_timeout.append(tr("1s"));
//    m_list_timeout.append(tr("2s"));
//    m_list_timeout.append(tr("5s"));
//    m_list_timeout.append(tr("10s"));
//    m_list_timeout.append(tr("1min"));
    m_map_timeout.insert(SerialModbusTimeout_100ms, tr("100ms"));
    m_map_timeout.insert(SerialModbusTimeout_200ms, tr("200ms"));
    m_map_timeout.insert(SerialModbusTimeout_250ms, tr("250ms"));
    m_map_timeout.insert(SerialModbusTimeout_500ms, tr("500ms"));
    m_map_timeout.insert(SerialModbusTimeout_1s, tr("1s"));
    m_map_timeout.insert(SerialModbusTimeout_2s, tr("2s"));
    m_map_timeout.insert(SerialModbusTimeout_5s, tr("5s"));
    m_map_timeout.insert(SerialModbusTimeout_10s, tr("10s"));
    m_map_timeout.insert(SerialModbusTimeout_1min, tr("1min"));
    slotResetMenus();
}

void WinSerialModbusTimeout::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map_timeout.begin();
    for(;item != m_map_timeout.end();item++){
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}


WinSerialModbusGap *WinSerialModbusGap::instance()
{
    static WinSerialModbusGap  instance;
    return &instance;
}

WinSerialModbusGap::WinSerialModbusGap(QWidget */*parent*/)
{
//    m_list_gap.append(tr("Off"));
//    m_list_gap.append(tr("5ms"));
//    m_list_gap.append(tr("10ms"));
//    m_list_gap.append(tr("20ms"));
//    m_list_gap.append(tr("50ms"));
//    m_list_gap.append(tr("100ms"));
    m_map_gap.insert(SerialModbusGap_off,tr("Off"));
    m_map_gap.insert(SerialModbusGap_5ms,tr("5ms"));
    m_map_gap.insert(SerialModbusGap_10ms,tr("10ms"));
    m_map_gap.insert(SerialModbusGap_20ms,tr("20ms"));
    m_map_gap.insert(SerialModbusGap_50ms,tr("50ms"));
    m_map_gap.insert(SerialModbusGap_100ms,tr("100ms"));
    slotResetMenus();
}

void WinSerialModbusGap::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map_gap.begin();
    for(;item != m_map_gap.end();item++){
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}


WinSerialModbusRetry *WinSerialModbusRetry::instance()
{
    static WinSerialModbusRetry  instance;
    return &instance;
}

WinSerialModbusRetry::WinSerialModbusRetry(QWidget */*parent*/)
{
    m_map_retry.insert(SerialModbusRetry_off, tr("Off"));
    m_map_retry.insert(SerialModbusRetry_1, tr("Once"));
    m_map_retry.insert(SerialModbusRetry_2, tr("Twice"));
    m_map_retry.insert(SerialModbusRetry_3, tr("3 times"));
    m_map_retry.insert(SerialModbusRetry_4, tr("4 times"));
    m_map_retry.insert(SerialModbusRetry_5, tr("5 times"));
    m_map_retry.insert(SerialModbusRetry_10, tr("10 times"));
    m_map_retry.insert(SerialModbusRetry_20, tr("20 times"));
    slotResetMenus();
}

void WinSerialModbusRetry::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map_retry.begin();
    for(;item != m_map_retry.end();item++){
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}


WinSerialModbusCommandChanelType *WinSerialModbusCommandChanelType::instance()
{
    static WinSerialModbusCommandChanelType  instance;
    return &instance;
}

WinSerialModbusCommandChanelType::WinSerialModbusCommandChanelType(QWidget */*parent*/)
{
    m_map_TypeW.insert(NetworkModbusCommandChanelType_IO, tr("I/O channel"));
    m_map_TypeW.insert(NetworkModbusCommandChanelType_COMM, tr("Communication channel"));
    m_map_TypeW.insert(NetworkModbusCommandChanelType_MATH, tr("Math channel"));

    m_map_TypeR.insert(NetworkModbusCommandChanelType_COMM, tr("Communication channel"));

    m_map_TypeHide.insert(NetworkModbusCommandChanelType_COMM, tr("Communication channel"));
    m_map_TypeHide.insert(NetworkModbusCommandChanelType_MATH, tr("Math channel"));

    slotResetMenus();
}

void WinSerialModbusCommandChanelType::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map_Type.begin();
    for(;item != m_map_Type.end();item++){
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}

void WinSerialModbusCommandChanelType::initMenu()
{
    QVariant value= ConfigTreeObject::instance()->getConfigObject("Config:Communication (Serial) Delegate:Modbus master:Command settings:Command settings:Type")->getData();
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
