#include "globalDef.h"
#include "cfgtrendgroup.h"
#include<QDateTime>
#include <QVector3D>
#include "../Threads/threaddatarecord.h"

#define DATA_BUF_SIZE_MAX   4096
#define DATA_BUF_CHANNUM_MAX 500
/*
 * 功能：
 * 	  构造函数
 * 参数：
 * 	  parent 父亲窗体指针
 * 返回值：
 * 	  无
 */
CfgTrendGroup::CfgTrendGroup(QObject *parent) :
    QObject(parent)
{
    m_currentIndex = 0;
    m_currentChanNo = -1;
    m_currentGroupIndex = 0;
    m_workMode = RealTimeRecord;
    m_rTState = StandBy;
    m_hWriteIndex = 0;
}

/*
 * 功能：
 * 	  析构函数
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
CfgTrendGroup::~CfgTrendGroup()
{
}

void CfgTrendGroup::startOverviewTrend()
{
    if (RealTimeRecord == m_workMode) {
        setWorkMode(BrowseHistory);
        switchToHistoryRecordMode();
    }
}

/*
 * 功能：
 * 	  设置标尺指向数据缓存数据点索引
 * 参数：
 *       无
 * 返回值：
 * 	  无
 */
void CfgTrendGroup::setCurrentRulerPointDataIndex(int index)
{
    if (index >(SCREEN_DATA_COUNT_MAX+1) || index <0) //默认取最后一点数据
        index = 0;

    m_currentIndex = index;
}

/*
 * 功能：
 * 	  返回当前标尺指向数据点数据
 * 参数：
 *       quint32 channelNumber 通道号
 * 返回值：
 * 	  无
 */
QVector3D CfgTrendGroup::getCurrentRulerPointData(quint32 channelNumber)
{

    if (m_workMode == RealTimeRecord ) {
        QVector3D markPoint;
        markPoint.setX(CfgChannel::instance()->getChannelData(channelNumber).toDouble());
        markPoint.setY(markPoint.x());
        markPoint.setZ(CfgChannel::instance()->getChannelStatus(channelNumber).toDouble());
        return markPoint;
    } else {
        qint32 rightPointIndex = m_trendDataBuf_history.getRecordPointIndex();
        qint32 validPointIndex = m_trendDataBuf_history.getBufValidLen();
        qint32 targetPointIndex = rightPointIndex - m_currentIndex;
        if (targetPointIndex>=0 && targetPointIndex<validPointIndex) {
            return m_trendDataBuf_history.getDataBuf().at(targetPointIndex).valuePoint.value(channelNumber);
        } else {
            return QVector3D();
        }
    }

}

/*
 * 功能：
 * 	  返回当前屏幕数据中，通道（channelNumber）对应的最大最小值
 * 参数：
 *       quint32 channelNumber 通道号
 * 返回值：
 * 	  无
 */
QPointF CfgTrendGroup::getCurrentMaxMinValue(quint32 channelNumber)
{
    return m_trendDataBuf_history.getMaxMinValue(channelNumber);
}

/*
 * 功能：
 * 	  开启趋势记录
 * 参数：
 *       无
 * 返回值：
 * 	  无
 */
void CfgTrendGroup::trendRecordStart()
{
    QDir trendRecordHandWrite;    //一次记录创建一个文件夹
    trendRecordHandWrite.mkdir(QString("./handWriteInfo/%1").
                               arg(QString::number(m_trendDataBuf_realtime.getRecordIndex())));

    m_hWriteCurrentDir = trendRecordHandWrite.absolutePath()+QString("/handWriteInfo/%1/").
            arg(QString::number(m_trendDataBuf_realtime.getRecordIndex()));
    m_hWriteIndex = 0;
}

/************************change start by wangpin*************************/

/*
 * 功能：
 * 	  趋势模块工作时， 响应用户界面动作引起的实时模式切换至浏览历史模式事件
 */
void CfgTrendGroup::switchToHistoryRecordMode()
{
    m_trendDataBuf_realtime.getDataLock();

    m_trendDataBuf_history = m_trendDataBuf_realtime;

    if (m_trendDataBuf_history.getBufValidLen()>0) {
        m_trendDataBuf_history.setRecordPointIndex(m_trendDataBuf_history.getBufValidLen() - 1);
    } else {
        m_trendDataBuf_history.setRecordPointIndex(0);
    }

    m_trendDataBuf_realtime.putDataLock();
}

/*
 * 功能：
 * 	  开始浏览任意时间刻度下趋势数据
 * 参数：
 *       1. bool isDisp   显示数据 or 事件数据
 *       2. qint64 recordIndex    记录ID
 *       3. qint64 dataEndTime    结束时间
 * 返回值：
 * 	  无
 */
void CfgTrendGroup::switchToHistoryRecordMode(bool isDisp, qint64 recordIndex, qint64 dataEndTime)
{
    emit(ThreadDataRecord::instance()->sigStartHistoryTrend(isDisp, recordIndex, dataEndTime));
}

/*
 * 功能：
 *      历史浏览模式下，屏幕最右测数据指针移动
 * 参数：
 *      1.qint32 dataCount：移动数据个数，dataCount>0 : 右移; dataCount<=0 : 左移
 * 返回值：
 *      无
 */
void CfgTrendGroup::moveHistoryDataCursor(qint32 dataCount)
{
    if (dataCount == 0)
        return;

    if (m_workMode == BrowseHistory) {
        qint32 validLen = m_trendDataBuf_history.getBufValidLen();
        qint32 pointCur = m_trendDataBuf_history.getRecordPointIndex();
        qint32 targetCur = pointCur + dataCount;

        if ((targetCur<0 && m_trendDataBuf_history.getHeadDataInFlag()) ||
            (targetCur>=validLen && m_trendDataBuf_history.getTailDataInFlag()))
            return;

        if (targetCur<0) {  //更新左侧历史数据缓冲
            if (!m_trendDataBuf_history.getHeadDataInFlag()) {
                emit(ThreadDataRecord::instance()->sigUpdateHistoryTrend(true));
                m_trendDataBuf_history.setTailDataInFlag(false);
            }

        } else if (targetCur>=validLen) {//更新右侧历史数据缓冲

            if (!m_trendDataBuf_history.getTailDataInFlag()) {
                emit(ThreadDataRecord::instance()->sigUpdateHistoryTrend(false));
                m_trendDataBuf_history.setHeadDataInFlag(false);
            }

        } else {
            m_trendDataBuf_history.setRecordPointIndex(targetCur);
        }

        emit(sigHistoryPointerChanged());
    }
}

void CfgTrendGroup::moveToBufRight()
{
    if (m_workMode == BrowseHistory) {
        if (!m_trendDataBuf_history.isEmpty())
            m_trendDataBuf_history.setRecordPointIndex(m_trendDataBuf_history.getBufValidLen()-1);
        else
            m_trendDataBuf_history.setRecordPointIndex(0);

        emit(sigHistoryPointerChanged());
    }
}

void CfgTrendGroup::moveToBufLeft()
{
    if (m_workMode == BrowseHistory) {
        if (m_trendDataBuf_history.getBufValidLen() > (SCREEN_DATA_COUNT_MAX+1))
            m_trendDataBuf_history.setRecordPointIndex(SCREEN_DATA_COUNT_MAX+1);
        else
            m_trendDataBuf_history.setRecordPointIndex(0);

        if (!m_trendDataBuf_history.isEmpty()) {
            if (m_trendDataBuf_history.getBufValidLen() > (SCREEN_DATA_COUNT_MAX+1))
                m_trendDataBuf_history.setRecordPointIndex(SCREEN_DATA_COUNT_MAX>>1);
            else
                m_trendDataBuf_history.setRecordPointIndex(m_trendDataBuf_history.getBufValidLen()-1);
        } else {
            m_trendDataBuf_history.setRecordPointIndex(0);
        }

        emit(sigHistoryPointerChanged());
    }
}

