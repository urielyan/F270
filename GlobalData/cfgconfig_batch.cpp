#include "cfgconfig_batch.h"
#include "./GlobalData/configtreeobject.h"

CfgConfig_Batch *CfgConfig_Batch::instance()
{
    static CfgConfig_Batch data;
    return &data;
}


CfgConfig_Batch::CfgConfig_Batch(QObject *parent) :
    QObject(parent)
{
}






QVariant CfgConfig_Batch::getBatchData(quint32 ID, quint32 group)
{
    switch (ID) {
    case ID_batch_Function_OnOff:
        return p_batchConfig->batchOnOff;
        break;
    case ID_batch_Function_Num:
        return p_batchConfig->batchNumber;
        break;
    case ID_batch_Function_AutoInc:
        return p_batchConfig->batchAutoInc;
        break;

    case ID_Batch_Text_TextField_Title:
        return getCharToString(p_batchConfig->batchTextTitle[group]);
        break;
    case ID_Batch_Text_TextField_TextStr:
        return getCharToString(p_batchConfig->batchTextString[group]);
        break;

    case ID_Batch_Text_Num_BatchNum:
        return getCharToString(p_batchConfig->batchBatNumber);
        break;
    case ID_Batch_Text_Num_LotNum:
        return p_batchConfig->batchLotNumber;
        break;

    case ID_Batch_Text_Comment_1:
        return getCharToString(p_batchConfig->batchComment[0]);
        break;
    case ID_Batch_Text_Comment_2:
        return getCharToString(p_batchConfig->batchComment[1]);
        break;
    case ID_Batch_Text_Comment_3:
        return getCharToString(p_batchConfig->batchComment[2]);
        break;
    default:
        break;
    }
    return QVariant();
}

QString CfgConfig_Batch::getBatchObjStr(quint32 ID, quint32 /*group*/)
{
    switch (ID) {
    case ID_Batch:
        return tr("Batch settings");
        break;
    case ID_Batch_Function:
        return tr("Batch function");
        break;

    case ID_batch_Function_OnOff:
        return tr("On/Off");
        break;
    case ID_batch_Function_Num:
        return tr("Lot-No. digit");
        break;
    case ID_batch_Function_AutoInc:
        return tr("Auto increament");
        break;

    case ID_Batch_TextText:
    case ID_Batch_Text_Text:
        return tr("Batch text");
        break;

    case ID_Batch_Text_Text_Num:
        return tr("Batch filed number");
        break;
    case ID_Batch_Text_TextFiled:
        return tr("Text filed");
        break;
    case ID_Batch_Text_TextField_Title:
        return tr("Title of filed");
        break;
    case ID_Batch_Text_TextField_TextStr:
        return tr("Characters");
        break;
    default:
        break;
    }
    return NULL;
}

QString CfgConfig_Batch::getBatchObjValStr(quint32 ID, quint32 /*group*/, ConfigObject *cfg)
{
    switch (ID) {
    case ID_batch_Function_OnOff:
    case ID_batch_Function_AutoInc:
        return cfg->getData().toInt() == 0 ? tr("Off") : tr("On");
        break;
    case ID_batch_Function_Num:
    {
        int data = cfg->getData().toInt();
        if (data == 0)
            return tr("Off");
        else
            return QString::number(data);
    }
        break;
    default:
        break;
    }
    return cfg->getData().toString();
}

void CfgConfig_Batch::setBatchData(quint32 ID, quint32 group)
{
    switch (ID) {
    case ID_batch_Function_OnOff:
        p_batchConfig->batchOnOff = ConfigTreeObject::getConfigData(Config_Batch_Setting_function).toInt();
        break;
    case ID_batch_Function_Num:
        p_batchConfig->batchNumber = ConfigTreeObject::getConfigData(Config_Batch_Setting_Lot).toInt();
        break;
    case ID_batch_Function_AutoInc:
        p_batchConfig->batchAutoInc = ConfigTreeObject::getConfigData(Config_Batch_Setting_Auto).toInt();
        break;

    case ID_Batch_Text_TextField_Title:
        setStrToChar(p_batchConfig->batchTextTitle[group], ConfigTreeObject::getConfigData(QString(Config_Batch_Setting_TextField).arg(group)).toString(), 24);
        break;
    case ID_Batch_Text_TextField_TextStr:
        setStrToChar(p_batchConfig->batchTextString[group], ConfigTreeObject::getConfigData(QString(Config_Batch_Setting_TextCharacters).arg(group)).toString(), 24);
        break;

//    case ID_Batch_Text_Num_BatchNum:
//        return getCharToString(p_batchConfig->batchBatNumber);
//        break;
//    case ID_Batch_Text_Num_LotNum:
//        return p_batchConfig->batchLotNumber;
//        break;

//    case ID_Batch_Text_Comment_1:
//        return getCharToString(p_batchConfig->batchComment[0]);
//        break;
//    case ID_Batch_Text_Comment_2:
//        return getCharToString(p_batchConfig->batchComment[1]);
//        break;
//    case ID_Batch_Text_Comment_3:
//        return getCharToString(p_batchConfig->batchComment[2]);
//        break;
    default:
        break;
    }
}







/*
 * 把uchar *类型转化为QString类型
*/
QVariant CfgConfig_Batch::getCharToString(uchar *str)
{
    QString data((char *)str);
    return data;
}

//把QString类型转化为uchar *类型
void CfgConfig_Batch::setStrToChar(uchar * uData, QString str, int len)
{
    memset(uData, 0, len);
    QByteArray baUnit = str.toLocal8Bit();
    char* valueUnit = baUnit.data();
//    memcpy(uData, valueUnit, strlen(valueUnit));
    while (*valueUnit != '\0') {
        *uData++ = *valueUnit++;
    }
}
