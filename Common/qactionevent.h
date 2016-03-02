#ifndef QACTIONEVENT_H
#define QACTIONEVENT_H

#include <QEvent>

#include "eventaction.h"

class ActionEvent : public QEvent
{
public:
    ActionEvent(Type type, EVENT_OBJECT *pEvent);
    inline EVENT_OBJECT *eventObj() { return &m_event; }

protected:
    EVENT_OBJECT m_event;
};

#endif // QACTIONEVENT_H
