/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：ThreadDataComm.h
 * 概   要：数据通信线程，即运行在网络及串口上的Modbus客户端程序，按用户设定的
 * 命令执行各个Modbus命令，从目标从机（服务器）上获取数据，并映射到原始数据对
 * 象中。
 *
 * 当前版本：V1.0.0
 * 作   者：马志刚
 * 完成日期：2015-9-6
 *
 * 修改版本：
 * 修改说明：
 * 修改作者：
 *
 ********************************************************************/
#ifndef THREADDATACOMM_H
#define THREADDATACOMM_H

#include <QObject>
#include <QThread>
#include <QSharedMemory>
#include <QTimer>
#include <QMutex>
#include <modbus/modbus.h>
#include <sys/types.h>
#include <sys/socket.h>


#include "./GlobalData/cfgGlobalDef.h"
#include "../GlobalData/dataGlobalDef_ModbusReg.h"
#include "GlobalData/cfgchannel.h"


typedef enum _ModbusStatus{
    ModbusStatus_Valid = 0, //已正常建立Modbus 通信。

    ModbusStatus_Start = 10,   //Modbus 通信已开始。
    ModbusStatus_Stop,    //Modbus 通信已停止。
    ModbusStatus_Dropout,   //不能在指定通信周期内发送全部命令。请重新检查Modbus 命令数和波特率等设定。

    ModbusStatus_Invalid = 20,  //不能执行命令。
    ModbusStatus_Waiting,   //与外部设备的通信受阻，等待恢复。请检查与外部设备的连接。
    ModbusStatus_CLosed,   //Modbus 通信已停止，与外部设备的连接已关闭。
    ModbusStatus_Resolving,  //正在建立与服务器/ 从机的连接( 地址解析中)。
    ModbusStatus_Connecting,   //正在建立与服务器/ 从机的连接( 连接请求中)
    ModbusStatus_Unreach,   //网络中找不到外部设备。请检查Modbus 服务器的地址设定及以太网电缆的连接。
    ModbusStatus_Timeout,  //等待外部设备的响应超时。请检查与外部设备的连接。
    ModbusStatus_Nodata,  //还未曾取得过数据。请检查通信设置。

    ModbusStatus_ErrFc = 30,
    ModbusStatus_ErrAddr,
    ModbusStatus_ErrValue,
    ModbusStatus_ErrDevice,
    ModbusStatus_Ack,
    ModbusStatus_Busy,

}ModbusStatus;

typedef struct Modbus_Summary_Data_Struct
{
    quint8      cmdNum; // 命令号
    quint8      cmdType;  //命令类型
    quint32     cmdStartNo;  //开始通道
    quint32     cmdEndNo;  //结束通道
    quint32	    cmdRegNo;  //寄存器号
    quint8      cmdServeCell;  //在以太网中用作单元号，在串口中用作从机地址
    Register_Type cmdRegType;  //数据类型
    ModbusStatus			status;   //在以太网中保留不使用，在串口中用作从机状态显示
}Modbus_Summary_Data;

typedef struct Modbus_Summary_Struct
{
    QString     		serverName;   //服务器名
    ModbusStatus			status;  //服务器状态
    QList<Modbus_Summary_Data>  dataList;  //配置链表
}Modbus_Summary;

class ThreadEthernetModbusClient;

class ThreadDataComm : public QObject
{
    Q_OBJECT
public:
    static ThreadDataComm *instance();

signals:
    void sigServerStart();//服务器开始信号
    void sigServerRestart();//服务器重启信号
public slots:
    void slotServerConfigChange();//服务器设定改变槽信号
private:
    explicit ThreadDataComm(QObject *parent = 0);
    void initCOMMChan();

public:
    ThreadEthernetModbusClient *tcpclient;
    QThread m_thread;

};


#define HEARTBEATTIME 500 //网络状态查询周期

//寄存器号用于和寄存器结构体映射获取地址
/******input register*******/
#define INPUTREG_MAIN_IO_START              300001
#define INPUTREG_MAIN_IO_END                303000

#define INPUTREG_MAIN_IO_C_START            304001
#define INPUTREG_MAIN_IO_C_END              304600

#define INPUTREG_MAIN_OPERATE_START         305001
#define INPUTREG_MAIN_OPERATE_END           306600

#define INPUTREG_MAIN_STATUS_START          308001
#define INPUTREG_MAIN_STATUS_END            308007
/***input ext1**/
#define INPUTREG_EXPAND_1_1_START           310001
#define INPUTREG_EXPAND_1_1_END             311000
#define INPUTREG_EXPAND_1_2_START           311001
#define INPUTREG_EXPAND_1_2_END             312000
#define INPUTREG_EXPAND_1_3_START           312001
#define INPUTREG_EXPAND_1_3_END             312500
#define INPUTREG_EXPAND_1_4_START           312501
#define INPUTREG_EXPAND_1_4_END             313000
/***input ext2**/
#define INPUTREG_EXPAND_2_1_START           315001
#define INPUTREG_EXPAND_2_1_END             316000
#define INPUTREG_EXPAND_2_2_START           316001
#define INPUTREG_EXPAND_2_2_END             317000
#define INPUTREG_EXPAND_2_3_START           317001
#define INPUTREG_EXPAND_2_3_END             317500
#define INPUTREG_EXPAND_2_4_START           317501
#define INPUTREG_EXPAND_2_4_END             318000
/***input ext3**/
#define INPUTREG_EXPAND_3_1_START           320001
#define INPUTREG_EXPAND_3_1_END             321000
#define INPUTREG_EXPAND_3_2_START           321001
#define INPUTREG_EXPAND_3_2_END             322000
#define INPUTREG_EXPAND_3_3_START           322001
#define INPUTREG_EXPAND_3_3_END             322500
#define INPUTREG_EXPAND_3_4_START           322501
#define INPUTREG_EXPAND_3_4_END             323000
/***input ext4**/
#define INPUTREG_EXPAND_4_1_START           325001
#define INPUTREG_EXPAND_4_1_END             326000
#define INPUTREG_EXPAND_4_2_START           326001
#define INPUTREG_EXPAND_4_2_END             327000
#define INPUTREG_EXPAND_4_3_START           327001
#define INPUTREG_EXPAND_4_3_END             327500
#define INPUTREG_EXPAND_4_4_START           327501
#define INPUTREG_EXPAND_4_4_END             328000
/***input ext5**/
#define INPUTREG_EXPAND_5_1_START           330001
#define INPUTREG_EXPAND_5_1_END             331000
#define INPUTREG_EXPAND_5_2_START           331001
#define INPUTREG_EXPAND_5_2_END             332000
#define INPUTREG_EXPAND_5_3_START           332001
#define INPUTREG_EXPAND_5_3_END             332500
#define INPUTREG_EXPAND_5_4_START           332501
#define INPUTREG_EXPAND_5_4_END             333000
/***input ext6**/
#define INPUTREG_EXPAND_6_1_START           335001
#define INPUTREG_EXPAND_6_1_END             336000
#define INPUTREG_EXPAND_6_2_START           336001
#define INPUTREG_EXPAND_6_2_END             337000
#define INPUTREG_EXPAND_6_3_START           337001
#define INPUTREG_EXPAND_6_3_END             337500
#define INPUTREG_EXPAND_6_4_START           337501
#define INPUTREG_EXPAND_6_4_END             338000

/*******hold register***/
#define HOLDREG_MAIN_IO_START               400001
#define HOLDREG_MAIN_IO_END                 403000

#define HOLDREG_MAIN_IO_C_START             404001
#define HOLDREG_MAIN_IO_C_END               404600

#define HOLDREG_MAIN_COMM_START             405001
#define HOLDREG_MAIN_COMM_END               408000

#define HOLDREG_MAIN_SET_START              408001
#define HOLDREG_MAIN_SET_END                408100

#define HOLDREG_MAIN_STATUS_START           409001
#define HOLDREG_MAIN_STATUS_END             410000
/***hold ext1 **/
#define HOLDREG_EXPAND_1_1_START            410001
#define HOLDREG_EXPAND_1_1_END              411000
#define HOLDREG_EXPAND_1_2_START            411001
#define HOLDREG_EXPAND_1_2_END              412000
#define HOLDREG_EXPAND_1_3_START            412001
#define HOLDREG_EXPAND_1_3_END              412500
#define HOLDREG_EXPAND_1_4_START            412501
#define HOLDREG_EXPAND_1_4_END              413000
/***hold ext2 **/
#define HOLDREG_EXPAND_2_1_START            415001
#define HOLDREG_EXPAND_2_1_END              416000
#define HOLDREG_EXPAND_2_2_START            416001
#define HOLDREG_EXPAND_2_2_END              417000
#define HOLDREG_EXPAND_2_3_START            417001
#define HOLDREG_EXPAND_2_3_END              417500
#define HOLDREG_EXPAND_2_4_START            417501
#define HOLDREG_EXPAND_2_4_END              418000
/***hold ext3 **/
#define HOLDREG_EXPAND_3_1_START            420001
#define HOLDREG_EXPAND_3_1_END              421000
#define HOLDREG_EXPAND_3_2_START            421001
#define HOLDREG_EXPAND_3_2_END              422000
#define HOLDREG_EXPAND_3_3_START            422001
#define HOLDREG_EXPAND_3_3_END              422500
#define HOLDREG_EXPAND_3_4_START            422501
#define HOLDREG_EXPAND_3_4_END              423000
/***hold ext4 **/
#define HOLDREG_EXPAND_4_1_START            425001
#define HOLDREG_EXPAND_4_1_END              426000
#define HOLDREG_EXPAND_4_2_START            426001
#define HOLDREG_EXPAND_4_2_END              427000
#define HOLDREG_EXPAND_4_3_START            427001
#define HOLDREG_EXPAND_4_3_END              427500
#define HOLDREG_EXPAND_4_4_START            427501
#define HOLDREG_EXPAND_4_4_END              428000
/***hold ext5 **/
#define HOLDREG_EXPAND_5_1_START            430001
#define HOLDREG_EXPAND_5_1_END              431000
#define HOLDREG_EXPAND_5_2_START            431001
#define HOLDREG_EXPAND_5_2_END              432000
#define HOLDREG_EXPAND_5_3_START            432001
#define HOLDREG_EXPAND_5_3_END              432500
#define HOLDREG_EXPAND_5_4_START            432501
#define HOLDREG_EXPAND_5_4_END              433000
/***hold ext6 **/
#define HOLDREG_EXPAND_6_1_START            435001
#define HOLDREG_EXPAND_6_1_END              436000
#define HOLDREG_EXPAND_6_2_START            436001
#define HOLDREG_EXPAND_6_2_END              437000
#define HOLDREG_EXPAND_6_3_START            437001
#define HOLDREG_EXPAND_6_3_END              437500
#define HOLDREG_EXPAND_6_4_START            437501
#define HOLDREG_EXPAND_6_4_END              438000

/****coil****/
#define COIL_IO_START                       1
#define COIL_IO_END                         1300

#define COIL_COMM_STATUS_START              11001
#define COIL_COMM_STATUS_END                11100

#define COIL_SWITCH_START                   2001
#define COIL_SWITCH_END                     2100

/********relay*****/
#define RELAY_1_START                       100001
#define RELAY_1_END                         101100

#define RELAY_2_START                       101500
#define RELAY_2_END                         101600

//服务器的结构体
typedef struct _ServerTCP{
    quint8   serverNo; //服务器号
    modbus_t *ctx;     //modbus 结构体
    quint8 cmd[100];   //服务器需要执行的命令号
    quint8 cmdSize;    //命令数目
    bool isRecover;    //服务器是否在进行恢复动作
    bool isConnect;    //服务器是否正在连接中
    bool state;        //服务器的网络状态
}ServerTCP;

class ThreadEthernetModbusClient : public QObject {

    Q_OBJECT

public:
    ThreadEthernetModbusClient(QObject *parent = 0);

    void init();          //初始化函数
    void serverAndCmdPacket();  //将服务器与命令进行对应分类
    void newModbusServer();     //new出各个modbus服务器

    void handleCmd(int ID, modbus_t *ctx, int cmdNo);  //处理命令
    void readAction(int ID, modbus_t *ctx, int cmdNo); //处理读命令
    void writeAction(int ID, modbus_t *ctx, int cmdNo);//处理些命令
    int getRegisterMapAddr(quint32 regNo);     //根据寄存器号获取寄存器结构体中的地址
    void writeDataToCommChanel(int cmdNo, quint8 *data_b, quint16 *data_w, int nb, int flag); //将读取到的数据写入到通信通道中
    void commAlarmHandle(int cmdNo); //通信通道报警处理
    void handleWriteAction(int ID, modbus_t *ctx, int cmdNo, int addr, quint8 *data_b, float *data_f, int nb, int flag);
    void closeServer();              //关闭服务器，释放资源

    void setOutLogAndInfo(int ID, int rc);

    QList<Modbus_Summary> getModbusSummary();
signals:

    void sigModbusSummaryChange(int sigNo = -1);

public slots:
    void slotServerStart();
    void slotServerRestart(quint32 ID);
    void slotHeartBeat();
    void slotSimple();
    void slotHold();
    void slotrecoverConnect();

private:
    ServerTCP tempServer[32];
    QList<ServerTCP *> m_serverList;  //服务器队列
    QTimer *p_heartBeatTimer;         //心跳测试定时器，定时检查网络状况
    QTimer *p_simpleTimer;            //采样定时器
    QTimer *p_holdTimer;              //保持连接定时器
    QList<ServerTCP *> m_needHoldConnectServer;  //需要进行保持连接的服务器队列
    QList<ServerTCP *> m_needRecoverServer;      //需要进行恢复网络连接的服务器队列
    QMutex m_mutex;                   //原子锁
    int m_cmdTotal;

    QList<Modbus_Summary> m_summaryList;

};

#if 1

#define SERIAL_PORT_OF_HOST    "/dev/ttyUSB0"

//串口命令的结构体
typedef struct _ServerCOM{
    int cmdNo;   //命令号
    bool state;  //从机连接状态
    bool recoverState;  //恢复状态
    quint32 recoverTime;    //恢复动作次数
}ServerCOM;

class ThreadCOMModbusClient : public QObject {

    Q_OBJECT

public:
    ThreadCOMModbusClient(QObject *parent = 0);
    void init();          //初始化函数
    void getVaildCmdNo();  //获取有效命令
    int newModbusServer();     //new出modbus服务器

    int handleCmd(int cmdNo);  //处理命令
    int readAction(int cmdNo); //处理读命令
    int writeAction(int cmdNo);//处理写命令
    int getRegisterMapAddr(quint32 regNo);     //根据寄存器号获取寄存器结构体中的地址
    void writeDataToCommChanel(int cmdNo, quint8 *data_b, quint16 *data_w, int nb, int flag); //将读取到的数据写入到通信通道中
    void commAlarmHandle(int cmdNo); //通信通道报警处理
    int handleWriteAction(modbus_t *ctx, int cmdNo, int addr, quint8 *data_b, float *data_f, int nb, int flag);
    void closeServer();              //关闭服务器，释放资源

    void setOutLogAndInfo(int ID, int rc);

signals:

    void sigModbusCommSummaryChange(int sigNo = -1);

public slots:
    void slotServerStart();
    void slotServerRestart(quint32 ID);
    void slotSimple();
    void slotBps();
    void slotrecover();

private:
    modbus_t *p_ctx;                 //modbus结构体

    ServerCOM serialCmd[100];

    QList<ServerCOM*> m_cmdNoList;   //有效命令队列
    QList<ServerCOM*> m_recoverList; //恢复命令的队列

    QTimer *p_recoverTimer;         //心跳测试定时器，定时检查通信状况
    QTimer *p_simpleTimer;            //采样定时器
    QTimer *p_bpsTimer;              //1S定时器，判断串口数据流的长度是否超过波特率的限定

    quint32 m_totalDataSize;         //串口1S中，数据流的长度
    quint32 m_limitDataSize;         //波特率限定数据流的长度
    qint8 m_currentCmd;              //当前执行的命令号
    qint8 m_cmdNumHInterval;         //在有命令间隔的情况下，采样周期里执行命令的数量
    bool m_overLimit;

    QList<Modbus_Summary_Data> m_summaryList;
};
#endif

#endif // THREADDATACOMM_H
