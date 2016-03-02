#include <QScroller>
#include <QScrollerProperties>
#include "winchannelselector.h"
#include "winmenugroup.h"
#include "../GlobalData/cfgchannel.h"
#include "../GlobalData/configtreeobject.h"
#include "../GlobalData/configobject.h"

WinChannelSelector::WinChannelSelector(QWidget *parent) :
    QScrollArea(parent),p_pMenu(NULL),m_drag(false),p_pGroup(NULL)
{
    p_vLayout = new QVBoxLayout;
    QWidget *pWidget = new QWidget;
    p_vLayout->setSpacing(0);
    p_vLayout->setContentsMargins(0, 0, 0, 1);
    pWidget->setLayout(p_vLayout);
    this->setWidget(pWidget);
    this->setWidgetResizable(true);
    setStyleSheet("QScrollBar:vertical {\
            height : 4px; width: 2px;\
            background:rgba(0,0,0,10%);\
            margin:0px,0px,0px,0px;\
            padding-top:0px;   \
            padding-bottom:0px;}\
            \
        QScrollBar::handle:vertical {\
            background: rgba(255,255,255,100%);\
            }\
        QWidget {background:rgba(0,0,0,0%); border : 0px}"
       );
       connect(CfgChannel::instance(),SIGNAL(sigBasicChannelChanged(quint8, bool)),this,SLOT(slotResetChanSelectMenus()));
}

void WinChannelSelector::slotResetChanSelectMenus()
{
    RemoveMenus();
    reloadChanSelectMenu();
    p_vLayout->addStretch();
}

void WinChannelSelector::slotResetMenus()
{
    RemoveMenus();
    reloadMenu();
    p_vLayout->addStretch();
}

void WinChannelSelector::addMenu(Menu *menu)
{
    connect(menu,SIGNAL(clicked()),this,SLOT(slotMenuClicked()));
    p_vLayout->addWidget(menu);
}

void WinChannelSelector::RemoveMenu(Menu *menu)
{
    disconnect(menu,SIGNAL(clicked()),this,SLOT(slotMenuClicked()));
    p_vLayout->removeWidget(menu);
}

void WinChannelSelector::RemoveMenus()
{
    int count = p_vLayout->count();
    while(count--){
        QLayoutItem * item = p_vLayout->takeAt(0);
        delete item->widget();
        delete item;
    }
}

void WinChannelSelector::slotMenuClicked()
{
    if(p_pMenu !=NULL && p_pGroup != NULL){
        Menu *menu = static_cast<Menu *>(this->sender());
        p_pMenu->setMenuData(menu->getMenuData());
        SysConfigSlidingScreen *slider = p_pGroup->getSlidingScreen();
//        slider->setPreWidget(p_pGroup);
        slider->moveToPreWidget();
        p_pMenu = NULL;
        p_pGroup = NULL;
    }
}

void WinChannelSelector::showSelector(Menu *pMenu, WinMenuGroup *pGroup)
{
    p_pMenu = pMenu;
    p_pGroup = pGroup;
    initMenu();
    SysConfigSlidingScreen *slider = p_pGroup->getSlidingScreen();
    slider->setNextWidget(this);
    slider->moveToNextWidget();
}

void WinChannelSelector::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        m_pos = event->pos().y();
        m_drag = true;
    }
    QScrollArea::mousePressEvent(event);
}

void WinChannelSelector::mouseMoveEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton || m_drag){
        int span =  m_pos - event->y();
        this->verticalScrollBar()->setValue(this->verticalScrollBar()->value() + span);
        m_pos = event->pos().y();
    }else{
        m_drag = true;
        m_pos = event->pos().y();
    }
    QScrollArea::mouseMoveEvent(event);
}

void WinChannelSelector::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        m_pos = event->pos().y();
        m_drag = false;
    }
    QScrollArea::mouseReleaseEvent(event);
}

void WinChannelSelector::resizeEvent(QResizeEvent *event)
{
    this->verticalScrollBar()->setRange(0,this->widget()->height()-this->viewport()->height());
    QScrollArea::resizeEvent(event);
}

WinAIChanSelector *WinAIChanSelector::instance()
{
    static WinAIChanSelector  instance;
    return &instance;
}

WinAIChanSelector::WinAIChanSelector(QWidget *parent):
    WinChannelSelector(parent),m_map_AI(CfgChannel::instance()->getExistChannelAI())
{
    slotResetChanSelectMenus();
}

void WinAIChanSelector::reloadChanSelectMenu()
{
    foreach(QString str, m_map_AI.keys()){
        Menu *menu = MenuFactory::instance()->createMenu(str, MenuType_item);
        menu->setMenuData(str);
        addMenu(menu);
    }
}

WinDIChanSelector *WinDIChanSelector::instance()
{
    static WinDIChanSelector  instance;
    return &instance;
}

WinDIChanSelector::WinDIChanSelector(QWidget *parent):
    WinChannelSelector(parent),m_map_DI(CfgChannel::instance()->getExistChannelDI())
{
    slotResetChanSelectMenus();
}

void WinDIChanSelector::reloadChanSelectMenu()
{
    foreach(QString str, m_map_DI.keys()){
        Menu *menu = MenuFactory::instance()->createMenu(str, MenuType_item);
        menu->setMenuData(str);
        addMenu(menu);
    }
}

WinDOChanSelector *WinDOChanSelector::instance()
{
    static WinDOChanSelector  instance;
    return &instance;
}

WinDOChanSelector::WinDOChanSelector(QWidget *parent):
    WinChannelSelector(parent),m_map_DO(CfgChannel::instance()->getExistChannelDO())
{
    slotResetChanSelectMenus();
}

void WinDOChanSelector::reloadChanSelectMenu()
{
    foreach(QString str, m_map_DO.keys()){
        Menu *menu = MenuFactory::instance()->createMenu(str, MenuType_item);
        menu->setMenuData(str);
        addMenu(menu);
    }
}

WinMathChanSelector *WinMathChanSelector::instance()
{
    static WinMathChanSelector  instance;
    return &instance;
}

WinMathChanSelector::WinMathChanSelector(QWidget *parent):
    WinChannelSelector(parent),m_map_Math(CfgChannel::instance()->getExistChannelMath())
{
    slotResetChanSelectMenus();
}

void WinMathChanSelector::reloadChanSelectMenu()
{
    foreach(QString str, m_map_Math.keys()){
        Menu *menu = MenuFactory::instance()->createMenu(str, MenuType_item);
        menu->setMenuData(str);
        addMenu(menu);
    }
}

WinCommChanSelector *WinCommChanSelector::instance()
{
    static WinCommChanSelector  instance;
    return &instance;
}

WinCommChanSelector::WinCommChanSelector(QWidget *parent):
    WinChannelSelector(parent),m_map_Comm(CfgChannel::instance()->getExistChannelComm())
{
    slotResetChanSelectMenus();
}

void WinCommChanSelector::reloadChanSelectMenu()
{
    foreach(QString str, m_map_Comm.keys()){
        Menu *menu = MenuFactory::instance()->createMenu(str, MenuType_item);
        menu->setMenuData(str);
        addMenu(menu);
    }
}
