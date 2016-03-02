#include <QBrush>
#include "modelsysteminformation.h"

ModelSystemInformation::ModelSystemInformation(QObject *parent) :
    QAbstractTableModel(parent)
{
}

int ModelSystemInformation::rowCount(const QModelIndex &/*parent*/) const
{
    return 9;
}

int ModelSystemInformation::columnCount(const QModelIndex &/*parent*/) const
{
    return 5;
}

QVariant ModelSystemInformation::data(const QModelIndex &index, int role) const
{
    if(role == Qt::DisplayRole){
        switch (index.column()) {
        case 0:
            return getItemTip(index.row());
        case 1:
            return getUnit(index.row());
        case 2:
            return getRecognizedModule(index.row());
        case 3:
            return getConnectedModule(index.row());
        }
    }else if(role == Qt::TextAlignmentRole){
        if(index.row() != 0 && index.row() != 2){
            return Qt::AlignCenter;
        }
    }else if(role == Qt::UserRole){
        if(index.column() == 0){
            return getStatus(index.row());
        }else{
           return getMainStatus(index.row());
        }
    }else if(role == Qt::TextColorRole || role == Qt::ForegroundRole){
        return QBrush(Qt::black);
    }

    return QVariant();
}

Qt::ItemFlags ModelSystemInformation::flags(const QModelIndex & index) const
{
    if(index.row() == 1 && (index.column() == 2 || index.column() == 3)){
        return Qt::ItemIsUserCheckable|Qt::ItemIsEnabled;
    }
    return Qt::NoItemFlags;
}

QVariant ModelSystemInformation::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role == Qt::DisplayRole){
        switch(section){
        case 0:
            return tr("Sts");
        case 1:
            return tr("Unit");
        case 2:
            return tr("Recognized module");
        case 3:
            return tr("Connected module");
        case 4:
            return tr("Mod Sts");
        }
    }else if(role == Qt::BackgroundRole || role == Qt::BackgroundColorRole){
        return QBrush(Qt::yellow);
    }

    return QAbstractItemModel::headerData(section, orientation, role);
}


QVariant ModelSystemInformation::getStatus(int row) const
{
    switch (row) {
    case 1:
        return true;
    case 3:
        return false;
    case 4:
        return false;
    case 5:
        return false;
    case 6:
        return false;
//    case 7:
//        return false;
//    case 8:
//        return false;
    }
    return QVariant();
}

QVariant ModelSystemInformation::getUnit(int row) const
{
    switch (row) {
    case 1:
        return tr("Main");
    case 3:
        return tr("1");
    case 4:
        return tr("2");
    case 5:
        return tr("3");
    case 6:
        return tr("4");
    case 7:
        return tr("5");
    case 8:
        return tr("6");
    }
    return QVariant();
}

QVariant ModelSystemInformation::getMainStatus(int row) const
{
    if(row == 1){
        return true;
    }
    return QVariant();
}

QVariant ModelSystemInformation::getRecognizedModule(int row) const
{
    if(row == 1){
        return tr("GX90EX-02-TP1");
    }else{
        return tr("--- --- --- ---");
    }
}

QVariant ModelSystemInformation::getConnectedModule(int row) const
{
    if(row == 1){
        return tr("GX90EX-02-TP1");
    }else{
        return tr("--- --- --- ---");
    }
}

QVariant ModelSystemInformation::getItemTip(int row) const
{
    if(row == 0){
        return tr("Main Unit: GP20-2E");
    }else if(row == 2){
        return tr("Extension Unit: GX60");
    }else{
        return QVariant();
    }
}

/***************************************************/
ModelSystemUnit::ModelSystemUnit(int unitIndex,QObject *parent):
    QAbstractTableModel(parent),m_unitIndex(unitIndex)
{
}

int ModelSystemUnit::rowCount(const QModelIndex &/*parent*/) const
{
    if(m_unitIndex){
        return 6;
    }else{
        return 10;
    }
}

int ModelSystemUnit::columnCount(const QModelIndex &/*parent*/) const
{
    return 4;
}

QVariant ModelSystemUnit::data(const QModelIndex &index, int role) const
{
    if(role == Qt::DisplayRole){
        switch (index.column()) {
        case 1:
            return getSlot(index.row());
        case 2:
            return getRecognizedModule(index.row());
        case 3:
            return getConnectedModule(index.row());
        }
    }else if(role == Qt::TextAlignmentRole){
            return Qt::AlignCenter;
    }else if(role == Qt::UserRole){
            return getStatus(index.row());
    }else if(role == Qt::TextColorRole || role == Qt::ForegroundRole){
        return QBrush(Qt::black);
    }

    return QVariant();
}

Qt::ItemFlags ModelSystemUnit::flags(const QModelIndex & index) const
{
    if(index.row() == 1 && (index.column() == 2 || index.column() == 3)){
        return Qt::ItemIsUserCheckable|Qt::ItemIsEnabled;
    }
    return Qt::NoItemFlags;
}

QVariant ModelSystemUnit::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role == Qt::DisplayRole){
        switch(section){
        case 0:
            return tr("Sts");
        case 1:
            return tr("Slot");
        case 2:
            return tr("Recognized module");
        case 3:
            return tr("Connected module");
        }
    }else if(role == Qt::BackgroundRole || role == Qt::BackgroundColorRole){
        return QBrush(Qt::yellow);
    }

    return QAbstractItemModel::headerData(section, orientation, role);
}

QVariant ModelSystemUnit::getStatus(int row) const
{
    switch (row) {
    case 0:
        return true;
    case 1:
        return false;
    case 2:
        return false;
    case 3:
        return false;
    case 4:
        return false;
//    case 7:
//        return false;
//    case 8:
//        return false;
    }
    return QVariant();
}

QVariant ModelSystemUnit::getSlot(int row) const
{
    switch (row) {
    case 0:
        return tr("0");
    case 1:
        return tr("1");
    case 2:
        return tr("2");
    case 3:
        return tr("3");
    case 4:
        return tr("4");
    case 5:
        return tr("5");
    case 6:
        return tr("6");
    case 7:
        return tr("7");
    case 8:
        return tr("8");
    case 9:
        return tr("9");
    }
    return QVariant();
}

QVariant ModelSystemUnit::getRecognizedModule(int row) const
{
    if(row == 1){
        return tr("GX90EX-02-TP1");
    }else{
        return tr("--- --- --- ---");
    }
}

QVariant ModelSystemUnit::getConnectedModule(int row) const
{
    if(row == 1){
        return tr("GX90EX-02-TP1");
    }else{
        return tr("--- --- --- ---");
    }
}
