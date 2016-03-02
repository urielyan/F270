/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：EventCalendarView.cpp
 * 概   要：日历事件接口中日历界面显示的视图
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
#include "eventcalendarview.h"
#include <QHeaderView>
#include <QMouseEvent>
#include <QDebug>
EventCalendarView::EventCalendarView(QWidget *parent) :
    QTableView(parent)
{
    this->horizontalHeader()->hide();
    this->verticalHeader()->hide();
    this->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    this->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    this->setShowGrid(false);
    this->setSelectionMode(QAbstractItemView::NoSelection);
}

void EventCalendarView::setSelectStatus(QModelIndex index)
{
    if(index.isValid()) {
        if(m_currentIndex.isValid()) {
            const_cast<QAbstractItemModel*>(m_currentIndex.model())->setData(m_currentIndex, false, CalendarClickedRole);
            update(m_currentIndex);
            m_currentIndex = index;
            const_cast<QAbstractItemModel*>(m_currentIndex.model())->setData(m_currentIndex, true, CalendarClickedRole);
            update(m_currentIndex);
        } else {
            m_currentIndex = index;
            const_cast<QAbstractItemModel*>(m_currentIndex.model())->setData(m_currentIndex, true, CalendarClickedRole);
        }
    } else {

    }
}

void EventCalendarView::mousePressEvent(QMouseEvent *event)
{
    if(m_currentIndex.isValid()) {
        const_cast<QAbstractItemModel*>(m_currentIndex.model())->setData(m_currentIndex, false, CalendarClickedRole);
        update(m_currentIndex);
    }
    m_point = event->pos();
    m_currentIndex = indexAt(m_point);
    if(m_currentIndex.isValid()) {          /** 当前索引是否为有效值 **/
        const_cast<QAbstractItemModel*>(m_currentIndex.model())->setData(m_currentIndex, true, CalendarPressRole);
        update(m_currentIndex);
    }
    QTableView::mousePressEvent(event);
}

void EventCalendarView::mouseReleaseEvent(QMouseEvent *event)
{
    const_cast<QAbstractItemModel*>(m_currentIndex.model())->setData(m_currentIndex, false, CalendarPressRole);
    const_cast<QAbstractItemModel*>(m_currentIndex.model())->setData(m_currentIndex, true, CalendarClickedRole);
    update(m_currentIndex);
    if(0 == m_currentIndex.data(CanledarActive).toInt())
        emit sigReleasedIndex(m_currentIndex);
    QTableView::mouseReleaseEvent(event);
}

void EventCalendarView::mouseMoveEvent(QMouseEvent *event)
{
    m_moveIndex = indexAt(event->pos());
    if(m_moveIndex.isValid() && m_moveIndex.operator !=(m_currentIndex)) {
        const_cast<QAbstractItemModel*>(m_currentIndex.model())->setData(m_currentIndex, false, CalendarPressRole);
        update(m_currentIndex);
        m_currentIndex = m_moveIndex;
        const_cast<QAbstractItemModel*>(m_currentIndex.model())->setData(m_currentIndex, true, CalendarPressRole);
        update(m_currentIndex);
    }
    QTableView::mouseMoveEvent(event);
}
