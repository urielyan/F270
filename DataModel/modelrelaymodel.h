#ifndef MODELRELAYMODEL_H
#define MODELRELAYMODEL_H

#include <QAbstractTableModel>

class ModelRelayModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit ModelRelayModel(QObject *parent = 0);
    int rowCount(const QModelIndex &parent = QModelIndex()) const ;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

private:
    QVariant getChData(const int row, const int col) const;
    QVariant getChType(const int row, const int col) const;
    QVariant getChTag(const int row, const int col) const;
    QVariant getChDataAccuracy(const int row, const int col) const;
    QVariant getChValidity(const int row, const int col) const;
};

#endif // MODELRELAYMODEL_H
