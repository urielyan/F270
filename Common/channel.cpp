/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：Channel.cpp
 * 概   要：通道抽象类，提供基于通道的函数接口，以获取该通道的相关属性
 *
 * 当前版本：V1.0.0
 * 作   者：王 品
 * 完成日期：2015-10-20
 *
 * 修改版本：
 * 修改说明：
 * 修改作者：
 *
 ********************************************************************/
#include <QSharedMemory>

#include "channel.h"

Channel::Channel(quint32 chanCoder, QObject *parent) :
    QObject(parent),
    m_chanCoder(chanCoder)
{
}



