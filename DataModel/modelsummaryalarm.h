/*********************************************************************
 * Copyright(c) 2015, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：ModelSummaryAlarm.h
 * 概   要：报警一览界面的数据模型，读取数据库中报警表信息显示在界面上
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
#ifndef MODELSUMMARYALARM_H
#define MODELSUMMARYALARM_H

#include <QAbstractTableModel>
#include <QSqlQuery>
#include <QSqlRecord>

class ModelSummaryAlarm : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit ModelSummaryAlarm(bool display, QObject *parent = 0);
    ~ModelSummaryAlarm();
    void init(bool display = true);
    int rowCount(const QModelIndex &parent = QModelIndex()) const ;
    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role = Qt::DisplayRole);
    int getMaxId();     //得到数据库最大id



private:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;  //
    void sort(int column, Qt::SortOrder order = Qt::AscendingOrder);        //排序


signals:

public slots:

public:
    QList<QSqlRecord>           m_list; //保存数据库查找结果
    bool                        m_display;
    QMap<int, QVariant>         m_headMap;

public:
    int                         m_maxId;
};



#endif // MODELSUMMARYALARM_H
