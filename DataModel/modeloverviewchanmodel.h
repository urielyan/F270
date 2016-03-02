#ifndef MODELOVERVIEWCHANMODEL_H
#define MODELOVERVIEWCHANMODEL_H

#include <QAbstractTableModel>

class ModelOverviewChanModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit ModelOverviewChanModel(int frameIndex = 0,QObject *parent = 0);
    int rowCount(const QModelIndex &parent = QModelIndex()) const ;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

signals:

public slots:
    void slotChannelConfigChanged();//通道配置变更

private:
    void init();
    QVariant getChData(const int row, const int col) const;
    QVariant getChType(const int row, const int col) const;
    QVariant getChTag(const int row, const int col) const;
    QVariant getChUnit(const int row, const int col) const;
    QVariant getChStatus(const int row, const int col) const;
    QVariant getChValidity(const int row, const int col) const;
    QVariant getChDataAccuracy(const int row, const int col) const;

    QVariant getChAlarm1OnOff(const int row, const int col) const;
    QVariant getChAlarm1Type(const int row, const int col) const;
    QVariant getChAlarm1Status(const int row, const int col) const;
    QVariant getChAlarm2OnOff(const int row, const int col) const;
    QVariant getChAlarm2Type(const int row, const int col) const;
    QVariant getChAlarm2Status(const int row, const int col) const;
    QVariant getChAlarm3OnOff(const int row, const int col) const;
    QVariant getChAlarm3Type(const int row, const int col) const;
    QVariant getChAlarm3Status(const int row, const int col) const;
    QVariant getChAlarm4OnOff(const int row, const int col) const;
    QVariant getChAlarm4Type(const int row, const int col) const;
    QVariant getChAlarm4Status(const int row, const int col) const;

private:
    int m_frameIndex;//画面编号。从０开始编号，一个画面最多显示100个通道。
    QList<quint32> m_list_channel;
};

#endif // MODELOVERVIEWCHANMODEL_H
