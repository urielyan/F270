/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：DevChannelDO.cpp
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
#include "devchanneldo.h"

#include "GlobalData/globalDef.h"
#include "GlobalData/dataGlobalDef_ModbusReg.h"

#include <QSharedMemory>

extern MODBUS_REGISTER_MAP  g_originDataMap;   //全局变量，原始数据对象

DevChannelDO::DevChannelDO(quint32 chanCoder, QObject *parent) :
    Channel(chanCoder, parent)
{
    MODBUS_REGISTER_MAP *pModbusRegMap;
    quint16 chanType, uninNum, moduleIndex, number;

    chanType = CHANNEL_TYPE(m_chanCoder);           //通道类型
    uninNum = CHANNEL_UNIT(m_chanCoder);           //单元号
    moduleIndex = CHANNEL_MODULE(m_chanCoder);  //卡槽号
    number = CHANNEL_NUM(m_chanCoder) - 1;              //通道号

    Q_ASSERT(chanType==Channel_Type_DO && uninNum<=CHANNEL_UNIN_EXT6);

    pModbusRegMap = const_cast<MODBUS_REGISTER_MAP*>(static_cast<MODBUS_REGISTER_MAP const *>(DevShareMem.constData()));
    if (uninNum == CHANNEL_UNIN_MAIN) { //本机DO通道
        Q_ASSERT(moduleIndex<MAIN_UNIT_MODULE_NUM && number<UNIT_CHAN_NUM_PRE);

        m_pData = &pModbusRegMap->holdReg_mainUnit_IO_Data_Int16[moduleIndex][number];
        m_pStatus = &(pModbusRegMap->holdReg_mainUnit_IO_Status_Int16[moduleIndex][number]);
        m_pConfig = &(DevConfigPtr->mainUnitDO[moduleIndex][number]);

        p_curData = &g_originDataMap.holdReg_mainUnit_IO_Data_Int16[moduleIndex][number];

    } else { //扩展DO通道
        Q_ASSERT(uninNum<=EXT_UNIT_NUM && moduleIndex<EXT_UNIT_MODULE_NUM && number<UNIT_CHAN_NUM_PRE);

        m_pData = &pModbusRegMap->holdReg_ExtUnit_IO_Data_Int16[uninNum-1][moduleIndex][number];
        m_pStatus = &(pModbusRegMap->holdReg_ExtUnit_IO_Status_Int16[uninNum-1][moduleIndex][number]);
        m_pConfig = &(DevConfigPtr->extUnitDO[uninNum-1][moduleIndex][number]);

        p_curData = &g_originDataMap.holdReg_ExtUnit_IO_Data_Int16[uninNum-1][moduleIndex][number];
    }
}

