#include "cfgconfig_measure.h"
#include "globalDef.h"
#include "./GlobalData/configtreeobject.h"
#include "./Common/devboard.h"

CfgConfig_Measure *CfgConfig_Measure::instance()
{
    static CfgConfig_Measure data;
    return &data;
}

CfgConfig_Measure::CfgConfig_Measure(QObject *parent) :
    QObject(parent)
{
}


QVariant CfgConfig_Measure::getMeasureData(quint32 ID, quint32 /*group*/)
{
    switch (ID) {
    case ID_Measure_Scan_Interval:
        return p_deviceConfig->measureInterval_ms;
        break;

    case ID_Measure_OverRange_Value:
        return p_deviceConfig->measureOverRange;
        break;

    case ID_Measure_DI_Unit_Mode_Value:
        return p_deviceConfig->DIRemoteModule;
        break;

    default:
        break;
    }
    return QVariant();
}

QString CfgConfig_Measure::getMeasureObjStr(quint32 ID, quint32 /*group*/)
{
    switch (ID) {
    case ID_Measure:
        return tr("Measure settings");
        break;
    case ID_Measure_Scan:
    case ID_Measure_Scan_Interval:
        return tr("Scan interval");
        break;

    case ID_Measure_OverRange:
        return tr("Over-range");
        break;
    case ID_Measure_OverRange_Value:
        return tr("Value of over-range");
        break;

    case ID_Measure_Select:
        return tr("Select unit");
        break;

    case ID_Measure_Select_MainUnit:
        return tr("Main Unit");
        break;

    case ID_Measure_Select_ExtUnit1:
    case ID_Measure_Select_ExtUnit2:
    case ID_Measure_Select_ExtUnit3:
    case ID_Measure_Select_ExtUnit4:
    case ID_Measure_Select_ExtUnit5:
    case ID_Measure_Select_ExtUnit6:
    {
        int id = ID % 10;
        return QString("%1 %2").arg(tr("Ext Unit")).arg(id);
    }
        break;

    case ID_Measure_Select_MainUnit_Mod0:
    case ID_Measure_Select_MainUnit_Mod1:
    case ID_Measure_Select_MainUnit_Mod2:
    case ID_Measure_Select_MainUnit_Mod3:
    case ID_Measure_Select_MainUnit_Mod4:
    case ID_Measure_Select_MainUnit_Mod5:
    case ID_Measure_Select_MainUnit_Mod6:
    case ID_Measure_Select_MainUnit_Mod7:
    case ID_Measure_Select_MainUnit_Mod8:
    case ID_Measure_Select_MainUnit_Mod9:


    case ID_Measure_Select_ExtUnit1_Mod0:
    case ID_Measure_Select_ExtUnit1_Mod1:
    case ID_Measure_Select_ExtUnit1_Mod2:
    case ID_Measure_Select_ExtUnit1_Mod3:
    case ID_Measure_Select_ExtUnit1_Mod4:
    case ID_Measure_Select_ExtUnit1_Mod5:
    case ID_Measure_Select_ExtUnit2_Mod0:
    case ID_Measure_Select_ExtUnit2_Mod1:
    case ID_Measure_Select_ExtUnit2_Mod2:
    case ID_Measure_Select_ExtUnit2_Mod3:
    case ID_Measure_Select_ExtUnit2_Mod4:
    case ID_Measure_Select_ExtUnit2_Mod5:
    case ID_Measure_Select_ExtUnit3_Mod0:
    case ID_Measure_Select_ExtUnit3_Mod1:
    case ID_Measure_Select_ExtUnit3_Mod2:
    case ID_Measure_Select_ExtUnit3_Mod3:
    case ID_Measure_Select_ExtUnit3_Mod4:
    case ID_Measure_Select_ExtUnit3_Mod5:
    case ID_Measure_Select_ExtUnit4_Mod0:
    case ID_Measure_Select_ExtUnit4_Mod1:
    case ID_Measure_Select_ExtUnit4_Mod2:
    case ID_Measure_Select_ExtUnit4_Mod3:
    case ID_Measure_Select_ExtUnit4_Mod4:
    case ID_Measure_Select_ExtUnit4_Mod5:
    case ID_Measure_Select_ExtUnit5_Mod0:
    case ID_Measure_Select_ExtUnit5_Mod1:
    case ID_Measure_Select_ExtUnit5_Mod2:
    case ID_Measure_Select_ExtUnit5_Mod3:
    case ID_Measure_Select_ExtUnit5_Mod4:
    case ID_Measure_Select_ExtUnit5_Mod5:
    case ID_Measure_Select_ExtUnit6_Mod0:
    case ID_Measure_Select_ExtUnit6_Mod1:
    case ID_Measure_Select_ExtUnit6_Mod2:
    case ID_Measure_Select_ExtUnit6_Mod3:
    case ID_Measure_Select_ExtUnit6_Mod4:
    case ID_Measure_Select_ExtUnit6_Mod5:
    {
        int id = ID % 10;
        return QString("%1 %2").arg(tr("Module")).arg(id);
    }
        break;

    case ID_Measure_AI_Unit_Mode:
    case ID_Measure_DI_Unit_Mode:
    case ID_Measure_AI_Unit_Mode_Value:
    case ID_Measure_DI_Unit_Mode_Value:
        return tr("Operation mode");
        break;
    case ID_Measure_AI_Unit_AD:
    case ID_Measure_AI_Unit_AD_Value:
        return tr("A/D integrate");
        break;
    case ID_Measure_AI_Unit_Gneral:
        return tr("General signal");
        break;
    case ID_Measure_AI_Unit_Gneral_Low:
        return tr("Lower limit of burnout set");
        break;
    case ID_Measure_AI_Unit_Gneral_Upper:
        return tr("Upper limit of burnout set");
        break;

    default:
        break;
    }
    return NULL;
}

QString CfgConfig_Measure::getMeasureObjValStr(quint32 ID, quint32 /*group*/, ConfigObject *cfg)
{
    switch (ID) {
    case ID_Measure_Scan_Interval:
    {
        int data = cfg->getData().toInt();
        if (data < 1000)
            return cfg->getData().toString() + " " + tr("ms");
        else
            return QString("%1 ").arg(data / 1000) + tr("s");
    }
        break;

    case ID_Measure_OverRange_Value:
        return cfg->getData().toInt() == 0 ? tr("Free") : tr("Over");
        break;

    case ID_Measure_AI_Unit_AD_Value:
    {
        int type = cfg->getData().toInt();
        if (type == Board_ADTime_Auto)
            return tr("Auto");
        else if (type == Board_ADTime_50Hz)
            return "50 HZ";
        else if (type == Board_ADTime_60Hz)
            return "60 HZ";
        else
            return NULL;
    }
        break;
    case ID_Measure_AI_Unit_Gneral_Low:
        return QString("%1 %").arg(cfg->getData().toString());
        break;
    case ID_Measure_AI_Unit_Gneral_Upper:
        return QString("%1 %").arg(cfg->getData().toString());
        break;

    case ID_Measure_DI_Unit_Mode_Value:
        return cfg->getData().toInt() == 0 ? tr("Normal") : tr("Remote");
        break;

    default:
        break;
    }
    return NULL;
}


void CfgConfig_Measure::setMeasureData(quint32 ID, quint32 /*group*/)
{
    switch (ID) {
    case ID_Measure_Scan_Interval:
        p_deviceConfig->measureInterval_ms = (Measure_Interval)ConfigTreeObject::getConfigData(MEASURECONFIG_INTERVAL_INTERVAL).toInt();
        break;

    case ID_Measure_OverRange_Value:
        p_deviceConfig->measureOverRange = (Measure_OverRange)ConfigTreeObject::getConfigData(MEASURECONFIG_OVERANG_VALUE).toInt();
        break;
    default:
        break;
    }
}

/*
 * 保存DI为远程模式下的Module号
 *      module : 远程模式下的Module号
*/
void CfgConfig_Measure::setMeasureDIModeData(quint32 module)
{
    p_deviceConfig->DIRemoteModule = module;
}
