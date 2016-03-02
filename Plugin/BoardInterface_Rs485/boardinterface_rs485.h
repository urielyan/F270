#ifndef BOARDINTERFACE_RS485_H
#define BOARDINTERFACE_RS485_H

#include <QtPlugin>

#include "boardinterface.h"

class BoardInterface_Rs485 : public QObject, BoardInterface
{
    Q_OBJECT
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "Dahao.F270.BoardInterface" FILE "BoardInterface_Rs485.json")
    Q_INTERFACES(BoardInterface)
#endif // QT_VERSION >= 0x050000

public:
    bool getInputRegister(int addr, int nb, uint16_t *dest, int slave=-1);
    void tcflushDataBus();

    bool getHoldRegister(int addr, int nb, uint16_t *dest, int slave=-1);
    bool setHoldRegister(int addr, int nb, uint16_t *src, int slave=-1);
    void tcflushConfigBus();
};

#endif // BOARDINTERFACE_RS485_H
