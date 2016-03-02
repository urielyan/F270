#include "modeltrenddigitalgroupchannel.h"
#include "../GlobalData/globalDef.h"

#include "../GlobalData/cfgtrendgroup.h"

#include <QVector3D>

extern CfgTrendGroup *p_gCfgTrendGroup;

ModelTrendDigitalGroupChannel::ModelTrendDigitalGroupChannel(quint32 grpIndex, QObject *parent) :
    QAbstractTableModel(parent),
    m_grpIndex(grpIndex)
{
    init();
//    m_oneRow = true;
//    m_oneClomn = true;
}

int ModelTrendDigitalGroupChannel::rowCount(const QModelIndex &) const
{
//    qDebug()<<"row"<<m_list_channel.size();
//    if (m_oneRow == true){
//        return 20;
//    }
    if (m_isVertical){
        switch (m_list_channel.size()) {
        case 1:
        case 2:
            return 2;
        case 3:
            return 3;
        case 4:
            return 4;
        case 5:
            return 5;
        case 6:
            return 6;
        case 7:
            return 7;
        case 8:
            return 8;
        case 9:
            return 9;
        case 10:
            return 10;
        case 11:
            return 11;
        case 12:
            return 12;
        case 13:
            return 13;
        case 14:
            return 14;
        case 15:
            return 15;
        case 16:
            return 16;
        case 17:
            return 17;
        case 18:
            return 18;
        case 19:
            return 19;
        case 20:
            return 20;
        default:
            return 0;
        }
    }else{
        switch (m_list_channel.size()) {
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
            return 1;
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
        case 16:
        case 17:
        case 18:
        case 19:
        case 20:
            return 2;
        default:
            return 0;
        }
    }

}
int ModelTrendDigitalGroupChannel::columnCount(const QModelIndex &) const
{
//    if (m_oneClomn){
//        return 10;
//    }
    //qDebug()<<"column"<<m_list_channel.size();
    if (m_isVertical){
        switch (m_list_channel.size()) {
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
        case 16:
        case 17:
        case 18:
        case 19:
        case 20:
            return 1;
        default:
            return 0;
        }
    }else{
        switch (m_list_channel.size()) {
        case 1:
        case 2:
            return 2;
        case 3:
            return 3;
        case 4:
            return 4;
        case 5:
            return 5;
        case 6:
            return 6;
        case 7:
            return 7;
        case 8:
            return 8;
        case 9:
            return 9;
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
        case 16:
        case 17:
        case 18:
        case 19:
        case 20:
            return 10;
        default:
            return 0;
        }
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
QVariant ModelTrendDigitalGroupChannel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    int col = index.column();

    switch (role)
    {
    case ChannelData:
        return getChData(row,col);
    case ChannelType:
        return getChType(row, col);
    case ChannelNum:
        return getChNum(row, col);
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

void ModelTrendDigitalGroupChannel::changePutDirection(bool isVertical)
{
    beginResetModel();
    m_isVertical = isVertical;
    endResetModel();
//    m_oneClomn = false;
//    m_oneRow = false;
}

/**
 * 功能：
 *      重置当前显示组
 * 参数：
 *      １、quint32 grpIndex　组索引
 */
void ModelTrendDigitalGroupChannel::resetCurrentGroup(quint32 grpIndex)
{
    m_grpIndex = grpIndex;
    slotGroupChannelChanged(grpIndex);
}

/*
 * 功能：
 *      根据当前对应组的配置信息，添加相应的通道编码号到m_list_channel
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
void ModelTrendDigitalGroupChannel::init()
{
    m_list_channel.clear();

    if (p_gCfgTrendGroup->getCurrentWorkMode() == RealTimeRecord) {
        if (m_grpIndex<GROUP_NUM_MAX && DevConfigPtr->displayGroup[m_grpIndex].groupSwitch) {
            m_grpValid = true;
            for (int i=0; i<DevConfigPtr->displayGroup[m_grpIndex].groupChannelNum && i<GROUP_CHANNEL_NUM_MAX; ++i){
                m_list_channel.append(DevConfigPtr->displayGroup[m_grpIndex].groupChannelItem[i]);
            }
        } else {
            m_grpValid = false;
        }
    } else {
        if (m_grpIndex<GROUP_NUM_MAX) {
            DISPLAY_GONFIG_GROUP *pGrpConfig = p_gCfgTrendGroup->getHistoryTrendBuf()->getGroupConfig(m_grpIndex);
            if (NULL == pGrpConfig) {
                m_grpValid = false;
                return;
            } else {
                m_grpValid = true;
                for (int i=0; i<pGrpConfig->groupChannelNum && i<GROUP_CHANNEL_NUM_MAX; ++i){
                    m_list_channel.append(pGrpConfig->groupChannelItem[i]);
                }
            }
        } else {
            m_grpValid = false;
        }
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
QVariant ModelTrendDigitalGroupChannel::getChData(const int row, const int col) const
{
    QLineF data;             //p1为当前值， p2为最大最小值
    int colcount = columnCount();
    int index = row*colcount + col;

    if (index<m_list_channel.size()){
        quint32 channelNumber = m_list_channel.at(index);
        if (p_gCfgTrendGroup->getCurrentWorkMode() == RealTimeRecord)
            return CfgChannel::instance()->getChannelData(channelNumber);
        else {
            QPointF point1, point2;
            point1.setX(p_gCfgTrendGroup->getCurrentRulerPointData(channelNumber).x());
            point1.setY(p_gCfgTrendGroup->getCurrentRulerPointData(channelNumber).y());
            point2 = p_gCfgTrendGroup->getCurrentMaxMinValue(channelNumber);
            data.setP1(point1);
            data.setP2(point2);
            return data;
        }
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
QVariant ModelTrendDigitalGroupChannel::getChType(const int row, const int col) const
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
 *      根据行、列号，获取对应通道的通道号
 * 参数：
 * 	  1.const int row：行号
 *       2.const int col：列号
 * 返回值：
 * 	  QVariant：通道号
 */
QVariant ModelTrendDigitalGroupChannel::getChNum(const int row, const int col) const
{
    int colcount = columnCount();
    int index = row*colcount + col;

    if (index<m_list_channel.size()){
        quint32 channelNumber = m_list_channel.at(index);
        return channelNumber;           //通道号
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
QVariant ModelTrendDigitalGroupChannel::getChTag(const int row, const int col) const
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
QVariant ModelTrendDigitalGroupChannel::getChColor(const int row, const int col) const
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
QVariant ModelTrendDigitalGroupChannel::getChUnit(const int row, const int col) const
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
QVariant ModelTrendDigitalGroupChannel::getChStatus(const int row, const int col) const
{
    int colcount = columnCount();
    int index = row*colcount + col;

    if (index<m_list_channel.size()){
        quint32 channelNumber = m_list_channel.at(index);
        if (p_gCfgTrendGroup->getCurrentWorkMode() == RealTimeRecord)
            return CfgChannel::instance()->getChannelStatus(channelNumber);
        else {
            //历史状态下，返回的是通道对应的原始状态（通道状态+报警状态）
            qint16 status = (qint16)p_gCfgTrendGroup->getCurrentRulerPointData(channelNumber).z();
            return status;
        }

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
 * 	  QVariant：通道有效性
 */
QVariant ModelTrendDigitalGroupChannel::getChValidity(const int row, const int col) const
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
QVariant ModelTrendDigitalGroupChannel::getChDataAccuracy(const int row, const int col) const
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
QVariant ModelTrendDigitalGroupChannel::getChRangeType(const int row, const int col) const
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
QVariant ModelTrendDigitalGroupChannel::getChAlarmOnOff(const int row, const int col) const
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
QVariant ModelTrendDigitalGroupChannel::getChAlarmType(const int row, const int col) const
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
QVariant ModelTrendDigitalGroupChannel::getChAlarm1OnOff(const int row, const int col) const
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
QVariant ModelTrendDigitalGroupChannel::getChAlarm1Type(const int row, const int col) const
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
QVariant ModelTrendDigitalGroupChannel::getChAlarm1Status(const int row, const int col) const
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
QVariant ModelTrendDigitalGroupChannel::getChAlarm1Value(const int row, const int col) const
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
QVariant ModelTrendDigitalGroupChannel::getChAlarm1Color(const int row, const int col) const
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
QVariant ModelTrendDigitalGroupChannel::getChAlarm2OnOff(const int row, const int col) const
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
QVariant ModelTrendDigitalGroupChannel::getChAlarm2Type(const int row, const int col) const
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
QVariant ModelTrendDigitalGroupChannel::getChAlarm2Status(const int row, const int col) const
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
QVariant ModelTrendDigitalGroupChannel::getChAlarm2Value(const int row, const int col) const
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
QVariant ModelTrendDigitalGroupChannel::getChAlarm2Color(const int row, const int col) const
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
QVariant ModelTrendDigitalGroupChannel::getChAlarm3OnOff(const int row, const int col) const
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
QVariant ModelTrendDigitalGroupChannel::getChAlarm3Type(const int row, const int col) const
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
QVariant ModelTrendDigitalGroupChannel::getChAlarm3Status(const int row, const int col) const
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
QVariant ModelTrendDigitalGroupChannel::getChAlarm3Value(const int row, const int col) const
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
QVariant ModelTrendDigitalGroupChannel::getChAlarm3Color(const int row, const int col) const
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
QVariant ModelTrendDigitalGroupChannel::getChAlarm4OnOff(const int row, const int col) const
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
QVariant ModelTrendDigitalGroupChannel::getChAlarm4Type(const int row, const int col) const
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
QVariant ModelTrendDigitalGroupChannel::getChAlarm4Status(const int row, const int col) const
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
QVariant ModelTrendDigitalGroupChannel::getChAlarm4Value(const int row, const int col) const
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
QVariant ModelTrendDigitalGroupChannel::getChAlarm4Color(const int row, const int col) const
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
//QVariant ModelTrendDigitalGroupChannel::getChRangeUpper(const int row, const int col) const
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
//QVariant ModelTrendDigitalGroupChannel::getChRangeDowner(const int row, const int col) const
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
//QVariant ModelTrendDigitalGroupChannel::getChDispUpper(const int row, const int col) const
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
//QVariant ModelTrendDigitalGroupChannel::getChDispDowner(const int row, const int col) const
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
//QVariant ModelTrendDigitalGroupChannel::getChScalePos(const int row, const int col) const
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
//QVariant ModelTrendDigitalGroupChannel::getChScaleDiv(const int row, const int col) const
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
//QVariant ModelTrendDigitalGroupChannel::getChScaleDiv(const int row, const int col) const
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
void ModelTrendDigitalGroupChannel::slotGroupChannelChanged(quint8 grpIndex)
{
    if (grpIndex == m_grpIndex){
        beginResetModel();
        init();
        endResetModel();
    }
}


Qt::ItemFlags ModelTrendDigitalGroupChannel::flags(const QModelIndex & index) const
{
    Qt::ItemFlags flags = QAbstractItemModel::flags(index);
    QVariant value1 = index.data(ChannelType);
    QVariant value2 = index.data(ChannelRangeType);
    if(value1.isValid() && value1.toUInt() != Channel_Type_DO && value2.isValid() && value2.toUInt() == OFF){
        flags &= ~(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    }
    return flags;
}
