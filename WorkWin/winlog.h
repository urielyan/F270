/*********************************************************************
 * Copyright(c) 2015, 温光自动化技术有限公司
 *
 * All Rights reserved
 *
 * 文件名称：WinLog.h
 * 概   要： 显示各种信息日志一栏的界面
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
#ifndef WINLOG_H
#define WINLOG_H

#include <QWidget>
#include "winmessagesummary.h"
#include <QFileInfo>
#include "./DataModel/modellog.h"



/**
 * @brief The LogType enum
 * 日志的类型
 */
enum LogType
{
    OperationLog = 0,
    ErrorLog,
    CommunicationLog,
    FTPLog,
    WEBLog,
    EmailLog,
    ModbusLog,
    SNTPLog,
    DHCPLog,
};

/**
 * @brief The LogDelegate class
 * @通用代理类，实现view的网格只有下面的线显示
 */
class LogDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit LogDelegate(QTableView *tabelView, LogType type, QObject *parent = 0);

    QImage getImage(int imgType) const;

    void setTimeFormateStr(QString timeFormateStr);

protected:
    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const;
    bool helpEvent(QHelpEvent *event, QAbstractItemView *view, const QStyleOptionViewItem &option, const QModelIndex &index);
    void drawLog(QPainter *painter, const QStyleOptionViewItem &option,
                              const QModelIndex &index) const;
    //重画modbus
    void drawModbus(QPainter *painter, const QStyleOptionViewItem &option,
                    const QModelIndex &index) const;
    //重画email
    void drawEmail(QPainter *painter, const QStyleOptionViewItem &option,
                   const QModelIndex &index) const;
    //重画web
    void drawWEB(QPainter *painter, const QStyleOptionViewItem &option,
                 const QModelIndex &index) const;



private:
    QPen         pen;
    QTableView*  view;
    LogType     logType;
    QImage      m_imgConnect, m_imgClose, m_imgBusy, m_imgAck;
    int         m_timeFormat;
    QString     m_timeFormatStr;
};


/*
** 显示各种日志信息的主界面
*/
class WinLog : public QWidget
{
    Q_OBJECT
public:
    explicit WinLog(QWidget * parent = 0);

public:
    virtual void initHeadVLine() {}
    virtual void setVline(){}

protected slots:
    virtual void slotDateTimeFormateChanged(quint32 ID) = 0;

protected:
    virtual void resizeEvent(QResizeEvent */*event*/){}
    void paintEvent(QPaintEvent */*event*/){}
};

/***************************operation log********************************/
class WinOperationLog : public WinLog
{
    Q_OBJECT
public:
    WinOperationLog(QWidget * parent = 0);
    ~WinOperationLog();

private:
    void initHeadVLine();
    void setCurrentRow(int current, int sumrow);
    void setVline();
    void setHeaderNull();
    void setHeaderData();
    void initModelHeaderData();
    void resetColumnWidth();

protected:
    void resizeEvent(QResizeEvent *event);
    void paintEvent(QPaintEvent *event);

protected slots:
    void slotDateTimeFormateChanged(quint32 ID);

private:
    SummaryUI                m_operationLog;
    QFrame                   m_operationHeadVFrame[3];
    ModelLog *               p_operationModel;
};

/***************************error log********************************/
class WinErrorLog : public WinLog
{
    Q_OBJECT
public:
    WinErrorLog(QWidget * parent = 0);
    ~WinErrorLog();

private:
    void initHeadVLine();
    void setCurrentRow(int current, int sumrow);
    void setVline();

    void setHeaderNull();
    void setHeaderData();
    void initModelHeaderData();
    void resetColumnWidth();

protected:
    void resizeEvent(QResizeEvent *event);
    void paintEvent(QPaintEvent *event);


protected slots:
    void slotDateTimeFormateChanged(quint32 ID);


private:
    SummaryUI                m_errorLog;
    QFrame                   m_erorHeadVFrame[2];
    ModelLog *               p_errorModel;
};

/***************************comm log********************************/
class WinCommLog : public WinLog
{
    Q_OBJECT
public:
    WinCommLog(QWidget * parent = 0);
    ~WinCommLog();

private:
    void initHeadVLine();
    void setCurrentRow(int current, int sumrow);
    void setVline();

    void setHeaderNull();
    void setHeaderData();
    void initModelHeaderData();
    void resetColumnWidth();

protected:
    void resizeEvent(QResizeEvent *event);
    void paintEvent(QPaintEvent *event);

protected slots:
    void slotDateTimeFormateChanged(quint32 ID);

private:
    SummaryUI                m_commLog;
    QFrame                   m_commHeadVFrame[5];
    ModelLog *               p_commModel;
};

/***************************ftp log********************************/
class WinFTPLog : public WinLog
{
    Q_OBJECT
public:
    WinFTPLog(QWidget * parent = 0);
    ~WinFTPLog();

private:
    void initHeadVLine();
    void setCurrentRow(int current, int sumrow);
    void setVline();

    void setHeaderNull();
    void setHeaderData();
    void initModelHeaderData();
    void resetColumnWidth();

protected slots:
    void slotDateTimeFormateChanged(quint32 ID);

protected:
    void resizeEvent(QResizeEvent *event);
    void paintEvent(QPaintEvent *event);

private:
    SummaryUI                m_FTPLog;
    QFrame                   m_FTPHeadVFrame[4];
    ModelLog *               p_ftpModel;
};

/***************************web log********************************/
class WinWebLog : public WinLog
{
    Q_OBJECT
public:
    WinWebLog(QWidget * parent = 0);
    ~WinWebLog();

private:
    void initHeadVLine();
    void setCurrentRow(int current, int sumrow);
    void setVline();

    void setHeaderNull();
    void setHeaderData();
    void initModelHeaderData();
    void resetColumnWidth();

protected:
    void resizeEvent(QResizeEvent *event);
    void paintEvent(QPaintEvent *event);

protected slots:
    void slotDateTimeFormateChanged(quint32 ID);


private:
    SummaryUI                m_WEBLog;
    QFrame                   m_WEBHeadVFrame[3];
    ModelLog *               p_webModel;
};

/***************************email log********************************/
class WinEmailLog : public WinLog
{
    Q_OBJECT
public:
    WinEmailLog(QWidget * parent = 0);
    ~WinEmailLog();

private:
    void initHeadVLine();
    void setCurrentRow(int current, int sumrow);
    void setVline();

    void setHeaderNull();
    void setHeaderData();
    void initModelHeaderData();
    void resetColumnWidth();

protected:
    void resizeEvent(QResizeEvent *event);
    void paintEvent(QPaintEvent *event);

protected slots:
    void slotDateTimeFormateChanged(quint32 ID);

private:
    SummaryUI                m_EmailLog;
    QFrame                   m_EmailHeadVFrame[3];
    ModelLog *               p_EmailModel;
};


/***************************modbus log********************************/
class WinModbusLog : public WinLog
{
    Q_OBJECT
public:
    WinModbusLog(QWidget * parent = 0);
    ~WinModbusLog();
private:
    void initHeadVLine();
    void setCurrentRow(int current, int sumrow);
    void setVline();

    void setHeaderNull();
    void setHeaderData();
    void initModelHeaderData();
    void resetColumnWidth();

protected:
    void resizeEvent(QResizeEvent *event);
    void paintEvent(QPaintEvent *event);

protected slots:
    void slotDateTimeFormateChanged(quint32 ID);


private:
    SummaryUI                m_modbusLog;
    QFrame                   m_modbusHeadVFrame[3];
    ModelLog *               p_modbusModel;
};

/***************************SNTP log********************************/
class WinSNTPLog : public WinLog
{
    Q_OBJECT
public:
    WinSNTPLog(QWidget * parent = 0);
    ~WinSNTPLog();

private:
    void initHeadVLine();
    void setCurrentRow(int current, int sumrow);
    void setVline();

    void setHeaderNull();
    void setHeaderData();
    void initModelHeaderData();
    void resetColumnWidth();

protected:
    void resizeEvent(QResizeEvent *event);
    void paintEvent(QPaintEvent *event);

protected slots:
    void slotDateTimeFormateChanged(quint32 ID);


private:
    SummaryUI                m_SNTPLog;
    QFrame                   m_SNTPHeadVFrame;
    ModelLog *               p_sntpModel;
};



/***************************DHCP log********************************/
class WinDHCPLog : public WinLog
{
    Q_OBJECT
public:
    WinDHCPLog(QWidget * parent = 0);\
    ~WinDHCPLog();

private:
    void initHeadVLine();
    void setCurrentRow(int current, int sumrow);
    void setVline();

    void setHeaderNull();
    void setHeaderData();
    void initModelHeaderData();
    void resetColumnWidth();

protected:
    void resizeEvent(QResizeEvent *event);
    void paintEvent(QPaintEvent *event);

protected slots:
    void slotDateTimeFormateChanged(quint32 ID);

private:
    SummaryUI                m_DHCPLog;
    QFrame                   m_DHCPHeadVFrame[2];
    ModelLog *               p_dhcpModel;
};



#endif // WINLOG_H
