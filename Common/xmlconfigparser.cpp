#include "xmlconfigparser.h"
#include "../GlobalData/cfgchannel.h"
#include "../GlobalData/configtreeobject.h"
#include "../GlobalData/cfgconfig_ai.h"
#include "../GlobalData/cfgconfig_di.h"
#include "../GlobalData/cfgconfig_do.h"
#include "../GlobalData/cfgconfig_comm.h"
#include "../GlobalData/cfgconfig_math.h"
#include "../GlobalData/cfgconfig_network.h"
#include "../GlobalData/cfgconfig_event.h"
#include "../GlobalData/cfgconfig_report.h"
#include "../GlobalData/cfgconfig_display.h"
#include "../GlobalData/cfgconfig_serial.h"
#include <QDebug>


XmlConfigParser::XmlConfigParser(QObject *parent) : QObject(parent)
{

}

/**
 * @brief 配置树文件解析，用于创建树形对象
 * @param xmlFile：待解析的文件对象
 * @param parent：根节点
 * @param reciver：代理信号槽绑定用
 */
void XmlConfigParser::parseConfigXml(QFile & xmlFile, ConfigObject *parent, QObject *reciver, quint32 group)
{
    QXmlStreamReader xmlReader(&xmlFile);

    while(!xmlReader.atEnd() && !xmlReader.hasError()) {
            parseConfigElement(xmlReader, parent, reciver, group);
    }

    if (xmlReader.hasError())
        qDebug()<<xmlFile.fileName()<<"Format Error!"<<"["<<__func__<<"]";

    xmlReader.clear();
}

void XmlConfigParser::parseConfigElement(QXmlStreamReader &xmlReader, ConfigObject *parent, QObject *reciver, quint32 group)
{
    ConfigObject *pConfigObj;
    QString name, ID;

    while(!xmlReader.atEnd() && !xmlReader.hasError()) {
        if (xmlReader.isStartElement())
            break;

        xmlReader.readNext();
    }

     if (xmlReader.isStartElement()) {
        if (xmlReader.name() == "Config") {
            //创建对象
            name = xmlReader.attributes().value("Name").toString();
            pConfigObj = new ConfigObject(parent);
            pConfigObj->setObjectName(name);

            if (xmlReader.attributes().hasAttribute("File")) {
                QString fileName = xmlReader.attributes().value("File").toString();
                QFile file(fileName);
                if(!file.open(QFile::ReadOnly | QFile::Text)){
                    qWarning()<< QString("Can't open the file %1").arg(fileName)<<"["<<__func__<<"]";
                }else{
                    parseConfigXml(file, pConfigObj, reciver, group);
                    file.close();
                }
            }
        } else if (xmlReader.name() == "Group") {
            //重复遍历xml
            Q_ASSERT(xmlReader.attributes().hasAttribute("File"));

            QString fileName = xmlReader.attributes().value("File").toString();
            QString groupType = xmlReader.attributes().value("Type").toString();
            if (groupType == "AI-Channel") {
                //AI通道
                foreach (QString chanNum, CfgChannel::instance()->getExistChannelAI().keys()) {
                    pConfigObj = new ConfigObject(parent);
                    pConfigObj->setObjectName(chanNum);
                    quint32 coder = CfgChannel::instance()->getExistChannelAI()[chanNum];
                    QFile file(fileName);
                    if(!file.open(QFile::ReadOnly | QFile::Text)){
                        qWarning()<< QString("Can't open the file %1").arg(fileName)<<"["<<__func__<<"]";
                    }else{
                        parseConfigXml(file, pConfigObj, reciver, coder);
                        file.close();
                    }
                }
            } else if (groupType == "DI-Channel") {
                //DI通道
                foreach (QString chanNum, CfgChannel::instance()->getExistChannelDI().keys()) {
                    pConfigObj = new ConfigObject(parent);
                    pConfigObj->setObjectName(chanNum);
                    quint32 coder = CfgChannel::instance()->getExistChannelDI()[chanNum];
                    QFile file(fileName);
                    if(!file.open(QFile::ReadOnly | QFile::Text)){
                        qWarning()<< QString("Can't open the file %1").arg(fileName)<<"["<<__func__<<"]";
                    }else{
                        parseConfigXml(file, pConfigObj, reciver, coder);
                        file.close();
                    }
                }
            } else if (groupType == "DO-Channel") {
                //DO通道
                foreach (QString chanNum, CfgChannel::instance()->getExistChannelDO().keys()) {
                    pConfigObj = new ConfigObject(parent);
                    pConfigObj->setObjectName(chanNum);
                    quint32 coder = CfgChannel::instance()->getExistChannelDO()[chanNum];
                    QFile file(fileName);
                    if(!file.open(QFile::ReadOnly | QFile::Text)){
                        qWarning()<< QString("Can't open the file %1").arg(fileName)<<"["<<__func__<<"]";
                    }else{
                        parseConfigXml(file, pConfigObj, reciver, coder);
                        file.close();
                    }
                }
            } else if (groupType == "Math-Channel") {
                //MATH通道
                foreach (QString chanNum, CfgChannel::instance()->getExistChannelMath().keys()) {
                    pConfigObj = new ConfigObject(parent);
                    pConfigObj->setObjectName(chanNum);
                    quint32 coder = CfgChannel::instance()->getExistChannelMath()[chanNum];
                    QFile file(fileName);
                    if(!file.open(QFile::ReadOnly | QFile::Text)){
                        qWarning()<< QString("Can't open the file %1").arg(fileName)<<"["<<__func__<<"]";
                    }else{
                        parseConfigXml(file, pConfigObj, reciver, coder);
                        file.close();
                    }
                }
            } else if (groupType == "Comm-Channel") {
                //COMM通道
                foreach (QString chanNum, CfgChannel::instance()->getExistChannelComm().keys()) {
                    pConfigObj = new ConfigObject(parent);
                    pConfigObj->setObjectName(chanNum);
                    quint32 coder = CfgChannel::instance()->getExistChannelComm()[chanNum];
                    QFile file(fileName);
                    if(!file.open(QFile::ReadOnly | QFile::Text)){
                        qWarning()<< QString("Can't open the file %1").arg(fileName)<<"["<<__func__<<"]";
                    }else{
                        parseConfigXml(file, pConfigObj, reciver, coder);
                        file.close();
                    }
                }
            } else if (groupType == "Server") {
                //服务器索引号
                for (int i=0; i<32; ++i) {
                    pConfigObj = new ConfigObject(parent);
                    pConfigObj->setObjectName(QString("%1").arg(i));
                    quint32 coder = i;
                    QFile file(fileName);
                    if(!file.open(QFile::ReadOnly | QFile::Text)){
                        qWarning()<< QString("Can't open the file %1").arg(fileName)<<"["<<__func__<<"]";
                    }else{
                        parseConfigXml(file, pConfigObj, reciver, coder);
                        file.close();
                    }
                }
            } else if ((groupType == "Network-Command") || (groupType == "Serial-Command") || (groupType == "Interval-Switch")){
                //网口/串口命令索引号/内部开关
                for (int i=0; i<100; ++i) {
                    pConfigObj = new ConfigObject(parent);
                    pConfigObj->setObjectName(QString("%1").arg(i));
                    quint32 coder = i;
                    QFile file(fileName);
                    if(!file.open(QFile::ReadOnly | QFile::Text)){
                        qWarning()<< QString("Can't open the file %1").arg(fileName)<<"["<<__func__<<"]";
                    }else{
                        parseConfigXml(file, pConfigObj, reciver, coder);
                        file.close();
                    }
                }
            }  else if ((groupType == "Disp-Group") || (groupType == "Event-Action") || (groupType == "User-settings")) {
                //显示设定-分组号/事件动作分组号/安全设定-用户分组号
                for (int i=0; i<50; ++i) {
                    pConfigObj = new ConfigObject(parent);
                    pConfigObj->setObjectName(QString("%1").arg(i));
                    quint32 coder = i;
                    QFile file(fileName);
                    if(!file.open(QFile::ReadOnly | QFile::Text)){
                        qWarning()<< QString("Can't open the file %1").arg(fileName)<<"["<<__func__<<"]";
                    }else{
                        parseConfigXml(file, pConfigObj, reciver, coder);
                        file.close();
                    }
                }
            } else if (groupType == "Batch-Text") {
                //批处理文本索引号
                for (int i=0; i<24; ++i) {
                    pConfigObj = new ConfigObject(parent);
                    pConfigObj->setObjectName(QString("%1").arg(i));
                    quint32 coder = i;
                    QFile file(fileName);
                    if(!file.open(QFile::ReadOnly | QFile::Text)){
                        qWarning()<< QString("Can't open the file %1").arg(fileName)<<"["<<__func__<<"]";
                    }else{
                        parseConfigXml(file, pConfigObj, reciver, coder);
                        file.close();
                    }
                }
            } else if (groupType == "Report-Channel") {
                //报表索引号
                for (int i=0; i<60; ++i) {
                    pConfigObj = new ConfigObject(parent);
                    pConfigObj->setObjectName(QString("%1").arg(i));
                    quint32 coder = i;
                    QFile file(fileName);
                    if(!file.open(QFile::ReadOnly | QFile::Text)){
                        qWarning()<< QString("Can't open the file %1").arg(fileName)<<"["<<__func__<<"]";
                    }else{
                        parseConfigXml(file, pConfigObj, reciver, coder);
                        file.close();
                    }
                }
            }  else if (groupType == "User-property") {
                //权限分组号
                for (int i=0; i<10; ++i) {
                    pConfigObj = new ConfigObject(parent);
                    pConfigObj->setObjectName(QString("%1").arg(i));
                    quint32 coder = i;
                    QFile file(fileName);
                    if(!file.open(QFile::ReadOnly | QFile::Text)){
                        qWarning()<< QString("Can't open the file %1").arg(fileName)<<"["<<__func__<<"]";
                    }else{
                        parseConfigXml(file, pConfigObj, reciver, coder);
                        file.close();
                    }
                }
            }

        } else if (xmlReader.name() == "Object") {
            //创建对象
            pConfigObj = new ConfigObject(parent);
            name = xmlReader.attributes().value("Name").toString();
            pConfigObj->setObjectName(name);

            //初始化
            if (xmlReader.attributes().hasAttribute("ID")) {
                ID = xmlReader.attributes().value("ID").toString();
                quint32 ID_obj = ID.toUInt();

                QList<quint32> ID_List;
                QList<quint32>::iterator i_ID;
                /*独立标记，即值的改变不会修改父对象的更改标记*/
                ID_List.clear();
                ID_List << ID_AI_Last_CH << ID_AI_First_CH                          // AI
                        << ID_DI_Last_CH << ID_DI_First_CH                          // DI
                        << ID_DO_Last_CH << ID_DO_First_CH                          // DO
                        << ID_Comm_Last_CH << ID_Comm_First_CH                      // Comm
                        << ID_Math_LastCH << ID_Math_FirstCH
                        << ID_Math_Const_NumVal                                     // Math
                        << ID_NetWork_Modbus_CmdNO << ID_NetWork_Modbus_ServerNO    // Network
                        << ID_Event_Num << ID_Report_Ch_Num                         // Event/Report
                        << ID_Disp_Grp_Number << ID_Disp_Msg_GrpNum                 // Display
                        << ID_Serial_Modbus_Cmd_ServerNum << ID_Batch_Text_Text_Num // Serial/Batch
                        << ID_System_Switch_FirNum << ID_System_Switch_LastNum      // Interval Switch
                        << ID_Security_UserProPerty_AuthNum
                        << ID_Security_UserSetting_UserNum;                         // Security
                for(i_ID = ID_List.begin();i_ID!=ID_List.end();i_ID++) {
                    if(ID_obj == *i_ID) {
                        pConfigObj->setStandAloneFlag(true);
                        break;
                    }
                }

                QVariant value = CfgConfig::instance()->getConfigData(ID_obj, group);
                pConfigObj->initData(value, group, ID_obj);
            }
        } else if (xmlReader.name() == "Delegate") {
            //创建对象
            pConfigObj = new ConfigObject(parent);
            name = xmlReader.attributes().value("Name").toString();
            pConfigObj->setObjectName(name);

            ID = xmlReader.attributes().value("ID").toString();
            quint32 ID_obj =  ID.toUInt();

            QList<quint32> ID_List;
            QList<quint32>::iterator i_ID;
            //color配置项属性变更
            ID_List.clear();
            ID_List << ID_AI_Disp_Color_Value << ID_AI_DISP_ColorScaleBand_Color
                    << ID_AI_DISP_AlarmMark_Color1 << ID_AI_DISP_AlarmMark_Color2
                    << ID_AI_DISP_AlarmMark_Color3 << ID_AI_DISP_AlarmMark_Color4
                    << ID_DO_Disp_Color_Value << ID_DI_Disp_Color_Value
                    << ID_DI_DISP_AlarmMark_Color1 << ID_DI_DISP_AlarmMark_Color2
                    << ID_DI_DISP_AlarmMark_Color3 << ID_DI_DISP_AlarmMark_Color4
                    << ID_Comm_Disp_Color_Value << ID_Comm_DISP_ColorScaleBand_Color
                    << ID_Comm_DISP_AlarmMark_Color1 << ID_Comm_DISP_AlarmMark_Color2
                    << ID_Comm_DISP_AlarmMark_Color3 << ID_Comm_DISP_AlarmMark_Color4
                    << ID_Disp_Grp_Set_Scale1_Color << ID_Disp_Grp_Set_Scale2_Color
                    << ID_Disp_Grp_Set_Scale3_Color << ID_Disp_Grp_Set_Scale4_Color
                    << ID_Math_Disp_Color_Value <<  ID_Math_DISP_ColorScaleBand_Color
                    << ID_Math_DISP_AlarmMark_Color1 << ID_Math_DISP_AlarmMark_Color2
                    << ID_Math_DISP_AlarmMark_Color3 << ID_Math_DISP_AlarmMark_Color4 ;
            for(i_ID = ID_List.begin();i_ID!=ID_List.end();i_ID++) {
                if(ID_obj == *i_ID) {
                    pConfigObj->setProperty("colorType", true);
                    break;
                }
            }

            /*独立标记，即值的改变不会修改父对象的更改标记*/
            ID_List.clear();
            ID_List << ID_AI_Last_CH << ID_AI_First_CH                          // AI
                    << ID_DI_Last_CH << ID_DI_First_CH                          // DI
                    << ID_DO_Last_CH << ID_DO_First_CH                          // DO
                    << ID_Comm_Last_CH << ID_Comm_First_CH                      // Comm
                    << ID_Math_LastCH << ID_Math_FirstCH
                    << ID_Math_Const_NumVal                                     // Math
                    << ID_NetWork_Modbus_CmdNO << ID_NetWork_Modbus_ServerNO    // Network
                    << ID_Event_Num << ID_Report_Ch_Num                         // Event/Report
                    << ID_Disp_Grp_Number << ID_Disp_Msg_GrpNum                 // Display
                    << ID_Serial_Modbus_Cmd_ServerNum << ID_Batch_Text_Text_Num // Serial/Batch
                    << ID_System_Switch_FirNum << ID_System_Switch_LastNum      // Interval Switch
                    << ID_Security_UserProPerty_AuthNum
                    << ID_Security_UserSetting_UserNum;                         // Security
            for(i_ID = ID_List.begin();i_ID!=ID_List.end();i_ID++) {
                if(ID_obj == *i_ID) {
                    pConfigObj->setStandAloneFlag(true);
                    break;
                }
            }

            QString showName = CfgConfig::instance()->getObjStr(ID_obj);
            pConfigObj->setStrObjName(showName);

            //信号绑定
            if (xmlReader.attributes().hasAttribute("Slot1")) {
                std::string slot = QString("1%1").arg(xmlReader.attributes().value("Slot1").toString()).toStdString();
                connect(pConfigObj, SIGNAL(sigDataChange(quint32)), reciver, slot.data());
            }
            if (xmlReader.attributes().hasAttribute("Slot2")) {
                std::string slot = QString("1%1").arg(xmlReader.attributes().value("Slot2").toString()).toStdString();
                connect(pConfigObj, SIGNAL(sigDataChange(quint32)), reciver, slot.data());
            }
            if (xmlReader.attributes().hasAttribute("Slot3")) {
                std::string slot = QString("1%1").arg(xmlReader.attributes().value("Slot3").toString()).toStdString();
                connect(pConfigObj, SIGNAL(sigDataChange(quint32)), reciver, slot.data());
            }

            //子文件解析
            if (xmlReader.attributes().hasAttribute("File")) {
                QString fileName = xmlReader.attributes().value("File").toString();
                QFile file(fileName);
                if(!file.open(QFile::ReadOnly | QFile::Text)){
                    qWarning()<< QString("Can't open the file %1").arg(fileName)<<"["<<__func__<<"]";
                }else{
                    parseConfigXml(file, pConfigObj, reciver, group);
                    file.close();
                }
            }
        } else if (xmlReader.name() == "Root") {
            pConfigObj = parent;
            //Do nothing
        } else {
            qDebug("Error: unknown Element");
            xmlReader.skipCurrentElement();
        }
     } else {
         if (!xmlReader.atEnd())
             qDebug()<<"XML Config Error at:"<<xmlReader.lineNumber()<<"["<<__func__<<"]";
         return;
     }

     while(!xmlReader.atEnd() && !xmlReader.hasError()) {
         xmlReader.readNext();

         if (xmlReader.isStartElement()) {
             parseConfigElement(xmlReader, pConfigObj, reciver, group);
         } else if (xmlReader.isEndElement()) {
             return;
         }
     }

//     qDebug()<<"2 XML Config Error!";
}

void XmlConfigParser::parseDelegateChangeXml(QFile &xmlFile, quint32 groupFirst, quint32 groupLast)
{
    QXmlStreamReader xmlReader(&xmlFile);

    while(!xmlReader.atEnd() && !xmlReader.hasError()) {
        parseDelegateChangeElement(xmlReader, groupFirst, groupLast);
    }

    if (xmlReader.hasError())
        qDebug()<<xmlFile.fileName()<<"Format Error!"<<"["<<__func__<<"]";

    xmlReader.clear();
}

void XmlConfigParser::parseDelegateChangeElement(QXmlStreamReader &xmlReader,quint32 groupFirst, quint32 groupLast)
{
    while(!xmlReader.atEnd() && !xmlReader.hasError()) {
        if (xmlReader.isStartElement())
            break;

        xmlReader.readNext();
    }

    if (xmlReader.isStartElement()) {
        if (xmlReader.name() == "DelegateChange") {
           Q_ASSERT(xmlReader.attributes().hasAttribute("Type") &&\
                             xmlReader.attributes().hasAttribute("Name") &&\
                             xmlReader.attributes().hasAttribute("Objects"));

            if (xmlReader.attributes().value("Type") == "Unique") {      //特殊，一对一
                    QList<ConfigObject*> delegateObjectList;
                    QString deleName = xmlReader.attributes().value("Name").toString();
                    QString deleObject = xmlReader.attributes().value("Objects").toString();
                    ConfigObject *pDelegate = ConfigTreeObject::getConfigObject(deleName);
                    ConfigObject *pDelegateObj = ConfigTreeObject::getConfigObject(deleObject);
                    delegateObjectList.append(pDelegateObj);
                    pDelegate->setDelegateObj(delegateObjectList);
            } else if (xmlReader.attributes().value("Type") == "AI-Channel") {  //AI通道
                if (!CfgChannel::instance()->getExistChannelAI().isEmpty()) {
                    int indexFirst = CfgChannel::instance()->getExistChannelAI().values().indexOf(groupFirst);
                    int indexLast = CfgChannel::instance()->getExistChannelAI().values().indexOf(groupLast);
                    QList<ConfigObject*> delegateObjectList;
                    QString deleName = xmlReader.attributes().value("Name").toString();

                    for (int index=indexFirst; index<=indexLast; index++) {
                        QString deleObject = xmlReader.attributes().value("Objects").toString();
                        QString defaultCoder = CfgChannel::instance()->getExistChannelAI().keys().at(index);
                        QString objPath = deleObject.replace("%1", defaultCoder);
                        ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(objPath);
                        delegateObjectList.append(pConfigObj);
                    }
                    ConfigTreeObject::getConfigObject(deleName)->setDelegateObj(delegateObjectList);
                }
            } else if (xmlReader.attributes().value("Type") == "DI-Channel") {  //DI通道
                if (!CfgChannel::instance()->getExistChannelDI().isEmpty()) {
                    int indexFirst = CfgChannel::instance()->getExistChannelDI().values().indexOf(groupFirst);
                    int indexLast = CfgChannel::instance()->getExistChannelDI().values().indexOf(groupLast);
                    QList<ConfigObject*> delegateObjectList;
                    QString deleName = xmlReader.attributes().value("Name").toString();

                    for (int index=indexFirst; index<=indexLast; index++) {
                        QString deleObject = xmlReader.attributes().value("Objects").toString();
                        QString defaultCoder = CfgChannel::instance()->getExistChannelDI().keys().at(index);
                        QString objPath = deleObject.replace("%1", defaultCoder);
                        ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(objPath);
                        delegateObjectList.append(pConfigObj);
                    }
                    ConfigTreeObject::getConfigObject(deleName)->setDelegateObj(delegateObjectList);
                }
            } else if (xmlReader.attributes().value("Type") == "DO-Channel") {  //DO通道
                if (!CfgChannel::instance()->getExistChannelDO().isEmpty()) {
                    int indexFirst = CfgChannel::instance()->getExistChannelDO().values().indexOf(groupFirst);
                    int indexLast = CfgChannel::instance()->getExistChannelDO().values().indexOf(groupLast);
                    QList<ConfigObject*> delegateObjectList;
                    QString deleName = xmlReader.attributes().value("Name").toString();

                    for (int index=indexFirst; index<=indexLast; index++) {
                        QString deleObject = xmlReader.attributes().value("Objects").toString();
                        QString defaultCoder = CfgChannel::instance()->getExistChannelDO().keys().at(index);
                        QString objPath = deleObject.replace("%1", defaultCoder);
                        ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(objPath);
                        delegateObjectList.append(pConfigObj);
                    }
                    ConfigTreeObject::getConfigObject(deleName)->setDelegateObj(delegateObjectList);
                }
            } else if (xmlReader.attributes().value("Type") == "Math-Channel") {    //MATH通道
                if (!CfgChannel::instance()->getExistChannelMath().isEmpty()) {
                    int indexFirst = CfgChannel::instance()->getExistChannelMath().values().indexOf(groupFirst);
                    int indexLast = CfgChannel::instance()->getExistChannelMath().values().indexOf(groupLast);
                    QList<ConfigObject*> delegateObjectList;
                    QString deleName = xmlReader.attributes().value("Name").toString();

                    for (int index=indexFirst; index<=indexLast; index++) {
                        QString deleObject = xmlReader.attributes().value("Objects").toString();
                        QString defaultCoder = CfgChannel::instance()->getExistChannelMath().keys().at(index);
                        QString objPath = deleObject.replace("%1", defaultCoder);
                        ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(objPath);
                        delegateObjectList.append(pConfigObj);
                    }
                    ConfigTreeObject::getConfigObject(deleName)->setDelegateObj(delegateObjectList);
                }
            } else if (xmlReader.attributes().value("Type") == "Comm-Channel") {    //COMM通道
                if (!CfgChannel::instance()->getExistChannelComm().isEmpty()) {
                    int indexFirst = CfgChannel::instance()->getExistChannelComm().values().indexOf(groupFirst);
                    int indexLast = CfgChannel::instance()->getExistChannelComm().values().indexOf(groupLast);
                    QList<ConfigObject*> delegateObjectList;
                    QString deleName = xmlReader.attributes().value("Name").toString();

                    for (int index=indexFirst; index<=indexLast; index++) {
                        QString deleObject = xmlReader.attributes().value("Objects").toString();
                        QString defaultCoder = CfgChannel::instance()->getExistChannelComm().keys().at(index);
                        QString objPath = deleObject.replace("%1", defaultCoder);
                        ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(objPath);
                        delegateObjectList.append(pConfigObj);
                    }
                    ConfigTreeObject::getConfigObject(deleName)->setDelegateObj(delegateObjectList);
                }
            } else if (xmlReader.attributes().value("Type") == "Server") {     //服务器索引号
                QList<ConfigObject*> delegateObjectList;
                QString deleName = xmlReader.attributes().value("Name").toString();
                QString deleObject = xmlReader.attributes().value("Objects").toString();
                QString defaultCoder = ConfigTreeObject::getConfigData(NETWORK_MODBUS_Deledate_SERVER_NUMBER).toString();
                QString objPath = deleObject.replace("%1", defaultCoder);

                ConfigObject *pDelegate = ConfigTreeObject::getConfigObject(deleName);
                ConfigObject *pDelegateObj = ConfigTreeObject::getConfigObject(objPath);
                delegateObjectList.append(pDelegateObj);
                pDelegate->setDelegateObj(delegateObjectList);
            } else if (xmlReader.attributes().value("Type") == "Network-Command") { //网口命令索引号
                QList<ConfigObject*> delegateObjectList;
                QString deleName = xmlReader.attributes().value("Name").toString();
                QString deleObject = xmlReader.attributes().value("Objects").toString();
                QString defaultCoder = ConfigTreeObject::getConfigData(NETWORK_MODBUS_Deledate_COMMAND_NUMBER).toString();
                QString objPath = deleObject.replace("%1", defaultCoder);

                ConfigObject *pDelegate = ConfigTreeObject::getConfigObject(deleName);
                ConfigObject *pDelegateObj = ConfigTreeObject::getConfigObject(objPath);
                delegateObjectList.append(pDelegateObj);
                pDelegate->setDelegateObj(delegateObjectList);
            } else if (xmlReader.attributes().value("Type") == "Serial-Command") {  //串口命令索引号
                QList<ConfigObject*> delegateObjectList;
                QString deleName = xmlReader.attributes().value("Name").toString();
                QString deleObject = xmlReader.attributes().value("Objects").toString();
                QString defaultCoder = ConfigTreeObject::getConfigData(SERIAL_DELEGATE_COMMAND_NUM).toString();
                QString objPath = deleObject.replace("%1", defaultCoder);

                ConfigObject *pDelegate = ConfigTreeObject::getConfigObject(deleName);
                ConfigObject *pDelegateObj = ConfigTreeObject::getConfigObject(objPath);
                delegateObjectList.append(pDelegateObj);
                pDelegate->setDelegateObj(delegateObjectList);
            } else if (xmlReader.attributes().value("Type") == "Disp-Group") {  //显示设定-分组号
                QList<ConfigObject*> delegateObjectList;
                QString deleName = xmlReader.attributes().value("Name").toString();
                QString deleObject = xmlReader.attributes().value("Objects").toString();
                QString defaultCoder = ConfigTreeObject::getConfigData(DISP_DELEGATE_GRPNUM).toString();
                QString objPath = deleObject.replace("%1", defaultCoder);

                ConfigObject *pDelegate = ConfigTreeObject::getConfigObject(deleName);
                ConfigObject *pDelegateObj = ConfigTreeObject::getConfigObject(objPath);
                delegateObjectList.append(pDelegateObj);
                pDelegate->setDelegateObj(delegateObjectList);
            } else if (xmlReader.attributes().value("Type") == "Interval-Switch") {
                    int indexFirst = ConfigTreeObject::getConfigObject(System_Delegate_Internal_First)->getData().toInt();
                    int indexLast = ConfigTreeObject::getConfigObject(System_Delegate_Internal_Last)->getData().toInt();
                    QList<ConfigObject*> delegateObjectList;
                    QString deleName = xmlReader.attributes().value("Name").toString();

                    for (int index=indexFirst; index<=indexLast; index++) {
                        QString deleObject = xmlReader.attributes().value("Objects").toString();
                        QString defaultCoder = QString("%1").arg(index);
                        QString objPath = deleObject.replace("%1", defaultCoder);
                        ConfigObject *pConfigObj = ConfigTreeObject::getConfigObject(objPath);
                        delegateObjectList.append(pConfigObj);
                    }
                    ConfigTreeObject::getConfigObject(deleName)->setDelegateObj(delegateObjectList);
            } else if (xmlReader.attributes().value("Type") == "Batch-Text") {
                QList<ConfigObject*> delegateObjectList;
                QString deleName = xmlReader.attributes().value("Name").toString();
                QString deleObject = xmlReader.attributes().value("Objects").toString();
                QString defaultCoder = ConfigTreeObject::getConfigObject(Config_Batch_Delegate_TextNum)->getData().toString();
                QString objPath = deleObject.replace("%1", defaultCoder);

                ConfigObject *pDelegate = ConfigTreeObject::getConfigObject(deleName);
                ConfigObject *pDelegateObj = ConfigTreeObject::getConfigObject(objPath);
                delegateObjectList.append(pDelegateObj);
                pDelegate->setDelegateObj(delegateObjectList);
            } else if (xmlReader.attributes().value("Type") == "Report-Channel") {
                QList<ConfigObject*> delegateObjectList;
                QString deleName = xmlReader.attributes().value("Name").toString();
                QString deleObject = xmlReader.attributes().value("Objects").toString();
                QString defaultCoder = ConfigTreeObject::getConfigObject(Config_Report_Delegate_ReportChannnel_Number)->getData().toString();
                QString objPath = deleObject.replace("%1", defaultCoder);

                ConfigObject *pDelegate = ConfigTreeObject::getConfigObject(deleName);
                ConfigObject *pDelegateObj = ConfigTreeObject::getConfigObject(objPath);
                delegateObjectList.append(pDelegateObj);
                pDelegate->setDelegateObj(delegateObjectList);
            } else if (xmlReader.attributes().value("Type") == "Event-Action") {
                QList<ConfigObject*> delegateObjectList;
                QString deleName = xmlReader.attributes().value("Name").toString();
                QString deleObject = xmlReader.attributes().value("Objects").toString();
                QString defaultCoder = ConfigTreeObject::getConfigObject(EVENT_DELEGATE_GRPNUM)->getData().toString();
                QString objPath = deleObject.replace("%1", defaultCoder);

                ConfigObject *pDelegate = ConfigTreeObject::getConfigObject(deleName);
                ConfigObject *pDelegateObj = ConfigTreeObject::getConfigObject(objPath);
                delegateObjectList.append(pDelegateObj);
                pDelegate->setDelegateObj(delegateObjectList);
            } else if (xmlReader.attributes().value("Type") == "User-settings") {
                QList<ConfigObject*> delegateObjectList;
                QString deleName = xmlReader.attributes().value("Name").toString();
                QString deleObject = xmlReader.attributes().value("Objects").toString();
                QString defaultCoder = ConfigTreeObject::getConfigObject(SECURITY_DELEGATE_SETTING_USERNUM)->getData().toString();
                QString objPath = deleObject.replace("%1", defaultCoder);

                ConfigObject *pDelegate = ConfigTreeObject::getConfigObject(deleName);
                ConfigObject *pDelegateObj = ConfigTreeObject::getConfigObject(objPath);
                delegateObjectList.append(pDelegateObj);
                pDelegate->setDelegateObj(delegateObjectList);
            } else if (xmlReader.attributes().value("Type") == "User-property") {
                QList<ConfigObject*> delegateObjectList;
                QString deleName = xmlReader.attributes().value("Name").toString();
                QString deleObject = xmlReader.attributes().value("Objects").toString();
                QString defaultCoder = ConfigTreeObject::getConfigObject(SECURITY_DELEGATE_PROPERTY_NUM)->getData().toString();
                QString objPath = deleObject.replace("%1", defaultCoder);

                ConfigObject *pDelegate = ConfigTreeObject::getConfigObject(deleName);
                ConfigObject *pDelegateObj = ConfigTreeObject::getConfigObject(objPath);
                delegateObjectList.append(pDelegateObj);
                pDelegate->setDelegateObj(delegateObjectList);
            } else if (xmlReader.attributes().value("Type") == "Module") {
                QList<ConfigObject*> delegateObjectList;
                QString deleName = xmlReader.attributes().value("Name").toString();
                QString deleObject = xmlReader.attributes().value("Objects").toString();
                QString objPath;
                switch(groupFirst) {
                case 0://main unit
                    objPath = deleObject.replace("%1", "Main Unit");
                    break;
                case 1://EXT unit 1 ~ 6
                case 2:
                case 3:
                case 4:
                case 5:
                case 6: {
                    QString str_1 = QString("EXT Unit %1").arg(groupFirst);
                    objPath = deleObject.replace("%1", str_1);
                }
                    break;
                default:
                    objPath = deleObject.replace("%1", "Main Unit");
                    break;
                }
                QString str_2 = QString("Unit %1 Module %2").arg(groupFirst).arg(groupLast);
                objPath = deleObject.replace("%2", str_2);

                ConfigObject *pDelegate = ConfigTreeObject::getConfigObject(deleName);
                ConfigObject *pDelegateObj = ConfigTreeObject::getConfigObject(objPath);
                delegateObjectList.append(pDelegateObj);
                pDelegate->setDelegateObj(delegateObjectList);
            }

        } else if (xmlReader.name() == "Root"){
            //Do nothing
        } else {
            qDebug("Error: unknown Element");
            xmlReader.skipCurrentElement();
        }
    } else {
        if (!xmlReader.atEnd())
            qDebug()<<"XML Config Error at:"<<xmlReader.lineNumber()<<"["<<__func__<<"]";
        return;
    }

    while(!xmlReader.atEnd() && !xmlReader.hasError()) {
        xmlReader.readNext();

        if (xmlReader.isStartElement()) {
            parseDelegateChangeElement(xmlReader, groupFirst, groupLast);
        } else if (xmlReader.isEndElement()) {
            return;
        }
    }

//    qDebug()<<"XML Config Error!";
}

void XmlConfigParser::parseValueChangeXml(QFile &xmlFile, QVariant curValue, quint32 group)
{
    QXmlStreamReader xmlReader(&xmlFile);

    while(!xmlReader.atEnd() && !xmlReader.hasError()) {
        parseValueChangeElement(xmlReader, curValue, group);
    }

    if (xmlReader.hasError())
        qDebug()<<xmlFile.fileName()<<"Format Error!"<<"["<<__func__<<"]";

    xmlReader.clear();
}

void XmlConfigParser::parseValueChangeElement(QXmlStreamReader &xmlReader, QVariant curValue, quint32 group)
{
    QString name,ID;
    QString hideflag;
    QString changdata;
    ConfigObject *pConfigObj;

    while(!xmlReader.atEnd() && !xmlReader.hasError()) {
        if (xmlReader.isStartElement())
            break;
        xmlReader.readNext();
    }

    if (xmlReader.isStartElement()) {
        if (xmlReader.name() == "ValueChange") {
            name = xmlReader.attributes().value("Name").toString();
            ID = xmlReader.attributes().value("ID").toString();

            pConfigObj= ConfigTreeObject::getConfigObject(name);
            if (xmlReader.attributes().hasAttribute("Val")) {   //enum change
                if (xmlReader.attributes().value("Val") == curValue.toString()) {
                    QString valueStr = CfgConfig::instance()->getValObjectStr(ID.toUInt(), group, pConfigObj);
                    pConfigObj->setStrShowOn(valueStr);
                }else{
                    xmlReader.skipCurrentElement();
                }
            }else{      //value float\char\double change
                QString valueStr = CfgConfig::instance()->getValObjectStr(ID.toUInt(), group, pConfigObj);
                pConfigObj->setStrShowOn(valueStr);
            }
        } else if (xmlReader.name() == "Root") {
            //do nothing
        } else if (xmlReader.name() == "Delegate") {
            name = xmlReader.attributes().value("Name").toString();
            hideflag = xmlReader.attributes().value("Hide").toString();
            if (xmlReader.attributes().hasAttribute("Value")) {
                changdata = xmlReader.attributes().value("Value").toString();
            }

            //某配置项值变更影响其他项
            if("true" == hideflag) {    //其他项：隐藏
                ConfigTreeObject::getConfigObject(name)->setHideFlag(true);
            }
            else if("false" == hideflag) {  //其他项：1.class、title、group项显示；2.normal项显示并刷新为给定值
                ConfigTreeObject::getConfigObject(name)->setHideFlag(false);
                ConfigTreeObject::getConfigObject(name)->changeData(changdata, false);
            }
            else if("refresh" == hideflag) {    //其他项：normal项显示并强制刷新为给定值，不论新旧值是否相同
                ConfigTreeObject::getConfigObject(name)->setHideFlag(false);
                ConfigTreeObject::getConfigObject(name)->changeData(changdata, false, false);
            }
            else if("slot" == hideflag) {   //其他项：normal项显示，该值需从配置树上获得
                ConfigTreeObject::getConfigObject(name)->setHideFlag(false);
                quint32 channelcoder = ConfigTreeObject::getConfigObject(name)->getChannelCoder();
                ConfigTreeObject::getConfigObject(name)->sigDataChange(channelcoder);
            }
            else {
                QFile file(hideflag);
                if(!file.open(QFile::ReadOnly | QFile::Text)){
                    qWarning()<< QString("Can't open the file %1").arg(hideflag)<<"["<<__func__<<"]";
                }else{
                    parseHideXml(file, group);
                    file.close();
                }
            }
        } else {
            qDebug("Error: unknown Element");
            xmlReader.skipCurrentElement();
        }
    } else {
        if (!xmlReader.atEnd())
            qDebug()<<"XML Config Error at:"<<xmlReader.lineNumber()<<"["<<__func__<<"]";
        return;
    }

    while(!xmlReader.atEnd() && !xmlReader.hasError()) {
        xmlReader.readNext();

        if (xmlReader.isStartElement()) {
            parseValueChangeElement(xmlReader, curValue, group);
        } else if (xmlReader.isEndElement()) {
            return;
        }
    }
}

void XmlConfigParser::parseHideXml(QFile &xmlFile, quint32 group)
{
    QXmlStreamReader xmlReader(&xmlFile);

    while(!xmlReader.atEnd() && !xmlReader.hasError()) {
            parseHideElement(xmlReader, group);
    }

    if (xmlReader.hasError())
        qDebug()<<xmlFile.fileName()<<"Format Error!"<<"["<<__func__<<"]";

    xmlReader.clear();
}

void XmlConfigParser::parseHideElement(QXmlStreamReader &xmlReader, quint32 group)
{
    QString path1, path2, path3, path4;
    QString val1, val2, val3, val4;

    while(!xmlReader.atEnd() && !xmlReader.hasError()) {
        if (xmlReader.isStartElement())
            break;

        xmlReader.readNext();
    }

    if (xmlReader.isStartElement()) {
        if (xmlReader.name() == "HideChange") {
            if (xmlReader.attributes().hasAttribute("Normal1")) {
                path1 = xmlReader.attributes().value("Normal1").toString();
                val1 = xmlReader.attributes().value("Value1").toString();
                if(ConfigTreeObject::getConfigData(path1).toString() != val1) {
                    xmlReader.skipCurrentElement();
                }
            }
            if (xmlReader.attributes().hasAttribute("Normal2")) {
                path2 = xmlReader.attributes().value("Normal2").toString();
                val2 = xmlReader.attributes().value("Value2").toString();
                if(ConfigTreeObject::getConfigData(path2).toString() != val2) {
                    xmlReader.skipCurrentElement();
                }
            }
            if (xmlReader.attributes().hasAttribute("Normal3")) {
                path3 = xmlReader.attributes().value("Normal3").toString();
                val3 = xmlReader.attributes().value("Value3").toString();
                if(ConfigTreeObject::getConfigData(path3).toString() != val3) {
                    xmlReader.skipCurrentElement();
                }
            }
            if (xmlReader.attributes().hasAttribute("Normal4")) {
                path4 = xmlReader.attributes().value("Normal4").toString();
                val4 = xmlReader.attributes().value("Value4").toString();
                if(ConfigTreeObject::getConfigData(path4).toString() != val4) {
                    xmlReader.skipCurrentElement();
                }
            }
        } else if (xmlReader.name() == "Root") {
            //do nothing
        } else if (xmlReader.name() == "Factor") {
            QString name = xmlReader.attributes().value("Name").toString();
            QString hideflag = xmlReader.attributes().value("Hide").toString();
            QString changdata = xmlReader.attributes().value("Value").toString();

            if("true" == hideflag) {
                ConfigTreeObject::getConfigObject(name)->setHideFlag(true);
            } else {
                ConfigTreeObject::getConfigObject(name)->setHideFlag(false);
                quint32 channelcoder = ConfigTreeObject::getConfigObject(name)->getChannelCoder();
                ConfigTreeObject::getConfigObject(name)->sigDataChange(channelcoder);
            }
        }
    } else {
        if (!xmlReader.atEnd())
            qDebug()<<"XML Hide Flag Error!";
        return;
    }

    while(!xmlReader.atEnd() && !xmlReader.hasError()) {
        xmlReader.readNext();

        if (xmlReader.isStartElement()) {
            parseHideElement(xmlReader, group);
        } else if (xmlReader.isEndElement()) {
            return;
        }
    }
}
