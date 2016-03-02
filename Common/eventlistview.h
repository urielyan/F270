/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：EventListview.h
 * 概   要：日历事件接口中事件一览显示的view
 *
 * 当前版本：V1.0.0
 * 作   者：葛 海 浪
 * 完成日期：2016-1-21
 *
 * 修改版本：
 * 修改说明：
 * 修改作者：
 *
 ********************************************************************/
#ifndef EVENTLISTVIEW_H
#define EVENTLISTVIEW_H

#include <QTableView>
#include <QModelIndex>
class EventListView : public QTableView
{
    Q_OBJECT
public:
    explicit EventListView(QWidget *parent = 0);
    void setModelIndexDefault();
signals:
    void sigIndexClicked(QModelIndex index);
    void sigMoseMove();
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void paintEvent(QPaintEvent*event);

private slots:
    void slotMouseClicked(QModelIndex index);
private:
    enum {
        MousePressRole = 0,
        MouseReleasedRole,
        MouseClickedRole,
        BeginTimeRole,
        EndTimeRole,
        BeginTimeStrRole,
        EndTimeStrRole
    };

private:
    QModelIndex m_PressIndex;
    QModelIndex m_clickedIndex;
   bool m_mouseMove;
   int m_PosY;
};

#endif // EVENTLISTVIEW_H
