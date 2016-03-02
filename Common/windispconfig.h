/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：windispconfig.h
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
#ifndef WINDISPCONFIG_H
#define WINDISPCONFIG_H

#include "winchannelselector.h"
#include "../GlobalData/configtreeobject.h"
#include "../GlobalData/cfgGlobalDef.h"

class WinDispGrpNum : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinDispGrpNum *instance();

private:
    WinDispGrpNum(QWidget *parent = 0);
    void reloadMenu();

private:
    QMap<quint32,QString> m_map_GrpNum;
};


class WinDispTrendTrendInterval : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinDispTrendTrendInterval *instance();

private:
    WinDispTrendTrendInterval(QWidget *parent = 0);
    void reloadMenu();
    void initMenu();

private:
    QMap<quint32,QString> m_map;
    QMap<quint32,QString> m_map_TrendTrendInterval_5s;
    QMap<quint32,QString> m_map_TrendTrendInterval_10s;
    QMap<quint32,QString> m_map_TrendTrendInterval_15s;
    QMap<quint32,QString> m_map_TrendTrendInterval_30s;
    QMap<quint32,QString> m_map_TrendTrendInterval_1min;
    QMap<quint32,QString> m_map_TrendTrendInterval_5min;
};


class WinDispTrendTrendIntervalSecond : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinDispTrendTrendIntervalSecond *instance();

private:
    WinDispTrendTrendIntervalSecond(QWidget *parent = 0);
    void reloadMenu();
    void initMenu();

private:
    QMap<quint32,QString> m_map;
    QMap<quint32,QString> m_map_TrendTrendInterval_5s;
    QMap<quint32,QString> m_map_TrendTrendInterval_10s;
    QMap<quint32,QString> m_map_TrendTrendInterval_15s;
    QMap<quint32,QString> m_map_TrendTrendInterval_30s;
    QMap<quint32,QString> m_map_TrendTrendInterval_1min;
    QMap<quint32,QString> m_map_TrendTrendInterval_5min;
};


class WinDispTrendTrendDirection : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinDispTrendTrendDirection *instance();

private:
    WinDispTrendTrendDirection(QWidget *parent = 0);
    void reloadMenu();

private:
    QMap<quint32,QString> m_map_TrendTrendDirection;
};


class WinDispTrendTrendLine : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinDispTrendTrendLine *instance();

private:
    WinDispTrendTrendLine(QWidget *parent = 0);
    void reloadMenu();

private:
    QMap<quint32,QString> m_map_TrendTrendLine;
};


class WinDispTrendTrendGrid : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinDispTrendTrendGrid *instance();

private:
    WinDispTrendTrendGrid(QWidget *parent = 0);
    void reloadMenu();

private:
    QMap<quint32,QString> m_map_TrendTrendGrid;
};


class WinDispTrendScaleDigit : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinDispTrendScaleDigit *instance();

private:
    WinDispTrendScaleDigit(QWidget *parent = 0);
    void reloadMenu();

private:
    QMap<quint32,QString> m_map_TrendScaleDigit;
};


class WinDispTrendScaleValue : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinDispTrendScaleValue *instance();

private:
    WinDispTrendScaleValue(QWidget *parent = 0);
    void reloadMenu();

private:
    QMap<quint32,QString> m_map_TrendScaleValue;
};


class WinDispTrendScaleMark : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinDispTrendScaleMark *instance();

private:
    WinDispTrendScaleMark(QWidget *parent = 0);
    void reloadMenu();

private:
    QMap<quint32,QString> m_map_TrendScaleMark;
};


class WinDispTrendMsgWrite : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinDispTrendMsgWrite *instance();

private:
    WinDispTrendMsgWrite(QWidget *parent = 0);
    void reloadMenu();

private:
    QMap<quint32,QString> m_map_TrendMsgWrite;
};

class WinDispMsgNum : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinDispMsgNum *instance();

private:
    WinDispMsgNum(QWidget *parent = 0);
    void reloadMenu();

private:
    QMap<quint32,QString> m_map_MsgNum;
};


class WinDispScreenLCDValue : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinDispScreenLCDValue *instance();

private:
    WinDispScreenLCDValue(QWidget *parent = 0);
    void reloadMenu();

private:
    QMap<quint32,QString> m_map_ScreenLCDValue;
};


class WinDispScreenLightMode : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinDispScreenLightMode *instance();

private:
    WinDispScreenLightMode(QWidget *parent = 0);
    void reloadMenu();

private:
    QMap<quint32,QString> m_map_ScreenLightMode;
};


class WinDispScreenLightTime : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinDispScreenLightTime *instance();

private:
    WinDispScreenLightTime(QWidget *parent = 0);
    void reloadMenu();

private:
    QMap<quint32,QString> m_map_ScreenLightTime;
};


class WinDispScreenLightRestore : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinDispScreenLightRestore *instance();

private:
    WinDispScreenLightRestore(QWidget *parent = 0);
    void reloadMenu();

private:
    QMap<quint32,QString> m_map_ScreenLightRestore;
};


class WinDispScreenMonitorBack : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinDispScreenMonitorBack *instance();

private:
    WinDispScreenMonitorBack(QWidget *parent = 0);
    void reloadMenu();

private:
    QMap<quint32,QString> m_map_ScreenMonitorBack;
};


class WinDispScreenMonitorScroll : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinDispScreenMonitorScroll *instance();

private:
    WinDispScreenMonitorScroll(QWidget *parent = 0);
    void reloadMenu();

private:
    QMap<quint32,QString> m_map_ScreenMonitorScroll;
};


class WinDispScreenMonitorJump : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinDispScreenMonitorJump *instance();

private:
    WinDispScreenMonitorJump(QWidget *parent = 0);
    void reloadMenu();

private:
    QMap<quint32,QString> m_map_ScreenMonitorJump;
};


class WinDispScreenCalendar1st : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinDispScreenCalendar1st *instance();

private:
    WinDispScreenCalendar1st(QWidget *parent = 0);
    void reloadMenu();

private:
    QMap<quint32,QString> m_map_ScreenCalendar1st;
};


#endif // WINDISPCONFIG_H
