/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：sqlBlock_fileFormat.h
 * 概   要：文件格式信息块数据结构体定义, 记录到处至外部存储器时，根据该配置进行
 * 信息写入、文件命名。
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

#ifndef SQLBLOCK_FILEFORMAT_H
#define SQLBLOCK_FILEFORMAT_H

typedef struct SQL_FILE_FORMAT_STRUCT {
    SQL_Block_Type          blockType;

    quint32                         fileFormatVersion;    //文件格式版本

    DATASAVE_CONFIG     fileSaveConfig;           //文件保存相关配置
}SQL_FILE_FORMAT;

#endif // SQLBLOCK_FILEFORMAT_H
