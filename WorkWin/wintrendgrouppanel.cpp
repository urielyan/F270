#include "wintrendgrouppanel.h"
#include "winmainframe.h"
#include "GlobalData/cfgchannel.h"
#include <QDir>

WinTrendGroupPanel::WinTrendGroupPanel(QGraphicsView*parent) :
    SlidingScreen(parent,false,true)
{
    p_messageBar = new DisPlayMessage(this);
    p_messageBar->setSize(DESKTOP_WIDTH, DESKTOP_HEIGHT * 0.94);
    p_messageBar->resieWidget();
    p_handWritePad = new HandWritePad(this);
    p_messageBar->setVisible(false);
    p_handWritePad->setVisible(false);

    connect(this,SIGNAL(sigCurrentIndex(quint32)),this,SLOT(slotGroupIndexChanged(quint32)));
    slotGroupConfigChanged();
    connect(CfgChannel::instance(), SIGNAL(sigGroupConfigChanged()), this, SLOT(slotGroupConfigChanged()));

    slotTrendConfigChanged(ID_Disp_Trend_Set_Direction);
    connect(CfgChannel::instance(), SIGNAL(sigNormalConfigChanged(quint32)), this, SLOT(slotTrendConfigChanged(quint32)));

    //TODO----测试按钮，待删除
//    m_pb_testStart.setParent(this);
//    m_pb_testStart.setText("testStart");
//    m_pb_testStart.move(0,0);
//    m_pb_testStop.setParent(this);
//    m_pb_testStop.setText("testStop");
//    m_pb_testStop.move(500,0);
//    m_pb_testReconfig.setParent(this);
//    m_pb_testReconfig.setText("testReconfig");
//    m_pb_testReconfig.move(200, 0);
//    connect(&m_pb_testStart, SIGNAL(clicked()), this, SLOT(slotTestStart()));
//    connect(&m_pb_testStop, SIGNAL(clicked()), this, SLOT(slotTestStop()));
//    connect(&m_pb_testReconfig, SIGNAL(clicked()), this, SLOT(slotTestReconfig()));
}

WinTrendGroupPanel::~WinTrendGroupPanel()
{
    delete p_messageBar;
    delete p_handWritePad;
}


void WinTrendGroupPanel::setCharMsg(int pos, QString msg)
{
    WinTrendGroup *pTrend = (WinTrendGroup *)this->getGroupWidget(this->getCurrentWidget());
    pTrend->setCharMsg(pos, msg);
}

void WinTrendGroupPanel::switchToHistoryReview(bool isDisp, qint64 recordIndex, qint64 dataEndTime)
{
    p_gCfgTrendGroup->setWorkMode(BrowseHistory);

    p_gCfgTrendGroup->switchToHistoryRecordMode(isDisp, recordIndex, dataEndTime);
    slotGroupConfigChanged();
}

void WinTrendGroupPanel::switchToRealtimeReview()
{
    if (p_gCfgTrendGroup->getCurrentWorkMode() == BrowseHistory) {
        p_gCfgTrendGroup->setWorkMode(RealTimeRecord);

        if (!p_gCfgTrendGroup->isSameIDofRealAndHistory()) {
            slotGroupConfigChanged();
        }
    }
}

void WinTrendGroupPanel::slotGroupConfigChanged()
{
    m_frames.clear();
    this->removeAllWidget(); //释放上次new 出来的内存

    /*实时模式，根据当前系统配置进行组窗体分配*/
    if (p_gCfgTrendGroup->getCurrentWorkMode() == RealTimeRecord) {
        const QList<quint32> &groupList = CfgChannel::instance()->getEffectiveGroup();
        foreach (quint32 grpIndex, groupList) {
            WinTrendGroup *frame = new WinTrendGroup(grpIndex);
            connect(frame,SIGNAL(sigChangeFrame(quint32)),this,SIGNAL(sigChangeFrame(quint32)));
            connect(frame, SIGNAL(sigOverViewHide()), this, SLOT(slotOverViewHide()));
            connect(frame, SIGNAL(sigOverViewShow()), this, SLOT(slotOverViewShow()));
            this->addWidgetItem(grpIndex,frame);
            m_frames.append(frame);
        }

        m_currentIndex = p_gCfgTrendGroup->getCurrentGroupIndex();
        if (!groupList.contains(m_currentIndex)) {
            p_gCfgTrendGroup->setCurrentGroupIndex(groupList.at(0));
            m_currentIndex = groupList.value(0,0);
        }

    } else {    /*历史模式，根据历史数据缓冲中的组信息进行组窗体分配*/
        quint32 groupNum = p_gCfgTrendGroup->getHistoryTrendBuf()->getGroupInforHead().groupNum;
        QList<quint32> groupList;
        for (quint32 i=0; i<groupNum; i++) {
            groupList.append(p_gCfgTrendGroup->getHistoryTrendBuf()->getGroupInfor(i).grpIndex);
        }
        foreach (quint32 grpIndex, groupList) {
            WinTrendGroup *frame = new WinTrendGroup(grpIndex);
            connect(frame,SIGNAL(sigChangeFrame(quint32)),this,SIGNAL(sigChangeFrame(quint32)));
            connect(frame, SIGNAL(sigOverViewHide()), this, SLOT(slotOverViewHide()));
            connect(frame, SIGNAL(sigOverViewShow()), this, SLOT(slotOverViewShow()));
            this->addWidgetItem(grpIndex,frame);
            m_frames.append(frame);
        }

        m_currentIndex = p_gCfgTrendGroup->getCurrentGroupIndex();
        if (!groupList.contains(m_currentIndex)) {
            p_gCfgTrendGroup->setCurrentGroupIndex(groupList.at(0));
            m_currentIndex = groupList.value(0,0);
        }
    }

    this->setCurrentWidget(m_currentIndex);
    WinTrendGroup *p = (WinTrendGroup*)getGroupWidget(m_currentIndex);
    p->loadHandWritePad(p_handWritePad);
    p->loadMessageBar(p_messageBar);
}

void WinTrendGroupPanel::slotOverViewHide()
{
    foreach (WinTrendGroup *p, m_frames) {
        p->slotHideOverViewTrend();
    }
}

void WinTrendGroupPanel::slotOverViewShow()
{
    foreach (WinTrendGroup *p, m_frames) {
        p->slotShowQverViewTrend();
    }
}

void WinTrendGroupPanel::slotTrendConfigChanged(quint32 ID)
{
    if(ID != ID_Disp_Trend_Set_Direction)
        return;

    if (DevConfigPtr->displayTrend.trendDisplayDirection) {
        m_isEnableXDir = true;
        m_isEnableYDir = false;
    } else {
        m_isEnableXDir = false;
        m_isEnableYDir = true;
    }

    foreach (WinTrendGroup *pTrendGrp, m_frames) {
        pTrendGrp->slotTrendConfigChanged();
    }
}

void WinTrendGroupPanel::slotGroupIndexChanged(quint32 grpIndex)
{
    m_currentIndex = grpIndex;
    WinTrendGroup *p = (WinTrendGroup*)getGroupWidget(m_currentIndex);
    p->loadHandWritePad(p_handWritePad);
    p->loadMessageBar(p_messageBar);

    p_gCfgTrendGroup->setCurrentGroupIndex(grpIndex);
}

//void WinTrendGroupPanel::slotTestStart()
//{
//    switchToHistoryReview(true, 1, 1448444120664);
//}

//void WinTrendGroupPanel::slotTestStop()
//{
//    switchToRealtimeReview();
//}

//void WinTrendGroupPanel::slotTestReconfig()
//{

//}
