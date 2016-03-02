#ifndef CFGCONFIG_SERIAL_H
#define CFGCONFIG_SERIAL_H

#include <QObject>
#include <QVariant>
#include "cfgGlobalDef_SerialClient.h"
#include "./GlobalData/configobject.h"

/****************************************  serial 22400 - 22500  *************************************************/
#define Serial_Start_ID                                 22400
#define Serial_End_ID                                   22499

#define Serial_Basic_Start_ID                           22400
#define Serial_Basic_End_ID                             22419

#define Serial_Modbus_Start_ID                          22420
#define Serial_Modbus_End_ID                            22499


//serial
#define ID_Serial                                       22400

#define ID_Serial_Basic                                 22401       //title

#define ID_Serial_Basic_Receiver                        22402   //group
#define ID_Serial_Basic_Receiver_Mode                   22403    //串口工作模式
#define ID_Serial_Basic_Receiver_Addr                   22404    //串口Modbus地址

#define ID_Serial_Basic_Transfer                        22410   //group
#define ID_Serial_Basic_Transfer_BaudRate               22411   //波特率
#define ID_Serial_Basic_Transfer_Parity                 22412   //校验
#define ID_Serial_Basic_Transfer_StopBits               22413   //停止位
#define ID_Serial_Basic_Transfer_DataLen                22414   //数据长度

#define ID_Serial_Modbus                                22420   //title

#define ID_Serial_Modbus_Basic                          22421   //title

#define ID_Serial_Modbus_Basic_Server                   22422   //group
#define ID_Serial_Modbus_Basic_Server_OnOff             22423   //Modbus客户端功能开关

#define ID_Serial_Modbus_Basic_Comm                     22430   //group
#define ID_Serial_Modbus_Basic_Comm_Interval            22431    //采样间隔，单位:ms,范围：100ms/200ms/500ms/1s/2s/5s/10s/20s/30s/1min,默认：1s
#define ID_Serial_Modbus_Basic_Comm_Timeout             22432   //通信超时，单位:ms,范围：100ms/200ms/250ms/500ms/1s/2s/5s/10s/1min,默认：1s
#define ID_Serial_Modbus_Basic_Comm_CmdInterval         22433   //命令间隔，单位：ms，范围：0(off)/5ms/10ms/20ms/50ms/100ms,默认：off

#define ID_Serial_Modbus_Basic_Recovery                 22440   //group
#define ID_Serial_Modbus_Basic_Recovery_Cnt             22441   //重试次数，范围：0(off)/1/2/3/4/5/10/20, 默认：1
#define ID_Serial_Modbus_Basic_Recovery_WaitTime        22442    //通信恢复等待时间，单位:s，范围：0(off)/5s/10s/30s/1min/2min/5min,默认:5s

//serial modbus cmd
#define ID_Serial_Modbus_Cmd                            22450   //title
#define ID_Serial_Modbus_Cmd_ServerNum                  22451   //number

#define ID_Serial_Modbus_Cmd_Cmd                        22460   //group
#define ID_Serial_Modbus_Cmd_Cmd_Type                   22461   //命令类型：0-off, 1-读取， 2-写入
#define ID_Serial_Modbus_Cmd_Cmd_NO                     22462    //命令对应的服务器(从机）号，范围:1~247,默认1
#define ID_Serial_Modbus_Cmd_Cmd_RegType                22463   //寄存器类型
#define ID_Serial_Modbus_Cmd_Cmd_RegNo                  22464   //寄存器号
#define ID_Serial_Modbus_Cmd_Cmd_ChType                 22465   //通道类型，读取时：通信通道；写入时：输入输出通道/运算通道/通信通道
#define ID_Serial_Modbus_Cmd_Cmd_StartNO                22466   //与通道类型匹配的起始通道号
#define ID_Serial_Modbus_Cmd_Cmd_EndNO                  22467       //与通道类型匹配的结束通道号


class CfgConfig_Serial : public QObject
{
    Q_OBJECT
public:
    static CfgConfig_Serial * instance();
    void setDeviceSerialConfig(SERIAL_CONFIG_MODBUSCLIENT & serial) { p_serialCfg = &serial;}

    QVariant getSerialData(quint32 ID, quint32 group);      //get data
    QString getSerialObjStr(quint32 ID, quint32 group = 0);         //get obj str
    QString getSerialObjValStr(quint32 ID, quint32 group, ConfigObject * cfg);         //get obj val str
    void setSerialData(quint32 ID, quint32 group);      //save data

    QString getSerialModbusChValObjStr(quint32 code);

private:
    explicit CfgConfig_Serial(QObject *parent = 0);

    //get data
    QVariant getSerialBasicData(quint32 ID, quint32 group);
    QVariant getSerialModbusData(quint32 ID, quint32 group);

    QVariant getSerialBasicBaudRateData(quint32 type);
    QVariant getSerialModbusIntervalData(quint32 type);
    QVariant getSerialModbusTimeoutData(quint32 type);
    QVariant getSerialModbusCmdIntertData(quint32 type);
    QVariant getSerialModbusRecTimeData(quint32 type);


    //get obj str
    QString getSerialBasicObjStr(quint32 ID, quint32 group = 0);
    QString getSerialModbusObjStr(quint32 ID, quint32 group = 0);

    //get obj val str
    QString getSerialBasicObjValStr(quint32 ID, quint32 group, ConfigObject *cfg);
    QString getSerialModbusObjValStr(quint32 ID, quint32 group, ConfigObject *cfg);

    QString getSerialBasicReciverModeObjValStr(quint32 type);
    QString getSerialBasicBaudRateObjValStr(quint32 type);
    QString getSerialModbusIntervalObjValStr(quint32 type);
    QString getSerialModbusTimeoutObjValStr(quint32 type);
    QString getSerialModbusCmdInterObjValStr(quint32 type);
    QString getSerialModbusretryObjValStr(quint32 type);
    QString getSerialModbusRecTimeObjValStr(quint32 type);
    QString getModbusRegTypeValStr(quint32 type);
    QString getModbusChValObjStr(quint32 code, bool flag, quint32 group);

    //save
    void setSerialBasicData(quint32 ID, quint32 group);
    void setSerialModbusData(quint32 ID, quint32 group);

    void setSerialBasicBaudRateData(Serial_BaudRate &data);
    void setSerialModbusIntervalData(quint32 &data);
    void setSerialModbusTimeoutData(quint32 &data);
    void setSerialCommIntervalData(quint32 &data);
    void setSerialRecTimeData(quint32 &data);

    void setSerialModbusChNum(quint32 &ch, quint32 type, quint32 coder);
    void setModbusChType(quint32 group, Channel_Type &chType);

signals:

public slots:

private:
    SERIAL_CONFIG_MODBUSCLIENT * p_serialCfg;
};

#endif // CFGCONFIG_SERIAL_H
