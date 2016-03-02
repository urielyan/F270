#ifndef WINOVERVIEW_H
#define WINOVERVIEW_H

#include <QWidget>
#include <QTableView>
#include <QStackedLayout>
#include <QModelIndex>
#include <QStackedWidget>

#include "winoverchannelview.h"

/**
 * 总览图
 */
class WinOverView : public QStackedWidget
{
    Q_OBJECT
public:
    WinOverView(QWidget *parent = 0);

signals:
    void sigChangeFrame(quint32 frameId);//切换显示画面

public slots:
    void slotDisplayConfigChanged(quint8 frameId, quint8 pageId);//显示配置变更
    void slotConfigChanged();//系统配置变更

private slots:
    void slotAllChannelFrameChanged(quint32 grpIndex);

protected:
    virtual void reconfig();

protected:
    WinOverviewAllChanPanel  m_allChannelView;//所有通道
    WinOverviewAllGroupPanel m_allGroupView;//所有分组
    static quint8 m_currentFrame;//当前画面
    static quint8 m_currentPage;//当前显示页面（当有效通道数超过100时有效）
};

class WinOverViewPanel : public WinOverView
{
    Q_OBJECT
public:
    WinOverViewPanel(QWidget *parent = 0);

protected:
    virtual void reconfig();

private slots:
    void slotGroupChanged(QModelIndex); //切换显示画面
    void slotGoBackButtonClicked();//返回按钮点击

private:
    WinOverviewDivGroupFrame  m_divGroupView;//单个分组
};

#endif // WINOVERVIEW_H
