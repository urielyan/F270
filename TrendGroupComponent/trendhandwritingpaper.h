/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：trendhandwritingpaper.h
 * 概   要：趋势手写信息绘制窗体
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
#ifndef TRENDHANDWRITINGPAPER_H
#define TRENDHANDWRITINGPAPER_H

#include <QGraphicsItem>
#include<QPainter>
#include <QImage>
#include <QSize>
#include <QDebug>

class TrendHandWritingPaper : public QGraphicsItem
{
//    Q_OBJECT
public:
    explicit TrendHandWritingPaper(QGraphicsItem *parent = 0);
    ~TrendHandWritingPaper();

    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void setSize(QSize size);

signals:

public slots:

private:
    QSize m_size;

};

#endif // TRENDHANDWRITINGPAPER_H
