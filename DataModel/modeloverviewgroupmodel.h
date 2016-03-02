#ifndef MODELOVERVIEWGROUPMODEL_H
#define MODELOVERVIEWGROUPMODEL_H

#include <QAbstractTableModel>

class ModelOverviewGroupModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit ModelOverviewGroupModel(QObject *parent = 0);
    int rowCount(const QModelIndex &parent = QModelIndex()) const ;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

private:
    QVariant getChData(const int row, const int col) const;
    QVariant getChTag(const int row, const int col) const;
    QVariant getChValidity(const int row, const int col) const;
};

#endif // MODELOVERVIEWGROUPMODEL_H
