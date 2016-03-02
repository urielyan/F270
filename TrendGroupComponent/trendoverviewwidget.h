/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：trendoverviewwidget.h
 * 概   要：趋势总览窗体，用与总体浏览当前一次完整记录下趋势图形，只工作于
 * 浏览历史数据两种模式下。
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
#ifndef TRENDOVERVIEWWIDGET_H
#define TRENDOVERVIEWWIDGET_H

#include <QDebug>
#include <QWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QPainter>
#include <QEvent>
#include <QMouseEvent>
#include <QPen>

#include "./GlobalData/cfgtrendgroup.h"
#include "./Common/circlebuf.h"

extern CfgTrendGroup *p_gCfgTrendGroup;

class TrendOverviewWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TrendOverviewWidget(quint32 groupIndex = 0,QWidget *parent = 0);
    ~TrendOverviewWidget();

    void setGroupIndex(quint32 groupIndex);
    const QRectF & getCurDataFrameRect() const { return m_frameRectF;}
    void setMoveFlag(bool flag) { m_moveValidFlag = flag;}
    void setPressPoint(QPointF point) { m_prePoint = point;}
    void countPaintParameter();                     //计算绘制需要的参数

signals:
    void sigHide();

public slots:
private slots:
    void slotCurrentFrameMoveToLeft();
    void slotCurrentFrameMoveToRight();
    void slotCurrentFrameMoveBigToLeft();
    void slotCurrentFrameMoveBigToRight();
    void slotMoveToRecordStart();
    void slotMoveToRecordEnd();

protected:
    void paintEvent(QPaintEvent *e);
    void showEvent(QShowEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);

private:
    void initialize();
    void release();
    void setUserDefineStyle();
    void setUserDefineContent();
    void setWinLayout();
    void sigAndSlotConnect();

    void paintOverviewTrend();
    void paintBackground();                             //计算绘制区域，并设置其背景色
    void paintCurrentTrendFrame();

private:
    QRectF m_frameRectF                                 //当前趋势图加亮框
                 ,m_paintBackgroundRectF;            //总览趋势绘制背景框
    qreal m_dataPointGap;                                //趋势点之间的距离
    quint32 m_paintWidth, m_paintHeight;    //总览趋势绘制区域的宽、高
    quint32 m_groupIndex;

    QGridLayout *p_gLayout_main, *p_gLayout_control;
    QLabel *p_label_dateStart, *p_label_dateEnd;
    QPushButton *p_pButton_toLeft, *p_pButton_toRight, *p_pButton_toRecordStart,
                            *p_pButton_toRecordEnd, *p_pButton_close, *p_pButton_toLeftBig,
                            *p_pButton_toRightBig;
    qint64 m_startTimePoint ;                             //本次记录开始时间
    qint64 m_endTimePoint ;                              //本次记录结束时间
    bool    m_moveValidFlag;                               //移动有效标记
    QPointF m_prePoint;

    CircleBuf *p_overViewDataBuf;
};

#endif // TRENDOVERVIEWWIDGET_H
