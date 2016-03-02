/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：Timer.cpp
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
#include "timer.h"
#include "../GlobalData/globalDef.h"

#include <QDateTime>

Timer *Timer::instance()
{
    static Timer data;
    return &data;
}

/*
 * 功能：
 * 	  无纸记录仪系统定时器、匹配定时器类构造函数
 * NOTE：
 *       依赖于系统配置中定时器相关配置初始化
 */
Timer::Timer(QObject *parent) :
    QObject(parent)
{
    m_timerSec.setInterval(1000);
    connect(&m_timerSec, SIGNAL(timeout()), this, SLOT(slotSecTimerHandle()));

    slotTimerConfigChanged();
}

/**
 * @brief Timer::slotTimerConfigChanged
 *       定时器/匹配定时器配置变更处理槽函数，Timer settings内任何配置变更时，
 * 触发该槽函数
 */
void Timer::slotTimerConfigChanged()
{
    if (m_timerSec.isActive())
        m_timerSec.stop();

    ::memcpy(&m_timerConfig, &DevConfigPtr->timerConfig, sizeof(TIMER_CONFIG));

    quint64 curTime = QDateTime::currentMSecsSinceEpoch();
    for (int i=0; i<4; ++i) {
        m_timer[i] = curTime;
        m_timerMatch[i] = curTime;
    }

    m_timerSec.start();
}

/*
 * 功能：
 * 	  秒定时器定时处理槽函数，根据系统定时器相关配置，决定是否生成
 * 相应的定时事件、时间匹配事件
 */
void Timer::slotSecTimerHandle()
{
    quint64 curTime = QDateTime::currentMSecsSinceEpoch();
    quint64 timerInter;
    int timerMinCur, timerMinSet, timerMinDiff, timerMinInter;

    timerMinCur = QTime::currentTime().hour() * 60;
    timerMinCur += QTime::currentTime().minute();    //当前的绝对时间：hour-min

    for (int i=0; i<4; ++i) {
        //定时器
        switch (m_timerConfig.timerType[i]) {
        case Timer_Type_Relative:
            timerInter = m_timerConfig.timerDay[i] * 86400000;
            timerInter += m_timerConfig.timerHour[i] * 3600000;
            timerInter += m_timerConfig.timerMin[i] * 60000;
            if ((curTime - m_timer[i]) > timerInter) {
                m_timer[i] = curTime;
                emit sigTimerTrigger(i);
            }
            break;
        case Timer_Type_Absolute:
            timerMinInter = m_timerConfig.timerInter[i];        //时间间隔，单位:min
            timerMinSet = m_timerConfig.timerHour[i] * 60;
            timerMinSet += m_timerConfig.timerMin[i];           //设定的绝对时间：hour-min
            timerMinDiff = timerMinCur - timerMinSet;
            if (0 == (timerMinDiff % timerMinInter) && \
                m_timer[i] != timerMinCur) {
                m_timer[i] = timerMinCur;
                emit sigTimerTrigger(i);
            }
            break;
        default:
            break;
        }

        int month, day, weekday, hour, minute;
        month = QDate::currentDate().month();   //1~12
        weekday = QDate::currentDate().dayOfWeek(); //1~7
        day = QDate::currentDate().day(); //1~31
        hour = QTime::currentTime().hour(); //0~23
        minute = QTime::currentTime().minute(); //0~59
        //匹配定时器
        switch (m_timerConfig.timerMatchType[i]) {
        case Timer_Match_Type_Daily:
            if (m_timerConfig.timerMatchHour[i] == hour && \
                m_timerConfig.timerMatchMinute[i] == minute) {  //时间匹配
                if (m_timerMatch[i] != timerMinCur) {
                    m_timerMatch[i] = timerMinCur;

                    if (m_timerConfig.timerMatchAction[i] == 0) {             //单次
                        m_timerConfig.timerMatchAction[i] = 2;
                        emit sigTimerMatchTrigger(i);
                    } else if (m_timerConfig.timerMatchAction[i] == 1) {   //循环
                        emit sigTimerMatchTrigger(i);
                    }
                }
            }
            break;
        case Timer_Match_Type_Weekly:
            if (m_timerConfig.timerMatchWeekDay[i] == weekday && \
                m_timerConfig.timerMatchHour[i] == hour && \
                m_timerConfig.timerMatchMinute[i] == minute) {  //时间匹配
                if (m_timerMatch[i] != timerMinCur) {
                    m_timerMatch[i] = timerMinCur;

                    if (m_timerConfig.timerMatchAction[i] == 0) {             //单次
                        m_timerConfig.timerMatchAction[i] = 2;
                        emit sigTimerMatchTrigger(i);
                    } else if (m_timerConfig.timerMatchAction[i] == 1) {   //循环
                        emit sigTimerMatchTrigger(i);
                    }
                }
            }
            break;
        case Timer_Match_Type_Monthly:
            if (m_timerConfig.timerMatchDay[i] == day && \
                m_timerConfig.timerMatchHour[i] == hour && \
                m_timerConfig.timerMatchMinute[i] == minute) {  //时间匹配
                if (m_timerMatch[i] != timerMinCur) {
                    m_timerMatch[i] = timerMinCur;

                    if (m_timerConfig.timerMatchAction[i] == 0) {             //单次
                        m_timerConfig.timerMatchAction[i] = 2;
                        emit sigTimerMatchTrigger(i);
                    } else if (m_timerConfig.timerMatchAction[i] == 1) {   //循环
                        emit sigTimerMatchTrigger(i);
                    }
                }
            }
            break;
        case Timer_Match_Type_Yearly:
            if (m_timerConfig.timerMatchMonth[i] == month && \
                m_timerConfig.timerMatchDay[i] == day && \
                m_timerConfig.timerMatchHour[i] == hour && \
                m_timerConfig.timerMatchMinute[i] == minute) {  //时间匹配
                if (m_timerMatch[i] != timerMinCur) {
                    m_timerMatch[i] = timerMinCur;

                    if (m_timerConfig.timerMatchAction[i] == 0) {             //单次
                        m_timerConfig.timerMatchAction[i] = 2;
                        emit sigTimerMatchTrigger(i);
                    } else if (m_timerConfig.timerMatchAction[i] == 1) {   //循环
                        emit sigTimerMatchTrigger(i);
                    }
                }
            }
            break;
        default:
            break;
        }
    }
}

/**
 * @brief 相对时间定时器复位
 */
void Timer::slotRelativeTimerReset()
{
    for (int i=0; i<4; ++i) {
        if (Timer_Type_Relative == m_timerConfig.timerType[i]) {
            m_timer[i] = QDateTime::currentMSecsSinceEpoch();
        }
    }
}
