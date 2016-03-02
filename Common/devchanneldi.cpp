/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：DevChannelDI.cpp
 * 概   要：DI通道实现类，继承类Channel, 提供DI通道的函数接口，以获取该通道的相关
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
#include "devchanneldi.h"

#include "GlobalData/globalDef.h"
#include "GlobalData/dataGlobalDef_ModbusReg.h"

#include "GlobalData/cfgchannel.h"

#include <QSharedMemory>

extern MODBUS_REGISTER_MAP  g_originDataMap;   //全局变量，原始数据对象

DevChannelDI::DevChannelDI(quint32 chanCoder, QObject *parent) :
    Channel(chanCoder, parent)
{
    MODBUS_REGISTER_MAP *pModbusRegMap;
    quint16 chanType, uninNum, moduleIndex, number;

    chanType = CHANNEL_TYPE(m_chanCoder);           //通道类型
    uninNum = CHANNEL_UNIT(m_chanCoder);           //单元号
    moduleIndex = CHANNEL_MODULE(m_chanCoder);  //卡槽号
    number = CHANNEL_NUM(m_chanCoder) - 1;              //通道号

    Q_ASSERT(chanType==Channel_Type_DI && uninNum<=CHANNEL_UNIN_EXT6);

    pModbusRegMap = const_cast<MODBUS_REGISTER_MAP*>(static_cast<MODBUS_REGISTER_MAP const *>(DevShareMem.constData()));
    if (uninNum == CHANNEL_UNIN_MAIN) { //本机DI通道
        Q_ASSERT(moduleIndex<MAIN_UNIT_MODULE_NUM && number<UNIT_CHAN_NUM_PRE);

        m_pData = (float *)&pModbusRegMap->inputReg_MainUnit_IO_Data_Float32[moduleIndex][number<<1];
        m_pStatus = &(pModbusRegMap->inputReg_MainUnit_IO_Status_Int16[moduleIndex][number]);
        m_pConfig = &(DevConfigPtr->mainUnitDI[moduleIndex][number]);

        /*运算相关*/
        p_inputCount = (qint32 *)&g_originDataMap.inputReg_MainUnit_IO_Data_Int32[moduleIndex][number<<1];
        p_inputData = &g_originDataMap.inputReg_MainUnit_IO_Data_Int16[moduleIndex][number<<1];
        p_chanStatus = &g_originDataMap.inputReg_MainUnit_IO_Status_Int16[moduleIndex][number];
        p_calculateData =  (float*)&g_originDataMap.inputReg_MainUnit_IO_Data_Float32[moduleIndex][number<<1];
        if (number < UNIT_CHAN_NUM_LINEAR) {
            p_calculateDataLinear = (float*)&g_originDataMap.inputReg_MainUnit_IO_Data_Float32_Linear[moduleIndex][number<<1];
            p_chanStatusLinear = &g_originDataMap.inputReg_MainUnit_IO_Status_Int16_Linear[moduleIndex][number];
        } else {
            p_calculateDataLinear = NULL;
            p_chanStatusLinear = NULL;
        }

    } else { //扩展DI通道
        Q_ASSERT(uninNum<=EXT_UNIT_NUM && moduleIndex<EXT_UNIT_MODULE_NUM && number<UNIT_CHAN_NUM_PRE);

        m_pData = (float*) &pModbusRegMap->inputReg_ExtUnit_IO_Data_Float32[uninNum-1][moduleIndex][number<<1];
        m_pStatus = &(pModbusRegMap->inputReg_ExtUnit_IO_Status_Int16[uninNum-1][moduleIndex][number]);
        m_pConfig = &(DevConfigPtr->extUnitDI[uninNum-1][moduleIndex][number]);

        /*运算相关*/
        p_inputCount = (qint32 *)&g_originDataMap.inputReg_ExtUnit_IO_Data_Int32[uninNum-1][moduleIndex][number<<1];
        p_inputData = &g_originDataMap.inputReg_ExtUnit_IO_Data_Int16[uninNum-1][moduleIndex][number<<1];
        p_chanStatus = &g_originDataMap.inputReg_ExtUnit_IO_Status_Int16[uninNum-1][moduleIndex][number];
        p_calculateData =  (float*)&g_originDataMap.inputReg_ExtUnit_IO_Data_Float32[uninNum-1][moduleIndex][number<<1];
        p_calculateDataLinear = NULL;
        p_chanStatusLinear = NULL;
    }

    m_DelayUpperCounter = 0;
    m_DelayDownerCounter = 0;
}

void DevChannelDI::chanCalculate()
{

    qint32 dataDI;

    //通道数据无效，直接返回
    if (IS_DATA_INVALID(*p_chanStatus)) {
        if(p_chanStatusLinear !=NULL) *p_chanStatusLinear = *p_chanStatus;
        return;
    }

    switch (m_pConfig->rangeConfig.type){
    case DI_Range_Type_Disabled://通道未使用，设置状态并返回
        *p_chanStatus = DATA_ERROR_OFF<<DATA_STATUS_ERROR_BITSOFF;
        if(p_chanStatusLinear !=NULL) *p_chanStatusLinear = *p_chanStatus;
        return;
    case DI_Range_Type_DI: //DI类型，进行Delta或Linear运算
        dataDI = *p_inputData;
        if (m_pConfig->rangeConfig.calculateType == DI_Range_Calculate_Delta){
            //通道计算
            *p_calculateData = dataDI - CfgChannel::instance()->getChannelData(this->getChannelCoder(), m_pConfig->rangeConfig.basicChannel);

            //溢出判定
            if((qint32)*p_calculateData>1) {
                *p_chanStatus = DATA_ERROR_OVER_POSITIVE<<DATA_STATUS_ERROR_BITSOFF;
                if(p_chanStatusLinear !=NULL) *p_chanStatusLinear = *p_chanStatus;
            } else if((qint32)*p_calculateData<-1) {
                *p_chanStatus = DATA_ERROR_OVER_NEGATIVE<<DATA_STATUS_ERROR_BITSOFF;
                if(p_chanStatusLinear !=NULL) *p_chanStatusLinear = *p_chanStatus;
            }
        } else if (m_pConfig->rangeConfig.calculateType == DI_Range_Calculate_Linear) {
            //通道计算
            if (dataDI == 0) {
                *p_calculateData = m_pConfig->rangeConfig.scale.scaleMin/::pow10(m_pConfig->rangeConfig.scale.scalePosition);
            } else {
                *p_calculateData = m_pConfig->rangeConfig.scale.scaleMax/::pow10(m_pConfig->rangeConfig.scale.scalePosition);
            }
            //溢出判定
            if (dataDI>1) {
                *p_chanStatus = DATA_ERROR_OVER_POSITIVE<<DATA_STATUS_ERROR_BITSOFF;
                if(p_chanStatusLinear !=NULL) *p_chanStatusLinear = *p_chanStatus;
            } else if (dataDI<0) {
                *p_chanStatus = DATA_ERROR_OVER_NEGATIVE<<DATA_STATUS_ERROR_BITSOFF;
                if(p_chanStatusLinear !=NULL) *p_chanStatusLinear = *p_chanStatus;
            }
        } else {
            *p_calculateData = dataDI;
            //溢出判定
            if (dataDI>1) {
                *p_chanStatus = DATA_ERROR_OVER_POSITIVE<<DATA_STATUS_ERROR_BITSOFF;
                if(p_chanStatusLinear !=NULL) *p_chanStatusLinear = *p_chanStatus;
            } else if (dataDI<0) {
                *p_chanStatus = DATA_ERROR_OVER_NEGATIVE<<DATA_STATUS_ERROR_BITSOFF;
                if(p_chanStatusLinear !=NULL) *p_chanStatusLinear = *p_chanStatus;
            }
        }
        break;
    case DI_Range_Type_Pluse://脉冲类型，无运算功能
        dataDI = *p_inputCount;
        *p_calculateData = dataDI;

        //溢出判定 0~999999
        if (dataDI>999999) {
            *p_chanStatus = DATA_ERROR_OVER_POSITIVE<<DATA_STATUS_ERROR_BITSOFF;
            if(p_chanStatusLinear !=NULL) *p_chanStatusLinear = *p_chanStatus;
        } else if (dataDI<0) {
            *p_chanStatus = DATA_ERROR_OVER_NEGATIVE<<DATA_STATUS_ERROR_BITSOFF;
            if(p_chanStatusLinear !=NULL) *p_chanStatusLinear = *p_chanStatus;
        }
        break;
    }

    /**
      * 输入值（H, L, T, t, R, r）==>测量值（h, l）
     **/
    //通道报警: 变化率/延迟报警相关数据处理
    quint8 rateIncCount = DevConfigPtr->systemConfig.rateIncNum;
    quint8 rateDecCount = DevConfigPtr->systemConfig.rateDecNum;
    qreal testData;
    QQueue<qreal> *pQueueInc, *pQueueDec;
    quint32 *pDelayUpper, *pDelayDowner;

    //R,r使用测量值
    testData = *p_calculateData; //R,r使用测量值
    if (m_RateIncFIFO.count()<(rateIncCount+1)){
        m_RateIncFIFO.enqueue(testData);
    } else {
        m_RateIncFIFO.dequeue();
        m_RateIncFIFO.enqueue(testData);
    }
    if (m_RateDecFIFO.count()<(rateDecCount+1)){
        m_RateDecFIFO.enqueue(testData);
    } else {
        m_RateDecFIFO.dequeue();
        m_RateDecFIFO.enqueue(testData);
    }
    pQueueInc = &m_RateIncFIFO;
    pQueueDec = &m_RateDecFIFO;
    pDelayUpper = &m_DelayUpperCounter;
    pDelayDowner  = &m_DelayDownerCounter;

    //通道报警: 状态判定
    quint32 curTimems, holdTimems;
    for (int i=0; i<4; ++i) {
        if (m_pConfig->alarmConfig.alarmOnOff[i]){
            switch(m_pConfig->alarmConfig.alarmType[i]){
            case Alarm_Type_H:       //上限报警(输入值)
                if (dataDI >= m_pConfig->alarmConfig.alarmValue[i])
                    *p_chanStatus |= 1<<i;
                break;
            case Alarm_Type_L:       //下限报警(输入值)
                if (dataDI <= m_pConfig->alarmConfig.alarmValue[i])
                    *p_chanStatus |= 1<<i;
                break;
            case Alarm_Type_R:       //变化率上升限报警(测量值)
                if (pQueueInc->count() == (rateIncCount+1)){
                    if ((pQueueInc->at(rateIncCount) - pQueueInc->at(0)) >= m_pConfig->alarmConfig.alarmValue[i])
                        *p_chanStatus |= 1<<i;
                }
                break;
            case Alarm_Type_r:        //变化率下降限报警(测量值)
                if (pQueueDec->count() == (rateDecCount+1)){
                    if ((pQueueInc->at(0) - pQueueInc->at(rateIncCount)) >= m_pConfig->alarmConfig.alarmValue[i])
                        *p_chanStatus |= 1<<i;
                }
                break;
            case Alarm_Type_T:       //延迟上限---上限持续时间报警(输入值)
                if (dataDI > m_pConfig->alarmConfig.alarmValue[i]){
                    (*pDelayUpper)++;
                    curTimems = (*pDelayUpper) * DevConfigPtr->measureInterval_ms;
                    holdTimems = (m_pConfig->alarmConfig.alarmDelay[2]) * 3600000;
                    holdTimems += (m_pConfig->alarmConfig.alarmDelay[1]) * 60000;
                    holdTimems += (m_pConfig->alarmConfig.alarmDelay[0]) * 1000;
                    if (curTimems > holdTimems)
                        *p_chanStatus |= 1<<i;
                } else {
                    *pDelayUpper = 0;
                }
                break;
            case Alarm_Type_t:       //延迟下限---下限持续时间报警(输入值)
                if (dataDI < m_pConfig->alarmConfig.alarmValue[i]){
                    (*pDelayDowner)++;
                    curTimems = (*pDelayDowner) * DevConfigPtr->measureInterval_ms;
                    holdTimems = (m_pConfig->alarmConfig.alarmDelay[2]) * 3600000;
                    holdTimems += (m_pConfig->alarmConfig.alarmDelay[1]) * 60000;
                    holdTimems += (m_pConfig->alarmConfig.alarmDelay[0]) * 1000;
                    if (curTimems > holdTimems)
                        *p_chanStatus |= 1<<i;
                } else {
                    *pDelayDowner = 0;
                }
                break;
            case Alarm_Type_h:      //Delta计算适用，差值上限报警(测量值)
                if(m_pConfig->rangeConfig.calculateType == DI_Range_Calculate_Delta && \
                   testData >= m_pConfig->alarmConfig.alarmValue[i])
                    *p_chanStatus |= 1<<i;
                break;
            case Alarm_Type_l:       //Delta计算适用，差值下限报警(测量值)
                if(m_pConfig->rangeConfig.calculateType == DI_Range_Calculate_Delta && \
                   testData <= m_pConfig->alarmConfig.alarmValue[i])
                    *p_chanStatus |= 1<<i;
                break;
            default:
                break;
            }
        }
    }

    //本机连续区域赋值
    if(p_calculateDataLinear != NULL) *p_calculateDataLinear = *p_calculateData;
    if(p_chanStatusLinear !=NULL) *p_chanStatusLinear = *p_chanStatus;
}

/*DI通道配置变更槽函数*/
void DevChannelDI::slotChanConfigChanged()
{
    m_RateIncFIFO.clear();
    m_RateDecFIFO.clear();

    m_DelayUpperCounter = 0;
    m_DelayDownerCounter = 0;
}
