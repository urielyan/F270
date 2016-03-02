#ifndef WINTRENDGROUPPANEL_H
#define WINTRENDGROUPPANEL_H

#include "./Common/slidingscreen.h"
#include "wintrendgroup.h"
#include <QGraphicsView>
#include <QPushButton>
#include <QVector>
#include "./GlobalData/cfgtrendgroup.h"
#include "./Common/displaymessage.h"
#include "./Common/handwritepad.h"

extern CfgTrendGroup  *p_gCfgTrendGroup;

class WinTrendGroupPanel : public SlidingScreen
{
    Q_OBJECT
public:
    explicit WinTrendGroupPanel(QGraphicsView *parent = 0);
    ~WinTrendGroupPanel();
    void setCharMsg(int pos, QString msg);       //pos消息的位置（1~100）,msg消息内容
    void switchToHistoryReview(bool isDisp, qint64 recordIndex, qint64 dataEndTime);
    void switchToRealtimeReview();

signals:
    void sigChangeFrame(quint32 frameId);//切换显示画面

public slots:
    void slotGroupConfigChanged();//显示组配置变更
    void slotOverViewHide();             //趋势组总览模块关闭
    void slotOverViewShow();            //趋势组总览模块打开
    void slotTrendConfigChanged(quint32 ID);        //趋势绘制方向变更处理槽函数

private slots:
    void slotGroupIndexChanged(quint32 grpIndex);//当前组索引变更
//    void slotTestStart();
//    void slotTestStop();
//    void slotTestReconfig();

private:
    int m_currentIndex;         //当前显示画面对应组号0-49
//    QPushButton m_pb_testStart, m_pb_testStop, m_pb_testReconfig;
    QVector<WinTrendGroup *> m_frames;
    DisPlayMessage *p_messageBar;
    HandWritePad *p_handWritePad;
};

#endif // WINTRENDGROUPPANEL_H
