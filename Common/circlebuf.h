/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：circleBuf.h
 * 概   要：该类为趋势图绘制提供绘制素材缓存，结构为一循环队列.其中包括两个缓存区一个信息结构
 * QVector<TREND_POINT>* p_circleBuf; 长度为一屏数据点
 * QVector<HAND_MSG> *p_handMsg;     长度为p_circleBuf的两倍，坐标上与p_circleBuf右对齐
 * TREND_PAINT_DATA *p_paintParameter;
 *当趋势数据点进入和退出p_circleBuf时，也要实时刷新p_handMsg;
 * 当前版本：V1.0.0
 * 作   者：王宸博
 * 完成日期：2015-10-22
 *
 * 修改版本：
 * 修改说明：
 * 修改作者：
 *
 ********************************************************************/
#ifndef CIRCLEBUF_H
#define CIRCLEBUF_H

#include <QObject>
#include <QVector>
#include <QQueue>
#include <QMap>
#include <QPointF>
#include <QMutex>
#include <QSemaphore>
#include <QtMath>
#include <QImage>

#include "../GlobalData/sqlBlock_GroupInfo.h"
#include "../GlobalData/sqlBlock_RecordChanInfo.h"
#include "../GlobalData/sqlBlock_sysConfig.h"
#include "../GlobalData/dataGlobalDef_Record.h"
#include "../GlobalData/cfgGlobalDef_Trend.h"

class CircleBuf : public QObject
{
    Q_OBJECT
public:
    explicit CircleBuf(int bufsize = BUF_DATA_SIZE_MAX, int chanNum = BUF_DATA_CHANNUM_MAX,  QObject *parent = 0);
    ~CircleBuf();
    //缓冲区操作相关
    int clearBuf();
    int getBufLen() {return m_bufSize_max;}


    //最值读写
    QPointF getMaxMinValue(quint32 channelNumber);

    CircleBuf & operator = (const CircleBuf &buf);
    void copyPaintPara(const CircleBuf &buf);

public:
    //数据相关方法
    int appendInBuf(TREND_POINT &point, HAND_MSG &msg);
    int preAppendInBuf(TREND_POINT &point, HAND_MSG &msg);

    void moveDataCursor(qint32 dataCount);

    int getBufValidLen() { return m_dataBuf.count();}
    int getHMsgBufValidLen() { return m_msgBuf.count(); }

    bool isFull() const { return m_dataBuf.count()==m_bufSize_max ? true : false;}
    bool isEmpty() const { return m_dataBuf.isEmpty(); }
    QQueue<TREND_POINT> & getDataBuf() { return m_dataBuf;}
    QQueue<HAND_MSG> & getMssgBuf() { return m_msgBuf;}
    void getDataLock() { m_mutexData.lock();}
    void putDataLock() { m_mutexData.unlock();}

    //配置相关set方法
    void setRecordIndex(qint64 recordIndex) { m_paintParaInfo.recordIndex = recordIndex;}
    void setRecordStartTime(qint64 recordStartTime) {m_paintParaInfo.recordStartTime = recordStartTime;}
    void setRecordTimeGap(qint64 recordTimeGap) {m_paintParaInfo.recordTimeGap = recordTimeGap;}
    void setRecordTimeTag(quint64 recordTimeTag) {m_paintParaInfo.recordTimeTag = recordTimeTag;}
    void setRecordPointIndex(qint32 rightPointIndex) { m_paintParaInfo.rightPointIndex = rightPointIndex;}
//    void setScreenHorDiv(qint32 horDIV) {m_paintParaInfo.screenHorDiv = horDIV;}
    void setTrendPeriodMax(quint32 periodMax) { m_paintParaInfo.trendPeriodMax = periodMax; }
    void setTrendPeriodMin(quint32 periodMin) { m_paintParaInfo.trendPeriodMin = periodMin; }
    void setTrendPeriodTarget(quint32 periodMin) { m_paintParaInfo.trendPeriodTarget= periodMin;}
    void changeTrendPeriod() {
        m_paintParaInfo.trendPeriodTarget = m_paintParaInfo.trendPeriodTarget==m_paintParaInfo.trendPeriodMin ? m_paintParaInfo.trendPeriodMax : m_paintParaInfo.trendPeriodMin;
    }
    void setGroupInforHead(SQL_GROUP_INFO &head) { m_paintParaInfo.sql_group = head;}                        //设置记录组信息块
    void setGroupInfor(quint32 index, GROUP_INFO &item) {//设置记录组详细信息块
        Q_ASSERT(index<50);
        m_paintParaInfo.group_info[index] = item;
    }
    void setChannelInforHead(SQL_RECORD_CHAN_INFO &head) {m_paintParaInfo.sql_channel = head;}  //设置记录通道信息块
    void setChannelInfor(quint32 index, CHAN_INFO &item) {//设置记录通道详细信息块
        Q_ASSERT(index<500);
        m_paintParaInfo.chan_info[index] = item;
    }
    void setSysConfig(SQL_SYS_CONFIG &config) { m_paintParaInfo.sys_config = config;}                          //设置记录文件配置块
    void setDisplayFlag(bool isDisp) {m_paintParaInfo.isDisplayData = isDisp; }
    void setHeadDataInFlag(bool in) {m_paintParaInfo.isHeadIn = in;}
    void setTailDataInFlag(bool in) {m_paintParaInfo.isTailIn = in;}
    //配置相关get方法
    qint64 getRecordIndex() {return m_paintParaInfo.recordIndex; }                //读取当前记录索引号
    qint64 getRecordStartTime() { return m_paintParaInfo.recordStartTime;}  //读取当前记录开始时间
    qint64 getRecordTimeGap() { return m_paintParaInfo.recordTimeGap;}     //读取当前记录数据记录间隔
    quint64 getRecordTimeTag() { return m_paintParaInfo.recordTimeTag;}     //读取当前记录数据时间标记
    qint32  getRecordPointIndex() { return m_paintParaInfo.rightPointIndex;} //获取右侧数据索引值
//    qint32 getScreenHorDiv() { return m_paintParaInfo.screenHorDiv;}          //读取当前缓冲数据对应的屏幕水平分割数
    quint32 getTrendPeriodMax() { return m_paintParaInfo.trendPeriodMax;}   //趋势周期时间每格最大值
    quint32 getTrendPeriodMin() { return m_paintParaInfo.trendPeriodMin;}     //趋势周期时间每格最小值
    quint32 getTrendPeriodTarget() { return m_paintParaInfo.trendPeriodTarget;}     //当前趋势时间每格
    SQL_GROUP_INFO getGroupInforHead() { return m_paintParaInfo.sql_group;}    //读取当前缓冲数据对应的记录组信息块
    GROUP_INFO getGroupInfor(quint32 index) {//获取记录组详细信息块
        Q_ASSERT(index<50);
        return m_paintParaInfo.group_info[index];
    }
    DISPLAY_GONFIG_GROUP *getGroupConfig(quint32 grpNum);
    SQL_RECORD_CHAN_INFO getChannelInforHead() { return m_paintParaInfo.sql_channel;}  //读取记录通道信息块
    CHAN_INFO getChannelInfor(quint32 index) {//读取记录通道详细信息块
        Q_ASSERT(index<500);
        return m_paintParaInfo.chan_info[index];
    }
    SQL_SYS_CONFIG getSysConfig() { return m_paintParaInfo.sys_config;}                          //读取记录文件配置块
    bool getDisplayFlag() { return m_paintParaInfo.isDisplayData; }
    bool getHeadDataInFlag() { return m_paintParaInfo.isHeadIn;}
    bool getTailDataInFlag() { return m_paintParaInfo.isTailIn;}

    qint32 getChannelIndex(quint32 chanNo);                         //返回通道编号所指向对象所在数组索引
    qint32 getGroupIndex(quint32 groupNo);                         //返回组编号所指向对象所在数组索引

    //标尺生成
    void produceRuler();
    const QMap<quint32, CHANNEL_RULER> *getRulerPaintItem() const {return &m_rulerPaint;}

private:
    QMutex m_mutexData;                                 //用来保护多线程访问
    int m_bufSize_max, m_chanNum_max;       //分别对应缓冲数据最大长度和每个数据点上的通道最大个数
    QQueue<TREND_POINT> m_dataBuf;          //数据点缓冲FIFO
    QQueue<HAND_MSG>     m_msgBuf;          //手写消息缓冲FIFO
    TREND_PAINT_INFO m_paintParaInfo;       //缓冲数据对应的配置信息
    QMap<quint32 ,CHANNEL_RULER> m_rulerPaint; //缓冲数据对应的标尺信息
};

#endif // CIRCLEBUF_H
