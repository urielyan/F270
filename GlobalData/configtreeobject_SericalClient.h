#ifndef CONFIGTREEOBJECT_SERICALCLIENT_H
#define CONFIGTREEOBJECT_SERICALCLIENT_H

#include "configobject.h"

//setting path
#define SERIAL_Settings_MODBUS_WAITTIME     "Config:Communication (Serial) settings:Modbus master:Basic settings:Recovery action:Wait time"
#define SERIAL_Settings_MODBUS_RECOVER      "Config:Communication (Serial) settings:Modbus master:Basic settings:Recovery action:Retry"
#define SERIAL_Settings_MODBUS_GAP          "Config:Communication (Serial) settings:Modbus master:Basic settings:Communication:Gap between message"
#define SERIAL_Settings_MODBUS_TIMEOUT      "Config:Communication (Serial) settings:Modbus master:Basic settings:Communication:Communication timeout"
#define SERIAL_Settings_MODBUS_INTERVAL     "Config:Communication (Serial) settings:Modbus master:Basic settings:Communication:Interval"
#define SERIAL_Settings_MODBUS_FUNCTION     "Config:Communication (Serial) settings:Modbus master:Basic settings:Master function:On/Off"
#define SERIAL_Settings_BASIC_DATALENGTH    "Config:Communication (Serial) settings:Basic settings:Data transfer:Data length"
#define SERIAL_Settings_BASIC_STOP          "Config:Communication (Serial) settings:Basic settings:Data transfer:Stop bit"
#define SERIAL_Settings_BASIC_PARITY        "Config:Communication (Serial) settings:Basic settings:Data transfer:Parity bit"
#define SERIAL_Settings_BASIC_BUAD          "Config:Communication (Serial) settings:Basic settings:Data transfer:Baud rate"
#define SERIAL_Settings_BASIC_ADDR          "Config:Communication (Serial) settings:Basic settings:Receiver:Address"
#define SERIAL_Settings_BASIC_FUNCTION      "Config:Communication (Serial) settings:Basic settings:Receiver:Function"

#define SERIAL_Settings_COMMAND_NUM         "Config:Communication (Serial) settings:Modbus master:Modbus master:Command settings:Master command number"
#define SERIAL_Settings_COMMAND_TYPE        "Config:Communication (Serial) settings:Modbus master:Command settings:%1:Command settings:Type"
#define SERIAL_Settings_COMMAND_SLAVE       "Config:Communication (Serial) settings:Modbus master:Command settings:%1:Command settings:Slave"
#define SERIAL_Settings_COMMAND_DATATYPE    "Config:Communication (Serial) settings:Modbus master:Command settings:%1:Command settings:Data type"
#define SERIAL_Settings_COMMAND_REGISTER    "Config:Communication (Serial) settings:Modbus master:Command settings:%1:Command settings:Register"
#define SERIAL_Settings_COMMAND_CHANELTYPE  "Config:Communication (Serial) settings:Modbus master:Command settings:%1:Command settings:Channel type"
#define SERIAL_Settings_COMMAND_FIRSTCH     "Config:Communication (Serial) settings:Modbus master:Command settings:%1:Command settings:First-CH"
#define SERIAL_Settings_COMMAND_LASTCH      "Config:Communication (Serial) settings:Modbus master:Command settings:%1:Command settings:Last-CH"

//delegate path
#define SERIAL_DELEGATE                 "Config:Communication (Serial) Delegate"
#define SERIAL_DELEGATE_MODBUS_BASIC    "Config:Communication (Serial) Delegate:Modbus master:Basic settings"

#define SERIAL_DELEGATE_MODBUS_WAITTIME     "Config:Communication (Serial) Delegate:Modbus master:Basic settings:Recovery action:Wait time"
#define SERIAL_DELEGATE_MODBUS_RECOVER      "Config:Communication (Serial) Delegate:Modbus master:Basic settings:Recovery action:Retry"
#define SERIAL_DELEGATE_MODBUS_GAP          "Config:Communication (Serial) Delegate:Modbus master:Basic settings:Communication:Gap between message"
#define SERIAL_DELEGATE_MODBUS_TIMEOUT      "Config:Communication (Serial) Delegate:Modbus master:Basic settings:Communication:Communication timeout"
#define SERIAL_DELEGATE_MODBUS_INTERVAL     "Config:Communication (Serial) Delegate:Modbus master:Basic settings:Communication:Interval"
#define SERIAL_DELEGATE_MODBUS_FUNCTION     "Config:Communication (Serial) Delegate:Modbus master:Basic settings:Master function:On/Off"
#define SERIAL_DELEGATE_BASIC_DATALENGTH    "Config:Communication (Serial) Delegate:Basic settings:Data transfer:Data length"
#define SERIAL_DELEGATE_BASIC_STOP          "Config:Communication (Serial) Delegate:Basic settings:Data transfer:Stop bit"
#define SERIAL_DELEGATE_BASIC_PARITY        "Config:Communication (Serial) Delegate:Basic settings:Data transfer:Parity bit"
#define SERIAL_DELEGATE_BASIC_BUAD          "Config:Communication (Serial) Delegate:Basic settings:Data transfer:Baud rate"
#define SERIAL_DELEGATE_BASIC_ADDR          "Config:Communication (Serial) Delegate:Basic settings:Receiver:Address"
#define SERIAL_DELEGATE_BASIC_FUNCTION      "Config:Communication (Serial) Delegate:Basic settings:Receiver:Function"

#define SERIAL_DELEGATE_COMMAND_NUM         "Config:Communication (Serial) Delegate:Modbus master:Command settings:Master command number"
#define SERIAL_DELEGATE_COMMAND_TYPE        "Config:Communication (Serial) Delegate:Modbus master:Command settings:Command settings:Type"
#define SERIAL_DELEGATE_COMMAND_SLAVE       "Config:Communication (Serial) Delegate:Modbus master:Command settings:Command settings:Slave"
#define SERIAL_DELEGATE_COMMAND_DATATYPE    "Config:Communication (Serial) Delegate:Modbus master:Command settings:Command settings:Data type"
#define SERIAL_DELEGATE_COMMAND_REGISTER    "Config:Communication (Serial) Delegate:Modbus master:Command settings:Command settings:Register"
#define SERIAL_DELEGATE_COMMAND_CHANELTYPE  "Config:Communication (Serial) Delegate:Modbus master:Command settings:Command settings:Channel type"
#define SERIAL_DELEGATE_COMMAND_FIRSTCH     "Config:Communication (Serial) Delegate:Modbus master:Command settings:Command settings:First-CH"
#define SERIAL_DELEGATE_COMMAND_LASTCH      "Config:Communication (Serial) Delegate:Modbus master:Command settings:Command settings:Last-CH"

//XML path
#define XML_SERIAL_MODBUS_WAITTIME      ":/configtree/Image/configTree/Serial/Serial_ValueChange/command_basic/Serial_Command_Basic_WaitTime.xml"
#define XML_SERIAL_MODBUS_RECOVER       ":/configtree/Image/configTree/Serial/Serial_ValueChange/command_basic/Serial_Command_Basic_Retry.xml"
#define XML_SERIAL_MODBUS_GAP           ":/configtree/Image/configTree/Serial/Serial_ValueChange/command_basic/Serial_Command_Basic_CommandInterval.xml"
#define XML_SERIAL_MODBUS_TIMEOUT       ":/configtree/Image/configTree/Serial/Serial_ValueChange/command_basic/Serial_Command_Basic_Timeout.xml"
#define XML_SERIAL_MODBUS_INTERVAL      ":/configtree/Image/configTree/Serial/Serial_ValueChange/command_basic/Serial_Command_Basic_Interval.xml"
#define XML_SERIAL_MODBUS_FUNCTION      ":/configtree/Image/configTree/Serial/Serial_ValueChange/command_basic/Serial_Command_Basic_Function.xml"
#define XML_SERIAL_BASIC_DATALENGTH     ":/configtree/Image/configTree/Serial/Serial_ValueChange/basic/Serial_Basic_Length.xml"
#define XML_SERIAL_BASIC_STOP           ":/configtree/Image/configTree/Serial/Serial_ValueChange/basic/Serial_Basic_Stop.xml"
#define XML_SERIAL_BASIC_PARITY         ":/configtree/Image/configTree/Serial/Serial_ValueChange/basic/Serial_Basic_Parity.xml"
#define XML_SERIAL_BASIC_BUAD           ":/configtree/Image/configTree/Serial/Serial_ValueChange/basic/Serial_Basic_Baud.xml"
#define XML_SERIAL_BASIC_ADDR           ":/configtree/Image/configTree/Serial/Serial_ValueChange/basic/Serial_Basic_Addr.xml"
#define XML_SERIAL_BASIC_FUNCTION       ":/configtree/Image/configTree/Serial/Serial_ValueChange/basic/Serial_Basic_Function.xml"

//#define XML_SERIAL_COMMAND_NUM        ":/configtree/Image/configTree/Serial/Serial_ValueChange/command_setting/Serial_Command_Number.xml"
#define XML_SERIAL_COMMAND_TYPE         ":/configtree/Image/configTree/Serial/Serial_ValueChange/command_setting/Serial_Command_Type.xml"
#define XML_SERIAL_COMMAND_SLAVE        ":/configtree/Image/configTree/Serial/Serial_ValueChange/command_setting/Serial_Command_Salve.xml"
#define XML_SERIAL_COMMAND_DATATYPE     ":/configtree/Image/configTree/Serial/Serial_ValueChange/command_setting/Serial_Command_DataType.xml"
#define XML_SERIAL_COMMAND_REGISTER     ":/configtree/Image/configTree/Serial/Serial_ValueChange/command_setting/Serial_Command_Register.xml"
#define XML_SERIAL_COMMAND_CHANELTYPE_R ":/configtree/Image/configTree/Serial/Serial_ValueChange/command_setting/Serial_Command_ChanelType_R.xml"
#define XML_SERIAL_COMMAND_CHANELTYPE_W ":/configtree/Image/configTree/Serial/Serial_ValueChange/command_setting/Serial_Command_ChanelType_W.xml"
#define XML_SERIAL_COMMAND_CHANELTYPE   ":/configtree/Image/configTree/Serial/Serial_ValueChange/command_setting/Serial_Command_ChanelType.xml"
#define XML_SERIAL_COMMAND_FIRSTCH      ":/configtree/Image/configTree/Serial/Serial_ValueChange/command_setting/Serial_Command_FirstCH.xml"
#define XML_SERIAL_COMMAND_LASTCH       ":/configtree/Image/configTree/Serial/Serial_ValueChange/command_setting/Serial_Command_LastCH.xml"

#define Serial_Setting_CommandNum       "Config:Communication (Serial) settings:Modbus master:Command settings:Master command number"

class ConfigTreeObject_SericalClient : public ConfigObject
{
    Q_OBJECT
public:
    static ConfigTreeObject_SericalClient *instance(QObject *parent);

signals:

private:
    explicit ConfigTreeObject_SericalClient(QObject *parent = 0);

    void updateStrShowOn();
public slots:
    void slotCreateSerialsettingTree(QObject *parent = 0);

    void slotSericalDelegateChange();

    void slotSerialBasicFunctionChange();
    void slotSerialBasicAddrChange();
    void slotSerialBasicBuadChange();
    void slotSerialBasicParityChange();
    void slotSerialBasicStopChange();
    void slotSerialBasicDataLengthChange();

    void slotSerialModbusFunctionChange();
    void slotSerialModbusIntervalChange();
    void slotSerialModbusTimeoutChange();
    void slotSerialModbusGapChange();
    void slotSerialModbusRecoverChange();
    void slotSerialModbusWaitTimeChange();

    void slotSerialModbusCommandNumberChange();
    void slotSerialModbusCommandTypeChange();
    void slotSerialModbusCommandSlaveChange();
    void slotSerialModbusCommandDataTypeChange();
    void slotSerialModbusCommandRegisterChange();
    void slotSerialModbusCommandChanelTypeChange();
    void slotSerialModbusCommandFirstCHChange();
    void slotSerialModbusCommandLastCHChange();

    void slotSerialCommandTypeRelationChange();
    void slotSerialCommandChanelTypeRelationChange();

};


#endif // CONFIGTREEOBJECT_SERICALCLIENT_H
