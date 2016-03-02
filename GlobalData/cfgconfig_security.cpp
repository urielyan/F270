#include "cfgconfig_security.h"
#include "./GlobalData/configtreeobject_Security.h"
#include "./GlobalData/configtreeobject.h"


CfgConfig_Security *CfgConfig_Security::instance()
{
    static CfgConfig_Security data;
    return &data;
}

CfgConfig_Security::CfgConfig_Security(QObject *parent) : QObject(parent)
{
}





QVariant CfgConfig_Security::getSecurityData(quint32 ID, quint32 group)
{
    if (ID >= Security_Basic_Start_ID && ID <= Security_Basic_End_ID)
        return getSecurityBasicData(ID, group);
    else if (ID >= Security_Ope_Start_ID && ID <= Security_Ope_End_ID)
        return getSecurityOperationData(ID, group);
    else if (ID >= Security_UserSetting_Start_ID && ID <= Security_UserSetting_End_ID)
        return getSecurityUserSettingData(ID, group + 1);
    else if (ID >= Security_Property_Start_ID && ID <= Security_Property_End_ID)
        return getSecurityUserPropertyData(ID, group + 1);
    return QVariant();
}

QString CfgConfig_Security::getSecurityObjStr(quint32 ID, quint32 group)
{
    if (ID >= Security_Basic_Start_ID && ID <= Security_Basic_End_ID)
        return getSecurityBasicObjStr(ID, group);
    else if (ID >= Security_Ope_Start_ID && ID <= Security_Ope_End_ID)
        return getSecurityOperationObjStr(ID, group);
    else if (ID >= Security_UserSetting_Start_ID && ID <= Security_UserSetting_End_ID)
        return getSecurityUserSettingObjStr(ID, group);
    else if (ID >= Security_Property_Start_ID && ID <= Security_Property_End_ID)
        return getSecurityUserPropertyObjStr(ID, group);
    return NULL;
}


QString CfgConfig_Security::getSecurityObjValStr(quint32 ID, quint32 group, ConfigObject *cfg)
{
    if (ID >= Security_Basic_Start_ID && ID <= Security_Basic_End_ID)
        return getSecurityBasicObjValStr(ID, group, cfg);
    else if (ID >= Security_Ope_Start_ID && ID <= Security_Ope_End_ID)
        return getSecurityOperationObjValStr(ID, group, cfg);
    else if (ID >= Security_UserSetting_Start_ID && ID <= Security_UserSetting_End_ID)
        return getSecurityUserSettingObjValStr(ID, group, cfg);
    else if (ID >= Security_Property_Start_ID && ID <= Security_Property_End_ID)
        return getSecurityUserPropertyObjValStr(ID, group, cfg);
    return NULL;
}


void CfgConfig_Security::setSecurityData(quint32 ID, quint32 group)
{
    if (ID >= Security_Basic_Start_ID && ID <= Security_Basic_End_ID)
        setSecurityBaiscData(ID, group);
    else if (ID >= Security_Ope_Start_ID && ID <= Security_Ope_End_ID)
        setSecurityOperationData(ID, group);
    else if (ID >= Security_UserSetting_Start_ID && ID <= Security_UserSetting_End_ID)
        setSecurityUserSettingData(ID, group);
    else if (ID >= Security_Property_Start_ID && ID <= Security_Property_End_ID)
        setSecurityUserPropertyData(ID, group);
}





QVariant CfgConfig_Security::getSecurityBasicData(quint32 ID, quint32 /*group*/)
{
    switch (ID) {
    case ID_Security_Basic_Function_Touch:
        return p_securityConfig->securityBasic.securityTouchOpe;
        break;
    case ID_Security_Basic_Function_Comm:
        return p_securityConfig->securityBasic.securityComm;
        break;
    case ID_Security_Basic_Logout_Auto:
        return p_securityConfig->securityBasic.securityLogout;
        break;
    case ID_Security_Basic_Logout_Quit:
        return p_securityConfig->securityBasic.securityQuitOpe;
        break;
    default:
        break;
    }
    return QVariant();
}

QVariant CfgConfig_Security::getSecurityOperationData(quint32 ID, quint32 /*group*/)
{
    if (ID == ID_Security_Ope_Function_Pwd)
        return getCharToString(p_securityConfig->securityUserGrp[0].securityUserPwd);

    int pos = ID - ID_Security_Ope_Limit_Record;
    return PROTY_Data(p_securityConfig->securityProperty[0], pos);
//    switch (ID) {
//    case ID_Security_Ope_Limit_Record:
//        return PROTY_Record(p_securityConfig->securityProperty[0]);
//        break;
//    case ID_Security_Ope_Limit_Math:
//        return PROTY_Math(p_securityConfig->securityProperty[0]);
//        break;
//    case ID_Security_Ope_Limit_DataSave:
//        return PROTY_Datasave(p_securityConfig->securityProperty[0]);
//        break;
//    case ID_Security_Ope_Limit_Msg:
//        return PROTY_Msg(p_securityConfig->securityProperty[0]);
//        break;
//    case ID_Security_Ope_Limit_Batch:
//        return PROTY_Batch(p_securityConfig->securityProperty[0]);
//        break;
//    case ID_Security_Ope_Limit_ACK:
//        return PROTY_ACK(p_securityConfig->securityProperty[0]);
//        break;
//    case ID_Security_Ope_Limit_Comm:
//        return PROTY_Comm(p_securityConfig->securityProperty[0]);
//        break;
//    case ID_Security_Ope_Limit_TouchOpe:
//        return PROTY_TouchOpe(p_securityConfig->securityProperty[0]);
//        break;
//    case ID_Security_Ope_Limit_Time:
//        return PROTY_Time(p_securityConfig->securityProperty[0]);
//        break;
//    case ID_Security_Ope_Limit_Setting:
//        return PROTY_Setting(p_securityConfig->securityProperty[0]);
//        break;
//    case ID_Security_Ope_Limit_ExtMedia:
//        return PROTY_EXTMedia(p_securityConfig->securityProperty[0]);
//        break;
//    case ID_Security_Ope_Limit_SysOpe:
//        return PROTY_SysOpe(p_securityConfig->securityProperty[0]);
//        break;
//    case ID_Security_Ope_Limit_OutputOpe:
//        return PROTY_OutputOPe(p_securityConfig->securityProperty[0]);
//        break;
//    default:
//        break;
//    }
}


QVariant CfgConfig_Security::getSecurityUserSettingData(quint32 ID, quint32 group)
{
    switch (ID) {
    case ID_Security_UserSetting_UserSetting_Level:
        return p_securityConfig->securityUserGrp[group].securityUserLvl;
        break;
    case ID_Security_UserSetting_UserSetting_Mode:
        return p_securityConfig->securityUserGrp[group].securityUserMode;
        break;
    case ID_Security_UserSetting_UserSetting_Name:
        return getCharToString(p_securityConfig->securityUserGrp[group].securityUserName);
        break;
    case ID_Security_UserSetting_UserSetting_UserPwd:
        return getCharToString(p_securityConfig->securityUserGrp[group].securityUserPwd);
        break;

    case ID_Security_UserSetting_UserSetting_InitPwd:
        return p_securityConfig->securityUserGrp[group].securityUserInitPwd;
        break;
    case ID_Security_UserSetting_UserSetting_Property:
        return p_securityConfig->securityUserGrp[group].securityUserProperty;
        break;
    case ID_Security_UserSetting_UserSetting_Num:
        return p_securityConfig->securityUserGrp[group].securityUserNum;
        break;
    default:
        break;
    }
    return QVariant();
}


QVariant CfgConfig_Security::getSecurityUserPropertyData(quint32 ID, quint32 group)
{
    int x = ID - ID_Security_UserProPerty_ProPerty_Record;
    return PROTY_Data(p_securityConfig->securityProperty[group], x);
//    switch (ID) {
//    case ID_Security_UserProPerty_ProPerty_Record:
//        return PROTY_Record(p_securityConfig->securityProperty[group]);
//        break;
//    case ID_Security_UserProPerty_ProPerty_Math:
//        return PROTY_Math(p_securityConfig->securityProperty[group]);
//        break;
//    case ID_Security_UserProPerty_ProPerty_DataSave:
//        return PROTY_Datasave(p_securityConfig->securityProperty[group]);
//        break;
//    case ID_Security_UserProPerty_ProPerty_Msg:
//        return PROTY_Msg(p_securityConfig->securityProperty[group]);
//        break;
//    case ID_Security_UserProPerty_ProPerty_Batch:
//        return PROTY_Batch(p_securityConfig->securityProperty[group]);
//        break;
//    case ID_Security_UserProPerty_ProPerty_ACK:
//        return PROTY_ACK(p_securityConfig->securityProperty[group]);
//        break;
//    case ID_Security_UserProPerty_ProPerty_Comm:
//        return PROTY_Comm(p_securityConfig->securityProperty[group]);
//        break;
//    case ID_Security_UserProPerty_ProPerty_TouchOpe:
//        return PROTY_TouchOpe(p_securityConfig->securityProperty[group]);
//        break;
//    case ID_Security_UserProPerty_ProPerty_Time:
//        return PROTY_Time(p_securityConfig->securityProperty[group]);
//        break;
//    case ID_Security_UserProPerty_ProPerty_Setting:
//        return PROTY_Setting(p_securityConfig->securityProperty[group]);
//        break;
//    case ID_Security_UserProPerty_ProPerty_ExtMedia:
//        return PROTY_EXTMedia(p_securityConfig->securityProperty[group]);
//        break;
//    case ID_Security_UserProPerty_ProPerty_SysOpe:
//        return PROTY_SysOpe(p_securityConfig->securityProperty[group]);
//        break;
//    case ID_Security_UserProPerty_ProPerty_OutputOpe:
//        return PROTY_OutputOPe(p_securityConfig->securityProperty[group]);
//        break;
//    default:
//        break;
//    }
//    return QVariant();
}








QString CfgConfig_Security::getSecurityBasicObjStr(quint32 ID, quint32 /*group*/)
{
    switch (ID) {
    case ID_Security:
        return tr("Security settings");
        break;
    case ID_Security_Basic:
        return tr("Basic settings");
        break;

    case ID_Security_Basic_Function:
        return tr("Security function");
        break;
    case ID_Security_Basic_Function_Touch:
        return tr("Touch operation");
        break;
    case ID_Security_Basic_Function_Comm:
        return tr("Communication");
        break;

    case ID_Security_Basic_Logout:
        return tr("Logout");
        break;
    case ID_Security_Basic_Logout_Auto:
        return tr("Auto logout");
        break;
    case ID_Security_Basic_Logout_Quit:
        return tr("Operation without Login");
        break;
    default:
        break;
    }
    return NULL;
}


QString CfgConfig_Security::getSecurityOperationObjStr(quint32 ID, quint32 /*group*/)
{
    switch (ID) {
    case ID_Security_Ope:
        return tr("Operation Lock");
        break;

    case ID_Security_Ope_Function:
        return tr("Operation Lock function");
        break;
    case ID_Security_Ope_Function_Pwd:
        return tr("Password");
        break;

    case ID_Security_Ope_Limit:
        return tr("Limiations");
        break;
    case ID_Security_Ope_Limit_Record:
        return tr("Record");
        break;
    case ID_Security_Ope_Limit_Math:
        return tr("Math");
        break;
    case ID_Security_Ope_Limit_DataSave:
        return tr("Data save");
        break;
    case ID_Security_Ope_Limit_Msg:
        return tr("Message");
        break;
    case ID_Security_Ope_Limit_Batch:
        return tr("Batch");
        break;
    case ID_Security_Ope_Limit_ACK:
        return tr("AlarmACK");
        break;
    case ID_Security_Ope_Limit_Comm:
        return tr("Communication");
        break;
    case ID_Security_Ope_Limit_TouchOpe:
        return tr("Touch operation");
        break;
    case ID_Security_Ope_Limit_Time:
        return tr("Time set");
        break;
    case ID_Security_Ope_Limit_Setting:
        return tr("Setting operation");
        break;
    case ID_Security_Ope_Limit_ExtMedia:
        return tr("External media");
        break;
    case ID_Security_Ope_Limit_SysOpe:
        return tr("System operation");
        break;
    case ID_Security_Ope_Limit_OutputOpe:
        return tr("Output operation");
        break;
    default:
        break;
    }
    return NULL;
}


QString CfgConfig_Security::getSecurityUserSettingObjStr(quint32 ID, quint32 /*group*/)
{
    switch (ID) {
    case ID_Security_UserSetting:
    case ID_Security_UserSetting_UserSetting:
        return tr("User settings");
        break;
    case ID_Security_UserSetting_UserNum:
        return tr("User number");
        break;

    case ID_Security_UserSetting_UserSetting_Level:
        return tr("User level");
        break;
    case ID_Security_UserSetting_UserSetting_Mode:
        return tr("Mode");
        break;
    case ID_Security_UserSetting_UserSetting_Name:
        return tr("User name");
        break;
    case ID_Security_UserSetting_UserSetting_UserPwd:       //不显示
        return tr("");
        break;
    case ID_Security_UserSetting_UserSetting_InitPwd:
        return tr("Initialize password");
        break;
    case ID_Security_UserSetting_UserSetting_Property:
        return tr("User property");
        break;
    case ID_Security_UserSetting_UserSetting_Num:
        return tr("Authority number");
        break;
    default:
        break;
    }
    return NULL;
}


QString CfgConfig_Security::getSecurityUserPropertyObjStr(quint32 ID, quint32 /*group*/)
{
    switch (ID) {
    case ID_Security_UserProPerty:
        return tr("User property");
        break;

    case ID_Security_UserProPerty_AuthNum:
        return tr("Authority number");
        break;

    case ID_Security_UserProPerty_ProPerty:
        return tr("User property");
        break;
    case ID_Security_UserProPerty_ProPerty_Record:
        return tr("Record");
        break;
    case ID_Security_UserProPerty_ProPerty_Math:
        return tr("Math");
        break;
    case ID_Security_UserProPerty_ProPerty_DataSave:
        return tr("Data save");
        break;
    case ID_Security_UserProPerty_ProPerty_Msg:
        return tr("Message");
        break;
    case ID_Security_UserProPerty_ProPerty_Batch:
        return tr("Batch");
        break;
    case ID_Security_UserProPerty_ProPerty_ACK:
        return tr("AlarmACK");
        break;
    case ID_Security_UserProPerty_ProPerty_Comm:
        return tr("Communication");
        break;
    case ID_Security_UserProPerty_ProPerty_TouchOpe:
        return tr("Touch operation");
        break;
    case ID_Security_UserProPerty_ProPerty_Time:
        return tr("Time set");
        break;
    case ID_Security_UserProPerty_ProPerty_Setting:
        return tr("Setting operation");
        break;
    case ID_Security_UserProPerty_ProPerty_ExtMedia:
        return tr("Externak media");
        break;
    case ID_Security_UserProPerty_ProPerty_SysOpe:
        return tr("System operation");
        break;
    case ID_Security_UserProPerty_ProPerty_OutputOpe:
        return tr("Output operation");
        break;
    default:
        break;
    }
    return NULL;
}



QString CfgConfig_Security::getSecurityBasicObjValStr(quint32 ID, quint32 /*group*/, ConfigObject *cfg)
{
    switch (ID) {
    case ID_Security_Basic_Function_Touch:
    {
        int type = cfg->getData().toUInt();
        if (type == 0)
            return tr("Off");
        else if (type == 1)
            return tr("Login");
        else if (type == 2)
            return tr("Operation Lock");
        else
            return NULL;
    }
        break;

    case ID_Security_Basic_Function_Comm:
    {
        int type = cfg->getData().toUInt();
        if (type == 0)
            return tr("Off");
        else if (type == 1)
            return tr("Login");
        else
            return NULL;
    }
        break;

    case ID_Security_Basic_Logout_Auto:
    {
        if (cfg->getData().toInt() == 0)
            return tr("Off");
        else
            return cfg->getData().toString() + " min";
    }
        break;
    case ID_Security_Basic_Logout_Quit:
        return cfg->getData().toInt() == 0 ? tr("Off") : tr("On");
        break;
    default:
        break;
    }
    return NULL;
}


QString CfgConfig_Security::getSecurityOperationObjValStr(quint32 ID, quint32 /*group*/, ConfigObject *cfg)
{
    switch (ID) {
    case ID_Security_Ope_Function_Pwd:
        return "****";
        break;
    default:
        break;
    }
    return cfg->getData().toInt() == 0 ? tr("Free") : tr("Lock");
}


QString CfgConfig_Security::getSecurityUserSettingObjValStr(quint32 ID, quint32 /*group*/, ConfigObject *cfg)
{
    switch (ID) {
    case ID_Security_UserSetting_UserNum:
        return cfg->getData().toString();
        break;
    case ID_Security_UserSetting_UserSetting_Level:
    {
        int type = cfg->getData().toInt();
        if (type == 0)
            return tr("Off");
        else if (type == 1)
            return tr("Admin");
        else
            return tr("User");
    }
        break;
    case ID_Security_UserSetting_UserSetting_Mode:
    {
        int type = cfg->getData().toInt();
        if (type == 0)
            return tr("Touch operation");
        else if (type == 1)
            return tr("Communication");
        else if (type == 2)
            return tr("Touch operation + Communication");
    }
    case ID_Security_UserSetting_UserSetting_InitPwd:
        return cfg->getData().toInt() == 0 ? tr("Back") : tr("Initialize password");
        break;
    case ID_Security_UserSetting_UserSetting_Property:
        return cfg->getData().toInt() == 0 ? tr("Off") : tr("On");
        break;
    default:
        break;
    }
    return cfg->getData().toString();
}


QString CfgConfig_Security::getSecurityUserPropertyObjValStr(quint32 ID, quint32 /*group*/, ConfigObject *cfg)
{
    switch (ID) {
    case ID_Security_UserProPerty_AuthNum:
        return cfg->getData().toString();
        break;
    default:
        break;
    }
    return cfg->getData().toInt() == 0 ? tr("Free") : tr("Lock");
}





void CfgConfig_Security::setSecurityBaiscData(quint32 ID, quint32 /*group*/)
{
    switch (ID) {
    case ID_Security_Basic_Function_Touch:
        p_securityConfig->securityBasic.securityTouchOpe = (Security_TouchOpe)ConfigTreeObject::instance()->getConfigData(SECURITYCONFIG_BASIC_FUNC_TOUCHOPE).toInt();
        break;
    case ID_Security_Basic_Function_Comm:
        p_securityConfig->securityBasic.securityComm = (Security_Comm)ConfigTreeObject::instance()->getConfigData(SECURITYCONFIG_BASIC_FUNC_COMM).toInt();
        break;
    case ID_Security_Basic_Logout_Auto:
        p_securityConfig->securityBasic.securityLogout = (Security_Logout)ConfigTreeObject::instance()->getConfigData(SECURITYCONFIG_BASIC_LOGOUT_AUTO).toInt();
        break;
    case ID_Security_Basic_Logout_Quit:
        p_securityConfig->securityBasic.securityQuitOpe = ConfigTreeObject::instance()->getConfigData(SECURITYCONFIG_BASIC_LOGOUT_QUIT).toInt();
        break;
    default:
        break;
    }
}

void CfgConfig_Security::setSecurityOperationData(quint32 ID, quint32 /*group*/)
{
    if (ID == ID_Security_Ope_Function_Pwd)
    {
        setStrToChar(p_securityConfig->securityUserGrp[0].securityUserPwd, ConfigTreeObject::instance()->getConfigData(SECURITYCONFIG_OPELOCK_FUNC_PWD).toString(), 20);
        return ;
    }
    int pos = ID - ID_Security_Ope_Limit_Record;
    QString path = NULL;
    switch (ID) {
    case ID_Security_Ope_Limit_Record:
        path = QString(SECURITYCONFIG_OPELOCK_LIMIT_RECORD);
        break;
    case ID_Security_Ope_Limit_Math:
        path = QString(SECURITYCONFIG_OPELOCK_LIMIT_MATH);
        break;
    case ID_Security_Ope_Limit_DataSave:
        path = QString(SECURITYCONFIG_OPELOCK_LIMIT_DATASAVE);
        break;
    case ID_Security_Ope_Limit_Msg:
        path = QString(SECURITYCONFIG_OPELOCK_LIMIT_MSG);
        break;
    case ID_Security_Ope_Limit_Batch:
        path = QString(SECURITYCONFIG_OPELOCK_LIMIT_BATCH);
        break;
    case ID_Security_Ope_Limit_ACK:
        path = QString(SECURITYCONFIG_OPELOCK_LIMIT_ACK);
        break;
    case ID_Security_Ope_Limit_Comm:
        path = QString(SECURITYCONFIG_OPELOCK_LIMIT_COMM);
        break;
    case ID_Security_Ope_Limit_TouchOpe:
        path = QString(SECURITYCONFIG_OPELOCK_LIMIT_TOUCH);
        break;
    case ID_Security_Ope_Limit_Time:
        path = QString(SECURITYCONFIG_OPELOCK_LIMIT_TIME);
        break;
    case ID_Security_Ope_Limit_Setting:
        path = QString(SECURITYCONFIG_OPELOCK_LIMIT_SETTING);
        break;
    case ID_Security_Ope_Limit_ExtMedia:
        path = QString(SECURITYCONFIG_OPELOCK_LIMIT_EXTMEDIA);
        break;
    case ID_Security_Ope_Limit_SysOpe:
        path = QString(SECURITYCONFIG_OPELOCK_LIMIT_SYSTEM);
        break;
    case ID_Security_Ope_Limit_OutputOpe:
        path = QString(SECURITYCONFIG_OPELOCK_LIMIT_OUTPUT);
        break;
    default:
        break;
    }
    int bit = ConfigTreeObject::instance()->getConfigData(path).toInt();

    Set_PROTY_Data(p_securityConfig->securityProperty[0], bit, pos);
}

void CfgConfig_Security::setSecurityUserSettingData(quint32 ID, quint32 group)
{
    int index = group + 1;
    switch (ID) {
    case ID_Security_UserSetting_UserSetting_Level:
        p_securityConfig->securityUserGrp[index].securityUserLvl = (Security_UserLvl)ConfigTreeObject::instance()->getConfigData(QString(SECURITYCONFIG_SETTING_SETTINGS_LVL).arg(group)).toInt();
        break;
    case ID_Security_UserSetting_UserSetting_Mode:
        p_securityConfig->securityUserGrp[index].securityUserMode = (Security_UserMode)ConfigTreeObject::instance()->getConfigData(QString(SECURITYCONFIG_SETTING_SETTINGS_MODE).arg(group)).toInt();
        break;
    case ID_Security_UserSetting_UserSetting_Name:
        setStrToChar(p_securityConfig->securityUserGrp[index].securityUserName, ConfigTreeObject::instance()->getConfigData(QString(SECURITYCONFIG_SETTING_SETTINGS_NAME).arg(group)).toString(), 20);
        break;
    case ID_Security_UserSetting_UserSetting_UserPwd:
        setStrToChar(p_securityConfig->securityUserGrp[index].securityUserPwd, ConfigTreeObject::instance()->getConfigData(QString(SECURITYCONFIG_SETTING_SETTINGS_PWD).arg(group)).toString(), 20);
        break;

    case ID_Security_UserSetting_UserSetting_InitPwd:
        p_securityConfig->securityUserGrp[index].securityUserInitPwd = ConfigTreeObject::instance()->getConfigData(QString(SECURITYCONFIG_SETTING_SETTINGS_INIT).arg(group)).toInt();
        break;
    case ID_Security_UserSetting_UserSetting_Property:
        p_securityConfig->securityUserGrp[index].securityUserProperty = ConfigTreeObject::instance()->getConfigData(QString(SECURITYCONFIG_SETTING_SETTINGS_PROPERTY).arg(group)).toInt();
        break;
    case ID_Security_UserSetting_UserSetting_Num:
        p_securityConfig->securityUserGrp[index].securityUserNum = ConfigTreeObject::instance()->getConfigData(QString(SECURITYCONFIG_SETTING_SETTINGS_AUTHNUM).arg(group)).toInt();
        break;
    default:
        break;
    }
}

void CfgConfig_Security::setSecurityUserPropertyData(quint32 ID, quint32 group)
{
    int pos = ID - ID_Security_UserProPerty_ProPerty_Record;
    QString path = NULL;
    switch (ID) {
    case ID_Security_UserProPerty_ProPerty_Record:
        path = QString(SECURITYCONFIG_PROPERTY_PROPERTY_RECORD).arg(group);
        break;
    case ID_Security_UserProPerty_ProPerty_Math:
        path = QString(SECURITYCONFIG_PROPERTY_PROPERTY_MATH).arg(group);
        break;
    case ID_Security_UserProPerty_ProPerty_DataSave:
        path = QString(SECURITYCONFIG_PROPERTY_PROPERTY_DATASAVE).arg(group);
        break;
    case ID_Security_UserProPerty_ProPerty_Msg:
        path = QString(SECURITYCONFIG_PROPERTY_PROPERTY_MSG).arg(group);
        break;
    case ID_Security_UserProPerty_ProPerty_Batch:
        path = QString(SECURITYCONFIG_PROPERTY_PROPERTY_BATCH).arg(group);
        break;
    case ID_Security_UserProPerty_ProPerty_ACK:
        path = QString(SECURITYCONFIG_PROPERTY_PROPERTY_ACK).arg(group);
        break;
    case ID_Security_UserProPerty_ProPerty_Comm:
        path = QString(SECURITYCONFIG_PROPERTY_PROPERTY_COMM).arg(group);
        break;
    case ID_Security_UserProPerty_ProPerty_TouchOpe:
        path = QString(SECURITYCONFIG_PROPERTY_PROPERTY_TOUCH).arg(group);
        break;
    case ID_Security_UserProPerty_ProPerty_Time:
        path = QString(SECURITYCONFIG_PROPERTY_PROPERTY_TIME).arg(group);
        break;
    case ID_Security_UserProPerty_ProPerty_Setting:
        path = QString(SECURITYCONFIG_PROPERTY_PROPERTY_SETTING).arg(group);
        break;
    case ID_Security_UserProPerty_ProPerty_ExtMedia:
        path = QString(SECURITYCONFIG_PROPERTY_PROPERTY_EXTMEDIA).arg(group);
        break;
    case ID_Security_UserProPerty_ProPerty_SysOpe:
        path = QString(SECURITYCONFIG_PROPERTY_PROPERTY_SYSTEM).arg(group);
        break;
    case ID_Security_UserProPerty_ProPerty_OutputOpe:
        path = QString(SECURITYCONFIG_PROPERTY_PROPERTY_OUTPUT).arg(group);
        break;
    default:
        break;
    }

    int bit = ConfigTreeObject::instance()->getConfigData(path).toInt();
    Set_PROTY_Data(p_securityConfig->securityProperty[group + 1], bit, pos);
}





/*
 * 把uchar *类型转化为QString类型
*/
QVariant CfgConfig_Security::getCharToString(uchar *str)
{
    QString data((char *)str);
    return data;
}

//把QString类型转化为uchar *类型
void CfgConfig_Security::setStrToChar(uchar * uData, QString str, int len)
{
    memset(uData, 0, len);
    QByteArray baUnit = str.toLocal8Bit();
    char* valueUnit = baUnit.data();
//    memcpy(uData, valueUnit, strlen(valueUnit));
    while (*valueUnit != '\0') {
        *uData++ = *valueUnit++;
    }
}

