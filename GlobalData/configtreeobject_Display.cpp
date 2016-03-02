#include "configtreeobject_Display.h"
#include "cfgchannel.h"
#include "./GlobalData/globalDef.h"

#include "cfgGlobalDef.h"
#include "configtreeobject.h"
#include "./Common/winmenu.h"

ConfigTreeObject_Display *ConfigTreeObject_Display::instance(QObject *parent)
{
    static ConfigTreeObject_Display data(parent);
    return &data;
}

ConfigTreeObject_Display::ConfigTreeObject_Display(QObject *parent):
      ConfigObject(parent)
{
    //Display settings代理树
    QFile fileDelegate(":/configtree/Image/configTree/Display/Disp_SettingDelegate/delegate.xml");
    if(!fileDelegate.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file: Display_settingDelegate!";
    }else{
        XmlConfigParser::parseConfigXml(fileDelegate, static_cast<ConfigObject*>(parent), \
                                                          (QObject*)this);
        fileDelegate.close();
    }
}

void ConfigTreeObject_Display::updateStrShowOn()
{
    updateStrShowTrend();
    updateStrShowGroup();
    updateStrShowMessage();
    updateStrShowBasic();
}

void ConfigTreeObject_Display::updateStrShowTrend()
{
    slotDispTrendIntervalFirstChanged();
    slotDispTrendIntervalSwitchChanged();
    slotDispTrendIntervalSecondChanged();

    slotDispTrendBasicDirectChanged();
    slotDispTrendBasicClearChanged();
    slotDispTrendBasicLineChanged();
    slotDispTrendBasicGridChanged();

    slotDispTrendScaleDigitChanged();
    slotDispTrendScaleValueChanged();
    slotDispTrendScaleMarkChanged();

    slotDispTrendPartSwitchChanged();

    slotDispTrendMsgGroupChanged();
    slotDispTrendMsgPowerChanged();
    slotDispTrendMsgChangeChanged();
}

void ConfigTreeObject_Display::updateStrShowGroup()
{
    slotDispGroupGroupSwitchChanged();
    slotDispGroupGroupNameChanged();
    slotDispGroupGroupSetChanged();

    slotDispGroupScaleSwitchChanged();

    slotDispGroupTrip1SwitchChanged();
    slotDispGroupTrip1PosChanged();
    slotDispGroupTrip1ColorChanged();
    slotDispGroupTrip1WidthChanged();
    slotDispGroupTrip2SwitchChanged();
    slotDispGroupTrip2PosChanged();
    slotDispGroupTrip2ColorChanged();
    slotDispGroupTrip2WidthChanged();
    slotDispGroupTrip3SwitchChanged();
    slotDispGroupTrip3PosChanged();
    slotDispGroupTrip3ColorChanged();
    slotDispGroupTrip3WidthChanged();
    slotDispGroupTrip4SwitchChanged();
    slotDispGroupTrip4PosChanged();
    slotDispGroupTrip4ColorChanged();
    slotDispGroupTrip4WidthChanged();
}

void ConfigTreeObject_Display::updateStrShowMessage()
{
    slotDispMessageNumberChanged();
    updateAllMsgInfoMenu();
}

void ConfigTreeObject_Display::updateStrShowBasic()
{
    slotDispBasicBarDirectChanged();

    slotDispBasicLCDValueChanged();

    slotDispBasicLightModeChanged();
    slotDispBasicLightTimeChanged();
    slotDispBasicLightRestoreChanged();

    slotDispBasicMonitorBackChanged();
    slotDispBasicMonitorScrollChanged();
    slotDispBasicMonitorJumpChanged();

    slotDispBasicCalendar1stChanged();

    slotDispBasicChangeOnoffChanged();
}

void ConfigTreeObject_Display::updateAllMsgInfoMenu()
{
    ConfigObject *pConfigObj;
    QString valueStr;
    for(int i=0;i<100;++i) {
        pConfigObj = ConfigTreeObject::getConfigObject(QString(DISP_DELEGATE_MESSAGE_MESSAGE_INDEX).arg(i+1));
        valueStr = pConfigObj->getData().toString();
        pConfigObj->setStrShowOn(valueStr);
    }
}

void ConfigTreeObject_Display::slotCreateDispsettingTree(QObject *parent)
{
    //Display settings配置树
    QFile file(":/configtree/Image/configTree/Display/Disp_Setting/config.xml");
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file: Display_setting!";
    }else{
        XmlConfigParser::parseConfigXml(file, static_cast<ConfigObject*>(parent));
        file.close();
    }

    //group number初始化--特殊情况，一般的object都在解析时初始化
    quint32 ID_group = ConfigTreeObject::getConfigObject(DISPCONFIG_GRPNUM)->getObjectID();
    ConfigTreeObject::getConfigObject(DISPCONFIG_GRPNUM)->initData(0, 0, ID_group);
    ConfigTreeObject::getConfigObject(DISPCONFIG_GRPNUM)->setStrShowOn("1");

    quint32 ID_msg = ConfigTreeObject::getConfigObject(DISPCONFIG_MSGNUM)->getObjectID();
    ConfigTreeObject::getConfigObject(DISPCONFIG_MSGNUM)->initData(0, 0, ID_msg);
    ConfigTreeObject::getConfigObject(DISPCONFIG_MSGNUM)->setStrShowOn("1 - 10");

}

void ConfigTreeObject_Display::slotDispDelegateChange()
{
    QFile delegateChange(":/configtree/Image/configTree/Display/Disp_DelegateChange/delegateChange.xml");
    if(!delegateChange.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file: Disp_settingDelegateChange!";
    }else{
        XmlConfigParser::parseDelegateChangeXml(delegateChange);
        delegateChange.close();
    }

    _curStatus = false;
    updateStrShowOn();
    _curStatus = true;
}

void ConfigTreeObject_Display::slotDispGroupNumberChanged()
{
    ConfigObject *pObj = ConfigTreeObject::getConfigObject(DISP_DELEGATE_GRPNUM);
    if(pObj) {
        quint32 groupNum = pObj->getData().toUInt();
        pObj->setStrShowOn(QString("%1").arg(groupNum+1));
    }
}

void ConfigTreeObject_Display::slotDispTrendIntervalFirstChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(DISP_DELEGATE_TREND_INTERVAL_INTERVAL)->getData();

    QFile file(XML_DISP_TREND_INTERVAL_INTERVAL);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:DispTrend_TrendFirst!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }

    Trend_Interval backup;
    Record_Mode Recmode;
    quint8 sw;
    ConfigObject* qObj = ConfigTreeObject::getConfigObject(RECORD_DELEGATE_BASIC_MODE_VALUE);
    ConfigObject* rObj = ConfigTreeObject::getConfigObject(DISP_DELEGATE_TREND_INTERVAL_SWITCH);
    if(qObj && rObj) {
        Recmode = (Record_Mode)qObj->getData().toUInt();
        sw = rObj->getData().toUInt();

        if (Record_Mode_Event == Recmode) {
            //Do nothing 事件记录模式
        } else {
            Trend_Interval valueCur = (Trend_Interval)value.toUInt();
            if(0 == sw) {
                backup = valueCur;
            } else {
                Trend_Interval second = (Trend_Interval)ConfigTreeObject::\
                        getConfigObject(DISP_DELEGATE_TREND_INTERVAL_SECOND)->getData().toUInt();
                backup = qMin(valueCur, second);
            }
        }
    }

    switch(backup) {
    case Trend_Interval_5s:
    case Trend_Interval_10s:
    case Trend_Interval_15s:
    case Trend_Interval_30s:
    case Trend_Interval_1min:
    case Trend_Interval_2min:
    case Trend_Interval_5min:
    case Trend_Interval_10min:
    case Trend_Interval_15min:
        ConfigTreeObject::getConfigObject(RECORD_DELEGATE_BASIC_DISP_VALUE)->changeData(Display_Record_DataLength_10min, false);
        break;
    case Trend_Interval_20min:
    case Trend_Interval_30min:
    case Trend_Interval_1h:
        ConfigTreeObject::getConfigObject(RECORD_DELEGATE_BASIC_DISP_VALUE)->changeData(Display_Record_DataLength_1h, false);
        break;
    case Trend_Interval_2h:
        ConfigTreeObject::getConfigObject(RECORD_DELEGATE_BASIC_DISP_VALUE)->changeData(Display_Record_DataLength_2h, false);
        break;
    case Trend_Interval_4h:
        ConfigTreeObject::getConfigObject(RECORD_DELEGATE_BASIC_DISP_VALUE)->changeData(Display_Record_DataLength_4h, false);
        break;
    case Trend_Interval_10h:
        ConfigTreeObject::getConfigObject(RECORD_DELEGATE_BASIC_DISP_VALUE)->changeData(Display_Record_DataLength_8h, false);
        break;
    }
}

void ConfigTreeObject_Display::slotDispTrendIntervalSwitchChanged()
{
    QFile file;
    ConfigObject *pObj = ConfigTreeObject::getConfigObject(RECORD_DELEGATE_BASIC_MODE_VALUE);
    if(pObj) {
        quint32 mode = pObj->getData().toUInt();
        switch (mode) {
        case 0:
            file.setFileName(XML_DISP_TREND_INTERVAL_SWITCH_C);   //display
            break;
        case 1:
            file.setFileName(XML_DISP_TREND_INTERVAL_SWITCH);   //event
            break;
        case 2:
            file.setFileName(XML_DISP_TREND_INTERVAL_SWITCH_C);   //both
            break;
        default:
            return;
        }
    }

    QVariant value = ConfigTreeObject::getConfigObject(DISP_DELEGATE_TREND_INTERVAL_SWITCH)->getData();

    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:DispTrend_TrendSwitch!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Display::slotDispTrendIntervalSecondChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(DISP_DELEGATE_TREND_INTERVAL_SECOND)->getData();

    QFile file(XML_DISP_TREND_INTERVAL_SECOND);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:DispTrend_TrendSecond!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }

    Trend_Interval valueCur = (Trend_Interval)value.toUInt();
    Trend_Interval first = (Trend_Interval)ConfigTreeObject::\
            getConfigObject(DISP_DELEGATE_TREND_INTERVAL_INTERVAL)->getData().toUInt();

    Trend_Interval backup = qMin(first, valueCur);
    switch(backup) {
    case Trend_Interval_5s:
    case Trend_Interval_10s:
    case Trend_Interval_15s:
    case Trend_Interval_30s:
    case Trend_Interval_1min:
    case Trend_Interval_2min:
    case Trend_Interval_5min:
    case Trend_Interval_10min:
    case Trend_Interval_15min:
        ConfigTreeObject::getConfigObject(RECORD_DELEGATE_BASIC_DISP_VALUE)->changeData(Display_Record_DataLength_10min, false);
        break;
    case Trend_Interval_20min:
    case Trend_Interval_30min:
    case Trend_Interval_1h:
        ConfigTreeObject::getConfigObject(RECORD_DELEGATE_BASIC_DISP_VALUE)->changeData(Display_Record_DataLength_1h, false);
        break;
    case Trend_Interval_2h:
        ConfigTreeObject::getConfigObject(RECORD_DELEGATE_BASIC_DISP_VALUE)->changeData(Display_Record_DataLength_2h, false);
        break;
    case Trend_Interval_4h:
        ConfigTreeObject::getConfigObject(RECORD_DELEGATE_BASIC_DISP_VALUE)->changeData(Display_Record_DataLength_4h, false);
        break;
    case Trend_Interval_10h:
        ConfigTreeObject::getConfigObject(RECORD_DELEGATE_BASIC_DISP_VALUE)->changeData(Display_Record_DataLength_8h, false);
        break;
    }
}

void ConfigTreeObject_Display::slotDispTrendBasicDirectChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(DISP_DELEGATE_TREND_BASIC_DIRECTION)->getData();

    QFile file(XML_DISP_TREND_BASIC_DIRECTION);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:DispTrend_BasicDirect!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Display::slotDispTrendBasicClearChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(DISP_DELEGATE_TREND_BASIC_CLEAR)->getData();

    QFile file(XML_DISP_TREND_BASIC_CLEAR);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:DispTrend_BasicClear!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Display::slotDispTrendBasicLineChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(DISP_DELEGATE_TREND_BASIC_LINE)->getData();

    QFile file(XML_DISP_TREND_BASIC_LINE);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:DispTrend_BasicLine!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Display::slotDispTrendBasicGridChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(DISP_DELEGATE_TREND_BASIC_GRID)->getData();

    QFile file(XML_DISP_TREND_BASIC_GRID);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:DispTrend_BasicGrid!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Display::slotDispTrendScaleDigitChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(DISP_DELEGATE_TREND_SCALE_DIGIT)->getData();

    QFile file(XML_DISP_TREND_SCALE_DIGIT);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:DispTrend_ScaleDigit!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Display::slotDispTrendScaleValueChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(DISP_DELEGATE_TREND_SCALE_VALUE)->getData();

    QFile file(XML_DISP_TREND_SCALE_VALUE);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:DispTrend_ScaleValue!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Display::slotDispTrendScaleMarkChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(DISP_DELEGATE_TREND_SCALE_MARK)->getData();

    QFile file(XML_DISP_TREND_SCALE_MARK);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:DispTrend_ScaleMark!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Display::slotDispTrendPartSwitchChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(DISP_DELEGATE_TREND_PARTIAL_SWITCH)->getData();

    QFile file(XML_DISP_TREND_PARTIAL_SWITCH);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:DispTrend_PartialOnoff!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }

    if(value.toUInt() == 0) {
        if (!CfgChannel::instance()->getExistChannelAI().isEmpty()) {
            ConfigTreeObject::getConfigObject(AI_DELEGATE_DISP_PART)->setHideFlag(true);
        }
        if (!CfgChannel::instance()->getExistChannelMath().isEmpty()) {
            ConfigTreeObject::getConfigObject(MATH_DELEGATE_DISP_PART)->setHideFlag(true);
        }
        if (!CfgChannel::instance()->getExistChannelComm().isEmpty()) {
            ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Diaplay_Partial)->setHideFlag(true);
        }
    } else {
        if (!CfgChannel::instance()->getExistChannelAI().isEmpty()) {       //AI
            ConfigTreeObject::getConfigObject(AI_DELEGATE_DISP_PART)->setHideFlag(false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_DISP_PART_ONOFF)->setHideFlag(false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_DISP_PART_ONOFF)->changeData(0, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_DISP_PART_BOUND)->setHideFlag(true);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_DISP_PART_EXPAND)->setHideFlag(true);
        }
        if (!CfgChannel::instance()->getExistChannelMath().isEmpty()) {     //Math
            ConfigTreeObject::getConfigObject(MATH_DELEGATE_DISP_PART)->setHideFlag(false);
            ConfigTreeObject::getConfigObject(MATH_DELEGATE_DISP_PART_ONOFF)->setHideFlag(false);
            ConfigTreeObject::getConfigObject(MATH_DELEGATE_DISP_PART_ONOFF)->changeData(0, false, false);
            ConfigTreeObject::getConfigObject(MATH_DELEGATE_DISP_PART_BOUND)->setHideFlag(true);
            ConfigTreeObject::getConfigObject(MATH_DELEGATE_DISP_PART_EXPAND)->setHideFlag(true);
        }
        if (!CfgChannel::instance()->getExistChannelComm().isEmpty()) {     //Comm
            ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Diaplay_Partial)->setHideFlag(false);
            ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Diaplay_Partial_OnOrOff)->setHideFlag(false);
            ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Diaplay_Partial_OnOrOff)->changeData(0, false, false);
            ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Diaplay_Partial_Boundary)->setHideFlag(true);
            ConfigTreeObject::getConfigObject(COMMChannel_Delegate_Diaplay_Partial_Expand)->setHideFlag(true);
        }
    }
}

void ConfigTreeObject_Display::slotDispTrendMsgGroupChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(DISP_DELEGATE_TREND_MESSAGE_WRITEGRP)->getData();

    QFile file(XML_DISP_TREND_MESSAGE_WRITEGRP);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:DispTrend_MsgWrite!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Display::slotDispTrendMsgPowerChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(DISP_DELEGATE_TREND_MESSAGE_POWERMSG)->getData();

    QFile file(XML_DISP_TREND_MESSAGE_POWERMSG);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:DispTrend_MsgPower!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Display::slotDispTrendMsgChangeChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(DISP_DELEGATE_TREND_MESSAGE_CHANGMSG)->getData();

    QFile file(XML_DISP_TREND_MESSAGE_CHANGMSG);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:DispTrend_MsgChange!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Display::slotDispGroupGroupSwitchChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(DISP_DELEGATE_GROUP_GROUP_SWITCH)->getData();

    QFile file(XML_DISP_GROUP_GROUP_SWITCH);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:DispGrp_GrpSwitch!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Display::slotDispGroupGroupNameChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(DISP_DELEGATE_GROUP_GROUP_GRPNAME)->getData();

    QFile file(XML_DISP_GROUP_GROUP_GRPNAME);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:DispGrp_GrpName!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Display::slotDispGroupGroupSetChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(DISP_DELEGATE_GROUP_GROUP_CHANSET)->getData();

    QFile file(XML_DISP_GROUP_GROUP_CHAN);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:DispGrp_GrpChannel!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Display::slotDispGroupScaleSwitchChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(DISP_DELEGATE_GROUP_SCALEIMAGE_SWITCH)->getData();

    QFile file(XML_DISP_GROUP_SCALEIMAGE_SWITCH);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:DispGrp_ScaleOnoff!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Display::slotDispGroupTrip1SwitchChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(DISP_DELEGATE_GROUP_TRIPLINE1_SWITCH)->getData();

    QFile file(XML_DISP_GROUP_TRIPLINE1_SWITCH);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:DispGrp_Line1Switch!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Display::slotDispGroupTrip1PosChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(DISP_DELEGATE_GROUP_TRIPLINE1_POS)->getData();

    QFile file(XML_DISP_GROUP_TRIPLINE1_POS);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:DispGrp_Line1Pos!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Display::slotDispGroupTrip1ColorChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(DISP_DELEGATE_GROUP_TRIPLINE1_COLOR)->getData();

    QFile file(XML_DISP_GROUP_TRIPLINE1_COLOR);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:DispGrp_Line1Color!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Display::slotDispGroupTrip1WidthChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(DISP_DELEGATE_GROUP_TRIPLINE1_WIDTH)->getData();

    QFile file(XML_DISP_GROUP_TRIPLINE1_WIDTH);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:DispGrp_Line1Width!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Display::slotDispGroupTrip2SwitchChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(DISP_DELEGATE_GROUP_TRIPLINE2_SWITCH)->getData();

    QFile file(XML_DISP_GROUP_TRIPLINE2_SWITCH);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:DispGrp_Line2Switch!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Display::slotDispGroupTrip2PosChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(DISP_DELEGATE_GROUP_TRIPLINE2_POS)->getData();

    QFile file(XML_DISP_GROUP_TRIPLINE2_POS);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:DispGrp_Line2Pos!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Display::slotDispGroupTrip2ColorChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(DISP_DELEGATE_GROUP_TRIPLINE2_COLOR)->getData();

    QFile file(XML_DISP_GROUP_TRIPLINE2_COLOR);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:DispGrp_Line2Color!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Display::slotDispGroupTrip2WidthChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(DISP_DELEGATE_GROUP_TRIPLINE2_WIDTH)->getData();

    QFile file(XML_DISP_GROUP_TRIPLINE2_WIDTH);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:DispGrp_Line2Width!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Display::slotDispGroupTrip3SwitchChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(DISP_DELEGATE_GROUP_TRIPLINE3_SWITCH)->getData();

    QFile file(XML_DISP_GROUP_TRIPLINE3_SWITCH);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:DispGrp_Line3Switch!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Display::slotDispGroupTrip3PosChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(DISP_DELEGATE_GROUP_TRIPLINE3_POS)->getData();

    QFile file(XML_DISP_GROUP_TRIPLINE3_POS);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:DispGrp_Line3Pos!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Display::slotDispGroupTrip3ColorChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(DISP_DELEGATE_GROUP_TRIPLINE3_COLOR)->getData();

    QFile file(XML_DISP_GROUP_TRIPLINE3_COLOR);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:DispGrp_Line3Color!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Display::slotDispGroupTrip3WidthChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(DISP_DELEGATE_GROUP_TRIPLINE3_WIDTH)->getData();

    QFile file(XML_DISP_GROUP_TRIPLINE3_WIDTH);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:DispGrp_Line3Width!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Display::slotDispGroupTrip4SwitchChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(DISP_DELEGATE_GROUP_TRIPLINE4_SWITCH)->getData();

    QFile file(XML_DISP_GROUP_TRIPLINE4_SWITCH);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:DispGrp_Line4Switch!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Display::slotDispGroupTrip4PosChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(DISP_DELEGATE_GROUP_TRIPLINE4_POS)->getData();

    QFile file(XML_DISP_GROUP_TRIPLINE4_POS);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:DispGrp_Line4Pos!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Display::slotDispGroupTrip4ColorChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(DISP_DELEGATE_GROUP_TRIPLINE4_COLOR)->getData();

    QFile file(XML_DISP_GROUP_TRIPLINE4_COLOR);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:DispGrp_Line4Color!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Display::slotDispGroupTrip4WidthChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(DISP_DELEGATE_GROUP_TRIPLINE4_WIDTH)->getData();

    QFile file(XML_DISP_GROUP_TRIPLINE4_WIDTH);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:DispGrp_Line4Width!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Display::slotDispMessageNumberChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(DISP_DELEGATE_MSGNUM)->getData();

    QFile file(XML_DISP_MSGNUM);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:DispMsgNum!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Display::slotDispMsgMsg100Changed()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    QString strName = menu->objectName();
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(QString(DISP_DELEGATE_MESSAGE_MSG_X).arg(strName));
    QString valueStr = pConfigObj->getData().toString();
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_Display::slotDispBasicBarDirectChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(DISP_DELEGATE_SCREEN_BAR_DIRECTION)->getData();

    QFile file(XML_DISP_SCREEN_BAR_DIRECTION);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:DispDisp_BarDirect!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Display::slotDispBasicLCDValueChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(DISP_DELEGATE_SCREEN_LCD_SELECT)->getData();

    QFile file(XML_DISP_SCREEN_LCD_SELECT);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:DispDisp_LCDValue!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Display::slotDispBasicLightModeChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(DISP_DELEGATE_SCREEN_BACKLIGHT_MODE)->getData();

    QFile file(XML_DISP_SCREEN_BACKLIGHT_MODE);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:DispDisp_LightMode!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Display::slotDispBasicLightTimeChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(DISP_DELEGATE_SCREEN_BACKLIGHT_TIME)->getData();

    QFile file(XML_DISP_SCREEN_BACKLIGHT_TIME);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:DispDisp_LightTime!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Display::slotDispBasicLightRestoreChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(DISP_DELEGATE_SCREEN_BACKLIGHT_RESTORE)->getData();

    QFile file(XML_DISP_SCREEN_BACKLIGHT_RESTORE);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:DispDisp_LightRestore!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Display::slotDispBasicMonitorBackChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(DISP_DELEGATE_SCREEN_MONITOR_BACK)->getData();

    QFile file(XML_DISP_SCREEN_MONITOR_BACK);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:DispDisp_MonitorBack!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Display::slotDispBasicMonitorScrollChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(DISP_DELEGATE_SCREEN_MONITOR_SCROLL)->getData();

    QFile file(XML_DISP_SCREEN_MONITOR_SCROLL);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:DispDisp_MonitorScroll!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Display::slotDispBasicMonitorJumpChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(DISP_DELEGATE_SCREEN_MONITOR_JUMP)->getData();

    QFile file(XML_DISP_SCREEN_MONITOR_JUMP);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:DispDisp_MonitorJump!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Display::slotDispBasicCalendar1stChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(DISP_DELEGATE_SCREEN_CALENDAR_ITEM)->getData();

    QFile file(XML_DISP_SCREEN_CALENDAR_ITEM);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:DispDisp_Calendar1st!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Display::slotDispBasicChangeOnoffChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(DISP_DELEGATE_SCREEN_CHANGEVALUE_ONOFF)->getData();

    QFile file(XML_DISP_SCREEN_CHANGEVALUE_ONOFF);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:DispDisp_ChangeSwitch!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}
