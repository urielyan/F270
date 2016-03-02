#include "cfgconfig_datasave.h"
#include "./GlobalData/configtreeobject.h"

CfgConfig_DataSave *CfgConfig_DataSave::instance()
{
    static CfgConfig_DataSave data;
    return &data;
}


CfgConfig_DataSave::CfgConfig_DataSave(QObject *parent) :
    QObject(parent)
{
}

QVariant CfgConfig_DataSave::getDataSaveData(quint32 ID, quint32 /*group*/)
{
    switch (ID) {
    case ID_DataSave_Path_Name:
        return getCharToString(p_dataSaveConfig->datasveDirectoryName);
        break;

    case ID_DataSave_Title_Info:
        return getCharToString(p_dataSaveConfig->datasaveTitleInfo);
        break;

    case ID_DataSave_File_Name:
        return p_dataSaveConfig->datasaveFileName;
        break;
    case ID_DataSave_File_NameString:
        return getCharToString(p_dataSaveConfig->datasaveFileNameString);
        break;

    case ID_DataSave_Ext_Auto:
        return p_dataSaveConfig->datasaveExtMediaAuto;
        break;
    case ID_DataSave_Ext_FIFO:
        return p_dataSaveConfig->datasaveExtMediaFIFO;
        break;

    case ID_DataSave_FileFormat_Format:
        return p_dataSaveConfig->datasaveFormat;
        break;

    default:
        break;
    }
    return QVariant();
}



QString CfgConfig_DataSave::getDataSaveObjStr(quint32 ID, quint32 /*group*/)
{
    switch (ID) {
    case ID_DataSave:
        return tr("Data save settings");
        break;
    case ID_DataSave_Path:
        return tr("Save directory");
        break;
    case ID_DataSave_Path_Name:
        return tr("Directory name");
        break;
    case ID_DataSave_Title:
        return tr("File header");
        break;
    case ID_DataSave_Title_Info:
        return tr("Characters");
        break;

    case ID_DataSave_File:
        return tr("Data file name");
        break;
    case ID_DataSave_File_Name:
        return tr("Structure");
        break;
    case ID_DataSave_File_NameString:
        return tr("Identified strings");
        break;

    case ID_DataSave_Ext:
        return tr("Media");
        break;
    case ID_DataSave_Ext_Auto:
        return tr("Auto save");
        break;
    case ID_DataSave_Ext_FIFO:
        return tr("Media FIFO");
        break;

    case ID_DataSave_FileFormat:
        return tr("File formate");
        break;
    case ID_DataSave_FileFormat_Format:
        return tr("Display / Event data");
        break;

    default:
        break;
    }
    return NULL;
}

QString CfgConfig_DataSave::getDataSaveObjValStr(quint32 ID, quint32 /*group*/, ConfigObject *cfg)
{
    switch (ID) {

    case ID_DataSave_File_Name:
    {
        int type = cfg->getData().toInt();
        if (type == DataSave_FileName_Type_Date)
            return tr("Date");
        else if (type == DataSave_FileName_Type_Serial)
            return tr("Serial");
        else
            return tr("Batch");
    }
        break;

    case ID_DataSave_Ext_Auto:
    case ID_DataSave_Ext_FIFO:
        return cfg->getData().toInt() == 0 ? tr("Off") : tr("On");
        break;

    case ID_DataSave_FileFormat_Format:
        return cfg->getData().toInt() == 0 ? tr("Binary") : tr("Text");
        break;

    default:
        break;
    }
    return cfg->getData().toString();
}


void CfgConfig_DataSave::setDataSaveData(quint32 ID, quint32 /*group*/)
{
    switch (ID) {
    case ID_DataSave_Path_Name:
        setStrToChar(p_dataSaveConfig->datasveDirectoryName, ConfigTreeObject::getConfigData(DATASAVECONFIG_DIR_NAME).toString(), 24);
        break;

    case ID_DataSave_Title_Info:
        setStrToChar(p_dataSaveConfig->datasaveTitleInfo, ConfigTreeObject::getConfigData(DATASAVECONFIG_HEADER_CHAR).toString(), 52);
        break;

    case ID_DataSave_File_Name:
        p_dataSaveConfig->datasaveFileName = (DataSave_FileName_Type)ConfigTreeObject::getConfigData(DATASAVECONFIG_FILENAME_STR).toInt();
        break;
    case ID_DataSave_File_NameString:
        setStrToChar(p_dataSaveConfig->datasaveFileNameString, ConfigTreeObject::getConfigData(DATASAVECONFIG_FILENAME_ID).toString(), 20);
        break;

    case ID_DataSave_Ext_Auto:
        p_dataSaveConfig->datasaveExtMediaAuto = ConfigTreeObject::getConfigData(DATASAVECONFIG_MEDIA_AUTO).toInt();
        break;
    case ID_DataSave_Ext_FIFO:
        p_dataSaveConfig->datasaveExtMediaFIFO = ConfigTreeObject::getConfigData(DATASAVECONFIG_MEDIA_FIFO).toInt();
        break;

    case ID_DataSave_FileFormat_Format:
        p_dataSaveConfig->datasaveFormat = (DataSave_File_Format_Enum)ConfigTreeObject::getConfigData(DATASAVECONFIG_FORMAT_DATA).toInt();
        break;

    default:
        break;
    }
}



/*
 * 把uchar *类型转化为QString类型
*/
QVariant CfgConfig_DataSave::getCharToString(uchar *str)
{
    QString data((char *)str);
    return data;
}

//把QString类型转化为uchar *类型
void CfgConfig_DataSave::setStrToChar(uchar * uData, QString str, int len)
{
    memset(uData, 0, len);
    QByteArray baUnit = str.toLocal8Bit();
    char* valueUnit = baUnit.data();
//    memcpy(uData, valueUnit, strlen(valueUnit));
    while (*valueUnit != '\0') {
        *uData++ = *valueUnit++;
    }
}

