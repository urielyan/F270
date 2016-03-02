/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：EventCalendar.h
 * 概   要：日历事件接口，显示日历中存在的事件，以列表的形
 *                 式给出，用户单击列表事件时会跳转到趋势画面
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
#ifndef EVENTCALENDAR_H
#define EVENTCALENDAR_H

#include <QWidget>
#include <QModelIndex>
#include <QMap>
#include "./GlobalData/globalDef.h"
#include "./GlobalData/cfgGlobalDef.h"
class QFrame;
class EventCalendarModel;
class EventCalendarDelegate;
class EventCalendarView;
class QHBoxLayout;
class QVBoxLayout;
class QPushButton;
class QLabel;
class QTableWidget;
class QTableWidgetItem;
class EventListModel;
class EventListView;
class EventListDelegate;
class EventCalendar : public QWidget
{
    Q_OBJECT

public:
    EventCalendar(QWidget *parent = 0);
    void show();
    ~EventCalendar();

protected:
    void paintEvent(QPaintEvent *);

private slots:
    void slotPreYearButtonClicked();        /** 前一年按钮下押  **/
    void slotNextYearButtonClicked();       /** 下一年按钮下押  **/
    void slotPreMonthButtonClicked();       /** 前一月按钮下押     **/
    void slotNextMonthButtonClicked();      /** 下一月按钮下押  **/
    void slotEventViewReleased(QModelIndex index);      /** 日历界面按钮下押        **/
    void slotEventListClicked(QModelIndex index);       /** 事件显示界面按钮下押      **/
    void slotCalendarCloseClicked();        /**  日历界面关闭按钮下押       **/
    void slotEventListCloseClicked();       /**  事件显示界面关闭按钮下押       **/
    void slotDispButtonClicked();           /** Display 事件  **/
    void slotEventButtonClicked();          /** Event 事件  **/
    void slotListMouseMove();
    void slotOKClicked();
private:
    /** week 类型的枚举类型  **/
    enum {
        Sunday = 0,
        Monday,
        Tuesday,
        Wednesday,
        Thursday,
        Friday,
        Saturday
    };

    /** 日历界面的角色成员  **/
    enum {
        CalendarDataRole = 0,
        CanledarActive,
        CalendarPressRole,
        CalendarClickedRole,
        CalendarDispRole,
        CalendarEventRole
    };

    /** eventList 界面的角色成员  **/
    enum {
        MousePressRole = 0,
        MouseReleasedRole,
        MouseClickedRole,
        BeginTimeRole,
        EndTimeRole,
        BeginTimeStrRole,
        EndTimeStrRole
    };

    enum {
        LastDay = 0,
        CurrentDay,
        NextDay
    };
    void initialnation();       /** 初始化变量  **/
    void setWidgetSize();       /** 控件大小初始化  **/
    void setCalendarLayout();       /** 界面布局  **/
    void setWeekFromDay(bool sunday);       /** 判断起始日期是周一还是周末  **/
    void setDaysList();     /** 日历界面日历链表  **/
    void firstDayWeek(int day);     /** 根据当前条件判断，进行日期链表的插入操作  **/
    void setYearShow();         /** 日历时间显示  **/
    void setWidgetAttitute();       /** 控件属性设置  **/
    void queryEvent(QMap<qlonglong, qlonglong>& listMap, QString tableName);        /** 查询当前日历日期中的所有事件  **/
    void getEventInfo(QMap<qlonglong, qlonglong>& map, bool DispEvent);     /** 遍历当前日历事件，在日历中进行标记  **/
    void setBeginEndTime(QDateTime& begin, QDateTime& end);
    void finCurrentDay();
    void connectFunC();
private:
    int m_width, m_height;
    int m_currentYear, m_currentMonth, m_currentDay;
    int m_selectDayStatus;
    bool m_fromSunday;
    bool m_spanMonthEvent;
    bool m_isDisp;
    bool m_listMouseMove;
    QModelIndex m_selectIndex;
    QWidget* p_eventWidget;
    QWidget* p_eventShowWidget;
    QFrame* p_titleFrame;
    QFrame* p_eventTitleFrame;
    QFrame* p_buttonFrame;
    QFrame* p_bottomFrame;
    QTableWidget* p_weekView;
    EventListView* p_eventListView;
    EventCalendarModel* p_eventModel;
    EventCalendarDelegate* p_eventDelegate;
    EventCalendarView* p_eventView;
    QHBoxLayout* p_hLayout;
    QVBoxLayout* p_vLayout;
    QPushButton* p_closeButton;
    QPushButton* p_NextYearButton;
    QPushButton* p_PreYearButton;
    QLabel* p_currentYearLabel;
    QPushButton* p_NextMonthButton;
    QPushButton* p_PreMonthButton;
    QLabel* p_currentMonthLabel;
    QLabel* p_PreYearLabel;
    QLabel* p_NextYearLabel;
    QLabel* p_PreMonthLabel;
    QLabel* p_NextMonthLabel;
    QLabel* p_infoLabel;
    QLabel* p_timeLabel;
    QPushButton* p_eventButton;
    QPushButton* p_dispButton;
    QPushButton* p_OKButton;
    QPushButton* p_CancelButton;
    QTableWidgetItem* p_fisrtDay;
    QTableWidgetItem* p_secondDay;
    QTableWidgetItem* p_thridDay;
    QTableWidgetItem* p_fourDay;
    QTableWidgetItem* p_fivthDay;
    QTableWidgetItem* p_sixthDay;
    QTableWidgetItem* p_sunDay;
    EventListModel* p_eventListEventModel;
    EventListModel* p_eventListDispModel;
    EventListDelegate* p_eventListDelegate;
    QList<int>  m_daysList;
    QList<int> m_ifCurrentDays;

    QMap<qlonglong, qlonglong> m_DispMap;
    QMap<qlonglong, qlonglong> m_EventMap;
    QMap<qlonglong, qlonglong> m_DispSelectDayMap;
    QMap<qlonglong, qlonglong> m_EventSelectDayMap;
    QMap<qlonglong, qlonglong>::iterator m_EventIterator;
    DEVICE_CONFIG * p_deviceConfig;
};

#endif // EVENTCALENDAR_H
