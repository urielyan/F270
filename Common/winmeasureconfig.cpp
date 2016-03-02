#include "winmeasureconfig.h"


WinDatasaveFileStr *WinDatasaveFileStr::instance()
{
    static WinDatasaveFileStr  instance;
    return &instance;
}

WinDatasaveFileStr::WinDatasaveFileStr(QWidget *parent)
    :WinChannelSelector(parent)
{
    m_map_DatasaveFileStr.insert(DataSave_FileName_Type_Date, tr("Date"));
    m_map_DatasaveFileStr.insert(DataSave_FileName_Type_Serial, tr("Serial"));
    m_map_DatasaveFileStr.insert(DataSave_FileName_Type_Batch, tr("Batch"));

    slotResetMenus();
}

void WinDatasaveFileStr::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map_DatasaveFileStr.begin();
    for(;item != m_map_DatasaveFileStr.end();item++) {
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}


WinDatasaveFormatData *WinDatasaveFormatData::instance()
{
    static WinDatasaveFormatData  instance;
    return &instance;
}

WinDatasaveFormatData::WinDatasaveFormatData(QWidget *parent)
    :WinChannelSelector(parent)
{
    m_map_DatasaveFormatData.insert(DataSave_FileName_Type_Bin, tr("Binary"));
    m_map_DatasaveFormatData.insert(DataSave_FileName_Type_Text, tr("Text"));

    slotResetMenus();
}

void WinDatasaveFormatData::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map_DatasaveFormatData.begin();
    for(;item != m_map_DatasaveFormatData.end();item++) {
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}


WinMeasureInterval *WinMeasureInterval::instance()
{
    static WinMeasureInterval  instance;
    return &instance;
}

WinMeasureInterval::WinMeasureInterval(QWidget *parent)
    :WinChannelSelector(parent)
{
    m_map_MeasureInterval.insert(Measure_Interval_100ms, tr("100 ms"));
    m_map_MeasureInterval.insert(Measure_Interval_200ms, tr("200 ms"));
    m_map_MeasureInterval.insert(Measure_Interval_500ms, tr("500 ms"));
    m_map_MeasureInterval.insert(Measure_Interval_1s, tr("1 s"));
    m_map_MeasureInterval.insert(Measure_Interval_2s, tr("2 s"));
    m_map_MeasureInterval.insert(Measure_Interval_5s, tr("5 s"));

    slotResetMenus();
}

void WinMeasureInterval::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map_MeasureInterval.begin();
    for(;item != m_map_MeasureInterval.end();item++) {
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}


WinMeasureOverRange *WinMeasureOverRange::instance()
{
    static WinMeasureOverRange  instance;
    return &instance;
}

WinMeasureOverRange::WinMeasureOverRange(QWidget *parent)
    :WinChannelSelector(parent)
{
    m_map_MeasureOverRange.insert(Measure_OverRange_Free, tr("Free"));
    m_map_MeasureOverRange.insert(Measure_OverRange_Over, tr("Over"));

    slotResetMenus();
}

void WinMeasureOverRange::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map_MeasureOverRange.begin();
    for(;item != m_map_MeasureOverRange.end();item++) {
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}


WinMeasureAIMode *WinMeasureAIMode::instance()
{
    static WinMeasureAIMode  instance;
    return &instance;
}

WinMeasureAIMode::WinMeasureAIMode(QWidget *parent)
    :WinChannelSelector(parent)
{
    m_map_MeasureAIMode.insert(0, tr("2ch Only, Low noise mode"));
    m_map_MeasureAIMode.insert(1, tr("10ch Normal mode"));

    slotResetMenus();
}

void WinMeasureAIMode::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map_MeasureAIMode.begin();
    for(;item != m_map_MeasureAIMode.end();item++) {
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}


WinMeasureADTime *WinMeasureADTime::instance()
{
    static WinMeasureADTime  instance;
    return &instance;
}

WinMeasureADTime::WinMeasureADTime(QWidget *parent)
    :WinChannelSelector(parent)
{
    m_map_MeasureADTime.insert(0, tr("Auto"));
    m_map_MeasureADTime.insert(1, tr("50Hz"));
    m_map_MeasureADTime.insert(2, tr("60Hz"));

    slotResetMenus();
}

void WinMeasureADTime::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map_MeasureADTime.begin();
    for(;item != m_map_MeasureADTime.end();item++) {
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}


WinMeasureDIMode *WinMeasureDIMode::instance()
{
    static WinMeasureDIMode  instance;
    return &instance;
}

WinMeasureDIMode::WinMeasureDIMode(QWidget *parent)
    :WinChannelSelector(parent)
{
    m_map_MeasureDIMode.insert(DI_OpeMode_STD, tr("Normal"));
    m_map_MeasureDIMode.insert(DI_OpeMode_Remote, tr("Remote"));

    slotResetMenus();
}

void WinMeasureDIMode::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map_MeasureDIMode.begin();
    for(;item != m_map_MeasureDIMode.end();item++) {
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}


WinRecordBasicModeMode *WinRecordBasicModeMode::instance()
{
    static WinRecordBasicModeMode  instance;
    return &instance;
}

WinRecordBasicModeMode::WinRecordBasicModeMode(QWidget *parent)
    :WinChannelSelector(parent)
{
    m_map_BasicModeMode.insert(Record_Mode_Display, tr("Display"));
    m_map_BasicModeMode.insert(Record_Mode_Event, tr("Event"));
    m_map_BasicModeMode.insert(Record_Mode_Both, tr("Display + Event"));

    slotResetMenus();
}

void WinRecordBasicModeMode::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map_BasicModeMode.begin();
    for(;item != m_map_BasicModeMode.end();item++) {
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}


WinRecordBasicDispValue *WinRecordBasicDispValue::instance()
{
    static WinRecordBasicDispValue  instance;
    return &instance;
}

WinRecordBasicDispValue::WinRecordBasicDispValue(QWidget *parent)
    :WinChannelSelector(parent)
{
    m_map_BasicDispValue.insert(Display_Record_DataLength_10min, tr("10min"));
    m_map_BasicDispValue.insert(Display_Record_DataLength_20min, tr("20min"));
    m_map_BasicDispValue.insert(Display_Record_DataLength_30min, tr("30min"));
    m_map_BasicDispValue.insert(Display_Record_DataLength_1h, tr("1h"));
    m_map_BasicDispValue.insert(Display_Record_DataLength_2h, tr("2h"));
    m_map_BasicDispValue.insert(Display_Record_DataLength_3h, tr("3h"));
    m_map_BasicDispValue.insert(Display_Record_DataLength_4h, tr("4h"));
    m_map_BasicDispValue.insert(Display_Record_DataLength_6h, tr("6h"));
    m_map_BasicDispValue.insert(Display_Record_DataLength_8h, tr("8h"));
    m_map_BasicDispValue.insert(Display_Record_DataLength_12h, tr("12h"));
    m_map_BasicDispValue.insert(Display_Record_DataLength_1day, tr("1day"));
    m_map_BasicDispValue.insert(Display_Record_DataLength_2day, tr("2day"));
    m_map_BasicDispValue.insert(Display_Record_DataLength_3day, tr("3day"));
    m_map_BasicDispValue.insert(Display_Record_DataLength_5day, tr("5day"));
    m_map_BasicDispValue.insert(Display_Record_DataLength_7day, tr("7day"));
    m_map_BasicDispValue.insert(Display_Record_DataLength_14day, tr("14day"));
    m_map_BasicDispValue.insert(Display_Record_DataLength_31day, tr("31day"));

    slotResetMenus();
}

void WinRecordBasicDispValue::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map.begin();
    for(;item != m_map.end();item++) {
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}

void WinRecordBasicDispValue::initMenu()
{
    m_map.clear();
    m_map = m_map_BasicDispValue;

    Trend_Interval backup;
    Record_Mode Recmode;
    quint8 sw;
    ConfigObject* qObj = ConfigTreeObject::getConfigObject(RECORD_DELEGATE_BASIC_MODE_VALUE);
    ConfigObject* rObj = ConfigTreeObject::getConfigObject(DISP_DELEGATE_TREND_INTERVAL_SWITCH);
    if(qObj && rObj) {
        Recmode = (Record_Mode)qObj->getData().toUInt();
        sw = rObj->getData().toUInt();

        if (Record_Mode_Event == Recmode) {
            //Do nothing 事件记录模式
        } else {
            Trend_Interval first = (Trend_Interval)ConfigTreeObject::\
                    getConfigObject(DISP_DELEGATE_TREND_INTERVAL_INTERVAL)->getData().toUInt();

            if(0 == sw) {
                backup = first;
            } else {
                Trend_Interval second = (Trend_Interval)ConfigTreeObject::\
                        getConfigObject(DISP_DELEGATE_TREND_INTERVAL_SECOND)->getData().toUInt();
                backup = qMin(first, second);
            }
        }
    }

    switch(backup) {
    case Trend_Interval_5s:
        m_map.remove(Display_Record_DataLength_1day);
        m_map.remove(Display_Record_DataLength_2day);
        m_map.remove(Display_Record_DataLength_3day);
        m_map.remove(Display_Record_DataLength_5day);
        m_map.remove(Display_Record_DataLength_7day);
        m_map.remove(Display_Record_DataLength_14day);
        m_map.remove(Display_Record_DataLength_31day);
        break;
    case Trend_Interval_10s:
        m_map.remove(Display_Record_DataLength_2day);
        m_map.remove(Display_Record_DataLength_3day);
        m_map.remove(Display_Record_DataLength_5day);
        m_map.remove(Display_Record_DataLength_7day);
        m_map.remove(Display_Record_DataLength_14day);
        m_map.remove(Display_Record_DataLength_31day);
        break;
    case Trend_Interval_15s:
        m_map.remove(Display_Record_DataLength_5day);
        m_map.remove(Display_Record_DataLength_7day);
        m_map.remove(Display_Record_DataLength_14day);
        m_map.remove(Display_Record_DataLength_31day);
        break;
    case Trend_Interval_30s:
        m_map.remove(Display_Record_DataLength_14day);
        m_map.remove(Display_Record_DataLength_31day);
        break;
    case Trend_Interval_1min:
    case Trend_Interval_2min:
        m_map.remove(Display_Record_DataLength_31day);
        break;
    case Trend_Interval_5min:
    case Trend_Interval_10min:
    case Trend_Interval_15min:
        break;
    case Trend_Interval_20min:
    case Trend_Interval_30min:
    case Trend_Interval_1h:
        m_map.remove(Display_Record_DataLength_10min);
        m_map.remove(Display_Record_DataLength_20min);
        m_map.remove(Display_Record_DataLength_30min);
        break;
    case Trend_Interval_2h:
        m_map.remove(Display_Record_DataLength_10min);
        m_map.remove(Display_Record_DataLength_20min);
        m_map.remove(Display_Record_DataLength_30min);
        m_map.remove(Display_Record_DataLength_1h);
        break;
    case Trend_Interval_4h:
        m_map.remove(Display_Record_DataLength_10min);
        m_map.remove(Display_Record_DataLength_20min);
        m_map.remove(Display_Record_DataLength_30min);
        m_map.remove(Display_Record_DataLength_1h);
        m_map.remove(Display_Record_DataLength_2h);
        m_map.remove(Display_Record_DataLength_3h);
        break;
    case Trend_Interval_10h:
        m_map.remove(Display_Record_DataLength_10min);
        m_map.remove(Display_Record_DataLength_20min);
        m_map.remove(Display_Record_DataLength_30min);
        m_map.remove(Display_Record_DataLength_1h);
        m_map.remove(Display_Record_DataLength_2h);
        m_map.remove(Display_Record_DataLength_3h);
        m_map.remove(Display_Record_DataLength_4h);
        m_map.remove(Display_Record_DataLength_6h);
        break;
    }

    slotResetMenus();
}


WinRecordBasicEventValue *WinRecordBasicEventValue::instance()
{
    static WinRecordBasicEventValue  instance;
    return &instance;
}

WinRecordBasicEventValue::WinRecordBasicEventValue(QWidget *parent)
    :WinChannelSelector(parent)
{
    m_map_BasicEventValue_100ms.insert(Event_Record_Interval_100ms, tr("100ms"));
    m_map_BasicEventValue_100ms.insert(Event_Record_Interval_200ms, tr("200ms"));
    m_map_BasicEventValue_100ms.insert(Event_Record_Interval_500ms, tr("500ms"));
    m_map_BasicEventValue_100ms.insert(Event_Record_Interval_1s, tr("1s"));
    m_map_BasicEventValue_100ms.insert(Event_Record_Interval_2s, tr("2s"));
    m_map_BasicEventValue_100ms.insert(Event_Record_Interval_5s, tr("5s"));
    m_map_BasicEventValue_100ms.insert(Event_Record_Interval_10s, tr("10s"));
    m_map_BasicEventValue_100ms.insert(Event_Record_Interval_15s, tr("15s"));
    m_map_BasicEventValue_100ms.insert(Event_Record_Interval_20s, tr("20s"));
    m_map_BasicEventValue_100ms.insert(Event_Record_Interval_30s, tr("30s"));
    m_map_BasicEventValue_100ms.insert(Event_Record_Interval_1min, tr("1min"));
    m_map_BasicEventValue_100ms.insert(Event_Record_Interval_2min, tr("2min"));
    m_map_BasicEventValue_100ms.insert(Event_Record_Interval_5min, tr("5min"));
    m_map_BasicEventValue_100ms.insert(Event_Record_Interval_10min, tr("10min"));
    m_map_BasicEventValue_100ms.insert(Event_Record_Interval_15min, tr("15min"));
    m_map_BasicEventValue_100ms.insert(Event_Record_Interval_20min, tr("20min"));
    m_map_BasicEventValue_100ms.insert(Event_Record_Interval_30min, tr("30min"));

    m_map_BasicEventValue_200ms.insert(Event_Record_Interval_200ms, tr("200ms"));
    m_map_BasicEventValue_200ms.insert(Event_Record_Interval_500ms, tr("500ms"));
    m_map_BasicEventValue_200ms.insert(Event_Record_Interval_1s, tr("1s"));
    m_map_BasicEventValue_200ms.insert(Event_Record_Interval_2s, tr("2s"));
    m_map_BasicEventValue_200ms.insert(Event_Record_Interval_5s, tr("5s"));
    m_map_BasicEventValue_200ms.insert(Event_Record_Interval_10s, tr("10s"));
    m_map_BasicEventValue_200ms.insert(Event_Record_Interval_15s, tr("15s"));
    m_map_BasicEventValue_200ms.insert(Event_Record_Interval_20s, tr("20s"));
    m_map_BasicEventValue_200ms.insert(Event_Record_Interval_30s, tr("30s"));
    m_map_BasicEventValue_200ms.insert(Event_Record_Interval_1min, tr("1min"));
    m_map_BasicEventValue_200ms.insert(Event_Record_Interval_2min, tr("2min"));
    m_map_BasicEventValue_200ms.insert(Event_Record_Interval_5min, tr("5min"));
    m_map_BasicEventValue_200ms.insert(Event_Record_Interval_10min, tr("10min"));
    m_map_BasicEventValue_200ms.insert(Event_Record_Interval_15min, tr("15min"));
    m_map_BasicEventValue_200ms.insert(Event_Record_Interval_20min, tr("20min"));
    m_map_BasicEventValue_200ms.insert(Event_Record_Interval_30min, tr("30min"));

    m_map_BasicEventValue_500ms.insert(Event_Record_Interval_500ms, tr("500ms"));
    m_map_BasicEventValue_500ms.insert(Event_Record_Interval_1s, tr("1s"));
    m_map_BasicEventValue_500ms.insert(Event_Record_Interval_2s, tr("2s"));
    m_map_BasicEventValue_500ms.insert(Event_Record_Interval_5s, tr("5s"));
    m_map_BasicEventValue_500ms.insert(Event_Record_Interval_10s, tr("10s"));
    m_map_BasicEventValue_500ms.insert(Event_Record_Interval_15s, tr("15s"));
    m_map_BasicEventValue_500ms.insert(Event_Record_Interval_20s, tr("20s"));
    m_map_BasicEventValue_500ms.insert(Event_Record_Interval_30s, tr("30s"));
    m_map_BasicEventValue_500ms.insert(Event_Record_Interval_1min, tr("1min"));
    m_map_BasicEventValue_500ms.insert(Event_Record_Interval_2min, tr("2min"));
    m_map_BasicEventValue_500ms.insert(Event_Record_Interval_5min, tr("5min"));
    m_map_BasicEventValue_500ms.insert(Event_Record_Interval_10min, tr("10min"));
    m_map_BasicEventValue_500ms.insert(Event_Record_Interval_15min, tr("15min"));
    m_map_BasicEventValue_500ms.insert(Event_Record_Interval_20min, tr("20min"));
    m_map_BasicEventValue_500ms.insert(Event_Record_Interval_30min, tr("30min"));

    m_map_BasicEventValue_1s.insert(Event_Record_Interval_1s, tr("1s"));
    m_map_BasicEventValue_1s.insert(Event_Record_Interval_2s, tr("2s"));
    m_map_BasicEventValue_1s.insert(Event_Record_Interval_5s, tr("5s"));
    m_map_BasicEventValue_1s.insert(Event_Record_Interval_10s, tr("10s"));
    m_map_BasicEventValue_1s.insert(Event_Record_Interval_15s, tr("15s"));
    m_map_BasicEventValue_1s.insert(Event_Record_Interval_20s, tr("20s"));
    m_map_BasicEventValue_1s.insert(Event_Record_Interval_30s, tr("30s"));
    m_map_BasicEventValue_1s.insert(Event_Record_Interval_1min, tr("1min"));
    m_map_BasicEventValue_1s.insert(Event_Record_Interval_2min, tr("2min"));
    m_map_BasicEventValue_1s.insert(Event_Record_Interval_5min, tr("5min"));
    m_map_BasicEventValue_1s.insert(Event_Record_Interval_10min, tr("10min"));
    m_map_BasicEventValue_1s.insert(Event_Record_Interval_15min, tr("15min"));
    m_map_BasicEventValue_1s.insert(Event_Record_Interval_20min, tr("20min"));
    m_map_BasicEventValue_1s.insert(Event_Record_Interval_30min, tr("30min"));

    m_map_BasicEventValue_2s.insert(Event_Record_Interval_2s, tr("2s"));
//    m_map_BasicEventValue_2s.insert(Event_Record_Interval_5s, tr("5s"));
    m_map_BasicEventValue_2s.insert(Event_Record_Interval_10s, tr("10s"));
//    m_map_BasicEventValue_2s.insert(Event_Record_Interval_15s, tr("15s"));
    m_map_BasicEventValue_2s.insert(Event_Record_Interval_20s, tr("20s"));
    m_map_BasicEventValue_2s.insert(Event_Record_Interval_30s, tr("30s"));
    m_map_BasicEventValue_2s.insert(Event_Record_Interval_1min, tr("1min"));
    m_map_BasicEventValue_2s.insert(Event_Record_Interval_2min, tr("2min"));
    m_map_BasicEventValue_2s.insert(Event_Record_Interval_5min, tr("5min"));
    m_map_BasicEventValue_2s.insert(Event_Record_Interval_10min, tr("10min"));
    m_map_BasicEventValue_2s.insert(Event_Record_Interval_15min, tr("15min"));
    m_map_BasicEventValue_2s.insert(Event_Record_Interval_20min, tr("20min"));
    m_map_BasicEventValue_2s.insert(Event_Record_Interval_30min, tr("30min"));

    m_map_BasicEventValue_5s.insert(Event_Record_Interval_5s, tr("5s"));
    m_map_BasicEventValue_5s.insert(Event_Record_Interval_10s, tr("10s"));
    m_map_BasicEventValue_5s.insert(Event_Record_Interval_15s, tr("15s"));
    m_map_BasicEventValue_5s.insert(Event_Record_Interval_20s, tr("20s"));
    m_map_BasicEventValue_5s.insert(Event_Record_Interval_30s, tr("30s"));
    m_map_BasicEventValue_5s.insert(Event_Record_Interval_1min, tr("1min"));
    m_map_BasicEventValue_5s.insert(Event_Record_Interval_2min, tr("2min"));
    m_map_BasicEventValue_5s.insert(Event_Record_Interval_5min, tr("5min"));
    m_map_BasicEventValue_5s.insert(Event_Record_Interval_10min, tr("10min"));
    m_map_BasicEventValue_5s.insert(Event_Record_Interval_15min, tr("15min"));
    m_map_BasicEventValue_5s.insert(Event_Record_Interval_20min, tr("20min"));
    m_map_BasicEventValue_5s.insert(Event_Record_Interval_30min, tr("30min"));

    slotResetMenus();
}

void WinRecordBasicEventValue::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map.begin();
    for(;item != m_map.end();item++) {
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}

void WinRecordBasicEventValue::initMenu()
{
    m_map.clear();

    //不能指定比测量周期短的记录周期
    ConfigObject* pObj = ConfigTreeObject::getConfigObject(MEASURE_DELEGATE_INTERVAL_INTERVAL);
    Measure_Interval testPeriod;
    if(pObj) {
        testPeriod = (Measure_Interval)pObj->getData().toUInt();
        switch(testPeriod) {
        case Measure_Interval_100ms:
            m_map = m_map_BasicEventValue_100ms;
            break;
        case Measure_Interval_200ms:
            m_map = m_map_BasicEventValue_200ms;
            break;
        case Measure_Interval_500ms:
            m_map = m_map_BasicEventValue_500ms;
            break;
        case Measure_Interval_1s:
            m_map = m_map_BasicEventValue_1s;
            break;
        case Measure_Interval_2s:
            m_map = m_map_BasicEventValue_2s;
            break;
        case Measure_Interval_5s:
            m_map = m_map_BasicEventValue_5s;
            break;
        }
    }

    slotResetMenus();
}


WinRecordBasicEventMode *WinRecordBasicEventMode::instance()
{
    static WinRecordBasicEventMode  instance;
    return &instance;
}

WinRecordBasicEventMode::WinRecordBasicEventMode(QWidget *parent)
    :WinChannelSelector(parent)
{
    m_map_BasicEventMode.insert(Event_Record_Mode_Free, tr("Free"));
    m_map_BasicEventMode.insert(Event_Record_Mode_Single, tr("Single"));
    m_map_BasicEventMode.insert(Event_Record_Mode_Loop, tr("Repeat"));

    slotResetMenus();
}

void WinRecordBasicEventMode::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map_BasicEventMode.begin();
    for(;item != m_map_BasicEventMode.end();item++) {
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}


WinRecordBasicEventLenth *WinRecordBasicEventLenth::instance()
{
    static WinRecordBasicEventLenth  instance;
    return &instance;
}

WinRecordBasicEventLenth::WinRecordBasicEventLenth(QWidget *parent)
    :WinChannelSelector(parent)
{
    m_map_BasicEventLenth.insert(Event_Record_DataLength_10min, tr("10min"));
    m_map_BasicEventLenth.insert(Event_Record_DataLength_20min, tr("20min"));
    m_map_BasicEventLenth.insert(Event_Record_DataLength_30min, tr("30min"));
    m_map_BasicEventLenth.insert(Event_Record_DataLength_1h, tr("1h"));
    m_map_BasicEventLenth.insert(Event_Record_DataLength_2h, tr("2h"));
    m_map_BasicEventLenth.insert(Event_Record_DataLength_3h, tr("3h"));
    m_map_BasicEventLenth.insert(Event_Record_DataLength_6h, tr("6h"));
    m_map_BasicEventLenth.insert(Event_Record_DataLength_8h, tr("8h"));
    m_map_BasicEventLenth.insert(Event_Record_DataLength_12h, tr("12h"));
    m_map_BasicEventLenth.insert(Event_Record_DataLength_1day, tr("1day"));
    m_map_BasicEventLenth.insert(Event_Record_DataLength_2day, tr("2day"));
    m_map_BasicEventLenth.insert(Event_Record_DataLength_3day, tr("3day"));
    m_map_BasicEventLenth.insert(Event_Record_DataLength_5day, tr("5day"));
    m_map_BasicEventLenth.insert(Event_Record_DataLength_7day, tr("7day"));
    m_map_BasicEventLenth.insert(Event_Record_DataLength_14day, tr("14day"));
    m_map_BasicEventLenth.insert(Event_Record_DataLength_31day, tr("31day"));

    slotResetMenus();
}

void WinRecordBasicEventLenth::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map.begin();
    for(;item != m_map.end();item++) {
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}

void WinRecordBasicEventLenth::initMenu()
{
    m_map.clear();
    m_map = m_map_BasicEventLenth;

    ConfigObject* pObj = ConfigTreeObject::getConfigObject(RECORD_DELEGATE_BASIC_EVENT_INTERVAL);
    if(pObj) {
        Event_Record_Interval eventRec = (Event_Record_Interval)pObj->getData().toUInt();
        switch(eventRec) {
        case Event_Record_Interval_100ms:
            m_map.remove(Event_Record_DataLength_2day);
            m_map.remove(Event_Record_DataLength_3day);
            m_map.remove(Event_Record_DataLength_5day);
            m_map.remove(Event_Record_DataLength_7day);
            m_map.remove(Event_Record_DataLength_14day);
            m_map.remove(Event_Record_DataLength_31day);
            break;
        case Event_Record_Interval_200ms:
            m_map.remove(Event_Record_DataLength_3day);
            m_map.remove(Event_Record_DataLength_5day);
            m_map.remove(Event_Record_DataLength_7day);
            m_map.remove(Event_Record_DataLength_14day);
            m_map.remove(Event_Record_DataLength_31day);
            break;
        case Event_Record_Interval_500ms:
            m_map.remove(Event_Record_DataLength_5day);
            m_map.remove(Event_Record_DataLength_7day);
            m_map.remove(Event_Record_DataLength_14day);
            m_map.remove(Event_Record_DataLength_31day);
            break;
        case Event_Record_Interval_1s:
            m_map.remove(Event_Record_DataLength_14day);
            m_map.remove(Event_Record_DataLength_31day);
            break;
        case Event_Record_Interval_2s:
            m_map.remove(Event_Record_DataLength_31day);
            break;
        case Event_Record_Interval_5s:
        case Event_Record_Interval_10s:
        case Event_Record_Interval_15s:
        case Event_Record_Interval_20s:
            break;
        case Event_Record_Interval_30s:
        case Event_Record_Interval_1min:
        case Event_Record_Interval_2min:
        case Event_Record_Interval_5min:
        case Event_Record_Interval_10min:
        case Event_Record_Interval_15min:
        case Event_Record_Interval_20min:
        case Event_Record_Interval_30min:
            m_map.remove(Event_Record_DataLength_10min);
            m_map.remove(Event_Record_DataLength_20min);
            m_map.remove(Event_Record_DataLength_30min);
            break;
        }
    }

    slotResetMenus();
}


WinRecordBasicEventTrigger *WinRecordBasicEventTrigger::instance()
{
    static WinRecordBasicEventTrigger  instance;
    return &instance;
}

WinRecordBasicEventTrigger::WinRecordBasicEventTrigger(QWidget *parent)
    :WinChannelSelector(parent)
{
    m_map_BasicEventTrigger.insert(0, tr("0%"));
    m_map_BasicEventTrigger.insert(5, tr("5%"));
    m_map_BasicEventTrigger.insert(25, tr("25%"));
    m_map_BasicEventTrigger.insert(50, tr("50%"));
    m_map_BasicEventTrigger.insert(75, tr("75%"));
    m_map_BasicEventTrigger.insert(95, tr("95%"));
    m_map_BasicEventTrigger.insert(100, tr("100%"));

    slotResetMenus();
}

void WinRecordBasicEventTrigger::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map_BasicEventTrigger.begin();
    for(;item != m_map_BasicEventTrigger.end();item++) {
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}


WinSecurityBasicTouchlock *WinSecurityBasicTouchlock::instance()
{
    static WinSecurityBasicTouchlock  instance;
    return &instance;
}

WinSecurityBasicTouchlock::WinSecurityBasicTouchlock(QWidget *parent)
    :WinChannelSelector(parent)
{
    m_map_Touchlock.insert(Security_TouchOpe_Off, "Off");
    m_map_Touchlock.insert(Security_TouchOpe_Login, tr("Login"));
    m_map_Touchlock.insert(Security_TouchOpe_Lock, tr("Operation Lock"));

    slotResetMenus();
}

void WinSecurityBasicTouchlock::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map_Touchlock.begin();
    for(;item != m_map_Touchlock.end();item++) {
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}


WinSecurityBasicComm *WinSecurityBasicComm::instance()
{
    static WinSecurityBasicComm  instance;
    return &instance;
}

WinSecurityBasicComm::WinSecurityBasicComm(QWidget *parent)
    :WinChannelSelector(parent)
{
    m_map_Comm.insert(Security_Comm_Off, "Off");
    m_map_Comm.insert(Security_Comm_Login, tr("Login"));

    slotResetMenus();
}

void WinSecurityBasicComm::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map_Comm.begin();
    for(;item != m_map_Comm.end();item++) {
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}

WinSecurityBasicAuto *WinSecurityBasicAuto::instance()
{
    static WinSecurityBasicAuto  instance;
    return &instance;
}

WinSecurityBasicAuto::WinSecurityBasicAuto(QWidget *parent)
    :WinChannelSelector(parent)
{
    m_map_Auto.insert(Security_Logout_Off, "Off");
    m_map_Auto.insert(Security_Logout_1min, "1min");
    m_map_Auto.insert(Security_Logout_2min, "2min");
    m_map_Auto.insert(Security_Logout_5min, "5min");
    m_map_Auto.insert(Security_Logout_10min, "10min");

    slotResetMenus();
}

void WinSecurityBasicAuto::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map_Auto.begin();
    for(;item != m_map_Auto.end();item++) {
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}

WinSecurityProperty *WinSecurityProperty::instance()
{
    static WinSecurityProperty  instance;
    return &instance;
}

WinSecurityProperty::WinSecurityProperty(QWidget *parent)
    :WinChannelSelector(parent)
{
    m_map_Property.insert(Security_Auth_Free, tr("Free"));
    m_map_Property.insert(Security_Auth_Lock, tr("Lock"));

    slotResetMenus();
}

void WinSecurityProperty::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map_Property.begin();
    for(;item != m_map_Property.end();item++) {
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}

WinSecurityUserNum *WinSecurityUserNum::instance()
{
    static WinSecurityUserNum  instance;
    return &instance;
}

WinSecurityUserNum::WinSecurityUserNum(QWidget *parent)
    :WinChannelSelector(parent)
{
    for(int i=0;i<50;++i) {
        QString istr = QString("%1").arg(i+1);
        m_map_UserNum.insert(i, istr);
    }

    slotResetMenus();
}

void WinSecurityUserNum::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map_UserNum.begin();
    for(;item != m_map_UserNum.end();item++) {
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}

WinSecurityUserLvl *WinSecurityUserLvl::instance()
{
    static WinSecurityUserLvl  instance;
    return &instance;
}

WinSecurityUserLvl::WinSecurityUserLvl(QWidget *parent)
    :WinChannelSelector(parent)
{
    m_map_UserLvl.insert(Security_UserLvl_Off, "Off");
    m_map_UserLvl.insert(Security_UserLvl_Admin, tr("Admin"));
    m_map_UserLvl.insert(Security_UserLvl_User, tr("User"));

    slotResetMenus();
}

void WinSecurityUserLvl::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map.begin();
    for(;item != m_map.end();item++) {
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}

void WinSecurityUserLvl::initMenu()
{
    m_map.clear();
    m_map = m_map_UserLvl;

    ConfigObject *pObj = ConfigTreeObject::getConfigObject(SECURITY_DELEGATE_SETTING_USERNUM);
    if(pObj) {
        quint32 userNum = pObj->getData().toUInt();
        //[用户号码1]的用户级别固定为[管理员]
        if(0 == userNum) {
            m_map.remove(Security_UserLvl_Off);
            m_map.remove(Security_UserLvl_User);
        }
    }

    slotResetMenus();
}

WinSecurityUserMode *WinSecurityUserMode::instance()
{
    static WinSecurityUserMode  instance;
    return &instance;
}

WinSecurityUserMode::WinSecurityUserMode(QWidget *parent)
    :WinChannelSelector(parent)
{
    m_map_UserMode.insert(Security_UserMode_Touch, tr("Touch operation"));
    m_map_UserMode.insert(Security_UserMode_Comm, tr("Communication"));
    m_map_UserMode.insert(Security_UserMode_Both, tr("Touch operation + Communication"));

    slotResetMenus();
}

void WinSecurityUserMode::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map_UserMode.begin();
    for(;item != m_map_UserMode.end();item++) {
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}

void WinSecurityUserMode::initMenu()
{
    m_map.clear();
    m_map = m_map_UserMode;

    ConfigObject *pObj = ConfigTreeObject::getConfigObject(SECURITY_DELEGATE_SETTING_USERNUM);
    if(pObj) {
        quint32 userNum = pObj->getData().toUInt();
        //用户号码1不能设定[通信]
        if(0 == userNum) {
            m_map.remove(Security_UserMode_Comm);
        }
    }

    slotResetMenus();
}

WinSecurityUserInit *WinSecurityUserInit::instance()
{
    static WinSecurityUserInit  instance;
    return &instance;
}

WinSecurityUserInit::WinSecurityUserInit(QWidget *parent)
    :WinChannelSelector(parent)
{
    m_map_UserInit.insert(0, tr("Back"));
    m_map_UserInit.insert(1, tr("Initialize password"));

    slotResetMenus();
}

void WinSecurityUserInit::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map_UserInit.begin();
    for(;item != m_map_UserInit.end();item++) {
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}

WinSecurityAuthNum *WinSecurityAuthNum::instance()
{
    static WinSecurityAuthNum  instance;
    return &instance;
}

WinSecurityAuthNum::WinSecurityAuthNum(QWidget *parent)
    :WinChannelSelector(parent)
{
    for(int i=0;i<10;++i) {
        QString istr = QString("%1").arg(i+1);
        m_map_AuthNum.insert(i, istr);
    }

    slotResetMenus();
}

void WinSecurityAuthNum::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map_AuthNum.begin();
    for(;item != m_map_AuthNum.end();item++) {
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}
