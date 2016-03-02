#include "trenddigitalgroup.h"
#include "./WorkWin/winmainframe.h"
#include "./WorkWin/winchannelinfodialog.h"

TrendDigitalGroup::TrendDigitalGroup(quint32 grpIndex, QWidget *parent) :
    QTableView(parent),m_pressed(false),m_trigger(false),m_grpIndex(grpIndex)
{
    p_delegate = new TrendDigitalDelegate(this);
    p_model = new ModelTrendDigitalGroupChannel(m_grpIndex, this);
    m_pos = RIGHT;
    this->setStyleSheet("TrendDigitalGroup{background-color:rgb(1,16,177);border:0 ;}");//1,16,177

    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setAutoScroll(false);

    this->setSelectionMode(QAbstractItemView::SingleSelection);
    this->horizontalHeader()->setVisible(false);
    this->horizontalHeader()->setHighlightSections(false);
    this->verticalHeader()->setVisible(false);
    this->verticalHeader()->setHighlightSections(false);

    this->setShowGrid(false);
    this->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    this->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    this->setMouseTracking(false);

    this->setModel(p_model);
    this->setItemDelegate(p_delegate);
}

void TrendDigitalGroup::changePos(TrendDigitalGroup::Position pos)
{
    m_pos = pos;
    if (m_pos == LEFT || m_pos ==RIGHT){
        p_model->changePutDirection(true);
    }else{
        p_model->changePutDirection(false);
    }
}

bool TrendDigitalGroup::isPress()
{
    return m_pressed;
}

TrendDigitalGroup::Position TrendDigitalGroup::getPosition()
{
    return m_pos;
}

/**
 * 功能：
 *      重置显示组
 * 参数：
 *      １、quint32 grpIndex　组索引
 */
void TrendDigitalGroup::resetGroupIndex(quint32 grpIndex)
{
//    ModelTrendDigitalGroupChannel *model = static_cast<ModelTrendDigitalGroupChannel *>(this->model());
    p_model->resetCurrentGroup(grpIndex);

//    if (m_pos == LEFT || m_pos ==RIGHT){
//        p_model->changePutDirection(true);
//    }else{
//        p_model->changePutDirection(false);
    //    }
}

void TrendDigitalGroup::changeDirection(bool dir)
{
    p_model->changePutDirection(dir);
}

void TrendDigitalGroup::mousePressEvent(QMouseEvent *event)
{
    m_point = event->pos();
    m_prevIndex = this->currentIndex();
    QModelIndex index = indexAt(m_point);
    if(event->button() == Qt::LeftButton && index.isValid()){
        p_gCfgTrendGroup->setCurrentChanNo(index.data(ChannelNum).toUInt());
        m_pressed = true;
    }

    QTableView::mousePressEvent(event);
    if(m_pressed &&index == this->currentIndex()){
        m_trigger = true;
        m_rect = visualRect(index);
        update(index);
    }else{
        m_rect = QRect();
    }
}

void TrendDigitalGroup::mouseReleaseEvent(QMouseEvent *event)
{
    m_trigger = m_pressed = false;
    QPoint pos = event->pos();
    QModelIndex index = this->indexAt(pos);
    if(index.isValid()){
        int rx = m_point.x() - pos.x();
        int ry = m_point.y() - pos.y();
        if(rx>-20 && rx<20 && ry>-20 && ry<20){
            if(m_prevIndex == index){
                emit sigShowDigitalItemInfor(index);
            }
        }
        update(index);
    }
    QTableView::mouseReleaseEvent(event);
}

void TrendDigitalGroup::mouseMoveEvent(QMouseEvent *event)
{
    QPoint pos = event->pos();
    if(m_pressed && m_rect.contains(pos)){
        m_trigger = true;
    }else{ 
        m_trigger = false;
    }
    qDebug()<<m_trigger<<"Mouse moving...." <<pos;
    update(indexAt(pos));
    QWidget::mouseMoveEvent(event);
}

//void TrendDigitalGroup::resizeEvent(QResizeEvent *event)
//{
//    if (m_pos == LEFT || m_pos ==RIGHT){
//        p_model->changePutDirection(true);
//    }else{
//        p_model->changePutDirection(false);
//    }
//    QTableView::resizeEvent(event);
//}

//void TrendDigitalGroup::showEvent(QShowEvent *e)
//{
//    if (m_grpIndex != p_gCfgTrendGroup->getCurrentGroupIndex()) {
//        this->clearSelection();
//        p_gCfgTrendGroup->setCurrentChanNo(-1);
//    }
//    QTableView::showEvent(e);
//}


QStyleOptionViewItem TrendDigitalGroup::viewOptions() const
{
    QStyleOptionViewItem option = QTableView::viewOptions();

    if(CfgChannel::instance()->getSysBackgroundColor()){
        option.backgroundBrush = QBrush(Qt::black);
        option.palette.setBrush(QPalette::WindowText,QBrush(Qt::white));
    }else{
        option.backgroundBrush = QBrush(Qt::white);
        option.palette.setBrush(QPalette::WindowText,QBrush(Qt::black));
    }

    if(m_trigger){
        option.state |= QStyle::State_Sunken;
    }

    if(WinMainFrame::getBlinkStatus()){
        option.state |= QStyle::State_On;
    }else{
        option.state |= QStyle::State_Off;
    }

    option.palette.setColor(QPalette::Active,QPalette::Highlight,QColor(Qt::transparent));
    return option;
}

void TrendDigitalGroup::slotChannelUnselect()
{
    this->selectionModel()->clearCurrentIndex();
    this->selectionModel()->clearSelection();
}

