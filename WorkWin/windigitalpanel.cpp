#include "windigitalpanel.h"
#include "GlobalData/cfgchannel.h"

//bool WinDigitalPanel::m_isLaunch = false;
WinDigitalPanel::WinDigitalPanel(QGraphicsView*parent) :
    SlidingScreen(parent)
{
//    connect(&m_timer,SIGNAL(timeout()),this,SLOT(slotTimeout()));
    slotGroupConfigChanged();
    connect(CfgChannel::instance(), SIGNAL(sigGroupConfigChanged()), this, SLOT(slotGroupConfigChanged()));
}

/**
 *功能：
 *      更新界面显示组
 */
void WinDigitalPanel::slotGroupConfigChanged()
{
    quint32 currentGroupIndex = this->getCurrentWidget();
    this->removeAllWidget();
//    for(QMap<quint32,WinDigitalGroup *>::iterator item = p_map_Frame.begin();item != p_map_Frame.end();++item){
//        item.value()->deleteLater();
//    }
//    p_map_Frame.clear();
    const QList<quint32> &groupList = CfgChannel::instance()->getEffectiveGroup();
    foreach (quint32 grpIndex, groupList) {
        WinDigitalGroup *frame = new WinDigitalGroup(grpIndex);
        this->addWidgetItem(grpIndex,frame);
//        p_map_Frame.insert(grpIndex,frame);
        connect(frame,SIGNAL(sigChangeFrame(quint32)),this,SIGNAL(sigChangeFrame(quint32)));
    }

    if(!groupList.contains(currentGroupIndex)){
        currentGroupIndex = groupList.value(0,0);
    }

    this->setCurrentWidget(currentGroupIndex);

//    m_timer.setInterval(CfgChannel::instance()->getSysAutochangeTime());
//    slotAutoChangeFrame(m_isLaunch);
}

/**
 *功能：
 *      自动更新界面显示组
 */
//void WinDigitalPanel::slotAutoChangeFrame(bool flag)
//{
//    m_isLaunch = flag;
//    if(m_isLaunch){
//        m_timer.start();
//    }
//}

//void WinDigitalPanel::slotTimeout()
//{
//    emit sigMoveLeft();
//}
