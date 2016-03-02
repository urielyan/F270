/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：channelview.h
 * 概   要：自定义model，用于通道选择界面
 *
 * 当前版本：V1.0.0
 * 作   者：葛 海 浪
 * 完成日期：2016-1-5
 *
 * 修改版本：
 * 修改说明：
 * 修改作者：
 *
 ********************************************************************/
#ifndef CHANNEMODEL_H
#define CHANNEMODEL_H

#define CHANNELNUMS 500                             // 定义通道数量的最大值
#include <QAbstractTableModel>

class ChanneModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit ChanneModel(QObject *parent = 0);
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    void setChannums(int nums);
    void setChannelType(int Type);
    void setChannelListName(const QMap<QString, quint32>  &map);
    void setBeginID(int id);        // 设置当前通道的起始ID
    int getSelectNums();            //  获取当前已经选择的通道数
    void initiaulzation();
signals:

public slots:

private:
    enum {
        ChannelNameRole = 0,
        ChannelPressRole,
        ChannelClickedRole,
        ChannelSequnceRole,
        ChannelType
    };

    int m_channelNums;          // 通道总数
    int m_channulCount;         // 当前已经选择的通道数
    int m_currentNum[CHANNELNUMS];
    bool m_PressStatus[CHANNELNUMS];
    bool m_ClickeStatus[CHANNELNUMS];
    int m_channelType;
    QList<QString> m_ChannelName;
};

#endif // CHANNEMODEL_H
