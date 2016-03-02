#ifndef WINSYSTEMINFOTABLEVIEW_H
#define WINSYSTEMINFOTABLEVIEW_H

#include <QTableView>
#include "DataModel/modelsysteminformation.h"
#include "winsysteminfodelegate.h"

//系统信息面板表格
class WinSystemInfoTableView : public QTableView
{
    Q_OBJECT
public:
    explicit WinSystemInfoTableView(QWidget *parent = 0);

signals:
    void sigUnitSwitch(int id);

private slots:
    void slotItemClicked(QModelIndex);

private:
    ModelSystemInformation m_model;
    WinSystemInfoDelegate m_delegate;
};

//系统单元信息面板表格
class WinSystemUnitTableView : public QTableView
{
    Q_OBJECT
public:
    WinSystemUnitTableView(int unitIndex,QWidget *parent = 0);

signals:

private slots:
    void slotItemClicked(QModelIndex);

private:
    ModelSystemUnit m_model;
    WinSystemInfoDelegate m_tableDelege;
};

#endif // WINSYSTEMINFOTABLEVIEW_H
