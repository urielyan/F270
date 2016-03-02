/*********************************************************************
 * Copyright(c) 2015, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：winmemsummary.h
 * 概   要：内存一览数据的显示(主要采用mvc架构) : 内存一览数据的显示，
 *                  该文件包括内存一览主界面类WinMemSummary      (model - view)
 *                  主界面代理类MemSummaryDelegate            (delegate)
 *                  弹出框显示详细信息类MemSummaryDetailUI
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
#ifndef WINMEMSUMMARY_H
#define WINMEMSUMMARY_H

#include <QWidget>
#include <QLabel>
#include <QRadioButton>
#include <QToolButton>
#include <QTimer>
#include <QMouseEvent>
#include "winmessagesummary.h"
#include "./DataModel/modelsummarymem.h"

/*
 * 重写的代理类，
 *      实现view的第一列的checkbox
 *      第二列的开始时间和结束时间显示在一列中
*/
class MemSummaryDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit MemSummaryDelegate(QTableView *tabelView, QObject * parent = 0);
    void setTimeFormate(QString timeFormate);

protected:
    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const;
    bool editorEvent(QEvent *event, QAbstractItemModel *model,\
                     const QStyleOptionViewItem &option, const QModelIndex &index);
    bool helpEvent(QHelpEvent *event, QAbstractItemView *view,
               const QStyleOptionViewItem &option,const QModelIndex &index);

private:
    QPen         pen;
    QTableView*  view;
    QString      m_timeFormate;
};



/**
 * @brief The WinMemSummary class
 *      内存一览的主界面
 */
class WinMemSummary : public QWidget
{
    Q_OBJECT
public:
    explicit WinMemSummary(QWidget *parent = 0);
    ~WinMemSummary();
private:
    void initMemSummaryUI();
    void signalAndSlotConnect();
    /*Model*/
    void initModel();
    /*View*/
    void showView();
    void showShortcutKey();
    void resizeEvent(QResizeEvent * event);
    void setSampleLabelData();  //设置p_sampleLable数据
    void setVline(QTableView * view);    //表头旁边的分割线
    void setviewColumnStyle();    //隐藏列
    void mapChanged(bool flag);    //map中选中标记的增加和删除
    void initCheckMap();
    void setCurrentRow(int current, int sumrow);
    void initModelHeaderData();
    void setHeaderData(ModelSummaryMem &model);
    void setHeaderNull(ModelSummaryMem &model);

    quint32 getManualMaxID();
    void getManualDataInfo();
    quint32 getReportMaxID();
    void getReportDataInfo();

    QString getSysDateFormate();

protected:
    void paintEvent(QPaintEvent *event);

signals:

public slots:
    void slotDetailMessage(QModelIndex index);  //点击某行显示详细信息
    void slotRadioBtnChange();  //显示数据和事件数据的切换
    void slotSaveBtnClicked();  //保存选中数据
    void slotCancleBtnClicked();    //取消选中数据
    void slotSectionSort(int section);  //点击某列，对应的排序
    void slotSaveData();

    void slotSave();


public:
    QFrame             m_topFrame;
    QLabel             m_sampleLabel;
    QToolButton        m_toolBtn;
    QRadioButton       m_displayRadioBtn;
    QRadioButton       m_eventRadionBtn;
    SummaryUI          m_memView;
    QFrame             m_btnBox;
    QPushButton        m_saveBtn;
    QPushButton        m_cancleBtn;

    ModelSummaryMem *           p_modelEvent;
    ModelSummaryMem *           p_modelDisp;

    QFrame                m_frameLine[3];

    QTimer                 m_saveTime;  //保存数据时定时器

    int                 m_displaySumRow;
    int                 m_eventSumRow;

    QList<quint32>      m_saveList; //选中行的ID链表
    int                 m_selectCnt;    //选中行数
    bool                m_isSave;   //是否正在保存

    quint32             m_manualCnt;
    quint32             m_reportCnt;
    QString             m_manualTime;
    QString             m_reportTime;
    QString             m_timeFormateStr;
};

#endif // WINMEMSUMMARY_H
