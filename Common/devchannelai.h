/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：DevChannelAI.h
 * 概   要：AI通道实现类，继承类Channel, 提供AI通道的函数接口，以获取该通道的相关
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
#ifndef DEVCHANNELAI_H
#define DEVCHANNELAI_H

#include "channel.h"
#include "eventaction.h"

#include <QColor>

class DevChannelAI : public Channel
{
    Q_OBJECT
public:
    explicit DevChannelAI(quint32 chanCoder, QObject *parent = 0);

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
    float *m_pData;                           //AI通道数据对象指针
    qint16 *m_pStatus;                     //AI通道状态指针
    AI_CONFIG *m_pConfig;             //AI通道配置指针

    /*通道计算相关*/
    float *p_inputData;                     //通道测量输入数据（输入值） 指针
    float *p_calculateData;               //通道测量值地址指针
    float *p_calculateDataLinear;    //通道测量值连续区域地址指针
    qint16 *p_chanStatus;                //通道状态地址指针
    qint16 *p_chanStatusLinear;     //通道连续区域通道状态地址指针

    //通道移动平均采样值FIFO，输入值是经过基本查表处理后的数据
    QQueue<qreal>    m_moveFIFO;

    //通道报警变化率采样值FIFO
    QQueue<qreal>    m_RateIncFIFO;
    QQueue<qreal>    m_RateDecFIFO;

    //通道报警延迟上、下限周期计数
    quint32 m_DelayUpperCounter;
    quint32 m_DelayDownerCounter;
};

/*获取通道当前测量值*/
inline QVariant DevChannelAI::getChannelCurData() const
{
    return *p_calculateData;
}
/*获取通道配置指针*/
inline void * DevChannelAI::getChannelConfig() const
{
    return (void*) m_pConfig;
}

/*获取通道数据*/
inline QVariant DevChannelAI::getChannelData() const
{
    if (AI_RANGE_Type_Vlot == m_pConfig->rangeConfig.rangeType &&
        m_pConfig->rangeConfig.config.rangeVolt < AI_Range_Volt_1V) {
        return *m_pData *1000;
    } else {
        return *m_pData;
    }
}
/*返回通道标记*/
inline QVariant DevChannelAI::getChannelTag() const
{
    if(m_pConfig->displayConfig.displayTagNo[0] != '\0'){
        return QString((const char *)m_pConfig->displayConfig.displayTagNo);
    }

    if(m_pConfig->displayConfig.displayTagChar[0] != '\0' ){
        return QString((const char *)m_pConfig->displayConfig.displayTagChar);
    }

    return this->getChannelDefCoder();
}

/*获取通道颜色*/
inline QVariant DevChannelAI::getChannelColor() const
{
    quint32 color = m_pConfig->displayConfig.displayColor;
    return QColor((color>>16) & 0xff, (color>>8) & 0xff, color &0xff);
}

/*获取通道单位*/
inline QVariant DevChannelAI::getChannelUnit() const
{
    QString channelUnit;

    if (m_pConfig->rangeConfig.rangeType == AI_RANGE_Type_Disabled) {
        channelUnit = QString();
    } else if (m_pConfig->rangeConfig.rangeCalculateType == AI_Range_Calculate_Linear ||
               m_pConfig->rangeConfig.rangeCalculateType == AI_Range_Calculate_Sqrt) {
        channelUnit = QString((char *)m_pConfig->rangeConfig.rangeScale.scaleUnit);
    } else {
        switch (m_pConfig->rangeConfig.rangeType) {
        case AI_RANGE_Type_Vlot:
            if(m_pConfig->rangeConfig.config.rangeVolt<=AI_Range_Volt_200mV)
                channelUnit = QString("mV");
            else
                channelUnit =  QString("V");
            break;
        case AI_RANGE_Type_GS:
            channelUnit =  QString("V");
            break;
        case AI_RANGE_Type_TC:
            if (m_pConfig->rangeConfig.config.rangeTC == AI_Range_TC_KpvsAu7Fe)
                channelUnit =  QString("K");
            else
                channelUnit = QString("°C");
            break;
        case AI_RANGE_Type_RTD:
            if (m_pConfig->rangeConfig.config.rangeRTD == AI_Range_RTD_J263B)
                channelUnit = QString("K");
            else
                channelUnit = QString("°C");
            break;
        case AI_RANGE_Type_DI:
            channelUnit =  QString();
            break;
        case AI_RANGE_Type_Cur0_20mA:
        case AI_RANGE_Type_GS4_20mA:
            channelUnit = QString("mA");
            break;
        default:
            break;
        }
    }

    return channelUnit;
}

/*获取通道量程类型*/
inline QVariant DevChannelAI::getChannelRangeType() const
{
    return m_pConfig->rangeConfig.rangeType;
}

/*获取通道量程实际上限*/
inline QVariant DevChannelAI::getChannelRangeUpper() const
{
    qreal channelRangeUpper;

    if (m_pConfig->rangeConfig.rangeType == AI_RANGE_Type_Disabled) {
        return QVariant();
    } else if (m_pConfig->rangeConfig.rangeCalculateType == AI_Range_Calculate_Linear ||
               m_pConfig->rangeConfig.rangeCalculateType == AI_Range_Calculate_Sqrt) {
        channelRangeUpper = (qreal)m_pConfig->rangeConfig.rangeScale.scaleMax/::pow10(m_pConfig->rangeConfig.rangeScale.scalePosition);
    } else {
        channelRangeUpper = (qreal)m_pConfig->rangeConfig.rangeDispMax;
    }
    return channelRangeUpper;
}

/*获取通道量程实际下限*/
inline QVariant DevChannelAI::getChannelRangeDowner() const
{
    qreal channelRangeDowner;

    if (m_pConfig->rangeConfig.rangeType == AI_RANGE_Type_Disabled) {
        return QVariant();
    } else if (m_pConfig->rangeConfig.rangeCalculateType == AI_Range_Calculate_Linear ||
               m_pConfig->rangeConfig.rangeCalculateType == AI_Range_Calculate_Sqrt) {
        channelRangeDowner = (qreal)m_pConfig->rangeConfig.rangeScale.scaleMin/::pow10(m_pConfig->rangeConfig.rangeScale.scalePosition);
    } else {
        channelRangeDowner = (qreal)m_pConfig->rangeConfig.rangeDispMin;
    }
    return channelRangeDowner;
}

/*获取通道状态*/
inline QVariant DevChannelAI::getChannelStatus() const
{
    return GET_DATA_STATUS(*m_pStatus);
}

/*获取通道数值精度*/
inline QVariant DevChannelAI::getChannelDataAccuracy() const
{
//    return 4;
    quint32 caltype = m_pConfig->rangeConfig.rangeCalculateType;
    if (caltype == AI_Range_Calculate_Linear || caltype == AI_Range_Calculate_Sqrt)
        return m_pConfig->rangeConfig.rangeScale.scalePosition;

    quint32 type = m_pConfig->rangeConfig.rangeType;
    quint32 range;
    if (type == AI_RANGE_Type_Disabled) {
        return QVariant();
    }
    else if (type == AI_RANGE_Type_Vlot)
    {
        range = m_pConfig->rangeConfig.config.rangeVolt;
        switch (range) {
        case AI_Range_Volt_60mV:
        case AI_Range_Volt_200mV:
        case AI_Range_Volt_50V:
            return 2;
            break;
        case AI_Range_Volt_20mV:
        case AI_Range_Volt_6V:
        case AI_Range_Volt_20V:
            return 3;
            break;
        case AI_Range_Volt_1V:
        case AI_Range_Volt_2V:
            return 4;
            break;
        default:
            break;
        }
    }
    else if (type == AI_RANGE_Type_GS)
    {
        return 4;
    }
    else if (type == AI_RANGE_Type_TC)
    {
        return 1;
    }
    else if (type == AI_RANGE_Type_RTD)
    {
        range = m_pConfig->rangeConfig.config.rangeRTD;
        switch (range) {
        case AI_Range_RTD_Pt100:
        case AI_Range_RTD_JPt100:
        case AI_Range_RTD_Cu10GE:
        case AI_Range_RTD_Cu10LN:
        case AI_Range_RTD_Cu10WEED:
        case AI_Range_RTD_Cu10BAILEY:
        case AI_Range_RTD_Cu10a392:
        case AI_Range_RTD_Cu10a393:
        case AI_Range_RTD_Cu25:
        case AI_Range_RTD_Cu53:
        case AI_Range_RTD_Cu100:
        case AI_Range_RTD_J263B:
        case AI_Range_RTD_Ni100SAMA:
        case AI_Range_RTD_Ni100DN:
        case AI_Range_RTD_Ni120:
        case AI_Range_RTD_Pt25:
        case AI_Range_RTD_Pt50:
        case AI_Range_RTD_Pt200WEED:
        case AI_Range_RTD_Cu10G:
        case AI_Range_RTD_Cu50G:
        case AI_Range_RTD_Cu100G:
        case AI_Range_RTD_Pt46G:
        case AI_Range_RTD_Pt100G:
            return 1;
            break;
        case AI_Range_RTD_Pt100_H:
        case AI_Range_RTD_JPt100_H:
            return 2;
            break;
        default:
            break;
        }
    }
    else if (type == AI_RANGE_Type_DI)
    {
        return 0;
    }
    else if (type == AI_RANGE_Type_Cur0_20mA || type == AI_RANGE_Type_GS4_20mA)
    {
        return 2;
    }

    return QVariant();
}

/*获取通道测量校正的点数*/
inline QVariant DevChannelAI::getChannelCaliCorrectNum() const
{
    if (m_pConfig->correctConfig.correctMode == 0)
        return QVariant();
    else
        return m_pConfig->correctConfig.correctCount;
}

/*获取通道显示区域上限(%)*/
inline QVariant DevChannelAI::getChannelDispZoneUpper() const
{
    if (m_pConfig->rangeConfig.rangeType == AI_RANGE_Type_Disabled)
        return QVariant();
    else
        return m_pConfig->displayConfig.displayZoneUpper;
}

/*获取通道显示区域下限(%)*/
inline QVariant DevChannelAI::getChannelDispZoneDowner() const
{
    if (m_pConfig->rangeConfig.rangeType == AI_RANGE_Type_Disabled)
        return QVariant();
    else
        return m_pConfig->displayConfig.displayZoneLower;
}

/*获取通道标尺位置*/
inline QVariant DevChannelAI::getChannelScalePos() const
{
    if (m_pConfig->rangeConfig.rangeType == AI_RANGE_Type_Disabled)
        return QVariant();
    else
        return m_pConfig->displayConfig.displayScalePos;
}

/*获取通道标尺分割数*/
inline QVariant DevChannelAI::getChannelScaleDiv() const
{
    if (m_pConfig->rangeConfig.rangeType == AI_RANGE_Type_Disabled)
        return QVariant();
    else
        return m_pConfig->displayConfig.displayScaleDiv;
}

/*获取通道棒图基准位置*/
inline QVariant DevChannelAI::getChannelBarBase() const
{
    if (m_pConfig->rangeConfig.rangeType == AI_RANGE_Type_Disabled) {
        return QVariant();
    } else {
        return m_pConfig->displayConfig.displayBarPos;
    }
}

/*获取通道帮图分割数*/
inline QVariant DevChannelAI::getChannelBarDiv() const
{
    if (m_pConfig->rangeConfig.rangeType == AI_RANGE_Type_Disabled) {
        return QVariant();
    } else {
        return m_pConfig->displayConfig.displayBarDiv;
    }
}

/*获取通道部分压缩放大开关: 0-off, 1-on*/
inline QVariant DevChannelAI::getChannelPartScaleOnOff() const
{
    if (m_pConfig->rangeConfig.rangeType == AI_RANGE_Type_Disabled) {
        return QVariant();
    } else {
        return m_pConfig->displayConfig.displayPartialOnOff;
    }
}

/*获取通道部分压缩放大位置，1-99, 单位：%，默认50*/
inline QVariant DevChannelAI::getChannelPartScalePos() const
{
    if (m_pConfig->rangeConfig.rangeType == AI_RANGE_Type_Disabled) {
        return QVariant();
    } else {
        return m_pConfig->displayConfig.displayPartialPos;
    }
}

/*获取通道部分压缩放大边界值*/
inline QVariant DevChannelAI::getChannelPartScaleBound() const
{
    if (m_pConfig->rangeConfig.rangeType == AI_RANGE_Type_Disabled) {
        return QVariant();
    } else {
        return m_pConfig->displayConfig.displayPartialBoundary;
    }
}

/*获取通道彩色标尺带模式， 0-off, 1-内侧， 2-外侧.*/
inline QVariant DevChannelAI::getChannelColorScalePos() const
{
    if (m_pConfig->rangeConfig.rangeType == AI_RANGE_Type_Disabled) {
        return QVariant();
    } else {
        return m_pConfig->displayConfig.displayColorbandMode;
    }
}
/*获取通道彩色标尺带颜色*/
inline QVariant DevChannelAI::getChannelColorScaleColor() const
{
    if (m_pConfig->rangeConfig.rangeType == AI_RANGE_Type_Disabled) {
        return QVariant();
    } else {
        quint32 color = m_pConfig->displayConfig.displayColorbandColor;
        return QColor((color>>16) & 0xff, (color>>8) & 0xff, color &0xff);
    }
}
/*获取通道彩色标尺带显示位置上限*/
inline QVariant DevChannelAI::getChannelColorScaleUpper() const
{
    if (m_pConfig->rangeConfig.rangeType == AI_RANGE_Type_Disabled) {
        return QVariant();
    } else {
        return m_pConfig->displayConfig.displayColorbandPosUpper;
    }
}
/*获取通道彩色标尺带显示位置下限*/
inline QVariant DevChannelAI::getChannelColorScaleDowner() const
{
    if (m_pConfig->rangeConfig.rangeType == AI_RANGE_Type_Disabled) {
        return QVariant();
    } else {
        return m_pConfig->displayConfig.displayColorbandPosLower;
    }
}

/*获取通道报警标记开关，0-0ff, 1-on*/
inline QVariant DevChannelAI::getChannelAlarmOnOff() const
{
    if (m_pConfig->rangeConfig.rangeType == AI_RANGE_Type_Disabled)
        return QVariant();
    else
        return m_pConfig->displayConfig.displayAlarmMarkOnOff==ON ? true : false;
}
/*获取通道报警标记类型，0-报警， 1-固定*/
inline QVariant DevChannelAI::getChannelAlarmType() const
{
    if (m_pConfig->rangeConfig.rangeType == AI_RANGE_Type_Disabled)
        return QVariant();
    else
        return m_pConfig->displayConfig.displayAlarmMarkType;
}

inline QVariant DevChannelAI::getChannelAlarm1OnOff() const
{
    if (m_pConfig->rangeConfig.rangeType == AI_RANGE_Type_Disabled) {
        return QVariant();
    } else {
        return m_pConfig->alarmConfig.alarmOnOff[0];
    }
}

inline QVariant DevChannelAI::getChannelAlarm1Type() const
{
    if (m_pConfig->rangeConfig.rangeType == AI_RANGE_Type_Disabled) {
        return QVariant();
    } else {
        return m_pConfig->alarmConfig.alarmType[0];
    }
}
inline QVariant DevChannelAI::getChannelAlarm1Status() const
{
    return EventAction::instance()->getAlarmStatus(m_chanCoder, 0);
}
inline QVariant DevChannelAI::getChannelAlarm1Value() const
{
    if (m_pConfig->rangeConfig.rangeType == AI_RANGE_Type_Disabled) {
        return QVariant();
    } else {
        return m_pConfig->alarmConfig.alarmValue[0];
    }
}
inline QVariant DevChannelAI::getChannelAlarm1Color() const
{
    if (m_pConfig->rangeConfig.rangeType == AI_RANGE_Type_Disabled) {
        return QVariant();
    } else {
        quint32 color = m_pConfig->displayConfig.displayAlarmMarkColor[0];
        return QColor((color>>16) & 0xff, (color>>8) & 0xff, color &0xff);
    }
}
inline QVariant DevChannelAI::getChannelAlarm2OnOff() const
{
    if (m_pConfig->rangeConfig.rangeType == AI_RANGE_Type_Disabled) {
        return QVariant();
    } else {
        return m_pConfig->alarmConfig.alarmOnOff[1];
    }
}
inline QVariant DevChannelAI::getChannelAlarm2Type() const
{
    if (m_pConfig->rangeConfig.rangeType == AI_RANGE_Type_Disabled) {
        return QVariant();
    } else {
        return m_pConfig->alarmConfig.alarmType[1];
    }
}
inline QVariant DevChannelAI::getChannelAlarm2Status() const
{
    return EventAction::instance()->getAlarmStatus(m_chanCoder, 1);
}
inline QVariant DevChannelAI::getChannelAlarm2Value() const
{
    if (m_pConfig->rangeConfig.rangeType == AI_RANGE_Type_Disabled) {
        return QVariant();
    } else {
        return m_pConfig->alarmConfig.alarmValue[1];
    }
}
inline QVariant DevChannelAI::getChannelAlarm2Color() const
{
    if (m_pConfig->rangeConfig.rangeType == AI_RANGE_Type_Disabled) {
        return QVariant();
    } else {
        quint32 color = m_pConfig->displayConfig.displayAlarmMarkColor[1];
        return QColor((color>>16) & 0xff, (color>>8) & 0xff, color &0xff);
    }
}
inline QVariant DevChannelAI::getChannelAlarm3OnOff() const
{
    if (m_pConfig->rangeConfig.rangeType == AI_RANGE_Type_Disabled) {
        return QVariant();
    } else {
        return m_pConfig->alarmConfig.alarmOnOff[2];
    }
}
inline QVariant DevChannelAI::getChannelAlarm3Type() const
{
    if (m_pConfig->rangeConfig.rangeType == AI_RANGE_Type_Disabled) {
        return QVariant();
    } else {
        return m_pConfig->alarmConfig.alarmType[2];
    }
}
inline QVariant DevChannelAI::getChannelAlarm3Status() const
{
    return EventAction::instance()->getAlarmStatus(m_chanCoder, 2);
}
inline QVariant DevChannelAI::getChannelAlarm3Value() const
{
    if (m_pConfig->rangeConfig.rangeType == AI_RANGE_Type_Disabled) {
        return QVariant();
    } else {
        return m_pConfig->alarmConfig.alarmValue[2];
    }
}
inline QVariant DevChannelAI::getChannelAlarm3Color() const
{
    if (m_pConfig->rangeConfig.rangeType == AI_RANGE_Type_Disabled) {
        return QVariant();
    } else {
        quint32 color = m_pConfig->displayConfig.displayAlarmMarkColor[2];
        return QColor((color>>16) & 0xff, (color>>8) & 0xff, color &0xff);
    }
}
inline QVariant DevChannelAI::getChannelAlarm4OnOff() const
{
    if (m_pConfig->rangeConfig.rangeType == AI_RANGE_Type_Disabled) {
        return QVariant();
    } else {
        return m_pConfig->alarmConfig.alarmOnOff[3];
    }
}
inline QVariant DevChannelAI::getChannelAlarm4Type() const
{
    if (m_pConfig->rangeConfig.rangeType == AI_RANGE_Type_Disabled) {
        return QVariant();
    } else {
        return m_pConfig->alarmConfig.alarmType[3];
    }
}
inline QVariant DevChannelAI::getChannelAlarm4Status() const
{
    return EventAction::instance()->getAlarmStatus(m_chanCoder, 3);
}
inline QVariant DevChannelAI::getChannelAlarm4Value() const
{
    if (m_pConfig->rangeConfig.rangeType == AI_RANGE_Type_Disabled) {
        return QVariant();
    } else {
        return m_pConfig->alarmConfig.alarmValue[3];
    }
}
inline QVariant DevChannelAI::getChannelAlarm4Color() const
{
    if (m_pConfig->rangeConfig.rangeType == AI_RANGE_Type_Disabled) {
        return QVariant();
    } else {
        quint32 color = m_pConfig->displayConfig.displayAlarmMarkColor[3];
        return QColor((color>>16) & 0xff, (color>>8) & 0xff, color &0xff);
    }
}
//获取通道报警值（bit0-bit3）
inline QVariant DevChannelAI::getChannelAlarm() const
{
    return GET_DATA_ALARM(*m_pStatus);
}

inline QVariant DevChannelAI::getChannelTagName() const
{
    return QString((const char*)m_pConfig->displayConfig.displayTagChar);
}
inline QVariant DevChannelAI::getChannelTagNumber() const
{
    return QString((const char*)m_pConfig->displayConfig.displayTagNo);
}
inline QVariant DevChannelAI::getChannelNumber() const
{
    return getChannelDefCoder();
}
#endif // DEVCHANNELAI_H
