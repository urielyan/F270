/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：trenddigitalgroup.h
 * 概   要：趋势数值浏览窗体，显示趋势模块中标尺所标注的时间点的数值信息，工作于实时刷新
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
#ifndef TRENDDIGITALGROUP_H
#define TRENDDIGITALGROUP_H

#include <QTableView>
#include <QPalette>
#include <QMouseEvent>
#include <QPainter>
#include <QHeaderView>
#include <QDebug>
#include <QEvent>

#include "./DataModel/modeltrenddigitalgroupchannel.h"
#include "trenddigitaldelegate.h"

extern CfgTrendGroup *p_gCfgTrendGroup;

class TrendDigitalGroup : public QTableView
{
    Q_OBJECT
public:
    typedef enum {
        UP,
        DOWN,
        LEFT,
        RIGHT
    }Position;
    explicit TrendDigitalGroup(quint32 grpIndex, QWidget *parent = 0);
    void changePos(Position pos);
    bool isPress();
    Position getPosition();
    void resetGroupIndex(quint32 grpIndex);
    void changeDirection(bool dir);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
//    void resizeEvent(QResizeEvent *event);
//    void showEvent(QShowEvent *e);
    QStyleOptionViewItem viewOptions() const;

signals:
    void sigShowDigitalItemInfor(QModelIndex index);

private slots:
    void slotChannelUnselect();//移除选中状态

private:
    Position m_pos;
    bool m_pressed,m_trigger;
    QPoint m_point;//鼠标点击坐标
    QRect m_rect;
    quint32 m_grpIndex;
    QModelIndex m_prevIndex;

    TrendDigitalDelegate *p_delegate;
    ModelTrendDigitalGroupChannel *p_model;

};

#endif // TRENDDIGITALGROUP_H
