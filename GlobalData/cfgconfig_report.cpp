#include "cfgconfig_report.h"
#include "../Common/devboard.h"
#include "./GlobalData/configtreeobject.h"

CfgConfig_Report *CfgConfig_Report::instance()
{
    static CfgConfig_Report data;
    return &data;
}


CfgConfig_Report::CfgConfig_Report(QObject *parent) :
    QObject(parent)
{
}





QVariant CfgConfig_Report::getReportData(quint32 ID, quint32 group)
{
    if (ID >= Report_Basic_Start_ID && ID <= Report_Basic_End_ID)
        return getReportBasicData(ID, group);
    else
        return getReportChannelData(ID, group);
    return QVariant();
}

QString CfgConfig_Report::getReportObjStr(quint32 ID, quint32 group)
{
    if (ID >= Report_Basic_Start_ID && ID <= Report_Basic_End_ID)
        return getReportBasicObjStr(ID, group);
    else
        return getReportChannelObjStr(ID, group);
    return NULL;
}

QString CfgConfig_Report::getReportObjValStr(quint32 ID, quint32 group, ConfigObject *cfg)
{
    if (ID >= Report_Basic_Start_ID && ID <= Report_Basic_End_ID)
        return getReportBasicObjValStr(ID, group, cfg);
    else
        return getReportChannelObjValStr(ID, group, cfg);
    return NULL;
}

void CfgConfig_Report::setReportData(quint32 ID, quint32 group)
{
    if (ID >= Report_Basic_Start_ID && ID <= Report_Basic_End_ID)
        setReportBasicData(ID, group);
    else
        setReportChannelData(ID, group);
}




QVariant CfgConfig_Report::getReportBasicData(quint32 ID, quint32 /*group*/)
{
    switch (ID) {
    case ID_Report_Basic_Type_Value:
        return p_reportCfg->reportType;
        break;
    case ID_Report_Basic_CreateTime_H:
        return p_reportCfg->reportHourly;
        break;
    case ID_Report_Basic_CreateTime_Min:
        return p_reportCfg->reportMinute;
        break;
    case ID_Report_Basic_CreateTime_Day:
        return p_reportCfg->reportDay;
        break;
    case ID_Report_Basic_CreateTime_Week:
        return p_reportCfg->reportWeek;
        break;
    case ID_Report_Basic_CreateTime_Period:
        return p_reportCfg->reportPeriod;
        break;
    case ID_Report_Basic_CreateTime_CreatePeriod:
        return p_reportCfg->reportFileCreatePeriod;
        break;

    case ID_Report_Basic_DataType_1:
        return p_reportCfg->reportDataType[0];
        break;
    case ID_Report_Basic_DataType_2:
        return p_reportCfg->reportDataType[1];
        break;
    case ID_Report_Basic_DataType_3:
        return p_reportCfg->reportDataType[2];
        break;
    case ID_Report_Basic_DataType_4:
        return p_reportCfg->reportDataType[3];
        break;
    case ID_Report_Basic_DataType_5:
        return p_reportCfg->reportDataType[4];
        break;

    case ID_Report_Basic_FileTypeValue:
        return p_reportCfg->reportFile;
        break;

    case ID_Report_Basic_Execl:
        return p_reportCfg->reportExcel;
        break;
    case ID_Report_Basic_PDF:
        return p_reportCfg->reportPDF;
        break;
    case ID_Report_Basic_Printer:
        return p_reportCfg->reportPrinter;
        break;

    case ID_Report_Basic_Signature_OnOff:
        return p_reportCfg->reportElecSignature;
        break;
    default:
        break;
    }
    return QVariant();
}

QVariant CfgConfig_Report::getReportChannelData(quint32 ID, quint32 group)
{
    switch (ID) {
    case ID_Report_Ch_ReportCh_Type:
        return p_reportCfg->reportChannelType[group];
        break;
    case ID_Report_Ch_ReportCh_NO:
    {
//        QList<quint32> list = CfgChannel::instance()->getEffectiveChannel();
//        qDebug() << list.count();
//        if (list.contains(p_reportCfg->reportChannelNum[group]))
        return p_reportCfg->reportChannelNum[group];
//        else
//            return list.first();
    }
        break;
    case ID_Report_Ch_ReportCh_Sum:
        return p_reportCfg->reportSumUnit[group];
        break;
    default:
        break;
    }
    return QVariant();
}



/*
 * 通道通道编码得到界面上显示的相应通道编码的样式
 *          code 是从底层配置结构体中获取，此时code是一个quint32类型的数字
*/
QString CfgConfig_Report::getReportChValObjStr(quint32 data)
{
    int type = CHANNEL_TYPE(data);
    if (type == Channel_Type_AI)
        return CfgChannel::instance()->getExistChannelAI().key(data);
    else if (type == Channel_Type_DI)
        return CfgChannel::instance()->getExistChannelDI().key(data);
    else if (type == Channel_Type_DO)
        return CfgChannel::instance()->getExistChannelDO().key(data);
    else if (type == Channel_Type_Comm)
    {
        QString str = CfgChannel::instance()->getExistChannelComm().key(data);
        str = "0" + str.right(3);
        return str;
    }
    else if (type == Channel_Type_Math)
    {
        QString str = CfgChannel::instance()->getExistChannelMath().key(data);
        str = "0" + str.right(3);
        return str;
    }
    else
        return "A";
}








QString CfgConfig_Report::getReportBasicObjStr(quint32 ID, quint32 /*group*/)
{
    switch (ID) {
    case ID_Report:
        return tr("Report settings");
        break;
    case ID_Report_Basic:
        return tr("Basic settings");
        break;
    case ID_Report_Basic_Type:
    case ID_Report_Basic_Type_Value:
        return tr("Type");
        break;

    case ID_Report_Basic_CreateTime:
        return tr("Creation time");
        break;
    case ID_Report_Basic_CreateTime_H:
        return tr("Hour");
        break;
    case ID_Report_Basic_CreateTime_Min:
        return tr("Minute");
        break;
    case ID_Report_Basic_CreateTime_Day:
        return tr("Day");
        break;
    case ID_Report_Basic_CreateTime_Week:
        return tr("Day of week");
        break;
    case ID_Report_Basic_CreateTime_Period:
        return tr("Save interval");
        break;
    case ID_Report_Basic_CreateTime_CreatePeriod:
        return tr("File creation interval");
        break;

    case ID_Report_Basic_DataType:
        return tr("Data type");
        break;
    case ID_Report_Basic_DataType_1:
        return tr("Report 1");
        break;
    case ID_Report_Basic_DataType_2:
        return tr("Report 2");
        break;
    case ID_Report_Basic_DataType_3:
        return tr("Report 3");
        break;
    case ID_Report_Basic_DataType_4:
        return tr("Report 4");
        break;
    case ID_Report_Basic_DataType_5:
        return tr("Report 5");
        break;

    case ID_Report_Basic_FileType:
    case ID_Report_Basic_FileTypeValue:
        return tr("File type");
        break;

    case ID_Report_Basic_Output:
        return tr("Report template output");
        break;
    case ID_Report_Basic_Execl:
        return tr("Excel file");
        break;
    case ID_Report_Basic_PDF:
        return tr("PDF file");
        break;
    case ID_Report_Basic_Printer:
        return tr("Printer");
        break;

    case ID_Report_Basic_Signature:
        return tr("Elevtronic signature");
        break;
    case ID_Report_Basic_Signature_OnOff:
        return tr("PDF electronic signature");
        break;
    default:
        break;
    }
    return NULL;
}


QString CfgConfig_Report::getReportChannelObjStr(quint32 ID, quint32 /*group*/)
{
    switch (ID) {
    case ID_Report_Ch:
        return tr("Report channel settings");
        break;
    case ID_Report_Ch_Num:
        return tr("Report channel numner");
        break;
    case ID_Report_Ch_ReportCh:
        return tr("Channel type");
        break;
    case ID_Report_Ch_ReportCh_Type:
        return tr("Report channel");
        break;
    case ID_Report_Ch_ReportCh_NO:
        return tr("Channel no");
        break;
    case ID_Report_Ch_ReportCh_Sum:
        return tr("Sum scale");
        break;
    default:
        break;
    }
    return NULL;
}






QString CfgConfig_Report::getReportBasicObjValStr(quint32 ID, quint32 /*group*/, ConfigObject *cfg)
{
    switch (ID) {
    case ID_Report_Basic_Type_Value:
        return getReportBasicTypeObjValStr(cfg->getData().toInt());
        break;
    case ID_Report_Basic_CreateTime_Week:
        return getReportBasicWeekObjValStr(cfg->getData().toInt());
        break;
    case ID_Report_Basic_CreateTime_Period:
    {
        int data = cfg->getData().toInt();
        if (data < 60)
            return QString("%1 min").arg(data);
        else
            return tr("1 h");
    }
        break;
    case ID_Report_Basic_CreateTime_CreatePeriod:
        return QString("%1 h").arg(cfg->getData().toInt());
        break;

    case ID_Report_Basic_DataType_1:
    case ID_Report_Basic_DataType_2:
    case ID_Report_Basic_DataType_3:
    case ID_Report_Basic_DataType_4:
    case ID_Report_Basic_DataType_5:
        return getReportBasicReportObjValStr(cfg->getData().toInt());
        break;

    case ID_Report_Basic_FileTypeValue:
        return cfg->getData().toInt() == 0 ? tr("Seperate") : tr("Combine");
        break;
    case ID_Report_Basic_Execl:
    case ID_Report_Basic_PDF:
    case ID_Report_Basic_Printer:
    case ID_Report_Basic_Signature_OnOff:
        return cfg->getData().toInt() == 0 ? tr("Off") : tr("On");
        break;
    default:
        break;
    }
    return cfg->getData().toString();
}

QString CfgConfig_Report::getReportChannelObjValStr(quint32 ID, quint32 /*group*/, ConfigObject *cfg)
{
    switch (ID) {
    case ID_Report_Ch_ReportCh_Type:
    {
        int type = cfg->getData().toInt();
        if (type == Channel_Type_Off)
            return tr("Off");
        else if (type == Channel_Type_AI || type == Channel_Type_DI || type == Channel_Type_DO)
            return tr("I/O channel");
        else if (type == Channel_Type_Comm)
            return tr("Communication channel");
        else
            return tr("Math");
    }
        break;
    case ID_Report_Ch_ReportCh_NO:
        return getReportChValObjStr(cfg->getData().toUInt());
        break;
    case ID_Report_Ch_ReportCh_Sum:
    {
        int type = cfg->getData().toInt();
        if (type == Report_Sum_Unit_Off)
            return tr("Off");
        else if (type == Report_Sum_Unit_S)
            return tr("/s");
        else if (type == Report_Sum_Unit_Min)
            return tr("/min");
        else if (type == Report_Sum_Unit_H)
            return tr("/h");
        else
            return tr("/day");
    }
        break;
    default:
        break;
    }
    return cfg->getData().toString();
}


QString CfgConfig_Report::getReportBasicTypeObjValStr(quint32 type)
{
    switch (type) {
    case Report_Type_Off:
        return tr("Off");
        break;
    case Report_Type_Hourly_Daily:
        return tr("Hourly + Daily");
        break;
    case Report_Type_Daily_Weekly:
        return tr("Daily + Weekly");
        break;
    case Report_Type_Daily_Monthly:
        return tr("Weekly + Monthy");
        break;
    case Report_Type_Batch:
        return tr("Batch");
        break;
    case Report_Type_Daily_Custom:
        return tr("Daily custom");
        break;
    default:
        break;
    }
    return NULL;
}

QString CfgConfig_Report::getReportBasicWeekObjValStr(int type)
{
    switch (type) {
    case 0:
        return tr("Sunday");
        break;
    case 1:
        return tr("Monday");
        break;
    case 2:
        return tr("Tuesday");
        break;
    case 3:
        return tr("Wednesday");
        break;
    case 4:
        return tr("Thursday");
        break;
    case 5:
        return tr("Friday");
        break;
    case 6:
        return tr("Satuary");
        break;
    default:
        break;
    }
    return NULL;
}

QString CfgConfig_Report::getReportBasicReportObjValStr(int type)
{
    switch (type) {
    case Report_Data_Type_AVG:
        return tr("Ave");
        break;
    case Report_Data_Type_MAX:
        return tr("Max");
        break;
    case Report_Data_Type_MIN:
        return tr("Min");
        break;
    case Report_Data_Type_SUM:
        return tr("Sum");
        break;
    case Report_Data_Type_INST:
        return tr("Inst");
        break;
    default:
        break;
    }
    return NULL;
}







void CfgConfig_Report::setReportBasicData(quint32 ID, quint32 /*group*/)
{
    switch (ID) {
    case ID_Report_Basic_Type_Value:
        p_reportCfg->reportType = (Report_Type)ConfigTreeObject::getConfigData(Config_Report_Basic_Type).toInt();
        break;
    case ID_Report_Basic_CreateTime_H:
        p_reportCfg->reportHourly = ConfigTreeObject::getConfigData(Config_Report_Basic_Time_Hour).toInt();
        break;
    case ID_Report_Basic_CreateTime_Min:
        p_reportCfg->reportMinute = ConfigTreeObject::getConfigData(Config_Report_Basic_Time_Minute).toInt();
        break;
    case ID_Report_Basic_CreateTime_Day:
         p_reportCfg->reportDay = ConfigTreeObject::getConfigData(Config_Report_Basic_Time_Day).toInt();
        break;
    case ID_Report_Basic_CreateTime_Week:
        p_reportCfg->reportWeek = ConfigTreeObject::getConfigData(Config_Report_Basic_Time_Week).toInt();
        break;
    case ID_Report_Basic_CreateTime_Period:
        p_reportCfg->reportPeriod = ConfigTreeObject::getConfigData(Config_Report_Basic_Time_Interval).toInt();
        break;
    case ID_Report_Basic_CreateTime_CreatePeriod:
        p_reportCfg->reportFileCreatePeriod = ConfigTreeObject::getConfigData(Config_Report_Basic_Time_File).toInt();
        break;

    case ID_Report_Basic_DataType_1:
        p_reportCfg->reportDataType[0] = (Report_Data_Type)ConfigTreeObject::getConfigData(Config_Report_Basic_Data_Report1).toInt();
        break;
    case ID_Report_Basic_DataType_2:
        p_reportCfg->reportDataType[1] = (Report_Data_Type)ConfigTreeObject::getConfigData(Config_Report_Basic_Data_Report2).toInt();
        break;
    case ID_Report_Basic_DataType_3:
        p_reportCfg->reportDataType[2] = (Report_Data_Type)ConfigTreeObject::getConfigData(Config_Report_Basic_Data_Report3).toInt();
        break;
    case ID_Report_Basic_DataType_4:
        p_reportCfg->reportDataType[3] = (Report_Data_Type)ConfigTreeObject::getConfigData(Config_Report_Basic_Data_Report4).toInt();
        break;
    case ID_Report_Basic_DataType_5:
        p_reportCfg->reportDataType[4] = (Report_Data_Type)ConfigTreeObject::getConfigData(Config_Report_Basic_Data_Report5).toInt();
        break;

    case ID_Report_Basic_FileTypeValue:
        p_reportCfg->reportFile = ConfigTreeObject::getConfigData(Config_Report_Basic_FileType).toInt();
        break;

    case ID_Report_Basic_Execl:
        p_reportCfg->reportExcel = ConfigTreeObject::getConfigData(Config_Report_Basic_Report_Excel).toInt();
        break;
    case ID_Report_Basic_PDF:
        p_reportCfg->reportPDF = ConfigTreeObject::getConfigData(Config_Report_Basic_Report_PDF).toInt();
        break;
    case ID_Report_Basic_Printer:
        p_reportCfg->reportPrinter = ConfigTreeObject::getConfigData(Config_Report_Basic_Report_Printer).toInt();
        break;

    case ID_Report_Basic_Signature_OnOff:
        p_reportCfg->reportElecSignature = ConfigTreeObject::getConfigData(Config_Report_Basic_Electronic).toInt();
        break;
    default:
        break;
    }
}

void CfgConfig_Report::setReportChannelData(quint32 ID, quint32 group)
{
    switch (ID) {
    case ID_Report_Ch_ReportCh_Type:
    {
        quint32 num = ConfigTreeObject::getConfigData(QString(Config_Report_ReportChannnel_ChannelNo).arg(group)).toUInt();
        p_reportCfg->reportChannelType[group] = (Channel_Type)CHANNEL_TYPE(num);
//         = (Channel_Type)ConfigTreeObject::getConfigData(QString(Config_Report_ReportChannnel_Type).arg(group)).toInt();
    }
        break;
    case ID_Report_Ch_ReportCh_NO:
        p_reportCfg->reportChannelNum[group] = ConfigTreeObject::getConfigData(QString(Config_Report_ReportChannnel_ChannelNo).arg(group)).toUInt();
        break;
    case ID_Report_Ch_ReportCh_Sum:
        p_reportCfg->reportSumUnit[group] = (Report_Sum_Unit)ConfigTreeObject::getConfigData(QString(Config_Report_ReportChannnel_Sum).arg(group)).toInt();
        break;
    default:
        break;
    }
}

