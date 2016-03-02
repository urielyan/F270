#ifndef BOARDINTERFACE
#define BOARDINTERFACE

#include <QObject>

#include <modbus/modbus.h>

class BoardInterface
{
public:
    virtual ~BoardInterface() {}

    virtual bool getInputRegister(int addr, int nb, uint16_t *dest, int slave=-1) = 0;
    virtual void tcflushDataBus() = 0;

    virtual bool getHoldRegister(int addr, int nb, uint16_t *dest, int slave=-1) = 0;
    virtual bool setHoldRegister(int addr, int nb, uint16_t *src, int slave=-1) = 0;
    virtual void tcflushConfigBus() = 0;
};

#define BoardInterface_iid "Dahao.F270.BoardInterface"
Q_DECLARE_INTERFACE(BoardInterface, BoardInterface_iid)

#endif // BOARDINTERFACE

