/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：sqlBlock_GroupInfo.h
 * 概   要：记录通道信息，相关数据结构及枚举声明
 *
 * 当前版本：V1.0.0
 * 作   者：王 品
 * 完成日期：2015-7-9
 *
 * 修改版本：
 * 修改说明：
 * 修改作者：
 *
 ********************************************************************/
#include <QtGlobal>
#include "cfgGlobalDef.h"

#ifndef SQLBLOCK_RECORDCHANINFO_H
#define SQLBLOCK_RECORDCHANINFO_H

/*
 * 通道信息结构体
 */
typedef struct CHAN_INFO_STRUCT {
    quint32 chanNum;        //通道编码

    union {
        AI_CONFIG           chanAI;
        DI_CONFIG           chanDI;
        DO_CONFIG         chanDO;
        MATH_CONFIG     chanMath;
        COMM_CONFIG   chanComm;
    }channelConfig;
}CHAN_INFO;

/*
 * 记录通道信息结构体
 * NOTE！
 * SQL中根据RECORD_CHAN_INFO获取组数channelNum，随后跟随channelNum个
 * CHAN_INFO数据结构体，分别表示各个通道的相关信息
 */
typedef struct SQL_RECORD_CHAN_INFO_STRUCT {
    SQL_Block_Type                      blockType;              //信息块类型
    quint32                                    channelNum;             //记录通道数
}SQL_RECORD_CHAN_INFO;

#endif // SQLBLOCK_RECORDCHANINFO_H
