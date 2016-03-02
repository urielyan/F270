#ifndef CONFIGTREEOBJECT_REPORT_H
#define CONFIGTREEOBJECT_REPORT_H

#include "configobject.h"

//setting path
#define Config_Report_ReportChannnel_Number     "Config:Report settings:Report channel settings:number"
#define Config_Report_ReportChannnel_Sum        "Config:Report settings:Report channel settings:%1:Report channel:Sum scale"
#define Config_Report_ReportChannnel_ChannelNo  "Config:Report settings:Report channel settings:%1:Report channel:Channel No."
#define Config_Report_ReportChannnel_Type       "Config:Report settings:Report channel settings:%1:Report channel:Channel type"
#define Config_Report_Basic_Electronic          "Config:Report settings:Basic settings:Electronic signature:PDF electronic signature"
#define Config_Report_Basic_Report_Printer      "Config:Report settings:Basic settings:Report template output:Printer"
#define Config_Report_Basic_Report_PDF          "Config:Report settings:Basic settings:Report template output:PDF file"
#define Config_Report_Basic_Report_Excel        "Config:Report settings:Basic settings:Report template output:Excel file"
#define Config_Report_Basic_FileType            "Config:Report settings:Basic settings:File type:File type"
#define Config_Report_Basic_Data_Report5        "Config:Report settings:Basic settings:Data type:Report5"
#define Config_Report_Basic_Data_Report4        "Config:Report settings:Basic settings:Data type:Report4"
#define Config_Report_Basic_Data_Report3        "Config:Report settings:Basic settings:Data type:Report3"
#define Config_Report_Basic_Data_Report2        "Config:Report settings:Basic settings:Data type:Report2"
#define Config_Report_Basic_Data_Report1        "Config:Report settings:Basic settings:Data type:Report1"
#define Config_Report_Basic_Time_File           "Config:Report settings:Basic settings:Creation time:File creation interval"
#define Config_Report_Basic_Time_Interval       "Config:Report settings:Basic settings:Creation time:Save interval"
#define Config_Report_Basic_Time_Minute         "Config:Report settings:Basic settings:Creation time:Minute"
#define Config_Report_Basic_Time_Hour           "Config:Report settings:Basic settings:Creation time:Hour"
#define Config_Report_Basic_Time_Week           "Config:Report settings:Basic settings:Creation time:Day of week"
#define Config_Report_Basic_Time_Day            "Config:Report settings:Basic settings:Creation time:Day"
#define Config_Report_Basic_Type                "Config:Report settings:Basic settings:Type:Type"

//Delegate path
#define Config_Report_Delegate_ReportChannnel_Number    "Config:Report Delegate:Report channel settings:number"
#define Config_Report_Delegate_ReportChannnel_Sum       "Config:Report Delegate:Report channel settings:Report channel:Sum scale"
#define Config_Report_Delegate_ReportChannnel_ChannelNo "Config:Report Delegate:Report channel settings:Report channel:Channel No."
#define Config_Report_Delegate_ReportChannnel_Type      "Config:Report Delegate:Report channel settings:Report channel:Channel type"
#define Config_Report_Delegate_Basic_Electronic         "Config:Report Delegate:Basic settings:Electronic signature:PDF electronic signature"
#define Config_Report_Delegate_Basic_Report_Printer     "Config:Report Delegate:Basic settings:Report template output:Printer"
#define Config_Report_Delegate_Basic_Report_PDF         "Config:Report Delegate:Basic settings:Report template output:PDF file"
#define Config_Report_Delegate_Basic_Report_Excel       "Config:Report Delegate:Basic settings:Report template output:Excel file"
#define Config_Report_Delegate_Basic_FileType           "Config:Report Delegate:Basic settings:File type:File type"
#define Config_Report_Delegate_Basic_Data_Report5       "Config:Report Delegate:Basic settings:Data type:Report5"
#define Config_Report_Delegate_Basic_Data_Report4       "Config:Report Delegate:Basic settings:Data type:Report4"
#define Config_Report_Delegate_Basic_Data_Report3       "Config:Report Delegate:Basic settings:Data type:Report3"
#define Config_Report_Delegate_Basic_Data_Report2       "Config:Report Delegate:Basic settings:Data type:Report2"
#define Config_Report_Delegate_Basic_Data_Report1       "Config:Report Delegate:Basic settings:Data type:Report1"
#define Config_Report_Delegate_Basic_Time_File          "Config:Report Delegate:Basic settings:Creation time:File creation interval"
#define Config_Report_Delegate_Basic_Time_Interval      "Config:Report Delegate:Basic settings:Creation time:Save interval"
#define Config_Report_Delegate_Basic_Time_Minute        "Config:Report Delegate:Basic settings:Creation time:Minute"
#define Config_Report_Delegate_Basic_Time_Hour          "Config:Report Delegate:Basic settings:Creation time:Hour"
#define Config_Report_Delegate_Basic_Time_Week          "Config:Report Delegate:Basic settings:Creation time:Day of week"
#define Config_Report_Delegate_Basic_Time_Day           "Config:Report Delegate:Basic settings:Creation time:Day"
#define Config_Report_Delegate_Basic_Type               "Config:Report Delegate:Basic settings:Type:Type"

//XML path
#define XML_Config_Report_ReportChannnel_Sum        ":/configtree/Image/configTree/Report/Report_ValueChange/Report_Channel_Sum.xml"
#define XML_Config_Report_ReportChannnel_ChannelNo  ":/configtree/Image/configTree/Report/Report_ValueChange/Report_Channel_NO.xml"
#define XML_Config_Report_ReportChannnel_Type       ":/configtree/Image/configTree/Report/Report_ValueChange/Report_Channel_Type.xml"
#define XML_Config_Report_Basic_Electronic          ":/configtree/Image/configTree/Report/Report_ValueChange/Report_Basic_Elecreonic.xml"
#define XML_Config_Report_Basic_Report_Printer      ":/configtree/Image/configTree/Report/Report_ValueChange/Report_Basic_Printer.xml"
#define XML_Config_Report_Basic_Report_PDF          ":/configtree/Image/configTree/Report/Report_ValueChange/Report_Basic_PDF.xml"
#define XML_Config_Report_Basic_Report_Excel        ":/configtree/Image/configTree/Report/Report_ValueChange/Report_Basic_Excel.xml"
#define XML_Config_Report_Basic_FileType            ":/configtree/Image/configTree/Report/Report_ValueChange/Report_Basic_FileType.xml"
#define XML_Config_Report_Basic_Data_Report5        ":/configtree/Image/configTree/Report/Report_ValueChange/Report_Basic_Report5.xml"
#define XML_Config_Report_Basic_Data_Report4        ":/configtree/Image/configTree/Report/Report_ValueChange/Report_Basic_Report4.xml"
#define XML_Config_Report_Basic_Data_Report3        ":/configtree/Image/configTree/Report/Report_ValueChange/Report_Basic_Report3.xml"
#define XML_Config_Report_Basic_Data_Report2        ":/configtree/Image/configTree/Report/Report_ValueChange/Report_Basic_Report2.xml"
#define XML_Config_Report_Basic_Data_Report1        ":/configtree/Image/configTree/Report/Report_ValueChange/Report_Basic_Report1.xml"
#define XML_Config_Report_Basic_Time_File           ":/configtree/Image/configTree/Report/Report_ValueChange/Report_Basic_File.xml"
#define XML_Config_Report_Basic_Time_Interval       ":/configtree/Image/configTree/Report/Report_ValueChange/Report_Basic_Interval.xml"
#define XML_Config_Report_Basic_Time_Week           ":/configtree/Image/configTree/Report/Report_ValueChange/Report_Basic_Week.xml"
#define XML_Config_Report_Basic_Type                ":/configtree/Image/configTree/Report/Report_ValueChange/Report_Basic_Type.xml"

class ConfigTreeObject_Report : public ConfigObject
{
    Q_OBJECT
public:
    static ConfigTreeObject_Report *instance(QObject *parent);

signals:

private:
    explicit ConfigTreeObject_Report(QObject *parent = 0);

    void updateStrShowOn();

public slots:
    void slotCreateReportsettingTree(QObject *parent = 0);

    void slotReportDelegateChange();

    void slotReportBasicTypeChange();
    void slotReportBasicTimeDayChange();
    void slotReportBasicTimeWeekChange();
    void slotReportBasicTimeHourChange();
    void slotReportBasicTimeMinuteChange();

    void slotReportBasicTimeIntervalChange();

    void slotReportBasicTimeFileChange();

    void slotReportBasicDataReport1Change();
    void slotReportBasicDataReport2Change();
    void slotReportBasicDataReport3Change();
    void slotReportBasicDataReport4Change();
    void slotReportBasicDataReport5Change();

    void slotReportBasicFileTypeChange();

    void slotReportBasicReportExcelChange();
    void slotReportBasicReportPDFChange();
    void slotReportBasicReportPrinterChange();

    void slotReportBasicElectronicChange();

    void slotReportChannelTypeChange();
    void slotReportChannelNOChange();
    void slotReportChannelSumChange();

    void slotReportChannelNumberChange();

};












#endif // CONFIGTREEOBJECT_REPORT_H
