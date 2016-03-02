/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：sqlTable_Message.h
 * 概   要：消息记录表相关数据结构声明
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
#include "cfgGlobalDef_Type.h"

#ifndef SQLTABLE_MESSAGE_H
#define SQLTABLE_MESSAGE_H

typedef enum Message_Type_Enum {
    Message_Type_Normal,            //字符串信息
    Message_Type_Freehand,         //手写信息
}Message_Type;

typedef struct MSG_DATA_STRUCT {
    Message_Type        msgType;        //消息类型
    quint8                     msgGrp;          //消息所属组，0--所有组，1-50:组号
    qint64                     msgTime;       //消息生成时间,从1970-01-01T00:00:00到现在的ms
    quint64                   msgID;            //消息ID号，对应主表中的主键ID值
    quint64                   msgSize;         //消息数据大小（byte）
    void *                     pMsgData;      //消息数据指针
}MSG_DATA;

#endif // SQLTABLE_MESSAGE_H
