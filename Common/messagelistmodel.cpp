/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：MessageListModel.cpp
 * 概   要：信息显示接口的模型
 *
 * 当前版本：V1.0.0
 * 作   者：葛 海 浪
 * 完成日期：2016-2-22
 *
 * 修改版本：
 * 修改说明：
 * 修改作者：
 *
 ********************************************************************/
#include "messagelistmodel.h"

MessageListModel::MessageListModel(QObject *parent) :
    QAbstractTableModel(parent)
  , m_rowCount(0)
{

}

int MessageListModel::rowCount(const QModelIndex &/*parent*/) const
{
    return m_rowCount;
}

int MessageListModel::columnCount(const QModelIndex &/*parent*/) const
{
    return 1;
}

QVariant MessageListModel::data(const QModelIndex &index, int role) const
{
    switch (role) {
    case SequnceRole:
        return QString(tr("Message%1")).arg(index.row() + 1);
    case MessageRole:
        return m_strMessage[index.row()];
    case MousePressRole:
        return m_mousePress[index.row()];
    case MouseClickedRole:
        return m_mouseClicked[index.row()];
    default:
        return QVariant();
        break;
    }
}

bool MessageListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(index.isValid()) {
        switch (role) {
        case MessageRole:
            m_strMessage[index.row()] = value.toString();
            return true;
        case MousePressRole:
            m_mousePress[index.row()] = value.toBool();
            return true;
        case MouseClickedRole:
            m_mouseClicked[index.row()] = value.toBool();
            return true;
        default:
            return false;
        }
    }
    return false;
}

void MessageListModel::setRowCount(int row)
{
    m_rowCount = row;
}

void MessageListModel::setMessage(int i, QString message)
{
    m_strMessage[i] = message;
}

QModelIndex MessageListModel::getDefaultIndex()
{
    return this->index(0, 0);
}

void MessageListModel::init()
{
    beginResetModel();
    for(int i = 0; i < MAXVALUE; ++i) {
        m_strMessage[i] = "";
        m_mousePress[i] = false;
        m_mouseClicked[i] = false;
    }
    endResetModel();
}
