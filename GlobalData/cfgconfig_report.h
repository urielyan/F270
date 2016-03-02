#ifndef CFGCONFIG_REPORT_H
#define CFGCONFIG_REPORT_H

#include <QObject>
#include "./GlobalData/cfgGlobalDef_Report.h"
#include "./GlobalData/configobject.h"

/****************************************  report 22000 - 22100  *************************************************/
#define Report_Start_ID                             22000
#define Report_End_ID                               22099

#define Report_Basic_Start_ID                       22000
#define Report_Basic_End_ID                         22059

#define Report_CH_Start_ID                          22060
#define Report_CH_End_ID                            22099

//report
#define ID_Report                                   22000

#define ID_Report_Basic                             22001

#define ID_Report_Basic_Type                        22002       //group
#define ID_Report_Basic_Type_Value                  22003       //报表类型

#define ID_Report_Basic_CreateTime                  22010
#define ID_Report_Basic_CreateTime_H                22011   //时：0~23
#define ID_Report_Basic_CreateTime_Min              22012   //分：0~59
#define ID_Report_Basic_CreateTime_Day              22013   //日：1~28
#define ID_Report_Basic_CreateTime_Week             22014   //星期几：1~7
#define ID_Report_Basic_CreateTime_Period           22015   //保存周期，单位:min, 范围：2min/3min/4min/5min/10min/15min/30min/1h
#define ID_Report_Basic_CreateTime_CreatePeriod     22016   //文件创建周期，单位:h，范围：4h/6h/8h/12h/24h

#define ID_Report_Basic_DataType                    22020
#define ID_Report_Basic_DataType_1                  22021
#define ID_Report_Basic_DataType_2                  22022
#define ID_Report_Basic_DataType_3                  22023
#define ID_Report_Basic_DataType_4                  22024
#define ID_Report_Basic_DataType_5                  22025

#define ID_Report_Basic_FileType                    22030   //group
#define ID_Report_Basic_FileTypeValue               22031    //创建文件类型：0-单个，1-多个，默认：0

#define ID_Report_Basic_Output                      22040
#define ID_Report_Basic_Execl                       22041    //Excel文件输出开关，0-off, 1-on
#define ID_Report_Basic_PDF                         22042   //PDF文件输出开关，0-off, 1-on
#define ID_Report_Basic_Printer                     22043   //打印机输出，0-off, 1-on

#define ID_Report_Basic_Signature                   22050   //PDF电子签名开关
#define ID_Report_Basic_Signature_OnOff             22051   //PDF电子签名开关，0-off, 1-on

//report channel
#define ID_Report_Ch                                22060   //title

#define ID_Report_Ch_Num                            22061   //channel num

#define ID_Report_Ch_ReportCh                       22070
#define ID_Report_Ch_ReportCh_Type                  22071       //通道类型
#define ID_Report_Ch_ReportCh_NO                    22072       //通道编码
#define ID_Report_Ch_ReportCh_Sum                   22073        //累计单位


class CfgConfig_Report : public QObject
{
    Q_OBJECT
public:
    static CfgConfig_Report * instance();
    void setDeviceReportConfig(REPORT_CONFIG & report) { p_reportCfg = &report;}

    QVariant getReportData(quint32 ID, quint32 group);      //get data
    QString getReportObjStr(quint32 ID, quint32 group = 0);         //get obj str
    QString getReportObjValStr(quint32 ID, quint32 group, ConfigObject * cfg);         //get obj val str
    void setReportData(quint32 ID, quint32 group);      //save data

private:
    explicit CfgConfig_Report(QObject *parent = 0);

    QVariant getReportBasicData(quint32 ID, quint32 group);
    QVariant getReportChannelData(quint32 ID, quint32 group);

    QString getReportChValObjStr(quint32 data);

    QString getReportBasicObjStr(quint32 ID, quint32 group = 0);
    QString getReportChannelObjStr(quint32 ID, quint32 group = 0);

    QString getReportBasicObjValStr(quint32 ID, quint32 group, ConfigObject * cfg);
    QString getReportChannelObjValStr(quint32 ID, quint32 group, ConfigObject * cfg);

    QString getReportBasicTypeObjValStr(quint32 type);
    QString getReportBasicWeekObjValStr(int type);
    QString getReportBasicReportObjValStr(int type);


    void setReportBasicData(quint32 ID, quint32 group);
    void setReportChannelData(quint32 ID, quint32 group);

signals:

public slots:

private:
    REPORT_CONFIG *                     p_reportCfg;

};

#endif // CFGCONFIG_REPORT_H
