#include "threadmodbusservermsg.h"
#include "../GlobalData/globalDef.h"

#include <QDebug>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

ThreadModbusServerMsg *ThreadModbusServerMsg::instance()
{
    static ThreadModbusServerMsg data;
    return &data;
}

ThreadModbusServerMsg::ThreadModbusServerMsg(QObject *parent) :
    QObject(parent)
{
    init();
}

void ThreadModbusServerMsg::init()
{
    CmdThread *msgCmd = new CmdThread;
    msgCmd->moveToThread(&m_thread);

    connect(CfgChannel::instance(), SIGNAL(sigNormalConfigChanged(quint32)), this, SLOT(slotModbusServerCfgChange(quint32)));
    connect(this, SIGNAL(sigModbusServerCfgChange()), msgCmd, SLOT(SendMsg()));
    m_thread.start(QThread::TimeCriticalPriority);
    msgCmd->slotStart();
    msgCmd->SendMsg(); // UI进程运行之后就发送当前与Modbus服务器相关的配置给服务器进程，作为服务器的配置初始化
}

void ThreadModbusServerMsg::slotModbusServerCfgChange(quint32 id)
{
    if (id == ID_NetWork_Server || id == ID_Serial)
        emit sigModbusServerCfgChange();
}


CmdThread::CmdThread(QObject *parent) :
    QObject(parent)
{

}

void CmdThread::sigAndSlot()
{
    connect(p_timer, SIGNAL(timeout()), this, SLOT(recvMsg()), Qt::DirectConnection);
}

void CmdThread::init()
{
    p_timer = new QTimer;

    sigAndSlot();
    if (createMsg()) {
        p_timer->start(TImer_Time);
    }
    else {
        qDebug()<<"CmdThread fail!";
    }
}

bool CmdThread::createMsg()
{
    m_key = ftok(Key_PathName, Key_ProjID);

    if (m_key == -1) {
        qDebug()<<"msg key fail!";
        return false;
    }

    m_msgId = msgget(m_key,  IPC_CREAT | 0664);

    if (m_msgId < 0) {
        qDebug()<<"msg create fail!";
        if (errno == EEXIST) {
            qDebug()<<"msg create exist";
            m_msgId = msgget(m_key, 0664);
        }
        else
            qDebug()<<"mesget error!";
        return false;
    }
    else {
        qDebug()<<"msg create success!";
    }
    qDebug()<<m_msgId;
    return true;
}

void CmdThread::slotStart()
{
    init();
}

void CmdThread::recvMsg()
{
    //采用定时非阻塞的模式收取消息队列的数据
    if (msgrcv(m_msgId, (void *)(&m_recvData), sizeof(StructMSG) - sizeof(long), MsgType_Opt, IPC_NOWAIT) < 0) {
//        qDebug()<<"msgrecv error!!";
    }
    else {
        qint16 data = m_recvData.data.optData;
        switch (m_recvData.data.type) {
        case MsgDataType_Opt_Record:
        {

            break;
        }
        case MsgDataType_Opt_Computation:
        {

            break;
        }
        case MsgDataType_Opt_AllAlarmAck:
        {

            break;
        }
        case MsgDataType_Opt_IndividAlarmAck:
        {

            break;
        }
        case MsgDataType_Opt_IndividNum:
        {

            break;
        }
        case MsgDataType_Opt_IndividLevel:
        {

            break;
        }
        case MsgDataType_Opt_Misc:
        {

            break;
        }
        case MsgDataType_Opt_SNTP:
        {

            break;
        }
        case MsgDataType_Opt_Email:
        {

            break;
        }
        case MsgDataType_Opt_Modbus:
        {

            break;
        }
        case MsgDataType_Opt_Timer:
        {

            break;
        }
        case MsgDataType_Opt_MatchTimer:
        {

            break;
        }
        case MsgDataType_Opt_MsgWrite:
        {

            break;
        }
        case MsgDataType_Opt_MsgNum:
        {

            break;
        }
        case MsgDataType_Opt_MsgDest:
        {

            break;
        }
        case MsgDataType_Opt_BatchGrpNum:
        {

            break;
        }
        default:
            break;
        }
    }
}

void CmdThread::SendMsg()
{
    m_recvData.msgType = MsgType_Config;
    m_recvData.data.type = MsgDataType_Config;

    m_recvData.data.config.serverModbusOnOff = DevConfigPtr->networkServer.serverModbusOnOff;
    m_recvData.data.config.serverModbusTimeOut = DevConfigPtr->networkServer.serverModbusTimeOut;
    m_recvData.data.config.serverModbusPort = DevConfigPtr->networkServer.serverModbusPort;
    m_recvData.data.config.serverModbusLimit = DevConfigPtr->networkServer.serverModbusLimit;
    for (int i = 0; i < MODBUS_CONNECT_LIMIT_MAX; ++i) {
        m_recvData.data.config.serverModbusLimitSwitch[i] = DevConfigPtr->networkServer.serverModbusLimitSwitch[i];
        for (int j = 0; j < 4; ++j) {
            m_recvData.data.config.serverModbusLimitIP[i][j] = DevConfigPtr->networkServer.serverModbusLimitIP[i][j];
        }
    }

    //T0D0
    if (msgsnd(m_msgId, (void *)&m_recvData, sizeof(StructMSG) - sizeof(long), 0) < 0) {
        qDebug()<<"msgsend error!!";
    }
    else {

    }
}
