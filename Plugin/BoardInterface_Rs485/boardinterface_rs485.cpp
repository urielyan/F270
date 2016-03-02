#include "boardinterface_rs485.h"

#include <QDebug>
#include <errno.h>

#define SERIAL_PORT_OF_SUBBOARD_DATA    "/dev/ttyPL0"
#define SERIAL_PORT_OF_SUBBOARD_STATUS "/dev/ttyPL1"
#define SERIAL_PORT_OF_BOARDRATE    1000000

#define BYTE_TIMEOUT    20000    //20ms
#define RESPONSE_TIMEOUT    20000    //20ms
#define DEBUG_FLAG  1

static modbus_t *s_ctx_data = 0;        //modbus数据通信上下文指针
static modbus_t *s_ctx_status = 0;      //modbus状态及配置上下文指针

bool BoardInterface_Rs485::getInputRegister(int addr, int nb, uint16_t *dest, int slave)
{
    if (!s_ctx_data) {
        s_ctx_data = modbus_new_rtu(SERIAL_PORT_OF_SUBBOARD_DATA, SERIAL_PORT_OF_BOARDRATE, 'N', 8, 1);

        if (NULL == s_ctx_data) {
            qDebug()<<"Error--modbus context null";
            return false;
        }

        if (-1 == modbus_connect(s_ctx_data)) {
            qDebug()<<"modbus connect failed:"<<modbus_strerror(errno);
            return false;
        }

        modbus_rtu_set_serial_mode(s_ctx_data, MODBUS_RTU_RS485);
        modbus_set_response_timeout(s_ctx_data, 0, RESPONSE_TIMEOUT);
        modbus_set_byte_timeout(s_ctx_data, 0, BYTE_TIMEOUT);
        modbus_set_debug(s_ctx_data, DEBUG_FLAG);
    }

    modbus_set_slave(s_ctx_data, slave);

    int rc = modbus_read_input_registers(s_ctx_data, addr, nb, dest);

    if (-1 == rc) {
        qDebug()<<"modbus read input reg failed:"<<modbus_strerror(errno);
        return false;
    } else {
        return true;
    }
}

void BoardInterface_Rs485::tcflushDataBus()
{
    if (s_ctx_data) {
        modbus_flush(s_ctx_data);
    } else {
        s_ctx_data = modbus_new_rtu(SERIAL_PORT_OF_SUBBOARD_DATA, SERIAL_PORT_OF_BOARDRATE, 'N', 8, 1);

        if (NULL == s_ctx_data) {
            qDebug()<<"Error--modbus context null";
            return;
        }

        if (-1 == modbus_connect(s_ctx_data)) {
            qDebug()<<"modbus connect failed:"<<modbus_strerror(errno);
            return;
        }

        modbus_rtu_set_serial_mode(s_ctx_data, MODBUS_RTU_RS485);
        modbus_set_response_timeout(s_ctx_data, 0, RESPONSE_TIMEOUT);
        modbus_set_byte_timeout(s_ctx_data, 0, BYTE_TIMEOUT);
        modbus_set_debug(s_ctx_data, DEBUG_FLAG);
    }
}

bool BoardInterface_Rs485::getHoldRegister(int addr, int nb, uint16_t *dest, int slave)
{
    if (!s_ctx_status) {
        s_ctx_status = modbus_new_rtu(SERIAL_PORT_OF_SUBBOARD_STATUS, SERIAL_PORT_OF_BOARDRATE, 'N', 8, 1);

        if (NULL == s_ctx_status) {
            qDebug()<<"Error--modbus context null";
            return false;
        }

        if (-1 == modbus_connect(s_ctx_status)) {
            qDebug()<<"modbus connect failed:"<<modbus_strerror(errno);
            return false;
        }

        modbus_rtu_set_serial_mode(s_ctx_status, MODBUS_RTU_RS485);
        modbus_set_response_timeout(s_ctx_status, 0, RESPONSE_TIMEOUT);
        modbus_set_byte_timeout(s_ctx_status, 0, BYTE_TIMEOUT);
        modbus_set_debug(s_ctx_status, DEBUG_FLAG);
    }

    modbus_set_slave(s_ctx_status, slave);

    int rc = modbus_read_registers(s_ctx_status, addr, nb, dest);

    if (-1 == rc) {
        qDebug()<<"modbus read hold reg failed:"<<modbus_strerror(errno);
        return false;
    } else {
        return true;
    }
}

bool BoardInterface_Rs485::setHoldRegister(int addr, int nb, uint16_t *src, int slave)
{
    if (!s_ctx_status) {
        s_ctx_status = modbus_new_rtu(SERIAL_PORT_OF_SUBBOARD_STATUS, SERIAL_PORT_OF_BOARDRATE, 'N', 8, 1);

        if (NULL == s_ctx_status) {
            qDebug()<<"Error--modbus context null";
            return false;
        }

        if (-1 == modbus_connect(s_ctx_status)) {
            qDebug()<<"modbus connect failed:"<<modbus_strerror(errno);
            return false;
        }

        modbus_rtu_set_serial_mode(s_ctx_status, MODBUS_RTU_RS485);
        modbus_set_response_timeout(s_ctx_status, 0, RESPONSE_TIMEOUT);
        modbus_set_byte_timeout(s_ctx_status, 0, BYTE_TIMEOUT);
        modbus_set_debug(s_ctx_status, DEBUG_FLAG);
    }

    modbus_set_slave(s_ctx_status, slave);

    int rc = modbus_write_registers(s_ctx_status, addr, nb, src);

    if (-1 == rc) {
        qDebug()<<"modbus set hold reg failed:"<<modbus_strerror(errno);
        return false;
    } else {
        return true;
    }
}

void BoardInterface_Rs485::tcflushConfigBus()
{
    if (s_ctx_status) {
        modbus_flush(s_ctx_status);
    } else {
        s_ctx_status = modbus_new_rtu(SERIAL_PORT_OF_SUBBOARD_STATUS, SERIAL_PORT_OF_BOARDRATE, 'N', 8, 1);

        if (NULL == s_ctx_status) {
            qDebug()<<"Error--modbus context null";
            return;
        }

        if (-1 == modbus_connect(s_ctx_status)) {
            qDebug()<<"modbus connect failed:"<<modbus_strerror(errno);
            return;
        }

        modbus_rtu_set_serial_mode(s_ctx_status, MODBUS_RTU_RS485);
        modbus_set_response_timeout(s_ctx_status, 0, RESPONSE_TIMEOUT);
        modbus_set_byte_timeout(s_ctx_status, 0, BYTE_TIMEOUT);
        modbus_set_debug(s_ctx_status, DEBUG_FLAG);
    }
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(BoardInterface_Rs485, BoardInterface_Rs485)
#endif // QT_VERSION < 0x050000
