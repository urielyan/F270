#ifndef DATAGLOBALDEF_RECORD_H
#define DATAGLOBALDEF_RECORD_H

#include <QtGlobal>

#include "cfgGlobalDef_Type.h"
#include "sqlBlock_GroupInfo.h"
#include "sqlBlock_RecordChanInfo.h"
#include "sqlBlock_sysConfig.h"

//显示数据单点数据结构
typedef struct DISPLAY_DATA_STRUCT {
    qreal   Max;
    qreal   Min;
    qint16 status;
}DISPLAY_DATA;

typedef struct EVENT_DATA_STRUCT {
    quint8 Max;
    quint8 Min;
    qint16 status;
}EVENT_DATA;

//所有通道显示数据结构体
typedef struct DISPLAY_DATA_RECORD_STRUCT {
    //本机AI类型通道数据及状态
    DISPLAY_DATA mainUnit_AI_Data[MAIN_UNIT_MODULE_NUM][UNIT_CHAN_NUM_PRE];
    //扩展AI类型通道数据及状态
    DISPLAY_DATA extUnit_AI_Data[EXT_UNIT_NUM][EXT_UNIT_MODULE_NUM][UNIT_CHAN_NUM_PRE];
    //本机DI类型通道数据及状态
    DISPLAY_DATA mainUnit_DI_Data[MAIN_UNIT_MODULE_NUM][UNIT_CHAN_NUM_PRE];
    //扩展DI类型通道数据及状态
    DISPLAY_DATA extUnit_DI_Data[EXT_UNIT_NUM][EXT_UNIT_MODULE_NUM][UNIT_CHAN_NUM_PRE];
    //本机DO类型通道数据及状态
    DISPLAY_DATA mainUnit_DO_Data[MAIN_UNIT_MODULE_NUM][UNIT_CHAN_NUM_PRE];
    //扩展DO类型通道数据及状态
    DISPLAY_DATA extUnit_DO_Data[EXT_UNIT_NUM][EXT_UNIT_MODULE_NUM][UNIT_CHAN_NUM_PRE];
    //MATH通道数据及状态
    DISPLAY_DATA device_Math_Data[MAIN_UNIT_MATH_CHAN_NUM_PRE];
    //COMM通道数据及状态
    DISPLAY_DATA device_Comm_Data[MAIN_UNIT_COMM_CHAN_NUM_PRE];

//    quint64 screenDivs;       //趋势水平个数，20/12, 20：30点/格， 12：50点/格
    quint32 trendPeriodMax;
    quint32 trendPeriodMin;

    qint64 recordInterval;   //记录间隔, 单位：ms
    quint64 dataCount;      //同次记录过程中，显示数据记录个数
    qint64 dataStartTime;  //记录开始时间
    bool    stopFlag;            //记录停止标记
}DISPLAY_DATA_RECORD;

//所有通道事件数据结构体
typedef struct EVENT_DATA_RECORD_STRUCT {
    //本机AI类型通道数据及状态
    EVENT_DATA mainUnit_AI_Data[MAIN_UNIT_MODULE_NUM][UNIT_CHAN_NUM_PRE];
    //扩展AI类型通道数据及状态
    EVENT_DATA extUnit_AI_Data[EXT_UNIT_NUM][EXT_UNIT_MODULE_NUM][UNIT_CHAN_NUM_PRE];
    //本机DI类型通道数据及状态
    EVENT_DATA mainUnit_DI_Data[MAIN_UNIT_MODULE_NUM][UNIT_CHAN_NUM_PRE];
    //扩展DI类型通道数据及状态
    EVENT_DATA extUnit_DI_Data[EXT_UNIT_NUM][EXT_UNIT_MODULE_NUM][UNIT_CHAN_NUM_PRE];
    //本机DO类型通道数据及状态
    EVENT_DATA mainUnit_DO_Data[MAIN_UNIT_MODULE_NUM][UNIT_CHAN_NUM_PRE];
    //扩展DO类型通道数据及状态
    EVENT_DATA extUnit_DO_Data[EXT_UNIT_NUM][EXT_UNIT_MODULE_NUM][UNIT_CHAN_NUM_PRE];
    //MATH通道数据及状态
    EVENT_DATA device_Math_Data[MAIN_UNIT_MATH_CHAN_NUM_PRE];
    //COMM通道数据及状态
    EVENT_DATA device_Comm_Data[MAIN_UNIT_COMM_CHAN_NUM_PRE];

//    quint64 screenDivs;       //趋势水平个数，20/12, 20：30点/格， 12：50点/格
    quint32 trendPeriodMax;
    quint32 trendPeriodMin;

    quint64 recordInterval;   //记录间隔, 单位：ms
    quint64 dataCount;      //同次记录过程中，事件数据记录个数
    qint64 dataStartTime;  //记录开始时间
    bool    stopFlag;            //记录停止标记
}EVENT_DATA_RECORD;

typedef struct TREND_PAINT_INFO_STRUCT {
    qint64 recordIndex;                                     //当前缓冲数据对应的记录索引号
    qint64 recordStartTime;                              //当前缓冲数据对应的记录开始时间，第一个数据的时间点
    qint64 recordTimeGap;                               //当前缓冲数据点之间的时间间隔
    quint64 recordTimeTag;                                //当前缓冲数据对应的时间标记
    qint32 rightPointIndex;                               //历史浏览模式下，屏幕最右侧数据点在缓冲区中的索引位置（实时模式为最后个）
//    qint32 screenHorDiv;                                   //屏幕水平分割数，12（50 points/DIV）/20(30 points/DIV)
    quint32 trendPeriodMin;
    quint32 trendPeriodMax;
    quint32 trendPeriodTarget;
    SQL_GROUP_INFO sql_group;                     //当前缓冲数据对应的记录组信息块
    GROUP_INFO group_info[50];                     //当前缓冲数据对应的记录组详细信息块
    SQL_RECORD_CHAN_INFO sql_channel;    //当前缓冲数据对应的记录通道信息块
    CHAN_INFO chan_info[500];                       //当前缓冲数据对应的记录通道详细信息块
    SQL_SYS_CONFIG sys_config;                      //当前缓冲数据对应的记录文件配置块
    bool    isDisplayData;                                   //表示该缓冲数据对应的记录索引号来自“显示数据表”、“事件数据表”
    bool    isHeadIn;                                           //表示是否包含本记录的开始数据点
    bool    isTailIn;                                              //表示是否包含本记录的结束数据点
}TREND_PAINT_INFO;

#endif // DATAGLOBALDEF_RECORD_H
