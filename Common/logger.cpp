#include <stdarg.h>
#include "logger.h"

Logger::Logger(QObject *parent) :
    QObject(parent)
{
    openlog("F270",0,LOG_LOCAL0|LOG_LOCAL1|LOG_LOCAL2|LOG_LOCAL3|LOG_LOCAL4|LOG_LOCAL5|LOG_LOCAL6|LOG_LOCAL7);
}


void Logger::log(int pri, QString msg)
{
    std::string str = msg.toStdString();
    syslog(pri | LOG_INFO,"%s\n",str.c_str());
}


void Logger::log(int pri, const char *format, ...)
{
    va_list args;
    va_start(args,format);
    vsyslog(pri | LOG_INFO,format,args);
    va_end(args);
}


EventLogger::EventLogger(QObject *parent):
    QObject(parent),lg(Logger::getInstance())
{
}

void EventLogger::log(EventLogger::Event_Acton action, EventLogger::Event_Factor factor,QString userName)
{
    if(userName.isEmpty()){
        lg.log(Logger::EVENT_LOG,QString("%1|%2").arg(action).arg(factor));
    }else{
        lg.log(Logger::EVENT_LOG,QString("%1|%2|%3").arg(action).arg(factor).arg(userName));
    }
    emit sigUpdate();
}

QString EventLogger::getActionString(QString actionStr)
{
    Event_Acton action = (Event_Acton)actionStr.toInt();
    switch (action) {
    case Event_Action_Login:
        return tr("Login");
    case Event_Action_Logout:
        return tr("Logout");
    case Event_Action_Newtime:
        return tr("Newtime");
    case Event_Action_Timechanged:
        return tr("Time changed");
    case Event_Action_Poweroff:
        return tr("PowerOff");
    case Event_Action_Poweron:
        return tr("PowerOn");
    case Event_Action_TRevbegin:
        return tr("TRev start");
    case Event_Action_TRevend:
        return tr("TRev stop");
    case Event_Action_SNTP:
        return tr("SNTP update");
    case Event_Action_DSTStart:
        return tr("DST start");
    case Event_Acton_DSTStop:
        return tr("DST stop");
    default:
        return QString();
    }
}
QString EventLogger::getFactorString(QString factorStr)
{
    Event_Factor factor = (Event_Factor)factorStr.toInt();
    switch (factor) {
    case Event_Factor_OPERATE:
         return tr("OPERATE");
    case Event_Factor_COMMU:
        return tr("COMMU");
    case Event_Factor_REMOTE:
        return tr("REMOTE");
    case Event_Factor_ACTION:
        return tr("ACTION");
    case Event_Factor_SYSTEM:
        return tr("SYSTEM");
    case Event_Factor_SERIAL:
        return tr("SERIAL");
    default:
        return QString();
    }
}


ErrorLogger::ErrorLogger(QObject *parent):
    QObject(parent),lg(Logger::getInstance())
{
}

void ErrorLogger::log(int errCode)
{
    lg.log(Logger::ERROR_LOG,QString("%1").arg(errCode));
    emit sigUpdate();
}

QString ErrorLogger::getErrorInfo(QString errCodeStr)
{
//    int code = errCodeStr.toInt();
    return QString();//TODO
}

GeneralLogger::GeneralLogger(QObject *parent):
    QObject(parent),lg(Logger::getInstance())
{
}

void GeneralLogger::log(GeneralLogger::General_User_Authnum authNum, GeneralLogger::General_IO_Symbol symbol, QString info)
{
    QString authNumStr = "";
    switch (authNum) {
    case General_User_Authnum_E0:
        authNumStr = "e0";
        break;
    case General_User_Authnum_E1:
        authNumStr = "e1";
        break;
    case General_User_Authnum_E2:
        authNumStr = "e2";
        break;
    case General_User_Authnum_E3:
        authNumStr = "e3";
        break;
    case General_User_Authnum_S0:
        authNumStr = "s0";
        break;
    }
    QString symbolStr = "<";
    if(symbol == General_IO_Symbol_Rcev){
        symbolStr = ">";
    }
    lg.log(Logger::GENERAL_LOG,QString("%1|%2|%3").arg(authNumStr, symbolStr,info));
    emit sigUpdate();
}


