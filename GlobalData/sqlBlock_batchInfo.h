/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：sqlBlock_fileInfo.h
 * 概   要：批处理相关信息
 *
 * 当前版本：V1.0.0
 * 作   者：王 品
 * 完成日期：2015-7-13
 *
 * 修改版本：
 * 修改说明：
 * 修改作者：
 *
 ********************************************************************/
#include <QtGlobal>
#include "cfgGlobalDef.h"

#ifndef SQLBLOCK_BATCHINFO_H
#define SQLBLOCK_BATCHINFO_H

typedef struct SQL_BATCH_INFO_STRUCT {
    SQL_Block_Type          blockType;

    BATCH_CONFIG          batchInfo;
}SQL_BATCH_INFO;

#endif // SQLBLOCK_BATCHINFO_H
