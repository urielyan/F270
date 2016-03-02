#ifndef WINNETWORKCONFIG_H
#define WINNETWORKCONFIG_H
#include <QByteArray>
#include "winchannelselector.h"

/**************modbus client************/
typedef enum _NetworkModbusInterval{
    NetworkModbusInterval_100ms = 0,
    NetworkModbusInterval_200ms,
    NetworkModbusInterval_500ms,
    NetworkModbusInterval_1s,
    NetworkModbusInterval_2s,
    NetworkModbusInterval_5s,
    NetworkModbusInterval_10s,
    NetworkModbusInterval_20s,
    NetworkModbusInterval_30s,
    NetworkModbusInterval_1min,
}NetworkModbusInterval;

class WinModbusClientInterval : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinModbusClientInterval *instance();

private:
    WinModbusClientInterval(QWidget *parent = 0);
    virtual void reloadMenu();


private:
    //QList<QString> m_list_interval;
    QMap<quint32, QString> m_map_interval;
};


class WinModbusClientConnectTime : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinModbusClientConnectTime *instance();

private:
    WinModbusClientConnectTime(QWidget *parent = 0);
    virtual void reloadMenu();


private:
//    QList<QString> m_list_connectTime;
    QMap<quint32, QString> m_map_connectTime;
};

typedef enum _NetworkModbusWaitTime{
    NetworkModbusWaitTime_off = 0,
    NetworkModbusWaitTime_5s,
    NetworkModbusWaitTime_10s,
    NetworkModbusWaitTime_30s,
    NetworkModbusWaitTime_1min,
    NetworkModbusWaitTime_2min,
    NetworkModbusWaitTime_5min,
}NetworkModbusWaitTime;

class WinModbusClientWaitTime : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinModbusClientWaitTime *instance();

private:
    WinModbusClientWaitTime(QWidget *parent = 0);
    virtual void reloadMenu();


private:
    QMap<quint32, QString> m_map_waitTime;
};

class WinModbusServerSettingNumber : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinModbusServerSettingNumber *instance();

private:
    WinModbusServerSettingNumber(QWidget *parent = 0);
    virtual void reloadMenu();


private:
    QMap<quint32, QString> m_map_number;
};

class WinModbusServerDelayRes : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinModbusServerDelayRes *instance();

private:
    WinModbusServerDelayRes(QWidget *parent = 0);
    void reloadMenu();

private:
    QMap<quint32, QString> m_map_DelayRes;
};

class WinModbusCommandNumber : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinModbusCommandNumber *instance();

private:
    WinModbusCommandNumber(QWidget *parent = 0);
    virtual void reloadMenu();


private:
    QMap<quint32, QString> m_map_number;
};

typedef enum _NetworkModbusCommandType{
    NetworkModbusCommandType_Off,
    NetworkModbusCommandType_Read,
    NetworkModbusCommandType_Write,
}NetworkModbusCommandType;

class WinModbusCommandType : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinModbusCommandType *instance();

private:
    WinModbusCommandType(QWidget *parent = 0);
    virtual void reloadMenu();


private:
    QMap<quint32, QString> m_map_Type;
};

typedef enum _NetworkModbusCommandDataType{

    NetworkModbusCommandDataType_Int16,
    NetworkModbusCommandDataType_UInt16,
    NetworkModbusCommandDataType_Int32_B,
    NetworkModbusCommandDataType_Int32_L,
    NetworkModbusCommandDataType_UInt32_B,
    NetworkModbusCommandDataType_UInt32_L,
    NetworkModbusCommandDataType_Float_B,
    NetworkModbusCommandDataType_Float_L,
    NetworkModbusCommandDataType_Bit,
}NetworkModbusCommandDataType;

class WinModbusCommandDataType : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinModbusCommandDataType *instance();

private:
    WinModbusCommandDataType(QWidget *parent = 0);
    virtual void reloadMenu();


private:
    QMap<quint32, QString> m_map_Type;
};

typedef enum _NetworkModbusCommandChanelType{
    NetworkModbusCommandChanelType_IO = 1,
    NetworkModbusCommandChanelType_COMM = 4,
    NetworkModbusCommandChanelType_MATH = 5,
}NetworkModbusCommandChanelType;

class WinModbusCommandChanelType : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinModbusCommandChanelType *instance();

private:
    WinModbusCommandChanelType(QWidget *parent = 0);
    virtual void reloadMenu();
    void initMenu();


private:
    QVariant data;
    QMap<quint32, QString> m_map_Type;
    QMap<quint32, QString> m_map_TypeR;
    QMap<quint32, QString> m_map_TypeW;
    QMap<quint32, QString> m_map_TypeHide;
};

/******************SMTP******************************/
typedef enum _SMTPAUTH{
    SMTP_AUTH_OFF,
    SMTP_AUTH_SMTP,
    SMTP_AUTH_POP,
    SMTP_AUTH_APOP,
}SMTPAUTH;


class WinSMTPAuthentication : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinSMTPAuthentication *instance();

private:
    WinSMTPAuthentication(QWidget *parent = 0);
    virtual void reloadMenu();


private:
//    QList<QString> m_list_authentication;
    QMap<quint32, QString> m_map_authentication;
};

/******************E-mail******************************/
typedef enum _EMAILAlarmNotify{
    EMAILAlarmNotify_OFF,
    EMAILAlarmNotify_R1,
    EMAILAlarmNotify_R2,
    EMAILAlarmNotify_R1_R2,
}EMAILAlarmNotify;
class WinEMailAlarmNotification : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinEMailAlarmNotification *instance();

private:
    WinEMailAlarmNotification(QWidget *parent = 0);
    virtual void reloadMenu();


private:
//    QList<QString> m_list_alarmNotification;
    QMap<quint32, QString> m_map_alarmNotification;
};

typedef enum _EMAILSET{
    EMAILSET_UpTo50,
    EMAILSET_IO,
    EMAILSET_Math,
    EMAILSET_Comm,
}EMAILSET;

class WinEMailChannelSet : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinEMailChannelSet *instance();

private:
    WinEMailChannelSet(QWidget *parent = 0);
    virtual void reloadMenu();


private:
//    QList<QString> m_list_channelSet;
    QMap<quint32, QString> m_map_channelSet;
};

typedef enum _EMAILInteral{
    EMAILInteral_1h,
    EMAILInteral_2h,
    EMAILInteral_3h,
    EMAILInteral_4h,
    EMAILInteral_6h,
    EMAILInteral_8h,
    EMAILInteral_12h,
    EMAILInteral_24h,
}EMAILInteral;

class WinEMailInteral : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinEMailInteral *instance();

private:
    WinEMailInteral(QWidget *parent = 0);
    virtual void reloadMenu();


private:
//    QList<QString> m_list_interal;
    QMap<quint32, QString> m_map_interal;
};

class WinEMailTimeHour : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinEMailTimeHour *instance();

private:
    WinEMailTimeHour(QWidget *parent = 0);
    virtual void reloadMenu();


private:
//    QList<QString> m_list_hour;
    QMap<quint32, QString> m_map_hour;
};

class WinEMailTimeMinute : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinEMailTimeMinute *instance();

private:
    WinEMailTimeMinute(QWidget *parent = 0);
    virtual void reloadMenu();


private:
//    QList<QString> m_list_minute;
    QMap<quint32, QString> m_map_minute;
};

/**********************************SNTP****************************/

typedef enum _SNTPInterval{
    SNTPInterval_6h,
    SNTPInterval12h,
    SNTPInterval24h,
}SNTPInterval;

class WinSNTPInterval : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinSNTPInterval *instance();

private:
    WinSNTPInterval(QWidget *parent = 0);
    virtual void reloadMenu();


private:
//    QList<QString> m_list_interval;
    QMap<quint32, QString> m_map_interval;
};


typedef enum _SNTPTimeout{
    SNTPTimeout_10s,
    SNTPTimeout_30s,
    SNTPTimeout_90s,
}SNTPTimeout;
class WinSNTPTimeout : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinSNTPTimeout *instance();

private:
    WinSNTPTimeout(QWidget *parent = 0);
    virtual void reloadMenu();


private:
//    QList<QString> m_list_timeout;
    QMap<quint32, QString> m_map_timeout;
};

#endif // WINNETWORKCONFIG_H
