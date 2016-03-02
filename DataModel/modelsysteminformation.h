#ifndef MODELSYSTEMINFORMATION_H
#define MODELSYSTEMINFORMATION_H

#include <QAbstractTableModel>

/*
 * 单元信息
 *
 */
class ModelSystemInformation : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit ModelSystemInformation(QObject *parent = 0);
    int rowCount(const QModelIndex &parent = QModelIndex()) const ;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    Qt::ItemFlags flags(const QModelIndex & index) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

private:
    QVariant getStatus(int row) const;
    QVariant getUnit(int row) const;
    QVariant getRecognizedModule(int row) const;
    QVariant getConnectedModule(int row) const;
    QVariant getMainStatus(int row) const;
    QVariant getItemTip(int row) const;

};

/*
 * 模块信息
 *
 */
class ModelSystemUnit : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit ModelSystemUnit(int unitIndex,QObject *parent = 0);
    int rowCount(const QModelIndex &parent = QModelIndex()) const ;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    Qt::ItemFlags flags(const QModelIndex & index) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

private:
    QVariant getStatus(int row) const;
    QVariant getSlot(int row) const;
    QVariant getRecognizedModule(int row) const;
    QVariant getConnectedModule(int row) const;

private:
    int m_unitIndex;
};

#endif // MODELSYSTEMINFORMATION_H
