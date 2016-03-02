#include "cfgconfig_do.h"
#include <QVariant>
#include "cfgchannel.h"


CfgConfig_DO *CfgConfig_DO::instance()
{
    static CfgConfig_DO data;
    return &data;
}

CfgConfig_DO::CfgConfig_DO(QObject *parent) :
    QObject(parent)
{
}

/*
 * get DO data by ID and group
*/
QVariant CfgConfig_DO::getDOData(quint32 ID, quint32 group)
{
    if (ID >= DO_Range_Start_ID && ID <= DO_Range_End_ID)
        return getDORangeData(ID, group);
    else if (ID >= DO_Disp_Start_ID && ID <= DO_Disp_End_ID)
        return getDODispData(ID, group);
    return QVariant();
}

//get DO obj str by ID and group
QString CfgConfig_DO::getDOObjStr(quint32 ID, quint32 group)
{
    if (ID >= DO_Start_ID && ID <= DO_Range_End_ID)
        return getDORangeObjStr(ID, group);
    else if (ID >= DO_Disp_Start_ID && ID <= DO_Disp_End_ID)
        return getDODispObjStr(ID, group);
    return NULL;
}

//get DO obj value str by ID and group
QString CfgConfig_DO::getDOValueObjStr(quint32 ID, quint32 group, ConfigObject *cfg)
{
    if (ID >= DO_Range_Start_ID && ID <= DO_Range_End_ID)
        return getDORangeObjValStr(ID, group, cfg);
    else if (ID >= DO_Disp_Start_ID && ID <= DO_Disp_End_ID)
        return getDODispObjValStr(ID, group, cfg);
    return NULL;
}

//set DO data by ID and group
void CfgConfig_DO::setDOData(quint32 ID, quint32 group)
{
    if (ID >= DO_Range_Start_ID && ID <= DO_Range_End_ID)
        setDORangeData(ID, group);
    if (ID >= DO_Disp_Start_ID && ID <= DO_Disp_End_ID)
        setDODispData(ID, group);
}



// get DO range data by ID and group
QVariant CfgConfig_DO::getDORangeData(quint32 ID, quint32 group)
{
    DO_CONFIG DoCfg = *CfgChannel::instance()->getDOConfig(group);
    switch (ID) {
    case ID_DO_Range_Range_Type:         //量程类型, 0-报警， 1-手动
        return DoCfg.rangeConfig.rangeType;
        break;
    case ID_DO_Range_Range_Min:         //范围下限：0/1，上下限值不能相同
        return DoCfg.rangeConfig.rangeMin;
        break;
    case ID_DO_Range_Range_Max:
        return DoCfg.rangeConfig.rangeMax;
        break;

    case ID_DO_Range_Range_DispMin:     //范围下限：0/1，上下限值不能相同
        return DoCfg.rangeConfig.rangeDispMin;
        break;
    case ID_DO_Range_Range_DispMax:
        return DoCfg.rangeConfig.rangeDispMax;
        break;
    case ID_DO_Range_Range_Unit:            //单位，字符串，最多6个半角字符
        return getCharToString(DoCfg.rangeConfig.rangeUnit);
        break;

        //action
    case ID_DO_Range_Action_Energize:         //励磁/非励磁， 0-励磁， 1-非励磁
        return DoCfg.activeConfig.actionEnergize;
        break;
    case ID_DO_Range_Action_ActionMode:         //动作，0-或， 1-与， 2-再故障再报警
        return DoCfg.activeConfig.actionMode;
        break;
    case ID_DO_Range_Action_ACK:                //报警接触动作，0-标准，1-复位
        return DoCfg.activeConfig.actionACK;
        break;
    case ID_DO_Range_Action_Hold:                //保持：0-保持，1-非保持 **再故障再报警时无效**
        return DoCfg.activeConfig.actionHold;
        break;
    case ID_DO_Range_Action_Interval:                 //继电器非动作间隔，500ms/1s/2s**再故障再报警时有效**
    {
        quint32 data = DoCfg.activeConfig.actionDeactiveInterval;
        if (data == 500)
            return 0;
        else if (data == 1000)
            return 1;
        else if (data == 2000)
            return 2;
    }
        break;
    default:
        break;
    }
    return QVariant();
}

/*
 * get DO display data by ID and group
*/
QVariant CfgConfig_DO::getDODispData(quint32 ID, quint32 group)
{
    DO_CONFIG DoCfg = *CfgChannel::instance()->getDOConfig(group);
    switch (ID) {
    case ID_DO_Disp_Color_Value:        //通道颜色，Byte2-Red, Byte1-Green, Byte0-Blue
        return DoCfg.displayConfig.displayColor;
        break;
    case ID_DO_Disp_Tag_Char:           //标记字符串，中英支持，最多32个半角字符
        return getCharToString(DoCfg.displayConfig.displayTagChar);
        break;
    case ID_DO_Disp_Tag_No:               //标记号码，符号，最多16个半角字符
        return getCharToString(DoCfg.displayConfig.displayTagNo);
        break;

    case ID_DO_Disp_Zone_Upper:           //显示区域上限，5-100,单位%
        return DoCfg.displayConfig.displayZoneUpper;
        break;
    case ID_DO_Disp_Zone_Lower:               //显示区域上限，0-95,单位%
        return DoCfg.displayConfig.dispalyZoneLower;
        break;

    case ID_DO_Disp_Scale_Pos:           //标尺位置，1-10,默认1
        return DoCfg.displayConfig.displayScalePos;
        break;
    case ID_DO_DISP_Bar_Pos:               //棒图基准位置， 0-Lower, 1-Center, 2-Upper
        return DoCfg.displayConfig.displayBarPos;
        break;

    case ID_DO_DISP_DI0_Str:           //最多8个半角字符，中英文
        return getCharToString(DoCfg.displayConfig.displayDI0);
        break;
    case ID_DO_DISP_DI1_Str:
        return getCharToString(DoCfg.displayConfig.displayDI1);
        break;
    default:
        break;
    }
    return QVariant();
}


// get DO range obj str by ID
QString CfgConfig_DO::getDORangeObjStr(quint32 ID, quint32 /*group*/)
{
    switch (ID) {
    case ID_DO:
        return QString(tr("DO channel settings"));
        break;
    case ID_DO_First_CH:
        return QString(tr("First-CH"));
        break;
    case ID_DO_Last_CH:
        return QString(tr("Last-CH"));
        break;
    case ID_DO_Range:         //量程
    case ID_DO_Range_Range:         //量程
        return QString(tr("Range"));
        break;
    case ID_DO_Range_Range_Type:         //量程类型, 0-报警， 1-手动
        return QString(tr("Type"));
        break;
    case ID_DO_Range_Range_Min:
        return QString(tr("Min"));
        break;
    case ID_DO_Range_Range_Max:
        return QString(tr("Max"));
        break;
    case ID_DO_Range_Range_DispMin:     //范围下限：0/1，上下限值不能相同
        return QString(tr("Span Lower"));
        break;
    case ID_DO_Range_Range_DispMax:
        return QString(tr("Span Upper"));
        break;
    case ID_DO_Range_Range_Unit:        //单位，字符串，最多6个半角字符
        return QString(tr("Unit"));
        break;

    case ID_DO_Range_Action:
        return QString(tr("Action"));
        break;
    case ID_DO_Range_Action_Energize:        //励磁/非励磁， 0-励磁， 1-非励磁
        return QString(tr("Energize/De-energize"));
        break;
    case ID_DO_Range_Action_ActionMode:     //动作，0-或， 1-与， 2-再故障再报警
        return QString(tr("Action"));
        break;
    case ID_DO_Range_Action_ACK:        //报警接触动作，0-标准，1-复位
        return QString(tr("Relay action on ACK"));
        break;

    case ID_DO_Range_Action_Hold:        //保持：0-保持，1-非保持 **再故障再报警时无效**
        return QString(tr("Hold"));
        break;
    case ID_DO_Range_Action_Interval:       //继电器非动作间隔，500ms/1s/2s**再故障再报警时有效*
        return QString(tr("Relay deactivated interval"));
        break;
    default:
        break;
    }
    return NULL;
}

// get DO display obj str by ID
QString CfgConfig_DO::getDODispObjStr(quint32 ID, quint32 /*group*/)
{
    switch (ID) {
    case ID_DO_Disp:
        return QString(tr("Display settings"));
        break;
    case ID_DO_Disp_Tag:
        return QString(tr("Tag"));
        break;
    case ID_DO_Disp_Tag_Char:       //标记字符串，中英支持，最多32个半角字符
        return QString(tr("Characters"));
        break;
    case ID_DO_Disp_Tag_No:      //标记号码，符号，最多16个半角字符
        return QString(tr("No."));
        break;

    case ID_DO_Disp_Color:
        return QString(tr("Color"));
        break;
    case ID_DO_Disp_Color_Value:    //通道颜色，Byte2-Red, Byte1-Green, Byte0-Blue
        return QString(tr("Color"));
        break;

    case ID_DO_Disp_Zone:
        return QString(tr("Zone"));
        break;
    case ID_DO_Disp_Zone_Upper:     //显示区域上限，5-100,单位%
        return QString(tr("Upper"));
        break;
    case ID_DO_Disp_Zone_Lower:    //显示区域上限，0-95,单位%
        return QString(tr("Lower"));
        break;

    case ID_DO_Disp_Scale:
        return QString(tr("Scale"));
        break;
    case ID_DO_Disp_Scale_Pos:       //标尺位置，1-10,默认1
        return QString(tr("Position"));
        break;

    case ID_DO_DISP_Bar:
        return QString(tr("Bar graph"));
        break;
    case ID_DO_DISP_Bar_Pos:       //棒图基准位置， 0-Lower, 1-Center, 2-Upper
        return QString(tr("Base position"));
        break;

    case ID_DO_DISP_DI:    //最多8个半角字符，中英文
        return QString(tr("Display characters of each value"));
        break;
    case ID_DO_DISP_DI0_Str:
        return QString(tr("0"));
        break;
    case ID_DO_DISP_DI1_Str:
        return QString(tr("1"));
        break;

    default:
        break;
    }
    return NULL;
}

// get DO range obj val str by ID,group and cfg
QString CfgConfig_DO::getDORangeObjValStr(quint32 ID, quint32 /*group*/, ConfigObject *cfg)
{
    switch (ID) {
    case ID_DO_Range_Range_Type:          //量程类型, 0-报警， 1-手动
        return cfg->getData().toInt() == 0 ? QString(tr("Alarm")) : QString(tr("Manual"));
        break;

    case ID_DO_Range_Range_Min:      //范围下限：0/1，上下限值不能相同
    case ID_DO_Range_Range_Max:
    case ID_DO_Range_Range_DispMin:
    case ID_DO_Range_Range_DispMax:
        return QString::number(cfg->getData().toInt());
        break;

    case ID_DO_Range_Range_Unit:
        return cfg->getData().toString();
        break;

    case ID_DO_Range_Action_Energize:         //励磁/非励磁， 0-励磁， 1-非励磁
        return cfg->getData().toInt() == 0 ? QString(tr("Energize")) : QString(tr("De-energize"));
        break;
    case ID_DO_Range_Action_ActionMode:        //动作，0-或， 1-与， 2-再故障再报警
    {
        int type = cfg->getData().toInt();
        if (type == 0)
            return tr("Or");
        else if (type == 1)
            return tr("And");
        else if (type == 2)
            return tr("Reflash");
    }
        break;
    case ID_DO_Range_Action_ACK:         //报警接触动作，0-标准，1-复位
        return cfg->getData().toInt() == 0 ? QString(tr("Normal")) : QString(tr("Reset"));
        break;
    case ID_DO_Range_Action_Hold:          //保持：0-保持，1-非保持 **再故障再报警时无效**
        return cfg->getData().toInt() == 0 ? QString(tr("Hold")) : QString(tr("Nonhold"));
        break;
    case ID_DO_Range_Action_Interval:          //继电器非动作间隔，500ms/1s/2s**再故障再报警时有效**
    {
        int type = cfg->getData().toInt();
        if (type == 0)
            return tr("500 ms");
        else if (type == 1)
            return tr("1 s");
        else if (type == 2)
            return tr("2 s");
    }
        break;
    default:
        break;
    }
    return NULL;
}


// get DO display obj val str by ID,group and cfg
QString CfgConfig_DO::getDODispObjValStr(quint32 ID, quint32 /*group*/, ConfigObject *cfg)
{
    switch (ID) {
    case ID_DO_Disp_Color_Value:         //通道颜色，Byte2-Red, Byte1-Green, Byte0-Blue
        return getDODisplayColorValueObjStr(cfg->getData().toUInt());
        break;

    case ID_DO_Disp_Tag_Char:        //标记字符串，中英支持，最多32个半角字符
    case ID_DO_Disp_Tag_No:     //标记号码，符号，最多16个半角字符
    case ID_DO_DISP_DI0_Str:       //最多8个半角字符，中英文
    case ID_DO_DISP_DI1_Str:
        return cfg->getData().toString();
        break;

    case ID_DO_Disp_Zone_Upper:        //显示区域上限，5-100,单位%
    case ID_DO_Disp_Zone_Lower:     //显示区域上限，0-95,单位%
        return cfg->getData().toString() + " %";
        break;

    case ID_DO_Disp_Scale_Pos:       //标尺位置，1-10,默认1
    {
        int type = cfg->getData().toInt();
        if (type == 0) return tr("Off");
        return cfg->getData().toString();
    }
        break;
    case ID_DO_DISP_Bar_Pos:     //棒图基准位置， 0-Lower, 1-Center, 2-Upper
    {
        int pos = cfg->getData().toInt();
        if (pos == 0)
            return tr("Lower");
        else if (pos == 1)
            return tr("Center");
        else
            return tr("Upper");
    }
    default:
        break;
    }
    return NULL;
}

//save DO range data by group and ID
void CfgConfig_DO::setDORangeData(quint32 ID, quint32 group)
{
    int index = CfgChannel::instance()->getExistChannelDO().values().indexOf(group);
    QString chanNum = CfgChannel::instance()->getExistChannelDO().keys().at(index);
    DO_CONFIG *DoCfg = CfgChannel::instance()->getDOConfig(group);
    switch (ID) {
    case ID_DO_Range_Range_Type:    //量程类型, 0-报警， 1-手动
        DoCfg->rangeConfig.rangeType = ConfigTreeObject::getConfigData(QString(DOCONFIG_RANGE_RANGE_TYPE).arg(chanNum)).toUInt();
        break;
    case ID_DO_Range_Range_Min:     //范围下限：0/1，上下限值不能相同
        DoCfg->rangeConfig.rangeMin = ConfigTreeObject::getConfigData(QString(DOCONFIG_RANGE_RANGE_MIN).arg(chanNum)).toUInt();
        break;
    case ID_DO_Range_Range_Max:     //范围下限：0/1，上下限值不能相同
        DoCfg->rangeConfig.rangeMax = ConfigTreeObject::getConfigData(QString(DOCONFIG_RANGE_RANGE_MAX).arg(chanNum)).toUInt();
        break;
    case ID_DO_Range_Range_DispMin:     //范围下限：0/1，上下限值不能相同
    case ID_DO_Range_Range_DispMax:     //范围下限：0/1，上下限值不能相同
    {
        DoCfg->rangeConfig.rangeDispMin = ConfigTreeObject::getConfigData(QString(DOCONFIG_RANGE_RANGE_LOWER).arg(chanNum)).toUInt();
        DoCfg->rangeConfig.rangeDispMax = ConfigTreeObject::getConfigData(QString(DOCONFIG_RANGE_RANGE_UPPER).arg(chanNum)).toUInt();
    }
        break;
    case ID_DO_Range_Range_Unit:     //单位，字符串，最多6个半角字符
        setStrToChar(DoCfg->rangeConfig.rangeUnit, ConfigTreeObject::getConfigData(QString(DOCONFIG_RANGE_RANGE_UNIT).arg(chanNum)).toString(), 6);
        break;

    case ID_DO_Range_Action_Energize:    //励磁/非励磁， 0-励磁， 1-非励磁
        DoCfg->activeConfig.actionEnergize = ConfigTreeObject::getConfigData(QString(DOCONFIG_RANGE_ACTION_ENERGIZE).arg(chanNum)).toUInt();
        break;
    case ID_DO_Range_Action_ActionMode:   //动作，0-或， 1-与， 2-再故障再报警
        DoCfg->activeConfig.actionMode = ConfigTreeObject::getConfigData(QString(DOCONFIG_RANGE_ACTION_ACTION).arg(chanNum)).toUInt();
        break;
    case ID_DO_Range_Action_ACK:    //报警接触动作，0-标准，1-复位
        DoCfg->activeConfig.actionACK = ConfigTreeObject::getConfigData(QString(DOCONFIG_RANGE_ACTION_RELAYACK).arg(chanNum)).toUInt();
        break;
    case ID_DO_Range_Action_Hold:   //保持：0-保持，1-非保持 **再故障再报警时无效**
        DoCfg->activeConfig.actionHold = ConfigTreeObject::getConfigData(QString(DOCONFIG_RANGE_ACTION_HOLD).arg(chanNum)).toUInt();
        break;
    case ID_DO_Range_Action_Interval:     //继电器非动作间隔，500ms/1s/2s**再故障再报警时有效**
    {
        int type = ConfigTreeObject::getConfigData(QString(DOCONFIG_RANGE_ACTION_INTERVAL).arg(chanNum)).toUInt();
        if (type == 0)
            DoCfg->activeConfig.actionDeactiveInterval = 500;
        else if (type == 1)
            DoCfg->activeConfig.actionDeactiveInterval = 1000;
        else if (type == 2)
            DoCfg->activeConfig.actionDeactiveInterval = 2000;
    }
        break;
    default:
        break;
    }
}

//save DO display data by ID and group
void CfgConfig_DO::setDODispData(quint32 ID, quint32 group)
{
    int index = CfgChannel::instance()->getExistChannelDO().values().indexOf(group);
    QString chanNum = CfgChannel::instance()->getExistChannelDO().keys().at(index);
    DO_CONFIG *DoCfg = CfgChannel::instance()->getDOConfig(group);
    switch (ID) {
    case ID_DO_Disp_Color_Value:    //通道颜色，Byte2-Red, Byte1-Green, Byte0-Blue
        DoCfg->displayConfig.displayColor = ConfigTreeObject::getConfigData(QString(DOCONFIG_DISP_COLOR_COLOR).arg(chanNum)).toUInt();
        break;
    case ID_DO_Disp_Tag_Char:       //标记字符串，中英支持，最多32个半角字符
        setStrToChar(DoCfg->displayConfig.displayTagChar, ConfigTreeObject::getConfigData(QString(DOCONFIG_DISP_TAG_CHAR).arg(chanNum)).toString(), 32);
        break;
    case ID_DO_Disp_Tag_No:      //标记号码，符号，最多16个半角字符
        setStrToChar(DoCfg->displayConfig.displayTagNo, ConfigTreeObject::getConfigData(QString(DOCONFIG_DISP_TAG_NO).arg(chanNum)).toString(), 16);
        break;

    case ID_DO_Disp_Zone_Upper:     //显示区域上限，5-100,单位%
        DoCfg->displayConfig.displayZoneUpper = ConfigTreeObject::getConfigData(QString(DOCONFIG_DISP_ZONE_UPPER).arg(chanNum)).toUInt();
        break;
    case ID_DO_Disp_Zone_Lower:     //显示区域上限，0-95,单位%
        DoCfg->displayConfig.dispalyZoneLower = ConfigTreeObject::getConfigData(QString(DOCONFIG_DISP_ZONE_LOWER).arg(chanNum)).toUInt();
        break;

    case ID_DO_Disp_Scale_Pos:      //标尺位置，1-10,默认1
        DoCfg->displayConfig.displayScalePos = ConfigTreeObject::getConfigData(QString(DOCONFIG_DISP_SCALE_POS).arg(chanNum)).toUInt();
        break;
    case ID_DO_DISP_Bar_Pos:      //棒图基准位置， 0-Lower, 1-Center, 2-Upper
        DoCfg->displayConfig.displayBarPos = ConfigTreeObject::getConfigData(QString(DOCONFIG_DISP_BAR_POSITION).arg(chanNum)).toUInt();
        break;

    case ID_DO_DISP_DI0_Str:
        setStrToChar(DoCfg->displayConfig.displayDI0, ConfigTreeObject::getConfigData(QString(DOCONFIG_DISP_DISCHAR_0).arg(chanNum)).toString(), 10);
        break;
    case ID_DO_DISP_DI1_Str:
        setStrToChar(DoCfg->displayConfig.displayDI1, ConfigTreeObject::getConfigData(QString(DOCONFIG_DISP_DISCHAR_1).arg(chanNum)).toString(), 10);
        break;
    default:
        break;
    }
}

/*
 * 把uchar *类型转化为QString类型
*/
QVariant CfgConfig_DO::getCharToString(uchar *str)
{
//    char * valueChar = NULL;
//    while(*str != '\0')
//        *valueChar++ = *str++;
//    QString data(valueChar);
//    return tr(data.toLatin1());
    QString data((char *)str);
    return data;
}

/*
 * 把从配置中得到的颜色配置值转换为界面label显示的样式字符串返回
*/
QString CfgConfig_DO::getDODisplayColorValueObjStr(quint32 x)
{
    QColor colorRGBx(COLOR_RED(x), COLOR_GREEN(x), COLOR_BLUE(x));
    QString str = QString("QLabel {background-color: %1}").arg(colorRGBx.name());
    return str;
}

//把QString类型转化为uchar *类型
void CfgConfig_DO::setStrToChar(uchar * uData, QString str, int len)
{
    memset(uData, 0, len);
    QByteArray baUnit = str.toLocal8Bit();
    char* valueUnit = baUnit.data();
//    memcpy(uData, valueUnit, strlen(valueUnit));
    while (*valueUnit != '\0') {
        *uData++ = *valueUnit++;
    }
}


