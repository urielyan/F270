#ifndef WINROOT_SYSCONFIG_SYSTEM_H
#define WINROOT_SYSCONFIG_SYSTEM_H

#include <QWidget>
#include "winwidget.h"

class WinRoot_SysConfig_System : public WinWidget
{
    Q_OBJECT
public:
    explicit WinRoot_SysConfig_System(QWidget *parent = 0);

signals:

public slots:

public:
    QWidget *           p_rightWidget[3];


};

#endif // WINROOT_SYSCONFIG_SYSTEM_H
