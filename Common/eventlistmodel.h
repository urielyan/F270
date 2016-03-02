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
#ifndef EVENTLISTMODEL_H
#define EVENTLISTMODEL_H

#include <QAbstractTableModel>
#include <QDateTime>
class EventListModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit EventListModel(QObject *parent = 0);
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    void setRowCount(int rowCount);
    void setTimeData(QMap<qlonglong , qlonglong>& map);

private:
    enum {
        MousePressRole = 0,
        MouseReleasedRole,
        MouseClickedRole,
        BeginTimeRole,
        EndTimeRole,
        BeginTimeStrRole,
        EndTimeStrRole
    };
private:
    int m_rowCount;
    QStringList p_beginTimeList;
    QStringList p_endTimeList;
    QStringList p_beginTimeStrList;
    QStringList p_endTimeStrList;
    QList<bool> m_pressed;
    QList<bool> m_clicked;
    QDateTime m_beginTime;
    QDateTime m_endTime;
};

#endif // EVENTLISTMODEL_H
