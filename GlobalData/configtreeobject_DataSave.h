#ifndef CONFIGTREEOBJECT_DATASAVE_H
#define CONFIGTREEOBJECT_DATASAVE_H

//Datasave 配置树路径
#define DATASAVECONFIG_DIR          "Config:Data save settings:Save directory"
#define DATASAVECONFIG_DIR_NAME     "Config:Data save settings:Save directory:Directory name"
#define DATASAVECONFIG_HEADER       "Config:Data save settings:File header"
#define DATASAVECONFIG_HEADER_CHAR  "Config:Data save settings:File header:Characters"
#define DATASAVECONFIG_FILENAME     "Config:Data save settings:Data file name"
#define DATASAVECONFIG_FILENAME_STR "Config:Data save settings:Data file name:Structure"
#define DATASAVECONFIG_FILENAME_ID  "Config:Data save settings:Data file name:Identified strings"
#define DATASAVECONFIG_MEDIA        "Config:Data save settings:Media save"
#define DATASAVECONFIG_MEDIA_AUTO   "Config:Data save settings:Media save:Auto save"
#define DATASAVECONFIG_MEDIA_FIFO   "Config:Data save settings:Media save:Media FIFO"
#define DATASAVECONFIG_FORMAT       "Config:Data save settings:File format"
#define DATASAVECONFIG_FORMAT_DATA  "Config:Data save settings:File format:Display / Event data"

//Datasave 代理树路径
#define DATASAVE_DELEGATE_DIR          "Config:Data save Delegate:Save directory"
#define DATASAVE_DELEGATE_DIR_NAME     "Config:Data save Delegate:Save directory:Directory name"
#define DATASAVE_DELEGATE_HEADER       "Config:Data save Delegate:File header"
#define DATASAVE_DELEGATE_HEADER_CHAR  "Config:Data save Delegate:File header:Characters"
#define DATASAVE_DELEGATE_FILENAME     "Config:Data save Delegate:Data file name"
#define DATASAVE_DELEGATE_FILENAME_STR "Config:Data save Delegate:Data file name:Structure"
#define DATASAVE_DELEGATE_FILENAME_ID  "Config:Data save Delegate:Data file name:Identified strings"
#define DATASAVE_DELEGATE_MEDIA        "Config:Data save Delegate:Media save"
#define DATASAVE_DELEGATE_MEDIA_AUTO   "Config:Data save Delegate:Media save:Auto save"
#define DATASAVE_DELEGATE_MEDIA_FIFO   "Config:Data save Delegate:Media save:Media FIFO"
#define DATASAVE_DELEGATE_FORMAT       "Config:Data save Delegate:File format"
#define DATASAVE_DELEGATE_FORMAT_DATA  "Config:Data save Delegate:File format:Display / Event data"

//Datasave 值变更 XML文件路径
#define XML_DATASAVE_DIR_NAME       ":/configtree/Image/configTree/DataSave/Data_ValueChange/DataDir_name.xml"
#define XML_DATASAVE_HEADER_CHAR    ":/configtree/Image/configTree/DataSave/Data_ValueChange/DataHeader_char.xml"
#define XML_DATASAVE_FILENAME_STR   ":/configtree/Image/configTree/DataSave/Data_ValueChange/DataFilename_Str.xml"
#define XML_DATASAVE_FILENAME_ID    ":/configtree/Image/configTree/DataSave/Data_ValueChange/DataFilename_Identy.xml"
#define XML_DATASAVE_MEDIA_AUTO     ":/configtree/Image/configTree/DataSave/Data_ValueChange/DataMedia_Auto.xml"
#define XML_DATASAVE_MEDIA_FIFO     ":/configtree/Image/configTree/DataSave/Data_ValueChange/DataMedia_FIFO.xml"
#define XML_DATASAVE_FORMAT_DATA    ":/configtree/Image/configTree/DataSave/Data_ValueChange/DataFormat_data.xml"

#include "configobject.h"

class ConfigTreeObject_DataSave : public ConfigObject
{
    Q_OBJECT
public:
    static ConfigTreeObject_DataSave *instance(QObject *parent);

signals:

private:
    explicit ConfigTreeObject_DataSave(QObject *parent = 0);
    void updateStrShowOn();

public slots:
    void slotCreateDatasavesettingTree(QObject *parent = 0);

    void slotDataDelegateChange();

public slots:
    void slotDatasaveDirectNameChanged();
    void slotDatasaveHeaderCharChanged();
    void slotDatasaveFileStructChanged();
    void slotDatasaveFileIdentyChanged();
    void slotDatasaveMediaAutoChanged();
    void slotDatasaveMediaFIFOChanged();
    void slotDatasaveFormatDataChanged();
};












#endif // CONFIGTREEOBJECT_DATASAVE_H
