/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：ModelDigitalGroupChannel.cpp
 * 概   要：通道组数据模型，根据用户设定的组信息生成相应的通道组数据模型，供数字、
 * 总览（分组模式）窗体的视图部件进行数据的显示。
 * 基于组配置、Modbus寄存器表（共享内存）
 *
 * 当前版本：V1.0.0
 * 作   者：王 品
 * 完成日期：2015-6-13
 *
 * 修改版本：
 * 修改说明：
 * 修改作者：
 *
 ********************************************************************/
#include "modeldigitalgroupchannel.h"
#include "./GlobalData/cfgGlobalDef.h"
#include "./GlobalData/cfgchannel.h"
#include "../GlobalData/globalDef.h"

#include <QDebug>
#include <QColor>
#include <QDateTime>

ModelDigitalGroupChannel::ModelDigitalGroupChannel(quint32 grpIndex, QObject *parent) :
    QAbstractTableModel(parent),m_grpIndex(grpIndex)
{
    init();
}

/**
 * 功能：
 *      重置当前显示组
 * 参数：
 *      １、quint32 grpIndex　组索引
 */
void ModelDigitalGroupChannel::resetCurrentGroup(quint32 grpIndex)
{
    m_grpIndex = grpIndex;
    slotGroupChannelChanged(grpIndex);
}

int ModelDigitalGroupChannel::rowCount(const QModelIndex &) const
{
    //qDebug()<<"row"<<m_list_channel.size();
    switch (m_list_channel.size()) {
    case 1:
    case 2:
    case 3:
    case 4:
        return 2;
    case 5:
    case 6:
        return 3;
    case 7:
    case 8:
        return 4;
    case 9:
    case 10:
        return 5;
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
    case 16:
        return 4;
    case 17:
    case 18:
    case 19:
    case 20:
        return 5;
    default:
        return 0;
    }
}
int ModelDigitalGroupChannel::columnCount(const QModelIndex &) const
{
    //qDebug()<<"column"<<m_list_channel.size();
    switch (m_list_channel.size()) {
    case 1:
    case 2:
        return 1;
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
        return 2;
    case 11:
    case 12:
        return 3;
    case 13:
    case 14:
    case 15:
    case 16:
    case 17:
    case 18:
    case 19:
    case 20:
        return 4;
    default:
        return 0;
    }
}

/*
 * 功能：
 * 	 根据模型索引以及数据角色提供相应的数据
 * 参数：
 * 	  1.QModelIndex &index: 模型索引
 *       2.int role: 角色
 * 返回值：
 * 	  QVariant
 */
QVariant ModelDigitalGroupChannel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    int col = index.column();

    switch (role)
    {
    case ChannelData:
        return getChData(row,col);
    case ChannelType:
        return getChType(row, col);
    case ChannelTag:
        return getChTag(row, col);
    case ChannelColor:
        return getChColor(row, col);
    case ChannelUnit:
        return getChUnit(row, col);
    case ChannelStatus:
        return getChStatus(row,col);
    case ChannelValidity:
        return getChValidity(row,col);
    case ChannelDataAccuracy:
        return getChDataAccuracy(row,col);
    case ChannelRangeType:
        return getChRangeType(row,col);
//    case ChannelRangeUpper:
//        return getChRangeUpper(row, col);
//    case ChannelRangeDowner:
//        return getChRangeDowner(row, col);
//    case ChannelDispUpper:
//        return getChDispUpper(row, col);
//    case ChannelDispDowner:
//        return getChDispDowner(row, col);
//    case ChannelScalePos:
//        return getChScalePos(row, col);
//    case ChannelSacleDiv:
//        return getChScaleDiv(row, col);
//    case ChannelBarBase:
//        return getChBarBase(row, col);
//    case ChannelBarDiv:
//        return getChBarDiv(row, col);
//    case ChannelPartScaleOnOff:
//        return getChPartScaleOnOff(row, col);
//    case ChannelPartScalePos:
//        return getChPartScalePos(row, col);
//    case ChannelPartScaleBound:
//        return getChPartScaleBound(row, col);
//    case ChannelColorScalePos:
//        return getChColorScalePos(row, col);
//    case ChannelColorScaleColor:
//        return getChColorScaleColor(row, col);
//    case ChannelColorScaleUpper:
//        return getChColorScaleUpper(row, col);
//    case ChannelColorScaleDowner:
//        return getChColorScaleDowner(row, col);
//    case ChannelAlarmOnOff:
//        return getChAlarmOnOff(row, col);
//    case ChannelAlarmType:
//        return getChAlarmType(row, col);
    case ChannelAlarm1OnOff:
        return getChAlarm1OnOff(row,col);
    case ChannelAlarm1Type:
        return getChAlarm1Type(row, col);
    case ChannelAlarm1Status:
        return getChAlarm1Status(row, col);
    case ChannelAlarm1Value:
        return getChAlarm1Value(row, col);
    case ChannelAlarm1Color:
        return getChAlarm1Color(row, col);
    case ChannelAlarm2OnOff:
        return getChAlarm2OnOff(row,col);
    case ChannelAlarm2Type:
        return getChAlarm2Type(row, col);
    case ChannelAlarm2Status:
        return getChAlarm2Status(row, col);
    case ChannelAlarm2Value:
        return getChAlarm2Value(row, col);
    case ChannelAlarm2Color:
        return getChAlarm2Color(row, col);
    case ChannelAlarm3OnOff:
        return getChAlarm3OnOff(row,col);
    case ChannelAlarm3Type:
        return getChAlarm3Type(row, col);
    case ChannelAlarm3Status:
        return getChAlarm3Status(row, col);
    case ChannelAlarm3Value:
        return getChAlarm3Value(row, col);
    case ChannelAlarm3Color:
        return getChAlarm3Color(row, col);
    case ChannelAlarm4OnOff:
        return getChAlarm4OnOff(row,col);
    case ChannelAlarm4Type:
        return getChAlarm4Type(row, col);
    case ChannelAlarm4Status:
        return getChAlarm4Status(row, col);
    case ChannelAlarm4Value:
        return getChAlarm4Value(row, col);
    case ChannelAlarm4Color:
        return getChAlarm4Color(row, col);
    default:
        return QVariant();
    }
}

/*
 * 功能：
 *      根据当前对应组的配置信息，添加相应的通道编码号到m_list_channel
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
void ModelDigitalGroupChannel::init()
{
    m_list_channel.clear();

    if (m_grpIndex<GROUP_NUM_MAX && DevConfigPtr->displayGroup[m_grpIndex].groupSwitch) {
        m_grpValid = true;
        for (int i=0; i<DevConfigPtr->displayGroup[m_grpIndex].groupChannelNum && i<GROUP_CHANNEL_NUM_MAX; ++i){
            m_list_channel.append(DevConfigPtr->displayGroup[m_grpIndex].groupChannelItem[i]);
        }
    } else {
        m_grpValid = false;
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
QVariant ModelDigitalGroupChannel::getChData(const int row, const int col) const
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
QVariant ModelDigitalGroupChannel::getChType(const int row, const int col) const
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
QVariant ModelDigitalGroupChannel::getChTag(const int row, const int col) const
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
 *      根据行、列号，获取对应通道的颜色
 * 参数：
 * 	  1.const int row：行号
 *       2.const int col：列号
 * 返回值：
 * 	  QVariant：通道测量数据
 */
QVariant ModelDigitalGroupChannel::getChColor(const int row, const int col) const
{
    int colcount = columnCount();
    int index = row*colcount + col;

    if (index<m_list_channel.size()){
        quint32 channelNumber = m_list_channel.at(index);
        return CfgChannel::instance()->getChannelColor(channelNumber);           //通道颜色
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
QVariant ModelDigitalGroupChannel::getChUnit(const int row, const int col) const
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
QVariant ModelDigitalGroupChannel::getChStatus(const int row, const int col) const
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
QVariant ModelDigitalGroupChannel::getChValidity(const int row, const int col) const
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
QVariant ModelDigitalGroupChannel::getChDataAccuracy(const int row, const int col) const
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
 *      根据行、列号，获取对应通道量程类型
 * 参数：
 * 	  1.const int row：行号
 *       2.const int col：列号
 * 返回值：
 * 	  QVariant：量程类型
 */
QVariant ModelDigitalGroupChannel::getChRangeType(const int row, const int col) const
{
    int colcount = columnCount();
    int index = row*colcount + col;

    if (index<m_list_channel.size()){
        quint32 channelNumber = m_list_channel.at(index);
        return CfgChannel::instance()->getChannelRangeType(channelNumber);
    } else {
        return QVariant();
    }
}

/*
 * 功能：
 *      根据行、列号，获取对应通道的报警标记开关
 * 参数：
 * 	  1.const int row：行号
 *       2.const int col：列号
 * 返回值：
 * 	  QVariant：通道报警标记开关
 */
QVariant ModelDigitalGroupChannel::getChAlarmOnOff(const int row, const int col) const
{
    int colcount = columnCount();
    int index = row*colcount + col;

    if (index<m_list_channel.size()){
        quint32 channelNumber = m_list_channel.at(index);
        return CfgChannel::instance()->getChannelAlarmOnOff(channelNumber);           //通道报警标记开关
    } else {
        return QVariant();
    }
}

/*
 * 功能：
 *      根据行、列号，获取对应通道的报警标记类型
 * 参数：
 * 	  1.const int row：行号
 *       2.const int col：列号
 * 返回值：
 * 	  QVariant：通道报警标记类型
 */
QVariant ModelDigitalGroupChannel::getChAlarmType(const int row, const int col) const
{
    int colcount = columnCount();
    int index = row*colcount + col;

    if (index<m_list_channel.size()){
        quint32 channelNumber = m_list_channel.at(index);
        return CfgChannel::instance()->getChannelAlarmType(channelNumber);           //通道报警标记类型
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
QVariant ModelDigitalGroupChannel::getChAlarm1OnOff(const int row, const int col) const
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
QVariant ModelDigitalGroupChannel::getChAlarm1Type(const int row, const int col) const
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
QVariant ModelDigitalGroupChannel::getChAlarm1Status(const int row, const int col) const
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
 *      根据行、列号，获取对应通道的报警1报警值
 * 参数：
 * 	  1.const int row：行号
 *       2.const int col：列号
 * 返回值：
 * 	  QVariant：通道报警1报警值
 */
QVariant ModelDigitalGroupChannel::getChAlarm1Value(const int row, const int col) const
{
    int colcount = columnCount();
    int index = row*colcount + col;

    if (index<m_list_channel.size()){
        quint32 channelNumber = m_list_channel.at(index);
        return CfgChannel::instance()->getChannelAlarm1Value(channelNumber);           //通道报警1报警值
    } else {
        return QVariant();
    }
}

/*
 * 功能：
 *      根据行、列号，获取对应通道的报警1颜色
 * 参数：
 * 	  1.const int row：行号
 *       2.const int col：列号
 * 返回值：
 * 	  QVariant：通道报警1颜色
 */
QVariant ModelDigitalGroupChannel::getChAlarm1Color(const int row, const int col) const
{
    int colcount = columnCount();
    int index = row*colcount + col;

    if (index<m_list_channel.size()){
        quint32 channelNumber = m_list_channel.at(index);
        return CfgChannel::instance()->getChannelAlarm1Color(channelNumber);           //通道报警1颜色
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
QVariant ModelDigitalGroupChannel::getChAlarm2OnOff(const int row, const int col) const
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
QVariant ModelDigitalGroupChannel::getChAlarm2Type(const int row, const int col) const
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
QVariant ModelDigitalGroupChannel::getChAlarm2Status(const int row, const int col) const
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
 *      根据行、列号，获取对应通道的报警2报警值
 * 参数：
 * 	  1.const int row：行号
 *       2.const int col：列号
 * 返回值：
 * 	  QVariant：通道报警2报警值
 */
QVariant ModelDigitalGroupChannel::getChAlarm2Value(const int row, const int col) const
{
    int colcount = columnCount();
    int index = row*colcount + col;

    if (index<m_list_channel.size()){
        quint32 channelNumber = m_list_channel.at(index);
        return CfgChannel::instance()->getChannelAlarm2Value(channelNumber);           //通道报警1报警值
    } else {
        return QVariant();
    }
}

/*
 * 功能：
 *      根据行、列号，获取对应通道的报警2颜色
 * 参数：
 * 	  1.const int row：行号
 *       2.const int col：列号
 * 返回值：
 * 	  QVariant：通道报警2颜色
 */
QVariant ModelDigitalGroupChannel::getChAlarm2Color(const int row, const int col) const
{
    int colcount = columnCount();
    int index = row*colcount + col;

    if (index<m_list_channel.size()){
        quint32 channelNumber = m_list_channel.at(index);
        return CfgChannel::instance()->getChannelAlarm2Color(channelNumber);           //通道报警1颜色
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
QVariant ModelDigitalGroupChannel::getChAlarm3OnOff(const int row, const int col) const
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
QVariant ModelDigitalGroupChannel::getChAlarm3Type(const int row, const int col) const
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
QVariant ModelDigitalGroupChannel::getChAlarm3Status(const int row, const int col) const
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
 *      根据行、列号，获取对应通道的报警3报警值
 * 参数：
 * 	  1.const int row：行号
 *       2.const int col：列号
 * 返回值：
 * 	  QVariant：通道报警3报警值
 */
QVariant ModelDigitalGroupChannel::getChAlarm3Value(const int row, const int col) const
{
    int colcount = columnCount();
    int index = row*colcount + col;

    if (index<m_list_channel.size()){
        quint32 channelNumber = m_list_channel.at(index);
        return CfgChannel::instance()->getChannelAlarm3Value(channelNumber);           //通道报警3报警值
    } else {
        return QVariant();
    }
}

/*
 * 功能：
 *      根据行、列号，获取对应通道的报警3颜色
 * 参数：
 * 	  1.const int row：行号
 *       2.const int col：列号
 * 返回值：
 * 	  QVariant：通道报警3颜色
 */
QVariant ModelDigitalGroupChannel::getChAlarm3Color(const int row, const int col) const
{
    int colcount = columnCount();
    int index = row*colcount + col;

    if (index<m_list_channel.size()){
        quint32 channelNumber = m_list_channel.at(index);
        return CfgChannel::instance()->getChannelAlarm3Color(channelNumber);           //通道报警3颜色
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
QVariant ModelDigitalGroupChannel::getChAlarm4OnOff(const int row, const int col) const
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
QVariant ModelDigitalGroupChannel::getChAlarm4Type(const int row, const int col) const
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
QVariant ModelDigitalGroupChannel::getChAlarm4Status(const int row, const int col) const
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

/*
 * 功能：
 *      根据行、列号，获取对应通道的报警4报警值
 * 参数：
 * 	  1.const int row：行号
 *       2.const int col：列号
 * 返回值：
 * 	  QVariant：通道报警4报警值
 */
QVariant ModelDigitalGroupChannel::getChAlarm4Value(const int row, const int col) const
{
    int colcount = columnCount();
    int index = row*colcount + col;

    if (index<m_list_channel.size()){
        quint32 channelNumber = m_list_channel.at(index);
        return CfgChannel::instance()->getChannelAlarm4Value(channelNumber);           //通道报警4报警值
    } else {
        return QVariant();
    }
}

/*
 * 功能：
 *      根据行、列号，获取对应通道的报警4颜色
 * 参数：
 * 	  1.const int row：行号
 *       2.const int col：列号
 * 返回值：
 * 	  QVariant：通道报警4颜色
 */
QVariant ModelDigitalGroupChannel::getChAlarm4Color(const int row, const int col) const
{
    int colcount = columnCount();
    int index = row*colcount + col;

    if (index<m_list_channel.size()){
        quint32 channelNumber = m_list_channel.at(index);
        return CfgChannel::instance()->getChannelAlarm4Color(channelNumber);           //通道报警4颜色
    } else {
        return QVariant();
    }
}

///*
// * 功能：
// *      根据行、列号，获取对应通道的量程上限
// * 参数：
// * 	  1.const int row：行号
// *       2.const int col：列号
// * 返回值：
// * 	  QVariant：通道量程上限
// */
//QVariant ModelDigitalGroupChannel::getChRangeUpper(const int row, const int col) const
//{
//    int colcount = columnCount();
//    int index = row*colcount + col;

//    if (index<m_list_channel.size()){
//        quint32 channelNumber = m_list_channel.at(index);
//        return CfgChannel::instance()->getChannelRangeUpper(channelNumber);           //量程上限
//    } else {
//        return QVariant();
//    }
//}

///*
// * 功能：
// *      根据行、列号，获取对应通道的量程下限
// * 参数：
// * 	  1.const int row：行号
// *       2.const int col：列号
// * 返回值：
// * 	  QVariant：通道量程下限
// */
//QVariant ModelDigitalGroupChannel::getChRangeDowner(const int row, const int col) const
//{
//    int colcount = columnCount();
//    int index = row*colcount + col;

//    if (index<m_list_channel.size()){
//        quint32 channelNumber = m_list_channel.at(index);
//        return CfgChannel::instance()->getChannelRangeDowner(channelNumber);           //量程下限
//    } else {
//        return QVariant();
//    }
//}

///*
// * 功能：
// *      根据行、列号，获取对应通道的显示区域上限
// * 参数：
// * 	  1.const int row：行号
// *       2.const int col：列号
// * 返回值：
// * 	  QVariant：通道显示区域上限
// */
//QVariant ModelDigitalGroupChannel::getChDispUpper(const int row, const int col) const
//{
//    int colcount = columnCount();
//    int index = row*colcount + col;

//    if (index<m_list_channel.size()){
//        quint32 channelNumber = m_list_channel.at(index);
//        return CfgChannel::instance()->getChannelDispUpper(channelNumber);           //显示区域上限
//    } else {
//        return QVariant();
//    }
//}

///*
// * 功能：
// *      根据行、列号，获取对应通道的显示区域下限
// * 参数：
// * 	  1.const int row：行号
// *       2.const int col：列号
// * 返回值：
// * 	  QVariant：通道显示区域下限
// */
//QVariant ModelDigitalGroupChannel::getChDispDowner(const int row, const int col) const
//{
//    int colcount = columnCount();
//    int index = row*colcount + col;

//    if (index<m_list_channel.size()){
//        quint32 channelNumber = m_list_channel.at(index);
//        return CfgChannel::instance()->getChannelDispDowner(channelNumber);           //显示区域下限
//    } else {
//        return QVariant();
//    }
//}

///*
// * 功能：
// *      根据行、列号，获取对应通道的标尺位置
// * 参数：
// * 	  1.const int row：行号
// *       2.const int col：列号
// * 返回值：
// * 	  QVariant：通道显示标尺位置
// */
//QVariant ModelDigitalGroupChannel::getChScalePos(const int row, const int col) const
//{
//    int colcount = columnCount();
//    int index = row*colcount + col;

//    if (index<m_list_channel.size()){
//        quint32 channelNumber = m_list_channel.at(index);
//        return CfgChannel::instance()->getChannelScalePos(channelNumber);           //显示标尺位置
//    } else {
//        return QVariant();
//    }
//}

///*
// * 功能：
// *      根据行、列号，获取对应通道的标尺分割数
// * 参数：
// * 	  1.const int row：行号
// *       2.const int col：列号
// * 返回值：
// * 	  QVariant：通道显示标尺标尺分割数
// */
//QVariant ModelDigitalGroupChannel::getChScaleDiv(const int row, const int col) const
//{
//    int colcount = columnCount();
//    int index = row*colcount + col;

//    if (index<m_list_channel.size()){
//        quint32 channelNumber = m_list_channel.at(index);
//        return CfgChannel::instance()->getChannelScaleDiv(channelNumber);           //显示标尺标尺分割数
//    } else {
//        return QVariant();
//    }
//}

///*
// * 功能：
// *      根据行、列号，获取对应通道的标尺分割数
// * 参数：
// * 	  1.const int row：行号
// *       2.const int col：列号
// * 返回值：
// * 	  QVariant：通道显示标尺标尺分割数
// */
//QVariant ModelDigitalGroupChannel::getChScaleDiv(const int row, const int col) const
//{
//    int colcount = columnCount();
//    int index = row*colcount + col;

//    if (index<m_list_channel.size()){
//        quint32 channelNumber = m_list_channel.at(index);
//        return CfgChannel::instance()->getChannelScaleDiv(channelNumber);           //显示标尺标尺分割数
//    } else {
//        return QVariant();
//    }
//}

/*
 * 功能：
 *      组配置修改处理槽函数
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
void ModelDigitalGroupChannel::slotGroupChannelChanged(quint8 grpIndex)
{
    if (grpIndex == m_grpIndex){
        beginResetModel();
        init();
        endResetModel();
    }
}
