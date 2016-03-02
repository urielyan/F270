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
#ifndef EVENTCALENDARMODEL_H
#define EVENTCALENDARMODEL_H

#include <QAbstractTableModel>

class EventCalendarModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit EventCalendarModel(QObject *parent = 0);
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    bool setDispShow(const QModelIndex &index, bool disp);      /** 设置Disp事件  **/
    bool setEventShow(const QModelIndex &index, bool event);        /** 设置Event事件  **/
    void setDaysList(QList<int> dayList);       /** 设置日期链表  **/
    void setDefault();      /** 初始化  **/
    void setCurrentDays(QList<int> currentList);        /** 设置日期状态  **/
signals:

public slots:

private:
    enum {
        CalendarDataRole = 0,
        CanledarActive,
        CalendarPressRole,
        CalendarClickedRole,
        CalendarDispRole,
        CalendarEventRole
    };

private:
    int  m_daysList[42];
    int m_ifCurrentDays[42];
    bool m_clicked[42];
    bool m_pressed[42];
    bool m_Disp[42];
    bool m_Event[42];
};

#endif // EVENTCALENDARMODEL_H
