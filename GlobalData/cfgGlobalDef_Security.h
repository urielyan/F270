/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：cfgGlobalDef_Security.h
 * 概   要：安全设定配置相关数据结构体、枚举定义
 *
 * 当前版本：V1.0.0
 * 作   者：王 品
 * 完成日期：2015-6-9
 *
 * 修改版本：
 * 修改说明：
 * 修改作者：
 *
 ********************************************************************/
#include <QtGlobal>

#include "cfgGlobalDef_Type.h"

#ifndef CFGGLOBALDEF_SECURITY_H
#define CFGGLOBALDEF_SECURITY_H

#define SECURITY_MAX_USERGROUP      51
#define SECURITY_MAX_MODULEGROUP    11

typedef enum  Security_TouchOpe_Enum {
    Security_TouchOpe_Off = 0,      //关闭
    Security_TouchOpe_Login,        //登陆
    Security_TouchOpe_Lock,         //操作锁定
}Security_TouchOpe ;

typedef enum  Security_Comm_Enum {
    Security_Comm_Off = 0,      //关闭
    Security_Comm_Login,        //登陆
}Security_Comm ;

typedef enum Security_Auth_Enum {
    Security_Auth_Free = 0,
    Security_Auth_Lock,
}Security_Auth;

typedef enum  Security_Logout_Enum {
    Security_Logout_Off = 0,
    Security_Logout_1min = 1,
    Security_Logout_2min = 2,
    Security_Logout_5min = 5,
    Security_Logout_10min = 10,
}Security_Logout ;

typedef enum  Security_UserLvl_Enum {
    Security_UserLvl_Off = 0,
    Security_UserLvl_Admin,
    Security_UserLvl_User,
}Security_UserLvl ;

typedef enum  Security_UserMode_Enum {
    Security_UserMode_Touch = 0,
    Security_UserMode_Comm,
    Security_UserMode_Both,
}Security_UserMode ;

typedef enum Security_ModuleAuth_Enum{
    Security_ModuleAuth_Record = 0,//记录
    Security_ModuleAuth_Math,//运算
    Security_ModuleAuth_Datasave,//数据保存
    Security_ModuleAuth_Msg,//信息
    Security_ModuleAuth_Batch,//批处理
    Security_ModuleAuth_ACK,//解除报警
    Security_ModuleAuth_Comm,//通信
    Security_ModuleAuth_TouchOpe,//画面操作
    Security_ModuleAuth_Time,//时间设定
    Security_ModuleAuth_Setting,//设定操作
    Security_ModuleAuth_EXTMedia,//外部存储器
    Security_ModuleAuth_SysOpe,//系统操作
    Security_ModuleAuth_OutPutOpe//输出操作
}Security_ModuleAuth;

/****************************************
 * 基本项相关数据结构体定义
 ****************************************/
typedef struct SECURITY_CONFIG_BASIC_STRUCT
{
    Security_TouchOpe   securityTouchOpe;       //触屏操作，0-off，1-登录，2-操作锁定
    Security_Comm       securityComm;           //通信，0-off，1-登录

    Security_Logout     securityLogout;         //自动注销，0-off，60-1min，120-2min，300-5min，600-10min
    quint8              securityQuitOpe;        //退出时的操作，0-off，1-on
}SECURITY_CONFIG_BASIC ;

/****************************************
 * 用户注册相关数据结构体定义
 ****************************************/
typedef struct SECURITY_CONFIG_USERGROUP_STRUCT
{
    Security_UserLvl    securityUserLvl;        //用户级别，0-off，1-管理员，2-用户
    Security_UserMode    securityUserMode;      //模式，0-触屏，1-通信，2-触屏+通信
    uchar       securityUserName[20];           //用户名，最多20个半角字符，英文
    uchar       securityUserPwd[20];            //密码，最多20个半角字符，英文
    quint8      securityUserInitPwd;            //初始化密码，0-返回，1-初始化密码
    quint8      securityUserProperty;           //用户权限，0-off，1-on
    quint8      securityUserNum;                //用户权限号码，1~10
}SECURITY_CONFIG_USERGROUP ;

#define     Read_Module_Auth(prop, modId)    ((prop) >> modId & 0x01);      //获取模块对应的权限，参数 [prop:权限, modId:模块ID]，返回值[0-自由，1-锁定]
#define     Write_Module_Auth(prop,modId, val) ((prop) | (((val) & 0x01) << modId)); //设置模块对应的权限，参数[prop:权限, modId:模块ID,val:当前值(0-自由，1-锁定)]

#define     PROTY_Record(x)     ((x)<<Security_ModuleAuth_Record & 0x01)      //记录，0-自由，1-锁定，下同
#define     PROTY_Math(x)       ((x)<<Security_ModuleAuth_Math & 0x01)      //运算
#define     PROTY_Datasave(x)   ((x)<<Security_ModuleAuth_Datasave & 0x01)      //数据保存
#define     PROTY_Msg(x)        ((x)<<Security_ModuleAuth_Msg & 0x01)      //信息
#define     PROTY_Batch(x)      ((x)<<Security_ModuleAuth_Batch & 0x01)      //批处理
#define     PROTY_ACK(x)        ((x)<<Security_ModuleAuth_ACK & 0x01)      //解除报警
#define     PROTY_Comm(x)       ((x)<<Security_ModuleAuth_Comm & 0x01)       //通信
#define     PROTY_TouchOpe(x)   ((x)<<Security_ModuleAuth_TouchOpe & 0x01)       //画面操作
#define     PROTY_Time(x)       ((x)<<Security_ModuleAuth_Time & 0x01)       //时间设定
#define     PROTY_Setting(x)    ((x)<<Security_ModuleAuth_Setting & 0x01)       //设定操作
#define     PROTY_EXTMedia(x)   ((x)<<Security_ModuleAuth_EXTMedia & 0x01)       //外部存储器
#define     PROTY_SysOpe(x)     ((x)<<Security_ModuleAuth_SysOpe & 0x01)       //系统操作
#define     PROTY_OutputOPe(x)  ((x)<<Security_ModuleAuth_OutPutOpe & 0x01)       //输出操作

/****************************************
 * 安全设定配置相关数据结构体定义
 ****************************************/
typedef struct SECURITY_CONFIG_STRUCT
{
    SECURITY_CONFIG_BASIC       securityBasic;          //安全配置之基本配置

    SECURITY_CONFIG_USERGROUP   securityUserGrp[SECURITY_MAX_USERGROUP];
    /****************************************
     *securityUserGrp[51]:  [1]-[50]                    //安全配置之用户注册
     *                      [0].securityUserName = "TouchLock";       //安全配置之操作锁定时的用户名
     *                      [0].securityUserPwd  = "*********";       //安全配置之操作锁定时的密码
     *                      [0].other---unused
     ****************************************/

    quint16             securityProperty[SECURITY_MAX_MODULEGROUP];
    /****************************************
     *securityProperty[11]: [1]-[10]                    //安全配置之用户权限
     *                      [0]                         //安全配置之操作锁定时的限制项目(触屏操作锁定时)
     ****************************************/

}SECURITY_CONFIG ;

#endif // CFGGLOBALDEF_SECURITY_H
