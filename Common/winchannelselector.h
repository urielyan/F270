#ifndef WINCHANNELSELECTOR_H
#define WINCHANNELSELECTOR_H

#include <QScrollArea>
#include <QVBoxLayout>
#include <QButtonGroup>
#include <QScrollBar>
#include "winmenu.h"
#include "sysconfigslidingscreen.h"

class WinMenuGroup;
class WinChannelSelector;
class WinAIChanSelector;
class WinDIChanSelector;
class WinDOChanSelector;
class WinMahtChanSelector;
class WinCommChanSelector;
class WinChannelSelector : public QScrollArea
{
    Q_OBJECT
public:
    void showSelector(Menu *pMenu, WinMenuGroup *pGroup);

protected:
    WinChannelSelector(QWidget *parent = 0);
    virtual void reloadChanSelectMenu(){}
    virtual void reloadMenu(){}
    virtual void initMenu(){}
    void addMenu(Menu *menu);
    void RemoveMenu(Menu *menu);
    void RemoveMenus();

protected:
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void resizeEvent(QResizeEvent *);

public slots:
    void slotResetChanSelectMenus();
    void slotResetMenus();

private slots:
    void slotMenuClicked();

protected:
    Menu *p_pMenu;

private:
    int m_pos;
    bool m_drag;
    QVBoxLayout *p_vLayout;
    WinMenuGroup *p_pGroup;
};

class WinAIChanSelector : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinAIChanSelector *instance();

private:
    WinAIChanSelector(QWidget *parent = 0);
    virtual void reloadChanSelectMenu();

private:
    const QMap<QString, quint32>&m_map_AI;
};

class WinDIChanSelector : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinDIChanSelector *instance();

private:
    WinDIChanSelector(QWidget *parent = 0);
    virtual void reloadChanSelectMenu();

private:
    const QMap<QString, quint32>&m_map_DI;
};

class WinDOChanSelector : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinDOChanSelector *instance();

private:
    WinDOChanSelector(QWidget *parent = 0);
    virtual void reloadChanSelectMenu();

private:
    const QMap<QString, quint32>&m_map_DO;
};

class WinMathChanSelector : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinMathChanSelector *instance();

private:
    WinMathChanSelector(QWidget *parent = 0);
    virtual void reloadChanSelectMenu();

private:
    const QMap<QString, quint32>&m_map_Math;
};

class WinCommChanSelector : public WinChannelSelector
{
    Q_OBJECT
public:
    static WinCommChanSelector *instance();

private:
    WinCommChanSelector(QWidget *parent = 0);
    virtual void reloadChanSelectMenu();

private:
    const QMap<QString, quint32>&m_map_Comm;
};

#endif // WINCHANNELSELECTOR_H
