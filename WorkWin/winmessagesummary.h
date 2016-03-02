/*********************************************************************
 * Copyright(c) 2015, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：winmessagesummary.h
 * 概   要：显示信息一览的界面
 *
 * 当前版本：V1.0.0
 * 作   者：邱洪利
 * 完成日期：2015-7-20
 *
 * 修改版本：
 * 修改说明：
 * 修改作者：
 *
 ********************************************************************/
#ifndef WINMESSAGESUMMARY_H
#define WINMESSAGESUMMARY_H

#include <QWidget>
#include <QDialog>
#include <QPushButton>
#include <QGridLayout>
#include <QTableView>
#include <QStandardItemModel>
#include <QEvent>
#include <QTimer>
#include <QItemDelegate>
#include <QPen>
#include <QPainter>
#include <QLabel>
#include "./DataModel/modelsummarymessage.h"

/*
 * 重写的代理类，实现view的第一列的自定义信息颜色变化
 * 第二列的数据时间和写入时间显示在一列中
 * 每一行只有横线(网格)
*/
class MessageSummaryDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit MessageSummaryDelegate(QTableView *tableView, QObject * parent = 0);
    void setTimeFormate(QString timeFormate);

protected:
    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const;
    bool helpEvent(QHelpEvent *event, QAbstractItemView *view, const QStyleOptionViewItem &option, const QModelIndex &index);

private:
    QPen         pen;
    QTableView*  view;
    QString      m_timeFormate;
};




/*
 * 重写tableview,主要是为了在代理那边得到自定义的闪烁状态
*/
class SummaryTableView : public QTableView
{
    Q_OBJECT
public:
    explicit SummaryTableView(QWidget *parent = 0);


protected:
    QStyleOptionViewItem viewOptions() const;
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    void paintEvent(QPaintEvent *event);

public slots:
    void slotBackgroundChanged(quint32 ID);

public:
    bool m_isBlink; //闪烁状态标记
    bool m_isPress;
    QPoint m_point;//鼠标点击坐标
    QRect m_rect;//鼠标点击区域
};




/*
 * 公共的类，该类为报警一览，信息一览，内存一览，日志一览，报表一览，modbus客户端信息等提供了一个MVC的模型
 */
class SummaryUI : public QWidget
{
    Q_OBJECT
public:
    SummaryUI(QWidget * parent = 0);
    ~SummaryUI();

private:
    void initMessageSummaryUI();    //init MVC UI
    void signalAndSlotConnect();    //bind signal and slot
   // QSqlDatabase getSqlDB(QString DBName);  //通过数据库名字得到QSlqDatabase对象

public slots:
    /* 上/下按钮按下和松开的信号的槽函数和与之相关的定时器槽函数 */
    void slotUpBtnPressed();
    void slotUpBtnRelease();
    void slotDownBtnPressed();
    void slotDownBtnRelease();
    void slotTimeOut();

public:
    QPushButton *           p_upBtn;
    QPushButton *           p_downBtn;
    QLabel *                p_headerLabel;
    QGridLayout *           p_gridLayout;

    SummaryTableView *      p_view;
    int                     m_btnPress;   //上 下按钮哪一个被按下  -1:无按下; 0:上按钮被按下;  1:下按钮被按下

    QTimer *                p_timer;
};




/*
 * 显示信息一览的主界面
*/
class WinMessageSummary : public QWidget
{
    Q_OBJECT
public:
    explicit WinMessageSummary(QWidget *parent = 0);
    ~WinMessageSummary();

    void setViewStyle();    //设置视图属性
    void initModelHeaderData(); //初始化模型头信息
    void signalAndSlotConnect();

    void setVline();
    void initFrameHeader();
    void setCurrentRow(int current, int sumrow);
    void setHeadDataString();
    void setHeadDataNull();
    void resetColumnSize();

protected:
    void resizeEvent(QResizeEvent *event);
    void paintEvent(QPaintEvent *event);

signals:

public slots:
    void slotDetailMessage(QModelIndex index);  //显示当前行详细信息的槽函数
    void slotSectionSort(int section);

public:
//    QSqlTableModel *        p_messagemodel;
    SummaryUI               m_messageSummary;
    int sumRow;  //总行数

    QFrame                  m_frameHeader[2];
    ModelSummaryMessage *   p_modelMessage;
//    MessageSummaryDelegate * p_messageDelegate;
};

#endif // WINMESSAGESUMMARY_H
