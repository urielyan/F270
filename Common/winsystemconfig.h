#ifndef WINSYSTEMCONFIG_H
#define WINSYSTEMCONFIG_H

#include "winchannelselector.h"
#include "../GlobalData/cfgGlobalDef_Type.h"
#include "../GlobalData/configtreeobject.h"

class WinSystemEnvironmentLanguage : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinSystemEnvironmentLanguage *instance();

private:
    WinSystemEnvironmentLanguage(QWidget *parent = 0);
     virtual void reloadMenu();


private:

    QMap<quint32, QString> m_map;
};

class WinSystemEnvironmentTemperature : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinSystemEnvironmentTemperature *instance();

private:
    WinSystemEnvironmentTemperature(QWidget *parent = 0);
     virtual void reloadMenu();


private:

    QMap<quint32, QString> m_map;
};

class WinSystemEnvironmentDecima : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinSystemEnvironmentDecima *instance();

private:
    WinSystemEnvironmentDecima(QWidget *parent = 0);
     virtual void reloadMenu();


private:

    QMap<quint32, QString> m_map;
};

class WinSystemEnvironmentData : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinSystemEnvironmentData *instance();

private:
    WinSystemEnvironmentData(QWidget *parent = 0);
     virtual void reloadMenu();


private:

    QMap<quint32, QString> m_map;
};

class WinSystemEnvironmentDelimiter : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinSystemEnvironmentDelimiter *instance();

private:
    WinSystemEnvironmentDelimiter(QWidget *parent = 0);
     virtual void reloadMenu();


private:

    QMap<quint32, QString> m_map;
};


class WinSystemEnvironmentMonth : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinSystemEnvironmentMonth *instance();

private:
    WinSystemEnvironmentMonth(QWidget *parent = 0);
     virtual void reloadMenu();


private:

    QMap<quint32, QString> m_map;
};

class WinSystemAlarmHold : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinSystemAlarmHold *instance();

private:
    WinSystemAlarmHold(QWidget *parent = 0);
     virtual void reloadMenu();
    void initMenu();


private:
    QMap<quint32, QString> m_map;
    QMap<quint32, QString> m_mapHold;
    QMap<quint32, QString> m_mapNo;
};

class WinSystemTimeHour : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinSystemTimeHour *instance();

private:
    WinSystemTimeHour(QWidget *parent = 0);
     virtual void reloadMenu();


private:

    QMap<quint32, QString> m_map;
};

class WinSystemIntervalType : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinSystemIntervalType *instance();

private:
    WinSystemIntervalType(QWidget *parent = 0);
     virtual void reloadMenu();


private:

    QMap<quint32, QString> m_map;
};

class WinSystemIntervalANDOR : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinSystemIntervalANDOR *instance();

private:
    WinSystemIntervalANDOR(QWidget *parent = 0);
     virtual void reloadMenu();


private:

    QMap<quint32, QString> m_map;
};

class WinSystemRelay : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinSystemRelay *instance();

private:
    WinSystemRelay(QWidget *parent = 0);
     virtual void reloadMenu();


private:

    QMap<quint32, QString> m_map;
};

class WinSystemCommon : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinSystemCommon *instance();

private:
    WinSystemCommon(QWidget *parent = 0);
     virtual void reloadMenu();
    void initMenu();


private:

    QMap<quint32, QString> m_map;
};

class WinBatchCommon : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinBatchCommon *instance();

private:
    WinBatchCommon(QWidget *parent = 0);
     virtual void reloadMenu();
    void initMenu();


private:

    QMap<quint32, QString> m_map;
};

class WinReportComm : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinReportComm *instance();

private:
    WinReportComm(QWidget *parent = 0);
    virtual void reloadMenu();
    void initMenu();


private:

    QMap<quint32, QString> m_map;
};

class WinTimerTimerComm : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinTimerTimerComm *instance();

private:
    WinTimerTimerComm(QWidget *parent = 0);
     virtual void reloadMenu();
    void initMenu();


private:

    QMap<quint32, QString> m_map;
};

class WinTimerMatchTimer : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinTimerMatchTimer *instance();

private:
    WinTimerMatchTimer(QWidget *parent = 0);
     virtual void reloadMenu();
    void initMenu();


private:

    QMap<quint32, QString> m_map;
};
#endif // WINSYSTEMCONFIG_H
