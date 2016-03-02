#ifndef WINSYSTEMINFOTABWIDGET_H
#define WINSYSTEMINFOTABWIDGET_H

#include <QVBoxLayout>
#include <QPushButton>
#include <QButtonGroup>
#include <QFrame>
#include <QStackedLayout>
#include "winsysteminfoframe.h"
#include "winsysteminfotableview.h"

//模块信息面板
class WinModuleInfoPanel : public QFrame
{
    Q_OBJECT
public:
    explicit WinModuleInfoPanel(QWidget *parent = 0);

public slots:
    void slotButtonClicked(int id);

private:
    QStackedLayout m_stackLayout;
    QVBoxLayout m_vLayout ;
    QHBoxLayout m_hLayout;

    WinSystemUnitTableView m_unitTable0;
    WinSystemUnitTableView m_unitTable1;
    WinSystemUnitTableView m_unitTable2;
    WinSystemUnitTableView m_unitTable3;
    WinSystemUnitTableView m_unitTable4;
    WinSystemUnitTableView m_unitTable5;
    WinSystemUnitTableView m_unitTable6;

    QButtonGroup m_btnGrp;
    QPushButton m_mainBtn;
    QPushButton m_unit1Btn ;
    QPushButton m_unit2Btn;
    QPushButton m_unit3Btn;
    QPushButton m_unit4Btn;
    QPushButton m_unit5Btn;
    QPushButton m_unit6Btn;
};
//系统信息标签显示模式类
class WinSystemInfoTabWidget : public QFrame
{
    Q_OBJECT
public:
    explicit WinSystemInfoTabWidget(QWidget *parent = 0);

signals:

private slots:
    void slotButtonClicked(int id);
    void slotSwtichUnit(int id);

private:
    QVBoxLayout m_vLayout;
    QHBoxLayout m_hLayout;

    QStackedLayout m_stackLayout;

    QButtonGroup m_btnGrp;
    QPushButton m_infoBtn;
    QPushButton m_unitBtn;
    QPushButton m_moduleBtn;
    WinModuleInfoPanel m_modulePanel;
    WinSystemInfoTableView m_unitPanel;
    WinSystemInfoFrame m_infoPanel;
};


#endif // WINSYSTEMINFOTABWIDGET_H
