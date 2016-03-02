#include "cfgconfig_serial.h"
#include "./GlobalData/cfgchannel.h"
#include "../Common/devboard.h"
#include "./Common/winserialconfig.h"

CfgConfig_Serial *CfgConfig_Serial::instance()
{
    static CfgConfig_Serial data;
    return &data;
}

CfgConfig_Serial::CfgConfig_Serial(QObject *parent) :
    QObject(parent)
{
}

/*
 * get serial data by ID and group
*/
QVariant CfgConfig_Serial::getSerialData(quint32 ID, quint32 group)
{
    if (ID >= Serial_Basic_Start_ID && ID <= Serial_Basic_End_ID)
        return getSerialBasicData(ID, group);
    else if (ID >= Serial_Modbus_Start_ID && ID <= Serial_Modbus_End_ID)
        return getSerialModbusData(ID, group);
    return QVariant();
}



/*
 * get serial obj str by ID and group
*/
QString CfgConfig_Serial::getSerialObjStr(quint32 ID, quint32 /*group*/)
{
    if (ID >= Serial_Basic_Start_ID && ID <= Serial_Basic_End_ID)
        return getSerialBasicObjStr(ID/*, group*/);
    else if (ID >= Serial_Modbus_Start_ID && ID <= Serial_Modbus_End_ID)
        return getSerialModbusObjStr(ID/*, group*/);
    return NULL;
}


// get serial obj val str by ID,group and cfg
QString CfgConfig_Serial::getSerialObjValStr(quint32 ID, quint32 group, ConfigObject *cfg)
{
    if (ID >= Serial_Basic_Start_ID && ID <= Serial_Basic_End_ID)
        return getSerialBasicObjValStr(ID, group, cfg);
    else if (ID >= Serial_Modbus_Start_ID && ID <= Serial_Modbus_End_ID)
        return getSerialModbusObjValStr(ID, group, cfg);
    return NULL;
}


//save
void CfgConfig_Serial::setSerialData(quint32 ID, quint32 group)
{
    if (ID >= Serial_Basic_Start_ID && ID <= Serial_Basic_End_ID)
        setSerialBasicData(ID, group);
    else if (ID >= Serial_Modbus_Start_ID && ID <= Serial_Modbus_End_ID)
        setSerialModbusData(ID, group);
}




/***********************                          get data                           *****************************/
/*
 * get serial basic data by ID
*/
QVariant CfgConfig_Serial::getSerialBasicData(quint32 ID, quint32 /*group*/)
{
    switch (ID) {
    case ID_Serial_Basic_Receiver_Mode:     //串口工作模式
        return p_serialCfg->serialMode;
        break;
    case ID_Serial_Basic_Receiver_Addr:     //串口Modbus地址
        return p_serialCfg->serialAddr;
        break;

    case ID_Serial_Basic_Transfer_BaudRate:     //波特率
        return getSerialBasicBaudRateData(p_serialCfg->serialBaudrate);
        break;
    case ID_Serial_Basic_Transfer_Parity:        //校验
        return p_serialCfg->serialParity;
        break;
    case ID_Serial_Basic_Transfer_StopBits:     //停止位
        return p_serialCfg->serialStopBit;
        break;
    case ID_Serial_Basic_Transfer_DataLen:       //数据长度
        return p_serialCfg->serilaDataLen;
        break;
    default:
        break;
    }
    return QVariant();
}

/*
 * get serial modbus data by ID and group
*/
QVariant CfgConfig_Serial::getSerialModbusData(quint32 ID, quint32 group)
{
    switch (ID) {
    case ID_Serial_Modbus_Basic_Server_OnOff:   //Modbus客户端功能开关
        return p_serialCfg->clientOnOff;
        break;

    case ID_Serial_Modbus_Basic_Comm_Interval:  //采样间隔，单位:ms,范围：100ms/200ms/500ms/1s/2s/5s/10s/20s/30s/1min,默认：1s
        return getSerialModbusIntervalData(p_serialCfg->commInterval);
        break;
    case ID_Serial_Modbus_Basic_Comm_Timeout:  //通信超时，单位:ms,范围：100ms/200ms/250ms/500ms/1s/2s/5s/10s/1min,默认：1s
        return getSerialModbusTimeoutData(p_serialCfg->commTimeOut);
        break;
    case ID_Serial_Modbus_Basic_Comm_CmdInterval:  //命令间隔，单位：ms，范围：0(off)/5ms/10ms/20ms/50ms/100ms,默认：off
        return getSerialModbusCmdIntertData(p_serialCfg->commCmdInterval);
        break;

    case ID_Serial_Modbus_Basic_Recovery_Cnt: //重试次数，范围：0(off)/1/2/3/4/5/10/20, 默认：1
    {
        int type = p_serialCfg->recoveryCount;
        if (type <= 5)
            return type;
        else if (type == 10)
            return 6;
        else if (type == 20)
            return 7;
    }
        break;
    case ID_Serial_Modbus_Basic_Recovery_WaitTime:  //通信恢复等待时间，单位:s，范围：0(off)/5s/10s/30s/1min/2min/5min,默认:5s
        return getSerialModbusRecTimeData(p_serialCfg->recoveryTime);
        break;

    case ID_Serial_Modbus_Cmd_ServerNum:      //命令类型：0-off, 1-读取， 2-写入
        return group + 1;
        break;

    case ID_Serial_Modbus_Cmd_Cmd_Type:      //命令类型：0-off, 1-读取， 2-写入
        return p_serialCfg->cmdType[group];
        break;
    case ID_Serial_Modbus_Cmd_Cmd_NO:         //命令对应的服务器号，范围:1~32
        return p_serialCfg->cmdServeNo[group];
        break;
    case ID_Serial_Modbus_Cmd_Cmd_RegType:         //寄存器类型
        return p_serialCfg->cmdRegType[group];
        break;
    case ID_Serial_Modbus_Cmd_Cmd_RegNo:         //寄存器号
        return p_serialCfg->cmdRegNo[group];
        break;
    case ID_Serial_Modbus_Cmd_Cmd_ChType:        //通道类型，读取时：通信通道；写入时：输入输出通道/运算通道/通信通道
        return p_serialCfg->cmdChType[group];
        break;
    case ID_Serial_Modbus_Cmd_Cmd_StartNO:         //与通道类型匹配的起始通道号
        return p_serialCfg->cmdStartNo[group];
        break;
    case ID_Serial_Modbus_Cmd_Cmd_EndNO:         //与通道类型匹配的结束通道号
        return p_serialCfg->cmdEndNo[group];
        break;
    default:
        break;
    }
    return QVariant();
}

/*
 * 通道从底层得到的配置值得到配置的时哪个选项
*/
QVariant CfgConfig_Serial::getSerialBasicBaudRateData(quint32 type)
{
    switch (type) {
    case Serial_BaudRate_1200:
        return SerialBasicBuad_1200;
        break;
    case Serial_BaudRate_2400:
        return SerialBasicBuad_2400;
        break;
    case Serial_BaudRate_4800:
        return SerialBasicBuad_4800;
        break;
    case Serial_BaudRate_9600:
        return SerialBasicBuad_9600;
        break;
    case Serial_BaudRate_19200:
        return SerialBasicBuad_19200;
        break;
    case Serial_BaudRate_38400:
        return SerialBasicBuad_38400;
        break;
    case Serial_BaudRate_57600:
        return SerialBasicBuad_57600;
        break;
    case Serial_BaudRate_115200:
        return SerialBasicBuad_115200;
        break;
    default:
        break;
    }
    return QVariant();
}

/*
 * 根据树上的周期配置值得到周期配置的值描述
*/
QVariant CfgConfig_Serial::getSerialModbusIntervalData(quint32 type)
{
    switch (type) {
    case 1:
        return 0;
        break;
    case 100:
        return 0;
        break;
    case 200:
        return 1;
        break;
    case 500:
        return 2;
        break;
    case 1000:
        return 3;
        break;
    case 2000:
        return 4;
        break;
    case 5000:
        return 5;
        break;
    case 10000:
        return 6;
        break;
    case 20000:
        return 7;
        break;
    case 30000:
        return 8;
        break;
    case 60000:
        return 9;
        break;
    default:
        break;
    }
    return QVariant();
}

QVariant CfgConfig_Serial::getSerialModbusTimeoutData(quint32 type)
{
    switch (type) {
    case 100:
        return 0;
        break;
    case 200:
        return 1;
        break;
    case 250:
        return 2;
        break;
    case 500:
        return 3;
        break;
    case 1000:
        return 4;
        break;
    case 2000:
        return 5;
        break;
    case 5000:
        return 6;
        break;
    case 10000:
        return 7;
        break;
    case 60000:
        return 8;
        break;
    default:
        break;
    }
    return QVariant();
}

QVariant CfgConfig_Serial::getSerialModbusCmdIntertData(quint32 type)
{
    switch (type) {
    case 0:
        return 0;
        break;
    case 5:
        return 1;
        break;
    case 10:
        return 2;
        break;
    case 20:
        return 3;
        break;
    case 50:
        return 4;
        break;
    case 100:
        return 5;
        break;
    default:
        break;
    }
    return QVariant();
}

QVariant CfgConfig_Serial::getSerialModbusRecTimeData(quint32 type)
{
    switch (type) {
    case 0:
        return 0;
        break;
    case 5:
        return 1;
        break;
    case 10:
        return 2;
        break;
    case 30:
        return 3;
        break;
    case 60:
        return 4;
        break;
    case 120:
        return 5;
        break;
    case 300:
        return 6;
        break;
    default:
        break;
    }
    return QVariant();
}




/***********************                          get obj str                          *****************************/

//get obj str by ID
QString CfgConfig_Serial::getSerialBasicObjStr(quint32 ID, quint32 /*group*/)
{
    switch (ID) {
    case ID_Serial:
        return tr("Communication (Serial) settings");
        break;
    case ID_Serial_Basic:
        return tr("Basic settings");
        break;
    case ID_Serial_Basic_Receiver:
        return tr("Reciver");
        break;
    case ID_Serial_Basic_Receiver_Mode: //串口工作模式
        return tr("Function");
        break;
    case ID_Serial_Basic_Receiver_Addr: //串口Modbus地址
        return tr("Address");
        break;

    case ID_Serial_Basic_Transfer:
        return tr("Data transfer");
        break;
    case ID_Serial_Basic_Transfer_BaudRate: //波特率
        return tr("Baud rate");
        break;
    case ID_Serial_Basic_Transfer_Parity:
        return tr("Parity bit");
        break;
    case ID_Serial_Basic_Transfer_StopBits:
        return tr("Stop bit");
        break;
    case ID_Serial_Basic_Transfer_DataLen:
        return tr("Data length");
        break;

    default:
        break;
    }
    return NULL;
}

QString CfgConfig_Serial::getSerialModbusObjStr(quint32 ID, quint32 /*group*/)
{
    switch (ID) {
    case ID_Serial_Modbus:          //title
        return tr("Modbus settings");
        break;

    case ID_Serial_Modbus_Basic:          //title
        return tr("Basic settings");
        break;
    case ID_Serial_Modbus_Basic_Server:  //group
        return tr("Master function");
        break;
    case ID_Serial_Modbus_Basic_Server_OnOff:
        return tr("On/Off");
        break;

    case ID_Serial_Modbus_Basic_Comm:  //group
        return tr("Communication");
        break;
    case ID_Serial_Modbus_Basic_Comm_Interval:
        return tr("Interval");
        break;
    case ID_Serial_Modbus_Basic_Comm_Timeout:
        return tr("Communication timeout");
        break;
    case ID_Serial_Modbus_Basic_Comm_CmdInterval:
        return tr("Gap between messages");
        break;

    case ID_Serial_Modbus_Basic_Recovery:      //group
        return tr("Recovery action");
        break;
    case ID_Serial_Modbus_Basic_Recovery_Cnt:
        return tr("Retry");
        break;
    case ID_Serial_Modbus_Basic_Recovery_WaitTime:
        return tr("Wait time");
        break;

    case ID_Serial_Modbus_Cmd:      //title
        return tr("Command settings");
        break;
    case ID_Serial_Modbus_Cmd_ServerNum:
        return tr("Master command number");
        break;
    case ID_Serial_Modbus_Cmd_Cmd:       //group
        return tr("Command settings");
        break;
    case ID_Serial_Modbus_Cmd_Cmd_Type:
        return tr("Type");
        break;
    case ID_Serial_Modbus_Cmd_Cmd_NO:
        return tr("Slave");
        break;
    case ID_Serial_Modbus_Cmd_Cmd_RegType:
        return tr("Data type");
        break;
    case ID_Serial_Modbus_Cmd_Cmd_RegNo:
        return tr("Register");
        break;
    case ID_Serial_Modbus_Cmd_Cmd_ChType:
        return tr("Channel type");
        break;
    case ID_Serial_Modbus_Cmd_Cmd_StartNO:
        return tr("First-CH");
        break;
    case ID_Serial_Modbus_Cmd_Cmd_EndNO:
        return tr("Last-CH");
        break;
    default:
        break;
    }
    return NULL;
}



/***********************                          get obj val str                          *****************************/

// get serial basic obj val str by ID ,group and cfg
QString CfgConfig_Serial::getSerialBasicObjValStr(quint32 ID, quint32 /*group*/, ConfigObject *cfg)
{
    switch (ID) {
    case ID_Serial_Basic_Receiver_Mode:
        return getSerialBasicReciverModeObjValStr(cfg->getData().toInt());
        break;

    case ID_Serial_Basic_Transfer_BaudRate:
        return getSerialBasicBaudRateObjValStr(cfg->getData().toInt());
        break;
    case ID_Serial_Basic_Transfer_Parity:     //校验
    {
        int type = cfg->getData().toInt();
        if (type == 0)
            return tr("None");
        else if (type == 1)
            return tr("Even");
        else if (type == 2)
            return tr("Odd");
    }
        break;
    case ID_Serial_Basic_Transfer_StopBits:     //停止位
        return cfg->getData().toInt() == 0 ? tr("1 bit") : tr("2 bit");
        break;
    case ID_Serial_Basic_Transfer_DataLen:     //停止位
        return cfg->getData().toInt() == 0 ? tr("7 bit") : tr("8 bit");
        break;

    default:
        break;
    }
    return cfg->getData().toString();
}

// get serial modbus obj val str by ID ,group and cfg
QString CfgConfig_Serial::getSerialModbusObjValStr(quint32 ID, quint32 /*group*/, ConfigObject *cfg)
{
    switch (ID) {
    case ID_Serial_Modbus_Basic_Server_OnOff:
        return cfg->getData().toInt() == 0 ? tr("Off") : tr("On");
        break;

    case ID_Serial_Modbus_Basic_Comm_Interval:
        return getSerialModbusIntervalObjValStr(cfg->getData().toInt());
        break;

    case ID_Serial_Modbus_Basic_Comm_Timeout:
        return getSerialModbusTimeoutObjValStr(cfg->getData().toInt());
        break;

    case ID_Serial_Modbus_Basic_Comm_CmdInterval:
        return getSerialModbusCmdInterObjValStr(cfg->getData().toInt());
        break;
    case ID_Serial_Modbus_Basic_Recovery_Cnt:       //重试次数
        return getSerialModbusretryObjValStr(cfg->getData().toInt());
        break;
    case ID_Serial_Modbus_Basic_Recovery_WaitTime:       //通信恢复等待时间
        return getSerialModbusRecTimeObjValStr(cfg->getData().toInt());
        break;

    case ID_Serial_Modbus_Cmd_Cmd_Type:
    {
        int type = cfg->getData().toUInt();
        if (type == 0)
            return tr("Off");
        else if (type == 1)
            return tr("Read");
        else if (type == 2)
            return tr("Write");
    }
        break;
    case ID_Serial_Modbus_Cmd_Cmd_RegType:       //寄存器类型
        return getModbusRegTypeValStr(cfg->getData().toInt());
        break;
    case ID_Serial_Modbus_Cmd_Cmd_ChType:    //通道类型，读取时：通信通道；写入时：输入输出通道/运算通道/通信通道
    {
        int type = cfg->getData().toInt();
        if (type == 1 || type == 2 || type == 3)
            return tr("I/O channel");
        else if (type == 4)
            return tr("Communication channel");
        else if (type ==5)
            return tr("Math channel");
    }
        break;
    case ID_Serial_Modbus_Cmd_Cmd_StartNO:
    case ID_Serial_Modbus_Cmd_Cmd_EndNO:
        return getSerialModbusChValObjStr(cfg->getData().toInt());
        break;
    default:
        break;
    }
    return cfg->getData().toString();
}


/*
 * 得到串口工作模式的值描述
*/
QString CfgConfig_Serial::getSerialBasicReciverModeObjValStr(quint32 type)
{
    switch (type) {
    case Serial_Mode_Off:    //不使用
        return tr("Off");
        break;
    case Serial_Mode_STD:    //标准模式
        return tr("Normal");
        break;
    case Serial_Mode_Modbus_Master: //Modbus主机功能
        return tr("Modbus master");
        break;
    case Serial_Mode_Modbus_Slave:  //Modbus从机功能
        return tr("Modbus slave");
        break;
    default:
        break;
    }
    return NULL;
}

/*
 * 得到串口工作模式的值描述
*/
QString CfgConfig_Serial::getSerialBasicBaudRateObjValStr(quint32 type)
{
    switch (type) {
    case SerialBasicBuad_1200:
        return QString::number(Serial_BaudRate_1200);
        break;
    case SerialBasicBuad_2400:
        return QString::number(Serial_BaudRate_2400);
        break;
    case SerialBasicBuad_4800:
        return QString::number(Serial_BaudRate_4800);
        break;
    case SerialBasicBuad_9600:
        return QString::number(Serial_BaudRate_9600);
        break;
    case SerialBasicBuad_19200:
        return QString::number(Serial_BaudRate_19200);
        break;
    case SerialBasicBuad_38400:
        return QString::number(Serial_BaudRate_38400);
        break;
    case SerialBasicBuad_57600:
        return QString::number(Serial_BaudRate_57600);
        break;
    case SerialBasicBuad_115200:
        return QString::number(Serial_BaudRate_115200);
        break;
    default:
        break;
    }
    return NULL;
}


/*
 * 根据树上的周期配置值得到周期配置的值描述
*/
QString CfgConfig_Serial::getSerialModbusIntervalObjValStr(quint32 type)
{
    switch (type) {
    case 0:
        return tr("100 ms");
        break;
    case 1:
        return tr("200ms");
        break;
    case 2:
        return tr("500 ms");
        break;
    case 3:
        return tr("1 s");
        break;
    case 4:
        return tr("2 s");
        break;
    case 5:
        return tr("5 s");
        break;
    case 6:
        return tr("10 s");
        break;
    case 7:
        return tr("20 s");
        break;
    case 8:
        return tr("30 s");
        break;
    case 9:
        return tr("1 min");
        break;
    default:
        break;
    }
    return NULL;
}


QString CfgConfig_Serial::getSerialModbusTimeoutObjValStr(quint32 type)
{
    switch (type) {
    case 0:
        return tr("100 ms");
        break;
    case 1:
        return tr("200ms");
        break;
    case 2:
        return tr("250 ms");
        break;
    case 3:
        return tr("500 ms");
        break;
    case 4:
        return tr("1 s");
        break;
    case 5:
        return tr("2 s");
        break;
    case 6:
        return tr("5 s");
        break;
    case 7:
        return tr("10 s");
        break;
    case 8:
        return tr("1 min");
        break;
    default:
        break;
    }
    return NULL;
}

QString CfgConfig_Serial::getSerialModbusCmdInterObjValStr(quint32 type)
{
    switch (type) {
    case 0:
        return tr("Off");
        break;
    case 1:
        return tr("5 ms");
        break;
    case 2:
        return tr("10 ms");
        break;
    case 3:
        return tr("20 ms");
        break;
    case 4:
        return tr("50 ms");
        break;
    case 5:
        return tr("100 ms");
        break;
    default:
        break;
    }
    return NULL;
}

QString CfgConfig_Serial::getSerialModbusretryObjValStr(quint32 type)
{
    switch (type) {
    case 0:
        return tr("Off");
        break;
    case 1:
        return tr("Once");
        break;
    case 2:
        return tr("Twice");
        break;
    case 3:
        return tr("3 times");
        break;
    case 4:
        return tr("4 times");
        break;
    case 5:
        return tr("5 times");
        break;
    case 6:
        return tr("10 times");
        break;
    case 7:
        return tr("20 times");
        break;
    default:
        break;
    }
    return NULL;
}

//通信恢复时间
QString CfgConfig_Serial::getSerialModbusRecTimeObjValStr(quint32 type)
{
    switch (type) {
    case 0:
        return tr("Off");
        break;
    case 1:
        return tr("5 s");
        break;
    case 2:
        return tr("10 s");
        break;
    case 3:
        return tr("30 s");
        break;
    case 4:
        return tr("1 min");
        break;
    case 5:
        return tr("2 min");
        break;
    case 6:
        return tr("5 min");
        break;
    default:
        break;
    }
    return NULL;
}

//network modbus reg type obj str
QString CfgConfig_Serial::getModbusRegTypeValStr(quint32 type)
{
    switch (type) {
    case Register_Type_INT16:
        return "INT16";
        break;
    case Register_Type_UINT16:
        return "UINT16";
        break;
    case Register_Type_INT32B:
        return "INT32_B";
        break;
    case Register_Type_INT32L:
        return "INT32_L";
        break;
    case Register_Type_UINT32B:
        return "UINT32_B";
        break;
    case Register_Type_UINT32L:
        return "UINT32_L";
        break;
//    case Register_Type_FLOAT:
//        return "FLOAT";
//        break;
    case Register_Type_FLOATB:
        return "FLOAT_B";
        break;
    case Register_Type_FLOATL:
        return "FLOAT_L";
        break;
    case Register_Type_BIT:
        return "BIT";
        break;
    default:
        break;
    }
    return NULL;
}



/***********************                          save data                           *****************************/

void CfgConfig_Serial::setSerialBasicData(quint32 ID, quint32 /*group*/)
{
    switch (ID) {
    case ID_Serial_Basic_Receiver_Mode:     //串口工作模式
        p_serialCfg->serialMode = (Serial_Mode_Enum)ConfigTreeObject::getConfigData(SERIAL_Settings_BASIC_FUNCTION).toUInt();
        break;
    case ID_Serial_Basic_Receiver_Addr:     //串口Modbus地址
        p_serialCfg->serialAddr = ConfigTreeObject::getConfigData(SERIAL_Settings_BASIC_ADDR).toUInt();
        break;
    case ID_Serial_Basic_Transfer_BaudRate:
        setSerialBasicBaudRateData(p_serialCfg->serialBaudrate);
        break;
    case ID_Serial_Basic_Transfer_Parity:
        p_serialCfg->serialParity = (Serial_Parity)ConfigTreeObject::getConfigData(SERIAL_Settings_BASIC_PARITY).toUInt();
        break;
    case ID_Serial_Basic_Transfer_StopBits:
        p_serialCfg->serialStopBit = (Serial_StopBits)ConfigTreeObject::getConfigData(SERIAL_Settings_BASIC_STOP).toUInt();
        break;
    case ID_Serial_Basic_Transfer_DataLen:
        p_serialCfg->serilaDataLen = (Serial_DataLen)ConfigTreeObject::getConfigData(SERIAL_Settings_BASIC_DATALENGTH).toUInt();
        break;
    default:
        break;
    }
}

void CfgConfig_Serial::setSerialModbusData(quint32 ID, quint32 group)
{
    switch (ID) {
    case ID_Serial_Modbus_Basic_Server_OnOff:
        p_serialCfg->clientOnOff = ConfigTreeObject::getConfigData(SERIAL_Settings_MODBUS_FUNCTION).toUInt();
        break;
    case ID_Serial_Modbus_Basic_Comm_Interval:
        setSerialModbusIntervalData(p_serialCfg->commInterval);
        break;
    case ID_Serial_Modbus_Basic_Comm_Timeout:
        setSerialModbusTimeoutData(p_serialCfg->commTimeOut);
        break;
    case ID_Serial_Modbus_Basic_Comm_CmdInterval:
        setSerialCommIntervalData(p_serialCfg->commCmdInterval);
        break;

    case ID_Serial_Modbus_Basic_Recovery_Cnt:
        p_serialCfg->recoveryCount = ConfigTreeObject::getConfigData(SERIAL_Settings_MODBUS_RECOVER).toUInt();
        break;
    case ID_Serial_Modbus_Basic_Recovery_WaitTime:
        setSerialRecTimeData(p_serialCfg->recoveryTime);
        break;

    case ID_Serial_Modbus_Cmd_Cmd_Type:      //cmd
        p_serialCfg->cmdType[group] = ConfigTreeObject::getConfigData(QString(SERIAL_Settings_COMMAND_TYPE).arg(group)).toUInt();
        break;
    case ID_Serial_Modbus_Cmd_Cmd_NO:
        p_serialCfg->cmdServeNo[group] = ConfigTreeObject::getConfigData(QString(SERIAL_Settings_COMMAND_SLAVE).arg(group)).toInt();
        break;
    case ID_Serial_Modbus_Cmd_Cmd_RegType:
        p_serialCfg->cmdRegType[group] = (Register_Type)ConfigTreeObject::getConfigData(QString(SERIAL_Settings_COMMAND_DATATYPE).arg(group)).toInt();
        break;
    case ID_Serial_Modbus_Cmd_Cmd_RegNo:
        p_serialCfg->cmdRegNo[group] = ConfigTreeObject::getConfigData(QString(SERIAL_Settings_COMMAND_REGISTER).arg(group)).toInt();
        break;

    case ID_Serial_Modbus_Cmd_Cmd_ChType:
        setModbusChType(group, p_serialCfg->cmdChType[group]);
    case ID_Serial_Modbus_Cmd_Cmd_StartNO:
    case ID_Serial_Modbus_Cmd_Cmd_EndNO:
    {
        p_serialCfg->cmdStartNo[group] = ConfigTreeObject::getConfigData(QString(SERIAL_Settings_COMMAND_FIRSTCH).arg(group)).toUInt();
        p_serialCfg->cmdEndNo[group] = ConfigTreeObject::getConfigData(QString(SERIAL_Settings_COMMAND_LASTCH).arg(group)).toUInt();
    }
        break;
    default:
        break;
    }
}


void CfgConfig_Serial::setSerialBasicBaudRateData(Serial_BaudRate &data)
{
    int type = ConfigTreeObject::getConfigData(SERIAL_Settings_BASIC_BUAD).toUInt();
    switch (type) {
    case SerialBasicBuad_1200:
        data = Serial_BaudRate_1200;
        break;
    case SerialBasicBuad_2400:
        data = Serial_BaudRate_2400;
        break;
    case SerialBasicBuad_4800:
        data = Serial_BaudRate_4800;
        break;
    case SerialBasicBuad_9600:
        data = Serial_BaudRate_9600;
        break;
    case SerialBasicBuad_19200:
        data = Serial_BaudRate_19200;
        break;
    case SerialBasicBuad_38400:
        data = Serial_BaudRate_38400;
        break;
    case SerialBasicBuad_57600:
        data = Serial_BaudRate_57600;
        break;
    case SerialBasicBuad_115200:
        data = Serial_BaudRate_115200;
        break;
    default:
        break;
    }
}

void CfgConfig_Serial::setSerialModbusIntervalData(quint32 &data)
{
    int type  = ConfigTreeObject::getConfigData(SERIAL_Settings_MODBUS_INTERVAL).toUInt();
    switch (type) {
    case 0:
        data = 100;
        break;
    case 1:
        data = 200;
        break;
    case 2:
        data = 500;
        break;
    case 3:
        data = 1000;
        break;
    case 4:
        data = 2000;
        break;
    case 5:
        data = 5000;
        break;
    case 6:
        data = 10000;
        break;
    case 7:
        data = 20000;
        break;
    case 8:
        data = 30000;
        break;
    case 9:
        data = 60000;
        break;
    default:
        break;
    }
}

void CfgConfig_Serial::setSerialModbusTimeoutData(quint32 &data)
{
    int type  = ConfigTreeObject::getConfigData(SERIAL_Settings_MODBUS_TIMEOUT).toUInt();
    switch (type) {
    case 0:
        data = 100;
        break;
    case 1:
        data = 200;
        break;
    case 2:
        data = 250;
        break;
    case 3:
        data = 500;
        break;
    case 4:
        data = 1000;
        break;
    case 5:
        data = 2000;
        break;
    case 6:
        data = 5000;
        break;
    case 7:
        data = 10000;
        break;
    case 8:
        data = 60000;
        break;
    default:
        break;
    }
}


void CfgConfig_Serial::setSerialCommIntervalData(quint32 &data)
{
    int type  = ConfigTreeObject::getConfigData(SERIAL_Settings_MODBUS_GAP).toUInt();
    switch (type) {
    case 0:
        data = 0;
        break;
    case 1:
        data = 5;
        break;
    case 2:
        data = 10;
        break;
    case 3:
        data = 20;
        break;
    case 4:
        data = 50;
        break;
    case 5:
        data = 100;
        break;
    default:
        break;
    }
}

void CfgConfig_Serial::setSerialRecTimeData(quint32 &data)
{
    int type  = ConfigTreeObject::getConfigData(SERIAL_Settings_MODBUS_WAITTIME).toUInt();
    switch (type) {
    case 0:
        data = 0;
        break;
    case 1:
        data = 5;
        break;
    case 2:
        data = 10;
        break;
    case 3:
        data = 30;
        break;
    case 4:
        data = 60;
        break;
    case 5:
        data = 120;
        break;
    case 6:
        data = 300;
        break;
    default:
        break;
    }
}





/*
 * 通道通道编码得到界面上显示的相应通道编码的样式
*/
QString CfgConfig_Serial::getSerialModbusChValObjStr(quint32 code)
{
    int type = CHANNEL_TYPE(code);
    switch (type) {
    case Channel_Type_AI:
        return CfgChannel::instance()->getExistChannelAI().key(code);
        break;
    case Channel_Type_DI:
        return CfgChannel::instance()->getExistChannelDI().key(code);
        break;
    case Channel_Type_DO:
        return CfgChannel::instance()->getExistChannelDO().key(code);
        break;
    case Channel_Type_Comm:
    {
        QString str = CfgChannel::instance()->getExistChannelComm().key(code);
        str = "0" + str.right(3);
        return str;
    }
        break;
    case Channel_Type_Math:
    {
        QString str = CfgChannel::instance()->getExistChannelMath().key(code);
        str = "0" + str.right(3);
        return str;
    }
        break;
    default:
        break;
    }
    return "0001";
}

/*
 * 设置板卡通道号
*/
void CfgConfig_Serial::setSerialModbusChNum(quint32 &ch, quint32 type, quint32 coder)
{
    int unit = coder / 1000;
    int mod = coder / 100 % 10;
    int num = coder % 100;
    switch (type) {
    case Channel_Type_Off:
        return ;
        break;
    case Channel_Type_AI:
    {
        if (unit != 0)
            mod = (mod - 9) / 6 + 1;
        ch = CHANNEL_AI_CODER(unit, mod, num);
    }
        break;
    case Channel_Type_DI:
        ch = CHANNEL_DI_CODER(unit, mod, num);
        break;
    case Channel_Type_DO:
        ch = CHANNEL_DO_CODER(unit, mod, num);
        break;
    case Channel_Type_Comm:
    case Channel_Type_Math:
        ch = coder;
        break;
    default:
        break;
    }
}


/*
 * 设置板卡类型
*/
void CfgConfig_Serial::setModbusChType(quint32 group, Channel_Type &chType)
{
//    Channel_Type type = (Channel_Type)ConfigTreeObject::getConfigData(QString(SERIAL_Settings_COMMAND_CHANELTYPE).arg(group)).toInt();
//    switch (type) {
//    case Channel_Type_Off:
//        chType = Channel_Type_Off;
//        break;
//    case Channel_Type_AI:
//    case Channel_Type_DO:
//    case Channel_Type_DI:
//    {
//        quint32 channel = ConfigTreeObject::getConfigData(QString(SERIAL_Settings_COMMAND_FIRSTCH).arg(group)).toUInt();
//        if (channel / 1000 != 0)       //AI扩展板卡
//             chType = Channel_Type_AI;
//        else
//        {
//            int model = channel / 100 % 10;
//            chType = (Channel_Type)(CfgChannel::instance()->getList_BoardIO().value(model))->getBoardType();
//        }
//    }
//        break;
//    case Channel_Type_Comm:
//        chType = Channel_Type_Comm;
//        break;
//    case Channel_Type_Math:
//        chType = Channel_Type_Math;
//        break;
//    default:
//        break;
//    }
    Channel_Type type = (Channel_Type)ConfigTreeObject::getConfigData(QString(SERIAL_Settings_COMMAND_CHANELTYPE).arg(group)).toInt();
    if (type == Channel_Type_Off || type == Channel_Type_Comm || type == Channel_Type_Math)
        chType = type;
    else
    {
        quint32 channel = ConfigTreeObject::getConfigData(QString(SERIAL_Settings_COMMAND_FIRSTCH).arg(group)).toUInt();
        chType = (Channel_Type)CHANNEL_TYPE(channel);
    }
}

