#ifndef MODELREPORT_H
#define MODELREPORT_H

#include <QAbstractTableModel>
#include <QSqlQuery>
#include "./GlobalData/sqlBlock_reportInfo.h"
#include "./GlobalData/sqlBlock_fileInfo.h"

#define Channel_Column              0
#define Unit_Column                 1
#define Status_Column               2
#define Ave_Column                  3
#define Max_Column                  4
#define Min_Column                  5
#define Inst_Column                 6
#define Sum_Column                  7

//报表一览的数据模型
class ModelReport : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit ModelReport(QObject *parent = 0);
    ~ModelReport();
    void init(int num);
    int getMaxId();
    int rowCount(const QModelIndex &parent = QModelIndex()) const ;
    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role = Qt::DisplayRole);

private:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;  //

    void queryDataBase(QString query, QString querySub);

    void addModelData(REPORT_CONFIG reportinfo, QByteArray data, QString tag);
    void addData(qreal *pData, Report_Data_Type type[]);
    QString getReportChannelNum(Channel_Type type, quint32 num);
    QString getReportStatus(int type);

signals:
    void sigFirstReport();

public slots:

private:
    QSqlQuery *                 p_query;
    QSqlQuery *                 p_querySub;
    QList<QVariant>             m_list; //保存数据库查找结果

    QMap<int, QVariant>         m_headMap;      //header

public:
    quint32                     m_maxId;
    quint32                     currentIndex;
};

#endif // MODELREPORT_H
