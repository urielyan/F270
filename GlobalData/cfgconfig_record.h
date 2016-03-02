#ifndef CFGCONFIG_RECORD_H
#define CFGCONFIG_RECORD_H

#include <QObject>
#include "./GlobalData/globalDef.h"
#include "./GlobalData/configobject.h"

/****************************************  Record 21600 - 21700  *************************************************/
#define Record_Start_ID                                 21600
#define Record_End_ID                                   21699

#define Record_Basic_Start_ID                           21600
#define Record_Basic_End_ID                             21639

#define Record_CH_Start_ID                              21640
#define Record_CH_End_ID                                21699

//record
#define ID_Record                                       21600

#define ID_Record_Basic                                 21601   //title

#define ID_Record_Basic_Mode                            21602   //group
#define ID_Record_Basic_Mode_Value                      21603       //数据记录模式
//event
#define ID_Record_Basic_Event                           21610    //group(事件数据)
#define ID_Record_Basic_Event_Interval                  21611   //事件记录周期
#define ID_Record_Basic_Event_Mode                      21612    //事件记录模式
#define ID_Record_Basic_Event_Len                       21613   //事件记录数据长度
#define ID_Record_Basic_Event_SignaFlag                 21614   //单次触发执行标记
#define ID_Record_Basic_Event_Triggered                 21615   //触发标记
#define ID_Record_Basic_Event_TriggeredSrc              21616   //触发源操作，0-off, 1-on， 通过画面操作触发，【自由】模式外有效
#define ID_Record_Basic_Event_TriggerPre                21617   //事件前置触发(%),范围:0/5/25/50/75/95/10, 【自由】模式外有效
//disp
#define ID_Record_Basic_Disp                            21620   //group 显示数据/趋势波形
#define ID_Record_Basic_Disp_Interval                   21621   //显示记录数据长度(保存周期)

#define ID_Record_Basic_Confirm                         21630   //记录画面确认
#define ID_Record_Basic_Confirm_OnOff                   21631   //记录画面确认，0-off, 1-on

//CH
#define ID_Record_CH                                    21640   //记录通道设定 title

#define ID_Record_Basic_Event_ChNum                     21641   //事件记录通道数，Max:500
#define ID_Record_Basic_Event_ChItem                    21642   //事件记录通道号

#define ID_Record_Basic_Disp_ChNum                      21651   //显示记录通道数，Max:500
#define ID_Record_Basic_Disp_ChItem                     21652   //显示记录通道号

#define ID_Record_Basic_Sample_ChNum                    21661   //手动采样通道数，Max:50
#define ID_Record_Basic_Sample_ChItem                   21662   //手动采样通道号


class CfgConfig_Record : public QObject
{
    Q_OBJECT
public:
    static CfgConfig_Record * instance();

    QVariant getRecordData(quint32 ID, quint32 group);
    void setRecordData(quint32 ID, quint32 group);

    QString getRecordObjStr(quint32 ID, quint32 group = 0);     //第二个参数先保留，目前没有用到
    QString getRecordObjValStr(quint32 ID, quint32 group, ConfigObject *cfg);

    void setDeviceRecordConfig(DEVICE_CONFIG  &recordConfig) {p_recordConfig = &recordConfig;}
public:
    explicit CfgConfig_Record(QObject *parent = 0);

    QVariant getRecordBasicData(quint32 ID, quint32 group);
    QVariant getRecordChannelData(quint32 ID, quint32 group);

    QString getRecordBasicObjStr(quint32 ID, quint32 group = 0);
    QString getRecordChannelObjStr(quint32 ID, quint32 group = 0);

    QString getRecordBasicObjValStr(quint32 ID, quint32 group, ConfigObject * cfg);
    QString getRecordChannelObjValStr(quint32 ID, quint32 group, ConfigObject * cfg);

    void setRecordBasicData(quint32 ID, quint32 group);
    void setRecordChannelData(quint32 ID, quint32 group);

signals:

public slots:

private:
    DEVICE_CONFIG *                 p_recordConfig;

};

#endif // CFGCONFIG_RECORD_H
