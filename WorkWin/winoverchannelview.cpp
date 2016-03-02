#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QTableView>
#include <QHeaderView>
#include <QTimer>
#include <QModelIndex>
#include <QMouseEvent>
#include <QPainter>
#include <QDebug>

#include "winoverview.h"
#include "winoverchannelview.h"
#include "winoverviewchandelegate.h"
#include "winoverviewgroupdelegate.h"
#include "winchannelinfodialog.h"
#include "./DataModel/modeldigitalgroupchannel.h"
#include "./DataModel/modeloverviewchanmodel.h"
#include "./DataModel/modeloverviewgroupmodel.h"
#include "./GlobalData/cfgchannel.h"
#include "./GlobalData/cfgGlobalDef.h"
#include "./Common/authorization.h"

//总览图通用表格
WinOverviewTable::WinOverviewTable(QWidget *parent):
    QTableView(parent),m_pressed(false)
{
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//取消滚动条
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setAutoScroll(false);

    this->setSelectionMode(QAbstractItemView::NoSelection);
    this->horizontalHeader()->setVisible(false);
    this->horizontalHeader()->setHighlightSections(false);
    this->horizontalHeader()->setMinimumSectionSize(3);
    this->verticalHeader()->setVisible(false);
    this->verticalHeader()->setHighlightSections(false);
    this->verticalHeader()->setMinimumSectionSize(3);

    this->setShowGrid(false);
    this->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);//表格宽度自动拉伸
    this->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);//表格高度自动拉伸

//    this->horizontalHeader()->setMinimumWidth(5);
    this->setMouseTracking(false);
    this->setStyleSheet("WinOverviewTable{background-color:rgb(1,16,177); border:0;}");

    //connect(WinMainFrame::instance(),SIGNAL(timeout()),this,SLOT(repaint()));

}

void WinOverviewTable::mousePressEvent(QMouseEvent *event)
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

void WinOverviewTable::mouseReleaseEvent(QMouseEvent *event)
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

void WinOverviewTable::mouseMoveEvent(QMouseEvent *event)
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
 *      重载添加自定义状态
 */
QStyleOptionViewItem WinOverviewTable::viewOptions() const
{
    QStyleOptionViewItem option = QTableView::viewOptions();
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

    if(WinMainFrame::getBlinkStatus()){
        option.state |= QStyle::State_On;
    }else{
        option.state |= QStyle::State_Off;
    }

    return option;
}

/**
 * 分组显示导航条
 */
WinOverviewNavigationBar::WinOverviewNavigationBar(QWidget *parent):
    QFrame(parent)
{
    p_groupNameLabel = new QLabel(this);
    p_groupNameLabel->setObjectName("groupNameLabel");
//    p_groupNameLabel->setText(tr("GROUP 1"));
    p_groupNameLabel->setAlignment(Qt::AlignCenter);

    QPixmap icon = QPixmap(":/overview/Image/overview/go_back_icon.png");
    QPushButton *goBackBtn = new QPushButton(this);
    goBackBtn->setObjectName("goBackButton");
    goBackBtn->setFocusPolicy(Qt::NoFocus);
    goBackBtn->setIcon(QIcon(icon));
    goBackBtn->setIconSize(icon.size());
    goBackBtn->setText(tr("Go back"));

    QPushButton *goTrendBtn = new QPushButton(this);
    goTrendBtn->setObjectName("goTrendButton");
    goTrendBtn->setFocusPolicy(Qt::NoFocus);
    icon = QPixmap(":/overview/Image/overview/go_trend_icon.png");
    goTrendBtn->setIcon(QIcon(icon));
    goTrendBtn->setIconSize(icon.size());
    goTrendBtn->setText(tr("Jump to trend"));

    QPushButton *lastBtn = new QPushButton(this);
     lastBtn->setObjectName("lastButton");
     lastBtn->setFocusPolicy(Qt::NoFocus);
     icon = QPixmap(":/overview/Image/overview/left_icon.png");
     lastBtn->setIcon(QIcon(icon));
     lastBtn->setIconSize(icon.size());

    QPushButton *nextBtn = new QPushButton(this);
    nextBtn->setObjectName("nextButton");
    nextBtn->setFocusPolicy(Qt::NoFocus);
    icon = QPixmap(":/overview/Image/overview/right_icon.png");
    nextBtn->setIcon(QIcon(icon));
    nextBtn->setIconSize(icon.size());

    QHBoxLayout *horizontalLayout = new QHBoxLayout(this);
    horizontalLayout->setSpacing(6);
    horizontalLayout->setContentsMargins(20,5,20,5);
    horizontalLayout->addWidget(goBackBtn);
    horizontalLayout->addSpacerItem(new QSpacerItem(0, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    horizontalLayout->addWidget(lastBtn);
    horizontalLayout->addWidget(p_groupNameLabel);
    horizontalLayout->addWidget(nextBtn);
    horizontalLayout->addSpacerItem(new QSpacerItem(0, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    horizontalLayout->addWidget(goTrendBtn);

    this->setStyleSheet("WinOverviewNavigationBar{\
                        border:none;\
                        height:75px;\
                        border-bottom:2px solid #2E37D4;\
                        background-color:rgb(1,16,177,0);\
                        }\
                        QPushButton#goBackButton,\
                        QPushButton#goTrendButton{\
                        border-width:2px;\
                        border-style:solid;\
                        border-radius:5px;\
                        border-color:rgb(159,222,253);\
                        color:rgb(159,222,253);\
                        min-width: 125px;\
                        min-height: 55px;\
                        font-size:24px;\
                        background-color: rgb(0,0,0,0);\
                        }\
                        QPushButton#lastButton,\
                        QPushButton#nextButton{\
                        border:2px solid #9FDEFD;\
                        min-width: 45px;\
                        min-height: 45px;\
                        }\
                        QLabel#groupNameLabel{\
                        min-height: 45px;\
                        min-width:200px;\
                        margin-right：35px;\
                        margin-left：35px;\
                        font-size:26px;\
                        color:rgb(255,255,255);\
                        text-align:center;\
                        }");

        connect(goBackBtn, SIGNAL(clicked()),this,SIGNAL(sigGoBack()));
        connect(goTrendBtn,SIGNAL(clicked()),this,SIGNAL(sigGoTrend()));
        connect(lastBtn,SIGNAL(clicked()),this,SIGNAL(sigLastFrame()));
        connect(nextBtn,SIGNAL(clicked()),this,SIGNAL(sigNextFrame()));

        this->setLayout(horizontalLayout);
}

void WinOverviewNavigationBar::slotGroupNameChanged(quint32 grpIndex)
{
    QVariant value = CfgChannel::instance()->getGroupName(grpIndex);
    if(value.isValid()){
        p_groupNameLabel->setText(value.toString());
    }
}


/**
 * 分组显示单个画面（不包含滑屏）
 */
WinOverviewDivGroupFrame::WinOverviewDivGroupFrame(quint32 grpIndex,QWidget *parent) :
    QFrame(parent)
{
    QVBoxLayout *verticalLayout =new QVBoxLayout(this);
     verticalLayout->setSpacing(10);
     verticalLayout->setMargin(0);
     p_navBar = new WinOverviewNavigationBar(this);
     verticalLayout->addWidget(p_navBar);
     p_chanPanel = new SlidingScreen(0, true, false);
     verticalLayout->addWidget(p_chanPanel);
     this->setStyleSheet("WinOverviewDivGroupFrame{background-color:rgb(1,16,177)}");
    updateGroupConfig();
    setCurrentGroupFrame(grpIndex);
     connect(p_navBar, SIGNAL(sigLastFrame()), p_chanPanel, SLOT(slotMoveToPrev()));
     connect(p_navBar, SIGNAL(sigNextFrame()), p_chanPanel, SLOT(slotMoveToNext()));
     connect(p_navBar,SIGNAL(sigGoBack()),this,SIGNAL(sigGoBack()));
     connect(p_navBar,SIGNAL(sigGoTrend()),this,SLOT(slotGoTrendButtonClicked()));
     connect(p_chanPanel,SIGNAL(sigCurrentIndex(quint32)), p_navBar,SLOT(slotGroupNameChanged(quint32)));
}

WinOverviewDivGroupFrame::~WinOverviewDivGroupFrame()
{
//    this->p_chanPanel->removeAllWidget();
//    for(QMap<quint32,WinOverviewTable *>::iterator item = p_map_Frame.begin();item != p_map_Frame.end();++item){
//        item.value()->deleteLater();
//    }
    delete p_chanPanel;
}
/**
 * 功能：
 *          设置当前显示分组
 *参数值：
 *          １、quint32 grpIndex　分组索引
 */
void WinOverviewDivGroupFrame::setCurrentGroupFrame(quint32 grpIndex)
{
    p_navBar->slotGroupNameChanged(grpIndex);
    p_chanPanel->setCurrentWidget(grpIndex);
}

/**
 * 功能：
 *          更新通道配置
 */
void WinOverviewDivGroupFrame::updateGroupConfig()
{
    quint32 currentGroupIndex = this->p_chanPanel->getCurrentWidget();
    this->p_chanPanel->removeAllWidget();
//    for(QMap<quint32,WinOverviewTable *>::iterator item = p_map_Frame.begin();item != p_map_Frame.end();++item){
//        item.value()->deleteLater();
//    }
    const QList<quint32> &groupList = CfgChannel::instance()->getEffectiveGroup();
    foreach (quint32 index, groupList) {
        WinOverviewTable *chanTable = new WinOverviewTable();
        chanTable->setModel(new ModelDigitalGroupChannel(index, chanTable));
        chanTable->setItemDelegate(new WinOverviewChanDelegate(chanTable));
        connect(chanTable,SIGNAL(sigClicked(QModelIndex)),this,SLOT(slotChannelClicked(QModelIndex)));
        this->p_chanPanel->addWidgetItem(index,chanTable);
//        p_map_Frame.insert(index,chanTable);
    }
    if(!groupList.contains(currentGroupIndex)){
        currentGroupIndex = groupList.value(0,0);
    }
    this->p_chanPanel->setCurrentWidget(currentGroupIndex);
}

void WinOverviewDivGroupFrame::slotGoTrendButtonClicked()
{
    emit sigChangeFrame(Win_Trend_Panel);
}

void WinOverviewDivGroupFrame::slotChannelClicked(QModelIndex index)
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

/**
 * 所有通道单个画面（不包含滑屏）
 */
WinOverviewAllChannelFrame::WinOverviewAllChannelFrame(quint32 frameIndex, QWidget *parent) :
    WinOverviewTable(parent)
{
    this->setModel(new ModelOverviewChanModel(frameIndex, this));
    this->setItemDelegate(new WinOverviewChanDelegate(this));
    connect(this,SIGNAL(sigClicked(QModelIndex)),this,SLOT(slotChannelClicked(QModelIndex)));
}

void WinOverviewAllChannelFrame::slotChannelClicked(QModelIndex index)
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

/**
 * 所有通道全部画面（包含滑屏）
 */
WinOverviewAllChanPanel::WinOverviewAllChanPanel(QGraphicsView *parent):
    SlidingScreen(parent)
{
    updateChannelConfig();
}

//WinOverviewAllChanPanel::~WinOverviewAllChanPanel()
//{
//    this->removeAllWidget();
//    for(QMap<quint32,WinOverviewAllChannelFrame *>::iterator item = p_map_Frame.begin();item != p_map_Frame.end();++item){
//        item.value()->deleteLater();
//    }
//}

/**
 * 功能：
 *      更新通道配置
 */
void WinOverviewAllChanPanel::updateChannelConfig()
{
    this->removeAllWidget();
//    for(QMap<quint32,WinOverviewAllChannelFrame *>::iterator item = p_map_Frame.begin();item != p_map_Frame.end();++item){
//        item.value()->deleteLater();
//    }
    quint32 frameCount = CfgChannel::instance()->getEffectiveChannel().size();
    frameCount = (frameCount%100 + frameCount)/100;
    for(quint32 frameIndex=0; frameIndex<frameCount; ++frameIndex){
        WinOverviewAllChannelFrame *frame = new WinOverviewAllChannelFrame(frameIndex, this);
        connect(frame,SIGNAL(sigChangeFrame(quint32)),this,SIGNAL(sigChangeFrame(quint32)));
        this->addWidgetItem(frameIndex,frame);
//        p_map_Frame.insert(frameIndex,frame);
    }

    this->setCurrentWidget(0);
}

/**
 * 所有分组画面
 */
WinOverviewAllGroupPanel::WinOverviewAllGroupPanel(QWidget *parent):
    WinOverviewTable(parent)
{
    this->setItemDelegate(new WinOverviewGroupDelegate(this));
    this->setModel(new ModelOverviewGroupModel(this));
}

