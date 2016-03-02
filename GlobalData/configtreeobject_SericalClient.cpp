#include "configtreeobject_SericalClient.h"
#include "cfgchannel.h"
#include "./GlobalData/globalDef.h"

#include "cfgGlobalDef.h"
#include "Common/winserialconfig.h"
#include "Common/winnetworkconfig.h"
#include "../Common/xmlconfigparser.h"
#include "Common/devboard.h"
#include <QDebug>

ConfigTreeObject_SericalClient *ConfigTreeObject_SericalClient::instance(QObject *parent)
{
    static ConfigTreeObject_SericalClient data(parent);
    return &data;
}

ConfigTreeObject_SericalClient::ConfigTreeObject_SericalClient(QObject *parent):
      ConfigObject(parent)
{
    //Serial settings代理树
    QFile fileDelegate(":/configtree/Image/configTree/Serial/Serial_SettingDelegate/delegate.xml");
    if(!fileDelegate.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file: Network_setting!";
    }else{
        XmlConfigParser::parseConfigXml(fileDelegate, static_cast<ConfigObject*>(parent), \
                                                                   (QObject*)this);
        fileDelegate.close();
    }
}

void ConfigTreeObject_SericalClient::updateStrShowOn()
{
    slotSerialBasicFunctionChange();
    slotSerialBasicAddrChange();
    slotSerialBasicBuadChange();
    slotSerialBasicParityChange();
    slotSerialBasicStopChange();
    slotSerialBasicDataLengthChange();

    slotSerialModbusFunctionChange();
    slotSerialModbusIntervalChange();
    slotSerialModbusTimeoutChange();
    slotSerialModbusGapChange();
    slotSerialModbusRecoverChange();
    slotSerialModbusWaitTimeChange();

    slotSerialModbusCommandNumberChange();
    slotSerialModbusCommandTypeChange();
    slotSerialModbusCommandSlaveChange();
    slotSerialModbusCommandDataTypeChange();
    slotSerialModbusCommandRegisterChange();
    slotSerialModbusCommandChanelTypeChange();
    slotSerialModbusCommandFirstCHChange();
    slotSerialModbusCommandLastCHChange();
}

void ConfigTreeObject_SericalClient::slotCreateSerialsettingTree(QObject *parent)
{
    //Serial setting 配置树
    QFile file(":/configtree/Image/configTree/Serial/Serial_Setting/config.xml");
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file: Network_setting!";
    }else{
        XmlConfigParser::parseConfigXml(file, static_cast<ConfigObject*>(parent));
        file.close();
    }

    quint32 ID_command = ConfigTreeObject::getConfigObject(Serial_Setting_CommandNum)->getObjectID();
    ConfigTreeObject::getConfigObject(Serial_Setting_CommandNum)->initData(0, 0, ID_command);
    ConfigTreeObject::getConfigObject(Serial_Setting_CommandNum)->setStrShowOn("1");
}

void ConfigTreeObject_SericalClient::slotSericalDelegateChange()
{
    QFile delegateChange(":/configtree/Image/configTree/Serial/Serial_DelegateChange/delegateChange.xml");
     if(!delegateChange.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file: AI_settingDelegateChange!";
     }else{
        XmlConfigParser::parseDelegateChangeXml(delegateChange);
        delegateChange.close();
     }

     _curStatus = false;
     updateStrShowOn();
     _curStatus = true;
}

void ConfigTreeObject_SericalClient::slotSerialBasicFunctionChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(SERIAL_DELEGATE_BASIC_FUNCTION)->getData();

    QFile file(XML_SERIAL_BASIC_FUNCTION);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_SericalClient::slotSerialBasicAddrChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(SERIAL_DELEGATE_BASIC_ADDR)->getData();

    QFile file(XML_SERIAL_BASIC_ADDR);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_SericalClient::slotSerialBasicBuadChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(SERIAL_DELEGATE_BASIC_BUAD)->getData();

    QFile file(XML_SERIAL_BASIC_BUAD);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_SericalClient::slotSerialBasicParityChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(SERIAL_DELEGATE_BASIC_PARITY)->getData();

    QFile file(XML_SERIAL_BASIC_PARITY);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_SericalClient::slotSerialBasicStopChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(SERIAL_DELEGATE_BASIC_STOP)->getData();

    QFile file(XML_SERIAL_BASIC_STOP);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_SericalClient::slotSerialBasicDataLengthChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(SERIAL_DELEGATE_BASIC_DATALENGTH)->getData();

    QFile file(XML_SERIAL_BASIC_DATALENGTH);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_SericalClient::slotSerialModbusFunctionChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(SERIAL_DELEGATE_MODBUS_FUNCTION)->getData();

    QFile file(XML_SERIAL_MODBUS_FUNCTION);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_SericalClient::slotSerialModbusIntervalChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(SERIAL_DELEGATE_MODBUS_INTERVAL)->getData();

    QFile file(XML_SERIAL_MODBUS_INTERVAL);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_SericalClient::slotSerialModbusTimeoutChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(SERIAL_DELEGATE_MODBUS_TIMEOUT)->getData();

    QFile file(XML_SERIAL_MODBUS_TIMEOUT);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_SericalClient::slotSerialModbusGapChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(SERIAL_DELEGATE_MODBUS_GAP)->getData();

    QFile file(XML_SERIAL_MODBUS_GAP);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_SericalClient::slotSerialModbusRecoverChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(SERIAL_DELEGATE_MODBUS_RECOVER)->getData();

    QFile file(XML_SERIAL_MODBUS_RECOVER);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_SericalClient::slotSerialModbusWaitTimeChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(SERIAL_DELEGATE_MODBUS_WAITTIME)->getData();

    QFile file(XML_SERIAL_MODBUS_WAITTIME);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_SericalClient::slotSerialModbusCommandNumberChange()
{
    ConfigObject *pObj = ConfigTreeObject::getConfigObject(SERIAL_DELEGATE_COMMAND_NUM);

    if(pObj){
        quint32 num = pObj->getData().toUInt();
        pObj->setStrShowOn(QString("%1").arg(num+1));
    }
}

void ConfigTreeObject_SericalClient::slotSerialModbusCommandTypeChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(SERIAL_DELEGATE_COMMAND_TYPE)->getData();

    QFile file(XML_SERIAL_COMMAND_TYPE);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_SericalClient::slotSerialModbusCommandSlaveChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(SERIAL_DELEGATE_COMMAND_SLAVE)->getData();

    QFile file(XML_SERIAL_COMMAND_SLAVE);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_SericalClient::slotSerialModbusCommandDataTypeChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(SERIAL_DELEGATE_COMMAND_DATATYPE)->getData();

    switch (value.toInt()) {
    case NetworkModbusCommandDataType_Bit:
        ConfigTreeObject::getConfigObject(SERIAL_DELEGATE_COMMAND_REGISTER)->changeData("1",false);
        break;
    default:
    {
        QVariant type = ConfigTreeObject::getConfigObject(SERIAL_DELEGATE_COMMAND_TYPE)->getData();
        switch (type.toInt()) {
        case NetworkModbusCommandType_Read:
            ConfigTreeObject::getConfigObject(SERIAL_DELEGATE_COMMAND_REGISTER)->changeData("30001",false);
            break;
        case NetworkModbusCommandType_Write:
            ConfigTreeObject::getConfigObject(SERIAL_DELEGATE_COMMAND_REGISTER)->changeData("40001",false);
            break;
        default:
            break;
        }

        break;
    }
    }

    QFile file(XML_SERIAL_COMMAND_DATATYPE);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_SericalClient::slotSerialModbusCommandRegisterChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(SERIAL_DELEGATE_COMMAND_REGISTER)->getData();

    QFile file(XML_SERIAL_COMMAND_REGISTER);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_SericalClient::slotSerialModbusCommandChanelTypeChange()
{
    quint32 type = ConfigTreeObject::getConfigObject(SERIAL_DELEGATE_COMMAND_TYPE)->getData().toUInt();

    QVariant value = ConfigTreeObject::getConfigObject(SERIAL_DELEGATE_COMMAND_CHANELTYPE)->getData();
    quint32 valueF = ConfigTreeObject::getConfigObject(SERIAL_DELEGATE_COMMAND_FIRSTCH)->getData().toUInt();
    quint32 valueL = ConfigTreeObject::getConfigObject(SERIAL_DELEGATE_COMMAND_LASTCH)->getData().toUInt();
    //QString样式转的int,非map对应的quint32
    int StrvalueF = CfgConfig_Serial::instance()->getSerialModbusChValObjStr(valueF).toInt();
    int StrvalueL = CfgConfig_Serial::instance()->getSerialModbusChValObjStr(valueL).toInt();

    QFile file;
    if (value.toString() == "0") {
        ConfigTreeObject::getConfigObject(SERIAL_DELEGATE_COMMAND_CHANELTYPE)->changeData(NetworkModbusCommandChanelType_COMM, false);
    }

    switch (type) {
    case NetworkModbusCommandType_Read:
    {
        ConfigTreeObject::getConfigObject(SERIAL_DELEGATE_COMMAND_CHANELTYPE)\
                ->changeData(NetworkModbusCommandChanelType_COMM, false);
        file.setFileName(XML_SERIAL_COMMAND_CHANELTYPE_R);

        //first channel
        if (StrvalueF > 300)
        ConfigTreeObject::getConfigObject(SERIAL_DELEGATE_COMMAND_FIRSTCH)\
                ->changeData(CfgChannel::instance()->getExistChannelComm().first(), false);
        //last channel
        if (StrvalueL > 300)
        ConfigTreeObject::getConfigObject(SERIAL_DELEGATE_COMMAND_LASTCH)\
                ->changeData(CfgChannel::instance()->getExistChannelComm().first(), false);
        break;
    }
    case NetworkModbusCommandType_Write:
    {
        QList<DevBoard *> boardList = CfgChannel::instance()->getList_BoardIO();
        //wait ....modify
        if (boardList.isEmpty()) {
            file.setFileName(XML_SERIAL_COMMAND_CHANELTYPE);
        } else {
            file.setFileName(XML_SERIAL_COMMAND_CHANELTYPE_W);
        }
        //first channel
        switch (value.toInt()) {
        case Channel_Type_Comm:
        {
            //first channel
            if (StrvalueF > 300)
            ConfigTreeObject::getConfigObject(SERIAL_DELEGATE_COMMAND_FIRSTCH)\
                    ->changeData(CfgChannel::instance()->getExistChannelComm().first(), false);
            //last channel
            if (StrvalueL > 300)
            ConfigTreeObject::getConfigObject(SERIAL_DELEGATE_COMMAND_LASTCH)\
                    ->changeData(CfgChannel::instance()->getExistChannelComm().first(), false);
            break;
        }
        case Channel_Type_Math:
        {
            //first channel
            if (StrvalueF > 100)
            ConfigTreeObject::getConfigObject(SERIAL_DELEGATE_COMMAND_FIRSTCH)\
                    ->changeData(CfgChannel::instance()->getExistChannelMath().first(), false);
            //last channel
            if (StrvalueL > 100)
            ConfigTreeObject::getConfigObject(SERIAL_DELEGATE_COMMAND_LASTCH)\
                    ->changeData(CfgChannel::instance()->getExistChannelMath().first(), false);
            break;
        }
        case Channel_Type_AI:
        case Channel_Type_DI:
        case Channel_Type_DO:
        {
            quint32 codeF, codeL;
            int tempModule = 0;
            int moduleMin = boardList.value(0)->getBoardModule();
            for (int i = 0; i < boardList.size(); ++i) {
                if (boardList.value(i)->getBoardModule() < moduleMin) {
                    tempModule = i;
                    moduleMin = boardList.value(i)->getBoardModule();
                }
            }

            switch (boardList.value(tempModule)->getBoardType()) {
            case STATUS_BOARD_TYPE_AI:
                codeF = CHANNEL_AI_CODER(CHANNEL_UNIT(valueF), CHANNEL_MODULE(valueF), CHANNEL_NUM(valueF));
                codeL = CHANNEL_AI_CODER(CHANNEL_UNIT(valueL), CHANNEL_MODULE(valueL), CHANNEL_NUM(valueL));
                break;
            case STATUS_BOARD_TYPE_DI:
                codeF = CHANNEL_DI_CODER(CHANNEL_UNIT(valueF), CHANNEL_MODULE(valueF), CHANNEL_NUM(valueF));
                codeL = CHANNEL_DI_CODER(CHANNEL_UNIT(valueL), CHANNEL_MODULE(valueL), CHANNEL_NUM(valueL));
                break;
            case STATUS_BOARD_TYPE_DO:
                codeF = CHANNEL_DO_CODER(CHANNEL_UNIT(valueF), CHANNEL_MODULE(valueF), CHANNEL_NUM(valueF));
                codeL = CHANNEL_DO_CODER(CHANNEL_UNIT(valueL), CHANNEL_MODULE(valueL), CHANNEL_NUM(valueL));
                break;
            default:
                break;
            }
            ConfigTreeObject::getConfigObject(SERIAL_DELEGATE_COMMAND_FIRSTCH)\
                    ->changeData(codeF, false);
            ConfigTreeObject::getConfigObject(SERIAL_DELEGATE_COMMAND_LASTCH)\
                    ->changeData(codeL, false);
            break;
        }
        default:
            break;
        }
        break;
    }
    default:
        return;
    }

    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Range!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_SericalClient::slotSerialModbusCommandFirstCHChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(SERIAL_DELEGATE_COMMAND_FIRSTCH)->getData();

    QFile file(XML_SERIAL_COMMAND_FIRSTCH);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_SericalClient::slotSerialModbusCommandLastCHChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(SERIAL_DELEGATE_COMMAND_LASTCH)->getData();

    QVariant value_f = ConfigTreeObject::getConfigObject(SERIAL_DELEGATE_COMMAND_FIRSTCH)->getData();

    if (value.toInt() < value_f.toInt()) {
        ConfigTreeObject::getConfigObject(SERIAL_DELEGATE_COMMAND_LASTCH)->changeData(value_f.toInt(), false);
        return;
    }

    QFile file(XML_SERIAL_COMMAND_LASTCH);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_SericalClient::slotSerialCommandTypeRelationChange()
{
    slotSerialModbusCommandChanelTypeChange();
    slotSerialModbusCommandDataTypeChange();
    slotSerialModbusCommandFirstCHChange();
    slotSerialModbusCommandLastCHChange();
}

void ConfigTreeObject_SericalClient::slotSerialCommandChanelTypeRelationChange()
{
    slotSerialModbusCommandFirstCHChange();
    slotSerialModbusCommandLastCHChange();
}
