#ifndef WINEVENTCONFIG_H
#define WINEVENTCONFIG_H

#include "winchannelselector.h"
#include "../GlobalData/configtreeobject.h"
#include "../GlobalData/cfgGlobalDef.h"

class WinEventGrpNum : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinEventGrpNum *instance();

private:
    WinEventGrpNum(QWidget *parent = 0);
    void reloadMenu();

private:
    QMap<quint32,QString> m_map_GrpNum;
};

class WinEventEventType : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinEventEventType *instance();

private:
    WinEventEventType(QWidget *parent = 0);
    void reloadMenu();
    void initMenu();

private:
    QMap<quint32,QString> m_map;
    QMap<quint32,QString> m_map_EventType;
};

class WinEventEventAlarmlevel : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinEventEventAlarmlevel *instance();

private:
    WinEventEventAlarmlevel(QWidget *parent = 0);
    void reloadMenu();

private:
    QMap<quint32,QString> m_map_EventAlarmlevel;
};

class WinEventEventDetails : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinEventEventDetails *instance();

private:
    WinEventEventDetails(QWidget *parent = 0);
    void reloadMenu();

private:
    QMap<quint32,QString> m_map_EventDetails;
};

class WinEventEventMode : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinEventEventMode *instance();

private:
    WinEventEventMode(QWidget *parent = 0);
    void reloadMenu();
    void initMenu();

private:
    QMap<quint32,QString> m_map_EventMode;
    QMap<quint32,QString> m_map;
};

class WinEventActionType : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinEventActionType *instance();

private:
    WinEventActionType(QWidget *parent = 0);
    void reloadMenu();
    void initMenu();

private:
    QMap<quint32,QString> m_map_ActionType;
    QMap<quint32,QString> m_map_ActionType_Both;
    QMap<quint32,QString> m_map;
};

class WinEventActionDetail : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinEventActionDetail *instance();

private:
    WinEventActionDetail(QWidget *parent = 0);
    void reloadMenu();
    void initMenu();

private:
    QMap<quint32,QString> m_map_ActionDetail_Record;
    QMap<quint32,QString> m_map_ActionDetail_Math;
    QMap<quint32,QString> m_map_ActionDetail_Msg;
    QMap<quint32,QString> m_map;
};

#endif // WINEVENTCONFIG_H
