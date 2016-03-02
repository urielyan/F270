#ifndef THREADMODBUSSERVERMSG_H
#define THREADMODBUSSERVERMSG_H

#include <QObject>
#include <QTimer>
#include <QThread>


class ThreadModbusServerMsg : public QObject
{
    Q_OBJECT
public:

    static ThreadModbusServerMsg *instance();


private:
    explicit ThreadModbusServerMsg(QObject *parent = 0);
    void init();

signals:
    void sigModbusServerCfgChange();

public slots:
    void slotModbusServerCfgChange(quint32 id);

private:
    QThread m_thread;
};

#define Key_PathName            "/"
#define Key_ProjID                   111
#define TImer_Time                 200
#define MODBUS_CONNECT_LIMIT_MAX 10

typedef enum {
    MsgType_Config = 100,
    MsgType_Opt = 200,
} MsgType;

typedef enum {
    MsgDataType_Config = 0,
    MsgDataType_Opt_Record,   //开始/停止记录：0-读取时始终为0； 1:记录开始；2:记录停止
    MsgDataType_Opt_Computation,  //运算操作：0-读取始终为0; 1:运算开始；2:运算停止；3:运算复位；4:解除运算丢失状态显示
    MsgDataType_Opt_AllAlarmAck,  //解除所有报警：0-读取始终为0；1-解除报警输出
    MsgDataType_Opt_IndividAlarmAck,  //解除单个报警：0-读取始终为0；1-输入输出通道；2:运算通道；3:通信通道
    MsgDataType_Opt_IndividNum,    //解除单个报警通道号码：0-读取始终为0
    MsgDataType_Opt_IndividLevel,   //报警电平号：0-读取始终为0，报警1~4 ACK
    MsgDataType_Opt_Misc,    //杂项操作，0-读取始终为0， 1-手动采样；2-手动触发；3-画面存储；4-执行显示数据强制超时；5-执行事件数据强制超时
    MsgDataType_Opt_SNTP,   //时间同步，读取始终为0， 1-执行手动SNTP
    MsgDataType_Opt_Email,  //Emai,读取始终为0, 1-Email发送开始；2-Email发送停止
    MsgDataType_Opt_Modbus,  //Modbus客户/服务恢复：读取始终为0; 1-手动恢复客户端；2-手动恢复服务端
    MsgDataType_Opt_Timer,       //计时器复位，读取始终为0, 1~4: 复位制定号码的计时器，100:复位所有计时器
    MsgDataType_Opt_MatchTimer,  //匹配计时器复位，读取始终为0, 1~4: 复位制定号码的计时器，100:复位所有计时器
    MsgDataType_Opt_MsgWrite = 19,      //写入信息，读取始终为0, 1-写入预设信息，2-写入自由信息
    MsgDataType_Opt_MsgNum,      //写入信息号，读取始终为0, 预设信息：1~100, 自由信息：1~10
    MsgDataType_Opt_MsgDest,      //信息写入位置，读取始终为0,  0-所有显示组，1~其它：指定组
    MsgDataType_Opt_BatchGrpNum,     //批处理号码，读取始终为0, 1~其它：比处理组号码
} MsgDataType;


typedef struct _NETWORK_CONFIG_SERVER_STRUCT
{
    //Modbus Server
    quint8 serverModbusOnOff;     //Modbus服务开关，0-off, 1-on
    quint16 serverModbusPort;         //Modbus服务端口号, 默认502
    quint8 serverModbusTimeOut; //Modbus服务器延迟响应，单位：ms,范围：0(off)/10ms/20ms/50ms
    quint8 serverModbusLimit;       //Modbus服务连接限制开关，0-off, 1-on
    quint8 serverModbusLimitSwitch[MODBUS_CONNECT_LIMIT_MAX];       //限制开关，
    quint8 serverModbusLimitIP[MODBUS_CONNECT_LIMIT_MAX][4];           //限制IP, IPv4
}NETWORK_CONFIG_SERVER_t ;

typedef struct _MsgData {
    MsgDataType type;

    NETWORK_CONFIG_SERVER_t config;
    qint16 optData;
}MsgData;

typedef struct _StructMSG {
    long msgType;
    MsgData data;
}StructMSG;


class CmdThread : public QObject {

    Q_OBJECT
public :

    CmdThread(QObject *parent = 0);

    void sigAndSlot();

private:

    void init();
    bool createMsg();

signals:

public slots:

    void slotStart();

    void recvMsg();
    void SendMsg();

private:

    key_t m_key;
    int m_msgId;
    QTimer *p_timer;
    StructMSG m_recvData;

};


#endif // THREADMODBUSSERVERMSG_H
