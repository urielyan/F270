/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：CfgTrendGroup.h
 * 概   要：CfgTrendGroup是趋势模块下各个子模块共有配置类，包括子模块之间逻辑关系配置和
 * 趋势模块数据显示数据缓存。
 *
 * 当前版本：V1.0.0
 * 作   者：王 宸 博
 * 完成日期：2015-6-29
 *
 * 修改版本：
 * 修改说明：
 * 修改作者：
 *
 ********************************************************************/
#ifndef CFGTRENDGROUP_H
#define CFGTRENDGROUP_H

#include <QObject>
#include <QByteArray>
#include <QVector>
#include <QPointF>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMap>
#include <QHash>
#include <QSqlError>
#include <QLine>
#include <QSize>
#include <QDir>

#include "../GlobalData/cfgGlobalDef.h"
#include "sqlBlock_GroupInfo.h"
#include "sqlBlock_RecordChanInfo.h"
#include "Common/circlebuf.h"
#include "cfgGlobalDef.h"
#include "cfgchannel.h"
#include "sqlBlock_sysConfig.h"

#define TOP_SPACE  20
#define BOTTOM_SPACE 20

class CfgTrendGroup : public QObject
{
    Q_OBJECT
public:
    explicit CfgTrendGroup(QObject *parent = 0);
    ~CfgTrendGroup();

    /*********实时模式相关***********/
   void trendRecordStart();   //趋势开始记录

    /******************change start by wangpin *********************/
public:
    void switchToHistoryRecordMode(); //用于将当前实时模式切换到历史浏览模式
    void switchToHistoryRecordMode(bool isDisp, qint64 recordIndex, qint64 dataEndTime);    //切换到趋势，并流程历史记录
    void moveHistoryDataCursor(qint32 dataCount);  //历史浏览模式下，数据光标移动
    void moveToBufRight();  //历史浏览模式下，移动右侧光标至当前缓冲区末尾
    void moveToBufLeft();    //历史浏览模式下，移动右侧光标至当前缓冲区开头

    void startOverviewTrend();  //总体波形预览

    CircleBuf *getRealTimeTrendBuf() { return &m_trendDataBuf_realtime; }
    CircleBuf *getHistoryTrendBuf() { return &m_trendDataBuf_history;}
    CircleBuf *getOverviewTrendBuf() { return &m_trendDataBuf_history;}

    /*********配置查询*************/
    TrendWorkMode getCurrentWorkMode() { return m_workMode;}
    bool isSameIDofRealAndHistory() {
        if (m_trendDataBuf_realtime.getRecordIndex() == m_trendDataBuf_history.getRecordIndex() &&
             m_trendDataBuf_realtime.getDisplayFlag() == m_trendDataBuf_history.getDisplayFlag())
            return true;
        else
            return false;
    }
    quint32 getCurrentChanNo() { return m_currentChanNo;}
    QVector3D getCurrentRulerPointData(quint32 channelNumber);
    TrendRTState getCurrentRTState() { return m_rTState;}
    quint32 getCurrentGroupIndex() { return m_currentGroupIndex;}
    QString getHWritePictureSaveDir() { return m_hWriteCurrentDir;}     //趋势手写信息文件路径
    quint32 getHWritePictureIndex() { return m_hWriteIndex++;}            //趋势手写信息索引值
    QPointF getCurrentMaxMinValue(quint32 channelNumber);              //返回当前屏幕数据中，通道（channelNumber）对应的最大最小值

    /*********配置设置*************/
    void setWorkMode(TrendWorkMode mode) {
        if (m_workMode != mode)
            m_workMode = mode;
    }
    void setCurrentChanNo(quint32 chanNo) {m_currentChanNo = chanNo;}
    void setCurrentRulerPointDataIndex(int index);
    void setRTState(TrendRTState state) {
        m_rTState = state;
        if (Running == state &&DevConfigPtr->displayTrend.trendClear) {
            m_trendDataBuf_realtime.clearBuf();
        }
    }
    void setCurrentGroupIndex(quint32 groupIndex) { m_currentGroupIndex = groupIndex;}

signals:
    void sigHistoryPointerChanged();

private:
    CircleBuf m_trendDataBuf_realtime, m_trendDataBuf_history;
    TrendWorkMode m_workMode;         //整体趋势组工作模式
    quint32 m_currentChanNo;                 //当前选中的通道号
    int m_currentIndex;                              //当前标尺指向数据索引号，坐标从右->左（0～SCREEN_DATA_COUNT_MAX+1）
    TrendRTState m_rTState;                     //趋势实时工作模式状态
    quint32 m_currentGroupIndex;          //当前组号

    /***************手写信息相关数据结构***************************/
    QString m_hWriteCurrentDir;             //手写图片保存完整路径
    quint32 m_hWriteIndex;                      //手写图片索引，每次记录开始时被清0
};

#endif // CFGTRENDGROUP_H
