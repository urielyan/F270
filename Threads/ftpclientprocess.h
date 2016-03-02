#ifndef FTPCLIENTPROCESS_H
#define FTPCLIENTPROCESS_H

#include <QObject>
#include <QProcess>

class FTPClientProcess : public QObject
{
    Q_OBJECT
public:
    explicit FTPClientProcess(QObject *parent = 0);

signals:

public slots:
    void slotUpload();


private:
    QString m_mode;
    QString m_localDir;
    QString m_remoteDir;
    QString m_remoteAddr;
    QString m_user;
    QString m_passwd;
    QProcess m_ftp;
};

#endif // FTPCLIENTPROCESS_H
