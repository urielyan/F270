/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：sqlTable_Alarm.h
 * 概   要：报警记录表相关数据结构声明
 *
 * 当前版本：V1.0.0
 * 作   者：王 品
 * 完成日期：2015-7-20
 *
 * 修改版本：
 * 修改说明：
 * 修改作者：
 *
 ********************************************************************/
#include <QtGlobal>
#include "cfgGlobalDef_Type.h"

#ifndef SQLTABLE_ALARM_H
#define SQLTABLE_ALARM_H

typedef struct ALARM_DATA_STRUCT {
    SQL_Alarm_Status    alarmStatus;                //报警状态
    uchar                         alarmTagChar[40];      //报警通道标记
    quint32                     alarmChanNum;          //报警通道编码
    quint32                     alarmLevelNo;              //报警电平号
    Alarm_Type              alarmType;                    //报警类型
    qint64                       alarmTime;                    //报警发生时间
}ALARM_DATA;

#endif // SQLTABLE_ALARM_H
