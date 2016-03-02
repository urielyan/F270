/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：ThreadDataProcess.h
 * 概   要：数据处理线程，基于Modbus协议，对板内RS485总线上的子板卡进行数据的
 * 查询，并根据通道配置做简单的运算，最后将数据各通道数据映射到原始数据对象中。
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

#ifndef THREADDATAPROCESS_H
#define THREADDATAPROCESS_H

#include <QObject>
#include <QThread>
#include <QQueue>
#include <QTimer>
#include <QSocketNotifier>
#include <QBitArray>
#include "./GlobalData/cfgGlobalDef.h"
#include "../GlobalData/dataGlobalDef_ModbusReg.h"

#include "../Common/dataqueryrtd.h"
#include "../Common/dataquerytc.h"
#include "../Common/mathexpanalysis.h"

#include "../GlobalData/cfgchannel.h"

#include <modbus/modbus.h>

#include "boardinterface.h"

typedef struct SUB_BOARD_DATA_PROCESSED_STRUCT{
    qint16 dataStatus[UNIT_CHAN_NUM_USE_MAX];        //通道状态
    float  dataSample[UNIT_CHAN_NUM_USE_MAX];        //AI类型板卡采样数据输入
    quint32 dataDICount[UNIT_CHAN_NUM_USE_MAX];  //DI脉冲类型脉冲计数

    quint16  bitsInputDI[UNIT_CHAN_NUM_USE_MAX];//DI输入值
    quint16  bitsStatusDI[UNIT_CHAN_NUM_USE_MAX];//DI状态值
    quint16  bitsInputDO[UNIT_CHAN_NUM_USE_MAX];//DO输入值
    quint16  bitsStatusDO[UNIT_CHAN_NUM_USE_MAX];//DO状态值
}SUB_BOARD_DATA_PROCESSED;

/*子板卡数据处理线程对象*/
class ThreadDataProcess : public QObject
{
    Q_OBJECT
public:
    static ThreadDataProcess *instance();
    void setMathStatus(Math_Status_Type status);        //运算状态设定
    Math_Status_Type getMathStatus();                           //获取运算状态
    void changeMeasurePeriod();                                    //测量周期变更, 定时驱动DataProcess对象进行数据查询

signals:
    void sigDataSampleStop();                               //数据采样停止信号
    void sigProcessDataPeriodly();

    void sigBoardStatusQuery();                             //子板状态查询信号
    void sigRelayDoOutput(quint32 coder, bool value);   //DO输出设置

public slots:

private:
    explicit ThreadDataProcess(QObject *parent = 0);
    BoardInterface *getBoardInterface();

private:
    QThread m_dataProcessThread;

    friend class DevBoard;
};

/*子板卡数据处理工作对象*/
class DataProcess : public QObject
{
    Q_OBJECT

public:
    explicit DataProcess(QObject *parent = 0);

public slots:
    void slotStart();
    void slotTimerStart();
    void slotTimerStop();
    int slotDataQuery();
    int slotStatusQuery();
    void slotBoardChanged();
    void slotRelayDOOutput(quint32 chanCoder, bool value);  //DO输出配置
    void slotTimerTrigger(int i);
    void slotTimerMatchTrigger(int i);

signals:
    void sigBoardPluginout(quint8 module, bool in);

private:
    void dataInputProcess(SUB_BOARD_DATA *dataSrc, SUB_BOARD_DATA_PROCESSED *dataDest);
    void dataInputRemap(quint8 unit, quint8 module, quint16 status, SUB_BOARD_DATA_PROCESSED *pData);

    bool boardStatusCheck(quint16 *pConfigStatus);
    void boardConfigUpdate(quint8 unit, quint8 module);
    void boardReset(quint8 unit, quint8 module, quint16 status);
    void configReset(quint8 unit, quint8 module, quint16 status);

private:
    bool loadPlugin();
    bool readBoardValidFlag();
    bool updateBoardStatus(quint8 unit, quint8 module);
    bool checkBoardStatus(quint8 unit, quint8 module);

private:
    QTimer *p_dataQueryTimer;               //数据查询定时器
    qint16 m_extModuleSlave;                   //扩展板卡slave地址，默认:-1
    bool m_newExtBoard;                          //动态扩展板卡插入标记
    QFile m_miscDev;                                 //板卡状态查询设备节点
    QSocketNotifier *p_notifier;                //MISC设备节点通知socket
    QBitArray  m_boardValidCur;             //当前本机板卡有效标记位数组
    QBitArray  m_boardValidPre;              //之前本机板卡有效标记位数组
    QTimer *p_boardPowerOn;                //子板卡启动定时器
};

#endif // THREADDATAPROCESS_H
