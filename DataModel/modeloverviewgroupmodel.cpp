#include "modeloverviewgroupmodel.h"
#include "GlobalData/cfgchannel.h"
#include "GlobalData/cfgGlobalDef.h"

ModelOverviewGroupModel::ModelOverviewGroupModel(QObject *parent) :
    QAbstractTableModel(parent)
{
}

int ModelOverviewGroupModel::rowCount(const QModelIndex &) const
{
    int count = CfgChannel::instance()->getEffectiveGroup().size();
    int i = 1, n = 0;
    do{
        ++i;
        n = i*i;
    }while(n < count);
    return i;
}

int ModelOverviewGroupModel::columnCount(const QModelIndex &) const
{
    int count = CfgChannel::instance()->getEffectiveGroup().size();
    int i = 1, n = 0;
    do{
        ++i;
        n = i*i;
    }while(n < count);
    return i - 1;
}

QVariant ModelOverviewGroupModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    int col = index.column();
    switch (role)
    {
    case ChannelData://是否发生报警
        return getChData(row,col);
    case ChannelTag://组名称
        return getChTag(row, col);
    case ChannelValidity://有效性
        return getChValidity(row,col);
    default:
        return QVariant();
    }
}

/*
 * 功能：
 *      根据行、列号，获取对应显示组报警状态
 * 参数：
 * 	  1.const int row：行号
 *       2.const int col：列号
 * 返回值：
 * 	  QVariant：显示组报警状态
 */
QVariant ModelOverviewGroupModel::getChData(const int row, const int col) const
{
    int colcount = columnCount();
    int index = row*colcount + col;
    const QList<quint32> &grpList = CfgChannel::instance()->getEffectiveGroup();
    if (index<grpList.size()){
        quint32 grpIndex = grpList.at(index);
        return CfgChannel::instance()->getGroupAlarmStatus(grpIndex);
    }else{
        return QVariant();
    }
}

/*
 * 功能：
 *      根据行、列号，获取对应显示组名称
 * 参数：
 * 	  1.const int row：行号
 *       2.const int col：列号
 * 返回值：
 * 	  QVariant：显示组名称
 */
QVariant ModelOverviewGroupModel::getChTag(const int row, const int col) const
{
    int colcount = columnCount();
    int index = row*colcount + col;
    const QList<quint32> grpList = CfgChannel::instance()->getEffectiveGroup();
    if (index<grpList.size()){
        quint32 grpIndex = grpList.at(index);
        return CfgChannel::instance()->getGroupName(grpIndex);
    }else{
        return QVariant();
    }
}

/*
 * 功能：
 *      根据行、列号，获取对应显示组有效性
 * 参数：
 * 	  1.const int row：行号
 *       2.const int col：列号
 * 返回值：
 * 	  QVariant：显示组有效时返回对应组索引
 */
QVariant ModelOverviewGroupModel::getChValidity(const int row, const int col) const
{
    int colcount = columnCount();
    int index = row*colcount + col;
    const QList<quint32> grpList = CfgChannel::instance()->getEffectiveGroup();
    if (index<grpList.size()){
        return grpList.at(index);
    }else{
        return QVariant();
    }
}

