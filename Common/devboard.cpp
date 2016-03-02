/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：DevBoard.cpp
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
#include "devboard.h"

#include "GlobalData/cfgGlobalDef.h"
#include "Common/devchannelai.h"
#include "Common/devchanneldi.h"
#include "Common/devchanneldo.h"
#include "Common/devchannelmath.h"
#include "Common/devchannelcomm.h"

#include "GlobalData/cfgchannel.h"

#include "Threads/threaddataprocess.h"
#include "Threads/boardinterface.h"

DevBoard::DevBoard(quint8 unit, quint8 module, quint16 status, QObject *parent) :
    QObject(parent),
    m_status(status),
    m_unit(unit),
    m_module(module)
{
    quint8 type  = STATUS_BOARD_TYPE(status);
    quint8 ch0Num = STATUS_BOARD_CHAN0_NUM(status);
    quint8 ch1Num = STATUS_BOARD_CHAN1_NUM(status);
    quint32 chanCoder;
    Channel *pChannel;

    ::memset(m_config, 0x00, sizeof(m_config));

    //板卡数据寄存器地址、从机地址设置
    if (unit == CHANNEL_UNIN_MAIN) {
        m_regAddr = 0x00;
        m_slave = module+1;
    } else {
        int boardDataSize = sizeof(SUB_BOARD_DATA)>>1;
        m_regAddr = (unit-1) * (boardDataSize * EXT_UNIT_MODULE_NUM) + module * boardDataSize;
        m_slave = CfgChannel::instance()->getExternBoardSlaveAddr();
    }

    //AI/DI/DO/DIO/COMM/MATH板卡通道添加
    switch (type) {
    case STATUS_BOARD_TYPE_AI:
        for(int i=1; i<=ch0Num; ++i) {
            chanCoder = CHANNEL_AI_CODER(unit, module, i);
            pChannel = new DevChannelAI(chanCoder, this);
            if (pChannel) {
                m_channelList.append(pChannel);
                CfgChannel::instance()->registerChannelObj(pChannel, Channel_Type_AI);
            }
        }

        //配置获取
        if (CHANNEL_UNIN_MAIN == unit) {
            ::memcpy(m_config, DevConfigPtr->sysStatus.mainUnit_Recognition[module].boardReg_config,
                     sizeof(quint16) * BOARD_IO_CONFIG_NUM);
        } else {
            ::memcpy(m_config, DevConfigPtr->sysStatus.extUnit_Recognition[unit-1][module].boardReg_config,
                    sizeof(quint16) * BOARD_IO_CONFIG_NUM);
        }

        m_AD_mode =  (Board_ADTime)m_config[BOARD_IO_AI_AD_REG];
        m_burnOutUpperLimit = (float)m_config[BOARD_IO_AI_BURNUP_REG]/10.0;
        m_burnOutDownLimit = (float)m_config[BOARD_IO_AI_BURNDOWN_REG]/10.0;
        break;
    case STATUS_BOARD_TYPE_DI:
        for(int i=1; i<=ch0Num; ++i) {
            chanCoder = CHANNEL_DI_CODER(unit, module, i);
            pChannel = new DevChannelDI(chanCoder, this);
            if (pChannel) {
                m_channelList.append(pChannel);
                CfgChannel::instance()->registerChannelObj(pChannel, Channel_Type_DI);
            }
        }
        //配置获取
        if (CHANNEL_UNIN_MAIN == unit) {
            ::memcpy(m_config, DevConfigPtr->sysStatus.mainUnit_Recognition[module].boardReg_config,
                     sizeof(quint16) * BOARD_IO_CONFIG_NUM);
        } else {
            ::memcpy(m_config, DevConfigPtr->sysStatus.extUnit_Recognition[unit-1][module].boardReg_config,
                    sizeof(quint16) * BOARD_IO_CONFIG_NUM);
        }
        break;
    case STATUS_BOARD_TYPE_DO:
        for(int i=1; i<=ch0Num; ++i) {
            chanCoder = CHANNEL_DO_CODER(unit, module, i);
            pChannel = new DevChannelDO(chanCoder, this);
            if (pChannel) {
                m_channelList.append(pChannel);
                CfgChannel::instance()->registerChannelObj(pChannel, Channel_Type_DO);
            }
        }
        //配置获取
        if (CHANNEL_UNIN_MAIN == unit) {
            ::memcpy(m_config, DevConfigPtr->sysStatus.mainUnit_Recognition[module].boardReg_config,
                     sizeof(quint16) * BOARD_IO_CONFIG_NUM);
        } else {
            ::memcpy(m_config, DevConfigPtr->sysStatus.extUnit_Recognition[unit-1][module].boardReg_config,
                    sizeof(quint16) * BOARD_IO_CONFIG_NUM);
        }
        break;
    case STATUS_BOARD_TYPE_DIO:
        for(int i=1; i<=ch0Num; ++i) {
            chanCoder = CHANNEL_DI_CODER(unit, module, i);
            pChannel = new DevChannelDI(chanCoder, this);
            if (pChannel) {
                m_channelList.append(pChannel);
                CfgChannel::instance()->registerChannelObj(pChannel, Channel_Type_DI);
            }
        }
        for(int i=1; i<=ch1Num; ++i) {
            chanCoder = CHANNEL_DO_CODER(unit, module, i);
            pChannel = new DevChannelDO(chanCoder, this);
            if (pChannel) {
                m_channelList.append(pChannel);
                CfgChannel::instance()->registerChannelObj(pChannel, Channel_Type_DO);
            }
        }
        //配置获取
        if (CHANNEL_UNIN_MAIN == unit) {
            ::memcpy(m_config, DevConfigPtr->sysStatus.mainUnit_Recognition[module].boardReg_config,
                     sizeof(quint16) * BOARD_IO_CONFIG_NUM);
        } else {
            ::memcpy(m_config, DevConfigPtr->sysStatus.extUnit_Recognition[unit-1][module].boardReg_config,
                    sizeof(quint16) * BOARD_IO_CONFIG_NUM);
        }
        break;
    case STATUS_BOARD_TYPE_COMM:
        for (int i=1; i<=MAIN_UNIT_COMM_CHAN_NUM_PRE; ++i) {
            chanCoder = CHANNEL_COMM_CODER(unit, module, i);
            pChannel = new DevChannelComm(chanCoder, this);
            if (pChannel) {
                m_channelList.append(pChannel);
                CfgChannel::instance()->registerChannelObj(pChannel, Channel_Type_Comm);
            }
        }
        break;
    case STATUS_BOARD_TYPE_MATH:
        for (int i=1; i<=MAIN_UNIT_MATH_CHAN_NUM_PRE; ++i) {
            chanCoder = CHANNEL_MATH_CODER(unit, module, i);
            pChannel = new DevChannelMath(chanCoder, this);
            if (pChannel) {
                m_channelList.append(pChannel);
                CfgChannel::instance()->registerChannelObj(pChannel, Channel_Type_Math);
            }
        }
        break;
    default:
        break;
    }
}

DevBoard::~DevBoard()
{
    foreach (Channel * pChannel, m_channelList) {
        CfgChannel::instance()->unRegisterChannelObj(pChannel, pChannel->getChannelType());
    }
}

void DevBoard::setDoOutput(quint32 chanNum, bool value)
{
    if (STATUS_BOARD_TYPE(m_status) != STATUS_BOARD_TYPE_DO &&
        STATUS_BOARD_TYPE(m_status) != STATUS_BOARD_TYPE_DIO)
        return;

    if (value) {
        m_config[BOARD_IO_DO_SET_REG] |= 0x01<<chanNum;
    } else {
        m_config[BOARD_IO_DO_CLR_REG] |= 0x01<<chanNum;
    }

    this->slotConfigChanged();
}

/*板卡通道配置变更处理槽函数*/
void DevBoard::slotConfigChanged()
{
    int regAddr, slave, channelNum0, channelNum1;
    AI_CONFIG *pAIConfig;
    DI_CONFIG *pDIConfig;
    DO_CONFIG *pDOConfig;
    BoardInterface * boardInterface;
    bool extFlag = false;

    //寄存器地址、子板modbus地址
    if (m_unit == CHANNEL_UNIN_MAIN) {
        regAddr = BOARD_IO_CONFIG_START;
    } else {
        int boardRegMapSize = sizeof(BOARD_IO_CONFIG)>>1;
        regAddr = (sizeof(BOARD_EXT_CONFIG)>>1) + \
                                (m_unit-1) * (boardRegMapSize * EXT_UNIT_MODULE_NUM) + \
                                boardRegMapSize * m_module;
        extFlag = true;
    }
    slave = CfgConfig::instance()->getBoardSlaveAddr(m_unit, m_module);
    channelNum0 = STATUS_BOARD_CHAN0_NUM(m_status);
    channelNum1 = STATUS_BOARD_CHAN1_NUM(m_status);

    //配置包ID设置
    m_config[BOARD_IO_PACKETID_REG] = qrand();

    //各IO通道配置数据赋值
    switch (STATUS_BOARD_TYPE(m_status)) {
    case STATUS_BOARD_TYPE_AI:
        for (int i=0; i<channelNum0; ++i) {
            pAIConfig = (AI_CONFIG *)m_channelList.at(i)->getChannelConfig();
            m_config[i] = AI_CHANNEL_CONFIG(pAIConfig->rangeConfig.rangeType, \
                                                                    pAIConfig->rangeConfig.config.rangeData, \
                                                                    0);
        }
        m_config[BOARD_IO_AI_AD_REG] = m_AD_mode;
        m_config[BOARD_IO_AI_BURNUP_REG] = m_burnOutUpperLimit * 10;
        m_config[BOARD_IO_AI_BURNDOWN_REG] = m_burnOutDownLimit * 10;
        break;
    case STATUS_BOARD_TYPE_DI:
        for (int i=0; i<channelNum0; ++i) {
            pDIConfig = (DI_CONFIG *)m_channelList.at(i)->getChannelConfig();
            m_config[i] = DI_CHANNEL_CONFIG(pDIConfig->rangeConfig.type);
        }
        break;
    case STATUS_BOARD_TYPE_DO:
        for (int i=0; i<channelNum0; ++i) {
            pDOConfig = (DO_CONFIG *)m_channelList.at(i)->getChannelConfig();
            m_config[i] = DO_CHANNEL_CONFIG(pDOConfig->activeConfig.actionEnergize);
        }
        break;
    case STATUS_BOARD_TYPE_DIO:
        for (int i=0; i<channelNum0; ++i) {
            pDIConfig = (DI_CONFIG *)m_channelList.at(i)->getChannelConfig();
            m_config[i] = DI_CHANNEL_CONFIG(pDIConfig->rangeConfig.type);
        }
        for (int i=0; i<channelNum1; ++i) {
            pDOConfig = (DO_CONFIG *)m_channelList.at(i)->getChannelConfig();
            m_config[8+i] = DO_CHANNEL_CONFIG(pDOConfig->activeConfig.actionEnergize);
        }
        break;
    default:
        return;
    }

    boardInterface = ThreadDataProcess::instance()->getBoardInterface();
    if (boardInterface) {
        boardInterface->tcflushConfigBus();
        boardInterface->setHoldRegister(regAddr, BOARD_IO_CONFIG_NUM, (uint16_t*)m_config, slave);

        //配置包ID更新
        if (CHANNEL_UNIN_MAIN == m_unit) {
            DevConfigPtr->sysStatus.configPacketID.mainUnit_PrePackID[m_module] = m_config[BOARD_IO_PACKETID_REG];
        } else {
            DevConfigPtr->sysStatus.configPacketID.extUnit_PrePackID[m_unit-1][m_module] = m_config[BOARD_IO_PACKETID_REG];
        }

        //扩展单元，修改IO板卡配置标记
        if(extFlag) {
            uint16_t data = 0xAA;
            regAddr = BOARD_IO_CONFIG_START + (m_unit-1)*EXT_UNIT_MODULE_NUM + m_module;
            boardInterface->setHoldRegister(regAddr, 1, &data, slave);
        }
        //DO、DIO类型清除置0、1标记
        if (STATUS_BOARD_TYPE(m_status) == STATUS_BOARD_TYPE_DO ||
            STATUS_BOARD_TYPE(m_status) == STATUS_BOARD_TYPE_DIO) {
            m_config[BOARD_IO_DO_SET_REG] = m_config[BOARD_IO_DO_CLR_REG] = 0;
        }

    } else {
        qDebug("BoardInterface Invalid\n");
    }
}
