/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：DevChannelDI.h
 * 概   要：DI通道实现类，继承类Channel, 提供DI通道的函数接口，以获取该通道的相关
 * 属性
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
#ifndef DEVCHANNELDI_H
#define DEVCHANNELDI_H

#include "channel.h"
#include "eventaction.h"

#include <QColor>


class DevChannelDI : public Channel
{
    Q_OBJECT
public:
    explicit DevChannelDI(quint32 chanCoder, QObject *parent = 0);

    void chanCalculate();                                                                                                   //通道运算
    QVariant getChannelCurData() const;                                                                        //获取通道本次测量值
    void * getChannelConfig() const;                                                                                //获取通道配置指针

    /*数据模型接口*/
    QVariant getChannelData() const;                                                                               //获取通道数据
    QVariant getChannelTag() const;                                                                                 //获取通道标记
    QVariant getChannelColor() const;                                                                              //获取通道颜色
    QVariant getChannelUnit() const;                                                                                //获取通道单位
    QVariant getChannelRangeType() const;                                                                    //获取通道量程类型
    QVariant getChannelRangeUpper() const;                                                                  //获取通道量程实际上限
    QVariant getChannelRangeDowner() const;                                                               //获取通道量程实际下限
    QVariant getChannelStatus() const;
    QVariant getChannelDataAccuracy() const;

    QVariant getChannelCaliCorrectNum() const;

    QVariant getChannelDispZoneUpper() const;
    QVariant getChannelDispZoneDowner() const;

    QVariant getChannelScalePos() const;
    QVariant getChannelScaleDiv() const;

    QVariant getChannelBarBase() const;
    QVariant getChannelBarDiv() const;

    QVariant getChannelPartScaleOnOff() const;
    QVariant getChannelPartScalePos() const;
    QVariant getChannelPartScaleBound() const;

    QVariant getChannelColorScalePos() const;
    QVariant getChannelColorScaleColor() const;
    QVariant getChannelColorScaleUpper() const;
    QVariant getChannelColorScaleDowner() const;

    QVariant getChannelAlarmOnOff() const;
    QVariant getChannelAlarmType() const;
    QVariant getChannelAlarm1OnOff() const;
    QVariant getChannelAlarm1Type() const;
    QVariant getChannelAlarm1Status() const;
    QVariant getChannelAlarm1Value() const;
    QVariant getChannelAlarm1Color() const;
    QVariant getChannelAlarm2OnOff() const;
    QVariant getChannelAlarm2Type() const;
    QVariant getChannelAlarm2Status() const;
    QVariant getChannelAlarm2Value() const;
    QVariant getChannelAlarm2Color() const;
    QVariant getChannelAlarm3OnOff() const;
    QVariant getChannelAlarm3Type() const;
    QVariant getChannelAlarm3Status() const;
    QVariant getChannelAlarm3Value() const;
    QVariant getChannelAlarm3Color() const;
    QVariant getChannelAlarm4OnOff() const;
    QVariant getChannelAlarm4Type() const;
    QVariant getChannelAlarm4Status() const;
    QVariant getChannelAlarm4Value() const;
    QVariant getChannelAlarm4Color() const;
    //获取通道报警值（bit0-bit3）
    QVariant getChannelAlarm() const;
    //通道配置
    QVariant getChannelTagName() const;
    QVariant getChannelTagNumber() const;
    QVariant getChannelNumber() const;

signals:

public slots:
    void slotChanConfigChanged();       //通道配置变更

private:
    float *m_pData;                           //DI通道数据对象指针
    qint16 *m_pStatus;                   //DI通道状态指针
    DI_CONFIG *m_pConfig;             //DI通道配置指针

    /*通道计算相关*/
    qint32 *p_inputCount;               //通道测量输入数据（输入值） 指针,脉冲模式--脉冲计数
    qint16 *p_inputData;                 //通道测量输入数据（输入值） 指针,DI模式
    float *p_calculateData;               //通道测量值地址指针
    float *p_calculateDataLinear;    //通道测量值连续区域地址指针
    qint16 *p_chanStatus;                //通道状态地址指针
    qint16 *p_chanStatusLinear;     //通道连续区域通道状态地址指针

    //通道报警变化率采样值FIFO
    QQueue<qreal>    m_RateIncFIFO;
    QQueue<qreal>    m_RateDecFIFO;

    //通道报警延迟上、下限周期计数
    quint32 m_DelayUpperCounter;
    quint32 m_DelayDownerCounter;
};

/*获取通道当前测量值*/
inline QVariant DevChannelDI::getChannelCurData() const
{
    return *p_calculateData;
}

/*获取通道配置指针*/
inline void * DevChannelDI::getChannelConfig() const
{
    return (void*) m_pConfig;
}

/*获取通道数据*/
inline QVariant DevChannelDI::getChannelData() const
{
    return *m_pData;
}
/*返回通道标记*/
inline QVariant DevChannelDI::getChannelTag() const
{
    if(m_pConfig->displayConfig.displayTagNo[0] != '\0'){
        return QString((const char *)m_pConfig->displayConfig.displayTagNo);
    }

    if(m_pConfig->displayConfig.displayTagChar[0] != '\0'){
        return QString((const char *)m_pConfig->displayConfig.displayTagChar);
    }

    return this->getChannelDefCoder();
}

/*获取通道颜色*/
inline QVariant DevChannelDI::getChannelColor() const
{
    quint32 color = m_pConfig->displayConfig.displayColor;
    return QColor((color>>16) & 0xff, (color>>8) & 0xff, color &0xff);
}

/*获取通道单位*/
inline QVariant DevChannelDI::getChannelUnit() const
{
    QString channelUnit;

    if (m_pConfig->rangeConfig.type == DI_Range_Type_Disabled) {
        channelUnit = QString();
    } else if (m_pConfig->rangeConfig.calculateType == DI_Range_Calculate_Linear) {
        channelUnit = QString((char *)m_pConfig->rangeConfig.scale.scaleUnit);
    } else {
        channelUnit = QString();
    }
    return channelUnit;
}

/*获取通道量程类型*/
inline QVariant DevChannelDI::getChannelRangeType() const
{
    return m_pConfig->rangeConfig.type;
}

/*获取通道量程实际上限*/
inline QVariant DevChannelDI::getChannelRangeUpper() const
{
    qint32 channelRangeUpper;

    if (m_pConfig->rangeConfig.type == DI_Range_Type_Disabled) {
        return QVariant();
    } else if (m_pConfig->rangeConfig.calculateType == DI_Range_Calculate_Linear) {
        channelRangeUpper = m_pConfig->rangeConfig.scale.scaleMax/::pow10(m_pConfig->rangeConfig.scale.scalePosition);
    } else {
        channelRangeUpper = m_pConfig->rangeConfig.rangeDispMax;
    }
    return channelRangeUpper;
}

/*获取通道量程实际下限*/
inline QVariant DevChannelDI::getChannelRangeDowner() const
{
    qint32 channelRangeDowner;

    if (m_pConfig->rangeConfig.type == DI_Range_Type_Disabled) {
        return QVariant();
    } else if (m_pConfig->rangeConfig.calculateType == DI_Range_Calculate_Linear) {
        channelRangeDowner = m_pConfig->rangeConfig.scale.scaleMin/::pow10(m_pConfig->rangeConfig.scale.scalePosition);
    } else {
        channelRangeDowner = m_pConfig->rangeConfig.rangeDispMin;
    }
    return channelRangeDowner;
}

/*获取通道状态*/
inline QVariant DevChannelDI::getChannelStatus() const
{
    return GET_DATA_STATUS(*m_pStatus);
}

/*获取通道数值精度*/
inline QVariant DevChannelDI::getChannelDataAccuracy() const
{
//    return 0;
    quint32 caltype = m_pConfig->rangeConfig.calculateType;

    if (caltype == DI_Range_Calculate_Linear)
    {
        return m_pConfig->rangeConfig.scale.scalePosition;
    }
    else
    {
        return 0;
    }

    return QVariant();
}

/*获取通道测量校正的点数*/
inline QVariant DevChannelDI::getChannelCaliCorrectNum() const
{
    return QVariant();
}

/*获取通道显示区域上限(%)*/
inline QVariant DevChannelDI::getChannelDispZoneUpper() const
{
    if (m_pConfig->rangeConfig.type == DI_Range_Type_Disabled) {
        return QVariant();
    } else {
        return m_pConfig->displayConfig.displayZoneUpper;
    }
}

/*获取通道显示区域下限(%)*/
inline QVariant DevChannelDI::getChannelDispZoneDowner() const
{
    if (m_pConfig->rangeConfig.type == DI_Range_Type_Disabled) {
        return QVariant();
    } else {
        return m_pConfig->displayConfig.dispalyZoneLower;
    }
}

/*获取通道标尺位置*/
inline QVariant DevChannelDI::getChannelScalePos() const
{
    if (m_pConfig->rangeConfig.type == DI_Range_Type_Disabled) {
        return QVariant();
    } else {
        return m_pConfig->displayConfig.displayScalePos;
    }
}

/*获取通道标尺分割数*/
inline QVariant DevChannelDI::getChannelScaleDiv() const
{
    if (m_pConfig->rangeConfig.type == DI_Range_Type_Disabled) {
        return QVariant();
    } else {
        return m_pConfig->displayConfig.displayScaleDiv;
    }
}

/*获取通道棒图基准位置*/
inline QVariant DevChannelDI::getChannelBarBase() const
{
    if (m_pConfig->rangeConfig.type == DI_Range_Type_Disabled) {
        return QVariant();
    } else {
        return m_pConfig->displayConfig.displayBarPos;
    }
}

/*获取通道帮图分割数*/
inline QVariant DevChannelDI::getChannelBarDiv() const
{
    if (m_pConfig->rangeConfig.type == DI_Range_Type_Disabled) {
        return QVariant();
    } else {
        return m_pConfig->displayConfig.displayBarDiv;
    }
}

/*获取通道部分压缩放大开关: 0-off, 1-on*/
inline QVariant DevChannelDI::getChannelPartScaleOnOff() const
{
    return QVariant();
}

/*获取通道部分压缩放大位置，1-99, 单位：%，默认50*/
inline QVariant DevChannelDI::getChannelPartScalePos() const
{
    return QVariant();
}

/*获取通道部分压缩放大边界值*/
inline QVariant DevChannelDI::getChannelPartScaleBound() const
{
    return QVariant();
}

/*获取通道彩色标尺带模式， 0-off, 1-内侧， 2-外侧.*/
inline QVariant DevChannelDI::getChannelColorScalePos() const
{
    return QVariant();
}
/*获取通道彩色标尺带颜色*/
inline QVariant DevChannelDI::getChannelColorScaleColor() const
{
    return QVariant();
}
/*获取通道彩色标尺带显示位置上限*/
inline QVariant DevChannelDI::getChannelColorScaleUpper() const
{
    return QVariant();
}
/*获取通道彩色标尺带显示位置下限*/
inline QVariant DevChannelDI::getChannelColorScaleDowner() const
{
    return QVariant();
}

/*获取通道报警标记开关，0-0ff, 1-on*/
inline QVariant DevChannelDI::getChannelAlarmOnOff() const
{
    if (m_pConfig->rangeConfig.type == DI_Range_Type_Disabled) {
        return QVariant();
    } else {
        return m_pConfig->displayConfig.displayAlarmMarkOnOff==ON ? true : false;
    }
}
/*获取通道报警标记类型，0-报警， 1-固定*/
inline QVariant DevChannelDI::getChannelAlarmType() const
{
    if (m_pConfig->rangeConfig.type == DI_Range_Type_Disabled) {
        return QVariant();
    } else {
        return m_pConfig->displayConfig.displayAlarmMarkType;
    }
}
inline QVariant DevChannelDI::getChannelAlarm1OnOff() const
{
    if (m_pConfig->rangeConfig.type == DI_Range_Type_Disabled) {
        return QVariant();
    } else {
        return m_pConfig->alarmConfig.alarmOnOff[0];
    }
}
inline QVariant DevChannelDI::getChannelAlarm1Type() const
{
    if (m_pConfig->rangeConfig.type == DI_Range_Type_Disabled) {
        return QVariant();
    } else {
        return m_pConfig->alarmConfig.alarmType[0];
    }
}
inline QVariant DevChannelDI::getChannelAlarm1Status() const
{
    return EventAction::instance()->getAlarmStatus(m_chanCoder, 0);
}
inline QVariant DevChannelDI::getChannelAlarm1Value() const
{
    if (m_pConfig->rangeConfig.type == DI_Range_Type_Disabled) {
        return QVariant();
    } else {
        return m_pConfig->alarmConfig.alarmValue[0];
    }
}
inline QVariant DevChannelDI::getChannelAlarm1Color() const
{
    if (m_pConfig->rangeConfig.type == DI_Range_Type_Disabled) {
        return QVariant();
    } else {
        quint32 color = m_pConfig->displayConfig.displayAlarmMarkColor[0];
        return QColor((color>>16) & 0xff, (color>>8) & 0xff, color &0xff);
    }
}
inline QVariant DevChannelDI::getChannelAlarm2OnOff() const
{
    if (m_pConfig->rangeConfig.type == DI_Range_Type_Disabled) {
        return QVariant();
    } else {
        return m_pConfig->alarmConfig.alarmOnOff[1];
    }
}
inline QVariant DevChannelDI::getChannelAlarm2Type() const
{
    if (m_pConfig->rangeConfig.type == DI_Range_Type_Disabled) {
        return QVariant();
    } else {
        return m_pConfig->alarmConfig.alarmType[1];
    }
}
inline QVariant DevChannelDI::getChannelAlarm2Status() const
{
    return EventAction::instance()->getAlarmStatus(m_chanCoder, 1);
}
inline QVariant DevChannelDI::getChannelAlarm2Value() const
{
    if (m_pConfig->rangeConfig.type == DI_Range_Type_Disabled) {
        return QVariant();
    } else {
        return m_pConfig->alarmConfig.alarmValue[1];
    }
}
inline QVariant DevChannelDI::getChannelAlarm2Color() const
{
    if (m_pConfig->rangeConfig.type == DI_Range_Type_Disabled) {
        return QVariant();
    } else {
        quint32 color = m_pConfig->displayConfig.displayAlarmMarkColor[1];
        return QColor((color>>16) & 0xff, (color>>8) & 0xff, color &0xff);
    }
}
inline QVariant DevChannelDI::getChannelAlarm3OnOff() const
{
    if (m_pConfig->rangeConfig.type == DI_Range_Type_Disabled) {
        return QVariant();
    } else {
        return m_pConfig->alarmConfig.alarmOnOff[2];
    }
}
inline QVariant DevChannelDI::getChannelAlarm3Type() const
{
    if (m_pConfig->rangeConfig.type == DI_Range_Type_Disabled) {
        return QVariant();
    } else {
        return m_pConfig->alarmConfig.alarmType[2];
    }
}
inline QVariant DevChannelDI::getChannelAlarm3Status() const
{
    return EventAction::instance()->getAlarmStatus(m_chanCoder, 2);
}
inline QVariant DevChannelDI::getChannelAlarm3Value() const
{
    if (m_pConfig->rangeConfig.type == DI_Range_Type_Disabled) {
        return QVariant();
    } else {
        return m_pConfig->alarmConfig.alarmValue[2];
    }
}
inline QVariant DevChannelDI::getChannelAlarm3Color() const
{
    if (m_pConfig->rangeConfig.type == DI_Range_Type_Disabled) {
        return QVariant();
    } else {
        quint32 color = m_pConfig->displayConfig.displayAlarmMarkColor[2];
        return QColor((color>>16) & 0xff, (color>>8) & 0xff, color &0xff);
    }
}
inline QVariant DevChannelDI::getChannelAlarm4OnOff() const
{
    if (m_pConfig->rangeConfig.type == DI_Range_Type_Disabled) {
        return QVariant();
    } else {
        return m_pConfig->alarmConfig.alarmOnOff[3];
    }
}
inline QVariant DevChannelDI::getChannelAlarm4Type() const
{
    if (m_pConfig->rangeConfig.type == DI_Range_Type_Disabled) {
        return QVariant();
    } else {
        return m_pConfig->alarmConfig.alarmType[3];
    }
}
inline QVariant DevChannelDI::getChannelAlarm4Status() const
{
    return EventAction::instance()->getAlarmStatus(m_chanCoder, 3);
}
inline QVariant DevChannelDI::getChannelAlarm4Value() const
{
    if (m_pConfig->rangeConfig.type == DI_Range_Type_Disabled) {
        return QVariant();
    } else {
        return m_pConfig->alarmConfig.alarmValue[3];
    }
}
inline QVariant DevChannelDI::getChannelAlarm4Color() const
{
    if (m_pConfig->rangeConfig.type == DI_Range_Type_Disabled) {
        return QVariant();
    } else {
        quint32 color = m_pConfig->displayConfig.displayAlarmMarkColor[3];
        return QColor((color>>16) & 0xff, (color>>8) & 0xff, color &0xff);
    }
}
//获取通道报警值（bit0-bit3）
inline QVariant DevChannelDI::getChannelAlarm() const
{
    return GET_DATA_ALARM(*m_pStatus);
}
inline QVariant DevChannelDI::getChannelTagName() const
{
    return QString((const char*)m_pConfig->displayConfig.displayTagChar);
}
inline QVariant DevChannelDI::getChannelTagNumber() const
{
    return QString((const char*)m_pConfig->displayConfig.displayTagNo);
}
inline QVariant DevChannelDI::getChannelNumber() const
{
    return getChannelDefCoder();
}
#endif // DEVCHANNELDI_H
