#include "qactionevent.h"

ActionEvent::ActionEvent(Type type, EVENT_OBJECT *pEvent):
    QEvent(type),
    m_event(*pEvent)
{

}

