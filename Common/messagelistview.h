/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：MessageListView.h
 * 概   要：信息显示接口的视图
 *
 * 当前版本：V1.0.0
 * 作   者：葛 海 浪
 * 完成日期：2016-2-22
 *
 * 修改版本：
 * 修改说明：
 * 修改作者：
 *
 ********************************************************************/
#ifndef MESSAGELISTVIEW_H
#define MESSAGELISTVIEW_H

#include <QTableView>

class MessageListView : public QTableView
{
    Q_OBJECT
public:
    explicit MessageListView(QWidget *parent = 0);
    /** 设置默认的索引  **/
    void setDefaultIndex(QModelIndex index);

signals:
    /** 信息确认选择时发送该信号  **/
    void sigMessageClicked(QModelIndex index);
protected:
    void paintEvent(QPaintEvent*event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent* event);

private:
    enum {
        SequnceRole = 0,
        MessageRole,
        MousePressRole,
        MouseClickedRole
    };

private:
    QModelIndex m_pressIndex;
    QModelIndex m_releasedIndex;

};

#endif // MESSAGELISTVIEW_H
