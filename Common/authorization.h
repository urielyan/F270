#ifndef AUTHORIZATION_H
#define AUTHORIZATION_H

#include <QObject>
#include <QTimer>
#include "GlobalData/cfgchannel.h"
#include "GlobalData/cfgGlobalDef.h"
#include "logger.h"

class Authority : public QObject
{
    Q_OBJECT
    friend class UserObject;

public:
    bool isLocked(quint8 moduleId) const;
    Authority(quint8 authId = SECURITY_MAX_MODULEGROUP, QObject *parent = 0);

private:
    quint8 m_authId;
};

class UserObject : public QObject
{
    Q_OBJECT
    friend class Authorization;

public:
    typedef enum Auth_UserType_Enum{
        Auth_UserType_Admin = 0,//管理员用户
        Auth_UserType_User,//普通用户
        Auth_UserType_Both,//特殊用户（锁定-普通用户权限, 解锁-管理员权限）
        Auth_UserType_Unknow //不识别类型
    }Auth_UserType;

    quint8 getUserType() const;//0-admin,1-user,2-both,3-unknow
    quint8 getUserLevel() const {return m_level;}//0-off,1-admin,2-user

    bool changePassword(QString oldPasswd, QString newPasswd) const;
    bool isLocked(quint8 moduleId) const;
    bool isEnableAuth() const;//user_type为user时权限是否开启


private:
    explicit UserObject(quint8 userId = SECURITY_MAX_USERGROUP, QObject *parent = 0);
    void reset(quint8 userId);

private:
    quint8 m_userId;
    quint8 m_level;
    quint8 m_mode;
    quint8 m_prop;
    QString m_name;
    QString m_passwd;
    Authority m_auth;
};

class Authorization : public QObject
{
    Q_OBJECT
public:
    typedef enum Auth_LoginStatus_Enum{
        Auth_LoginStatus_Off = 0,//关闭登陆
        Auth_LoginStatus_Login,//登陆
        Auth_LoginStatus_Logout//注销
    }Auth_LoginStatus;

    static Authorization *instance(){static Authorization obj; return &obj;}//获取权限认证实例
    bool authenticate(quint8 moduleId);//认证终端模块权限
    bool login(QString name, QString passwd);//登陆
    void logout();//注销

    const UserObject* getLoginedUser();//获取已登陆用户
    quint8 getLoginStatus();//0-关闭登陆，1-登陆，2-无登陆
    bool isApprovalOperation();//无登陆状态是否允许操作
    QString getCurrentUserName();
    void resetTimer();

private:
    Authorization(QObject *parent = 0);

signals:
    void sigAuthUpdate();

public slots:
    void slotConfigChanged(quint32);

private slots:
    void slotAutoLogout();

private:
    quint8 m_touchOpe;//触摸操作安全认证功能
    quint8 m_autoLogout;//自动注销
    quint8 m_quitOpe;//允许终端无登陆操作

    quint8 m_loginStatus;//登陆状态，0-关闭登陆，1-登陆，2-无登陆
    UserObject m_user;//用户实例
    QTimer m_logoutTimer;
};

#endif // AUTHORIZATION_H
