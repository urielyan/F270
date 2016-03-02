/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：MessageListView.h
 * 概   要：信息显示接口的视图
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
#include "messagelistview.h"
#include "./GlobalData/cfgchannel.h"
#include <QHeaderView>
#include <QScroller>
#include <QEasingCurve>
#include <QMouseEvent>
#include <QDebug>
MessageListView::MessageListView(QWidget *parent) :
    QTableView(parent)
{
    this->horizontalHeader()->hide();
    this->verticalHeader()->hide();
    this->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    this->verticalHeader()->setDefaultSectionSize(60);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setShowGrid(false);
    this->setSelectionMode(QAbstractItemView::NoSelection);
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

void MessageListView::setDefaultIndex(QModelIndex index)
{
    m_pressIndex = index;
}

void MessageListView::paintEvent(QPaintEvent *event)
{
    QTableView::paintEvent(event);
}

void MessageListView::mousePressEvent(QMouseEvent *event)
{
    if(m_pressIndex.isValid()) {    /** 恢复上一个单击的状态  **/
        const_cast<QAbstractItemModel*>(m_pressIndex.model())->setData(m_pressIndex, false, MouseClickedRole);
        update(m_pressIndex);
    }
    m_pressIndex = indexAt(event->pos());
    if(m_pressIndex.isValid()) {    /** 设置当前索引为下押状态  **/
        const_cast<QAbstractItemModel*>(m_pressIndex.model())->setData(m_pressIndex, true, MousePressRole);
        update(m_pressIndex);
    }
    QTableView::mousePressEvent(event);
}

void MessageListView::mouseReleaseEvent(QMouseEvent *event)
{
    const_cast<QAbstractItemModel*>(m_pressIndex.model())->setData(m_pressIndex, false, MousePressRole);
    update(m_pressIndex);
    m_releasedIndex = indexAt(event->pos());
    if(m_pressIndex.operator !=(m_releasedIndex)) {     /** 如果下押的index和释放的index不同，不设置选择状态 **/
        const_cast<QAbstractItemModel*>(m_pressIndex.model())->setData(m_pressIndex, false, MouseClickedRole);
    } else {
        const_cast<QAbstractItemModel*>(m_pressIndex.model())->setData(m_pressIndex, true, MouseClickedRole);
        emit sigMessageClicked(m_pressIndex);
    }
    update(m_pressIndex);
    QTableView::mouseReleaseEvent(event);
}
