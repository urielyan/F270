#include "authorization.h"
#include "logger.h"
Authority::Authority(quint8 authId, QObject *parent) :
    QObject(parent),m_authId(authId)
{
}

bool Authority::isLocked(quint8 moduleId) const
{
    QVariant value =CfgChannel::instance()->getSecurityModuleProperty(m_authId,moduleId);
    if(value.isValid()){
        return value.toUInt() == Security_Auth_Lock;
    }
    return false;
}

UserObject::UserObject(quint8 userId, QObject *parent) :
    QObject(parent)
{
    reset(userId);
}

quint8 UserObject::getUserType() const
{
    if(m_userId == 0)
        return Auth_UserType_Both;

    if(m_userId == SECURITY_MAX_USERGROUP || m_level == Security_UserLvl_Off)
        return Auth_UserType_Unknow;
    else if(m_level == Security_UserLvl_Admin)
        return Auth_UserType_Admin;
    else
        return Auth_UserType_User;
}

bool UserObject::isEnableAuth() const
{
    if(m_userId == SECURITY_MAX_USERGROUP)
        return false;
    else
        return m_prop == ON;
}

bool UserObject::changePassword(QString oldPasswd, QString newPasswd) const
{
    if(m_userId == SECURITY_MAX_USERGROUP)
        return false;

    if(oldPasswd == m_passwd){
        CfgChannel::instance()->setSecurityUserPassword(m_userId,newPasswd);
        return true;
    }
    return false;
}

bool UserObject::isLocked(quint8 moduleId) const
{
    if(m_userId == SECURITY_MAX_USERGROUP)
        return true;

    if(m_level == Security_UserLvl_Admin)//admin
        return false;

    if(m_prop == OFF)//权限关闭
        return false;

    return m_auth.isLocked(moduleId);
}

void UserObject::reset(quint8 userId)
{
    if((m_userId = userId) == SECURITY_MAX_USERGROUP){
        m_auth.m_authId = SECURITY_MAX_MODULEGROUP;
        return;
    }

    m_name = CfgChannel::instance()->getSecurityUserName(m_userId).toString();
    m_passwd = CfgChannel::instance()->getSecurityUserPassword(m_userId).toString();
    m_level = CfgChannel::instance()->getSecurityUserLevel(m_userId).toUInt();
    m_mode = CfgChannel::instance()->getSecurityUserMode(m_userId).toUInt();
    m_prop = CfgChannel::instance()->getSecurityUserPropertyOnOff(m_userId).toUInt();

    if(m_prop == ON){
        m_auth.m_authId  = CfgChannel::instance()->getSecurityUserPropertyNum(m_userId).toUInt();
    }else{
        m_auth.m_authId = SECURITY_MAX_MODULEGROUP;
    }
}

Authorization::Authorization(QObject *parent) :
    QObject(parent),m_loginStatus(0)
{
    //TODO 日志记录上一次状态
    connect(CfgChannel::instance(),SIGNAL(sigNormalConfigChanged(quint32)),this,SLOT(slotConfigChanged(quint32)));
    connect(&m_logoutTimer,SIGNAL(timeout()),this,SLOT(slotAutoLogout()));
    slotConfigChanged(ID_Security);
}

void Authorization::slotConfigChanged(quint32 id)
{
    if(id != ID_Security)
        return;
    if(m_logoutTimer.isActive())
        m_logoutTimer.stop();

    m_touchOpe = CfgChannel::instance()->getSecurityTouchOperation();
    m_autoLogout = CfgChannel::instance()->getSecurityAutoLogoutOnOff();
    m_quitOpe = CfgChannel::instance()->getSecurityQuitOpeOnOff();

    if(m_touchOpe == Security_TouchOpe_Login){//本次为用户登陆模式
        if(m_loginStatus == Auth_LoginStatus_Off){//上一次为登陆关闭状态
            m_loginStatus = Auth_LoginStatus_Logout;//切换到注销状态重新要求登陆
            m_user.reset(SECURITY_MAX_USERGROUP);//使用户无效
            emit sigAuthUpdate();
        }else if(m_loginStatus == Auth_LoginStatus_Login){//上一次为登陆状态
            if(m_user.m_userId == 0){//上一次为特殊用户
                m_loginStatus = Auth_LoginStatus_Logout;//切换到注销状态重新要求登陆
                m_user.reset(SECURITY_MAX_USERGROUP);//使用户无效
            }else{//上一次为管理员(普通用户没有配置权限)
                m_user.reset(m_user.m_userId);//更新用户配置
                if(m_user.m_mode != Security_UserMode_Touch || m_user.m_mode != Security_UserMode_Both || /*丢失触屏操作权限*/
                        m_user.m_level != Security_UserLvl_Admin){//用户级别丢失管理员权限
                    m_loginStatus = Auth_LoginStatus_Logout;//切换到注销状态重新要求登陆
                    EventLogger::getInstance().log(EventLogger::Event_Action_Logout,EventLogger::Event_Factor_OPERATE,getCurrentUserName());
                    m_user.reset(SECURITY_MAX_USERGROUP);//使用户无效
                }else{
                    if(m_autoLogout != Security_Logout_Off){//判断超时注销
                        m_logoutTimer.setInterval(m_autoLogout*1000*60);
                        m_logoutTimer.start();
                    }
                }
            }
            emit sigAuthUpdate();
        }//上一次为注销状态,不会出现配置变更
    }else if(m_touchOpe == Security_TouchOpe_Lock){//本次为操作锁定模式
        if(m_loginStatus == Auth_LoginStatus_Off){//上一次为登陆关闭状态
            m_loginStatus = Auth_LoginStatus_Login;//切换到登陆状态
            CfgChannel::instance()->setSecurityUserLevel(0,Security_UserLvl_User);//默认锁定状态
            m_user.reset(0);//更新用户配置
            emit sigAuthUpdate();
        }else if(m_loginStatus == Auth_LoginStatus_Login){//上一次为登陆状态
            if(m_user.m_userId == 0){//上一次为特殊用户
                m_user.reset(m_user.m_userId);//更新用户配置
                emit sigAuthUpdate();
            }else{//上一次为管理员或普通用户
                EventLogger::getInstance().log(EventLogger::Event_Action_Logout,EventLogger::Event_Factor_OPERATE,getCurrentUserName());
                CfgChannel::instance()->setSecurityUserLevel(0,Security_UserLvl_User);//默认锁定状态
                m_user.reset(0);//切换到特殊用户配置
                //emit sigAuthUpdate();
            }
        }//上一次为注销状态,不会出现配置变更
    }else{//本次为关闭登陆模式
        if(m_loginStatus == Auth_LoginStatus_Login && m_user.m_userId != 0){//上一次为普通用户或管理员用户登陆状态
            EventLogger::getInstance().log(EventLogger::Event_Action_Logout,EventLogger::Event_Factor_OPERATE,getCurrentUserName());
        }
        m_loginStatus = Auth_LoginStatus_Off;//初始化为登陆关闭状态
        m_user.reset(SECURITY_MAX_USERGROUP);//使用户无效
        emit sigAuthUpdate();
    }
}

void Authorization::slotAutoLogout()
{
    logout();
}

bool Authorization::authenticate(quint8 moduleId)//认证终端模块权限(只包含可配置锁定的模块)
{
    if(m_loginStatus == Auth_LoginStatus_Off){//登陆关闭状态,所有模块解锁
        return true;
    }else if(m_loginStatus == Auth_LoginStatus_Login){//登陆状态,根据用户权限设定
        return !m_user.isLocked(moduleId);//用户认证
    }else{//非登陆状态,注销可操作模块权限判断在其他接口中
       return false;
    }
}

bool Authorization::login(QString name, QString passwd)
{
    if(m_loginStatus == Auth_LoginStatus_Login){//登陆状态，特殊用户
        if(m_user.m_userId == 0 && m_user.m_passwd == passwd){
            m_user.m_level = Security_UserLvl_Admin;
            CfgChannel::instance()->setSecurityUserLevel(0,Security_UserLvl_Admin);
            emit sigAuthUpdate();
            return true;
        }else{
//            m_user.m_level = Security_UserLvl_User;
//            CfgChannel::instance()->setSecurityUserLevel(0,Security_UserLvl_User);
            return false;
        }
    }else if(m_loginStatus == Auth_LoginStatus_Logout){//注销状态，管理员或普通用户
        QVariant value = CfgChannel::instance()->findSecurityUserIdentification(name);
        if(value.isValid()){//有效用户
            m_user.reset(value.toUInt());
            if(passwd == m_user.m_passwd && /*密码正确*/
                    (m_user.m_mode == Security_UserMode_Touch || m_user.m_mode == Security_UserMode_Both) &&/*打开触屏模式权限*/
                    (m_user.m_level != Security_UserLvl_Off)){/*用户级别为普通用户或管理员*/
                m_loginStatus = Auth_LoginStatus_Login;//切换到登陆状态
                if(m_autoLogout != Security_Logout_Off){//判断超时注销
                    m_logoutTimer.setInterval(m_autoLogout*1000*60);
                    m_logoutTimer.start();
                }
                EventLogger::getInstance().log(EventLogger::Event_Action_Login,EventLogger::Event_Factor_OPERATE,getCurrentUserName());
                emit sigAuthUpdate();
                return true;
            }else{//密码错误或无登陆权限
                m_user.reset(SECURITY_MAX_USERGROUP);//使用户无效
                return false;
            }
        }else{//无效用户名
            return false;
        }
    }else{//关闭登陆状态
        return false;
    }
}

void Authorization::logout()
{
    if(m_logoutTimer.isActive())
        m_logoutTimer.stop();

    if(m_loginStatus == Auth_LoginStatus_Login){
        if(m_user.m_userId == 0){//特殊用户
            m_user.m_level = Security_UserLvl_User;
        }else{//管理员或普通用户
            EventLogger::getInstance().log(EventLogger::Event_Action_Logout,EventLogger::Event_Factor_OPERATE,getCurrentUserName());
            m_loginStatus = Auth_LoginStatus_Logout;
            m_user.reset(SECURITY_MAX_USERGROUP);
        }
        emit sigAuthUpdate();
    }
}

const UserObject* Authorization::getLoginedUser()//获取已登陆用户
{
    if(m_loginStatus == Auth_LoginStatus_Login){
        return &m_user;
    }else{
        return NULL;
    }
}

quint8 Authorization::getLoginStatus()//0-关闭登陆，1-登陆，2-无登陆
{
    return m_loginStatus;
}

bool Authorization::isApprovalOperation()//无登陆状态是否允许操作
{
    return m_quitOpe == ON;
}

QString Authorization::getCurrentUserName()
{
    if(m_loginStatus == Auth_LoginStatus_Login && m_user.m_userId != 0){
        return m_user.m_name;
    }
    return QString();
}

void Authorization::resetTimer()
{
    if(m_logoutTimer.isActive()){
        m_logoutTimer.stop();
        m_logoutTimer.start();
    }
}
