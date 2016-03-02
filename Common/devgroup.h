#ifndef DEVGROUP_H
#define DEVGROUP_H

#include <QObject>
#include <QVariant>

#include "GlobalData/globalDef.h"

class DevGroup : public QObject
{
    Q_OBJECT
public:
    explicit DevGroup(quint8 index, QObject *parent = 0);

    quint8 getGroupIndex() const { return m_index;}
    bool getGroupOnOff() const { return m_pConfig->groupSwitch;}

signals:

public slots:

private:
    quint8 m_index;                                                 //组索引号0-49
    DISPLAY_GONFIG_GROUP *m_pConfig;           //组配置指针
};

#endif // DEVGROUP_H
