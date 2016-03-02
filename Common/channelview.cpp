/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：channelview.h
 * 概   要：自定义视图，用于通道选择界面
 *
 * 当前版本：V1.0.0
 * 作   者：葛 海 浪
 * 完成日期：2016-1-5
 *
 * 修改版本：
 * 修改说明：
 * 修改作者：
 *
 ********************************************************************/
#include "channelview.h"
#include <QHeaderView>
#include <QMouseEvent>
#include <QModelIndex>
#include <QList>
#include <QDebug>
ChannelView::ChannelView(QWidget *parent) :
    QTableView(parent)
  , m_selectCount(0)
  , m_selectSpan(0)
{
    this->horizontalHeader()->hide();
    this->verticalHeader()->hide();
    this->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    this->setShowGrid(false);
    this->setSelectionMode(QAbstractItemView::NoSelection);
    this->verticalHeader()->setDefaultSectionSize(60);
    indexList.clear();
    m_sequnce = true;
    connect(this, SIGNAL(clicked(QModelIndex)), this, SLOT(slotMouseClicked(QModelIndex)));
}

void ChannelView::setSequnceStyle(bool sequnce)
{
    m_sequnce = sequnce;
}

void ChannelView::setViewClicked(QModelIndex &index)
{
    slotMouseClicked(index);
}

void ChannelView::setSelectMaxNums(quint32 max)
{
    m_selectSpan = max;
}

QModelIndexList ChannelView::getSelectChannel()
{
    return indexList;
}

void ChannelView::initial()
{
    indexList.clear();
    m_selectCount = 0;
    m_selectSpan = 0;
    m_press = false;
    m_clicked = false;
}

void ChannelView::mousePressEvent(QMouseEvent *event)
{
    m_point = event->pos();
    m_move = false;
    m_currentIndex = indexAt(m_point);
    if(m_currentIndex.isValid()) {      /** 当前索引是否为有效值 **/
        if(m_sequnce) {
            const_cast<QAbstractItemModel*>(m_currentIndex.model())->setData(m_currentIndex, true, ChannelPressRole);
            emit dataChanged(m_currentIndex, m_currentIndex);
        } else {
            m_press = true;
        }
    }
    emit sigViewPressed(m_selectCount);
    QTableView::mousePressEvent(event);
}

void ChannelView::mouseReleaseEvent(QMouseEvent *event)
{
    if(m_currentIndex.isValid()) {
        if(m_sequnce) {
            const_cast<QAbstractItemModel*>(m_currentIndex.model())->setData(m_currentIndex, false, ChannelPressRole);
            update(m_currentIndex);
        } else {
            m_press = false;
        }
    }
    QTableView::mouseReleaseEvent(event);
}

void ChannelView::mouseMoveEvent(QMouseEvent *event)
{
    if(m_currentIndex.isValid()) {
        m_move = true;
        QModelIndex index = indexAt(event->pos());
        if(index.isValid()) {
            if(m_sequnce) {     /** 如果是有序状态 **/
                if(!index.operator ==(m_currentIndex)) {
                    if(index.data(ChannelClickedRole).toBool()) {       /** 如果当前通道处于被选择状态 **/
                        // do nothing
                    } else {
                        const_cast<QAbstractItemModel*>(m_currentIndex.model())->setData(m_currentIndex, false, ChannelPressRole);
                    }
                    update(m_currentIndex);
                }
            } else {            /** 如果是无序状态 **/
                if(m_press) {
                    m_clicked = m_currentIndex.data(ChannelClickedRole).toBool();
                    m_press = false;
                }
                if(!m_indexDecide.operator ==(index)) {
                    if(m_clicked) {     /** 如果参考通道处于被选择状态 **/
                        if(index.data(ChannelClickedRole).toBool()) {       /** 如果当前道处于被选择状态 **/
                            for(quint32 i = 0; i < m_selectCount; ++i) {
                                if(indexList.at(i).operator ==(index)) {
                                    indexList.removeAt(i);
                                    break;
                                }
                            }
                            --m_selectCount;
                            const_cast<QAbstractItemModel*>(index.model())->setData(index, false, ChannelClickedRole);
                        } else {
                            /** do nothing **/
                        }
                    } else {         /** 如果参考通道未被选择 **/
                        if(m_selectCount < m_selectSpan) {          /** 如果数目未满  **/
                            if(!index.data(ChannelClickedRole).toBool()) {      /** 如果当前道未被选择 **/
                                ++m_selectCount;
                                indexList.append(index);
                                const_cast<QAbstractItemModel*>(index.model())->setData(index, true, ChannelClickedRole);
                            } else {
                                /** do nothing **/
                            }
                        } else {        /** 如果数目满  **/
                            /** do nothing **/
                        }
                    }
                }
                m_indexDecide = index;
                update(index);
            }
        }
    }
    emit sigViewPressed(m_selectCount);
    QTableView::mouseMoveEvent(event);
}

void ChannelView::slotMouseClicked(QModelIndex index)
{
    if(m_sequnce) {     /** 如果是有序状态 **/
        if(index.data(ChannelClickedRole).toBool()) {        /** 如果当前通道处于被选择状态 **/
            --m_selectCount;
            int m_channelID = index.data(ChannelSequnceRole).toInt();
            const_cast<QAbstractItemModel*>(index.model())->setData(index, false, ChannelClickedRole);  /** 取消选择状态**/

            /** 在链表中删除当前节点  **/
            for(int i = 0; i < indexList.size(); ++i) {
                if(indexList.at(i).data(ChannelSequnceRole).toInt() == m_channelID) {
                    indexList.removeAt(i);
                    break;
                }
            }
            const_cast<QAbstractItemModel*>(index.model())->setData(index, -1, ChannelSequnceRole);      /** -1代表不显示通道  **/
            for(int i = 0; i < indexList.size(); ++i) {
                int m_changeID = indexList.at(i).data(ChannelSequnceRole).toInt();
                if(m_changeID > m_channelID) {
                    const_cast<QAbstractItemModel*>(indexList.at(i).model())->setData(indexList.at(i), --m_changeID, ChannelSequnceRole);
                    update(indexList.at(i));
                } else {
                    /** do nothing **/
                }
            }
            update(index);

        } else {        /** 如果当前通道未被选择 **/
            if(m_selectCount < m_selectSpan) {          /** 如果数目未满  **/
                ++m_selectCount;
                const_cast<QAbstractItemModel*>(index.model())->setData(index, true, ChannelClickedRole);  /** 设置为选择状态**/
                const_cast<QAbstractItemModel*>(index.model())->setData(index, -2, ChannelSequnceRole);      /** -2代表在当前的基础上增加通道  **/
                indexList << index;
            }
        }
    } else {        /** 如果是无序状态 **/
        if(m_move && index.operator ==(m_currentIndex)) {
            // do nothing
        } else {
            if(index.data(ChannelClickedRole).toBool()) {  /** 如果当前通道处于被选择状态 **/
                --m_selectCount;
                const_cast<QAbstractItemModel*>(index.model())->setData(index, false, ChannelClickedRole);  /** 取消选择状态**/
                for(int i = 0; i < indexList.size(); ++i) {
                    if(indexList.at(i).operator ==(index))
                        indexList.removeAt(i);
                }
            } else {            /** 如果当前通道未被选择 **/
                if(m_selectCount < m_selectSpan) {          /** 如果数目未满  **/
                    ++m_selectCount;
                    const_cast<QAbstractItemModel*>(index.model())->setData(index, true, ChannelClickedRole);  /** 设置为选择状态**/
                    indexList << index;
                }
            }
            const_cast<QAbstractItemModel*>(index.model())->setData(index, 0, ChannelSequnceRole);      /** 0代表不显示通道  **/
        }
    }
    update(index);
    emit sigViewPressed(m_selectCount);
}
