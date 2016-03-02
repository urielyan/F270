#ifndef CONFIGTREEOBJECT_BATCH_H
#define CONFIGTREEOBJECT_BATCH_H


#include "configobject.h"

//批处理 setting path
#define Config_Batch_Setting_TextCharacters     "Config:Batch settings:Batch text:Batch text:%1:Text field:Characters"
#define Config_Batch_Setting_TextField          "Config:Batch settings:Batch text:Batch text:%1:Text field:Title of field"
#define Config_Batch_Setting_TextNum            "Config:Batch settings:Batch text:Batch text:Text num"
#define Config_Batch_Setting_Auto               "Config:Batch settings:Batch function:Auto increment"
#define Config_Batch_Setting_Lot                "Config:Batch settings:Batch function:Lot-No. digit"
#define Config_Batch_Setting_function           "Config:Batch settings:Batch function:On/Off"

//批处理 delegate path
#define Config_Batch_Delegate_TextCharacters    "Config:Batch Delegate:Batch text:Batch text:Text field:Characters"
#define Config_Batch_Delegate_TextField         "Config:Batch Delegate:Batch text:Batch text:Text field:Title of field"
#define Config_Batch_Delegate_TextNum           "Config:Batch Delegate:Batch text:Batch text:Text num"
#define Config_Batch_Delegate_Auto              "Config:Batch Delegate:Batch function:Auto increment"
#define Config_Batch_Delegate_Lot               "Config:Batch Delegate:Batch function:Lot-No. digit"
#define Config_Batch_Delegate_function          "Config:Batch Delegate:Batch function:On/Off"

//批处理 XML path
#define XML_Config_Batch_Setting_TextCharacters ":/configtree/Image/configTree/Batch/Batch_ValueChange/Batch_Character.xml"
#define XML_Config_Batch_Setting_TextField      ":/configtree/Image/configTree/Batch/Batch_ValueChange/Batch_Tiltle.xml"
#define XML_Config_Batch_Setting_Auto           ":/configtree/Image/configTree/Batch/Batch_ValueChange/Batch_Auto.xml"
#define XML_Config_Batch_Setting_Lot            ":/configtree/Image/configTree/Batch/Batch_ValueChange/Batch_Lot.xml"
#define XML_Config_Batch_Setting_function       ":/configtree/Image/configTree/Batch/Batch_ValueChange/Batch_Function.xml"

class ConfigTreeObject_Batch : public ConfigObject
{
    Q_OBJECT
public:
    static ConfigTreeObject_Batch *instance(QObject *parent);

signals:

private:
    explicit ConfigTreeObject_Batch(QObject *parent = 0);

    void updateStrShowOn();

public slots:
    void slotCreateBatchsettingTree(QObject *parent = 0);

    void slotBatchDelegateChange();

    void slotConfigBatchFunctionChange();
    void slotConfigBatchLotChange();
    void slotConfigBatchAutoChange();
    void slotConfigBatchTextNumChange();
    void slotConfigBatchTextFieldChange();
    void slotConfigBatchTextCharactersChange();


    void slotConfigBatchTextNumReationChange();

};

#endif // CONFIGTREEOBJECT_BATCH_H
