/*********************************************************************
 * Copyright(c) 2015, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：ModelSummaryMem.h
 * 概   要：内存一览界面的数据模型，读取数据库中显示数据表和事件数据表的信息显示在界面上
 *
 * 当前版本：V1.0.0
 * 作   者：邱洪利
 * 完成日期：2015-9-25
 *
 * 修改版本：
 * 修改说明：
 * 修改作者：
 *
 *******************************************************************/
#ifndef MODELSUMMARYMEM_H
#define MODELSUMMARYMEM_H
#include <QAbstractTableModel>
#include <QSqlQuery>

enum SummaryType
{
    Display_Summary_Type = 0,
    Event_Summary_Type
};


class ModelSummaryMem : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit ModelSummaryMem(SummaryType type,QObject *parent = 0);
    ~ModelSummaryMem();
    int rowCount(const QModelIndex &parent = QModelIndex()) const ;
    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role = Qt::DisplayRole);

    void init();
    int getMaxId();     //得到数据库最大id

private:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;  //
    void getDataBaseName(); //得到要操作的数据库名称
    void queryDataBase(QString query);  //根据query条件查找数据库
    void sort(int column, Qt::SortOrder order = Qt::AscendingOrder);        //排序

signals:

public slots:

private:
    SummaryType                 m_type;     //数据显示类型
    QSqlQuery *                 p_query;
    QList<QVariant>             m_list; //保存数据库查找结果
    QString                     m_databaseName; //数据库名称
    QMap<int, QVariant>         m_headMap;
public:
    int                         m_maxId;
};

#endif // MODELSUMMARYMEM_H
