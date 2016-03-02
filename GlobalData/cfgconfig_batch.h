#ifndef CFGCONFIG_BATCH_H
#define CFGCONFIG_BATCH_H

#include <QObject>
#include "./GlobalData/configobject.h"
#include "./GlobalData/cfgGlobalDef_Batch.h"

/****************************************  Batch 21900 - 22000  *************************************************/
#define Batch_Start_ID                          21900
#define Batch_End_ID                            21999

//batch
#define ID_Batch                                21900

#define ID_Batch_Function                       21901       //group
#define ID_batch_Function_OnOff                 21902       //批处理开关, 0-off, 1-on
#define ID_batch_Function_Num                   21903       //批处理号码位数， 0(off)/4/6/8
#define ID_batch_Function_AutoInc               21904        //批处理号码自动增加开关，0-off, 1-on

#define ID_Batch_TextText                       21920   //title
#define ID_Batch_Text_Text                      21921
#define ID_Batch_Text_Text_Num                  21922   //
#define ID_Batch_Text_TextFiled                 21923   //group
#define ID_Batch_Text_TextField_Title           21924   //批处理文本区域标题，最多20个半角字符，中英文支持
#define ID_Batch_Text_TextField_TextStr         21925   //批处理文本字符串，最多30个半角字符，中英文支持

#define ID_Batch_Text_Num                       21930   //批处理号码及批次号码
#define ID_Batch_Text_Num_BatchNum              21931   //批处理号码，最多32个字符
#define ID_Batch_Text_Num_LotNum                21932   //批次号码，batchNumber决定数据位数

#define ID_Batch_Text_Comment                   21940   //3个批处理注释，每个最多50个半角字符，中英文支持
#define ID_Batch_Text_Comment_1                 21941
#define ID_Batch_Text_Comment_2                 21942
#define ID_Batch_Text_Comment_3                 21943


class CfgConfig_Batch : public QObject
{
    Q_OBJECT

public:
    static CfgConfig_Batch * instance();
    QVariant getBatchData(quint32 ID, quint32 group);
    void setBatchData(quint32 ID, quint32 group);

    QString getBatchObjStr(quint32 ID, quint32 group = 0);
    QString getBatchObjValStr(quint32 ID, quint32 group, ConfigObject *cfg);

    void setDeviceBatchConfig(BATCH_CONFIG & batchConfig) { p_batchConfig = &batchConfig;}
private:
    explicit CfgConfig_Batch(QObject *parent = 0);

    QVariant getCharToString(uchar *str);
    void setStrToChar(uchar * uData, QString str, int len);
signals:

public slots:

private:
    BATCH_CONFIG *              p_batchConfig;

};

#endif // CFGCONFIG_BATCH_H
