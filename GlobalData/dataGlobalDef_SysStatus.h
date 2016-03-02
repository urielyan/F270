/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：dataGlobbalDef_SysStatus.h
 * 概   要：系统状态结构体定义
 *
 * 当前版本：V1.0.0
 * 作   者：王 品
 * 完成日期：2015-6-10
 *
 * 修改版本：
 * 修改说明：
 * 修改作者：
 *
 ********************************************************************/
#include <QtGlobal>

#ifndef DATAGLOBALDEF_SYSSTATUS_H
#define DATAGLOBALDEF_SYSSTATUS_H

#include "cfgGlobalDef_Type.h"
#include "GlobalData/globalDef.h"

/*IO板卡配置结构体*/
typedef struct BOARD_IO_CONFIG_STRUCT {
    quint16 boardReg_status;                //IO板卡状态寄存器
    quint16 boardReg_version;             //板卡固件版本号
    quint16 boardReg_config[BOARD_IO_CONFIG_NUM];         //板卡配置：根据板块类型AI/DI/DO/DIO区分，具体参考文档
}BOARD_IO_CONFIG;

/*扩展板卡配置结构体*/
typedef struct BOARD_EXT_CONFIG_STRUCT {
    quint16 boardReg_status;                //扩展板卡状态寄存器
    quint16 boardReg_version;             //板卡固件版本号
    quint16 boardConfig[EXT_UNIT_MODULE_TOTAL];//自身及级联扩展单元板卡配置标记，0xAA表示有效
}BOARD_EXT_CONFIG;

/*扩展板卡配置寄存器表结构体*/
typedef struct EXT_CONFIG_STRUCT {
    BOARD_EXT_CONFIG   selfConfig;          //扩展单元自身配置
    BOARD_IO_CONFIG     extConfig[EXT_UNIT_NUM][EXT_UNIT_MODULE_NUM]; //本单元及向下级联单元IO板卡配置
}EXT_CONFIG;

/*
 *板卡数据寄存器表结构体
 */
typedef struct SUB_BOARD_DATA_STRUCT{
    union{
        quint16  dataModbus[UNIT_CHAN_NUM_USE_MAX<<1];                         //Modbus寄存器表
        float       dataSample[UNIT_CHAN_NUM_USE_MAX];                                 //AI类型板卡采样数据, 对应的float32
        quint32  dataDICount[UNIT_CHAN_NUM_USE_MAX];                               //DI脉冲类型脉冲计数, 对应的uint32
    };
    union {
        struct {
            quint16 bitsInputDI;           //DI输入值
            quint16 bitsInputDO;         //DO输入值
        };
        float   tempAI;                         //AI温度值
    };

}SUB_BOARD_DATA;

typedef struct SYSTEM_CONFIG_PACKID_T {
    quint16 mainUnit_PrePackID[MAIN_UNIT_MODULE_NUM];                       //本机板卡配置包ID
    quint16 extUnit_PrePackID[EXT_UNIT_NUM][EXT_UNIT_MODULE_NUM]; //扩展板卡配置包ID
}SYSTEM_CONFIG_PACKID;

/*
 * quint16状态说明：
 *  bit0    :   子板卡状态，0-Not Ready;  1- Ready
 *  bit1-6 :   DIO板卡DO通道数，最多支持50个通道
 *  bit7-12:  子板卡通道数，最多支持50个通道，各类型板卡通道数，DIO对应DI通道数
 *  bit13-15: 子板卡类型， 0-None, 1-AI， 2-DI,  3-DO， 4-DIO， 5-Extern
 */
typedef struct SYSTEM_STATUS_STRUCT{
    quint16 mainUnit_ModuleStatus[MAIN_UNIT_MODULE_NUM];                            //本机板卡（模块）当前状态
    quint16 extUnit_ModuleStatus[EXT_UNIT_NUM][EXT_UNIT_MODULE_NUM];      //扩展单元板卡（模块）当前状态

    BOARD_IO_CONFIG mainUnit_Recognition[MAIN_UNIT_MODULE_NUM];        //本机板卡（模块）识别状态
    BOARD_IO_CONFIG extUnit_Recognition[EXT_UNIT_NUM][EXT_UNIT_MODULE_NUM];      //扩展单元板卡（模块）当前状态

    SYSTEM_CONFIG_PACKID configPacketID;                                                            //板卡配置包ID
}SYSTEM_STATUS;

#endif // DATAGLOBALDEF_SYSSTATUS_H
