/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：windiconfig.h
 * 概    要：DI-channel settings下存在item单例的所有配置项目，构建并展开选项菜单list。
 *
 * 当前版本：V1.0.0
 * 作    者：邢 俊 杰
 * 完成日期：2015-11-03
 *
 * 修改版本：
 * 修改说明：
 * 修改作者：
 *
 ********************************************************************/
#ifndef WINDICONFIG_H
#define WINDICONFIG_H

#include "winchannelselector.h"
#include "../GlobalData/configtreeobject.h"
#include "../GlobalData/cfgGlobalDef.h"


class WinDIRangeRangeType : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinDIRangeRangeType *instance();

private:
    WinDIRangeRangeType(QWidget *parent = 0);
    void reloadMenu();
    void initMenu();

private:
    QMap<quint32,QString> m_map_RangeRangeType;
    QMap<quint32,QString> m_map_RangeRangeType_noPluse;
    QMap<quint32,QString> m_map;
};

class WinDIRangeRangeRange : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinDIRangeRangeRange *instance();

private:
    WinDIRangeRangeRange(QWidget *parent = 0);
    void reloadMenu();

private:
    QMap<quint32,QString> m_map_RangeRangeRange;
};


class WinDIRangeCalculation : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinDIRangeCalculation *instance();

private:
    WinDIRangeCalculation(QWidget *parent = 0);
    void reloadMenu();
    void initMenu();

private:
    QMap<quint32,QString> m_map;
    QMap<quint32,QString> m_map_RangeCalcu;
    QMap<quint32,QString> m_map_RangeCalcu_off;
};


class WinDIAlarmLvl1Type : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinDIAlarmLvl1Type *instance();

private:
    WinDIAlarmLvl1Type(QWidget *parent = 0);
    void reloadMenu();
    void initMenu();

private:
    QMap<quint32,QString> m_map_AlarmLvl1Type;
    QMap<quint32,QString> m_map_AlarmLvl1Type_Delta;
    QMap<quint32,QString> m_map;
};

#endif // WINDICONFIG_H
