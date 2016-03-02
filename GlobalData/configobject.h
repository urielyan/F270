/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：ConfigObject.h
 * 概   要：配置对象类，每个实列对应系统配置树中的一个叶节点
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
#ifndef CONFIGOBJECT_H
#define CONFIGOBJECT_H

#include <QObject>
#include <QVariant>
#include <QList>
#include <QColor>

extern  bool _curStatus;

class ConfigTreeObject;

class ConfigObject : public QObject
{
    Q_OBJECT
public:
    explicit ConfigObject(QObject *parent = 0);
    ~ConfigObject();

    /*独立标记，即值的改变不会修改父对象的更改标记*/
    void setStandAloneFlag(bool standalone);

    /*配置变更标记，用于决定UI是否显示相应的更改图标*/
    bool getChangedFlag();
    void setChangeFlag(bool flag);

    /*配置值变更标记，用于指示当前项的配置值是否发生变更*/
    bool getValuechangedFlag();
    void setValuechangedFlag(bool flag);

    /*隐藏标记，用于决定对应的菜单的显示/隐藏*/
    bool getHideFlag();
    void setHideFlag(bool hide);

    /*显示值字符串，用于在菜单上值的字面显示*/
    void setStrShowOn(QString str);
    QString getStrShowON();

    /*对象路径名，支持翻译，用于菜单的显示*/
    void setStrObjName(QString str);
    QString getStrObjName();

    /*配置对象数值相关函数*/
    QVariant getData();
    void changeData(QVariant variant, bool changeFlag = true, bool refresh = true);   //改变值，如与当前值不同，则发送变更信号

    /*被代理配置对象相关函数*/
    void setDelegateObj(QList<ConfigObject*> list);
    ConfigObject *getDelegateObj();
    bool isDelegateObj();
    QList<ConfigObject *> getDelegateList();

    /*代理对象设置*/
    void setParentDelegateObj(ConfigObject *pObj);

    /*通道编码获取*/
    quint32 getChannelCoder();

    /*对象ID获取*/
    quint32 getObjectID();

public:
    /*配置对象数据还原*/
    void  recoverData();
    /*配置对象数据生效*/
    void implementData();
    /*配置对象数据初始化*/
    void initData(const QVariant & variant, quint32 coder=0, quint32 id=0);

signals:
    //数据改变信号，表示数据改变，可用于显示字面值的设置
    void sigDataChange(quint32);
    //配置对象显示字面值改变信号，可用于通知菜单栏值的显示
    void sigDataValueStrChanged();
    //数据变更图标信号，用于通知对应的菜单显示变更图标
    void sigDataChangeFlag();
    //隐藏标记变更，需要GUI显示/隐藏与该配置对象绑定的GUI元素
    void sigHideFlagChanged();
    //代理对象变更信号，用于通知menu重新进行与配置对象的信号绑定
    void sigDelegateChanged();
    //菜单跳转信号，用于跳转到配置对象对应菜单单击时的界面
    void sigMenuJump();

public slots:

private:
    bool        m_changedFlag;              //更改标记，指示绑定的GUI元素是否显示更改图标
    bool        m_valuechangeFlag;          //值变更标记，指示配置树上的值与配置表中是否相同
    bool        m_hideFlag;                 //隐藏标记，指示绑定的GUI元素的显示/隐藏
    bool        m_standaloneFlag;           //独立标记，即值的改变不会修改父对象的更改标记
    QVariant    m_value, m_valuePrev;       //配置对象当前值、之前值
    quint32     m_channelCoder;             //通道编码, 组号。。。
    quint32     m_objID;                    //对象ID号
    QString     m_strShowOn;                //该对象的显示字面字符串
    QString     m_strObjName;               //路径转换中间值，避免中外文切换时路径异常

    bool        m_delegateFlag;             //代理对象标记
    QList<ConfigObject *> m_delegateList;   //被代理对象指针链表
    ConfigObject    *p_delegateParentObj;   //代理对象指针
};

inline void ConfigObject::setStandAloneFlag(bool standalone) {
    m_standaloneFlag = standalone;
}

inline bool ConfigObject::getChangedFlag() {
    if (m_delegateFlag && !m_delegateList.isEmpty())
        return m_delegateList.first()->getChangedFlag();
    else
        return m_changedFlag;
}

inline void ConfigObject::setChangeFlag(bool flag) {
    if (m_delegateFlag) {
        foreach (ConfigObject *pObj, m_delegateList) {
            pObj->setChangeFlag(flag);
        }
    } else {
        m_changedFlag = flag;

        if (p_delegateParentObj != NULL )
            emit p_delegateParentObj->sigDataChangeFlag();
        else
            emit sigDataChangeFlag();
    }
}

inline bool ConfigObject::getValuechangedFlag() {
    return m_valuechangeFlag;
}

inline void ConfigObject::setValuechangedFlag(bool flag) {
    if(m_delegateFlag) {
        m_valuechangeFlag = flag;
    } else {    //被代理对象 值发生了变更，自身值变更标记改变，同时通知其代理置位；
        m_valuechangeFlag = flag;
        if(p_delegateParentObj != NULL) {
            p_delegateParentObj->setValuechangedFlag(flag);
        }
    }
}

inline bool ConfigObject::getHideFlag() {
    if (m_delegateFlag && !m_delegateList.isEmpty())
        return m_delegateList.first()->getHideFlag();
    else
        return m_hideFlag;
}

inline void ConfigObject::setHideFlag(bool hide) {
    if (m_delegateFlag) {
        foreach (ConfigObject *pObj, m_delegateList) {
            pObj->setHideFlag(hide);
        }
    } else {
        m_hideFlag = hide;

        if (p_delegateParentObj != NULL)
            emit p_delegateParentObj->sigHideFlagChanged();
        else
            emit sigHideFlagChanged();

        foreach (QObject *pObj, this->children()) {
            ConfigObject *pChild = static_cast<ConfigObject *>(pObj);
            pChild->setHideFlag(hide);
        }
    }
}

inline void ConfigObject::setStrShowOn(QString str) {
    if (m_delegateFlag && !m_delegateList.isEmpty())
        m_delegateList.first()->setStrShowOn(str);
    else
        m_strShowOn = str;

    if ( p_delegateParentObj != NULL )
        emit p_delegateParentObj->sigDataValueStrChanged();
    else
        emit sigDataValueStrChanged();
}

inline QString ConfigObject::getStrShowON() {
    if (m_delegateFlag && !m_delegateList.isEmpty())
        return m_delegateList.first()->getStrShowON();
    else
        return m_strShowOn;
}

inline void ConfigObject::setStrObjName(QString str){
        m_strObjName = str;
}

inline QString ConfigObject::getStrObjName(){
        return m_strObjName;
}

inline QVariant ConfigObject::getData() {
    if (m_delegateFlag && !m_delegateList.isEmpty())
        return m_delegateList.first()->getData();
    else
        return m_value;
}

inline void ConfigObject::changeData(QVariant variant, bool changeFlag, bool refresh)  //refresh == false,change m_value;
{
    if(!_curStatus) {    //_curStatus: true--运行中    false--上电初始化
        return;
    }

    if (m_delegateFlag) {
        foreach (ConfigObject *pObj, m_delegateList) {
            pObj->changeData(variant, changeFlag, refresh);
        }
    } else {
        if ((m_value != variant) || (!refresh)) {
            m_value = variant;

            if (p_delegateParentObj != NULL)
                emit p_delegateParentObj->sigDataChange(m_channelCoder);
            else
                emit sigDataChange(m_channelCoder); //数据改变，用于变更对象的显示字符串

            if (changeFlag && !m_standaloneFlag) {  //带修改标记 && 不是独立的配置对象
                /*所有父对象的变更标志更新*/
                ConfigObject *pParent = static_cast<ConfigObject *> (this->parent());
                while (pParent) {
                    pParent->setChangeFlag(true);
                    pParent = static_cast<ConfigObject *> (pParent->parent());
                }

                this->setChangeFlag(true);
            }
        }
    }
}

inline void ConfigObject::setDelegateObj(QList<ConfigObject*> list) {
    foreach (ConfigObject *pObj, m_delegateList) {
        pObj->setParentDelegateObj(NULL);
    }
    m_delegateList.clear();
    m_delegateList = list;

    if (m_delegateList.isEmpty()) {
        m_delegateFlag = false;
    } else {
        m_delegateFlag = true;
        foreach (ConfigObject *pObj, m_delegateList) {
            pObj->setParentDelegateObj(this);

            //代理变更时，所有被代理对象自动赋值为第一个代理对象的配置
//            pObj->setValuechangedFlag(this->getValuechangedFlag());
            pObj->setHideFlag(this->getHideFlag());
            pObj->setStrShowOn(this->getStrShowON());
            pObj->changeData(this->getData());
        }
    }
}

inline ConfigObject *ConfigObject::getDelegateObj() {
    if (m_delegateList.isEmpty())
        return this;
    else
        return m_delegateList.first();
}

inline bool ConfigObject::isDelegateObj() {
    return m_delegateFlag;
}

inline QList<ConfigObject *> ConfigObject::getDelegateList() {
    return m_delegateList;
}

inline void ConfigObject::setParentDelegateObj(ConfigObject *pObj) {
    p_delegateParentObj = pObj;
}

inline quint32 ConfigObject::getChannelCoder()
{
    if (m_delegateFlag && !m_delegateList.isEmpty()) {
        return m_delegateList.first()->getChannelCoder();
    } else {
        return m_channelCoder;
    }
}

inline quint32 ConfigObject::getObjectID()
{
    if (m_delegateFlag && !m_delegateList.isEmpty()) {
        return m_delegateList.first()->getObjectID();
    } else {
        return m_objID;
    }
}

inline void ConfigObject::recoverData() {
    if (m_delegateFlag) {
        return;
    } else {
        if (m_value != m_valuePrev) {   //若当前对象配置项发生过改变，则恢复旧值，否则跳过。
            m_value = m_valuePrev;
        }
    }
}

inline void ConfigObject::initData(const QVariant & variant, quint32 coder, quint32 id) {
    if (m_delegateFlag && !m_delegateList.isEmpty()) {
        m_delegateList.first()->initData(variant, coder, id);
    } else {
        m_value = m_valuePrev = variant;
        m_channelCoder = coder;
        m_objID = id;
    }
}

#endif // CONFIGOBJECT_H
