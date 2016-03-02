/*********************************************************************
 * Copyright(c) 2015, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：ModelLog.h
 * 概   要：日志数据模型，读取文本中的日志文件信息，显示在界面上
 *
 * 当前版本：V1.0.0
 * 作   者：邱洪利
 * 完成日期：2015-9-25
 *
 * 修改版本：
 * 修改说明：
 * 修改作者：
 *
 ********************************************************************/
#ifndef MODELLOG_H
#define MODELLOG_H

#include <QAbstractTableModel>
#include <QDateTime>

enum Log_Type
{
    Operation_Log = 0,  //operation日志
    Error_Log,      //错误日志
    Comm_Log,
    FTP_Log,
    WEB_Log,
    EMail_Log,
    Modbus_Log,
    SNTP_Log,
    DHCP_Log
};

class ModelLog : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit ModelLog(Log_Type type, QObject *parent = 0);
    int rowCount(const QModelIndex &parent = QModelIndex()) const ;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role = Qt::DisplayRole);
    void init();

    void logFileIsChanged(QString path);

private:
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;  //
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    void initListData(QString path, int maxCnt, int col);    //把读取的日志内容存放到链表中

    QString getData(QString path, int maxCnt);
    void initData(QString data, int col);        //把日志信息解析出来放入到链表里
    void initOpeData(QString data);
    void initErrorData(QString data);


signals:

private:
    Log_Type            m_type;     //日志类型
    QList<QVariant>     m_list;     //日志内容
    QMap<int, QVariant> m_headMap;

public:
    QDateTime                 m_lastTime;
};

#endif // MODELLOG_H
