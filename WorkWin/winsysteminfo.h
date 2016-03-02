#ifndef WINSYSTEMINFO_H
#define WINSYSTEMINFO_H

#include <QFrame>
#include <QStackedWidget>
#include <QLabel>
#include <QTableView>
#include <QPushButton>
#include "winsysteminfotabwidget.h"
#include "winsysteminfoscroll.h"

//系统信息底部操作面板
class WinSystemBottomPanel : public QFrame
{
    Q_OBJECT
public:
    explicit WinSystemBottomPanel(QWidget *parent = 0);

public slots:
    void slotHideInfoButton(bool flag);
    void slotHideReconfigButton(bool flag);

signals:
    void sigReconfig();
    void sigExit();
    void sigShowSystemInfo();
    void sigShowModuleInfo();

private:
    QHBoxLayout m_layout;
    QPushButton m_resetBtn;//重新配置按钮（执行跳转时有效）
    QPushButton m_systemBtn;//系统信息按钮（无扩展模块时有效）
    QPushButton m_moduleBtn;//模块信息按钮（无扩展模块时有效）
    QPushButton m_exitBtn;//退出按钮
};

//中间主面板
class WinSystemMiddlePanel : public QStackedWidget
{
    Q_OBJECT
public:
    explicit WinSystemMiddlePanel(QWidget *parent = 0);
    enum {
        Info_Scroll,
        Info_Table
    };

signals:
    void sigHideInfoButton(bool flag);//隐藏底部面板系统信息和模块信息按钮

public slots:
    void switchFrame(int id);//显示模式切换
    void slotShowSystemInfo();//显示系统信息窗体（无扩展模块有效）
    void slotShowModuleInfo();//显示模块信息窗体（无扩展模块有效）

private:
    WinSystemInfoScroll m_systemScroll;//滚动模式（无扩展模块）
    WinSystemInfoTabWidget m_systemTab;//标签切换模式（有扩展模块）
};


//系统信息模块
class WinSystemInfo : public QFrame
{
    Q_OBJECT
public:
    explicit WinSystemInfo(QWidget *parent = 0);

signals:
    void sigChangeFrame(quint32);

private slots:
    void slotReconfigure();//重新配置模块
    void slotQuit();//退出
    void slotConfigChanged();//系统配置信息变更

private:
    QVBoxLayout m_layout;
    WinSystemBottomPanel m_bottomPanel;//底部操作面板
    WinSystemMiddlePanel m_middlePanel;//中间主面板（包含两种不同显示模式）
};

#endif // WINSYSTEMINFO_H
