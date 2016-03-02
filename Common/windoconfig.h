/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：windoconfig.h
 * 概    要：DO-channel settings下存在item单例的所有配置项目，构建并展开选项菜单list。
 *
 * 当前版本：V1.0.0
 * 作    者：邢 俊 杰
 * 完成日期：2015-10-28
 *
 * 修改版本：
 * 修改说明：
 * 修改作者：
 *
 ********************************************************************/
#ifndef WINDOCONFIG_H
#define WINDOCONFIG_H

#include "winchannelselector.h"
#include "../GlobalData/configtreeobject.h"
#include "../GlobalData/cfgGlobalDef.h"


class WinDORangeRangeType : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinDORangeRangeType *instance();

private:
    WinDORangeRangeType(QWidget *parent = 0);
    void reloadMenu();

private:
    QMap<quint32,QString> m_map_RangeRangeType;
};

class WinDORangeRangeRange : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinDORangeRangeRange *instance();

private:
    WinDORangeRangeRange(QWidget *parent = 0);
    void reloadMenu();

private:
    QMap<quint32,QString> m_map_RangeRangeRange;
};

class WinDORangeActionEnergize : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinDORangeActionEnergize *instance();

private:
    WinDORangeActionEnergize(QWidget *parent = 0);
    void reloadMenu();

private:
    QMap<quint32,QString> m_map_RangeActionEnergize;
};

class WinDORangeActionAction : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinDORangeActionAction *instance();

private:
    WinDORangeActionAction(QWidget *parent = 0);
    void reloadMenu();

private:
    QMap<quint32,QString> m_map_RangeActionAction;
};

class WinDORangeActionHold : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinDORangeActionHold *instance();

private:
    WinDORangeActionHold(QWidget *parent = 0);
    void reloadMenu();

private:
    QMap<quint32,QString> m_map_RangeActionHold;
};

class WinDORangeActionACK : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinDORangeActionACK *instance();

private:
    WinDORangeActionACK(QWidget *parent = 0);
    void reloadMenu();
    void initMenu();

private:
    QMap<quint32, QString> m_map_RangeActionACK;
    QMap<quint32, QString> m_map_Reset;
    QMap<quint32, QString> m_map;
};

class WinDORangeActionInterval : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinDORangeActionInterval *instance();

private:
    WinDORangeActionInterval(QWidget *parent = 0);
    void reloadMenu();

private:
    QMap<quint32,QString> m_map_RangeActionInterval;
};

#endif // WINDOCONFIG_H
