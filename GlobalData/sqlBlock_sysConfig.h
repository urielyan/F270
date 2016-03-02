/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：sqlBlock_fileInfo.h
 * 概   要：记录数据时对应的系统配置信息，包括：测量周期、趋势周期、时区等等
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

#ifndef SQLBLOCK_SYSCONFIG_H
#define SQLBLOCK_SYSCONFIG_H

typedef struct SQL_SYS_CONFIG_STRUCT {
    SQL_Block_Type          blockType;

    quint32                        measureInterval;        //测量周期（ms）
    quint32                        trendInterval;             //趋势周期（s）

    qint32                          diffGMTHour;              //与GMT的时差，单位：h,范围：-13~13
    quint32                        diffGMTMin;                //与GMT的时差，单位：min,范围：0~59

    qint64                          recordStartTime;
    qint64                          recordTimeGap;
    qint32                          periodMin;
}SQL_SYS_CONFIG;

#endif // SQLBLOCK_SYSCONFIG_H
