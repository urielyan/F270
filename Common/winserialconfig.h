#ifndef WINSERIALCONFIG_H
#define WINSERIALCONFIG_H
#include <QByteArray>
#include <QVariant>
#include "winchannelselector.h"
#include "../GlobalData/configtreeobject.h"
#include "../GlobalData/cfgGlobalDef.h"
#include "winnetworkconfig.h"
#include "../GlobalData/cfgchannel.h"

typedef enum _SerialBasicFunction {
    SerialBasicFunction_off = 0,
    SerialBasicFunction_Normal,
    SerialBasicFunction_ModbusMaster,
    SerialBasicFunction_ModbusSlave,
}SerialBasicFunction;

class WinSerialBasicReceiveFunction : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinSerialBasicReceiveFunction *instance();

private:
    WinSerialBasicReceiveFunction(QWidget *parent = 0);
    virtual void reloadMenu();


private:
//    QList<QString> m_list_function;
    QMap<quint32, QString> m_map_function;
};

class WinSerialModbusCommandChanelType : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinSerialModbusCommandChanelType *instance();

private:
    WinSerialModbusCommandChanelType(QWidget *parent = 0);
    virtual void reloadMenu();
    void initMenu();


private:
    QVariant data;
    QMap<quint32, QString> m_map_Type;
    QMap<quint32, QString> m_map_TypeR;
    QMap<quint32, QString> m_map_TypeW;
    QMap<quint32, QString> m_map_TypeHide;
};


typedef enum _SerialBasicBuad {
    SerialBasicBuad_1200 = 0,
    SerialBasicBuad_2400,
    SerialBasicBuad_4800,
    SerialBasicBuad_9600,
    SerialBasicBuad_19200,
    SerialBasicBuad_38400,
    SerialBasicBuad_57600,
    SerialBasicBuad_115200,
}SerialBasicBuad;

class WinSerialBasicBaudFunction : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinSerialBasicBaudFunction *instance();

private:
    WinSerialBasicBaudFunction(QWidget *parent = 0);
    virtual void reloadMenu();


private:
//    QList<QString> m_list_baud;
    QMap<quint32, QString> m_map_buad;
};

typedef enum _SerialBasicParity {
    SerialBasicParity_None = 0,
    SerialBasicParity_Even,
    SerialBasicParity_Odd,
}SerialBasicParity;

class WinSerialBasicParityFunction : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinSerialBasicParityFunction *instance();

private:
    WinSerialBasicParityFunction(QWidget *parent = 0);
    virtual void reloadMenu();


private:
//    QList<QString> m_list_parity;
    QMap<quint32, QString> m_map_parity;
};


typedef enum _SerialBasicStop {
    SerialBasicStop_1bit = 0,
    SerialBasicStop_2bit,
}SerialBasicStop;

class WinSerialBasicStopFunction : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinSerialBasicStopFunction *instance();

private:
    WinSerialBasicStopFunction(QWidget *parent = 0);
    virtual void reloadMenu();


private:
//    QList<QString> m_list_stop;
    QMap<quint32, QString> m_map_stop;
};

typedef enum _SerialBasicDataLength {
    SerialBasicDataLength_7bit = 0,
    SerialBasicDataLength_8bit,
}SerialBasicDataLength;

class WinSerialBasicLengthFunction : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinSerialBasicLengthFunction *instance();

private:
    WinSerialBasicLengthFunction(QWidget *parent = 0);
    virtual void reloadMenu();


private:
//    QList<QString> m_list_length;
    QMap<quint32, QString> m_map_length;
};

typedef enum _SerialModbusTimeout {
    SerialModbusTimeout_100ms = 0,
    SerialModbusTimeout_200ms,
    SerialModbusTimeout_250ms,
    SerialModbusTimeout_500ms,
    SerialModbusTimeout_1s,
    SerialModbusTimeout_2s,
    SerialModbusTimeout_5s,
    SerialModbusTimeout_10s,
    SerialModbusTimeout_1min,
}SerialModbusTimeout;

class WinSerialModbusTimeout : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinSerialModbusTimeout *instance();

private:
    WinSerialModbusTimeout(QWidget *parent = 0);
    virtual void reloadMenu();


private:
//    QList<QString> m_list_timeout;
    QMap<quint32, QString> m_map_timeout;
};

typedef enum _SerialModbusGap {
    SerialModbusGap_off = 0,
    SerialModbusGap_5ms,
    SerialModbusGap_10ms,
    SerialModbusGap_20ms,
    SerialModbusGap_50ms,
    SerialModbusGap_100ms,
}SerialModbusGap;

class WinSerialModbusGap : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinSerialModbusGap *instance();

private:
    WinSerialModbusGap(QWidget *parent = 0);
    virtual void reloadMenu();


private:
//    QList<QString> m_list_gap;
    QMap<quint32, QString> m_map_gap;
};

typedef enum _SerialModbusRetry {
    SerialModbusRetry_off = 0,
    SerialModbusRetry_1,
    SerialModbusRetry_2,
    SerialModbusRetry_3,
    SerialModbusRetry_4,
    SerialModbusRetry_5,
    SerialModbusRetry_10,
    SerialModbusRetry_20,
}SerialModbusRetry;

class WinSerialModbusRetry : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinSerialModbusRetry *instance();

private:
    WinSerialModbusRetry(QWidget *parent = 0);
    virtual void reloadMenu();


private:
//    QList<QString> m_list_retry;
    QMap<quint32, QString> m_map_retry;
};

#endif // WINSERIALCONFIG_H
