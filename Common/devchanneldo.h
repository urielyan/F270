/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：DevChannelDO.h
 * 概   要：DO通道实现类，继承类Channel, 提供DO通道的函数接口，以获取该通道的相关
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
#ifndef DEVCHANNELDO_H
#define DEVCHANNELDO_H

#include "channel.h"
#include "eventaction.h"

#include <QColor>

class DevChannelDO : public Channel
{
    Q_OBJECT
public:
    explicit DevChannelDO(quint32 chanCoder, QObject *parent = 0);

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
    void slotChanConfigChanged() {}      //通道配置变更

private:
    qint16 *m_pData;                           //DO通道数据对象指针
    qint16 *m_pStatus;                       //DO通道状态指针
    DO_CONFIG *m_pConfig;             //DO通道配置指针

    /*计算相关*/
    qint16 *p_curData;                      //当前测量值指针
};

/*获取当前测量值指针*/
inline QVariant DevChannelDO::getChannelCurData() const
{
    return *p_curData;
}
/*获取通道配置指针*/
inline void * DevChannelDO::getChannelConfig() const
{
    return (void*) m_pConfig;
}

/*获取通道数据*/
inline QVariant DevChannelDO::getChannelData() const
{
    return *m_pData;
}
/*返回通道标记*/
inline QVariant DevChannelDO::getChannelTag() const
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
inline QVariant DevChannelDO::getChannelColor() const
{
    quint32 color = m_pConfig->displayConfig.displayColor;
    return QColor((color>>16) & 0xff, (color>>8) & 0xff, color &0xff);
}

/*获取通道单位*/
inline QVariant DevChannelDO::getChannelUnit() const
{
    return QString((char *)m_pConfig->rangeConfig.rangeUnit);
}

/*获取通道量程类型*/
inline QVariant DevChannelDO::getChannelRangeType() const
{
    return m_pConfig->rangeConfig.rangeType;
}

/*获取通道量程实际上限*/
inline QVariant DevChannelDO::getChannelRangeUpper() const
{
    return m_pConfig->rangeConfig.rangeDispMax;
}

/*获取通道量程实际下限*/
inline QVariant DevChannelDO::getChannelRangeDowner() const
{
    return m_pConfig->rangeConfig.rangeDispMin;
}

/*获取通道状态*/
inline QVariant DevChannelDO::getChannelStatus() const
{
    return GET_DATA_STATUS(*m_pStatus);
}

/*获取通道数值精度*/
inline QVariant DevChannelDO::getChannelDataAccuracy() const
{
    QString ret = "";
    if (m_pConfig->displayConfig.displayDI0[0] == '\0' ) {
        ret.append("0");
    } else{
        ret.append((const char *)m_pConfig->displayConfig.displayDI0);
    }
    ret.append("|");
    if (m_pConfig->displayConfig.displayDI1[0] == '\0'){
        ret.append("1");
    } else {
        ret.append((const char *)m_pConfig->displayConfig.displayDI1);
    }
    return ret;
}

/*获取通道测量校正的点数*/
inline QVariant DevChannelDO::getChannelCaliCorrectNum() const
{
    return QVariant();
}

/*获取通道显示区域上限(%)*/
inline QVariant DevChannelDO::getChannelDispZoneUpper() const
{
    return m_pConfig->displayConfig.displayZoneUpper;
}

/*获取通道显示区域下限(%)*/
inline QVariant DevChannelDO::getChannelDispZoneDowner() const
{
    return m_pConfig->displayConfig.dispalyZoneLower;
}

/*获取通道标尺位置*/
inline QVariant DevChannelDO::getChannelScalePos() const
{
    return m_pConfig->displayConfig.displayScalePos;
}

/*获取通道标尺分割数*/
inline QVariant DevChannelDO::getChannelScaleDiv() const
{
    return QVariant();
}

/*获取通道棒图基准位置*/
inline QVariant DevChannelDO::getChannelBarBase() const
{
    return m_pConfig->displayConfig.displayBarPos;
}

/*获取通道帮图分割数*/
inline QVariant DevChannelDO::getChannelBarDiv() const
{
    return QVariant();
}

/*获取通道部分压缩放大开关: 0-off, 1-on*/
inline QVariant DevChannelDO::getChannelPartScaleOnOff() const
{
    return QVariant();
}

/*获取通道部分压缩放大位置，1-99, 单位：%，默认50*/
inline QVariant DevChannelDO::getChannelPartScalePos() const
{
    return QVariant();
}

/*获取通道部分压缩放大边界值*/
inline QVariant DevChannelDO::getChannelPartScaleBound() const
{
    return QVariant();
}

/*获取通道彩色标尺带模式， 0-off, 1-内侧， 2-外侧.*/
inline QVariant DevChannelDO::getChannelColorScalePos() const
{
    return QVariant();
}
/*获取通道彩色标尺带颜色*/
inline QVariant DevChannelDO::getChannelColorScaleColor() const
{
    return QVariant();
}
/*获取通道彩色标尺带显示位置上限*/
inline QVariant DevChannelDO::getChannelColorScaleUpper() const
{
    return QVariant();
}
/*获取通道彩色标尺带显示位置下限*/
inline QVariant DevChannelDO::getChannelColorScaleDowner() const
{
    return QVariant();
}

/*获取通道报警标记开关，0-0ff, 1-on*/
inline QVariant DevChannelDO::getChannelAlarmOnOff() const
{
    return QVariant();
}
/*获取通道报警标记类型，0-报警， 1-固定*/
inline QVariant DevChannelDO::getChannelAlarmType() const
{
    return QVariant();
}
inline QVariant DevChannelDO::getChannelAlarm1Type() const
{
    return QVariant();
}
inline QVariant DevChannelDO::getChannelAlarm1OnOff() const
{
    return QVariant();
}
inline QVariant DevChannelDO::getChannelAlarm1Status() const
{
    return EventAction::instance()->getAlarmStatus(m_chanCoder, 0);
}
inline QVariant DevChannelDO::getChannelAlarm1Value() const
{
    return QVariant();
}
inline QVariant DevChannelDO::getChannelAlarm1Color() const
{
    return QVariant();
}
inline QVariant DevChannelDO::getChannelAlarm2OnOff() const
{
    return QVariant();
}
inline QVariant DevChannelDO::getChannelAlarm2Type() const
{
    return QVariant();
}
inline QVariant DevChannelDO::getChannelAlarm2Status() const
{
    return EventAction::instance()->getAlarmStatus(m_chanCoder, 1);
}
inline QVariant DevChannelDO::getChannelAlarm2Value() const
{
    return QVariant();
}
inline QVariant DevChannelDO::getChannelAlarm2Color() const
{
    return QVariant();
}
inline QVariant DevChannelDO::getChannelAlarm3OnOff() const
{
    return QVariant();
}
inline QVariant DevChannelDO::getChannelAlarm3Type() const
{
    return QVariant();
}
inline QVariant DevChannelDO::getChannelAlarm3Status() const
{
    return EventAction::instance()->getAlarmStatus(m_chanCoder, 2);
}
inline QVariant DevChannelDO::getChannelAlarm3Value() const
{
    return QVariant();
}
inline QVariant DevChannelDO::getChannelAlarm3Color() const
{
    return QVariant();
}
inline QVariant DevChannelDO::getChannelAlarm4OnOff() const
{
    return QVariant();
}
inline QVariant DevChannelDO::getChannelAlarm4Type() const
{
    return QVariant();
}
inline QVariant DevChannelDO::getChannelAlarm4Status() const
{
    return EventAction::instance()->getAlarmStatus(m_chanCoder, 3);
}
inline QVariant DevChannelDO::getChannelAlarm4Value() const
{
    return QVariant();
}
inline QVariant DevChannelDO::getChannelAlarm4Color() const
{
    return QVariant();
}
//获取通道报警值（bit0-bit3）
inline QVariant DevChannelDO::getChannelAlarm() const
{
    return GET_DATA_ALARM(*m_pStatus);
}
inline QVariant DevChannelDO::getChannelTagName() const
{
    return QString((const char*)m_pConfig->displayConfig.displayTagChar);
}
inline QVariant DevChannelDO::getChannelTagNumber() const
{
    return QString((const char*)m_pConfig->displayConfig.displayTagNo);
}
inline QVariant DevChannelDO::getChannelNumber() const
{
    return getChannelDefCoder();
}
#endif // DEVCHANNELDO_H
