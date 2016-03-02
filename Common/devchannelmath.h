/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：DevChannelMath.h
 * 概   要：Math通道实现类，继承类Channel, 提供Math通道的函数接口，以获取该通道
 * 的相关属性
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
#ifndef DEVCHANNELMATH_H
#define DEVCHANNELMATH_H

#include "channel.h"
#include "eventaction.h"
#include "mathexpanalysis.h"

#include <QColor>

class DevChannelMath : public Channel
{
    Q_OBJECT
public:
    explicit DevChannelMath(quint32 chanCoder, QObject *parent = 0);

    void chanTlogCalculate();                                                                                            //通道TLOG运算
    void chanCalculate();                                                                                                   //通道运算
    QVariant getChannelCurData() const;                                                                        //获取通道本次测量值
    QString getChannelDefCoder() const;                                                                        //获取通道默认编码
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
    void slotChanConfigChanged();       //运算式配置变更

private:
    float *m_pData;                                 //Math通道数据对象指针
    qint16 *m_pStatus;                           //Math通道状态指针
    MATH_CONFIG *m_pConfig;             //Math通道配置指针
    MathExpAnalysis *p_expAnalysis;    //Math表达式指针

    /*通道计算相关*/
    float *p_calculateData;                     //通道测量值地址指针
    qint16 *p_chanStatus;                      //通道状态地址指针


    quint32 m_sampleCount;                        //运算通道采样间隔计数
    QQueue<qreal>    m_sample_moving;   //运算通道移动平均采样值FIFO
    //运算通道报警延迟上、下限周期计数
    quint32 m_math_DelayUpper;
    quint32 m_math_DelayDowner;

    friend class DataProcess;
};

/*获取通道当前测量值*/
inline QVariant DevChannelMath::getChannelCurData() const
{
    return *p_calculateData;
}
/*获取通道默认编码*/
inline QString DevChannelMath::getChannelDefCoder() const
{
    quint16 unit, module, channel;
    unit = CHANNEL_UNIT(m_chanCoder);
    module = CHANNEL_MODULE(m_chanCoder);
    channel = CHANNEL_NUM(m_chanCoder);

    QString defCoder = QString::number(CHANNEL_DEFAULT_CODER(unit, module, channel), 10).rightJustified(4, '0');
    return defCoder.replace(0, 1, 'A');
}

/*获取通道配置指针*/
inline void * DevChannelMath::getChannelConfig() const
{
    return (void*) m_pConfig;
}

/*获取通道数据*/
inline QVariant DevChannelMath::getChannelData() const
{
    return *m_pData;
}
/*返回通道标记*/
inline QVariant DevChannelMath::getChannelTag() const
{
    if(m_pConfig->displayConfig.displayTagNo[0] != '\0'){
        return QString((const char *)m_pConfig->displayConfig.displayTagNo) ;
    }

    if(m_pConfig->displayConfig.displayTagChar[0] != '\0' ){
        return QString((const char *)m_pConfig->displayConfig.displayTagChar);
    }

    return this->getChannelDefCoder();
}

/*获取通道颜色*/
inline QVariant DevChannelMath::getChannelColor() const
{
    quint32 color = m_pConfig->displayConfig.displayColor;
    return QColor((color>>16) & 0xff, (color>>8) & 0xff, color &0xff);
}

/*获取通道单位*/
inline QVariant DevChannelMath::getChannelUnit() const
{
    return QString((char *)m_pConfig->expressConfig.expUnit);
}

/*获取通道量程类型*/
inline QVariant DevChannelMath::getChannelRangeType() const
{
    return m_pConfig->expressConfig.expSwtich;
}

/*获取通道量程实际上限*/
inline QVariant DevChannelMath::getChannelRangeUpper() const
{
    if (0==m_pConfig->expressConfig.expSwtich) {
        return QVariant();
    } else {
        qreal channelRangeUpper = (qreal)m_pConfig->expressConfig.expMax/::pow10(m_pConfig->expressConfig.expPos);
        return channelRangeUpper;
    }
}

/*获取通道量程实际下限*/
inline QVariant DevChannelMath::getChannelRangeDowner() const
{
    if (0==m_pConfig->expressConfig.expSwtich) {
        return QVariant();
    } else {
        qreal channelRangeDowner = (qreal)m_pConfig->expressConfig.expMin/::pow10(m_pConfig->expressConfig.expPos);
        return channelRangeDowner;
    }
}

/*获取通道状态*/
inline QVariant DevChannelMath::getChannelStatus() const
{
    return GET_DATA_STATUS(*m_pStatus);
}

/*获取通道数值精度*/
inline QVariant DevChannelMath::getChannelDataAccuracy() const
{
    return m_pConfig->expressConfig.expPos;
}

/*获取通道测量校正的点数*/
inline QVariant DevChannelMath::getChannelCaliCorrectNum() const
{
    return QVariant();
}

/*获取通道显示区域上限(%)*/
inline QVariant DevChannelMath::getChannelDispZoneUpper() const
{
    if (OFF == m_pConfig->expressConfig.expSwtich) {
        return QVariant();
    } else {
        return m_pConfig->displayConfig.displayZoneUpper;
    }
}

/*获取通道显示区域下限(%)*/
inline QVariant DevChannelMath::getChannelDispZoneDowner() const
{
    if (OFF == m_pConfig->expressConfig.expSwtich) {
        return QVariant();
    } else {
        return m_pConfig->displayConfig.dispalyZoneLower;
    }
}

/*获取通道标尺位置*/
inline QVariant DevChannelMath::getChannelScalePos() const
{
    if (OFF == m_pConfig->expressConfig.expSwtich) {
        return QVariant();
    } else {
        return m_pConfig->displayConfig.displayScalePos;
    }
}

/*获取通道标尺分割数*/
inline QVariant DevChannelMath::getChannelScaleDiv() const
{
    if (OFF == m_pConfig->expressConfig.expSwtich) {
        return QVariant();
    } else {
        return m_pConfig->displayConfig.displayScaleDiv;
    }
}

/*获取通道棒图基准位置*/
inline QVariant DevChannelMath::getChannelBarBase() const
{
    if (OFF == m_pConfig->expressConfig.expSwtich) {
        return QVariant();
    } else {
        return m_pConfig->displayConfig.displayBarPos;
    }
}

/*获取通道帮图分割数*/
inline QVariant DevChannelMath::getChannelBarDiv() const
{
    if (OFF == m_pConfig->expressConfig.expSwtich) {
        return QVariant();
    } else {
        return m_pConfig->displayConfig.displayBarDiv;
    }
}

/*获取通道部分压缩放大开关: 0-off, 1-on*/
inline QVariant DevChannelMath::getChannelPartScaleOnOff() const
{
    if (OFF == m_pConfig->expressConfig.expSwtich) {
        return QVariant();
    } else {
        return m_pConfig->displayConfig.displayPartialOnOff;
    }
}

/*获取通道部分压缩放大位置，1-99, 单位：%，默认50*/
inline QVariant DevChannelMath::getChannelPartScalePos() const
{
    if (OFF == m_pConfig->expressConfig.expSwtich) {
        return QVariant();
    } else {
        return m_pConfig->displayConfig.displayPartialPos;
    }
}

/*获取通道部分压缩放大边界值*/
inline QVariant DevChannelMath::getChannelPartScaleBound() const
{
    if (OFF == m_pConfig->expressConfig.expSwtich) {
        return QVariant();
    } else {
        return m_pConfig->displayConfig.displayPartialBoundary;
    }
}

/*获取通道彩色标尺带模式， 0-off, 1-内侧， 2-外侧.*/
inline QVariant DevChannelMath::getChannelColorScalePos() const
{
    if (OFF == m_pConfig->expressConfig.expSwtich) {
        return QVariant();
    } else {
        return m_pConfig->displayConfig.displayColorbandMode;
    }
}
/*获取通道彩色标尺带颜色*/
inline QVariant DevChannelMath::getChannelColorScaleColor() const
{
    if (OFF == m_pConfig->expressConfig.expSwtich) {
        return QVariant();
    } else {
        quint32 color = m_pConfig->displayConfig.displayColorbandColor;
        return QColor((color>>16) & 0xff, (color>>8) & 0xff, color &0xff);
    }
}
/*获取通道彩色标尺带显示位置上限*/
inline QVariant DevChannelMath::getChannelColorScaleUpper() const
{
    if (OFF == m_pConfig->expressConfig.expSwtich) {
        return QVariant();
    } else {
        return m_pConfig->displayConfig.displayColorbandPosUpper;
    }
}
/*获取通道彩色标尺带显示位置下限*/
inline QVariant DevChannelMath::getChannelColorScaleDowner() const
{
    if (OFF == m_pConfig->expressConfig.expSwtich) {
        return QVariant();
    } else {
        return m_pConfig->displayConfig.displayColorbandPosLower;
    }
}

/*获取通道报警标记开关，0-0ff, 1-on*/
inline QVariant DevChannelMath::getChannelAlarmOnOff() const
{
    if (OFF == m_pConfig->expressConfig.expSwtich) {
        return QVariant();
    } else {
        return m_pConfig->displayConfig.displayAlarmMarkOnOff==ON ? true : false;
    }
}
/*获取通道报警标记类型，0-报警， 1-固定*/
inline QVariant DevChannelMath::getChannelAlarmType() const
{
    if (OFF == m_pConfig->expressConfig.expSwtich) {
        return QVariant();
    } else {
        return m_pConfig->displayConfig.displayAlarmMarkType;
    }
}
inline QVariant DevChannelMath::getChannelAlarm1OnOff() const
{
    if (OFF == m_pConfig->expressConfig.expSwtich) {
        return QVariant();
    } else {
        return m_pConfig->alarmConfig.alarmOnOff[0];
    }
}
inline QVariant DevChannelMath::getChannelAlarm1Type() const
{
    if (OFF == m_pConfig->expressConfig.expSwtich) {
        return QVariant();
    } else {
        return m_pConfig->alarmConfig.alarmType[0];
    }
}
inline QVariant DevChannelMath::getChannelAlarm1Status() const
{
    return EventAction::instance()->getAlarmStatus(m_chanCoder, 0);
}
inline QVariant DevChannelMath::getChannelAlarm1Value() const
{
    if (OFF == m_pConfig->expressConfig.expSwtich) {
        return QVariant();
    } else {
        return m_pConfig->alarmConfig.alarmValue[0];
    }
}
inline QVariant DevChannelMath::getChannelAlarm1Color() const
{
    if (OFF == m_pConfig->expressConfig.expSwtich) {
        return QVariant();
    } else {
        quint32 color = m_pConfig->displayConfig.displayAlarmMarkColor[0];
        return QColor((color>>16) & 0xff, (color>>8) & 0xff, color &0xff);
    }
}
inline QVariant DevChannelMath::getChannelAlarm2Type() const
{
    if (OFF == m_pConfig->expressConfig.expSwtich) {
        return QVariant();
    } else {
        return m_pConfig->alarmConfig.alarmType[1];
    }
}
inline QVariant DevChannelMath::getChannelAlarm2OnOff() const
{
    if (OFF == m_pConfig->expressConfig.expSwtich) {
        return QVariant();
    } else {
        return m_pConfig->alarmConfig.alarmOnOff[1];
    }
}
inline QVariant DevChannelMath::getChannelAlarm2Status() const
{
    return EventAction::instance()->getAlarmStatus(m_chanCoder, 1);
}
inline QVariant DevChannelMath::getChannelAlarm2Value() const
{
    if (OFF == m_pConfig->expressConfig.expSwtich) {
        return QVariant();
    } else {
        return m_pConfig->alarmConfig.alarmValue[1];
    }
}
inline QVariant DevChannelMath::getChannelAlarm2Color() const
{
    if (OFF == m_pConfig->expressConfig.expSwtich) {
        return QVariant();
    } else {
        quint32 color = m_pConfig->displayConfig.displayAlarmMarkColor[1];
        return QColor((color>>16) & 0xff, (color>>8) & 0xff, color &0xff);
    }
}
inline QVariant DevChannelMath::getChannelAlarm3OnOff() const
{
    if (OFF == m_pConfig->expressConfig.expSwtich) {
        return QVariant();
    } else {
        return m_pConfig->alarmConfig.alarmOnOff[2];
    }
}
inline QVariant DevChannelMath::getChannelAlarm3Type() const
{
    if (OFF == m_pConfig->expressConfig.expSwtich) {
        return QVariant();
    } else {
        return m_pConfig->alarmConfig.alarmType[2];
    }
}
inline QVariant DevChannelMath::getChannelAlarm3Status() const
{
    return EventAction::instance()->getAlarmStatus(m_chanCoder, 2);
}
inline QVariant DevChannelMath::getChannelAlarm3Value() const
{
    if (OFF == m_pConfig->expressConfig.expSwtich) {
        return QVariant();
    } else {
        return m_pConfig->alarmConfig.alarmValue[2];
    }
}
inline QVariant DevChannelMath::getChannelAlarm3Color() const
{
    if (OFF == m_pConfig->expressConfig.expSwtich) {
        return QVariant();
    } else {
        quint32 color = m_pConfig->displayConfig.displayAlarmMarkColor[2];
        return QColor((color>>16) & 0xff, (color>>8) & 0xff, color &0xff);
    }
}
inline QVariant DevChannelMath::getChannelAlarm4OnOff() const
{
    if (OFF == m_pConfig->expressConfig.expSwtich) {
        return QVariant();
    } else {
        return m_pConfig->alarmConfig.alarmOnOff[3];
    }
}
inline QVariant DevChannelMath::getChannelAlarm4Type() const
{
    if (OFF == m_pConfig->expressConfig.expSwtich) {
        return QVariant();
    } else {
        return m_pConfig->alarmConfig.alarmType[3];
    }
}
inline QVariant DevChannelMath::getChannelAlarm4Status() const
{
    return EventAction::instance()->getAlarmStatus(m_chanCoder, 3);
}
inline QVariant DevChannelMath::getChannelAlarm4Value() const
{
    if (OFF == m_pConfig->expressConfig.expSwtich) {
        return QVariant();
    } else {
        return m_pConfig->alarmConfig.alarmValue[3];
    }
}
inline QVariant DevChannelMath::getChannelAlarm4Color() const
{
    if (OFF == m_pConfig->expressConfig.expSwtich) {
        return QVariant();
    } else {
        quint32 color = m_pConfig->displayConfig.displayAlarmMarkColor[3];
        return QColor((color>>16) & 0xff, (color>>8) & 0xff, color &0xff);
    }
}
//获取通道报警值（bit0-bit3）
inline QVariant DevChannelMath::getChannelAlarm() const
{
    return GET_DATA_ALARM(*m_pStatus);
}
inline QVariant DevChannelMath::getChannelTagName() const
{
    return QString((const char*)m_pConfig->displayConfig.displayTagChar);
}
inline QVariant DevChannelMath::getChannelTagNumber() const
{
    return QString((const char*)m_pConfig->displayConfig.displayTagNo);
}
inline QVariant DevChannelMath::getChannelNumber() const
{
    return getChannelDefCoder();
}
#endif // DEVCHANNELMATH_H
