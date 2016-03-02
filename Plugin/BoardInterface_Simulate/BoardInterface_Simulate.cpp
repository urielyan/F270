#include "BoardInterface_Simulate.h"

#include <QDebug>

#define IP_ADDR "127.0.0.1"


bool BoardInterface_Simulate::getInputRegister(int addr, int nb, uint16_t *dest, int slave)
{
    Q_ASSERT(slave>0);
    modbus_t *ctx_test = modbus_new_tcp(IP_ADDR, 1502+((slave-1)<<1));
    if (NULL == ctx_test)
        return false;

    if ((-1 == modbus_connect(ctx_test))) {
        modbus_free(ctx_test);
        return false;
    }

    int rc = modbus_read_input_registers(ctx_test, addr, nb, dest);
    if (-1 == rc) {
        modbus_close(ctx_test);
        modbus_free(ctx_test);
        return false;
    }

    modbus_close(ctx_test);
    modbus_free(ctx_test);
    return true;
}

bool BoardInterface_Simulate::getHoldRegister(int addr, int nb, uint16_t *dest, int slave)
{
    Q_ASSERT(slave>0);
    modbus_t *ctx_test = modbus_new_tcp(IP_ADDR, 1503+((slave-1)<<1));
    if (NULL == ctx_test)
        return false;

    if ((-1 == modbus_connect(ctx_test))) {
        modbus_free(ctx_test);
        return false;
    }

    int rc = modbus_read_registers(ctx_test, addr, nb, dest);
    if (-1 == rc) {
        modbus_close(ctx_test);
        modbus_free(ctx_test);
        return false;
    }

    modbus_close(ctx_test);
    modbus_free(ctx_test);
    return true;
}

bool BoardInterface_Simulate::setHoldRegister(int addr, int nb, uint16_t *src, int slave)
{
    qDebug("BoardInterface_Simulate::setHoldRegister");
    return true;
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(BoardInterface_Simulate, BoardInterface_Simulate)
#endif // QT_VERSION < 0x050000
