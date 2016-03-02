#ifndef CONFIGTREEOBJECT_DI_H
#define CONFIGTREEOBJECT_DI_H

//DI 配置树路径
#define DICONFIG    "Config:DI channel settings"
#define DICONFIG_RANG       "Config:DI channel settings:Range"
#define DICONFIG_ALARM      "Config:DI channel settings:Alarm"
#define DICONFIG_DISP       "Config:DI channel settings:Display settings"

#define DICONFIG_FIRSTCHAN  "Config:DI channel settings:First-CH"
#define DICONFIG_LASTCHAN   "Config:DI channel settings:Last-CH"

#define DICONFIG_RANG_RANGE             "Config:DI channel settings:Range:%1:Range" //分组Range
#define DICONFIG_RANG_RANGE_TYPE        "Config:DI channel settings:Range:%1:Range:Type"
#define DICONFIG_RANG_RANGE_MIN         "Config:DI channel settings:Range:%1:Range:Range Lower"
#define DICONFIG_RANG_RANGE_MAX         "Config:DI channel settings:Range:%1:Range:Range Upper"
#define DICONFIG_RANG_RANGE_LOWER       "Config:DI channel settings:Range:%1:Range:Span Lower"
#define DICONFIG_RANG_RANGE_UPPER       "Config:DI channel settings:Range:%1:Range:Span Upper"
#define DICONFIG_RANG_RANGE_CALCU       "Config:DI channel settings:Range:%1:Range:Calculation"
#define DICONFIG_RANG_RANGE_REFER       "Config:DI channel settings:Range:%1:Range:Reference channel"
#define DICONFIG_RANG_SCALE             "Config:DI channel settings:Range:%1:Scale"   //分组Scale
#define DICONFIG_RANG_SCALE_DEC         "Config:DI channel settings:Range:%1:Scale:Decimal place"
#define DICONFIG_RANG_SCALE_LOWER       "Config:DI channel settings:Range:%1:Scale:Scale Lower"
#define DICONFIG_RANG_SCALE_UPPER       "Config:DI channel settings:Range:%1:Scale:Scale Upper"
#define DICONFIG_RANG_SCALE_UNIT        "Config:DI channel settings:Range:%1:Scale:Unit"

#define DICONFIG_ALARM_LVL1             "Config:DI channel settings:Alarm:%1:Level 1"   //分组Level 1
#define DICONFIG_ALARM_LVL1_ONOFF       "Config:DI channel settings:Alarm:%1:Level 1:On/Off"
#define DICONFIG_ALARM_LVL1_TYPE        "Config:DI channel settings:Alarm:%1:Level 1:Type"
#define DICONFIG_ALARM_LVL1_VALUE       "Config:DI channel settings:Alarm:%1:Level 1:Value"
#define DICONFIG_ALARM_LVL1_HYST        "Config:DI channel settings:Alarm:%1:Level 1:Hysteresis"
#define DICONFIG_ALARM_LVL1_LOGG        "Config:DI channel settings:Alarm:%1:Level 1:Logging"
#define DICONFIG_ALARM_LVL1_OUTTYPE     "Config:DI channel settings:Alarm:%1:Level 1:Output type"
#define DICONFIG_ALARM_LVL1_OUTNO       "Config:DI channel settings:Alarm:%1:Level 1:Output No."
#define DICONFIG_ALARM_LVL2             "Config:DI channel settings:Alarm:%1:Level 2"   //分组Level 2
#define DICONFIG_ALARM_LVL2_ONOFF       "Config:DI channel settings:Alarm:%1:Level 2:On/Off"
#define DICONFIG_ALARM_LVL2_TYPE        "Config:DI channel settings:Alarm:%1:Level 2:Type"
#define DICONFIG_ALARM_LVL2_VALUE       "Config:DI channel settings:Alarm:%1:Level 2:Value"
#define DICONFIG_ALARM_LVL2_HYST        "Config:DI channel settings:Alarm:%1:Level 2:Hysteresis"
#define DICONFIG_ALARM_LVL2_LOGG        "Config:DI channel settings:Alarm:%1:Level 2:Logging"
#define DICONFIG_ALARM_LVL2_OUTTYPE     "Config:DI channel settings:Alarm:%1:Level 2:Output type"
#define DICONFIG_ALARM_LVL2_OUTNO       "Config:DI channel settings:Alarm:%1:Level 2:Output No."
#define DICONFIG_ALARM_LVL3             "Config:DI channel settings:Alarm:%1:Level 3"   //分组Level 3
#define DICONFIG_ALARM_LVL3_ONOFF       "Config:DI channel settings:Alarm:%1:Level 3:On/Off"
#define DICONFIG_ALARM_LVL3_TYPE        "Config:DI channel settings:Alarm:%1:Level 3:Type"
#define DICONFIG_ALARM_LVL3_VALUE       "Config:DI channel settings:Alarm:%1:Level 3:Value"
#define DICONFIG_ALARM_LVL3_HYST        "Config:DI channel settings:Alarm:%1:Level 3:Hysteresis"
#define DICONFIG_ALARM_LVL3_LOGG        "Config:DI channel settings:Alarm:%1:Level 3:Logging"
#define DICONFIG_ALARM_LVL3_OUTTYPE     "Config:DI channel settings:Alarm:%1:Level 3:Output type"
#define DICONFIG_ALARM_LVL3_OUTNO       "Config:DI channel settings:Alarm:%1:Level 3:Output No."
#define DICONFIG_ALARM_LVL4             "Config:DI channel settings:Alarm:%1:Level 4"   //分组Level 4
#define DICONFIG_ALARM_LVL4_ONOFF       "Config:DI channel settings:Alarm:%1:Level 4:On/Off"
#define DICONFIG_ALARM_LVL4_TYPE        "Config:DI channel settings:Alarm:%1:Level 4:Type"
#define DICONFIG_ALARM_LVL4_VALUE       "Config:DI channel settings:Alarm:%1:Level 4:Value"
#define DICONFIG_ALARM_LVL4_HYST        "Config:DI channel settings:Alarm:%1:Level 4:Hysteresis"
#define DICONFIG_ALARM_LVL4_LOGG        "Config:DI channel settings:Alarm:%1:Level 4:Logging"
#define DICONFIG_ALARM_LVL4_OUTTYPE     "Config:DI channel settings:Alarm:%1:Level 4:Output type"
#define DICONFIG_ALARM_LVL4_OUTNO       "Config:DI channel settings:Alarm:%1:Level 4:Output No."
#define DICONFIG_ALARM_DELAY            "Config:DI channel settings:Alarm:%1:Alarm delay"   //分组报警Delay
#define DICONFIG_ALARM_DELAY_HOUR       "Config:DI channel settings:Alarm:%1:Alarm delay:Hour"
#define DICONFIG_ALARM_DELAY_MIN        "Config:DI channel settings:Alarm:%1:Alarm delay:Minute"
#define DICONFIG_ALARM_DELAY_SEC        "Config:DI channel settings:Alarm:%1:Alarm delay:Second"

#define DICONFIG_DISP_TAG               "Config:DI channel settings:Display settings:%1:Tag"    //标记分组
#define DICONFIG_DISP_TAG_CHAR          "Config:DI channel settings:Display settings:%1:Tag:Characters"
#define DICONFIG_DISP_TAG_NO            "Config:DI channel settings:Display settings:%1:Tag:No."
#define DICONFIG_DISP_COLOR             "Config:DI channel settings:Display settings:%1:Color"    //颜色分组
#define DICONFIG_DISP_COLOR_COLOR       "Config:DI channel settings:Display settings:%1:Color:Color"
#define DICONFIG_DISP_ZONE              "Config:DI channel settings:Display settings:%1:Zone"   //显示区域分组
#define DICONFIG_DISP_ZONE_UPPER        "Config:DI channel settings:Display settings:%1:Zone:Upper"
#define DICONFIG_DISP_ZONE_LOWER        "Config:DI channel settings:Display settings:%1:Zone:Lower"
#define DICONFIG_DISP_SCALE             "Config:DI channel settings:Display settings:%1:Scale"  //标尺分组
#define DICONFIG_DISP_SCALE_POS         "Config:DI channel settings:Display settings:%1:Scale:Position"
#define DICONFIG_DISP_SCALE_DIV         "Config:DI channel settings:Display settings:%1:Scale:Division"
#define DICONFIG_DISP_BAR               "Config:DI channel settings:Display settings:%1:Bar graph"  //棒图分组
#define DICONFIG_DISP_BAR_BASEPOS       "Config:DI channel settings:Display settings:%1:Bar graph:Base position"
#define DICONFIG_DISP_BAR_DIVISION      "Config:DI channel settings:Display settings:%1:Bar graph:Division"
#define DICONFIG_DISP_ALARM             "Config:DI channel settings:Display settings:%1:Alarm point mark"     //报警标记分组
#define DICONFIG_DISP_ALARM_INDIC       "Config:DI channel settings:Display settings:%1:Alarm point mark:Indicate on Scale"
#define DICONFIG_DISP_ALARM_MARKTYPE    "Config:DI channel settings:Display settings:%1:Alarm point mark:Mark type"
#define DICONFIG_DISP_ALARM_COLOR1      "Config:DI channel settings:Display settings:%1:Alarm point mark:Alarm 1 color"
#define DICONFIG_DISP_ALARM_COLOR2      "Config:DI channel settings:Display settings:%1:Alarm point mark:Alarm 2 color"
#define DICONFIG_DISP_ALARM_COLOR3      "Config:DI channel settings:Display settings:%1:Alarm point mark:Alarm 3 color"
#define DICONFIG_DISP_ALARM_COLOR4      "Config:DI channel settings:Display settings:%1:Alarm point mark:Alarm 4 color"
#define DICONFIG_DISP_DISPCHAR          "Config:DI channel settings:Display settings:%1:Display characters of each value"    //测量值为0/1时显示分组
#define DICONFIG_DISP_DISPCHAR_0        "Config:DI channel settings:Display settings:%1:Display characters of each value:0"
#define DICONFIG_DISP_DISPCHAR_1        "Config:DI channel settings:Display settings:%1:Display characters of each value:1"

//DI 代理树路径
#define DI_DELEGATE              "Config:DI Delegate"
#define DI_DELEGATE_FIRSTCHAN    "Config:DI Delegate:First-CH"
#define DI_DELEGATE_LASTCHAN     "Config:DI Delegate:Last-CH"

#define DI_DELEGATE_RANG_RANGE_TYPE     "Config:DI Delegate:Range:Range:Type"
#define DI_DELEGATE_RANG_RANGE_MIN      "Config:DI Delegate:Range:Range:Range Lower"
#define DI_DELEGATE_RANG_RANGE_MAX      "Config:DI Delegate:Range:Range:Range Upper"
#define DI_DELEGATE_RANG_RANGE_LOWER    "Config:DI Delegate:Range:Range:Span Lower"
#define DI_DELEGATE_RANG_RANGE_UPPER    "Config:DI Delegate:Range:Range:Span Upper"
#define DI_DELEGATE_RANG_RANGE_CALCU    "Config:DI Delegate:Range:Range:Calculation"
#define DI_DELEGATE_RANG_RANGE_REFER    "Config:DI Delegate:Range:Range:Reference channel"
#define DI_DELEGATE_RANG_SCALE_DEC      "Config:DI Delegate:Range:Scale:Decimal place"
#define DI_DELEGATE_RANG_SCALE_LOWER    "Config:DI Delegate:Range:Scale:Scale Lower"
#define DI_DELEGATE_RANG_SCALE_UPPER    "Config:DI Delegate:Range:Scale:Scale Upper"
#define DI_DELEGATE_RANG_SCALE_UNIT     "Config:DI Delegate:Range:Scale:Unit"

#define DI_DELEGATE_ALARM    "Config:DI Delegate:Alarm"
#define DI_DELEGATE_ALARM_LVL1_ONOFF    "Config:DI Delegate:Alarm:Level 1:On/Off"
#define DI_DELEGATE_ALARM_LVL1_TYPE     "Config:DI Delegate:Alarm:Level 1:Type"
#define DI_DELEGATE_ALARM_LVL1_VALUE    "Config:DI Delegate:Alarm:Level 1:Value"
#define DI_DELEGATE_ALARM_LVL1_HYST     "Config:DI Delegate:Alarm:Level 1:Hysteresis"
#define DI_DELEGATE_ALARM_LVL1_LOGG     "Config:DI Delegate:Alarm:Level 1:Logging"
#define DI_DELEGATE_ALARM_LVL1_OUTTYPE  "Config:DI Delegate:Alarm:Level 1:Output type"
#define DI_DELEGATE_ALARM_LVL1_OUTNO    "Config:DI Delegate:Alarm:Level 1:Output No."
#define DI_DELEGATE_ALARM_LVL2_ONOFF    "Config:DI Delegate:Alarm:Level 2:On/Off"
#define DI_DELEGATE_ALARM_LVL2_TYPE     "Config:DI Delegate:Alarm:Level 2:Type"
#define DI_DELEGATE_ALARM_LVL2_VALUE    "Config:DI Delegate:Alarm:Level 2:Value"
#define DI_DELEGATE_ALARM_LVL2_HYST     "Config:DI Delegate:Alarm:Level 2:Hysteresis"
#define DI_DELEGATE_ALARM_LVL2_LOGG     "Config:DI Delegate:Alarm:Level 2:Logging"
#define DI_DELEGATE_ALARM_LVL2_OUTTYPE  "Config:DI Delegate:Alarm:Level 2:Output type"
#define DI_DELEGATE_ALARM_LVL2_OUTNO    "Config:DI Delegate:Alarm:Level 2:Output No."
#define DI_DELEGATE_ALARM_LVL3_ONOFF    "Config:DI Delegate:Alarm:Level 3:On/Off"
#define DI_DELEGATE_ALARM_LVL3_TYPE     "Config:DI Delegate:Alarm:Level 3:Type"
#define DI_DELEGATE_ALARM_LVL3_VALUE    "Config:DI Delegate:Alarm:Level 3:Value"
#define DI_DELEGATE_ALARM_LVL3_HYST     "Config:DI Delegate:Alarm:Level 3:Hysteresis"
#define DI_DELEGATE_ALARM_LVL3_LOGG     "Config:DI Delegate:Alarm:Level 3:Logging"
#define DI_DELEGATE_ALARM_LVL3_OUTTYPE  "Config:DI Delegate:Alarm:Level 3:Output type"
#define DI_DELEGATE_ALARM_LVL3_OUTNO    "Config:DI Delegate:Alarm:Level 3:Output No."
#define DI_DELEGATE_ALARM_LVL4_ONOFF    "Config:DI Delegate:Alarm:Level 4:On/Off"
#define DI_DELEGATE_ALARM_LVL4_TYPE     "Config:DI Delegate:Alarm:Level 4:Type"
#define DI_DELEGATE_ALARM_LVL4_VALUE    "Config:DI Delegate:Alarm:Level 4:Value"
#define DI_DELEGATE_ALARM_LVL4_HYST     "Config:DI Delegate:Alarm:Level 4:Hysteresis"
#define DI_DELEGATE_ALARM_LVL4_LOGG     "Config:DI Delegate:Alarm:Level 4:Logging"
#define DI_DELEGATE_ALARM_LVL4_OUTTYPE  "Config:DI Delegate:Alarm:Level 4:Output type"
#define DI_DELEGATE_ALARM_LVL4_OUTNO    "Config:DI Delegate:Alarm:Level 4:Output No."
#define DI_DELEGATE_ALARM_DELAY_HOUR    "Config:DI Delegate:Alarm:Alarm delay:Hour"
#define DI_DELEGATE_ALARM_DELAY_MIN     "Config:DI Delegate:Alarm:Alarm delay:Minute"
#define DI_DELEGATE_ALARM_DELAY_SEC     "Config:DI Delegate:Alarm:Alarm delay:Second"

#define DI_DELEGATE_DISP_TAG_CHAR       "Config:DI Delegate:Display settings:Tag:Characters"
#define DI_DELEGATE_DISP_TAG_NO         "Config:DI Delegate:Display settings:Tag:No."
#define DI_DELEGATE_DISP_COLOR_COLOR    "Config:DI Delegate:Display settings:Color:Color"
#define DI_DELEGATE_DISP_ZONE_UPPER     "Config:DI Delegate:Display settings:Zone:Upper"
#define DI_DELEGATE_DISP_ZONE_LOWER     "Config:DI Delegate:Display settings:Zone:Lower"
#define DI_DELEGATE_DISP_SCALE_POS      "Config:DI Delegate:Display settings:Scale:Position"
#define DI_DELEGATE_DISP_SCALE_DIV      "Config:DI Delegate:Display settings:Scale:Division"
#define DI_DELEGATE_DISP_BAR_BASEPOS    "Config:DI Delegate:Display settings:Bar graph:Base position"
#define DI_DELEGATE_DISP_BAR_DIVISION   "Config:DI Delegate:Display settings:Bar graph:Division"
#define DI_DELEGATE_DISP_ALARM_INDIC    "Config:DI Delegate:Display settings:Alarm point mark:Indicate on Scale"
#define DI_DELEGATE_DISP_ALARM_MARKTYPE "Config:DI Delegate:Display settings:Alarm point mark:Mark type"
#define DI_DELEGATE_DISP_ALARM_COLOR1   "Config:DI Delegate:Display settings:Alarm point mark:Alarm 1 color"
#define DI_DELEGATE_DISP_ALARM_COLOR2   "Config:DI Delegate:Display settings:Alarm point mark:Alarm 2 color"
#define DI_DELEGATE_DISP_ALARM_COLOR3   "Config:DI Delegate:Display settings:Alarm point mark:Alarm 3 color"
#define DI_DELEGATE_DISP_ALARM_COLOR4   "Config:DI Delegate:Display settings:Alarm point mark:Alarm 4 color"
#define DI_DELEGATE_DISP_DISPCHAR_0     "Config:DI Delegate:Display settings:Display characters of each value:0"
#define DI_DELEGATE_DISP_DISPCHAR_1     "Config:DI Delegate:Display settings:Display characters of each value:1"

//DI 值变更 XML文件路径
#define XML_DI_RANGE_RANGE_TYPE     ":/configtree/Image/configTree/DI/DI_ValueChange/DIRange_RangeType.xml"
//#define XML_DI_RANGE_RANGE_LOWER    ":/configtree/Image/configTree/DI/DI_ValueChange/DIRange_RangeLower.xml"
//#define XML_DI_RANGE_RANGE_UPPER    ":/configtree/Image/configTree/DI/DI_ValueChange/DIRange_RangeUpper.xml"
#define XML_DI_RANGE_RANGE_CALCU    ":/configtree/Image/configTree/DI/DI_ValueChange/DIRange_RangeCalcu.xml"
//#define XML_DI_RANGE_RANGE_REFER    ":/configtree/Image/configTree/DI/DI_ValueChange/DIRange_RangeRefer.xml"
#define XML_DI_RANGE_SCALE_DEC      ":/configtree/Image/configTree/DI/DI_ValueChange/DIRange_ScaleDec.xml"
//#define XML_DI_RANGE_SCALE_LOWER    ":/configtree/Image/configTree/DI/DI_ValueChange/DIRange_ScaleLower.xml"
//#define XML_DI_RANGE_SCALE_UPPER    ":/configtree/Image/configTree/DI/DI_ValueChange/DIRange_ScaleUpper.xml"
//#define XML_DI_RANGE_SCALE_UNIT     ":/configtree/Image/configTree/DI/DI_ValueChange/DIRange_ScaleUnit.xml"

#define XML_DI_ALARM_LVL1_ONOFF     ":/configtree/Image/configTree/DI/DI_ValueChange/DIAlarm_Lvl1Onoff.xml"
//#define XML_DI_ALARM_LVL1_ONOFF_C   ":/configtree/Image/configTree/DI/DI_ValueChange/DIAlarm_Lvl1Onoff_c.xml"
#define XML_DI_ALARM_LVL1_TYPE      ":/configtree/Image/configTree/DI/DI_ValueChange/DIAlarm_Lvl1Type.xml"
//#define XML_DI_ALARM_LVL1_VALUE     ":/configtree/Image/configTree/DI/DI_ValueChange/DIAlarm_Lvl1Value.xml"
//#define XML_DI_ALARM_LVL1_HYST      ":/configtree/Image/configTree/DI/DI_ValueChange/DIAlarm_Lvl1Hyst.xml"
#define XML_DI_ALARM_LVL1_LOGG      ":/configtree/Image/configTree/DI/DI_ValueChange/DIAlarm_Lvl1Logg.xml"
#define XML_DI_ALARM_LVL1_OUTTYPE   ":/configtree/Image/configTree/DI/DI_ValueChange/DIAlarm_Lvl1Outype.xml"
//#define XML_DI_ALARM_LVL1_OUTNO     ":/configtree/Image/configTree/DI/DI_ValueChange/DIAlarm_Lvl1OutNo.xml"
#define XML_DI_ALARM_LVL2_ONOFF     ":/configtree/Image/configTree/DI/DI_ValueChange/DIAlarm_Lvl2Onoff.xml"
//#define XML_DI_ALARM_LVL2_ONOFF_C   ":/configtree/Image/configTree/DI/DI_ValueChange/DIAlarm_Lvl2Onoff_c.xml"
#define XML_DI_ALARM_LVL2_TYPE      ":/configtree/Image/configTree/DI/DI_ValueChange/DIAlarm_Lvl2Type.xml"
//#define XML_DI_ALARM_LVL2_VALUE     ":/configtree/Image/configTree/DI/DI_ValueChange/DIAlarm_Lvl2Value.xml"
//#define XML_DI_ALARM_LVL2_HYST      ":/configtree/Image/configTree/DI/DI_ValueChange/DIAlarm_Lvl2Hyst.xml"
#define XML_DI_ALARM_LVL2_LOGG      ":/configtree/Image/configTree/DI/DI_ValueChange/DIAlarm_Lvl2Logg.xml"
#define XML_DI_ALARM_LVL2_OUTTYPE   ":/configtree/Image/configTree/DI/DI_ValueChange/DIAlarm_Lvl2Outype.xml"
//#define XML_DI_ALARM_LVL2_OUTNO     ":/configtree/Image/configTree/DI/DI_ValueChange/DIAlarm_Lvl2OutNo.xml"
#define XML_DI_ALARM_LVL3_ONOFF     ":/configtree/Image/configTree/DI/DI_ValueChange/DIAlarm_Lvl3Onoff.xml"
//#define XML_DI_ALARM_LVL3_ONOFF_C   ":/configtree/Image/configTree/DI/DI_ValueChange/DIAlarm_Lvl3Onoff_c.xml"
#define XML_DI_ALARM_LVL3_TYPE      ":/configtree/Image/configTree/DI/DI_ValueChange/DIAlarm_Lvl3Type.xml"
//#define XML_DI_ALARM_LVL3_VALUE     ":/configtree/Image/configTree/DI/DI_ValueChange/DIAlarm_Lvl3Value.xml"
//#define XML_DI_ALARM_LVL3_HYST      ":/configtree/Image/configTree/DI/DI_ValueChange/DIAlarm_Lvl3Hyst.xml"
#define XML_DI_ALARM_LVL3_LOGG      ":/configtree/Image/configTree/DI/DI_ValueChange/DIAlarm_Lvl3Logg.xml"
#define XML_DI_ALARM_LVL3_OUTTYPE   ":/configtree/Image/configTree/DI/DI_ValueChange/DIAlarm_Lvl3Outype.xml"
//#define XML_DI_ALARM_LVL3_OUTNO     ":/configtree/Image/configTree/DI/DI_ValueChange/DIAlarm_Lvl3OutNo.xml"
#define XML_DI_ALARM_LVL4_ONOFF     ":/configtree/Image/configTree/DI/DI_ValueChange/DIAlarm_Lvl4Onoff.xml"
//#define XML_DI_ALARM_LVL4_ONOFF_C   ":/configtree/Image/configTree/DI/DI_ValueChange/DIAlarm_Lvl4Onoff_c.xml"
#define XML_DI_ALARM_LVL4_TYPE      ":/configtree/Image/configTree/DI/DI_ValueChange/DIAlarm_Lvl4Type.xml"
//#define XML_DI_ALARM_LVL4_VALUE     ":/configtree/Image/configTree/DI/DI_ValueChange/DIAlarm_Lvl4Value.xml"
//#define XML_DI_ALARM_LVL4_HYST      ":/configtree/Image/configTree/DI/DI_ValueChange/DIAlarm_Lvl4Hyst.xml"
#define XML_DI_ALARM_LVL4_LOGG      ":/configtree/Image/configTree/DI/DI_ValueChange/DIAlarm_Lvl4Logg.xml"
#define XML_DI_ALARM_LVL4_OUTTYPE   ":/configtree/Image/configTree/DI/DI_ValueChange/DIAlarm_Lvl4Outype.xml"
//#define XML_DI_ALARM_LVL4_OUTNO     ":/configtree/Image/configTree/DI/DI_ValueChange/DIAlarm_Lvl4OutNo.xml"
//#define XML_DI_ALARM_DELAY_HOUR     ":/configtree/Image/configTree/DI/DI_ValueChange/DIAlarm_DelayHour.xml"
//#define XML_DI_ALARM_DELAY_MIN      ":/configtree/Image/configTree/DI/DI_ValueChange/DIAlarm_DelayMin.xml"
//#define XML_DI_ALARM_DELAY_SEC      ":/configtree/Image/configTree/DI/DI_ValueChange/DIAlarm_DelaySec.xml"

//#define XML_DI_DISP_TAG_CHAR        ":/configtree/Image/configTree/DI/DI_ValueChange/DIDisp_TagChar.xml"
//#define XML_DI_DISP_TAG_NO          ":/configtree/Image/configTree/DI/DI_ValueChange/DIDisp_TagNo.xml"
//#define XML_DI_DISP_COLOR_COLOR     ":/configtree/Image/configTree/DI/DI_ValueChange/DIDisp_ColorColor.xml"
//#define XML_DI_DISP_ZONE_UPPER      ":/configtree/Image/configTree/DI/DI_ValueChange/DIDisp_ZoneUpper.xml"
//#define XML_DI_DISP_ZONE_LOWER      ":/configtree/Image/configTree/DI/DI_ValueChange/DIDisp_ZoneLower.xml"
//#define XML_DI_DISP_SCALE_POS       ":/configtree/Image/configTree/DI/DI_ValueChange/DIDisp_ScalePos.xml"
//#define XML_DI_DISP_SCALE_DIV       ":/configtree/Image/configTree/DI/DI_ValueChange/DIDisp_ScaleDiv.xml"
//#define XML_DI_DISP_BAR_BASEPOS     ":/configtree/Image/configTree/DI/DI_ValueChange/DIDisp_BarBasePos.xml"
//#define XML_DI_DISP_BAR_DIVISION    ":/configtree/Image/configTree/DI/DI_ValueChange/DIDisp_BarDiv.xml"
//#define XML_DI_DISP_ALARM_INDIC     ":/configtree/Image/configTree/DI/DI_ValueChange/DIDisp_AlarmIndic.xml"
#define XML_DI_DISP_ALARM_MARKTYPE  ":/configtree/Image/configTree/DI/DI_ValueChange/DIDisp_AlarmMarkType.xml"
//#define XML_DI_DISP_ALARM_COLOR1    ":/configtree/Image/configTree/DI/DI_ValueChange/DIDisp_AlarmColor1.xml"
//#define XML_DI_DISP_ALARM_COLOR2    ":/configtree/Image/configTree/DI/DI_ValueChange/DIDisp_AlarmColor2.xml"
//#define XML_DI_DISP_ALARM_COLOR3    ":/configtree/Image/configTree/DI/DI_ValueChange/DIDisp_AlarmColor3.xml"
//#define XML_DI_DISP_ALARM_COLOR4    ":/configtree/Image/configTree/DI/DI_ValueChange/DIDisp_AlarmColor4.xml"
//#define XML_DI_DISP_DISPCHAR_0      ":/configtree/Image/configTree/DI/DI_ValueChange/DIDisp_DisChar_0.xml"
//#define XML_DI_DISP_DISPCHAR_1      ":/configtree/Image/configTree/DI/DI_ValueChange/DIDisp_DisChar_1.xml"

#include "configobject.h"

class ConfigTreeObject_DI : public ConfigObject
{
    Q_OBJECT
public:
    static ConfigTreeObject_DI *instance(QObject *parent);

signals:

private:
    explicit ConfigTreeObject_DI(QObject *parent = 0);
    void updateStrShowOn();

    void updateStrShowRange();
    void updateStrShowAlarm();
    void updateStrShowDisplay();

public slots:
    void slotCreateDIsettingTree(QObject *parent = 0);
    void slotDIBoardInoutCfgTree(quint8 unit, quint8 module, quint8 chNum, bool in);

    void slotDIDelegateChange();
public slots:
    //DI_channel change
    void slotDIFirst_chanSelectchanged();
    void slotDILast_chanSelectchanged();

    //DI_Range Range
    void slotDIRange_RangeTypeChanged();
    void slotDIRange_SpanLowerChanged();
    void slotDIRange_SpanUpperChanged();
    void slotDIRange_CalculationChanged();
    void slotDIRange_RefChannelChanged();
    //DI_Range Scale
    void slotDIRange_ScaleDecPosChanged();
    void slotDIRange_ScaleLowerChanged();
    void slotDIRange_ScaleUpperChanged();
    void slotDIRange_ScaleUnitChanged();

    //DI_Alarm Level 1
    void slotDIAlarm_Level1OnoffChanged();
    void slotDIAlarm_Level1TypeChanged();
    void slotDIAlarm_Level1ValueChanged();
    void slotDIAlarm_Level1HysterChanged();
    void slotDIAlarm_Level1LogChanged();
    void slotDIAlarm_Level1OutTypeChanged();
    void slotDIAlarm_Level1OutNoChanged();
    //DI_Alarm Level 2
    void slotDIAlarm_Level2OnoffChanged();
    void slotDIAlarm_Level2TypeChanged();
    void slotDIAlarm_Level2ValueChanged();
    void slotDIAlarm_Level2HysterChanged();
    void slotDIAlarm_Level2LogChanged();
    void slotDIAlarm_Level2OutTypeChanged();
    void slotDIAlarm_Level2OutNoChanged();
    //DI_Alarm Level 3
    void slotDIAlarm_Level3OnoffChanged();
    void slotDIAlarm_Level3TypeChanged();
    void slotDIAlarm_Level3ValueChanged();
    void slotDIAlarm_Level3HysterChanged();
    void slotDIAlarm_Level3LogChanged();
    void slotDIAlarm_Level3OutTypeChanged();
    void slotDIAlarm_Level3OutNoChanged();
    //DI_Alarm Level 4
    void slotDIAlarm_Level4OnoffChanged();
    void slotDIAlarm_Level4TypeChanged();
    void slotDIAlarm_Level4ValueChanged();
    void slotDIAlarm_Level4HysterChanged();
    void slotDIAlarm_Level4LogChanged();
    void slotDIAlarm_Level4OutTypeChanged();
    void slotDIAlarm_Level4OutNoChanged();
    //DI_Alarm Delay H:M:S
    void slotDIAlarm_DelayHourChanged();
    void slotDIAlarm_DelayMinChanged();
    void slotDIAlarm_DelaySecChanged();

    //DI_Disp Color
    void slotDIDisp_ColorColorChanged();
    //DI_Disp Tag
    void slotDIDisp_TagCharChanged();
    void slotDIDisp_TagNoChanged();
    //DI_Disp Zone
    void slotDIDisp_ZoneLowerChanged();
    void slotDIDisp_ZoneUpperChanged();
    //DI_Disp Scale
    void slotDIDisp_ScalePosChanged();
    void slotDIDisp_ScaleDivChanged();
    //DI_Disp Bar graph
    void slotDIDisp_BarBasePosChanged();
    void slotDIDisp_BarDivChanged();
    //DI_Disp Alarm point mark
    void slotDIDisp_AlarmIndicateChanged();
    void slotDIDisp_AlarmMarkTypeChanged();
    void slotDIDisp_AlarmAlarm1Changed();
    void slotDIDisp_AlarmAlarm2Changed();
    void slotDIDisp_AlarmAlarm3Changed();
    void slotDIDisp_AlarmAlarm4Changed();
    //DI_Disp Display characters of each value
    void slotDIDisp_DisVal_0_Changed();
    void slotDIDisp_DisVal_1_Changed();
};

#endif // CONFIGTREEOBJECT_DI_H
