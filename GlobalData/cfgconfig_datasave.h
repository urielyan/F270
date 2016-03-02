#ifndef CFGCONFIG_DATASAVE_H
#define CFGCONFIG_DATASAVE_H

#include <QObject>
#include "./GlobalData/cfgGlobalDef_DataSave.h"
#include "./GlobalData/configobject.h"

/****************************************  data save 21700 - 21800  *************************************************/
#define DataSave_Start_ID                           21700
#define DataSave_End_ID                             21799

//data save
#define ID_DataSave                                 21700

#define ID_DataSave_Path                            21701   //group
#define ID_DataSave_Path_Name                       21702   //文件保存目录名，最多20个半角字符

#define ID_DataSave_Title                           21710   //group
#define ID_DataSave_Title_Info                      21712   //文件标题注释，最多50个半角字符，中英文

#define ID_DataSave_File                            21720   //group
#define ID_DataSave_File_Name                       21721   //数据文件名称命名方式
#define ID_DataSave_File_NameString                 21722   //文件名称指定字符串，最多16个半角字符

#define ID_DataSave_Ext                             21730  //外部存储器保存
#define ID_DataSave_Ext_Auto                        21731   //外部存储器自动保存开关，0-off, 1-on
#define ID_DataSave_Ext_FIFO                        21732   //外部存储器FIFO开关，0-off, 1-on

#define ID_DataSave_FileFormat                      21740   //文件保存格式   group
#define ID_DataSave_FileFormat_Format               21741


/**********************  21750～21770之间的ID被界面保存载入(winsaveloadmainframe.h)中使用，以后添加ID应避开此段  ************/


class CfgConfig_DataSave : public QObject
{
    Q_OBJECT
public:
    static CfgConfig_DataSave * instance();
    QVariant getDataSaveData(quint32 ID, quint32 group);
    void setDataSaveData(quint32 ID, quint32 group);

    QString getDataSaveObjStr(quint32 ID, quint32 group = 0);
    QString getDataSaveObjValStr(quint32 ID, quint32 group, ConfigObject *cfg);

    void setDeviceDataSaveConfig(DATASAVE_CONFIG & dataSaveConfig) { p_dataSaveConfig = &dataSaveConfig;}


private:
    explicit CfgConfig_DataSave(QObject *parent = 0);

    QVariant getCharToString(uchar *str);
    void setStrToChar(uchar * uData, QString str, int len);

signals:

public slots:

private:
    DATASAVE_CONFIG *                   p_dataSaveConfig;
};

#endif // CFGCONFIG_DATASAVE_H
