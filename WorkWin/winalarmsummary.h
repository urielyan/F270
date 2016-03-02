/*********************************************************************
 * Copyright(c) 2015, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：WinAlarmSummary.h
 * 概   要： 显示报警一览信息的界面
 *
 * 当前版本：V1.0.0
 * 作   者：邱 洪 利
 * 完成日期：2015-7-31
 *
 * 修改版本：
 * 修改说明：
 * 修改作者：
 *
 ********************************************************************/
#ifndef WINALARMSUMMARY_H
#define WINALARMSUMMARY_H

#include <QWidget>
#include <QItemDelegate>
#include "winmessagesummary.h"
#include "./DataModel/modelsummaryalarm.h"


/*
 * 报警一栏的代理类，实现第一列显示报警状态的图片
*/
class AlarmSummaryDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit AlarmSummaryDelegate(QTableView *tabelView, QObject * parent = 0);
    void setTimeFormate(QString timeFormate);

protected:
    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const;
//    bool editorEvent(QEvent *event, QAbstractItemModel *model,
//                     const QStyleOptionViewItem &option, const QModelIndex &index);
    bool helpEvent(QHelpEvent *event, QAbstractItemView *view, const QStyleOptionViewItem &option, const QModelIndex &index);

private:
    QImage getAlarmImage(int type, bool isBlink) const;

private:
    QPen         pen;
    QTableView*  view;
    QImage       m_imageOn, m_imageOff, m_imageAck;
    QString      m_timeFormate;
};


/*
 * 显示报警一栏信息的界面
*/
class WinAlarmSummary : public QWidget
{
    Q_OBJECT
public:
    explicit WinAlarmSummary(QWidget *parent = 0);
    ~WinAlarmSummary();

private:
    void initBtnShowAndHidden();
    void signalAndSlotConnect();    //
    void initFrameHeader();     //表头上竖线
    void setViewStyle();        //设置view风格
    void setVline();       //设置竖线
    void resizeEvent(QResizeEvent * event); //重新调整窗体大小
    void setviewColumnStyle();
    void getTimeFormate();  //得到显示时间的格式
    void getDisplayType();  //得到显示界面的显示模式：仅显示正在报警的信息/全部显示
    void setCurrentRow(int current, int sumrow);

    void initHeaderData();
    void setHeadDataNull();
    void setHeadDataString();
    void resetColumnSize();
    void paintEvent(QPaintEvent *event);

signals:

public slots:
    void slotDetailMessage(QModelIndex index);      //点击某行时显示改行详细信息槽函数
    void slotSectionSort(int section);
    void slotTimeFormateChangeed(); //显示时间的格式
    void slotDisplayType(); //显示界面的显示模式 false 仅显示正在报警的信息   true全部显示

    void slotUpdateData();

private:
    SummaryUI               m_alarmSummary;
    bool                    m_display;//显示模式的配置： false 仅显示正在报警的信息   true全部显示
    QFrame                  m_frameHeader[3];
    int                     alarmCurrentRow;  //当前行
    int                     alarmSumRow;  //总行数
    QString                 m_dateTimeFormate;

    ModelSummaryAlarm *     p_alarmModel;
};

#endif // WINALARMSUMMARY_H
