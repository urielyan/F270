#include <QDebug>
#include "modelrelaymodel.h"
#include "./GlobalData/cfgGlobalDef.h"
#include "./GlobalData/cfgchannel.h"

ModelRelayModel::ModelRelayModel(QObject *parent) :
    QAbstractTableModel(parent)
{
}

int ModelRelayModel::rowCount(const QModelIndex &/*parent*/) const
{
    int count =  CfgChannel::instance()->getExistChannelDO().size();
    int i = 1, n = 0;
    do{
        ++i;
        n = i*i;
    }while(n < count);
    return i;
}

int ModelRelayModel::columnCount(const QModelIndex &/*parent*/ ) const
{
    int count =  CfgChannel::instance()->getExistChannelDO().size();
    int i = 1, n = 0;
    do{
        ++i;
        n = i*i;
    }while(n < count);
    return i - 1;
}

QVariant ModelRelayModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    int col = index.column();
    switch (role) {
    case ChannelData:
        return getChData(row,col);
    case ChannelType:
        return getChType(row, col);
    case ChannelTag:
        return getChTag(row, col);
    case ChannelValidity:
        return getChValidity(row,col);
    case ChannelDataAccuracy:
        return getChDataAccuracy(row,col);
    default:
        return QVariant();
    }
}

/*
 * 功能：
 *      根据行、列号，获取对应通道的测量数据
 * 参数：
 * 	  1.const int row：行号
 *       2.const int col：列号
 * 返回值：
 * 	  QVariant：通道测量数据
 */
QVariant ModelRelayModel::getChData(const int row, const int col) const
{
    int colcount = columnCount();
    int index = row*colcount + col;
    const QMap<QString, quint32> &doMap = CfgChannel::instance()->getExistChannelDO();
    if(index < doMap.size()){
        quint32 chanNum = doMap.values().at(index);
        return CfgChannel::instance()->getChannelData(chanNum);
    }else{
        return QVariant();
    }
}


/*
 * 功能：
 *      根据行、列号，获取对应通道的类型
 * 参数：
 * 	  1.const int row：行号
 *       2.const int col：列号
 * 返回值：
 * 	  QVariant：通道的类型
 */
QVariant ModelRelayModel::getChType(const int row, const int col) const
{
    int colcount = columnCount();
    int index = row*colcount + col;

    const QMap<QString, quint32> &doMap = CfgChannel::instance()->getExistChannelDO();
    if(index < doMap.size()){
        quint32 channelNumber = doMap.values().at(index);
        return CHANNEL_TYPE(channelNumber);           //通道类型
    } else {
        return QVariant();
    }
}

/*
 * 功能：
 *      根据行、列号，获取对应通道的标记
 * 参数：
 * 	  1.const int row：行号
 *       2.const int col：列号
 * 返回值：
 * 	  QVariant：通道的标记
 */
QVariant ModelRelayModel::getChTag(const int row, const int col) const
{
    int colcount = columnCount();
    int index = row*colcount + col;

    const QMap<QString, quint32> &doMap = CfgChannel::instance()->getExistChannelDO();
    if(index < doMap.size()){
        quint32 channelNumber = doMap.values().at(index);
        return CfgChannel::instance()->getChannelTag(channelNumber);           //通道标记
    } else {
        return QVariant();
    }
}

/*
 * 功能：
 *      根据行、列号，获取对应通道的用户标记
 * 参数：
 * 	  1.const int row：行号
 *       2.const int col：列号
 * 返回值：
 * 	  QVariant：通道的用户标记
 */
QVariant ModelRelayModel::getChDataAccuracy(const int row, const int col) const
{
    int colcount = columnCount();
    int index = row*colcount + col;

    const QMap<QString, quint32> &doMap = CfgChannel::instance()->getExistChannelDO();
    if(index < doMap.size()){
        quint32 channelNumber = doMap.values().at(index);
        return CfgChannel::instance()->getChannelDataAccuracy(channelNumber);           //通道标记
    } else {
        return QVariant();
    }
}

/*
 * 功能：
 *      根据行、列号，获取对应通道有效性
 * 参数：
 * 	  1.const int row：行号
 *       2.const int col：列号
 * 返回值：
 * 	  QVariant：通道有效时返回其通道编号
 */
QVariant ModelRelayModel::getChValidity(const int row, const int col) const
{
    int colcount = columnCount();
    int index = row*colcount + col;

    const QMap<QString, quint32> &doMap = CfgChannel::instance()->getExistChannelDO();
    if(index < doMap.size()){
        return doMap.values().at(index);
    } else {
        return QVariant();
    }
}
