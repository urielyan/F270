#ifndef CONFIGTREEOBJECT_DO_H
#define CONFIGTREEOBJECT_DO_H

//DO 配置树路径
#define DOCONFIG  "Config:DO channel settings"
#define DOCONFIG_RANGE      "Config:DO channel settings:Range"
#define DOCONFIG_DISP       "Config:DO channel settings:Display settings"

#define DOCONFIG_FIRSTCHAN  "Config:DO channel settings:First-CH"
#define DOCONFIG_LASTCHAN   "Config:DO channel settings:Last-CH"

#define DOCONFIG_RANGE_RANGE            "Config:DO channel settings:Range:%1:Range"          //量程分组
#define DOCONFIG_RANGE_RANGE_TYPE       "Config:DO channel settings:Range:%1:Range:Type"
#define DOCONFIG_RANGE_RANGE_MIN        "Config:DO channel settings:Range:%1:Range:Range Lower"
#define DOCONFIG_RANGE_RANGE_MAX        "Config:DO channel settings:Range:%1:Range:Range Upper"
#define DOCONFIG_RANGE_RANGE_LOWER      "Config:DO channel settings:Range:%1:Range:Span Lower"
#define DOCONFIG_RANGE_RANGE_UPPER      "Config:DO channel settings:Range:%1:Range:Span Upper"
#define DOCONFIG_RANGE_RANGE_UNIT       "Config:DO channel settings:Range:%1:Range:Unit"
#define DOCONFIG_RANGE_ACTION           "Config:DO channel settings:Range:%1:Action"        // 动作分组
#define DOCONFIG_RANGE_ACTION_ENERGIZE  "Config:DO channel settings:Range:%1:Action:Energize/De-engergize"
#define DOCONFIG_RANGE_ACTION_ACTION    "Config:DO channel settings:Range:%1:Action:Action"
#define DOCONFIG_RANGE_ACTION_RELAYACK  "Config:DO channel settings:Range:%1:Action:Relay Action on ACK"
#define DOCONFIG_RANGE_ACTION_HOLD      "Config:DO channel settings:Range:%1:Action:Hold"
#define DOCONFIG_RANGE_ACTION_INTERVAL  "Config:DO channel settings:Range:%1:Action:Relay deactivated interval"

#define DOCONFIG_DISP_TAG            "Config:DO channel settings:Display settings:%1:Tag"    //标记分组
#define DOCONFIG_DISP_TAG_CHAR       "Config:DO channel settings:Display settings:%1:Tag:Characters"
#define DOCONFIG_DISP_TAG_NO         "Config:DO channel settings:Display settings:%1:Tag:No."
#define DOCONFIG_DISP_COLOR          "Config:DO channel settings:Display settings:%1:Color"    //颜色分组
#define DOCONFIG_DISP_COLOR_COLOR    "Config:DO channel settings:Display settings:%1:Color:Color"
#define DOCONFIG_DISP_ZONE           "Config:DO channel settings:Display settings:%1:Zone"   //显示区域分组
#define DOCONFIG_DISP_ZONE_UPPER     "Config:DO channel settings:Display settings:%1:Zone:Upper"
#define DOCONFIG_DISP_ZONE_LOWER     "Config:DO channel settings:Display settings:%1:Zone:Lower"
#define DOCONFIG_DISP_SCALE          "Config:DO channel settings:Display settings:%1:Scale"  //标尺分组
#define DOCONFIG_DISP_SCALE_POS      "Config:DO channel settings:Display settings:%1:Scale:Position"
#define DOCONFIG_DISP_BAR            "Config:DO channel settings:Display settings:%1:Bar graph"  //棒图分组
#define DOCONFIG_DISP_BAR_POSITION   "Config:DO channel settings:Display settings:%1:Bar graph:Base position"
#define DOCONFIG_DISP_DISCHAR        "Config:DO channel settings:Display settings:%1:Display characters of each value"   //测量值为0/1时显示分组
#define DOCONFIG_DISP_DISCHAR_0      "Config:DO channel settings:Display settings:%1:Display characters of each value:0"
#define DOCONFIG_DISP_DISCHAR_1      "Config:DO channel settings:Display settings:%1:Display characters of each value:1"

//DO 代理树路径
#define DO_DELEGATE            "Config:DO Delegate"
#define DO_DELEGATE_FIRSTCHAN  "Config:DO Delegate:First-CH"
#define DO_DELEGATE_LASTCHAN   "Config:DO Delegate:Last-CH"

#define DO_DELEGATE_RANGE       "Config:DO Delegate:Range"
#define DO_DELEGATE_RANGE_RANGE_TYPE       "Config:DO Delegate:Range:Range:Type"
#define DO_DELEGATE_RANGE_RANGE_MIN        "Config:DO Delegate:Range:Range:Range Lower"
#define DO_DELEGATE_RANGE_RANGE_MAX        "Config:DO Delegate:Range:Range:Range Upper"
#define DO_DELEGATE_RANGE_RANGE_LOWER      "Config:DO Delegate:Range:Range:Span Lower"
#define DO_DELEGATE_RANGE_RANGE_UPPER      "Config:DO Delegate:Range:Range:Span Upper"
#define DO_DELEGATE_RANGE_RANGE_UNIT       "Config:DO Delegate:Range:Range:Unit"
#define DO_DELEGATE_RANGE_ACTION_ENERGIZE  "Config:DO Delegate:Range:Action:Energize/De-engergize"
#define DO_DELEGATE_RANGE_ACTION_ACTION    "Config:DO Delegate:Range:Action:Action"
#define DO_DELEGATE_RANGE_ACTION_RELAYACK  "Config:DO Delegate:Range:Action:Relay Action on ACK"
#define DO_DELEGATE_RANGE_ACTION_HOLD      "Config:DO Delegate:Range:Action:Hold"
#define DO_DELEGATE_RANGE_ACTION_INTERVAL  "Config:DO Delegate:Range:Action:Relay deactivated interval"

#define DO_DELEGATE_DISP_TAG_CHAR       "Config:DO Delegate:Display settings:Tag:Characters"
#define DO_DELEGATE_DISP_TAG_NO         "Config:DO Delegate:Display settings:Tag:No."
#define DO_DELEGATE_DISP_COLOR_COLOR    "Config:DO Delegate:Display settings:Color:Color"
#define DO_DELEGATE_DISP_ZONE_UPPER     "Config:DO Delegate:Display settings:Zone:Upper"
#define DO_DELEGATE_DISP_ZONE_LOWER     "Config:DO Delegate:Display settings:Zone:Lower"
#define DO_DELEGATE_DISP_SCALE_POS      "Config:DO Delegate:Display settings:Scale:Position"
#define DO_DELEGATE_DISP_BAR_POS        "Config:DO Delegate:Display settings:Bar graph:Base position"
#define DO_DELEGATE_DISP_DISCHAR_0      "Config:DO Delegate:Display settings:Display characters of each value:0"
#define DO_DELEGATE_DISP_DISCHAR_1      "Config:DO Delegate:Display settings:Display characters of each value:1"

//DO 值变更 XML文件路径
#define XML_DO_RANGE_RANGE_TYPE         ":/configtree/Image/configTree/DO/DO_ValueChange/DORange_RangeType.xml"
#define XML_DO_RANGE_RANGE_LOWER        ":/configtree/Image/configTree/DO/DO_ValueChange/DORange_RangeLower.xml"
#define XML_DO_RANGE_RANGE_UPPER        ":/configtree/Image/configTree/DO/DO_ValueChange/DORange_RangeUpper.xml"
#define XML_DO_RANGE_RANGE_UNIT         ":/configtree/Image/configTree/DO/DO_ValueChange/DORange_RangeUnit.xml"
#define XML_DO_RANGE_ACTION_ENERGIZE    ":/configtree/Image/configTree/DO/DO_ValueChange/DORange_ActionEnergize.xml"
#define XML_DO_RANGE_ACTION_ACTION      ":/configtree/Image/configTree/DO/DO_ValueChange/DORange_ActionAction.xml"
#define XML_DO_RANGE_ACTION_RELAYACK    ":/configtree/Image/configTree/DO/DO_ValueChange/DORange_ActionRelayAck.xml"
#define XML_DO_RANGE_ACTION_HOLD        ":/configtree/Image/configTree/DO/DO_ValueChange/DORange_ActionHold.xml"
#define XML_DO_RANGE_ACTION_INTERVAL    ":/configtree/Image/configTree/DO/DO_ValueChange/DORange_ActionInterval.xml"

#define XML_DO_DISP_COLOR_COLOR  ":/configtree/Image/configTree/DO/DO_ValueChange/DODisp_ColorColor.xml"
#define XML_DO_DISP_TAG_CHAR     ":/configtree/Image/configTree/DO/DO_ValueChange/DODisp_TagChar.xml"
#define XML_DO_DISP_TAG_NO       ":/configtree/Image/configTree/DO/DO_ValueChange/DODisp_TagNo.xml"
#define XML_DO_DISP_ZONE_UPPER   ":/configtree/Image/configTree/DO/DO_ValueChange/DODisp_ZoneUpper.xml"
#define XML_DO_DISP_ZONE_LOWER   ":/configtree/Image/configTree/DO/DO_ValueChange/DODisp_ZoneLower.xml"
#define XML_DO_DISP_SCALE_POS    ":/configtree/Image/configTree/DO/DO_ValueChange/DODisp_ScalePos.xml"
#define XML_DO_DISP_BAR_POS      ":/configtree/Image/configTree/DO/DO_ValueChange/DODisp_BarPos.xml"
#define XML_DO_DISP_DISCHAR_0    ":/configtree/Image/configTree/DO/DO_ValueChange/DODisp_DispChar0.xml"
#define XML_DO_DISP_DISCHAR_1    ":/configtree/Image/configTree/DO/DO_ValueChange/DODisp_DispChar1.xml"

#include "configobject.h"

class ConfigTreeObject_DO : public ConfigObject
{
    Q_OBJECT
public:
    static ConfigTreeObject_DO *instance(QObject *parent);

signals:

private:
    explicit ConfigTreeObject_DO(QObject *parent = 0);
    void updateStrShowOn();

    void updateStrShowRange();
    void updateStrShowDisplay();

public slots:
    void slotCreateDOsettingTree(QObject *parent = 0);
    void slotDOBoardInoutCfgTree(quint8 unit, quint8 module, quint8 chNum, bool in);

    void slotDODelegateChange();
public slots:
    void slotDOFirst_chanSelectchanged();
    void slotDOLast_chanSelectchanged();
    //DO_Range Range
    void slotDORange_RangeTypeChanged();
    void slotDORange_RangeUpperChanged();
    void slotDORange_RangeLowerChanged();
    void slotDORange_RangeUnitChanged();
    //DO_Range Action
    void slotDORange_ActionTypeChanged();
    void slotDORange_ActionActionChanged();
    void slotDORange_ActionRelayAckChanged();
    void slotDORange_ActionHoldChanged();
    void slotDORange_ActionRelayInChanged();

    //DO_Display Tag
    void slotDODisp_TagCharChanged();
    void slotDODisp_TagNoChanged();
    //DO_Display Color
    void slotDODisp_ColorColorChanged();
    //DO_Display Zone
    void slotDODisp_ZoneUpperChanged();
    void slotDODisp_ZoneLowerChanged();
    //DO_Display Scale
    void slotDODisp_ScalePosChanged();
    //DO_Display Bar
    void slotDODisp_BarPosChanged();
    //DO_Display Dischar
    void slotDODisp_DischarVal0Changed();
    void slotDODisp_DischarVal1Changed();

};


#endif // CONFIGTREEOBJECT_DO_H
