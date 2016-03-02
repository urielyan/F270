/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：Channel.h
 * 概   要：通道抽象类，提供基于通道的函数接口，以获取该通道的相关属性
 *
 * 当前版本：V1.0.0
 * 作   者：王 品
 * 完成日期：2015-10-20
 *
 * 修改版本：
 * 修改说明：
 * 修改作者：
 *
 ********************************************************************/
#ifndef CHANNEL_H
#define CHANNEL_H

#include <QObject>
#include <QVariant>

#include "GlobalData/cfgchannel.h"
#include "GlobalData/cfgGlobalDef_AI.h"
#include "GlobalData/cfgGlobalDef_DI.h"
#include "GlobalData/cfgGlobalDef_DO.h"
#include "GlobalData/cfgGlobalDef_Math.h"
#include "GlobalData/cfgGlobalDef_Comm.h"

#include "GlobalData/globalDef.h"
#include "GlobalData/dataGlobalDef_ModbusReg.h"

class Channel : public QObject
{
    Q_OBJECT
public:
    explicit Channel(quint32 chanCoder, QObject *parent = 0);

    Channel_Type getChannelType() const { return (Channel_Type)CHANNEL_TYPE(m_chanCoder);}       //获取通道类型
    quint8 getChannelUnitNum() const { return CHANNEL_UNIT(m_chanCoder);}                   //获取通道单元号
    quint8 getChannelModuleNum() const { return CHANNEL_MODULE(m_chanCoder);}       //获取通道模块号（卡槽号）
    quint8 getChannelIndex() const { return CHANNEL_NUM(m_chanCoder);}              //获取通道号
    quint32 getChannelCoder() const {return m_chanCoder;}                                             //获取通道编码


    virtual void chanCalculate() {}                                                                                              //通道计算
    virtual QVariant getChannelCurData() const {return QVariant();}                                  //获取通道本次测量值

    virtual QString getChannelDefCoder() const;                                                                               //获取通道默认编码
    virtual void * getChannelConfig() const = 0;                                                                                //获取通道配置指针

    /*数据模型接口*/
    virtual QVariant getChannelData() const = 0;                                                                               //获取通道数据
    virtual QVariant getChannelTag() const = 0;                                                                                 //获取通道标记
    virtual QVariant getChannelColor() const = 0;                                                                              //获取通道颜色
    virtual QVariant getChannelUnit() const = 0;                                                                                //获取通道单位
    virtual QVariant getChannelRangeType() const = 0;                                                                    //获取通道量程类型
    virtual QVariant getChannelRangeUpper() const = 0;                                                                  //获取通道量程实际上限
    virtual QVariant getChannelRangeDowner() const = 0;                                                               //获取通道量程实际下限
    virtual QVariant getChannelStatus() const = 0;
    virtual QVariant getChannelDataAccuracy() const = 0;

    virtual QVariant getChannelCaliCorrectNum() const = 0;

    virtual QVariant getChannelDispZoneUpper() const = 0;
    virtual QVariant getChannelDispZoneDowner() const = 0;

    virtual QVariant getChannelScalePos() const = 0;
    virtual QVariant getChannelScaleDiv() const = 0;

    virtual QVariant getChannelBarBase() const = 0;
    virtual QVariant getChannelBarDiv() const = 0;

    virtual QVariant getChannelPartScaleOnOff() const = 0;
    virtual QVariant getChannelPartScalePos() const = 0;
    virtual QVariant getChannelPartScaleBound() const = 0;

    virtual QVariant getChannelColorScalePos() const = 0;
    virtual QVariant getChannelColorScaleColor() const = 0;
    virtual QVariant getChannelColorScaleUpper() const = 0;
    virtual QVariant getChannelColorScaleDowner() const = 0;

    virtual QVariant getChannelAlarmOnOff() const = 0;
    virtual QVariant getChannelAlarmType() const = 0;
    virtual QVariant getChannelAlarm1OnOff() const = 0;
    virtual QVariant getChannelAlarm1Type() const = 0;
    virtual QVariant getChannelAlarm1Status() const = 0;
    virtual QVariant getChannelAlarm1Value() const = 0;
    virtual QVariant getChannelAlarm1Color() const = 0;
    virtual QVariant getChannelAlarm2OnOff() const = 0;
    virtual QVariant getChannelAlarm2Type() const = 0;
    virtual QVariant getChannelAlarm2Status() const = 0;
    virtual QVariant getChannelAlarm2Value() const = 0;
    virtual QVariant getChannelAlarm2Color() const = 0;
    virtual QVariant getChannelAlarm3OnOff() const = 0;
    virtual QVariant getChannelAlarm3Type() const = 0;
    virtual QVariant getChannelAlarm3Status() const = 0;
    virtual QVariant getChannelAlarm3Value() const = 0;
    virtual QVariant getChannelAlarm3Color() const = 0;
    virtual QVariant getChannelAlarm4OnOff() const = 0;
    virtual QVariant getChannelAlarm4Type() const = 0;
    virtual QVariant getChannelAlarm4Status() const = 0;
    virtual QVariant getChannelAlarm4Value() const = 0;
    virtual QVariant getChannelAlarm4Color() const = 0;
    //获取通道报警值（bit0-bit3）
    virtual QVariant getChannelAlarm() const = 0;

    //通道配置
    virtual QVariant getChannelTagName() const = 0;
    virtual QVariant getChannelTagNumber() const = 0;
    virtual QVariant getChannelNumber() const = 0;
signals:

public slots:

protected:
    quint32 m_chanCoder;    //通道编码
};


inline QString Channel::getChannelDefCoder() const
{
    quint8 unit, module, channel;
    unit = CHANNEL_UNIT(m_chanCoder);
    module = CHANNEL_MODULE(m_chanCoder);
    channel = CHANNEL_NUM(m_chanCoder);

    QString defCoder = QString::number(CHANNEL_DEFAULT_CODER(unit, module, channel), 10);
    return defCoder.rightJustified(4, '0');
}
#endif // CHANNEL_H
