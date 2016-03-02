#include "configtreeobject_Timer.h"
#include "cfgchannel.h"
#include "./GlobalData/globalDef.h"

#include "cfgGlobalDef.h"
#include <QDebug>

ConfigTreeObject_Timer *ConfigTreeObject_Timer::instance(QObject *parent)
{
    static ConfigTreeObject_Timer data(parent);
    return &data;
}

ConfigTreeObject_Timer::ConfigTreeObject_Timer(QObject *parent):
      ConfigObject(parent)
{
    //Timer settings代理树
    QFile fileDelegate(":/configtree/Image/configTree/Timer/Timer_Delegate/delegate.xml");
    if(!fileDelegate.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file: Timer_Delegate!";
    }else{
        XmlConfigParser::parseConfigXml(fileDelegate, static_cast<ConfigObject*>(parent), \
                                                                   (QObject*)this);
    }
}

void ConfigTreeObject_Timer::updateStrShowOn()
{
    slotTImerSettingTime1TypeChange();
    slotTImerSettingTime1DayChange();
    slotTImerSettingTime1HourChange();
    slotTImerSettingTime1MinuteChange();
    slotTImerSettingTime1IntervalChange();

    slotTImerSettingTime2TypeChange();
    slotTImerSettingTime2DayChange();
    slotTImerSettingTime2HourChange();
    slotTImerSettingTime2MinuteChange();
    slotTImerSettingTime2IntervalChange();

    slotTImerSettingTime3TypeChange();
    slotTImerSettingTime3DayChange();
    slotTImerSettingTime3HourChange();
    slotTImerSettingTime3MinuteChange();
    slotTImerSettingTime3IntervalChange();

    slotTImerSettingTime4TypeChange();
    slotTImerSettingTime4DayChange();
    slotTImerSettingTime4HourChange();
    slotTImerSettingTime4MinuteChange();
    slotTImerSettingTime4IntervalChange();

    slotTImerSettingMatchTime1TypeChange();
    slotTImerSettingMatchTime1MonthChange();
    slotTImerSettingMatchTime1DayChange();
    slotTImerSettingMatchTime1WeekChange();
    slotTImerSettingMatchTime1HourChange();
    slotTImerSettingMatchTime1MinuteChange();
    slotTImerSettingMatchTime1ActionChange();

    slotTImerSettingMatchTime2TypeChange();
    slotTImerSettingMatchTime2MonthChange();
    slotTImerSettingMatchTime2DayChange();
    slotTImerSettingMatchTime2WeekChange();
    slotTImerSettingMatchTime2HourChange();
    slotTImerSettingMatchTime2MinuteChange();
    slotTImerSettingMatchTime2ActionChange();

    slotTImerSettingMatchTime3TypeChange();
    slotTImerSettingMatchTime3MonthChange();
    slotTImerSettingMatchTime3DayChange();
    slotTImerSettingMatchTime3WeekChange();
    slotTImerSettingMatchTime3HourChange();
    slotTImerSettingMatchTime3MinuteChange();
    slotTImerSettingMatchTime3ActionChange();

    slotTImerSettingMatchTime4TypeChange();
    slotTImerSettingMatchTime4MonthChange();
    slotTImerSettingMatchTime4DayChange();
    slotTImerSettingMatchTime4WeekChange();
    slotTImerSettingMatchTime4HourChange();
    slotTImerSettingMatchTime4MinuteChange();
    slotTImerSettingMatchTime4ActionChange();
}

void ConfigTreeObject_Timer::slotCreateTimersettingTree(QObject *parent)
{
    //Timer setting 配置树
    QFile file(":/configtree/Image/configTree/Timer/Timer_Setting/config.xml");
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file: Timer_Setting!";
    }else{
        XmlConfigParser::parseConfigXml(file, static_cast<ConfigObject*>(parent));
        file.close();
    }
}

void ConfigTreeObject_Timer::slotTimerDelegateChange()
{
    QFile delegateChange(":/configtree/Image/configTree/Timer/Timer_DelegateChange/delegateChange.xml");
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

void ConfigTreeObject_Timer::slotTImerSettingTime1TypeChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(Config_Timer_Delegate_Timer1_Type)->getData();

    QFile file(XML_Config_Timer_Setting_Timer1_Type);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Timer::slotTImerSettingTime1DayChange()
{
    QString num;

    ConfigObject *pObj = ConfigTreeObject::getConfigObject(Config_Timer_Delegate_Timer1_Day);


    if(pObj){
        num = pObj->getData().toString();

        pObj->setStrShowOn(QString("%1").arg(num.toInt()));
    }
}

void ConfigTreeObject_Timer::slotTImerSettingTime1HourChange()
{
    QString num;

    ConfigObject *pObj = ConfigTreeObject::getConfigObject(Config_Timer_Delegate_Timer1_Hour);


    if(pObj){
        num = pObj->getData().toString();

        pObj->setStrShowOn(QString("%1").arg(num.toInt()));
    }
}

void ConfigTreeObject_Timer::slotTImerSettingTime1MinuteChange()
{
    QString num;

    ConfigObject *pObj = ConfigTreeObject::getConfigObject(Config_Timer_Delegate_Timer1_Minute);


    if(pObj){
        num = pObj->getData().toString();

        pObj->setStrShowOn(QString("%1").arg(num.toInt()));
    }
}

void ConfigTreeObject_Timer::slotTImerSettingTime1IntervalChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(Config_Timer_Delegate_Timer1_Interval)->getData();

    QFile file(XML_Config_Timer_Setting_Timer1_Interval);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Timer::slotTImerSettingTime2TypeChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(Config_Timer_Delegate_Timer2_Type)->getData();

    QFile file(XML_Config_Timer_Setting_Timer2_Type);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Timer::slotTImerSettingTime2DayChange()
{
    QString num;

    ConfigObject *pObj = ConfigTreeObject::getConfigObject(Config_Timer_Delegate_Timer2_Day);


    if(pObj){
        num = pObj->getData().toString();

        pObj->setStrShowOn(QString("%1").arg(num.toInt()));
    }
}

void ConfigTreeObject_Timer::slotTImerSettingTime2HourChange()
{
    QString num;

    ConfigObject *pObj = ConfigTreeObject::getConfigObject(Config_Timer_Delegate_Timer2_Hour);


    if(pObj){
        num = pObj->getData().toString();

        pObj->setStrShowOn(QString("%1").arg(num.toInt()));
    }
}

void ConfigTreeObject_Timer::slotTImerSettingTime2MinuteChange()
{
    QString num;

    ConfigObject *pObj = ConfigTreeObject::getConfigObject(Config_Timer_Delegate_Timer2_Minute);


    if(pObj){
        num = pObj->getData().toString();

        pObj->setStrShowOn(QString("%1").arg(num.toInt()));
    }
}

void ConfigTreeObject_Timer::slotTImerSettingTime2IntervalChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(Config_Timer_Delegate_Timer2_Interval)->getData();

    QFile file(XML_Config_Timer_Setting_Timer2_Interval);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Timer::slotTImerSettingTime3TypeChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(Config_Timer_Delegate_Timer3_Type)->getData();

    QFile file(XML_Config_Timer_Setting_Timer3_Type);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Timer::slotTImerSettingTime3DayChange()
{
    QString num;

    ConfigObject *pObj = ConfigTreeObject::getConfigObject(Config_Timer_Delegate_Timer3_Day);


    if(pObj){
        num = pObj->getData().toString();

        pObj->setStrShowOn(QString("%1").arg(num.toInt()));
    }
}

void ConfigTreeObject_Timer::slotTImerSettingTime3HourChange()
{
    QString num;

    ConfigObject *pObj = ConfigTreeObject::getConfigObject(Config_Timer_Delegate_Timer3_Hour);


    if(pObj){
        num = pObj->getData().toString();

        pObj->setStrShowOn(QString("%1").arg(num.toInt()));
    }
}

void ConfigTreeObject_Timer::slotTImerSettingTime3MinuteChange()
{
    QString num;

    ConfigObject *pObj = ConfigTreeObject::getConfigObject(Config_Timer_Delegate_Timer3_Minute);


    if(pObj){
        num = pObj->getData().toString();

        pObj->setStrShowOn(QString("%1").arg(num.toInt()));
    }
}

void ConfigTreeObject_Timer::slotTImerSettingTime3IntervalChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(Config_Timer_Delegate_Timer3_Interval)->getData();

    QFile file(XML_Config_Timer_Setting_Timer3_Interval);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Timer::slotTImerSettingTime4TypeChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(Config_Timer_Delegate_Timer4_Type)->getData();

    QFile file(XML_Config_Timer_Setting_Timer4_Type);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Timer::slotTImerSettingTime4DayChange()
{
    QString num;

    ConfigObject *pObj = ConfigTreeObject::getConfigObject(Config_Timer_Delegate_Timer4_Day);


    if(pObj){
        num = pObj->getData().toString();

        pObj->setStrShowOn(QString("%1").arg(num.toInt()));
    }
}

void ConfigTreeObject_Timer::slotTImerSettingTime4HourChange()
{
    QString num;

    ConfigObject *pObj = ConfigTreeObject::getConfigObject(Config_Timer_Delegate_Timer4_Hour);


    if(pObj){
        num = pObj->getData().toString();

        pObj->setStrShowOn(QString("%1").arg(num.toInt()));
    }
}

void ConfigTreeObject_Timer::slotTImerSettingTime4MinuteChange()
{
    QString num;

    ConfigObject *pObj = ConfigTreeObject::getConfigObject(Config_Timer_Delegate_Timer4_Minute);


    if(pObj){
        num = pObj->getData().toString();

        pObj->setStrShowOn(QString("%1").arg(num.toInt()));
    }
}

void ConfigTreeObject_Timer::slotTImerSettingTime4IntervalChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(Config_Timer_Delegate_Timer4_Interval)->getData();

    QFile file(XML_Config_Timer_Setting_Timer4_Interval);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Timer::slotTImerSettingMatchTime1TypeChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(Config_Timer_Delegate_Match_Timer1_Type)->getData();

    QFile file(XML_Config_Timer_Setting_Match_Timer1_Type);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Timer::slotTImerSettingMatchTime1MonthChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(Config_Timer_Delegate_Match_Timer1_Month)->getData();

    QFile file(XML_Config_Timer_Setting_Match_Timer1_Month);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Timer::slotTImerSettingMatchTime1DayChange()
{
    QString num;

    ConfigObject *pObj = ConfigTreeObject::getConfigObject(Config_Timer_Delegate_Match_Timer1_Day);


    if(pObj){
        num = pObj->getData().toString();

        pObj->setStrShowOn(QString("%1").arg(num.toInt()));
    }
}

void ConfigTreeObject_Timer::slotTImerSettingMatchTime1WeekChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(Config_Timer_Delegate_Match_Timer1_Week)->getData();

    QFile file(XML_Config_Timer_Setting_Match_Timer1_Week);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Timer::slotTImerSettingMatchTime1HourChange()
{
    QString num;

    ConfigObject *pObj = ConfigTreeObject::getConfigObject(Config_Timer_Delegate_Match_Timer1_Hour);


    if(pObj){
        num = pObj->getData().toString();

        pObj->setStrShowOn(QString("%1").arg(num.toInt()));
    }
}

void ConfigTreeObject_Timer::slotTImerSettingMatchTime1MinuteChange()
{
    QString num;

    ConfigObject *pObj = ConfigTreeObject::getConfigObject(Config_Timer_Delegate_Match_Timer1_Minute);


    if(pObj){
        num = pObj->getData().toString();

        pObj->setStrShowOn(QString("%1").arg(num.toInt()));
    }
}

void ConfigTreeObject_Timer::slotTImerSettingMatchTime1ActionChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(Config_Timer_Delegate_Match_Timer1_Action)->getData();

    QFile file(XML_Config_Timer_Setting_Match_Timer1_Action);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Timer::slotTImerSettingMatchTime2TypeChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(Config_Timer_Delegate_Match_Timer2_Type)->getData();

    QFile file(XML_Config_Timer_Setting_Match_Timer2_Type);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Timer::slotTImerSettingMatchTime2MonthChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(Config_Timer_Delegate_Match_Timer2_Month)->getData();

    QFile file(XML_Config_Timer_Setting_Match_Timer2_Month);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Timer::slotTImerSettingMatchTime2DayChange()
{
    QString num;

    ConfigObject *pObj = ConfigTreeObject::getConfigObject(Config_Timer_Delegate_Match_Timer2_Day);


    if(pObj){
        num = pObj->getData().toString();

        pObj->setStrShowOn(QString("%1").arg(num.toInt()));
    }
}

void ConfigTreeObject_Timer::slotTImerSettingMatchTime2WeekChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(Config_Timer_Delegate_Match_Timer2_Week)->getData();

    QFile file(XML_Config_Timer_Setting_Match_Timer2_Week);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Timer::slotTImerSettingMatchTime2HourChange()
{
    QString num;

    ConfigObject *pObj = ConfigTreeObject::getConfigObject(Config_Timer_Delegate_Match_Timer2_Hour);


    if(pObj){
        num = pObj->getData().toString();

        pObj->setStrShowOn(QString("%1").arg(num.toInt()));
    }
}

void ConfigTreeObject_Timer::slotTImerSettingMatchTime2MinuteChange()
{
    QString num;

    ConfigObject *pObj = ConfigTreeObject::getConfigObject(Config_Timer_Delegate_Match_Timer2_Minute);


    if(pObj){
        num = pObj->getData().toString();

        pObj->setStrShowOn(QString("%1").arg(num.toInt()));
    }
}

void ConfigTreeObject_Timer::slotTImerSettingMatchTime2ActionChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(Config_Timer_Delegate_Match_Timer2_Action)->getData();

    QFile file(XML_Config_Timer_Setting_Match_Timer2_Action);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Timer::slotTImerSettingMatchTime3TypeChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(Config_Timer_Delegate_Match_Timer3_Type)->getData();

    QFile file(XML_Config_Timer_Setting_Match_Timer3_Type);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Timer::slotTImerSettingMatchTime3MonthChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(Config_Timer_Delegate_Match_Timer3_Month)->getData();

    QFile file(XML_Config_Timer_Setting_Match_Timer3_Month);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Timer::slotTImerSettingMatchTime3DayChange()
{
    QString num;

    ConfigObject *pObj = ConfigTreeObject::getConfigObject(Config_Timer_Delegate_Match_Timer3_Day);


    if(pObj){
        num = pObj->getData().toString();

        pObj->setStrShowOn(QString("%1").arg(num.toInt()));
    }
}

void ConfigTreeObject_Timer::slotTImerSettingMatchTime3WeekChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(Config_Timer_Delegate_Match_Timer3_Week)->getData();

    QFile file(XML_Config_Timer_Setting_Match_Timer3_Week);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Timer::slotTImerSettingMatchTime3HourChange()
{
    QString num;

    ConfigObject *pObj = ConfigTreeObject::getConfigObject(Config_Timer_Delegate_Match_Timer3_Hour);


    if(pObj){
        num = pObj->getData().toString();

        pObj->setStrShowOn(QString("%1").arg(num.toInt()));
    }
}

void ConfigTreeObject_Timer::slotTImerSettingMatchTime3MinuteChange()
{
    QString num;

    ConfigObject *pObj = ConfigTreeObject::getConfigObject(Config_Timer_Delegate_Match_Timer3_Minute);


    if(pObj){
        num = pObj->getData().toString();

        pObj->setStrShowOn(QString("%1").arg(num.toInt()));
    }
}

void ConfigTreeObject_Timer::slotTImerSettingMatchTime3ActionChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(Config_Timer_Delegate_Match_Timer3_Action)->getData();

    QFile file(XML_Config_Timer_Setting_Match_Timer3_Action);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Timer::slotTImerSettingMatchTime4TypeChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(Config_Timer_Delegate_Match_Timer4_Type)->getData();

    QFile file(XML_Config_Timer_Setting_Match_Timer4_Type);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Timer::slotTImerSettingMatchTime4MonthChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(Config_Timer_Delegate_Match_Timer4_Month)->getData();

    QFile file(XML_Config_Timer_Setting_Match_Timer4_Month);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Timer::slotTImerSettingMatchTime4DayChange()
{
    QString num;

    ConfigObject *pObj = ConfigTreeObject::getConfigObject(Config_Timer_Delegate_Match_Timer4_Day);


    if(pObj){
        num = pObj->getData().toString();

        pObj->setStrShowOn(QString("%1").arg(num.toInt()));
    }
}

void ConfigTreeObject_Timer::slotTImerSettingMatchTime4WeekChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(Config_Timer_Delegate_Match_Timer4_Week)->getData();

    QFile file(XML_Config_Timer_Setting_Match_Timer4_Week);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}

void ConfigTreeObject_Timer::slotTImerSettingMatchTime4HourChange()
{
    QString num;

    ConfigObject *pObj = ConfigTreeObject::getConfigObject(Config_Timer_Delegate_Match_Timer4_Hour);


    if(pObj){
        num = pObj->getData().toString();

        pObj->setStrShowOn(QString("%1").arg(num.toInt()));
    }
}

void ConfigTreeObject_Timer::slotTImerSettingMatchTime4MinuteChange()
{
    QString num;

    ConfigObject *pObj = ConfigTreeObject::getConfigObject(Config_Timer_Delegate_Match_Timer4_Minute);


    if(pObj){
        num = pObj->getData().toString();

        pObj->setStrShowOn(QString("%1").arg(num.toInt()));
    }
}

void ConfigTreeObject_Timer::slotTImerSettingMatchTime4ActionChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(Config_Timer_Delegate_Match_Timer4_Action)->getData();

    QFile file(XML_Config_Timer_Setting_Match_Timer4_Action);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
     }
}
