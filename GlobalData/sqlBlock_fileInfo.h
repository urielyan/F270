/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：sqlBlock_fileInfo.h
 * 概   要：文件额外信息，主要包括：设备名称、号码、标签、标签号等，该部分内容在
 * 到处到外部存储器时，需要写入对应的文件中。
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
#include "cfgGlobalDef_Type.h"

#ifndef SQLBLOCK_FILEINFO_H
#define SQLBLOCK_FILEINFO_H

/*数据库文件信息结构体*/
typedef struct SQL_FILE_INFO_STRUCT {
    SQL_Block_Type          blockType;
    uchar                            deviceModel[16];       //设备名称
    uchar                            deviceSerialNo[16];   //机器号码
    uchar                            deviceTag[32];            //设备标签
    uchar                            deviceTagNo[16];       //设备标签号码
}SQL_FILE_INFO;

#endif // SQLBLOCK_FILEINFO_H
