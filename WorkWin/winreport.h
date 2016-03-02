/*********************************************************************
 * Copyright(c) 2015, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：winreport.h
 * 概   要：显示报表的主界面
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
#ifndef WINREPORT_H
#define WINREPORT_H

#include <QEvent>
#include "winmessagesummary.h"
#include <QGroupBox>
#include <QFrame>
#include "./DataModel/modelreport.h"
#include "winreportsummary.h"

/*
 *  实现第3列...
 *  实现第4,5列在一个item中，上下两行显示数据
 *  实现所有列表格只显示下边横线，竖线竖不显示
*/
class ReportDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit ReportDelegate(QTableView *tableView, QObject * parent = 0);

protected:
    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const;
//    bool editorEvent(QEvent *event, QAbstractItemModel *model,
//                     const QStyleOptionViewItem &option, const QModelIndex &index);
    bool helpEvent(QHelpEvent */*event*/,
                                         QAbstractItemView *view,
                                         const QStyleOptionViewItem &option,
                                         const QModelIndex &index);

private:
    QPen         pen;
    QTableView*  view;
};





/*
 * 显示报表数据的主界面
*/
class WinReport : public QWidget
{
    Q_OBJECT
public:
    explicit WinReport(QWidget *parent = 0, bool isSingle = false);
    ~WinReport();


private:
    /*UI*/
    void initReportUI();
    void setTopFrameValue(ModelReport * currentModel = NULL);

    void signalAndSlotConnect();

    void setViewStyle();
    void setVline();
    QString getReoprtType(int type);
    void setColumnHiden(ModelReport *currentModel = NULL, WinReportSummary *currentSummary = NULL);
    void setCurrentRow(int current, int sumrow);
    void initHeaderData();

    void showHeaaderVLine();

    void setHeaderNull();
    void setHeaderData(ModelReport *currentModel = NULL, WinReportSummary *currentSummary = NULL);
    void initModelHeaderData(ModelReport *currentModel = NULL, WinReportSummary *currentSummary = NULL);
    void resetColumnWidth();



protected:
    void resizeEvent(QResizeEvent *event);
    void paintEvent(QPaintEvent *event);

signals:

public slots:
     void slotDisplayPreviousReport();
     void slotDisplayNextReport();
     void slotDisplayPreviousTenReport();
     void slotDisplayNextTenReport();

     void slotMoveToNext();
     void slotMoveToPre();

private:
    WinReportSummary            m_reportSummary[2];
    bool                        m_isSingle;

    ReportSlidingScreen         m_screen;

    QFrame                      m_topFrame;
    QPushButton                 m_leftBtn_2;
    QPushButton                 m_leftBtn;
    QLabel                      m_typeLabel;
    QLabel                      m_timeLabel;
    QPushButton                 m_rightBtn;
    QPushButton                 m_rightBtn_2;

    ModelReport                 m_reportModel[2];

    QFrame                      m_frameLine[4];
    int                     m_index;
};


#endif // WINREPORT_H
