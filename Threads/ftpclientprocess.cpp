#include "ftpclientprocess.h"

FTPClientProcess::FTPClientProcess(QObject *parent) :
    QObject(parent)
{
    m_mode = "";
    m_localDir = "/var/log/local/";
    m_remoteDir = "/home/documents/";
    m_remoteAddr = "127.0.0.1";
    m_user = "anonymous";
    m_passwd = "123456";
}


void FTPClientProcess::slotUpload()
{
    if(m_ftp.state() == QProcess::NotRunning){
        m_ftp.start("ftp_client.sh",QStringList()<<m_user<<m_passwd<<m_remoteAddr<<m_remoteDir<<m_localDir<<m_mode);
    }
}
