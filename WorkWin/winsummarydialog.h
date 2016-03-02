/*********************************************************************
 * Copyright(c) 2015, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：winsummarydialog.h
 * 概   要：用于内存一览，报警一览，信息一览，报表一览，modbus客户端一览界面弹出框显示
 *
 * 当前版本：V1.0.0
 * 作   者：邱洪利
 * 完成日期：2015-12-14
 *
 * 修改版本：
 * 修改说明：
 * 修改作者：
 *
 ********************************************************************/
#ifndef WINSUMMARYDIALOG_H
#define WINSUMMARYDIALOG_H

#include <QObject>
#include "windialog.h"
#include <QProgressBar>
#include <QSqlQuery>

enum Summary_Dialog_Type
{
    Summary_Alarm_Type = 0,
    Summary_Mem_Type,
    Summary_Message_Type,
    Summary_ModbusClient_Type,
    Summary_Report_Type,
    Summary_Save_Type
};

class WinSummaryAlarmDialog;
class WinSummaryMessageDialog;
class WinSummaryMemDialog;
class WinSummaryModbusClientDialog;
class WinSummaryReportDialog;
class WinSummarySaveDialog;
class WinSummaryProcessDialog;


/*
 * 内存一览，报警一览，信息一览，modbus客户端一览，报表一览界面弹出框
*/
class WinSummaryDialog : public WinDialog
{
    Q_OBJECT

public:
    WinSummaryDialog(Summary_Dialog_Type type, QModelIndex index, bool flag = true, QWidget *parent = 0);
    ~WinSummaryDialog();
    void setSaveDataList(QList<quint32> saveList);

private:
    void initData(Summary_Dialog_Type type, QModelIndex index, bool flag);
    void switchToTrend(bool isDisp, QString tableName);

    void setBtnAndTitleLabel(QString title);

    void setSaveDialog();


public slots:
    void slotDisplayTrend();
    void slotEventTrend();
    void slotRefresh();

public:
    QPushButton                     m_okBtn;

private:
    QVBoxLayout *                   p_contextLayout;
    QLabel                          m_titleLabel;
    QPushButton                     m_toDispTrend;
    QPushButton                     m_toEventTrend;
    QPushButton                     m_closeBtn;
    quint64                         m_time;

    QSqlQuery *                     p_query;
};


/*
 * 报警一览弹出框详细内容
*/
class WinSummaryAlarmDialog : public QWidget
{
public:
    WinSummaryAlarmDialog(QModelIndex index, QWidget *parent = 0);

private:
    QString getAlarmType(int type); //通过报警状态配置值得到状态值描述
};


/*
 * 信息一览弹出框详细内容
*/
class WinSummaryMessageDialog : public QWidget
{
public:
    WinSummaryMessageDialog(QModelIndex index, QWidget *parent = 0);

private:
    QString getMessageType(int type);   //

private:
};


/*
 * 内存一览弹出框详细内容
*/
class WinSummaryMemDialog : public QWidget
{
public:
    WinSummaryMemDialog(QModelIndex index, QWidget *parent = 0);
};


/*
 * modbus客户端一览弹出框详细内容
*/
class WinSummaryModbusClientDialog : public QWidget
{
public:
    WinSummaryModbusClientDialog(QModelIndex index, bool flag, QWidget *parent = 0);

private:
    QString getType(int type);
};


/*
 * 报表一览弹出框详细内容
*/
class WinSummaryReportDialog : public QWidget
{
public:
    WinSummaryReportDialog(QModelIndex index, QWidget *parent = 0);
};



/*
 * 内存一览保存数据界面弹出框
*/
class WinSummarySaveDialog : public WinDialog
{
    Q_OBJECT
public:
    static WinSummarySaveDialog *instance();
    void showMsg(quint32 range);
    void setBarValue(quint32 value);

private:
    WinSummarySaveDialog(QWidget *parent = 0);
    void initSaveDialog();
    void setBarRange(quint32 range);
    void setButtonStyle();  //SD/USB存在时样式和不存在时样式


signals:
    void sigChooseSD(bool isID);

public slots:
    void slotChooseBtnClicked();

public:
    QPushButton                     m_okBtn;
    QPushButton                     m_cancleBtn;
    QProgressBar                    m_bar;
    bool                            m_isSD;   //用来保存是保存到SD还是USB  true : SD(默认)   false : USB
private:
    QPushButton                     m_SDBtn;
    QPushButton                     m_USBBtn;

};




#endif // WINSUMMARYDIALOG_H
