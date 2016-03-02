/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：windispconfig.cpp
 * 概    要：Display settings下存在item单例的所有配置项目，构建并展开选项菜单list。
 *
 * 当前版本：V1.0.0
 * 作    者：邢 俊 杰
 * 完成日期：2015-11-11
 *
 * 修改版本：
 * 修改说明：
 * 修改作者：
 *
 ********************************************************************/
#include "windispconfig.h"

WinDispGrpNum *WinDispGrpNum::instance()
{
    static WinDispGrpNum  instance;
    return &instance;
}

WinDispGrpNum::WinDispGrpNum(QWidget *parent)
    :WinChannelSelector(parent)
{
    for(int i=0;i<GROUP_NUM_MAX;++i) {
        QString istr = QString("%1").arg(i+1);
        //QByteArray ba = istr.toLatin1();
        //m_map_GrpNum.insert(i, tr(ba.data()));
        m_map_GrpNum.insert(i, istr);
    }

    slotResetMenus();
}

void WinDispGrpNum::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map_GrpNum.begin();
    for(;item != m_map_GrpNum.end();item++) {
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}


WinDispTrendTrendInterval *WinDispTrendTrendInterval::instance()
{
    static WinDispTrendTrendInterval  instance;
    return &instance;
}

WinDispTrendTrendInterval::WinDispTrendTrendInterval(QWidget *parent)
    :WinChannelSelector(parent)
{
    m_map_TrendTrendInterval_5s.insert(Trend_Interval_5s, tr("5s"));
    m_map_TrendTrendInterval_5s.insert(Trend_Interval_10s, tr("10s"));
    m_map_TrendTrendInterval_5s.insert(Trend_Interval_15s, tr("15s"));
    m_map_TrendTrendInterval_5s.insert(Trend_Interval_30s, tr("30s"));
    m_map_TrendTrendInterval_5s.insert(Trend_Interval_1min, tr("1min"));
    m_map_TrendTrendInterval_5s.insert(Trend_Interval_2min, tr("2min"));
    m_map_TrendTrendInterval_5s.insert(Trend_Interval_5min, tr("5min"));
    m_map_TrendTrendInterval_5s.insert(Trend_Interval_10min, tr("10min"));
    m_map_TrendTrendInterval_5s.insert(Trend_Interval_15min, tr("15min"));
    m_map_TrendTrendInterval_5s.insert(Trend_Interval_20min, tr("20min"));
    m_map_TrendTrendInterval_5s.insert(Trend_Interval_30min, tr("30min"));
    m_map_TrendTrendInterval_5s.insert(Trend_Interval_1h, tr("1h"));
    m_map_TrendTrendInterval_5s.insert(Trend_Interval_2h, tr("2h"));
    m_map_TrendTrendInterval_5s.insert(Trend_Interval_4h, tr("4h"));
    m_map_TrendTrendInterval_5s.insert(Trend_Interval_10h, tr("10h"));


    m_map_TrendTrendInterval_10s.insert(Trend_Interval_10s, tr("10s"));
    m_map_TrendTrendInterval_10s.insert(Trend_Interval_15s, tr("15s"));
    m_map_TrendTrendInterval_10s.insert(Trend_Interval_30s, tr("30s"));
    m_map_TrendTrendInterval_10s.insert(Trend_Interval_1min, tr("1min"));
    m_map_TrendTrendInterval_10s.insert(Trend_Interval_2min, tr("2min"));
    m_map_TrendTrendInterval_10s.insert(Trend_Interval_5min, tr("5min"));
    m_map_TrendTrendInterval_10s.insert(Trend_Interval_10min, tr("10min"));
    m_map_TrendTrendInterval_10s.insert(Trend_Interval_15min, tr("15min"));
    m_map_TrendTrendInterval_10s.insert(Trend_Interval_20min, tr("20min"));
    m_map_TrendTrendInterval_10s.insert(Trend_Interval_30min, tr("30min"));
    m_map_TrendTrendInterval_10s.insert(Trend_Interval_1h, tr("1h"));
    m_map_TrendTrendInterval_10s.insert(Trend_Interval_2h, tr("2h"));
    m_map_TrendTrendInterval_10s.insert(Trend_Interval_4h, tr("4h"));
    m_map_TrendTrendInterval_10s.insert(Trend_Interval_10h, tr("10h"));


    m_map_TrendTrendInterval_15s.insert(Trend_Interval_15s, tr("15s"));
    m_map_TrendTrendInterval_15s.insert(Trend_Interval_30s, tr("30s"));
    m_map_TrendTrendInterval_15s.insert(Trend_Interval_1min, tr("1min"));
    m_map_TrendTrendInterval_15s.insert(Trend_Interval_2min, tr("2min"));
    m_map_TrendTrendInterval_15s.insert(Trend_Interval_5min, tr("5min"));
    m_map_TrendTrendInterval_15s.insert(Trend_Interval_10min, tr("10min"));
    m_map_TrendTrendInterval_15s.insert(Trend_Interval_15min, tr("15min"));
    m_map_TrendTrendInterval_15s.insert(Trend_Interval_20min, tr("20min"));
    m_map_TrendTrendInterval_15s.insert(Trend_Interval_30min, tr("30min"));
    m_map_TrendTrendInterval_15s.insert(Trend_Interval_1h, tr("1h"));
    m_map_TrendTrendInterval_15s.insert(Trend_Interval_2h, tr("2h"));
    m_map_TrendTrendInterval_15s.insert(Trend_Interval_4h, tr("4h"));
    m_map_TrendTrendInterval_15s.insert(Trend_Interval_10h, tr("10h"));


    m_map_TrendTrendInterval_30s.insert(Trend_Interval_30s, tr("30s"));
    m_map_TrendTrendInterval_30s.insert(Trend_Interval_1min, tr("1min"));
    m_map_TrendTrendInterval_30s.insert(Trend_Interval_2min, tr("2min"));
    m_map_TrendTrendInterval_30s.insert(Trend_Interval_5min, tr("5min"));
    m_map_TrendTrendInterval_30s.insert(Trend_Interval_10min, tr("10min"));
    m_map_TrendTrendInterval_30s.insert(Trend_Interval_15min, tr("15min"));
    m_map_TrendTrendInterval_30s.insert(Trend_Interval_20min, tr("20min"));
    m_map_TrendTrendInterval_30s.insert(Trend_Interval_30min, tr("30min"));
    m_map_TrendTrendInterval_30s.insert(Trend_Interval_1h, tr("1h"));
    m_map_TrendTrendInterval_30s.insert(Trend_Interval_2h, tr("2h"));
    m_map_TrendTrendInterval_30s.insert(Trend_Interval_4h, tr("4h"));
    m_map_TrendTrendInterval_30s.insert(Trend_Interval_10h, tr("10h"));


    m_map_TrendTrendInterval_1min.insert(Trend_Interval_1min, tr("1min"));
    m_map_TrendTrendInterval_1min.insert(Trend_Interval_2min, tr("2min"));
    m_map_TrendTrendInterval_1min.insert(Trend_Interval_5min, tr("5min"));
    m_map_TrendTrendInterval_1min.insert(Trend_Interval_10min, tr("10min"));
    m_map_TrendTrendInterval_1min.insert(Trend_Interval_15min, tr("15min"));
    m_map_TrendTrendInterval_1min.insert(Trend_Interval_20min, tr("20min"));
    m_map_TrendTrendInterval_1min.insert(Trend_Interval_30min, tr("30min"));
    m_map_TrendTrendInterval_1min.insert(Trend_Interval_1h, tr("1h"));
    m_map_TrendTrendInterval_1min.insert(Trend_Interval_2h, tr("2h"));
    m_map_TrendTrendInterval_1min.insert(Trend_Interval_4h, tr("4h"));
    m_map_TrendTrendInterval_1min.insert(Trend_Interval_10h, tr("10h"));


    m_map_TrendTrendInterval_5min.insert(Trend_Interval_5min, tr("5min"));
    m_map_TrendTrendInterval_5min.insert(Trend_Interval_10min, tr("10min"));
    m_map_TrendTrendInterval_5min.insert(Trend_Interval_15min, tr("15min"));
    m_map_TrendTrendInterval_5min.insert(Trend_Interval_20min, tr("20min"));
    m_map_TrendTrendInterval_5min.insert(Trend_Interval_30min, tr("30min"));
    m_map_TrendTrendInterval_5min.insert(Trend_Interval_1h, tr("1h"));
    m_map_TrendTrendInterval_5min.insert(Trend_Interval_2h, tr("2h"));
    m_map_TrendTrendInterval_5min.insert(Trend_Interval_4h, tr("4h"));
    m_map_TrendTrendInterval_5min.insert(Trend_Interval_10h, tr("10h"));

    slotResetMenus();
}

void WinDispTrendTrendInterval::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map.begin();
    for(;item != m_map.end();item++) {
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}

void WinDispTrendTrendInterval::initMenu()
{
    m_map.clear();

    //不同测量周期会影响趋势周期备选项的最小值
    ConfigObject* pObj = ConfigTreeObject::getConfigObject(MEASURE_DELEGATE_INTERVAL_INTERVAL);
    Measure_Interval testPeriod;
    if(pObj) {
        testPeriod = (Measure_Interval)pObj->getData().toUInt();
        switch(testPeriod) {
        case Measure_Interval_100ms:
            m_map = m_map_TrendTrendInterval_5s;
            break;
        case Measure_Interval_200ms:
            m_map = m_map_TrendTrendInterval_10s;
            break;
        case Measure_Interval_500ms:
            m_map = m_map_TrendTrendInterval_15s;
            break;
        case Measure_Interval_1s:
            m_map = m_map_TrendTrendInterval_30s;
            break;
        case Measure_Interval_2s:
            m_map = m_map_TrendTrendInterval_1min;
            break;
        case Measure_Interval_5s:
            m_map = m_map_TrendTrendInterval_5min;
            break;
        }
    }

    quint32 trendTimeDiv;
    Record_Mode Recmode;
    quint8 sw;
    quint8 countDIV;
    ConfigObject* qObj = ConfigTreeObject::getConfigObject(RECORD_DELEGATE_BASIC_MODE_VALUE);
    ConfigObject* rObj = ConfigTreeObject::getConfigObject(DISP_DELEGATE_TREND_INTERVAL_SWITCH);
    if(qObj && rObj) {
        Recmode = (Record_Mode)qObj->getData().toUInt();
        sw = rObj->getData().toUInt();

        if ((Record_Mode_Event == Recmode) && (sw == 0)) {
            //Do nothing 事件记录模式时，参考事件记录周期与趋势周期关系表，与配置无关，内定
        } else {
            Trend_Interval second = (Trend_Interval)ConfigTreeObject::\
                    getConfigObject(DISP_DELEGATE_TREND_INTERVAL_SECOND)->getData().toUInt();
            Trend_Interval backup;

            for(quint8 i=0;i<m_map.size();++i) {
                backup = (Trend_Interval)m_map.keys().at(i);
                if (1 == sw) {
                    trendTimeDiv = qMin(backup, second);
                } else {
                    trendTimeDiv = backup;
                }

                countDIV = trendTimeDiv>Trend_Interval_10s ? 30 : 50;
                quint32 x = trendTimeDiv * 1000;
                quint32 y = countDIV * testPeriod;
                if(x%y != 0) {
                    m_map.remove((quint32)backup);
                }
            }
        }
    }

    slotResetMenus();
}


WinDispTrendTrendIntervalSecond *WinDispTrendTrendIntervalSecond::instance()
{
    static WinDispTrendTrendIntervalSecond  instance;
    return &instance;
}

WinDispTrendTrendIntervalSecond::WinDispTrendTrendIntervalSecond(QWidget *parent)
    :WinChannelSelector(parent)
{
    m_map_TrendTrendInterval_5s.insert(Trend_Interval_5s, tr("5s"));
    m_map_TrendTrendInterval_5s.insert(Trend_Interval_10s, tr("10s"));
    m_map_TrendTrendInterval_5s.insert(Trend_Interval_15s, tr("15s"));
    m_map_TrendTrendInterval_5s.insert(Trend_Interval_30s, tr("30s"));
    m_map_TrendTrendInterval_5s.insert(Trend_Interval_1min, tr("1min"));
    m_map_TrendTrendInterval_5s.insert(Trend_Interval_2min, tr("2min"));
    m_map_TrendTrendInterval_5s.insert(Trend_Interval_5min, tr("5min"));
    m_map_TrendTrendInterval_5s.insert(Trend_Interval_10min, tr("10min"));
    m_map_TrendTrendInterval_5s.insert(Trend_Interval_15min, tr("15min"));
    m_map_TrendTrendInterval_5s.insert(Trend_Interval_20min, tr("20min"));
    m_map_TrendTrendInterval_5s.insert(Trend_Interval_30min, tr("30min"));
    m_map_TrendTrendInterval_5s.insert(Trend_Interval_1h, tr("1h"));
    m_map_TrendTrendInterval_5s.insert(Trend_Interval_2h, tr("2h"));
    m_map_TrendTrendInterval_5s.insert(Trend_Interval_4h, tr("4h"));
    m_map_TrendTrendInterval_5s.insert(Trend_Interval_10h, tr("10h"));


    m_map_TrendTrendInterval_10s.insert(Trend_Interval_10s, tr("10s"));
    m_map_TrendTrendInterval_10s.insert(Trend_Interval_15s, tr("15s"));
    m_map_TrendTrendInterval_10s.insert(Trend_Interval_30s, tr("30s"));
    m_map_TrendTrendInterval_10s.insert(Trend_Interval_1min, tr("1min"));
    m_map_TrendTrendInterval_10s.insert(Trend_Interval_2min, tr("2min"));
    m_map_TrendTrendInterval_10s.insert(Trend_Interval_5min, tr("5min"));
    m_map_TrendTrendInterval_10s.insert(Trend_Interval_10min, tr("10min"));
    m_map_TrendTrendInterval_10s.insert(Trend_Interval_15min, tr("15min"));
    m_map_TrendTrendInterval_10s.insert(Trend_Interval_20min, tr("20min"));
    m_map_TrendTrendInterval_10s.insert(Trend_Interval_30min, tr("30min"));
    m_map_TrendTrendInterval_10s.insert(Trend_Interval_1h, tr("1h"));
    m_map_TrendTrendInterval_10s.insert(Trend_Interval_2h, tr("2h"));
    m_map_TrendTrendInterval_10s.insert(Trend_Interval_4h, tr("4h"));
    m_map_TrendTrendInterval_10s.insert(Trend_Interval_10h, tr("10h"));


    m_map_TrendTrendInterval_15s.insert(Trend_Interval_15s, tr("15s"));
    m_map_TrendTrendInterval_15s.insert(Trend_Interval_30s, tr("30s"));
    m_map_TrendTrendInterval_15s.insert(Trend_Interval_1min, tr("1min"));
    m_map_TrendTrendInterval_15s.insert(Trend_Interval_2min, tr("2min"));
    m_map_TrendTrendInterval_15s.insert(Trend_Interval_5min, tr("5min"));
    m_map_TrendTrendInterval_15s.insert(Trend_Interval_10min, tr("10min"));
    m_map_TrendTrendInterval_15s.insert(Trend_Interval_15min, tr("15min"));
    m_map_TrendTrendInterval_15s.insert(Trend_Interval_20min, tr("20min"));
    m_map_TrendTrendInterval_15s.insert(Trend_Interval_30min, tr("30min"));
    m_map_TrendTrendInterval_15s.insert(Trend_Interval_1h, tr("1h"));
    m_map_TrendTrendInterval_15s.insert(Trend_Interval_2h, tr("2h"));
    m_map_TrendTrendInterval_15s.insert(Trend_Interval_4h, tr("4h"));
    m_map_TrendTrendInterval_15s.insert(Trend_Interval_10h, tr("10h"));


    m_map_TrendTrendInterval_30s.insert(Trend_Interval_30s, tr("30s"));
    m_map_TrendTrendInterval_30s.insert(Trend_Interval_1min, tr("1min"));
    m_map_TrendTrendInterval_30s.insert(Trend_Interval_2min, tr("2min"));
    m_map_TrendTrendInterval_30s.insert(Trend_Interval_5min, tr("5min"));
    m_map_TrendTrendInterval_30s.insert(Trend_Interval_10min, tr("10min"));
    m_map_TrendTrendInterval_30s.insert(Trend_Interval_15min, tr("15min"));
    m_map_TrendTrendInterval_30s.insert(Trend_Interval_20min, tr("20min"));
    m_map_TrendTrendInterval_30s.insert(Trend_Interval_30min, tr("30min"));
    m_map_TrendTrendInterval_30s.insert(Trend_Interval_1h, tr("1h"));
    m_map_TrendTrendInterval_30s.insert(Trend_Interval_2h, tr("2h"));
    m_map_TrendTrendInterval_30s.insert(Trend_Interval_4h, tr("4h"));
    m_map_TrendTrendInterval_30s.insert(Trend_Interval_10h, tr("10h"));


    m_map_TrendTrendInterval_1min.insert(Trend_Interval_1min, tr("1min"));
    m_map_TrendTrendInterval_1min.insert(Trend_Interval_2min, tr("2min"));
    m_map_TrendTrendInterval_1min.insert(Trend_Interval_5min, tr("5min"));
    m_map_TrendTrendInterval_1min.insert(Trend_Interval_10min, tr("10min"));
    m_map_TrendTrendInterval_1min.insert(Trend_Interval_15min, tr("15min"));
    m_map_TrendTrendInterval_1min.insert(Trend_Interval_20min, tr("20min"));
    m_map_TrendTrendInterval_1min.insert(Trend_Interval_30min, tr("30min"));
    m_map_TrendTrendInterval_1min.insert(Trend_Interval_1h, tr("1h"));
    m_map_TrendTrendInterval_1min.insert(Trend_Interval_2h, tr("2h"));
    m_map_TrendTrendInterval_1min.insert(Trend_Interval_4h, tr("4h"));
    m_map_TrendTrendInterval_1min.insert(Trend_Interval_10h, tr("10h"));


    m_map_TrendTrendInterval_5min.insert(Trend_Interval_5min, tr("5min"));
    m_map_TrendTrendInterval_5min.insert(Trend_Interval_10min, tr("10min"));
    m_map_TrendTrendInterval_5min.insert(Trend_Interval_15min, tr("15min"));
    m_map_TrendTrendInterval_5min.insert(Trend_Interval_20min, tr("20min"));
    m_map_TrendTrendInterval_5min.insert(Trend_Interval_30min, tr("30min"));
    m_map_TrendTrendInterval_5min.insert(Trend_Interval_1h, tr("1h"));
    m_map_TrendTrendInterval_5min.insert(Trend_Interval_2h, tr("2h"));
    m_map_TrendTrendInterval_5min.insert(Trend_Interval_4h, tr("4h"));
    m_map_TrendTrendInterval_5min.insert(Trend_Interval_10h, tr("10h"));

    slotResetMenus();
}

void WinDispTrendTrendIntervalSecond::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map.begin();
    for(;item != m_map.end();item++) {
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}

void WinDispTrendTrendIntervalSecond::initMenu()
{
    m_map.clear();

    //不同测量周期会影响趋势周期备选项的最小值
    ConfigObject* pObj = ConfigTreeObject::getConfigObject(MEASURE_DELEGATE_INTERVAL_INTERVAL);
    Measure_Interval testPeriod;
    if(pObj) {
        testPeriod = (Measure_Interval)pObj->getData().toUInt();
        switch(testPeriod) {
        case Measure_Interval_100ms:
            m_map = m_map_TrendTrendInterval_5s;
            break;
        case Measure_Interval_200ms:
            m_map = m_map_TrendTrendInterval_10s;
            break;
        case Measure_Interval_500ms:
            m_map = m_map_TrendTrendInterval_15s;
            break;
        case Measure_Interval_1s:
            m_map = m_map_TrendTrendInterval_30s;
            break;
        case Measure_Interval_2s:
            m_map = m_map_TrendTrendInterval_1min;
            break;
        case Measure_Interval_5s:
            m_map = m_map_TrendTrendInterval_5min;
            break;
        }
    }

    quint32 trendTimeDiv;
    Record_Mode Recmode;
    quint8 sw;
    quint8 countDIV;
    ConfigObject* qObj = ConfigTreeObject::getConfigObject(RECORD_DELEGATE_BASIC_MODE_VALUE);
    ConfigObject* rObj = ConfigTreeObject::getConfigObject(DISP_DELEGATE_TREND_INTERVAL_SWITCH);
    if(qObj && rObj) {
        Recmode = (Record_Mode)qObj->getData().toUInt();
        sw = rObj->getData().toUInt();

        if ((Record_Mode_Event == Recmode) && (sw == 0)) {
            //Do nothing 事件记录模式时，参考事件记录周期与趋势周期关系表，与配置无关，内定
        } else {
            Trend_Interval first = (Trend_Interval)ConfigTreeObject::\
                    getConfigObject(DISP_DELEGATE_TREND_INTERVAL_INTERVAL)->getData().toUInt();
            Trend_Interval backup;

            for(quint8 i=0;i<m_map.size();++i) {
                backup = (Trend_Interval)m_map.keys().at(i);
                if (1 == sw) {
                    trendTimeDiv = qMin(first, backup);
                } else {
                    trendTimeDiv = first;
                }

                countDIV = trendTimeDiv>Trend_Interval_10s ? 30 : 50;
                quint32 x = trendTimeDiv * 1000;
                quint32 y = countDIV * testPeriod;
                if(x%y != 0) {
                    m_map.remove((quint32)backup);
                }
            }
        }
    }

    slotResetMenus();
}


WinDispTrendTrendDirection *WinDispTrendTrendDirection::instance()
{
    static WinDispTrendTrendDirection  instance;
    return &instance;
}

WinDispTrendTrendDirection::WinDispTrendTrendDirection(QWidget *parent)
    :WinChannelSelector(parent)
{
    m_map_TrendTrendDirection.insert(0, tr("Horizontal"));
    m_map_TrendTrendDirection.insert(1, tr("Vertical"));

    slotResetMenus();
}

void WinDispTrendTrendDirection::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map_TrendTrendDirection.begin();
    for(;item != m_map_TrendTrendDirection.end();item++) {
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}


WinDispTrendTrendLine *WinDispTrendTrendLine::instance()
{
    static WinDispTrendTrendLine  instance;
    return &instance;
}

WinDispTrendTrendLine::WinDispTrendTrendLine(QWidget *parent)
    :WinChannelSelector(parent)
{
    m_map_TrendTrendLine.insert(0, tr("Normal"));
    m_map_TrendTrendLine.insert(1, tr("Thick"));
    m_map_TrendTrendLine.insert(2, tr("Thin"));

    slotResetMenus();
}

void WinDispTrendTrendLine::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map_TrendTrendLine.begin();
    for(;item != m_map_TrendTrendLine.end();item++) {
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}


WinDispTrendTrendGrid *WinDispTrendTrendGrid::instance()
{
    static WinDispTrendTrendGrid  instance;
    return &instance;
}

WinDispTrendTrendGrid::WinDispTrendTrendGrid(QWidget *parent)
    :WinChannelSelector(parent)
{
    m_map_TrendTrendGrid.insert(0, tr("Auto"));
    for(int i=4;i<13;++i) {
        QString istr = QString("%1").arg(i);
        m_map_TrendTrendGrid.insert(i, istr);
    }

    slotResetMenus();
}

void WinDispTrendTrendGrid::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map_TrendTrendGrid.begin();
    for(;item != m_map_TrendTrendGrid.end();item++) {
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}


WinDispTrendScaleDigit *WinDispTrendScaleDigit::instance()
{
    static WinDispTrendScaleDigit  instance;
    return &instance;
}

WinDispTrendScaleDigit::WinDispTrendScaleDigit(QWidget *parent)
    :WinChannelSelector(parent)
{
    m_map_TrendScaleDigit.insert(0, tr("Normal"));
    m_map_TrendScaleDigit.insert(1, tr("Fine"));

    slotResetMenus();
}

void WinDispTrendScaleDigit::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map_TrendScaleDigit.begin();
    for(;item != m_map_TrendScaleDigit.end();item++) {
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}


WinDispTrendScaleValue *WinDispTrendScaleValue::instance()
{
    static WinDispTrendScaleValue  instance;
    return &instance;
}

WinDispTrendScaleValue::WinDispTrendScaleValue(QWidget *parent)
    :WinChannelSelector(parent)
{
    m_map_TrendScaleValue.insert(0, tr("Mark"));
    m_map_TrendScaleValue.insert(1, tr("Bar graph"));

    slotResetMenus();
}

void WinDispTrendScaleValue::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map_TrendScaleValue.begin();
    for(;item != m_map_TrendScaleValue.end();item++) {
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}


WinDispTrendScaleMark *WinDispTrendScaleMark::instance()
{
    static WinDispTrendScaleMark  instance;
    return &instance;
}

WinDispTrendScaleMark::WinDispTrendScaleMark(QWidget *parent)
    :WinChannelSelector(parent)
{
    m_map_TrendScaleMark.insert(0, tr("0 digit"));
    m_map_TrendScaleMark.insert(3, tr("3 digit"));
    m_map_TrendScaleMark.insert(4, tr("4 digit"));

    slotResetMenus();
}

void WinDispTrendScaleMark::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map_TrendScaleMark.begin();
    for(;item != m_map_TrendScaleMark.end();item++) {
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}


WinDispTrendMsgWrite *WinDispTrendMsgWrite::instance()
{
    static WinDispTrendMsgWrite  instance;
    return &instance;
}

WinDispTrendMsgWrite::WinDispTrendMsgWrite(QWidget *parent)
    :WinChannelSelector(parent)
{
    m_map_TrendMsgWrite.insert(0, tr("Common"));
    m_map_TrendMsgWrite.insert(1, tr("Separate"));

    slotResetMenus();
}

void WinDispTrendMsgWrite::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map_TrendMsgWrite.begin();
    for(;item != m_map_TrendMsgWrite.end();item++) {
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}


WinDispMsgNum *WinDispMsgNum::instance()
{
    static WinDispMsgNum  instance;
    return &instance;
}

WinDispMsgNum::WinDispMsgNum(QWidget *parent)
    :WinChannelSelector(parent)
{
    m_map_MsgNum.insert(0, tr("1 - 10"));
    m_map_MsgNum.insert(1, tr("11 - 20"));
    m_map_MsgNum.insert(2, tr("21 - 30"));
    m_map_MsgNum.insert(3, tr("31 - 40"));
    m_map_MsgNum.insert(4, tr("41 - 50"));
    m_map_MsgNum.insert(5, tr("51 - 60"));
    m_map_MsgNum.insert(6, tr("61 - 70"));
    m_map_MsgNum.insert(7, tr("71 - 80"));
    m_map_MsgNum.insert(8, tr("81 - 90"));
    m_map_MsgNum.insert(9, tr("91 - 100"));

    slotResetMenus();
}

void WinDispMsgNum::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map_MsgNum.begin();
    for(;item != m_map_MsgNum.end();item++) {
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}


WinDispScreenLCDValue *WinDispScreenLCDValue::instance()
{
    static WinDispScreenLCDValue  instance;
    return &instance;
}

WinDispScreenLCDValue::WinDispScreenLCDValue(QWidget *parent)
    :WinChannelSelector(parent)
{
    m_map_ScreenLCDValue.insert(0, tr("Brightness 1"));
    m_map_ScreenLCDValue.insert(20, tr("Brightness 2"));
    m_map_ScreenLCDValue.insert(40, tr("Brightness 3"));
    m_map_ScreenLCDValue.insert(60, tr("Brightness 4"));
    m_map_ScreenLCDValue.insert(80, tr("Brightness 5"));
    m_map_ScreenLCDValue.insert(100, tr("Brightness 6"));

    slotResetMenus();
}

void WinDispScreenLCDValue::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map_ScreenLCDValue.begin();
    for(;item != m_map_ScreenLCDValue.end();item++) {
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}


WinDispScreenLightMode *WinDispScreenLightMode::instance()
{
    static WinDispScreenLightMode  instance;
    return &instance;
}

WinDispScreenLightMode::WinDispScreenLightMode(QWidget *parent)
    :WinChannelSelector(parent)
{
    m_map_ScreenLightMode.insert(0, tr("Off"));
    m_map_ScreenLightMode.insert(1, tr("Dimmer"));
    m_map_ScreenLightMode.insert(2, tr("Time-off"));

    slotResetMenus();
}

void WinDispScreenLightMode::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map_ScreenLightMode.begin();
    for(;item != m_map_ScreenLightMode.end();item++) {
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}


WinDispScreenLightTime *WinDispScreenLightTime::instance()
{
    static WinDispScreenLightTime  instance;
    return &instance;
}

WinDispScreenLightTime::WinDispScreenLightTime(QWidget *parent)
    :WinChannelSelector(parent)
{
    m_map_ScreenLightTime.insert(0, tr("1min"));
    m_map_ScreenLightTime.insert(1, tr("2min"));
    m_map_ScreenLightTime.insert(2, tr("5min"));
    m_map_ScreenLightTime.insert(3, tr("10min"));
    m_map_ScreenLightTime.insert(4, tr("30min"));
    m_map_ScreenLightTime.insert(5, tr("1h"));

    slotResetMenus();
}

void WinDispScreenLightTime::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map_ScreenLightTime.begin();
    for(;item != m_map_ScreenLightTime.end();item++) {
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}


WinDispScreenLightRestore *WinDispScreenLightRestore::instance()
{
    static WinDispScreenLightRestore  instance;
    return &instance;
}

WinDispScreenLightRestore::WinDispScreenLightRestore(QWidget *parent)
    :WinChannelSelector(parent)
{
    m_map_ScreenLightRestore.insert(0, tr("Key && Touch screen && Alarm"));
    m_map_ScreenLightRestore.insert(1, tr("Key && Touch screen"));

    slotResetMenus();
}

void WinDispScreenLightRestore::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map_ScreenLightRestore.begin();
    for(;item != m_map_ScreenLightRestore.end();item++) {
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}


WinDispScreenMonitorBack *WinDispScreenMonitorBack::instance()
{
    static WinDispScreenMonitorBack  instance;
    return &instance;
}

WinDispScreenMonitorBack::WinDispScreenMonitorBack(QWidget *parent)
    :WinChannelSelector(parent)
{
    m_map_ScreenMonitorBack.insert(0, tr("White"));
    m_map_ScreenMonitorBack.insert(1, tr("Black"));

    slotResetMenus();
}

void WinDispScreenMonitorBack::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map_ScreenMonitorBack.begin();
    for(;item != m_map_ScreenMonitorBack.end();item++) {
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}


WinDispScreenMonitorScroll *WinDispScreenMonitorScroll::instance()
{
    static WinDispScreenMonitorScroll  instance;
    return &instance;
}

WinDispScreenMonitorScroll::WinDispScreenMonitorScroll(QWidget *parent)
    :WinChannelSelector(parent)
{
    m_map_ScreenMonitorScroll.insert(0, tr("5s"));
    m_map_ScreenMonitorScroll.insert(1, tr("10s"));
    m_map_ScreenMonitorScroll.insert(2, tr("20s"));
    m_map_ScreenMonitorScroll.insert(3, tr("30s"));
    m_map_ScreenMonitorScroll.insert(4, tr("1min"));

    slotResetMenus();
}

void WinDispScreenMonitorScroll::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map_ScreenMonitorScroll.begin();
    for(;item != m_map_ScreenMonitorScroll.end();item++) {
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}


WinDispScreenMonitorJump *WinDispScreenMonitorJump::instance()
{
    static WinDispScreenMonitorJump  instance;
    return &instance;
}

WinDispScreenMonitorJump::WinDispScreenMonitorJump(QWidget *parent)
    :WinChannelSelector(parent)
{
    m_map_ScreenMonitorJump.insert(0, tr("Off"));
    m_map_ScreenMonitorJump.insert(1, tr("1min"));
    m_map_ScreenMonitorJump.insert(2, tr("2min"));
    m_map_ScreenMonitorJump.insert(3, tr("5min"));
    m_map_ScreenMonitorJump.insert(4, tr("10min"));
    m_map_ScreenMonitorJump.insert(5, tr("20min"));
    m_map_ScreenMonitorJump.insert(6, tr("30min"));
    m_map_ScreenMonitorJump.insert(7, tr("1h"));

    slotResetMenus();
}

void WinDispScreenMonitorJump::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map_ScreenMonitorJump.begin();
    for(;item != m_map_ScreenMonitorJump.end();item++) {
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}


WinDispScreenCalendar1st *WinDispScreenCalendar1st::instance()
{
    static WinDispScreenCalendar1st  instance;
    return &instance;
}

WinDispScreenCalendar1st::WinDispScreenCalendar1st(QWidget *parent)
    :WinChannelSelector(parent)
{
    m_map_ScreenCalendar1st.insert(0, tr("Sunday"));
    m_map_ScreenCalendar1st.insert(1, tr("Monday"));

    slotResetMenus();
}

void WinDispScreenCalendar1st::reloadMenu()
{
    QMap<quint32,QString>::iterator item = m_map_ScreenCalendar1st.begin();
    for(;item != m_map_ScreenCalendar1st.end();item++) {
        Menu *menu = MenuFactory::instance()->createMenu(item.value(), MenuType_item);
        menu->setMenuData(item.key());
        addMenu(menu);
    }
}

