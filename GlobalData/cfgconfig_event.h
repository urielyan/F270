#ifndef CFGCONFIG_EVENT_H
#define CFGCONFIG_EVENT_H

#include <QObject>
#include "./GlobalData/cfgGlobalDef_EventAction.h"
#include "./GlobalData/configobject.h"

/****************************************  event 22300 - 22400  *************************************************/
#define Event_Start_ID                                 22300
#define Event_End_ID                                   22399


#define ID_Event                                        22300
#define ID_Event_Num                                    22301

#define ID_Event_EventAction                            22310
#define ID_Event_EventAction_Switch                     22311   //事件动作开关，0-off, 1-on

#define ID_Event_Event                                  22320
#define ID_Event_Event_Type                             22321       //事件动作类型
#define ID_Event_Event_Num                              22322       //事件动作类型对应的号码，【所有报警】时无效
#define ID_Event_Event_AlarmNum                         22323       //报警号，范围：1~4，【报警-IO/报警-Math/报警-Comm】时有效
#define ID_Event_Event_AlarmStatus                      22324       //仪表状态类型，在【仪表状态】类型时有效
#define ID_Event_Event_Mode                             22325       //操作模式

#define ID_Event_Action                                 22330
#define ID_Event_Action_Type                            22331       //动作类型, 操作模式：上升沿\下降沿\边缘时按Action_Type
#define ID_Event_Action_Num                             22332        //动作号码，具体范围依赖于动作类型
#define ID_Event_Action_Detail                          22333       //动作细节，具体可分配值依赖于动作类型
#define ID_Event_Action_GrpNum                          22334        //组号码，【信息】类型且详细信息为【指定组】时有效


class CfgConfig_Event : public QObject
{
    Q_OBJECT
public:
    static CfgConfig_Event * instance();

    QVariant getEventData(quint32 ID, quint32 group);
    void setEventData(quint32 ID, quint32 group);

    QString getEventObjStr(quint32 ID, quint32 group = 0);
    QString getEventObjValStr(quint32 ID, quint32 group, ConfigObject *cfg);

    void setDeviceEventConfig(EVENT_ACTION_CONFIG & eventConfig) { p_eventConfig = &eventConfig;}


private:
    explicit CfgConfig_Event(QObject *parent = 0);

    QString getEventNumObjValStr(quint32 num, Event_Type type);

    QString getEventTypeObjValStr(quint32 type);
    QString getEventStatusObjValStr(quint32 type);
    QString getEventOperationObjValStr(quint32 type);
    QString getEventActionTypeObjValStr(quint32 type);
    QString getEventDetailTypeObjValStr(quint32 type);

signals:

public slots:

private:
    EVENT_ACTION_CONFIG *               p_eventConfig;
};

#endif // CFGCONFIG_EVENT_H
