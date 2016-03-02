#ifndef WINMEASURECONFIG_H
#define WINMEASURECONFIG_H

#include "winchannelselector.h"
#include "../GlobalData/configtreeobject.h"
#include "../GlobalData/cfgGlobalDef.h"

class WinDatasaveFileStr : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinDatasaveFileStr *instance();

private:
    WinDatasaveFileStr(QWidget *parent = 0);
    void reloadMenu();

private:
    QMap<quint32,QString> m_map_DatasaveFileStr;
};

class WinDatasaveFormatData : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinDatasaveFormatData *instance();

private:
    WinDatasaveFormatData(QWidget *parent = 0);
    void reloadMenu();

private:
    QMap<quint32,QString> m_map_DatasaveFormatData;
};

class WinMeasureInterval : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinMeasureInterval *instance();

private:
    WinMeasureInterval(QWidget *parent = 0);
    void reloadMenu();

private:
    QMap<quint32,QString> m_map_MeasureInterval;
};

class WinMeasureOverRange : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinMeasureOverRange *instance();

private:
    WinMeasureOverRange(QWidget *parent = 0);
    void reloadMenu();

private:
    QMap<quint32,QString> m_map_MeasureOverRange;
};

class WinMeasureAIMode : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinMeasureAIMode *instance();

private:
    WinMeasureAIMode(QWidget *parent = 0);
    void reloadMenu();

private:
    QMap<quint32,QString> m_map_MeasureAIMode;
};

class WinMeasureADTime : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinMeasureADTime *instance();

private:
    WinMeasureADTime(QWidget *parent = 0);
    void reloadMenu();

private:
    QMap<quint32,QString> m_map_MeasureADTime;
};

class WinMeasureDIMode : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinMeasureDIMode *instance();

private:
    WinMeasureDIMode(QWidget *parent = 0);
    void reloadMenu();

private:
    QMap<quint32,QString> m_map_MeasureDIMode;
};

class WinRecordBasicModeMode : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinRecordBasicModeMode *instance();

private:
    WinRecordBasicModeMode(QWidget *parent = 0);
    void reloadMenu();

private:
    QMap<quint32,QString> m_map_BasicModeMode;
};

class WinRecordBasicDispValue : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinRecordBasicDispValue *instance();

private:
    WinRecordBasicDispValue(QWidget *parent = 0);
    void reloadMenu();
    void initMenu();

private:
    QMap<quint32,QString> m_map_BasicDispValue;
    QMap<quint32,QString> m_map;
};

class WinRecordBasicEventValue : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinRecordBasicEventValue *instance();

private:
    WinRecordBasicEventValue(QWidget *parent = 0);
    void reloadMenu();
    void initMenu();

private:
    QMap<quint32,QString> m_map;
    QMap<quint32,QString> m_map_BasicEventValue_100ms;
    QMap<quint32,QString> m_map_BasicEventValue_200ms;
    QMap<quint32,QString> m_map_BasicEventValue_500ms;
    QMap<quint32,QString> m_map_BasicEventValue_1s;
    QMap<quint32,QString> m_map_BasicEventValue_2s;
    QMap<quint32,QString> m_map_BasicEventValue_5s;
};

class WinRecordBasicEventMode : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinRecordBasicEventMode *instance();

private:
    WinRecordBasicEventMode(QWidget *parent = 0);
    void reloadMenu();

private:
    QMap<quint32,QString> m_map_BasicEventMode;
};

class WinRecordBasicEventLenth : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinRecordBasicEventLenth *instance();

private:
    WinRecordBasicEventLenth(QWidget *parent = 0);
    void reloadMenu();
    void initMenu();

private:
    QMap<quint32,QString> m_map_BasicEventLenth;
    QMap<quint32,QString> m_map;
};

class WinRecordBasicEventTrigger : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinRecordBasicEventTrigger *instance();

private:
    WinRecordBasicEventTrigger(QWidget *parent = 0);
    void reloadMenu();

private:
    QMap<quint32,QString> m_map_BasicEventTrigger;
};

class WinSecurityBasicTouchlock : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinSecurityBasicTouchlock *instance();

private:
    WinSecurityBasicTouchlock(QWidget *parent = 0);
    void reloadMenu();

private:
    QMap<quint32,QString> m_map_Touchlock;
};

class WinSecurityBasicComm : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinSecurityBasicComm *instance();

private:
    WinSecurityBasicComm(QWidget *parent = 0);
    void reloadMenu();

private:
    QMap<quint32,QString> m_map_Comm;
};

class WinSecurityBasicAuto : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinSecurityBasicAuto *instance();

private:
    WinSecurityBasicAuto(QWidget *parent = 0);
    void reloadMenu();

private:
    QMap<quint32,QString> m_map_Auto;
};

class WinSecurityProperty : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinSecurityProperty *instance();

private:
    WinSecurityProperty(QWidget *parent = 0);
    void reloadMenu();

private:
    QMap<quint32,QString> m_map_Property;
};

class WinSecurityUserNum : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinSecurityUserNum *instance();

private:
    WinSecurityUserNum(QWidget *parent = 0);
    void reloadMenu();

private:
    QMap<quint32,QString> m_map_UserNum;
};

class WinSecurityUserLvl : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinSecurityUserLvl *instance();

private:
    WinSecurityUserLvl(QWidget *parent = 0);
    void reloadMenu();
    void initMenu();

private:
    QMap<quint32,QString> m_map_UserLvl;
    QMap<quint32,QString> m_map;
};

class WinSecurityUserMode : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinSecurityUserMode *instance();

private:
    WinSecurityUserMode(QWidget *parent = 0);
    void reloadMenu();
    void initMenu();

private:
    QMap<quint32,QString> m_map_UserMode;
    QMap<quint32,QString> m_map;
};

class WinSecurityUserInit : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinSecurityUserInit *instance();

private:
    WinSecurityUserInit(QWidget *parent = 0);
    void reloadMenu();

private:
    QMap<quint32,QString> m_map_UserInit;
};

class WinSecurityAuthNum : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinSecurityAuthNum *instance();

private:
    WinSecurityAuthNum(QWidget *parent = 0);
    void reloadMenu();

private:
    QMap<quint32,QString> m_map_AuthNum;
};

#endif // WINMEASURECONFIG_H
