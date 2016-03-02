#include <QEvent>
#include <QMouseEvent>
#include <QDebug>
#include <QPainter>
#include <QFrame>
#include <QTableWidget>

#include "winmultipanel.h"
#include "windigitalgroup.h"
#include "winbargroup.h"
#include "winoverchannelview.h"
#include "wintrendgroup.h"
#include "winrelayandsw.h"
#include "winmultipaneldialog.h"
#include "winalarmsummary.h"
#include "winmemsummary.h"
#include "winmessagesummary.h"
#include "winmodbusclient.h"
#include "winreport.h"
#include "winnetworkinfo.h"
#include "winroot.h"
#include "winlog.h"
#include "winmultipanelstyledialog.h"
#include "winoverview.h"
#include "wininfolistdialog.h"

WinMultiFrame::WinMultiFrame(quint32 frameIndex, ModelMultiPanelModel *model, QWidget *parent) :
    QFrame(parent),m_frameIndex(frameIndex),p_model(model),p_maskView(new QFrame)
{
    p_maskView->setObjectName("MaskView");
    p_maskView->setVisible(false);

    setLayout(&m_layout);
    m_layout.setSpacing(6);
    m_layout.setContentsMargins(11, 11, 11, 11);
    m_layout.setObjectName(QStringLiteral("gridLayout"));
    for(int i=0; i<6; ++i){
        p_panel[i] = getItemPanel(i);
    }

    this->setStyleSheet("WinMultiFrame{\
                 background-color:rgb(51,16,177);\
                 border:0;\
                 }\
                .QWidget{background-color:green;}\
                 QFrame#MaskView{\
                 background-color:rgb(100,50,50,80);\
                 border-radius:0px;\
                 border:3px solid;\
                 border-color:rgb(0,0,255,100);\
                 }");
    m_styleName = p_model->getLayoutName(frameIndex);
    m_styleId = p_model->getLayoutStyle(frameIndex);
    slotLayoutStyleChanged(m_styleId);
    connect(this, SIGNAL(sigPanelClick(int)),this,SLOT(slotPanelClicked(int)));
    connect(CfgChannel::instance(),SIGNAL(sigGroupConfigChanged()),this,SLOT(slotGroupConfigChanged()));
}

bool WinMultiFrame::eventFilter(QObject * watched, QEvent * event)
{
    if(event->type() == QEvent::MouseButtonPress){
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
        if(mouseEvent->button() == Qt::LeftButton && !m_pressed){
            m_pressed = true;
            m_pos = mouseEvent->pos();
            QWidget *widget = static_cast<QWidget *>(watched);
            widget->setGeometry(widget->x()+1, widget->y()+1,widget->width(),widget->height());
            p_maskView->setParent(widget);
            p_maskView->setGeometry(0,0,widget->width(),widget->height());
            p_maskView->setVisible(true);
            return true;
        }else{
            return false;
        }
    }else if(event->type() == QEvent::MouseButtonRelease){
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
        if(mouseEvent->button() == Qt::LeftButton && m_pressed){
            m_pressed = false;
            QWidget *widget = static_cast<QWidget *>(watched);
            widget->setGeometry(widget->x()-1, widget->y()-1,widget->width(),widget->height());
            p_maskView->setVisible(false);
            p_maskView->setParent(0);
            QVariant value = widget->property("PanelIndex");
            QPoint pos = mouseEvent->pos();
            int w = pos.x() - m_pos.x();
            int h = pos.y() - m_pos.y();
            //判断当前动作是否为滑屏
            if(value.isValid() && -50 < w && w< 50 && -20 < h && h < 20){
                emit sigPanelClick(value.toInt());
            }
            return true;
        }else{
            return false;
        }
    }

    return QWidget::eventFilter(watched, event);
}


QRect WinMultiFrame::getPanelRect(int panelIndex)
{
    QStackedLayout *levelLayout = p_panel[panelIndex];
    QWidget *maskFrame = levelLayout->currentWidget();
    QPoint glb = mapToGlobal(maskFrame->pos());
    QRect rect(glb.x(),glb.y()+WinMainFrame::instance()->getStatusBarHeight(), maskFrame->width(),maskFrame->height());
    return rect;
}

/*
 * 功能：
 *          保存配置信息
 *
 */
void WinMultiFrame::slotSaveConfig()
{
    p_model->setLayoutStyle(m_frameIndex,m_styleId);
    p_model->setLayoutName(m_frameIndex,m_styleName);

    for(int i=0; i<6;++i){
        QStackedLayout *levelLayout = p_panel[i];
        QWidget *maskFrame = levelLayout->currentWidget();
        QVariant typeValue = maskFrame->property("PanelType");
        QVariant paramValue = maskFrame->property("PanelParam");
        QVariant indexValue = maskFrame->property("PanelIndex");
        if(!typeValue.isValid() || !indexValue.isValid()){
            continue;
        }

        p_model->setPanelType(m_frameIndex,indexValue.toInt(),typeValue.toUInt());
        p_model->setPanelParam(m_frameIndex,indexValue.toInt(),paramValue);
    }
}

void WinMultiFrame::slotStyleIdChanged(quint32 styleId)
{
    if(styleId == m_styleId)
        return;
    for(int i=0; i<6; ++i){
        QStackedLayout *levelLayout = p_panel[i];
        QWidget *maskFrame = levelLayout->currentWidget();
        QWidget *peerWidget= maskFrame->property("PeerWidget").value<QWidget *>();
        maskFrame->setVisible(false);
        peerWidget->setVisible(false);
        m_layout.removeItem(levelLayout);
    }
    for(int i=0; i<m_layout.rowCount();++i){
        m_layout.setRowStretch(i,0);
    }
    for(int j=0; j<m_layout.columnCount();++j){
        m_layout.setColumnStretch(j,0);
    }
//    delete m_layout;
//    m_layout.setSpacing(6);
//    m_layout.setContentsMargins(11, 11, 11, 11);
//    m_layout.setObjectName(QStringLiteral("gridLayout"));
    m_styleId = styleId;
    slotLayoutStyleChanged(m_styleId);
}

void WinMultiFrame::slotStyleNameChanged(QString styleName)
{
    m_styleName = styleName;
}

void WinMultiFrame::slotPanelTypeChanged(int panelIndex, quint32 panelType)
{
    QStackedLayout *levelLayout = p_panel[panelIndex];
    QWidget *maskFrame = levelLayout->currentWidget();
    QWidget *from = maskFrame->property("PeerWidget").value<QWidget *>();
    QVariant typeValue = maskFrame->property("PanelType");
    QVariant paramValue = maskFrame->property("PanelParam");

    if(!typeValue.isValid() || typeValue.toUInt() == panelType || !from){
        qWarning("Failed to call slotPanelTypeChanged().");
        return;
    }

    QWidget *to = getPanel(panelType,paramValue);
    maskFrame->setProperty("PeerWidget",QVariant::fromValue(to));
    maskFrame->setProperty("PanelType",panelType);
    levelLayout->addWidget(to);
    levelLayout->removeWidget(from);
    levelLayout->setCurrentWidget(maskFrame);
    //from->deleteLater();
    delete from;
    to->show();
    this->repaint();
}


void WinMultiFrame::slotPanelParamChanged(int panelIndex, QVariant panelParam)
{
    QStackedLayout *levelLayout = p_panel[panelIndex];
    QWidget *maskFrame = levelLayout->currentWidget();
    QWidget *widget = maskFrame->property("PeerWidget").value<QWidget *>();
    QVariant value = maskFrame->property("PanelType");

    if(!value.isValid() || !widget){
        qWarning("Failed to call slotPanelParamChanged().");
        return;
    }

    maskFrame->setProperty("PanelParam",panelParam);

    switch (value.toUInt()) {
    case Win_Trend_Panel:
    {
        WinTrendGroup *tg = static_cast<WinTrendGroup *>(widget);
        tg->setGroupIndex(panelParam.toUInt());
        return;
    }
    case Win_Digital_Panel:
    {
        WinDigitalGroup *dg = static_cast<WinDigitalGroup *>(widget);
        dg->resetGroupIndex(panelParam.toUInt());
        return;
    }
    case Win_Bar_Panel:
    {
        WinBarFrame* bg = static_cast<WinBarFrame*>(widget);
        bg->resetGroupIndex(panelParam.toUInt());
        return;
    }
    default:
//        qWarning("This panel wasn't found.");
        return;
    }
}


/**
 * 功能：
 *      根据风格编号布局画面
 * 参数：
 *      quint32 styleId　风格编号
 */
void WinMultiFrame::slotLayoutStyleChanged(quint32 styleId)
{
    switch (styleId) {
    case Win_Vertical_Two_Panel :
        doVertical2Panel();
        break;
    case Win_Horizontal_Two_Panel :
        doHorizontal2Panel();
        break;
    case Win_Vertical_Three_Panel :
        doVertical3Panel();
        break;
    case Win_Horizontal_Three_Panel:
        doHorizontal3Panel();
        break;
    case Win_Equal_Four_Panel:
        doEqual4Panel();
        break;
    case Win_Vertical_Five_Panel:
        doVertical5Panel();
        break;
    case Win_Unequal_Five_Panel:
        doUnequal5Panel();
        break;
    case Win_Vertical_Six_Panel:
        doVertical6Panel();
        break;
    case Win_Unequal_Six_Panel:
        doUnequal6Panel();
        break;
    default:
        doEqual4Panel();
        break;
    }
}

/**
 * 功能：
 *      布局２画面风格样式 row:2 col:1
 */
void WinMultiFrame::doVertical2Panel()
{
    QLayoutItem *subLayout = getItem(0);
    m_layout.addItem(subLayout,0,0,1,1);

    subLayout = getItem(1);
    m_layout.addItem(subLayout,1,0,1,1);

    m_layout.setColumnStretch(0,1);

    m_layout.setRowStretch(0,1);
    m_layout.setRowStretch(1,1);
}

/**
 * 功能：
 *      布局２画面风格样式 row:1 col:2
 */
void WinMultiFrame::doHorizontal2Panel()
{
    QLayoutItem *subLayout = getItem(0);
    m_layout.addItem(subLayout,0,0,1,1);

    subLayout = getItem(1);
    m_layout.addItem(subLayout,0,1,1,1);

    m_layout.setColumnStretch(0,1);
    m_layout.setColumnStretch(1,1);

    m_layout.setRowStretch(0,1);
}

/**
 * 功能：
 *      布局３画面风格样式 row:2 col:2
 */
void WinMultiFrame::doVertical3Panel()
{
    QLayoutItem *subLayout = getItem(0);
    m_layout.addItem(subLayout,0,0,2,1);

    subLayout = getItem(1);
    m_layout.addItem(subLayout,0,1,1,1);

    subLayout = getItem(2);
    m_layout.addItem(subLayout,1,1,1,1);

    m_layout.setColumnStretch(0,1);
    m_layout.setColumnStretch(1,1);

    m_layout.setRowStretch(0,1);
    m_layout.setRowStretch(1,1);
}

/**
 * 功能：
 *      布局３画面风格样式 row:2 col:2
 */
void WinMultiFrame::doHorizontal3Panel()
{
    QLayoutItem *subLayout = getItem(0);
    m_layout.addItem(subLayout,0,0,1,2);

    subLayout = getItem(1);
    m_layout.addItem(subLayout,1,0,1,1);

    subLayout = getItem(2);
    m_layout.addItem(subLayout,1,1,1,1);

    m_layout.setColumnStretch(0,1);
    m_layout.setColumnStretch(1,1);

    m_layout.setRowStretch(0,1);
    m_layout.setRowStretch(1,1);
}

/**
 * 功能：
 *      布局４画面风格样式row:2 col:2
 */
void WinMultiFrame::doEqual4Panel()
{
    QLayoutItem *subLayout = getItem(0);
    m_layout.addItem(subLayout,0,0,1,1);

    subLayout = getItem(1);
    m_layout.addItem(subLayout,0,1,1,1);

    subLayout = getItem(2);
    m_layout.addItem(subLayout,1,0,1,1);

    subLayout = getItem(3);
    m_layout.addItem(subLayout,1,1,1,1);

    m_layout.setColumnStretch(0,1);
    m_layout.setColumnStretch(1,1);

    m_layout.setRowStretch(0,1);
    m_layout.setRowStretch(1,1);
}

/**
 * 功能：
 *      布局５画面风格样式 row:2 col:6
 */
void WinMultiFrame::doVertical5Panel()
{
    QLayoutItem *subLayout = getItem(0);
    m_layout.addItem(subLayout,0,0,1,3);

    subLayout = getItem(1);
    m_layout.addItem(subLayout,0,3,1,3);

    subLayout = getItem(2);
    m_layout.addItem(subLayout,1,0,1,2);

    subLayout = getItem(3);
    m_layout.addItem(subLayout,1,2,1,2);

    subLayout = getItem(4);
    m_layout.addItem(subLayout,1,4,1,2);

    m_layout.setColumnStretch(0,1);
    m_layout.setColumnStretch(1,1);
    m_layout.setColumnStretch(2,1);
    m_layout.setColumnStretch(3,1);
    m_layout.setColumnStretch(4,1);
    m_layout.setColumnStretch(5,1);

    m_layout.setRowStretch(0,1);
    m_layout.setRowStretch(1,1);
}

/**
 * 功能：
 *      布局５画面风格样式 row:3 col:6
 */
void WinMultiFrame::doUnequal5Panel()
{
    QLayoutItem *subLayout = getItem(0);
    m_layout.addItem(subLayout,0,0,2,4);

    subLayout = getItem(1);
    m_layout.addItem(subLayout,0,4,1,2);

    subLayout = getItem(2);
    m_layout.addItem(subLayout,1,4,1,2);

    subLayout = getItem(3);
    m_layout.addItem(subLayout,2,0,1,3);

    subLayout = getItem(4);
    m_layout.addItem(subLayout,2,3,1,3);

    m_layout.setColumnStretch(0,1);
    m_layout.setColumnStretch(1,1);
    m_layout.setColumnStretch(2,1);
    m_layout.setColumnStretch(3,1);
    m_layout.setColumnStretch(4,1);
    m_layout.setColumnStretch(5,1);

    m_layout.setRowStretch(0,1);
    m_layout.setRowStretch(1,1);
    m_layout.setRowStretch(2,1);
}

/**
 * 功能：
 *      布局６画面风格样式 row:2 col:3
 */
void WinMultiFrame::doVertical6Panel()
{
    QLayoutItem *subLayout = getItem(0);
    m_layout.addItem(subLayout,0,0,1,1);

    subLayout = getItem(1);
    m_layout.addItem(subLayout,0,1,1,1);

    subLayout = getItem(2);
    m_layout.addItem(subLayout,0,2,1,1);

    subLayout = getItem(3);
    m_layout.addItem(subLayout,1,0,1,1);

    subLayout = getItem(4);
    m_layout.addItem(subLayout,1,1,1,1);

    subLayout = getItem(5);
    m_layout.addItem(subLayout,1,2,1,1);

    m_layout.setColumnStretch(0,1);
    m_layout.setColumnStretch(1,1);
    m_layout.setColumnStretch(2,1);

    m_layout.setRowStretch(0,1);
    m_layout.setRowStretch(1,1);
}

/**
 * 功能：
 *      布局６画面风格样式 row:3 col:3
 */
void WinMultiFrame::doUnequal6Panel()
{

    QLayoutItem *subLayout = getItem(0);
    m_layout.addItem(subLayout,0,0,2,2);

    subLayout = getItem(1);
    m_layout.addItem(subLayout,0,2,1,1);

    subLayout = getItem(2);
    m_layout.addItem(subLayout,1,2,1,1);

    subLayout = getItem(3);
    m_layout.addItem(subLayout,2,0,1,1);

    subLayout = getItem(4);
    m_layout.addItem(subLayout,2,1,1,1);

    subLayout = getItem(5);
    m_layout.addItem(subLayout,2,2,1,1);

    m_layout.setColumnStretch(0,1);
    m_layout.setColumnStretch(1,1);
    m_layout.setColumnStretch(2,1);

    m_layout.setRowStretch(0,1);
    m_layout.setRowStretch(1,1);
    m_layout.setRowStretch(2,1);
}

/*
 *功能：
 *          根据多画面分配面板索引获取对应指针
 * 参数：
 *          int panelIndex 面板索引，范围0～5（目前最大支持同时显示6个画面）
 * 返回值：
 *          QLayoutItem * 子面板布局项（包含两个层叠面板）
 */
QLayoutItem *WinMultiFrame::getItem(int panelIndex)
{
    QStackedLayout *levelLayout = p_panel[panelIndex];
    QWidget *maskFrame = levelLayout->currentWidget();
    QWidget *peerWidget= maskFrame->property("PeerWidget").value<QWidget *>();
    maskFrame->setVisible(true);
    peerWidget->setVisible(true);
    return levelLayout;
}

/**
 * 功能：
 *      根据子画面索引获取对应子画面
 * 参数：
 *      １、panelIndex　子画面索引，范围1～5（最多支持同时显示6个子面板）
 * 返回值：
 *      QStackedLayout *　子画面布局对象指针（包含两个子画面）
 */
QStackedLayout * WinMultiFrame::getItemPanel(int panelIndex)
{
    quint32 panelType = p_model->getPanelType(m_frameIndex, panelIndex);
    QVariant param = p_model->getPanelParam(m_frameIndex,panelIndex);
    QWidget *widget =  getPanel( panelType, param);

    QFrame *maskFrame = new QFrame(this);
//    maskFrame->setObjectName(QString("MaskFrame_%0_%1").arg(m_frameIndex).arg(panelIndex));
//    maskFrame->setStyleSheet(QString("QFrame#%0{background-color:rgb(255,255,255,0);border:none;}").arg(maskFrame->objectName()));
    maskFrame->installEventFilter(this);
    maskFrame->setProperty("PanelIndex", panelIndex);
    maskFrame->setProperty("PanelType", panelType);
    maskFrame->setProperty("PanelParam", param);
    maskFrame->setProperty("PeerWidget", QVariant::fromValue(widget));

    QStackedLayout *levelLayout = new QStackedLayout();
    levelLayout->setStackingMode(QStackedLayout::StackAll);
    levelLayout->setSpacing(0);
    levelLayout->setMargin(0);
    levelLayout->addWidget(maskFrame);//上层用于按压时的半透明效果
    levelLayout->addWidget(widget);//底层显示对应子面板实例
    levelLayout->setCurrentWidget(maskFrame);

    widget->setVisible(false);
    maskFrame->setVisible(false);
    return levelLayout;
}

/*
 * 功能：
 *          根据面板枚举类型和参数创建面板实例
 * 参数：
 *          1、quint32 panelType   面板枚举类型
 *          2、QVariant param  参数（可选）
 * 返回值：
 *      QWidget * 枚举类型存在时则构建实例，否则返回默认QWidget
 */
QWidget* WinMultiFrame::getPanel(quint32 panelType, QVariant param)
{
    QWidget *widget = NULL;
    switch (panelType) {
    case Win_Trend_Panel://趋势图
       widget = new /*QWidget(this);//*/WinTrendGroup(param.toUInt(),this);
        break;
    case Win_Digital_Panel://数字图
        widget = new WinDigitalGroup(param.toUInt(),this);
        break;
    case Win_Bar_Panel://棒图
        widget = new WinBarFrame(param.toUInt(),this);
        break;
    case Win_Overview_Panel://总览图
        widget = new WinOverView(this);
        break;
    case Win_Relay_SW_Panel://继电器和内部开关
        widget = new WinRSSingleFrame(this);
        break;
    case  Win_Alarm_Summary_Panel:          //报警一览
        widget = new WinAlarmSummary(this);
        break;
    case  Win_Info_Summary_Panel:              //信息一览
        widget = new WinMessageSummary(this);
        break;
    case  Win_Mem_Summary_Panel:             //内存一览
        widget = new WinMemSummary(this);
        break;
    case  Win_Report_Panel:                            //报表
        widget = new WinReport(this);
        break;
    case  Win_Modbus_Client_Panel:             //Modbus客户端
        widget = new WinModbusClient(this);
        break;
    case  Win_Modbus_Server_Panel:             //Modbus服务器
        widget = new QWidget(this);
        break;
    case  Win_Event_Log_Panel:                        //事件日志
        widget = new WinOperationLog(this);
        break;
    case  Win_Error_Log_Panel:                               //错误日志
        widget = new WinErrorLog(this);
        break;
    case  Win_General_Log_Panel:                            //通信日志
        widget = new WinCommLog(this);
        break;
    case  Win_FTP_Log_Panel:                                //FTP日志
        widget = new WinFTPLog(this);
        break;
    case  Win_WEB_Log_Panel:                               //WEB日志
        widget = new WinWebLog(this);
        break;
    case  Win_Email_Log_Panel:                             //Email日志
        widget = new WinEmailLog(this);
        break;
    case  Win_Modbus_Log_Panel:                       //Modbus日志
        widget = new WinModbusLog(this);
        break;
    case  Win_SNTP_Log_Panel:                            //SNTP日志
        widget = new WinSNTPLog(this);
        break;
    case  Win_DHCP_Log_Panel:                           //DHCP日志
        widget = new WinDHCPLog(this);
        break;
    case  Win_Network_Panel:                      //网络操作
        widget = new WinNetworkInfo(this);
        break;
    default:
        widget = new QWidget(this);
        break;
    }
    return widget;
}

/**
 * 功能：
 *      子画面被点击
 * 参数：
 *      int  panelIndex　子画面索引
 */
void WinMultiFrame::slotPanelClicked(int panelIndex)
{
    if(Authorization::instance()->getLoginStatus() == Authorization::Auth_LoginStatus_Logout){
        WinInfoListDialog::instance()->showMsg(818);
    }else{
        QStackedLayout *levelLayout = p_panel[panelIndex];
        QWidget *maskFrame = levelLayout->currentWidget();
        QVariant typeValue = maskFrame->property("PanelType");
        QVariant paramValue = maskFrame->property("PanelParam");

        if(!typeValue.isValid()){
            qWarning("Failed to call slotPanelClicked().");
            return;
        }

        WinMultiPanelDialog dialog(m_frameIndex,panelIndex,typeValue.toUInt(),paramValue,this);
        dialog.exec();
    }
}

void WinMultiFrame::slotGroupConfigChanged()//显示组配置变更
{
    for(int i=0; i<6; ++i){
        slotPanelParamChanged(i, p_model->getPanelParam(m_frameIndex,i));
    }
}

WinMultiPanel::WinMultiPanel(QGraphicsView *parent) :
    SlidingScreen(parent)
{
    for(quint32 i=0; i<10; i++){
        WinMultiFrame *frame = new WinMultiFrame(i,&m_model);
        connect(frame,SIGNAL(sigChangeFrame(quint32)),this,SIGNAL(sigChangeFrame(quint32)));
        this->addWidgetItem(i, frame);
//        p_map_Frame.insert(i,frame);
    }
    this->setCurrentWidget(0);
}
//WinMultiPanel::~WinMultiPanel()
//{
//    this->removeAllWidget();
//    for(QMap<quint32,WinMultiFrame *>::iterator item = p_map_Frame.begin();item != p_map_Frame.end();++item){
//        item.value()->deleteLater();
//    }
//    p_map_Frame.clear();
//}


void WinMultiPanel::showConfigDialog()
{
    if(Authorization::instance()->getLoginStatus() == Authorization::Auth_LoginStatus_Logout){
        WinInfoListDialog::instance()->showMsg(818);
    }else{
        WinMultiPanelStyleDialog styleDialog(static_cast<WinMultiFrame *>(this->getGroupWidget(this->getCurrentWidget())));
        styleDialog.exec();
    }
}

