#ifndef WINTRENDGROUP_H
#define WINTRENDGROUP_H
#include <QDebug>

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>

#include "./TrendGroupComponent/trenddetailwidget.h"
#include "./TrendGroupComponent/trendoverviewwidget.h"
#include "./Common/displaymessage.h"
#include "./Common/handwritepad.h"
#include "./TrendGroupComponent/trendrulerwidget.h"
#include "./TrendGroupComponent/trenddigitalgroup.h"

#include "./GlobalData/cfgtrendgroup.h"

extern CfgTrendGroup  *p_gCfgTrendGroup;

class WinTrendGroup : public QWidget
{
    Q_OBJECT
public:
    explicit WinTrendGroup( quint32 index = 0, QWidget *parent = 0);

    void setGroupIndex(quint32 groupIndex);
    void resetTrendAllGroup();
    void setCharMsg(int pos, QString msg);       //pos消息的位置（1~100）,msg消息内容
    void loadMessageBar(DisPlayMessage *p);   //载入消息bar
    void loadHandWritePad(HandWritePad *p); //载入手写板

signals:
    void sigChangeFrame(quint32 frameId);//切换显示画面
    void sigOverViewHide();
    void sigOverViewShow();

public slots:
    void slotHideOverViewTrend();
    void slotShowQverViewTrend();
    void slotShowHandWritingWidget();
    void slotShowCharInfor();
    void slotHistoryPointerChanged();

    void slotTrendConfigChanged() {m_changedFlag = 2;} //趋势绘制相关配置变更
    void slotshowWarningDialog();   //标尺图像丢失警告提示处理

private slots:
    void slotDigitalItemInfor(QModelIndex index);

protected:
    void resizeEvent(QResizeEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    bool eventFilter(QObject *object, QEvent *event);
    void paintEvent(QPaintEvent *e);

private:
    void initialize();
    void setUserDefineStyle();
    void setWinLayout();
    void sigAndSlotConnect();

    void dragDigitalWidget(TrendDigitalGroup::Position state); //state == 1 means left, 2 means down, 3 means right, 4 meas up

private:
    quint32 m_grpIndex;            //趋势组索引值
    QBoxLayout *p_boxLayout;    //尺子与趋势图布局器
    QVBoxLayout  *p_vLayout;    //详细与概要趋势窗体布局器
    QGridLayout *p_gLayout;     //总体布局器；

    TrendDetailWidget *p_detailTrend;
    TrendDigitalGroup *p_digitalGroup;
    TrendOverviewWidget *p_overviewWidget;
    HandWritePad *p_handWriting;
    DisPlayMessage *p_messageBar;
    TrendRulerWidget *p_ruler;

    bool m_isResponseModeSwitch;                  //是否响应工作模式切换
    int m_changedFlag;
};

#endif // WINTRENDGROUP_H
