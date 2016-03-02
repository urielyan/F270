/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：ConfigObject.cpp
 * 概   要：配置对象类，每个实列对应系统配置树中的一个叶节点
 *
 * 当前版本：V1.0.0
 * 作   者：王 品
 * 完成日期：2015-7-8
 *
 * 修改版本：
 * 修改说明：
 * 修改作者：
 *
 ********************************************************************/
#include "configobject.h"
#include "cfgconfig.h"

ConfigObject::ConfigObject(QObject *parent) :
    QObject(parent),
    m_changedFlag(false),
    m_valuechangeFlag(false),
    m_hideFlag(false),
    m_standaloneFlag(false),
    m_delegateFlag(false),
    p_delegateParentObj(NULL)
{
}

ConfigObject::~ConfigObject()
{
    foreach (ConfigObject *pObj, m_delegateList) {
        pObj->setParentDelegateObj(NULL);
    }
    p_delegateParentObj = NULL;
}

void ConfigObject::implementData() {
    if (m_delegateFlag) {
        return;
    } else {
        if (m_value != m_valuePrev) {
            /*若当前对象配置项发生改变，则变更旧值，自身及父对象值变更标记改变，同步设备配置表*/
            m_valuePrev = m_value;

            this->setValuechangedFlag(true);
            if (!m_standaloneFlag) {
                /*不是独立的配置对象，所有父对象的值变更标记更新*/
                ConfigObject *pParent = static_cast<ConfigObject *> (this->parent());
                while (pParent) {
                    pParent->setValuechangedFlag(true);
                    pParent = static_cast<ConfigObject *> (pParent->parent());
                }
            }

            //配置->设备配置表
            CfgConfig::instance()->setConfigData(m_objID, m_channelCoder);
        }
    }
}
