/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：ConfigTreeObject.h
 * 概   要：配置树对象，根据系统配置（DevConfigPtr）构建配置树对象单列，并提供
 * 接口供外部对不同系统配置的访问
 *
 * 当前版本：V1.0.0
 * 作   者：王 品
 * 完成日期：2015-8-15
 *
 * 修改版本：
 * 修改说明：
 * 修改作者：
 *
 ********************************************************************/
#ifndef CONFIGTREEOBJECT_H
#define CONFIGTREEOBJECT_H

#include "configobject.h"

#include "configtreeobject_AI.h"
#include "configtreeobject_Batch.h"
#include "configtreeobject_Comm.h"
#include "configtreeobject_DataSave.h"
#include "configtreeobject_DI.h"
#include "configtreeobject_Display.h"
#include "configtreeobject_DO.h"
#include "configtreeobject_EventAction.h"
#include "configtreeobject_Math.h"
#include "configtreeobject_Measure.h"
#include "configtreeobject_Network.h"
#include "configtreeobject_Record.h"
#include "configtreeobject_Report.h"
#include "configtreeobject_SericalClient.h"
#include "configtreeobject_Sys.h"
#include "configtreeobject_Timer.h"
#include "configtreeobject_Security.h"

#include <QDebug>
#include <QStack>
#include <QFile>
#include <QColor>

#include "../Common/xmlconfigparser.h"

class ConfigTreeObject : public ConfigObject
{
    Q_OBJECT
public:
    static ConfigTreeObject *instance();
    static QVariant getConfigData(QString conPath);
    static ConfigObject *getConfigObject(QString conPath);

    static void recoverData();              //丢弃当前配置变更，恢复之前的配置
    static void implementData();            //使当前配置生效

    static void destroyConfigTree(QString conPath);        //销毁配置树某节点下的所有子节点
    static void printTreeStructInfo();

    static void start(){  qDebug("ConfigtreeObject::instance create");}

public slots:
    void slotBoardInoutCfgTree(quint8 module, bool in);     //板卡热插拔处理

private:
    explicit ConfigTreeObject(QObject *parent = 0);
    static void createCfgDelegateTree();               //构建所有配置项的代理树
    static void createCfgSettingTree();                //构建当前所有配置项的真实树
    static void CfgDelegateChange();                   //当前所有配置项代理变更，内部包含刷新界面函数

    void BoardInoutDelegateChange();

signals:

};

#endif // CONFIGTREEOBJECT_H
