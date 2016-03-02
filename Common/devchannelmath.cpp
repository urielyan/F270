/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：DevChannelMath.cpp
 * 概   要：Math通道实现类，继承类Channel, 提供Math通道的函数接口，以获取该通道
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
#include "devchannelmath.h"

#include "GlobalData/globalDef.h"
#include "GlobalData/dataGlobalDef_ModbusReg.h"

#include <QSharedMemory>

extern MODBUS_REGISTER_MAP  g_originDataMap;   //全局变量，原始数据对象

DevChannelMath::DevChannelMath(quint32 chanCoder, QObject *parent) :
    Channel(chanCoder, parent)
{
    MODBUS_REGISTER_MAP *pModbusRegMap;
    quint16 chanType, uninNum, moduleIndex, number;

    chanType = CHANNEL_TYPE(m_chanCoder);           //通道类型
    uninNum = CHANNEL_UNIT(m_chanCoder);           //单元号
    moduleIndex = CHANNEL_MODULE(m_chanCoder);  //卡槽号
    number = CHANNEL_NUM(m_chanCoder) - 1;              //通道号

    Q_ASSERT(chanType==Channel_Type_Math && uninNum==0 && moduleIndex==0 && number<MAIN_UNIT_MATH_CHAN_NUM_PRE);

    pModbusRegMap = const_cast<MODBUS_REGISTER_MAP*>(static_cast<MODBUS_REGISTER_MAP const *>(DevShareMem.constData()));
    m_pData = (float *)&pModbusRegMap->inputReg_MainUnit_Math_Data_Float32[number<<1];
    m_pStatus = &pModbusRegMap->inputReg_MainUnit_Math_Status_Int16[number];
    m_pConfig = &DevConfigPtr->chanMath[number];

    p_calculateData = (float *)&g_originDataMap.inputReg_MainUnit_Math_Data_Float32[number<<1];
    p_chanStatus = &g_originDataMap.inputReg_MainUnit_Math_Status_Int16[number];

    //表达式初始化
    if (m_pConfig->expressConfig.expSwtich) {
        QByteArray expArray((const char *)m_pConfig->expressConfig.expExpression);
        if (expArray.isEmpty()) {
            p_expAnalysis = NULL;
        } else {
            p_expAnalysis = new MathExpAnalysis(number+1, expArray);
            if (p_expAnalysis) {
                if (!p_expAnalysis->isExpValid()) {
                    delete p_expAnalysis;
                    p_expAnalysis = NULL;
                }
            } else {
                qDebug("Error: Mem alloc Failed[%s]\n", __func__);
            }
        }
    } else {
        p_expAnalysis = NULL;
    }

    m_sampleCount = 0;
    m_math_DelayUpper = 0;
    m_math_DelayDowner = 0;
}

/*运算通道TLOG计算*/
void DevChannelMath::chanTlogCalculate()
{
    if ((p_expAnalysis != NULL) && (Tlog_type_none != p_expAnalysis->m_tlogCalculate.m_tlog_type)) {
        qreal chanData = CfgChannel::instance()->getChannelData(this->getChannelCoder(), \
                                                                                p_expAnalysis->m_tlogCalculate.m_tlog_channel);

        p_expAnalysis->m_tlogCalculate.calculateTlogValue(p_expAnalysis->m_index, chanData);
    }
}

/*运算通道运算*/
void DevChannelMath::chanCalculate()
{
    if (p_expAnalysis) {
        //根据MATH通道配置计算TLOG最终值m_tlog_value
        quint32 measureInterval_ms = DevConfigPtr->measureInterval_ms;
        float mathData = p_expAnalysis->getMathExpValue();

        //长时间移动平均计算
        if (m_pConfig->expressConfig.expRollingAvgSwtich) {
            qreal movingAve = 0;
            quint32 count = m_pConfig->expressConfig.expRollingAvgNumber;    //采样个数
            quint32 interval_ms = m_pConfig->expressConfig.expRollingAvgInterval *1000;
            interval_ms = interval_ms < measureInterval_ms ? measureInterval_ms : interval_ms;
            quint32 sampleInterval = interval_ms/measureInterval_ms;        //采样间隔---隔多少个测量周期采样
            if (interval_ms%measureInterval_ms) {
                sampleInterval += 1;
                interval_ms = sampleInterval * measureInterval_ms;
            }

            if (++m_sampleCount == sampleInterval) {  //采样间隔满足，则添加采样值
                m_sampleCount = 0;
                if (m_sample_moving.count()<(int)count) {
                    m_sample_moving.enqueue(mathData);
                } else {
                    m_sample_moving.dequeue();
                    m_sample_moving.enqueue(mathData);
                }
            }


            for (int i=0; i<m_sample_moving.count(); ++i) {
                movingAve += m_sample_moving.at(i);
            }
            mathData = movingAve/m_sample_moving.count();
        }

        //Math通道赋值
        *p_calculateData = mathData;

        //通道状态判定
        //Math通道无实际量程上、下限，配置中的上下限仅用于显示，故状态判定略
        qint16 preStatus = *p_chanStatus;                                  //通道之前状态值
        *p_chanStatus = 0;

        //通道报警判定
        quint32 curTimems, holdTimems;
        quint32 *pDelayUpper, *pDelayDowner;
        pDelayUpper = &m_math_DelayUpper;
        pDelayDowner = &m_math_DelayDowner;
        float alarmValue;
        for (int i=0; i<4; ++i) {
            if (m_pConfig->alarmConfig.alarmOnOff[i]) {
                alarmValue = m_pConfig->alarmConfig.alarmValue[i]/::pow10(m_pConfig->expressConfig.expPos);
                switch (m_pConfig->alarmConfig.alarmType[i]) {
                case Alarm_Type_H:
                    if (mathData > alarmValue) {
                        *p_chanStatus |= 1<<i;
                    } else if (preStatus & (1<<i) && \
                    (mathData > (alarmValue - m_pConfig->alarmConfig.alarmHysteresis[i]))) {  //滞后
                        *p_chanStatus |= 1<<i;
                    }
                    break;
                case Alarm_Type_L:
                    if (mathData < alarmValue) {
                        *p_chanStatus |= 1<<i;
                    } else if (preStatus & (1<<i) && \
                    (mathData < (alarmValue + m_pConfig->alarmConfig.alarmHysteresis[i]))) {  //滞后
                        *p_chanStatus |= 1<<i;
                    }
                    break;
                case Alarm_Type_T:
                    if (mathData > alarmValue) {
                        (*pDelayUpper)++;
                        curTimems = (*pDelayUpper) * measureInterval_ms;
                        holdTimems = (m_pConfig->alarmConfig.alarmDelay[2]) * 3600000;
                        holdTimems += (m_pConfig->alarmConfig.alarmDelay[1]) * 60000;
                        holdTimems += (m_pConfig->alarmConfig.alarmDelay[0]) * 1000;
                        if (curTimems > holdTimems)
                            *p_chanStatus |= 1<<i;
                    } else {
                        *pDelayUpper = 0;
                    }
                    break;
                case Alarm_Type_t:
                    if (mathData < alarmValue) {
                        (*pDelayDowner)++;
                        curTimems = (*pDelayUpper) * measureInterval_ms;
                        holdTimems = (m_pConfig->alarmConfig.alarmDelay[2]) * 3600000;
                        holdTimems += (m_pConfig->alarmConfig.alarmDelay[1]) * 60000;
                        holdTimems += (m_pConfig->alarmConfig.alarmDelay[0]) * 1000;
                        if (curTimems > holdTimems)
                            *p_chanStatus |= 1<<i;
                    } else {
                        *pDelayDowner = 0;
                    }
                    break;
                default:
                    break;
                }
            }
        }
    } else {
        *p_calculateData = 0;
        //通道未使用，设置状态并返回
        if (0 == m_pConfig->expressConfig.expSwtich) {
            *p_chanStatus = DATA_ERROR_OFF<<DATA_STATUS_ERROR_BITSOFF;
            return;
        }
    }
}

/*运算式配置变更*/
void DevChannelMath::slotChanConfigChanged()
{
    m_sampleCount = 0;
    m_sample_moving.clear();
    m_math_DelayUpper = 0;
    m_math_DelayDowner = 0;

    if (p_expAnalysis) {
        delete p_expAnalysis;
        p_expAnalysis = NULL;
    }

    //表达式初始化
    if (m_pConfig->expressConfig.expSwtich) {
        QByteArray expArray((const char *)m_pConfig->expressConfig.expExpression);
        if (expArray.isEmpty()) {
            p_expAnalysis = NULL;
        } else {
            p_expAnalysis = new MathExpAnalysis(CHANNEL_NUM(m_chanCoder), expArray);
            if (p_expAnalysis) {
                if (!p_expAnalysis->isExpValid()) {
                    delete p_expAnalysis;
                    p_expAnalysis = NULL;
                }
            } else {
                qDebug("Error: Mem alloc Failed[%s]\n", __func__);
            }
        }
    } else {
        p_expAnalysis = NULL;
    }
}
