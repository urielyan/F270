#include "modelmultipanelmodel.h"
#include "GlobalData/cfgGlobalDef.h"
#include <QDebug>

ModelMultiPanelModel::ModelMultiPanelModel(QObject *parent) :
    QObject(parent),Settings("DH","F270", this)
{
    //Settings.beginGroup("MultiPanel");
}

/**
 * 功能：
 *          获取多画面布局风格
 * 参数：
 *          １、int fmIndex　当前帧编号
 * 返回值：
 *          quint32：布局风格
 */
quint32 ModelMultiPanelModel::getLayoutStyle(quint32 fmIndex)
{
    Settings.beginReadArray("MultiPanel");
    Settings.setArrayIndex(fmIndex);
    quint32 ret = Settings.value("LayoutStyle", Win_Unequal_Five_Panel).toUInt();
    Settings.endArray();
    return ret;
}

/**
 * 功能：
 *          设置多画面布局风格
 * 参数：
 *          １、int fmIndex　当前帧编号
 *          ２、quint32 styleId   布局风格
 */
void ModelMultiPanelModel::setLayoutStyle(quint32 fmIndex, quint32 styleId)
{
    Settings.beginReadArray("MultiPanel");
    Settings.setArrayIndex(fmIndex);
    Settings.setValue("LayoutStyle", styleId);
    Settings.endArray();
}

/**
 * 功能：
 *          获取多画面布局名称
 * 参数：
 *          １、int fmIndex　当前帧编号
 * 返回值：
 *          QString：布局名称
 */
QString ModelMultiPanelModel::getLayoutName(quint32 fmIndex)
{
    Settings.beginReadArray("MultiPanel");
    Settings.setArrayIndex(fmIndex);
    QString ret = Settings.value("LayoutName", tr("Multi %1").arg(fmIndex)).toString();
    Settings.endArray();
    return ret;
}

/**
 * 功能：
 *          设置多画面布局名称
 * 参数：
 *          １、int fmIndex　当前帧编号
 *          ２、QString fmName　布局名称
 */
void ModelMultiPanelModel::setLayoutName(quint32 fmIndex, QString fmName)
{
    Settings.beginReadArray("MultiPanel");
    Settings.setArrayIndex(fmIndex);
    Settings.setValue("LayoutName", fmName);
    Settings.endArray();
}

/**
 * 功能：
 *          获取子画面类型
 * 参数：
 *          １、quint32 fmIndex　当前帧编号
 *          ２、int paIndex   当前子画面编号
 * 返回值：
 *          quint32：当前画面类型
 */
quint32 ModelMultiPanelModel::getPanelType(quint32 fmIndex, int paIndex)
{
    Settings.beginReadArray("MultiPanel");
    Settings.setArrayIndex(fmIndex);
    Settings.beginReadArray("SubPanle");
    Settings.setArrayIndex(paIndex);
    quint32 ret = Settings.value("PanelID", paIndex).toUInt();
    Settings.endArray();
    Settings.endArray();
    return ret;
}

/**
 * 功能：
 *          设置子画面类型
 * 参数：
 *          １、quint32 fmIndex　当前帧编号
 *          ２、int paIndex   当前子画面编号
 *           3、quint32 PanelId　当前画面类型
 */
void ModelMultiPanelModel::setPanelType(quint32 fmIndex, int paIndex, quint32 PanelId)
{
    Settings.beginReadArray("MultiPanel");
    Settings.setArrayIndex(fmIndex);
    Settings.beginReadArray("SubPanel");
    Settings.setArrayIndex(paIndex);
    Settings.setValue("PanelID", PanelId);
    Settings.endArray();
    Settings.endArray();
}

/**
 * 功能：
 *          获取子画面参数
 * 参数：
 *          １、quint32 fmIndex　当前帧编号
 *          ２、int paIndex   当前子画面编号
 * 返回值：
 *          QVariant：当前画面参数值
 */
QVariant ModelMultiPanelModel::getPanelParam(quint32 fmIndex, int paIndex)
{
    Settings.beginReadArray("MultiPanel");
    Settings.setArrayIndex(fmIndex);
    Settings.beginReadArray("SubPanel");
    Settings.setArrayIndex(paIndex);
    quint32 grpIndex = Settings.value("PanleParam", 0).toUInt();
    if(!CfgChannel::instance()->getEffectiveGroup().contains(grpIndex)){
        grpIndex = CfgChannel::instance()->getEffectiveGroup().value(0,0);
    }
    Settings.endArray();
    Settings.endArray();
    return grpIndex;
}

/**
 * 功能：
 *          设置子画面参数
 * 参数：
 *          １、quint32 fmIndex　当前帧编号
 *          ２、int paIndex   当前子画面编号
 *          ３、QVariant  value：当前画面参数值
 */
void ModelMultiPanelModel::setPanelParam(quint32 fmIndex, int paIndex, QVariant value)
{
    Settings.beginReadArray("MultiPanel");
    Settings.setArrayIndex(fmIndex);
    Settings.beginReadArray("SubPanel");
    Settings.setArrayIndex(paIndex);
    Settings.value("PanleParam", value);
    Settings.endArray();
    Settings.endArray();
}
