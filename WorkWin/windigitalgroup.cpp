#include <QPalette>
#include <QMouseEvent>
#include <QPainter>
#include <QHeaderView>
#include <QDebug>
#include <QEvent>
#include <QTimer>

#include "windigitalgroup.h"
#include "winchannelinfodialog.h"
#include "../GlobalData/cfgGlobalDef.h"
#include "./Common/authorization.h"

WinDigitalGroup::WinDigitalGroup(quint32 grpIndex, QWidget *parent) :
    QTableView(parent),m_pressed(false),m_grpIndex(grpIndex),m_model(grpIndex,this)
{
    this->setStyleSheet("WinDigitalGroup{background-color:rgb(1,16,177);border:0;}");//1,16,177

    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setAutoScroll(false);

    this->setSelectionMode(QAbstractItemView::NoSelection);
    this->horizontalHeader()->setVisible(false);
    this->horizontalHeader()->setHighlightSections(false);
    this->verticalHeader()->setVisible(false);
    this->verticalHeader()->setHighlightSections(false);

    this->setShowGrid(false);
    this->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    this->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    this->setMouseTracking(false);

    this->setModel(&m_model);
    this->setItemDelegate(&m_delegate);

//    connect(this,SIGNAL(clicked(QModelIndex)),this,SLOT(slotChannelClicked(QModelIndex)));
}

/**
 * 功能：
 *      重置显示组
 * 参数：
 *      １、quint32 grpIndex　组索引
 */
void WinDigitalGroup::resetGroupIndex(quint32 grpIndex)
{
    m_grpIndex = grpIndex;
    m_model.resetCurrentGroup(grpIndex);
}

void WinDigitalGroup::mousePressEvent(QMouseEvent *event)
{
    m_point = event->pos();
    QModelIndex index = indexAt(m_point);
    if(event->button() == Qt::LeftButton && index.isValid()){
        m_rect = visualRect(index);
        m_pressed = true;
        update(index);
    }
    QTableView::mousePressEvent(event);
}

void WinDigitalGroup::mouseReleaseEvent(QMouseEvent *event)
{
    m_pressed = false;
    QPoint pos = event->pos();
    QModelIndex index = indexAt(pos);
    update(index);
    
    QTableView::mouseReleaseEvent(event);
    int rx = m_point.x() - pos.x();
    int ry = m_point.y() - pos.y();
    if(rx>-20 && rx<20 && ry>-20 && ry<20){
        slotChannelClicked(index);
    }
}

void WinDigitalGroup::mouseMoveEvent(QMouseEvent *event)
{
    QModelIndex index = indexAt(event->pos());
    if(index.isValid() && !m_rect.intersects(visualRect(index))){
        m_pressed = false;
        update(indexAt(event->pos()));
    }else{
        m_pressed = true;
        update(indexAt(event->pos()));
    }

    QTableView::mouseMoveEvent(event);
}

/**
 * 功能:
 *      添加自定义状态值
 */
QStyleOptionViewItem WinDigitalGroup::viewOptions() const
{
    QStyleOptionViewItem option = QTableView::viewOptions();
    if(CfgChannel::instance()->getSysBackgroundColor()){
        option.backgroundBrush = QBrush(Qt::black);
        option.palette.setBrush(QPalette::WindowText,QBrush(Qt::white));
    }else{
        option.backgroundBrush = QBrush(Qt::white);
        option.palette.setBrush(QPalette::WindowText,QBrush(Qt::black));
    }

    if(m_pressed){
        option.state |= QStyle::State_Sunken;
    }

    if(WinMainFrame::getBlinkStatus()){
        option.state |= QStyle::State_On;
    }else{
        option.state |= QStyle::State_Off;
    }
    return option;
}


void WinDigitalGroup::slotChannelClicked(QModelIndex index)
{
    if(Authorization::instance()->getLoginStatus() == Authorization::Auth_LoginStatus_Logout){
        WinInfoListDialog::instance()->showMsg(818);
    }else{
        QVariant value = index.data(ChannelValidity);
        if(value.isValid()){
            quint32 chanNum = value.toUInt();
            if(CHANNEL_TYPE(chanNum) == Channel_Type_DO){
                WinDOChannelDialog::instance().resetCurrentChannelNumber(chanNum);
                WinDOChannelDialog::instance().show();
            }else{
                value = index.data(ChannelRangeType);
                if(value.isValid() && value.toUInt() != OFF){
                    WinChannelInfoDialog::instance().resetCurrentChannelNumber(chanNum);
                    WinChannelInfoDialog::instance().show();
                }
            }
        }
    }
}
