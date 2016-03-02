/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：EventAction.h
 * 概   要：事件动作类，主要根据当前的系统状态、各个通道值、报警等等，决定是否
 * 触发相应的事件，如果该事件有关联的动作，则执行相应的动作。
 *
 * 依赖：配置结构体 && Modbus寄存器表
 *
 * 当前版本：V1.0.0
 * 作   者：王 品
 * 完成日期：2015-8-27
 *
 * 修改版本：
 * 修改说明：
 * 修改作者：
 *
 ********************************************************************/
#ifndef EVENTACTION_H
#define EVENTACTION_H

#include <QObject>
#include <QBitArray>
#include <QTimer>
#include <QList>
#include <QDebug>
#include <QVector>

#include "./GlobalData/cfgGlobalDef.h"

#include "../Threads/threaddatarecord.h"

typedef enum Trigger_Type_Enum {
    Trigger_Type_Raise = 0,     //上升沿触发
    Trigger_Type_Falling,         //下降沿触发
    Trigger_Type_Edge,           //边缘触发
}Trigger_Type;

typedef struct EVENT_OBJECT_STRUCT {
    Event_Type                     type;
    quint32                           number;
    quint8                             alarmNumber;
    Device_Status_Type       status;
    Operation_Mode_Type  opeMode;
}EVENT_OBJECT;

typedef struct ACTION_OBJECT_STRUCT {
    quint8              typeClass;      //动作类型区分，1:type1有效；2:type2有效
    Action_Type     type1;
    Action_Type2   type2;
    quint32            number;       //解除报警：通道编码(0-所有报警)
    Action_Detail   detail;            //动作细节描述
    quint32            msgGroup;    //解除报警：电平号

    Trigger_Type    trigType;       //事件触发类型
}ACTION_OBJECT;

class EventAction : public QObject
{
    Q_OBJECT
public:
    static EventAction *instance();
    void start() { qDebug()<<"Event Action SubSystem started..."; }
    void setInterFlag(int index, bool val);                           //内部标志设置
    quint8 getInterFlag(int index);                                      //内部标志获取

    void setInterSW(int index, bool val);                            //内部开关设置，手动/报警
    void setRelayDO(quint32 chanCoder, bool val);         //继电器输出设置
    void genEvent_interSW();                                               //内部开关事件生成
    void genEvent_remoteDI();                                            //远程控制输入事件生成
    void genEvent_relayDO();                                              //继电器事件生成
    void genEvent_alarmIO();                                              //输入通道报警事件生成
    void genEvent_alarmMath();                                         //运算通道报警事件生成
    void genEvent_alarmComm();                                       //通信通道报警事件生成
    void genEvent_alarmAll();                                              //所有报警事件生成

    void ackAlarmAll();                                                         //解除所有报警
    void ackAlarmSingle(quint32 chanCoder, quint8 levelNum);  //单个报警ACK
    void raiseAlarm(quint32 chanCoder, quint8 levelNum);    //报警发生
    void releaseAlarm(quint32 chanCoder, quint8 levelNum); //报警释放
    SQL_Alarm_Status getAlarmStatus(quint32 chanCoder, quint8 levelNum);

private:
    explicit EventAction(QObject *parent = 0);
    void resetBitArray();
    void resetInterSW();
    void resetChanDO();
    void resetChanAlarmIO();
    void resetChanAlarmMath();
    void resetChanAlarmComm();

    bool getIndexofAlarmStatus(quint32 chanCoder, quint32 *index);

    void initAlarmData(quint32 chanCoder, quint32 level, SQL_Alarm_Status status, ALARM_DATA *pData);

private:
    void execAction(ACTION_OBJECT &action);

signals:

public slots:
    void slotEventConfigChanged();                     //事件配置变更处理槽函数

    void slotSysTimerHandle(int index);             //系统定时器事件处理
    void slotSysMatchTimerHandle(int index);   //系统匹配定时器事件处理
    void slotUserFunc(int index);                         //用户功能键事件处理

protected:
    bool event(QEvent *event);

private:
    QBitArray   *p_InterFlag;                                           //内部标志F01~F20

    QList<quint32>   m_eventInterSW;                           //内部开关事件索引链表
    QBitArray   *p_InterSW_cur, *p_InterSW_pre;         //内部开关

    QList<quint32>   m_eventRemoteDI;                       //远程控制输入事件索引链表
    QBitArray   *p_RemoteDI_pre;                                 //远程控制输入DI

    QList<quint32>   m_eventRelayDO;                         //继电器事件索引链表
    QBitArray   *p_RelayDO_cur, *p_RelayDO_pre;      //继电器

    QVector<SQL_Alarm_Status>   m_alarmStatus;      //IO/MATH/COMM通道报警状态, 依次为AI、DI、MATH、COMM

    QList<quint32>   m_eventAlarmIO;                         //IO通道报警事件索引链表
    QBitArray   *p_AlarmIO_pre;                                   //IO通道报警(DI/AI, 不包括DO)

    QList<quint32>   m_eventAlarmMath;                    //运算通道报警事件索引链表
    QBitArray   *p_AlarmMath_pre;                               //运算通道报警

    QList<quint32>   m_eventAlarmComm;                  //通信通道报警事件索引链表
    QBitArray   *p_AlarmComm_pre;                            //通信通道报警

    QList<quint32>   m_eventAlarmAll;                        //所有报警事件索引链表
    bool   m_allAlarm_pre;                                             //所有通道报警

    QList<quint32>   m_eventTimer;                            //计时器事件索引链表
    QList<quint32>   m_eventMatchTimer;                 //匹配计时器事件索引链表
    QList<quint32>   m_eventUserFunc;                      //用户功能事件索引链表

    QList<quint32>   m_eventDevStaus;                      //仪表状态事件索引链表

    EVENT_OBJECT    m_eventObj[EVENT_ACTION_NUMBER];        //事件对象
    ACTION_OBJECT  m_actionObj[EVENT_ACTION_NUMBER];      //动作对象
};

#endif // EVENTACTION_H
