/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：Timer.h
 * 概   要：无纸记录仪系统定时器、匹配定时器类, 定时刷新，并根据当前系统配置，
 * 生成相应的定时器事件、匹配定时器事件
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
#ifndef TIMER_H
#define TIMER_H

#include <QObject>
#include <QTimer>

#include "../GlobalData/cfgGlobalDef.h"

class Timer : public QObject
{
    Q_OBJECT
public:
    static Timer *instance();

private:
    explicit Timer(QObject *parent = 0);

signals:
    void sigTimerTrigger(int i);                //定时器i(1-4)触发信号
    void sigTimerMatchTrigger(int i);     //匹配定时器i(1-4)触发信号

public slots:
    void slotTimerConfigChanged();      //与系统定时器配置相关的修改信号连接
    void slotSecTimerHandle();

    void slotRelativeTimerReset();         //相对时间定时器复位

private:
    TIMER_CONFIG    m_timerConfig;      //系统定时器相关配置
    QTimer  m_timerSec;                           //秒定时器
    qint64 m_timer[4];
    qint64 m_timerMatch[4];
};

#endif // TIMER_H
