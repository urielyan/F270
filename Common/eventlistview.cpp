/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：EventListview.h
 * 概   要：日历事件接口中事件一览显示的view
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
#include "eventlistview.h"
#include "./GlobalData/cfgchannel.h"
#include <QMouseEvent>
#include <QScrollBar>
#include <QScroller>
#include <QEasingCurve>
#include <QHeaderView>
EventListView::EventListView(QWidget *parent) :
    QTableView(parent)
  , m_mouseMove(true)
  , m_PosY(0)
{
    this->horizontalHeader()->hide();
    this->verticalHeader()->hide();
    this->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    this->verticalHeader()->setDefaultSectionSize(60);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setShowGrid(false);
    this->setSelectionMode(QAbstractItemView::NoSelection);
    connect(this, SIGNAL(clicked(QModelIndex)), this, SLOT(slotMouseClicked(QModelIndex)));
    /*
     * 添加滑屏操作
    */
    this->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);

    QScroller *scroller = QScroller::scroller(this->viewport());

    QScrollerProperties prop = scroller->scrollerProperties();

    prop.setScrollMetric(QScrollerProperties::AxisLockThreshold, 0.66);
    prop.setScrollMetric(QScrollerProperties::ScrollingCurve, QEasingCurve(QEasingCurve::OutExpo));
    prop.setScrollMetric(QScrollerProperties::DecelerationFactor, 0.05);
    prop.setScrollMetric(QScrollerProperties::MaximumVelocity, 0);
    prop.setScrollMetric(QScrollerProperties::OvershootDragResistanceFactor, 0.33);
    prop.setScrollMetric(QScrollerProperties::OvershootScrollDistanceFactor, 0.33);
    prop.setScrollMetric(QScrollerProperties::SnapPositionRatio, 0.93);
    prop.setScrollMetric(QScrollerProperties::DragStartDistance, 0.001);
    prop.setScrollMetric(QScrollerProperties::SnapPositionRatio,0.5);
    scroller->setScrollerProperties(prop);

    scroller->grabGesture(this, QScroller::TouchGesture);
    scroller->grabGesture(this, QScroller::LeftMouseButtonGesture);
}

void EventListView::setModelIndexDefault()
{
    QModelIndex index;
    m_clickedIndex = index;
}

void EventListView::mousePressEvent(QMouseEvent *event)
{
    if(indexAt(event->pos()).isValid()) {
        if(m_clickedIndex.isValid()) {
            const_cast<QAbstractItemModel*>(m_clickedIndex.model())->setData(m_clickedIndex, false, MouseClickedRole);
        }
        m_PressIndex = indexAt(event->pos());
        m_PosY = event->pos().y();
        m_mouseMove = false;
        const_cast<QAbstractItemModel*>(m_PressIndex.model())->setData(m_PressIndex, true, MousePressRole);
        update(m_PressIndex);
    }
    QTableView::mousePressEvent(event);
}

void EventListView::mouseReleaseEvent(QMouseEvent *event)
{
    if(!m_mouseMove) {
        const_cast<QAbstractItemModel*>(m_PressIndex.model())->setData(m_PressIndex, false, MousePressRole);
        update(m_PressIndex);
    }
    QTableView::mouseReleaseEvent(event);
}

void EventListView::mouseMoveEvent(QMouseEvent *event)
{
    if(indexAt(event->pos()).isValid()) {
        if(!m_mouseMove) {
            const_cast<QAbstractItemModel*>(m_PressIndex.model())->setData(m_PressIndex, false, MousePressRole);
            update(m_PressIndex);
            m_mouseMove = true;
        }
    }
    emit sigMoseMove();
    QTableView::mouseMoveEvent(event);
}

void EventListView::paintEvent(QPaintEvent *event)
{
    QTableView::paintEvent(event);
}

void EventListView::slotMouseClicked(QModelIndex index)
{
    const_cast<QAbstractItemModel*>(index.model())->setData(index, true, MouseClickedRole);
    m_clickedIndex = index;
    update(index);
    emit sigIndexClicked(index);
}
