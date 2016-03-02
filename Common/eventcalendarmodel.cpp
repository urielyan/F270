/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：EventCalendarModel.h
 * 概   要：日历事件接口中日历界面显示的模型
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
#include "eventcalendarmodel.h"

EventCalendarModel::EventCalendarModel(QObject *parent) :
    QAbstractTableModel(parent)
{
    setDefault();
}

int EventCalendarModel::rowCount(const QModelIndex &/*parent*/) const
{
    return 6;
}

int EventCalendarModel::columnCount(const QModelIndex &/*parent*/) const
{
    return 7;
}

QVariant EventCalendarModel::data(const QModelIndex &index, int role) const
{
    switch (role) {
    case CalendarDataRole:
        return m_daysList[index.row() * 7 + index.column()];
        break;
    case CanledarActive:
        return m_ifCurrentDays[index.row() * 7 + index.column()];
        break;
    case CalendarPressRole:
        return m_pressed[index.row() * 7 + index.column()];
        break;
    case CalendarClickedRole:
        return m_clicked[index.row() * 7 + index.column()];
        break;
    case CalendarDispRole:
        return m_Disp[index.row() * 7 + index.column()];
        break;
    case CalendarEventRole:
        return m_Event[index.row() * 7 + index.column()];
        break;
    default:
        return QVariant();
        break;
    }
}

bool EventCalendarModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(index.isValid()) {
        switch (role) {
        case CalendarPressRole:
            m_pressed[index.row() * 7 + index.column()] = value.toBool();
            return true;
        case CalendarClickedRole:
            m_clicked[index.row() * 7 + index.column()] = value.toBool();
            return true;
        case CalendarDispRole:
            m_Disp[index.row() * 7 + index.column()] = value.toBool();
            return true;
        case CalendarEventRole:
            m_Event[index.row() * 7 + index.column()] = value.toBool();
            return true;
        default:
            return false;
        }
    }
    return false;
}

bool EventCalendarModel::setDispShow(const QModelIndex &index, bool disp)
{
    return const_cast<QAbstractItemModel*>(index.model())->setData(index, disp, CalendarDispRole);
}

bool EventCalendarModel::setEventShow(const QModelIndex &index, bool event)
{
    return const_cast<QAbstractItemModel*>(index.model())->setData(index, event, CalendarEventRole);
}

void EventCalendarModel::setDaysList(QList<int> dayList)
{
    for(int i = 0; i < 42; ++i)
        m_daysList[i] = dayList.at(i);
    emit dataChanged(this->index(0, 0), this->index(5, 6));
}

void EventCalendarModel::setDefault()
{
    for(int i = 0; i < 42; ++i) {
        m_ifCurrentDays[i] = -1;
        m_clicked[i] = false;
        m_pressed[i] = false;
        m_Disp[i] = false;
        m_Event[i] = false;
    }
}

void EventCalendarModel::setCurrentDays(QList<int> currentList)
{
    for(int i = 0; i < 42; ++i)
        m_ifCurrentDays[i] = currentList.at(i);
    emit dataChanged(this->index(0, 0), this->index(5, 6));
}
