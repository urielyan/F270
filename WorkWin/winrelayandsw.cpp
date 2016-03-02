#include <QPushButton>
#include <QLabel>
#include <QTableView>
#include <QHeaderView>
#include <QMouseEvent>
#include <QModelIndex>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDebug>

#include "winrelayandsw.h"
#include "winchannelinfodialog.h"
#include "winsoftswitchdialog.h"
#include "./GlobalData/cfgchannel.h"
#include "./GlobalData/cfgGlobalDef.h"
#include "./Common/authorization.h"

//继电器和内部开关模块通用表格
WinRSTable::WinRSTable(QWidget *parent):
    QTableView(parent),m_pressed(false)
{
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setAutoScroll(false);

    this->setSelectionMode(QAbstractItemView::NoSelection);
    this->horizontalHeader()->setVisible(false);
    this->horizontalHeader()->setHighlightSections(false);
    this->verticalHeader()->setVisible(false);
    this->verticalHeader()->setHighlightSections(false);
    this->verticalHeader()->setMinimumSectionSize(10);

    this->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    this->setShowGrid(false);
    this->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    this->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    this->setMouseTracking(false);
    this->setStyleSheet("WinRSTable{background-color:rgb(1,16,177); border:0;}");

}


void WinRSTable::mousePressEvent(QMouseEvent *event)
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

void WinRSTable::mouseReleaseEvent(QMouseEvent *event)
{
    m_pressed = false;
    QPoint pos = event->pos();
    QModelIndex index = indexAt(pos);
    update(index);
    int rx = m_point.x() - pos.x();
    int ry = m_point.y() - pos.y();
    if(rx>-20 && rx<20 && ry>-20 && ry<20){
        emit sigClicked(index);
    }
    QTableView::mouseReleaseEvent(event);
}

void WinRSTable::mouseMoveEvent(QMouseEvent *event)
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
 *      添加自定义状态
 */
QStyleOptionViewItem WinRSTable::viewOptions() const
{
    QStyleOptionViewItem option = QTableView::viewOptions();
    option.backgroundBrush = QBrush(Qt::white);
    if(m_pressed){
        option.state |= QStyle::State_Sunken;
    }

    if(CfgChannel::instance()->getSysBackgroundColor()){
        option.backgroundBrush = QBrush(Qt::black);
        option.palette.setBrush(QPalette::Foreground,QBrush(Qt::white));
    }else{
        option.backgroundBrush = QBrush(Qt::white);
        option.palette.setBrush(QPalette::Foreground,QBrush(Qt::black));
    }

//    if(WinMainFrame::getBlinkStatus()){
//        option.state |= QStyle::State_On;
//    }else{
//        option.state |= QStyle::State_Off;
//    }
    return option;
}

quint32 WinRSNavigationBar::m_formIndex = WIN_SW_FRAME;
WinRSNavigationBar::WinRSNavigationBar(QWidget *parent):
    QFrame(parent)
{
    setLayout(&m_horizontalLayout);
    m_horizontalLayout.setSpacing(6);
    m_horizontalLayout.setContentsMargins(25,5,25,5);
    m_horizontalLayout.addWidget(&m_shrinkBtn);
    m_horizontalLayout.addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    m_horizontalLayout.addWidget(&m_prevBtn);
    m_horizontalLayout.addWidget(&m_formNameLabel);
    m_horizontalLayout.addWidget(&m_nextBtn);
    m_horizontalLayout.addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));

    m_formNameLabel.setObjectName("formNameLabel");
    m_formNameLabel.setText(tr("Internal switch"));
    m_formNameLabel.setAlignment(Qt::AlignCenter);

    m_shrinkBtn.setObjectName("shrinkButton");
    m_shrinkBtn.setFocusPolicy(Qt::NoFocus);
    m_shrinkBtn.setText(tr("Display all"));
//    m_shrinkBtn.setIcon(QIcon(":/summary/Image/summary/left.png"));

    m_prevBtn.setObjectName("previousButton");
    m_prevBtn.setFocusPolicy(Qt::NoFocus);
    m_prevBtn.setIcon(QIcon(":/overview/Image/overview/left_icon.png"));
    m_prevBtn.setIconSize(QSize(31,31));
    //m_prevBtn.setText("<");

    m_nextBtn.setObjectName("nextButton");
    m_nextBtn.setFocusPolicy(Qt::NoFocus);
    m_nextBtn.setIcon(QIcon(":/overview/Image/overview/right_icon.png"));
    m_nextBtn.setIconSize(QSize(31,31));
    //m_nextBtn.setText(">");


    setStyleSheet("WinRSNavigationBar{\
                        border:none;\
                        height:75px;\
                        border-bottom:2px solid #2E37D4;\
                        background-color:rgb(1,16,177,0);\
                        }\
                        QPushButton#shrinkButton{\
                        width:125px;\
                        height:55px;\
                        background-color:rgb(0,0,0,0);\
                        border-style: solid;\
                        border-width: 2px;\
                        border-radius: 4px;\
                        border-color:rgb(127,200,235);\
                        color:rgb(127,200,235);\
                        font-size: 20px; \
                        text-align: center;\
                        }\
                        QPushButton#shrinkButton:pressed{\
                        background-color:rgb(0,0,0,100);\
                        }\
                        QPushButton#previousButton{\
                        width:45px;\
                        height:45px;\
                        background-color:rgb(0,0,0,0);\
                        border-width:2px;\
                        border-style:solid;\
                        border-color:rgb(127,200,235);\
                        color:rgb(127,200,235);\
                        font-size: 30px; \
                        text-align: center;\
                        }\
                        QPushButton#previousButton:pressed{\
                        background-color:rgb(0,0,0,100);\
                        }\
                        QPushButton#nextButton{\
                        width:45px;\
                        height:45px;\
                        background-color:rgb(0,0,0,0);\
                        border-width:2px;\
                        border-style:solid;\
                        border-color:rgb(127,200,235);\
                        color:rgb(127,200,235);\
                        font-size: 30px; \
                        text-align: center;\
                        }\
                        QPushButton#nextButton:pressed{\
                        background-color:rgb(0,0,0,100);\
                        }\
                        QLabel#formNameLabel{\
                        min-width: 300px;\
                        min-height: 45px;\
                        margin-right：25px;\
                        margin-left：25px;\
                        font-size:28px;\
                        color:rgb(255,255,255);\
                        }");

    connect(&m_prevBtn,SIGNAL(clicked()),this,SIGNAL(sigPreviousButtonClick()));
    connect(&m_nextBtn,SIGNAL(clicked()),this,SIGNAL(sigNextButtonClick()));
    connect(&m_shrinkBtn,SIGNAL(clicked()),this,SIGNAL(sigShrinkButtonClick()));
}

/**
 * 功能：
 *      当前画面名称更改
 * 参数：
 *      １、 frameIndex　帧索引
 */
void WinRSNavigationBar::slotFormNameChanged(quint32 formIndex)
{
    if(formIndex == WIN_RELAY_FRAME){
        m_formNameLabel.setText(tr("Do channel"));
    }else if(formIndex == WIN_SW_FRAME){
        m_formNameLabel.setText("Internal switch");
    }
    m_formIndex = formIndex;
}

/**
 * 单画面图
 */
WinRSSingleFrame::WinRSSingleFrame(QWidget *parent):
    QFrame(parent),m_pressed(false)
{
    setLayout(&m_layout);
    m_layout.setSpacing(0);
    m_layout.setMargin(0);
    m_layout.addWidget(&m_relayTitle);
    m_layout.addWidget(&m_relayTable);
    m_layout.addWidget(&m_swTitle);
    m_layout.addWidget(&m_swTable);

    m_maskView.setObjectName("MaskView");
    m_maskView.setVisible(false);

    m_relayTable.setItemDelegate(&m_relayDelegate);
    m_relayTable.setModel(&m_relayModel);
    m_relayTable.viewport()->setProperty("frameIndex",WIN_RELAY_FRAME);
    m_relayTable.viewport()->installEventFilter(this);
    m_relayTable.setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    m_swTable.setItemDelegate(&m_swDelegate);
    m_swTable.setModel(&m_swModel);
    m_swTable.viewport()->setProperty("frameIndex",WIN_SW_FRAME);
    m_swTable.viewport()->installEventFilter(this);
    m_swTable.setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    connect(&m_swTable,SIGNAL(sigClicked(QModelIndex)),this,SLOT(slotSwitchItemClicked(QModelIndex)));

    m_relayTitle.setObjectName("RelayLabel");
    m_relayTitle.setText(tr("Do channel"));
    m_relayTitle.setAlignment(Qt::AlignCenter);
    m_relayTitle.setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    m_swTitle.setObjectName("SoftSwitchLabel");
    m_swTitle.setText(tr("Internal switch"));
    m_swTitle.setAlignment(Qt::AlignCenter);
    m_swTitle.setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    setStyleSheet("WinRSSingleFrame{\
                        background-color:rgb(1,16,177);\
                        }\
                        QLabel#RelayLabel,\
                        QLabel#SoftSwitchLabel{\
                        height:45px;\
                        background-image:url(:/relay/Image/relay/divider.png);\
                        background-position:center;\
                        background-repeat:repeat-x;\
                        padding-top:3px;\
                        padding-bottom:3px;\
                        font-size:20px;\
                        color:rgb(127,200,235);\
                        }\
                        QFrame#MaskView{\
                        background-color:rgb(100,50,50,80);\
                        border-radius:0px;\
                        border:3px solid;\
                        border-color:rgb(0,0,255,100);\
                        }");
}

bool WinRSSingleFrame::eventFilter(QObject *watched, QEvent *event)
{
    if(getDoStatus()){
        if(event->type() == QEvent::MouseButtonPress){
            if(this->parent()->eventFilter(this,event)){
                return true;
            }
            QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
            if(mouseEvent->button() == Qt::LeftButton && !m_pressed){
                m_pressed = true;
                QWidget *widget = static_cast<QWidget *>(watched);
                widget->setGeometry(widget->x()+1, widget->y()+1,widget->width(),widget->height());
                m_maskView.setParent(widget);
                m_maskView.setGeometry(0,0,widget->width(),widget->height());
                m_maskView.setVisible(m_pressed);
                return true;
            } else{
                return false;
            }
        }else if(event->type() == QEvent::MouseButtonRelease){
            if(this->parent()->eventFilter(this,event)){
                return true;
            }
            QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
            if(mouseEvent->button() == Qt::LeftButton && m_pressed){
                m_pressed = false;
                QWidget *widget = static_cast<QWidget *>(watched);
                widget->setGeometry(widget->x()-1, widget->y()-1,widget->width(),widget->height());
                m_maskView.setVisible(m_pressed);
                QVariant value = widget->property("frameIndex");
                if(value.isValid()){
                    emit sigFrameClick(value.toUInt());
                }
                return true;
            }else{
                return false;
            }
        }
    }
    return QWidget::eventFilter(watched, event);
}

void WinRSSingleFrame::paintEvent(QPaintEvent *e)
{
    if(!getDoStatus()){
        m_relayTable.setVisible(false);
        m_relayTitle.setVisible(false);
        m_swTable.setVisible(true);
        m_swTitle.setVisible(true);
    }else{
        if(WinRelayAndSW::getCurrentIndex() == WIN_SIGNAL_FRAME){
            m_relayTable.setVisible(true);
            m_relayTitle.setVisible(true);
            m_swTable.setVisible(true);
            m_swTitle.setVisible(true);
        }else{
            if(WinRSNavigationBar::getFormIndex() == WIN_SW_FRAME){
                m_relayTable.setVisible(false);
                m_relayTitle.setVisible(false);
                m_swTable.setVisible(true);
                m_swTitle.setVisible(true);
            }else{
                m_relayTable.setVisible(true);
                m_relayTitle.setVisible(true);
                m_swTable.setVisible(false);
                m_swTitle.setVisible(false);
            }
        }
    }

    QFrame::paintEvent(e);
}

void WinRSSingleFrame::slotSwitchItemClicked(QModelIndex index)
{
    if(Authorization::instance()->getLoginStatus() == Authorization::Auth_LoginStatus_Logout){
        WinInfoListDialog::instance()->showMsg(818);
    }else{
        QVariant value = index.data(ChannelValidity);
        if(value.isValid()){
            WinSoftSwitchDialog dialog(value.toUInt());
            dialog.exec();
        }
    }
}

/*
 * 功能：
 *          获取系统DO模块是否有效
 * 返回值：
 *          bool DO模块状态(TRUE/有效)
 *
 */
bool WinRSSingleFrame::getDoStatus()
{
    return CfgChannel::instance()->getExistChannelDO().count() > 0;
}

//继电器和内部开关分两屏显示
WinRSMultiFrame::WinRSMultiFrame(QWidget *parent,quint32 frameIndex):
    QFrame(parent),m_slinder(0,true,false)
{
    setLayout(&m_layout);
    m_layout.setSpacing(0);
    m_layout.setMargin(0);
    m_layout.addWidget(&m_navBar);
    m_layout.addWidget(&m_slinder);

    WinRSTable *relayTable = new WinRSTable(this);
    relayTable->setItemDelegate(new WinRelayDelegate(this));
    relayTable->setModel(new ModelRelayModel(this));


    WinRSTable *swTable = new WinRSTable(this);
    swTable->setItemDelegate(new WinSoftSwitchDelegate(this));
    swTable->setModel(new ModelSoftSwitchModel(this));

    m_slinder.addWidgetItem(WIN_RELAY_FRAME, relayTable);
    m_slinder.addWidgetItem(WIN_SW_FRAME, swTable);

    connect(relayTable,SIGNAL(sigClicked(QModelIndex)),this,SLOT(slotRelayItemClicked(QModelIndex)));
    connect(swTable,SIGNAL(sigClicked(QModelIndex)),this,SLOT(slotSwitchItemClicked(QModelIndex)));
    connect(&m_navBar,SIGNAL(sigShrinkButtonClick()),this,SIGNAL(sigShrinkButtonClick()));
    connect(&m_navBar,SIGNAL(sigPreviousButtonClick()), &m_slinder,SLOT(slotMoveToPrev()));
    connect(&m_navBar,SIGNAL(sigNextButtonClick()), &m_slinder,SLOT(slotMoveToNext()));
    connect(&m_slinder,SIGNAL(sigCurrentIndex(quint32)), &m_navBar,SLOT(slotFormNameChanged(quint32)));

    setStyleSheet("WinRSMultiFrame{background-color:rgb(1,16,177);}");
    setCurrentFrame(frameIndex);
}


/**
 * 功能：
 *      继电器被点击
 */
void WinRSMultiFrame::slotRelayItemClicked(QModelIndex index)
{
    if(Authorization::instance()->getLoginStatus() == Authorization::Auth_LoginStatus_Logout){
        WinInfoListDialog::instance()->showMsg(818);
    }else{
        QVariant value = index.data(ChannelValidity);
        if(value.isValid()){
            WinDOChannelDialog::instance().resetCurrentChannelNumber(value.toUInt());
            WinDOChannelDialog::instance().show();
        }
    }
}

/**
 * 功能：
 *      内部开关被点击
 */
void WinRSMultiFrame::slotSwitchItemClicked(QModelIndex index)
{
    if(Authorization::instance()->getLoginStatus() == Authorization::Auth_LoginStatus_Logout){
        WinInfoListDialog::instance()->showMsg(818);
    }else{
        QVariant value = index.data(ChannelValidity);
        if(value.isValid()){
            WinSoftSwitchDialog dialog(value.toUInt());
            dialog.exec();
        }
    }
}

/**
 * 功能：
 *      设置当前显示画面
 * 参数：
 *      １、 frameIndex　帧索引
 */
void WinRSMultiFrame::setCurrentFrame(quint32 frameIndex)
{
    m_slinder.setCurrentWidget(frameIndex);
}

int WinRelayAndSW::m_index = WIN_SIGNAL_FRAME;

WinRelayAndSW::WinRelayAndSW(QWidget *parent) :
    QStackedWidget(parent)
{
    addWidget(&m_sFrame);
    addWidget(&m_mFrame);

    connect(&m_sFrame,SIGNAL(sigFrameClick(quint32)),this,SLOT(slotZoomInFrame(quint32)));
    connect(&m_mFrame,SIGNAL(sigShrinkButtonClick()),this,SLOT(slotZoomOutFrame()));

    switchFrame(m_index);
}

/**
 * 功能：
 *      显示内部开关和Ｄｏ画面
 */
void WinRelayAndSW::slotZoomOutFrame()
{
    m_index = WIN_SIGNAL_FRAME;
    setCurrentWidget(&m_sFrame);
}

void WinRelayAndSW::slotZoomInFrame(quint32 frameIndex)
{
    m_index = WIN_MULTI_FRAME;
    m_mFrame.setCurrentFrame(frameIndex);
    setCurrentWidget(&m_mFrame);
}

void WinRelayAndSW::switchFrame(int fmIndex)
{
    if((CfgChannel::instance()->getExistChannelDO().count() > 0) && (fmIndex == WIN_MULTI_FRAME)){
        slotZoomInFrame(WIN_SW_FRAME);
    }else{
        slotZoomOutFrame();
    }
}
