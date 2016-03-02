/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：trendrulerwidget.h
 * 概   要：趋势标尺窗体，标注当前时间点下谐波点值。
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
#ifndef TRENDRULERWIDGET_H
#define TRENDRULERWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QDebug>
#include <QMouseEvent>
#include <QDateTime>
#include <QLabel>
#include "./GlobalData/cfgtrendgroup.h"

#include "./GlobalData/cfgGlobalDef.h"
#include "./GlobalData/cfgchannel.h"

extern CfgTrendGroup *p_gCfgTrendGroup;

class TrendRulerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TrendRulerWidget(quint32 groupIndex = 0, QWidget *parent = 0);
    ~TrendRulerWidget();

    int setRulerMoveRange(int startPoint, int width);   //设置标尺在父窗体中的移动范围
    void setBackgroundAlpha(int alpha) { m_backAlpha = alpha; }
    quint8 produceRulerMap();
    void reCountTruleArg();    //重新计数标尺相关参数

    void setGroupIndex(quint32 groupIndex) { m_groupIndex = groupIndex;}

signals:
    void sigWarningScaleImageLose();

protected:
    void paintEvent(QPaintEvent *e);
    void showEvent(QShowEvent *e);
    void mouseMoveEvent(QMouseEvent *e);

private:
    void paintScale(int pos, quint32 chanNo);
    void paintRulerBackGroup();
    void paintMark(int rulerPos, quint32 chanNo);
    void paintBar(int rulerPos, quint32 chanNo);

private:
    int m_largeScaleLen, m_smallScaleLen, m_rulerWidth, m_rulerNum;
    int m_backAlpha;
    quint32 m_groupIndex;
    int m_startPoint, m_paintWidth;  //标准尺移动范围,父窗体中的开始点和水平移动范围
    int m_largeScaleNum, m_smallScaleNum; //相对于尺子长度的大刻度数量， 相对于大刻度长度的小刻度数量

    QVector<QVector<quint32> > *p_rulerMap;
    CircleBuf *p_paintParam;                          //绘制参数
    bool m_scaleImageLoadFlag;        //标尺图像加载标记
    QString m_filePath;
};

#endif // TRENDRULERWIDGET_H
