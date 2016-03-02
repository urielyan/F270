/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：channelview.h
 * 概   要：自定义视图，用于通道选择界面
 *
 * 当前版本：V1.0.0
 * 作   者：葛 海 浪
 * 完成日期：2016-1-5
 *
 * 修改版本：
 * 修改说明：
 * 修改作者：
 *
 ********************************************************************/
#ifndef CHANNELVIEW_H
#define CHANNELVIEW_H

#include <QTableView>

class ChannelView : public QTableView
{
    Q_OBJECT
public:
    ChannelView(QWidget *parent = 0);
    void setSequnceStyle(bool sequnce);     /** 设置是否为有序选择状态 **/
    void setViewClicked(QModelIndex& index);    /** 画面进入时调用该函数，初始化相关参数 **/
    void setSelectMaxNums(quint32 max);         /** 设置可以选择的通道数量最大值 **/
    QModelIndexList getSelectChannel();         /** 获取当前已经选择的通道索引的集合 **/
    void initial();

signals:
    void sigViewPressed(quint32 nums);
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

private slots:
    void slotMouseClicked(QModelIndex index);       /** 鼠标单击事件 **/

private:
    enum {
        ChannelNameRole = 0,
        ChannelPressRole,
        ChannelClickedRole,
        ChannelSequnceRole
    };
    QPoint m_point;//鼠标点击坐标
    QModelIndex m_currentIndex;
    QModelIndex m_indexDecide;
    quint32 m_selectCount;      // 已经选择的通道总数
    quint32 m_selectSpan;       // 可以选择的通道总数
    bool m_sequnce;         // 用于判断是否需要现实顺序
    bool m_press;               // 判断通道下押状态
    bool m_clicked;             // 判断通道单击状态
    bool m_move;                // 判断通道移动状态
    QModelIndexList indexList;
};

#endif // CHANNELVIEW_H
