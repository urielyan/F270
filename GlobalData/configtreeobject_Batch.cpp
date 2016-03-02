#include "configtreeobject_Batch.h"
#include "cfgchannel.h"
#include "./GlobalData/globalDef.h"

#include "cfgGlobalDef.h"


/* 批处理设定配置树*/
ConfigTreeObject_Batch *ConfigTreeObject_Batch::instance(QObject *parent)
{
    static ConfigTreeObject_Batch data(parent);
    return &data;
}


ConfigTreeObject_Batch::ConfigTreeObject_Batch(QObject *parent):
      ConfigObject(parent)
{
    //Batch settings代理树
    QFile fileDelegate(":/configtree/Image/configTree/Batch/Batch_Delegate/delegate.xml");
    if(!fileDelegate.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file: Network_setting!";
    }else{
        XmlConfigParser::parseConfigXml(fileDelegate, static_cast<ConfigObject*>(parent), \
                                                                   (QObject*)this);
        fileDelegate.close();
    }
}

void ConfigTreeObject_Batch::updateStrShowOn()
{
    slotConfigBatchFunctionChange();
    slotConfigBatchLotChange();
    slotConfigBatchAutoChange();
//    slotConfigBatchTextNumChange();
    slotConfigBatchTextFieldChange();
    slotConfigBatchTextCharactersChange();
}

void ConfigTreeObject_Batch::slotCreateBatchsettingTree(QObject *parent)
{
    //Batch setting 配置树
    QFile file(":/configtree/Image/configTree/Batch/Batch_Setting/config.xml");
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file: Network_setting!";
    }else{
        XmlConfigParser::parseConfigXml(file, static_cast<ConfigObject*>(parent));
        file.close();
    }

    quint32 ID_batch = ConfigTreeObject::getConfigObject(Config_Batch_Setting_TextNum)->getObjectID();
    ConfigTreeObject::getConfigObject(Config_Batch_Setting_TextNum)->initData(0, 0, ID_batch);
    ConfigTreeObject::getConfigObject(Config_Batch_Setting_TextNum)->setStrShowOn("1");
}

void ConfigTreeObject_Batch::slotBatchDelegateChange()
{
    QFile delegateChange(":/configtree/Image/configTree/Batch/Batch_DelegateChange/delegateChange.xml");
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

void ConfigTreeObject_Batch::slotConfigBatchFunctionChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(Config_Batch_Delegate_function)->getData();

    QFile file(XML_Config_Batch_Setting_function);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Batch::slotConfigBatchLotChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(Config_Batch_Delegate_Lot)->getData();

    QFile file(XML_Config_Batch_Setting_Lot);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Batch::slotConfigBatchAutoChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(Config_Batch_Delegate_Auto)->getData();

    QFile file(XML_Config_Batch_Setting_Auto);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Batch::slotConfigBatchTextNumChange()
{
    QString num;

    ConfigObject *pObj = ConfigTreeObject::getConfigObject(Config_Batch_Delegate_TextNum);


    if(pObj){
        num = pObj->getData().toString();

        pObj->setStrShowOn(QString("%1").arg(num.toInt()+1));
    }

}

void ConfigTreeObject_Batch::slotConfigBatchTextFieldChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(Config_Batch_Delegate_TextField)->getData();

    QFile file(XML_Config_Batch_Setting_TextField);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Batch::slotConfigBatchTextCharactersChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(Config_Batch_Delegate_TextCharacters)->getData();

    QFile file(XML_Config_Batch_Setting_TextCharacters);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Batch::slotConfigBatchTextNumReationChange()
{
    slotBatchDelegateChange();
    slotConfigBatchTextFieldChange();
    slotConfigBatchTextCharactersChange();
}
