#ifndef XMLCONFIGPARSER_H
#define XMLCONFIGPARSER_H

#include <QObject>
#include <QXmlStreamReader>
#include <QFile>

#include "../GlobalData/configobject.h"
#include "../GlobalData/cfgconfig.h"

class XmlConfigParser : public QObject
{
    Q_OBJECT
public:
    explicit XmlConfigParser(QObject *parent = 0);

    //配置树构建文件解析（包括普通配置、代理配置）
    static void parseConfigXml(QFile &xmlFile, ConfigObject *parent, QObject *reciver=0, quint32 group=0);
    static void parseConfigElement(QXmlStreamReader &xmlReader, ConfigObject *parent, QObject *reciver, quint32 group);

    //代理变更文件解析
    static void parseDelegateChangeXml(QFile &xmlFile, quint32 groupFirst=0, quint32 groupLast=0);
    static void parseDelegateChangeElement(QXmlStreamReader &xmlReader, quint32 groupFirst, quint32 groupLast);

    //代理值变更文件解析
    static void parseValueChangeXml(QFile &xmlFile, QVariant curValue, quint32 group=0);
    static void parseValueChangeElement(QXmlStreamReader &xmlReader, QVariant curValue, quint32 group);

    //某些配置项目隐藏与否依赖文件解析
    static void parseHideXml(QFile &xmlFile, quint32 group=0);
    static void parseHideElement(QXmlStreamReader &xmlReader, quint32 group);
signals:

public slots:
};

#endif // XMLCONFIGPARSER_H
