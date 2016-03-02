#include <QPalette>
#include <QMouseEvent>
#include <QPainter>
#include <QHeaderView>
#include <QDebug>
#include <QEvent>
#include <QTimer>

#include "winbargroup.h"
#include "winbardelegate.h"
#include "windigitaldelegate.h"
#include "winchannelinfodialog.h"
#include "./DataModel/modelbargroupchannel.h"
#include "./Common/authorization.h"

WinBarGroup::WinBarGroup(QWidget *parent) :
    QTableView(parent),m_pressed(false)
{
    this->setStyleSheet("WinBarGroup{background-color:rgb(1,16,177);border:0;}");//WinBarGroup{background-color:rgb(1,16,177);}

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

//    this->setModel(new ModelBarGroupChannel(m_grpIndex, this));
//    this->setItemDelegate(new WinBarDelegate(this));

//    connect(this,SIGNAL(clicked(QModelIndex)),this,SLOT(slotChannelClicked(QModelIndex)));
}

///**
// * 功能：
// *      重置显示组
// * 参数：
// *      １、quint32 grpIndex　组索引
// */
//void WinBarGroup::resetGroupIndex(quint32 grpIndex)
//{
//    ModelBarGroupChannel *model = static_cast<ModelBarGroupChannel *>(this->model());
//    model->resetCurrentGroup(grpIndex);
//}

void WinBarGroup::mousePressEvent(QMouseEvent *event)
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

void WinBarGroup::mouseReleaseEvent(QMouseEvent *event)
{
    m_pressed = false;
    QPoint pos = event->pos();
    QModelIndex index = indexAt(pos);
    update(index);
    int rx = m_point.x() - pos.x();
    int ry = m_point.y() - pos.y();
    if(rx>-20 && rx<20 && ry>-20 && ry<20){
        slotChannelClicked(index);
    }
    QTableView::mouseReleaseEvent(event);
}

void WinBarGroup::mouseMoveEvent(QMouseEvent *event)
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
 * 功能：
 *          添加自定义状态
 */
QStyleOptionViewItem WinBarGroup::viewOptions() const
{
    QStyleOptionViewItem option = QTableView::viewOptions();
    option.backgroundBrush = QBrush(Qt::white);
    if(m_pressed){
        option.state |= QStyle::State_Sunken;
    }

    if(CfgChannel::instance()->getSysBackgroundColor()){
        option.backgroundBrush = QBrush(Qt::black);
        option.palette.setBrush(QPalette::WindowText,QBrush(Qt::white));
    }else{
        option.backgroundBrush = QBrush(Qt::white);
        option.palette.setBrush(QPalette::WindowText,QBrush(Qt::black));
    }

    if(WinMainFrame::getBlinkStatus()){
        option.state |= QStyle::State_On;
    }else{
        option.state |= QStyle::State_Off;
    }

    return option;
}

/*
 * 功能：
 *          棒图通道item被点击
 *参数：
 *          QModelIndex index 通道item索引，用于获取通道数据
 */
void WinBarGroup::slotChannelClicked(QModelIndex index)
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


WinHBarFrame::WinHBarFrame(quint32 grpIndex, QWidget *parent) :
    QWidget(parent),m_grpIndex(grpIndex),m_horLeftModel(grpIndex, &m_horLeftFrame),
    m_horRightModel(grpIndex, &m_horRightFrame)
{
    setLayout(&m_layout);
    m_layout.setSpacing(0);
    m_layout.setContentsMargins(0,0,0,0);
    m_layout.addWidget(&m_horLeftFrame,4);
    m_layout.addWidget(&m_horRightFrame,1);

    m_horLeftFrame.setModel(&m_horLeftModel);
    m_horLeftFrame.setItemDelegate(&m_horLeftDelegate);
    connect(&m_horLeftFrame,SIGNAL(sigChangeFrame(quint32)),this,SIGNAL(sigChangeFrame(quint32)));

    m_horRightFrame.setModel(&m_horRightModel);
    m_horRightDelegate.setMargins(-1,-1,-1,-1);
    m_horRightFrame.setItemDelegate(&m_horRightDelegate);
    connect(&m_horRightFrame,SIGNAL(sigChangeFrame(quint32)),this,SIGNAL(sigChangeFrame(quint32)));
}

void WinHBarFrame::resetGroupIndex(quint32 grpIndex)
{
    m_grpIndex = grpIndex;
    m_horLeftModel.resetCurrentGroup(grpIndex);
    m_horRightModel.resetCurrentGroup(grpIndex);
}


WinBarFrame::WinBarFrame(quint32 grpIndex, QWidget *parent):
    QStackedWidget(parent),m_grpIndex(grpIndex),m_horFrame(grpIndex,parent),m_verModel(grpIndex,&m_verFrame)
{
    addWidget(&m_horFrame);
    addWidget(&m_verFrame);

    slotDirectionChanged(ID_Disp_Basic_Bar_Direction);

    m_verFrame.setModel(&m_verModel);
    m_verFrame.setItemDelegate(&m_verDelegate);

    connect(&m_horFrame,SIGNAL(sigChangeFrame(quint32)),this,SIGNAL(sigChangeFrame(quint32)));
    connect(&m_verFrame,SIGNAL(sigChangeFrame(quint32)),this,SIGNAL(sigChangeFrame(quint32)));
    connect(CfgChannel::instance(),SIGNAL(sigNormalConfigChanged(quint32)),SLOT(slotDirectionChanged(quint32)));
}

void WinBarFrame::resetGroupIndex(quint32 grpIndex)
{
    m_grpIndex = grpIndex;
    m_verModel.resetCurrentGroup(grpIndex);
    m_horFrame.resetGroupIndex(grpIndex);
}


void WinBarFrame::slotDirectionChanged(quint32 id)
{
    if(id == ID_Disp_Basic_Bar_Direction){
        if(CfgChannel::instance()->getSysDispalyBarDirection()){
            setCurrentWidget(&m_verFrame);
        }else{
            setCurrentWidget(&m_horFrame);
        }
    }
}

