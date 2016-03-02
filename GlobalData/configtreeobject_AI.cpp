#include "configtreeobject_AI.h"
#include "cfgchannel.h"
#include "./GlobalData/globalDef.h"

#include "cfgGlobalDef.h"
#include "configtreeobject.h"

ConfigTreeObject_AI *ConfigTreeObject_AI::instance(QObject *parent)
{
    static ConfigTreeObject_AI data(parent);
    return &data;
}

ConfigTreeObject_AI::ConfigTreeObject_AI(QObject *parent):
      ConfigObject(parent)
{
    //AI channel settings代理树
    QFile fileDelegate(":/configtree/Image/configTree/AI_SettingDelegate/delegate.xml");
    if(!fileDelegate.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file: AI_settingDelegate!";
    }else{
        XmlConfigParser::parseConfigXml(fileDelegate, static_cast<ConfigObject*>(parent), \
                                                          (QObject*)this);
        fileDelegate.close();
    }
}

/*
 * 功能：
 * 	  AI配置窗体当前界面刷新
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
void ConfigTreeObject_AI::updateStrShowOn()
{
    if (CfgChannel::instance()->getExistChannelAI().isEmpty())
        return;
    else
        ConfigTreeObject::getConfigObject(AI_DELEGATE)->setHideFlag(false);

    updateStrShowRange();
    updateStrShowAlarm();
    updateStrShowDisplay();
    updateStrShowCalib();
}

void ConfigTreeObject_AI::updateStrShowRange()
{
    slotAIRange_RangeRangeChanged();
    slotAIRange_RangeUpperChanged();
    slotAIRange_RangeLowerChanged();
    slotAIRange_RangeRefChanChanged();

    slotAIRange_ScaleDecimalChanged();
    slotAIRange_ScaleLowerChanged();
    slotAIRange_ScaleUpperChanged();
    slotAIRange_ScaleUnitChanged();

    slotAIRange_RangeCalculationChanged();

    slotAIRange_LowcutOnoffChanged();
    slotAIRange_LowcutValueChanged();
    slotAIRange_LowcutOutputChanged();

    slotAIRange_MovingOnoffChanged();
    slotAIRange_MovingCountChanged();

    slotAIRange_RJCModeChanged();
    slotAIRange_RJCTemperChanged();

    slotAIRange_BurnoutModeChanged();

    slotAIRange_BiasValueChanged();

    slotAIRange_RangeTypeChanged();
}

void ConfigTreeObject_AI::updateStrShowAlarm()
{
    slotAIAlarm_Level1TypeChanged();
    slotAIAlarm_Level1ValueChanged();
    slotAIAlarm_Level1HysterChanged();
    slotAIAlarm_Level1LoggChanged();
    slotAIAlarm_Level1OutTypeChanged();
    slotAIAlarm_Level1OutNoChanged();
    slotAIAlarm_Level1OnoffChanged();

    slotAIAlarm_Level2TypeChanged();
    slotAIAlarm_Level2ValueChanged();
    slotAIAlarm_Level2HysterChanged();
    slotAIAlarm_Level2LoggChanged();
    slotAIAlarm_Level2OutTypeChanged();
    slotAIAlarm_Level2OutNoChanged();
    slotAIAlarm_Level2OnoffChanged();

    slotAIAlarm_Level3TypeChanged();
    slotAIAlarm_Level3ValueChanged();
    slotAIAlarm_Level3HysterChanged();
    slotAIAlarm_Level3LoggChanged();
    slotAIAlarm_Level3OutTypeChanged();
    slotAIAlarm_Level3OutNoChanged();
    slotAIAlarm_Level3OnoffChanged();

    slotAIAlarm_Level4TypeChanged();
    slotAIAlarm_Level4ValueChanged();
    slotAIAlarm_Level4HysterChanged();
    slotAIAlarm_Level4LoggChanged();
    slotAIAlarm_Level4OutTypeChanged();
    slotAIAlarm_Level4OutNoChanged();
    slotAIAlarm_Level4OnoffChanged();

    slotAIAlarm_DelayHourChanged();
    slotAIAlarm_DelayMinChanged();
    slotAIAlarm_DelaySecChanged();
}

void ConfigTreeObject_AI::updateStrShowDisplay()
{
    slotAIDisp_TagCharChanged();
    slotAIDisp_TagNoChanged();

    slotAIDisp_ColorColorChanged();

    slotAIDisp_ZoneLowerChanged();
    slotAIDisp_ZoneUpperChanged();

    slotAIDisp_ScalePosChanged();
    slotAIDisp_ScaleDivChanged();

    slotAIDisp_BarBasePosChanged();
    slotAIDisp_BarDivChanged();

    slotAIDisp_PartOnoffChanged();
    slotAIDisp_PartExpandChanged();
    slotAIDisp_PartBoundChanged();

    slotAIDisp_CsbandBandareaChanged();
    slotAIDisp_CsbandDispLowChanged();
    slotAIDisp_CsbandDispUpChanged();
    slotAIDisp_CsbandColorChanged();

    slotAIDisp_AlarmIndicateChanged();
    slotAIDisp_AlarmMarkTypeChanged();
    slotAIDisp_AlarmAlarm1Changed();
    slotAIDisp_AlarmAlarm2Changed();
    slotAIDisp_AlarmAlarm3Changed();
    slotAIDisp_AlarmAlarm4Changed();

    slotAIDisp_DisVal_0_Changed();
    slotAIDisp_DisVal_1_Changed();
}

void ConfigTreeObject_AI::updateStrShowCalib()
{
    slotAICali_OneLineInChanged();
    slotAICali_OneLineOutChanged();

    slotAICali_TwoLineInChanged();
    slotAICali_TwoLineOutChanged();

    slotAICali_ThreeLineInChanged();
    slotAICali_ThreeLineOutChanged();

    slotAICali_FourLineInChanged();
    slotAICali_FourLineOutChanged();

    slotAICali_FiveLineInChanged();
    slotAICali_FiveLineOutChanged();

    slotAICali_SixLineInChanged();
    slotAICali_SixLineOutChanged();

    slotAICali_SevenLineInChanged();
    slotAICali_SevenLineOutChanged();

    slotAICali_EightLineInChanged();
    slotAICali_EightLineOutChanged();

    slotAICali_NineLineInChanged();
    slotAICali_NineLineOutChanged();

    slotAICali_TenLineInChanged();
    slotAICali_TenLineOutChanged();

    slotAICali_ElevenLineInChanged();
    slotAICali_ElevenLineOutChanged();

    slotAICali_TwelveLineInChanged();
    slotAICali_TwelveLineOutChanged();

    slotAICali_ModeNumSetChanged();
    slotAICali_ModeModeChanged();
}

void ConfigTreeObject_AI::setAICaliCorrect_Value(quint32 correctCount)
{
    float Upper, Lower, value, differ;

    AI_Range_Calculate calcu = (AI_Range_Calculate)ConfigTreeObject::getConfigData(AI_DELEGATE_RANGE_RANGE_CALCU).toUInt();
    if((AI_Range_Calculate_Linear == calcu) || (AI_Range_Calculate_Sqrt == calcu)) {
        Lower = -999999;
        Upper = 999999;
    } else {
        QString showOnMin = Cfgconfig_AI::instance()->getAIValueObjStr(ID_AI_Range_Range_Min, \
                                        0, ConfigTreeObject::getConfigObject(AI_DELEGATE_RANGE_RANGE_MIN));
        QString showOnMax = Cfgconfig_AI::instance()->getAIValueObjStr(ID_AI_Range_Range_Max, \
                                        0, ConfigTreeObject::getConfigObject(AI_DELEGATE_RANGE_RANGE_MAX));

        QStringList strListMin = showOnMin.split(" ");
        QStringList strListMax = showOnMax.split(" ");
        QString strValueMin = strListMin[0].remove(".");
        QString strValueMax = strListMax[0].remove(".");

        Lower = strValueMin.toFloat();
        Upper = strValueMax.toFloat();
    }

    differ = fabs(Upper - Lower);

    for(quint32 i=0;i<12;++i) {
        ConfigTreeObject::getConfigObject(QString(AI_DELEGATE_CALI_GROUP).arg(i+1))->setHideFlag(true);
    }

    quint32 correctMode = ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_MODE_MODE)->getData().toUInt();
    for(quint32 i=0;i<correctCount;++i) {
        ConfigTreeObject::getConfigObject(QString(AI_DELEGATE_CALI_GROUP).arg(i+1))->setHideFlag(false);
    }

    if(0 == correctMode) {

    } else if (1 == correctMode) {
        ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_1_INPUT)->changeData(Lower, false, false);
        ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_1_OUTPUT)->changeData(Lower, false, false);
        switch(correctCount) {
        case 2:
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_2_INPUT)->changeData(Upper, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_2_OUTPUT)->changeData(Upper, false, false);
            break;
        case 3:
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_2_INPUT)->changeData(0, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_2_OUTPUT)->changeData(0, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_3_INPUT)->changeData(Upper, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_3_OUTPUT)->changeData(Upper, false, false);
            break;
        case 4:
            value = differ/3.0;
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_2_INPUT)->changeData(Lower+value*1, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_2_OUTPUT)->changeData(Lower+value*1, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_3_INPUT)->changeData(Lower+value*2, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_3_OUTPUT)->changeData(Lower+value*2, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_4_INPUT)->changeData(Upper, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_4_OUTPUT)->changeData(Upper, false, false);
            break;
        case 5:
            value = differ/4.0;
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_2_INPUT)->changeData(Lower+value*1, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_2_OUTPUT)->changeData(Lower+value*1, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_3_INPUT)->changeData(0, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_3_OUTPUT)->changeData(0, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_4_INPUT)->changeData(Lower+value*3, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_4_OUTPUT)->changeData(Lower+value*3, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_5_INPUT)->changeData(Upper, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_5_OUTPUT)->changeData(Upper, false, false);
            break;
        case 6:
            value = differ/5.0;
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_2_INPUT)->changeData(Lower+value*1, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_2_OUTPUT)->changeData(Lower+value*1, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_3_INPUT)->changeData(Lower+value*2, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_3_OUTPUT)->changeData(Lower+value*2, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_4_INPUT)->changeData(Lower+value*3, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_4_OUTPUT)->changeData(Lower+value*3, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_5_INPUT)->changeData(Lower+value*4, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_5_OUTPUT)->changeData(Lower+value*4, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_6_INPUT)->changeData(Upper, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_6_OUTPUT)->changeData(Upper, false, false);
            break;
        case 7:
            value = differ/6.0;
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_2_INPUT)->changeData(Lower+value*1, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_2_OUTPUT)->changeData(Lower+value*1, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_3_INPUT)->changeData(Lower+value*2, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_3_OUTPUT)->changeData(Lower+value*2, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_4_INPUT)->changeData(0, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_4_OUTPUT)->changeData(0, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_5_INPUT)->changeData(Lower+value*4, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_5_OUTPUT)->changeData(Lower+value*4, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_6_INPUT)->changeData(Lower+value*5, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_6_OUTPUT)->changeData(Lower+value*5, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_7_INPUT)->changeData(Upper, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_7_OUTPUT)->changeData(Upper, false, false);
            break;
        case 8:
            value = differ/7.0;
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_2_INPUT)->changeData(Lower+value*1, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_2_OUTPUT)->changeData(Lower+value*1, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_3_INPUT)->changeData(Lower+value*2, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_3_OUTPUT)->changeData(Lower+value*2, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_4_INPUT)->changeData(Lower+value*3, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_4_OUTPUT)->changeData(Lower+value*3, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_5_INPUT)->changeData(Lower+value*4, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_5_OUTPUT)->changeData(Lower+value*4, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_6_INPUT)->changeData(Lower+value*5, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_6_OUTPUT)->changeData(Lower+value*5, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_7_INPUT)->changeData(Lower+value*6, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_7_OUTPUT)->changeData(Lower+value*6, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_8_INPUT)->changeData(Upper, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_8_OUTPUT)->changeData(Upper, false, false);
            break;
        case 9:
            value = differ/8.0;
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_2_INPUT)->changeData(Lower+value*1, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_2_OUTPUT)->changeData(Lower+value*1, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_3_INPUT)->changeData(Lower+value*2, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_3_OUTPUT)->changeData(Lower+value*2, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_4_INPUT)->changeData(Lower+value*3, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_4_OUTPUT)->changeData(Lower+value*3, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_5_INPUT)->changeData(0, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_5_OUTPUT)->changeData(0, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_6_INPUT)->changeData(Lower+value*5, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_6_OUTPUT)->changeData(Lower+value*5, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_7_INPUT)->changeData(Lower+value*6, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_7_OUTPUT)->changeData(Lower+value*6, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_8_INPUT)->changeData(Lower+value*7, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_8_OUTPUT)->changeData(Lower+value*7, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_9_INPUT)->changeData(Upper, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_9_OUTPUT)->changeData(Upper, false, false);
            break;
        case 10:
            value = differ/9.0;
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_2_INPUT)->changeData(Lower+value*1, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_2_OUTPUT)->changeData(Lower+value*1, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_3_INPUT)->changeData(Lower+value*2, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_3_OUTPUT)->changeData(Lower+value*2, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_4_INPUT)->changeData(Lower+value*3, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_4_OUTPUT)->changeData(Lower+value*3, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_5_INPUT)->changeData(Lower+value*4, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_5_OUTPUT)->changeData(Lower+value*4, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_6_INPUT)->changeData(Lower+value*5, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_6_OUTPUT)->changeData(Lower+value*5, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_7_INPUT)->changeData(Lower+value*6, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_7_OUTPUT)->changeData(Lower+value*6, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_8_INPUT)->changeData(Lower+value*7, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_8_OUTPUT)->changeData(Lower+value*7, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_9_INPUT)->changeData(Lower+value*8, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_9_OUTPUT)->changeData(Lower+value*8, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_10_INPUT)->changeData(Upper, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_10_OUTPUT)->changeData(Upper, false, false);
            break;
        case 11:
            value = differ/10.0;
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_2_INPUT)->changeData(Lower+value*1, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_2_OUTPUT)->changeData(Lower+value*1, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_3_INPUT)->changeData(Lower+value*2, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_3_OUTPUT)->changeData(Lower+value*2, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_4_INPUT)->changeData(Lower+value*3, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_4_OUTPUT)->changeData(Lower+value*3, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_5_INPUT)->changeData(Lower+value*4, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_5_OUTPUT)->changeData(Lower+value*4, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_6_INPUT)->changeData(0, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_6_OUTPUT)->changeData(0, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_7_INPUT)->changeData(Lower+value*6, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_7_OUTPUT)->changeData(Lower+value*6, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_8_INPUT)->changeData(Lower+value*7, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_8_OUTPUT)->changeData(Lower+value*7, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_9_INPUT)->changeData(Lower+value*8, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_9_OUTPUT)->changeData(Lower+value*8, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_10_INPUT)->changeData(Lower+value*9, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_10_OUTPUT)->changeData(Lower+value*9, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_11_INPUT)->changeData(Upper, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_11_OUTPUT)->changeData(Upper, false, false);
            break;
        case 12:
            value = differ/11.0;
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_2_INPUT)->changeData(Lower+value*1, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_2_OUTPUT)->changeData(Lower+value*1, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_3_INPUT)->changeData(Lower+value*2, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_3_OUTPUT)->changeData(Lower+value*2, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_4_INPUT)->changeData(Lower+value*3, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_4_OUTPUT)->changeData(Lower+value*3, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_5_INPUT)->changeData(Lower+value*4, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_5_OUTPUT)->changeData(Lower+value*4, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_6_INPUT)->changeData(Lower+value*5, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_6_OUTPUT)->changeData(Lower+value*5, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_7_INPUT)->changeData(Lower+value*6, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_7_OUTPUT)->changeData(Lower+value*6, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_8_INPUT)->changeData(Lower+value*7, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_8_OUTPUT)->changeData(Lower+value*7, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_9_INPUT)->changeData(Lower+value*8, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_9_OUTPUT)->changeData(Lower+value*8, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_10_INPUT)->changeData(Lower+value*9, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_10_OUTPUT)->changeData(Lower+value*9, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_11_INPUT)->changeData(Lower+value*10, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_11_OUTPUT)->changeData(Lower+value*10, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_12_INPUT)->changeData(Upper, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_12_OUTPUT)->changeData(Upper, false, false);
            break;
        }
    } else if (2 == correctMode) {
        ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_1_INPUT)->changeData(Lower, false, false);
        ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_1_OUTPUT)->changeData(0, false, false);
        switch(correctCount) {
        case 2:
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_2_INPUT)->changeData(Upper, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_2_OUTPUT)->changeData(0, false, false);
            break;
        case 3:
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_2_INPUT)->changeData(0, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_2_OUTPUT)->changeData(0, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_3_INPUT)->changeData(Upper, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_3_OUTPUT)->changeData(0, false, false);
            break;
        case 4:
            value = differ/3.0;
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_2_INPUT)->changeData(Lower+value*1, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_2_OUTPUT)->changeData(0, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_3_INPUT)->changeData(Lower+value*2, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_3_OUTPUT)->changeData(0, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_4_INPUT)->changeData(Upper, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_4_OUTPUT)->changeData(0, false, false);
            break;
        case 5:
            value = differ/4.0;
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_2_INPUT)->changeData(Lower+value*1, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_2_OUTPUT)->changeData(0, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_3_INPUT)->changeData(0, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_3_OUTPUT)->changeData(0, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_4_INPUT)->changeData(Lower+value*3, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_4_OUTPUT)->changeData(0, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_5_INPUT)->changeData(Upper, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_5_OUTPUT)->changeData(0, false, false);
            break;
        case 6:
            value = differ/5.0;
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_2_INPUT)->changeData(Lower+value*1, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_2_OUTPUT)->changeData(0, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_3_INPUT)->changeData(Lower+value*2, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_3_OUTPUT)->changeData(0, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_4_INPUT)->changeData(Lower+value*3, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_4_OUTPUT)->changeData(0, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_5_INPUT)->changeData(Lower+value*4, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_5_OUTPUT)->changeData(0, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_6_INPUT)->changeData(Upper, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_6_OUTPUT)->changeData(0, false, false);
            break;
        case 7:
            value = differ/6.0;
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_2_INPUT)->changeData(Lower+value*1, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_2_OUTPUT)->changeData(0, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_3_INPUT)->changeData(Lower+value*2, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_3_OUTPUT)->changeData(0, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_4_INPUT)->changeData(0, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_4_OUTPUT)->changeData(0, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_5_INPUT)->changeData(Lower+value*4, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_5_OUTPUT)->changeData(0, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_6_INPUT)->changeData(Lower+value*5, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_6_OUTPUT)->changeData(0, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_7_INPUT)->changeData(Upper, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_7_OUTPUT)->changeData(0, false, false);
            break;
        case 8:
            value = differ/7.0;
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_2_INPUT)->changeData(Lower+value*1, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_2_OUTPUT)->changeData(0, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_3_INPUT)->changeData(Lower+value*2, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_3_OUTPUT)->changeData(0, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_4_INPUT)->changeData(Lower+value*3, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_4_OUTPUT)->changeData(0, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_5_INPUT)->changeData(Lower+value*4, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_5_OUTPUT)->changeData(0, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_6_INPUT)->changeData(Lower+value*5, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_6_OUTPUT)->changeData(0, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_7_INPUT)->changeData(Lower+value*6, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_7_OUTPUT)->changeData(0, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_8_INPUT)->changeData(Upper, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_8_OUTPUT)->changeData(0, false, false);
            break;
        case 9:
            value = differ/8.0;
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_2_INPUT)->changeData(Lower+value*1, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_2_OUTPUT)->changeData(0, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_3_INPUT)->changeData(Lower+value*2, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_3_OUTPUT)->changeData(0, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_4_INPUT)->changeData(Lower+value*3, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_4_OUTPUT)->changeData(0, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_5_INPUT)->changeData(0, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_5_OUTPUT)->changeData(0, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_6_INPUT)->changeData(Lower+value*5, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_6_OUTPUT)->changeData(0, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_7_INPUT)->changeData(Lower+value*6, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_7_OUTPUT)->changeData(0, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_8_INPUT)->changeData(Lower+value*7, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_8_OUTPUT)->changeData(0, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_9_INPUT)->changeData(Upper, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_9_OUTPUT)->changeData(0, false, false);
            break;
        case 10:
            value = differ/9.0;
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_2_INPUT)->changeData(Lower+value*1, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_2_OUTPUT)->changeData(0, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_3_INPUT)->changeData(Lower+value*2, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_3_OUTPUT)->changeData(0, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_4_INPUT)->changeData(Lower+value*3, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_4_OUTPUT)->changeData(0, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_5_INPUT)->changeData(Lower+value*4, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_5_OUTPUT)->changeData(0, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_6_INPUT)->changeData(Lower+value*5, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_6_OUTPUT)->changeData(0, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_7_INPUT)->changeData(Lower+value*6, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_7_OUTPUT)->changeData(0, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_8_INPUT)->changeData(Lower+value*7, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_8_OUTPUT)->changeData(0, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_9_INPUT)->changeData(Lower+value*8, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_9_OUTPUT)->changeData(0, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_10_INPUT)->changeData(Upper, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_10_OUTPUT)->changeData(0, false, false);
            break;
        case 11:
            value = differ/10.0;
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_2_INPUT)->changeData(Lower+value*1, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_2_OUTPUT)->changeData(0, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_3_INPUT)->changeData(Lower+value*2, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_3_OUTPUT)->changeData(0, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_4_INPUT)->changeData(Lower+value*3, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_4_OUTPUT)->changeData(0, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_5_INPUT)->changeData(Lower+value*4, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_5_OUTPUT)->changeData(0, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_6_INPUT)->changeData(0, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_6_OUTPUT)->changeData(0, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_7_INPUT)->changeData(Lower+value*6, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_7_OUTPUT)->changeData(0, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_8_INPUT)->changeData(Lower+value*7, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_8_OUTPUT)->changeData(0, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_9_INPUT)->changeData(Lower+value*8, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_9_OUTPUT)->changeData(0, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_10_INPUT)->changeData(Lower+value*9, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_10_OUTPUT)->changeData(0, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_11_INPUT)->changeData(Upper, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_11_OUTPUT)->changeData(0, false, false);
            break;
        case 12:
            value = differ/11.0;
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_2_INPUT)->changeData(Lower+value*1, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_2_OUTPUT)->changeData(0, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_3_INPUT)->changeData(Lower+value*2, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_3_OUTPUT)->changeData(0, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_4_INPUT)->changeData(Lower+value*3, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_4_OUTPUT)->changeData(0, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_5_INPUT)->changeData(Lower+value*4, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_5_OUTPUT)->changeData(0, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_6_INPUT)->changeData(Lower+value*5, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_6_OUTPUT)->changeData(0, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_7_INPUT)->changeData(Lower+value*6, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_7_OUTPUT)->changeData(0, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_8_INPUT)->changeData(Lower+value*7, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_8_OUTPUT)->changeData(0, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_9_INPUT)->changeData(Lower+value*8, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_9_OUTPUT)->changeData(0, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_10_INPUT)->changeData(Lower+value*9, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_10_OUTPUT)->changeData(0, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_11_INPUT)->changeData(Lower+value*10, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_11_OUTPUT)->changeData(0, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_12_INPUT)->changeData(Upper, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_12_OUTPUT)->changeData(0, false, false);
            break;
        }
    }
}

void ConfigTreeObject_AI::resetPartialSettings()
{
    ConfigObject* pObj = ConfigTreeObject::getConfigObject(DISP_DELEGATE_TREND_PARTIAL_SWITCH);
    if(pObj) {
        QVariant valueSwitch = pObj->getData();
        if(valueSwitch.toUInt() == 1) {
            ConfigTreeObject::getConfigObject(AI_DELEGATE_DISP_PART)->setHideFlag(false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_DISP_PART_ONOFF)->setHideFlag(false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_DISP_PART_ONOFF)->changeData(0, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_DISP_PART_BOUND)->setHideFlag(true);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_DISP_PART_EXPAND)->setHideFlag(true);
        }
    }
}

void ConfigTreeObject_AI::slotCreateAIsettingTree(QObject *parent)
{
    if (CfgChannel::instance()->getExistChannelAI().isEmpty())
        return;

    //AI channel settings配置树
    QFile file(":/configtree/Image/configTree/AI_Setting/config.xml");
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file: AI_setting!";
    }else{
        XmlConfigParser::parseConfigXml(file, static_cast<ConfigObject*>(parent));
        file.close();
    }

    //开始通道、结束通道初始化--特殊情况，一般的object都在解析时初始化
    QString keyFirst = CfgChannel::instance()->getExistChannelAI().firstKey();
    quint32 valueFirst = CfgChannel::instance()->getExistChannelAI().value(keyFirst);

    quint32 ID_first = ConfigTreeObject::getConfigObject(AICONFIG_FIRSTCHAN)->getObjectID();
    quint32 ID_last = ConfigTreeObject::getConfigObject(AICONFIG_LASTCHAN)->getObjectID();

    ConfigTreeObject::getConfigObject(AICONFIG_FIRSTCHAN)->initData(keyFirst, valueFirst, ID_first);
    ConfigTreeObject::getConfigObject(AICONFIG_LASTCHAN)->initData(keyFirst, valueFirst, ID_last);

    ConfigTreeObject::getConfigObject(AICONFIG_FIRSTCHAN)->setStrShowOn(keyFirst);
    ConfigTreeObject::getConfigObject(AICONFIG_LASTCHAN)->setStrShowOn(keyFirst);

}

void ConfigTreeObject_AI::slotAIBoardInoutCfgTree(quint8 unit, quint8 module, quint8 chNum, bool in)
{
    quint32 chanCoder;
    ConfigObject *pConfigParentObj;
    ConfigObject *pConfigObj;
    QFile file;

    if(in) {    //板卡插入
        for(int i=1; i<=chNum; ++i) {
            chanCoder = CHANNEL_AI_CODER(unit, module, i);
            int index = CfgChannel::instance()->getExistChannelAI().values().indexOf(chanCoder);
            QString chanNum = CfgChannel::instance()->getExistChannelAI().keys().at(index);

            pConfigParentObj = ConfigTreeObject::getConfigObject(AICONFIG_RANG);
            pConfigObj = new ConfigObject(pConfigParentObj);
            pConfigObj->setObjectName(chanNum);
            file.setFileName(":/configtree/Image/configTree/AI_Setting/config-Range.xml");
            if(!file.open(QFile::ReadOnly | QFile::Text)){
                qWarning()<< "Can't open the file: config-Range.xml";
            }else{
                XmlConfigParser::parseConfigXml(file, pConfigObj, 0, chanCoder);
                file.close();
            }

            pConfigParentObj = ConfigTreeObject::getConfigObject(AICONFIG_ALARM);
            pConfigObj = new ConfigObject(pConfigParentObj);
            pConfigObj->setObjectName(chanNum);
            file.setFileName(":/configtree/Image/configTree/AI_Setting/config-Alarm.xml");
            if(!file.open(QFile::ReadOnly | QFile::Text)){
                qWarning()<< "Can't open the file: config-Alarm.xml";
            }else{
                XmlConfigParser::parseConfigXml(file, pConfigObj, 0, chanCoder);
                file.close();
            }

            pConfigParentObj = ConfigTreeObject::getConfigObject(AICONFIG_DISP);
            pConfigObj = new ConfigObject(pConfigParentObj);
            pConfigObj->setObjectName(chanNum);
            file.setFileName(":/configtree/Image/configTree/AI_Setting/config-Display settings.xml");
            if(!file.open(QFile::ReadOnly | QFile::Text)){
                qWarning()<< "Can't open the file: config-Display settings.xml";
            }else{
                XmlConfigParser::parseConfigXml(file, pConfigObj, 0, chanCoder);
                file.close();
            }

            pConfigParentObj = ConfigTreeObject::getConfigObject(AICONFIG_CALI);
            pConfigObj = new ConfigObject(pConfigParentObj);
            pConfigObj->setObjectName(chanNum);
            file.setFileName(":/configtree/Image/configTree/AI_Setting/config-Calibration correction.xml");
            if(!file.open(QFile::ReadOnly | QFile::Text)){
                qWarning()<< "Can't open the file: config-Calibration correction.xml";
            }else{
                XmlConfigParser::parseConfigXml(file, pConfigObj, 0, chanCoder);
                file.close();
            }
        }
    } else {    //板卡拔出
        for(int i=1; i<=chNum; ++i) {
            chanCoder = CHANNEL_AI_CODER(unit, module, i);
            int index = CfgChannel::instance()->getExistChannelAI().values().indexOf(chanCoder);
            QString chanNum = CfgChannel::instance()->getExistChannelAI().keys().at(index);

            QString delPath;
            delPath = QString(AICONFIG_RANG) + ":" + QString(chanNum);
            ConfigTreeObject::destroyConfigTree(delPath);
            delPath = QString(AICONFIG_ALARM) + ":" + QString(chanNum);
            ConfigTreeObject::destroyConfigTree(delPath);
            delPath = QString(AICONFIG_DISP) + ":" + QString(chanNum);
            ConfigTreeObject::destroyConfigTree(delPath);
            delPath = QString(AICONFIG_CALI) + ":" + QString(chanNum);
            ConfigTreeObject::destroyConfigTree(delPath);
        }
    }
}

/*
 * 功能：
 * 	  AI配置窗体当前通道选择更改处理：修改所有代理配置对象，使它们
 *    根据当前选择的通道号指向相应的配置对象，同时刷新当前界面
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
void ConfigTreeObject_AI::slotAIDelegateChange()
{
    if (CfgChannel::instance()->getExistChannelAI().isEmpty())
        return;

    QString fistChan = ConfigTreeObject::getConfigObject(AICONFIG_FIRSTCHAN)->getData().toString();
    QString lastChan = ConfigTreeObject::getConfigObject(AICONFIG_LASTCHAN)->getData().toString();
    quint32 firstCoder = CfgChannel::instance()->getExistChannelAI().value(fistChan);
    quint32 lastCoder = CfgChannel::instance()->getExistChannelAI().value(lastChan);
    QFile delegateChange(":/configtree/Image/configTree/AI_DelegateChange/delegateChange.xml");
     if(!delegateChange.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file: AI_settingDelegateChange!";
     }else{
        XmlConfigParser::parseDelegateChangeXml(delegateChange, firstCoder, lastCoder);
        delegateChange.close();
     }

     _curStatus = false;
     updateStrShowOn();
     _curStatus = true;
}

/*
 * 功能：
 * 	  AI起始通道变更槽函数，其他配置项目类似。
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
void ConfigTreeObject_AI::slotAIFirst_chanSelectchanged()
{
    QString chanNumF;
    QString chanNumL;
    ConfigObject *pObj = ConfigTreeObject::getConfigObject(AI_DELEGATE_FIRSTCHAN);
    ConfigObject *qObj = ConfigTreeObject::getConfigObject(AI_DELEGATE_LASTCHAN);

    if(pObj && qObj){
        chanNumF = pObj->getData().toString();
        chanNumL = qObj->getData().toString();

        pObj->setStrShowOn(chanNumF);
        qObj->changeData(chanNumF, false);
    }
}

void ConfigTreeObject_AI::slotAILast_chanSelectchanged()
{
    QString chanNumF;
    QString chanNumL;
    ConfigObject *pObj = ConfigTreeObject::getConfigObject(AI_DELEGATE_FIRSTCHAN);
    ConfigObject *qObj = ConfigTreeObject::getConfigObject(AI_DELEGATE_LASTCHAN);

    if(pObj && qObj){
        chanNumL = qObj->getData().toString();
        chanNumF = pObj->getData().toString();
        int value = QString::compare(chanNumF, chanNumL);
        if(value <= 0) {
            qObj->setStrShowOn(chanNumL);
        }
        else{
            qObj->setStrShowOn(chanNumL);
            pObj->changeData(chanNumL ,false);
        }
    }
}

void ConfigTreeObject_AI::slotAIRange_RangeTypeChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(AI_DELEGATE_RANGE_RANGE_TYPE)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(AI_DELEGATE_RANGE_RANGE_TYPE)->getChannelCoder();
    QFile file(XML_AI_RANGE_RANGE_TYPE);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
     }
     resetPartialSettings();
}


//Range_Range量程改变影响的部分写在此处
void ConfigTreeObject_AI::slotAIRange_RangeRangeChanged()
{
    QFile file;
    AI_RANGE_Type type = (AI_RANGE_Type)ConfigTreeObject::getConfigObject(AI_DELEGATE_RANGE_RANGE_TYPE)->getData().toUInt();
    switch (type) {
    case AI_RANGE_Type_Vlot:
        file.setFileName(XML_AI_RANGE_RANGE_VOLT);
        break;
    case AI_RANGE_Type_GS:
        file.setFileName(XML_AI_RANGE_RANGE_GS);
        break;
    case AI_RANGE_Type_TC:
        file.setFileName(XML_AI_RANGE_RANGE_TC);
        break;
    case AI_RANGE_Type_RTD:
        file.setFileName(XML_AI_RANGE_RANGE_RTD);
        break;
    case AI_RANGE_Type_DI:
        file.setFileName(XML_AI_RANGE_RANGE_DI);
        break;
    case AI_RANGE_Type_Cur0_20mA:
//        file.setFileName(":/configtree/Image/configTree/AI_ValueChange/AIRange_RangeType.xml");
//        break;
    case AI_RANGE_Type_GS4_20mA:
//        file.setFileName(":/configtree/Image/configTree/AI_ValueChange/AIRange_RangeType.xml");
//        break;
    default:
        return;
    }

    QVariant value = ConfigTreeObject::getConfigObject(AI_DELEGATE_RANGE_RANGE_RANGE)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(AI_DELEGATE_RANGE_RANGE_RANGE)->getChannelCoder();

    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Range!"<<"["<<__func__<<"]";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
    }

    resetPartialSettings();

    if(type == AI_RANGE_Type_GS) {
        ConfigTreeObject::getConfigObject(AI_DELEGATE_RANGE_RANGE_CALCU)->changeData(AI_Range_Calculate_Linear, false, false);
    }else{
        ConfigTreeObject::getConfigObject(AI_DELEGATE_RANGE_RANGE_CALCU)->changeData(AI_Range_Calculate_Disabled, false, false);
    }
}

//Range_Range量程上限根据实际上限来确定并显示
void ConfigTreeObject_AI::slotAIRange_RangeUpperChanged()
{
    ConfigObject *rObj = ConfigTreeObject::getConfigObject(AI_DELEGATE_RANGE_RANGE_UPPER);
    ConfigObject *sObj = ConfigTreeObject::getConfigObject(AI_DELEGATE_RANGE_RANGE_LOWER);

    float value = rObj->getData().toFloat();    //Up
    float value_t = sObj->getData().toFloat();  //Low
    if(value == value_t) {
        if(value_t == (ConfigTreeObject::getConfigData(AI_DELEGATE_RANGE_RANGE_MIN).toFloat())) {
            value_t = ConfigTreeObject::getConfigData(AI_DELEGATE_RANGE_RANGE_MAX).toFloat();
        }else{
            value_t = ConfigTreeObject::getConfigData(AI_DELEGATE_RANGE_RANGE_MIN).toFloat();
        }
        sObj->changeData(value_t, false);
    }

    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(AI_DELEGATE_RANGE_RANGE_UPPER);
    quint32 ID = ID_AI_Range_Range_DispMax;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

//Range_Range量程下限根据实际下限来确定并显示
void ConfigTreeObject_AI::slotAIRange_RangeLowerChanged()
{
    ConfigObject *rObj = ConfigTreeObject::getConfigObject(AI_DELEGATE_RANGE_RANGE_LOWER);
    ConfigObject *sObj = ConfigTreeObject::getConfigObject(AI_DELEGATE_RANGE_RANGE_UPPER);

    float value = rObj->getData().toFloat();    //Low
    float value_t = sObj->getData().toFloat();  //Up
    if(value == value_t) {
        if(value_t == (ConfigTreeObject::getConfigData(AI_DELEGATE_RANGE_RANGE_MAX).toFloat())) {
            value_t = ConfigTreeObject::getConfigData(AI_DELEGATE_RANGE_RANGE_MIN).toFloat();
        }else{
            value_t = ConfigTreeObject::getConfigData(AI_DELEGATE_RANGE_RANGE_MAX).toFloat();
        }
        sObj->changeData(value_t, false);
    }

    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(AI_DELEGATE_RANGE_RANGE_LOWER);
    quint32 ID = ID_AI_Range_Range_DispMin;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}


void ConfigTreeObject_AI::slotAIRange_RangeCalculationChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(AI_DELEGATE_RANGE_RANGE_CALCU)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(AI_DELEGATE_RANGE_RANGE_CALCU)->getChannelCoder();

    QFile file(XML_AI_RANGE_RANGE_CALCU);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Range:Calculation!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
     }
     resetPartialSettings();

     ConfigObject* pObj = ConfigTreeObject::getConfigObject(AI_DELEGATE_RANGE_RANGE_TYPE);
     quint32 type;
     if(pObj) {
        type = pObj->getData().toUInt();
        if(type == AI_RANGE_Type_DI) {
            ConfigTreeObject::getConfigObject(AI_DELEGATE_RANGE_BIAS)->setHideFlag(true);
        }
     }

     float x = ConfigTreeObject::getConfigObject(AI_DELEGATE_RANGE_RANGE_MIN)->getData().toFloat();
     float y = ConfigTreeObject::getConfigObject(AI_DELEGATE_RANGE_RANGE_MAX)->getData().toFloat();
     quint32 calcu = value.toUInt();
     if(AI_Range_Calculate_Delta == calcu) {
         float differ = fabs(y-x);
         ConfigTreeObject::getConfigObject(AI_DELEGATE_RANGE_RANGE_LOWER)->changeData(-differ, false, false);
         ConfigTreeObject::getConfigObject(AI_DELEGATE_RANGE_RANGE_UPPER)->changeData(differ, false, false);
     } else {
        if(AI_RANGE_Type_GS == type) {
            quint32 range = ConfigTreeObject::getConfigObject(AI_DELEGATE_RANGE_RANGE_RANGE)->getData().toUInt();
            if(AI_Range_GS_Low == range) {
                ConfigTreeObject::getConfigObject(AI_DELEGATE_RANGE_RANGE_LOWER)->changeData(0.4000, false, false);
                ConfigTreeObject::getConfigObject(AI_DELEGATE_RANGE_RANGE_UPPER)->changeData(2.0000, false, false);
            }else{
                ConfigTreeObject::getConfigObject(AI_DELEGATE_RANGE_RANGE_LOWER)->changeData(1.000, false, false);
                ConfigTreeObject::getConfigObject(AI_DELEGATE_RANGE_RANGE_UPPER)->changeData(5.000, false, false);
            }
        } else {
            ConfigTreeObject::getConfigObject(AI_DELEGATE_RANGE_RANGE_LOWER)->changeData(x, false, false);
            ConfigTreeObject::getConfigObject(AI_DELEGATE_RANGE_RANGE_UPPER)->changeData(y, false, false);
        }
    }
}

//基准通道，只有在运算[Calculation]存在且为[Delta]时才会显示。
void ConfigTreeObject_AI::slotAIRange_RangeRefChanChanged()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(AI_DELEGATE_RANGE_RANGE_REF);
    quint32 ID = ID_AI_Range_Range_BasicChannel;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_AI::slotAIRange_ScaleDecimalChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(AI_DELEGATE_RANGE_SCALE_DECPOS)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(AI_DELEGATE_RANGE_SCALE_DECPOS)->getChannelCoder();

    QFile file(XML_AI_RANGE_SCALE_POS);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Scale:Decimal place!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
    }

    resetPartialSettings();
}

void ConfigTreeObject_AI::slotAIRange_ScaleLowerChanged()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(AI_DELEGATE_RANGE_SCALE_LOWER);
    quint32 ID = ID_AI_Range_Scale_Min;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_AI::slotAIRange_ScaleUpperChanged()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(AI_DELEGATE_RANGE_SCALE_UPPER);
    quint32 ID = ID_AI_Range_Scale_Max;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_AI::slotAIRange_ScaleUnitChanged()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(AI_DELEGATE_RANGE_SCALE_UNIT);
    quint32 ID = ID_AI_Range_Scale_Unit;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_AI::slotAIRange_LowcutOnoffChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(AI_DELEGATE_RANGE_LOWCUT_ONOFF)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(AI_DELEGATE_RANGE_LOWCUT_ONOFF)->getChannelCoder();

    QFile file(XML_AI_RANGE_LOWCUT_ONOFF);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Low-cut:On/Off!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
     }
}

void ConfigTreeObject_AI::slotAIRange_LowcutValueChanged()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(AI_DELEGATE_RANGE_LOWCUT_VALUE);
    quint32 ID = ID_AI_Range_LowCut_Value;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_AI::slotAIRange_LowcutOutputChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(AI_DELEGATE_RANGE_LOWCUT_OUTPUT)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(AI_DELEGATE_RANGE_LOWCUT_OUTPUT)->getChannelCoder();

    QFile file(XML_AI_RANGE_LOWCUT_OUTPUT);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Low-cut:Low-cut output!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
     }
}

void ConfigTreeObject_AI::slotAIRange_MovingOnoffChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(AI_DELEGATE_RANGE_MOVAVE_ONOFF)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(AI_DELEGATE_RANGE_MOVAVE_ONOFF)->getChannelCoder();

    QFile file(XML_AI_RANGE_MOVAVE_ONOFF);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Moving average:On/Off!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
     }
}

void ConfigTreeObject_AI::slotAIRange_MovingCountChanged()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(AI_DELEGATE_RANGE_MOVAVE_COUNT);
    quint32 ID = ID_AI_Range_MoveAverange_Cnt;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_AI::slotAIRange_RJCModeChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(AI_DELEGATE_RANGE_RJC_MODE)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(AI_DELEGATE_RANGE_RJC_MODE)->getChannelCoder();

    QFile file(XML_AI_RANGE_RJC_MODE);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:RJC:Mode!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
     }
}

void ConfigTreeObject_AI::slotAIRange_RJCTemperChanged()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(AI_DELEGATE_RANGE_RJC_TEMPER);
    quint32 ID = ID_AI_Range_RJC_Temp;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_AI::slotAIRange_BurnoutModeChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(AI_DELEGATE_RANGE_BURN_MODE)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(AI_DELEGATE_RANGE_BURN_MODE)->getChannelCoder();

    QFile file(XML_AI_RANGE_BURN_MODE);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Burnout set:Mode!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
     }
}

void ConfigTreeObject_AI::slotAIRange_BiasValueChanged()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(AI_DELEGATE_RANGE_BIAS_VALUE);
    quint32 ID = ID_AI_Range_Bias_Value;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_AI::slotAIAlarm_Level1OnoffChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(AI_DELEGATE_ALARM_LVL1_ONOFF)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(AI_DELEGATE_ALARM_LVL1_ONOFF)->getChannelCoder();

    QFile file(XML_AI_ALARM_LVL1_ONOFF);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
       qWarning()<< "Can't open the file: Level 1:On/Off!";
    }else{
       XmlConfigParser::parseValueChangeXml(file, value, coder);
       file.close();
    }
}

void ConfigTreeObject_AI::slotAIAlarm_Level1TypeChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(AI_DELEGATE_ALARM_LVL1_TYPE)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(AI_DELEGATE_ALARM_LVL1_TYPE)->getChannelCoder();

    QFile file(XML_AI_ALARM_LVL1_TYPE);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file: Level 1:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
     }
}

void ConfigTreeObject_AI::slotAIAlarm_Level1ValueChanged()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(AI_DELEGATE_ALARM_LVL1_VALUE);
    quint32 ID = ID_AI_Alarm_Level1_Value;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_AI::slotAIAlarm_Level1HysterChanged()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(AI_DELEGATE_ALARM_LVL1_HYST);
    quint32 ID = ID_AI_Alarm_Level1_Hysteresis;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);

}

void ConfigTreeObject_AI::slotAIAlarm_Level1LoggChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(AI_DELEGATE_ALARM_LVL1_LOGG)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(AI_DELEGATE_ALARM_LVL1_LOGG)->getChannelCoder();

    QFile file(XML_AI_ALARM_LVL1_LOGG);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file: Level 1:Logging!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
     }
}

void ConfigTreeObject_AI::slotAIAlarm_Level1OutTypeChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(AI_DELEGATE_ALARM_LVL1_OUTTYPE)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(AI_DELEGATE_ALARM_LVL1_OUTTYPE)->getChannelCoder();

    QFile file(XML_AI_ALARM_LVL1_OUTTYPE);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file: Level 1:Output type!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
    }

    quint32 valueNo = value.toUInt();
    if(0 == valueNo) {

    } else if (1 == valueNo) {
        QMap<QString, quint32> curChanDO = CfgChannel::instance()->getExistChannelDO();
        if(!curChanDO.isEmpty()) {
            ConfigTreeObject::getConfigObject(AI_DELEGATE_ALARM_LVL1_OUTNO)->changeData(curChanDO.first(), false);
        }
    } else if (2 == valueNo) {
        ConfigTreeObject::getConfigObject(AI_DELEGATE_ALARM_LVL1_OUTNO)->changeData(1, false);
    }
}

void ConfigTreeObject_AI::slotAIAlarm_Level1OutNoChanged()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(AI_DELEGATE_ALARM_LVL1_OUTNO);
    quint32 ID = ID_AI_Alarm_Level1_OutputNo;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_AI::slotAIAlarm_Level2OnoffChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(AI_DELEGATE_ALARM_LVL2_ONOFF)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(AI_DELEGATE_ALARM_LVL2_ONOFF)->getChannelCoder();

    QFile file(XML_AI_ALARM_LVL2_ONOFF);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
       qWarning()<< "Can't open the file: Level 2:On/Off!";
    }else{
       XmlConfigParser::parseValueChangeXml(file, value, coder);
       file.close();
    }
}

void ConfigTreeObject_AI::slotAIAlarm_Level2TypeChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(AI_DELEGATE_ALARM_LVL2_TYPE)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(AI_DELEGATE_ALARM_LVL2_TYPE)->getChannelCoder();

    QFile file(XML_AI_ALARM_LVL2_TYPE);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file: Level 2:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
     }
}

void ConfigTreeObject_AI::slotAIAlarm_Level2ValueChanged()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(AI_DELEGATE_ALARM_LVL2_VALUE);
    quint32 ID = ID_AI_Alarm_Level2_Value;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_AI::slotAIAlarm_Level2HysterChanged()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(AI_DELEGATE_ALARM_LVL2_HYST);
    quint32 ID = ID_AI_Alarm_Level2_Hysteresis;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_AI::slotAIAlarm_Level2LoggChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(AI_DELEGATE_ALARM_LVL2_LOGG)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(AI_DELEGATE_ALARM_LVL2_LOGG)->getChannelCoder();

    QFile file(XML_AI_ALARM_LVL2_LOGG);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file: Level 2:Logging!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
     }
}

void ConfigTreeObject_AI::slotAIAlarm_Level2OutTypeChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(AI_DELEGATE_ALARM_LVL2_OUTTYPE)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(AI_DELEGATE_ALARM_LVL2_OUTTYPE)->getChannelCoder();

    QFile file(XML_AI_ALARM_LVL2_OUTTYPE);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file: Level 2:Output type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
     }

     quint32 valueNo = value.toUInt();
     if(0 == valueNo) {

     } else if (1 == valueNo) {
         QMap<QString, quint32> curChanDO = CfgChannel::instance()->getExistChannelDO();
         if(!curChanDO.isEmpty()) {
             ConfigTreeObject::getConfigObject(AI_DELEGATE_ALARM_LVL2_OUTNO)->changeData(curChanDO.first(), false);
         }
     } else if (2 == valueNo) {
         ConfigTreeObject::getConfigObject(AI_DELEGATE_ALARM_LVL2_OUTNO)->changeData(1, false);
     }
}

void ConfigTreeObject_AI::slotAIAlarm_Level2OutNoChanged()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(AI_DELEGATE_ALARM_LVL2_OUTNO);
    quint32 ID = ID_AI_Alarm_Level2_OutputNo;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_AI::slotAIAlarm_Level3OnoffChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(AI_DELEGATE_ALARM_LVL3_ONOFF)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(AI_DELEGATE_ALARM_LVL3_ONOFF)->getChannelCoder();

    QFile file(XML_AI_ALARM_LVL3_ONOFF);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
       qWarning()<< "Can't open the file: Level 3:On/Off!";
    }else{
       XmlConfigParser::parseValueChangeXml(file, value, coder);
       file.close();
    }
}

void ConfigTreeObject_AI::slotAIAlarm_Level3TypeChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(AI_DELEGATE_ALARM_LVL3_TYPE)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(AI_DELEGATE_ALARM_LVL3_TYPE)->getChannelCoder();

    QFile file(XML_AI_ALARM_LVL3_TYPE);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file: Level 3:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
     }
}

void ConfigTreeObject_AI::slotAIAlarm_Level3ValueChanged()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(AI_DELEGATE_ALARM_LVL3_VALUE);
    quint32 ID = ID_AI_Alarm_Level3_Value;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);

}

void ConfigTreeObject_AI::slotAIAlarm_Level3HysterChanged()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(AI_DELEGATE_ALARM_LVL3_HYST);
    quint32 ID = ID_AI_Alarm_Level3_Hysteresis;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_AI::slotAIAlarm_Level3LoggChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(AI_DELEGATE_ALARM_LVL3_LOGG)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(AI_DELEGATE_ALARM_LVL3_LOGG)->getChannelCoder();

    QFile file(XML_AI_ALARM_LVL3_LOGG);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file: Level 3:Logging!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
     }
}

void ConfigTreeObject_AI::slotAIAlarm_Level3OutTypeChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(AI_DELEGATE_ALARM_LVL3_OUTTYPE)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(AI_DELEGATE_ALARM_LVL3_OUTTYPE)->getChannelCoder();

    QFile file(XML_AI_ALARM_LVL3_OUTTYPE);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file: Level 3:Output type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
     }

     quint32 valueNo = value.toUInt();
     if(0 == valueNo) {

     } else if (1 == valueNo) {
         QMap<QString, quint32> curChanDO = CfgChannel::instance()->getExistChannelDO();
         if(!curChanDO.isEmpty()) {
             ConfigTreeObject::getConfigObject(AI_DELEGATE_ALARM_LVL3_OUTNO)->changeData(curChanDO.first(), false);
         }
     } else if (2 == valueNo) {
         ConfigTreeObject::getConfigObject(AI_DELEGATE_ALARM_LVL3_OUTNO)->changeData(1, false);
     }
}

void ConfigTreeObject_AI::slotAIAlarm_Level3OutNoChanged()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(AI_DELEGATE_ALARM_LVL3_OUTNO);
    quint32 ID = ID_AI_Alarm_Level3_OutputNo;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_AI::slotAIAlarm_Level4OnoffChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(AI_DELEGATE_ALARM_LVL4_ONOFF)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(AI_DELEGATE_ALARM_LVL4_ONOFF)->getChannelCoder();

    QFile file(XML_AI_ALARM_LVL4_ONOFF);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
       qWarning()<< "Can't open the file: Level 4:On/Off!";
    }else{
       XmlConfigParser::parseValueChangeXml(file, value, coder);
       file.close();
    }
}

void ConfigTreeObject_AI::slotAIAlarm_Level4TypeChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(AI_DELEGATE_ALARM_LVL4_TYPE)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(AI_DELEGATE_ALARM_LVL4_TYPE)->getChannelCoder();

    QFile file(XML_AI_ALARM_LVL4_TYPE);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file: Level 4:Type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
     }
}

void ConfigTreeObject_AI::slotAIAlarm_Level4ValueChanged()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(AI_DELEGATE_ALARM_LVL4_VALUE);
    quint32 ID = ID_AI_Alarm_Level4_Value;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);

}

void ConfigTreeObject_AI::slotAIAlarm_Level4HysterChanged()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(AI_DELEGATE_ALARM_LVL4_HYST);
    quint32 ID = ID_AI_Alarm_Level4_Hysteresis;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);

}

void ConfigTreeObject_AI::slotAIAlarm_Level4LoggChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(AI_DELEGATE_ALARM_LVL4_LOGG)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(AI_DELEGATE_ALARM_LVL4_LOGG)->getChannelCoder();

    QFile file(XML_AI_ALARM_LVL4_LOGG);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file: Level 4:Logging!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
     }
}

void ConfigTreeObject_AI::slotAIAlarm_Level4OutTypeChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(AI_DELEGATE_ALARM_LVL4_OUTTYPE)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(AI_DELEGATE_ALARM_LVL4_OUTTYPE)->getChannelCoder();

    QFile file(XML_AI_ALARM_LVL4_OUTTYPE);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file: Level 4:Output type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
     }

     quint32 valueNo = value.toUInt();
     if(0 == valueNo) {

     } else if (1 == valueNo) {
         QMap<QString, quint32> curChanDO = CfgChannel::instance()->getExistChannelDO();
         if(!curChanDO.isEmpty()) {
             ConfigTreeObject::getConfigObject(AI_DELEGATE_ALARM_LVL4_OUTNO)->changeData(curChanDO.first(), false);
         }
     } else if (2 == valueNo) {
         ConfigTreeObject::getConfigObject(AI_DELEGATE_ALARM_LVL4_OUTNO)->changeData(1, false);
     }
}

void ConfigTreeObject_AI::slotAIAlarm_Level4OutNoChanged()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(AI_DELEGATE_ALARM_LVL4_OUTNO);
    quint32 ID = ID_AI_Alarm_Level4_OutputNo;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_AI::slotAIAlarm_DelayHourChanged()
{
     ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(AI_DELEGATE_ALARM_DELAY_HOUR);
     quint32 ID = ID_AI_Alarm_Leve_Delay_h;

     QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
     pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_AI::slotAIAlarm_DelayMinChanged()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(AI_DELEGATE_ALARM_DELAY_MIN);
    quint32 ID = ID_AI_Alarm_Leve_Delay_m;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_AI::slotAIAlarm_DelaySecChanged()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(AI_DELEGATE_ALARM_DELAY_SEC);
    quint32 ID = ID_AI_Alarm_Leve_Delay_s;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_AI::slotAIDisp_ColorColorChanged()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(AI_DELEGATE_DISP_COLOR_COLOR);
    quint32 ID = ID_AI_Disp_Color_Value;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_AI::slotAIDisp_TagCharChanged()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(AI_DELEGATE_DISP_TAG_CHAR);
    quint32 ID = ID_AI_Disp_Tag_Char;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_AI::slotAIDisp_TagNoChanged()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(AI_DELEGATE_DISP_TAG_NO);
    quint32 ID = ID_AI_Disp_Tag_No;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_AI::slotAIDisp_ZoneLowerChanged()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(AI_DELEGATE_DISP_ZONE_LOWER);
    quint32 ID = ID_AI_Disp_Zone_Lower;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_AI::slotAIDisp_ZoneUpperChanged()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(AI_DELEGATE_DISP_ZONE_UPPER);
    quint32 ID = ID_AI_Disp_Zone_Upper;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_AI::slotAIDisp_ScalePosChanged()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(AI_DELEGATE_DISP_SCALE_POS);
    quint32 ID = ID_AI_Disp_Scale_Pos;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_AI::slotAIDisp_ScaleDivChanged()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(AI_DELEGATE_DISP_SCALE_DIV);
    quint32 ID = ID_AI_Disp_Scale_Div;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_AI::slotAIDisp_BarBasePosChanged()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(AI_DELEGATE_DISP_BAR_BASEPOS);
    quint32 ID = ID_AI_DISP_Bar_Pos;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_AI::slotAIDisp_BarDivChanged()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(AI_DELEGATE_DISP_BAR_DIVISION);
    quint32 ID = ID_AI_DISP_Bar_Div;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_AI::slotAIDisp_PartOnoffChanged() //display settings->trend settings->partial打开时，AI-Display中才有此项。
{
    QVariant value = ConfigTreeObject::getConfigObject(AI_DELEGATE_DISP_PART_ONOFF)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(AI_DELEGATE_DISP_PART_ONOFF)->getChannelCoder();

    QFile file(XML_AI_DISP_PART_ONOFF);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file: Partial:On/Off!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
     }
}

void ConfigTreeObject_AI::slotAIDisp_PartExpandChanged()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(AI_DELEGATE_DISP_PART_EXPAND);
    quint32 ID = ID_AI_DISP_Partial_Pos;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_AI::slotAIDisp_PartBoundChanged()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(AI_DELEGATE_DISP_PART_BOUND);
    quint32 ID = ID_AI_DISP_Partial_Boundary;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_AI::slotAIDisp_CsbandBandareaChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(AI_DELEGATE_DISP_CSBAND_AREA)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(AI_DELEGATE_DISP_CSBAND_AREA)->getChannelCoder();

    QFile file(XML_AI_DISP_CSBAND_AREA);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file: Color scale band:Band area!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
     }
}

void ConfigTreeObject_AI::slotAIDisp_CsbandDispLowChanged()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(AI_DELEGATE_DISP_CSBAND_LOWER);
    quint32 ID = ID_AI_DISP_ColorScaleBand_PosLower;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_AI::slotAIDisp_CsbandDispUpChanged()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(AI_DELEGATE_DISP_CSBAND_UPPER);
    quint32 ID = ID_AI_DISP_ColorScaleBand_PosUpper;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_AI::slotAIDisp_CsbandColorChanged()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(AI_DELEGATE_DISP_CSBAND_COLOR);
    quint32 ID = ID_AI_DISP_ColorScaleBand_Color;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_AI::slotAIDisp_AlarmIndicateChanged()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(AI_DELEGATE_DISP_ALARM_INDIC);
    quint32 ID = ID_AI_DISP_AlarmMark_OnOff;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_AI::slotAIDisp_AlarmMarkTypeChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(AI_DELEGATE_DISP_ALARM_MARKTYPE)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(AI_DELEGATE_DISP_ALARM_MARKTYPE)->getChannelCoder();

    QFile file(XML_AI_DISP_ALARM_MARKTYPE);
     if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file: Alarm point mark:Mark type!";
     }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
     }
}

void ConfigTreeObject_AI::slotAIDisp_AlarmAlarm1Changed()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(AI_DELEGATE_DISP_ALARM_COLOR1);
    quint32 ID = ID_AI_DISP_AlarmMark_Color1;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_AI::slotAIDisp_AlarmAlarm2Changed()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(AI_DELEGATE_DISP_ALARM_COLOR2);
    quint32 ID = ID_AI_DISP_AlarmMark_Color2;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_AI::slotAIDisp_AlarmAlarm3Changed()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(AI_DELEGATE_DISP_ALARM_COLOR3);
    quint32 ID = ID_AI_DISP_AlarmMark_Color3;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_AI::slotAIDisp_AlarmAlarm4Changed()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(AI_DELEGATE_DISP_ALARM_COLOR4);
    quint32 ID = ID_AI_DISP_AlarmMark_Color4;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

//【DI】类型量程且运算off时有效，测量值为0/1时显示的字符串
void ConfigTreeObject_AI::slotAIDisp_DisVal_0_Changed()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(AI_DELEGATE_DISP_DISPCHAR_0);
    quint32 ID = ID_AI_DISP_DI0_Str;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_AI::slotAIDisp_DisVal_1_Changed()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(AI_DELEGATE_DISP_DISPCHAR_1);
    quint32 ID = ID_AI_DISP_DI1_Str;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_AI::slotAICali_ModeModeChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_MODE_MODE)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_MODE_MODE)->getChannelCoder();

    QFile file(XML_AI_CALI_MODE_MODE);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file: Mode:Mode!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();
    }
}

void ConfigTreeObject_AI::slotAICali_ModeNumSetChanged()
{
    QVariant value = ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_MODE_NUM)->getData();
    quint32 coder = ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_MODE_NUM)->getChannelCoder();

    QFile file(XML_AI_CALI_MODE_NUM);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file: Mode:Number of set points!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value, coder);
        file.close();

        quint32 correctCount = value.toUInt();
        setAICaliCorrect_Value(correctCount);
    }
}

void ConfigTreeObject_AI::slotAICali_OneLineInChanged()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_1_INPUT);
    quint32 ID = ID_AI_Correct_Point1_In;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_AI::slotAICali_OneLineOutChanged()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_1_OUTPUT);
    quint32 ID = ID_AI_Correct_Point1_Out;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_AI::slotAICali_TwoLineInChanged()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_2_INPUT);
    quint32 ID = ID_AI_Correct_Point2_In;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_AI::slotAICali_TwoLineOutChanged()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_2_OUTPUT);
    quint32 ID = ID_AI_Correct_Point2_Out;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_AI::slotAICali_ThreeLineInChanged()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_3_INPUT);
    quint32 ID = ID_AI_Correct_Point3_In;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_AI::slotAICali_ThreeLineOutChanged()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_3_OUTPUT);
    quint32 ID = ID_AI_Correct_Point3_Out;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_AI::slotAICali_FourLineInChanged()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_4_INPUT);
    quint32 ID = ID_AI_Correct_Point4_In;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_AI::slotAICali_FourLineOutChanged()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_4_OUTPUT);
    quint32 ID = ID_AI_Correct_Point4_Out;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_AI::slotAICali_FiveLineInChanged()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_5_INPUT);
    quint32 ID = ID_AI_Correct_Point5_In;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_AI::slotAICali_FiveLineOutChanged()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_5_OUTPUT);
    quint32 ID = ID_AI_Correct_Point5_Out;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_AI::slotAICali_SixLineInChanged()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_6_INPUT);
    quint32 ID = ID_AI_Correct_Point6_In;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_AI::slotAICali_SixLineOutChanged()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_6_OUTPUT);
    quint32 ID = ID_AI_Correct_Point6_Out;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_AI::slotAICali_SevenLineInChanged()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_7_INPUT);
    quint32 ID = ID_AI_Correct_Point7_In;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_AI::slotAICali_SevenLineOutChanged()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_7_OUTPUT);
    quint32 ID = ID_AI_Correct_Point7_Out;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_AI::slotAICali_EightLineInChanged()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_8_INPUT);
    quint32 ID = ID_AI_Correct_Point8_In;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_AI::slotAICali_EightLineOutChanged()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_8_OUTPUT);
    quint32 ID = ID_AI_Correct_Point8_Out;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_AI::slotAICali_NineLineInChanged()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_9_INPUT);
    quint32 ID = ID_AI_Correct_Point9_In;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_AI::slotAICali_NineLineOutChanged()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_9_OUTPUT);
    quint32 ID = ID_AI_Correct_Point9_Out;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_AI::slotAICali_TenLineInChanged()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_10_INPUT);
    quint32 ID = ID_AI_Correct_Point10_In;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_AI::slotAICali_TenLineOutChanged()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_10_OUTPUT);
    quint32 ID = ID_AI_Correct_Point10_Out;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_AI::slotAICali_ElevenLineInChanged()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_11_INPUT);
    quint32 ID = ID_AI_Correct_Point11_In;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_AI::slotAICali_ElevenLineOutChanged()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_11_OUTPUT);
    quint32 ID = ID_AI_Correct_Point11_Out;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_AI::slotAICali_TwelveLineInChanged()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_12_INPUT);
    quint32 ID = ID_AI_Correct_Point12_In;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_AI::slotAICali_TwelveLineOutChanged()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_12_OUTPUT);
    quint32 ID = ID_AI_Correct_Point12_Out;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}
