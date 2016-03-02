#ifndef CFGCONFIG_SYSTEM_H
#define CFGCONFIG_SYSTEM_H

#include <QObject>
#include "./GlobalData/configobject.h"
#include "./GlobalData/cfgGlobalDef_SysEnvironment.h"

/***********************************************  system setting 22700 - 22899  ********************************************/
#define System_Start_ID                                         22700
#define System_End_ID                                           22899

#define System_Env_Start_ID                                      22700
#define System_Env_End_ID                                        22719

#define System_Alarm_Start_ID                                      22720
#define System_Alarm_End_ID                                        22729

#define System_Time_Start_ID                                      22730
#define System_Time_End_ID                                        22749

#define System_Snd_Start_ID                                      22750
#define System_Snd_End_ID                                        22759

#define System_Relay_Start_ID                                      22760
#define System_Relay_End_ID                                        22769

#define System_Switch_Start_ID                                      22770
#define System_Switch_End_ID                                        22779

#define System_Printer_Start_ID                                      22780
#define System_Printer_End_ID                                        22789

#define System_DevAndCom_Start_ID                                      22790
#define System_DevAndCom_End_ID                                        22820

//system
#define ID_System                                               22700

//system env
#define ID_System_Env                                           22701   //title

#define ID_System_Env_Language                                  22702   //语言类型
#define ID_System_Env_Temp                                      22703   //温度单位
#define ID_System_Env_PointType                                 22704   //小数点表示

#define ID_System_Env_Date                                      22710
#define ID_System_Env_Date_Format                               22711   //日期格式
#define ID_System_Env_Date_delimiter                            22712   //分割符类型
#define ID_System_Env_Date_Mon                                  22713   //月份表示方式

//system alarm
#define ID_System_Alarm                                         22720   //title

#define ID_System_Alarm_Rate                                    22721   //group
#define ID_System_Alarm_Rate_Dec                                22722   //变化率报警：下降数据数，范围：1~32, 实际间隔 = 数据数 * 测量周期
#define ID_System_Alarm_Rate_Inc                                22723   //变化率报警：上升数据数，范围：1~32,

#define ID_System_Alarm_Ind                                     22725   //报警指示
#define ID_System_Alarm_Ind_Value                               22726

#define ID_System_Alarm_ACK                                     22728
#define ID_System_Alarm_ACK_Switch                              22729   //单个报警解除开关

//system time
#define ID_System_Time                                          22730
#define ID_System_Time_Zone                                     22731   //group
#define ID_System_Time_H                                        22732
#define ID_System_Time_m                                        22733

//system snd
#define ID_System_Snd                                           22750
#define ID_System_Snd_Snd                                       22751   //group
#define ID_System_Snd_Snd_Touch                                 22752   //触摸音开关
#define ID_System_Snd_Snd_Warning                               22753   //警告音开关

#define ID_System_Snd_LED                                       22755   //group
#define ID_System_Snd_LED_LED                                   22756   //MENU按键LED开关，off/报警

//system relay
#define ID_System_Relay                                         22760
#define ID_System_Relay_OutputMode                              22761   //继电器输出模式
#define ID_System_Relay_MemMediaStatus                          22762   //内存/外部存储器异常， 状态输出模式时有效，下同！
#define ID_System_Relay_Measure                                 22763   //测量错误
#define ID_System_Relay_Comm                                    22764   //通信错误
#define ID_System_Relay_Record                                  22765    //记录停止
#define ID_System_Relay_Alarm                                   22766   //报警

//system inter switch
#define ID_System_Switch                                        22770
#define ID_System_Switch_FirNum                                 22771   //
#define ID_System_Switch_LastNum                                22772

#define ID_System_Switch_Switch                                 22773
#define ID_System_Switch_Switch_Mode                            22774   //内部开关模式，报警/手动
#define ID_System_Switch_Switch_Operate                         22775   //内部开关操作，与/或

//system printer
#define ID_System_Printer                                       22780
#define ID_System_Printer_IP                                    22781   //打印机IP地址，IPV4
#define ID_System_Printer_Size                                  22782   //纸张大小
#define ID_System_Printer_Orientation                           22783   //打印方向
#define ID_System_Printer_DPI                                   22784    //打印精度
#define ID_System_Printer_Num                                   22785   //打印份数，范围：1~10

//system device
#define ID_System_Device                                        22790
#define ID_System_Device_Tag                                    22791   //设备标签，最多32个半角字符，中英文支持
#define ID_System_Device_TagNo                                  22792   //设备标签号，最多16个半角字符

//system comment
#define ID_System_Comment                                       22800
#define ID_System_Comment_Comment                               22801   //配置注释，最多50个半角字符，中英文支持

//system status
#define ID_System_Status                                        22810
#define ID_System_Status_MainUnit                               22811   //本机板卡（模块）状态
#define ID_System_Status_ExtUnit                                22812   //扩展单元板卡（模块）状态


class CfgConfig_System : public QObject
{
    Q_OBJECT
public:
    static CfgConfig_System * instance();

    QVariant getSystemData(quint32 ID, quint32 group);
    void setSystemData(quint32 ID, quint32 group);

    QString getSystemObjStr(quint32 ID, quint32 group = 0);
    QString getSystemObjValStr(quint32 ID, quint32 group, ConfigObject * cfg);

    void setDeviceSysConfig(SYS_CONFIG_ENVRIONMENT & envConfig) { p_envConfig = &envConfig;}

private:
    explicit CfgConfig_System(QObject *parent = 0);

    QVariant getSystemEnvData(quint32 ID, quint32 group);
    QVariant getSystemAlarmData(quint32 ID, quint32 group);
    QVariant getSystemTimeData(quint32 ID, quint32 group);
    QVariant getSystemSndData(quint32 ID, quint32 group);
    QVariant getSystemRelayData(quint32 ID, quint32 group);
    QVariant getSystemSwitchData(quint32 ID, quint32 group);
    QVariant getSystemPrintData(quint32 ID, quint32 group);
    QVariant getSystemDevAndComData(quint32 ID, quint32 group);

    QString getSystemEnvObjStr(quint32 ID, quint32 group = 0);
    QString getSystemAlarmObjStr(quint32 ID, quint32 group = 0);
    QString getSystemTimeObjStr(quint32 ID, quint32 group = 0);
    QString getSystemSndObjStr(quint32 ID, quint32 group = 0);
    QString getSystemRelayObjStr(quint32 ID, quint32 group = 0);
    QString getSystemSwitchObjStr(quint32 ID, quint32 group = 0);
    QString getSystemPrintObjStr(quint32 ID, quint32 group = 0);
    QString getSystemDevAndComObjStr(quint32 ID, quint32 group = 0);

    QString getSystemEnvObjValStr(quint32 ID, quint32 group, ConfigObject *cfg);
    QString getSystemAlarmObjValStr(quint32 ID, quint32 group, ConfigObject *cfg);
    QString getSystemTimeObjValStr(quint32 ID, quint32 group, ConfigObject *cfg);
    QString getSystemSndObjValStr(quint32 ID, quint32 group, ConfigObject *cfg);
    QString getSystemRelayObjValStr(quint32 ID, quint32 group, ConfigObject *cfg);
    QString getSystemSwitchObjValStr(quint32 ID, quint32 group, ConfigObject *cfg);
    QString getSystemPrintObjValStr(quint32 ID, quint32 group, ConfigObject *cfg);
    QString getSystemDevAndComObjValStr(quint32 ID, quint32 group, ConfigObject *cfg);

    QString getSystemEnvLagObjValStr(quint32 type);


    void setSystemEnvData(quint32 ID, quint32 group);
    void setSystemAlarmData(quint32 ID, quint32 group);
    void setSystemTimeData(quint32 ID, quint32 group);
    void setSystemSndData(quint32 ID, quint32 group);
    void setSystemRelayData(quint32 ID, quint32 group);
    void setSystemSwitchData(quint32 ID, quint32 group);
    void setSystemPrintData(quint32 ID, quint32 group);
    void setSystemDevAndComData(quint32 ID, quint32 group);

    QVariant getCharToString(uchar *str);
    void setStrToChar(uchar * uData, QString str, int len);

signals:

public slots:

private:
    SYS_CONFIG_ENVRIONMENT *            p_envConfig;

};

#endif // CFGCONFIG_SYSTEM_H
