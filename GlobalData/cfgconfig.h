#ifndef CFGCONFIG_H
#define CFGCONFIG_H

#include <QObject>
#include <QSharedMemory>

#include "cfgGlobalDef.h"
#include "dataGlobalDef_ModbusReg.h"
#include "configobject.h"
#include "./Common/devboard.h"

#include "cfgconfig_serial.h"
#include "cfgconfig_ai.h"
#include "cfgconfig_network.h"
#include "cfgconfig_do.h"
#include "cfgconfig_di.h"
#include "cfgconfig_comm.h"
#include "cfgconfig_display.h"
#include "cfgconfig_math.h"
#include "cfgconfig_system.h"
#include "cfgconfig_batch.h"
#include "cfgconfig_event.h"
#include "cfgconfig_report.h"
#include "cfgconfig_measure.h"
#include "cfgconfig_datasave.h"
#include "cfgconfig_record.h"
#include "cfgconfig_timer.h"
#include "cfgconfig_security.h"


class CfgConfig : public QObject
{
    Q_OBJECT
public:
    static CfgConfig * instance();
    void start() { qDebug("CfgConfig::instance create");}
    DEVICE_CONFIG *getDeviceConfig() {  return p_deviceConfig; }

    QString getObjStr(quint32 ID, quint32 group = 0);   //第二个参数先保留，目前没有用到

    QString getValObjectStr(quint32 ID, quint32 group, ConfigObject * cfg); //一个是从传过来的对象中得到相应的值，在必要的时候要使用相关联对象的路径

    QVariant getConfigData(quint32 ID, quint32 group);  //
    void setConfigData(quint32 ID, quint32 group);

    /****************板卡状态****************/
    quint16 getBoardStatus(quint8 unit, quint8 module);
    void setBoardStatus(quint8 unit, quint8 module, quint16 status);
    int getBoardSlaveAddr(quint8 unit, quint8 module);
    void clearBoardStatus(quint8 unit, quint8 module);
    bool isExtBoardValid();

    quint16 getBoardConfig(quint32 chanCoder);

    /****************仪表状态****************/
    void setDeviceStatus_Record(bool onoff);
    void setDeviceStatus_Math(bool onoff);
    void setDeviceStatus_UserLock(bool onoff);
    void setDeviceStatus_UserLogin(bool onoff);
    void setDeviceStatus_MemError(bool onoff);
    void setDeviceStatus_MeasureError(bool onoff);
    void setDeviceStatus_CommError(bool onoff);

    /*通道设置*/
    void resetChannelConfig(quint8 unit, quint8 module, quint8 channelNum, Channel_Type type);

    /*系统配置*/

private:
    explicit CfgConfig(QObject *parent = 0);

    void resetDefaultConfig();
    void resetDefaultConfig_AI(AI_CONFIG *pConfig, int index);
    void resetDefaultConfig_DI(DI_CONFIG *pConfig, int index);
    void resetDefaultConfig_DO(DO_CONFIG *pConfig, int index);
    void resetDefaultConfig_Math(MATH_CONFIG *pConfig, int index);
    void resetDefaultConfig_Comm(COMM_CONFIG *pConfig, int index);

signals:

public slots:

public:
    QSharedMemory m_sharedDataTable;    //共享内存对象，Modbus寄存器分配表
private:
    DEVICE_CONFIG *p_deviceConfig;      //设备配置结构体指针，指向LPSRAM

    //yan ge
public:

    //void storeConfigToBinaryFile();//将当前设备配置数据保存在/tmp/currentdeviceConfig中.

    //按下保存前和按下保存后的所有配置数据并比较两个文件然后打印不同的数据
    void storeDeviceConfig1();
    void storeDeviceConfig2();
    void compareFile();
    bool examineMem(DEVICE_CONFIG * p_examineMem);

private:
    inline void measureConfiguration(QTextStream &out);
    inline void recordConfiguration(QTextStream &out);

    inline void displayTrend(QTextStream &out);
    inline void displayGroup(QTextStream &out);
    inline void displayMessage(QTextStream &out);
    inline void displayBasic(QTextStream &out);

    inline void dataSaveConfig(QTextStream &out);
    inline void batchConfig(QTextStream &out);
    inline void reportConfig(QTextStream &out);
    inline void timerConfig(QTextStream &out);
    inline void eventActiveConfig(QTextStream &out);

    inline void networkConfiguration(QTextStream &out);

    inline void serialConfig(QTextStream &out);
    inline void systemConfig(QTextStream &out);
    inline void channelSetup_AI(QTextStream &out);
    inline void channelSetup_DI(QTextStream &out);
    inline void channelSetup_DO(QTextStream &out);
    inline void mathConnfig(QTextStream &out);
    inline void commConnfig(QTextStream &out);
    inline void caculateChannel_systemStatus(QTextStream &out);

    inline QString ucharArrayToQString(uchar *str);

    void mapDev_Mem();//将内存中数据映射出来以供使用

};

#endif // CFGCONFIG_H


