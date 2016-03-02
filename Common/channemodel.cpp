/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：channelview.h
 * 概   要：自定义model，用于通道选择界面
 *
 * 当前版本：V1.0.0
 * 作   者：葛 海 浪
 * 完成日期：2016-1-5
 *
 * 修改版本：
 * 修改说明：
 * 修改作者：
 *
 ********************************************************************/
#include "channemodel.h"
#include <QDebug>
ChanneModel::ChanneModel(QObject *parent) :
    QAbstractTableModel(parent)
  , m_channelNums(0)
  , m_channulCount(0)
  , m_channelType(0)
{
    for(int i = 0; i < CHANNELNUMS; ++i) {
        m_PressStatus[i] = false;
        m_ClickeStatus[i] = false;
        m_currentNum[i] = 0;
    }
}

int ChanneModel::rowCount(const QModelIndex &/*parent*/) const
{
    int row = m_channelNums - (m_channelNums / 10) * 10;
    if(0 == row) {
        return m_channelNums / 10;
    } else {
        return (m_channelNums / 10 + 1);
    }
}

int ChanneModel::columnCount(const QModelIndex &/*parent*/) const
{
    return 10;
}

QVariant ChanneModel::data(const QModelIndex &index, int role) const
{
    switch (role) {
    case ChannelNameRole:
        return m_ChannelName.at(index.row() * 10 + index.column());
        break;
    case ChannelPressRole:
        return m_PressStatus[index.row() * 10 + index.column()];
        break;
    case ChannelClickedRole:
        return m_ClickeStatus[index.row() * 10 + index.column()];
        break;
    case ChannelSequnceRole:
        return m_currentNum[index.row() * 10 + index.column()];
        break;
    case ChannelType:
        return m_channelType;
        break;
    default:
        return QVariant();
        break;
    }
}

bool ChanneModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(index.isValid() && role == ChannelPressRole) {
        m_PressStatus[index.row() * 10 + index.column()] = value.toBool();
        return true;
    } else if(index.isValid() && role == ChannelClickedRole) {
        m_ClickeStatus[index.row() * 10 + index.column()] = value.toBool();
        return true;
    } else if(index.isValid() && role == ChannelSequnceRole){
        if(value.toInt() == -2) {
            ++m_channulCount;
            m_currentNum[index.row() * 10 + index.column()] = m_channulCount;
        } else if(value.toInt() == -1) {
            if(--m_channulCount < 0)
                m_channulCount = 0;
            m_currentNum[index.row() * 10 + index.column()] = 0;
        } else {
            m_currentNum[index.row() * 10 + index.column()] = value.toInt();
        }
    }
    return false;
}

void ChanneModel::setChannums(int nums)
{
    m_channelNums = nums;
}

void ChanneModel::setChannelType(int Type)
{
    m_channelType = Type;
}

void ChanneModel::setChannelListName(const QMap<QString, quint32> &map)
{
    QMap<QString, quint32>::const_iterator it;
    for(it = map.begin(); it != map.end(); ++it)
        m_ChannelName.append(it.key());
}

void ChanneModel::setBeginID(int id)
{
    m_channulCount = id;
}

int ChanneModel::getSelectNums()
{
    int m_count = 0;
    for(int i = 0; i < m_channelNums; ++i) {
        if(this->data(this->index(i / 11, i % 11), ChannelClickedRole).toBool())
            ++m_count;
    }
    return m_count;
}

void ChanneModel::initiaulzation()
{
    for(int i = 0; i < CHANNELNUMS; ++i) {
        m_PressStatus[i] = false;
        m_ClickeStatus[i] = false;
        m_currentNum[i] = 0;
    }
}
