#include "modelsoftswitchmodel.h"
#include "./GlobalData/cfgGlobalDef.h"
#include "./GlobalData/cfgchannel.h"

ModelSoftSwitchModel::ModelSoftSwitchModel(QObject *parent) :
    QAbstractTableModel(parent)
{
}

int ModelSoftSwitchModel::rowCount(const QModelIndex &/*parent*/) const
{
    return 10;
}

int ModelSoftSwitchModel::columnCount(const QModelIndex &/*parent*/ ) const
{
    return 10;
}

QVariant ModelSoftSwitchModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    int col = index.column();
    switch (role) {
    case ChannelData:
        return getChData(row,col);
    case ChannelTag:
        return getChTag(row, col);
    case ChannelValidity:
        return getChValidity(row,col);
    default:
        break;
    }
    return QVariant();
}

/*
 * 功能：
 *      根据行、列号，获取对应内部开关状态
 * 参数：
 * 	  1.const int row：行号
 *       2.const int col：列号
 * 返回值：
 * 	  QVariant：内部开关状态
 */
QVariant ModelSoftSwitchModel::getChData(const int row, const int col) const
{
    int colcount = columnCount();
    int index = row*colcount + col;
    return CfgChannel::instance()->getDeviceSwitchStatus(index);
}

/*
 * 功能：
 *      根据行、列号，获取对应内部开关的标记
 * 参数：
 * 	  1.const int row：行号
 *       2.const int col：列号
 * 返回值：
 * 	  QVariant：内部开关的标记
 */
QVariant ModelSoftSwitchModel::getChTag(const int row, const int col) const
{
    int colcount = columnCount();
    int index = row*colcount + col;
    return CfgChannel::instance()->getDeviceSwitchName(index);
}

/*
 * 功能：
 *      根据行、列号，获取对应内部开关有效性
 * 参数：
 * 	  1.const int row：行号
 *       2.const int col：列号
 * 返回值：
 * 	  QVariant：内部开关有效时返回对应索引
 */
QVariant ModelSoftSwitchModel::getChValidity(const int row, const int col) const
{
    int colcount = columnCount();
    int index = row*colcount + col;
    if(index<MAIN_UNIT_INTERNAL_SWITCH){
        return index;
    }else{
        return QVariant();
    }
}
