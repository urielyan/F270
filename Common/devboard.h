/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：DevBoard.h
 * 概   要：子板卡类，包括物理子板卡（AI/DI/DO/DIO），虚拟子板卡（Math/Comm）,
 * '不包括'扩展板卡
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

#ifndef DEVBOARD_H
#define DEVBOARD_H

#include <QObject>
#include <QList>

#include "channel.h"

class DevBoard : public QObject
{
    Q_OBJECT
public:
    explicit DevBoard(quint8 unit, quint8 module, quint16 status, QObject *parent = 0);
    ~DevBoard();

    /*获取板卡类型：1-AI，2-DI，3-DO，4-DIO，6-COMM，7-MATH*/
    quint8 getBoardType() const {return STATUS_BOARD_TYPE(m_status);}

    quint16 getBoardStatus() const { return m_status;}                                              //获取板卡状态
    quint8 getBoardUnit() const { return m_unit;}                                                       //获取板卡单元号
    quint8 getBoardModule() const { return m_module;}                                           //获取板卡模块号（卡槽号）
    const QList<Channel *> & getBoardChannel() const { return m_channelList;}   //获取板卡通道链表对象引用

    int getBoardDataRegBaseAddr() const { return m_regAddr;}                                //获取板卡数据寄存器地址
    int getBoardBusSlaveAddr() const { return m_slave;}                                             //获取板卡总线从机地址

    void setBoardColdTemperature(float temp) {m_clodTemp = temp;}
    float getBoardColdTemperature() const { return m_clodTemp;}

    void setADMode(Board_ADTime mode) { m_AD_mode = mode;}
    Board_ADTime getADMode() const { return m_AD_mode;}

    void setBurnoutUpperLimit(float upper) { m_burnOutUpperLimit = upper;}
    float getBurnoutUpperLimit() const { return m_burnOutUpperLimit; }

    void setBurnoutDownLimit(float down) { m_burnOutDownLimit = down;}
    float getBurnoutDownLimit() const { return m_burnOutDownLimit;}

    void setDoOutput(quint32 chanNum, bool value);
signals:

public slots:
    void slotConfigChanged();

private:
    const quint16 m_status;   //子板卡状态
    const quint8 m_unit;         //子板卡单元号
    const quint8 m_module;   //子板卡模块号
    QList<Channel *> m_channelList; //子板卡通道链表

    qint32 m_regAddr;  //子板卡数据寄存器起始地址
    qint32 m_slave;        //子板卡从机地址
    quint16 m_config[BOARD_IO_CONFIG_NUM];   //子板卡配置寄存器

    float   m_clodTemp; //AI类型板卡冷端温度测量值

    Board_ADTime    m_AD_mode; //AI类型板卡AD积分模式

    float  m_burnOutUpperLimit;    //断偶上限
    float  m_burnOutDownLimit;     //断偶下限

    friend class AIChannel;
};

#endif // DEVBOARD_H
