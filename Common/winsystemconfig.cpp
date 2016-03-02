#include "winsystemconfig.h"


WinSystemEnvironmentLanguage *WinSystemEnvironmentLanguage::instance()
{
    static WinSystemEnvironmentLanguage  instance;
    return &instance;
}


WinSystemEnvironmentLanguage::WinSystemEnvironmentLanguage(QWidget */*parent*/)
{
    m_map.insert(Language_Type_English, tr("English"));
    m_map.insert(Language_Type_Japanese, tr("Japanese"));
    m_map.insert(Language_Type_German, tr("German"));
    m_map.insert(Language_Type_French, tr("French"));
    m_map.insert(Language_Type_Russian, tr("Russian"));
    m_map.insert(Language_Type_Chinese, tr("Chinese"));
    m_map.insert(Language_Type_Korean, tr("Korean"));

    slotResetMenus();
}

void WinSystemEnvironmentLanguage::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map.begin();
    for(;item != m_map.end();item++){
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}


WinSystemEnvironmentTemperature *WinSystemEnvironmentTemperature::instance()
{
    static WinSystemEnvironmentTemperature  instance;
    return &instance;
}

WinSystemEnvironmentTemperature::WinSystemEnvironmentTemperature(QWidget */*parent*/)
{
    m_map.insert(Temperature_Unit_C, tr("℃"));
    m_map.insert(Temperature_Unit_F, tr("℉"));
    slotResetMenus();
}


void WinSystemEnvironmentTemperature::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map.begin();
    for(;item != m_map.end();item++){
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}


WinSystemEnvironmentDecima *WinSystemEnvironmentDecima::instance()
{
    static WinSystemEnvironmentDecima  instance;
    return &instance;
}

WinSystemEnvironmentDecima::WinSystemEnvironmentDecima(QWidget */*parent*/)
{
    m_map.insert(Decimal_Point_Type_Point, tr("Point"));
    m_map.insert(Decimal_Point_Type_Comma, tr("Comma"));
    slotResetMenus();
}

void WinSystemEnvironmentDecima::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map.begin();
    for(;item != m_map.end();item++){
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}


WinSystemEnvironmentData *WinSystemEnvironmentData::instance()
{
    static WinSystemEnvironmentData  instance;
    return &instance;
}

WinSystemEnvironmentData::WinSystemEnvironmentData(QWidget */*parent*/)
{
    m_map.insert(Date_Format_YMD, tr("Year Month Day"));
    m_map.insert(Date_Format_MDY, tr("Month Day Year"));
    m_map.insert(Date_Format_DMY, tr("Day Month Year"));
    slotResetMenus();
}

void WinSystemEnvironmentData::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map.begin();
    for(;item != m_map.end();item++){
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}


WinSystemEnvironmentDelimiter *WinSystemEnvironmentDelimiter::instance()
{
    static WinSystemEnvironmentDelimiter  instance;
    return &instance;
}

WinSystemEnvironmentDelimiter::WinSystemEnvironmentDelimiter(QWidget */*parent*/)
{
    m_map.insert(Delimiter_Type_Slash, tr("Slash (/)"));
    m_map.insert(Delimiter_Type_Point, tr("Point (.)"));
    m_map.insert(Delimiter_Type_Hyphen, tr("Hyphen (-)"));
    slotResetMenus();
}

void WinSystemEnvironmentDelimiter::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map.begin();
    for(;item != m_map.end();item++){
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}


WinSystemEnvironmentMonth *WinSystemEnvironmentMonth::instance()
{
    static WinSystemEnvironmentMonth  instance;
    return &instance;
}

WinSystemEnvironmentMonth::WinSystemEnvironmentMonth(QWidget */*parent*/)
{
    m_map.insert(Month_Indicator_Numerical, tr("Numerical"));
    m_map.insert(Month_Indicator_Character, tr("Characters"));
    slotResetMenus();
}

void WinSystemEnvironmentMonth::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map.begin();
    for(;item != m_map.end();item++){
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}


WinSystemAlarmHold *WinSystemAlarmHold::instance()
{
    static WinSystemAlarmHold  instance;
    return &instance;
}

WinSystemAlarmHold::WinSystemAlarmHold(QWidget */*parent*/)
{
    m_mapHold.insert(Alarm_Indicator_NonHold, tr("Nonhold"));
    m_mapHold.insert(Alarm_Indicator_Hold, tr("Hold"));
    m_mapNo.insert(Alarm_Indicator_Hold, tr("Hold"));
    slotResetMenus();
}

void WinSystemAlarmHold::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map.begin();
    for(;item != m_map.end();item++){
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}

void WinSystemAlarmHold::initMenu()
{
    QVariant value = ConfigTreeObject::getConfigObject(System_Delegate_Alarm_AlarmACK)->getData();

    switch (value.toInt()) {
    case 0:
        m_map = m_mapHold;
        break;
    case 1:
        m_map = m_mapNo;
        break;
    default:
        break;
    }
    slotResetMenus();
}


WinSystemTimeHour *WinSystemTimeHour::instance()
{
    static WinSystemTimeHour  instance;
    return &instance;
}

WinSystemTimeHour::WinSystemTimeHour(QWidget */*parent*/)
{
    for (int i = 0; i < 27; i++) {
        QString str = QString("%1").arg(i-13);
        m_map.insert(i, str);
    }
    slotResetMenus();
}

void WinSystemTimeHour::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map.begin();
    for(;item != m_map.end();item++){
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}


WinSystemIntervalType *WinSystemIntervalType::instance()
{
    static WinSystemIntervalType  instance;
    return &instance;
}

WinSystemIntervalType::WinSystemIntervalType(QWidget */*parent*/)
{
    m_map.insert(InterSwitch_Mode_Alarm, tr("Alarm"));
    m_map.insert(InterSwitch_Mode_Manual, tr("Manual"));
    slotResetMenus();
}

void WinSystemIntervalType::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map.begin();
    for(;item != m_map.end();item++){
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}


WinSystemIntervalANDOR *WinSystemIntervalANDOR::instance()
{
    static WinSystemIntervalANDOR  instance;
    return &instance;
}

WinSystemIntervalANDOR::WinSystemIntervalANDOR(QWidget */*parent*/)
{
    m_map.insert(InterSwitch_Operate_And, tr("AND"));
    m_map.insert(InterSwitch_Operate_Or, tr("OR"));
    slotResetMenus();
}

void WinSystemIntervalANDOR::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map.begin();
    for(;item != m_map.end();item++){
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}


WinSystemRelay *WinSystemRelay::instance()
{
    static WinSystemRelay  instance;
    return &instance;
}

WinSystemRelay::WinSystemRelay(QWidget */*parent*/)
{
    m_map.insert(Relay_Output_Mode_Fail, tr("Fail"));
    m_map.insert(Relay_Output_Mode_Status, tr("Status"));
    slotResetMenus();
}

void WinSystemRelay::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map.begin();
    for(;item != m_map.end();item++){
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}


WinSystemCommon *WinSystemCommon::instance()
{
    static WinSystemCommon  instance;
    return &instance;
}

WinSystemCommon::WinSystemCommon(QWidget */*parent*/)
{

}

void WinSystemCommon::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map.begin();
    for(;item != m_map.end();item++){
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}

void WinSystemCommon::initMenu()
{
    QString str = ((MenuNormal*)p_pMenu)->getMenuName().toString();

    m_map.clear();
    if (str == "Paper size") {
        m_map.insert(Printer_Paper_Size_A4, tr("A4"));
        m_map.insert(Printer_Paper_Size_A3, tr("A3"));
        m_map.insert(Printer_Paper_Size_Letter, tr("Letter"));
    }
    else if (str == "Page orientation") {
        m_map.insert(Printer_Orientation_Ver, tr("Vertical"));
        m_map.insert(Printer_Orientation_Hor, tr("Horizontal"));
    }
    else if (str == "Resolution (dpi)") {
        m_map.insert(Printer_Resolution_DPI_300, tr("300dpi"));
        m_map.insert(Printer_Resolution_DPI_600, tr("600dpi"));
    }
    else if (str == "MENU key LED") {
        m_map.insert(0, tr("Off"));
        m_map.insert(1, tr("Alarm"));
    }


    slotResetMenus();
}


WinBatchCommon *WinBatchCommon::instance()
{
    static WinBatchCommon  instance;
    return &instance;
}

WinBatchCommon::WinBatchCommon(QWidget */*parent*/)
{

}

void WinBatchCommon::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map.begin();
    for(;item != m_map.end();item++){
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}

void WinBatchCommon::initMenu()
{
//    int ID = ((MenuNormal*)p_pMenu)->getMenuID().toInt();

//    switch (ID) {
//    case ID_batch_Function_Num:
//        m_map.insert(0, tr("Off"));
//        m_map.insert(4, tr("4"));
//        m_map.insert(6, tr("6"));
//        m_map.insert(8, tr("8"));
//        break;
//    case ID_Batch_Text_Text_Num:
//        for (int i = 0; i < 24; i++) {
//            m_map.insert(i, tr((QString("%1").arg(i+1)).toStdString().c_str()));
//        }
//        break;
//    default:
//        break;
//    }

    QString str = ((MenuNormal*)p_pMenu)->getMenuName().toString();

    m_map.clear();

    if (str == "Lot-No. digit") {
        m_map.insert(0, tr("Off"));
        m_map.insert(4, tr("4"));
        m_map.insert(6, tr("6"));
        m_map.insert(8, tr("8"));
    }
    else if (str == "Text num") {
        for (int i = 0; i < 24; i++) {
            m_map.insert(i, tr((QString("%1").arg(i+1)).toStdString().c_str()));
        }
    }

    slotResetMenus();
}


WinReportComm *WinReportComm::instance()
{
    static WinReportComm  instance;
    return &instance;
}

WinReportComm::WinReportComm(QWidget */*parent*/)
{

}

void WinReportComm::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map.begin();
    for(;item != m_map.end();item++){
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}

void WinReportComm::initMenu()
{
    QString str = ((MenuNormal*)p_pMenu)->getMenuName().toString();

    m_map.clear();
    if (str == "Type") {
        m_map.insert(Report_Type_Off, tr("Off"));
        m_map.insert(Report_Type_Hourly_Daily, tr("Hourly + Daily"));
        m_map.insert(Report_Type_Daily_Weekly, tr("Daily + Weekly"));
        m_map.insert(Report_Type_Daily_Monthly, tr("Weekly + Monthly"));
        m_map.insert(Report_Type_Batch, tr("Batch"));
        m_map.insert(Report_Type_Daily_Custom, tr("Daily custom"));
    }
    else if (str == "Day") {
        for (int i = 1; i <= 28; ++i) {
            m_map.insert(i, tr((QString("%1").arg(i)).toStdString().c_str()));
        }
    }
    else if (str == "Day of week") {
        m_map.insert(Report_Week_Day_Sun, tr("Sunday"));
        m_map.insert(Report_Week_Day_Mon, tr("Monday"));
        m_map.insert(Report_Week_Day_Tues, tr("Tuesday"));
        m_map.insert(Report_Week_Day_Wed, tr("Wednesday"));
        m_map.insert(Report_Week_Day_Thur, tr("Thursday"));
        m_map.insert(Report_Week_Day_Fri, tr("Friday"));
        m_map.insert(Report_Week_Day_Sat, tr("Saturday"));
    }
    else if (str == "Hour") {
        for (int i = 0; i < 24; ++i) {
            m_map.insert(i, tr((QString("%1").arg(i)).toStdString().c_str()));
        }
    }
    else if (str == "Minute") {
        for (int i = 0; i < 60; ++i) {
            m_map.insert(i, tr((QString("%1").arg(i)).toStdString().c_str()));
        }
    }
    else if (str == "Save interval") {
        m_map.insert(2, tr("2min"));
        m_map.insert(3, tr("3min"));
        m_map.insert(4, tr("4min"));
        m_map.insert(5, tr("5min"));
        m_map.insert(10, tr("10min"));
        m_map.insert(15, tr("15min"));
        m_map.insert(30, tr("30min"));
        m_map.insert(60, tr("1h"));
    }
    else if (str == "File creation interval") {
        m_map.insert(4, tr("4h"));
        m_map.insert(6, tr("6h"));
        m_map.insert(8, tr("8h"));
        m_map.insert(12, tr("12h"));
        m_map.insert(24, tr("24h"));
    }
    else if (str == "Report1" || str == "Report2" || str == "Report3" || str == "Report4" || str == "Report5") {
        m_map.insert(Report_Data_Type_AVG, tr("Ave"));
        m_map.insert(Report_Data_Type_MAX, tr("Max"));
        m_map.insert(Report_Data_Type_MIN, tr("Min"));
        m_map.insert(Report_Data_Type_SUM, tr("Sum"));
        m_map.insert(Report_Data_Type_INST, tr("Inst"));
    }
    else if (str == "File type") {
        m_map.insert(0, tr("Separate"));
        m_map.insert(1, tr("Combine"));
    }
    else if (str == "number") {
        for (int i = 0; i < 60; ++i) {
            m_map.insert(i, tr((QString("%1").arg(i+1)).toStdString().c_str()));
        }
    }
    else if (str == "Channel type") {
        if (CfgChannel::instance()->getList_BoardIO().isEmpty()) {
            m_map.insert(Channel_Type_Math, tr("Math channel"));
            m_map.insert(Channel_Type_Comm, tr("Communication channel"));
            m_map.insert(Channel_Type_Off, tr("Off"));
        }
        else {
            m_map.insert(Channel_Type_AI, tr("I/O channel"));
            m_map.insert(Channel_Type_Math, tr("Math channel"));
            m_map.insert(Channel_Type_Comm, tr("Communication channel"));
            m_map.insert(Channel_Type_Off, tr("Off"));
        }
    }
    else if (str == "Sum scale") {
        m_map.insert(Report_Sum_Unit_Off, tr("Off"));
        m_map.insert(Report_Sum_Unit_S, tr("/s"));
        m_map.insert(Report_Sum_Unit_Min, tr("/min"));
        m_map.insert(Report_Sum_Unit_H, tr("/h"));
        m_map.insert(Report_Sum_Unit_Day, tr("/day"));
    }

    slotResetMenus();
}


WinTimerTimerComm *WinTimerTimerComm::instance()
{
    static WinTimerTimerComm  instance;
    return &instance;
}

WinTimerTimerComm::WinTimerTimerComm(QWidget */*parent*/)
{

}

void WinTimerTimerComm::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map.begin();
    for(;item != m_map.end();item++){
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}

void WinTimerTimerComm::initMenu()
{

    QString str = ((MenuNormal*)p_pMenu)->getMenuName().toString();

    m_map.clear();
    if (str == "Type") {
        m_map.insert(Timer_Type_Off, tr("Off"));
        m_map.insert(Timer_Type_Relative, tr("Relative timer"));
        m_map.insert(Timer_Type_Absolute, tr("Absolute timer"));
    }
    else if (str == "Day") {
        for (int i = 0;i <= 31; ++i) {
            m_map.insert(i, tr((QString("%1").arg(i)).toStdString().c_str()));
        }
    }
    else if (str == "Hour") {
        for (int i = 0;i <= 23; ++i) {
            m_map.insert(i, tr((QString("%1").arg(i)).toStdString().c_str()));
        }
    }
    else if (str == "Minute") {
        for (int i = 0;i <= 59; ++i) {
            m_map.insert(i, tr((QString("%1").arg(i)).toStdString().c_str()));
        }
    }
    else if (str == "Interval") {
        m_map.insert(1,tr("1min"));
        m_map.insert(2,tr("2min"));
        m_map.insert(3,tr("3min"));
        m_map.insert(4,tr("4min"));
        m_map.insert(5,tr("5min"));

        m_map.insert(6,tr("6min"));
        m_map.insert(10,tr("10min"));
        m_map.insert(12,tr("12min"));
        m_map.insert(15,tr("15min"));
        m_map.insert(20,tr("20min"));

        m_map.insert(30,tr("30min"));
        m_map.insert(60,tr("1h"));
        m_map.insert(120,tr("2h"));
        m_map.insert(180,tr("3h"));
        m_map.insert(240,tr("4h"));

        m_map.insert(360,tr("6h"));
        m_map.insert(480,tr("8h"));
        m_map.insert(720,tr("12h"));
        m_map.insert(1440,tr("24h"));
    }

    slotResetMenus();
}


WinTimerMatchTimer *WinTimerMatchTimer::instance()
{
    static WinTimerMatchTimer  instance;
    return &instance;
}

WinTimerMatchTimer::WinTimerMatchTimer(QWidget */*parent*/)
{

}

void WinTimerMatchTimer::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map.begin();
    for(;item != m_map.end();item++){
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}

void WinTimerMatchTimer::initMenu()
{
    QString str = ((MenuNormal*)p_pMenu)->getMenuName().toString();

    m_map.clear();

    if (str == "Type") {
        m_map.insert(Timer_Match_Type_Off, tr("Off"));
        m_map.insert(Timer_Match_Type_Daily, tr("Day"));
        m_map.insert(Timer_Match_Type_Weekly, tr("Week"));
        m_map.insert(Timer_Match_Type_Monthly, tr("Month"));
        m_map.insert(Timer_Match_Type_Yearly, tr("Year"));
    }
    else if (str == "Month") {
        m_map.insert(1,tr("January"));
        m_map.insert(2,tr("February"));
        m_map.insert(3,tr("March"));
        m_map.insert(4,tr("April"));

        m_map.insert(5,tr("May"));
        m_map.insert(6,tr("June"));
        m_map.insert(7,tr("July"));
        m_map.insert(8,tr("August"));

        m_map.insert(9,tr("September"));
        m_map.insert(10,tr("October"));
        m_map.insert(11,tr("November"));
        m_map.insert(12,tr("December"));
    }
    else if (str == "Day") {
        for (int i = 1;i <= 31; ++i) {
            m_map.insert(i, tr((QString("%1").arg(i)).toStdString().c_str()));
        }
    }
    else if (str == "Day of week") {
        m_map.insert(0, tr("Sunday"));
        m_map.insert(1, tr("Monday"));
        m_map.insert(2, tr("Tuesday"));
        m_map.insert(3, tr("Wednesday"));
        m_map.insert(4, tr("Thursday"));
        m_map.insert(5, tr("Friday"));
        m_map.insert(6, tr("Saturday"));
    }
    else if (str == "Hour") {
        for (int i = 0;i <= 23; ++i) {
            m_map.insert(i, tr((QString("%1").arg(i)).toStdString().c_str()));
        }
    }
    else if (str == "Minute") {
        for (int i = 0;i <= 59; ++i) {
            m_map.insert(i, tr((QString("%1").arg(i)).toStdString().c_str()));
        }
    }
    else if (str == "Timer action") {
        m_map.insert(0,tr("Single"));
        m_map.insert(1,tr("Repeat"));
    }

    slotResetMenus();
}
