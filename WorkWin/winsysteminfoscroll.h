#ifndef WINSYSTEMINFOSCROLL_H
#define WINSYSTEMINFOSCROLL_H

#include <QLabel>
#include <QScrollArea>
#include "Common/slidingscreen.h"
#include "winsysteminfoframe.h"
#include "winsysteminfotableview.h"


//滑屏显示模式系统信息类
class WinSystemInfoPanel : public QFrame
{
    Q_OBJECT
public:
    WinSystemInfoPanel(QWidget *parent = 0);

private:
    QVBoxLayout m_layout;
    QLabel m_systemInfoTitle;
    WinSystemInfoFrame m_systemInfo;
};

//滑屏显示模式主单元信息类
class WinMainUnitInfoPanel : public QFrame
{
    Q_OBJECT
public:
    WinMainUnitInfoPanel(QWidget *parent = 0);

private:
    QVBoxLayout m_layout;
    QLabel m_mainUnitTitle;
    WinSystemUnitTableView m_mainUnit;
};

//滑屏显示模式类
class WinSystemInfoScroll : public QScrollArea
{
    Q_OBJECT
public:
    explicit WinSystemInfoScroll(QWidget *parent = 0);
    void showSystemInfo();
    void showUnitInfo();

protected:
    void resizeEvent(QResizeEvent *);

private:
    WinSystemInfoPanel m_systemInfo;
    WinMainUnitInfoPanel m_mainUnit;
};

#endif // WINSYSTEMINFOSCROLL_H
