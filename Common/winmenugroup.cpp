#include <QVBoxLayout>
#include <QFile>
#include <QScrollBar>
#include <QScroller>
#include <QScrollerProperties>
#include <QButtonGroup>
#include <QXmlStreamReader>
#include "winmenugroup.h"

WinMenuGroup::WinMenuGroup(QString xml, SysConfigSlidingScreen *scene, QWidget *parent) :
    QScrollArea(parent),m_depth(0),p_parentMenu(0),p_group_Scene(scene)
{
    this->setWidget(new QWidget);
    this->setWidgetResizable(true);
    this->setGroupStyleSheet();
    p_vLayout = new QVBoxLayout(this->widget());
    p_vLayout->setSpacing(1);       // 原值为3，修改人：gehailang
    p_vLayout->setContentsMargins(0,0,1,0);     //  原值为(0, 1, 0, 1)，修改人：gehailang

    QFile file(xml);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< QString("Can't open the file '%1'!").arg(xml)<<"["<<__func__<<"]";
        return;
    }else{
        parseXml(file);
        file.close();
    }

    QButtonGroup *btnGrp = new QButtonGroup(this);
    bool flag = false;
    for(int i=0; i<p_vLayout->count() - 1; ++i){
        Menu * menu = static_cast<Menu *>(p_vLayout->itemAt(i)->widget());
        if(menu != NULL){
            btnGrp->addButton(menu);
            menu->setCheckable(true);
            if(!flag && !menu->isHidden()){
                menu->click();
                menu->setChecked(true);
                flag = true;
            }
        }
    } 
}

WinMenuGroup::WinMenuGroup(SysConfigSlidingScreen *scene, QWidget *parent):
    QScrollArea(parent),m_depth(0),p_parentMenu(0),p_group_Scene(scene)
{
    this->setWidget(new QWidget);
    this->setWidgetResizable(true);
    this->setGroupStyleSheet();
    p_vLayout = new QVBoxLayout(this->widget());
    p_vLayout->setSpacing(3);
    p_vLayout->setContentsMargins(0,1,0,1);
}

WinMenuGroup::WinMenuGroup(QString xml, Menu *parentMenu, SysConfigSlidingScreen *scene,int depth, QWidget *parent) :
    QScrollArea(parent),m_depth(depth + 1),p_parentMenu(parentMenu),p_group_Scene(scene)
{
    this->setWidget(new QWidget);
    this->setWidgetResizable(true);
    this->setGroupStyleSheet();
    p_vLayout = new QVBoxLayout(this->widget());
    p_vLayout->setSpacing(3);
    p_vLayout->setContentsMargins(0,1,0,1);
    connect(p_parentMenu, SIGNAL(clicked()),this,SLOT(slotParentMenuClicked()));
    QFile file(xml);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qWarning()<< QString("Can't open the file '%1'!").arg(xml)<<"["<<__func__<<"]";
        return;
    }else{
        parseXml(file);
        file.close();
    }
}

WinMenuGroup::WinMenuGroup(Menu *parentMenu, SysConfigSlidingScreen *scene, int depth, QWidget *parent):
    QScrollArea(parent),m_depth(depth + 1),p_parentMenu(parentMenu),p_group_Scene(scene)
{
    this->setWidget(new QWidget);
    this->setWidgetResizable(true);
    this->setGroupStyleSheet();
    p_vLayout = new QVBoxLayout(this->widget());
    p_vLayout->setSpacing(0);       //  原值为3，修改人：gehailang
    p_vLayout->setContentsMargins(0,0,0,0);     //  原值为(0, 1, 0, 1)，修改人：gehailang
    connect(p_parentMenu, SIGNAL(clicked()),this,SLOT(slotParentMenuClicked()));
}

void WinMenuGroup::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        m_pos = event->pos().y();
        m_drag = true;
    }
    QScrollArea::mousePressEvent(event);
}

void WinMenuGroup::mouseMoveEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton || m_drag){
        int span =  m_pos - event->y();
        this->verticalScrollBar()->setValue(this->verticalScrollBar()->value() + span);
        m_pos = event->pos().y();
    }else{
        m_drag = true;
        m_pos = event->pos().y();
    }
    QScrollArea::mouseMoveEvent(event);
}

void WinMenuGroup::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        m_pos = event->pos().y();
        m_drag = false;
    }
    QScrollArea::mouseReleaseEvent(event);
}

void WinMenuGroup::resizeEvent(QResizeEvent *event)
{
//     if(p_vLayout->count()>0){
//         Menu * menu = static_cast<Menu *>(p_vLayout->itemAt(0)->widget());
//         QScroller::scroller(this->viewport())->setSnapPositionsY(0,menu->height() + p_vLayout->spacing());
//     }
    this->verticalScrollBar()->setRange(0,this->widget()->height()-this->viewport()->height());
    QScrollArea::resizeEvent(event);
}

void WinMenuGroup::setGroupStyleSheet()
{
    m_drag = false;
//#ifdef __arm__
//    this->setAttribute(Qt::WA_AcceptTouchEvents);
//    QScroller::grabGesture(this->viewport(),QScroller::TouchGesture);
//#else
//    QScroller::grabGesture(this->viewport(),QScroller::LeftMouseButtonGesture);
//#endif
//    QScroller *scroller = QScroller::scroller(this->viewport());
//    QScrollerProperties prop = scroller->scrollerProperties();
//    prop.setScrollMetric(QScrollerProperties::OvershootDragDistanceFactor,1);
//    prop.setScrollMetric(QScrollerProperties::OvershootDragResistanceFactor,0.1);
//    prop.setScrollMetric(QScrollerProperties::OvershootScrollDistanceFactor,0.5);
//    prop.setScrollMetric(QScrollerProperties::OvershootScrollTime,0.3);
////    prop.setScrollMetric(QScrollerProperties::VerticalOvershootPolicy,QScrollerProperties::OvershootAlwaysOff);
////    prop.setScrollMetric(QScrollerProperties::HorizontalOvershootPolicy,QScrollerProperties::OvershootAlwaysOff);
//    scroller->setScrollerProperties(prop);
    this->setStyleSheet("QScrollBar:vertical {"\
                        "width: 6px;"\
                        "background:rgba(0,0,0,10%);"\
                        "margin:0px,0px,0px,0px;"\
                        "padding-top:0px;   "\
                        "padding-bottom:0px;}"\
                        "QScrollBar::handle:vertical {"\
                        "background: rgba(1,6,134,80%);"\
                        "min-height: 20px;"\
                        "min-width: 4px;}"\
                        "QScrollBar::down-button,QScrollBar::up-button{width:0px;height:0px}"\
                        "QWidget {background:rgba(0,0,0,0%); border : 0px}");
}

void WinMenuGroup::setParentMenu(Menu *parentMenu)
{
    this->p_parentMenu = parentMenu;
}

Menu *WinMenuGroup::parentMenu()
{
    return p_parentMenu;
}


void WinMenuGroup::parseXml(QFile & xmlFile)
{
    QXmlStreamReader xmlReader(&xmlFile);

    while(!xmlReader.atEnd() || !xmlReader.hasError()){
        xmlReader.readNext();
        if(xmlReader.tokenType() == QXmlStreamReader::StartDocument ){
            continue;
        }

        if(xmlReader.isStartElement() && xmlReader.name() == "Config"){
            parseElement(xmlReader);
        }
    }
    xmlReader.clear();
    //p_vLayout->addSpacerItem(new QSpacerItem(40,40,QSizePolicy::Expanding,QSizePolicy::Expanding));
    p_vLayout->addStretch();
}

void WinMenuGroup::parseElement(QXmlStreamReader &xmlReader)
{
    while(!xmlReader.atEnd() ||  !xmlReader.hasError()){
        xmlReader.readNext();
        if(xmlReader.isStartElement()){
            QXmlStreamAttributes attrs = xmlReader.attributes();
            if(xmlReader.name() == "MenuClass"){
                createMenuClass(attrs);
            }else if(xmlReader.name() == "MenuTitle"){
                createMenuTitle(attrs);
            }else if(xmlReader.name() == "MenuGroup"){
                createMenuGroup(attrs);
            }else if(xmlReader.name() == "MenuNormal"){
                createMenuNormal(attrs);
            }else if(xmlReader.name() == "MenuItem"){
                createMenuItem(attrs);
            }else{
                xmlReader.skipCurrentElement();
            }
        }
    }
}

Menu* WinMenuGroup::createMenuGroup(QXmlStreamAttributes &attrs)
{
    Menu *pMemu = MenuFactory::instance()->createMenu(attrs.value("Path").toString(),MenuType_group);
//    pMemu->setEnabled(false);
    pMemu->setFocusPolicy(Qt::NoFocus);
    pMemu->setCheckable(false);
    p_vLayout->addWidget(pMemu);
    return pMemu;
}

Menu* WinMenuGroup::createMenuTitle(QXmlStreamAttributes &attrs)
{
    Menu *pMemu = MenuFactory::instance()->createMenu(attrs.value("Path").toString(),MenuType_title);
    QString changedPath = attrs.value("Changed").toString();
    QString extendPath = attrs.value("Extend").toString();
    if(changedPath != "none" && extendPath != "none" ){
        pMemu->setMenuIcon(QPixmap(changedPath), QPixmap(extendPath));
    }
    p_vLayout->addWidget(pMemu);
    WinMenuGroup *pMenuGroup = createSubConfig(attrs.value("Include").toString(), pMemu);
    if(pMenuGroup != NULL){
        pMemu->setProperty("GroupMenu",QVariant::fromValue(pMenuGroup));
    }
    QString method = attrs.value("Clicked").toString();
    if(method != "none"){
        std::string slot = QString("1%1").arg(method).toStdString();
        connect(pMemu,SIGNAL(clicked()),this, slot.data());
    }
    return pMemu;
}

Menu* WinMenuGroup::createMenuClass(QXmlStreamAttributes &attrs)
{
    Menu *pMemu = MenuFactory::instance()->createMenu(attrs.value("Path").toString(),MenuType_class);
    QString normalPath = attrs.value("Normal").toString();
    QString changedPath = attrs.value("Changed").toString();
    if(normalPath != "none"&& changedPath != "none" ){
        pMemu->setMenuIcon(QPixmap(changedPath), QPixmap(normalPath));
    }
    p_vLayout->addWidget(pMemu);
    WinMenuGroup *pMenuGroup = createSubConfig(attrs.value("Include").toString(), pMemu);
    if(pMenuGroup != NULL){
        pMemu->setProperty("GroupMenu",QVariant::fromValue(pMenuGroup));
    }
    return pMemu;
}

Menu* WinMenuGroup::createMenuNormal(QXmlStreamAttributes &attrs)
{
    Menu *pMemu = MenuFactory::instance()->createMenu(attrs.value("Path").toString(),MenuType_normal);
    QString changedPath = attrs.value("Changed").toString();
    if(changedPath != "none" ){
        pMemu->setMenuIcon(QPixmap(changedPath), QPixmap());
    }
    p_vLayout->addWidget(pMemu);
    WinMenuGroup *pMenuGroup = createSubConfig(attrs.value("Include").toString(), pMemu);
    if(pMenuGroup != NULL){
        pMemu->setProperty("GroupMenu",QVariant::fromValue(pMenuGroup));
    }
    QString method = attrs.value("Clicked").toString();
    if(method != "none"){
        std::string slot = QString("1%1").arg(method).toStdString();
        connect(pMemu,SIGNAL(clicked()),this, slot.data());
    }
    return pMemu;
}

Menu* WinMenuGroup::createMenuItem(QXmlStreamAttributes &attrs)
{
    Menu *pMemu = MenuFactory::instance()->createMenu(attrs.value("Path").toString(),MenuType_item);
    p_vLayout->addWidget(pMemu);
    WinMenuGroup *pMenuGroup = createSubConfig(attrs.value("Include").toString(), pMemu);
    if(pMenuGroup != NULL){
        pMemu->setProperty("GroupMenu",QVariant::fromValue(pMenuGroup));
    }

    return pMemu;
}

WinMenuGroup * WinMenuGroup::createSubConfig(QString xml, Menu* menu)
{
    QFile file(xml);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        return NULL;
    }
    WinMenuGroup *groupMenu = new WinMenuGroup( menu, p_group_Scene,m_depth);
    groupMenu->parseXml(file);
    file.close();
    return groupMenu;
}

void WinMenuGroup::slotParentMenuClicked()
{
    if(m_depth < 2){
        p_group_Scene->setInitWidget(this);
    }else{
        p_group_Scene->setNextWidget(this);
        p_group_Scene->moveToNextWidget();
    }
}

void WinMenuGroup::slotAIMenuClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinAIChanSelector::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotDIMenuClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinDIChanSelector::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotDOMenuClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinDOChanSelector::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotMathMenuClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinMathChanSelector::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotCommMenuClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinCommChanSelector::instance()->showSelector(menu,this);
}


/**************common iuput***************/
void WinMenuGroup::slotNumericKeypadClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinNumericKeypad::instance()->show(menu);
}

void WinMenuGroup::slotSoftKeypadClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());

    Language_Type Lang_type = (Language_Type)DevConfigPtr->systemConfig.languageType;

    if(Language_Type_Chinese == Lang_type) {
        WinSoftKeypad::instance()->show(menu);
    } else {    //当前配置语言为非中文时，只能调用英文键盘
        WinEnglishKeypad::instance()->show(menu);
    }
}

void WinMenuGroup::slotSelectColorClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinColorSetKeypad::instance()->show(menu);
}

void WinMenuGroup::slotEnglishInputClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinEnglishKeypad::instance()->show(menu);
}

void WinMenuGroup::slotNetworkIPNumericKeypadClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinIpSetKeypad::instance()->show(menu);
}

void WinMenuGroup::slotChannelSelectClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinChannelSelectKeypad::instance()->show(menu);
}

void WinMenuGroup::slotCalculationKeypadClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinCalculationKeypad::instance()->show(menu);
}

void WinMenuGroup::slotSwitchClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinSwitchOnOrOff::instance()->showSelector(menu,this);
}

/*****************AI  item*******************/
void WinMenuGroup::slotAIRangeRangeTypeMenuClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinAIRangeTypeSelector::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotAIRangeRangeRangeMenuClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinAIRangeRangeSelector::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotAIRangeRangeCalculationMenuClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinAIRangeCalculation::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotAIRangeScalePosMenuClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinAIRangeScalePos::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotAIRangeLowCutOutputMenuClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinAIRangeLowCutOutput::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotAIRangeRJCModeMenuClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinAIRangeRJCMode::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotAIRangeBurnoutModeMenuClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinAIRangeBurnoutMode::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotAIAlarmLvl1TypeMenuClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinAIAlarmLvl1Type::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotAIAlarmLvl1OutTypeMenuClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinAIAlarmLvl1OutType::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotAIAlarmDelayHourMenuClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinAIAlarmDelayHour::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotAIAlarmDelayMinMenuClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinAIAlarmDelayMin::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotAIAlarmDelaySecMenuClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinAIAlarmDelaySec::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotAIDisplayScalePosMenuClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinAIDisplayScalePos::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotAIDisplayScaleDivMenuClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinAIDisplayScaleDiv::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotAIDisplayBarPosMenuClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinAIDisplayBarPos::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotAIDisplayBarDivMenuClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinAIDisplayBarDiv::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotAIDisplayCsbandModeMenuClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinAIDisplayCsbandMode::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotAIDisplayAlarmTypeMenuClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinAIDisplayAlarmType::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotAICaliCorrectModeMenuClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinAICaliCorrectMode::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotAICaliCorrectCountMenuClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinAICaliCorrectCount::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotAICaliCorrectExecuMenuClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());

    quint32 curObjID = menu->getMenuID().toUInt() - 2;  //ID: exec->input
    ConfigObject *pObj;

    switch(curObjID) {
    case ID_AI_Correct_Point1_In:
        pObj = ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_1_INPUT);
        break;
    case ID_AI_Correct_Point2_In:
        pObj = ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_2_INPUT);
        break;
    case ID_AI_Correct_Point3_In:
        pObj = ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_3_INPUT);
        break;
    case ID_AI_Correct_Point4_In:
        pObj = ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_4_INPUT);
        break;
    case ID_AI_Correct_Point5_In:
        pObj = ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_5_INPUT);
        break;
    case ID_AI_Correct_Point6_In:
        pObj = ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_6_INPUT);
        break;
    case ID_AI_Correct_Point7_In:
        pObj = ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_7_INPUT);
        break;
    case ID_AI_Correct_Point8_In:
        pObj = ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_8_INPUT);
        break;
    case ID_AI_Correct_Point9_In:
        pObj = ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_9_INPUT);
        break;
    case ID_AI_Correct_Point10_In:
        pObj = ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_10_INPUT);
        break;
    case ID_AI_Correct_Point11_In:
        pObj = ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_11_INPUT);
        break;
    case ID_AI_Correct_Point12_In:
        pObj = ConfigTreeObject::getConfigObject(AI_DELEGATE_CALI_12_INPUT);
        break;
    default:
        break;
    }

    WinCfgAICaliDialog a;
    if(a.exec() == QDialog::Accepted) {
        float curValue;
        quint32 curAccuracy;
        quint32 chanNum;
        quint32 curCorrectNum;
        if(CfgChannel::instance()->getChannelCoder( \
                    ConfigTreeObject::getConfigData(AI_DELEGATE_FIRSTCHAN).toString(), chanNum)) {
            curAccuracy = CfgChannel::instance()->getChannelDataAccuracy(chanNum).toUInt();
            curValue = QString::number(CfgChannel::instance()->getChannelData(chanNum).toFloat(), 'f', curAccuracy).remove(".").toFloat();
            curCorrectNum = CfgChannel::instance()->getChannelCaliCorrectNum(chanNum).toUInt();
        } else {
            WinInfoListDialog::instance()->showMsg(12);
            return;
        }

        QStringList MinMaxList = DigitalOperation::getLimitAICaliValue(curObjID, curAccuracy);
        if((curValue >= MinMaxList[0].remove(".").toFloat()) && (curValue <= MinMaxList[1].remove(".").toFloat())) {
            pObj->changeData(curValue, false);

            //change other CaliInput
            for(quint8 i=0;i<curCorrectNum;++i) {
                MinMaxList = DigitalOperation::getLimitAICaliValue(ID_AI_Correct_Point1_In + i*10, curAccuracy);
                pObj = ConfigTreeObject::getConfigObject(QString(AI_DELEGATE_CALI_X_INPUT).arg(i+1));
                curValue = pObj->getData().toFloat();
                if((curValue >= MinMaxList[0].remove(".").toFloat()) && (curValue <= MinMaxList[1].remove(".").toFloat())) {
                    //hold the value
                } else {
                    MinMaxList = DigitalOperation::getLimitAICaliValue(pObj->getObjectID(), curAccuracy);
                    pObj->changeData(MinMaxList[0].remove(".").toFloat(), false);
                }
            }
        } else {
            WinInfoListDialog::instance()->showMsg(31);
        }
    }
}

/*****************DI  item*******************/
void WinMenuGroup::slotDIRangeRangeTypeMenuClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinDIRangeRangeType::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotDIRangeRangeRangeMenuClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());

    DI_Range_Type type = (DI_Range_Type)ConfigTreeObject::getConfigObject(DI_DELEGATE_RANG_RANGE_TYPE)->getData().toUInt();
    DI_Range_Calculate calcu = (DI_Range_Calculate)ConfigTreeObject::getConfigObject(DI_DELEGATE_RANG_RANGE_CALCU)->getData().toUInt();
    if((DI_Range_Type_Pluse == type) || (DI_Range_Calculate_Delta == calcu)) {
        WinNumericKeypad::instance()->show(menu);
    } else {
        WinDIRangeRangeRange::instance()->showSelector(menu,this);
    }
}

void WinMenuGroup::slotDIRangeRangeCalculationMenuClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinDIRangeCalculation::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotDIAlarmLvl1TypeMenuClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinDIAlarmLvl1Type::instance()->showSelector(menu,this);
}

/*****************DO  item*******************/
void WinMenuGroup::slotDORangeRangeTypeMenuClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinDORangeRangeType::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotDORangeRangeRangeMenuClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinDORangeRangeRange::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotDORangeActionEnergizeMenuClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinDORangeActionEnergize::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotDORangeActionActionMenuClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinDORangeActionAction::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotDORangeActionHoldMenuClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinDORangeActionHold::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotDORangeActionACKMenuClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinDORangeActionACK::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotDORangeActionIntervalMenuClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinDORangeActionInterval::instance()->showSelector(menu,this);
}

/*****************Math  item*******************/
void WinMenuGroup::slotMathActionErrorMenuClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinMathActionError::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotMathActionActionMenuClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinMathActionAction::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotMathActionSumaveMenuClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinMathActionSumave::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotMathActionPPMenuClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinMathActionPP::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotMathExpTLOGTypeMenuClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinMathExpTLOGType::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotMathExpTLOGNoMenuClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinMathExpTLOGNo::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotMathExpTLOGSumMenuClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinMathExpTLOGSum::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotMathExpRollInterMenuClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinMathExpRollInter::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotMathAlarmLvl1TypeMenuClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinMathAlarmLvl1Type::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotMathConstKNumMenuClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinMathConstKNum::instance()->showSelector(menu,this);
}

/*******************************COMM************************/
void WinMenuGroup::slotCommChannelChooseClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinCommChannelChoose::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotCommChannelValuePowerClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinCommChannelValuePower::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotCommChannelDecimalPlaceClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinCommChannelDecimalPlace::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotCommChannelAlarmTypeClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinCommChannelAlarmType::instance()->showSelector(menu,this);
}


/*****************Display  item*******************/
void WinMenuGroup::slotDispGrpNumMenuClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinDispGrpNum::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotDispTrendTrendIntervalMenuClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinDispTrendTrendInterval::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotDispTrendSecondIntervalMenuClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinDispTrendTrendIntervalSecond::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotDispTrendTrendDirectionMenuClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinDispTrendTrendDirection::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotDispTrendTrendLineMenuClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinDispTrendTrendLine::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotDispTrendTrendGridMenuClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinDispTrendTrendGrid::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotDispTrendScaleDigitMenuClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinDispTrendScaleDigit::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotDispTrendScaleValueMenuClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinDispTrendScaleValue::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotDispTrendScaleMarkMenuClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinDispTrendScaleMark::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotDispTrendMsgWriteMenuClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinDispTrendMsgWrite::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotDispMsgNumMenuClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinDispMsgNum::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotDispScreenLCDValueMenuClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinDispScreenLCDValue::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotDispScreenLightModeMenuClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinDispScreenLightMode::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotDispScreenLightTimeMenuClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinDispScreenLightTime::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotDispScreenLightRestoreMenuClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinDispScreenLightRestore::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotDispScreenMonitorBackMenuClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinDispScreenMonitorBack::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotDispScreenMonitorScrollMenuClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinDispScreenMonitorScroll::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotDispScreenMonitorJumpMenuClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinDispScreenMonitorJump::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotDispScreenCalendar1stMenuClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinDispScreenCalendar1st::instance()->showSelector(menu,this);
}

/*****************Event  item*******************/
void WinMenuGroup::slotEventGrpNumMenuClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinEventGrpNum::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotEventEventTypeMenuClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinEventEventType::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotEventEventAlarmlevelMenuClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinEventEventAlarmlevel::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotEventEventDetailsMenuClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinEventEventDetails::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotEventEventModeMenuClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinEventEventMode::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotEventActionTypeMenuClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinEventActionType::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotEventActionDetailMenuClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinEventActionDetail::instance()->showSelector(menu,this);
}

/*****************Datasave  item*******************/
void WinMenuGroup::slotDatasaveDataStrMenuClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinDatasaveFileStr::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotDatasaveFormatMenuClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinDatasaveFormatData::instance()->showSelector(menu,this);
}

/*****************Measure  item*******************/
void WinMenuGroup::slotMeasureIntervalMenuClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinMeasureInterval::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotMeasureOverRangeMenuClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinMeasureOverRange::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotMeasureUnitModuleMenuClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    ConfigTreeObject_Measure::instance(0)->slotMeasureDelegateChange_Unit(menu);
}

void WinMenuGroup::slotMeasureAIModeMenuClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinMeasureAIMode::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotMeasureADTimeMenuClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinMeasureADTime::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotMeasureDIModeMenuClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinMeasureDIMode::instance()->showSelector(menu,this);
}

/*****************Record  item*******************/
void WinMenuGroup::slotRecordBasicModeModeMenuClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinRecordBasicModeMode::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotRecordBasicDispValueMenuClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinRecordBasicDispValue::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotRecordBasicEventValueMenuClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinRecordBasicEventValue::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotRecordBasicEventModeMenuClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinRecordBasicEventMode::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotRecordBasicEventLenthMenuClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinRecordBasicEventLenth::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotRecordBasicEventTriggerMenuClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinRecordBasicEventTrigger::instance()->showSelector(menu,this);
}

/************************modbus client************************/
void WinMenuGroup::slotModbusCLientIntervalClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinModbusClientInterval::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotModbusClientWaitTimeClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinModbusClientWaitTime::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotModbusClientConnectTimeClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinModbusClientConnectTime::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotModbusServerSettingNumberClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinModbusServerSettingNumber::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotModbusDelayResponseMenuClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinModbusServerDelayRes::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotModbusCommandNumberClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinModbusCommandNumber::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotModbusCommandTypeClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinModbusCommandType::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotModbusCommandDataTypeClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinModbusCommandDataType::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotModbusCommandChanelTypeClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinModbusCommandChanelType::instance()->showSelector(menu,this);
}
/*************************SMTP*****************************************/
void WinMenuGroup::slotSMTPAuthenticationCLicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinSMTPAuthentication::instance()->showSelector(menu,this);
}

/*************************E-mail****************************************/
void WinMenuGroup::slotEMailAlarmNotificationCLicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinEMailAlarmNotification::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotEMailChannelSetCLicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinEMailChannelSet::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotEMailInteralCLicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinEMailInteral::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotEMailTimeHourCLicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinEMailTimeHour::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotEMailTimeMinuteCLicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinEMailTimeMinute::instance()->showSelector(menu,this);
}

/**********************************SNTP****************************/
void WinMenuGroup::slotSNTPIntervalCLicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinSNTPInterval::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotSNTPTimeoutCLicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinSNTPTimeout::instance()->showSelector(menu,this);
}

/**********************************Serial***********************************/
void WinMenuGroup::slotSerialBasicReceiveFunctionClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinSerialBasicReceiveFunction::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotSerialBasicBaudClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinSerialBasicBaudFunction::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotSerialBasicParityClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinSerialBasicParityFunction::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotSerialBasicStopClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinSerialBasicStopFunction::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotSerialBasicLengthClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinSerialBasicLengthFunction::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotSerialModbusTimeoutClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinSerialModbusTimeout::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotSerialModbusGapClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinSerialModbusGap::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotSerialModbusRetryClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinSerialModbusRetry::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotSerialModbusCommandChanelTypeClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinSerialModbusCommandChanelType::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotSystemEnvironmentLanguageClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinSystemEnvironmentLanguage::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotSystemEnvironmentTemperatureClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinSystemEnvironmentTemperature::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotSystemEnvironmentDecimalClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinSystemEnvironmentDecima::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotSystemEnvironmentDataClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinSystemEnvironmentData::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotSystemEnvironmentDelimiterClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinSystemEnvironmentDelimiter::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotSystemEnvironmentMonthClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinSystemEnvironmentMonth::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotSystemAlarmHoldClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinSystemAlarmHold::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotSystemTimeHourClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinSystemTimeHour::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotSystemIntervalTypeClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinSystemIntervalType::instance()->showSelector(menu,this);
}


void WinMenuGroup::slotSystemIntervalANDORClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinSystemIntervalANDOR::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotSystemRelayClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinSystemRelay::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotSystemCommonClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinSystemCommon::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotBatchCommonClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinBatchCommon::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotReportCommClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinReportComm::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotTimerTimerCommClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinTimerTimerComm::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotTimerMatchTimerCommClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinTimerMatchTimer::instance()->showSelector(menu,this);
}


void WinMenuGroup::slotSecurityBasicTouchClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinSecurityBasicTouchlock::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotSecurityBasicCommClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinSecurityBasicComm::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotSecurityBasicAutoClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinSecurityBasicAuto::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotSecurityPropertyClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinSecurityProperty::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotSecurityUserNumClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinSecurityUserNum::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotSecurityUserLvlClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinSecurityUserLvl::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotSecurityUserModeClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinSecurityUserMode::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotSecurityUserInitClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinSecurityUserInit::instance()->showSelector(menu,this);
}

void WinMenuGroup::slotSecurityAuthNumClicked()
{
    Menu *menu = static_cast<Menu *>(this->sender());
    WinSecurityAuthNum::instance()->showSelector(menu,this);
}


