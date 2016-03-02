/*********************************************************************
 * Copyright(c) 2015, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：winmoubusclient.h
 * 概   要：modbus客户端信息显示界面
 *
 * 当前版本：V1.0.0
 * 作   者：邱洪利
 * 完成日期：2015-8-3
 *
 * 修改版本：
 * 修改说明：
 * 修改作者：
 *
 ********************************************************************/
#ifndef WINMODBUSCLIENT_H
#define WINMODBUSCLIENT_H

#include <QWidget>
#include <QGridLayout>
#include <QFrame>
#include <QPushButton>
#include <QLabel>
#include "winmessagesummary.h"
#include <QAbstractTableModel>
#include "./DataModel/modelmodbusclient.h"


/*
** Modbus客户端界面代理类
*/
class ModbusClientDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit ModbusClientDelegate(QTableView *tableView, QObject * parent = 0);

protected:
    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const;
    bool helpEvent(QHelpEvent *event, QAbstractItemView *view,
               const QStyleOptionViewItem &option,const QModelIndex &index);

private:
    QPen         pen;
    QTableView*  view;
    QImage      m_imgConnect, m_imgClose, m_imgBusy, m_imgAck;
};

class ModbusGridlayoutDelegate : public QItemDelegate
{
public:
    ModbusGridlayoutDelegate(SummaryTableView *tableview, QWidget * parent = 0);
    virtual void	paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const;

    virtual QSize	sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index) const;

    void doLayout(const QStyleOptionViewItem &option, QRectF &modbusRect, qreal &modbusSpan) const;
    void drawModbusRect(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index, QRectF &modbusRect) const;
    bool helpEvent(QHelpEvent *event,QAbstractItemView *view,\
                   const QStyleOptionViewItem &option, const QModelIndex &index);
    QStyleOptionViewItem setOption(const QStyleOptionViewItem & option,const QModelIndex & index) const;
    void drawPressingRect(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
private:
    QPen         pen;
    SummaryTableView*  view;
    QMargins m_margins;//边缘间距
};


/*
 * modbus 客户端界面类
*/
class WinModbusClient : public QWidget
{
    Q_OBJECT
public:
    explicit WinModbusClient(QWidget *parent = 0);
    ~WinModbusClient();

private:
    void initModbusUI();

    void signalAndSlotConnect();
    void initHeaderVline();
    void setVline();
    void getModbusClientShowStatus();   //该界面是用tableView显示还是其他显示形式

    void setHeaderNull();
    void setHeaderData();
    void initModelHeaderData();
    void resetColumnWidth();

    void setViewStyle();

protected:
    void resizeEvent(QResizeEvent *event);
    void paintEvent(QPaintEvent *event);

signals:

public slots:
    void slotDetailMessage(QModelIndex index);
    void slotShowModbusClient();
    void slotModbusSettingChanged();    //当modbus客户端的周期，自动恢复时间，连接时间改变时绑定的槽函数

    void settopLabelVlaue(quint32 ID = 0);    //设置显示客户端的周期等信息
    void slotTimeout();

public:
    QFrame                 m_frame;
    QPushButton            m_upBtn;
    QPushButton            m_downBtn;
    QLabel                 m_topLabel;
    SummaryUI              m_modbusView;
    bool                   m_timerStatus;  //

    ModelModbusClient *     p_normalModel;
    ModelModbusClient *     p_gridModel;
    ModbusClientDelegate *  p_normalDelegate;
    ModbusGridlayoutDelegate * p_gridDelegate;

    QFrame              m_vLine[4];
    bool                m_status;   //记录modbusClient的界面显示形式的状态 true :一行一行的显示           false:网格显示
};

#endif // WINMODBUSCLIENT_H
