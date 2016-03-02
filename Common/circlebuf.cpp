#include "circlebuf.h"
#include <QDebug>
#include <QVector3D>

CircleBuf::CircleBuf(int bufsize, int chanNum, QObject *parent) :
    QObject(parent),
    m_bufSize_max(bufsize),
    m_chanNum_max(chanNum)
{
    ::memset(&m_paintParaInfo, 0, sizeof(TREND_PAINT_INFO));
}

CircleBuf::~CircleBuf()
{
}

int CircleBuf::preAppendInBuf(TREND_POINT &point, HAND_MSG &msg)
{
    if (point.valuePoint.isEmpty() || point.valuePoint.count() > m_chanNum_max)
        return 1;

    if (m_dataBuf.count() == m_bufSize_max) {
        m_dataBuf.removeLast();

        if (m_msgBuf.last().msgImage != nullptr) { //释放覆盖的图片指针
            delete m_msgBuf.last().msgImage;
            m_msgBuf.last().msgImage = nullptr;
        }
        m_msgBuf.removeLast();
    }

    m_dataBuf.prepend(point);
    m_msgBuf.prepend(msg);

    return 0;
}

int CircleBuf::appendInBuf(TREND_POINT &point, HAND_MSG &msg)
{
    if (point.valuePoint.isEmpty() || point.valuePoint.count() > m_chanNum_max)
        return 1;

    if (m_bufSize_max == m_dataBuf.count()) {
        this->setHeadDataInFlag(false);
        m_dataBuf.dequeue();

        if (m_msgBuf.first().msgImage != nullptr) {
            delete m_msgBuf.first().msgImage;
            m_msgBuf.first().msgImage = nullptr;
        }
        m_msgBuf.dequeue();
    }
    m_dataBuf.enqueue(point);
    m_msgBuf.enqueue(msg);

    return 0;
}

/*
 * 功能：
 *          历史浏览模式下，右侧数据索引值变更
 * 参数：
 *          qint32 dataCount：变更数据个数
 * 返回值：
 *          无
 */
void CircleBuf::moveDataCursor(qint32 dataCount)
{
    int validLen = m_dataBuf.count();
    qint32 curIndex = m_paintParaInfo.rightPointIndex;
    qint32 targetIndex = curIndex + dataCount;

    if (targetIndex <0) {
        targetIndex = 0;
    } else if (targetIndex >= validLen) {
        targetIndex = validLen;
    }

    setRecordPointIndex(targetIndex);
}

int CircleBuf::clearBuf()
{
    m_dataBuf.clear();
    foreach (HAND_MSG msg, m_msgBuf) {
        if (msg.msgImage != nullptr)
            delete msg.msgImage;
    }
    m_msgBuf.clear();

    m_rulerPaint.clear();
    ::memset(&m_paintParaInfo,0, sizeof(TREND_PAINT_INFO));
    return 0;
}

QPointF CircleBuf::getMaxMinValue(quint32 channelNumber)
{
    QPointF maxMin, temp;
    int rightPointIndex = m_paintParaInfo.rightPointIndex;
    int totalLen = m_dataBuf.count();

    //数据无效
    if (rightPointIndex<0 || rightPointIndex >= totalLen) {
        maxMin.setX(qQNaN());
        maxMin.setY(qQNaN());
        return maxMin;
    }
    int validLen = rightPointIndex>(SCREEN_DATA_COUNT_MAX+1)?(SCREEN_DATA_COUNT_MAX+1):rightPointIndex;
    maxMin.setX(m_dataBuf.at(rightPointIndex).valuePoint.value(channelNumber).x());
    maxMin.setY(m_dataBuf.at(rightPointIndex).valuePoint.value(channelNumber).y());
    rightPointIndex--;
    for (int i = 1; i<validLen; ++i, --rightPointIndex) {
        temp.setX(m_dataBuf.at(rightPointIndex).valuePoint.value(channelNumber).x());
        temp.setY(m_dataBuf.at(rightPointIndex).valuePoint.value(channelNumber).y());
        if (temp.x() > maxMin.x())
            maxMin.setX(temp.x());
        if (temp.y() < maxMin.y())
            maxMin.setY(temp.y());
    }
    return maxMin;
}

qint32 CircleBuf::getChannelIndex(quint32 chanNo)
{
    for (quint32 i = 0; i < m_paintParaInfo.sql_channel.channelNum; ++i) {
        if (m_paintParaInfo.chan_info[i].chanNum == chanNo)
            return i;
    }

    return -1;
}

qint32 CircleBuf::getGroupIndex(quint32 groupNo)
{
    for (quint32 i = 0; i < m_paintParaInfo.sql_group.groupNum; ++i) {
        if (m_paintParaInfo.group_info[i].grpIndex == groupNo)
            return i;
    }
    return -1;
}

CircleBuf &CircleBuf::operator =(const CircleBuf &buf)
{
    m_bufSize_max = buf.m_bufSize_max;
    m_chanNum_max = buf.m_chanNum_max;
    m_dataBuf = buf.m_dataBuf;
    m_msgBuf = buf.m_msgBuf;
    m_paintParaInfo = buf.m_paintParaInfo;
    m_rulerPaint = buf.m_rulerPaint;

     return *this;
}

void CircleBuf::copyPaintPara(const CircleBuf &buf)
{
    m_paintParaInfo = buf.m_paintParaInfo;
}

void CircleBuf::produceRuler()
{
    CHANNEL_RULER ruler;
    CHAN_INFO  chanInfor;
    quint16 chanType;
    quint32 chanNo;  //临时通道号

    m_rulerPaint.clear();

    //取得系统配置参数
    quint32 recordChannelNum = getChannelInforHead().channelNum;

    //由组信息块中的通道号数组访问通道信息块并生成标尺信息向量
    for (quint32 i=0; i<recordChannelNum; ++i) {
        chanInfor = getChannelInfor(i); //获取通道信息块
        chanNo = chanInfor.chanNum;              //获取通道编码
        chanType = CHANNEL_TYPE(chanNo);   //解析通道类型
        switch(chanType){
        case Channel_Type_AI:   //AI类型通道
            if (AI_Range_Calculate_Disabled == chanInfor.channelConfig.chanAI.rangeConfig.rangeCalculateType) {
                ruler.rangeTop = chanInfor.channelConfig.chanAI.rangeConfig.rangeDispMax;
                ruler.rangeBottom = chanInfor.channelConfig.chanAI.rangeConfig.rangeDispMin;

            } else {
                ruler.rangeTop = chanInfor.channelConfig.chanAI.rangeConfig.rangeScale.scaleMax/
                        ::pow10(chanInfor.channelConfig.chanAI.rangeConfig.rangeScale.scalePosition);
                ruler.rangeBottom = chanInfor.channelConfig.chanAI.rangeConfig.rangeScale.scaleMin/
                        ::pow10(chanInfor.channelConfig.chanAI.rangeConfig.rangeScale.scalePosition);
            }

            ruler.blue = chanInfor.channelConfig.chanAI.displayConfig.displayColor & 0x000000FF;
            ruler.green = (chanInfor.channelConfig.chanAI.displayConfig.displayColor & 0x0000FF00)>>8;
            ruler.red = (chanInfor.channelConfig.chanAI.displayConfig.displayColor & 0x00FF0000)>>16;

            ruler.displayZoneUpper = chanInfor.channelConfig.chanAI.displayConfig.displayZoneUpper;
            ruler.displayZoneLower = chanInfor.channelConfig.chanAI.displayConfig.displayZoneLower;

            ruler.displayPartialBoundary = chanInfor.channelConfig.chanAI.displayConfig.displayPartialBoundary;
            ruler.displayPartialOnOff = chanInfor.channelConfig.chanAI.displayConfig.displayPartialOnOff;
            ruler.displayPartialPos = chanInfor.channelConfig.chanAI.displayConfig.displayPartialPos;

            ruler.displayScaleDiv = chanInfor.channelConfig.chanAI.displayConfig.displayScaleDiv;
            ruler.displayScalePos = chanInfor.channelConfig.chanAI.displayConfig.displayScalePos;

            ruler.colorBandMode = chanInfor.channelConfig.chanAI.displayConfig.displayColorbandMode;
            ruler.colorBandColor = chanInfor.channelConfig.chanAI.displayConfig.displayColorbandColor;
            ruler.colorBandPosLower = chanInfor.channelConfig.chanAI.displayConfig.displayColorbandPosLower;
            ruler.colorBandPosUpper = chanInfor.channelConfig.chanAI.displayConfig.displayColorbandPosUpper;

            ruler.alarmMark = chanInfor.channelConfig.chanAI.displayConfig.displayAlarmMarkOnOff;
            ruler.alarmMode = chanInfor.channelConfig.chanAI.displayConfig.displayAlarmMarkType;
            for (int i=0; i<4; ++i) {
                ruler.alarmColor[i] = chanInfor.channelConfig.chanAI.displayConfig.displayAlarmMarkColor[i];
                ruler.alarmOnOff[i] = chanInfor.channelConfig.chanAI.alarmConfig.alarmOnOff[i];
                ruler.alarmValue[i] = chanInfor.channelConfig.chanAI.alarmConfig.alarmValue[i];
            }

            ::memset(ruler.displayTagChar, 0, 34);
            ::memset(ruler.displayTagNo, 0, 18);
            ::memcpy(ruler.displayTagChar, chanInfor.channelConfig.chanAI.displayConfig.displayTagChar, 34);
            ::memcpy(ruler.displayTagNo, chanInfor.channelConfig.chanAI.displayConfig.displayTagNo, 18);

            break;
        case Channel_Type_DI:   //DI类型通道
            if (DI_Range_Calculate_Linear != chanInfor.channelConfig.chanDI.rangeConfig.calculateType) {
                ruler.rangeTop = chanInfor.channelConfig.chanDI.rangeConfig.rangeDispMax;
                ruler.rangeBottom = chanInfor.channelConfig.chanDI.rangeConfig.rangeDispMin;
            } else {
                ruler.rangeTop = chanInfor.channelConfig.chanDI.rangeConfig.scale.scaleMax/
                        ::pow10(chanInfor.channelConfig.chanDI.rangeConfig.scale.scalePosition);
                ruler.rangeBottom = chanInfor.channelConfig.chanDI.rangeConfig.scale.scaleMin/
                        ::pow10(chanInfor.channelConfig.chanDI.rangeConfig.scale.scalePosition);
            }

            ruler.blue = chanInfor.channelConfig.chanDI.displayConfig.displayColor & 0x000000FF;
            ruler.green = (chanInfor.channelConfig.chanDI.displayConfig.displayColor & 0x0000FF00)>>8;
            ruler.red = (chanInfor.channelConfig.chanDI.displayConfig.displayColor & 0x00FF0000)>>16;

            ruler.displayZoneUpper = chanInfor.channelConfig.chanDI.displayConfig.displayZoneUpper;
            ruler.displayZoneLower = chanInfor.channelConfig.chanDI.displayConfig.dispalyZoneLower;

            ruler.displayPartialBoundary = 0;
            ruler.displayPartialOnOff = false;
            ruler.displayPartialPos = 0;

//            ruler.displayScaleDiv = chanInfor.channelConfig.chanDI.displayConfig.displayScaleDiv;
            ruler.displayScalePos = chanInfor.channelConfig.chanDI.displayConfig.displayScalePos;
            if (qAbs(ruler.rangeTop - ruler.rangeBottom)>1.5)
                ruler.displayScaleDiv = 2;
            else
                ruler.displayScaleDiv = 1;

            ruler.alarmMark = chanInfor.channelConfig.chanDI.displayConfig.displayAlarmMarkOnOff;
            ruler.alarmMode = chanInfor.channelConfig.chanDI.displayConfig.displayAlarmMarkType;
            for (int i=0; i<4; ++i) {
                ruler.alarmColor[i] = chanInfor.channelConfig.chanDI.displayConfig.displayAlarmMarkColor[i];
                ruler.alarmOnOff[i] = chanInfor.channelConfig.chanDI.alarmConfig.alarmOnOff[i];
                ruler.alarmValue[i] = chanInfor.channelConfig.chanDI.alarmConfig.alarmValue[i];
            }

            ::memset(ruler.displayTagChar, 0, 34);
            ::memset(ruler.displayTagNo, 0, 18);
            ::memcpy(ruler.displayTagChar, chanInfor.channelConfig.chanDI.displayConfig.displayTagChar, 34);
            ::memcpy(ruler.displayTagNo, chanInfor.channelConfig.chanDI.displayConfig.displayTagNo, 18);

            break;
        case Channel_Type_DO:
            ruler.rangeTop = chanInfor.channelConfig.chanDO.rangeConfig.rangeDispMax;
            ruler.rangeBottom = chanInfor.channelConfig.chanDO.rangeConfig.rangeDispMin;

            ruler.blue = chanInfor.channelConfig.chanDO.displayConfig.displayColor & 0x000000FF;
            ruler.green = (chanInfor.channelConfig.chanDO.displayConfig.displayColor & 0x0000FF00)>>8;
            ruler.red = (chanInfor.channelConfig.chanDO.displayConfig.displayColor & 0x00FF0000)>>16;

            ruler.displayZoneUpper = chanInfor.channelConfig.chanDO.displayConfig.displayZoneUpper;
            ruler.displayZoneLower = chanInfor.channelConfig.chanDO.displayConfig.dispalyZoneLower;

            ruler.displayPartialBoundary = 0;
            ruler.displayPartialOnOff = false;
            ruler.displayPartialPos = 0;

            ruler.displayScaleDiv = 0;
            ruler.displayScalePos = chanInfor.channelConfig.chanDO.displayConfig.displayScalePos;

            ::memset(ruler.displayTagChar, 0, 34);
            ::memset(ruler.displayTagNo, 0, 18);
            ::memcpy(ruler.displayTagChar, chanInfor.channelConfig.chanDO.displayConfig.displayTagChar, 34);
            ::memcpy(ruler.displayTagNo, chanInfor.channelConfig.chanDO.displayConfig.displayTagNo, 18);

            break;
        case Channel_Type_Math:
            ruler.rangeTop = (qreal)chanInfor.channelConfig.chanMath.expressConfig.expMax/::pow10(chanInfor.channelConfig.chanMath.expressConfig.expPos);
            ruler.rangeBottom = (qreal)chanInfor.channelConfig.chanMath.expressConfig.expMin/::pow10(chanInfor.channelConfig.chanMath.expressConfig.expPos);

            ruler.blue = chanInfor.channelConfig.chanMath.displayConfig.displayColor & 0x000000FF;
            ruler.green = (chanInfor.channelConfig.chanMath.displayConfig.displayColor & 0x0000FF00)>>8;
            ruler.red = (chanInfor.channelConfig.chanMath.displayConfig.displayColor & 0x00FF0000)>>16;

            ruler.displayZoneUpper = chanInfor.channelConfig.chanMath.displayConfig.displayZoneUpper;
            ruler.displayZoneLower = chanInfor.channelConfig.chanMath.displayConfig.dispalyZoneLower;

            ruler.displayPartialBoundary = chanInfor.channelConfig.chanMath.displayConfig.displayPartialBoundary;
            ruler.displayPartialOnOff = chanInfor.channelConfig.chanMath.displayConfig.displayPartialOnOff;
            ruler.displayPartialPos = chanInfor.channelConfig.chanMath.displayConfig.displayPartialPos;

            ruler.displayScaleDiv = chanInfor.channelConfig.chanMath.displayConfig.displayScaleDiv;
            ruler.displayScalePos = chanInfor.channelConfig.chanMath.displayConfig.displayScalePos;

            ruler.colorBandMode = chanInfor.channelConfig.chanMath.displayConfig.displayColorbandMode;
            ruler.colorBandColor = chanInfor.channelConfig.chanMath.displayConfig.displayColorbandColor;
            ruler.colorBandPosLower = chanInfor.channelConfig.chanMath.displayConfig.displayColorbandPosLower;
            ruler.colorBandPosUpper = chanInfor.channelConfig.chanMath.displayConfig.displayColorbandPosUpper;

            ruler.alarmMark = chanInfor.channelConfig.chanMath.displayConfig.displayAlarmMarkOnOff;
            ruler.alarmMode = chanInfor.channelConfig.chanMath.displayConfig.displayAlarmMarkType;
            for (int i=0; i<4; ++i) {
                ruler.alarmColor[i] = chanInfor.channelConfig.chanMath.displayConfig.displayAlarmMarkColor[i];
                ruler.alarmOnOff[i] = chanInfor.channelConfig.chanMath.alarmConfig.alarmOnOff[i];
                ruler.alarmValue[i] = (float)chanInfor.channelConfig.chanMath.alarmConfig.alarmValue[i]/
                                                    ::pow10(chanInfor.channelConfig.chanMath.expressConfig.expPos);
            }

            ::memset(ruler.displayTagChar, 0, 34);
            ::memset(ruler.displayTagNo, 0, 18);
            ::memcpy(ruler.displayTagChar, chanInfor.channelConfig.chanMath.displayConfig.displayTagChar, 34);
            ::memcpy(ruler.displayTagNo, chanInfor.channelConfig.chanMath.displayConfig.displayTagNo, 18);

            break;
        case Channel_Type_Comm: //通信类型通道
            ruler.rangeTop = (qreal)chanInfor.channelConfig.chanComm.rangeConfig.rangeMax/::pow10(chanInfor.channelConfig.chanComm.rangeConfig.rangePos);
            ruler.rangeBottom = (qreal)chanInfor.channelConfig.chanComm.rangeConfig.rangeMin/::pow10(chanInfor.channelConfig.chanComm.rangeConfig.rangePos);

            ruler.blue = chanInfor.channelConfig.chanComm.displayConfig.displayColor & 0x000000FF;
            ruler.green = (chanInfor.channelConfig.chanComm.displayConfig.displayColor & 0x0000FF00)>>8;
            ruler.red = (chanInfor.channelConfig.chanComm.displayConfig.displayColor & 0x00FF0000)>>16;

            ruler.displayZoneUpper = chanInfor.channelConfig.chanComm.displayConfig.displayZoneUpper;
            ruler.displayZoneLower = chanInfor.channelConfig.chanComm.displayConfig.dispalyZoneLower;

            ruler.displayPartialBoundary = chanInfor.channelConfig.chanComm.displayConfig.displayPartialBoundary;
            ruler.displayPartialOnOff = chanInfor.channelConfig.chanComm.displayConfig.displayPartialOnOff;
            ruler.displayPartialPos = chanInfor.channelConfig.chanComm.displayConfig.displayPartialPos;

            ruler.displayScaleDiv = chanInfor.channelConfig.chanComm.displayConfig.displayScaleDiv;
            ruler.displayScalePos = chanInfor.channelConfig.chanComm.displayConfig.displayScalePos;

            ruler.colorBandMode = chanInfor.channelConfig.chanComm.displayConfig.displayColorbandMode;
            ruler.colorBandColor = chanInfor.channelConfig.chanComm.displayConfig.displayColorbandColor;
            ruler.colorBandPosLower = chanInfor.channelConfig.chanComm.displayConfig.displayColorbandPosLower;
            ruler.colorBandPosUpper = chanInfor.channelConfig.chanComm.displayConfig.displayColorbandPosUpper;

            ruler.alarmMark = chanInfor.channelConfig.chanComm.displayConfig.displayAlarmMarkOnOff;
            ruler.alarmMode = chanInfor.channelConfig.chanComm.displayConfig.displayAlarmMarkType;
            for (int i=0; i<4; ++i) {
                ruler.alarmColor[i] = chanInfor.channelConfig.chanComm.displayConfig.displayAlarmMarkColor[i];
                ruler.alarmOnOff[i] = chanInfor.channelConfig.chanComm.alarmConfig.alarmOnOff[i];
                ruler.alarmValue[i] = (float)chanInfor.channelConfig.chanComm.alarmConfig.alarmValue[i]/
                                                    ::pow10(chanInfor.channelConfig.chanComm.rangeConfig.rangePos);
            }

            ::memset(ruler.displayTagChar, 0, 34);
            ::memset(ruler.displayTagNo, 0, 18);
            ::memcpy(ruler.displayTagChar, chanInfor.channelConfig.chanComm.displayConfig.displayTagChar, 34);
            ::memcpy(ruler.displayTagNo, chanInfor.channelConfig.chanComm.displayConfig.displayTagNo, 18);

            break;
        default:
            break;
        }
        m_rulerPaint.insert(chanNo, ruler);
    }
}

DISPLAY_GONFIG_GROUP *CircleBuf::getGroupConfig(quint32 grpNum)
{
    quint32 grpTotal = m_paintParaInfo.sql_group.groupNum;
    for (quint32 i=0; i<grpTotal; ++i) {
        if (m_paintParaInfo.group_info[i].grpIndex == grpNum) {
            return &m_paintParaInfo.group_info[i].groupInfo;
        }
    }

    return NULL;
}
