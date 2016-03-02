#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include <syslog.h>

/******************************************************
 * syslog 配置
 *
 * 1、添加配置信息（/etc/rsyslog.d/50-default.conf）
 * $template myformat,"%TIMESTAMP:::date-unixtimestamp%|%msg%\n"
 * local1.info			/var/log/local/event.log;myformat
 * local2.info			/var/log/local/general.log;myformat
 * local3.info			/var/log/local/error.log;myformat
 * local4.info			/var/log/local/modbus.log;myformat
 *
 * 2、重启syslog
 * sudo service rsyslog restart
 *
 *******************************************************/

#define EVENT_LOG_PATH              "/var/log/local/event.log"
#define GENERAL_LOG_PATH         "/var/log/local/general.log"
#define ERROR_LOG_PATH              "/var/log/local/error.log"
#define MODBUS_LOG_PATH         "/var/log/local/modbus.log"
#define FTP_LOG_PATH        "/var/log/local/ftp.log"
#define WEB_LOG_PATH        "/var/log/local/web.log"
#define SNTP_LOG_PATH       "/var/log/local/sntp.log"
#define DHCP_LOG_PATH       "/var/log/local/dhcp.log"
#define EMAIL_LOG_PATH      "/var/log/local/email.log"

class Logger : public QObject
{
    Q_OBJECT
public:
    static Logger& getInstance(){static Logger instance; return instance;}
    void log(int pri, QString msg);
    void log(int pri, const char *format, ...);
    ~Logger(){closelog();}

    typedef enum{
        EVENT_LOG=LOG_LOCAL1,
        GENERAL_LOG=LOG_LOCAL2,
        ERROR_LOG=LOG_LOCAL3,
        MODBUS_LOG=LOG_LOCAL4,
        FTP_LOG=LOG_LOCAL5,
        WEB_LOG=LOG_LOCAL6,
        SNTP_LOG=LOG_LOCAL7,
        DHCP_LOG=LOG_LOCAL0,
        EMAIL_LOG=LOG_MAIL,
    }LOG_MODULE;
private:
    explicit Logger(QObject *parent = 0);

signals:

public slots:

};

class EventLogger : public QObject
{
    Q_OBJECT
public:
    typedef enum Event_Action_Enum{
        Event_Action_Login = 0,
        Event_Action_Logout,
        Event_Action_Newtime,
        Event_Action_Timechanged,
        Event_Action_Poweroff,
        Event_Action_Poweron,
        Event_Action_TRevbegin,
        Event_Action_TRevend,
        Event_Action_SNTP,
        Event_Action_DSTStart,
        Event_Acton_DSTStop
    }Event_Acton;

    typedef enum Event_Factor_Enum{
        Event_Factor_OPERATE = 0,
        Event_Factor_COMMU,
        Event_Factor_REMOTE,
        Event_Factor_ACTION,
        Event_Factor_SYSTEM,
        Event_Factor_SERIAL
    }Event_Factor;

    static EventLogger& getInstance(){static EventLogger instance; return instance;}
    void log(EventLogger::Event_Acton action, EventLogger::Event_Factor factor,QString userName);
    static QString getActionString(QString actionStr);
    static QString getFactorString(QString factorStr);

signals:
    void sigUpdate();

private:
    EventLogger(QObject *parent = 0);

private:
    Logger &lg;
};

class ErrorLogger : public QObject
{
    Q_OBJECT
public:
    static ErrorLogger& getInstance(){static ErrorLogger instance; return instance;}
    void log(int errCode);
    static QString getErrorInfo(QString errCodeStr);

private:
    ErrorLogger(QObject *parent = 0);

signals:
    void sigUpdate();

private:
    Logger &lg;
};

class GeneralLogger : public QObject
{
    Q_OBJECT
public:
    typedef enum General_User_Authnum_Enum{
        General_User_Authnum_E0 = 0,
        General_User_Authnum_E1,
        General_User_Authnum_E2,
        General_User_Authnum_E3,
        General_User_Authnum_S0
    }General_User_Authnum;

    typedef enum General_IO_Symbol_Enum{
        General_IO_Symbol_Rcev = 0,
        General_IO_Symbol_Send,
    }General_IO_Symbol;


    static GeneralLogger& getInstance(){static GeneralLogger instance; return instance;}
    void log(GeneralLogger::General_User_Authnum authNum, GeneralLogger::General_IO_Symbol symbol, QString info);

private:
    GeneralLogger(QObject *parent = 0);

signals:
    void sigUpdate();

private:
    Logger &lg;
};

#endif // LOGGER_H
