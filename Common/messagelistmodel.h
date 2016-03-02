/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：MessageListModel.cpp
 * 概   要：信息显示接口的模型
 *
 * 当前版本：V1.0.0
 * 作   者：葛 海 浪
 * 完成日期：2016-2-22
 *
 * 修改版本：
 * 修改说明：
 * 修改作者：
 *
 ********************************************************************/
#ifndef MESSAGELISTMODEL_H
#define MESSAGELISTMODEL_H

#include <QAbstractTableModel>
#define MAXVALUE 100
class MessageListModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit MessageListModel(QObject *parent = 0);
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    /** 设置视图的行数  **/
    void setRowCount(int row);
    /** 设置显示的内容  **/
    void setMessage(int i, QString message);
    /** 获取第一行的索引号  **/
    QModelIndex getDefaultIndex();
    /** 初始化模型  **/
    void init();

private:
    enum {
        SequnceRole = 0,
        MessageRole,
        MousePressRole,
        MouseClickedRole
    };
private:
    int m_rowCount;
    QString m_strMessage[MAXVALUE];     // 存储信息内容
    bool m_mousePress[MAXVALUE];        // 存储鼠标下押状态
    bool m_mouseClicked[MAXVALUE];      // 存储鼠标单击状态
};

#endif // MESSAGELISTMODEL_H
