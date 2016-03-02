#include "threaddatacomm.h"
#include "../GlobalData/globalDef.h"
#include "../Common/logger.h"

#include <QDateTime>
#include <unistd.h>



extern MODBUS_REGISTER_MAP  g_originDataMap;   //全局变量，原始数据对象

/*
 * 功能：
 *  创建单例
 * 参数：
 *
 * 返回值：
 * 	  无
 */
ThreadDataComm *ThreadDataComm::instance()
{
    static ThreadDataComm data;
    return &data;
}

/*
 * 功能：
 *  配置改变槽函数
 * 参数：
 *
 * 返回值：
 * 	  无
 */
void ThreadDataComm::slotServerConfigChange()
{
    emit sigServerRestart();//发送重启功能信号
}

/*
 * 功能：
 *  构造函数
 * 参数：
 *
 * 返回值：
 * 	  无
 */
ThreadDataComm::ThreadDataComm(QObject *parent) :
    QObject(parent)
{
    tcpclient = new ThreadEthernetModbusClient;
    tcpclient->moveToThread(&m_thread);
    connect(this, SIGNAL(sigServerStart()), tcpclient, SLOT(slotServerStart()));
    connect(CfgChannel::instance(), SIGNAL(sigNormalConfigChanged(quint32)), tcpclient, SLOT(slotServerRestart(quint32)));

//    ThreadCOMModbusClient *comClient = new ThreadCOMModbusClient;
//    comClient->moveToThread(&m_thread);
//    connect(this, SIGNAL(sigServerStart()), comClient, SLOT(slotServerStart()));
//    connect(CfgChannel::instance(), SIGNAL(sigNormalConfigChanged(quint32)), comClient, SLOT(slotServerRestart(quint32)));

    initCOMMChan();

    m_thread.start(QThread::TimeCriticalPriority);

    emit sigServerStart();
}

void ThreadDataComm::initCOMMChan()
{
    for(int i = 0; i < MAIN_UNIT_COMM_CHAN_NUM_PRE; ++i) {
        if (DevConfigPtr->chanComm[i].rangeConfig.rangeSwtich == 1) {
            if (DevConfigPtr->chanComm[i].rangeConfig.rangePowerOnVal == 1) {
                DevConfigPtr->chanComm[i].rangeConfig.rangePreVal = 0;
            }
        }
    }
}

/**************************Ethernet modbus client*************************************************/

/*
 * 功能：
 *  构造函数
 * 参数：
 *
 * 返回值：
 * 	  无
 */
ThreadEthernetModbusClient::ThreadEthernetModbusClient(QObject *parent) :
    QObject(parent)
{
    //new出检查网络状态定时器，并绑定槽函数
    p_heartBeatTimer = new QTimer;
    connect(p_heartBeatTimer, SIGNAL(timeout()), this, SLOT(slotHeartBeat()), Qt::DirectConnection);

    //new出采样定时器，并绑定槽函数
    p_simpleTimer = new QTimer;
    connect(p_simpleTimer, SIGNAL(timeout()), this, SLOT(slotSimple()), Qt::DirectConnection);

    //new出保持连接定时器，并绑定槽函数
    p_holdTimer = new QTimer;
    connect(p_holdTimer, SIGNAL(timeout()), this, SLOT(slotHold()), Qt::DirectConnection);
}

/*
 * 功能：
 *  初始化函数
 * 参数：
 *
 * 返回值：
 * 	  无
 */
void ThreadEthernetModbusClient::init()
{

    m_summaryList.clear();

    //通过设定结构体，判断主机功能是否开启
    if (DevConfigPtr->networkModbus.clientOnOff) {
        //将服务器与命令进行对应分类
        serverAndCmdPacket();
        //new出各个modbus服务器
        newModbusServer();
        //开启网络状态检查定时器
        p_heartBeatTimer->start(HEARTBEATTIME);
        //开启周期采样定时器
        p_simpleTimer->start(DevConfigPtr->networkModbus.commInterval);
    }
    else {
        qDebug("newwork modbus client off!");
    }
}

/*
 * 功能：
 *  将服务器与命令进行对应分类
 * 参数：
 *
 * 返回值：
 * 	  无
 */
void ThreadEthernetModbusClient::serverAndCmdPacket()
{
    m_cmdTotal = 0;
    int j = 0;
    //遍历100条命令
    for (quint8 i = 0; i < COMM_CMD_NETWORK_NUMBER; ++i) {
        //判断各个命令是否打开
        if (DevConfigPtr->networkModbus.cmdType[i] == 1 || DevConfigPtr->networkModbus.cmdType[i] == 2) {
            //判断服务器队列中是否有当前命令对应的服务器号，有则在当前服务器中加入命令号，没有则新建服务器并加入队列
            for (quint8 j = 0; j < m_serverList.size(); ++j) {
                if (((m_serverList.value(j)))->serverNo == (DevConfigPtr->networkModbus.cmdServeNo[i])) {
                    (m_serverList.value(j))->cmd[(m_serverList.value(j))->cmdSize++] = i;
                    {
                        Modbus_Summary tempSummary;
                        Modbus_Summary_Data tempSummaryData;

                        tempSummary = m_summaryList.value(j);

                        tempSummaryData.cmdNum = i;
                        tempSummaryData.cmdType = DevConfigPtr->networkModbus.cmdType[i];
                        tempSummaryData.cmdStartNo = DevConfigPtr->networkModbus.cmdStartNo[i];
                        tempSummaryData.cmdEndNo = DevConfigPtr->networkModbus.cmdEndNo[i];
                        tempSummaryData.cmdServeCell = DevConfigPtr->networkModbus.cmdServeCell[i];
                        tempSummaryData.cmdRegType = DevConfigPtr->networkModbus.cmdRegType[i];
                        tempSummaryData.cmdRegNo = DevConfigPtr->networkModbus.cmdRegNo[i];
                        tempSummary.dataList.push_back(tempSummaryData);
                        m_summaryList.replace(j, tempSummary);
                    }
                    goto LP1;
                }
            }
            {
                tempServer[j].cmd[0] = i;
                tempServer[j].cmdSize = 1;
                tempServer[j].serverNo = DevConfigPtr->networkModbus.cmdServeNo[i];
                m_serverList.push_back(&(tempServer[j]));

                Modbus_Summary tempSummary;
                Modbus_Summary_Data tempSummaryData;

                tempSummary.serverName = QString(QLatin1String((char*)(DevConfigPtr->networkModbus.serverName[tempServer[j].serverNo])));
                tempSummary.status = ModbusStatus_Start;//默认值

                tempSummaryData.cmdNum = i;
                tempSummaryData.cmdType = DevConfigPtr->networkModbus.cmdType[i];
                tempSummaryData.cmdStartNo = DevConfigPtr->networkModbus.cmdStartNo[i];
                tempSummaryData.cmdEndNo = DevConfigPtr->networkModbus.cmdEndNo[i];
                tempSummaryData.cmdServeCell = DevConfigPtr->networkModbus.cmdServeCell[i];
                tempSummaryData.cmdRegType = DevConfigPtr->networkModbus.cmdRegType[i];
                tempSummaryData.cmdRegNo = DevConfigPtr->networkModbus.cmdRegNo[i];
                tempSummary.dataList.push_back(tempSummaryData);
                m_summaryList.push_back(tempSummary);
                j++;
            }
LP1:
            ;
        }
    }

    //对服务器队列中的各个服务器结构体进行初始化操作
    for (int i = 0; i < m_serverList.size(); ++i) {
        (m_serverList.value(i))->isConnect = false;
        (m_serverList.value(i))->isRecover = false;
        (m_serverList.value(i))->state = false;
        m_cmdTotal += (m_serverList.value(i))->cmdSize;
    }

    //初始化重启log信息
    setOutLogAndInfo(-1, 0);
    setOutLogAndInfo(-2, 0);

    emit sigModbusSummaryChange();
}

/*
 * 功能：
 *  new出服务器队列中的各个服务器
 * 参数：
 *
 * 返回值：
 * 	  无
 */
void ThreadEthernetModbusClient::newModbusServer()
{
    for(int i = 0; i < m_serverList.size(); ++i) {
        int tempServerNo = (m_serverList.value(i))->serverNo;
        (m_serverList.value(i))->ctx = modbus_new_tcp((char *)DevConfigPtr->networkModbus.serverName[tempServerNo],\
                                                   DevConfigPtr->networkModbus.serverPort[tempServerNo]);
        if (m_serverList.value(i)->ctx == NULL)
            setOutLogAndInfo(i, ModbusStatus_Dropout);
    }
}

/*
 * 功能：
 *  执行命令函数
 * 参数：
 *
 * 返回值：
 * 	  无
 */
void ThreadEthernetModbusClient::handleCmd(int ID, modbus_t *ctx, int cmdNo)
{
    //判断命令类型
    switch (DevConfigPtr->networkModbus.cmdType[cmdNo]) {
    /**off**/
    case 0:
        qDebug("No Action!");
        break;
    /**read**/
    case 1:
        //执行读命令后，进行通信通道的报警
        readAction(ID, ctx, cmdNo);
        commAlarmHandle(cmdNo);
        break;
    /**write**/
    case 2:
        //执行写命令
        qDebug("write!");
        writeAction(ID, ctx, cmdNo);
        break;
    default:
        break;
    }
}

/*
 * 功能：
 *  执行读命令函数
 * 参数：
 *  ctx:服务器，cmdNo:命令号
 * 返回值：
 * 	  无
 */
void ThreadEthernetModbusClient::readAction(int ID, modbus_t *ctx, int cmdNo)
{
    int nb;
    int addr;
    int rc;


    //根据寄存器号获取地址，如地址小于0，则是无效地址
    addr = getRegisterMapAddr(DevConfigPtr->networkModbus.cmdRegNo[cmdNo]);
    if (addr < 0) {
        qDebug("Register addr error!");
        setOutLogAndInfo(ID, ModbusStatus_ErrAddr);
        return;
    }

    //获取读取数目
    nb = DevConfigPtr->networkModbus.cmdEndNo[cmdNo] - DevConfigPtr->networkModbus.cmdStartNo[cmdNo] + 1;
    //判断数据类型，如为bit这从线圈或继电器中读取，否则从输入寄存器或者保持寄存器中读取
    if (DevConfigPtr->networkModbus.cmdRegType[cmdNo] == Register_Type_BIT) {
        quint8 data[nb];
        ::memset(data, 0, nb);
        //根据寄存器号判断是从线圈读取还是继电器读取
        if (DevConfigPtr->networkModbus.cmdRegNo[cmdNo] <= COIL_SWITCH_END) {
            //读取数据
            rc = modbus_read_bits(ctx, addr, nb, data);
            if (rc == -1) {
                qDebug("Read coil ragister fail!");
                setOutLogAndInfo(ID, rc);
                return;
            }
        }
        else if (DevConfigPtr->networkModbus.cmdRegNo[cmdNo] >= RELAY_1_START){
            //读取数据
            rc = modbus_read_input_bits(ctx, addr, nb, data);
            if (rc == -1) {
                qDebug("Read relay ragister fail!");
                setOutLogAndInfo(ID, rc);
                return;
            }
        }
        else {
            qDebug("Read bit ragister number fail!");
            setOutLogAndInfo(ID, ModbusStatus_Unreach);
        }
        /*********************test**********************/
//        qDebug("**********");
//            for (int i = 0;i < nb; ++i) {
//                qDebug("%d",data[i]);
//            }
//        qDebug("**********");
        /*********************test**********************/
        //写入通信通道中
        writeDataToCommChanel(cmdNo, data, NULL, nb, 1);
    }
    //按位读取
    else {
        //数据类型为int16时，数目就为通道数目，数据类型为int32/float时，数目就是2倍的通道数目
        if (DevConfigPtr->networkModbus.cmdRegType[cmdNo] == Register_Type_INT16 ||
                DevConfigPtr->networkModbus.cmdRegType[cmdNo] == Register_Type_UINT16) {
            quint16 data[nb];
            ::memset(data, 0, nb);
            //根据寄存器号判断是从输入寄存器读还是保持寄存器读取
            if (DevConfigPtr->networkModbus.cmdRegNo[cmdNo] <= INPUTREG_EXPAND_6_4_END) {
                //读取数据
                rc = modbus_read_input_registers(ctx, addr, nb, data);
                if (rc == -1) {
                    qDebug("Read input ragister fail! --int16");
                    setOutLogAndInfo(ID, rc);
                    return;
                }
            }
            else if (DevConfigPtr->networkModbus.cmdRegNo[cmdNo] >= HOLDREG_MAIN_IO_START) {
                //读取数据
                rc = modbus_read_registers(ctx, addr, nb, data);
                if (rc == -1) {
                    qDebug("Read hold ragister fail! --int16");
                    setOutLogAndInfo(ID, rc);
                    return;
                }
            }
            else {
                qDebug("Read word ragister number fail!");
                setOutLogAndInfo(ID, ModbusStatus_Unreach);
            }
            /*********************test**********************/
//            for (int i = 0;i < nb; ++i) {
//                qDebug("%d",data[i]);
//            }
            /*********************test**********************/
            //将数据写入到通信通道中
            writeDataToCommChanel(cmdNo, NULL, data, nb, 2);
        }
        else {
            quint16 data[nb*2];
            ::memset(data, 0, nb*2);
            //根据寄存器号判断是从输入寄存器读还是保持寄存器读取
            if (DevConfigPtr->networkModbus.cmdRegNo[cmdNo] <= INPUTREG_EXPAND_6_4_END) {
                //读取数据
                rc = modbus_read_input_registers(ctx, addr, nb*2, data);
                if (rc == -1) {
                    qDebug("Read input ragister fail! --32");
                    setOutLogAndInfo(ID, rc);
                    return;
                }
            }
            else if (DevConfigPtr->networkModbus.cmdRegNo[cmdNo] >= HOLDREG_MAIN_IO_START) {
                //读取数据
                rc = modbus_read_registers(ctx, addr, nb*2, data);
                if (rc == -1) {
                    qDebug("Read hold ragister fail! --32");
                    setOutLogAndInfo(ID, rc);
                    return;
                }
            }
            else {
                qDebug("Read word ragister number fail!");
                setOutLogAndInfo(ID, ModbusStatus_Unreach);
            }
            //如果是大端的数据类型，则先进行数据的转换在写入到通信通道中
            switch (DevConfigPtr->networkModbus.cmdRegType[cmdNo]) {
            case Register_Type_INT32B:
            case Register_Type_UINT32B:
            case Register_Type_FLOATB:
                quint16 temp;
                for(int i = 0; i < nb*2; i++) {
                    temp = data[i];
                    data[i] = data[i+1];
                    data[++i] = temp;
                }
                break;
            default:
                break;
            }
            //将数据写入到通信通道中
            writeDataToCommChanel(cmdNo, NULL, data, nb*2, 3);
        }
    }
}



/*
 * 功能：
 *  写命令执行函数
 * 参数：
 *  ctx:modbus服务器,cmdNo：命令号
 * 返回值：
 * 	  无
 */
void ThreadEthernetModbusClient::writeAction(int ID, modbus_t *ctx, int cmdNo)
{
    int nb;
    int addr;
    int chanNo_start,chanNo_end;
    int chanType,chanUnit,chanModule;

    //根据寄存器号获取地址，如地址小于0，则是无效地址
    addr = getRegisterMapAddr(DevConfigPtr->networkModbus.cmdRegNo[cmdNo]);
    if (addr < 0) {
        qDebug("Register addr error!");
        setOutLogAndInfo(ID, ModbusStatus_ErrAddr);
        return;
    }

    //获取读取数目
    if (DevConfigPtr->networkModbus.cmdChType[cmdNo] != Channel_Type_Comm  &&
            DevConfigPtr->networkModbus.cmdChType[cmdNo] != Channel_Type_Math ) {
        chanNo_start = CHANNEL_NUM(DevConfigPtr->networkModbus.cmdStartNo[cmdNo]);
        chanNo_end = CHANNEL_NUM(DevConfigPtr->networkModbus.cmdEndNo[cmdNo]);
        chanType = CHANNEL_TYPE(DevConfigPtr->networkModbus.cmdStartNo[cmdNo]);
        chanUnit = CHANNEL_UNIT(DevConfigPtr->networkModbus.cmdStartNo[cmdNo]);
        if (chanUnit) {
            chanModule = CHANNEL_MODULE(DevConfigPtr->networkModbus.cmdStartNo[cmdNo]);
        }
        nb = chanNo_end - chanNo_start + 1;
    }
    else {
        chanNo_start = DevConfigPtr->networkModbus.cmdStartNo[cmdNo];
        chanNo_end = DevConfigPtr->networkModbus.cmdEndNo[cmdNo];
        nb = DevConfigPtr->networkModbus.cmdEndNo[cmdNo] - DevConfigPtr->networkModbus.cmdStartNo[cmdNo] + 1;
    }

//    quint16 data_w[nb*2];
    quint8 data_b[nb];
    float f_data[nb];
    int j = 0;

    switch (DevConfigPtr->networkModbus.cmdChType[cmdNo]) {
    case Channel_Type_Comm:
    {
        ::memset(f_data,0,nb);

        for (int i = chanNo_start - 1; i < chanNo_end; ++i)
            f_data[j++] = *(float*)(&g_originDataMap.holdReg_MainUnit_Comm_Data_Float32[2*i]);
//        //test
//        for (int i = 0;i<nb;++i)
//            qDebug("%f",f_data[i]);
//        //test

        handleWriteAction(ID, ctx,cmdNo,addr,NULL,f_data,nb,2);
        break;
    }
    case Channel_Type_Math:
    {
        ::memset(f_data,0,nb);

        for (int i = chanNo_start - 1; i < chanNo_end; ++i)
            f_data[j++] = *(float*)(&g_originDataMap.holdReg_MainUnit_Comm_Data_Float32[2*i]);
//        //test
//        for (int i = 0;i<nb;++i)
//            qDebug("%f",f_data[i]);
//        //test

        handleWriteAction(ID, ctx,cmdNo,addr,NULL,f_data,nb,2);

        break;
    }
    default:
    {
        switch (chanType) {
        case Channel_Type_AI:
        {
            if (chanType == 0) {
                ::memset(f_data,0,nb);
                for (int i = chanNo_start - 1; i < chanNo_end; ++i)
                    f_data[j++] = *(float*)(&g_originDataMap.inputReg_MainUnit_IO_Data_Float32[chanModule][2*i]);
                //test
                for (int i = 0;i<nb;++i)
                    qDebug("%f",f_data[i]);
                //test
            }
            else {
                ::memset(f_data,0,nb);
                for (int i = chanNo_start - 1; i < chanNo_end; ++i)
                    f_data[j++] = *(float*)(&g_originDataMap.inputReg_ExtUnit_IO_Data_Float32[chanUnit][chanModule][2*i]);
                //test
                for (int i = 0;i<nb;++i)
                    qDebug("%f",f_data[i]);
                //test
            }
            handleWriteAction(ID, ctx,cmdNo,addr,NULL,f_data,nb,2);
            break;
        }
        case Channel_Type_DI:
        {
            ::memset(data_b,0,nb);
            for (int i = chanNo_start - 1; i < chanNo_end; ++i) {
                data_b[j++] = g_originDataMap.coil_MainUnit_IO_Data_Bit[chanModule][i];
            }
            handleWriteAction(ID, ctx,cmdNo,addr,data_b,NULL,nb,1);
            break;
        }
        case Channel_Type_DO:
        {
            ::memset(data_b,0,nb);
            for (int i = chanNo_start - 1; i < chanNo_end; ++i) {
                data_b[j++] = g_originDataMap.relay_MainUnit_IO_Data_Bit[chanModule][i];
            }
            handleWriteAction(ID, ctx,cmdNo,addr,data_b,NULL,nb,1);
            break;
        }
        default:
            break;
        }
        break;
    }
    }

}

/*
 * 功能：
 *  根据寄存器号获得在寄存器结构体中对应的地址
 * 参数：
 *  regNo:命令号
 * 返回值：
 * 	  无
 */
int ThreadEthernetModbusClient::getRegisterMapAddr(quint32 regNo)
{
    int addr;
    /******input register main*******/
    if (regNo >= INPUTREG_MAIN_IO_START && regNo <=INPUTREG_MAIN_IO_END) {
        addr = regNo - INPUTREG_MAIN_IO_START;
    }
    else if (regNo >= INPUTREG_MAIN_IO_C_START && regNo <=INPUTREG_MAIN_IO_C_END) {
        addr = regNo - INPUTREG_MAIN_IO_C_START
                + g_originDataMap.inputReg_MainUnit_IO_Data_Int32_Linear[0]
                - g_originDataMap.inputReg_MainUnit_IO_Data_Int32[0];
    }
    else if (regNo >= INPUTREG_MAIN_OPERATE_START && regNo <=INPUTREG_MAIN_OPERATE_END) {
        addr = regNo - INPUTREG_MAIN_OPERATE_START
                + g_originDataMap.inputReg_MainUnit_Math_Data_Int32
                - g_originDataMap.inputReg_MainUnit_IO_Data_Int32[0];
    }
    else if (regNo >= INPUTREG_MAIN_STATUS_START && regNo <=INPUTREG_MAIN_STATUS_END) {
        addr = regNo - INPUTREG_MAIN_STATUS_START
                + g_originDataMap.inputReg_Device_None_Status_Int16
                - (qint16*)g_originDataMap.inputReg_MainUnit_IO_Data_Int32[0];
    }
    /******input register expand*******/
    /*****etx1****/
    else if (regNo >= INPUTREG_EXPAND_1_1_START && regNo <=INPUTREG_EXPAND_1_1_END) {
        addr = regNo - INPUTREG_EXPAND_1_1_START
                + g_originDataMap.inputReg_ExtUnit_IO_Data_Int32[0][0]
                - g_originDataMap.inputReg_MainUnit_IO_Data_Int32[0];
    }
    else if (regNo >= INPUTREG_EXPAND_1_2_START && regNo <=INPUTREG_EXPAND_1_2_END) {
        addr = regNo - INPUTREG_EXPAND_1_2_START
                + g_originDataMap.inputReg_ExtUnit_IO_Data_Float32[0][0]
                - g_originDataMap.inputReg_MainUnit_IO_Data_Int32[0];
    }
    else if (regNo >= INPUTREG_EXPAND_1_3_START && regNo <=INPUTREG_EXPAND_1_3_END) {
        addr = regNo - INPUTREG_EXPAND_1_3_START
                + g_originDataMap.inputReg_ExtUnit_IO_Data_Int16[0][0]
                - (qint16*)g_originDataMap.inputReg_MainUnit_IO_Data_Int32[0];
    }
    else if (regNo >= INPUTREG_EXPAND_1_4_START && regNo <=INPUTREG_EXPAND_1_4_END) {
        addr = regNo - INPUTREG_EXPAND_1_4_START
                + g_originDataMap.inputReg_ExtUnit_IO_Status_Int16[0][0]
                - (qint16*)g_originDataMap.inputReg_MainUnit_IO_Data_Int32[0];
    }
    /*****etx2****/
    else if (regNo >= INPUTREG_EXPAND_2_1_START && regNo <=INPUTREG_EXPAND_2_1_END) {
        addr = regNo - INPUTREG_EXPAND_2_1_START
                + g_originDataMap.inputReg_ExtUnit_IO_Data_Int32[1][0]
                - g_originDataMap.inputReg_MainUnit_IO_Data_Int32[0];
    }
    else if (regNo >= INPUTREG_EXPAND_2_2_START && regNo <=INPUTREG_EXPAND_2_2_END) {
        addr = regNo - INPUTREG_EXPAND_2_2_START
                + g_originDataMap.inputReg_ExtUnit_IO_Data_Float32[1][0]
                - g_originDataMap.inputReg_MainUnit_IO_Data_Int32[0];
    }
    else if (regNo >= INPUTREG_EXPAND_2_3_START && regNo <=INPUTREG_EXPAND_2_3_END) {
        addr = regNo - INPUTREG_EXPAND_2_3_START
                + g_originDataMap.inputReg_ExtUnit_IO_Data_Int16[1][0]
                - (qint16*)g_originDataMap.inputReg_MainUnit_IO_Data_Int32[0];
    }
    else if (regNo >= INPUTREG_EXPAND_2_4_START && regNo <=INPUTREG_EXPAND_2_4_END) {
        addr = regNo - INPUTREG_EXPAND_2_4_START
                + g_originDataMap.inputReg_ExtUnit_IO_Status_Int16[1][0]
                - (qint16*)g_originDataMap.inputReg_MainUnit_IO_Data_Int32[0];
    }
    /*****etx3****/
    else if (regNo >= INPUTREG_EXPAND_3_1_START && regNo <=INPUTREG_EXPAND_3_1_END) {
        addr = regNo - INPUTREG_EXPAND_3_1_START
                + g_originDataMap.inputReg_ExtUnit_IO_Data_Int32[2][0]
                - g_originDataMap.inputReg_MainUnit_IO_Data_Int32[0];
    }
    else if (regNo >= INPUTREG_EXPAND_3_2_START && regNo <=INPUTREG_EXPAND_3_2_END) {
        addr = regNo - INPUTREG_EXPAND_3_2_START
                + g_originDataMap.inputReg_ExtUnit_IO_Data_Float32[2][0]
                - g_originDataMap.inputReg_MainUnit_IO_Data_Int32[0];
    }
    else if (regNo >= INPUTREG_EXPAND_3_3_START && regNo <=INPUTREG_EXPAND_3_3_END) {
        addr = regNo - INPUTREG_EXPAND_3_3_START
                + g_originDataMap.inputReg_ExtUnit_IO_Data_Int16[2][0]
                - (qint16*)g_originDataMap.inputReg_MainUnit_IO_Data_Int32[0];
    }
    else if (regNo >= INPUTREG_EXPAND_3_4_START && regNo <=INPUTREG_EXPAND_3_4_END) {
        addr = regNo - INPUTREG_EXPAND_3_4_START
                + g_originDataMap.inputReg_ExtUnit_IO_Status_Int16[2][0]
                - (qint16*)g_originDataMap.inputReg_MainUnit_IO_Data_Int32[0];
    }
    /*****etx4****/
    else if (regNo >= INPUTREG_EXPAND_4_1_START && regNo <=INPUTREG_EXPAND_4_1_END) {
        addr = regNo - INPUTREG_EXPAND_4_1_START
                + g_originDataMap.inputReg_ExtUnit_IO_Data_Int32[3][0]
                - g_originDataMap.inputReg_MainUnit_IO_Data_Int32[0];
    }
    else if (regNo >= INPUTREG_EXPAND_4_2_START && regNo <=INPUTREG_EXPAND_4_2_END) {
        addr = regNo - INPUTREG_EXPAND_4_2_START
                + g_originDataMap.inputReg_ExtUnit_IO_Data_Float32[3][0]
                - g_originDataMap.inputReg_MainUnit_IO_Data_Int32[0];
    }
    else if (regNo >= INPUTREG_EXPAND_4_3_START && regNo <=INPUTREG_EXPAND_4_3_END) {
        addr = regNo - INPUTREG_EXPAND_4_3_START
                + g_originDataMap.inputReg_ExtUnit_IO_Data_Int16[3][0]
                - (qint16*)g_originDataMap.inputReg_MainUnit_IO_Data_Int32[0];
    }
    else if (regNo >= INPUTREG_EXPAND_4_4_START && regNo <=INPUTREG_EXPAND_4_4_END) {
        addr = regNo - INPUTREG_EXPAND_4_4_START
                + g_originDataMap.inputReg_ExtUnit_IO_Status_Int16[3][0]
                - (qint16*)g_originDataMap.inputReg_MainUnit_IO_Data_Int32[0];
    }
    /*****etx5****/
    else if (regNo >= INPUTREG_EXPAND_5_1_START && regNo <=INPUTREG_EXPAND_5_1_END) {
        addr = regNo - INPUTREG_EXPAND_5_1_START
                + g_originDataMap.inputReg_ExtUnit_IO_Data_Int32[4][0]
                - g_originDataMap.inputReg_MainUnit_IO_Data_Int32[0];
    }
    else if (regNo >= INPUTREG_EXPAND_5_2_START && regNo <=INPUTREG_EXPAND_5_2_END) {
        addr = regNo - INPUTREG_EXPAND_5_2_START
                + g_originDataMap.inputReg_ExtUnit_IO_Data_Float32[4][0]
                - g_originDataMap.inputReg_MainUnit_IO_Data_Int32[0];
    }
    else if (regNo >= INPUTREG_EXPAND_5_3_START && regNo <=INPUTREG_EXPAND_5_3_END) {
        addr = regNo - INPUTREG_EXPAND_5_3_START
                + g_originDataMap.inputReg_ExtUnit_IO_Data_Int16[4][0]
                - (qint16*)g_originDataMap.inputReg_MainUnit_IO_Data_Int32[0];
    }
    else if (regNo >= INPUTREG_EXPAND_5_4_START && regNo <=INPUTREG_EXPAND_5_4_END) {
        addr = regNo - INPUTREG_EXPAND_5_4_START
                + g_originDataMap.inputReg_ExtUnit_IO_Status_Int16[4][0]
                - (qint16*)g_originDataMap.inputReg_MainUnit_IO_Data_Int32[0];
    }
    /*****etx6****/
    else if (regNo >= INPUTREG_EXPAND_6_1_START && regNo <=INPUTREG_EXPAND_6_1_END) {
        addr = regNo - INPUTREG_EXPAND_6_1_START
                + g_originDataMap.inputReg_ExtUnit_IO_Data_Int32[5][0]
                - g_originDataMap.inputReg_MainUnit_IO_Data_Int32[0];
    }
    else if (regNo >= INPUTREG_EXPAND_6_2_START && regNo <=INPUTREG_EXPAND_6_2_END) {
        addr = regNo - INPUTREG_EXPAND_6_2_START
                + g_originDataMap.inputReg_ExtUnit_IO_Data_Float32[5][0]
                - g_originDataMap.inputReg_MainUnit_IO_Data_Int32[0];
    }
    else if (regNo >= INPUTREG_EXPAND_6_3_START && regNo <=INPUTREG_EXPAND_6_3_END) {
        addr = regNo - INPUTREG_EXPAND_6_3_START
                + g_originDataMap.inputReg_ExtUnit_IO_Data_Int16[5][0]
                - (qint16*)g_originDataMap.inputReg_MainUnit_IO_Data_Int32[0];
    }
    else if (regNo >= INPUTREG_EXPAND_6_4_START && regNo <=INPUTREG_EXPAND_6_4_END) {
        addr = regNo - INPUTREG_EXPAND_6_4_START
                + g_originDataMap.inputReg_ExtUnit_IO_Status_Int16[5][0]
                - (qint16*)g_originDataMap.inputReg_MainUnit_IO_Data_Int32[0];
    }
    /*******hold register main********/
    else if (regNo >= HOLDREG_MAIN_IO_START && regNo <=HOLDREG_MAIN_IO_END) {
        addr = regNo - HOLDREG_MAIN_IO_START;
    }
    else if (regNo >= HOLDREG_MAIN_IO_C_START && regNo <=HOLDREG_MAIN_IO_C_END) {
        addr = regNo - INPUTREG_MAIN_IO_C_START
                + g_originDataMap.holdReg_MainUnit_IO_Data_Int32_Linear[0]
                - g_originDataMap.holdReg_mainUnit_IO_Data_Int32[0];
    }
    else if (regNo >= HOLDREG_MAIN_COMM_START && regNo <=HOLDREG_MAIN_COMM_END) {
        addr = regNo - HOLDREG_MAIN_COMM_START
                + g_originDataMap.holdReg_MainUnit_Comm_Data_Int32
                - g_originDataMap.holdReg_mainUnit_IO_Data_Int32[0];
    }
    else if (regNo >= HOLDREG_MAIN_SET_START && regNo <=HOLDREG_MAIN_SET_END) {
        addr = regNo - HOLDREG_MAIN_SET_START
                + &(g_originDataMap.holdReg_Device_Operate_Data_Int16_Record)
                - (qint16*)g_originDataMap.holdReg_mainUnit_IO_Data_Int32[0];
    }
    else if (regNo >= HOLDREG_MAIN_STATUS_START && regNo <=HOLDREG_MAIN_STATUS_END) {
        addr = regNo - HOLDREG_MAIN_STATUS_START
                + g_originDataMap.holdReg_Device_Switch_Data_Int16
                - (qint16*)g_originDataMap.holdReg_mainUnit_IO_Data_Int32[0];
    }
    /**********hold register expand***************/
    /*****etx1****/
    else if (regNo >= HOLDREG_EXPAND_1_1_START && regNo <=HOLDREG_EXPAND_1_1_END) {
        addr = regNo - HOLDREG_EXPAND_1_1_START
                + g_originDataMap.holdReg_ExtUnit_IO_Data_Int32[0][0]
                - g_originDataMap.holdReg_mainUnit_IO_Data_Int32[0];
    }
    else if (regNo >= HOLDREG_EXPAND_1_2_START && regNo <=HOLDREG_EXPAND_1_2_END) {
        addr = regNo - HOLDREG_EXPAND_1_2_START
                + g_originDataMap.holdReg_ExtUnit_IO_Data_Float32[0][0]
                - g_originDataMap.holdReg_mainUnit_IO_Data_Int32[0];
    }
    else if (regNo >= HOLDREG_EXPAND_1_3_START && regNo <=HOLDREG_EXPAND_1_3_END) {
        addr = regNo - HOLDREG_EXPAND_1_3_START
                + g_originDataMap.holdReg_ExtUnit_IO_Data_Int16[0][0]
                - (qint16*)g_originDataMap.holdReg_mainUnit_IO_Data_Int32[0];
    }
    else if (regNo >= HOLDREG_EXPAND_1_4_START && regNo <=HOLDREG_EXPAND_1_4_END) {
        addr = regNo - HOLDREG_EXPAND_1_4_START
                + g_originDataMap.holdReg_ExtUnit_IO_Status_Int16[0][0]
                - (qint16*)g_originDataMap.holdReg_mainUnit_IO_Data_Int32[0];
    }
    /*****etx2****/
    else if (regNo >= HOLDREG_EXPAND_2_1_START && regNo <=HOLDREG_EXPAND_2_1_END) {
        addr = regNo - HOLDREG_EXPAND_2_1_START
                + g_originDataMap.holdReg_ExtUnit_IO_Data_Int32[1][0]
                - g_originDataMap.holdReg_mainUnit_IO_Data_Int32[0];
    }
    else if (regNo >= HOLDREG_EXPAND_2_2_START && regNo <=HOLDREG_EXPAND_2_2_END) {
        addr = regNo - HOLDREG_EXPAND_2_2_START
                + g_originDataMap.holdReg_ExtUnit_IO_Data_Float32[1][0]
                - g_originDataMap.holdReg_mainUnit_IO_Data_Int32[0];
    }
    else if (regNo >= HOLDREG_EXPAND_2_3_START && regNo <=HOLDREG_EXPAND_2_3_END) {
        addr = regNo - HOLDREG_EXPAND_2_3_START
                + g_originDataMap.holdReg_ExtUnit_IO_Data_Int16[1][0]
                - (qint16*)g_originDataMap.holdReg_mainUnit_IO_Data_Int32[0];
    }
    else if (regNo >= HOLDREG_EXPAND_2_4_START && regNo <=HOLDREG_EXPAND_2_4_END) {
        addr = regNo - HOLDREG_EXPAND_2_4_START
                + g_originDataMap.holdReg_ExtUnit_IO_Status_Int16[1][0]
                - (qint16*)g_originDataMap.holdReg_mainUnit_IO_Data_Int32[0];
    }
    /*****etx3****/
    else if (regNo >= HOLDREG_EXPAND_3_1_START && regNo <=HOLDREG_EXPAND_3_1_END) {
        addr = regNo - HOLDREG_EXPAND_3_1_START
                + g_originDataMap.holdReg_ExtUnit_IO_Data_Int32[2][0]
                - g_originDataMap.holdReg_mainUnit_IO_Data_Int32[0];
    }
    else if (regNo >= HOLDREG_EXPAND_3_2_START && regNo <=HOLDREG_EXPAND_3_2_END) {
        addr = regNo - HOLDREG_EXPAND_3_2_START
                + g_originDataMap.holdReg_ExtUnit_IO_Data_Float32[2][0]
                - g_originDataMap.holdReg_mainUnit_IO_Data_Int32[0];
    }
    else if (regNo >= HOLDREG_EXPAND_3_3_START && regNo <=HOLDREG_EXPAND_3_3_END) {
        addr = regNo - HOLDREG_EXPAND_3_3_START
                + g_originDataMap.holdReg_ExtUnit_IO_Data_Int16[2][0]
                - (qint16*)g_originDataMap.holdReg_mainUnit_IO_Data_Int32[0];
    }
    else if (regNo >= HOLDREG_EXPAND_3_4_START && regNo <=HOLDREG_EXPAND_3_4_END) {
        addr = regNo - HOLDREG_EXPAND_3_4_START
                + g_originDataMap.holdReg_ExtUnit_IO_Status_Int16[2][0]
                - (qint16*)g_originDataMap.holdReg_mainUnit_IO_Data_Int32[0];
    }
    /*****etx4****/
    else if (regNo >= HOLDREG_EXPAND_4_1_START && regNo <=HOLDREG_EXPAND_4_1_END) {
        addr = regNo - HOLDREG_EXPAND_4_1_START
                + g_originDataMap.holdReg_ExtUnit_IO_Data_Int32[3][0]
                - g_originDataMap.holdReg_mainUnit_IO_Data_Int32[0];
    }
    else if (regNo >= HOLDREG_EXPAND_4_2_START && regNo <=HOLDREG_EXPAND_4_2_END) {
        addr = regNo - HOLDREG_EXPAND_4_2_START
                + g_originDataMap.holdReg_ExtUnit_IO_Data_Float32[3][0]
                - g_originDataMap.holdReg_mainUnit_IO_Data_Int32[0];
    }
    else if (regNo >= HOLDREG_EXPAND_4_3_START && regNo <=HOLDREG_EXPAND_4_3_END) {
        addr = regNo - HOLDREG_EXPAND_4_3_START
                + g_originDataMap.holdReg_ExtUnit_IO_Data_Int16[3][0]
                - (qint16*)g_originDataMap.holdReg_mainUnit_IO_Data_Int32[0];
    }
    else if (regNo >= HOLDREG_EXPAND_4_4_START && regNo <=HOLDREG_EXPAND_4_4_END) {
        addr = regNo - HOLDREG_EXPAND_4_4_START
                + g_originDataMap.holdReg_ExtUnit_IO_Status_Int16[3][0]
                - (qint16*)g_originDataMap.holdReg_mainUnit_IO_Data_Int32[0];
    }
    /*****etx5****/
    else if (regNo >= HOLDREG_EXPAND_5_1_START && regNo <=HOLDREG_EXPAND_5_1_END) {
        addr = regNo - HOLDREG_EXPAND_5_1_START
                + g_originDataMap.holdReg_ExtUnit_IO_Data_Int32[4][0]
                - g_originDataMap.holdReg_mainUnit_IO_Data_Int32[0];
    }
    else if (regNo >= HOLDREG_EXPAND_5_2_START && regNo <=HOLDREG_EXPAND_5_2_END) {
        addr = regNo - HOLDREG_EXPAND_5_2_START
                + g_originDataMap.holdReg_ExtUnit_IO_Data_Float32[4][0]
                - g_originDataMap.holdReg_mainUnit_IO_Data_Int32[0];
    }
    else if (regNo >= HOLDREG_EXPAND_5_3_START && regNo <=HOLDREG_EXPAND_5_3_END) {
        addr = regNo - HOLDREG_EXPAND_5_3_START
                + g_originDataMap.holdReg_ExtUnit_IO_Data_Int16[4][0]
                - (qint16*)g_originDataMap.holdReg_mainUnit_IO_Data_Int32[0];
    }
    else if (regNo >= HOLDREG_EXPAND_5_4_START && regNo <=HOLDREG_EXPAND_5_4_END) {
        addr = regNo - HOLDREG_EXPAND_5_4_START
                + g_originDataMap.holdReg_ExtUnit_IO_Status_Int16[4][0]
                - (qint16*)g_originDataMap.holdReg_mainUnit_IO_Data_Int32[0];
    }
    /*****etx6****/
    else if (regNo >= HOLDREG_EXPAND_6_1_START && regNo <=HOLDREG_EXPAND_6_1_END) {
        addr = regNo - HOLDREG_EXPAND_6_1_START
                + g_originDataMap.holdReg_ExtUnit_IO_Data_Int32[5][0]
                - g_originDataMap.holdReg_mainUnit_IO_Data_Int32[0];
    }
    else if (regNo >= HOLDREG_EXPAND_6_2_START && regNo <=HOLDREG_EXPAND_6_2_END) {
        addr = regNo - HOLDREG_EXPAND_6_2_START
                + g_originDataMap.holdReg_ExtUnit_IO_Data_Float32[5][0]
                - g_originDataMap.holdReg_mainUnit_IO_Data_Int32[0];
    }
    else if (regNo >= HOLDREG_EXPAND_6_3_START && regNo <=HOLDREG_EXPAND_6_3_END) {
        addr = regNo - HOLDREG_EXPAND_6_3_START
                + g_originDataMap.holdReg_ExtUnit_IO_Data_Int16[5][0]
                - (qint16*)g_originDataMap.holdReg_mainUnit_IO_Data_Int32[0];
    }
    else if (regNo >= HOLDREG_EXPAND_6_4_START && regNo <=HOLDREG_EXPAND_6_4_END) {
        addr = regNo - HOLDREG_EXPAND_6_4_START
                + g_originDataMap.holdReg_ExtUnit_IO_Status_Int16[5][0]
                - (qint16*)g_originDataMap.holdReg_mainUnit_IO_Data_Int32[0];
    }
    /***********coil**************/
    else if (regNo >= COIL_IO_START && regNo <=COIL_IO_END) {
        addr = regNo - COIL_IO_START;
    }
    else if (regNo >= COIL_COMM_STATUS_START && regNo <=COIL_COMM_STATUS_END) {
        addr = regNo - COIL_COMM_STATUS_START
                + g_originDataMap.coil_MainUnit_Comm_Status_Bit
                - g_originDataMap.coil_MainUnit_IO_Data_Bit[0];
    }
    else if (regNo >= COIL_SWITCH_START && regNo <=COIL_SWITCH_END) {
        addr = regNo - COIL_SWITCH_START
                + g_originDataMap.coil_MainUnit_Switch_Data_Bit
                - g_originDataMap.coil_MainUnit_IO_Data_Bit[0];
    }
    /**********relay**************/
    else if (regNo >= RELAY_1_START && regNo <=RELAY_1_END) {
        addr = regNo - RELAY_1_START;
    }
    else if (regNo >= RELAY_2_START && regNo <=RELAY_2_END) {
        addr = regNo - RELAY_2_START
                + g_originDataMap.relay_MainUnit_Math_Status_Bit
                - g_originDataMap.relay_MainUnit_IO_Data_Bit[0];
    }
    /**********Invalid register number*********/
    else {
        qDebug("Invalid register number!");
        addr = -1;
    }
    return addr;
}

/*
 * 功能：
 *  将从服务器读取的数据写到通信通道中，根据flag的值来判断是那种类型的数据类型
 *      flag = 1时：bit，flag = 2时:int16,flag = 3时：int32/float
 * 参数：
 *  cmdNo:命令号，data_b:bit数据,data_w:word数据，nb：数据数目
 * 返回值：
 * 	  无
 */
void ThreadEthernetModbusClient::writeDataToCommChanel(int cmdNo, quint8 *data_b, quint16 *data_w, int nb, int flag)
{
    int j = 0;
    float *f_data;



    //将bit数据写入到通信通道中
    if (flag == 1) {
        //判断数据读取的数目和应该读取的数目是否一致
        if ((quint32)nb != (DevConfigPtr->networkModbus.cmdEndNo[cmdNo] - DevConfigPtr->networkModbus.cmdStartNo[cmdNo] + 1)) {
            qDebug("bit read number lack!");
            return;
        }

        /*********************test**********************/
        for (int i = 0;i < nb; ++i) {
            qDebug("%d",data_b[i]);
        }
        /*********************test**********************/

        for (quint32 i = DevConfigPtr->networkModbus.cmdStartNo[cmdNo] - 1;\
             i < DevConfigPtr->networkModbus.cmdEndNo[cmdNo]; ++i) {

            if (DevConfigPtr->chanComm[i].rangeConfig.rangeSwtich == 1) {
//                if (DevConfigPtr->chanComm[i].rangeConfig.rangeMax >= data_b[j] && DevConfigPtr->chanComm[i].rangeConfig.rangeMin <= data_b[j]) {
                g_originDataMap.coil_MainUnit_Comm_Data_Bit[i] = data_b[j];
                f_data = (float*)(&g_originDataMap.holdReg_MainUnit_Comm_Data_Float32[i*2]);
                *f_data = data_b[j];
//                }
            }
            j++;
//            //***************test use********************//
//                    g_originDataMap.coil_MainUnit_Comm_Data_Bit[i] = data_b[j];
//                    f_data = (float*)(&g_originDataMap.holdReg_MainUnit_Comm_Data_Float32[i*2]);
//                    *f_data = data_b[j];
//                    j++;
//            //***************test use********************//

        }
#if 0
        /*********************test**********************/
        for (quint32 i = DevConfigPtr->networkModbus.cmdStartNo[cmdNo] - 1;\
             i < DevConfigPtr->networkModbus.cmdEndNo[cmdNo]; ++i) {
            f_data = (float*)(&g_originDataMap.holdReg_MainUnit_Comm_Data_Float32[i*2]);
            qDebug("%f",*f_data);
        }
        /*********************test**********************/
#endif
    }
    //将int16类型的数据写入通信通道
    else if (flag == 2) {
        //判断数据读取的数目和应该读取的数目是否一致
        if ((quint32)nb != (DevConfigPtr->networkModbus.cmdEndNo[cmdNo] - DevConfigPtr->networkModbus.cmdStartNo[cmdNo] + 1)) {
            qDebug("word read number lack! --16");
            return;
        }
        for (quint32 i = DevConfigPtr->networkModbus.cmdStartNo[cmdNo] - 1;\
             i < DevConfigPtr->networkModbus.cmdEndNo[cmdNo]; i++) {
            if (DevConfigPtr->chanComm[i].rangeConfig.rangeSwtich == 1) {
//                if (DevConfigPtr->chanComm[i].rangeConfig.rangeMax >= data_w[j] && DevConfigPtr->chanComm[i].rangeConfig.rangeMin <= data_w[j]) {
                f_data = (float*)(&g_originDataMap.holdReg_MainUnit_Comm_Data_Float32[i*2]);
                *f_data = data_w[j];
//                }
            }
            j++;
//            //test
//            f_data = (float*)(&g_originDataMap.holdReg_MainUnit_Comm_Data_Float32[i*2]);
//            *f_data = data_w[j++];
//            //test
        }
#if 0
        //test
        for (quint32 i = DevConfigPtr->networkModbus.cmdStartNo[cmdNo] - 1;\
             i < DevConfigPtr->networkModbus.cmdEndNo[cmdNo]; i++) {
            f_data = (float*)(&g_originDataMap.holdReg_MainUnit_Comm_Data_Float32[i*2]);
            qDebug("%f",*f_data);
        }
        //test
#endif

    }
    //将int32/float类型的数据写入通信通道
    else if (flag == 3) {
        //判断数据读取的数目和应该读取的数目是否一致
        if ((quint32)nb/2 != (DevConfigPtr->networkModbus.cmdEndNo[cmdNo] - DevConfigPtr->networkModbus.cmdStartNo[cmdNo] + 1)) {
            qDebug("word read number lack! --16");
            return;
        }
        for (quint32 i = DevConfigPtr->networkModbus.cmdStartNo[cmdNo] - 1;\
             i < DevConfigPtr->networkModbus.cmdEndNo[cmdNo]; ++i) {
            float data;

            if (DevConfigPtr->chanComm[i].rangeConfig.rangeSwtich == 1) {
                data = data_w[j] + (data_w[j+1]<<16);
//                if (DevConfigPtr->chanComm[i].rangeConfig.rangeMax >= data && DevConfigPtr->chanComm[i].rangeConfig.rangeMin <= data) {
                f_data = (float*)(&g_originDataMap.holdReg_MainUnit_Comm_Data_Float32[i*2]);
                *f_data = data;
//                }
            }
            j++;
        }
    }
    else {
        qDebug("writeDataToCommChanel flag fail!");
    }
}

/*
 * 功能：
 *      通信通道报警处理
 * 参数：
 *      cmdNo：命令号
 * 返回值：
 * 	  无
 */
void ThreadEthernetModbusClient::commAlarmHandle(int cmdNo)
{
    float* value;

    //遍历写入的通信通道，对每个通信通道进行报警处理
    for (quint32 i = DevConfigPtr->networkModbus.cmdStartNo[cmdNo];
         i <= DevConfigPtr->networkModbus.cmdEndNo[cmdNo]; i++) {

        value = (float*)(&g_originDataMap.holdReg_MainUnit_Comm_Data_Float32[i*2]);

        int pos = DevConfigPtr->chanComm[i].rangeConfig.rangePos;

        if (DevConfigPtr->chanComm[i].rangeConfig.rangeSwtich == 1) {

            //分别处理通道状况的0-3位的电平
            for (int j = 0; j < 4; j++) {
                if (DevConfigPtr->chanComm[i].alarmConfig.alarmOnOff[j]) {
                    g_originDataMap.coil_MainUnit_Comm_Status_Bit[i] |= 0<<j;

                    //判断报警类型
                    switch (DevConfigPtr->chanComm[i].alarmConfig.alarmType[j]) {
                        //上限
                    case Alarm_Type_H:
                        if (*value >= (float)(DevConfigPtr->chanComm[i].alarmConfig.alarmValue[j] /:: pow10(pos))) {
                            g_originDataMap.coil_MainUnit_Comm_Status_Bit[i] |= 1<<j;
                        }
                        break;
                        //下限
                    case Alarm_Type_L:
                        if (*value <= (float)(DevConfigPtr->chanComm[i].alarmConfig.alarmValue[j]) /:: pow10(pos)) {
                            g_originDataMap.coil_MainUnit_Comm_Status_Bit[i] |= 1<<j;
                        }
                        break;
                        //延迟上限
                    case Alarm_Type_T:
                        if (*value > (float)(DevConfigPtr->chanComm[i].alarmConfig.alarmValue[j]) /:: pow10(pos)) {
                            g_originDataMap.coil_MainUnit_Comm_Status_Bit[i] |= 1<<j;
                        }
                        break;
                        //延迟下限
                    case Alarm_Type_t:
                        if (*value < (float)(DevConfigPtr->chanComm[i].alarmConfig.alarmValue[j]) /:: pow10(pos)) {
                            g_originDataMap.coil_MainUnit_Comm_Status_Bit[i] |= 1<<j;
                        }
                        break;
                    default:
                        break;
                    }
                }
            }
        }
    }
}

void ThreadEthernetModbusClient::handleWriteAction(int ID, modbus_t *ctx, int cmdNo, int addr, quint8 *data_b, float *data_f, int nb, int flag)
{
    int rc;

    if (flag == 1) {
        if (DevConfigPtr->networkModbus.cmdRegType[cmdNo] == Register_Type_BIT) {
            //将数据写到服务器的线圈中
            rc = modbus_write_bits(ctx, addr, nb, data_b);
            if (rc == -1) {
                qDebug("write coil fail!");
                setOutLogAndInfo(ID, rc);
            }
        }
        else if (DevConfigPtr->networkModbus.cmdRegType[cmdNo] == Register_Type_INT16 ||
                 DevConfigPtr->networkModbus.cmdRegType[cmdNo] == Register_Type_UINT16 ) {
            quint16 dataT_b[nb];
            for (int i = 0; i < nb; ++i)
                dataT_b[i] = data_b[i];
            //将数据写到服务器的保持寄存器中
            rc = modbus_write_registers(ctx, addr, nb, dataT_b);
            if (rc == -1) {
                qDebug("write register fail! --16");
                setOutLogAndInfo(ID, rc);
            }
        }
        else if (DevConfigPtr->networkModbus.cmdRegType[cmdNo] == Register_Type_INT32L ||
                DevConfigPtr->networkModbus.cmdRegType[cmdNo] == Register_Type_FLOATL){
            quint16 dataT_b[2*nb];
            ::memset(dataT_b, 0, nb*2);
            for (int i = 0; i < nb; ++i) {
                dataT_b[2*i] = data_b[i];
            }
            rc = modbus_write_registers(ctx, addr, nb*2, dataT_b);
            if (rc == -1) {
                qDebug("write register fail! --16");
                setOutLogAndInfo(ID, rc);
            }
        }
        else {
            quint16 dataT_b[2*nb];
            ::memset(dataT_b, 0, nb*2);
            for (int i = 0; i < nb; ++i) {
                dataT_b[2*i+1] = data_b[i];
            }
            rc = modbus_write_registers(ctx, addr, nb*2, dataT_b);
            if (rc == -1) {
                qDebug("write register fail! --16");
                setOutLogAndInfo(ID, rc);
            }
        }
    }
    else if (flag == 2) {
        if (DevConfigPtr->networkModbus.cmdRegType[cmdNo] == Register_Type_BIT) {
            quint8 data_bit[nb];

            for (int i = 0; i < nb; ++i) {
                data_bit[i] = data_f[i];
                qDebug("%d,  %f",data_bit[i],data_f[i]);
            }
            //将数据写到服务器的线圈中
            rc = modbus_write_bits(ctx, addr, nb, data_bit);
            if (rc == -1) {
                qDebug("write coil fail!");
                setOutLogAndInfo(ID, rc);
            }
        }
        else if (DevConfigPtr->networkModbus.cmdRegType[cmdNo] == Register_Type_INT16 ||
                 DevConfigPtr->networkModbus.cmdRegType[cmdNo] == Register_Type_UINT16 ) {
            quint16 dataT_b[nb];

            for (int i = 0; i < nb; ++i) {
                dataT_b[i] = data_f[i];
                qDebug("%d,  %f",dataT_b[i],data_f[i]);
            }

            //将数据写到服务器的保持寄存器中
            rc = modbus_write_registers(ctx, addr, nb, dataT_b);
            if (rc == -1) {
                qDebug("write register fail! --16");
                setOutLogAndInfo(ID, rc);
            }
        }
        else if (DevConfigPtr->networkModbus.cmdRegType[cmdNo] == Register_Type_INT32L ||
                 DevConfigPtr->networkModbus.cmdRegType[cmdNo] == Register_Type_FLOATL ) {
            quint16 dataT_b[nb*2];
            float *fData;

            for (int i = 0; i < nb; ++i) {
                fData = (float*)(&dataT_b[2*i]);
                *fData = data_f[i];
                qDebug("%f,  %f",*fData,data_f[i]);
            }

            //将数据写到服务器的保持寄存器中
            rc = modbus_write_registers(ctx, addr, nb, dataT_b);
            if (rc == -1) {
                qDebug("write register fail! --16");
                setOutLogAndInfo(ID, rc);
            }
        }
        else {
            quint16 dataT_b[nb*2],temp;
            float *fData;

            for (int i = 0; i < nb; ++i) {
                fData = (float*)(&dataT_b[2*i]);
                *fData = data_f[i];
                qDebug("%f,  %f",*fData,data_f[i]);
            }

            for (int i = 0; i < nb; ++i) {
                temp = dataT_b[i];
                dataT_b[i] = dataT_b[i+1];
                dataT_b[i+1] = temp;
                i++;
            }
            //将数据写到服务器的保持寄存器中
            rc = modbus_write_registers(ctx, addr, nb*2, dataT_b);
            if (rc == -1) {
                qDebug("write register fail! --16");
                setOutLogAndInfo(ID, rc);
            }
        }

    }
    else {

    }
}

/*
 * 功能：
 *      客户端关闭函数，释放资源
 * 参数：
 *
 * 返回值：
 * 	  无
 */
void ThreadEthernetModbusClient::closeServer()
{
    //关闭网络测试定时器
    if (p_heartBeatTimer->isActive()) {
        p_heartBeatTimer->stop();
    }
    //关闭周期采样定时器
    if (p_simpleTimer->isActive()) {
        p_simpleTimer->stop();
    }
    //关闭保持连接定时器
    if (p_holdTimer->isActive()) {
        p_holdTimer->stop();
    }
    //关闭服务器连接
    for (int i = 0; i < m_serverList.size(); ++i) {
        if (m_serverList.value(i)->isConnect) {
            modbus_close(m_serverList.value(i)->ctx);
        }
        modbus_free(m_serverList.value(i)->ctx);
    }
    //清空服务器队列
    while (!m_serverList.empty()) {
        m_serverList.pop_front();
    }
    //情空保持连接队列
    while (!m_needHoldConnectServer.empty()) {
        m_needHoldConnectServer.pop_front();
    }
    //清空恢复连接队列
    while (!m_needRecoverServer.empty()) {
        m_needRecoverServer.pop_front();
    }
}

void ThreadEthernetModbusClient::setOutLogAndInfo(int ID, int rc)
{
    ModbusStatus errNum;
    QString logStr;

    if (ID == -1) {
        logStr = QString("C|%1|000 N ").arg(ModbusStatus_Stop);
        Logger::getInstance().log(Logger::MODBUS_LOG, logStr);
        return;
    }
    else if (ID == -2) {
        logStr = QString("C|%1|000 N ").arg(ModbusStatus_Start);
        Logger::getInstance().log(Logger::MODBUS_LOG, logStr);
        return;
    }

    if ((m_summaryList.value(ID).status == ModbusStatus_CLosed
            || m_summaryList.value(ID).status == ModbusStatus_Valid
            || m_summaryList.value(ID).status == ModbusStatus_Start) && (
                (ModbusStatus)rc == ModbusStatus_CLosed ||
                (ModbusStatus)rc == ModbusStatus_Valid ||
                (ModbusStatus)rc == ModbusStatus_Start))
        return;

    if (rc >= MODBUS_ENOBASE) {
        switch (rc) {
        case EMBXILFUN:
            //            return "Illegal function";
            errNum = ModbusStatus_ErrFc;
            break;
        case EMBXILADD:
            //            return "Illegal data address";
            errNum = ModbusStatus_ErrAddr;
            break;
        case EMBXILVAL:
            //            return "Illegal data value";
            errNum = ModbusStatus_ErrValue;
            break;
        case EMBXSFAIL:
            //            return "Slave device or server failure";
            errNum = ModbusStatus_ErrDevice;
            break;
        case EMBXACK:
            //            return "Acknowledge";
            errNum =ModbusStatus_Ack ;
            break;
        case EMBXSBUSY:
            //            return "Slave device or server is busy";
            errNum = ModbusStatus_Busy;
            break;
#if 0
        case EMBXNACK:
            //            return "Negative acknowledge";
            errNum = ;
            break;
        case EMBXMEMPAR:
            //            return "Memory parity error";
            errNum = ;
            break;
        case EMBXGPATH:
            //            return "Gateway path unavailable";
            errNum = ;
            break;
        case EMBXGTAR:
            //            return "Target device failed to respond";
            errNum = ;
            break;
        case EMBBADDATA:
            //            return "Invalid data";
            errNum = ;
            break;
        case EMBBADEXC:
            //            return "Invalid exception code";
            errNum = ;
            break;
        case EMBMDATA:
            //            return "Too many data";
            errNum = ;
            break;
        case EMBBADSLAVE:
            //            return "Response not from requested slave";
            errNum = ;
            break;
#endif
        default:
            errNum = ModbusStatus_Invalid;
            break;
        }
    }
    else {
        errNum = (ModbusStatus)rc;
    }

    Modbus_Summary s;

    if (!m_summaryList.isEmpty()) {
//        m_summaryList.value(ID).status = errNum;
        s = m_summaryList.value(ID);
        s.status = errNum;
        m_summaryList.replace(ID, s);
    }

    emit sigModbusSummaryChange(ID);

//    QDateTime current_date_time = QDateTime::currentDateTime();
//    QString current_date = current_date_time.toString("yyyy/MM/dd hh:mm:ss ");


    for (int i = 0; i < m_summaryList.value(ID).dataList.size(); ++i) {
        logStr = QString("C|%1|%2 ").arg(errNum).arg(m_summaryList.value(ID).dataList.value(i).cmdNum, 3, 10, QChar('0'));
        if (m_summaryList.value(ID).dataList.value(i).cmdType == 1)
            logStr += "R";
        else
            logStr += "W";
    }

    Logger::getInstance().log(Logger::MODBUS_LOG, logStr);

}

QList<Modbus_Summary> ThreadEthernetModbusClient::getModbusSummary()
{
    return m_summaryList;
}

/*
 * 功能：
 *  功能开启槽函数
 * 参数：
 *
 * 返回值：
 * 	  无
 */
void ThreadEthernetModbusClient::slotServerStart()
{
    init();

}

/*
 * 功能：
 *  功能重启槽函数,先关闭，再开启
 * 参数：
 *
 * 返回值：
 * 	  无
 */
void ThreadEthernetModbusClient::slotServerRestart(quint32 ID)
{
    qDebug()<<"+++++++++++++++++++++++";
//    if (ID == ID_NetWork_Modbus ||
//            ID == ID_NetWork_Modbus_Basic) {
        closeServer();
        init();
//    }
}

/*
 * 功能：
 *  定时检查网络状态槽函数
 * 参数：
 *
 * 返回值：
 * 	  无
 */
void ThreadEthernetModbusClient::slotHeartBeat()
{
    int rc;
    //遍历各个服务器
    for (int i = 0; i < m_serverList.size(); ++i) {
        //当前服务器连接服务正在进行恢复操作
        if ((m_serverList.value(i))->isRecover) {
            continue;
        }
        m_mutex.lock();
        //获取socket
        int sockfd;
        int optval;
        quint32 optlen = sizeof(int);
        //判断是否正在连接当前服务器
        if ((m_serverList.value(i))->isConnect) {
            sockfd = modbus_get_socket((m_serverList.value(i))->ctx);
            rc = getsockopt(sockfd, SOL_SOCKET, SO_ERROR,(char*) &optval, &optlen);//获取socket网络状态
            if (rc < 0) {
                qDebug("getsockopt Fail!");
                setOutLogAndInfo(i, ModbusStatus_Dropout);
            }
            else {
                //若rc==0则表示网络正常，否则加入需要进行恢复动作的队列
                switch (optval) {
                case 0:
                    qDebug("success %d!",sockfd);
                    (m_serverList.value(i))->state = true;
//                    setOutLogAndInfo(i, ModbusStatus_Start);
                    break;
                default:
                    (m_serverList.value(i))->state = false;
                    (m_serverList.value(i))->isRecover = true;
                    setOutLogAndInfo(i, ModbusStatus_CLosed);
                    m_needRecoverServer.push_back((m_serverList.value(i)));
                }
            }
        }
        else {
            //连接服务器
            rc = modbus_connect((m_serverList.value(i))->ctx);
            sockfd = modbus_get_socket((m_serverList.value(i))->ctx);
            //连接失败则直接加入需要进行恢复动作的队列
            if (rc == -1) {
                qDebug("modbus_connect Fail!");
                (m_serverList.value(i))->state = false;
                (m_serverList.value(i))->isRecover = true;
                setOutLogAndInfo(i, ModbusStatus_CLosed);
                m_needRecoverServer.push_back((m_serverList.value(i)));
                modbus_close((m_serverList.value(i))->ctx);
                m_mutex.unlock();
                continue;
            }
            rc = getsockopt(sockfd, SOL_SOCKET, SO_ERROR,(char*) &optval, &optlen);//获取socket网络状态
            if (rc < 0) {
                qDebug("getsockopt Fail!");
            }
            else {
                //若rc==0则表示网络正常，否则加入需要进行恢复动作的队列
                switch (optval) {
                case 0:
                    (m_serverList.value(i))->state = true;
                    qDebug("success %d!",sockfd);
//                    setOutLogAndInfo(i, ModbusStatus_Start);
                    break;
                default:
                    (m_serverList.value(i))->state = false;
                    (m_serverList.value(i))->isRecover = true;
                    setOutLogAndInfo(i, ModbusStatus_CLosed);
                    m_needRecoverServer.push_back((m_serverList.value(i)));
                }
            }
            modbus_close((m_serverList.value(i))->ctx);
        }
        m_mutex.unlock();
    }
    //开启恢复网络操作
    QTimer::singleShot(DevConfigPtr->networkModbus.recoveryTime * 1000, this, SLOT(slotrecoverConnect()));
}

/*
 * 功能：
 *  周期采样槽函数
 * 参数：
 *
 * 返回值：
 * 	  无
 */
void ThreadEthernetModbusClient::slotSimple()
{
    //quint32 timeOut = DevConfigPtr->networkModbus.commInterval / m_cmdTotal;
    //判断是否需要保持连接
    if (DevConfigPtr->networkModbus.connectHold) {
        //查看保持连接定时器使用正在运行
        if (p_holdTimer->isActive()) {
            //停止保持连接定时器
            p_holdTimer->stop();
            //关闭各个连接
            slotHold();
        }
        //遍历各个服务器。执行每个服务器中的命令
        for (int i = 0; i < m_serverList.size(); ++i) {
            if ((m_serverList.value(i))->state) {
                modbus_connect((m_serverList.value(i)->ctx));
                (m_serverList.value(i))->isConnect = true;
                for (int j = 0; j < (m_serverList.value(i)->cmdSize); ++j) {
                    //set modbus timeout
                    if (DevConfigPtr->networkModbus.cmdRegNo[m_serverList.value(i)->cmd[j]] <= 101600) {
                        modbus_set_byte_timeout((m_serverList.value(i))->ctx, DevConfigPtr->networkModbus.recoveryTime, 0);
                    }
                    else {
                        modbus_set_response_timeout((m_serverList.value(i))->ctx, DevConfigPtr->networkModbus.recoveryTime, 0);
                    }
                    //执行命令
                    handleCmd(i, (m_serverList.value(i))->ctx, (m_serverList.value(i)->cmd[j]));
                }
                //将服务器加入保持连接队列
                m_needHoldConnectServer.push_back((m_serverList.value(i)));
            }
            else {
                continue;
            }
        }
        //开启保持连接
        p_holdTimer->start(DevConfigPtr->networkModbus.connectTime * 1000);
    }
    //不需要保持连接
    else {
        //遍历各个服务器。执行每个服务器中的命令
        for (int i = 0; i < m_serverList.size(); ++i) {
            if ((m_serverList.value(i))->state) {
                modbus_connect((m_serverList.value(i)->ctx));
                (m_serverList.value(i))->isConnect = true;
                for (int j = 0; j < (m_serverList.value(i)->cmdSize); ++j) {
                    //set modbus timeout
                    if (DevConfigPtr->networkModbus.cmdRegNo[m_serverList.value(i)->cmd[j]] <= 101600) {
                        modbus_set_byte_timeout((m_serverList.value(i))->ctx, DevConfigPtr->networkModbus.recoveryTime, 0);
                    }
                    else {
                        modbus_set_response_timeout((m_serverList.value(i))->ctx, DevConfigPtr->networkModbus.recoveryTime, 0);
                    }
                    //执行命令
                    handleCmd(i, (m_serverList.value(i))->ctx, (m_serverList.value(i)->cmd[j]));
                }
                modbus_close((m_serverList.value(i)->ctx));
                (m_serverList.value(i))->isConnect = false;
            }
            else {
                continue;
            }
        }
    }
}

/*
 * 功能：
 *  保持连接槽函数
 * 参数：
 *
 * 返回值：
 * 	  无
 */
void ThreadEthernetModbusClient::slotHold()
{
    //遍历各个服务器，执行断开连接操作和出队列
    while(!m_needHoldConnectServer.isEmpty()){
        modbus_close((m_needHoldConnectServer.value(0))->ctx);
        (m_needHoldConnectServer.value(0))->isConnect = false;
        m_needHoldConnectServer.pop_front();
    }
}

/*
 * 功能：
 *      恢复连接槽函数
 * 参数：
 *
 * 返回值：
 * 	  无
 */
void ThreadEthernetModbusClient::slotrecoverConnect()
{
    int rc;
    //遍历各个需要进行恢复连接的服务器
    for (int i = 0; i < m_needRecoverServer.size(); ++i) {
        m_mutex.lock();
        //判断是否正在连接中，是则关闭，
        if ((m_needRecoverServer.value(i))->isConnect) {
            modbus_close((m_needRecoverServer.value(i))->ctx);
            (m_needRecoverServer.value(i))->isConnect = false;
        }
        //尝试连接服务器
        rc = modbus_connect((m_needRecoverServer.value(i))->ctx);
        if (rc == 0) {
            (m_needRecoverServer.value(i))->state = true;
        }
        else {
            (m_needRecoverServer.value(i))->state = false;
        }
        modbus_close((m_needRecoverServer.value(i))->ctx);
        (m_needRecoverServer.value(i))->isConnect = false;
        m_mutex.unlock();
    }
    //清空队列
    while (!m_needRecoverServer.empty()) {
        (m_needRecoverServer.first())->isRecover = false;
        m_needRecoverServer.pop_front();
    }
}


/******************************COM*************************************/
#if 1
/*
 * 功能：
 *  构造函数
 * 参数：
 *
 * 返回值：
 * 	  无
 */
ThreadCOMModbusClient::ThreadCOMModbusClient(QObject *parent) :
    QObject(parent)
{
    p_bpsTimer = new QTimer;
    connect(p_bpsTimer, SIGNAL(timeout()), this, SLOT(slotBps()));

    p_simpleTimer = new QTimer;
    connect(p_simpleTimer, SIGNAL(timeout()), this, SLOT(slotSimple()));

    p_recoverTimer = new QTimer;
    connect(p_recoverTimer, SIGNAL(timeout()), this, SLOT(slotrecover()));
}

/*
 * 功能：
 *  初始化函数
 * 参数：
 *
 * 返回值：
 * 	  无
 */
void ThreadCOMModbusClient::init()
{
    //判断串口是否为主机功能和主机功能是否打开
    if (DevConfigPtr->serialConfig.serialMode == Serial_Mode_Modbus_Master) {
        if (DevConfigPtr->serialConfig.clientOnOff) {
            newModbusServer();//new modbus RTU
            if (p_ctx != NULL) {
                m_totalDataSize = 0; //初始化数据流长度
                m_currentCmd = 0;    //初始化保存命令号
                m_overLimit = false; //初始化标志位
                if (DevConfigPtr->serialConfig.commCmdInterval != 0) {
                    m_cmdNumHInterval = DevConfigPtr->serialConfig.commInterval / DevConfigPtr->serialConfig.commCmdInterval;
                }
                else {
                    m_cmdNumHInterval = 0;
                }
                getVaildCmdNo();  //获取有效命令
                p_simpleTimer->start(DevConfigPtr->serialConfig.commInterval);//开启采样定时器
                p_bpsTimer->start(1000); //开启波特率限制判断定时器
                p_recoverTimer->start(DevConfigPtr->serialConfig.recoveryTime*1000);//开启恢复动作定时器
            }
        }
        else {
            qDebug("serial master close!");
        }
    }
    else {
        qDebug("serial mode is not master!");
    }
}

/*
 * 功能：
 *  获取有效命令
 * 参数：
 *
 * 返回值：
 * 	  无
 */
void ThreadCOMModbusClient::getVaildCmdNo()
{
    m_summaryList.clear();

    for (qint8 i = 0; i < COMM_CMD_SERIAL_NUMBER; ++i) {
        if (DevConfigPtr->serialConfig.cmdType[i] == 0)
            continue;
        serialCmd[i].cmdNo = i;
        serialCmd[i].recoverState = false;
        serialCmd[i].state = true;
        serialCmd[i].recoverTime = 0;
        m_cmdNoList.push_back(&(serialCmd[i]));

        //添加配置输出信息
        Modbus_Summary_Data tempSummaryData;
        tempSummaryData.cmdNum = i;
        tempSummaryData.cmdServeCell = DevConfigPtr->serialConfig.cmdServeNo[i];
        tempSummaryData.cmdRegNo = DevConfigPtr->serialConfig.cmdRegNo[i];
        tempSummaryData.cmdRegType = DevConfigPtr->serialConfig.cmdRegType[i];
        tempSummaryData.cmdType = DevConfigPtr->serialConfig.cmdType[i];
        tempSummaryData.cmdStartNo = DevConfigPtr->serialConfig.cmdStartNo[i];
        tempSummaryData.cmdEndNo = DevConfigPtr->serialConfig.cmdEndNo[i];
        m_summaryList.push_back(tempSummaryData);
    }

    //初始化配置信息的状态默认值
    for (int i = 0; i < m_cmdNoList.size(); ++i) {
        Modbus_Summary_Data s;

        if (i < m_cmdNumHInterval && m_cmdNumHInterval != 0) {
            s = m_summaryList.value(i);
            s.status = ModbusStatus_Valid;
            m_summaryList.replace(i, s);
        }
        else {
            s = m_summaryList.value(i);
            s.status = ModbusStatus_Dropout;
            m_summaryList.replace(i, s);
        }
    }

    //通知窗体更新显示
    emit sigModbusCommSummaryChange();
}

/*
 * 功能：
 *  new出modbusRTU
 * 参数：
 *
 * 返回值：
 * 	  无
 */
int ThreadCOMModbusClient::newModbusServer()
{
    int baud,stop_bit;
//    int data_bit;
    char parity;

    //获取波特率
    baud = DevConfigPtr->serialConfig.serialBaudrate;

    m_limitDataSize = baud / 8;//波特率限制数据流长度

    //获取校验位
    switch (DevConfigPtr->serialConfig.serialParity) {
    case Serial_Parity_EvenParity:
        parity = 'E';
        break;
    case Serial_Parity_OddParity:
        parity = 'O';
        break;
    default:
        break;
    }

    //获取停止位
    switch (DevConfigPtr->serialConfig.serialStopBit) {
    case Serial_StopBits_1:
        stop_bit = 1;
        break;
    case Serial_StopBits_2:
        stop_bit = 2;
        break;
    default:
        break;
    }

//    //获取数据长度
//    switch (DevConfigPtr->serialConfig.serilaDataLen) {
//    case Serial_DataLen_7:
//        data_bit = 7;
//        break;
//    case Serial_DataLen_8:
//        data_bit = 8;
//        break;
//    default:
//        break;
//    }
    //new出modbusRTU
    p_ctx = modbus_new_rtu(SERIAL_PORT_OF_HOST, baud, parity, 8, stop_bit);
    if (p_ctx == NULL) {
        qDebug("modbus new rtu fail!");
        return -1;
    }
    return 0;
}

/*
 * 功能：
 *  执行命令函数
 * 参数：
 *  cmdNo:命令号
 * 返回值：
 * 	  无
 */
int ThreadCOMModbusClient::handleCmd(int cmdNo)
{
    int rc;
    bool flag = true;

    //connect
    rc = modbus_connect(p_ctx);
    if (rc == -1) {
        qDebug("modbus connect fail! --COM");
        flag = false;
    }
    if (flag) {
        //set modbus timeout
        if (DevConfigPtr->serialConfig.cmdRegNo[cmdNo] <= 101600) {
            modbus_set_byte_timeout(p_ctx, DevConfigPtr->serialConfig.recoveryTime, 0);
        }
        else {
            modbus_set_response_timeout(p_ctx, DevConfigPtr->serialConfig.recoveryTime, 0);
        }

        //判断命令类型
        switch (DevConfigPtr->serialConfig.cmdType[cmdNo]) {
        /**off**/
        case 0:
            qDebug("No Action!");
            break;
            /**read**/
        case 1:
            //执行读命令后，进行通信通道的报警
            rc = readAction(cmdNo);
            commAlarmHandle(cmdNo);
            break;
            /**write**/
        case 2:
            //执行写命令
            rc = writeAction(cmdNo);
            break;
        default:
            break;
        }
    }
    modbus_close(p_ctx);

    return rc;
}

/*
 * 功能：
 *  执行读命令函数
 * 参数：
 *  cmdNo:命令号
 * 返回值：
 * 	  无
 */
int ThreadCOMModbusClient::readAction(int cmdNo)
{
    int nb;
    int addr;
    int rc;

    //根据寄存器号获取地址，如地址小于0，则是无效地址
    addr = getRegisterMapAddr(DevConfigPtr->serialConfig.cmdRegNo[cmdNo]);
    if (addr < 0) {
        qDebug("Register addr error!");
        return -1;
    }

    //获取读取数目
    nb = DevConfigPtr->serialConfig.cmdEndNo[cmdNo] - DevConfigPtr->serialConfig.cmdStartNo[cmdNo] + 1;
    //判断数据类型，如为bit这从线圈或继电器中读取，否则从输入寄存器或者保持寄存器中读取
    if (DevConfigPtr->serialConfig.cmdRegType[cmdNo] == Register_Type_BIT) {
        quint8 data[nb];
        ::memset(data, 0, nb);
        //根据寄存器号判断是从线圈读取还是继电器读取
        if (DevConfigPtr->serialConfig.cmdRegNo[cmdNo] <= COIL_SWITCH_END) {
            //读取数据
            rc = modbus_read_bits(p_ctx, addr, nb, data);
            if (rc == -1) {
                qDebug("Read coil ragister fail!");
                return -1;
            }
        }
        else if (DevConfigPtr->serialConfig.cmdRegNo[cmdNo] >= RELAY_1_START){
            //读取数据
            rc = modbus_read_input_bits(p_ctx, addr, nb, data);
            if (rc == -1) {
                qDebug("Read relay ragister fail!");
                return -1;
            }
        }
        else {
            qDebug("Read bit ragister number fail!");
        }
        //写入通信通道中
        writeDataToCommChanel(cmdNo, data, NULL, nb, 1);
    }
    //按位读取
    else {
        //数据类型为int16时，数目就为通道数目，数据类型为int32/float时，数目就是2倍的通道数目
        if (DevConfigPtr->serialConfig.cmdRegType[cmdNo] == Register_Type_INT16 ||
                DevConfigPtr->serialConfig.cmdRegType[cmdNo] == Register_Type_UINT16) {
            quint16 data[nb];
            ::memset(data, 0, nb);
            //根据寄存器号判断是从输入寄存器读还是保持寄存器读取
            if (DevConfigPtr->serialConfig.cmdRegNo[cmdNo] <= INPUTREG_EXPAND_6_4_END) {
                //读取数据
                rc = modbus_read_input_registers(p_ctx, addr, nb, data);
                if (rc == -1) {
                    qDebug("Read input ragister fail! --int16");
                    return -1;
                }
            }
            else if (DevConfigPtr->serialConfig.cmdRegNo[cmdNo] >= HOLDREG_MAIN_IO_START) {
                //读取数据
                rc = modbus_read_registers(p_ctx, addr, nb, data);
                if (rc == -1) {
                    qDebug("Read hold ragister fail! --int16");
                    return -1;
                }
            }
            else {
                qDebug("Read word ragister number fail!");
            }
            //将数据写入到通信通道中
            writeDataToCommChanel(cmdNo, NULL, data, nb, 2);
            nb *= 2;
        }
        else {
            quint16 data[nb*2];
            ::memset(data, 0, nb*2);
            //根据寄存器号判断是从输入寄存器读还是保持寄存器读取
            if (DevConfigPtr->serialConfig.cmdRegNo[cmdNo] <= INPUTREG_EXPAND_6_4_END) {
                //读取数据
                rc = modbus_read_input_registers(p_ctx, addr, nb*2, data);
                if (rc == -1) {
                    qDebug("Read input ragister fail! --32");
                    return -1;
                }
            }
            else if (DevConfigPtr->serialConfig.cmdRegNo[cmdNo] >= HOLDREG_MAIN_IO_START) {
                //读取数据
                rc = modbus_read_registers(p_ctx, addr, nb*2, data);
                if (rc == -1) {
                    qDebug("Read hold ragister fail! --32");
                    return -1;
                }
            }
            else {
                qDebug("Read word ragister number fail!");
            }
            //如果是大端的数据类型，则先进行数据的转换在写入到通信通道中
            switch (DevConfigPtr->serialConfig.cmdRegType[cmdNo]) {
            case Register_Type_INT32B:
            case Register_Type_UINT32B:
            case Register_Type_FLOATB:
                quint16 temp;
                for(int i = 0; i < nb*2; i++) {
                    temp = data[i];
                    data[i] = data[i+1];
                    data[++i] = temp;
                }
                break;
            default:
                break;
            }
            //将数据写入到通信通道中
            writeDataToCommChanel(cmdNo, NULL, data, nb*2, 3);
            nb *= 4;
        }
    }
    return nb;
}

/*
 * 功能：
 *  写命令执行函数
 * 参数：
 *  cmdNo：命令号
 * 返回值：
 * 	  无
 */
int ThreadCOMModbusClient::writeAction(int cmdNo)
{
    int nb;
    int addr;
    int chanNo_start,chanNo_end;
    int chanType,chanUnit,chanModule;

    //根据寄存器号获取地址，如地址小于0，则是无效地址
    addr = getRegisterMapAddr(DevConfigPtr->serialConfig.cmdRegNo[cmdNo]);
    if (addr < 0) {
        qDebug("Register addr error!");
        return -1;
    }

    //获取读取数目
    if (DevConfigPtr->serialConfig.cmdChType[cmdNo] != Channel_Type_Comm  &&
            DevConfigPtr->serialConfig.cmdChType[cmdNo] != Channel_Type_Math ) {
        chanNo_start = CHANNEL_NUM(DevConfigPtr->serialConfig.cmdStartNo[cmdNo]);
        chanNo_end = CHANNEL_NUM(DevConfigPtr->serialConfig.cmdEndNo[cmdNo]);
        chanType = CHANNEL_TYPE(DevConfigPtr->serialConfig.cmdStartNo[cmdNo]);
        chanUnit = CHANNEL_UNIT(DevConfigPtr->serialConfig.cmdStartNo[cmdNo]);
        if (chanUnit) {
            chanModule = CHANNEL_MODULE(DevConfigPtr->serialConfig.cmdStartNo[cmdNo]);
        }
        nb = chanNo_end - chanNo_start + 1;
    }
    else {
        chanNo_start = DevConfigPtr->serialConfig.cmdStartNo[cmdNo];
        nb = DevConfigPtr->serialConfig.cmdEndNo[cmdNo] - DevConfigPtr->serialConfig.cmdStartNo[cmdNo] + 1;
    }

    quint8 data_b[nb];
    float f_data[nb];
    int j = 0;

    switch (DevConfigPtr->serialConfig.cmdChType[cmdNo]) {
    case Channel_Type_Comm:
    {
        ::memset(f_data,0,nb);

        for (int i = chanNo_start - 1; i < chanNo_end; ++i)
            f_data[j++] = *(float*)(&g_originDataMap.holdReg_MainUnit_Comm_Data_Float32[2*i]);
//        //test
//        for (int i = 0;i<nb;++i)
//            qDebug("%f",f_data[i]);
//        //test

        nb = handleWriteAction(p_ctx,cmdNo,addr,NULL,f_data,nb,2);
        break;
    }
    case Channel_Type_Math:
    {
        ::memset(f_data,0,nb);

        for (int i = chanNo_start - 1; i < chanNo_end; ++i)
            f_data[j++] = *(float*)(&g_originDataMap.holdReg_MainUnit_Comm_Data_Float32[2*i]);
//        //test
//        for (int i = 0;i<nb;++i)
//            qDebug("%f",f_data[i]);
//        //test

        nb = handleWriteAction(p_ctx,cmdNo,addr,NULL,f_data,nb,2);

        break;
    }
    default:
    {
        switch (chanType) {
        case Channel_Type_AI:
        {
            if (chanType == 0) {
                ::memset(f_data,0,nb);
                for (int i = chanNo_start - 1; i < chanNo_end; ++i)
                    f_data[j++] = *(float*)(&g_originDataMap.inputReg_MainUnit_IO_Data_Float32[chanModule][2*i]);
                //test
                for (int i = 0;i<nb;++i)
                    qDebug("%f",f_data[i]);
                //test
            }
            else {
                ::memset(f_data,0,nb);
                for (int i = chanNo_start - 1; i < chanNo_end; ++i)
                    f_data[j++] = *(float*)(&g_originDataMap.inputReg_ExtUnit_IO_Data_Float32[chanUnit][chanModule][2*i]);
                //test
                for (int i = 0;i<nb;++i)
                    qDebug("%f",f_data[i]);
                //test
            }
            nb = handleWriteAction(p_ctx,cmdNo,addr,NULL,f_data,nb,2);
            break;
        }
        case Channel_Type_DI:
        {
            ::memset(data_b,0,nb);
            for (int i = chanNo_start - 1; i < chanNo_end; ++i) {
                data_b[j++] = g_originDataMap.coil_MainUnit_IO_Data_Bit[chanModule][i];
            }
            nb = handleWriteAction(p_ctx,cmdNo,addr,data_b,NULL,nb,1);
            break;
        }
        case Channel_Type_DO:
        {
            ::memset(data_b,0,nb);
            for (int i = chanNo_start - 1; i < chanNo_end; ++i) {
                data_b[j++] = g_originDataMap.relay_MainUnit_IO_Data_Bit[chanModule][i];
            }
            nb = handleWriteAction(p_ctx,cmdNo,addr,data_b,NULL,nb,1);
            break;
        }
        default:
            break;
        }
        break;
    }
    }

    return nb;
}

/*
 * 功能：
 *  根据寄存器号获得在寄存器结构体中对应的地址
 * 参数：
 *  regNo:命令号
 * 返回值：
 * 	  无
 */
int ThreadCOMModbusClient::getRegisterMapAddr(quint32 regNo)
{
    int addr;
    /******input register main*******/
    if (regNo >= INPUTREG_MAIN_IO_START && regNo <=INPUTREG_MAIN_IO_END) {
        addr = regNo - INPUTREG_MAIN_IO_START;
    }
    else if (regNo >= INPUTREG_MAIN_IO_C_START && regNo <=INPUTREG_MAIN_IO_C_END) {
        addr = regNo - INPUTREG_MAIN_IO_C_START
                + g_originDataMap.inputReg_MainUnit_IO_Data_Int32_Linear[0]
                - g_originDataMap.inputReg_MainUnit_IO_Data_Int32[0];
    }
    else if (regNo >= INPUTREG_MAIN_OPERATE_START && regNo <=INPUTREG_MAIN_OPERATE_END) {
        addr = regNo - INPUTREG_MAIN_OPERATE_START
                + g_originDataMap.inputReg_MainUnit_Math_Data_Int32
                - g_originDataMap.inputReg_MainUnit_IO_Data_Int32[0];
    }
    else if (regNo >= INPUTREG_MAIN_STATUS_START && regNo <=INPUTREG_MAIN_STATUS_END) {
        addr = regNo - INPUTREG_MAIN_STATUS_START
                + g_originDataMap.inputReg_Device_None_Status_Int16
                - (qint16*)g_originDataMap.inputReg_MainUnit_IO_Data_Int32[0];
    }
    /******input register expand*******/
    /*****etx1****/
    else if (regNo >= INPUTREG_EXPAND_1_1_START && regNo <=INPUTREG_EXPAND_1_1_END) {
        addr = regNo - INPUTREG_EXPAND_1_1_START
                + g_originDataMap.inputReg_ExtUnit_IO_Data_Int32[0][0]
                - g_originDataMap.inputReg_MainUnit_IO_Data_Int32[0];
    }
    else if (regNo >= INPUTREG_EXPAND_1_2_START && regNo <=INPUTREG_EXPAND_1_2_END) {
        addr = regNo - INPUTREG_EXPAND_1_2_START
                + g_originDataMap.inputReg_ExtUnit_IO_Data_Float32[0][0]
                - g_originDataMap.inputReg_MainUnit_IO_Data_Int32[0];
    }
    else if (regNo >= INPUTREG_EXPAND_1_3_START && regNo <=INPUTREG_EXPAND_1_3_END) {
        addr = regNo - INPUTREG_EXPAND_1_3_START
                + g_originDataMap.inputReg_ExtUnit_IO_Data_Int16[0][0]
                - (qint16*)g_originDataMap.inputReg_MainUnit_IO_Data_Int32[0];
    }
    else if (regNo >= INPUTREG_EXPAND_1_4_START && regNo <=INPUTREG_EXPAND_1_4_END) {
        addr = regNo - INPUTREG_EXPAND_1_4_START
                + g_originDataMap.inputReg_ExtUnit_IO_Status_Int16[0][0]
                - (qint16*)g_originDataMap.inputReg_MainUnit_IO_Data_Int32[0];
    }
    /*****etx2****/
    else if (regNo >= INPUTREG_EXPAND_2_1_START && regNo <=INPUTREG_EXPAND_2_1_END) {
        addr = regNo - INPUTREG_EXPAND_2_1_START
                + g_originDataMap.inputReg_ExtUnit_IO_Data_Int32[1][0]
                - g_originDataMap.inputReg_MainUnit_IO_Data_Int32[0];
    }
    else if (regNo >= INPUTREG_EXPAND_2_2_START && regNo <=INPUTREG_EXPAND_2_2_END) {
        addr = regNo - INPUTREG_EXPAND_2_2_START
                + g_originDataMap.inputReg_ExtUnit_IO_Data_Float32[1][0]
                - g_originDataMap.inputReg_MainUnit_IO_Data_Int32[0];
    }
    else if (regNo >= INPUTREG_EXPAND_2_3_START && regNo <=INPUTREG_EXPAND_2_3_END) {
        addr = regNo - INPUTREG_EXPAND_2_3_START
                + g_originDataMap.inputReg_ExtUnit_IO_Data_Int16[1][0]
                - (qint16*)g_originDataMap.inputReg_MainUnit_IO_Data_Int32[0];
    }
    else if (regNo >= INPUTREG_EXPAND_2_4_START && regNo <=INPUTREG_EXPAND_2_4_END) {
        addr = regNo - INPUTREG_EXPAND_2_4_START
                + g_originDataMap.inputReg_ExtUnit_IO_Status_Int16[1][0]
                - (qint16*)g_originDataMap.inputReg_MainUnit_IO_Data_Int32[0];
    }
    /*****etx3****/
    else if (regNo >= INPUTREG_EXPAND_3_1_START && regNo <=INPUTREG_EXPAND_3_1_END) {
        addr = regNo - INPUTREG_EXPAND_3_1_START
                + g_originDataMap.inputReg_ExtUnit_IO_Data_Int32[2][0]
                - g_originDataMap.inputReg_MainUnit_IO_Data_Int32[0];
    }
    else if (regNo >= INPUTREG_EXPAND_3_2_START && regNo <=INPUTREG_EXPAND_3_2_END) {
        addr = regNo - INPUTREG_EXPAND_3_2_START
                + g_originDataMap.inputReg_ExtUnit_IO_Data_Float32[2][0]
                - g_originDataMap.inputReg_MainUnit_IO_Data_Int32[0];
    }
    else if (regNo >= INPUTREG_EXPAND_3_3_START && regNo <=INPUTREG_EXPAND_3_3_END) {
        addr = regNo - INPUTREG_EXPAND_3_3_START
                + g_originDataMap.inputReg_ExtUnit_IO_Data_Int16[2][0]
                - (qint16*)g_originDataMap.inputReg_MainUnit_IO_Data_Int32[0];
    }
    else if (regNo >= INPUTREG_EXPAND_3_4_START && regNo <=INPUTREG_EXPAND_3_4_END) {
        addr = regNo - INPUTREG_EXPAND_3_4_START
                + g_originDataMap.inputReg_ExtUnit_IO_Status_Int16[2][0]
                - (qint16*)g_originDataMap.inputReg_MainUnit_IO_Data_Int32[0];
    }
    /*****etx4****/
    else if (regNo >= INPUTREG_EXPAND_4_1_START && regNo <=INPUTREG_EXPAND_4_1_END) {
        addr = regNo - INPUTREG_EXPAND_4_1_START
                + g_originDataMap.inputReg_ExtUnit_IO_Data_Int32[3][0]
                - g_originDataMap.inputReg_MainUnit_IO_Data_Int32[0];
    }
    else if (regNo >= INPUTREG_EXPAND_4_2_START && regNo <=INPUTREG_EXPAND_4_2_END) {
        addr = regNo - INPUTREG_EXPAND_4_2_START
                + g_originDataMap.inputReg_ExtUnit_IO_Data_Float32[3][0]
                - g_originDataMap.inputReg_MainUnit_IO_Data_Int32[0];
    }
    else if (regNo >= INPUTREG_EXPAND_4_3_START && regNo <=INPUTREG_EXPAND_4_3_END) {
        addr = regNo - INPUTREG_EXPAND_4_3_START
                + g_originDataMap.inputReg_ExtUnit_IO_Data_Int16[3][0]
                - (qint16*)g_originDataMap.inputReg_MainUnit_IO_Data_Int32[0];
    }
    else if (regNo >= INPUTREG_EXPAND_4_4_START && regNo <=INPUTREG_EXPAND_4_4_END) {
        addr = regNo - INPUTREG_EXPAND_4_4_START
                + g_originDataMap.inputReg_ExtUnit_IO_Status_Int16[3][0]
                - (qint16*)g_originDataMap.inputReg_MainUnit_IO_Data_Int32[0];
    }
    /*****etx5****/
    else if (regNo >= INPUTREG_EXPAND_5_1_START && regNo <=INPUTREG_EXPAND_5_1_END) {
        addr = regNo - INPUTREG_EXPAND_5_1_START
                + g_originDataMap.inputReg_ExtUnit_IO_Data_Int32[4][0]
                - g_originDataMap.inputReg_MainUnit_IO_Data_Int32[0];
    }
    else if (regNo >= INPUTREG_EXPAND_5_2_START && regNo <=INPUTREG_EXPAND_5_2_END) {
        addr = regNo - INPUTREG_EXPAND_5_2_START
                + g_originDataMap.inputReg_ExtUnit_IO_Data_Float32[4][0]
                - g_originDataMap.inputReg_MainUnit_IO_Data_Int32[0];
    }
    else if (regNo >= INPUTREG_EXPAND_5_3_START && regNo <=INPUTREG_EXPAND_5_3_END) {
        addr = regNo - INPUTREG_EXPAND_5_3_START
                + g_originDataMap.inputReg_ExtUnit_IO_Data_Int16[4][0]
                - (qint16*)g_originDataMap.inputReg_MainUnit_IO_Data_Int32[0];
    }
    else if (regNo >= INPUTREG_EXPAND_5_4_START && regNo <=INPUTREG_EXPAND_5_4_END) {
        addr = regNo - INPUTREG_EXPAND_5_4_START
                + g_originDataMap.inputReg_ExtUnit_IO_Status_Int16[4][0]
                - (qint16*)g_originDataMap.inputReg_MainUnit_IO_Data_Int32[0];
    }
    /*****etx6****/
    else if (regNo >= INPUTREG_EXPAND_6_1_START && regNo <=INPUTREG_EXPAND_6_1_END) {
        addr = regNo - INPUTREG_EXPAND_6_1_START
                + g_originDataMap.inputReg_ExtUnit_IO_Data_Int32[5][0]
                - g_originDataMap.inputReg_MainUnit_IO_Data_Int32[0];
    }
    else if (regNo >= INPUTREG_EXPAND_6_2_START && regNo <=INPUTREG_EXPAND_6_2_END) {
        addr = regNo - INPUTREG_EXPAND_6_2_START
                + g_originDataMap.inputReg_ExtUnit_IO_Data_Float32[5][0]
                - g_originDataMap.inputReg_MainUnit_IO_Data_Int32[0];
    }
    else if (regNo >= INPUTREG_EXPAND_6_3_START && regNo <=INPUTREG_EXPAND_6_3_END) {
        addr = regNo - INPUTREG_EXPAND_6_3_START
                + g_originDataMap.inputReg_ExtUnit_IO_Data_Int16[5][0]
                - (qint16*)g_originDataMap.inputReg_MainUnit_IO_Data_Int32[0];
    }
    else if (regNo >= INPUTREG_EXPAND_6_4_START && regNo <=INPUTREG_EXPAND_6_4_END) {
        addr = regNo - INPUTREG_EXPAND_6_4_START
                + g_originDataMap.inputReg_ExtUnit_IO_Status_Int16[5][0]
                - (qint16*)g_originDataMap.inputReg_MainUnit_IO_Data_Int32[0];
    }
    /*******hold register main********/
    else if (regNo >= HOLDREG_MAIN_IO_START && regNo <=HOLDREG_MAIN_IO_END) {
        addr = regNo - HOLDREG_MAIN_IO_START;
    }
    else if (regNo >= HOLDREG_MAIN_IO_C_START && regNo <=HOLDREG_MAIN_IO_C_END) {
        addr = regNo - INPUTREG_MAIN_IO_C_START
                + g_originDataMap.holdReg_MainUnit_IO_Data_Int32_Linear[0]
                - g_originDataMap.holdReg_mainUnit_IO_Data_Int32[0];
    }
    else if (regNo >= HOLDREG_MAIN_COMM_START && regNo <=HOLDREG_MAIN_COMM_END) {
        addr = regNo - HOLDREG_MAIN_COMM_START
                + g_originDataMap.holdReg_MainUnit_Comm_Data_Int32
                - g_originDataMap.holdReg_mainUnit_IO_Data_Int32[0];
    }
    else if (regNo >= HOLDREG_MAIN_SET_START && regNo <=HOLDREG_MAIN_SET_END) {
        addr = regNo - HOLDREG_MAIN_SET_START
                + &(g_originDataMap.holdReg_Device_Operate_Data_Int16_Record)
                - (qint16*)g_originDataMap.holdReg_mainUnit_IO_Data_Int32[0];
    }
    else if (regNo >= HOLDREG_MAIN_STATUS_START && regNo <=HOLDREG_MAIN_STATUS_END) {
        addr = regNo - HOLDREG_MAIN_STATUS_START
                + g_originDataMap.holdReg_Device_Switch_Data_Int16
                - (qint16*)g_originDataMap.holdReg_mainUnit_IO_Data_Int32[0];
    }
    /**********hold register expand***************/
    /*****etx1****/
    else if (regNo >= HOLDREG_EXPAND_1_1_START && regNo <=HOLDREG_EXPAND_1_1_END) {
        addr = regNo - HOLDREG_EXPAND_1_1_START
                + g_originDataMap.holdReg_ExtUnit_IO_Data_Int32[0][0]
                - g_originDataMap.holdReg_mainUnit_IO_Data_Int32[0];
    }
    else if (regNo >= HOLDREG_EXPAND_1_2_START && regNo <=HOLDREG_EXPAND_1_2_END) {
        addr = regNo - HOLDREG_EXPAND_1_2_START
                + g_originDataMap.holdReg_ExtUnit_IO_Data_Float32[0][0]
                - g_originDataMap.holdReg_mainUnit_IO_Data_Int32[0];
    }
    else if (regNo >= HOLDREG_EXPAND_1_3_START && regNo <=HOLDREG_EXPAND_1_3_END) {
        addr = regNo - HOLDREG_EXPAND_1_3_START
                + g_originDataMap.holdReg_ExtUnit_IO_Data_Int16[0][0]
                - (qint16*)g_originDataMap.holdReg_mainUnit_IO_Data_Int32[0];
    }
    else if (regNo >= HOLDREG_EXPAND_1_4_START && regNo <=HOLDREG_EXPAND_1_4_END) {
        addr = regNo - HOLDREG_EXPAND_1_4_START
                + g_originDataMap.holdReg_ExtUnit_IO_Status_Int16[0][0]
                - (qint16*)g_originDataMap.holdReg_mainUnit_IO_Data_Int32[0];
    }
    /*****etx2****/
    else if (regNo >= HOLDREG_EXPAND_2_1_START && regNo <=HOLDREG_EXPAND_2_1_END) {
        addr = regNo - HOLDREG_EXPAND_2_1_START
                + g_originDataMap.holdReg_ExtUnit_IO_Data_Int32[1][0]
                - g_originDataMap.holdReg_mainUnit_IO_Data_Int32[0];
    }
    else if (regNo >= HOLDREG_EXPAND_2_2_START && regNo <=HOLDREG_EXPAND_2_2_END) {
        addr = regNo - HOLDREG_EXPAND_2_2_START
                + g_originDataMap.holdReg_ExtUnit_IO_Data_Float32[1][0]
                - g_originDataMap.holdReg_mainUnit_IO_Data_Int32[0];
    }
    else if (regNo >= HOLDREG_EXPAND_2_3_START && regNo <=HOLDREG_EXPAND_2_3_END) {
        addr = regNo - HOLDREG_EXPAND_2_3_START
                + g_originDataMap.holdReg_ExtUnit_IO_Data_Int16[1][0]
                - (qint16*)g_originDataMap.holdReg_mainUnit_IO_Data_Int32[0];
    }
    else if (regNo >= HOLDREG_EXPAND_2_4_START && regNo <=HOLDREG_EXPAND_2_4_END) {
        addr = regNo - HOLDREG_EXPAND_2_4_START
                + g_originDataMap.holdReg_ExtUnit_IO_Status_Int16[1][0]
                - (qint16*)g_originDataMap.holdReg_mainUnit_IO_Data_Int32[0];
    }
    /*****etx3****/
    else if (regNo >= HOLDREG_EXPAND_3_1_START && regNo <=HOLDREG_EXPAND_3_1_END) {
        addr = regNo - HOLDREG_EXPAND_3_1_START
                + g_originDataMap.holdReg_ExtUnit_IO_Data_Int32[2][0]
                - g_originDataMap.holdReg_mainUnit_IO_Data_Int32[0];
    }
    else if (regNo >= HOLDREG_EXPAND_3_2_START && regNo <=HOLDREG_EXPAND_3_2_END) {
        addr = regNo - HOLDREG_EXPAND_3_2_START
                + g_originDataMap.holdReg_ExtUnit_IO_Data_Float32[2][0]
                - g_originDataMap.holdReg_mainUnit_IO_Data_Int32[0];
    }
    else if (regNo >= HOLDREG_EXPAND_3_3_START && regNo <=HOLDREG_EXPAND_3_3_END) {
        addr = regNo - HOLDREG_EXPAND_3_3_START
                + g_originDataMap.holdReg_ExtUnit_IO_Data_Int16[2][0]
                - (qint16*)g_originDataMap.holdReg_mainUnit_IO_Data_Int32[0];
    }
    else if (regNo >= HOLDREG_EXPAND_3_4_START && regNo <=HOLDREG_EXPAND_3_4_END) {
        addr = regNo - HOLDREG_EXPAND_3_4_START
                + g_originDataMap.holdReg_ExtUnit_IO_Status_Int16[2][0]
                - (qint16*)g_originDataMap.holdReg_mainUnit_IO_Data_Int32[0];
    }
    /*****etx4****/
    else if (regNo >= HOLDREG_EXPAND_4_1_START && regNo <=HOLDREG_EXPAND_4_1_END) {
        addr = regNo - HOLDREG_EXPAND_4_1_START
                + g_originDataMap.holdReg_ExtUnit_IO_Data_Int32[3][0]
                - g_originDataMap.holdReg_mainUnit_IO_Data_Int32[0];
    }
    else if (regNo >= HOLDREG_EXPAND_4_2_START && regNo <=HOLDREG_EXPAND_4_2_END) {
        addr = regNo - HOLDREG_EXPAND_4_2_START
                + g_originDataMap.holdReg_ExtUnit_IO_Data_Float32[3][0]
                - g_originDataMap.holdReg_mainUnit_IO_Data_Int32[0];
    }
    else if (regNo >= HOLDREG_EXPAND_4_3_START && regNo <=HOLDREG_EXPAND_4_3_END) {
        addr = regNo - HOLDREG_EXPAND_4_3_START
                + g_originDataMap.holdReg_ExtUnit_IO_Data_Int16[3][0]
                - (qint16*)g_originDataMap.holdReg_mainUnit_IO_Data_Int32[0];
    }
    else if (regNo >= HOLDREG_EXPAND_4_4_START && regNo <=HOLDREG_EXPAND_4_4_END) {
        addr = regNo - HOLDREG_EXPAND_4_4_START
                + g_originDataMap.holdReg_ExtUnit_IO_Status_Int16[3][0]
                - (qint16*)g_originDataMap.holdReg_mainUnit_IO_Data_Int32[0];
    }
    /*****etx5****/
    else if (regNo >= HOLDREG_EXPAND_5_1_START && regNo <=HOLDREG_EXPAND_5_1_END) {
        addr = regNo - HOLDREG_EXPAND_5_1_START
                + g_originDataMap.holdReg_ExtUnit_IO_Data_Int32[4][0]
                - g_originDataMap.holdReg_mainUnit_IO_Data_Int32[0];
    }
    else if (regNo >= HOLDREG_EXPAND_5_2_START && regNo <=HOLDREG_EXPAND_5_2_END) {
        addr = regNo - HOLDREG_EXPAND_5_2_START
                + g_originDataMap.holdReg_ExtUnit_IO_Data_Float32[4][0]
                - g_originDataMap.holdReg_mainUnit_IO_Data_Int32[0];
    }
    else if (regNo >= HOLDREG_EXPAND_5_3_START && regNo <=HOLDREG_EXPAND_5_3_END) {
        addr = regNo - HOLDREG_EXPAND_5_3_START
                + g_originDataMap.holdReg_ExtUnit_IO_Data_Int16[4][0]
                - (qint16*)g_originDataMap.holdReg_mainUnit_IO_Data_Int32[0];
    }
    else if (regNo >= HOLDREG_EXPAND_5_4_START && regNo <=HOLDREG_EXPAND_5_4_END) {
        addr = regNo - HOLDREG_EXPAND_5_4_START
                + g_originDataMap.holdReg_ExtUnit_IO_Status_Int16[4][0]
                - (qint16*)g_originDataMap.holdReg_mainUnit_IO_Data_Int32[0];
    }
    /*****etx6****/
    else if (regNo >= HOLDREG_EXPAND_6_1_START && regNo <=HOLDREG_EXPAND_6_1_END) {
        addr = regNo - HOLDREG_EXPAND_6_1_START
                + g_originDataMap.holdReg_ExtUnit_IO_Data_Int32[5][0]
                - g_originDataMap.holdReg_mainUnit_IO_Data_Int32[0];
    }
    else if (regNo >= HOLDREG_EXPAND_6_2_START && regNo <=HOLDREG_EXPAND_6_2_END) {
        addr = regNo - HOLDREG_EXPAND_6_2_START
                + g_originDataMap.holdReg_ExtUnit_IO_Data_Float32[5][0]
                - g_originDataMap.holdReg_mainUnit_IO_Data_Int32[0];
    }
    else if (regNo >= HOLDREG_EXPAND_6_3_START && regNo <=HOLDREG_EXPAND_6_3_END) {
        addr = regNo - HOLDREG_EXPAND_6_3_START
                + g_originDataMap.holdReg_ExtUnit_IO_Data_Int16[5][0]
                - (qint16*)g_originDataMap.holdReg_mainUnit_IO_Data_Int32[0];
    }
    else if (regNo >= HOLDREG_EXPAND_6_4_START && regNo <=HOLDREG_EXPAND_6_4_END) {
        addr = regNo - HOLDREG_EXPAND_6_4_START
                + g_originDataMap.holdReg_ExtUnit_IO_Status_Int16[5][0]
                - (qint16*)g_originDataMap.holdReg_mainUnit_IO_Data_Int32[0];
    }
    /***********coil**************/
    else if (regNo >= COIL_IO_START && regNo <=COIL_IO_END) {
        addr = regNo - COIL_IO_START;
    }
    else if (regNo >= COIL_COMM_STATUS_START && regNo <=COIL_COMM_STATUS_END) {
        addr = regNo - COIL_COMM_STATUS_START
                + g_originDataMap.coil_MainUnit_Comm_Status_Bit
                - g_originDataMap.coil_MainUnit_IO_Data_Bit[0];
    }
    else if (regNo >= COIL_SWITCH_START && regNo <=COIL_SWITCH_END) {
        addr = regNo - COIL_SWITCH_START
                + g_originDataMap.coil_MainUnit_Switch_Data_Bit
                - g_originDataMap.coil_MainUnit_IO_Data_Bit[0];
    }
    /**********relay**************/
    else if (regNo >= RELAY_1_START && regNo <=RELAY_1_END) {
        addr = regNo - RELAY_1_START;
    }
    else if (regNo >= RELAY_2_START && regNo <=RELAY_2_END) {
        addr = regNo - RELAY_2_START
                + g_originDataMap.relay_MainUnit_Math_Status_Bit
                - g_originDataMap.relay_MainUnit_IO_Data_Bit[0];
    }
    /**********Invalid register number*********/
    else {
        qDebug("Invalid register number!");
        addr = -2;
    }
    return addr + 1;
}

/*
 * 功能：
 *  将从服务器读取的数据写到通信通道中，根据flag的值来判断是那种类型的数据类型
 *      flag = 1时：bit，flag = 2时:int16,flag = 3时：int32/float
 * 参数：
 *  cmdNo:命令号，data_b:bit数据,data_w:word数据，nb：数据数目
 * 返回值：
 * 	  无
 */
void ThreadCOMModbusClient::writeDataToCommChanel(int cmdNo, quint8 *data_b, quint16 *data_w, int nb, int flag)
{
    int j = 0;
    float *f_data;

    //将bit数据写入到通信通道中
    if (flag == 1) {
        //判断数据读取的数目和应该读取的数目是否一致
        if ((quint32)nb != (DevConfigPtr->serialConfig.cmdEndNo[cmdNo] - DevConfigPtr->serialConfig.cmdStartNo[cmdNo] + 1)) {
            qDebug("bit read number lack!");
            return;
        }
        for (quint32 i = DevConfigPtr->serialConfig.cmdStartNo[cmdNo] - 1;\
             i <= DevConfigPtr->serialConfig.cmdEndNo[cmdNo]; ++i) {

            if (DevConfigPtr->chanComm[i].rangeConfig.rangeSwtich == 1) {
//                if (DevConfigPtr->chanComm[i].rangeConfig.rangeMax >= data_b[j] && DevConfigPtr->chanComm[i].rangeConfig.rangeMin <= data_b[j]) {
                g_originDataMap.coil_MainUnit_Comm_Data_Bit[i] = data_b[j];
                f_data = (float*)(&g_originDataMap.holdReg_MainUnit_Comm_Data_Float32[i*2]);
                *f_data = data_b[j];
//                }
            }
        }
    }
    //将int16类型的数据写入通信通道
    else if (flag == 2) {
        //判断数据读取的数目和应该读取的数目是否一致
        if ((quint32)nb != (DevConfigPtr->serialConfig.cmdEndNo[cmdNo] - DevConfigPtr->serialConfig.cmdStartNo[cmdNo] + 1)) {
            qDebug("word read number lack! --16");
            return;
        }
        for (quint32 i = DevConfigPtr->serialConfig.cmdStartNo[cmdNo] - 1;\
             i <= DevConfigPtr->serialConfig.cmdEndNo[cmdNo]; i++) {

            if (DevConfigPtr->chanComm[i].rangeConfig.rangeSwtich == 1) {
//                if (DevConfigPtr->chanComm[i].rangeConfig.rangeMax >= data_w[j] && DevConfigPtr->chanComm[i].rangeConfig.rangeMin <= data_w[j]) {
                f_data = (float*)(&g_originDataMap.holdReg_MainUnit_Comm_Data_Float32[i*2]);
                *f_data = data_w[j];
//                }
            }
            j++;
        }
    }
    //将int32/float类型的数据写入通信通道
    else if (flag == 3) {
        //判断数据读取的数目和应该读取的数目是否一致
        if ((quint32)nb/2 != (DevConfigPtr->serialConfig.cmdEndNo[cmdNo] - DevConfigPtr->serialConfig.cmdStartNo[cmdNo] + 1)) {
            qDebug("word read number lack! --16");
            return;
        }
        for (quint32 i = DevConfigPtr->serialConfig.cmdStartNo[cmdNo] - 1;\
             i <= DevConfigPtr->serialConfig.cmdEndNo[cmdNo]; ++i) {
            float data;

            if (DevConfigPtr->chanComm[i].rangeConfig.rangeSwtich == 1) {
                data = data_w[j] + (data_w[j+1]<<16);
//                if (DevConfigPtr->chanComm[i].rangeConfig.rangeMax >= data && DevConfigPtr->chanComm[i].rangeConfig.rangeMin <= data) {
                f_data = (float*)(&g_originDataMap.holdReg_MainUnit_Comm_Data_Float32[i*2]);
                *f_data = data;
//                }
            }
            j++;
        }
    }
    else {
        qDebug("writeDataToCommChanel flag fail!");
    }
}

/*
 * 功能：
 *      通信通道报警处理
 * 参数：
 *      cmdNo：命令号
 * 返回值：
 * 	  无
 */
void ThreadCOMModbusClient::commAlarmHandle(int cmdNo)
{
    float* value;

    //遍历写入的通信通道，对每个通信通道进行报警处理
    for (quint32 i = DevConfigPtr->serialConfig.cmdStartNo[cmdNo];
         i <= DevConfigPtr->serialConfig.cmdEndNo[cmdNo]; i++) {

        value = (float*)(&g_originDataMap.holdReg_MainUnit_Comm_Data_Float32[i*2]);
        int pos = DevConfigPtr->chanComm[i].rangeConfig.rangePos;
        if (DevConfigPtr->chanComm[i].rangeConfig.rangeSwtich == 1) {

            //分别处理通道状况的0-3位的电平
            for (int j = 0; j < 4; j++) {
                if (DevConfigPtr->chanComm[i].alarmConfig.alarmOnOff[j]) {
                    g_originDataMap.coil_MainUnit_Comm_Status_Bit[i] |= 0<<j;
                    //判断报警类型
                    switch (DevConfigPtr->chanComm[i].alarmConfig.alarmType[j]) {
                        //上限
                    case Alarm_Type_H:
                        if (*value >= (float)(DevConfigPtr->chanComm[i].alarmConfig.alarmValue[j]) /:: pow10(pos)) {
                            g_originDataMap.coil_MainUnit_Comm_Status_Bit[i] |= 1<<j;
                        }
                        break;
                        //下限
                    case Alarm_Type_L:
                        if (*value <= (float)(DevConfigPtr->chanComm[i].alarmConfig.alarmValue[j]) /:: pow10(pos)) {
                            g_originDataMap.coil_MainUnit_Comm_Status_Bit[i] |= 1<<j;
                        }
                        break;
                        //延迟上限
                    case Alarm_Type_T:
                        if (*value > (float)(DevConfigPtr->chanComm[i].alarmConfig.alarmValue[j]) /:: pow10(pos)) {
                            g_originDataMap.coil_MainUnit_Comm_Status_Bit[i] |= 1<<j;
                        }
                        break;
                        //延迟下限
                    case Alarm_Type_t:
                        if (*value < (float)(DevConfigPtr->chanComm[i].alarmConfig.alarmValue[j]) /:: pow10(pos)) {
                            g_originDataMap.coil_MainUnit_Comm_Status_Bit[i] |= 1<<j;
                        }
                        break;
                    default:
                        break;
                    }
                }
            }


        }
    }
}

int ThreadCOMModbusClient::handleWriteAction(modbus_t *ctx, int cmdNo, int addr, quint8 *data_b, float *data_f, int nb, int flag)
{
    int rc;

    if (flag == 1) {
        if (DevConfigPtr->serialConfig.cmdRegType[cmdNo] == Register_Type_BIT) {
            //将数据写到服务器的线圈中
            rc = modbus_write_bits(ctx, addr, nb, data_b);
            if (rc == -1) {
                qDebug("write coil fail!");
            }
        }
        else if (DevConfigPtr->serialConfig.cmdRegType[cmdNo] == Register_Type_INT16 ||
                 DevConfigPtr->serialConfig.cmdRegType[cmdNo] == Register_Type_UINT16 ) {
            quint16 dataT_b[nb];
            for (int i = 0; i < nb; ++i)
                dataT_b[i] = data_b[i];
            //将数据写到服务器的保持寄存器中
            rc = modbus_write_registers(ctx, addr, nb, dataT_b);
            if (rc == -1) {
                qDebug("write register fail! --16");
            }
        }
        else if (DevConfigPtr->serialConfig.cmdRegType[cmdNo] == Register_Type_INT32L ||
                DevConfigPtr->serialConfig.cmdRegType[cmdNo] == Register_Type_FLOATL){
            quint16 dataT_b[2*nb];
            ::memset(dataT_b, 0, nb*2);
            for (int i = 0; i < nb; ++i) {
                dataT_b[2*i] = data_b[i];
            }
            rc = modbus_write_registers(ctx, addr, nb*2, dataT_b);
            if (rc == -1) {
                qDebug("write register fail! --16");
            }
        }
        else {
            quint16 dataT_b[2*nb];
            ::memset(dataT_b, 0, nb*2);
            for (int i = 0; i < nb; ++i) {
                dataT_b[2*i+1] = data_b[i];
            }
            rc = modbus_write_registers(ctx, addr, nb*2, dataT_b);
            if (rc == -1) {
                qDebug("write register fail! --16");
            }
        }
    }
    else if (flag == 2) {
        if (DevConfigPtr->serialConfig.cmdRegType[cmdNo] == Register_Type_BIT) {
            quint8 data_bit[nb];

            for (int i = 0; i < nb; ++i) {
                data_bit[i] = data_f[i];
                qDebug("%d,  %f",data_bit[i],data_f[i]);
            }
            //将数据写到服务器的线圈中
            rc = modbus_write_bits(ctx, addr, nb, data_bit);
            if (rc == -1) {
                qDebug("write coil fail!");
            }
        }
        else if (DevConfigPtr->serialConfig.cmdRegType[cmdNo] == Register_Type_INT16 ||
                 DevConfigPtr->serialConfig.cmdRegType[cmdNo] == Register_Type_UINT16 ) {
            quint16 dataT_b[nb];

            for (int i = 0; i < nb; ++i) {
                dataT_b[i] = data_f[i];
                qDebug("%d,  %f",dataT_b[i],data_f[i]);
            }

            //将数据写到服务器的保持寄存器中
            rc = modbus_write_registers(ctx, addr, nb, dataT_b);
            if (rc == -1) {
                qDebug("write register fail! --16");
            }
        }
        else if (DevConfigPtr->serialConfig.cmdRegType[cmdNo] == Register_Type_INT32L ||
                 DevConfigPtr->serialConfig.cmdRegType[cmdNo] == Register_Type_FLOATL ) {
            quint16 dataT_b[nb*2];
            float *fData;

            for (int i = 0; i < nb; ++i) {
                fData = (float*)(&dataT_b[2*i]);
                *fData = data_f[i];
                qDebug("%f,  %f",*fData,data_f[i]);
            }

            //将数据写到服务器的保持寄存器中
            rc = modbus_write_registers(ctx, addr, nb, dataT_b);
            if (rc == -1) {
                qDebug("write register fail! --16");
            }
        }
        else {
            quint16 dataT_b[nb*2],temp;
            float *fData;

            for (int i = 0; i < nb; ++i) {
                fData = (float*)(&dataT_b[2*i]);
                *fData = data_f[i];
                qDebug("%f,  %f",*fData,data_f[i]);
            }

            for (int i = 0; i < nb; ++i) {
                temp = dataT_b[i];
                dataT_b[i] = dataT_b[i+1];
                dataT_b[i+1] = temp;
                i++;
            }
            //将数据写到服务器的保持寄存器中
            rc = modbus_write_registers(ctx, addr, nb*2, dataT_b);
            if (rc == -1) {
                qDebug("write register fail! --16");
            }
        }

    }
    else {

    }
    return rc;
}

/*
 * 功能：
 *      客户端关闭函数，释放资源
 * 参数：
 *
 * 返回值：
 * 	  无
 */
void ThreadCOMModbusClient::closeServer()
{
    if (p_recoverTimer->isActive()) {
        p_recoverTimer->stop();
    }
    if (p_simpleTimer->isActive()) {
        p_simpleTimer->stop();
    }
    if (p_bpsTimer->isActive()) {
        p_bpsTimer->stop();
    }
    while (!m_cmdNoList.empty()) {
        m_cmdNoList.pop_back();
    }
    while (!m_recoverList.empty()) {
        m_recoverList.pop_back();
    }
    modbus_free(p_ctx);
}

void ThreadCOMModbusClient::setOutLogAndInfo(int ID, int rc)
{
    ModbusStatus errNum;

    if (rc >= MODBUS_ENOBASE) {
        switch (rc) {
        case EMBXILFUN:
            //            return "Illegal function";
            errNum = ModbusStatus_ErrFc;
            break;
        case EMBXILADD:
            //            return "Illegal data address";
            errNum = ModbusStatus_ErrAddr;
            break;
        case EMBXILVAL:
            //            return "Illegal data value";
            errNum = ModbusStatus_ErrValue;
            break;
        case EMBXSFAIL:
            //            return "Slave device or server failure";
            errNum = ModbusStatus_ErrDevice;
            break;
        case EMBXACK:
            //            return "Acknowledge";
            errNum =ModbusStatus_Ack ;
            break;
        case EMBXSBUSY:
            //            return "Slave device or server is busy";
            errNum = ModbusStatus_Busy;
            break;
#if 0
        case EMBXNACK:
            //            return "Negative acknowledge";
            errNum = ;
            break;
        case EMBXMEMPAR:
            //            return "Memory parity error";
            errNum = ;
            break;
        case EMBXGPATH:
            //            return "Gateway path unavailable";
            errNum = ;
            break;
        case EMBXGTAR:
            //            return "Target device failed to respond";
            errNum = ;
            break;
        case EMBBADDATA:
            //            return "Invalid data";
            errNum = ;
            break;
        case EMBBADEXC:
            //            return "Invalid exception code";
            errNum = ;
            break;
        case EMBMDATA:
            //            return "Too many data";
            errNum = ;
            break;
        case EMBBADSLAVE:
            //            return "Response not from requested slave";
            errNum = ;
            break;
#endif
        default:
            errNum = ModbusStatus_Invalid;
            break;
        }
    }
    else {
        errNum = (ModbusStatus)rc;
    }

    Modbus_Summary_Data s;

    if (!m_summaryList.isEmpty()) {
//        m_summaryList.value(ID).status = errNum;
        s = m_summaryList.value(ID);
        s.status = errNum;
        m_summaryList.replace(ID, s);
    }

    emit sigModbusCommSummaryChange(ID);

    QDateTime current_date_time = QDateTime::currentDateTime();
    QString current_date = current_date_time.toString("yyyy/MM/dd hh:mm:ss ");
    QString logStr;

    for (int i = 0; i < m_summaryList.size(); ++i) {
        logStr = current_date + QString("M %1 %2 ").arg(errNum).arg(m_summaryList.value(ID).cmdNum);
        if (m_summaryList.value(ID).cmdType == 1)
            logStr += "R";
        else
            logStr += "W";
    }

    Logger::getInstance().log(Logger::MODBUS_LOG, logStr);
}

/*
 * 功能：
 *  功能开启槽函数
 * 参数：
 *
 * 返回值：
 * 	  无
 */
void ThreadCOMModbusClient::slotServerStart()
{
    init();
}

/*
 * 功能：
 *  功能重启槽函数,先关闭，再开启
 * 参数：
 *
 * 返回值：
 * 	  无
 */
void ThreadCOMModbusClient::slotServerRestart(quint32 ID)
{
    if (ID == ID_Serial ||
            ID == ID_Serial_Modbus_Basic) {
        closeServer();
        init();
    }
}

/*
 * 功能：
 *  周期采样槽函数
 * 参数：
 *
 * 返回值：
 * 	  无
 */
void ThreadCOMModbusClient::slotSimple()
{
    int rc;

    //没有命令间隔
    if (DevConfigPtr->serialConfig.commCmdInterval == 0) {
        //从保存命令开始执行
        for (qint8 i = 0; i <= m_cmdNoList.size(); ++i) {
            //若从机状态为false或者正在进行恢复动作的，执行下条命令
            if (!(m_cmdNoList.value(i)->state) || m_cmdNoList.value(i)->recoverState)
                continue;
            //设置从机
            rc = modbus_set_slave(p_ctx, DevConfigPtr->serialConfig.cmdServeNo[m_cmdNoList.value(i)->cmdNo]);
            if (rc < 0 ) {
                qDebug("modbus set salve fail! --COM");
                setOutLogAndInfo(i, ModbusStatus_Unreach);
                continue;
            }
            rc = handleCmd(m_cmdNoList.value(i)->cmdNo);
            if (rc < 0) {
                m_cmdNoList.value(i)->recoverState = true;
                m_recoverList.push_back(m_cmdNoList.value(i));
                setOutLogAndInfo(i, rc);
                rc = 0;
            }
            //更新数据流长度
            m_totalDataSize += rc;
            //如果数据流长度大于波特率限定长度，
            if (m_totalDataSize >= m_limitDataSize) {
                for (int j = i; j < m_cmdNoList.size(); ++j) {
                    setOutLogAndInfo(j, ModbusStatus_Dropout);
                }
                break;
            }
        }
    }
    //有命令间隔
    else {
        //从保存命令号开始到采样周期执行命令的上限
        for (qint8 i = 0; i < m_cmdNumHInterval; ++i) {
            //若从机状态为false或者正在进行恢复动作的，执行下条命令
            if (!(m_cmdNoList.value(i)->state) || m_cmdNoList.value(i)->recoverState)
                continue;
            //设置从机
            rc = modbus_set_slave(p_ctx, DevConfigPtr->serialConfig.cmdServeNo[m_cmdNoList.value(i)->cmdNo]);
            if (rc < 0) {
                qDebug("modbus set salve fail! --COM");
                setOutLogAndInfo(i, ModbusStatus_Unreach);
                continue;
            }
            rc = handleCmd(m_cmdNoList.value(i)->cmdNo);
            if (rc < 0 ) {
                m_cmdNoList.value(i)->recoverState = true;
                m_recoverList.push_back(m_cmdNoList.value(i));
                setOutLogAndInfo(i, rc);
                rc = 0;
            }
            //更新数据流长度
            m_totalDataSize += rc;
            //如果数据流长度大于波特率限定长度，
            if (m_totalDataSize >= m_limitDataSize) {
                for (int j = i; j < m_cmdNumHInterval; ++j) {
                    setOutLogAndInfo(j, ModbusStatus_Dropout);
                }
                break;
            }
            //命令间隔耗时
            usleep(DevConfigPtr->serialConfig.commCmdInterval);
        }

    }
}

/*
 * 功能：
 *      波特率限制槽函数
 * 参数：
 *
 * 返回值：
 * 	  无
 */
void ThreadCOMModbusClient::slotBps()
{
    m_totalDataSize = 0; //数据流长度清零
}


/*
 * 功能：
 *      恢复连接槽函数
 * 参数：
 *
 * 返回值：
 * 	  无
 */
void ThreadCOMModbusClient::slotrecover()
{
    int rc;
    int j;

    //遍历恢复队列
    for (int i = 0; i < m_recoverList.size(); ++i) {
        //若恢复次数超过设定上限，则置从机状态为false
        if (m_recoverList.value(i)->recoverTime >= DevConfigPtr->serialConfig.recoveryCount) {
            m_recoverList.value(i)->state = false;
            for (j = 0; j < m_cmdNoList.size(); ++j) {
                if (m_cmdNoList.value(j)->cmdNo == m_recoverList.value(i)->cmdNo)
                    break;
            }
            setOutLogAndInfo(j, ModbusStatus_Unreach);
            continue;
        }
        m_recoverList.value(i)->recoverTime++;  //恢复次数+1
        rc = handleCmd(m_recoverList.value(i)->cmdNo);  //rc != -1则从机状态正常，否则非正常
        if (rc < 0) {
            m_recoverList.value(i)->recoverState = false;
        }
        else {
            m_recoverList.value(i)->recoverState = true;
            for (j = 0; j < m_cmdNoList.size(); ++j) {
                if (m_cmdNoList.value(j)->cmdNo == m_recoverList.value(i)->cmdNo)
                    break;
            }
            setOutLogAndInfo(j, ModbusStatus_Valid);
        }
        m_totalDataSize += rc;         //更新1S串口数据流长度
    }

    rc = 0;

    //将从机状态为false 或者恢复动作完成的出队列
    while(!m_recoverList.empty()) {
        if (m_recoverList.value(rc)->recoverState || !(m_recoverList.value(rc)->state)) {
            m_recoverList.value(rc)->recoverTime = 0;
            m_cmdNoList.removeAt(rc);
        }
        else {
            rc++;
        }
    }
}

#endif
