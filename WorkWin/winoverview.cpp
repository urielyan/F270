#include <QPalette>
#include <QPainter>
#include <QDebug>
#include <QTime>

#include "winoverview.h"
#include "winoverviewchandelegate.h"
#include "winoverviewgroupdelegate.h"
#include "GlobalData/cfgGlobalDef.h"
#include "GlobalData/cfgchannel.h"

quint8 WinOverView::m_currentFrame = 0;//默认显示所有分组
quint8 WinOverView::m_currentPage = 0;//默认显示通道页面

WinOverView::WinOverView(QWidget *parent) :
    QStackedWidget(parent)
{
    addWidget(&m_allGroupView);
    addWidget(&m_allChannelView);

    connect(&m_allChannelView,SIGNAL(sigChangeFrame(quint32)),this,SLOT(slotAllChannelFrameChanged(quint32)));

    slotDisplayConfigChanged(m_currentFrame,m_currentPage);
}

/**
 * 功能：
 *      显示配置变更
 * 参数：
 *      quint32 flag 显示面板标记（当显示全部通道面板时标记当前通道画面）
 */
void WinOverView::slotDisplayConfigChanged(quint8 frameId, quint8 pageId)
{
    m_currentFrame = frameId;
    if(frameId != 0){
        quint32 count = CfgChannel::instance()->getEffectiveChannel().size();
        if(count > 100 && pageId > 0 && pageId * 100 < count ){
            m_currentPage = pageId;
        }else{
            m_currentPage = 0;
        }
        m_allChannelView.setCurrentWidget(m_currentPage);
    }
    setCurrentIndex(m_currentFrame);
}

/**
 * 功能：
 *      显示配置变更
 */
void WinOverView::slotConfigChanged()
{
    m_currentFrame = 0;
    m_currentPage = 0;
    reconfig();
}

void WinOverView::reconfig()
{
    m_allChannelView.updateChannelConfig();
}

void WinOverView::slotAllChannelFrameChanged(quint32 grpIndex)
{
    m_currentPage = grpIndex;
}

WinOverViewPanel::WinOverViewPanel(QWidget *parent) :
    WinOverView(parent)
{
    addWidget(&m_divGroupView);
    connect(&m_divGroupView,SIGNAL(sigGoBack()),this,SLOT(slotGoBackButtonClicked()));
    connect(&m_divGroupView,SIGNAL(sigChangeFrame(quint32)),this,SIGNAL(sigChangeFrame(quint32)));

    connect(&m_allGroupView,SIGNAL(clicked(QModelIndex)),this,SLOT(slotGroupChanged(QModelIndex)));
    connect(&m_allChannelView,SIGNAL(sigChangeFrame(quint32)),this,SIGNAL(sigChangeFrame(quint32)));
}

/**
 * 功能：
 *      切换到分组显示画面
 */
void WinOverViewPanel::slotGroupChanged(QModelIndex index)
{
    Q_ASSERT(index.isValid());

    QVariant value = index.data(ChannelValidity);
    if(value.isValid()){
        m_divGroupView.setCurrentGroupFrame(value.toUInt());
        setCurrentWidget(&m_divGroupView);
    }
}

/**
 * 功能：
 *      返回到显示所有分组
 */
void WinOverViewPanel::slotGoBackButtonClicked()
{
    setCurrentWidget(&m_allGroupView);
}


void WinOverViewPanel::reconfig()
{
    m_allChannelView.updateChannelConfig();
    m_divGroupView.updateGroupConfig();
}
