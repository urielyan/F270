/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：EventListModel.h
 * 概   要：日历事件接口中事件一览显示的model
 *
 * 当前版本：V1.0.0
 * 作   者：葛 海 浪
 * 完成日期：2016-1-21
 *
 * 修改版本：
 * 修改说明：
 * 修改作者：
 *
 ********************************************************************/
#include "eventlistmodel.h"
#include <QDateTime>
EventListModel::EventListModel(QObject *parent) :
    QAbstractTableModel(parent)
  , m_rowCount(0)
{
}

int EventListModel::rowCount(const QModelIndex &/*parent*/) const
{
    return m_rowCount;
}

int EventListModel::columnCount(const QModelIndex &/*parent*/) const
{
    return 1;
}

QVariant EventListModel::data(const QModelIndex &index, int role) const
{
    switch (role) {
    case MousePressRole:
        return m_pressed.at(index.row());
    case MouseClickedRole:
        return m_clicked.at(index.row());
    case BeginTimeRole:
        return p_beginTimeList.at(index.row());
    case EndTimeRole:
        return p_endTimeList.at(index.row());
    case BeginTimeStrRole:
        return p_beginTimeStrList.at(index.row());
    case EndTimeStrRole:
        return p_endTimeStrList.at(index.row());
    default:
        return QVariant();
    }
}

bool EventListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(index.isValid()) {
        switch (role) {
        case MousePressRole:
            m_pressed.replace(index.row(), value.toBool());
            return true;
        case MouseClickedRole:
            m_clicked.replace(index.row(), value.toBool());
            return true;
        case BeginTimeRole:
            p_beginTimeList.replace(index.row(), value.toString());
            return true;
        case EndTimeRole:
            p_endTimeList.replace(index.row(), value.toString());
            return true;
        case BeginTimeStrRole:
            p_beginTimeStrList.replace(index.row(), value.toString());
            return true;
        case EndTimeStrRole:
            p_endTimeStrList.replace(index.row(), value.toString());
            return true;
        default:
            return false;
        }
    }
    return false;
}

void EventListModel::setRowCount(int rowCount)
{
    p_beginTimeList.clear();
    p_endTimeList.clear();
    p_beginTimeStrList.clear();
    p_endTimeStrList.clear();
    m_rowCount = rowCount;
    for(int i = 0; i < m_rowCount; ++i) {
        p_beginTimeList.append("");
        p_endTimeList.append("");
        m_pressed.append(false);
        m_clicked.append(false);
        p_beginTimeStrList.append("");
        p_endTimeStrList.append("");
    }
}
#include <QDebug>
void EventListModel::setTimeData(QMap<qlonglong , qlonglong>& map)
{
    int count = 0;
    QMap<qlonglong, qlonglong>::iterator iter;
    for(iter = map.begin(); iter != map.end(); ++iter) {
        m_beginTime.setMSecsSinceEpoch(iter.key());
        m_endTime.setMSecsSinceEpoch(iter.value());
        QModelIndex modelIndex = this->index(count, 0);
        const_cast<QAbstractItemModel*>(modelIndex.model())->setData(modelIndex, QString("%1").arg(iter.key()), BeginTimeRole);
        const_cast<QAbstractItemModel*>(modelIndex.model())->setData(modelIndex, QString("%1").arg(iter.value()), EndTimeRole);
        const_cast<QAbstractItemModel*>(modelIndex.model())->setData(modelIndex, \
                                                                     m_beginTime.toString("yyyy/MM/dd  hh : mm : ss"), BeginTimeStrRole);
        const_cast<QAbstractItemModel*>(modelIndex.model())->setData(modelIndex, \
                                                                     m_endTime.toString("yyyy/MM/dd  hh : mm : ss"), EndTimeStrRole);
        ++count;
    }
}
