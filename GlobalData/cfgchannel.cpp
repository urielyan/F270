/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：CfgChannel.cpp
 * 概   要：CfgChannel是有效通道配置类，根据LPSRAM系统配置的“系统状态”，添加
 * 有效的通道号至相应的通道号链表中, 并提供通道相应的访问函数
 *
 * 当前版本：V1.0.0
 * 作   者：王 品
 * 完成日期：2015-6-29
 *
 * 修改版本：
 * 修改说明：
 * 修改作者：
 *
 ********************************************************************/
#include "cfgchannel.h"

#include "./GlobalData/cfgGlobalDef.h"
#include "./GlobalData/dataGlobalDef_ModbusReg.h"
#include "../Common/mathexpanalysis.h"
#include "../Common/eventaction.h"

#include "globalDef.h"

#include <QSharedMemory>
#include <QVariant>
#include <QColor>
#include <QDebug>
#include <QNetworkInterface>

#include "Common/devboard.h"
#include "Common/devgroup.h"
#include "Common/devchannelai.h"
#include "Common/devchanneldi.h"
#include "Common/devchanneldo.h"
#include "Common/devchannelmath.h"
#include "Common/devchannelcomm.h"



CfgChannel * CfgChannel::instance()
{
    static CfgChannel data;
    return &data;
}

CfgChannel::CfgChannel(QObject *parent) :
    QObject(parent)
{
    //内部开关, Relay DO配置
    slotSWConfigChanged();
    slotDOConfigChanged();
}

/*
 * 功能：
 * 	  获取基准通道测量值，如果基准通道号比当前通道号大，则取基准通道上次测量值，否则取当前测量值
 * 参数：
 * 	  1.quint32 self: 当前通道编码
 *       2.quint32 basic：基准通道编码
 * 返回值：
 * 	  基准通道测量值
 */
float CfgChannel::getChannelData(quint32 chanSelf, quint32 basic)
{
    QVariant data;
    if (basic<=100 && basic>0) {   //常量值
        return DevConfigPtr->mathConstK[basic-1];
    }

    if(chanSelf<basic) {
         //获取basic通道上次测量值
        data = getChannelData(basic);
    } else {
        data = getChannelObj(basic)->getChannelCurData();
    }

    if (data.isValid())
        return data.toFloat();
    else
        return qQNaN();
}

/*
 * 功能：
 * 	  根据通道号，查询并返回对应通道的测量数据
 * 参数：
 * 	  1.quint32 chanNum: 通道号编码
 * 返回值：
 * 	  QVariant：通道数据
 */
QVariant CfgChannel::getChannelData(quint32 chanNum)
{
    Channel *pChannel = getChannelObj(chanNum);
    if (pChannel)
        return pChannel->getChannelData();
    else
        return QVariant();
}

/*
 * 功能：
 * 	  根据通道号，查询并返回对应通道的通道标记
 * 参数：
 * 	  1.quint32 chanNum: 通道号编码
 * 返回值：
 * 	  QVariant：通道标记
 */
QVariant CfgChannel::getChannelTag(quint32 chanNum)
{
    Channel *pChannel = getChannelObj(chanNum);
    if (pChannel)
        return pChannel->getChannelTag();
    else
        return QVariant();
}

/*
 * 功能：
 * 	  根据通道号，查询并返回对应通道的通道颜色
 * 参数：
 * 	  1.quint32 chanNum: 通道号编码
 * 返回值：
 * 	  QVariant：通道颜色
 */
QVariant CfgChannel::getChannelColor(quint32 chanNum)
{
    Channel *pChannel = getChannelObj(chanNum);
    if (pChannel)
        return pChannel->getChannelColor();
    else
        return QVariant();
}

/*
 * 功能：
 * 	  根据通道号，查询并返回对应通道的通道单位
 * 参数：
 * 	  1.quint32 chanNum: 通道号编码
 * 返回值：
 * 	  QVariant：通道单位
 */
QVariant CfgChannel::getChannelUnit(quint32 chanNum)
{
    Channel *pChannel = getChannelObj(chanNum);
    if (pChannel)
        return pChannel->getChannelUnit();
    else
        return QVariant();
}

/*
 * 功能：
 * 	  根据通道号，查询并返回对应通道的量程类型
 * 参数：
 * 	  1.quint32 chanNum: 通道号编码
 * 返回值：
 * 	  QVariant：通道量程类型
 */
QVariant CfgChannel::getChannelRangeType(quint32 chanNum)
{
    Channel *pChannel = getChannelObj(chanNum);
    if (pChannel)
        return pChannel->getChannelRangeType();
    else
        return QVariant();
}

/*
 * 功能：
 * 	  根据通道号，查询并返回对应通道的量程上限
 * 参数：
 * 	  1.quint32 chanNum: 通道号编码
 * 返回值：
 * 	  QVariant：通道量程上限，qreal
 */
QVariant CfgChannel::getChannelRangeUpper(quint32 chanNum)
{
    Channel *pChannel = getChannelObj(chanNum);
    if (pChannel)
        return pChannel->getChannelRangeUpper();
    else
        return QVariant();
}

/*
 * 功能：
 * 	  根据通道号，查询并返回对应通道的量程下限
 * 参数：
 * 	  1.quint32 chanNum: 通道号编码
 * 返回值：
 * 	  QVariant：通道量程下限
 */
QVariant CfgChannel::getChannelRangeDowner(quint32 chanNum)
{
    Channel *pChannel = getChannelObj(chanNum);
    if (pChannel)
        return pChannel->getChannelRangeDowner();
    else
        return QVariant();
}

/*
 * 功能：
 * 	  根据通道号，查询并返回对应通道的状态
 * 参数：
 * 	  1.quint32 chanNum: 通道号编码
 * 返回值：
 * 	  QVariant：通道状态
 */
QVariant CfgChannel::getChannelStatus(quint32 chanNum)
{
    Channel *pChannel = getChannelObj(chanNum);
    if (pChannel)
        return pChannel->getChannelStatus();
    else
        return QVariant();
}

/*
 * 功能：
 * 	  根据通道号，查询并返回对应通道的数据精度
 * 参数：
 * 	  1.quint32 chanNum: 通道号编码
 * 返回值：
 * 	  QVariant：精度值，范围０～５
 */
QVariant CfgChannel::getChannelDataAccuracy(quint32 chanNum)
{
    Channel *pChannel = getChannelObj(chanNum);
    if (pChannel)
        return pChannel->getChannelDataAccuracy();
    else
        return QVariant();
}

/*
 * 功能：
 * 	  根据通道号，查询并返回对应通道测量校正的点数
 * 参数：
 * 	  1.quint32 chanNum: 通道号编码
 * 返回值：
 * 	  QVariant：点数，范围 2～12
 */
QVariant CfgChannel::getChannelCaliCorrectNum(quint32 chanNum)
{
    Channel *pChannel = getChannelObj(chanNum);
    if (pChannel)
        return pChannel->getChannelCaliCorrectNum();
    else
        return QVariant();
}

/*
 * 功能：
 * 	  根据通道号，查询并返回对应通道的显示区域上限
 * 参数：
 * 	  1.quint32 chanNum: 通道号编码
 * 返回值：
 * 	  QVariant：通道显示区域上限
 */
QVariant CfgChannel::getChannelDispUpper(quint32 chanNum)
{
    Channel *pChannel = getChannelObj(chanNum);
    if (pChannel)
        return pChannel->getChannelDispZoneUpper();
    else
        return QVariant();
}

/*
 * 功能：
 * 	  根据通道号，查询并返回对应通道的显示区域下限
 * 参数：
 * 	  1.quint32 chanNum: 通道号编码
 * 返回值：
 * 	  QVariant：通道显示区域下限
 */
QVariant CfgChannel::getChannelDispDowner(quint32 chanNum)
{
    Channel *pChannel = getChannelObj(chanNum);
    if (pChannel)
        return pChannel->getChannelDispZoneDowner();
    else
        return QVariant();
}

/*
 * 功能：
 * 	  根据通道号，查询并返回对应通道的标尺位置
 * 参数：
 * 	  1.quint32 chanNum: 通道号编码
 * 返回值：
 * 	  QVariant：通道标尺位置
 */
QVariant CfgChannel::getChannelScalePos(quint32 chanNum)
{
    Channel *pChannel = getChannelObj(chanNum);
    if (pChannel)
        return pChannel->getChannelScalePos();
    else
        return QVariant();
}


/*
 * 功能：
 * 	  根据通道号，查询并返回对应通道的标尺分割数
 * 参数：
 * 	  1.quint32 chanNum: 通道号编码
 * 返回值：
 * 	  QVariant：通道的标尺分割数
 */
QVariant CfgChannel::getChannelScaleDiv(quint32 chanNum)
{
    Channel *pChannel = getChannelObj(chanNum);
    if (pChannel)
        return pChannel->getChannelScaleDiv();
    else
        return QVariant();
}

/*
 * 功能：
 * 	  根据通道号，查询并返回对应通道的棒图基准位置
 * 参数：
 * 	  1.quint32 chanNum: 通道号编码
 * 返回值：
 * 	  QVariant：通道的棒图基准位置
 */
QVariant CfgChannel::getChannelBarBase(quint32 chanNum)
{
    Channel *pChannel = getChannelObj(chanNum);
    if (pChannel)
        return pChannel->getChannelBarBase();
    else
        return QVariant();
}

/*
 * 功能：
 * 	  根据通道号，查询并返回对应通道的棒图分割数
 * 参数：
 * 	  1.quint32 chanNum: 通道号编码
 * 返回值：
 * 	  QVariant：通道的棒图分割数
 */
QVariant CfgChannel::getChannelBarDiv(quint32 chanNum)
{
    Channel *pChannel = getChannelObj(chanNum);
    if (pChannel)
        return pChannel->getChannelBarDiv();
    else
        return QVariant();
}


/*
 * 功能：
 * 	  根据通道号，查询并返回对应通道的部分压缩放大开关
 * 参数：
 * 	  1.quint32 chanNum: 通道号编码
 * 返回值：
 * 	  QVariant：通道的部分压缩放大开关
 */
QVariant CfgChannel::getChannelPartScaleOnOff(quint32 chanNum)
{
    Channel *pChannel = getChannelObj(chanNum);
    if (pChannel)
        return pChannel->getChannelPartScaleOnOff();
    else
        return QVariant();
}

/*
 * 功能：
 * 	  根据通道号，查询并返回对应通道的部分压缩放大位置
 * 参数：
 * 	  1.quint32 chanNum: 通道号编码
 * 返回值：
 * 	  QVariant：通道的部分压缩放大位置
 */
QVariant CfgChannel::getChannelPartScalePos(quint32 chanNum)
{
    Channel *pChannel = getChannelObj(chanNum);
    if (pChannel)
        return pChannel->getChannelPartScalePos();
    else
        return QVariant();
}


/*
 * 功能：
 * 	  根据通道号，查询并返回对应通道的部分压缩放大边界值
 * 参数：
 * 	  1.quint32 chanNum: 通道号编码
 * 返回值：
 * 	  QVariant：通道的部分压缩放大边界值
 */
QVariant CfgChannel::getChannelPartScaleBound(quint32 chanNum)
{
    Channel *pChannel = getChannelObj(chanNum);
    if (pChannel)
        return pChannel->getChannelPartScaleBound();
    else
        return QVariant();
}

/*
 * 功能：
 * 	  根据通道号，查询并返回对应通道的彩色标尺带显示位置
 * 参数：
 * 	  1.quint32 chanNum: 通道号编码
 * 返回值：
 * 	  QVariant：通道的彩色标尺带显示位置
 */
QVariant CfgChannel::getChannelColorScalePos(quint32 chanNum)
{
    Channel *pChannel = getChannelObj(chanNum);
    if (pChannel)
        return pChannel->getChannelColorScalePos();
    else
        return QVariant();
}

/*
 * 功能：
 * 	  根据通道号，查询并返回对应通道的彩色标尺带显示颜色
 * 参数：
 * 	  1.quint32 chanNum: 通道号编码
 * 返回值：
 * 	  QVariant：通道的彩色标尺带显示颜色
 */
QVariant CfgChannel::getChannelColorScaleColor(quint32 chanNum)
{
    Channel *pChannel = getChannelObj(chanNum);
    if (pChannel)
        return pChannel->getChannelColorScaleColor();
    else
        return QVariant();
}

/*
 * 功能：
 * 	  根据通道号，查询并返回对应通道的彩色标尺带显示位置上限
 * 参数：
 * 	  1.quint32 chanNum: 通道号编码
 * 返回值：
 * 	  QVariant：通道的彩色标尺带显示位置上限
 */
QVariant CfgChannel::getChannelColorScaleUpper(quint32 chanNum)
{
    Channel *pChannel = getChannelObj(chanNum);
    if (pChannel)
        return pChannel->getChannelColorScaleUpper();
    else
        return QVariant();
}

/*
 * 功能：
 * 	  根据通道号，查询并返回对应通道的彩色标尺带显示位置下限
 * 参数：
 * 	  1.quint32 chanNum: 通道号编码
 * 返回值：
 * 	  QVariant：通道的彩色标尺带显示位置下限
 */
QVariant CfgChannel::getChannelColorScaleDowner(quint32 chanNum)
{
    Channel *pChannel = getChannelObj(chanNum);
    if (pChannel)
        return pChannel->getChannelColorScaleDowner();
    else
        return QVariant();
}

/*
 * 功能：
 * 	  根据通道号，查询并返回对应通道的报警点标记开关
 * 参数：
 * 	  1.quint32 chanNum: 通道号编码
 * 返回值：
 * 	  QVariant：通道的报警点标记开关， bool
 */
QVariant CfgChannel::getChannelAlarmOnOff(quint32 chanNum)
{
    Channel *pChannel = getChannelObj(chanNum);
    if (pChannel)
        return pChannel->getChannelAlarmOnOff();
    else
        return QVariant();
}

/*
 * 功能：
 * 	  根据通道号，查询并返回对应通道的报警点标记类型
 * 参数：
 * 	  1.quint32 chanNum: 通道号编码
 * 返回值：
 * 	  QVariant：通道的报警点标记类型，uint
 */
QVariant CfgChannel::getChannelAlarmType(quint32 chanNum)
{
    Channel *pChannel = getChannelObj(chanNum);
    if (pChannel)
        return pChannel->getChannelAlarmType();
    else
        return QVariant();
}

/*
 * 功能：
 * 	  根据通道号，查询并返回对应通道的报警1开关状态
 * 参数：
 * 	  1.quint32 chanNum: 通道号编码
 * 返回值：
 * 	  QVariant：通道的报警点报警1开关状态
 */
QVariant CfgChannel::getChannelAlarm1OnOff(quint32 chanNum)
{
    Channel *pChannel = getChannelObj(chanNum);
    if (pChannel)
        return pChannel->getChannelAlarm1OnOff();
    else
        return QVariant();
}
/*
 * 功能：
 * 	  根据通道号，查询并返回对应通道的报警1类型
 * 参数：
 * 	  1.quint32 chanNum: 通道号编码
 * 返回值：
 * 	  QVariant：通道的报警点报警1类型，uint
 */
QVariant CfgChannel::getChannelAlarm1Type(quint32 chanNum)
{
    Channel *pChannel = getChannelObj(chanNum);
    if (pChannel)
        return pChannel->getChannelAlarm1Type();
    else
        return QVariant();
}

/*
 * 功能：
 * 	  根据通道号，查询并返回对应通道的报警1状态
 * 参数：
 * 	  1.quint32 chanNum: 通道号编码
 * 返回值：
 * 	  QVariant：通道的报警点报警1状态
 */
QVariant CfgChannel::getChannelAlarm1Status(quint32 chanNum)
{
    Channel *pChannel = getChannelObj(chanNum);
    if (pChannel)
        return pChannel->getChannelAlarm1Status();
    else
        return QVariant();
}

/*
 * 功能：
 * 	  根据通道号，查询并返回对应通道的报警1报警值
 * 参数：
 * 	  1.quint32 chanNum: 通道号编码
 * 返回值：
 * 	  QVariant：通道的报警点报警1报警值
 */
QVariant CfgChannel::getChannelAlarm1Value(quint32 chanNum)
{
    Channel *pChannel = getChannelObj(chanNum);
    if (pChannel)
        return pChannel->getChannelAlarm1Value();
    else
        return QVariant();
}

/*
 * 功能：
 * 	  根据通道号，查询并返回对应通道的报警2颜色
 * 参数：
 * 	  1.quint32 chanNum: 通道号编码
 * 返回值：
 * 	  QVariant：通道的报警点报警2颜色
 */
QVariant CfgChannel::getChannelAlarm1Color(quint32 chanNum)
{
    Channel *pChannel = getChannelObj(chanNum);
    if (pChannel)
        return pChannel->getChannelAlarm1Color();
    else
        return QVariant();
}

/*
 * 功能：
 * 	  根据通道号，查询并返回对应通道的报警2开关状态
 * 参数：
 * 	  1.quint32 chanNum: 通道号编码
 * 返回值：
 * 	  QVariant：通道的报警点报警2开关状态
 */
QVariant CfgChannel::getChannelAlarm2OnOff(quint32 chanNum)
{
    Channel *pChannel = getChannelObj(chanNum);
    if (pChannel)
        return pChannel->getChannelAlarm2OnOff();
    else
        return QVariant();
}

/*
 * 功能：
 * 	  根据通道号，查询并返回对应通道的报警2类型
 * 参数：
 * 	  1.quint32 chanNum: 通道号编码
 * 返回值：
 * 	  QVariant：通道的报警点报警2类型，uint
 */
QVariant CfgChannel::getChannelAlarm2Type(quint32 chanNum)
{
    Channel *pChannel = getChannelObj(chanNum);
    if (pChannel)
        return pChannel->getChannelAlarm2Type();
    else
        return QVariant();
}

/*
 * 功能：
 * 	  根据通道号，查询并返回对应通道的报警2状态
 * 参数：
 * 	  1.quint32 chanNum: 通道号编码
 * 返回值：
 * 	  QVariant：通道的报警点报警2状态
 */
QVariant CfgChannel::getChannelAlarm2Status(quint32 chanNum)
{
    Channel *pChannel = getChannelObj(chanNum);
    if (pChannel)
        return pChannel->getChannelAlarm2Status();
    else
        return QVariant();
}

/*
 * 功能：
 * 	  根据通道号，查询并返回对应通道的报警2报警值
 * 参数：
 * 	  1.quint32 chanNum: 通道号编码
 * 返回值：
 * 	  QVariant：通道的报警点报警2报警值
 */
QVariant CfgChannel::getChannelAlarm2Value(quint32 chanNum)
{
    Channel *pChannel = getChannelObj(chanNum);
    if (pChannel)
        return pChannel->getChannelAlarm2Value();
    else
        return QVariant();
}
/*
 * 功能：
 * 	  根据通道号，查询并返回对应通道的报警2颜色
 * 参数：
 * 	  1.quint32 chanNum: 通道号编码
 * 返回值：
 * 	  QVariant：通道的报警点报警2颜色
 */
QVariant CfgChannel::getChannelAlarm2Color(quint32 chanNum)
{
    Channel *pChannel = getChannelObj(chanNum);
    if (pChannel)
        return pChannel->getChannelAlarm2Color();
    else
        return QVariant();
}

/*
 * 功能：
 * 	  根据通道号，查询并返回对应通道的报警3开关状态
 * 参数：
 * 	  1.quint32 chanNum: 通道号编码
 * 返回值：
 * 	  QVariant：通道的报警点报警3开关状态
 */
QVariant CfgChannel::getChannelAlarm3OnOff(quint32 chanNum)
{
    Channel *pChannel = getChannelObj(chanNum);
    if (pChannel)
        return pChannel->getChannelAlarm3OnOff();
    else
        return QVariant();
}

/*
 * 功能：
 * 	  根据通道号，查询并返回对应通道的报警3类型
 * 参数：
 * 	  1.quint32 chanNum: 通道号编码
 * 返回值：
 * 	  QVariant：通道的报警点报警3类型，uint
 */
QVariant CfgChannel::getChannelAlarm3Type(quint32 chanNum)
{
    Channel *pChannel = getChannelObj(chanNum);
    if (pChannel)
        return pChannel->getChannelAlarm3Type();
    else
        return QVariant();
}

/*
 * 功能：
 * 	  根据通道号，查询并返回对应通道的报警3状态
 * 参数：
 * 	  1.quint32 chanNum: 通道号编码
 * 返回值：
 * 	  QVariant：通道的报警点报警3状态
 */
QVariant CfgChannel::getChannelAlarm3Status(quint32 chanNum)
{
    Channel *pChannel = getChannelObj(chanNum);
    if (pChannel)
        return pChannel->getChannelAlarm3Status();
    else
        return QVariant();
}

/*
 * 功能：
 * 	  根据通道号，查询并返回对应通道的报警3报警值
 * 参数：
 * 	  1.quint32 chanNum: 通道号编码
 * 返回值：
 * 	  QVariant：通道的报警点报警3报警值
 */
QVariant CfgChannel::getChannelAlarm3Value(quint32 chanNum)
{
    Channel *pChannel = getChannelObj(chanNum);
    if (pChannel)
        return pChannel->getChannelAlarm3Value();
    else
        return QVariant();
}
/*
 * 功能：
 * 	  根据通道号，查询并返回对应通道的报警3颜色
 * 参数：
 * 	  1.quint32 chanNum: 通道号编码
 * 返回值：
 * 	  QVariant：通道的报警点报警3颜色
 */
QVariant CfgChannel::getChannelAlarm3Color(quint32 chanNum)
{
    Channel *pChannel = getChannelObj(chanNum);
    if (pChannel)
        return pChannel->getChannelAlarm3Color();
    else
        return QVariant();
}

/*
 * 功能：
 * 	  根据通道号，查询并返回对应通道的报警4开关状态
 * 参数：
 * 	  1.quint32 chanNum: 通道号编码
 * 返回值：
 * 	  QVariant：通道的报警点报警4开关状态
 */
QVariant CfgChannel::getChannelAlarm4OnOff(quint32 chanNum)
{
    Channel *pChannel = getChannelObj(chanNum);
    if (pChannel)
        return pChannel->getChannelAlarm4OnOff();
    else
        return QVariant();
}


/*
 * 功能：
 * 	  根据通道号，查询并返回对应通道的报警4类型
 * 参数：
 * 	  1.quint32 chanNum: 通道号编码
 * 返回值：
 * 	  QVariant：通道的报警点报警4类型，uint
 */
QVariant CfgChannel::getChannelAlarm4Type(quint32 chanNum)
{
    Channel *pChannel = getChannelObj(chanNum);
    if (pChannel)
        return pChannel->getChannelAlarm4Type();
    else
        return QVariant();
}


/*
 * 功能：
 * 	  根据通道号，查询并返回对应通道的报警4状态
 * 参数：
 * 	  1.quint32 chanNum: 通道号编码
 * 返回值：
 * 	  QVariant：通道的报警点报警4状态
 */
QVariant CfgChannel::getChannelAlarm4Status(quint32 chanNum)
{
    Channel *pChannel = getChannelObj(chanNum);
    if (pChannel)
        return pChannel->getChannelAlarm4Status();
    else
        return QVariant();
}

/*
 * 功能：
 * 	  根据通道号，查询并返回对应通道的报警4报警值
 * 参数：
 * 	  1.quint32 chanNum: 通道号编码
 * 返回值：
 * 	  QVariant：通道的报警点报警4报警值
 */
QVariant CfgChannel::getChannelAlarm4Value(quint32 chanNum)
{
    Channel *pChannel = getChannelObj(chanNum);
    if (pChannel)
        return pChannel->getChannelAlarm4Value();
    else
        return QVariant();
}
/*
 * 功能：
 * 	  根据通道号，查询并返回对应通道的报警4颜色
 * 参数：
 * 	  1.quint32 chanNum: 通道号编码
 * 返回值：
 * 	  QVariant：通道的报警点报警4颜色
 */
QVariant CfgChannel::getChannelAlarm4Color(quint32 chanNum)
{
    Channel *pChannel = getChannelObj(chanNum);
    if (pChannel)
        return pChannel->getChannelAlarm4Color();
    else
        return QVariant();
}

QVariant CfgChannel::getChannelAlarm(quint32 chanNum)
{
    Channel *pChannel = getChannelObj(chanNum);
    if (pChannel)
        return pChannel->getChannelAlarm();
    else
        return QVariant();
}

/**
 * 功能：
 *          获取通道标记
 * 参数：
 *          １、quint32 chanNum 通道编号
 * 返回值：
 *          QVariant：通道标记
 */
QVariant CfgChannel::getChannelTagName(quint32 chanNum)
{
    Channel *pChannel = getChannelObj(chanNum);
    if (pChannel)
        return pChannel->getChannelTagName();
    else
        return QVariant();
}
/**
 * 功能：
 *          获取通道标记号码
 * 参数：
 *          １、quint32 chanNum 通道编号
 * 返回值：
 *          QVariant：通道标记号码
 */
QVariant CfgChannel::getChannelTagNumber(quint32 chanNum)
{
    Channel *pChannel = getChannelObj(chanNum);
    if (pChannel)
        return pChannel->getChannelTagNumber();
    else
        return QVariant();
}
/**
 * 功能：
 *          获取通道号码
 * 参数：
 *          １、quint32 chanNum 通道编号
 * 返回值：
 *          QVariant：通道号码(QString)
 */
QVariant CfgChannel::getChannelNumber(quint32 chanNum)
{
    Channel *pChannel = getChannelObj(chanNum);
    if (pChannel)
        return pChannel->getChannelNumber();
    else
        return QVariant();
}
/*
 * 功能：
 * 	  根据通道号，查询通道报警状态
 * 参数：
 * 	  1.quint32 chanNum: 通道号编码
 * 返回值：
 * 	  QVariant：通道发生报警时返回优先级最高的状态值
 */
QVariant CfgChannel::getChannelAlarmStatus(quint32 chanNum)
{
    QVariant chanAlarmStatus;
    quint32 value = SQL_Alarm_Status_Hold_Release_Ack;

    if(m_list_Chan.contains(chanNum)){
        QVariant chanAlarmOnOff = getChannelAlarm1OnOff(chanNum);
        if(chanAlarmOnOff.isValid() && chanAlarmOnOff.toUInt() != OFF){
            QVariant alarmStatusValue = getChannelAlarm1Status(chanNum);
            if(value >= alarmStatusValue.toUInt()){
                chanAlarmStatus = alarmStatusValue;
                value = alarmStatusValue.toUInt();
            }
        }

        chanAlarmOnOff = getChannelAlarm2OnOff(chanNum);
        if(chanAlarmOnOff.isValid() && chanAlarmOnOff.toUInt() != OFF){
            QVariant alarmStatusValue = getChannelAlarm2Status(chanNum);
            if(value >= alarmStatusValue.toUInt()){
                chanAlarmStatus = alarmStatusValue;
                value = alarmStatusValue.toUInt();
            }
        }


        chanAlarmOnOff = getChannelAlarm3OnOff(chanNum);
        if(chanAlarmOnOff.isValid() && chanAlarmOnOff.toUInt() != OFF){
            QVariant alarmStatusValue = getChannelAlarm3Status(chanNum);
            if(value >= alarmStatusValue.toUInt()){
                chanAlarmStatus = alarmStatusValue;
                value = alarmStatusValue.toUInt();
            }
        }


        chanAlarmOnOff = getChannelAlarm4OnOff(chanNum);
        if(chanAlarmOnOff.isValid() && chanAlarmOnOff.toUInt() != OFF){
            QVariant alarmStatusValue = getChannelAlarm4Status(chanNum);
            if(value >= alarmStatusValue.toUInt()){
                chanAlarmStatus = alarmStatusValue;
                value = alarmStatusValue.toUInt();
            }
        }
    }

    return chanAlarmStatus;
}

/**
 * 功能：
 *          获取设备内部开关状态
 * 参数：
 *          １、quint32 swNum 开关编号，范围０～９９
 * 返回值：
 *          QVariant：返回开关状态 true/false:开/关
 */
QVariant CfgChannel::getDeviceSwitchStatus(quint32 swNum)
{
    if(swNum < MAIN_UNIT_INTERNAL_SWITCH){
        MODBUS_REGISTER_MAP *pModbusRegMap;
        bool switchStatus = false;
        DevShareMem.lock();
        pModbusRegMap = const_cast<MODBUS_REGISTER_MAP*>(static_cast<MODBUS_REGISTER_MAP const *>(DevShareMem.constData()));
        switchStatus = pModbusRegMap->holdReg_Device_Switch_Data_Int16[swNum] == 0;
        DevShareMem.unlock();
        return switchStatus;
    }else{
        return QVariant();
    }
}

/**
 * 功能：
 *          获取设备内部开关名称（Tag）
 * 参数：
 *          １、quint32 swNum 开关编号，范围０～９９
 * 返回值：
 *          QVariant：返回开关名称
 */
QVariant CfgChannel::getDeviceSwitchName(quint32 swNum)
{
    if(swNum < MAIN_UNIT_INTERNAL_SWITCH){
        return "S" + QString::number(swNum + 1).rightJustified(3,'0');
    }else{
        return QVariant();
    }
}

/**
 * 功能：
 *          获取设备内部开关工作模式
 * 参数：
 *          １、quint32 swNum 开关编号，范围０～９９
 * 返回值：
 *          QVariant：返回开关工作模式 ，报警/手动
 */
QVariant CfgChannel::getDeviceSwitchMode(quint32 swNum)
{
    if(swNum < MAIN_UNIT_INTERNAL_SWITCH){
        return DevConfigPtr->systemConfig.interSwitchMode[swNum];
    }else{
        return QVariant();
    }
}

/**
 * 功能：
 *          获取设备IP
 * 返回值：
 *          QString：设备IP
 */
QString CfgChannel::getDeviceIPAddress()
{
    return IPToString(DevConfigPtr->networkBasic.ipAddress);
}

/**
 * 功能：
 *          获取设备子网掩码
 * 返回值：
 *          QString：子网掩码
 */
QString CfgChannel::getDeviceSubnetMask()
{
    return IPToString(DevConfigPtr->networkBasic.subnetMask);
}

/**
 * 功能：
 *          获取设备默认网关
 * 返回值：
 *          QString：默认网关
 */
QString CfgChannel::getDefaultGateway()
{
    return IPToString(DevConfigPtr->networkBasic.defaultGateway);
}

/**
 * 功能：
 *          获取设备MAC地址
 * 返回值：
 *          QString：MAC地址
 */
QString CfgChannel::getDeviceMACAddress()
{
    QList<QNetworkInterface> list = QNetworkInterface::allInterfaces();
    foreach (QNetworkInterface interface, list) {
        if((interface.flags() & QNetworkInterface::IsUp) &&  (!(interface.flags() & QNetworkInterface::IsLoopBack))){
            return interface.hardwareAddress().replace(':','-');
        }
    }
    return QString();
}

/**
 * 功能：
 *          获取设备主DNS服务器IP
 * 返回值：
 *          QString：主DNS服务器IP
 */
QString CfgChannel::getPrimaryDnsServer()
{
    return IPToString(DevConfigPtr->networkBasic.DNSAddrFirst);
}

/**
 * 功能：
 *          获取设备从DNS服务器IP
 * 返回值：
 *          QString：从DNS服务器IP
 */
QString CfgChannel::getSecondaryDnsServer()
{
    return IPToString(DevConfigPtr->networkBasic.DNSAddrSecond);
}

/**
 * 功能：
 *          获取设备FTP服务器状态
 * 返回值：
 *          bool：状态true/on, false/off
 */
bool CfgChannel::getFTPServerStatus()
{
    return DevConfigPtr->networkServer.serverFTPOnOff != 0;
}

/**
 * 功能：
 *          获取设备WEB服务器状态
 * 返回值：
 *          bool：状态true/on, false/off
 */
bool CfgChannel::getWEBServerStatus()
{
    return DevConfigPtr->networkServer.serverHTTPOnOff != 0;
}

/**
 * 功能：
 *          获取设备SNTP服务器状态
 * 返回值：
 *          bool：状态true/on, false/off
 */
bool CfgChannel::getSNTPServerStatus()
{
    return DevConfigPtr->networkServer.serverSNTPOnOff != 0;
}

/**
 * 功能：
 *          获取设备Modbus服务状态
 * 返回值：
 *          bool：状态true/on, false/off
 */
bool CfgChannel::getModbusStatus()
{
    return DevConfigPtr->networkServer.serverModbusOnOff != 0;
}

//bool CfgChannel::getGeneralStatus()
//{
//    return DevConfigPtr->networkServer.serverFTPOnOff != 0;
//}

//bool CfgChannel::getEthernetStatus()
//{
//}

//bool CfgChannel::getDarwinStatus()
//{
//}

/**
 * 功能：
 *          IP字节数组转字符串
 * 参数：
 *          quint8 *ip: IP字节数组
 * 返回值：
 *          QString：IP字符串
 */
QString CfgChannel::IPToString(quint8 *ip)
{
    Q_ASSERT(ip);
    return QString("%0.%1.%2.%3").arg(ip[0]).arg(ip[1]).arg(ip[2]).arg(ip[3]);
}
/**
 * 功能：
 *          IP字符串转字节数组
 * 参数：
 *          1、quint8 *ip: IP字节数组
 *          2、QString ipStr：IP字符串
 *   返回值：
 *          bool： 成功返回true
 */
bool CfgChannel::IPFromString(quint8 *ip, QString ipStr)
{
    Q_ASSERT(ip);
    QStringList sl = ipStr.split('.');
    if(sl.length() != 4){
        return false;
    }

    for(int i=0; i<4; ++i){
        ip[i] = (quint8)(sl[i].toUInt());
    }
    return true;
}

/**
 * 功能：
 *          MAC字节数组转字符串
 * 参数：
 *          quint8 *mac: MAC字节数组
 * 返回值：
 *          QString：MAC字符串
 */
QString CfgChannel::MacToString(quint8 *mac)
{
    Q_ASSERT(mac);
    return (QString("%0-%1-%2-%3-%4-%5").arg((uint)(mac[0]),0,16).arg((uint)(mac[1]),0,16).arg((uint)(mac[2]),0,16).arg((uint)(mac[3]),0,16).arg((uint)(mac[4]),0,16).arg((uint)(mac[5]),0,16)).toUpper();
}

/**
 * 功能：
 *          MAC字符串转字节数组
 * 参数：
 *          1、quint8 *mac: mac字节数组
 *          2、QString macStr：mac字符串
 *   返回值：
 *          bool： 成功返回true
 */
bool CfgChannel::MACFromString(quint8 *mac, QString macStr)
{
    Q_ASSERT(mac);
    QStringList sl = macStr.split(":-");
    if(sl.length() != 6){
        return false;
    }

    for(int i=0; i<6; ++i){
        mac[i] = (quint8)(sl[i].toUInt(0,16));
    }
    return true;
}

/**
 * 功能：
 *          获取显示分组的组名
 * 参数：
 *          １、quint32 grpIndex 组索引
 * 返回值：
 *          QVariant：返回指定显示分组的组名
 */
QVariant CfgChannel::getGroupName(quint32 grpIndex)
{
    if(grpIndex < GROUP_NUM_MAX){
        return QString((const char *)DevConfigPtr->displayGroup[grpIndex].groupName);
    }else{
        return QVariant();
    }
}

/**
 * 功能：
 *          根据组索引返回对应组报警状态
 *  *参数：
 *          １、quint32 grpIndex 组索引, 0-49
 * 返回值：
 *          QVariant：组内通道发生报警时返回优先级最高的状态值，否则返回无效值。
 */
QVariant CfgChannel::getGroupAlarmStatus(quint32 grpIndex)
{
    QVariant grpAlarmStatus;
    quint32 value = SQL_Alarm_Status_Hold_Release_Ack;
    if (grpIndex<m_list_Group.contains(grpIndex)){
        for (quint32 i=0; i<DevConfigPtr->displayGroup[grpIndex].groupChannelNum && i<GROUP_CHANNEL_NUM_MAX; ++i){
            quint32 chanNum = DevConfigPtr->displayGroup[grpIndex].groupChannelItem[i];
            QVariant chanAlarmStatue = getChannelAlarmStatus(chanNum);

            if(chanAlarmStatue.isValid() && chanAlarmStatue.toUInt() <= value){
                grpAlarmStatus = chanAlarmStatue;
                value = chanAlarmStatue.toUInt();
                if(value == SQL_Alarm_Status_NonHold_Occur || value==SQL_Alarm_Status_Hold_Occur_NoAck){
                    break;
                }
            }
        }
    }

    return grpAlarmStatus;
}

/*获取系统日期格式*/
QString CfgChannel::getSysDateFormat()
{
    QString fmt = "%1%4%2%4%3";
    QString mm = "MM";
    QString deli = "/";
    QString ret = "";
    if(DevConfigPtr->systemConfig.monthIndicator == Month_Indicator_Character){
        mm = "MMM";
    }

    if(DevConfigPtr->systemConfig.delimiterType == Delimiter_Type_Point){
        deli = ".";
    }else if(DevConfigPtr->systemConfig.delimiterType == Delimiter_Type_Hyphen){
        deli = "-";
    }

    if(DevConfigPtr->systemConfig.dateFormat == Date_Format_YMD){
        ret = fmt.arg("yyyy",mm,"dd",deli);
    }else if(DevConfigPtr->systemConfig.dateFormat == Date_Format_MDY){
        ret = fmt.arg(mm, "yyyy","dd",deli);
    }else{
        ret = fmt.arg("dd",mm, "yyyy",deli);
    }

    return ret;
}

/**
 * 功能：
 *          获取系统报警状态
 * 返回值：
 *          QVariant：通道发生报警时返回优先级最高的状态值，否则返回无效值。
 */
QVariant CfgChannel::getSysAlarmStatus()
{
    QVariant sysAlarmStatus;
    quint32 value = SQL_Alarm_Status_Hold_Release_Ack;
    foreach (quint32 grpIndex, m_list_Group) {
        QVariant grpAlarmStatus = getGroupAlarmStatus(grpIndex);
        if(grpAlarmStatus.isValid() && value >= grpAlarmStatus.toUInt()){
            value = grpAlarmStatus.toUInt();
            sysAlarmStatus = grpAlarmStatus;
            if(value == SQL_Alarm_Status_NonHold_Occur || value == SQL_Alarm_Status_Hold_Occur_NoAck){
                break;
            }
        }
    }

    return sysAlarmStatus;
}

/**
 * 功能：
 *          获取系统解除单个报警配置
 * 返回值：
 *          bool 解除单个报警配置true/false:是/否。
 */
bool CfgChannel::getSysIndividualAlarmAckConfig()
{
    return DevConfigPtr->systemConfig.individualAlarmAck;
}

/**
 * 功能：
 *          获取系统报警保持或非保持配置
 * 返回值：
 *          bool 报警保持或非保持配置true/false:保持/非保持。
 */
bool CfgChannel::getSysAlarmHoldConfig()
{
    return DevConfigPtr->systemConfig.alramIndicator;
}

/**
 * 功能：
 *          获取系统界面更改报警配置
 * 返回值：
 *          bool 界面更改报警配置，false-off, true-on
 */
bool CfgChannel::getSysAlarmChangeConfig()
{
    return DevConfigPtr->displayBasic.basicChangeFromMonitor != 0;
}

/**
 * 功能：
 *          获取系统界面背景颜色
 * 返回值：
 *          bool 界面更改报警配置，0-白色，1-黑色，默认：0
 */
quint8 CfgChannel::getSysBackgroundColor()
{
    return DevConfigPtr->displayBasic.basicBackground;
}

/**
 * 功能：
 *          获取系统显示组自动切换时间
 * 返回值：
 *          quint8 显示组自动切换时间
 */
quint8 CfgChannel::getSysAutochangeTime()
{
    return DevConfigPtr->displayBasic.basicGroupChangeTime;
}

/**
 * 功能：
 *          获取棒图显示方向
 * 返回值：
 *          quint8 棒图显示方向，0-横，1-纵，默认1
 */
quint8 CfgChannel::getSysDispalyBarDirection()
{
    return DevConfigPtr->displayBasic.basicBarDirection;
}

/**
 * 功能：
 *          获取批量设置开关
 * 返回值：
 *          quint8 批量设置开关状态，0-off，1-on，默认0
 */
quint8 CfgChannel::getSysBatchSettingOnOff()
{
    return DevConfigPtr->batchConfig.batchOnOff;
}

/**
 * 功能：
 *          获取记录设置确认屏开关
 * 返回值：
 *          quint8 记录设置确认屏开关，0-off，1-on，默认0
 */
quint8 CfgChannel::getSysRecordConfirmScreen()
{
    return DevConfigPtr->recordConfirm;
}

quint8 CfgChannel::getFlagData(quint8 index)
{
    Q_ASSERT(index>0 && index<=INTER_FLAG_COUNT);
    return EventAction::instance()->getInterFlag(index-1);
}

/*获取常量值K*/
qreal CfgChannel::getConstData(quint32 index)
{
    Q_ASSERT(index>0 && index<=100);
    return DevConfigPtr->mathConstK[index-1];
}
/*根据常量索引值，获取对应的内部开关值*/
qint16 CfgChannel::getSWData(quint32 index)
{
    qint16 ret = 0;
    MODBUS_REGISTER_MAP *data;

    Q_ASSERT(index>0 && index<=100);

    if(DevShareMem.isAttached()) {
        DevShareMem.lock();
        data = static_cast<MODBUS_REGISTER_MAP*>(DevShareMem.data());
        ret = data->holdReg_Device_Switch_Data_Int16[index];
        DevShareMem.unlock();
    }

    return ret;
}

/*获取安全设置终端用户自动登陆属性开关*/
quint8 CfgChannel::getSecurityAutoLogoutOnOff()
{
    return DevConfigPtr->securityConfig.securityBasic.securityLogout;
}

/*获取安全设置终端用户触摸操作属性开关*/
quint8 CfgChannel::getSecurityTouchOperation()
{
    return DevConfigPtr->securityConfig.securityBasic.securityTouchOpe;
}

/*获取安全设置终端用户非登陆操作属性开关*/
quint8 CfgChannel::getSecurityQuitOpeOnOff()
{
    return DevConfigPtr->securityConfig.securityBasic.securityQuitOpe;
}

/*设置安全设置终端用户的用户登陆密码*/
void CfgChannel::setSecurityUserPassword(quint8 userId,QString passwd)
{
    if(userId < SECURITY_MAX_USERGROUP)
        qstrcpy((char *)(DevConfigPtr->securityConfig.securityUserGrp[userId].securityUserPwd),passwd.toLocal8Bit().data());
}

void CfgChannel::setSecurityUserLevel(quint8 userId,quint8 userLevel)
{
    if(userId < SECURITY_MAX_USERGROUP)
        DevConfigPtr->securityConfig.securityUserGrp[userId].securityUserLvl = (Security_UserLvl)userLevel;
}

/*获取安全设置终端用户查找用户ID*/
QVariant CfgChannel::findSecurityUserIdentification(QString userName)
{
    for(quint8 i=0; i < SECURITY_MAX_USERGROUP; i++){
        if(qstrcmp(userName.toLocal8Bit(), (const char *)(DevConfigPtr->securityConfig.securityUserGrp[i].securityUserName)) == 0){
            return i;
        }
    }
    return QVariant();
}

/*获取安全设置终端用户的用户名*/
QVariant CfgChannel::getSecurityUserName(quint8 userId)
{
    if(userId < SECURITY_MAX_USERGROUP)
        return QString((const char *)(DevConfigPtr->securityConfig.securityUserGrp[userId].securityUserName));
    return QVariant();
}

/*获取安全设置终端用户的用户登陆密码*/
QVariant CfgChannel::getSecurityUserPassword(quint8 userId)
{
    if(userId < SECURITY_MAX_USERGROUP)
        return QString((const char *)(DevConfigPtr->securityConfig.securityUserGrp[userId].securityUserPwd));
    return QVariant();
}

/*获取安全设置终端用户的用户级别*/
QVariant CfgChannel::getSecurityUserLevel(quint8 userId)
{
    if(userId < SECURITY_MAX_USERGROUP)
        return DevConfigPtr->securityConfig.securityUserGrp[userId].securityUserLvl;
    return QVariant();
}

/*获取安全设置终端用户的用户工作模式*/
QVariant CfgChannel::getSecurityUserMode(quint8 userId)
{
    if(userId < SECURITY_MAX_USERGROUP)
        return DevConfigPtr->securityConfig.securityUserGrp[userId].securityUserMode;
    return QVariant();
}

/*获取安全设置终端用户的用户权限属性开关*/
QVariant CfgChannel::getSecurityUserPropertyOnOff(quint8 userId)
{
    if(userId < SECURITY_MAX_USERGROUP)
        return DevConfigPtr->securityConfig.securityUserGrp[userId].securityUserProperty;
    return QVariant();
}

/*获取安全设置终端用户的用户权限ID*/
QVariant CfgChannel::getSecurityUserPropertyNum(quint8 userId)
{
    if(userId < SECURITY_MAX_USERGROUP)
        return DevConfigPtr->securityConfig.securityUserGrp[userId].securityUserNum;
    return QVariant();
}

/*获取安全设置终端用户的模块权限属性*/
QVariant CfgChannel::getSecurityModuleProperty(quint8 propNum, quint8 moduleId)
{
    if(propNum < SECURITY_MAX_MODULEGROUP)
        return Read_Module_Auth(DevConfigPtr->securityConfig.securityProperty[propNum],moduleId);
    return QVariant();
}

AI_CONFIG * CfgChannel::getAIConfig(quint32 chanNum)
{
    if (m_obj_AI.contains(chanNum))
        return (AI_CONFIG *)m_obj_AI[chanNum]->getChannelConfig();
    else
        return NULL;
}

DI_CONFIG * CfgChannel::getDIConfig(quint32 chanNum)
{
    if (m_obj_DI.contains(chanNum))
        return (DI_CONFIG *)m_obj_DI[chanNum]->getChannelConfig();
    else
        return NULL;
}

DO_CONFIG * CfgChannel::getDOConfig(quint32 chanNum)
{
    if (m_obj_DO.contains(chanNum))
        return (DO_CONFIG *)m_obj_DO[chanNum]->getChannelConfig();
    else
        return NULL;
}

MATH_CONFIG * CfgChannel::getMathConfig(quint32 chanNum)
{
    if (m_obj_MATH.contains(chanNum))
        return (MATH_CONFIG *)m_obj_MATH[chanNum]->getChannelConfig();
    else
        return NULL;
}

COMM_CONFIG * CfgChannel::getCommConfig(quint32 chanNum)
{
    if (m_obj_COMM.contains(chanNum))
        return (COMM_CONFIG *)m_obj_COMM[chanNum]->getChannelConfig();
    else
        return NULL;
}

Channel * CfgChannel::getChannelObj(quint32 chanCoder) const
{
    Channel *pChannel = NULL;
    switch (CHANNEL_TYPE(chanCoder)) {
    case Channel_Type_AI:
        if (m_obj_AI.contains(chanCoder))
            pChannel = m_obj_AI[chanCoder];
        else
            pChannel = NULL;
        break;
    case Channel_Type_DI:
        if (m_obj_DI.contains(chanCoder))
            pChannel = m_obj_DI[chanCoder];
        else
            pChannel = NULL;
        break;
    case Channel_Type_DO:
        if (m_obj_DO.contains(chanCoder))
            pChannel = m_obj_DO[chanCoder];
        else
            pChannel = NULL;
        break;
    case Channel_Type_Comm:
        if (m_obj_COMM.contains(chanCoder))
            pChannel = m_obj_COMM[chanCoder];
        else
            pChannel = NULL;
        break;
    case Channel_Type_Math:
        if (m_obj_MATH.contains(chanCoder))
            pChannel = m_obj_MATH[chanCoder];
        else
            pChannel = NULL;
        break;
    default:
        break;
    }

    return pChannel;
}

/*获取扩展单元从机地址*/
int CfgChannel::getExternBoardSlaveAddr() const
{
    for (int i=0; i<MAIN_UNIT_MODULE_NUM; ++i) {
        quint16 boardStatus = DevConfigPtr->sysStatus.mainUnit_ModuleStatus[i];
        if (STATUS_BOARD_TYPE(boardStatus) == STATUS_BOARD_TYPE_EXT)
            return i+1;
    }

    return -1;
}

/**
 *  TODO, signal & slot connect
 * @brief CfgChannel::slotChannelConfigChanged
 *      通道配置变更槽函数，用于刷新当前系统的有效通道链表m_list_Chan
 * 调用前提：
 *      1.IO板卡热插拔时
 *      2.IO板卡通道使能/关闭时
 *      3.Math通道打开/关闭时
 *      4.Comm通道打开/关闭时
 */
void CfgChannel::slotChannelConfigChanged()
{
    m_list_Chan.clear();
    m_list_ChanAll.clear();

    /********************遍历AI通道**********************/
    foreach (Channel *pObj, m_obj_AI.values()) {
        m_list_ChanAll.append(pObj->getChannelCoder());

        AI_CONFIG *pConfig = (AI_CONFIG *)pObj->getChannelConfig();
        if (pConfig->rangeConfig.rangeType != AI_RANGE_Type_Disabled) {
            m_list_Chan.append(pObj->getChannelCoder());
        }
    }

    /********************遍历DI通道**********************/
    foreach (Channel *pObj, m_obj_DI.values()) {
        m_list_ChanAll.append(pObj->getChannelCoder());

        DI_CONFIG *pConfig = (DI_CONFIG *)pObj->getChannelConfig();
        if (pConfig->rangeConfig.type != DI_Range_Type_Disabled) {
            m_list_Chan.append(pObj->getChannelCoder());
        }
    }

    /********************遍历运算通道**********************/
    foreach (Channel *pObj, m_obj_MATH.values()) {
        m_list_ChanAll.append(pObj->getChannelCoder());

        MATH_CONFIG *pConfig = (MATH_CONFIG *)pObj->getChannelConfig();
        if (pConfig->expressConfig.expSwtich) {
            m_list_Chan.append(pObj->getChannelCoder());
        }
    }

    /**********************遍历通信通道**********************/
    foreach (Channel *pObj, m_obj_COMM.values()) {
        m_list_ChanAll.append(pObj->getChannelCoder());

        COMM_CONFIG *pConfig = (COMM_CONFIG *)pObj->getChannelConfig();
        if (pConfig->rangeConfig.rangeSwtich) {
            m_list_Chan.append(pObj->getChannelCoder());
        }
    }

    /********************遍历DO通道**********************/
    foreach (Channel *pObj, m_obj_DO.values()) {
        m_list_ChanAll.append(pObj->getChannelCoder());

        DO_CONFIG *pConfig = (DO_CONFIG *)pObj->getChannelConfig();
        if(pConfig->rangeConfig.rangeType != 1) {
            m_list_Chan.append(pObj->getChannelCoder());
        }
    }
}

/**
 * @brief CfgChannel::slotGroupConfigChanged
 *      组配置变更处理槽函数
 * 调用前提：
 *      1.组配置开关on/off
 */
void CfgChannel::slotGroupConfigChanged()
{
    m_list_Group.clear();

    foreach (DevGroup *pGroup, m_list_GroupObj) {
        if (pGroup->getGroupOnOff()) {
            m_list_Group.append(pGroup->getGroupIndex());
        }
    }
}

/**
 * @brief CfgChannel::slotMathExpAnalysisChanged
 *          运算通道配置变更处理槽函数
 * 调用前提：
 *          1.运算通道公式变更
 */
void CfgChannel::slotMathExpAnalysisChanged()
{
    foreach(DevBoard *pBoard, m_list_BoardVirutal) {
        if (pBoard->getBoardType() == STATUS_BOARD_TYPE_MATH) {
            foreach (Channel *pChan, pBoard->getBoardChannel()) {
                qobject_cast<DevChannelMath *>(pChan)->slotChanConfigChanged();
            }
        }
    }
}

/**
 * @brief CfgChannel::slotSWConfigChanged
 *          内部开关配置变更处理槽函数
 * 调用前提：
 *          1. 任意内部开关配置变更时
 *          2. AI/DI/MATH/COMM通道报警开关/输出类型变更时
 */
void CfgChannel::slotSWConfigChanged()
{
    m_list_Alarm_SW.clear();
    m_list_Ope_SW.clear();
    m_chanMap_Alarm_SW.clear();

    //内部开关类型分类
    for(quint8 index=0; index<INTERNAL_SWITCH_NUMBER; ++index) {
        if (InterSwitch_Mode_Alarm == \
            DevConfigPtr->systemConfig.interSwitchMode[index]) {
            m_list_Alarm_SW.append(index);
        } else {
            m_list_Ope_SW.append(index);
        }
    }

    //IO、MATH、COMM各通道报警输出->内部开关遍历
    foreach (quint32 coder, m_map_AI.values()) {
        AI_CONFIG *pConfig;
        pConfig = getAIConfig(coder);
        if (pConfig) {
            for (int i=0; i<4; ++i) {
                if (pConfig->alarmConfig.alarmOnOff[i] && \
                    pConfig->alarmConfig.alarmOutputType[i] == 2) {
                    quint32 index = pConfig->alarmConfig.alarmOuputNo[i] - 1;   //内部开关索引
                    QMap<quint32, quint8> map = m_chanMap_Alarm_SW[index];
                    map[coder] |= 0x01<<i;
                    m_chanMap_Alarm_SW[index] = map;
                    qDebug("coder:0x%x, map:0x%d, SWIndex:%d, alarm:%d", coder, map[coder], index, i);
                }
            }
        }
    }

    foreach (quint32 coder, m_map_DI.values()) {
        DI_CONFIG *pConfig;
        pConfig = getDIConfig(coder);
        if (pConfig) {
            for (int i=0; i<4; ++i) {
                if (pConfig->alarmConfig.alarmOnOff[i] && \
                     pConfig->alarmConfig.alarmOutputType[i] == 2) {
                    quint32 index = pConfig->alarmConfig.alarmOuputNo[i] - 1;   //内部开关索引
                    QMap<quint32, quint8> map = m_chanMap_Alarm_SW[index];  //通道编码 + 电平号
                    map[coder] |= 0x01<<i;
                    m_chanMap_Alarm_SW[index] = map;
                }
            }
        }
    }

    foreach (quint32 coder, m_map_MATH.values()) {
        MATH_CONFIG *pConfig;
        pConfig = getMathConfig(coder);
        if (pConfig) {
            for (int i=0; i<4; ++i) {
                if (pConfig->alarmConfig.alarmOnOff[i] && \
                     pConfig->alarmConfig.alarmOutputType[i] == 2) {
                    quint32 index = pConfig->alarmConfig.alarmOuputNo[i] - 1;   //内部开关索引
                    QMap<quint32, quint8> map = m_chanMap_Alarm_SW[index];  //通道编码 + 电平号
                    map[coder] |= 0x01<<i;
                    m_chanMap_Alarm_SW[index] = map;
                }
            }
        }
    }

    foreach (quint32 coder, m_map_COMM.values()) {
        COMM_CONFIG *pConfig;
        pConfig = getCommConfig(coder);
        if (pConfig) {
            for (int i=0; i<4; ++i) {
                if (pConfig->alarmConfig.alarmOnOff[i] && \
                     pConfig->alarmConfig.alarmOutputType[i] == 2) {
                    quint32 index = pConfig->alarmConfig.alarmOuputNo[i] - 1;   //内部开关索引
                    QMap<quint32, quint8> map = m_chanMap_Alarm_SW[index];  //通道编码 + 电平号
                    map[coder] |= 0x01<<i;
                    m_chanMap_Alarm_SW[index] = map;
                }
            }
        }
    }
}

/**
 * @brief CfgChannel::slotDOConfigChanged
 *          DO配置变更处理槽函数
 * 调用前提：
 *          1. 任意DO配置变更时
 *          2. AI/DI/MATH/COMM通道报警开关/输出类型变更时
 */
void CfgChannel::slotDOConfigChanged()
{
    m_list_Alarm_DO.clear();
    m_list_Ope_DO.clear();
    m_chanMap_Alarm_DO.clear();
    DO_CONFIG *pConfig;

    //DO通道分类
    foreach (quint32 coder, m_map_DO.values()) {
        pConfig = getDOConfig(coder);
        if (pConfig) {
            if (pConfig->rangeConfig.rangeType == 0) {  //报警
                m_list_Alarm_DO.append(coder);
            } else {    //手动
                m_list_Ope_DO.append(coder);
            }
        }
    }

    //IO、MATH、COMM各通道报警输出->DO输出遍历
    foreach (quint32 coder, m_map_AI.values()) {
        AI_CONFIG *pConfig;
        pConfig = getAIConfig(coder);
        if (pConfig) {
            for (int i=0; i<4; ++i) {
                if (pConfig->alarmConfig.alarmOnOff[i] && \
                    pConfig->alarmConfig.alarmOutputType[i] == 1) {
                    quint32 index = pConfig->alarmConfig.alarmOuputNo[i];   //继电器通道编码
                    QMap<quint32, quint8> map = m_chanMap_Alarm_DO[index];
                    map[coder] |= 0x01<<i;
                    m_chanMap_Alarm_DO[index] = map;
                }
            }
        }
    }

    foreach (quint32 coder, m_map_DI.values()) {
        DI_CONFIG *pConfig;
        pConfig = getDIConfig(coder);
        if (pConfig) {
            for (int i=0; i<4; ++i) {
                if (pConfig->alarmConfig.alarmOnOff[i] && \
                     pConfig->alarmConfig.alarmOutputType[i] == 2) {
                    quint32 index = pConfig->alarmConfig.alarmOuputNo[i] - 1;   //继电器通道编码
                    QMap<quint32, quint8> map = m_chanMap_Alarm_DO[index];  //通道编码 + 电平号
                    map[coder] |= 0x01<<i;
                    m_chanMap_Alarm_DO[index] = map;
                }
            }
        }
    }

    foreach (quint32 coder, m_map_MATH.values()) {
        MATH_CONFIG *pConfig;
        pConfig = getMathConfig(coder);
        if (pConfig) {
            for (int i=0; i<4; ++i) {
                if (pConfig->alarmConfig.alarmOnOff[i] && \
                     pConfig->alarmConfig.alarmOutputType[i] == 2) {
                    quint32 index = pConfig->alarmConfig.alarmOuputNo[i] - 1;   //继电器通道编码
                    QMap<quint32, quint8> map = m_chanMap_Alarm_DO[index];  //通道编码 + 电平号
                    map[coder] |= 0x01<<i;
                    m_chanMap_Alarm_DO[index] = map;
                }
            }
        }
    }

    foreach (quint32 coder, m_map_COMM.values()) {
        COMM_CONFIG *pConfig;
        pConfig = getCommConfig(coder);
        if (pConfig) {
            for (int i=0; i<4; ++i) {
                if (pConfig->alarmConfig.alarmOnOff[i] && \
                     pConfig->alarmConfig.alarmOutputType[i] == 2) {
                    quint32 index = pConfig->alarmConfig.alarmOuputNo[i] - 1;   //继电器通道编码
                    QMap<quint32, quint8> map = m_chanMap_Alarm_DO[index];  //通道编码 + 电平号
                    map[coder] |= 0x01<<i;
                    m_chanMap_Alarm_DO[index] = map;
                }
            }
        }
    }
}

/**
 * @brief CfgChannel::slotBoardPluginout
 *              板卡热插拔处理
 * @param module
 *              对应卡槽号：0-9
 * @param in
 *              插入/拔出：true--插入， false--拔出
 */
void CfgChannel::slotBoardPluginout(quint8 module, bool in)
{
    quint16 boardStatus = CfgConfig::instance()->getBoardStatus(0, module);

    if (in) {   //板卡插入
        if (STATUS_BOARD_TYPE_EXT == STATUS_BOARD_TYPE(boardStatus)) {
            /*扩展板卡插入,创建各扩展单元IO板卡对象*/
            for (int i=0; i<EXT_UNIT_NUM; ++i) {
                for (int j=0; j<EXT_UNIT_MODULE_NUM; ++j) {
                    boardStatus = CfgConfig::instance()->getBoardStatus(i+1, j);
                    if (STATUS_BOARD_TYPE(boardStatus) > STATUS_BOARD_TYPE_NONE &&\
                        STATUS_BOARD_TYPE(boardStatus) < STATUS_BOARD_TYPE_EXT && \
                        STATUS_BOARD_STATUS(boardStatus)) {
                        newBoard(i+1, j, boardStatus);
                    }
                }
            }
        } else {
            /*IO板卡插入*/
            newBoard(0, module, boardStatus);
        }
    } else {  //板卡拔出
        if (STATUS_BOARD_TYPE_EXT == STATUS_BOARD_TYPE(boardStatus)) {
            /*扩展板卡拔出*/
            for (int i=0; i<EXT_UNIT_NUM; ++i) {
                for (int j=0; j<EXT_UNIT_MODULE_NUM; ++j) {
                    boardStatus = CfgConfig::instance()->getBoardStatus(i+1, j);
                    if (STATUS_BOARD_TYPE(boardStatus) > STATUS_BOARD_TYPE_NONE &&\
                        STATUS_BOARD_TYPE(boardStatus) < STATUS_BOARD_TYPE_EXT && \
                        STATUS_BOARD_STATUS(boardStatus)) {
                        delBoard(i+1, j, boardStatus);
                    }
                }
            }
        } else {
            /*IO板卡拔出*/
            delBoard(0, module, boardStatus);
        }
    }

    //因配置树的创建与销毁依赖于m_list_chan，所以m_list_chan必须先生后灭
    if(in) {    //板卡插入，先刷新当前系统的有效通道链表m_list_Chan，再发信号构建配置树
        slotChannelConfigChanged();
        emit sigBasicChannelChanged(module, in);
    } else {    //板卡拔出，先发信号销毁配置树，再刷新当前系统的有效通道链表m_list_Chan
        emit sigBasicChannelChanged(module, in);
        slotChannelConfigChanged();
    }
}

void CfgChannel::newBoard(quint8 unit, quint8 module, quint16 status)
{
    qDebug("%s---0x%x",__func__, status);
    Q_ASSERT(STATUS_BOARD_TYPE(status) != STATUS_BOARD_TYPE_NONE &&\
                     STATUS_BOARD_TYPE(status) != STATUS_BOARD_TYPE_EXT && \
                     STATUS_BOARD_STATUS(status));

    DevBoard *pBoard = new DevBoard(unit, module, status, this);
    if (pBoard) {
        if (STATUS_BOARD_TYPE(status) == STATUS_BOARD_TYPE_AI ||
            STATUS_BOARD_TYPE(status) == STATUS_BOARD_TYPE_DI ||
            STATUS_BOARD_TYPE(status) == STATUS_BOARD_TYPE_DO ||
            STATUS_BOARD_TYPE(status) == STATUS_BOARD_TYPE_DIO) {
            m_list_BoardIO.append(pBoard);
        } else if (STATUS_BOARD_TYPE(status) == STATUS_BOARD_TYPE_COMM ||
                    STATUS_BOARD_TYPE(status) == STATUS_BOARD_TYPE_MATH) {
            m_list_BoardVirutal.append(pBoard);
        }
    } else {
        qCritical("MemError--Board object create faile\n");
    }
}

void CfgChannel::delBoard(quint8 unit, quint8 module, quint16 status)
{
    Q_ASSERT(STATUS_BOARD_TYPE(status) > STATUS_BOARD_TYPE_NONE &&\
                     STATUS_BOARD_TYPE(status) < STATUS_BOARD_TYPE_EXT && \
                     STATUS_BOARD_STATUS(status));

    for (int i=0; i<m_list_BoardIO.size(); ++i) {
        DevBoard *pBoard = m_list_BoardIO.at(i);
        if (pBoard->getBoardStatus() == status &&
            pBoard->getBoardUnit() == unit &&
            pBoard->getBoardModule() == module) {

            delete m_list_BoardIO.takeAt(i);
            return;
        }
    }
}

void CfgChannel::initGroup()
{
    m_list_GroupObj.clear();
    for (int i=0; i<GROUP_NUM_MAX; ++i) {
        DevGroup *pGroup = new DevGroup(i, this);
        if (pGroup) {
            m_list_GroupObj.append(pGroup);
        } else {
            qCritical("MemError--Group object create faile\n");
        }
    }
}

void CfgChannel::init()
{
    static bool initFlag = false;

    if (initFlag == false) {
        initFlag = true;
        ::memset(&DevConfigPtr->sysStatus, 0x00, sizeof(SYSTEM_STATUS));    //系统状态清除
        initBoardVirtual();
    }

    initBoardIO();
    initGroup();
    slotChannelConfigChanged();
    slotGroupConfigChanged();
}

void CfgChannel::reset()
{
    //通道信息清除
    m_map_AI.clear();
    m_obj_AI.clear();
    m_map_DI.clear();
    m_obj_DI.clear();
    m_map_DO.clear();
    m_obj_DO.clear();

    //子板对象链表清除
    foreach (DevBoard *pBoard, m_list_BoardIO) {
            delete pBoard;
    }
    m_list_BoardIO.clear();

    //组对象链表清除
    foreach (DevGroup *pGroup, m_list_GroupObj) {
        delete pGroup;
    }
    m_list_GroupObj.clear();

    //初始化
    init();
}

/**
 * @brief CfgChannel::initBoard
 *  根据当前系统状态，构建相应的板卡及通道对象
 */
void CfgChannel::initBoardIO()
{
    quint16 boardStatus;
    for (int i=0; i<MAIN_UNIT_MODULE_NUM; ++i) {
        boardStatus =  CfgConfig::instance()->getBoardStatus(0, i);
        //板卡对象构建
        if (STATUS_BOARD_TYPE(boardStatus) != STATUS_BOARD_TYPE_NONE &&\
            STATUS_BOARD_TYPE(boardStatus) != STATUS_BOARD_TYPE_EXT && \
            STATUS_BOARD_STATUS(boardStatus)) {
            newBoard(0, i, boardStatus);
        }
    }

    for (int i=0; i<EXT_UNIT_NUM; ++i) {
        for (int j=0; j<EXT_UNIT_MODULE_NUM; ++j) {
            boardStatus = CfgConfig::instance()->getBoardStatus(i+1, j);
            //板卡对象构建
            if (STATUS_BOARD_TYPE(boardStatus) != STATUS_BOARD_TYPE_NONE &&\
                STATUS_BOARD_TYPE(boardStatus) != STATUS_BOARD_TYPE_EXT && \
                STATUS_BOARD_STATUS(boardStatus)) {
                newBoard(i+1, j, boardStatus);
            }
        }
    }
}

/**
 * @brief CfgChannel::registerChannelObj
 *      通道注册
 * @param pChannel  ：通道对象指针
 * @param type  ： 通道类型
 */
void CfgChannel::registerChannelObj(Channel *pChannel, Channel_Type type)
{
    quint32 chanCoder = pChannel->getChannelCoder();
    QString defCoder = pChannel->getChannelDefCoder();

    switch (type) {
    case Channel_Type_AI:
        m_map_AI[defCoder] = chanCoder;
        m_obj_AI[chanCoder] = pChannel;
        break;
    case Channel_Type_DI:
        m_map_DI[defCoder] = chanCoder;
        m_obj_DI[chanCoder] = pChannel;
        break;
    case Channel_Type_DO:
        m_map_DO[defCoder] = chanCoder;
        m_obj_DO[chanCoder] = pChannel;
        break;
    case Channel_Type_Comm:
        m_map_COMM[defCoder] = chanCoder;
        m_obj_COMM[chanCoder] = pChannel;
        break;
    case Channel_Type_Math:
        m_map_MATH[defCoder] = chanCoder;
        m_obj_MATH[chanCoder] = pChannel;
        break;
    default:
        break;
    }
}

/**
 * @brief CfgChannel::unRegisterChannelObj
 *      通道注销
 * @param pChannel  ：通道对象指针
 * @param type  ： 通道类型
 */
void CfgChannel::unRegisterChannelObj(Channel *pChannel, Channel_Type type)
{
    quint32 chanCoder = pChannel->getChannelCoder();
    QString defCoder = pChannel->getChannelDefCoder();

    switch (type) {
    case Channel_Type_AI:
        m_map_AI.take(defCoder);
        m_obj_AI.take(chanCoder);
        break;
    case Channel_Type_DI:
        m_map_DI.take(defCoder);
        m_obj_DI.take(chanCoder);
        break;
    case Channel_Type_DO:
        m_map_DO.take(defCoder);
        m_obj_DO.take(chanCoder);
        break;
    case Channel_Type_Comm:
        m_map_COMM.take(defCoder);
        m_obj_COMM.take(chanCoder);
        break;
    case Channel_Type_Math:
        m_map_MATH.take(defCoder);
        m_obj_MATH.take(chanCoder);
        break;
    default:
        break;
    }
}

/**
 * 功能：
 *          根据通道默认显示字符串(如0001, 0010, 1001, 1010, A001, A300....)，获取相应的通道编码
 *          如果对应的通道不存在，则返回false
 * 参数：
 *          1. QString chan: 通道显示字符串
 *          2. quint32 &chanCoder:对应通道编码
 * 返回值：
 *          成功返回true,失败返回false
 */
bool CfgChannel::getChannelCoder(QString chan, quint32 &chanCoder)
{
    if (chan.isEmpty() || chan.count() != 4) return false;

    if (chan.at(0)>='0' &&  chan.at(0)<='6') { //输入输出通道
        if (m_map_AI.contains(chan)){
            chanCoder = m_map_AI.value(chan);
            return true;
        } else if (m_map_DI.contains(chan)) {
            chanCoder = m_map_DI.value(chan);
            return true;
        } else if (m_map_DO.contains(chan)) {
            chanCoder = m_map_DO.value(chan);
            return true;
        } else {
            return false;
        }
    } else if ( chan.at(0) == 'A') {  //运算通道
        if (m_map_MATH.contains(chan)) {
            chanCoder = m_map_MATH.value(chan);
            return true;
        } else {
            return false;
        }
    } else if ( chan.at(0) == 'C') {  //通信通道
        if (m_map_COMM.contains(chan)) {
            chanCoder = m_map_COMM.value(chan);
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }

}

/**
 * 功能：
 *          根据通道编码，获取通道默认显示编码字符串，如0101, 1001, 2010...
 * 参数：
 *          quint32 : 通道编码
 *          1. unit   : 单元号, 0-本机，1-6：扩展单元， 7-运算， 8-通信
 *          2. module : 模块号
 *          3. number : 通道号
 * 返回值：
 *          QString : 通道默认显示编码
 */
QString CfgChannel::getChannelStr(quint32 chanCoder)
{
    quint16 type = CHANNEL_TYPE(chanCoder);             //通道类型
    quint16 unit = CHANNEL_UNIT(chanCoder);          //单元号
    quint16 module = CHANNEL_MODULE(chanCoder);    //卡槽号
    quint16 number = CHANNEL_NUM(chanCoder);            //通道号

    switch((Channel_Type)type) {
    case Channel_Type_AI:
    case Channel_Type_DI:
    case Channel_Type_DO:
        break;
    case Channel_Type_Comm:
        unit = 8;
        break;
    case Channel_Type_Math:
        unit = 7;
        break;
    default:
        return QString();
        break;
    }

    if (unit > 8 || module>9) return QString();

    QString chanNum, chanStr;
    chanNum = QString("%1").arg(number);

    switch (unit) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
        if (chanNum.count()<2)
            chanNum.prepend(QString(2-chanNum.count(), '0'));
        chanStr = QString("%1%2").arg(unit).arg(module) + chanNum;
        break;
    case 7:
        if (chanNum.count()<3)
            chanNum.prepend(QString(3-chanNum.count(), '0'));
        chanStr = "A" + chanNum;
        break;
    case 8:
        if (chanNum.count()<3)
            chanNum.prepend(QString(3-chanNum.count(), '0'));
        chanStr = "C" + chanNum;
        break;
    default:
        break;
    }

    return chanStr;
}

/**
 * 功能：
 *          根据通道编码chanCoder获取对应通道的配置指针
 * 参数：
 *          quint32 chanCoder:通道编码
 * 返回值：
 *          void *通道配置指针
 */
void *CfgChannel::getChannelConfig(quint32 chanCoder)
{
    void * pAddr = NULL;
    quint16 chanType, uninNum, moduleIndex, number;
    chanType = CHANNEL_TYPE(chanCoder);           //通道类型
    uninNum = CHANNEL_UNIT(chanCoder);           //单元号
    moduleIndex = CHANNEL_MODULE(chanCoder);  //卡槽号
    number = CHANNEL_NUM(chanCoder) - 1;              //通道号

    switch (chanType) {
    case Channel_Type_AI:
        if (uninNum == CHANNEL_UNIN_MAIN) {
            Q_ASSERT(moduleIndex<MAIN_UNIT_MODULE_NUM && number<UNIT_CHAN_NUM_PRE);
            pAddr =(void *) &DevConfigPtr->mainUnitAI[moduleIndex][number];
        } else {
            Q_ASSERT(uninNum<=EXT_UNIT_NUM && moduleIndex<EXT_UNIT_MODULE_NUM && number<UNIT_CHAN_NUM_PRE);
            pAddr =(void *) &DevConfigPtr->extUnitAI[uninNum][moduleIndex][number];
        }
        break;
    case Channel_Type_DI:
        if (uninNum == CHANNEL_UNIN_MAIN) {
            Q_ASSERT(moduleIndex<MAIN_UNIT_MODULE_NUM && number<UNIT_CHAN_NUM_PRE);
            pAddr =(void *) &DevConfigPtr->mainUnitDI[moduleIndex][number];
        } else {
            Q_ASSERT(uninNum<=EXT_UNIT_NUM && moduleIndex<EXT_UNIT_MODULE_NUM && number<UNIT_CHAN_NUM_PRE);
            pAddr =(void *) &DevConfigPtr->extUnitDI[uninNum][moduleIndex][number];
        }
        break;
    case Channel_Type_DO:
        if (uninNum == CHANNEL_UNIN_MAIN) {
            Q_ASSERT(moduleIndex<MAIN_UNIT_MODULE_NUM && number<UNIT_CHAN_NUM_PRE);
            pAddr =(void *) &DevConfigPtr->mainUnitDO[moduleIndex][number];
        } else {
            Q_ASSERT(uninNum<=EXT_UNIT_NUM && moduleIndex<EXT_UNIT_MODULE_NUM && number<UNIT_CHAN_NUM_PRE);
            pAddr =(void *) &DevConfigPtr->extUnitDO[uninNum][moduleIndex][number];
        }
        break;
    case Channel_Type_Comm:
        Q_ASSERT(number<MAIN_UNIT_COMM_CHAN_NUM_PRE);
        pAddr =(void *) &DevConfigPtr->chanComm[number];
        break;
    case Channel_Type_Math:
        Q_ASSERT(number<MAIN_UNIT_MATH_CHAN_NUM_PRE);
        pAddr =(void *) &DevConfigPtr->chanMath[number];
        break;
    }

    return pAddr;
}


/**
 * @brief CfgChannel::initBoardVirtual
 *  虚拟板卡（Comm/Math）及通道对象构建
 */
void CfgChannel::initBoardVirtual()
{
    //Math, Comm虚拟板卡构建
    quint16 boardStatus = BOARD_STATUS_COMM(MAIN_UNIT_COMM_CHAN_NUM_PRE);
    CfgChannel::instance()->newBoard(0, 0, boardStatus);
    boardStatus = BOARD_STATUS_MATH(MAIN_UNIT_MATH_CHAN_NUM_PRE);
    CfgChannel::instance()->newBoard(0, 0, boardStatus);

}
