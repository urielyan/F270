#include "configtreeobject_Security.h"
#include "cfgchannel.h"
#include "./GlobalData/globalDef.h"

#include "cfgGlobalDef.h"
#include "cfgGlobalDef_Security.h"
#include "configtreeobject.h"
#include "./Common/authorization.h"

ConfigTreeObject_Security *ConfigTreeObject_Security::instance(QObject *parent)
{
    static ConfigTreeObject_Security data(parent);
    return &data;
}

ConfigTreeObject_Security::ConfigTreeObject_Security(QObject *parent):
    ConfigObject(parent)
{
    //权限变更时安全设定的显示隐藏
    connect(Authorization::instance(), SIGNAL(sigAuthUpdate()), this, SLOT(slotSecurityAuthUpdate()));

    //Security settings代理树
    QFile fileDelegate(":/configtree/Image/configTree/Security/Security_SettingDelegate/delegate.xml");
    if(!fileDelegate.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file: Security_settingDelegate!";
    }else{
        XmlConfigParser::parseConfigXml(fileDelegate, static_cast<ConfigObject*>(parent), \
                                                                   (QObject*)this);
        fileDelegate.close();
    }
}

void ConfigTreeObject_Security::updateStrShowOn()
{
    slotSecurityAuthUpdate();
}

void ConfigTreeObject_Security::slotCreateSecuritysettingTree(QObject *parent)
{
    //Security setting 配置树
    QFile file(":/configtree/Image/configTree/Security/Security_Setting/config.xml");
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file: Security_setting!";
    }else{
        XmlConfigParser::parseConfigXml(file, static_cast<ConfigObject*>(parent));
        file.close();
    }

    quint32 ID_userNum = ConfigTreeObject::getConfigObject(SECURITYCONFIG_SETTING_USERNUM)->getObjectID();
    ConfigTreeObject::getConfigObject(SECURITYCONFIG_SETTING_USERNUM)->initData(0, 0, ID_userNum);
    ConfigTreeObject::getConfigObject(SECURITYCONFIG_SETTING_USERNUM)->setStrShowOn("1");

    quint32 ID_authNum = ConfigTreeObject::getConfigObject(SECURITYCONFIG_PROPERTY_NUM)->getObjectID();
    ConfigTreeObject::getConfigObject(SECURITYCONFIG_PROPERTY_NUM)->initData(0, 0, ID_authNum);
    ConfigTreeObject::getConfigObject(SECURITYCONFIG_PROPERTY_NUM)->setStrShowOn("1");
}

void ConfigTreeObject_Security::slotSecurityDelegateChange()
{
    QFile delegateChange(":/configtree/Image/configTree/Security/Security_DelegateChange/delegateChange.xml");
    if(!delegateChange.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file: Security_settingDelegateChange!";
    }else{
        XmlConfigParser::parseDelegateChangeXml(delegateChange);
        delegateChange.close();
    }

    _curStatus = false;
    updateStrShowOn();
    _curStatus = true;
}

void ConfigTreeObject_Security::slotSecurityAuthUpdate()
{
    quint8 loginStatus = Authorization::instance()->getLoginStatus();

    if(Authorization::Auth_LoginStatus_Login == loginStatus) {
        quint8 userId = Authorization::instance()->getLoginedUser()->getUserLevel();
        if(Security_UserLvl_User == userId) {
            ConfigTreeObject::getConfigObject(SECURITY_DELEGATE)->setHideFlag(true);
            return;
        }
    }

    ConfigTreeObject::getConfigObject(SECURITY_DELEGATE)->setHideFlag(false);
    slotSecurityBasicTouchChange();
    slotSecurityBasicCommChange();
    slotSecurityBasicAutoChange();
    slotSecurityBasicQuitChange();

    slotSecurityOpelockPwdChange();
    slotSecurityOpelockRecordChange();
    slotSecurityOpelockMathChange();
    slotSecurityOpelockDatasaveChange();
    slotSecurityOpelockMsgChange();
    slotSecurityOpelockBatchChange();
    slotSecurityOpelockACKChange();
    slotSecurityOpelockCommChange();
    slotSecurityOpelockTouchChange();
    slotSecurityOpelockTimeChange();
    slotSecurityOpelockSettingChange();
    slotSecurityOpelockEXTmediaChange();
    slotSecurityOpelockSystemChange();
    slotSecurityOpelockOutputChange();

    slotSecuritySettingLvlChange();
    slotSecuritySettingModeChange();
    slotSecuritySettingNameChange();
    slotSecuritySettingInitChange();
    slotSecuritySettingPropertyChange();
    slotSecuritySettingAuthNumChange();

    slotSecurityPropertyRecordChange();
    slotSecurityPropertyMathChange();
    slotSecurityPropertyDatasaveChange();
    slotSecurityPropertyMsgChange();
    slotSecurityPropertyBatchChange();
    slotSecurityPropertyACKChange();
    slotSecurityPropertyCommChange();
    slotSecurityPropertyTouchChange();
    slotSecurityPropertyTimeChange();
    slotSecurityPropertySettingChange();
    slotSecurityPropertyEXTmediaChange();
    slotSecurityPropertySystemChange();
    slotSecurityPropertyOutputChange();
}

void ConfigTreeObject_Security::slotSecurityBasicTouchChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(SECURITY_DELEGATE_BASIC_FUNC_TOUCHOPE)->getData();

    QFile file(XML_SECURITY_BASIC_TOUCH);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Security_BasicTouch!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Security::slotSecurityBasicCommChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(SECURITY_DELEGATE_BASIC_FUNC_COMM)->getData();

    QFile file(XML_SECURITY_BASIC_COMM);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Security_BasicComm!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Security::slotSecurityBasicAutoChange()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(SECURITY_DELEGATE_BASIC_LOGOUT_AUTO);
    quint32 ID = ID_Security_Basic_Logout_Auto;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_Security::slotSecurityBasicQuitChange()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(SECURITY_DELEGATE_BASIC_LOGOUT_QUIT);
    quint32 ID = ID_Security_Basic_Logout_Quit;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_Security::slotSecurityOpelockPwdChange()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(SECURITY_DELEGATE_OPELOCK_FUNC_PWD);
    quint32 ID = ID_Security_Ope_Function_Pwd;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_Security::slotSecurityOpelockRecordChange()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(SECURITY_DELEGATE_OPELOCK_LIMIT_RECORD);
    quint32 ID = ID_Security_Ope_Limit_Record;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_Security::slotSecurityOpelockMathChange()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(SECURITY_DELEGATE_OPELOCK_LIMIT_MATH);
    quint32 ID = ID_Security_Ope_Limit_Math;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_Security::slotSecurityOpelockDatasaveChange()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(SECURITY_DELEGATE_OPELOCK_LIMIT_DATASAVE);
    quint32 ID = ID_Security_Ope_Limit_DataSave;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_Security::slotSecurityOpelockMsgChange()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(SECURITY_DELEGATE_OPELOCK_LIMIT_MSG);
    quint32 ID = ID_Security_Ope_Limit_Msg;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_Security::slotSecurityOpelockBatchChange()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(SECURITY_DELEGATE_OPELOCK_LIMIT_BATCH);
    quint32 ID = ID_Security_Ope_Limit_Batch;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_Security::slotSecurityOpelockACKChange()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(SECURITY_DELEGATE_OPELOCK_LIMIT_ACK);
    quint32 ID = ID_Security_Ope_Limit_ACK;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_Security::slotSecurityOpelockCommChange()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(SECURITY_DELEGATE_OPELOCK_LIMIT_COMM);
    quint32 ID = ID_Security_Ope_Limit_Comm;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_Security::slotSecurityOpelockTouchChange()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(SECURITY_DELEGATE_OPELOCK_LIMIT_TOUCH);
    quint32 ID = ID_Security_Ope_Limit_TouchOpe;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_Security::slotSecurityOpelockTimeChange()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(SECURITY_DELEGATE_OPELOCK_LIMIT_TIME);
    quint32 ID = ID_Security_Ope_Limit_Time;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_Security::slotSecurityOpelockSettingChange()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(SECURITY_DELEGATE_OPELOCK_LIMIT_SETTING);
    quint32 ID = ID_Security_Ope_Limit_Setting;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_Security::slotSecurityOpelockEXTmediaChange()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(SECURITY_DELEGATE_OPELOCK_LIMIT_EXTMEDIA);
    quint32 ID = ID_Security_Ope_Limit_ExtMedia;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_Security::slotSecurityOpelockSystemChange()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(SECURITY_DELEGATE_OPELOCK_LIMIT_SYSTEM);
    quint32 ID = ID_Security_Ope_Limit_SysOpe;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_Security::slotSecurityOpelockOutputChange()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(SECURITY_DELEGATE_OPELOCK_LIMIT_OUTPUT);
    quint32 ID = ID_Security_Ope_Limit_OutputOpe;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_Security::slotSecuritySettingUserNumChange()
{
    ConfigObject *pObj = ConfigTreeObject::getConfigObject(SECURITY_DELEGATE_SETTING_USERNUM);

    if(pObj) {
        quint32 UserNum = pObj->getData().toUInt();
        pObj->setStrShowOn(QString("%1").arg(UserNum+1));
    }
}

void ConfigTreeObject_Security::slotSecuritySettingLvlChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(SECURITY_DELEGATE_SETTING_SETTINGS_LVL)->getData();

    QFile file(XML_SECURITY_SETTING_LVL);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Security_SettingLvl!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Security::slotSecuritySettingModeChange()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(SECURITY_DELEGATE_SETTING_SETTINGS_MODE);
    quint32 ID = ID_Security_UserSetting_UserSetting_Mode;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_Security::slotSecuritySettingNameChange()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(SECURITY_DELEGATE_SETTING_SETTINGS_NAME);
    quint32 ID = ID_Security_UserSetting_UserSetting_Name;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_Security::slotSecuritySettingInitChange()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(SECURITY_DELEGATE_SETTING_SETTINGS_INIT);
    quint32 ID = ID_Security_UserSetting_UserSetting_InitPwd;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_Security::slotSecuritySettingPropertyChange()
{
    QVariant value = ConfigTreeObject::getConfigObject(SECURITY_DELEGATE_SETTING_SETTINGS_PROPERTY)->getData();

    QFile file(XML_SECURITY_SETTING_AUTH);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< "Can't open the file:Security_SettingAuth!";
    }else{
        XmlConfigParser::parseValueChangeXml(file, value);
        file.close();
    }
}

void ConfigTreeObject_Security::slotSecuritySettingAuthNumChange()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(SECURITY_DELEGATE_SETTING_SETTINGS_AUTHNUM);
    quint32 ID = ID_Security_UserSetting_UserSetting_Num;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_Security::slotSecurityPropertyNumChange()
{
    ConfigObject *pObj = ConfigTreeObject::getConfigObject(SECURITY_DELEGATE_PROPERTY_NUM);

    if(pObj) {
        quint32 PropertyNum = pObj->getData().toUInt();
        pObj->setStrShowOn(QString("%1").arg(PropertyNum+1));
    }
}

void ConfigTreeObject_Security::slotSecurityPropertyRecordChange()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(SECURITY_DELEGATE_PROPERTY_PROPERTY_RECORD);
    quint32 ID = ID_Security_UserProPerty_ProPerty_Record;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_Security::slotSecurityPropertyMathChange()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(SECURITY_DELEGATE_PROPERTY_PROPERTY_MATH);
    quint32 ID = ID_Security_UserProPerty_ProPerty_Math;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_Security::slotSecurityPropertyDatasaveChange()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(SECURITY_DELEGATE_PROPERTY_PROPERTY_DATASAVE);
    quint32 ID = ID_Security_UserProPerty_ProPerty_DataSave;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_Security::slotSecurityPropertyMsgChange()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(SECURITY_DELEGATE_PROPERTY_PROPERTY_MSG);
    quint32 ID = ID_Security_UserProPerty_ProPerty_Msg;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_Security::slotSecurityPropertyBatchChange()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(SECURITY_DELEGATE_PROPERTY_PROPERTY_BATCH);
    quint32 ID = ID_Security_UserProPerty_ProPerty_Batch;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_Security::slotSecurityPropertyACKChange()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(SECURITY_DELEGATE_PROPERTY_PROPERTY_ACK);
    quint32 ID = ID_Security_UserProPerty_ProPerty_ACK;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_Security::slotSecurityPropertyCommChange()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(SECURITY_DELEGATE_PROPERTY_PROPERTY_COMM);
    quint32 ID = ID_Security_UserProPerty_ProPerty_Comm;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_Security::slotSecurityPropertyTouchChange()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(SECURITY_DELEGATE_PROPERTY_PROPERTY_TOUCH);
    quint32 ID = ID_Security_UserProPerty_ProPerty_TouchOpe;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_Security::slotSecurityPropertyTimeChange()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(SECURITY_DELEGATE_PROPERTY_PROPERTY_TIME);
    quint32 ID = ID_Security_UserProPerty_ProPerty_Time;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_Security::slotSecurityPropertySettingChange()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(SECURITY_DELEGATE_PROPERTY_PROPERTY_SETTING);
    quint32 ID = ID_Security_UserProPerty_ProPerty_Setting;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_Security::slotSecurityPropertyEXTmediaChange()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(SECURITY_DELEGATE_PROPERTY_PROPERTY_EXTMEDIA);
    quint32 ID = ID_Security_UserProPerty_ProPerty_ExtMedia;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_Security::slotSecurityPropertySystemChange()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(SECURITY_DELEGATE_PROPERTY_PROPERTY_SYSTEM);
    quint32 ID = ID_Security_UserProPerty_ProPerty_SysOpe;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}

void ConfigTreeObject_Security::slotSecurityPropertyOutputChange()
{
    ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(SECURITY_DELEGATE_PROPERTY_PROPERTY_OUTPUT);
    quint32 ID = ID_Security_UserProPerty_ProPerty_OutputOpe;

    QString valueStr = CfgConfig::instance()->getValObjectStr(ID, 0, pConfigObj);
    pConfigObj->setStrShowOn(valueStr);
}
