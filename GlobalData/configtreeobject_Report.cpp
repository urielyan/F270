#include "configtreeobject_Report.h"
#include "cfgchannel.h"
#include "./GlobalData/globalDef.h"
#include "Common/devboard.h"
#include "cfgGlobalDef.h"

#include <QDebug>

ConfigTreeObject_Report *ConfigTreeObject_Report::instance(QObject *parent)
{
    static ConfigTreeObject_Report data(parent);
    return &data;
}

ConfigTreeObject_Report::ConfigTreeObject_Report(QObject *parent):
      ConfigObject(parent)
{
    //Report settings代理树
    QFile fileDelegate(":/configtree/Image/configTree/Report/Report_Delegate/delegate.xml");
    if(!fileDelegate.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file: System_setting!";
    }else{
        XmlConfigParser::parseConfigXml(fileDelegate, static_cast<ConfigObject*>(parent), \
                                                                   (QObject*)this);
    }
}

void ConfigTreeObject_Report::updateStrShowOn()
{
    slotReportBasicTypeChange();

    slotReportBasicTimeDayChange();
    slotReportBasicTimeWeekChange();
    slotReportBasicTimeHourChange();
    slotReportBasicTimeMinuteChange();
    slotReportBasicTimeIntervalChange();
    slotReportBasicTimeFileChange();

    slotReportBasicDataReport1Change();
    slotReportBasicDataReport2Change();
    slotReportBasicDataReport3Change();
    slotReportBasicDataReport4Change();
    slotReportBasicDataReport5Change();

    slotReportBasicFileTypeChange();
    slotReportBasicReportExcelChange();
    slotReportBasicReportPDFChange();
    slotReportBasicReportPrinterChange();

    slotReportBasicElectronicChange();

    slotReportChannelTypeChange();
    slotReportChannelNOChange();
    slotReportChannelSumChange();
    slotReportChannelNumberChange();
}

void ConfigTreeObject_Report::slotCreateReportsettingTree(QObject *parent)
{
    //Report setting 配置树
    QFile file(":/configtree/Image/configTree/Report/Report_Setting/config.xml");
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file: System_setting!";
    }else{
        XmlConfigParser::parseConfigXml(file, static_cast<ConfigObject*>(parent));
        file.close();
    }

    quint32 ID_channel = ConfigTreeObject::getConfigObject(Config_Report_ReportChannnel_Number)->getObjectID();
    ConfigTreeObject::getConfigObject(Config_Report_ReportChannnel_Number)->initData(0, 0, ID_channel);
    ConfigTreeObject::getConfigObject(Config_Report_ReportChannnel_Number)->setStrShowOn("1");
}

void ConfigTreeObject_Report::slotReportDelegateChange()
{
    QFile delegateChange(":/configtree/Image/configTree/Report/Report_DelegateChange/delegateChange.xml");
    if(!delegateChange.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file: Report_settingDelegateChange!";
    }else{
        XmlConfigParser::parseDelegateChangeXml(delegateChange);
        delegateChange.close();
    }

    _curStatus = false;
    updateStrShowOn();
    _curStatus = true;
}

void ConfigTreeObject_Report::slotReportBasicTypeChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(Config_Report_Delegate_Basic_Type)->getData();

    QFile file(XML_Config_Report_Basic_Type);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Report::slotReportBasicTimeDayChange()
{
    QString num;

    ConfigObject *pObj = ConfigTreeObject::getConfigObject(Config_Report_Delegate_Basic_Time_Day);


    if(pObj){
        num = pObj->getData().toString();

        pObj->setStrShowOn(QString("%1").arg(num.toInt()));
    }
}

void ConfigTreeObject_Report::slotReportBasicTimeWeekChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(Config_Report_Delegate_Basic_Time_Week)->getData();

    QFile file(XML_Config_Report_Basic_Time_Week);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Report::slotReportBasicTimeHourChange()
{
    QString num;

    ConfigObject *pObj = ConfigTreeObject::getConfigObject(Config_Report_Delegate_Basic_Time_Hour);


    if(pObj){
        num = pObj->getData().toString();

        pObj->setStrShowOn(QString("%1").arg(num.toInt()));
    }
}

void ConfigTreeObject_Report::slotReportBasicTimeMinuteChange()
{
    QString num;

    ConfigObject *pObj = ConfigTreeObject::getConfigObject(Config_Report_Delegate_Basic_Time_Minute);


    if(pObj){
        num = pObj->getData().toString();

        pObj->setStrShowOn(QString("%1").arg(num.toInt()));
    }
}

void ConfigTreeObject_Report::slotReportBasicTimeIntervalChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(Config_Report_Delegate_Basic_Time_Interval)->getData();

    QFile file(XML_Config_Report_Basic_Time_Interval);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Report::slotReportBasicTimeFileChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(Config_Report_Delegate_Basic_Time_File)->getData();

    QFile file(XML_Config_Report_Basic_Time_File);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Report::slotReportBasicDataReport1Change()
{
    QVariant value = ConfigTreeObject::getConfigObject(Config_Report_Delegate_Basic_Data_Report1)->getData();

    QFile file(XML_Config_Report_Basic_Data_Report1);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Report::slotReportBasicDataReport2Change()
{
    QVariant value = ConfigTreeObject::getConfigObject(Config_Report_Delegate_Basic_Data_Report2)->getData();

    QFile file(XML_Config_Report_Basic_Data_Report2);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Report::slotReportBasicDataReport3Change()
{
    QVariant value = ConfigTreeObject::getConfigObject(Config_Report_Delegate_Basic_Data_Report3)->getData();

    QFile file(XML_Config_Report_Basic_Data_Report3);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Report::slotReportBasicDataReport4Change()
{
    QVariant value = ConfigTreeObject::getConfigObject(Config_Report_Delegate_Basic_Data_Report4)->getData();

    QFile file(XML_Config_Report_Basic_Data_Report4);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Report::slotReportBasicDataReport5Change()
{
    QVariant value = ConfigTreeObject::getConfigObject(Config_Report_Delegate_Basic_Data_Report5)->getData();

    QFile file(XML_Config_Report_Basic_Data_Report5);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Report::slotReportBasicFileTypeChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(Config_Report_Delegate_Basic_FileType)->getData();

    QFile file(XML_Config_Report_Basic_FileType);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Report::slotReportBasicReportExcelChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(Config_Report_Delegate_Basic_Report_Excel)->getData();

    QFile file(XML_Config_Report_Basic_Report_Excel);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Report::slotReportBasicReportPDFChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(Config_Report_Delegate_Basic_Report_PDF)->getData();

    QFile file(XML_Config_Report_Basic_Report_PDF);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Report::slotReportBasicReportPrinterChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(Config_Report_Delegate_Basic_Report_Printer)->getData();

    QFile file(XML_Config_Report_Basic_Report_Printer);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Report::slotReportBasicElectronicChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(Config_Report_Delegate_Basic_Electronic)->getData();

    QFile file(XML_Config_Report_Basic_Electronic);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Report::slotReportChannelTypeChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(Config_Report_Delegate_ReportChannnel_Type)->getData();

    switch (value.toInt()) {
    case Channel_Type_Comm:
        ConfigTreeObject::getConfigObject(Config_Report_Delegate_ReportChannnel_ChannelNo)->changeData(CfgChannel::instance()->getExistChannelComm().first(), false);
        break;
    case Channel_Type_Math:
        ConfigTreeObject::getConfigObject(Config_Report_Delegate_ReportChannnel_ChannelNo)->changeData(CfgChannel::instance()->getExistChannelMath().first(), false);
        break;
    case Channel_Type_AI:
    case Channel_Type_DI:
    case Channel_Type_DO:
    {
        int tempModule, moduleMin;
        int code;
        tempModule = 0;
        QList<DevBoard *> boardList = CfgChannel::instance()->getList_BoardIO();

        moduleMin = boardList.value(0)->getBoardModule();
        for (int i = 0; i < boardList.size(); ++i) {
            if (boardList.value(i)->getBoardModule() < moduleMin) {
                tempModule = i;
                moduleMin = boardList.value(i)->getBoardModule();
            }
        }

        switch (boardList.value(tempModule)->getBoardType()) {
        case STATUS_BOARD_TYPE_AI:
            code = CHANNEL_AI_CODER(boardList.value(tempModule)->getBoardUnit(), moduleMin, 1);
            break;
        case STATUS_BOARD_TYPE_DI:
            code = CHANNEL_DI_CODER(boardList.value(tempModule)->getBoardUnit(), moduleMin, 1);
            break;
        case STATUS_BOARD_TYPE_DO:
            code = CHANNEL_DO_CODER(boardList.value(tempModule)->getBoardUnit(), moduleMin, 1);
            break;
        default:
            break;
        }
        ConfigTreeObject::getConfigObject(Config_Report_Delegate_ReportChannnel_ChannelNo)->changeData(code, false);
        break;
    }
    default:
        break;
    }


    QFile file(XML_Config_Report_ReportChannnel_Type);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Report::slotReportChannelNOChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(Config_Report_Delegate_ReportChannnel_ChannelNo)->getData();

    QFile file(XML_Config_Report_ReportChannnel_ChannelNo);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Report::slotReportChannelSumChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(Config_Report_Delegate_ReportChannnel_Sum)->getData();

    QFile file(XML_Config_Report_ReportChannnel_Sum);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Report::slotReportChannelNumberChange()
{
    QString num;
    ConfigObject *pObj = ConfigTreeObject::getConfigObject(Config_Report_Delegate_ReportChannnel_Number);

    if(pObj){
        num = pObj->getData().toString();
        pObj->setStrShowOn(QString("%1").arg(num.toInt()+1));
    }
}

