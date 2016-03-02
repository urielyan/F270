#include "devgroup.h"

#include "GlobalData/cfgGlobalDef.h"

DevGroup::DevGroup(quint8 index, QObject *parent) :
    QObject(parent),
    m_index(index)
{
    Q_ASSERT(index<GROUP_NUM_MAX);
    m_pConfig = &DevConfigPtr->displayGroup[index];
}

