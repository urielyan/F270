/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：ThreadDataSync.h
 * 概   要：数据同步线程。
 *                前置任务：数据处理线程 | 数据通信线程
 *                内容：1.基于前置任务，根据系统配置生成有效的显示数据和事件数据
 *                            2.添加有效的显示数据和事件数据至各自的缓冲区，并更新相应的数据库
 *                            3.根据配置决定是否刷新到数据库
 *
 * 当前版本：V1.0.0
 * 作   者：王 品
 * 完成日期：2015-6-15
 *
 * 修改版本：
 * 修改说明：
 * 修改作者：
 *
 ********************************************************************/
#ifndef THREADDATASYNC_H
#define THREADDATASYNC_H

#include <QObject>
#include <QThread>
#include <QMap>
#include <QQueue>
#include <QMutex>
#include <QTimer>

#include "./GlobalData/cfgGlobalDef.h"
#include "../GlobalData/dataGlobalDef_ModbusReg.h"

#define EVNET_INTERVAL_NUM  17          //事件记录周期个数

#define FRAME_BUFFER_NUM    10          //数据帧最大缓冲数
#define TIMER_INTERVAL            500        //定时器间隔ms

class ThreadDataSync : public QObject
{
    Q_OBJECT
public:
    static ThreadDataSync *instance();
    void setRecordStatus(bool onoff);      //记录状态设定
    bool getRecordStatus();                       //获取记录状态
    void setManualSample();                      //启动手动采样
    quint32 getTrendTimeDiv() { return m_trendTimeDiv;}

    void buffDataFrame(MODBUS_REGISTER_MAP *pData);

signals:
    void sigSyncStopDisp();                         //显示数据停止记录信号
    void sigSyncStopEvent();                        //事件数据停止记录信号
    void sigSyncStop();                                 //数据记录停止信号

private:
    void calculateParameter();   //测量周期、趋势周期、事件记录周期改变处理
    explicit ThreadDataSync(QObject *parent = 0);
    void mapInit();
private:
    QThread m_dataSyncThread;               //数据同步线程对象
    QMap<quint32, quint32> m_event2display;   //"ms" -> "s"
    quint32 m_trendTimeDiv;                       //趋势实际周期s/div
};

/*数据同步工作对象*/
class SyncProcess : public QObject
{
    Q_OBJECT
public:
    explicit SyncProcess(QObject *parent = 0);

public slots:
    void slotSyncProcess();
    void slotSyncDispStoped();
    void slotSyncEventStoped();
    void slotSyncReportStoped();
    void slotStart() {
        connect(p_dataSyncTimer, SIGNAL(timeout()), this, SLOT(slotSyncProcess()));
        p_dataSyncTimer->start(TIMER_INTERVAL);
    }

signals:

private:
    void dataBufferProcess(bool first, quint8 count); //显示、事件数据处理
    void dataSampleProcess();  //采样数据处理
    void dataReportProcess();   //报表数据处理

    void sendReportDataSingle(); //单次数据报发送
    void sendReportDataWhole();//完整数据报发送

    void transAlarmStatus(quint32 chanCoder, quint8 alarmIndex);
    void chanAlarmStatusTrans();
    void chanAlarmOutput();

    void cpyBufferData();
    quint8 m_dataBufferCount;
    MODBUS_REGISTER_MAP m_dataTemp[FRAME_BUFFER_NUM];
    QTimer *p_dataSyncTimer;                   //同步工作驱动定时器
};

#endif // THREADDATASYNC_H
