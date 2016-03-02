/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：sqlBlock_reportInfo.h
 * 概   要：报表相关信息
 *
 * 当前版本：V1.0.0
 * 作   者：王 品
 * 完成日期：2015-7-20
 *
 * 修改版本：
 * 修改说明：
 * 修改作者：
 *
 ********************************************************************/
#include <QtGlobal>
#include "cfgGlobalDef.h"

#ifndef SQLBLOCK_REPORTINFO_H
#define SQLBLOCK_REPORTINFO_H

/* 报表主表信息块结构体*/
typedef struct SQL_REPORT_INFO_STRUCT {
    SQL_Block_Type          blockType;

    REPORT_CONFIG         reportInfo;
}SQL_REPORT_INFO;

#endif // SQLBLOCK_REPORTINFO_H
