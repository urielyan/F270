#ifndef WINMATHCONFIG_H
#define WINMATHCONFIG_H

#include "winchannelselector.h"
#include "../GlobalData/configtreeobject.h"
#include "../GlobalData/cfgGlobalDef.h"

class WinMathActionError : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinMathActionError *instance();

private:
    WinMathActionError(QWidget *parent = 0);
    void reloadMenu();

private:
    QMap<quint32,QString> m_map_ActionError;
};

class WinMathActionAction : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinMathActionAction *instance();

private:
    WinMathActionAction(QWidget *parent = 0);
    void reloadMenu();

private:
    QMap<quint32,QString> m_map_ActionAction;
};

class WinMathActionSumave : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinMathActionSumave *instance();

private:
    WinMathActionSumave(QWidget *parent = 0);
    void reloadMenu();

private:
    QMap<quint32,QString> m_map_ActionSumave;
};

class WinMathActionPP : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinMathActionPP *instance();

private:
    WinMathActionPP(QWidget *parent = 0);
    void reloadMenu();

private:
    QMap<quint32,QString> m_map_ActionPP;
};

class WinMathExpTLOGType : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinMathExpTLOGType *instance();

private:
    WinMathExpTLOGType(QWidget *parent = 0);
    void reloadMenu();

private:
    QMap<quint32,QString> m_map_ExpTLOGType;
};

class WinMathExpTLOGNo : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinMathExpTLOGNo *instance();

private:
    WinMathExpTLOGNo(QWidget *parent = 0);
    void reloadMenu();

private:
    QMap<quint32,QString> m_map_ExpTLOGNo;
};

class WinMathExpTLOGSum : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinMathExpTLOGSum *instance();

private:
    WinMathExpTLOGSum(QWidget *parent = 0);
    void reloadMenu();

private:
    QMap<quint32,QString> m_map_ExpTLOGSum;
};

class WinMathExpRollInter : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinMathExpRollInter *instance();

private:
    WinMathExpRollInter(QWidget *parent = 0);
    void reloadMenu();

private:
    QMap<quint32,QString> m_map_ExpRollInter;
};

class WinMathConstKNum : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinMathConstKNum *instance();

private:
    WinMathConstKNum(QWidget *parent = 0);
    void reloadMenu();

private:
    QMap<quint32,QString> m_map_ConstKNum;
};


class WinMathAlarmLvl1Type : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinMathAlarmLvl1Type *instance();

private:
    WinMathAlarmLvl1Type(QWidget *parent = 0);
    void reloadMenu();

private:
    QMap<quint32,QString> m_map_AlarmLvl1Type;
};

#endif // WINMATHCONFIG_H
