
#ifndef CFGGLOBALDEF_TREND_H
#define CFGGLOBALDEF_TREND_H
#include <QtGlobal>
#include <QPointF>
#include <QMap>
#include <QImage>
/*标尺绘制参数数据结构*/
typedef struct CHANNEL_RULER_STRUCT{
    int red;
    int green;
    int blue;                                                                  //标记颜色
    uchar   displayTagChar[34];                                 //标记字符串，中英支持，最多32个半角字符
    uchar   displayTagNo[18];                                    //标记号码，符号，最多16个半角字符

    quint8  displayZoneUpper;                                  //显示区域上限，5-100,单位%
    quint8  displayZoneLower;                                  //显示区域上限，0-95,单位%

    quint8  displayScalePos;                                     //标尺位置，1-10,默认1
    quint8  displayScaleDiv;                                      //标尺分割，4-13, 13:C10

    //趋势设定的部分压缩放大功能打开时有效
    bool  displayPartialOnOff;                                  //部分压缩放大开关
    quint8  displayPartialPos;                                   //部分压缩放大位置，1-99, 单位：%，默认50
    float  displayPartialBoundary;                           //部分压缩放大边界值

    qreal rangeTop;                                                    //量程上限
    qreal rangeBottom;                                             //量程下限

    quint32 colorBandMode;                                     //彩带模式， 0-off, 1-内侧， 2-外侧.
    quint32 colorBandColor;                                      //彩带颜色，Byte2-Red, Byte1-Green, Byte0-Blue
    float       colorBandPosUpper;                              //彩带上限位置
    float       colorBandPosLower;                              //彩带下限位置

    quint32 alarmMark;                                              //报警标记开关，0-off, 1-on
    quint32 alarmMode;                                             //报警标记模式，0-报警， 1-固定
    quint32 alarmColor[4];                                         //报警1-4标记颜色，Byte2-Red, Byte1-Green, Byte0-Blue
    quint32 alarmOnOff[4];                                       //报警1-4开关
    float alarmValue[4];                                             //报警1-4报警值
} CHANNEL_RULER;

/*趋势缓存数据节点数据结构*/
typedef struct TREND_POINT_STRUCT {
    qint64 timePoint;                                                //数据时间点
    QMap<quint32, QVector3D> valuePoint;              //数据点, <chanCoder, data>
    QMap<quint32, QPair<quint8, QString>> msgInfor;   //消息 char消息，一个时间点最多10个<point(1~10), <grpIndex, string>>
}TREND_POINT;
/*趋势手写信息绘制缓存节点结构*/
typedef struct HAND_MSG_STRUCT{
    quint32    msgGroupInfor;                                //消息组信息
    QImage * msgImage;                                        //消息图片
    qreal         imageYPos;                                       //图片y轴坐标
}HAND_MSG;

#endif // CFGGLOBALDEF_TREND_H
