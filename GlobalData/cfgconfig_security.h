#ifndef CFGCONFIG_SECURITY_H
#define CFGCONFIG_SECURITY_H

#include <QObject>
#include <QVariant>
#include "./GlobalData/configobject.h"
#include "./cfgGlobalDef_Security.h"

/****************************************  security 24000 - 25000  *************************************************/

#define Security_Start_ID                               24000
#define Security_End_ID                                 24999

#define Security_Basic_Start_ID                         24000
#define Security_Basic_End_ID                           24019

#define Security_Ope_Start_ID                           24020
#define Security_Ope_End_ID                             24049

#define Security_UserSetting_Start_ID                   24050
#define Security_UserSetting_End_ID                     24069

#define Security_Property_Start_ID                      24070
#define Security_Property_End_ID                        24089



#define ID_Security                                     24000
//basic
#define ID_Security_Basic                               24001   //title
#define ID_Security_Basic_Function                      24002   //group
#define ID_Security_Basic_Function_Touch                24003   //normal 触屏操作，0-off，1-登录，2-操作锁定
#define ID_Security_Basic_Function_Comm                 24004   //group 通信，0-off，1-登录

#define ID_Security_Basic_Logout                        24010   //group
#define ID_Security_Basic_Logout_Auto                   24011   //自动注销，0-off，60-1min，120-2min，300-5min，600-10min
#define ID_Security_Basic_Logout_Quit                   24012   //退出时的操作，0-off，1-on

//limit
#define ID_Security_Ope                                 24020   //title

#define ID_Security_Ope_Function                        24021   //group
#define ID_Security_Ope_Function_Pwd                    24022   //密码，最多20个半角字符，英文

#define ID_Security_Ope_Limit                           24030   //group
#define ID_Security_Ope_Limit_Record                    24031   //记录，0-自由，1-锁定，下同
#define ID_Security_Ope_Limit_Math                      24032   //运算
#define ID_Security_Ope_Limit_DataSave                  24033   //数据保存
#define ID_Security_Ope_Limit_Msg                       24034   //信息
#define ID_Security_Ope_Limit_Batch                     24035   //批处理
#define ID_Security_Ope_Limit_ACK                       24036   //解除报警
#define ID_Security_Ope_Limit_Comm                      24037   //通信
#define ID_Security_Ope_Limit_TouchOpe                  24038   //画面操作
#define ID_Security_Ope_Limit_Time                      24039   //时间设定
#define ID_Security_Ope_Limit_Setting                   24040   //设定操作
#define ID_Security_Ope_Limit_ExtMedia                  24041   //外部存储器
#define ID_Security_Ope_Limit_SysOpe                    24042   //系统操作
#define ID_Security_Ope_Limit_OutputOpe                 24043   //输出操作


//user setting
#define ID_Security_UserSetting                         24050   //title

#define ID_Security_UserSetting_UserNum                 24051   //normal

#define ID_Security_UserSetting_UserSetting             24052   //group
#define ID_Security_UserSetting_UserSetting_Level       24053   //用户级别，0-off，1-管理员，2-用户
#define ID_Security_UserSetting_UserSetting_Mode        24054    //模式，0-触屏，1-通信，2-触屏+通信
#define ID_Security_UserSetting_UserSetting_Name        24055    //用户名，最多20个半角字符，英文
#define ID_Security_UserSetting_UserSetting_UserPwd     24056    //用户名，最多20个半角字符，英文
#define ID_Security_UserSetting_UserSetting_InitPwd     24057   //初始化密码，0-返回，1-初始化密码
#define ID_Security_UserSetting_UserSetting_Property    24058   //用户权限，0-off，1-on
#define ID_Security_UserSetting_UserSetting_Num         24059   //用户权限号码，1~10


//user property
#define ID_Security_UserProPerty                        24071  //title

#define ID_Security_UserProPerty_AuthNum                24072   //Normal

#define ID_Security_UserProPerty_ProPerty               24073   //group
#define ID_Security_UserProPerty_ProPerty_Record        24074   //记录，0-自由，1-锁定，下同
#define ID_Security_UserProPerty_ProPerty_Math          24075    //运算
#define ID_Security_UserProPerty_ProPerty_DataSave      24076   //数据保存
#define ID_Security_UserProPerty_ProPerty_Msg           24077   //信息
#define ID_Security_UserProPerty_ProPerty_Batch         24078   //批处理
#define ID_Security_UserProPerty_ProPerty_ACK           24079   //解除报警
#define ID_Security_UserProPerty_ProPerty_Comm          24080    //通信
#define ID_Security_UserProPerty_ProPerty_TouchOpe      24081   //画面操作
#define ID_Security_UserProPerty_ProPerty_Time          24082   //时间设定
#define ID_Security_UserProPerty_ProPerty_Setting       24083   //设定操作
#define ID_Security_UserProPerty_ProPerty_ExtMedia      24084   //外部存储器
#define ID_Security_UserProPerty_ProPerty_SysOpe        24085   //系统操作
#define ID_Security_UserProPerty_ProPerty_OutputOpe     24086   //输出操作

#define PROTY_Data(data, pos)                 ((data) >> (pos) & 0x01)

//先对相应的位置0,再设置相应的位
#define Set_PROTY_Data(data, bit ,pos)      {(data) &= (~(1 << pos));(data) |= ((bit)  << (pos));}

class CfgConfig_Security : public QObject
{
    Q_OBJECT
public:
    static CfgConfig_Security * instance();
    void setDeviceSecurityConfig(SECURITY_CONFIG & securityConfig) {p_securityConfig = &securityConfig;}

    QVariant getSecurityData(quint32 ID, quint32 group);      //get data
    QString getSecurityObjStr(quint32 ID, quint32 group = 0);         //get obj str
    QString getSecurityObjValStr(quint32 ID, quint32 group, ConfigObject * cfg);         //get obj val str
    void setSecurityData(quint32 ID, quint32 group);      //save data

private:
    explicit CfgConfig_Security(QObject *parent = 0);

    QVariant getSecurityBasicData(quint32 ID, quint32 group);
    QVariant getSecurityOperationData(quint32 ID, quint32 group);
    QVariant getSecurityUserSettingData(quint32 ID, quint32 group);
    QVariant getSecurityUserPropertyData(quint32 ID, quint32 group);

    QString getSecurityBasicObjStr(quint32 ID, quint32 group = 0);
    QString getSecurityOperationObjStr(quint32 ID, quint32 group = 0);
    QString getSecurityUserSettingObjStr(quint32 ID, quint32 group = 0);
    QString getSecurityUserPropertyObjStr(quint32 ID, quint32 group = 0);

    QString getSecurityBasicObjValStr(quint32 ID, quint32 group, ConfigObject * cfg);
    QString getSecurityOperationObjValStr(quint32 ID, quint32 group, ConfigObject * cfg);
    QString getSecurityUserSettingObjValStr(quint32 ID, quint32 group, ConfigObject * cfg);
    QString getSecurityUserPropertyObjValStr(quint32 ID, quint32 group, ConfigObject * cfg);

    void setSecurityBaiscData(quint32 ID, quint32 group);
    void setSecurityOperationData(quint32 ID, quint32 group);
    void setSecurityUserSettingData(quint32 ID, quint32 group);
    void setSecurityUserPropertyData(quint32 ID, quint32 group);



    QVariant getCharToString(uchar *str);
    //把QString类型转化为uchar *类型
    void setStrToChar(uchar * uData, QString str, int len);

private:
    SECURITY_CONFIG *            p_securityConfig;          //安全配置
};

#endif // CFGCONFIG_SERIAL_H
