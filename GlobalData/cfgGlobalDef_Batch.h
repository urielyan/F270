/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：CfgGlobbalDef_Batch.h
 * 概   要：批处理配置相关数据结构、枚举值定义
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

#ifndef CFGGLOBALDEF_BATCH_H
#define CFGGLOBALDEF_BATCH_H

/****************************************
 * 批处理配置相关数据结构体定义
 ****************************************/
typedef struct BATCH_CONFIG_STRUCT
{
    //配置
    quint8 batchOnOff;            //批处理开关, 0-off, 1-on
    quint8 batchNumber;        //批处理号码位数， 0(off)/4/6/8
    quint8 batchAutoInc;         //批处理号码自动增加开关，0-off, 1-on

    //最多设置24个文本区域，包括：区域标题+字符串
    uchar batchTextTitle[24][24];   //批处理文本区域标题，最多20个半角字符，中英文支持
    uchar batchTextString[24][32];  //批处理文本字符串，最多30个半角字符，中英文支持

    //批处理号码及批次号码
    uchar batchBatNumber[32];      //批处理号码，最多32个字符
    quint32 batchLotNumber;          //批次号码，batchNumber决定数据位数

    //3个批处理注释
    uchar batchComment[3][52];      //3个批处理注释，每个最多50个半角字符，中英文支持
}BATCH_CONFIG ;

#endif // CFGGLOBALDEF_BATCH_H
