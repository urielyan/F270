#ifndef MODELBARGROUPCHANNEL_H
#define MODELBARGROUPCHANNEL_H

#include <QAbstractTableModel>
#include <QDebug>

class ModelBarGroupChannel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit ModelBarGroupChannel(quint32 grpIndex, QObject *parent = 0);
    int rowCount(const QModelIndex &parent = QModelIndex()) const ;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    void resetCurrentGroup(quint32 grpIndex);

private:
    void init();
    QVariant getChData(const int row, const int col) const;
    QVariant getChType(const int row, const int col) const;
    QVariant getChTag(const int row, const int col) const;
    QVariant getChColor(const int row, const int col) const;
    QVariant getChUnit(const int row, const int col) const;
    QVariant getChStatus(const int row, const int col) const;
    QVariant getChValidity(const int row, const int col) const;
    QVariant getChDataAccuracy(const int row, const int col) const;
    QVariant getChRangeUpper(const int row, const int col) const;
    QVariant getChRangeDowner(const int row, const int col) const;
    QVariant getChRangeType(const int row, const int col) const;

//    QVariant getChDispUpper(const int row, const int col) const;
//    QVariant getChDispDowner(const int row, const int col) const;

    QVariant getChScalePos(const int row, const int col) const;
    QVariant getChScaleDiv(const int row, const int col) const;

    QVariant getChBarBase(const int row, const int col) const;
    QVariant getChBarDiv(const int row, const int col) const;

//    QVariant getChPartScaleOnOff(const int row, const int col) const;
//    QVariant getChPartScalePos(const int row, const int col) const;
//    QVariant getChPartScaleBound(const int row, const int col) const;

    QVariant getChColorScalePos(const int row, const int col) const;
    QVariant getChColorScaleColor(const int row, const int col) const;
    QVariant getChColorScaleUpper(const int row, const int col) const;
    QVariant getChColorScaleDowner(const int row, const int col) const;

    QVariant getChAlarmOnOff(const int row, const int col) const;
    QVariant getChAlarmType(const int row, const int col) const;
    QVariant getChAlarm1OnOff(const int row, const int col) const;
    QVariant getChAlarm1Type(const int row, const int col) const;
    QVariant getChAlarm1Status(const int row, const int col) const;
    QVariant getChAlarm1Value(const int row, const int col) const;
    QVariant getChAlarm1Color(const int row, const int col) const;
    QVariant getChAlarm2OnOff(const int row, const int col) const;
    QVariant getChAlarm2Type(const int row, const int col) const;
    QVariant getChAlarm2Status(const int row, const int col) const;
    QVariant getChAlarm2Value(const int row, const int col) const;
    QVariant getChAlarm2Color(const int row, const int col) const;
    QVariant getChAlarm3OnOff(const int row, const int col) const;
    QVariant getChAlarm3Type(const int row, const int col) const;
    QVariant getChAlarm3Status(const int row, const int col) const;
    QVariant getChAlarm3Value(const int row, const int col) const;
    QVariant getChAlarm3Color(const int row, const int col) const;
    QVariant getChAlarm4OnOff(const int row, const int col) const;
    QVariant getChAlarm4Type(const int row, const int col) const;
    QVariant getChAlarm4Status(const int row, const int col) const;
    QVariant getChAlarm4Value(const int row, const int col) const;
    QVariant getChAlarm4Color(const int row, const int col) const;
signals:

public slots:
    void slotGroupChannelChanged(quint8 grpIndex);

private:
    quint32 m_grpIndex;                     //组索引号
    bool      m_grpValid;                       //组开关
    QList<quint32>  m_list_channel;  //组通道列表

};


class ModelHBarGroupChannel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit ModelHBarGroupChannel(quint32 grpIndex, QObject *parent = 0);
    int rowCount(const QModelIndex &parent = QModelIndex()) const ;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    void resetCurrentGroup(quint32 grpIndex);

private:
    void init();
    QVariant getChData(const int row, const int col) const;
    QVariant getChType(const int row, const int col) const;
    QVariant getChTag(const int row, const int col) const;
    QVariant getChColor(const int row, const int col) const;
    QVariant getChUnit(const int row, const int col) const;
    QVariant getChStatus(const int row, const int col) const;
    QVariant getChValidity(const int row, const int col) const;
    QVariant getChDataAccuracy(const int row, const int col) const;
    QVariant getChRangeUpper(const int row, const int col) const;
    QVariant getChRangeDowner(const int row, const int col) const;
    QVariant getChRangeType(const int row, const int col) const;

//    QVariant getChDispUpper(const int row, const int col) const;
//    QVariant getChDispDowner(const int row, const int col) const;

    QVariant getChScalePos(const int row, const int col) const;
    QVariant getChScaleDiv(const int row, const int col) const;

    QVariant getChBarBase(const int row, const int col) const;
    QVariant getChBarDiv(const int row, const int col) const;

//    QVariant getChPartScaleOnOff(const int row, const int col) const;
//    QVariant getChPartScalePos(const int row, const int col) const;
//    QVariant getChPartScaleBound(const int row, const int col) const;

    QVariant getChColorScalePos(const int row, const int col) const;
    QVariant getChColorScaleColor(const int row, const int col) const;
    QVariant getChColorScaleUpper(const int row, const int col) const;
    QVariant getChColorScaleDowner(const int row, const int col) const;

    QVariant getChAlarmOnOff(const int row, const int col) const;
    QVariant getChAlarmType(const int row, const int col) const;
    QVariant getChAlarm1OnOff(const int row, const int col) const;
    QVariant getChAlarm1Type(const int row, const int col) const;
    QVariant getChAlarm1Status(const int row, const int col) const;
    QVariant getChAlarm1Value(const int row, const int col) const;
    QVariant getChAlarm1Color(const int row, const int col) const;
    QVariant getChAlarm2OnOff(const int row, const int col) const;
    QVariant getChAlarm2Type(const int row, const int col) const;
    QVariant getChAlarm2Status(const int row, const int col) const;
    QVariant getChAlarm2Value(const int row, const int col) const;
    QVariant getChAlarm2Color(const int row, const int col) const;
    QVariant getChAlarm3OnOff(const int row, const int col) const;
    QVariant getChAlarm3Type(const int row, const int col) const;
    QVariant getChAlarm3Status(const int row, const int col) const;
    QVariant getChAlarm3Value(const int row, const int col) const;
    QVariant getChAlarm3Color(const int row, const int col) const;
    QVariant getChAlarm4OnOff(const int row, const int col) const;
    QVariant getChAlarm4Type(const int row, const int col) const;
    QVariant getChAlarm4Status(const int row, const int col) const;
    QVariant getChAlarm4Value(const int row, const int col) const;
    QVariant getChAlarm4Color(const int row, const int col) const;
signals:

public slots:
    void slotGroupChannelChanged(quint8 grpIndex);

private:
    quint32 m_grpIndex;                     //组索引号
    bool      m_grpValid;                       //组开关
    QList<quint32>  m_list_channel;  //组通道列表

};
#endif // MODELBARGROUPCHANNEL_H
