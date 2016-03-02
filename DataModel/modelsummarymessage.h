#ifndef MODELSUMMARYMESSAGE_H
#define MODELSUMMARYMESSAGE_H

#include <QAbstractTableModel>
#include <QObject>
#include <QSqlQuery>




class ModelSummaryMessage : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit ModelSummaryMessage(QObject *parent = 0);
    ~ModelSummaryMessage();
    void init();
    int rowCount(const QModelIndex &parent = QModelIndex()) const ;
    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role = Qt::DisplayRole);
    int getMaxId();     //得到数据库最大id

private:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;  //
    void sort(int column, Qt::SortOrder order = Qt::AscendingOrder);        //排序

    void queryDataBase(QString query);  //根据query条件查找数据库


signals:

public slots:

public:
    QSqlQuery *                 p_query;
    QList<QVariant>             m_list; //保存数据库查找结果
    bool                        m_display;
    QMap<int, QVariant>         m_headMap;
public:
    int                         m_maxId;
};

#endif // MODELSUMMARYMESSAGE_H
