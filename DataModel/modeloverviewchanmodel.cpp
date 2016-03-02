#include "modeloverviewchanmodel.h"
#include "./GlobalData/cfgGlobalDef.h"
#include "./GlobalData/cfgchannel.h"

#include <QDebug>
#include <QColor>
#include <QDateTime>

ModelOverviewChanModel::ModelOverviewChanModel(int frameIndex, QObject *parent) :
    QAbstractTableModel(parent),m_frameIndex(frameIndex)
{
    init();
}

int ModelOverviewChanModel::rowCount(const QModelIndex &/*parent*/) const
{
    int count = m_list_channel.size();
    int i = 1, n = 0;
    do{
        ++i;
        n = i*i;
    }while(n < count);
    return i;
    return 0;
}
int ModelOverviewChanModel::columnCount(const QModelIndex &/*parent */) const
{
    int count = m_list_channel.size();
    int i = 1, n = 0;
    do{
        ++i;
        n = i*i;
    }while(n < count);
    return i - 1;
}
QVariant ModelOverviewChanModel::data(const QModelIndex &index, int role) const
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
    case ChannelUnit:
        return getChUnit(row, col);
    case ChannelStatus:
        return getChStatus(row,col);
    case ChannelValidity:
        return getChValidity(row,col);
    case ChannelDataAccuracy:
        return getChDataAccuracy(row,col);
    case ChannelAlarm1OnOff:
        return getChAlarm1OnOff(row,col);
    case ChannelAlarm1Type:
        return getChAlarm1Type(row, col);
    case ChannelAlarm1Status:
        return getChAlarm1Status(row, col);
    case ChannelAlarm2OnOff:
        return getChAlarm2OnOff(row,col);
    case ChannelAlarm2Type:
        return getChAlarm2Type(row, col);
    case ChannelAlarm2Status:
        return getChAlarm2Status(row, col);
    case ChannelAlarm3OnOff:
        return getChAlarm3OnOff(row,col);
    case ChannelAlarm3Type:
        return getChAlarm3Type(row, col);
    case ChannelAlarm3Status:
        return getChAlarm3Status(row, col);
    case ChannelAlarm4OnOff:
        return getChAlarm4OnOff(row,col);
    case ChannelAlarm4Type:
        return getChAlarm4Type(row, col);
    case ChannelAlarm4Status:
        return getChAlarm4Status(row, col);
    default:
        return QVariant();
    }
}

void ModelOverviewChanModel::init()
{
    m_list_channel.clear();
    const QList<quint32> &chanList = CfgChannel::instance()->getEffectiveChannel();
    for(int i=m_frameIndex*100; i<chanList.size(); ++i){
        m_list_channel.append(chanList.at(i));
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
QVariant ModelOverviewChanModel::getChData(const int row, const int col) const
{
    int colcount = columnCount();
    int index = row*colcount + col;
    if (index<m_list_channel.size()){
        quint32 channelNumber = m_list_channel.at(index);
        return CfgChannel::instance()->getChannelData(channelNumber);
    } else {
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
 * 	  QVariant：通道测量数据
 */
QVariant ModelOverviewChanModel::getChType(const int row, const int col) const
{
    int colcount = columnCount();
    int index = row*colcount + col;

    if (index<m_list_channel.size()){
        quint32 channelNumber = m_list_channel.at(index);
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
 * 	  QVariant：通道测量数据
 */
QVariant ModelOverviewChanModel::getChTag(const int row, const int col) const
{
    int colcount = columnCount();
    int index = row*colcount + col;

    if (index<m_list_channel.size()){
        quint32 channelNumber = m_list_channel.at(index);
        return CfgChannel::instance()->getChannelTag(channelNumber);           //通道标记
    } else {
        return QVariant();
    }
}

/*
 * 功能：
 *      根据行、列号，获取对应通道的单位
 * 参数：
 * 	  1.const int row：行号
 *       2.const int col：列号
 * 返回值：
 * 	  QVariant：通道测量单位
 */
QVariant ModelOverviewChanModel::getChUnit(const int row, const int col) const
{
    int colcount = columnCount();
    int index = row*colcount + col;

    if (index<m_list_channel.size()){
        quint32 channelNumber = m_list_channel.at(index);
        return CfgChannel::instance()->getChannelUnit(channelNumber);           //通道单位
    } else {
        return QVariant();
    }
}

/*
 * 功能：
 *      根据行、列号，获取对应通道的状态
 * 参数：
 * 	  1.const int row：行号
 *       2.const int col：列号
 * 返回值：
 * 	  QVariant：通道状态
 */
QVariant ModelOverviewChanModel::getChStatus(const int row, const int col) const
{
    int colcount = columnCount();
    int index = row*colcount + col;

    if (index<m_list_channel.size()){
        quint32 channelNumber = m_list_channel.at(index);
        return CfgChannel::instance()->getChannelStatus(channelNumber);
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
 * 	  QVariant：当前通道有效时返回对应通道号
 */
QVariant ModelOverviewChanModel::getChValidity(const int row, const int col) const
{
    int colcount = columnCount();
    int index = row*colcount + col;
    if(index<m_list_channel.size()){
        return m_list_channel.at(index);
    }else{
        return QVariant();
    }
}

/*
 * 功能：
 *      根据行、列号，获取对应通道数据精度
 * 参数：
 * 	  1.const int row：行号
 *       2.const int col：列号
 * 返回值：
 * 	  QVariant：精度值
 */
QVariant ModelOverviewChanModel::getChDataAccuracy(const int row, const int col) const
{
    int colcount = columnCount();
    int index = row*colcount + col;

    if (index<m_list_channel.size()){
        quint32 channelNumber = m_list_channel.at(index);
        return CfgChannel::instance()->getChannelDataAccuracy(channelNumber);
    } else {
        return QVariant();
    }
}
/*
 * 功能：
 *      根据行、列号，获取对应通道的报警1开关
 * 参数：
 * 	  1.const int row：行号
 *       2.const int col：列号
 * 返回值：
 * 	  QVariant：通道报警1开关
 */
QVariant ModelOverviewChanModel::getChAlarm1OnOff(const int row, const int col) const
{
    int colcount = columnCount();
    int index = row*colcount + col;

    if (index<m_list_channel.size()){
        quint32 channelNumber = m_list_channel.at(index);
        return CfgChannel::instance()->getChannelAlarm1OnOff(channelNumber);           //通道报警1类型
    } else {
        return QVariant();
    }
}
/*
 * 功能：
 *      根据行、列号，获取对应通道的报警1类型
 * 参数：
 * 	  1.const int row：行号
 *       2.const int col：列号
 * 返回值：
 * 	  QVariant：通道报警1类型
 */
QVariant ModelOverviewChanModel::getChAlarm1Type(const int row, const int col) const
{
    int colcount = columnCount();
    int index = row*colcount + col;

    if (index<m_list_channel.size()){
        quint32 channelNumber = m_list_channel.at(index);
        return CfgChannel::instance()->getChannelAlarm1Type(channelNumber);           //通道报警1类型
    } else {
        return QVariant();
    }
}

/*
 * 功能：
 *      根据行、列号，获取对应通道的报警1状态
 * 参数：
 * 	  1.const int row：行号
 *       2.const int col：列号
 * 返回值：
 * 	  QVariant：通道报警1状态
 */
QVariant ModelOverviewChanModel::getChAlarm1Status(const int row, const int col) const
{
    int colcount = columnCount();
    int index = row*colcount + col;

    if (index<m_list_channel.size()){
        quint32 channelNumber = m_list_channel.at(index);
        return CfgChannel::instance()->getChannelAlarm1Status(channelNumber);           //通道报警1状态
    } else {
        return QVariant();
    }
}
/*
 * 功能：
 *      根据行、列号，获取对应通道的报警2开关
 * 参数：
 * 	  1.const int row：行号
 *       2.const int col：列号
 * 返回值：
 * 	  QVariant：通道报警1开关
 */
QVariant ModelOverviewChanModel::getChAlarm2OnOff(const int row, const int col) const
{
    int colcount = columnCount();
    int index = row*colcount + col;

    if (index<m_list_channel.size()){
        quint32 channelNumber = m_list_channel.at(index);
        return CfgChannel::instance()->getChannelAlarm2OnOff(channelNumber);           //通道报警1类型
    } else {
        return QVariant();
    }
}
/*
 * 功能：
 *      根据行、列号，获取对应通道的报警2类型
 * 参数：
 * 	  1.const int row：行号
 *       2.const int col：列号
 * 返回值：
 * 	  QVariant：通道报警1类型
 */
QVariant ModelOverviewChanModel::getChAlarm2Type(const int row, const int col) const
{
    int colcount = columnCount();
    int index = row*colcount + col;

    if (index<m_list_channel.size()){
        quint32 channelNumber = m_list_channel.at(index);
        return CfgChannel::instance()->getChannelAlarm2Type(channelNumber);           //通道报警1类型
    } else {
        return QVariant();
    }
}

/*
 * 功能：
 *      根据行、列号，获取对应通道的报警2状态
 * 参数：
 * 	  1.const int row：行号
 *       2.const int col：列号
 * 返回值：
 * 	  QVariant：通道报警2状态
 */
QVariant ModelOverviewChanModel::getChAlarm2Status(const int row, const int col) const
{
    int colcount = columnCount();
    int index = row*colcount + col;

    if (index<m_list_channel.size()){
        quint32 channelNumber = m_list_channel.at(index);
        return CfgChannel::instance()->getChannelAlarm2Status(channelNumber);           //通道报警1状态
    } else {
        return QVariant();
    }
}
/*
 * 功能：
 *      根据行、列号，获取对应通道的报警3开关
 * 参数：
 * 	  1.const int row：行号
 *       2.const int col：列号
 * 返回值：
 * 	  QVariant：通道报警1开关
 */
QVariant ModelOverviewChanModel::getChAlarm3OnOff(const int row, const int col) const
{
    int colcount = columnCount();
    int index = row*colcount + col;

    if (index<m_list_channel.size()){
        quint32 channelNumber = m_list_channel.at(index);
        return CfgChannel::instance()->getChannelAlarm3OnOff(channelNumber);           //通道报警1类型
    } else {
        return QVariant();
    }
}
/*
 * 功能：
 *      根据行、列号，获取对应通道的报警3类型
 * 参数：
 * 	  1.const int row：行号
 *       2.const int col：列号
 * 返回值：
 * 	  QVariant：通道报警13类型
 */
QVariant ModelOverviewChanModel::getChAlarm3Type(const int row, const int col) const
{
    int colcount = columnCount();
    int index = row*colcount + col;

    if (index<m_list_channel.size()){
        quint32 channelNumber = m_list_channel.at(index);
        return CfgChannel::instance()->getChannelAlarm3Type(channelNumber);           //通道报警3类型
    } else {
        return QVariant();
    }
}

/*
 * 功能：
 *      根据行、列号，获取对应通道的报警3状态
 * 参数：
 * 	  1.const int row：行号
 *       2.const int col：列号
 * 返回值：
 * 	  QVariant：通道报警3状态
 */
QVariant ModelOverviewChanModel::getChAlarm3Status(const int row, const int col) const
{
    int colcount = columnCount();
    int index = row*colcount + col;

    if (index<m_list_channel.size()){
        quint32 channelNumber = m_list_channel.at(index);
        return CfgChannel::instance()->getChannelAlarm3Status(channelNumber);           //通道报警3状态
    } else {
        return QVariant();
    }
}
/*
 * 功能：
 *      根据行、列号，获取对应通道的报警4开关
 * 参数：
 * 	  1.const int row：行号
 *       2.const int col：列号
 * 返回值：
 * 	  QVariant：通道报警1开关
 */
QVariant ModelOverviewChanModel::getChAlarm4OnOff(const int row, const int col) const
{
    int colcount = columnCount();
    int index = row*colcount + col;

    if (index<m_list_channel.size()){
        quint32 channelNumber = m_list_channel.at(index);
        return CfgChannel::instance()->getChannelAlarm4OnOff(channelNumber);           //通道报警1类型
    } else {
        return QVariant();
    }
}
/*
 * 功能：
 *      根据行、列号，获取对应通道的报警4类型
 * 参数：
 * 	  1.const int row：行号
 *       2.const int col：列号
 * 返回值：
 * 	  QVariant：通道报警4类型
 */
QVariant ModelOverviewChanModel::getChAlarm4Type(const int row, const int col) const
{
    int colcount = columnCount();
    int index = row*colcount + col;

    if (index<m_list_channel.size()){
        quint32 channelNumber = m_list_channel.at(index);
        return CfgChannel::instance()->getChannelAlarm4Type(channelNumber);           //通道报警4类型
    } else {
        return QVariant();
    }
}

/*
 * 功能：
 *      根据行、列号，获取对应通道的报警4状态
 * 参数：
 * 	  1.const int row：行号
 *       2.const int col：列号
 * 返回值：
 * 	  QVariant：通道报警4状态
 */
QVariant ModelOverviewChanModel::getChAlarm4Status(const int row, const int col) const
{
    int colcount = columnCount();
    int index = row*colcount + col;

    if (index<m_list_channel.size()){
        quint32 channelNumber = m_list_channel.at(index);
        return CfgChannel::instance()->getChannelAlarm4Status(channelNumber);           //通道报警4状态
    } else {
        return QVariant();
    }
}

void ModelOverviewChanModel::slotChannelConfigChanged()
{
    beginResetModel();
    init();
    endResetModel();
}
