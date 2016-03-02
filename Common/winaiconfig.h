/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：winaiconfig.h
 * 概    要：AI-channel settings下存在item单例的所有配置项目，构建并展开选项菜单list。
 *
 * 当前版本：V1.0.0
 * 作    者：邢 俊 杰
 * 完成日期：2015-9-22
 *
 * 修改版本：
 * 修改说明：
 * 修改作者：
 *
 ********************************************************************/
#ifndef WINAICONFIG_H
#define WINAICONFIG_H

#include "winchannelselector.h"
#include "../GlobalData/configtreeobject.h"
#include "../GlobalData/cfgGlobalDef.h"
#include "./WorkWin/windialog.h"

class WinAIRangeTypeSelector : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinAIRangeTypeSelector *instance();

private:
    WinAIRangeTypeSelector(QWidget *parent = 0);
    void reloadMenu();

private:
    QMap<quint32,QString> m_map_RangeType;
};

class WinAIRangeRangeSelector : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinAIRangeRangeSelector *instance();

private:
    WinAIRangeRangeSelector(QWidget *parent = 0);
    void reloadMenu();
    void initMenu();

private:
    QVariant data;
    QMap<quint32,QString>  *map;
    QMap<quint32,QString> m_map_Volt;
    QMap<quint32,QString> m_map_GS;
    QMap<quint32,QString> m_map_TC;
    QMap<quint32,QString> m_map_RTD;
    QMap<quint32,QString> m_map_DI;
    QMap<quint32,QString> m_map_Cur0_20mA;
    QMap<quint32,QString> m_map_GS4_20mA;
};

class WinAIRangeCalculation : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinAIRangeCalculation *instance();

private:
    WinAIRangeCalculation(QWidget *parent = 0);
    void reloadMenu();
    void initMenu();

private:
    QMap<quint32,QString> m_map;
    QMap<quint32,QString> m_map_RangeCalcu_volt;
    QMap<quint32,QString> m_map_RangeCalcu_gs;
    QMap<quint32,QString> m_map_RangeCalcu_other;
};

class WinAIRangeScalePos : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinAIRangeScalePos *instance();

private:
    WinAIRangeScalePos(QWidget *parent = 0);
    void reloadMenu();

private:
    QMap<quint32,QString> m_map_RangeScalePos;
};

class WinAIRangeLowCutOutput : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinAIRangeLowCutOutput *instance();

private:
    WinAIRangeLowCutOutput(QWidget *parent = 0);
    void reloadMenu();

private:
    QMap<quint32,QString> m_map_RangeLowCutOutput;
};

class WinAIRangeRJCMode : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinAIRangeRJCMode *instance();

private:
    WinAIRangeRJCMode(QWidget *parent = 0);
    void reloadMenu();

private:
    QMap<quint32,QString> m_map_RangeRJCMode;
};

class WinAIRangeBurnoutMode : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinAIRangeBurnoutMode *instance();

private:
    WinAIRangeBurnoutMode(QWidget *parent = 0);
    void reloadMenu();

private:
    QMap<quint32,QString> m_map_RangeBurnoutMode;
};

class WinAIAlarmLvl1Type : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinAIAlarmLvl1Type *instance();

private:
    WinAIAlarmLvl1Type(QWidget *parent = 0);
    void reloadMenu();
    void initMenu();

private:
    QMap<quint32,QString> m_map_AlarmLvl1Type;
    QMap<quint32,QString> m_map_AlarmLvl1Type_Delta;
    QMap<quint32,QString> m_map;
};

class WinAIAlarmLvl1OutType : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinAIAlarmLvl1OutType *instance();

private:
    WinAIAlarmLvl1OutType(QWidget *parent = 0);
    void reloadMenu();
    void initMenu();

private:
    QMap<quint32,QString> m_map_AlarmLvl1OutType;
    QMap<quint32,QString> m_map_AlarmLvl1OutType_Both;
    QMap<quint32,QString> m_map_AlarmLvl1OutType_NoRelay;
    QMap<quint32,QString> m_map_AlarmLvl1OutType_NoSW;
    QMap<quint32,QString> m_map;
};

class WinAIAlarmDelayHour : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinAIAlarmDelayHour *instance();

private:
    WinAIAlarmDelayHour(QWidget *parent = 0);
    void reloadMenu();

private:
    QMap<quint32,QString> m_map_AlarmDelayHour;
};

class WinAIAlarmDelayMin : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinAIAlarmDelayMin *instance();

private:
    WinAIAlarmDelayMin(QWidget *parent = 0);
    void reloadMenu();

private:
    QMap<quint32,QString> m_map_AlarmDelayMin;
};

class WinAIAlarmDelaySec : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinAIAlarmDelaySec *instance();

private:
    WinAIAlarmDelaySec(QWidget *parent = 0);
    void reloadMenu();

private:
    QMap<quint32,QString> m_map_AlarmDelaySec;
};

class WinAIDisplayScalePos : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinAIDisplayScalePos *instance();

private:
    WinAIDisplayScalePos(QWidget *parent = 0);
    void reloadMenu();

private:
    QMap<quint32,QString> m_map_DisplayScalePos;
};

class WinAIDisplayScaleDiv : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinAIDisplayScaleDiv *instance();

private:
    WinAIDisplayScaleDiv(QWidget *parent = 0);
    void reloadMenu();

private:
    QMap<quint32,QString> m_map_DisplayScaleDiv;
};

class WinAIDisplayBarPos : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinAIDisplayBarPos *instance();

private:
    WinAIDisplayBarPos(QWidget *parent = 0);
    void reloadMenu();

private:
    QMap<quint32,QString> m_map_DisplayBarPos;
};

class WinAIDisplayBarDiv : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinAIDisplayBarDiv *instance();

private:
    WinAIDisplayBarDiv(QWidget *parent = 0);
    void reloadMenu();

private:
    QMap<quint32,QString> m_map_DisplayBarDiv;
};

class WinAIDisplayCsbandMode : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinAIDisplayCsbandMode *instance();

private:
    WinAIDisplayCsbandMode(QWidget *parent = 0);
    void reloadMenu();
    void initMenu();


private:
    QMap<quint32,QString> m_map_DisplayCsbandMode;
    QMap<quint32,QString> m_map_Off;
    QMap<quint32,QString> m_map;
};

class WinAIDisplayAlarmType : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinAIDisplayAlarmType *instance();

private:
    WinAIDisplayAlarmType(QWidget *parent = 0);
    void reloadMenu();

private:
    QMap<quint32,QString> m_map_DisplayAlarmType;
};

class WinAICaliCorrectMode : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinAICaliCorrectMode *instance();

private:
    WinAICaliCorrectMode(QWidget *parent = 0);
    void reloadMenu();
    void initMenu();

private:
    QMap<quint32,QString> m_map_CaliCorrectMode;
    QMap<quint32,QString> m_map_CaliCorrectMode_Off;
    QMap<quint32,QString> m_map;
};

class WinAICaliCorrectCount : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinAICaliCorrectCount *instance();

private:
    WinAICaliCorrectCount(QWidget *parent = 0);
    void reloadMenu();

private:
    QMap<quint32,QString> m_map_CaliCorrectCount;
};

//执行输入测量 按钮对话框
class WinCfgAICaliDialog : public WinDialog
{
    Q_OBJECT
public:
    explicit WinCfgAICaliDialog();

private slots:

};

#endif // WINAICONFIG_H
