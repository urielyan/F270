/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：CfgGlobbalDef_Network_SysEnvironment.h
 * 概   要：系统环境变量配置相关数据结构体、枚举定义
 *
 * 当前版本：V1.0.0
 * 作   者：王 品
 * 完成日期：2015-6-9
 *
 * 修改版本：
 * 修改说明：
 * 修改作者：
 *
 ********************************************************************/
#include <QtGlobal>

#include "cfgGlobalDef_Type.h"

#ifndef CFGGLOBALDEF_SYSENVIRONMENT_H
#define CFGGLOBALDEF_SYSENVIRONMENT_H

#define INTERNAL_SWITCH_NUMBER  100         //内部开关数

/****************************************
 * 系统环境变量配置结构体定义
 ****************************************/
typedef struct SYS_CONFIG_ENVRIONMENT_STRUCT
{
    Language_Type   languageType;                   //语言类型
    Temperature_Unit    temperatureUnit;        //温度单位
    Decimal_Point_Type  decimalPointType;     //小数点表示
    Date_Format         dateFormat;                     //日期格式
    Delimiter_Type      delimiterType;                 //分割符类型
    Month_Indicator    monthIndicator;             //月份表示方式

    /*报警设定*/
    Alarm_Indicator alramIndicator;              //报警指示
    quint8 rateDecNum;                                  //变化率报警：下降数据数，范围：1~32, 实际间隔 = 数据数 * 测量周期
    quint8 rateIncNum;                                   //变化率报警：上升数据数，范围：1~32,
    quint8 individualAlarmAck;                       //单个报警解除开关

    /*时区设定*/
    qint8   diffGMTHour;                            //与GMT的时差，单位：h,范围：-13~13
    quint8   diffGMTMin;                              //与GMT的时差，单位：min,范围：0~59

    /*音效设定*/
    quint8    touchSnd;                                       //触摸音开关
    quint8    warningSnd;                                   //警告音开关
    quint8    menuKeyLED;                                 //MENU按键LED开关，off/报警

    /*继电器设定*/
    quint8    memMediaStatus;                         //内存/外部存储器异常， 状态输出模式时有效，下同！
    quint8    measureError;                               //测量错误
    quint8    commError;                                   //通信错误
    quint8    recordStop;                                    //记录停止
    quint8    alarm;                                            //报警
    quint8    RESERVED[3];
    Relay_Output_Mode   relayOutMode;     //继电器输出模式

    /*内部开关设定*/
    InterSwitch_Mode    interSwitchMode[INTERNAL_SWITCH_NUMBER];        //内部开关模式，报警/手动
    InterSwitch_Operate interSwitchOperate[INTERNAL_SWITCH_NUMBER];   //内部开关操作，与/或

    /*打印机设定*/
    quint8  printerIP[4];                                  //打印机IP地址，IPV4
    Printer_Paper_Size  printerPaperSize;     //纸张大小
    Printer_Orientation printerOrientation; //打印方向
    Printer_Resolution_DPI printerDPI;         //打印精度
    quint32 pageNumber;                               //打印份数，范围：1~10

    /*设备标签*/
    uchar   deviceTag[32];                              //设备标签，最多32个半角字符，中英文支持
    uchar   deviceTagNo[16];                         //设备标签号，最多16个半角字符

    /*配置文件注释*/
    uchar   settingComment[52];                 //配置注释，最多50个半角字符，中英文支持
}SYS_CONFIG_ENVRIONMENT ;

#endif // CFGGLOBALDEF_SYSENVIRONMENT_H
