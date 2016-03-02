/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：CfgGlobbalDef_Display.h
 * 概   要：画面显示条件配置相关数据结构、枚举值定义
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

#ifndef CFGGLOBALDEF_DISPLAY_H
#define CFGGLOBALDEF_DISPLAY_H

/* 趋势周期值定义， 根据测量周期不同，趋势周期最小值有限制*/
typedef enum Trend_Interval_Enum{                  //默认值:Trend_Interval_1min
    Trend_Interval_5s = 5,
    Trend_Interval_10s = 10,
    Trend_Interval_15s = 15,                            //5s, 10, 15s在使用电磁继电器/低耐压继电器方式时无效！
    Trend_Interval_30s = 30,
    Trend_Interval_1min = 60,
    Trend_Interval_2min = 120,
    Trend_Interval_5min = 300,
    Trend_Interval_10min = 600,
    Trend_Interval_15min = 900,
    Trend_Interval_20min = 1200,
    Trend_Interval_30min = 1800,
    Trend_Interval_1h = 3600,
    Trend_Interval_2h = 7200,
    Trend_Interval_4h = 14400,
    Trend_Interval_10h = 36000,
} Trend_Interval;

/****************************************
 * 趋势周期相关数据结构体定义
 ****************************************/
typedef struct DISPLAY_GONFIG_TREND_STRUCT
{
    Trend_Interval trendInterval_s;                     //趋势周期，最小值依赖于测量周期或事件记录周期
                                                                           /*
                                                                           ** 记录模式为【事件】时，趋势周期取决与事件数据的
                                                                           ** 的记录周期，具体对应关系请参考说明手册
                                                                           */
    Trend_Interval trendIntervalSecond;         //第二趋势周期
    quint8 trendIntervalSecondSwitch;             //第二趋势周期切换开关，0-off, 1-on

    quint8 trendClear;                                         //采样开始时清除波形，0-off, 1-on
    quint8 trendDisplayDirection;                  //趋势显示方向， 0-横， 1-纵，默认：0
    quint8 trendLineWidth;                             //趋势波形线宽， 0-标准， 1-粗， 2-细， 默认0
    quint8 trendGrid;                                      //趋势栅格，0/4/5/6/7/8/9/10/11/12, 默认0(自动)

    quint8 trendScaleMode;                           //标尺显示模式：0-标准，1-详细，默认：0
    quint8 trendScaleType;                             //标尺显示方式：0-标记，1-棒图，默认：0
    quint8 trendScaleNumber;                       //当前值标记通道号，0/3/4,   默认3位

    quint8 trendPartialOnOff;                             //部分压缩放大开关,0-off, 1-on

    quint8 trendMsgWriteMode;                    //趋势信息写入方式，0-共同，1-个别
    quint8 trendMsgPowerDown;                     //停电信息开关，0-off, 1-on
    quint8 trendMsgIntervalChange;                //趋势周期变更信息写入开关，0-off, 1-on
}DISPLAY_GONFIG_TREND ;


/****************************************
 * 组设置相关数据结构体定义
 ****************************************/
typedef struct DISPLAY_GONFIG_GROUP_STRUCT
{
    //标尺图像
    quint8 groupScaleImage;                           //组标尺图像开关，0-off, 1-on

    //组设置
    quint8 groupSwitch;                                   //组开关，0-off, 1-on
    quint8 groupChannelNum;                        //组通道数，最多20个通道
    quint8 RESERVED;
    uchar groupName[20];                                //组名，最多16个半角字符，中英文支持
    quint32 groupChannelItem[20];                 //组通道号，有效通道号由groupChannelNum决定

    //分割线
    quint8 groupTripLineSwitch[4];                //分割线1-4开关，0-off, 1-on, 默认false
    quint8 groupTripLinePos[4];                     //分割线1-4位置，单位:%, 0-100,默认50
    quint8 groupTripLineWidth[4];                  //分割线1-4线宽，0-标准， 1-粗， 2-细， 默认0
    quint32 groupTripLineColor[4];                  //分割线1-4颜色值，BYTE2-Red, BYTE1-Green, BYTE0-Blue
}DISPLAY_GONFIG_GROUP ;

/****************************************
 * 预设定信息相关数据结构体定义
 ****************************************/
typedef struct DISPLAY_GONFIG_PRE_MESSAGE_STRUCT
{
    uchar preMessage[100][32];                       //预先设定信息，100个，每个最多32个半角字符；
}DISPLAY_GONFIG_PRE_MESSAGE ;

/****************************************
 * 画面现实基本项相关数据结构体定义
 ****************************************/
typedef struct DISPLAY_GONFIG_BASIC_STRUCT
{
    quint8 basicBarDirection;                   //棒图显示方向，(0-横，1-纵)，默认：1

    quint8 basicLCDBrightness;                  //LCD显示辉度，0-100:<B1-0>,<B2-20>,...<B6-100>,默认:<B3-40>
    quint8 basicBacklightSaverMode;         //LCD屏保模式，0-off, 1-变暗，2-熄灭
    quint8 basicBacklightSaverTime;          //LCD屏保等待时间，单位：min, 范围：1min/2min/5min/10min/30min/1h
    quint8 basicBacklightRestore;              //LCD屏保恢复模式，0-按键/触摸/报警， 1-按键/触摸，默认：0

    quint8 basicBackground;                       //UI背景色，0-白色，1-黑色，默认：0

    quint8 basicGroupChangeTime;           //组自动切换时间，单位：s,范围：5s/10s/20s/30s/60s(1min),默认10s
    quint8 basicAutoHomeWinTime;          //自动默认画面跳转时间，单位:min,范围：0(off)/1/2/5/10/20/30/1h,默认：0

    quint8 basicFirstWeekday;                     //周起始日，0-星期日，1-星期一

    quint8 basicChangeFromMonitor;         //从监视器修改各值，false-off, true-on
    quint8 RESERVED[2];
}DISPLAY_GONFIG_BASIC ;

#endif // CFGGLOBALDEF_DISPLAY_H
