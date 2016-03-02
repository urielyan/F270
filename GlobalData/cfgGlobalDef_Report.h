/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：CfgGlobbalDef_Report.h
 * 概   要： 报表配置相关数据结构、枚举值定义
 *
 * 当前版本：V1.0.0
 * 作   者：王 品
 * 完成日期：2015-6-8
 *
 * 修改版本：
 * 修改说明：
 * 修改作者：
 *
 ********************************************************************/
#include <QtGlobal>

#include "cfgGlobalDef_Type.h"

#ifndef CFGGLOBALDEF_REPORT_H
#define CFGGLOBALDEF_REPORT_H

#define REPORT_CHAN_NUM 60

typedef enum  Report_Type_Enum {
    Report_Type_Off = 0,                        //不输出报表
    Report_Type_Hourly_Daily,              //时报+日报
    Report_Type_Daily_Weekly,             //日报+周报
    Report_Type_Daily_Monthly,           //日报+月报
    Report_Type_Batch,                          //批处理报
    Report_Type_Daily_Custom,             //以指定时间间隔分割日报，创建自定义日报
}Report_Type;

typedef enum  Report_Week_Day_Enum {
    Report_Week_Day_Sun = 0,
    Report_Week_Day_Mon,
    Report_Week_Day_Tues,
    Report_Week_Day_Wed,
    Report_Week_Day_Thur,
    Report_Week_Day_Fri,
    Report_Week_Day_Sat,
}Report_Week_Day;

typedef enum  Report_Data_Type_Enum {
    Report_Data_Type_AVG = 0,               //平均值
    Report_Data_Type_MAX,                    //最大值
    Report_Data_Type_MIN,                     //最小值
    Report_Data_Type_SUM,                    //累计值
    Report_Data_Type_INST,                    //瞬时值
}Report_Data_Type;

typedef enum  Report_Sum_Unit_Enum{
    Report_Sum_Unit_Off = 0,                    //不使用
    Report_Sum_Unit_S,                             // /s
    Report_Sum_Unit_Min,                         // /min
    Report_Sum_Unit_H,                            // /h
    Report_Sum_Unit_Day,                        // /day
}Report_Sum_Unit;

//报表数据结构体定义
typedef struct REPORT_DATA_RECORD_STRUCT {
    qreal   reportAve[REPORT_CHAN_NUM];
    qreal   reportSum[REPORT_CHAN_NUM];
    qreal   reportMax[REPORT_CHAN_NUM];
    qreal   reportMin[REPORT_CHAN_NUM];
    qreal   reportReal[REPORT_CHAN_NUM];

    quint8  reportStatus[REPORT_CHAN_NUM];
    quint8  reportUnit[REPORT_CHAN_NUM][8];

    quint64 recordInterval;   //记录间隔,单位ms
    quint64 dataCount;      //同次记录过程中，显示数据记录个数
    qint64 dataStartTime;  //记录开始时间
    quint64 dataSingle;     //单次数据报数据记录个数
    int lastTime;         //上次同步时间，表示：整时（时报+日报）/星期*（日报+周报）/日（日报+月报）
    bool    stopFlag;            //记录停止标记
}REPORT_DATA_RECORD;

typedef struct REPORT_DATA_STORE_STRUCT {
    qreal reportData[5];
    quint8 reportStatus;
    quint8 reportUnit[8];
}REPORT_DATA_STORE;

/****************************************
 * 报表配置相关数据结构体定义
 ****************************************/
typedef struct REPORT_CONFIG_STRUCT
{
    /*基本配置*/
    Report_Type reportType;                 //报表类型

    //5个数据类型
    Report_Data_Type    reportDataType[5];  //报表1-5数据类型

    //创建时间
    quint8 reportDay;                          //日：1~28
    quint8 reportWeek;                       //星期几：1~7
    quint8 reportHourly;                     //时：0~23
    quint8 reportMinute;                     //分：0~59
    quint8 reportPeriod;                      //保存周期，单位:min, 范围：2min/3min/4min/5min/10min/15min/30min/1h
    quint8 reportFileCreatePeriod;     //文件创建周期，单位:h，范围：4h/6h/8h/12h/24h

    //创建文件
    quint8 reportFile;                             //创建文件类型：0-单个，1-多个，默认：0

    //报表输出
    quint8 reportExcel;                        //Excel文件输出开关，0-off, 1-on
    quint8 reportPDF;                          //PDF文件输出开关，0-off, 1-on
    quint8 reportPrinter;                     //打印机输出，0-off, 1-on

    //PDF电子签名
    quint8 reportElecSignature;         //PDF电子签名开关，0-off, 1-on
    quint8 RESERVED;

    /*通道配置*/
    //60个报表输出通道
    Channel_Type reportChannelType[REPORT_CHAN_NUM];      //通道类型
    quint32   reportChannelNum[REPORT_CHAN_NUM];              //通道编码
    Report_Sum_Unit reportSumUnit[REPORT_CHAN_NUM];        //累计单位
}REPORT_CONFIG ;
#endif // CFGGLOBALDEF_REPORT_H
