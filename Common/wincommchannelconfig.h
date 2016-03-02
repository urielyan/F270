#ifndef WINCOMMCHANNELCONFIG_H
#define WINCOMMCHANNELCONFIG_H

#include <QByteArray>
#include "winchannelselector.h"


class WinCommChannelChoose : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinCommChannelChoose *instance();

private:
    WinCommChannelChoose(QWidget *parent = 0);
    virtual void reloadMenu();


private:

    QMap<quint32, QString> m_map;
};

class WinCommChannelDecimalPlace : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinCommChannelDecimalPlace *instance();

private:
    WinCommChannelDecimalPlace(QWidget *parent = 0);
    virtual void reloadMenu();


private:

    QMap<quint32, QString> m_map;
};

typedef enum _CommChannelAlarmType {
    WinCommChannelAlarmType_H = 1,
    WinCommChannelAlarmType_L = 2,
    WinCommChannelAlarmType_T = 5,
    WinCommChannelAlarmType_t = 6,
}CommChannelAlarmType;

class WinCommChannelAlarmType : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinCommChannelAlarmType *instance();

private:
    WinCommChannelAlarmType(QWidget *parent = 0);
    virtual void reloadMenu();


private:

    QMap<quint32, QString> m_map;
};

typedef enum _CommChannelValuePower {
    CommChannelValuePower_Last = 0,
    CommChannelValuePower_Preset,
}CommChannelValuePower;

class WinCommChannelValuePower : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinCommChannelValuePower *instance();

private:
    WinCommChannelValuePower(QWidget *parent = 0);
    virtual void reloadMenu();


private:

    QMap<quint32, QString> m_map;
};

#endif // WINCOMMCHANNELCONFIG_H
