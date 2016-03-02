/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：CfgGlobbalDef_AI.h
 * 概   要：模拟板卡类型通道配置相关数据结构体、枚举值定义
 *
 * 当前版本：V1.0.0
 * 作   者：王 品
 * 完成日期：2015-6-4
 *
 * 修改版本：
 * 修改说明：
 * 修改作者：
 *
 ********************************************************************/
#include <QtGlobal>
#include "cfgGlobalDef_Type.h"

#ifndef CFGGLOBALDEF_AI_H
#define CFGGLOBALDEF_AI_H

/* AI通道量程类型定义*/
typedef enum AI_RANGE_Type_Enum{                                  //默认值:AI_RANGE_Type_Disabled
    AI_RANGE_Type_Disabled = 0,                                             //不使用，即不测量
    AI_RANGE_Type_Vlot,                                                            //直流电压
    AI_RANGE_Type_GS,                                                              //GS（0.4-2V, 1-5V标准信号）
    AI_RANGE_Type_TC,                                                              //热电耦
    AI_RANGE_Type_RTD,                                                           //热电阻
    AI_RANGE_Type_DI,                                                              //开关量
    AI_RANGE_Type_Cur0_20mA,                                               //电流（0-20mA）
    AI_RANGE_Type_GS4_20mA,                                                 //GS(4-20mA标准信号)
} AI_RANGE_Type;

/* AI通道电压类型通道量程定义*/
typedef enum AI_Range_Volt_Enum{                    //默认值:AI_Range_Volt_2V
    AI_Range_Volt_20mV = 0,
    AI_Range_Volt_60mV ,
    AI_Range_Volt_200mV,
    AI_Range_Volt_1V,
    AI_Range_Volt_2V,
    AI_Range_Volt_6V,
    AI_Range_Volt_20V,
    AI_Range_Volt_50V,
} AI_Range_Volt;

/* AI通道GS标准信号类型通道量程定义*/
typedef enum AI_Range_GS_Enum{                      //默认值:AI_Range_GS_High
    AI_Range_GS_Low = 0,                                        //0.4 - 2V，范围限定：0.3200V - 2.0800V
    AI_Range_GS_High ,                                            //1-5V, 范围限定：0.800V - 5.200V
} AI_Range_GS;

/* AI通道热电耦类型通道量程定义*/
typedef enum AI_Range_TC_Enum{                      //默认值:AI_Range_TC_K
    AI_Range_TC_R = 0,                                            //R型，范围限定：0.0°C - 1760°C
    AI_Range_TC_S ,                                                 //S型，范围限定：0.0°C - 1760°C
    AI_Range_TC_B ,                                                 //B型，范围限定：0.0°C - 1820°C
    AI_Range_TC_K ,                                                 //K型，范围限定：-270.0°C - 1370°C
    AI_Range_TC_K_H ,                                            //K型（高精度），范围限定：-200.0°C - 500°C
    AI_Range_TC_E ,                                                 //K型，范围限定：-270.0°C - 800°C
    AI_Range_TC_C ,                                                 //J型，范围限定：-200.0°C - 1100°C
    AI_Range_TC_T ,                                                 //T型，范围限定：-270.0°C - 400°C
    AI_Range_TC_N ,                                                 //N型，范围限定：-270.0°C - 1300°C
    AI_Range_TC_W ,                                                //W型，范围限定：0.0°C - 2315°C
    AI_Range_TC_L ,                                                 //L型，范围限定：-200.0°C - 900°C
    AI_Range_TC_U ,                                                 //U型，范围限定：-200.0°C - 400°C
    AI_Range_TC_WRe3_25 ,                                   //WRe型，范围限定：0.0°C - 2320°C
    AI_Range_TC_PLATINEL ,                                  //范围限定：0.0°C - 1395°C
    AI_Range_TC_PR20_40 ,                                   //范围限定：0.0°C - 1900°C
    AI_Range_TC_KpvsAu7Fe ,                              //KpvsAu7Fe型，范围限定：0.0K - 300K
    AI_Range_TC_NiNiMo ,                                    //范围限定：0.0°C - 1310°C
    AI_Range_TC_WWRe26 ,                                 //WWRe26 型，范围限定：0.0°C - 2320°C
    AI_Range_TC_N14 ,                                          //N型，范围限定：0.0°C - 1300°C
    AI_Range_TC_XK ,                                            //XK GOST 型，范围限定：-200.0°C - 600°C
} AI_Range_TC;

/* AI通道热电阻类型通道量程定义*/
typedef enum AI_Range_RTD_Enum{                 //默认值:AI_Range_RTD_Pt100
    AI_Range_RTD_Pt100 = 0,                                 //Pt100, 范围限定：-200.0°C - 850°C
    AI_Range_RTD_Pt100_H ,                                 //Pt100高精度, 范围限定：-150.0°C - 150°C
    AI_Range_RTD_JPt100 ,                                    //JPt100, 范围限定：-200.0°C - 550°C
    AI_Range_RTD_JPt100_H ,                                //JPt100高精度, 范围限定：-150.0°C - 150°C
    AI_Range_RTD_Cu10GE ,                                 //Cu10GE, 范围限定：-200.0°C - 300°C
    AI_Range_RTD_Cu10LN ,                                 //Cu10LN, 范围限定：-200.0°C - 300°C
    AI_Range_RTD_Cu10WEED ,                            //Cu10WEED, 范围限定：-200.0°C - 300°C
    AI_Range_RTD_Cu10BAILEY ,                         //Cu10BAILEY , 范围限定：-200.0°C - 300°C
    AI_Range_RTD_Cu10a392 ,                             //Cu10a392 , 范围限定：-200.0°C - 300°C
    AI_Range_RTD_Cu10a393 ,                            //Cu10a393 , 范围限定：-200.0°C - 300°C
    AI_Range_RTD_Cu25 ,                                     //Cu25 , 范围限定：-200.0°C - 300°C
    AI_Range_RTD_Cu53 ,                                     //Cu53 , 范围限定：-50.0°C - 150°C
    AI_Range_RTD_Cu100 ,                                  //Cu100 , 范围限定：-50.0°C - 150°C
    AI_Range_RTD_J263B ,                                   //J263*B , 范围限定：0.0K - 300.0K
    AI_Range_RTD_Ni100SAMA ,                        //Ni100(SAMA) , 范围限定：-200.0°C - 250°C
    AI_Range_RTD_Ni100DN ,                              //Ni100(DN) , 范围限定：-60.0°C - 180°C
    AI_Range_RTD_Ni120 ,                                    //Ni120 , 范围限定：-70.0°C - 200°C
    AI_Range_RTD_Pt25 ,                                     //范围限定：-200.0°C - 550°C
    AI_Range_RTD_Pt50 ,                                     //范围限定：-200.0°C - 550°C
    AI_Range_RTD_Pt200WEED ,                        //Pt200(WEED) , 范围限定：-100.0°C - 250°C
    AI_Range_RTD_Cu10G ,                                //Cu10 GOST , 范围限定：-200.0°C - 200°C
    AI_Range_RTD_Cu50G ,                                //Cu50 GOST , 范围限定：-200.0°C - 200°C
    AI_Range_RTD_Cu100G ,                              //Cu100 GOST , 范围限定：-200.0°C - 200°C
    AI_Range_RTD_Pt46G ,                                //Pt46 GOST , 范围限定：-200.0°C - 550°C
    AI_Range_RTD_Pt100G ,                               //Pt100 GOST , 范围限定：-200.0°C - 600°C
} AI_Range_RTD;

/* AI通道DI类型通道量程定义*/
typedef enum AI_Range_DI_Enum{              //默认值:AI_Range_DI_Level
    AI_Range_DI_Level = 0,                              //电平，on(1)/off(0), on: >=2.5V, off: <=2.3V
    AI_Range_DI_Contact ,                               //接点,  on(1)/off(0)
} AI_Range_DI;

/* AI通道直流电流类型通道量程定义*/
typedef enum AI_Range_Cur0_20mA_Enum{ //默认值:AI_Range_Cur0_20mA_Const
    AI_Range_Cur0_20mA_Const = 0,               //直流电流，范围限定：0.000mA - 20.000mA
} AI_Range_Cur0_20mA;

/* AI通道GS4-20mA类型通道量程定义*/
typedef enum AI_Range_GS4_20mA_Enum{ //默认值:AI_Range_GS4_20mA_Type
    AI_Range_GS4_20mA_Type = 0,                 //标准信号，范围限定：3.2000mA - 20.800mA
} AI_Range_GS4_20mA;

/* AI通道辅助定义*/
typedef enum AI_Range_Data_Enum{
    AI_Range_Data_0 = 0,
    AI_Range_Data_1,
    AI_Range_Data_2,
    AI_Range_Data_3,
    AI_Range_Data_4,
    AI_Range_Data_5,
    AI_Range_Data_6,
    AI_Range_Data_7,
    AI_Range_Data_8,
    AI_Range_Data_9,
    AI_Range_Data_10,
    AI_Range_Data_11,
    AI_Range_Data_12,
    AI_Range_Data_13,
    AI_Range_Data_14,
    AI_Range_Data_15,
    AI_Range_Data_16,
    AI_Range_Data_17,
    AI_Range_Data_18,
    AI_Range_Data_19,
    AI_Range_Data_20,
    AI_Range_Data_21,
    AI_Range_Data_22,
    AI_Range_Data_23,
    AI_Range_Data_24,
    AI_Range_Data_25,
    AI_Range_Data_26,
    AI_Range_Data_27,
    AI_Range_Data_28,
    AI_Range_Data_29,
    AI_Range_Data_30,
    AI_Range_Data_31,
} AI_Range_Data;

/* AI通道运算方式定义*/
typedef enum AI_Range_Calculate_Enum{             //默认值:AI_Range_Calculate_Disabled
    AI_Range_Calculate_Disabled = 0,                       //off
    AI_Range_Calculate_Delta,                                  //与基准通道测量值的差值，作为该通道的测量值
    AI_Range_Calculate_Linear,                                //线性转换以获取测量值
    AI_Range_Calculate_Sqrt,                                    //计算输入值的平方根并转换单位，获得测量值
    AI_Range_Calculate_Log,                                     //“LOG选件”，LOG输入
    AI_Range_Calculate_LogApproach,                    //“LOG选件”，LOG近似输入
    AI_Range_Calculate_LogLinear,                          //“LOG选件”，LOG线性输入
} AI_Range_Calculate;

/* 标尺数据结构定义*/
typedef struct AI_RANGE_SCALE_STRUCT
{
    quint32    scalePosition;                                          //小数点位置, 0/1/2/3/4/5
    qint32      scaleMin;                                                  //标尺下限，-999999 ~ 999999
    qint32      scaleMax;                                                 //标尺上限 , -999999 ~ 999999
    uchar       scaleUnit[8];                                            //单位，字符串，最多6个半角字符
}AI_RANGE_SCALE;

/* 小信号处理配置结构定义*/
typedef struct AI_RANGE_LOW_CUT_STRUCT
{
    quint32    lowCutOnOff;                                           //小信号切除开关,0-off, 1-on
    quint32    lowCutOut;                                               //0: %0输出； 1:线性输出
    float         lowCutPos;                                                //切除点, 0.0%~5.0%, 单位:%
}AI_RANGE_LOW_CUT;

/* 移动平均配置结构定义*/
typedef struct AI_RANGE_MOVING_AVG_STRUCT
{
    quint32 avgOnOff;                                                   //移动平均开关,0-off, 1-on
    quint32 avgCount;                                                   //移动平均采样次数 2 ~100
}AI_RANGE_MOVING_AVG;

/* 冷端补偿配置结构定义*/
typedef struct AI_RANGE_RJC_COMP_STRUCT
{
    quint32    rjcCompMode;                                       //补偿模式，0:内部， 1:外部
    float         rjcTemp;                                                  //温度，外部模式时有效， -20.0°C - 80.0°C默认0
                                                                                     //或 253.1 ~353.2K(KpvsAu7Fe)默认273.2K
}AI_RANGE_RJC_COMP;

/* 断偶配置定义*/
typedef enum AI_Range_Burnout_Enum{            //默认值:AI_Range_Burnout_Off
    AI_Range_Burnout_Off = 0,                                //不检测传感器中的断偶
    AI_Range_Burnout_Up,                                      //正显示
    AI_Range_Burnout_Down,                                 //负显示
} AI_Range_Burnout;


/****************************************
 * AI通道量程配置相关数据结构体定义
 ****************************************/
typedef struct AI_CONFIG_RANGE_STRUCT
{
    AI_RANGE_Type rangeType;                                       //量程类型
    union
    {
        AI_Range_Volt rangeVolt;
        AI_Range_GS rangeGS;
        AI_Range_TC rangeTC;
        AI_Range_RTD rangeRTD;
        AI_Range_DI  rangeDI;
        AI_Range_Cur0_20mA  rangeCur0_20mA;
        AI_Range_GS4_20mA   rangeGS4_20mA;
        AI_Range_Data   rangeData;
    }config;                                                                        //量程，依赖量程类型

    float   rangeMin;                                                       //范围下限，依赖量程类型和量程
    float   rangeMax;                                                       //范围上限，依赖量程类型和量程

    float   rangeDispMin;                                               //范围下限，用户设定量程下限值
    float   rangeDispMax;                                               //范围上限，用户设定量程上限值

    AI_Range_Calculate    rangeCalculateType;           //通道运算类型
    quint32    rangeBasicChannel;                                //基础通道索引值，Delta运算方式时使用

    AI_RANGE_SCALE    rangeScale;                             //标尺配置，在【线性缩放】或【平方根】运算方式时有效

    AI_RANGE_LOW_CUT rangeLowCut;                      //【电压】类型且运算为【平方根】/【GS】类型且运算为
                                                                                      //【线性缩放】或【平方根】时显示

    AI_RANGE_MOVING_AVG   rangeMovingAvg;      //移动平均配置

    AI_RANGE_RJC_COMP rangeRJCCompensation;   //【TC】类型时有效

    AI_Range_Burnout  rangeBurnout;                        //【GS】、【GS(4-20mA)】、【TC】、【RTD】类型时有效
    qint32    rangeBias;                                                 //【DI】类型时无效！ 数值范围: -999999 ~ 999999
                                                                                      //作为偏移值加算到输入值或缩放值中
}AI_CONFIG_RANGE ;

/* AI通道报警类型定义*/
//typedef enum AI_Alarm_Type_Enum{                  //默认值:AI_Alarm_Type_H
//    AI_Alarm_Type_Off = 0,
//    AI_Alarm_Type_H,                                              //上限
//    AI_Alarm_Type_L ,                                              //下限
//    AI_Alarm_Type_R,                                              //变化率上升限
//    AI_Alarm_Type_r,                                               //变化率下升限
//    AI_Alarm_Type_T,                                              //延迟上限
//    AI_Alarm_Type_t,                                              //延迟下限
//    AI_Alarm_Type_h,                                             //差值上限， Delta运算时有效
//    AI_Alarm_Type_l,                                              //差值下限， Delta运算时有效
//} AI_Alarm_Type;

/****************************************
 * AI通道报警相关数据结构体定义
 ****************************************/
typedef struct AI_CONFIG_ALARM_STRUCT
{
    quint8 alarmOnOff[4];                                         //报警开关, 0-off, 1-on
    quint8 alarmDelay[4];                                          //报警延迟时间，延迟上下限时有效，[0]-秒，[1]-分，[2]-时
    Alarm_Type   alarmType[4];                                //报警类型
    float   alarmValue[4];                                           //报警值
    float   alarmHysteresis[4];                                   //滞后，设定报警发生值和接触值的差
    quint8 alarmLogging[4];                                     //检出, 0-off, 1-on
    quint8 alarmOutputType[4];                              //报警输出类型， 0-off, 1-Relay(继电器), 2-Switch(内部开关)
    quint32 alarmOuputNo[4];                                 //报警输出通道， Relay/Switches
}AI_CONFIG_ALARM ;

/****************************************
 * AI通道显示相关数据结构体定义
 ****************************************/
typedef struct AI_CONFIG_DISPLAY_STRUCT
{
    quint32 displayColor;                                           //通道颜色，Byte2-Red, Byte1-Green, Byte0-Blue

    uchar   displayTagChar[32];                                 //标记字符串，中英支持，最多32个半角字符
    uchar   displayTagNo[16];                                    //标记号码，符号，最多16个半角字符

    quint8  displayZoneUpper;                                  //显示区域上限，5-100,单位%
    quint8  displayZoneLower;                                  //显示区域上限，0-95,单位%

    quint8  displayScalePos;                                     //标尺位置，1-10,默认1
    quint8  displayScaleDiv;                                      //标尺分割，4-13, 13:C10

    quint8  displayBarPos;                                        //棒图基准位置， 0-Lower, 1-Center, 2-Upper
    quint8  displayBarDiv;                                         //棒图分割数，4-12, 默认10

    //趋势设定的部分压缩放大功能打开时有效
    quint8  displayPartialOnOff;                             //部分压缩放大开关,0-off, 1-on
    quint8  displayPartialPos;                                   //部分压缩放大位置，1-99, 单位：%，默认50
    float  displayPartialBoundary;                         //部分压缩放大边界值

    // 棒图共用
    quint32  displayColorbandMode;                        //彩色标尺带模式， 0-off, 1-内侧， 2-外侧.
    float  displayColorbandPosUpper;                 //彩色标尺带显示位置上限
    float  displayColorbandPosLower;                 //彩色标尺带显示位置下限
    quint32 displayColorbandColor;                       //彩色标尺带颜色，Byte2-Red, Byte1-Green, Byte0-Blue

    // 棒图共用
    quint32  displayAlarmMarkOnOff;                     //报警标记开关，0-0ff, 1-on
    quint32  displayAlarmMarkType;                        //报警标记类型，0-报警， 1-固定
    quint32 displayAlarmMarkColor[4];                  //报警1-4颜色， Byte2-Red, Byte1-Green, Byte0-Blue

    //【DI】类型量程且运算off时有效，测量值为0/1时显示的字符串
    uchar   displayDI0[10];                                        //最多8个半角字符，中英文
    uchar   displayDI1[10];                                        //最多8个半角字符，中英文
}AI_CONFIG_DISPLAY ;

/****************************************
 * AI通道校准相关数据结构体定义
 ****************************************/
typedef struct AI_CONFIG_CRORECTION_STRUCT
{
    quint32  correctMode;                                           //校准模式，0-off, 1-折线近似， 2-折线偏移
    quint32  correctCount;                                           //校正点数, 2-12, 默认2
    float   correctPointIn[12];                                   //折线输入点
    float   correctPointOut[12];                                //折线输出点
} AI_CONFIG_CRORECTION;

/****************************************
 * AI通道定义
 ****************************************/
typedef struct AI_CONFIG_STRUCT
{
    AI_CONFIG_RANGE             rangeConfig;
    AI_CONFIG_ALARM             alarmConfig;
    AI_CONFIG_DISPLAY           displayConfig;
    AI_CONFIG_CRORECTION   correctConfig;
}AI_CONFIG;

#endif // CFGGLOBALDEF_AI_H
