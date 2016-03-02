/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：EventCalendar.cpp
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
#include "eventcalendar.h"
#include "eventcalendardelegate.h"
#include "eventcalendarmodel.h"
#include "eventcalendarview.h"
#include "eventlistdelegate.h"
#include "eventlistmodel.h"
#include "eventlistview.h"
#include "Threads/threaddatarecord.h"
#include "./WorkWin/wintrendgrouppanel.h"
#include "./WorkWin/winmainframe.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QPainter>
#include <QFrame>
#include <QTableView>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QStandardItemModel>
#include <QPushButton>
#include <QLabel>
#include <QTableWidget>
#include <QDebug>
#include <QHeaderView>
#include <QTableWidgetItem>
#include <QDate>
#include <QScrollBar>
EventCalendar::EventCalendar(QWidget *parent)
    : QWidget(parent)
    , m_width(0), m_height(0)
    , m_currentYear(0)
    , m_currentMonth(0)
    , m_currentDay(0)
    , m_selectDayStatus(0)
    , m_fromSunday(true)
    , m_spanMonthEvent(false)
    , m_isDisp(true)
    , m_listMouseMove(true)
    , p_eventWidget(NULL)
    , p_eventShowWidget(NULL)
    , p_titleFrame(NULL)
    , p_eventTitleFrame(NULL)
    , p_buttonFrame(NULL)
    , p_bottomFrame(NULL)
    , p_weekView(NULL)
    , p_eventListView(NULL)
    , p_eventModel(NULL)
    , p_eventDelegate(NULL)
    , p_eventView(NULL)
    , p_hLayout(NULL)
    , p_vLayout(NULL)
    , p_closeButton(NULL)
    , p_NextYearButton(NULL)
    , p_PreYearButton(NULL)
    , p_currentYearLabel(NULL)
    , p_NextMonthButton(NULL)
    , p_PreMonthButton(NULL)
    , p_currentMonthLabel(NULL)
    , p_PreYearLabel(NULL)
    , p_NextYearLabel(NULL)
    , p_PreMonthLabel(NULL)
    , p_NextMonthLabel(NULL)
    , p_infoLabel(NULL)
    , p_timeLabel(NULL)
    , p_eventButton(NULL)
    , p_dispButton(NULL)
    , p_OKButton(NULL)
    , p_CancelButton(NULL)
    , p_fisrtDay(NULL)
    , p_secondDay(NULL)
    , p_thridDay(NULL)
    , p_fourDay(NULL)
    , p_fivthDay(NULL)
    , p_sixthDay(NULL)
    , p_sunDay(NULL)
    , p_eventListEventModel(NULL)
    , p_eventListDispModel(NULL)
    , p_eventListDelegate(NULL)
{
    initialnation();
    setWidgetSize();
    setCalendarLayout();
    setWidgetAttitute();
    connectFunC();
}

void EventCalendar::show()
{
    /** 获取系统配置中日期显示的信息  **/
    p_deviceConfig = DevConfigPtr;
    int num = p_deviceConfig->displayBasic.basicFirstWeekday;
    if(0 == num) {
        m_fromSunday = true;
    } else {
        m_fromSunday = false;
    }
    m_listMouseMove = true;
    m_currentYear = QDate::currentDate().year() - 1;
    m_currentMonth = QDate::currentDate().month();
    m_currentDay = QDate::currentDate().day();
    slotNextYearButtonClicked();
    setWeekFromDay(m_fromSunday);
    finCurrentDay();
    QWidget::show();
}

EventCalendar::~EventCalendar()
{

}

void EventCalendar::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.fillRect(this->rect(), QColor(20, 20, 20, 200));
}

void EventCalendar::slotPreYearButtonClicked()
{
    /** 日期回退最晚到1970年  **/
    if(--m_currentYear < 1970)
        m_currentYear = 1970;
    setYearShow();
    setDaysList();
    p_eventModel->setDefault();
    p_eventModel->setDaysList(m_daysList);
    p_eventModel->setCurrentDays(m_ifCurrentDays);
    getEventInfo(m_DispMap, true);
    getEventInfo(m_EventMap, false);
    if(-1 == m_selectDayStatus) {
        m_currentDay = 1;
        finCurrentDay();
    } else if(0 == m_selectDayStatus) {
        finCurrentDay();
    } else {
        QDate date(m_currentYear, m_currentMonth, m_currentDay);
        m_currentDay = date.daysInMonth();
        finCurrentDay();
    }
}

void EventCalendar::slotNextYearButtonClicked()
{
    ++m_currentYear;
    setYearShow();
    setDaysList();
    p_eventModel->setDefault();
    p_eventModel->setDaysList(m_daysList);
    p_eventModel->setCurrentDays(m_ifCurrentDays);
    getEventInfo(m_DispMap, true);
    getEventInfo(m_EventMap, false);
    if(-1 == m_selectDayStatus) {
        m_currentDay = 1;
        finCurrentDay();
    } else if(0 == m_selectDayStatus) {
        finCurrentDay();
    } else {
        QDate date(m_currentYear, m_currentMonth, m_currentDay);
        m_currentDay = date.daysInMonth();
        finCurrentDay();
    }
}

void EventCalendar::slotPreMonthButtonClicked()
{
    if(--m_currentMonth < 1) {
        m_currentMonth = 12;
        if(--m_currentYear < 1970)
            m_currentYear = 1970;
    }
    setYearShow();
    setDaysList();
    p_eventModel->setDefault();
    p_eventModel->setDaysList(m_daysList);
    p_eventModel->setCurrentDays(m_ifCurrentDays);
    getEventInfo(m_DispMap, true);
    getEventInfo(m_EventMap, false);
    if(-1 == m_selectDayStatus) {
        m_currentDay = 1;
        finCurrentDay();
    } else if(0 == m_selectDayStatus) {
        finCurrentDay();
    } else {
        QDate date(m_currentYear, m_currentMonth, m_currentDay);
        m_currentDay = date.daysInMonth();
        finCurrentDay();
    }
}

void EventCalendar::slotNextMonthButtonClicked()
{
    if(++m_currentMonth > 12) {
        m_currentMonth = 1;
        ++m_currentYear;
    }
    setYearShow();
    setDaysList();
    p_eventModel->setDefault();
    p_eventModel->setDaysList(m_daysList);
    p_eventModel->setCurrentDays(m_ifCurrentDays);
    getEventInfo(m_DispMap, true);
    getEventInfo(m_EventMap, false);
    if(-1 == m_selectDayStatus) {
        m_currentDay = 1;
        finCurrentDay();
    } else if(0 == m_selectDayStatus) {
        finCurrentDay();
    } else {
        QDate date(m_currentYear, m_currentMonth, m_currentDay);
        m_currentDay = date.daysInMonth();
        finCurrentDay();
    }
}

void EventCalendar::slotEventViewReleased(QModelIndex index)
{
    m_currentDay = index.data(CalendarDataRole).toInt();
    m_selectDayStatus = index.data(CanledarActive).toInt();
    p_eventListEventModel = new EventListModel(p_eventShowWidget);
    p_eventListDispModel = new EventListModel(p_eventShowWidget);
    m_DispSelectDayMap.clear();
    m_EventSelectDayMap.clear();
    QDateTime beginDateTime;
    QDateTime endDateTime;
    p_eventListView->setModelIndexDefault();
    setBeginEndTime(beginDateTime, endDateTime);
    p_timeLabel->setText(QString("%1/%2/%3").arg(beginDateTime.date().year())\
                         .arg(beginDateTime.date().month()).arg(beginDateTime.date().day()));
    if(index.data(CalendarDispRole).toBool() || index.data(CalendarEventRole).toBool()) {
        p_eventWidget->setHidden(true);
        p_eventShowWidget->setHidden(false);
        /** get DIsp Map  **/
        if(0 != m_DispMap.size()) {
            for(m_EventIterator = m_DispMap.begin(); m_EventIterator != m_DispMap.end(); ++m_EventIterator) {
                if((m_EventIterator.key() >= beginDateTime.toMSecsSinceEpoch() && \
                    m_EventIterator.key() <= endDateTime.toMSecsSinceEpoch()) || \
                        (m_EventIterator.value() >= beginDateTime.toMSecsSinceEpoch() && \
                         m_EventIterator.value() <= endDateTime.toMSecsSinceEpoch()) || \
                        (m_EventIterator.key() < beginDateTime.toMSecsSinceEpoch() && \
                         m_EventIterator.value() > endDateTime.toMSecsSinceEpoch())) {
                    m_DispSelectDayMap.insert(m_EventIterator.key(), m_EventIterator.value());
                }
            }
        } else {
            /** do nothing **/
        }

        /** get Event Map **/
        if(0 != m_EventMap.size()) {
            for(m_EventIterator = m_EventMap.begin(); m_EventIterator != m_EventMap.end(); ++m_EventIterator) {
                if((m_EventIterator.key() >= beginDateTime.toMSecsSinceEpoch() && \
                    m_EventIterator.key() <= endDateTime.toMSecsSinceEpoch()) || \
                        (m_EventIterator.value() >= beginDateTime.toMSecsSinceEpoch() && \
                         m_EventIterator.value() <= endDateTime.toMSecsSinceEpoch()) || \
                        (m_EventIterator.key() < beginDateTime.toMSecsSinceEpoch() && \
                         m_EventIterator.value() > endDateTime.toMSecsSinceEpoch())) {
                    m_EventSelectDayMap.insert(m_EventIterator.key(), m_EventIterator.value());
                }
            }
        } else {
            /** do nothing **/
        }
        if(0 == m_DispSelectDayMap.size()) {
            m_isDisp = false;
            p_dispButton->setEnabled(false);
            slotEventButtonClicked();
        } else {
            m_isDisp = true;
            p_dispButton->setEnabled(true);
            slotDispButtonClicked();
        }
        if(0 == m_EventSelectDayMap.size()) {
            p_eventButton->setEnabled(false);
        } else {
            p_eventButton->setEnabled(true);
        }
    } else {
        /** do nothing **/
    }
}

void EventCalendar::slotEventListClicked(QModelIndex index)
{
    m_listMouseMove = false;
    m_selectIndex = index;
}

void EventCalendar::slotCalendarCloseClicked()
{
    this->close();
}

void EventCalendar::slotEventListCloseClicked()
{
    m_listMouseMove = true;
    p_eventShowWidget->setHidden(true);
    p_eventWidget->setHidden(false);
    delete p_eventListEventModel;
    delete p_eventListDispModel;
}

void EventCalendar::slotDispButtonClicked()
{
    p_dispButton->setChecked(true);
    p_eventButton->setChecked(false);
    m_isDisp = true;
    /** 设置model的行数  **/
    p_eventListDispModel->setRowCount(m_DispSelectDayMap.size());

    /** 遍历map，显示开始时间和结束时间  **/
    p_eventListDispModel->setTimeData(m_DispSelectDayMap);
    p_eventListView->setModel(p_eventListDispModel);
}

void EventCalendar::slotEventButtonClicked()
{
    p_dispButton->setChecked(false);
    p_eventButton->setChecked(true);
    m_isDisp = false;
    p_eventListEventModel->setRowCount(m_EventSelectDayMap.size());
    p_eventListEventModel->setTimeData(m_EventSelectDayMap);
    p_eventListView->setModel(p_eventListEventModel);
    p_eventListView->verticalScrollBar()->setValue(0);
}

void EventCalendar::slotListMouseMove()
{
    m_listMouseMove = true;
}

void EventCalendar::slotOKClicked()
{
    if(m_listMouseMove) {
        /** do nothing **/
    } else {
        m_listMouseMove = true;
        qlonglong beginTime = m_selectIndex.data(BeginTimeRole).toLongLong();
        qlonglong endTime = m_selectIndex.data(EndTimeRole).toLongLong();
        quint64 ID;
        int EndStatus;
        QString tableName;
        /** 画面跳转到趋势图信息 **/
        if(m_isDisp) {
            tableName = "table_Main_Disp";
        } else {
            tableName = "table_Main_Event";
        }
        QSqlQuery query(QString("select ID , EndStatus from %1 where StartTime = %2").arg(tableName).arg(beginTime), \
                        *ThreadDataRecord::instance()->getDatabaseConnect());
        query.exec();
        ThreadDataRecord::instance()->putDatabaseConnect();
        if(query.next()) {
            ID = query.value(0).toLongLong();
            EndStatus = query.value(1).toInt();
        }

        if(EndStatus != 2) {    /** 如果当前记录不是正在进行的记录，跳转到记录画面  **/
            WinTrendGroupPanel * groupPanel = WinMainFrame::instance()->getTrendGroup();
            this->close();
            groupPanel->switchToHistoryReview(m_isDisp, ID, endTime);
            WinMainFrame::instance()->slotFrameChanged(Win_Trend_Panel);
        }
    }
}

void EventCalendar::initialnation()
{
    m_width = QApplication::desktop()->width();
    m_height = QApplication::desktop()->height();
    this->resize(m_width, m_height);
    this->setAttribute(Qt::WA_TranslucentBackground, true);
    QDate::currentDate().getDate(&m_currentYear, &m_currentMonth, &m_currentDay);

    p_eventWidget = new QWidget(this);
    p_eventShowWidget = new QWidget(this);
    p_eventShowWidget->setHidden(true);
    p_titleFrame = new QFrame(p_eventWidget);
    p_eventTitleFrame = new QFrame(p_eventShowWidget);
    p_buttonFrame = new QFrame(p_eventWidget);
    p_bottomFrame = new QFrame(p_eventShowWidget);
    p_weekView = new QTableWidget(p_eventWidget);
    p_eventListView = new EventListView(p_eventShowWidget);
    p_eventModel = new EventCalendarModel(p_eventWidget);
    p_eventDelegate = new EventCalendarDelegate(p_eventWidget);
    p_eventView = new EventCalendarView(p_eventWidget);

    p_eventListDelegate = new EventListDelegate(p_eventShowWidget);
    p_hLayout = new QHBoxLayout(p_eventWidget);
    p_vLayout = new QVBoxLayout;
    p_closeButton = new QPushButton(p_titleFrame);
    p_NextYearButton = new QPushButton(p_buttonFrame);
    p_PreYearButton = new QPushButton(p_buttonFrame);
    p_currentYearLabel = new QLabel(p_buttonFrame);
    p_NextMonthButton = new QPushButton(p_buttonFrame);
    p_PreMonthButton = new QPushButton(p_buttonFrame);
    p_currentMonthLabel = new QLabel(p_buttonFrame);
    p_PreYearLabel = new QLabel(p_buttonFrame);
    p_NextYearLabel = new QLabel(p_buttonFrame);
    p_PreMonthLabel = new QLabel(p_buttonFrame);
    p_NextMonthLabel = new QLabel(p_buttonFrame);
    p_infoLabel = new QLabel(p_titleFrame);
    p_timeLabel = new QLabel(p_eventShowWidget);
    p_eventButton = new QPushButton(p_eventTitleFrame);
    p_dispButton = new QPushButton(p_eventTitleFrame);
    p_OKButton = new QPushButton(p_bottomFrame);
    p_CancelButton = new QPushButton(p_bottomFrame);
    p_fisrtDay = new QTableWidgetItem;
    p_secondDay = new QTableWidgetItem;
    p_thridDay = new QTableWidgetItem;
    p_fourDay = new QTableWidgetItem;
    p_fivthDay = new QTableWidgetItem;
    p_sixthDay = new QTableWidgetItem;
    p_sunDay = new QTableWidgetItem;
    p_eventListView->setItemDelegate(p_eventListDelegate);
    p_infoLabel->setObjectName("TitleInfo");
    p_infoLabel->setText(tr("Event List"));
    p_dispButton->setText(tr("Display"));
    p_eventButton->setText(tr("Event"));
    p_dispButton->setObjectName("DispButton");
    p_eventButton->setObjectName("EventButton");
    p_titleFrame->setObjectName("TitleFrame");
    p_eventTitleFrame->setObjectName("EventTitleFrame");
    p_weekView->setObjectName("WeekView");
    p_buttonFrame->setObjectName("ButtonFrame");
    p_PreYearButton->setObjectName("PreYearButton");
    p_NextYearButton->setObjectName("NextYearButton");
    p_PreMonthButton->setObjectName("PreMonthButton");
    p_NextMonthButton->setObjectName("NextMonthButton");
    p_currentYearLabel->setObjectName("CurrentYearLabel");
    p_currentMonthLabel->setObjectName("CurrentMonthLabel");
    p_timeLabel->setObjectName("TimeLabel");
    p_bottomFrame->setObjectName("BottomFrame");
    p_OKButton->setText(tr("OK"));
    p_OKButton->setObjectName("OKButton");
    p_CancelButton->setText(tr("Cancel"));
    p_CancelButton->setObjectName(tr("CancelButton"));
    p_eventView->setItemDelegate(p_eventDelegate);
    p_eventView->setModel(p_eventModel);

    p_timeLabel->setAlignment(Qt::AlignCenter);
}

void EventCalendar::setWidgetSize()
{
    p_eventWidget->setGeometry(0, 0, m_width, m_height);
    p_eventShowWidget->setGeometry(0, 0, m_width, m_height);
    p_titleFrame->setMaximumSize((int)(m_width * 0.68), (int)(m_height * 0.075));
    p_titleFrame->setMinimumSize((int)(m_width * 0.65), (int)(m_height * 0.072));

    p_eventTitleFrame->setMaximumSize((int)(m_width * 0.55), (int)(m_height * 0.075));
    p_eventTitleFrame->setMinimumSize((int)(m_width * 0.5), (int)(m_height * 0.072));

    p_timeLabel->setMaximumSize((int)(m_width * 0.55), (int)(m_height * 0.068));
    p_timeLabel->setMinimumSize((int)(m_width * 0.5), (int)(m_height * 0.063));

    p_eventButton->setMaximumSize((int)(m_width * 0.075), (int)(m_height * 0.068));
    p_eventButton->setMinimumSize((int)(m_width * 0.07), (int)(m_height * 0.063));

    p_bottomFrame->setMaximumSize((int)(m_width * 0.55), (int)(m_height * 0.12));
    p_bottomFrame->setMinimumSize((int)(m_width * 0.5), (int)(m_height * 0.1));

    p_dispButton->setMaximumSize((int)(m_width * 0.075), (int)(m_height * 0.068));
    p_dispButton->setMinimumSize((int)(m_width * 0.07), (int)(m_height * 0.063));

    p_buttonFrame->setMaximumSize((int)(m_width * 0.68), (int)(m_height * 0.096));
    p_buttonFrame->setMinimumSize((int)(m_width * 0.65), (int)(m_height * 0.093));

    p_weekView->setMaximumSize((int)(m_width * 0.68), (int)(m_height * 0.045));
    p_weekView->setMinimumSize((int)(m_width * 0.65), (int)(m_height * 0.043));

    p_eventListView->setMaximumSize((int)(m_width * 0.55), (int)(m_height * 0.5));
    p_eventListView->setMinimumSize((int)(m_width * 0.5), (int)(m_height * 0.45));

    p_eventView->setMaximumSize((int)(m_width * 0.68), (int)(m_height * 0.56));
    p_eventView->setMinimumSize((int)(m_width * 0.65), (int)(m_height * 0.54));

    p_closeButton->setMaximumSize((int)(m_width * 0.056), (int)(m_height * 0.075));
    p_closeButton->setMinimumSize((int)(m_width * 0.053), (int)(m_height * 0.072));
    p_closeButton->setFocusPolicy(Qt::NoFocus);
    p_closeButton->setIcon(QIcon(":/calendar/Image/calendar/list_close.png"));
    p_closeButton->setIconSize(QSize(p_closeButton->width(), p_closeButton->height()));

    p_NextYearButton->setMaximumSize((int)(m_width * 0.038), (int)(m_height * 0.05));
    p_NextYearButton->setMinimumSize((int)(m_width * 0.035), (int)(m_height * 0.048));
    p_NextYearButton->setIcon(QIcon(":/calendar/Image/calendar/right_icon.png"));
    p_NextYearButton->setIconSize(QSize(p_NextYearButton->width(), p_NextYearButton->height()));

    p_PreYearButton->setMaximumSize((int)(m_width * 0.038), (int)(m_height * 0.05));
    p_PreYearButton->setMinimumSize((int)(m_width * 0.035), (int)(m_height * 0.048));
    p_PreYearButton->setIcon(QIcon(":/calendar/Image/calendar/left_icon.png"));
    p_PreYearButton->setIconSize(QSize(p_PreYearButton->width(), p_PreYearButton->height()));

    p_NextMonthButton->setMaximumSize((int)(m_width * 0.038), (int)(m_height * 0.05));
    p_NextMonthButton->setMinimumSize((int)(m_width * 0.035), (int)(m_height * 0.048));
    p_NextMonthButton->setIcon(QIcon(":/calendar/Image/calendar/right_icon.png"));
    p_NextMonthButton->setIconSize(QSize(p_NextMonthButton->width(), p_NextMonthButton->height()));

    p_PreMonthButton->setMaximumSize((int)(m_width * 0.038), (int)(m_height * 0.05));
    p_PreMonthButton->setMinimumSize((int)(m_width * 0.035), (int)(m_height * 0.048));
    p_PreMonthButton->setIcon(QIcon(":/calendar/Image/calendar/left_icon.png"));
    p_PreMonthButton->setIconSize(QSize(p_PreMonthButton->width(), p_PreMonthButton->height()));

    p_currentYearLabel->setMaximumSize((int)(m_width * 0.08), (int)(m_height * 0.05));
    p_currentYearLabel->setMinimumSize((int)(m_width * 0.07), (int)(m_height * 0.048));

    p_currentMonthLabel->setMaximumSize((int)(m_width * 0.07), (int)(m_height * 0.05));
    p_currentMonthLabel->setMinimumSize((int)(m_width * 0.06), (int)(m_height * 0.048));

    p_PreYearLabel->setMaximumSize((int)(m_width * 0.038), (int)(m_height * 0.025));
    p_PreYearLabel->setMinimumSize((int)(m_width * 0.035), (int)(m_height * 0.024));

    p_NextYearLabel->setMaximumSize((int)(m_width * 0.037), (int)(m_height * 0.025));
    p_NextYearLabel->setMinimumSize((int)(m_width * 0.035), (int)(m_height * 0.024));

    p_PreMonthLabel->setMaximumSize((int)(m_width * 0.038), (int)(m_height * 0.025));
    p_PreMonthLabel->setMinimumSize((int)(m_width * 0.035), (int)(m_height * 0.024));

    p_NextMonthLabel->setMaximumSize((int)(m_width * 0.038), (int)(m_height * 0.025));
    p_NextMonthLabel->setMinimumSize((int)(m_width * 0.035), (int)(m_height * 0.024));

    p_OKButton->setMinimumSize(QSize(m_width / 9,m_height / 14));
    p_OKButton->setMaximumSize(QSize(m_width / 8, m_height / 13));

    p_CancelButton->setMinimumSize(QSize(m_width / 9,m_height / 14));
    p_CancelButton->setMaximumSize(QSize(m_width / 8, m_height / 13));

    p_infoLabel->setMaximumSize((int)(m_width * 0.16), (int)(m_height * 0.075));
    p_infoLabel->setMinimumSize((int)(m_width * 0.145), (int)(m_height * 0.072));
}

void EventCalendar::setCalendarLayout()
{
    QHBoxLayout* titleLayout = new QHBoxLayout(p_titleFrame);
    titleLayout->addSpacing(10);
    titleLayout->addWidget(p_infoLabel);
    titleLayout->addStretch(0);
    titleLayout->addWidget(p_closeButton);
    titleLayout->setSizeConstraint(QLayout::SetMinimumSize);

    QHBoxLayout* listTitleLayout = new QHBoxLayout(p_eventTitleFrame);
    listTitleLayout->setSpacing(0);
    listTitleLayout->addSpacing(20);
    listTitleLayout->addWidget(p_dispButton);
    listTitleLayout->addWidget(p_eventButton);
    listTitleLayout->addStretch(0);

    QHBoxLayout* listBottomLayout = new QHBoxLayout(p_bottomFrame);
    listBottomLayout->addSpacing(20);
    listBottomLayout->addWidget(p_CancelButton);
    listBottomLayout->addStretch(0);
    listBottomLayout->addWidget(p_OKButton);
    listBottomLayout->addSpacing(20);

    p_vLayout->addStretch(0);
    p_vLayout->addWidget(p_titleFrame);
    p_vLayout->addWidget(p_buttonFrame);
    p_vLayout->addWidget(p_weekView);
    p_vLayout->addWidget(p_eventView);
    p_vLayout->addStretch(0);
    p_vLayout->setSizeConstraint(QLayout::SetMinimumSize);
    p_vLayout->setSpacing(0);

    p_hLayout->addStretch(0);
    p_hLayout->addLayout(p_vLayout);
    p_hLayout->addStretch(0);
    p_hLayout->setSizeConstraint(QLayout::SetMinimumSize);

    QVBoxLayout* p_leftYearVL = new QVBoxLayout;
    QVBoxLayout* p_rightYearVL = new QVBoxLayout;
    QVBoxLayout* p_leftMonthVL = new QVBoxLayout;
    QVBoxLayout* p_rightMonthVL = new QVBoxLayout;
    QVBoxLayout* p_currentYearLayout = new QVBoxLayout;
    QVBoxLayout* p_currentMonthLayout = new QVBoxLayout;
    QHBoxLayout* p_centerLayout = new QHBoxLayout(p_buttonFrame);

    p_leftYearVL->addWidget(p_PreYearButton);
    p_leftYearVL->addWidget(p_PreYearLabel);
    p_leftYearVL->setSpacing(0);

    p_rightYearVL->addWidget(p_NextYearButton);
    p_rightYearVL->addWidget(p_NextYearLabel);
    p_rightYearVL->setSpacing(0);

    p_leftMonthVL->addWidget(p_PreMonthButton);
    p_leftMonthVL->addWidget(p_PreMonthLabel);
    p_leftMonthVL->setSpacing(0);
    p_rightMonthVL->addWidget(p_NextMonthButton);
    p_rightMonthVL->addWidget(p_NextMonthLabel);
    p_rightMonthVL->setSpacing(0);

    p_currentYearLayout->addWidget(p_currentYearLabel);
    p_currentYearLayout->addStretch(0);

    p_currentMonthLayout->addWidget(p_currentMonthLabel);
    p_currentMonthLayout->addStretch(0);

    p_centerLayout->setSpacing(0);
    p_centerLayout->addStretch(0);
    p_centerLayout->addLayout(p_leftYearVL);
    p_centerLayout->addLayout(p_currentYearLayout);
    p_centerLayout->addLayout(p_rightYearVL);
    p_centerLayout->addStretch(0);
    p_centerLayout->addLayout(p_leftMonthVL);
    p_centerLayout->addLayout(p_currentMonthLayout);
    p_centerLayout->addLayout(p_rightMonthVL);
    p_centerLayout->addStretch(0);
    p_centerLayout->setSizeConstraint(QLayout::SetMinimumSize);

    QVBoxLayout* eventShowVLayout = new QVBoxLayout;
    eventShowVLayout->addStretch(0);
    eventShowVLayout->addWidget(p_eventTitleFrame);
    eventShowVLayout->addWidget(p_timeLabel);
    eventShowVLayout->addWidget(p_eventListView);
    eventShowVLayout->addWidget(p_bottomFrame);
    eventShowVLayout->addStretch(0);
    eventShowVLayout->setSizeConstraint(QLayout::SetMinimumSize);

    QHBoxLayout* eventSHowHLayout = new QHBoxLayout(p_eventShowWidget);
    eventSHowHLayout->setSpacing(0);
    eventSHowHLayout->addStretch(0);
    eventSHowHLayout->addLayout(eventShowVLayout);
    eventSHowHLayout->addStretch(0);
}

void EventCalendar::setWeekFromDay(bool sunday)
{
    if(sunday) {    /** week from sunday **/
        p_fisrtDay->setText(tr("Sunday"));
        p_secondDay->setText(tr("Monday"));
        p_thridDay->setText(tr("Tuesday"));
        p_fourDay->setText(tr("Wednesday"));
        p_fivthDay->setText(tr("Thursday"));
        p_sixthDay->setText(tr("Friday"));
        p_sunDay->setText(tr("Saturday"));
    } else {
        p_fisrtDay->setText(tr("Monday"));
        p_secondDay->setText(tr("Tuesday"));
        p_thridDay->setText(tr("Wednesday"));
        p_fourDay->setText(tr("Thursday"));
        p_fivthDay->setText(tr("Friday"));
        p_sixthDay->setText(tr("Saturday"));
        p_sunDay->setText(tr("Sunday"));
    }

    p_weekView->setItem(0, 0, p_fisrtDay);
    p_weekView->setItem(0, 1, p_secondDay);
    p_weekView->setItem(0, 2, p_thridDay);
    p_weekView->setItem(0, 3, p_fourDay);
    p_weekView->setItem(0, 4, p_fivthDay);
    p_weekView->setItem(0, 5, p_sixthDay);
    p_weekView->setItem(0, 6, p_sunDay);
}

/*
 * 功能：
 * 	  根据当前月，上一个月和下一个月的日期，进行链表的排序
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
void EventCalendar::setDaysList()
{
    m_daysList.clear();
    m_ifCurrentDays.clear();
    int m_week = QDate(m_currentYear, m_currentMonth, 1).dayOfWeek();
    switch (m_week) {
    case 1:     /** Monday  **/
        firstDayWeek(Monday);
        break;
    case 2:   /** Tuesday **/
        firstDayWeek(Tuesday);
        break;
    case 3:     /** Wednesday  **/
        firstDayWeek(Wednesday);
        break;
    case 4:     /** Thursday    **/
        firstDayWeek(Thursday);
        break;
    case 5:     /** Friday      **/
        firstDayWeek(Friday);
        break;
    case 6:     /** Saturday  **/
        firstDayWeek(Saturday);
        break;
    case 7:     /** Sunday      **/
        firstDayWeek(Sunday);
        break;
    default:
        break;
    }
}

/*
 * 功能：
 * 	  根据当前月1号是周几来进行链表的插入
 * 参数：
 * 	  int day
 * 返回值：
 * 	  无
 */
void EventCalendar::firstDayWeek(int day)
{
    QDate currentDate = QDate(m_currentYear, m_currentMonth, m_currentDay);  /** 计算当前月的天数 **/
    int m_preMonth = m_currentMonth - 1;
    int m_preYear = m_currentYear;
    if(m_preMonth == 0) {
        m_preMonth = 12;
        m_preYear -= 1;
    }
    QDate PreDate = QDate(m_preYear, m_preMonth, m_currentDay);  /** 计算上一个月的天数 **/
    if(m_fromSunday) {      /** 如果日期从周日开始 **/
        for(int i = PreDate.daysInMonth() - day + 1; i <= PreDate.daysInMonth(); ++i) {
            m_daysList.append(i);
            m_ifCurrentDays.append(-1);
        }

        for(int j = 1; j <= currentDate.daysInMonth(); ++j) {
            m_daysList.append(j);
            m_ifCurrentDays.append(0);
        }

        for(int k = 1; k <= 42 - currentDate.daysInMonth() - day; ++k) {
            m_daysList.append(k);
            m_ifCurrentDays.append(1);
        }
    } else {        /** 如果日期从周一开始 **/
        /** 填充上个月的日期到链表 **/
        int m_days = day - 1;
        if(m_days < 0)
            m_days = 6;
        for(int i = PreDate.daysInMonth() - m_days; i <= PreDate.daysInMonth(); ++i) {
            m_daysList.append(i);
            m_ifCurrentDays.append(-1);
        }

        /** 添加当前月的日期到链表 **/
        for(int i = 1; i <= currentDate.daysInMonth(); ++i) {
            m_daysList.append(i);
            m_ifCurrentDays.append(0);
        }
        /** 填充下个月的日期到链表 **/
        for(int j = 1; j <= 42 - currentDate.daysInMonth() - m_days; ++j) {
            m_daysList.append(j);
            m_ifCurrentDays.append(1);
        }
    }
}

void EventCalendar::setYearShow()
{
    p_PreYearLabel->setText(QString("%1").arg(m_currentYear - 1));
    p_currentYearLabel->setText(QString("%1").arg(m_currentYear));
    p_NextYearLabel->setText(QString("%1").arg(m_currentYear + 1));
    if(m_currentMonth - 1 < 1) {
        p_PreMonthLabel->setText(QString("%1").arg(12));
    } else {
        p_PreMonthLabel->setText(QString("%1").arg(m_currentMonth - 1));
    }
    p_currentMonthLabel->setText(QString("%1").arg(m_currentMonth));
    if(m_currentMonth + 1 > 12) {
        p_NextMonthLabel->setText(QString("%1").arg(1));
    } else {
        p_NextMonthLabel->setText(QString("%1").arg(m_currentMonth + 1));
    }
}

void EventCalendar::setWidgetAttitute()
{
    p_eventWidget->setAttribute(Qt::WA_TranslucentBackground, true);
    p_eventShowWidget->setAttribute(Qt::WA_TranslucentBackground, true);
    p_dispButton->setFocusPolicy(Qt::NoFocus);
    p_dispButton->setCheckable(true);
    p_eventButton->setFocusPolicy(Qt::NoFocus);
    p_eventButton->setCheckable(true);
    p_OKButton->setFocusPolicy(Qt::NoFocus);
    p_CancelButton->setFocusPolicy(Qt::NoFocus);

    p_infoLabel->setAttribute(Qt::WA_TranslucentBackground, true);
    p_closeButton->setStyleSheet("background-color:transparent");
    p_weekView->verticalHeader()->hide();
    p_weekView->horizontalHeader()->hide();
    p_weekView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    p_weekView->setSelectionMode(QAbstractItemView::NoSelection);
    p_weekView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    p_weekView->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    p_weekView->setRowCount(1);
    p_weekView->setColumnCount(7);

    p_fisrtDay->setTextAlignment(132);
    p_secondDay->setTextAlignment(132);
    p_thridDay->setTextAlignment(132);
    p_fourDay->setTextAlignment(132);
    p_fivthDay->setTextAlignment(132);
    p_sixthDay->setTextAlignment(132);
    p_sunDay->setTextAlignment(132);

    p_PreYearLabel->setAlignment(Qt::AlignCenter);
    p_PreMonthLabel->setAlignment(Qt::AlignCenter);
    p_NextYearLabel->setAlignment(Qt::AlignCenter);
    p_NextMonthLabel->setAlignment(Qt::AlignCenter);
    p_currentMonthLabel->setAlignment(Qt::AlignCenter);
    p_currentYearLabel->setAlignment(Qt::AlignCenter);

    p_PreYearButton->setFocusPolicy(Qt::NoFocus);
    p_PreMonthButton->setFocusPolicy(Qt::NoFocus);
    p_NextYearButton->setFocusPolicy(Qt::NoFocus);
    p_NextMonthButton->setFocusPolicy(Qt::NoFocus);
    this->setStyleSheet(QString("QLabel{font-family:'Microsoft YaHei';font-size:%1px; color: rgb(180, 180, 180);}")\
                        .arg((int)(p_PreYearButton->height() * 0.5)) +\
                        QString("QLabel#CurrentYearLabel{font-family:'Microsoft YaHei';font-size:%1px; color: rgb(81, 93, 119);}")\
                        .arg((int)(p_currentYearLabel->height() * 0.9)) +\
                        QString("QLabel#CurrentMonthLabel{font-family:'Microsoft YaHei';font-size:%1px; color: rgb(81, 93, 119);}")\
                        .arg((int)(p_currentMonthLabel->height() * 0.9)) + \
                        QString("QFrame#TitleFrame{border-top-left-radius:8px;"\
                                "border-top-right-radius:8px;"\
                                "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, "\
                                "stop: 0 rgb(119, 141, 180), stop: 0.52 rgb(36, 36, 36), "\
                                "stop: 0.53 rgb(0, 0, 0), stop: 1 rgb(0, 34, 69));}"\
                                "QFrame#EventTitleFrame{border-top-left-radius:8px;"\
                                "border-top-right-radius:8px;"\
                                "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, "\
                                "stop: 0 rgb(119, 141, 180), stop: 0.52 rgb(36, 36, 36), "\
                                "stop: 0.53 rgb(0, 0, 0), stop: 1 rgb(0, 34, 69));}"\
                                "QFrame#BottomFrame{background-color: rgb(255, 255, 255);}") +\
                        QString("QLabel#TitleInfo{font-family:'Microsoft YaHei';font-size:%1px; color: rgb(253, 253, 253);}")\
                        .arg((int)(p_infoLabel->height() * 0.6)) + \
                        QString("QLabel#TimeLabel{font-family:'Microsoft YaHei';font-size:%1px; color: rgb(40, 40, 40);"\
                                "background-color: rgb(255, 255, 255);}").arg((int)(p_timeLabel->height() * 0.7)) +\
                        QString("QFrame#ButtonFrame{background-color: rgb(229, 236, 246);}"\
                                "QPushButton#PreYearButton{border: 1px solid rgb(190, 190, 190); background-color: "\
                                "rgb(229, 236, 246);}"\
                                "QPushButton#PreMonthButton{border: 1px solid rgb(190, 190, 190); background-color: "\
                                "rgb(229, 236, 246);}"\
                                "QPushButton#NextYearButton{border: 1px solid rgb(190, 190, 190); background-color: "\
                                "rgb(229, 236, 246);}"\
                                "QPushButton#NextMonthButton{border: 1px solid rgb(190, 190, 190); background-color: "\
                                "rgb(229, 236, 246);}") +\
                        QString("QPushButton#DispButton{border: 1px solid rgb(180, 185, 190);font-family:'Microsoft YaHei';"\
                                "font-size:%1px; color: rgb(255, 255, 255);}").arg((int)(p_dispButton->height() * 0.45)) +\
                        QString("QPushButton#DispButton:checked{border: 1px solid rgb(255, 255, 255);font-family:'Microsoft YaHei';"\
                                "font-size:%1px; color: rgb(64, 64, 64); background-color: rgb(255, 255, 255);}")\
                        .arg((int)(p_dispButton->height() * 0.45)) +\
                        QString("QPushButton#EventButton{border: 1px solid rgb(180, 185, 190);font-family:'Microsoft YaHei';"\
                                "font-size:%1px; color: rgb(255, 255, 255);}").arg((int)(p_eventButton->height() * 0.45)) +\
                        QString("QPushButton#EventButton:checked{border: 1px solid rgb(255, 255, 255);font-family:'Microsoft YaHei';"\
                                "font-size:%1px; color: rgb(64, 64, 64); background-color: rgb(255, 255, 255);}")\
                        .arg((int)(p_eventButton->height() * 0.45)) + \
                        QString("QPushButton#OKButton{background-color: qlineargradient(x1: 0, y1: 0,"\
                                "x2: 0, y2: 1, stop: 0 rgb(159, 222, 255), stop: 0.13 "\
                                "rgb(20, 158, 249), stop: 0.14 rgb(0, 143, 248), stop: 1 "\
                                "rgb(5, 117, 253));color: rgb(255, 255, 255);"\
                                "border-width: 1px; border-style: solid; border-color: rgb(0, 7, 132);"\
                                "border-radius: 10px; font-family:'Microsoft YaHei';font-size:%1px;}")\
                        .arg((int)(p_OKButton->height() * 0.55)) +\
                        QString("QPushButton#OKButton:pressed{background-color: qlineargradient(x1: 0, y1: 0,"\
                                "x2: 0, y2: 1, stop: 0 rgb(136, 251, 254), stop: 0.13 rgb(157, 241, 253), stop: 0.14 "\
                                "rgb(89, 234, 254), stop: 1 rgb(70, 200, 255));border-width: 1px; "\
                                "border-style: solid; border-color: rgb(168, 177, 186);"\
                                "border-radius: 8px; font-family:'Microsoft YaHei';font-size:%1px;}")\
                        .arg((int)(p_OKButton->height() * 0.55)) +\
                        QString("QPushButton#CancelButton{background-color: qlineargradient(x1: 0, y1: 0,"\
                                "x2: 0, y2: 1, stop: 0 rgb(159, 222, 255), stop: 0.13 "\
                                "rgb(20, 158, 249), stop: 0.14 rgb(0, 143, 248), stop: 1 "\
                                "rgb(5, 117, 253));;color: rgb(255, 255, 255);"\
                                "border-width: 1px; border-style: solid; border-color: rgb(0, 7, 132);"\
                                "border-radius: 10px; font-family:'Microsoft YaHei';font-size:%1px;}")\
                        .arg((int)(p_CancelButton->height() * 0.55)) +\
                        QString("QPushButton#CancelButton:pressed{background-color: qlineargradient(x1: 0, y1: 0,"\
                                "x2: 0, y2: 1, stop: 0 rgb(136, 251, 254), stop: 0.13 rgb(157, 241, 253), stop: 0.14 "\
                                "rgb(89, 234, 254), stop: 1 rgb(70, 200, 255));border-width: 1px; "\
                                "border-style: solid; border-color: rgb(168, 177, 186);"\
                                "border-radius: 8px; font-family:'Microsoft YaHei';font-size:%1px;}")\
                        .arg((int)(p_CancelButton->height() * 0.55)) +\
                        QString("QTableWidget#WeekView{background-color: rgb(250, 250, 250);}"));
}

void EventCalendar::queryEvent(QMap<qlonglong, qlonglong>& listMap, QString tableName)
{
    QDate m_beginDate;
    QDate m_endDate;
    m_spanMonthEvent = false;
    if(-1 == p_eventModel->index(0, 0).data(CanledarActive).toInt()) {      // 如果index(0, 0)是上一天
        int m_preDay = p_eventModel->index(0, 0).data(CalendarDataRole).toInt();
        int m_preMonth = m_currentMonth - 1;
        int m_preYear = m_currentYear;
        if(m_preMonth < 1) {
            m_preMonth = 12;
            m_preYear = m_currentYear - 1;
        }
        m_beginDate = QDate(m_preYear, m_preMonth, m_preDay);
    } else {
        m_beginDate = QDate(m_currentYear, m_currentMonth, 1);
    }

    if(1 == p_eventModel->index(5, 6).data(CanledarActive).toInt()) {
        int m_nextDay = p_eventModel->index(5, 6).data(CalendarDataRole).toInt();
        int m_nextMonth = m_currentMonth + 1;
        int m_nextYear = m_currentYear;
        if(m_nextMonth > 12) {
            m_nextMonth = 1;
            m_nextYear = m_currentYear + 1;
        }
        m_endDate =QDate(m_nextYear,m_nextMonth,  m_nextDay);
    }
    QTime m_beginTime(0, 0, 0);
    QTime m_endTime(23, 59, 59);
    QDateTime m_beginDateTime;
    QDateTime m_endDateTime;
    m_beginDateTime.setDate(m_beginDate);
    m_beginDateTime.setTime(m_beginTime);
    m_endDateTime.setDate(m_endDate);
    m_endDateTime.setTime(m_endTime);

    QList<qlonglong> m_listDispBeginTime;
    QList<qlonglong> m_listDispEndTime;
    qlonglong m_tempBeginTime;
    qlonglong m_tempEndTime;

    /** 如果有事件跨越42天 **/
    QSqlQuery query(QString("select StartTime, EndTime from %1 where StartTime < %2 "\
                            "and EndTime > %3").arg(tableName).arg(m_beginDateTime.toMSecsSinceEpoch())\
                    .arg(m_endDateTime.toMSecsSinceEpoch()), *ThreadDataRecord::instance()->getDatabaseConnect());
    query.exec();
    while(query.next()) {
        m_spanMonthEvent = true;
    }

    if(!m_spanMonthEvent) {
        /** 查询事件开始时间 **/
        if(query.exec(QString("select StartTime from %1 where"\
                              " StartTime between %2 and %3").arg(tableName).arg(m_beginDateTime.toMSecsSinceEpoch())\
                      .arg(m_endDateTime.toMSecsSinceEpoch()))) {
            while (query.next()) {
                m_listDispBeginTime.append(query.value(0).toLongLong());
            }
        } else {
            qDebug() << "query db_record error!";
        }

        /** 查询事件结束时间 **/
        if(query.exec(QString("select EndTime from %1 where"\
                              " StartTime between %2 and %3").arg(tableName).arg(m_beginDateTime.toMSecsSinceEpoch())\
                      .arg(m_endDateTime.toMSecsSinceEpoch()))) {
            while (query.next()) {
                m_listDispEndTime.append(query.value(0).toLongLong());
            }
        } else {
            qDebug() << "query db_record error!";
        }

        if(m_listDispBeginTime.size() == m_listDispEndTime.size() && !m_listDispEndTime.isEmpty()) {    /** 获取到的开始时间点和结束时间点一样多 **/
            if(m_listDispBeginTime.at(0) < m_listDispEndTime.at(0)) {       /** 如果所查询到的结果正好是成对出现  **/
                for(int i = 0; i < m_listDispEndTime.size(); ++i)
                    listMap.insert(m_listDispBeginTime.at(i), m_listDispEndTime.at(i));
            } else {        /** 如果所查询到的结果不是成对出现  **/
                if(query.exec(QString("select StartTime from %1 "\
                                      "where EndTime = %2 ").arg(tableName).arg(m_listDispEndTime.at(0)))) {       /** 查询开始时间  **/
                    while (query.next()) {
                        m_tempBeginTime = query.value(0).toLongLong();
                    }
                } else {
                    qDebug() << "select StartTime from table Error!";
                }
                listMap.insert(m_tempBeginTime, m_listDispEndTime.at(0));


                for(int i = 0; i < m_listDispBeginTime.size() - 1; ++i) {
                    listMap.insert(m_listDispBeginTime.at(i), m_listDispEndTime.at(i + 1));
                }

                if(query.exec(QString("select EndTime from %1 "\
                                      "where StartTime = %2").arg(tableName).arg(m_listDispBeginTime.last()))) {       /** 查询结束时间  **/
                    while (query.next()) {
                        m_tempEndTime = query.value(0).toLongLong();
                    }
                } else {
                    qDebug() << "select EndTime from table Error!";
                }
                listMap.insert(m_listDispBeginTime.last(), m_tempEndTime);
            }
        } else if(m_listDispBeginTime.size() > m_listDispEndTime.size()) {      /** 如果搜索到的开始时间点多余结束时间点 **/
            for(int i = 0; i < m_listDispEndTime.size(); ++i)
                listMap.insert(m_listDispBeginTime.at(i + 1), m_listDispEndTime.at(i));

            if(query.exec(QString("select EndTime from %1"\
                                  " where StartTime = %2").arg(tableName).arg(m_listDispBeginTime.last()))) {      /** 查询最后一个开始时间对应的结束时间 **/
                while (query.next()) {
                    m_tempEndTime = query.value(0).toLongLong();
                }
            } else {
                qDebug() << "select EndTime frim table Error!";
            }
            listMap.insert(m_listDispBeginTime.last(), m_tempEndTime);
        } else if(m_listDispBeginTime.size() < m_listDispEndTime.size()){        /** 如果搜索到的结束时间点多余开始时间点 **/
            if(query.exec(QString("select StartTime from %1"\
                                  " where EndTime = %2").arg(tableName).arg(m_listDispEndTime.at(0)))) {
                while (query.next()) {
                    m_tempBeginTime = query.value(0).toInt();
                }
            } else {
                qDebug() << "select StartTime frim table Error!";
            }
            listMap.insert(m_tempBeginTime, m_listDispEndTime.at(0));

            for(int i = 0; i < m_listDispBeginTime.size(); ++i)
                listMap.insert(m_listDispBeginTime.at(i), m_listDispEndTime.at(i + 1));
        }
    }

    ThreadDataRecord::instance()->putDatabaseConnect();
}

void EventCalendar::getEventInfo(QMap<qlonglong, qlonglong>& map, bool DispEvent)
{
    QDateTime beginDateTime;
    QDateTime endDateTime;
    map.clear();

    /** QUERY  EVENT        **/
    if(DispEvent) {
        queryEvent(map, "table_Main_Disp");
    } else {
        queryEvent(map, "table_Main_Event");
    }

    if(m_spanMonthEvent) {      /** if the day of current shows has Disp event **/
        for(int i = 0; i < 7; ++i)
            for(int j = 0; j < 6; ++j) {
                if(DispEvent) {
                    p_eventModel->setDispShow(p_eventModel->index(i, j), true);
                } else {
                    p_eventModel->setEventShow(p_eventModel->index(i, j), true);
                }
            }
    } else {
        /** traversal disp map  **/
        for(m_EventIterator = map.begin(); m_EventIterator != map.end(); ++m_EventIterator) {
            beginDateTime.setMSecsSinceEpoch(m_EventIterator.key());    /**  begin time  **/
            endDateTime.setMSecsSinceEpoch(m_EventIterator.value());    /** end time     **/

            /** traversal current calendar  **/
            for(int i = 0; i < 7; ++i)
                for(int j = 0; j < 6; ++j) {
                    int calendarDay = p_eventModel->index(i, j).data(CalendarDataRole).toInt();
                    int thridDayStatus = p_eventModel->index(i, j).data(CanledarActive).toInt();

                    /** if the day of calendar same as begin day    **/
                    if(calendarDay == beginDateTime.date().day()) {
                        if((beginDateTime.date().month() == m_currentMonth && 0 == thridDayStatus) || \
                                (beginDateTime.date().month() < m_currentMonth && -1 == thridDayStatus) || \
                                (beginDateTime.date().month() > m_currentMonth && 1 == thridDayStatus)) {
                            if(DispEvent) {
                                p_eventModel->setDispShow(p_eventModel->index(i, j), true);
                            } else {
                                p_eventModel->setEventShow(p_eventModel->index(i, j), true);
                            }
                        } else {
                            /** do nothing **/
                        }
                    } else {
                        /** do nothing **/
                    }

                    /** if the day of calendar same as end day    **/
                    if(calendarDay == endDateTime.date().day()) {
                        if((endDateTime.date().month() == m_currentMonth && 0 == thridDayStatus) || \
                                (endDateTime.date().month() < m_currentMonth && -1 == thridDayStatus) || \
                                (endDateTime.date().month() > m_currentMonth && 1 == thridDayStatus)) {
                            if(DispEvent) {
                                p_eventModel->setDispShow(p_eventModel->index(i, j), true);
                            } else {
                                p_eventModel->setEventShow(p_eventModel->index(i, j), true);
                            }
                        } else {
                            /** do nothing **/
                        }
                    } else {
                        /** do nothing **/
                    }

                    /** if the span of the event more than a day  **/
                    QDateTime calendarDateTimeBegin;
                    QDateTime   calendarDateTimeEnd;
                    if(-1 == thridDayStatus) {      /** current day belong to last month   **/
                        if(1 == m_currentMonth) {
                            calendarDateTimeBegin.setDate(QDate(m_currentYear - 1, 12, calendarDay));
                            calendarDateTimeEnd.setDate(QDate(m_currentYear - 1, 12, calendarDay));
                        } else {
                            calendarDateTimeBegin.setDate(QDate(m_currentYear, m_currentMonth - 1, calendarDay));
                            calendarDateTimeEnd.setDate(QDate(m_currentYear, m_currentMonth - 1, calendarDay));
                        }
                    } else if(0 == thridDayStatus) {        /** current day belong to current month   **/
                        calendarDateTimeBegin.setDate(QDate(m_currentYear, m_currentMonth , calendarDay));
                        calendarDateTimeEnd.setDate(QDate(m_currentYear, m_currentMonth, calendarDay));
                    } else {        /** current day belong to next month   **/
                        if(12 == m_currentMonth) {
                            calendarDateTimeBegin.setDate(QDate(m_currentYear + 1, 1, calendarDay));
                            calendarDateTimeEnd.setDate(QDate(m_currentYear + 1, 1, calendarDay));
                        } else {
                            calendarDateTimeBegin.setDate(QDate(m_currentYear, m_currentMonth + 1, calendarDay));
                            calendarDateTimeEnd.setDate(QDate(m_currentYear, m_currentMonth + 1, calendarDay));
                        }
                    }
                    calendarDateTimeBegin.setTime(QTime(0, 0, 0));
                    calendarDateTimeEnd.setTime(QTime(23, 59, 59));
                    if(beginDateTime < calendarDateTimeBegin && endDateTime > calendarDateTimeEnd) {
                        if(DispEvent) {
                            p_eventModel->setDispShow(p_eventModel->index(i, j), true);
                        } else {
                            p_eventModel->setEventShow(p_eventModel->index(i, j), true);
                        }
                    }
                }
        }
    }
}

void EventCalendar::setBeginEndTime(QDateTime &beginDateTime, QDateTime &endDateTime)
{
    if(-1 == m_selectDayStatus) {       /** last month day  **/
        if(1 == m_currentMonth) {
            beginDateTime.setDate(QDate(m_currentYear - 1, 12, m_currentDay));
            beginDateTime.setTime(QTime(0, 0, 0));
            endDateTime.setDate(QDate(m_currentYear - 1, 12, m_currentDay));
            endDateTime.setTime(QTime(23, 59, 59));
        } else {
            beginDateTime.setDate(QDate(m_currentYear, m_currentMonth - 1, m_currentDay));
            beginDateTime.setTime(QTime(0, 0, 0));
            endDateTime.setDate(QDate(m_currentYear, m_currentMonth - 1, m_currentDay));
            endDateTime.setTime(QTime(23, 59, 59));
        }
    } else if(0 == m_selectDayStatus) {     /** current month day  **/
        beginDateTime.setDate(QDate(m_currentYear, m_currentMonth, m_currentDay));
        beginDateTime.setTime(QTime(0, 0, 0));
        endDateTime.setDate(QDate(m_currentYear, m_currentMonth, m_currentDay));
        endDateTime.setTime(QTime(23, 59, 59));
    } else {        /** next month day  **/
        if(12 == m_currentMonth) {
            beginDateTime.setDate(QDate(m_currentYear + 1, 1, m_currentDay));
            beginDateTime.setTime(QTime(0, 0, 0));
            endDateTime.setDate(QDate(m_currentYear + 1, 1, m_currentDay));
            endDateTime.setTime(QTime(23, 59, 59));
        } else {
            beginDateTime.setDate(QDate(m_currentYear, m_currentMonth, m_currentDay));
            beginDateTime.setTime(QTime(0, 0, 0));
            endDateTime.setDate(QDate(m_currentYear, m_currentMonth, m_currentDay));
            endDateTime.setTime(QTime(23, 59, 59));
        }
    }
}

void EventCalendar::finCurrentDay()
{
    for(int i = 0; i < 6; ++i)
        for(int j = 0; j < 7; ++j) {
            if(m_currentDay == p_eventModel->index(i, j).data(CalendarDataRole).toInt() \
                    && 0 == p_eventModel->index(i, j).data(CanledarActive).toInt()) {
                p_eventView->setSelectStatus(p_eventModel->index(i, j));
                break;
            }
        }
}

void EventCalendar::connectFunC()
{
    connect(p_PreYearButton, SIGNAL(clicked()), this, SLOT(slotPreYearButtonClicked()));
    connect(p_NextYearButton, SIGNAL(clicked()), this, SLOT(slotNextYearButtonClicked()));
    connect(p_PreMonthButton, SIGNAL(clicked()), this, SLOT(slotPreMonthButtonClicked()));
    connect(p_NextMonthButton, SIGNAL(clicked()), this, SLOT(slotNextMonthButtonClicked()));
    connect(p_closeButton, SIGNAL(clicked()), this, SLOT(slotCalendarCloseClicked()));
    connect(p_CancelButton, SIGNAL(clicked()), this, SLOT(slotEventListCloseClicked()));
    connect(p_OKButton, SIGNAL(clicked()), this, SLOT(slotOKClicked()));
    connect(p_dispButton, SIGNAL(clicked()), this, SLOT(slotDispButtonClicked()));
    connect(p_eventButton, SIGNAL(clicked()), this, SLOT(slotEventButtonClicked()));
    connect(p_eventListView,SIGNAL(sigIndexClicked(QModelIndex)), this, SLOT(slotEventListClicked(QModelIndex)));
    connect(p_eventView, SIGNAL(sigReleasedIndex(QModelIndex)), this, SLOT(slotEventViewReleased(QModelIndex)));
    connect(p_eventListView, SIGNAL(sigMoseMove()), this, SLOT(slotListMouseMove()));
}
