#include "configtreeobject_DataSave.h"
#include "cfgchannel.h"
#include "./GlobalData/globalDef.h"

#include "cfgGlobalDef.h"
#include "configtreeobject.h"

ConfigTreeObject_DataSave *ConfigTreeObject_DataSave::instance(QObject *parent)
{
    static ConfigTreeObject_DataSave data(parent);
    return &data;
}

ConfigTreeObject_DataSave::ConfigTreeObject_DataSave(QObject *parent):
      ConfigObject(parent)
{
    //Datasave settings代理树
    QFile fileDelegate(":/configtree/Image/configTree/DataSave/Data_SettingDelegate/delegate.xml");
    if(!fileDelegate.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file: Datasave_settingDelegate!";
    }else{
        XmlConfigParser::parseConfigXml(fileDelegate, static_cast<ConfigObject*>(parent), \
                                                          (QObject*)this);
        fileDelegate.close();
    }
}

void ConfigTreeObject_DataSave::updateStrShowOn()
{
    slotDatasaveDirectNameChanged();
    slotDatasaveHeaderCharChanged();
    slotDatasaveFileStructChanged();
    slotDatasaveFileIdentyChanged();
    slotDatasaveMediaAutoChanged();
    slotDatasaveMediaFIFOChanged();
    slotDatasaveFormatDataChanged();
}

void ConfigTreeObject_DataSave::slotCreateDatasavesettingTree(QObject *parent)
{
    //Datasave settings配置树
    QFile file(":/configtree/Image/configTree/DataSave/Data_Setting/config.xml");
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file: Datasave_setting!";
    }else{
        XmlConfigParser::parseConfigXml(file, static_cast<ConfigObject*>(parent));
        file.close();
    }
}

void ConfigTreeObject_DataSave::slotDataDelegateChange()
{
    QFile delegateChange(":/configtree/Image/configTree/DataSave/Data_DelegateChange/delegateChange.xml");
    if(!delegateChange.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file: Datasave_DelegateChange!";
    }else{
        XmlConfigParser::parseDelegateChangeXml(delegateChange);
        delegateChange.close();
    }

    _curStatus = false;
    updateStrShowOn();
    _curStatus = true;
}

void ConfigTreeObject_DataSave::slotDatasaveDirectNameChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(DATASAVE_DELEGATE_DIR_NAME)->getData();

    QFile file(XML_DATASAVE_DIR_NAME);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:DataDir_name!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_DataSave::slotDatasaveHeaderCharChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(DATASAVE_DELEGATE_HEADER_CHAR)->getData();

    QFile file(XML_DATASAVE_HEADER_CHAR);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:DataHeader_char!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_DataSave::slotDatasaveFileStructChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(DATASAVE_DELEGATE_FILENAME_STR)->getData();

    QFile file(XML_DATASAVE_FILENAME_STR);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:DataFilename_Str!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_DataSave::slotDatasaveFileIdentyChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(DATASAVE_DELEGATE_FILENAME_ID)->getData();

    QFile file(XML_DATASAVE_FILENAME_ID);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:DataFilename_Identy!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_DataSave::slotDatasaveMediaAutoChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(DATASAVE_DELEGATE_MEDIA_AUTO)->getData();

    QFile file(XML_DATASAVE_MEDIA_AUTO);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:DataMedia_Auto!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_DataSave::slotDatasaveMediaFIFOChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(DATASAVE_DELEGATE_MEDIA_FIFO)->getData();

    QFile file(XML_DATASAVE_MEDIA_FIFO);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:DataMedia_FIFO!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_DataSave::slotDatasaveFormatDataChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(DATASAVE_DELEGATE_FORMAT_DATA)->getData();

    QFile file(XML_DATASAVE_FORMAT_DATA);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:DataFormat_data!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}
