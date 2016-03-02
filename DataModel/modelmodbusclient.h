/*********************************************************************
 * Copyright(c) 2015, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：ModelModbusClient.h
 * 概   要：modbus客户端数据模型，读取文本中文件的信息，显示在界面上.
 *              该文件有两种数据模型:
 *                  1. 分行显示modbus客户端的数据
 *                  2. 网格形式显示数据
 *
 * 当前版本：V1.0.0
 * 作   者：邱洪利
 * 完成日期：2015-9-28
 *
 * 修改版本：
 * 修改说明：
 * 修改作者：
 ********************************************************************/
#ifndef MODELMODBUSCLIENT_H
#define MODELMODBUSCLIENT_H

#include <QAbstractTableModel>
#include "./Threads/threaddatacomm.h"


/*
 *
*/
enum ModbusClient_type
{
    Normal_Type = 0,
    Grid_Type
};

class ModelModbusClient : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit ModelModbusClient(ModbusClient_type type, QObject *parent = 0);
    int rowCount(const QModelIndex &parent = QModelIndex()) const ;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    void init();

    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role = Qt::DisplayRole);

private:
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;  //
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    QString getChannnel(quint32 channelNo);

    int gridRowCount() const ;
    int gridColumnCount() const ;

signals:

public slots:
    void slotUpdateData(int index);

public:
    ModbusClient_type                   m_type;     //modbus客户端显示形式
    QList<QVariant>                     m_list;     //modbus客户端数据内容
    QList<QMap<int, QString> >          m_data;
    QMap<int, QVariant>                 m_headMap;
    QList<Modbus_Summary>               modbusList;
};

#endif // MODELMODBUSCLIENT_H
