#ifndef CFGCONFIG_TIMER_H
#define CFGCONFIG_TIMER_H

#include <QObject>
#include "./GlobalData/configobject.h"
#include "./GlobalData/cfgGlobalDef_Timer.h"

/****************************************  timer 22100 - 22300  *************************************************/
#define Timer_Start_ID                              22100
#define Timer_End_ID                                22299

#define Timer_Timer_Start_ID                        22100
#define Timer_Timer_End_ID                          22189

#define Timer_Match_Start_ID                        22190
#define Timer_match_End_ID                          22299

//timer
#define ID_Timer                                    22100

//timer timer
#define ID_Timer_Timer                              22101   //group
#define ID_Timer_Timer_Referencetime                22102   //group   timer 1-4 当类型选absolute timer时显示的一个组Reference time

#define ID_Timer_Timer_1                            22110   //title
#define ID_Timer_Timer_1_Type                       22111
#define ID_Timer_Timer_1_Type_Type                  22112    //计时器1类型

#define ID_Timer_Timer_1_Interval                   22120   //group
#define ID_Timer_Timer_1_Interval_D                 22121    //计时器1天数间隔，0-31
#define ID_Timer_Timer_1_Interval_H                 22122     //计时器1小时间隔，0-24
#define ID_Timer_Timer_1_Interval_M                 22123    //计时器1分钟间隔，0-60
#define ID_Timer_Timer_1_Interval_Interval          22124    //时间间隔，绝对时间下使用，单位:min (1min - 24h)

#define ID_Timer_Timer_2                            22130
#define ID_Timer_Timer_2_Type                       22131
#define ID_Timer_Timer_2_Type_Type                  22132

#define ID_Timer_Timer_2_Interval                   22140
#define ID_Timer_Timer_2_Interval_D                 22141
#define ID_Timer_Timer_2_Interval_H                 22142
#define ID_Timer_Timer_2_Interval_M                 22143
#define ID_Timer_Timer_2_Interval_Interval          22144

#define ID_Timer_Timer_3                            22150
#define ID_Timer_Timer_3_Type                       22151
#define ID_Timer_Timer_3_Type_Type                  22152

#define ID_Timer_Timer_3_Interval                   22160
#define ID_Timer_Timer_3_Interval_D                 22161
#define ID_Timer_Timer_3_Interval_H                 22162
#define ID_Timer_Timer_3_Interval_M                 22163
#define ID_Timer_Timer_3_Interval_Interval          22164

#define ID_Timer_Timer_4                            22170
#define ID_Timer_Timer_4_Type                       22171
#define ID_Timer_Timer_4_Type_Type                  22172

#define ID_Timer_Timer_4_Interval                   22180
#define ID_Timer_Timer_4_Interval_D                 22181
#define ID_Timer_Timer_4_Interval_H                 22182
#define ID_Timer_Timer_4_Interval_M                 22183
#define ID_Timer_Timer_4_Interval_Interval          22184

//match time timer
#define ID_Timer_Match                              22190
#define ID_Timer_Match_Action                       22191   //match time timer 1-4公用

#define ID_Timer_Match_1                            22200   //title
#define ID_Timer_Match_1_Type                       22201   //group
#define ID_Timer_Match_1_Type_Type                  22202   //匹配计时器1类型

#define ID_Timer_Match_1_Condition                  22210
#define ID_Timer_Match_1_Condition_M                22211   //匹配计时器1-4匹配月，范围:1~12
#define ID_Timer_Match_1_Condition_D                22212    //匹配计时器1-4匹配天，范围：1~31
#define ID_Timer_Match_1_Condition_W                22213     //匹配计时器1-4匹配星期几，范围：1~7
#define ID_Timer_Match_1_Condition_H                22214   //匹配计时器1-4匹配小时，范围：0~23
#define ID_Timer_Match_1_Condition_Min              22215   //匹配计时器1-4匹配分钟，范围：0~59
#define ID_Timer_Match_1_Action                     22216   //匹配计时器1-4动作，0-单次， 1-循环，其它：单次下已执行


#define ID_Timer_Match_2                            22220   //title
#define ID_Timer_Match_2_Type                       22221
#define ID_Timer_Match_2_Type_Type                  22222

#define ID_Timer_Match_2_Condition                  22230
#define ID_Timer_Match_2_Condition_M                22231
#define ID_Timer_Match_2_Condition_D                22232
#define ID_Timer_Match_2_Condition_W                22233
#define ID_Timer_Match_2_Condition_H                22234
#define ID_Timer_Match_2_Condition_Min              22235
#define ID_Timer_Match_2_Action                     22236


#define ID_Timer_Match_3                            22240   //title
#define ID_Timer_Match_3_Type                       22241
#define ID_Timer_Match_3_Type_Type                  22242

#define ID_Timer_Match_3_Condition                  22250
#define ID_Timer_Match_3_Condition_M                22251
#define ID_Timer_Match_3_Condition_D                22252
#define ID_Timer_Match_3_Condition_W                22253
#define ID_Timer_Match_3_Condition_H                22254
#define ID_Timer_Match_3_Condition_Min              22255
#define ID_Timer_Match_3_Action                     22256


#define ID_Timer_Match_4                            22260   //title
#define ID_Timer_Match_4_Type                       22261
#define ID_Timer_Match_4_Type_Type                  22262

#define ID_Timer_Match_4_Condition                  22270
#define ID_Timer_Match_4_Condition_M                22271
#define ID_Timer_Match_4_Condition_D                22272
#define ID_Timer_Match_4_Condition_W                22273
#define ID_Timer_Match_4_Condition_H                22274
#define ID_Timer_Match_4_Condition_Min              22275
#define ID_Timer_Match_4_Action                     22276

class CfgConfig_Timer : public QObject
{
    Q_OBJECT
public:
    static CfgConfig_Timer * instance();

    QVariant getTimerData(quint32 ID, quint32 group);
    void setTimerData(quint32 ID, quint32 group);

    QString getTimerObjStr(quint32 ID, quint32 group = 0);     //第二个参数先保留，目前没有用到
    QString getTimerObjValStr(quint32 ID, quint32 group, ConfigObject *cfg);

    void setDeviceTimerConfig(TIMER_CONFIG  &timerConfig) {p_timerConfig = &timerConfig;}

private:
    explicit CfgConfig_Timer(QObject *parent = 0);

    QVariant getTimerTimerData(quint32 ID, quint32 group);
    QVariant getTimerMatchData(quint32 ID, quint32 group);

    QString getTimerTimerObjStr(quint32 ID, quint32 group = 0);
    QString getTimerMatchObjStr(quint32 ID, quint32 group = 0);

    QString getTimerTimerObjValStr(quint32 ID, quint32 group, ConfigObject * cfg);
    QString getTimerMatchObjValStr(quint32 ID, quint32 group, ConfigObject * cfg);

    QString getTimerWeekObjValStr(int type);

    void setTimerTimerData(quint32 ID, quint32 group);
    void setTimerMatchData(quint32 ID, quint32 group);

signals:

public slots:

private:
    TIMER_CONFIG *                          p_timerConfig;

};

#endif // CFGCONFIG_TIMER_H
