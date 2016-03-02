/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：trenddetailwidget.h
 * 概   要：趋势详细浏览窗体，工作于实时刷新和历史浏览两种模式下。
 *
 * 当前版本：V1.0.0
 * 作   者：王 宸 博
 * 完成日期：2015-6-15
 *
 * 修改版本：
 * 修改说明：
 * 修改作者：
 *
 ********************************************************************/
#ifndef TRENDDETAILWIDGET_H
#define TRENDDETAILWIDGET_H

#include <QDebug>
#include <QPainter>
#include <QPushButton>
#include <QResizeEvent>
#include <QMouseEvent>
#include <QTimer>
#include <QPoint>
#include <QHBoxLayout>
#include <QPainter>
#include <QLabel>
#include <QWidget>

#include "trendrecordpaper.h"
#include "../Common/custombutton.h"

#define PB_Show_Time 4000
extern CfgTrendGroup *p_gCfgTrendGroup;

class TrendDetailWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TrendDetailWidget(int groupIndex = 0, QWidget *parent = 0);
    ~TrendDetailWidget();

    TrendRecordPaper * getRecordPaper() { return p_recordPaper2;}

    void setGroupIndex(quint32 groupIndex);
    void reCountDetailArg();    //重新计数相关参数
signals:
    void sigInforHand();                    //手写信息按钮被按下时，发射该信号
    void sigInforChar();                     //信息输入按钮按下时，发射该信号
    void sigOverviewTrend();
public slots:
    void slotDealCharMsg(QString msg);             //处理字符信息输入槽函数
    void slotDealHWMsg(QImage* image,QString& dir,quint8 msgGr, QPoint & rightBottom);  //处理手写信息输入槽函数
    void slotPBTimerOut();                                      //界面浮动按钮显示time out槽

private slots:
    void slotPBOverviewTrendIsClicked();
    void slotPBOverviewTrendIsPressed();
    void slotPBOverviewTrendIsReleased();

    void slotPBInforHandIsClicked();
    void slotPBInforHandIsPressed();
    void slotPBInforHandIsReleased();

    void slotPBInforCharIsClicked();
    void slotPBInforCharIsPressed();
    void slotPBInforCharIsReleased();

    void slotPBMoveRightIsClicked();
    void slotPBMoveLeftStepIsClicked(quint32 points);
    void slotPBMoveRightStepIsClicked(quint32 points);

protected:
    void resizeEvent(QResizeEvent *e);
    void mousePressEvent(QMouseEvent *event);

private:
    void initialize();
    void release();
    void setUserDefineStyle();
    void setUserDefineContent();
    void setWinLayout();
    void sigAndSlotConnect();

    void showControlPB();                             //显示浮动控制按钮函数


private:
    QTimer *p_qt_pbTimer;
    QPushButton *p_pb_inforHand, *p_pb_inforChar,*p_pb_overviewTrend,
                            *p_pb_moveRight, *p_pb_moveUp;

    CustomButton *p_pb_moveLeftStep, *p_pb_moveRightStep, *p_pb_moveUpStep, *p_pb_moveDownStep;
    QHBoxLayout m_hlayout_ruler;

    TrendRecordPaper *p_recordPaper2;      //趋势记录纸张

    quint32 m_groupIndex;
    qreal m_gap;

    TrendWorkMode m_workMode;        // 工作模式
};
#endif // TRENDDETAILWIDGET_H
