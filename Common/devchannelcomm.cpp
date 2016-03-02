/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：DevChannelComm.cpp
 * 概   要：Comm通道实现类，继承类Channel, 提供Comm通道的函数接口，以获取该通道
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
#include "devchannelcomm.h"

#include "GlobalData/globalDef.h"
#include "GlobalData/dataGlobalDef_ModbusReg.h"

#include <QSharedMemory>

extern MODBUS_REGISTER_MAP  g_originDataMap;   //全局变量，原始数据对象

DevChannelComm::DevChannelComm(quint32 chanCoder, QObject *parent) :
    Channel(chanCoder, parent)
{
    MODBUS_REGISTER_MAP *pModbusRegMap;
    quint16 chanType, uninNum, moduleIndex, number;

    chanType = CHANNEL_TYPE(m_chanCoder);           //通道类型
    uninNum = CHANNEL_UNIT(m_chanCoder);           //单元号
    moduleIndex = CHANNEL_MODULE(m_chanCoder);  //卡槽号
    number = CHANNEL_NUM(m_chanCoder) - 1;              //通道号

    Q_ASSERT(chanType==Channel_Type_Comm && uninNum==0 && moduleIndex==0 && number<MAIN_UNIT_COMM_CHAN_NUM_PRE);

    pModbusRegMap = const_cast<MODBUS_REGISTER_MAP*>(static_cast<MODBUS_REGISTER_MAP const *>(DevShareMem.constData()));
    m_pData = (float *)&pModbusRegMap->holdReg_MainUnit_Comm_Data_Float32[number<<1];
    m_pStatus = &pModbusRegMap->holdReg_MainUnit_Comm_Status_Int16[number];
    m_pConfig = &DevConfigPtr->chanComm[number];

    p_calculateData = (float *)&g_originDataMap.holdReg_MainUnit_Comm_Data_Float32[number<<1];
}
