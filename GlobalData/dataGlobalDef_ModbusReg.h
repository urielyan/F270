/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：dataGlobbalDef_ModbusReg.h
 * 概   要：Modbus寄存器分配表定义
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

#ifndef DATAGLOBALDEF_MODBUSREG_H
#define DATAGLOBALDEF_MODBUSREG_H

#include "cfgGlobalDef_Type.h"
#include "globalDef.h"
/*
 * Modbus寄存器分配表数据结构定义
 * 成员命名规则：
 *          “寄存器类型 + 单元类型 + 通道类型 + 状态/数据 + 数据类型 + [其它信息]”
 *
 * 寄存器类型:          inputReg/holdReg/Coil/Relay
 * 单元类型:              MainUnit/ExtUnit?/Device
 * 通道类型:              IO/Math/Comm/Switch/Operate/None
 * 状态/数据:            Data/Status
 * 数据类型:              Int32/Float32/Int16/Bit
 * 其它信息:              附加信息
*/
typedef struct MODBUS_REGISTER_MAP_STRUCT{
    /***输入寄存器：输入输出通道（本机/扩展）、运算通道***/
    //本机IO通道
    quint16             inputReg_MainUnit_IO_Data_Int32[MAIN_UNIT_MODULE_NUM][UNIT_CHAN_NUM_PRE<<1];       //本机输入输出通道数据，32bit整型, 2个quint16组成Int32, 对应500个
    quint16             inputReg_MainUnit_IO_Data_Float32[MAIN_UNIT_MODULE_NUM][UNIT_CHAN_NUM_PRE<<1];   //本机输入输出通道数据，32bit浮点数据, 2个quint16组成float32, 对应500个通道
    qint16               inputReg_MainUnit_IO_Data_Int16[MAIN_UNIT_MODULE_NUM][UNIT_CHAN_NUM_PRE];         //本机输入输出通道数据，16bit整型，对应500个通道
    qint16               inputReg_MainUnit_IO_Status_Int16[MAIN_UNIT_MODULE_NUM][UNIT_CHAN_NUM_PRE];      //本机输入输出通道状态数据，16bit整型，对应500个通道
    //本机IO通道连续区域
    quint16             inputReg_MainUnit_IO_Data_Int32_Linear[MAIN_UNIT_MODULE_NUM][UNIT_CHAN_NUM_LINEAR<<1];
    quint16             inputReg_MainUnit_IO_Data_Float32_Linear[MAIN_UNIT_MODULE_NUM][UNIT_CHAN_NUM_LINEAR<<1];
    qint16               inputReg_MainUnit_IO_Data_Int16_Linear[MAIN_UNIT_MODULE_NUM][UNIT_CHAN_NUM_LINEAR];
    qint16               inputReg_MainUnit_IO_Status_Int16_Linear[MAIN_UNIT_MODULE_NUM][UNIT_CHAN_NUM_LINEAR];
    //本机运算通道
    quint16             inputReg_MainUnit_Math_Data_Int32[MAIN_UNIT_MATH_CHAN_NUM_PRE<<1];
    quint16             inputReg_MainUnit_Math_Data_Float32[MAIN_UNIT_MATH_CHAN_NUM_PRE<<1];
    qint16               inputReg_MainUnit_Math_Data_Int16[MAIN_UNIT_MATH_CHAN_NUM_PRE];
    qint16               inputReg_MainUnit_Math_Status_Int16[MAIN_UNIT_MATH_CHAN_NUM_PRE];
    //本机状态
    qint16               inputReg_Device_None_Status_Int16[8];             //设备状态寄存器
    //扩展单元
    quint16             inputReg_ExtUnit_IO_Data_Int32[EXT_UNIT_NUM][EXT_UNIT_MODULE_NUM][UNIT_CHAN_NUM_PRE<<1];
    quint16             inputReg_ExtUnit_IO_Data_Float32[EXT_UNIT_NUM][EXT_UNIT_MODULE_NUM][UNIT_CHAN_NUM_PRE<<1];
    qint16               inputReg_ExtUnit_IO_Data_Int16[EXT_UNIT_NUM][EXT_UNIT_MODULE_NUM][UNIT_CHAN_NUM_PRE];
    qint16               inputReg_ExtUnit_IO_Status_Int16[EXT_UNIT_NUM][EXT_UNIT_MODULE_NUM][UNIT_CHAN_NUM_PRE];

    /***保持寄存器：输入输出通道（本机/扩展）、运算通道、通信通道***/
    //本机IO通道: 10(卡槽数) * 50(每板卡预留50个通道) = 500(本机通道数)
    quint16             holdReg_mainUnit_IO_Data_Int32[MAIN_UNIT_MODULE_NUM][UNIT_CHAN_NUM_PRE<<1];       //本机输入输出通道数据，32bit整型, 2个quint16组成Int32, 对应500个
    quint16             holdReg_mainUnit_IO_Data_Float32[MAIN_UNIT_MODULE_NUM][UNIT_CHAN_NUM_PRE<<1];   //本机输入输出通道数据，32bit浮点数据, 2个quint16组成float32, 对应500个通道
    qint16               holdReg_mainUnit_IO_Data_Int16[MAIN_UNIT_MODULE_NUM][UNIT_CHAN_NUM_PRE];             //本机输入输出通道数据，16bit整型，对应500个通道
    qint16               holdReg_mainUnit_IO_Status_Int16[MAIN_UNIT_MODULE_NUM][UNIT_CHAN_NUM_PRE];           //本机输入输出通道状态数据，16bit整型，对应500个通道
    //本机IO通道连续区域：10(卡槽数) * 10(每板卡实际最多) = 100(本机最多实际通道数)
    quint16             holdReg_MainUnit_IO_Data_Int32_Linear[MAIN_UNIT_MODULE_NUM][UNIT_CHAN_NUM_LINEAR<<1];
    quint16             holdReg_MainUnit_IO_Data_Float32_Linear[MAIN_UNIT_MODULE_NUM][UNIT_CHAN_NUM_LINEAR<<1];
    qint16               holdReg_MainUnit_IO_Data_Int16_Linear[MAIN_UNIT_MODULE_NUM][UNIT_CHAN_NUM_LINEAR];
    qint16               holdReg_MainUnit_IO_Status_Int16_Linear[MAIN_UNIT_MODULE_NUM][UNIT_CHAN_NUM_LINEAR];
    //本机通信通道：预留300个通信通道
    quint16             holdReg_MainUnit_Comm_Data_Int32[MAIN_UNIT_COMM_CHAN_NUM_PRE<<1];
    quint16             holdReg_MainUnit_Comm_Data_Float32[MAIN_UNIT_COMM_CHAN_NUM_PRE<<1];
    qint16               holdReg_MainUnit_Comm_Data_Int16[MAIN_UNIT_COMM_CHAN_NUM_PRE];
    qint16               holdReg_MainUnit_Comm_Status_Int16[MAIN_UNIT_COMM_CHAN_NUM_PRE];
    //内部开关
    qint16               holdReg_Device_Switch_Data_Int16[MAIN_UNIT_INTERNAL_SWITCH];
    //操作设定
    qint16               holdReg_Device_Operate_Data_Int16_Record;          //开始/停止记录：0-读取时始终为0； 1:记录开始；2:记录停止
    qint16               holdReg_Device_Operate_Data_Int16_Computation; //运算操作：0-读取始终为0; 1:运算开始；2:运算停止；3:运算复位；4:解除运算丢失状态显示
    qint16               holdReg_Device_Operate_Data_Int16_AllAlarmAck;  //解除所有报警：0-读取始终为0；1-解除报警输出

    qint16               holdReg_Device_Operate_Data_Int16_IndividAlarmAck; //解除单个报警：0-读取始终为0；1-输入输出通道；2:运算通道；3:通信通道
    qint16               holdReg_Device_Operate_Data_Int16_IndividNum;    //解除单个报警通道号码：0-读取始终为0
    qint16               holdReg_Device_Operate_Data_Int16_IndividLevel;    //报警电平号：0-读取始终为0，报警1~4 ACK

    qint16               holdReg_Device_Operate_Data_Int16_Misc;                //杂项操作，0-读取始终为0， 1-手动采样；2-手动触发；3-画面存储；4-执行显示数据强制超时；5-执行事件数据强制超时
    qint16               holdReg_Device_Operate_Data_Int16_SNTP;               //时间同步，读取始终为0， 1-执行手动SNTP
    qint16               holdReg_Device_Operate_Data_Int16_Email;               //Emai,读取始终为0, 1-Email发送开始；2-Email发送停止
    qint16               holdReg_Device_Operate_Data_Int16_Modbus;          //Modbus客户/服务恢复：读取始终为0; 1-手动恢复客户端；2-手动恢复服务端
    qint16               holdReg_Device_Operate_Data_Int16_Timer;              //计时器复位，读取始终为0, 1~4: 复位制定号码的计时器，100:复位所有计时器
    qint16               holdReg_Device_Operate_Data_Int16_MatchTimer;   //匹配计时器复位，读取始终为0, 1~4: 复位制定号码的计时器，100:复位所有计时器
    qint16               holdReg_Device_Operate_Data_Int16_Year;                //年，只读
    qint16               holdReg_Device_Operate_Data_Int16_Month;            //月，只读
    qint16               holdReg_Device_Operate_Data_Int16_Day;                 //日，只读
    qint16               holdReg_Device_Operate_Data_Int16_Hour;               //时，只读
    qint16               holdReg_Device_Operate_Data_Int16_Minute;           //分，只读
    qint16               holdReg_Device_Operate_Data_Int16_Second;           //秒，只读

    qint16               holdReg_Device_Operate_Data_Int16_MsgWrite;        //写入信息，读取始终为0, 1-写入预设信息，2-写入自由信息
    qint16               holdReg_Device_Operate_Data_Int16_MsgNum;        //写入信息号，读取始终为0, 预设信息：1~100, 自由信息：1~10
    qint16               holdReg_Device_Operate_Data_Int16_MsgDest;         //信息写入位置，读取始终为0,  0-所有显示组，1~其它：指定组
    qint16               holdReg_Device_Operate_Data_Int16_BatchGrpNum;      //批处理号码，读取始终为0, 1~其它：比处理组号码
    qint16               holdReg_Device_Operate_Data_Int16_FreeMsg[17];    //自由信息, [预设信息]时无效
    qint16               holdReg_Device_Operate_Data_Int16_BatchString[17];//批处理号码
    qint16               holdReg_Device_Operate_Data_Int32_BatchNum[2]; //批次号码，32位整数型

    //扩展单元
    quint16             holdReg_ExtUnit_IO_Data_Int32[EXT_UNIT_NUM][EXT_UNIT_MODULE_NUM][UNIT_CHAN_NUM_PRE<<1];
    quint16             holdReg_ExtUnit_IO_Data_Float32[EXT_UNIT_NUM][EXT_UNIT_MODULE_NUM][UNIT_CHAN_NUM_PRE<<1];
    qint16               holdReg_ExtUnit_IO_Data_Int16[EXT_UNIT_NUM][EXT_UNIT_MODULE_NUM][UNIT_CHAN_NUM_PRE];
    qint16               holdReg_ExtUnit_IO_Status_Int16[EXT_UNIT_NUM][EXT_UNIT_MODULE_NUM][UNIT_CHAN_NUM_PRE];

    /***线圈：仅访问本机上通道***/
    quint8              coil_MainUnit_IO_Data_Bit[MAIN_UNIT_MODULE_NUM][UNIT_CHAN_NUM_PRE];
    quint8              coil_MainUnit_IO_Status_Bit[MAIN_UNIT_MODULE_NUM][UNIT_CHAN_NUM_PRE];
    quint8              coil_MainUnit_Comm_Data_Bit[MAIN_UNIT_COMM_CHAN_NUM_PRE];
    quint8              coil_MainUnit_Comm_Status_Bit[MAIN_UNIT_COMM_CHAN_NUM_PRE];
    quint8              coil_MainUnit_Switch_Data_Bit[MAIN_UNIT_INTERNAL_SWITCH];

    /***输入继电器：仅访问本机上通道***/
    quint8              relay_MainUnit_IO_Data_Bit[MAIN_UNIT_MODULE_NUM][UNIT_CHAN_NUM_PRE];
    quint8              relay_MainUnit_IO_Status_Bit[MAIN_UNIT_MODULE_NUM][UNIT_CHAN_NUM_PRE];
    quint8              relay_MainUnit_Math_Data_Bit[MAIN_UNIT_MATH_CHAN_NUM_PRE];
    quint8              relay_MainUnit_Math_Status_Bit[MAIN_UNIT_MATH_CHAN_NUM_PRE];
}MODBUS_REGISTER_MAP __attribute__((aligned(4)));
#endif // DATAGLOBALDEF_MODBUSREG_H
