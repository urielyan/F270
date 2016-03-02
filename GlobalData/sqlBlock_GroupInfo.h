/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：sqlBlock_GroupInfo.h
 * 概   要：通道组信息，相关数据结构及枚举声明。
 * 当数据信息用来作为历史数据进行显示时，组信息提供历史时刻的组分配信息。
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

#ifndef SQLBLOCK_GROUPINFO_H
#define SQLBLOCK_GROUPINFO_H

/*
 * 组信息结构体
 */
typedef struct GROUP_INFO_STRUCT {
    quint32 grpIndex;        //组索引号

    DISPLAY_GONFIG_GROUP    groupInfo;
}GROUP_INFO;

/*
 * 组信息结构体
 * NOTE！
 * SQL中根据GROUP_INFO获取组数groupNum，随后跟随groupNum个
 * GROUP_INFO数据结构体，分别表示各个组的相关信息
 */
typedef struct SQL_GROUP_INFO_STRUCT {
    SQL_Block_Type                      blockType;              //信息块类型
    quint32                                    groupNum;             //组数
}SQL_GROUP_INFO;


#endif // SQLBLOCK_GROUPINFO_H
