/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：CfgGlobbalDef_DataSave.h
 * 概   要：数据保存配置相关数据结构、枚举值定义
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

#ifndef CFGGLOBALDEF_DATASAVE_H
#define CFGGLOBALDEF_DATASAVE_H

typedef enum  DataSave_FileName_Type_Enum {
    DataSave_FileName_Type_Date = 0,        //日期：序号+用户指定字符串+日期
    DataSave_FileName_Type_Serial,             //连续：序号+用户指定字符串
    DataSave_FileName_Type_Batch,             //批处理：序号+批处理名（使用批处理功能时）
}DataSave_FileName_Type;

typedef enum  DataSave_File_Format_Enum{
    DataSave_FileName_Type_Bin = 0,        //二进制
    DataSave_FileName_Type_Text,             //文本
}DataSave_File_Format_Enum;

/****************************************
 * 数据保存相关数据结构体定义
 ****************************************/
typedef struct DATASAVE_CONFIG_STRUCT
{
    //保存目录及标题信息
    uchar   datasveDirectoryName[24];           //文件保存目录名，最多20个半角字符
    uchar   datasaveTitleInfo[52];                     //文件标题注释，最多50个半角字符，中英文

    //数据文件名称
    DataSave_FileName_Type datasaveFileName;    //数据文件名称命名方式
    uchar   datasaveFileNameString[20];                   //文件名称指定字符串，最多16个半角字符

    //外部存储器保存
    quint8 datasaveExtMediaAuto;                //外部存储器自动保存开关，0-off, 1-on
    quint8 datasaveExtMediaFIFO;                //外部存储器FIFO开关，0-off, 1-on
    quint8 RESERVED[2];

    //文件格式
    DataSave_File_Format_Enum datasaveFormat;   //文件保存格式
}DATASAVE_CONFIG ;

#endif // CFGGLOBALDEF_DATASAVE_H
