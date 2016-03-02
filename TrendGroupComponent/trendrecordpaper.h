/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：trendrecordpaper.h
 * 概   要：趋势图形绘制窗体，绘制趋势图形，工作于实时刷新
 * 和浏览历史数据两种模式下。
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
#ifndef TRENDRECORDPAPER_H
#define TRENDRECORDPAPER_H

#include <QWidget>
#include <QPaintEvent>
#include <QPainter>
#include <QVector>
#include <QPointF>
#include <QDebug>
#include <QMap>
#include <QTimer>
#include <QDateTime>

#include "./GlobalData/globalDef.h"
#include "./GlobalData/cfgtrendgroup.h"
#include "./Common/circlebuf.h"
#include "./Threads/threaddatarecord.h"
#include "./DataModel/modeltrenddigitalgroupchannel.h"

extern CfgTrendGroup *p_gCfgTrendGroup;

//#define DIV_H 19
//#define DIV_V 8
#define TOP_SPACE  20
#define BOTTOM_SPACE 20

class TrendRecordPaper : public QWidget
{
    Q_OBJECT
public:
    explicit TrendRecordPaper(quint32 groupIndex = 0,  QWidget *parent = 0);
    ~TrendRecordPaper();

    qreal getPointGap() { return m_pixelGap; }
    void setGroupIndex(quint32 groupIndex);

    void dealCharMsg(QString &msg, quint8 msgGrp);           //处理字符输入信息
    void dealHWMsg(QImage *image, QString &imageDir, quint8 msgGr, QPoint &rightBottom); //处理手写输入信息
    qreal getCurrentTimerGap() {return m_pixelGap;}
signals:

protected:
    void paintEvent(QPaintEvent *e);

private:
    void paintTimeInfor();
    void paintBackGround();
    void paintTrend();
    void paintCharInfor(const TREND_POINT *data, qreal x, qreal rulerStart, qreal rulerEnd);   //绘制 缓存字符信息
    void paintHandInfor();
    void resetChNum(GROUP_INFO &groupInfor);   //重新设置组信息块中，通道号的顺序。

private:
    qreal m_pixelGap;                                     //每个数据点之间的像素间隙
    qreal m_diff;                                              //背景拖动像素偏移
    quint32 m_groupIndex;                           //绘制面板对应的组号
    CircleBuf *p_paintParam;                        //绘制数据包括辅助数据和趋势值
    qint64 m_rightPointTime;                        //屏幕右侧数据点对应时间
    qint32 m_screenDiv;                                //屏幕横向分割数
    qint64 m_screenTimeDiv;                        //屏幕横向时间/每格
    qint64 m_recordStartTime;                     //缓冲数据记录开始时间
    quint32 m_trendPeriodMin, m_trendPeriodMax, m_trendPeriodTarget;
    int m_targetIndex, m_rightIndex;
    qreal m_scale;
};

#endif // TRENDRECORDPAPER_H
