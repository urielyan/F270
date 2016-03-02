/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：DevChannelAI.cpp
 * 概   要：AI通道实现类，继承类Channel, 提供AI通道的函数接口，以获取该通道的相关
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
#include "devchannelai.h"
#include "devboard.h"

#include "GlobalData/globalDef.h"
#include "GlobalData/dataGlobalDef_ModbusReg.h"

#include "Common/dataqueryrtd.h"
#include "Common/dataquerytc.h"
#include "GlobalData/cfgchannel.h"

#include <QSharedMemory>

extern MODBUS_REGISTER_MAP  g_originDataMap;   //全局变量，原始数据对象

DevChannelAI::DevChannelAI(quint32 chanCoder, QObject *parent) :
    Channel(chanCoder, parent)
{
    MODBUS_REGISTER_MAP *pModbusRegMap;
    quint16 chanType, uninNum, moduleIndex, number;

    chanType = CHANNEL_TYPE(m_chanCoder);           //通道类型
    uninNum = CHANNEL_UNIT(m_chanCoder);           //单元号
    moduleIndex = CHANNEL_MODULE(m_chanCoder);  //卡槽号
    number = CHANNEL_NUM(m_chanCoder) - 1;              //通道号

    Q_ASSERT(chanType==Channel_Type_AI && uninNum<=CHANNEL_UNIN_EXT6);

    pModbusRegMap = const_cast<MODBUS_REGISTER_MAP*>(static_cast<MODBUS_REGISTER_MAP const *>(DevShareMem.constData()));
    if (uninNum == CHANNEL_UNIN_MAIN) { //本机AI通道
        Q_ASSERT(moduleIndex<MAIN_UNIT_MODULE_NUM && number<UNIT_CHAN_NUM_PRE);

        m_pData = (float *)&pModbusRegMap->inputReg_MainUnit_IO_Data_Float32[moduleIndex][number<<1];
        m_pStatus = &(pModbusRegMap->inputReg_MainUnit_IO_Status_Int16[moduleIndex][number]);
        m_pConfig = &(DevConfigPtr->mainUnitAI[moduleIndex][number]);

        /*运算相关*/
        p_inputData = (float*)&g_originDataMap.inputReg_MainUnit_IO_Data_Int32[moduleIndex][number<<1];
        p_calculateData =  (float*)&g_originDataMap.inputReg_MainUnit_IO_Data_Float32[moduleIndex][number<<1];
        p_chanStatus = &g_originDataMap.inputReg_MainUnit_IO_Status_Int16[moduleIndex][number];
        if (number < UNIT_CHAN_NUM_LINEAR) {
            p_calculateDataLinear = (float*)&g_originDataMap.inputReg_MainUnit_IO_Data_Float32_Linear[moduleIndex][number<<1];
            p_chanStatusLinear = &g_originDataMap.inputReg_MainUnit_IO_Status_Int16_Linear[moduleIndex][number];
        } else {
            p_calculateDataLinear = NULL;
            p_chanStatusLinear = NULL;
        }

    } else { //扩展AI通道
        Q_ASSERT(uninNum<=EXT_UNIT_NUM && moduleIndex<EXT_UNIT_MODULE_NUM && number<UNIT_CHAN_NUM_PRE);

        m_pData = (float*) &pModbusRegMap->inputReg_ExtUnit_IO_Data_Float32[uninNum-1][moduleIndex][number<<1];
        m_pStatus = &(pModbusRegMap->inputReg_ExtUnit_IO_Status_Int16[uninNum-1][moduleIndex][number]);
        m_pConfig = &(DevConfigPtr->extUnitAI[uninNum-1][moduleIndex][number]);

        /*运算相关*/
        p_inputData = (float*)&g_originDataMap.inputReg_ExtUnit_IO_Data_Int32[uninNum-1][moduleIndex][number<<1];
        p_calculateData =  (float*)&g_originDataMap.inputReg_ExtUnit_IO_Data_Float32[uninNum-1][moduleIndex][number<<1];
        p_chanStatus = &g_originDataMap.inputReg_ExtUnit_IO_Status_Int16[uninNum-1][moduleIndex][number];
        p_calculateDataLinear = NULL;
        p_chanStatusLinear = NULL;
    }

    m_DelayUpperCounter = 0;
    m_DelayDownerCounter = 0;
}

void DevChannelAI::chanCalculate()
{
    float inputData = *p_inputData;          //通道测量输入数据（输入值）
    float tempData;                                     //临时数据,代表inputData经过查表、移动平均、冷端补偿后的值
    qreal tempDataCalc;                             //测量数据，代表tempData经过Delta/Linear/Sqrt以及偏移处理后的值
    qint16 preStatus = *p_chanStatus;      //通道之前状态值

    //通道数据无效，直接返回
    if (IS_DATA_INVALID(*p_chanStatus)) {
        if (p_chanStatusLinear != NULL) *p_chanStatusLinear = *p_chanStatus;
        return;
    }

    //查表、补偿处理
    if(m_pConfig->rangeConfig.rangeType == AI_RANGE_Type_Disabled) { //通道未使用，设置状态并返回
        *p_chanStatus = DATA_ERROR_OFF<<DATA_STATUS_ERROR_BITSOFF;
        if (p_chanStatusLinear != NULL) *p_chanStatusLinear = *p_chanStatus;
        return;
    } else if (m_pConfig->rangeConfig.rangeType == AI_RANGE_Type_TC) { //热电耦，查表获取对应值
        //断耦判定
        if (qIsNaN(inputData)) {
            switch (m_pConfig->rangeConfig.rangeBurnout) {
            case AI_Range_Burnout_Off:
                *p_chanStatus = DATA_ERROR_INVALID<<DATA_STATUS_ERROR_BITSOFF;
                break;
            case AI_Range_Burnout_Up:
                *p_chanStatus = DATA_ERROR_BURNOUT_POSITIVE<<DATA_STATUS_ERROR_BITSOFF;
                break;
            case AI_Range_Burnout_Down:
                *p_chanStatus = DATA_ERROR_BURNOUT_NEGATIVE<<DATA_STATUS_ERROR_BITSOFF;
                break;
            }
            if (p_chanStatusLinear != NULL) *p_chanStatusLinear = *p_chanStatus;
            return;
        }

        float tempRef;  //补偿温度
        quint32 mode = m_pConfig->rangeConfig.rangeRJCCompensation.rjcCompMode;
        if (1 == mode) {   //外部补偿
            tempRef = m_pConfig->rangeConfig.rangeRJCCompensation.rjcTemp;
        } else {                //内部补偿
            tempRef = (qobject_cast<DevBoard *>(this->parent()))->getBoardColdTemperature();
        }

        if (DataQueryTC::instance()->getTempValTC(m_pConfig->rangeConfig.config.rangeTC, inputData, tempRef, mode, &tempData) == false){
            *p_chanStatus = DATA_ERROR_AD_ERROR<<DATA_STATUS_ERROR_BITSOFF;
            if (p_chanStatusLinear != NULL) *p_chanStatusLinear = *p_chanStatus;
            return;
        }
    } else if (m_pConfig->rangeConfig.rangeType == AI_RANGE_Type_RTD) { //热电阻，查表获取对应值
        //断耦判定
        if (qIsNaN(inputData)) {
            switch (m_pConfig->rangeConfig.rangeBurnout) {
            case AI_Range_Burnout_Off:
                *p_chanStatus = DATA_ERROR_INVALID<<DATA_STATUS_ERROR_BITSOFF;
                break;
            case AI_Range_Burnout_Up:
                *p_chanStatus = DATA_ERROR_BURNOUT_POSITIVE<<DATA_STATUS_ERROR_BITSOFF;
                break;
            case AI_Range_Burnout_Down:
                *p_chanStatus = DATA_ERROR_BURNOUT_NEGATIVE<<DATA_STATUS_ERROR_BITSOFF;
                break;
            }
            if (p_chanStatusLinear != NULL) *p_chanStatusLinear = *p_chanStatus;
            return;
        }

        if (DataQueryRTD::instance()->getTempValRTD(m_pConfig->rangeConfig.config.rangeRTD, inputData, &tempData) == false){
            *p_chanStatus = DATA_ERROR_AD_ERROR<<DATA_STATUS_ERROR_BITSOFF;
            if (p_chanStatusLinear != NULL) *p_chanStatusLinear = *p_chanStatus;
            return;
        }
    } else {
        tempData = inputData;
    }

    //移动平均计算
    if (m_pConfig->rangeConfig.rangeMovingAvg.avgOnOff){
        quint8 count = m_pConfig->rangeConfig.rangeMovingAvg.avgCount;
        qreal ave=0;
        if (m_moveFIFO.count() < count) {
            m_moveFIFO.enqueue(tempData);
        } else {
            m_moveFIFO.dequeue();
            m_moveFIFO.enqueue(tempData);
        }
        for (int i=0; i<m_moveFIFO.count(); ++i){
            ave += m_moveFIFO.at(i);
        }
        tempData = ave/m_moveFIFO.count();
    }

    //通道计算
    qreal K,D;
    qreal a,b,A,B;  //a/b : 量程范围； A/B : 设定范围

    if (m_pConfig->rangeConfig.rangeCalculateType == AI_Range_Calculate_Linear ||
        m_pConfig->rangeConfig.rangeCalculateType == AI_Range_Calculate_Sqrt) {
        a = m_pConfig->rangeConfig.rangeDispMin;
        b = m_pConfig->rangeConfig.rangeDispMax;
        A = m_pConfig->rangeConfig.rangeScale.scaleMin/::pow10(m_pConfig->rangeConfig.rangeScale.scalePosition);
        B = m_pConfig->rangeConfig.rangeScale.scaleMax/::pow10(m_pConfig->rangeConfig.rangeScale.scalePosition);
        K = (B-A)/(b-a);
        D = (b*A - a*B)/(b-a);
    }
    switch (m_pConfig->rangeConfig.rangeCalculateType) {
    case AI_Range_Calculate_Disabled:
        tempDataCalc = tempData;
        break;
    case AI_Range_Calculate_Delta:  //如果基准通道号比当前通道号大，则取该基准通道上次的测量值
        if (this->getChannelCoder() == m_pConfig->rangeConfig.rangeBasicChannel) {
            tempDataCalc = 0;
        } else {
            tempDataCalc = tempData - CfgChannel::instance()->getChannelData(this->getChannelCoder(), m_pConfig->rangeConfig.rangeBasicChannel);
        }
        break;
    case AI_Range_Calculate_Linear: //线性缩放运算
        tempDataCalc = K*tempData + D;
        break;
    case AI_Range_Calculate_Sqrt:   //平方根运算
        tempDataCalc = (B-A)*::sqrt((tempData-a)/(b-a)) + A;
        break;
    default:
        tempDataCalc = tempData;
        break;
    }

    //小信号切除
    if ((m_pConfig->rangeConfig.rangeType == AI_RANGE_Type_Vlot && m_pConfig->rangeConfig.rangeCalculateType == AI_Range_Calculate_Sqrt) || \
        (m_pConfig->rangeConfig.rangeType == AI_RANGE_Type_GS && \
        (m_pConfig->rangeConfig.rangeCalculateType == AI_Range_Calculate_Linear || m_pConfig->rangeConfig.rangeCalculateType == AI_Range_Calculate_Sqrt))) {
        if(m_pConfig->rangeConfig.rangeLowCut.lowCutOnOff) {
            qreal lowCutVal = (b-a)*m_pConfig->rangeConfig.rangeLowCut.lowCutPos/100;
            if (tempData<=lowCutVal) {
                if (m_pConfig->rangeConfig.rangeLowCut.lowCutOut == 0) {  //%0输出
                    tempDataCalc = A;
                } else {    //线性输出
                    tempDataCalc = K*tempData + D;
                }
            }
        }
    }

    //偏移处理
    if (m_pConfig->rangeConfig.rangeType != AI_RANGE_Type_DI)
        tempDataCalc += (float)m_pConfig->rangeConfig.rangeBias/::pow10(this->getChannelDataAccuracy().toUInt());

    //通道溢出判定
    if (DevConfigPtr->measureOverRange == Measure_OverRange_Free) { //自由，基于实际量程范围5%进行判定
        qreal rangeOver = (m_pConfig->rangeConfig.rangeMax - m_pConfig->rangeConfig.rangeMin) /20;
        if (inputData > (m_pConfig->rangeConfig.rangeMax + rangeOver)){    //正溢出
            *p_chanStatus = DATA_ERROR_OVER_POSITIVE<<DATA_STATUS_ERROR_BITSOFF;
            if (p_chanStatusLinear != NULL) *p_chanStatusLinear = *p_chanStatus;
        } else if (inputData < (m_pConfig->rangeConfig.rangeMin - rangeOver)){ //负溢出
            *p_chanStatus = DATA_ERROR_OVER_NEGATIVE<<DATA_STATUS_ERROR_BITSOFF;
            if (p_chanStatusLinear != NULL) *p_chanStatusLinear = *p_chanStatus;
        }
    } else {    //溢出，基于设定范围5%进行判定
        qreal rangMax, rangMin;
        if (m_pConfig->rangeConfig.rangeCalculateType == AI_Range_Calculate_Disabled) {
            qreal rangeOver = (m_pConfig->rangeConfig.rangeDispMax - m_pConfig->rangeConfig.rangeDispMin) /20;
            rangMax = m_pConfig->rangeConfig.rangeDispMax + rangeOver;
            rangMin = m_pConfig->rangeConfig.rangeDispMin - rangeOver;
        } else {
            qreal rangeOver = (B - A) /20;
            rangMax = B + rangeOver;
            rangMin = A - rangeOver;
        }

        if (tempDataCalc > rangMax){    //正溢出
            *p_chanStatus = DATA_ERROR_OVER_POSITIVE<<DATA_STATUS_ERROR_BITSOFF;
            if (p_chanStatusLinear != NULL) *p_chanStatusLinear = *p_chanStatus;
        } else if (tempDataCalc < rangMin){ //负溢出
            *p_chanStatus = DATA_ERROR_OVER_NEGATIVE<<DATA_STATUS_ERROR_BITSOFF;
            if (p_chanStatusLinear != NULL) *p_chanStatusLinear = *p_chanStatus;
        }
    }

    //通道断偶判定--[AI_RANGE_Type_GS], [AI_RANGE_Type_GS4_20mA]
    if (m_pConfig->rangeConfig.rangeType == AI_RANGE_Type_GS ||
        m_pConfig->rangeConfig.rangeType == AI_RANGE_Type_GS4_20mA) {
        if (AI_Range_Burnout_Off  != m_pConfig->rangeConfig.rangeBurnout) {
            qreal upper = (m_pConfig->rangeConfig.rangeDispMax - m_pConfig->rangeConfig.rangeDispMin) * ((DevBoard*)this->parent())->getBurnoutUpperLimit() /100;
            upper += m_pConfig->rangeConfig.rangeDispMax;
            qreal downer = (m_pConfig->rangeConfig.rangeDispMax - m_pConfig->rangeConfig.rangeDispMin) * ((DevBoard*)this->parent())->getBurnoutDownLimit() /100;
            downer += m_pConfig->rangeConfig.rangeDispMin;

            if (tempDataCalc > upper) {
                *p_chanStatus = DATA_ERROR_BURNOUT_POSITIVE<<DATA_STATUS_ERROR_BITSOFF;
            } else if (tempDataCalc < downer) {
                *p_chanStatus = DATA_ERROR_BURNOUT_NEGATIVE<<DATA_STATUS_ERROR_BITSOFF;
            }
            if (p_chanStatusLinear != NULL) *p_chanStatusLinear = *p_chanStatus;
        }
    }

//    AI测量值校正：量程为[DI]、[不使用]时，选项固定为Off。
//    点击[执行输入测量]时，显示确认画面。
//    点击[OK]时，将当前测量值设为折线输入值。

//    前提：确定AI校准的上下限及坐标值；最少有两个点（P1,Q1）~ (P12,Q12）；每组的X值（线性输入值），
//          请务必设定一个大于上一X的值。
//    计算方法：Y-仪表显示值，即校正后的值；X-测量值；
//        1、折线近似--使用折线指定的特性校正输入值，获得校正后显示值。
//            k = Δy/Δx;    // (-∞,0), [0,1), ..., [10,11], (11,+∞)
//            12个点，11+2个区间，11个k值，2个偏移值，位于哪个[P?,P?]区间，就使用哪个k值；
//            Y = kX + b；
//        2、折线偏移--使用折线指定的偏移值校正输入值，获得校正后显示值。
//            偏移值 = kX + b；方法同上；
//            Y - X = 偏移值；
//            Y = kX + b + X；
//            => Y = (k+1)X + b；

    qreal slopeK[11] = {0};    //斜率
    qreal biasB[11] = {0};     //截距
    qreal kk,bb;
    qreal p,q,P,Q;

    if (m_pConfig->correctConfig.correctMode == 1 ||
        m_pConfig->correctConfig.correctMode == 2) {     //校准模式，0-off，1-折线近似，2-折线偏移
        for(quint8 i=0;i<m_pConfig->correctConfig.correctCount-1;++i) {
            p = m_pConfig->correctConfig.correctPointIn[i];
            P = m_pConfig->correctConfig.correctPointOut[i];
            q = m_pConfig->correctConfig.correctPointIn[i+1];
            Q = m_pConfig->correctConfig.correctPointOut[i+1];

            slopeK[i] = (Q-P)/(q-p);
            biasB[i] = (q*P - p*Q)/(q-p);
        }
    }

    switch(m_pConfig->correctConfig.correctMode) {
    case 0:
        tempDataCalc = tempData;
        break;
    case 1: //1-折线近似
        switch(m_pConfig->correctConfig.correctCount) {
        case 2: // (-∞,0), [0,1], (1,+∞)
            if(tempData < m_pConfig->correctConfig.correctPointIn[0]) {
                kk = 1;
                bb = m_pConfig->correctConfig.correctPointOut[0];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[0]) \
                       && (tempData <= m_pConfig->correctConfig.correctPointIn[1])) {
                kk = slopeK[0];
                bb = biasB[0];
            } else if (tempData > m_pConfig->correctConfig.correctPointIn[1]) {
                kk = 1;
                bb = m_pConfig->correctConfig.correctPointOut[1];
            }
            break;
        case 3: // (-∞,0), [0,1), [1,2], (2,+∞)
            if(tempData < m_pConfig->correctConfig.correctPointIn[0]) {
                kk = 1;
                bb = m_pConfig->correctConfig.correctPointOut[0];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[0]) \
                       && (tempData < m_pConfig->correctConfig.correctPointIn[1])) {
                kk = slopeK[0];
                bb = biasB[0];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[1]) \
                       && (tempData <= m_pConfig->correctConfig.correctPointIn[2])) {
                kk = slopeK[1];
                bb = biasB[1];
            } else if (tempData > m_pConfig->correctConfig.correctPointIn[2]) {
                kk = 1;
                bb = m_pConfig->correctConfig.correctPointOut[2];
            }
            break;
        case 4: // (-∞,0), [0,1), [1,2), [2,3], (3,+∞)
            if(tempData < m_pConfig->correctConfig.correctPointIn[0]) {
                kk = 1;
                bb = m_pConfig->correctConfig.correctPointOut[0];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[0]) \
                       && (tempData < m_pConfig->correctConfig.correctPointIn[1])) {
                kk = slopeK[0];
                bb = biasB[0];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[1]) \
                       && (tempData < m_pConfig->correctConfig.correctPointIn[2])) {
                kk = slopeK[1];
                bb = biasB[1];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[2]) \
                       && (tempData <= m_pConfig->correctConfig.correctPointIn[3])) {
                kk = slopeK[2];
                bb = biasB[2];
            } else if (tempData > m_pConfig->correctConfig.correctPointIn[3]) {
                kk = 1;
                bb = m_pConfig->correctConfig.correctPointOut[3];
            }
            break;
        case 5: // (-∞,0), [0,1), [1,2), [2,3), [3,4], (4,+∞)
            if(tempData < m_pConfig->correctConfig.correctPointIn[0]) {
                kk = 1;
                bb = m_pConfig->correctConfig.correctPointOut[0];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[0]) \
                       && (tempData < m_pConfig->correctConfig.correctPointIn[1])) {
                kk = slopeK[0];
                bb = biasB[0];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[1]) \
                       && (tempData < m_pConfig->correctConfig.correctPointIn[2])) {
                kk = slopeK[1];
                bb = biasB[1];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[2]) \
                       && (tempData < m_pConfig->correctConfig.correctPointIn[3])) {
                kk = slopeK[2];
                bb = biasB[2];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[3]) \
                       && (tempData <= m_pConfig->correctConfig.correctPointIn[4])) {
                kk = slopeK[3];
                bb = biasB[3];
            } else if (tempData > m_pConfig->correctConfig.correctPointIn[4]) {
                kk = 1;
                bb = m_pConfig->correctConfig.correctPointOut[4];
            }
            break;
        case 6: // (-∞,0), [0,1), ..., [4,5], (5,+∞)
            if(tempData < m_pConfig->correctConfig.correctPointIn[0]) {
                kk = 1;
                bb = m_pConfig->correctConfig.correctPointOut[0];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[0]) \
                       && (tempData < m_pConfig->correctConfig.correctPointIn[1])) {
                kk = slopeK[0];
                bb = biasB[0];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[1]) \
                       && (tempData < m_pConfig->correctConfig.correctPointIn[2])) {
                kk = slopeK[1];
                bb = biasB[1];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[2]) \
                       && (tempData < m_pConfig->correctConfig.correctPointIn[3])) {
                kk = slopeK[2];
                bb = biasB[2];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[3]) \
                       && (tempData < m_pConfig->correctConfig.correctPointIn[4])) {
                kk = slopeK[3];
                bb = biasB[3];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[4]) \
                       && (tempData <= m_pConfig->correctConfig.correctPointIn[5])) {
                kk = slopeK[4];
                bb = biasB[4];
            } else if (tempData > m_pConfig->correctConfig.correctPointIn[5]) {
                kk = 1;
                bb = m_pConfig->correctConfig.correctPointOut[5];
            }
            break;
        case 7: // (-∞,0), [0,1), ..., [5,6], (6,+∞)
            if(tempData < m_pConfig->correctConfig.correctPointIn[0]) {
                kk = 1;
                bb = m_pConfig->correctConfig.correctPointOut[0];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[0]) \
                       && (tempData < m_pConfig->correctConfig.correctPointIn[1])) {
                kk = slopeK[0];
                bb = biasB[0];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[1]) \
                       && (tempData < m_pConfig->correctConfig.correctPointIn[2])) {
                kk = slopeK[1];
                bb = biasB[1];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[2]) \
                       && (tempData < m_pConfig->correctConfig.correctPointIn[3])) {
                kk = slopeK[2];
                bb = biasB[2];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[3]) \
                       && (tempData < m_pConfig->correctConfig.correctPointIn[4])) {
                kk = slopeK[3];
                bb = biasB[3];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[4]) \
                       && (tempData < m_pConfig->correctConfig.correctPointIn[5])) {
                kk = slopeK[4];
                bb = biasB[4];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[5]) \
                       && (tempData <= m_pConfig->correctConfig.correctPointIn[6])) {
                kk = slopeK[5];
                bb = biasB[5];
            } else if (tempData > m_pConfig->correctConfig.correctPointIn[6]) {
                kk = 1;
                bb = m_pConfig->correctConfig.correctPointOut[6];
            }
            break;
        case 8: // (-∞,0), [0,1), ..., [6,7], (7,+∞)
            if(tempData < m_pConfig->correctConfig.correctPointIn[0]) {
                kk = 1;
                bb = m_pConfig->correctConfig.correctPointOut[0];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[0]) \
                       && (tempData < m_pConfig->correctConfig.correctPointIn[1])) {
                kk = slopeK[0];
                bb = biasB[0];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[1]) \
                       && (tempData < m_pConfig->correctConfig.correctPointIn[2])) {
                kk = slopeK[1];
                bb = biasB[1];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[2]) \
                       && (tempData < m_pConfig->correctConfig.correctPointIn[3])) {
                kk = slopeK[2];
                bb = biasB[2];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[3]) \
                       && (tempData < m_pConfig->correctConfig.correctPointIn[4])) {
                kk = slopeK[3];
                bb = biasB[3];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[4]) \
                       && (tempData < m_pConfig->correctConfig.correctPointIn[5])) {
                kk = slopeK[4];
                bb = biasB[4];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[5]) \
                       && (tempData < m_pConfig->correctConfig.correctPointIn[6])) {
                kk = slopeK[5];
                bb = biasB[5];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[6]) \
                       && (tempData <= m_pConfig->correctConfig.correctPointIn[7])) {
                kk = slopeK[6];
                bb = biasB[6];
            } else if (tempData > m_pConfig->correctConfig.correctPointIn[7]) {
                kk = 1;
                bb = m_pConfig->correctConfig.correctPointOut[7];
            }
            break;
        case 9: // (-∞,0), [0,1), ..., [7,8], (8,+∞)
            if(tempData < m_pConfig->correctConfig.correctPointIn[0]) {
                kk = 1;
                bb = m_pConfig->correctConfig.correctPointOut[0];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[0]) \
                       && (tempData < m_pConfig->correctConfig.correctPointIn[1])) {
                kk = slopeK[0];
                bb = biasB[0];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[1]) \
                       && (tempData < m_pConfig->correctConfig.correctPointIn[2])) {
                kk = slopeK[1];
                bb = biasB[1];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[2]) \
                       && (tempData < m_pConfig->correctConfig.correctPointIn[3])) {
                kk = slopeK[2];
                bb = biasB[2];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[3]) \
                       && (tempData < m_pConfig->correctConfig.correctPointIn[4])) {
                kk = slopeK[3];
                bb = biasB[3];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[4]) \
                       && (tempData < m_pConfig->correctConfig.correctPointIn[5])) {
                kk = slopeK[4];
                bb = biasB[4];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[5]) \
                       && (tempData < m_pConfig->correctConfig.correctPointIn[6])) {
                kk = slopeK[5];
                bb = biasB[5];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[6]) \
                       && (tempData < m_pConfig->correctConfig.correctPointIn[7])) {
                kk = slopeK[6];
                bb = biasB[6];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[7]) \
                       && (tempData <= m_pConfig->correctConfig.correctPointIn[8])) {
                kk = slopeK[7];
                bb = biasB[7];
            } else if (tempData > m_pConfig->correctConfig.correctPointIn[8]) {
                kk = 1;
                bb = m_pConfig->correctConfig.correctPointOut[8];
            }
            break;
        case 10: // (-∞,0), [0,1), ..., [8,9], (9,+∞)
            if(tempData < m_pConfig->correctConfig.correctPointIn[0]) {
                kk = 1;
                bb = m_pConfig->correctConfig.correctPointOut[0];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[0]) \
                       && (tempData < m_pConfig->correctConfig.correctPointIn[1])) {
                kk = slopeK[0];
                bb = biasB[0];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[1]) \
                       && (tempData < m_pConfig->correctConfig.correctPointIn[2])) {
                kk = slopeK[1];
                bb = biasB[1];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[2]) \
                       && (tempData < m_pConfig->correctConfig.correctPointIn[3])) {
                kk = slopeK[2];
                bb = biasB[2];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[3]) \
                       && (tempData < m_pConfig->correctConfig.correctPointIn[4])) {
                kk = slopeK[3];
                bb = biasB[3];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[4]) \
                       && (tempData < m_pConfig->correctConfig.correctPointIn[5])) {
                kk = slopeK[4];
                bb = biasB[4];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[5]) \
                       && (tempData < m_pConfig->correctConfig.correctPointIn[6])) {
                kk = slopeK[5];
                bb = biasB[5];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[6]) \
                       && (tempData < m_pConfig->correctConfig.correctPointIn[7])) {
                kk = slopeK[6];
                bb = biasB[6];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[7]) \
                       && (tempData < m_pConfig->correctConfig.correctPointIn[8])) {
                kk = slopeK[7];
                bb = biasB[7];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[8]) \
                       && (tempData <= m_pConfig->correctConfig.correctPointIn[9])) {
                kk = slopeK[8];
                bb = biasB[8];
            } else if (tempData > m_pConfig->correctConfig.correctPointIn[9]) {
                kk = 1;
                bb = m_pConfig->correctConfig.correctPointOut[9];
            }
            break;
        case 11: // (-∞,0), [0,1), ..., [9,10], (10,+∞)
            if(tempData < m_pConfig->correctConfig.correctPointIn[0]) {
                kk = 1;
                bb = m_pConfig->correctConfig.correctPointOut[0];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[0]) \
                       && (tempData < m_pConfig->correctConfig.correctPointIn[1])) {
                kk = slopeK[0];
                bb = biasB[0];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[1]) \
                       && (tempData < m_pConfig->correctConfig.correctPointIn[2])) {
                kk = slopeK[1];
                bb = biasB[1];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[2]) \
                       && (tempData < m_pConfig->correctConfig.correctPointIn[3])) {
                kk = slopeK[2];
                bb = biasB[2];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[3]) \
                       && (tempData < m_pConfig->correctConfig.correctPointIn[4])) {
                kk = slopeK[3];
                bb = biasB[3];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[4]) \
                       && (tempData < m_pConfig->correctConfig.correctPointIn[5])) {
                kk = slopeK[4];
                bb = biasB[4];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[5]) \
                       && (tempData < m_pConfig->correctConfig.correctPointIn[6])) {
                kk = slopeK[5];
                bb = biasB[5];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[6]) \
                       && (tempData < m_pConfig->correctConfig.correctPointIn[7])) {
                kk = slopeK[6];
                bb = biasB[6];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[7]) \
                       && (tempData < m_pConfig->correctConfig.correctPointIn[8])) {
                kk = slopeK[7];
                bb = biasB[7];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[8]) \
                       && (tempData < m_pConfig->correctConfig.correctPointIn[9])) {
                kk = slopeK[8];
                bb = biasB[8];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[9]) \
                       && (tempData <= m_pConfig->correctConfig.correctPointIn[10])) {
                kk = slopeK[9];
                bb = biasB[9];
            } else if (tempData > m_pConfig->correctConfig.correctPointIn[10]) {
                kk = 1;
                bb = m_pConfig->correctConfig.correctPointOut[10];
            }
            break;
        case 12: // (-∞,0), [0,1), ..., [10,11], (11,+∞)
            if(tempData < m_pConfig->correctConfig.correctPointIn[0]) {
                kk = 1;
                bb = m_pConfig->correctConfig.correctPointOut[0];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[0]) \
                       && (tempData < m_pConfig->correctConfig.correctPointIn[1])) {
                kk = slopeK[0];
                bb = biasB[0];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[1]) \
                       && (tempData < m_pConfig->correctConfig.correctPointIn[2])) {
                kk = slopeK[1];
                bb = biasB[1];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[2]) \
                       && (tempData < m_pConfig->correctConfig.correctPointIn[3])) {
                kk = slopeK[2];
                bb = biasB[2];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[3]) \
                       && (tempData < m_pConfig->correctConfig.correctPointIn[4])) {
                kk = slopeK[3];
                bb = biasB[3];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[4]) \
                       && (tempData < m_pConfig->correctConfig.correctPointIn[5])) {
                kk = slopeK[4];
                bb = biasB[4];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[5]) \
                       && (tempData < m_pConfig->correctConfig.correctPointIn[6])) {
                kk = slopeK[5];
                bb = biasB[5];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[6]) \
                       && (tempData < m_pConfig->correctConfig.correctPointIn[7])) {
                kk = slopeK[6];
                bb = biasB[6];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[7]) \
                       && (tempData < m_pConfig->correctConfig.correctPointIn[8])) {
                kk = slopeK[7];
                bb = biasB[7];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[8]) \
                       && (tempData < m_pConfig->correctConfig.correctPointIn[9])) {
                kk = slopeK[8];
                bb = biasB[8];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[9]) \
                       && (tempData < m_pConfig->correctConfig.correctPointIn[10])) {
                kk = slopeK[9];
                bb = biasB[9];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[10]) \
                       && (tempData <= m_pConfig->correctConfig.correctPointIn[11])) {
                kk = slopeK[10];
                bb = biasB[10];
            } else if (tempData > m_pConfig->correctConfig.correctPointIn[11]) {
                kk = 1;
                bb = m_pConfig->correctConfig.correctPointOut[11];
            }
            break;
        }
        tempDataCalc = kk*tempData + bb;
        break;
    case 2: //2-折线偏移
        switch(m_pConfig->correctConfig.correctCount) {
        case 2: // (-∞,0), [0,1], (1,+∞)
            if(tempData < m_pConfig->correctConfig.correctPointIn[0]) {
                kk = 0;
                bb = m_pConfig->correctConfig.correctPointOut[0];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[0]) \
                       && (tempData <= m_pConfig->correctConfig.correctPointIn[1])) {
                kk = slopeK[0];
                bb = biasB[0];
            } else if (tempData > m_pConfig->correctConfig.correctPointIn[1]) {
                kk = 0;
                bb = m_pConfig->correctConfig.correctPointOut[1];
            }
            break;
        case 3: // (-∞,0), [0,1), [1,2], (2,+∞)
            if(tempData < m_pConfig->correctConfig.correctPointIn[0]) {
                kk = 0;
                bb = m_pConfig->correctConfig.correctPointOut[0];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[0]) \
                       && (tempData < m_pConfig->correctConfig.correctPointIn[1])) {
                kk = slopeK[0];
                bb = biasB[0];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[1]) \
                       && (tempData <= m_pConfig->correctConfig.correctPointIn[2])) {
                kk = slopeK[1];
                bb = biasB[1];
            } else if (tempData > m_pConfig->correctConfig.correctPointIn[2]) {
                kk = 0;
                bb = m_pConfig->correctConfig.correctPointOut[2];
            }
            break;
        case 4: // (-∞,0), [0,1), [1,2), [2,3], (3,+∞)
            if(tempData < m_pConfig->correctConfig.correctPointIn[0]) {
                kk = 0;
                bb = m_pConfig->correctConfig.correctPointOut[0];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[0]) \
                       && (tempData < m_pConfig->correctConfig.correctPointIn[1])) {
                kk = slopeK[0];
                bb = biasB[0];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[1]) \
                       && (tempData < m_pConfig->correctConfig.correctPointIn[2])) {
                kk = slopeK[1];
                bb = biasB[1];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[2]) \
                       && (tempData <= m_pConfig->correctConfig.correctPointIn[3])) {
                kk = slopeK[2];
                bb = biasB[2];
            } else if (tempData > m_pConfig->correctConfig.correctPointIn[3]) {
                kk = 0;
                bb = m_pConfig->correctConfig.correctPointOut[3];
            }
            break;
        case 5: // (-∞,0), [0,1), [1,2), [2,3), [3,4], (4,+∞)
            if(tempData < m_pConfig->correctConfig.correctPointIn[0]) {
                kk = 0;
                bb = m_pConfig->correctConfig.correctPointOut[0];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[0]) \
                       && (tempData < m_pConfig->correctConfig.correctPointIn[1])) {
                kk = slopeK[0];
                bb = biasB[0];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[1]) \
                       && (tempData < m_pConfig->correctConfig.correctPointIn[2])) {
                kk = slopeK[1];
                bb = biasB[1];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[2]) \
                       && (tempData < m_pConfig->correctConfig.correctPointIn[3])) {
                kk = slopeK[2];
                bb = biasB[2];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[3]) \
                       && (tempData <= m_pConfig->correctConfig.correctPointIn[4])) {
                kk = slopeK[3];
                bb = biasB[3];
            } else if (tempData > m_pConfig->correctConfig.correctPointIn[4]) {
                kk = 0;
                bb = m_pConfig->correctConfig.correctPointOut[4];
            }
            break;
        case 6: // (-∞,0), [0,1), ..., [4,5], (5,+∞)
            if(tempData < m_pConfig->correctConfig.correctPointIn[0]) {
                kk = 0;
                bb = m_pConfig->correctConfig.correctPointOut[0];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[0]) \
                       && (tempData < m_pConfig->correctConfig.correctPointIn[1])) {
                kk = slopeK[0];
                bb = biasB[0];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[1]) \
                       && (tempData < m_pConfig->correctConfig.correctPointIn[2])) {
                kk = slopeK[1];
                bb = biasB[1];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[2]) \
                       && (tempData < m_pConfig->correctConfig.correctPointIn[3])) {
                kk = slopeK[2];
                bb = biasB[2];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[3]) \
                       && (tempData < m_pConfig->correctConfig.correctPointIn[4])) {
                kk = slopeK[3];
                bb = biasB[3];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[4]) \
                       && (tempData <= m_pConfig->correctConfig.correctPointIn[5])) {
                kk = slopeK[4];
                bb = biasB[4];
            } else if (tempData > m_pConfig->correctConfig.correctPointIn[5]) {
                kk = 0;
                bb = m_pConfig->correctConfig.correctPointOut[5];
            }
            break;
        case 7: // (-∞,0), [0,1), ..., [5,6], (6,+∞)
            if(tempData < m_pConfig->correctConfig.correctPointIn[0]) {
                kk = 0;
                bb = m_pConfig->correctConfig.correctPointOut[0];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[0]) \
                       && (tempData < m_pConfig->correctConfig.correctPointIn[1])) {
                kk = slopeK[0];
                bb = biasB[0];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[1]) \
                       && (tempData < m_pConfig->correctConfig.correctPointIn[2])) {
                kk = slopeK[1];
                bb = biasB[1];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[2]) \
                       && (tempData < m_pConfig->correctConfig.correctPointIn[3])) {
                kk = slopeK[2];
                bb = biasB[2];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[3]) \
                       && (tempData < m_pConfig->correctConfig.correctPointIn[4])) {
                kk = slopeK[3];
                bb = biasB[3];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[4]) \
                       && (tempData < m_pConfig->correctConfig.correctPointIn[5])) {
                kk = slopeK[4];
                bb = biasB[4];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[5]) \
                       && (tempData <= m_pConfig->correctConfig.correctPointIn[6])) {
                kk = slopeK[5];
                bb = biasB[5];
            } else if (tempData > m_pConfig->correctConfig.correctPointIn[6]) {
                kk = 0;
                bb = m_pConfig->correctConfig.correctPointOut[6];
            }
            break;
        case 8: // (-∞,0), [0,1), ..., [6,7], (7,+∞)
            if(tempData < m_pConfig->correctConfig.correctPointIn[0]) {
                kk = 0;
                bb = m_pConfig->correctConfig.correctPointOut[0];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[0]) \
                       && (tempData < m_pConfig->correctConfig.correctPointIn[1])) {
                kk = slopeK[0];
                bb = biasB[0];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[1]) \
                       && (tempData < m_pConfig->correctConfig.correctPointIn[2])) {
                kk = slopeK[1];
                bb = biasB[1];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[2]) \
                       && (tempData < m_pConfig->correctConfig.correctPointIn[3])) {
                kk = slopeK[2];
                bb = biasB[2];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[3]) \
                       && (tempData < m_pConfig->correctConfig.correctPointIn[4])) {
                kk = slopeK[3];
                bb = biasB[3];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[4]) \
                       && (tempData < m_pConfig->correctConfig.correctPointIn[5])) {
                kk = slopeK[4];
                bb = biasB[4];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[5]) \
                       && (tempData < m_pConfig->correctConfig.correctPointIn[6])) {
                kk = slopeK[5];
                bb = biasB[5];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[6]) \
                       && (tempData <= m_pConfig->correctConfig.correctPointIn[7])) {
                kk = slopeK[6];
                bb = biasB[6];
            } else if (tempData > m_pConfig->correctConfig.correctPointIn[7]) {
                kk = 0;
                bb = m_pConfig->correctConfig.correctPointOut[7];
            }
            break;
        case 9: // (-∞,0), [0,1), ..., [7,8], (8,+∞)
            if(tempData < m_pConfig->correctConfig.correctPointIn[0]) {
                kk = 0;
                bb = m_pConfig->correctConfig.correctPointOut[0];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[0]) \
                       && (tempData < m_pConfig->correctConfig.correctPointIn[1])) {
                kk = slopeK[0];
                bb = biasB[0];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[1]) \
                       && (tempData < m_pConfig->correctConfig.correctPointIn[2])) {
                kk = slopeK[1];
                bb = biasB[1];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[2]) \
                       && (tempData < m_pConfig->correctConfig.correctPointIn[3])) {
                kk = slopeK[2];
                bb = biasB[2];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[3]) \
                       && (tempData < m_pConfig->correctConfig.correctPointIn[4])) {
                kk = slopeK[3];
                bb = biasB[3];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[4]) \
                       && (tempData < m_pConfig->correctConfig.correctPointIn[5])) {
                kk = slopeK[4];
                bb = biasB[4];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[5]) \
                       && (tempData < m_pConfig->correctConfig.correctPointIn[6])) {
                kk = slopeK[5];
                bb = biasB[5];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[6]) \
                       && (tempData < m_pConfig->correctConfig.correctPointIn[7])) {
                kk = slopeK[6];
                bb = biasB[6];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[7]) \
                       && (tempData <= m_pConfig->correctConfig.correctPointIn[8])) {
                kk = slopeK[7];
                bb = biasB[7];
            } else if (tempData > m_pConfig->correctConfig.correctPointIn[8]) {
                kk = 0;
                bb = m_pConfig->correctConfig.correctPointOut[8];
            }
            break;
        case 10: // (-∞,0), [0,1), ..., [8,9], (9,+∞)
            if(tempData < m_pConfig->correctConfig.correctPointIn[0]) {
                kk = 0;
                bb = m_pConfig->correctConfig.correctPointOut[0];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[0]) \
                       && (tempData < m_pConfig->correctConfig.correctPointIn[1])) {
                kk = slopeK[0];
                bb = biasB[0];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[1]) \
                       && (tempData < m_pConfig->correctConfig.correctPointIn[2])) {
                kk = slopeK[1];
                bb = biasB[1];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[2]) \
                       && (tempData < m_pConfig->correctConfig.correctPointIn[3])) {
                kk = slopeK[2];
                bb = biasB[2];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[3]) \
                       && (tempData < m_pConfig->correctConfig.correctPointIn[4])) {
                kk = slopeK[3];
                bb = biasB[3];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[4]) \
                       && (tempData < m_pConfig->correctConfig.correctPointIn[5])) {
                kk = slopeK[4];
                bb = biasB[4];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[5]) \
                       && (tempData < m_pConfig->correctConfig.correctPointIn[6])) {
                kk = slopeK[5];
                bb = biasB[5];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[6]) \
                       && (tempData < m_pConfig->correctConfig.correctPointIn[7])) {
                kk = slopeK[6];
                bb = biasB[6];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[7]) \
                       && (tempData < m_pConfig->correctConfig.correctPointIn[8])) {
                kk = slopeK[7];
                bb = biasB[7];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[8]) \
                       && (tempData <= m_pConfig->correctConfig.correctPointIn[9])) {
                kk = slopeK[8];
                bb = biasB[8];
            } else if (tempData > m_pConfig->correctConfig.correctPointIn[9]) {
                kk = 0;
                bb = m_pConfig->correctConfig.correctPointOut[9];
            }
            break;
        case 11: // (-∞,0), [0,1), ..., [9,10], (10,+∞)
            if(tempData < m_pConfig->correctConfig.correctPointIn[0]) {
                kk = 0;
                bb = m_pConfig->correctConfig.correctPointOut[0];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[0]) \
                       && (tempData < m_pConfig->correctConfig.correctPointIn[1])) {
                kk = slopeK[0];
                bb = biasB[0];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[1]) \
                       && (tempData < m_pConfig->correctConfig.correctPointIn[2])) {
                kk = slopeK[1];
                bb = biasB[1];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[2]) \
                       && (tempData < m_pConfig->correctConfig.correctPointIn[3])) {
                kk = slopeK[2];
                bb = biasB[2];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[3]) \
                       && (tempData < m_pConfig->correctConfig.correctPointIn[4])) {
                kk = slopeK[3];
                bb = biasB[3];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[4]) \
                       && (tempData < m_pConfig->correctConfig.correctPointIn[5])) {
                kk = slopeK[4];
                bb = biasB[4];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[5]) \
                       && (tempData < m_pConfig->correctConfig.correctPointIn[6])) {
                kk = slopeK[5];
                bb = biasB[5];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[6]) \
                       && (tempData < m_pConfig->correctConfig.correctPointIn[7])) {
                kk = slopeK[6];
                bb = biasB[6];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[7]) \
                       && (tempData < m_pConfig->correctConfig.correctPointIn[8])) {
                kk = slopeK[7];
                bb = biasB[7];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[8]) \
                       && (tempData < m_pConfig->correctConfig.correctPointIn[9])) {
                kk = slopeK[8];
                bb = biasB[8];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[9]) \
                       && (tempData <= m_pConfig->correctConfig.correctPointIn[10])) {
                kk = slopeK[9];
                bb = biasB[9];
            } else if (tempData > m_pConfig->correctConfig.correctPointIn[10]) {
                kk = 0;
                bb = m_pConfig->correctConfig.correctPointOut[10];
            }
            break;
        case 12: // (-∞,0), [0,1), ..., [10,11], (11,+∞)
            if(tempData < m_pConfig->correctConfig.correctPointIn[0]) {
                kk = 0;
                bb = m_pConfig->correctConfig.correctPointOut[0];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[0]) \
                       && (tempData < m_pConfig->correctConfig.correctPointIn[1])) {
                kk = slopeK[0];
                bb = biasB[0];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[1]) \
                       && (tempData < m_pConfig->correctConfig.correctPointIn[2])) {
                kk = slopeK[1];
                bb = biasB[1];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[2]) \
                       && (tempData < m_pConfig->correctConfig.correctPointIn[3])) {
                kk = slopeK[2];
                bb = biasB[2];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[3]) \
                       && (tempData < m_pConfig->correctConfig.correctPointIn[4])) {
                kk = slopeK[3];
                bb = biasB[3];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[4]) \
                       && (tempData < m_pConfig->correctConfig.correctPointIn[5])) {
                kk = slopeK[4];
                bb = biasB[4];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[5]) \
                       && (tempData < m_pConfig->correctConfig.correctPointIn[6])) {
                kk = slopeK[5];
                bb = biasB[5];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[6]) \
                       && (tempData < m_pConfig->correctConfig.correctPointIn[7])) {
                kk = slopeK[6];
                bb = biasB[6];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[7]) \
                       && (tempData < m_pConfig->correctConfig.correctPointIn[8])) {
                kk = slopeK[7];
                bb = biasB[7];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[8]) \
                       && (tempData < m_pConfig->correctConfig.correctPointIn[9])) {
                kk = slopeK[8];
                bb = biasB[8];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[9]) \
                       && (tempData < m_pConfig->correctConfig.correctPointIn[10])) {
                kk = slopeK[9];
                bb = biasB[9];
            } else if ((tempData >= m_pConfig->correctConfig.correctPointIn[10]) \
                       && (tempData <= m_pConfig->correctConfig.correctPointIn[11])) {
                kk = slopeK[10];
                bb = biasB[10];
            } else if (tempData > m_pConfig->correctConfig.correctPointIn[11]) {
                kk = 0;
                bb = m_pConfig->correctConfig.correctPointOut[11];
            }
            break;
        }
        tempDataCalc = (kk + 1)*tempData + bb;
        break;
    default:
        tempDataCalc = tempData;
        break;
    }

    //通道报警: 变化率/延迟报警相关数据处理
    quint8 rateIncCount = DevConfigPtr->systemConfig.rateIncNum;
    quint8 rateDecCount = DevConfigPtr->systemConfig.rateDecNum;
    qreal compareData;
    QQueue<qreal> *pQueueInc, *pQueueDec;
    quint32 *pDelayUpper, *pDelayDowner;

    if (m_pConfig->rangeConfig.rangeCalculateType == AI_Range_Calculate_Delta)
        compareData = tempData;
    else
        compareData = tempDataCalc;

    if (m_RateIncFIFO.count()<(rateIncCount+1)){
        m_RateIncFIFO.enqueue(compareData);
    } else {
        m_RateIncFIFO.dequeue();
        m_RateIncFIFO.enqueue(compareData);
    }

    if (m_RateDecFIFO.count()<(rateDecCount+1)){
        m_RateDecFIFO.enqueue(compareData);
    } else {
        m_RateDecFIFO.dequeue();
        m_RateDecFIFO.enqueue(compareData);
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
            case Alarm_Type_H:       //上限报警
                if (compareData > m_pConfig->alarmConfig.alarmValue[i]) {
                    *p_chanStatus |= 1<<i;
                } else if (preStatus & (1<<i) && \
               (compareData > (m_pConfig->alarmConfig.alarmValue[i] - m_pConfig->alarmConfig.alarmHysteresis[i]))) {    //滞后
                    *p_chanStatus |= 1<<i;
                }
                break;
            case Alarm_Type_L:       //下限报警
                if (compareData < m_pConfig->alarmConfig.alarmValue[i]) {
                    *p_chanStatus |= 1<<i;
                } else if  (preStatus & (1<<i) && \
                (compareData < (m_pConfig->alarmConfig.alarmValue[i] + m_pConfig->alarmConfig.alarmHysteresis[i]))) {    //滞后
                    *p_chanStatus |= 1<<i;
                }
                break;
            case Alarm_Type_R:       //变化率上升限报警
                if (pQueueInc->count() == (rateIncCount+1)){
                    if ((pQueueInc->at(rateIncCount) - pQueueInc->at(0)) > m_pConfig->alarmConfig.alarmValue[i])
                        *p_chanStatus |= 1<<i;
                }
                break;
            case Alarm_Type_r:        //变化率下降限报警
                if (pQueueDec->count() == (rateDecCount+1)){
                    if ((pQueueInc->at(0) - pQueueInc->at(rateIncCount)) > m_pConfig->alarmConfig.alarmValue[i])
                        *p_chanStatus |= 1<<i;
                }
                break;
            case Alarm_Type_T:       //延迟上限---上限持续时间报警
                if (compareData > m_pConfig->alarmConfig.alarmValue[i]){
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
            case Alarm_Type_t:       //延迟下限---下限持续时间报警
                if (compareData < m_pConfig->alarmConfig.alarmValue[i]){
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
            case Alarm_Type_h:      //Delta计算适用，差值上限报警
                if(m_pConfig->rangeConfig.rangeCalculateType == AI_Range_Calculate_Delta)
                {
                    if (compareData > m_pConfig->alarmConfig.alarmValue[i]) {
                        *p_chanStatus |= 1<<i;
                    } else if (preStatus & (1<<i) && \
                    (compareData > (m_pConfig->alarmConfig.alarmValue[i] - m_pConfig->alarmConfig.alarmHysteresis[i]))) {    //滞后
                        *p_chanStatus |= 1<<i;
                    }
                }
                break;
            case Alarm_Type_l:       //Delta计算适用，差值下限报警
                if(m_pConfig->rangeConfig.rangeCalculateType == AI_Range_Calculate_Delta)
                {
                    if (compareData < m_pConfig->alarmConfig.alarmValue[i]) {
                        *p_chanStatus |= 1<<i;
                    } else if (preStatus & (1<<i) && \
                    (compareData < (m_pConfig->alarmConfig.alarmValue[i] + m_pConfig->alarmConfig.alarmHysteresis[i]))) {    //滞后
                        *p_chanStatus |= 1<<i;
                    }
                }
                break;
            default:
                break;
            }
        }
    }

    *p_calculateData = tempDataCalc;
    if(p_calculateDataLinear != NULL) *p_calculateDataLinear = tempDataCalc;
    if(p_chanStatusLinear !=NULL) *p_chanStatusLinear = *p_chanStatus;
}

/*AI通道配置变更函数*/
void DevChannelAI::slotChanConfigChanged()
{
    m_moveFIFO.clear();
    m_RateIncFIFO.clear();
    m_RateDecFIFO.clear();

    m_DelayUpperCounter = 0;
    m_DelayDownerCounter = 0;
}
