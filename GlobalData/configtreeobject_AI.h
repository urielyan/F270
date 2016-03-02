#ifndef CONFIGTREEOBJECT_AI_H
#define CONFIGTREEOBJECT_AI_H

//AI 配置树路径
#define AICONFIG  "Config:AI channel settings"
#define AICONFIG_RANG             "Config:AI channel settings:Range"
#define AICONFIG_ALARM            "Config:AI channel settings:Alarm"
#define AICONFIG_DISP             "Config:AI channel settings:Display settings"
#define AICONFIG_CALI             "Config:AI channel settings:Calibration correction"

#define AICONFIG_FIRSTCHAN  "Config:AI channel settings:First-CH"
#define AICONFIG_LASTCHAN   "Config:AI channel settings:Last-CH"

#define AICONFIG_RANG_RANGE             "Config:AI channel settings:Range:%1:Range" //分组Range
#define AICONFIG_RANG_RANGE_TYPE        "Config:AI channel settings:Range:%1:Range:Type"
#define AICONFIG_RANG_RANGE_RANGE       "Config:AI channel settings:Range:%1:Range:Range"
#define AICONFIG_RANG_RANGE_MIN         "Config:AI channel settings:Range:%1:Range:Range Lower"
#define AICONFIG_RANG_RANGE_MAX         "Config:AI channel settings:Range:%1:Range:Range Upper"
#define AICONFIG_RANG_RANGE_LOWER       "Config:AI channel settings:Range:%1:Range:Span Lower"
#define AICONFIG_RANG_RANGE_UPPER       "Config:AI channel settings:Range:%1:Range:Span Upper"
#define AICONFIG_RANG_RANGE_CALCU       "Config:AI channel settings:Range:%1:Range:Calculation"
#define AICONFIG_RANG_RANGE_REFER       "Config:AI channel settings:Range:%1:Range:Reference channel"
#define AICONFIG_RANG_SCALE             "Config:AI channel settings:Range:%1:Scale"   //分组Scale
#define AICONFIG_RANG_SCALE_DEC         "Config:AI channel settings:Range:%1:Scale:Decimal place"
#define AICONFIG_RANG_SCALE_LOWER       "Config:AI channel settings:Range:%1:Scale:Scale Lower"
#define AICONFIG_RANG_SCALE_UPPER       "Config:AI channel settings:Range:%1:Scale:Scale Upper"
#define AICONFIG_RANG_SCALE_UNIT        "Config:AI channel settings:Range:%1:Scale:Unit"
#define AICONFIG_RANGE_LOWCUT           "Config:AI channel settings:Range:%1:Low-cut"  //分组Low-out
#define AICONFIG_RANGE_LOWCUT_ONOFF     "Config:AI channel settings:Range:%1:Low-cut:On/Off"
#define AICONFIG_RANGE_LOWCUT_VALUE     "Config:AI channel settings:Range:%1:Low-cut:Low-cut value"
#define AICONFIG_RANGE_LOWCUT_OUTPUT    "Config:AI channel settings:Range:%1:Low-cut:Low-cut output"
#define AICONFIG_RANGE_MOVAVE           "Config:AI channel settings:Range:%1:Moving average"  //分组Moving average
#define AICONFIG_RANGE_MOVAVE_ONOFF     "Config:AI channel settings:Range:%1:Moving average:On/Off"
#define AICONFIG_RANGE_MOVAVE_COUNT     "Config:AI channel settings:Range:%1:Moving average:Count"
#define AICONFIG_RANGE_RJC              "Config:AI channel settings:Range:%1:RJC"      //分组RJC
#define AICONFIG_RANGE_RJC_MODE         "Config:AI channel settings:Range:%1:RJC:Mode"
#define AICONFIG_RANGE_RJC_TEMPER       "Config:AI channel settings:Range:%1:RJC:Temperature"
#define AICONFIG_RANGE_BURN             "Config:AI channel settings:Range:%1:Burnout set"  //分组Burnout set
#define AICONFIG_RANGE_BURN_MODE        "Config:AI channel settings:Range:%1:Burnout set:Mode"
#define AICONFIG_RANGE_BIAS             "Config:AI channel settings:Range:%1:Bias"     //分组Bias
#define AICONFIG_RANGE_BIAS_VALUE       "Config:AI channel settings:Range:%1:Bias:Value"

#define AICONFIG_ALARM_LVL1             "Config:AI channel settings:Alarm:%1:Level 1"   //分组Level 1
#define AICONFIG_ALARM_LVL1_ONOFF       "Config:AI channel settings:Alarm:%1:Level 1:On/Off"
#define AICONFIG_ALARM_LVL1_TYPE        "Config:AI channel settings:Alarm:%1:Level 1:Type"
#define AICONFIG_ALARM_LVL1_VALUE       "Config:AI channel settings:Alarm:%1:Level 1:Value"
#define AICONFIG_ALARM_LVL1_HYST        "Config:AI channel settings:Alarm:%1:Level 1:Hysteresis"
#define AICONFIG_ALARM_LVL1_LOGG        "Config:AI channel settings:Alarm:%1:Level 1:Logging"
#define AICONFIG_ALARM_LVL1_OUTTYPE     "Config:AI channel settings:Alarm:%1:Level 1:Output type"
#define AICONFIG_ALARM_LVL1_OUTNO       "Config:AI channel settings:Alarm:%1:Level 1:Output No."
#define AICONFIG_ALARM_LVL2             "Config:AI channel settings:Alarm:%1:Level 2"   //分组Level 2
#define AICONFIG_ALARM_LVL2_ONOFF       "Config:AI channel settings:Alarm:%1:Level 2:On/Off"
#define AICONFIG_ALARM_LVL2_TYPE        "Config:AI channel settings:Alarm:%1:Level 2:Type"
#define AICONFIG_ALARM_LVL2_VALUE       "Config:AI channel settings:Alarm:%1:Level 2:Value"
#define AICONFIG_ALARM_LVL2_HYST        "Config:AI channel settings:Alarm:%1:Level 2:Hysteresis"
#define AICONFIG_ALARM_LVL2_LOGG        "Config:AI channel settings:Alarm:%1:Level 2:Logging"
#define AICONFIG_ALARM_LVL2_OUTTYPE     "Config:AI channel settings:Alarm:%1:Level 2:Output type"
#define AICONFIG_ALARM_LVL2_OUTNO       "Config:AI channel settings:Alarm:%1:Level 2:Output No."
#define AICONFIG_ALARM_LVL3             "Config:AI channel settings:Alarm:%1:Level 3"   //分组Level 3
#define AICONFIG_ALARM_LVL3_ONOFF       "Config:AI channel settings:Alarm:%1:Level 3:On/Off"
#define AICONFIG_ALARM_LVL3_TYPE        "Config:AI channel settings:Alarm:%1:Level 3:Type"
#define AICONFIG_ALARM_LVL3_VALUE       "Config:AI channel settings:Alarm:%1:Level 3:Value"
#define AICONFIG_ALARM_LVL3_HYST        "Config:AI channel settings:Alarm:%1:Level 3:Hysteresis"
#define AICONFIG_ALARM_LVL3_LOGG        "Config:AI channel settings:Alarm:%1:Level 3:Logging"
#define AICONFIG_ALARM_LVL3_OUTTYPE     "Config:AI channel settings:Alarm:%1:Level 3:Output type"
#define AICONFIG_ALARM_LVL3_OUTNO       "Config:AI channel settings:Alarm:%1:Level 3:Output No."
#define AICONFIG_ALARM_LVL4             "Config:AI channel settings:Alarm:%1:Level 4"   //分组Level 4
#define AICONFIG_ALARM_LVL4_ONOFF       "Config:AI channel settings:Alarm:%1:Level 4:On/Off"
#define AICONFIG_ALARM_LVL4_TYPE        "Config:AI channel settings:Alarm:%1:Level 4:Type"
#define AICONFIG_ALARM_LVL4_VALUE       "Config:AI channel settings:Alarm:%1:Level 4:Value"
#define AICONFIG_ALARM_LVL4_HYST        "Config:AI channel settings:Alarm:%1:Level 4:Hysteresis"
#define AICONFIG_ALARM_LVL4_LOGG        "Config:AI channel settings:Alarm:%1:Level 4:Logging"
#define AICONFIG_ALARM_LVL4_OUTTYPE     "Config:AI channel settings:Alarm:%1:Level 4:Output type"
#define AICONFIG_ALARM_LVL4_OUTNO       "Config:AI channel settings:Alarm:%1:Level 4:Output No."
#define AICONFIG_ALARM_DELAY            "Config:AI channel settings:Alarm:%1:Alarm delay"   //分组报警Delay
#define AICONFIG_ALARM_DELAY_HOUR       "Config:AI channel settings:Alarm:%1:Alarm delay:Hour"
#define AICONFIG_ALARM_DELAY_MIN        "Config:AI channel settings:Alarm:%1:Alarm delay:Minute"
#define AICONFIG_ALARM_DELAY_SEC        "Config:AI channel settings:Alarm:%1:Alarm delay:Second"

#define AICONFIG_DISP_TAG               "Config:AI channel settings:Display settings:%1:Tag"    //标记分组
#define AICONFIG_DISP_TAG_CHAR          "Config:AI channel settings:Display settings:%1:Tag:Characters"
#define AICONFIG_DISP_TAG_NO            "Config:AI channel settings:Display settings:%1:Tag:No."
#define AICONFIG_DISP_COLOR             "Config:AI channel settings:Display settings:%1:Color"    //颜色分组
#define AICONFIG_DISP_COLOR_COLOR       "Config:AI channel settings:Display settings:%1:Color:Color"
#define AICONFIG_DISP_ZONE              "Config:AI channel settings:Display settings:%1:Zone"   //显示区域分组
#define AICONFIG_DISP_ZONE_UPPER        "Config:AI channel settings:Display settings:%1:Zone:Upper"
#define AICONFIG_DISP_ZONE_LOWER        "Config:AI channel settings:Display settings:%1:Zone:Lower"
#define AICONFIG_DISP_SCALE             "Config:AI channel settings:Display settings:%1:Scale"  //标尺分组
#define AICONFIG_DISP_SCALE_POS         "Config:AI channel settings:Display settings:%1:Scale:Position"
#define AICONFIG_DISP_SCALE_DIV         "Config:AI channel settings:Display settings:%1:Scale:Division"
#define AICONFIG_DISP_BAR               "Config:AI channel settings:Display settings:%1:Bar graph"  //棒图分组
#define AICONFIG_DISP_BAR_BASEPOS       "Config:AI channel settings:Display settings:%1:Bar graph:Base position"
#define AICONFIG_DISP_BAR_DIVISION      "Config:AI channel settings:Display settings:%1:Bar graph:Division"
#define AICONFIG_DISP_PART              "Config:AI channel settings:Display settings:%1:Partial"    //
#define AICONFIG_DISP_PART_ONOFF        "Config:AI channel settings:Display settings:%1:Partial:On/Off"
#define AICONFIG_DISP_PART_EXPAND       "Config:AI channel settings:Display settings:%1:Partial:Expand"
#define AICONFIG_DISP_PART_BOUND        "Config:AI channel settings:Display settings:%1:Partial:Boundary"
#define AICONFIG_DISP_CSBAND            "Config:AI channel settings:Display settings:%1:Color scale band"   //
#define AICONFIG_DISP_CSBAND_AREA       "Config:AI channel settings:Display settings:%1:Color scale band:Band area"
#define AICONFIG_DISP_CSBAND_UPPER      "Config:AI channel settings:Display settings:%1:Color scale band:Display position Upper"
#define AICONFIG_DISP_CSBAND_LOWER      "Config:AI channel settings:Display settings:%1:Color scale band:Display position Lower"
#define AICONFIG_DISP_CSBAND_COLOR      "Config:AI channel settings:Display settings:%1:Color scale band:Color"
#define AICONFIG_DISP_ALARM             "Config:AI channel settings:Display settings:%1:Alarm point mark"     //报警标记分组
#define AICONFIG_DISP_ALARM_INDIC       "Config:AI channel settings:Display settings:%1:Alarm point mark:Indicate on Scale"
#define AICONFIG_DISP_ALARM_MARKTYPE    "Config:AI channel settings:Display settings:%1:Alarm point mark:Mark type"
#define AICONFIG_DISP_ALARM_COLOR1      "Config:AI channel settings:Display settings:%1:Alarm point mark:Alarm 1 color"
#define AICONFIG_DISP_ALARM_COLOR2      "Config:AI channel settings:Display settings:%1:Alarm point mark:Alarm 2 color"
#define AICONFIG_DISP_ALARM_COLOR3      "Config:AI channel settings:Display settings:%1:Alarm point mark:Alarm 3 color"
#define AICONFIG_DISP_ALARM_COLOR4      "Config:AI channel settings:Display settings:%1:Alarm point mark:Alarm 4 color"
#define AICONFIG_DISP_DISPCHAR          "Config:AI channel settings:Display settings:%1:Display characters of each value"    //测量值为0/1时显示分组
#define AICONFIG_DISP_DISPCHAR_0        "Config:AI channel settings:Display settings:%1:Display characters of each value:0"
#define AICONFIG_DISP_DISPCHAR_1        "Config:AI channel settings:Display settings:%1:Display characters of each value:1"

#define AICONFIG_CALI_MODE           "Config:AI channel settings:Calibration correction:%1:Mode"
#define AICONFIG_CALI_MODE_MODE      "Config:AI channel settings:Calibration correction:%1:Mode:Mode"
#define AICONFIG_CALI_MODE_NUM       "Config:AI channel settings:Calibration correction:%1:Mode:Number of set points"
#define AICONFIG_CALI_1              "Config:AI channel settings:Calibration correction:%1:1"
#define AICONFIG_CALI_1_INPUT        "Config:AI channel settings:Calibration correction:%1:1:Linearizer input"
#define AICONFIG_CALI_1_OUTPUT       "Config:AI channel settings:Calibration correction:%1:1:Linearizer output"
#define AICONFIG_CALI_1_EXEC         "Config:AI channel settings:Calibration correction:%1:1:Execution of the input measurement"
#define AICONFIG_CALI_2              "Config:AI channel settings:Calibration correction:%1:2"
#define AICONFIG_CALI_2_INPUT        "Config:AI channel settings:Calibration correction:%1:2:Linearizer input"
#define AICONFIG_CALI_2_OUTPUT       "Config:AI channel settings:Calibration correction:%1:2:Linearizer output"
#define AICONFIG_CALI_2_EXEC         "Config:AI channel settings:Calibration correction:%1:2:Execution of the input measurement"
#define AICONFIG_CALI_3              "Config:AI channel settings:Calibration correction:%1:3"
#define AICONFIG_CALI_3_INPUT        "Config:AI channel settings:Calibration correction:%1:3:Linearizer input"
#define AICONFIG_CALI_3_OUTPUT       "Config:AI channel settings:Calibration correction:%1:3:Linearizer output"
#define AICONFIG_CALI_3_EXEC         "Config:AI channel settings:Calibration correction:%1:3:Execution of the input measurement"
#define AICONFIG_CALI_4              "Config:AI channel settings:Calibration correction:%1:4"
#define AICONFIG_CALI_4_INPUT        "Config:AI channel settings:Calibration correction:%1:4:Linearizer input"
#define AICONFIG_CALI_4_OUTPUT       "Config:AI channel settings:Calibration correction:%1:4:Linearizer output"
#define AICONFIG_CALI_4_EXEC         "Config:AI channel settings:Calibration correction:%1:4:Execution of the input measurement"
#define AICONFIG_CALI_5              "Config:AI channel settings:Calibration correction:%1:5"
#define AICONFIG_CALI_5_INPUT        "Config:AI channel settings:Calibration correction:%1:5:Linearizer input"
#define AICONFIG_CALI_5_OUTPUT       "Config:AI channel settings:Calibration correction:%1:5:Linearizer output"
#define AICONFIG_CALI_5_EXEC         "Config:AI channel settings:Calibration correction:%1:5:Execution of the input measurement"
#define AICONFIG_CALI_6              "Config:AI channel settings:Calibration correction:%1:6"
#define AICONFIG_CALI_6_INPUT        "Config:AI channel settings:Calibration correction:%1:6:Linearizer input"
#define AICONFIG_CALI_6_OUTPUT       "Config:AI channel settings:Calibration correction:%1:6:Linearizer output"
#define AICONFIG_CALI_6_EXEC         "Config:AI channel settings:Calibration correction:%1:6:Execution of the input measurement"
#define AICONFIG_CALI_7              "Config:AI channel settings:Calibration correction:%1:7"
#define AICONFIG_CALI_7_INPUT        "Config:AI channel settings:Calibration correction:%1:7:Linearizer input"
#define AICONFIG_CALI_7_OUTPUT       "Config:AI channel settings:Calibration correction:%1:7:Linearizer output"
#define AICONFIG_CALI_7_EXEC         "Config:AI channel settings:Calibration correction:%1:7:Execution of the input measurement"
#define AICONFIG_CALI_8              "Config:AI channel settings:Calibration correction:%1:8"
#define AICONFIG_CALI_8_INPUT        "Config:AI channel settings:Calibration correction:%1:8:Linearizer input"
#define AICONFIG_CALI_8_OUTPUT       "Config:AI channel settings:Calibration correction:%1:8:Linearizer output"
#define AICONFIG_CALI_8_EXEC         "Config:AI channel settings:Calibration correction:%1:8:Execution of the input measurement"
#define AICONFIG_CALI_9              "Config:AI channel settings:Calibration correction:%1:9"
#define AICONFIG_CALI_9_INPUT        "Config:AI channel settings:Calibration correction:%1:9:Linearizer input"
#define AICONFIG_CALI_9_OUTPUT       "Config:AI channel settings:Calibration correction:%1:9:Linearizer output"
#define AICONFIG_CALI_9_EXEC         "Config:AI channel settings:Calibration correction:%1:9:Execution of the input measurement"
#define AICONFIG_CALI_10             "Config:AI channel settings:Calibration correction:%1:10"
#define AICONFIG_CALI_10_INPUT       "Config:AI channel settings:Calibration correction:%1:10:Linearizer input"
#define AICONFIG_CALI_10_OUTPUT      "Config:AI channel settings:Calibration correction:%1:10:Linearizer output"
#define AICONFIG_CALI_10_EXEC        "Config:AI channel settings:Calibration correction:%1:10:Execution of the input measurement"
#define AICONFIG_CALI_11             "Config:AI channel settings:Calibration correction:%1:11"
#define AICONFIG_CALI_11_INPUT       "Config:AI channel settings:Calibration correction:%1:11:Linearizer input"
#define AICONFIG_CALI_11_OUTPUT      "Config:AI channel settings:Calibration correction:%1:11:Linearizer output"
#define AICONFIG_CALI_11_EXEC        "Config:AI channel settings:Calibration correction:%1:11:Execution of the input measurement"
#define AICONFIG_CALI_12             "Config:AI channel settings:Calibration correction:%1:12"
#define AICONFIG_CALI_12_INPUT       "Config:AI channel settings:Calibration correction:%1:12:Linearizer input"
#define AICONFIG_CALI_12_OUTPUT      "Config:AI channel settings:Calibration correction:%1:12:Linearizer output"
#define AICONFIG_CALI_12_EXEC        "Config:AI channel settings:Calibration correction:%1:12:Execution of the input measurement"

//AI 代理树路径
#define AI_DELEGATE               "Config:AI Delegate"
#define AI_DELEGATE_FIRSTCHAN     "Config:AI Delegate:First-CH"
#define AI_DELEGATE_LASTCHAN      "Config:AI Delegate:Last-CH"

#define AI_DELEGATE_RANGE_RANGE_TYPE    "Config:AI Delegate:Range:Range:Type"
#define AI_DELEGATE_RANGE_RANGE_RANGE   "Config:AI Delegate:Range:Range:Range"
#define AI_DELEGATE_RANGE_RANGE_MAX     "Config:AI Delegate:Range:Range:Range Upper"
#define AI_DELEGATE_RANGE_RANGE_MIN     "Config:AI Delegate:Range:Range:Range Lower"
#define AI_DELEGATE_RANGE_RANGE_UPPER   "Config:AI Delegate:Range:Range:Span Upper"
#define AI_DELEGATE_RANGE_RANGE_LOWER   "Config:AI Delegate:Range:Range:Span Lower"
#define AI_DELEGATE_RANGE_RANGE_CALCU   "Config:AI Delegate:Range:Range:Calculation"
#define AI_DELEGATE_RANGE_RANGE_REF     "Config:AI Delegate:Range:Range:Reference channel"
#define AI_DELEGATE_RANGE_SCALE_DECPOS  "Config:AI Delegate:Range:Scale:Decimal place"
#define AI_DELEGATE_RANGE_SCALE_LOWER   "Config:AI Delegate:Range:Scale:Scale Lower"
#define AI_DELEGATE_RANGE_SCALE_UPPER   "Config:AI Delegate:Range:Scale:Scale Upper"
#define AI_DELEGATE_RANGE_SCALE_UNIT    "Config:AI Delegate:Range:Scale:Unit"
#define AI_DELEGATE_RANGE_LOWCUT_ONOFF  "Config:AI Delegate:Range:Low-cut:On/Off"
#define AI_DELEGATE_RANGE_LOWCUT_VALUE  "Config:AI Delegate:Range:Low-cut:Low-cut value"
#define AI_DELEGATE_RANGE_LOWCUT_OUTPUT "Config:AI Delegate:Range:Low-cut:Low-cut output"
#define AI_DELEGATE_RANGE_MOVAVE_ONOFF  "Config:AI Delegate:Range:Moving average:On/Off"
#define AI_DELEGATE_RANGE_MOVAVE_COUNT  "Config:AI Delegate:Range:Moving average:Count"
#define AI_DELEGATE_RANGE_RJC_MODE      "Config:AI Delegate:Range:RJC:Mode"
#define AI_DELEGATE_RANGE_RJC_TEMPER    "Config:AI Delegate:Range:RJC:Temperature"
#define AI_DELEGATE_RANGE_BURN_MODE     "Config:AI Delegate:Range:Burnout set:Mode"
#define AI_DELEGATE_RANGE_BIAS          "Config:AI Delegate:Range:Bias"
#define AI_DELEGATE_RANGE_BIAS_VALUE    "Config:AI Delegate:Range:Bias:Value"

#define AI_DELEGATE_ALARM    "Config:AI Delegate:Alarm"
#define AI_DELEGATE_ALARM_LVL1_ONOFF    "Config:AI Delegate:Alarm:Level 1:On/Off"
#define AI_DELEGATE_ALARM_LVL1_TYPE     "Config:AI Delegate:Alarm:Level 1:Type"
#define AI_DELEGATE_ALARM_LVL1_VALUE    "Config:AI Delegate:Alarm:Level 1:Value"
#define AI_DELEGATE_ALARM_LVL1_HYST     "Config:AI Delegate:Alarm:Level 1:Hysteresis"
#define AI_DELEGATE_ALARM_LVL1_LOGG     "Config:AI Delegate:Alarm:Level 1:Logging"
#define AI_DELEGATE_ALARM_LVL1_OUTTYPE  "Config:AI Delegate:Alarm:Level 1:Output type"
#define AI_DELEGATE_ALARM_LVL1_OUTNO    "Config:AI Delegate:Alarm:Level 1:Output No."
#define AI_DELEGATE_ALARM_LVL2_ONOFF    "Config:AI Delegate:Alarm:Level 2:On/Off"
#define AI_DELEGATE_ALARM_LVL2_TYPE     "Config:AI Delegate:Alarm:Level 2:Type"
#define AI_DELEGATE_ALARM_LVL2_VALUE    "Config:AI Delegate:Alarm:Level 2:Value"
#define AI_DELEGATE_ALARM_LVL2_HYST     "Config:AI Delegate:Alarm:Level 2:Hysteresis"
#define AI_DELEGATE_ALARM_LVL2_LOGG     "Config:AI Delegate:Alarm:Level 2:Logging"
#define AI_DELEGATE_ALARM_LVL2_OUTTYPE  "Config:AI Delegate:Alarm:Level 2:Output type"
#define AI_DELEGATE_ALARM_LVL2_OUTNO    "Config:AI Delegate:Alarm:Level 2:Output No."
#define AI_DELEGATE_ALARM_LVL3_ONOFF    "Config:AI Delegate:Alarm:Level 3:On/Off"
#define AI_DELEGATE_ALARM_LVL3_TYPE     "Config:AI Delegate:Alarm:Level 3:Type"
#define AI_DELEGATE_ALARM_LVL3_VALUE    "Config:AI Delegate:Alarm:Level 3:Value"
#define AI_DELEGATE_ALARM_LVL3_HYST     "Config:AI Delegate:Alarm:Level 3:Hysteresis"
#define AI_DELEGATE_ALARM_LVL3_LOGG     "Config:AI Delegate:Alarm:Level 3:Logging"
#define AI_DELEGATE_ALARM_LVL3_OUTTYPE  "Config:AI Delegate:Alarm:Level 3:Output type"
#define AI_DELEGATE_ALARM_LVL3_OUTNO    "Config:AI Delegate:Alarm:Level 3:Output No."
#define AI_DELEGATE_ALARM_LVL4_ONOFF    "Config:AI Delegate:Alarm:Level 4:On/Off"
#define AI_DELEGATE_ALARM_LVL4_TYPE     "Config:AI Delegate:Alarm:Level 4:Type"
#define AI_DELEGATE_ALARM_LVL4_VALUE    "Config:AI Delegate:Alarm:Level 4:Value"
#define AI_DELEGATE_ALARM_LVL4_HYST     "Config:AI Delegate:Alarm:Level 4:Hysteresis"
#define AI_DELEGATE_ALARM_LVL4_LOGG     "Config:AI Delegate:Alarm:Level 4:Logging"
#define AI_DELEGATE_ALARM_LVL4_OUTTYPE  "Config:AI Delegate:Alarm:Level 4:Output type"
#define AI_DELEGATE_ALARM_LVL4_OUTNO    "Config:AI Delegate:Alarm:Level 4:Output No."
#define AI_DELEGATE_ALARM_DELAY_HOUR    "Config:AI Delegate:Alarm:Alarm delay:Hour"
#define AI_DELEGATE_ALARM_DELAY_MIN     "Config:AI Delegate:Alarm:Alarm delay:Minute"
#define AI_DELEGATE_ALARM_DELAY_SEC     "Config:AI Delegate:Alarm:Alarm delay:Second"

#define AI_DELEGATE_DISP_TAG_CHAR       "Config:AI Delegate:Display settings:Tag:Characters"
#define AI_DELEGATE_DISP_TAG_NO         "Config:AI Delegate:Display settings:Tag:No."
#define AI_DELEGATE_DISP_COLOR_COLOR    "Config:AI Delegate:Display settings:Color:Color"
#define AI_DELEGATE_DISP_ZONE_UPPER     "Config:AI Delegate:Display settings:Zone:Upper"
#define AI_DELEGATE_DISP_ZONE_LOWER     "Config:AI Delegate:Display settings:Zone:Lower"
#define AI_DELEGATE_DISP_SCALE_POS      "Config:AI Delegate:Display settings:Scale:Position"
#define AI_DELEGATE_DISP_SCALE_DIV      "Config:AI Delegate:Display settings:Scale:Division"
#define AI_DELEGATE_DISP_BAR_BASEPOS    "Config:AI Delegate:Display settings:Bar graph:Base position"
#define AI_DELEGATE_DISP_BAR_DIVISION   "Config:AI Delegate:Display settings:Bar graph:Division"
#define AI_DELEGATE_DISP_PART           "Config:AI Delegate:Display settings:Partial"
#define AI_DELEGATE_DISP_PART_ONOFF     "Config:AI Delegate:Display settings:Partial:On/Off"
#define AI_DELEGATE_DISP_PART_EXPAND    "Config:AI Delegate:Display settings:Partial:Expand"
#define AI_DELEGATE_DISP_PART_BOUND     "Config:AI Delegate:Display settings:Partial:Boundary"
#define AI_DELEGATE_DISP_CSBAND_AREA    "Config:AI Delegate:Display settings:Color scale band:Band area"
#define AI_DELEGATE_DISP_CSBAND_UPPER   "Config:AI Delegate:Display settings:Color scale band:Display position Upper"
#define AI_DELEGATE_DISP_CSBAND_LOWER   "Config:AI Delegate:Display settings:Color scale band:Display position Lower"
#define AI_DELEGATE_DISP_CSBAND_COLOR   "Config:AI Delegate:Display settings:Color scale band:Color"
#define AI_DELEGATE_DISP_ALARM_INDIC    "Config:AI Delegate:Display settings:Alarm point mark:Indicate on Scale"
#define AI_DELEGATE_DISP_ALARM_MARKTYPE "Config:AI Delegate:Display settings:Alarm point mark:Mark type"
#define AI_DELEGATE_DISP_ALARM_COLOR1   "Config:AI Delegate:Display settings:Alarm point mark:Alarm 1 color"
#define AI_DELEGATE_DISP_ALARM_COLOR2   "Config:AI Delegate:Display settings:Alarm point mark:Alarm 2 color"
#define AI_DELEGATE_DISP_ALARM_COLOR3   "Config:AI Delegate:Display settings:Alarm point mark:Alarm 3 color"
#define AI_DELEGATE_DISP_ALARM_COLOR4   "Config:AI Delegate:Display settings:Alarm point mark:Alarm 4 color"
#define AI_DELEGATE_DISP_DISPCHAR_0     "Config:AI Delegate:Display settings:Display characters of each value:0"
#define AI_DELEGATE_DISP_DISPCHAR_1     "Config:AI Delegate:Display settings:Display characters of each value:1"

#define AI_DELEGATE_CALI_MODE_MODE      "Config:AI Delegate:Calibration correction:Mode:Mode"
#define AI_DELEGATE_CALI_MODE_NUM       "Config:AI Delegate:Calibration correction:Mode:Number of set points"
#define AI_DELEGATE_CALI_1_INPUT        "Config:AI Delegate:Calibration correction:1:Linearizer input"
#define AI_DELEGATE_CALI_1_OUTPUT       "Config:AI Delegate:Calibration correction:1:Linearizer output"
#define AI_DELEGATE_CALI_1_EXEC         "Config:AI Delegate:Calibration correction:1:Execution of the input measurement"
#define AI_DELEGATE_CALI_2_INPUT        "Config:AI Delegate:Calibration correction:2:Linearizer input"
#define AI_DELEGATE_CALI_2_OUTPUT       "Config:AI Delegate:Calibration correction:2:Linearizer output"
#define AI_DELEGATE_CALI_2_EXEC         "Config:AI Delegate:Calibration correction:2:Execution of the input measurement"
#define AI_DELEGATE_CALI_3_INPUT        "Config:AI Delegate:Calibration correction:3:Linearizer input"
#define AI_DELEGATE_CALI_3_OUTPUT       "Config:AI Delegate:Calibration correction:3:Linearizer output"
#define AI_DELEGATE_CALI_3_EXEC         "Config:AI Delegate:Calibration correction:3:Execution of the input measurement"
#define AI_DELEGATE_CALI_4_INPUT        "Config:AI Delegate:Calibration correction:4:Linearizer input"
#define AI_DELEGATE_CALI_4_OUTPUT       "Config:AI Delegate:Calibration correction:4:Linearizer output"
#define AI_DELEGATE_CALI_4_EXEC         "Config:AI Delegate:Calibration correction:4:Execution of the input measurement"
#define AI_DELEGATE_CALI_5_INPUT        "Config:AI Delegate:Calibration correction:5:Linearizer input"
#define AI_DELEGATE_CALI_5_OUTPUT       "Config:AI Delegate:Calibration correction:5:Linearizer output"
#define AI_DELEGATE_CALI_5_EXEC         "Config:AI Delegate:Calibration correction:5:Execution of the input measurement"
#define AI_DELEGATE_CALI_6_INPUT        "Config:AI Delegate:Calibration correction:6:Linearizer input"
#define AI_DELEGATE_CALI_6_OUTPUT       "Config:AI Delegate:Calibration correction:6:Linearizer output"
#define AI_DELEGATE_CALI_6_EXEC         "Config:AI Delegate:Calibration correction:6:Execution of the input measurement"
#define AI_DELEGATE_CALI_7_INPUT        "Config:AI Delegate:Calibration correction:7:Linearizer input"
#define AI_DELEGATE_CALI_7_OUTPUT       "Config:AI Delegate:Calibration correction:7:Linearizer output"
#define AI_DELEGATE_CALI_7_EXEC         "Config:AI Delegate:Calibration correction:7:Execution of the input measurement"
#define AI_DELEGATE_CALI_8_INPUT        "Config:AI Delegate:Calibration correction:8:Linearizer input"
#define AI_DELEGATE_CALI_8_OUTPUT       "Config:AI Delegate:Calibration correction:8:Linearizer output"
#define AI_DELEGATE_CALI_8_EXEC         "Config:AI Delegate:Calibration correction:8:Execution of the input measurement"
#define AI_DELEGATE_CALI_9_INPUT        "Config:AI Delegate:Calibration correction:9:Linearizer input"
#define AI_DELEGATE_CALI_9_OUTPUT       "Config:AI Delegate:Calibration correction:9:Linearizer output"
#define AI_DELEGATE_CALI_9_EXEC         "Config:AI Delegate:Calibration correction:9:Execution of the input measurement"
#define AI_DELEGATE_CALI_10_INPUT       "Config:AI Delegate:Calibration correction:10:Linearizer input"
#define AI_DELEGATE_CALI_10_OUTPUT      "Config:AI Delegate:Calibration correction:10:Linearizer output"
#define AI_DELEGATE_CALI_10_EXEC        "Config:AI Delegate:Calibration correction:10:Execution of the input measurement"
#define AI_DELEGATE_CALI_11_INPUT       "Config:AI Delegate:Calibration correction:11:Linearizer input"
#define AI_DELEGATE_CALI_11_OUTPUT      "Config:AI Delegate:Calibration correction:11:Linearizer output"
#define AI_DELEGATE_CALI_11_EXEC        "Config:AI Delegate:Calibration correction:11:Execution of the input measurement"
#define AI_DELEGATE_CALI_12_INPUT       "Config:AI Delegate:Calibration correction:12:Linearizer input"
#define AI_DELEGATE_CALI_12_OUTPUT      "Config:AI Delegate:Calibration correction:12:Linearizer output"
#define AI_DELEGATE_CALI_12_EXEC        "Config:AI Delegate:Calibration correction:12:Execution of the input measurement"

#define AI_DELEGATE_CALI_X_INPUT        "Config:AI Delegate:Calibration correction:%1:Linearizer input"

//AI 值变更 XML文件路径
#define XML_AI_RANGE_RANGE_TYPE     ":/configtree/Image/configTree/AI_ValueChange/AIRange_RangeType.xml"
#define XML_AI_RANGE_RANGE_VOLT     ":/configtree/Image/configTree/AI_ValueChange/AIRange_RangeRange-Volt.xml"
#define XML_AI_RANGE_RANGE_GS       ":/configtree/Image/configTree/AI_ValueChange/AIRange_RangeRange-GS.xml"
#define XML_AI_RANGE_RANGE_TC       ":/configtree/Image/configTree/AI_ValueChange/AIRange_RangeRange-TC.xml"
#define XML_AI_RANGE_RANGE_RTD      ":/configtree/Image/configTree/AI_ValueChange/AIRange_RangeRange-RTD.xml"
#define XML_AI_RANGE_RANGE_DI       ":/configtree/Image/configTree/AI_ValueChange/AIRange_RangeRange-DI.xml"
//#define XML_AI_RANGE_RANGE_UPPER    ":/configtree/Image/configTree/AI_ValueChange/AIRange_RangeUpper.xml"
//#define XML_AI_RANGE_RANGE_LOWER    ":/configtree/Image/configTree/AI_ValueChange/AIRange_RangeLower.xml"
#define XML_AI_RANGE_RANGE_CALCU    ":/configtree/Image/configTree/AI_ValueChange/AIRange_RangeCalculation.xml"
//#define XML_AI_RANGE_RANGE_REF      ":/configtree/Image/configTree/AI_ValueChange/AIRange_RangeRef.xml"
#define XML_AI_RANGE_SCALE_POS      ":/configtree/Image/configTree/AI_ValueChange/AIRange_ScaleDecPlace.xml"
//#define XML_AI_RANGE_SCALE_LOWER    ":/configtree/Image/configTree/AI_ValueChange/AIRange_ScaleLower.xml"
//#define XML_AI_RANGE_SCALE_UPPER    ":/configtree/Image/configTree/AI_ValueChange/AIRange_ScaleUpper.xml"
//#define XML_AI_RANGE_SCALE_UNIT     ":/configtree/Image/configTree/AI_ValueChange/AIRange_ScaleUnit.xml"
#define XML_AI_RANGE_LOWCUT_ONOFF   ":/configtree/Image/configTree/AI_ValueChange/AIRange_LowCutOnOff.xml"
//#define XML_AI_RANGE_LOWCUT_VALUE   ":/configtree/Image/configTree/AI_ValueChange/AIRange_LowCutValue.xml"
#define XML_AI_RANGE_LOWCUT_OUTPUT  ":/configtree/Image/configTree/AI_ValueChange/AIRange_LowCutOutPut.xml"
#define XML_AI_RANGE_MOVAVE_ONOFF   ":/configtree/Image/configTree/AI_ValueChange/AIRange_MoveAveOnOff.xml"
//#define XML_AI_RANGE_MOVAVE_COUNT   ":/configtree/Image/configTree/AI_ValueChange/AIRange_MoveAveCnt.xml"
#define XML_AI_RANGE_RJC_MODE       ":/configtree/Image/configTree/AI_ValueChange/AIRange_RJCMode.xml"
//#define XML_AI_RANGE_RJC_TEMPER     ":/configtree/Image/configTree/AI_ValueChange/AIRange_RJCTemper.xml"
#define XML_AI_RANGE_BURN_MODE      ":/configtree/Image/configTree/AI_ValueChange/AIRange_BurnoutMode.xml"
//#define XML_AI_RANGE_BIAS_VALUE     ":/configtree/Image/configTree/AI_ValueChange/AIRange_BiasValue.xml"

#define XML_AI_ALARM_LVL1_ONOFF     ":/configtree/Image/configTree/AI_ValueChange/AIAlarm_Lvl1Onoff.xml"
//#define XML_AI_ALARM_LVL1_ONOFF_C   ":/configtree/Image/configTree/AI_ValueChange/AIAlarm_Lvl1Onoff_c.xml"
#define XML_AI_ALARM_LVL1_TYPE      ":/configtree/Image/configTree/AI_ValueChange/AIAlarm_Lvl1Type.xml"
//#define XML_AI_ALARM_LVL1_VALUE     ":/configtree/Image/configTree/AI_ValueChange/AIAlarm_Lvl1Value.xml"
//#define XML_AI_ALARM_LVL1_HYST      ":/configtree/Image/configTree/AI_ValueChange/AIAlarm_Lvl1Hysteresis.xml"
#define XML_AI_ALARM_LVL1_LOGG      ":/configtree/Image/configTree/AI_ValueChange/AIAlarm_Lvl1Logging.xml"
#define XML_AI_ALARM_LVL1_OUTTYPE   ":/configtree/Image/configTree/AI_ValueChange/AIAlarm_Lvl1OutType.xml"
//#define XML_AI_ALARM_LVL1_OUTNO     ":/configtree/Image/configTree/AI_ValueChange/AIAlarm_Lvl1OutNo.xml"
#define XML_AI_ALARM_LVL2_ONOFF     ":/configtree/Image/configTree/AI_ValueChange/AIAlarm_Lvl2Onoff.xml"
//#define XML_AI_ALARM_LVL2_ONOFF_C   ":/configtree/Image/configTree/AI_ValueChange/AIAlarm_Lvl2Onoff_c.xml"
#define XML_AI_ALARM_LVL2_TYPE      ":/configtree/Image/configTree/AI_ValueChange/AIAlarm_Lvl2Type.xml"
//#define XML_AI_ALARM_LVL2_VALUE     ":/configtree/Image/configTree/AI_ValueChange/AIAlarm_Lvl2Value.xml"
//#define XML_AI_ALARM_LVL2_HYST      ":/configtree/Image/configTree/AI_ValueChange/AIAlarm_Lvl2Hysteresis.xml"
#define XML_AI_ALARM_LVL2_LOGG      ":/configtree/Image/configTree/AI_ValueChange/AIAlarm_Lvl2Logging.xml"
#define XML_AI_ALARM_LVL2_OUTTYPE   ":/configtree/Image/configTree/AI_ValueChange/AIAlarm_Lvl2OutType.xml"
//#define XML_AI_ALARM_LVL2_OUTNO     ":/configtree/Image/configTree/AI_ValueChange/AIAlarm_Lvl2OutNo.xml"
#define XML_AI_ALARM_LVL3_ONOFF     ":/configtree/Image/configTree/AI_ValueChange/AIAlarm_Lvl3Onoff.xml"
//#define XML_AI_ALARM_LVL3_ONOFF_C   ":/configtree/Image/configTree/AI_ValueChange/AIAlarm_Lvl3Onoff_c.xml"
#define XML_AI_ALARM_LVL3_TYPE      ":/configtree/Image/configTree/AI_ValueChange/AIAlarm_Lvl3Type.xml"
//#define XML_AI_ALARM_LVL3_VALUE     ":/configtree/Image/configTree/AI_ValueChange/AIAlarm_Lvl3Value.xml"
//#define XML_AI_ALARM_LVL3_HYST      ":/configtree/Image/configTree/AI_ValueChange/AIAlarm_Lvl3Hysteresis.xml"
#define XML_AI_ALARM_LVL3_LOGG      ":/configtree/Image/configTree/AI_ValueChange/AIAlarm_Lvl3Logging.xml"
#define XML_AI_ALARM_LVL3_OUTTYPE   ":/configtree/Image/configTree/AI_ValueChange/AIAlarm_Lvl3OutType.xml"
//#define XML_AI_ALARM_LVL3_OUTNO     ":/configtree/Image/configTree/AI_ValueChange/AIAlarm_Lvl3OutNo.xml"
#define XML_AI_ALARM_LVL4_ONOFF     ":/configtree/Image/configTree/AI_ValueChange/AIAlarm_Lvl4Onoff.xml"
//#define XML_AI_ALARM_LVL4_ONOFF_C   ":/configtree/Image/configTree/AI_ValueChange/AIAlarm_Lvl4Onoff_c.xml"
#define XML_AI_ALARM_LVL4_TYPE      ":/configtree/Image/configTree/AI_ValueChange/AIAlarm_Lvl4Type.xml"
//#define XML_AI_ALARM_LVL4_VALUE     ":/configtree/Image/configTree/AI_ValueChange/AIAlarm_Lvl4Value.xml"
//#define XML_AI_ALARM_LVL4_HYST      ":/configtree/Image/configTree/AI_ValueChange/AIAlarm_Lvl4Hysteresis.xml"
#define XML_AI_ALARM_LVL4_LOGG      ":/configtree/Image/configTree/AI_ValueChange/AIAlarm_Lvl4Logging.xml"
#define XML_AI_ALARM_LVL4_OUTTYPE   ":/configtree/Image/configTree/AI_ValueChange/AIAlarm_Lvl4OutType.xml"
//#define XML_AI_ALARM_LVL4_OUTNO     ":/configtree/Image/configTree/AI_ValueChange/AIAlarm_Lvl4OutNo.xml"
//#define XML_AI_ALARM_DELAY_HOUR     ":/configtree/Image/configTree/AI_ValueChange/AIAlarm_DelayHour.xml"
//#define XML_AI_ALARM_DELAY_MIN      ":/configtree/Image/configTree/AI_ValueChange/AIAlarm_DelayMin.xml"
//#define XML_AI_ALARM_DELAY_SEC      ":/configtree/Image/configTree/AI_ValueChange/AIAlarm_DelaySec.xml"

//#define XML_AI_DISP_TAG_CHAR       ":/configtree/Image/configTree/AI_ValueChange/AIDisp_TagChar.xml"
//#define XML_AI_DISP_TAG_NO         ":/configtree/Image/configTree/AI_ValueChange/AIDisp_TagNo.xml"
//#define XML_AI_DISP_COLOR_COLOR    ":/configtree/Image/configTree/AI_ValueChange/AIDisp_ColorColor.xml"
//#define XML_AI_DISP_ZONE_UPPER     ":/configtree/Image/configTree/AI_ValueChange/AIDisp_ZoneUpper.xml"
//#define XML_AI_DISP_ZONE_LOWER     ":/configtree/Image/configTree/AI_ValueChange/AIDisp_ZoneLower.xml"
//#define XML_AI_DISP_SCALE_POS      ":/configtree/Image/configTree/AI_ValueChange/AIDisp_ScalePos.xml"
//#define XML_AI_DISP_SCALE_DIV      ":/configtree/Image/configTree/AI_ValueChange/AIDisp_ScaleDiv.xml"
//#define XML_AI_DISP_BAR_BASEPOS    ":/configtree/Image/configTree/AI_ValueChange/AIDisp_BarBasePos.xml"
//#define XML_AI_DISP_BAR_DIVISION   ":/configtree/Image/configTree/AI_ValueChange/AIDisp_BarDivision.xml"
#define XML_AI_DISP_PART_ONOFF     ":/configtree/Image/configTree/AI_ValueChange/AIDisp_PartOnoff.xml"
//#define XML_AI_DISP_PART_EXPAND    ":/configtree/Image/configTree/AI_ValueChange/AIDisp_PartExpand.xml"
//#define XML_AI_DISP_PART_BOUND     ":/configtree/Image/configTree/AI_ValueChange/AIDisp_PartBound.xml"
#define XML_AI_DISP_CSBAND_AREA    ":/configtree/Image/configTree/AI_ValueChange/AIDisp_CsbandArea.xml"
//#define XML_AI_DISP_CSBAND_UPPER   ":/configtree/Image/configTree/AI_ValueChange/AIDisp_CsbandUpper.xml"
//#define XML_AI_DISP_CSBAND_LOWER   ":/configtree/Image/configTree/AI_ValueChange/AIDisp_CsbandLower.xml"
//#define XML_AI_DISP_CSBAND_COLOR   ":/configtree/Image/configTree/AI_ValueChange/AIDisp_CsbandColor.xml"
//#define XML_AI_DISP_ALARM_INDIC    ":/configtree/Image/configTree/AI_ValueChange/AIDisp_AlarmIndic.xml"
#define XML_AI_DISP_ALARM_MARKTYPE ":/configtree/Image/configTree/AI_ValueChange/AIDisp_AlarmMarkType.xml"
//#define XML_AI_DISP_ALARM_COLOR1   ":/configtree/Image/configTree/AI_ValueChange/AIDisp_AlarmColor1.xml"
//#define XML_AI_DISP_ALARM_COLOR2   ":/configtree/Image/configTree/AI_ValueChange/AIDisp_AlarmColor2.xml"
//#define XML_AI_DISP_ALARM_COLOR3   ":/configtree/Image/configTree/AI_ValueChange/AIDisp_AlarmColor3.xml"
//#define XML_AI_DISP_ALARM_COLOR4   ":/configtree/Image/configTree/AI_ValueChange/AIDisp_AlarmColor4.xml"
//#define XML_AI_DISP_DISPCHAR_0     ":/configtree/Image/configTree/AI_ValueChange/AIDisp_DispChar0.xml"
//#define XML_AI_DISP_DISPCHAR_1     ":/configtree/Image/configTree/AI_ValueChange/AIDisp_DispChar1.xml"

#define XML_AI_CALI_MODE_MODE      ":/configtree/Image/configTree/AI_ValueChange/AICali_ModeMode.xml"
#define XML_AI_CALI_MODE_NUM       ":/configtree/Image/configTree/AI_ValueChange/AICali_ModeNumber.xml"
//#define XML_AI_CALI_1_INPUT        ":/configtree/Image/configTree/AI_ValueChange/AICali_1_Input.xml"
//#define XML_AI_CALI_1_OUTPUT       ":/configtree/Image/configTree/AI_ValueChange/AICali_1_Output.xml"
//#define XML_AI_CALI_2_INPUT        ":/configtree/Image/configTree/AI_ValueChange/AICali_2_Input.xml"
//#define XML_AI_CALI_2_OUTPUT       ":/configtree/Image/configTree/AI_ValueChange/AICali_2_Output.xml"
//#define XML_AI_CALI_3_INPUT        ":/configtree/Image/configTree/AI_ValueChange/AICali_3_Input.xml"
//#define XML_AI_CALI_3_OUTPUT       ":/configtree/Image/configTree/AI_ValueChange/AICali_3_Output.xml"
//#define XML_AI_CALI_4_INPUT        ":/configtree/Image/configTree/AI_ValueChange/AICali_4_Input.xml"
//#define XML_AI_CALI_4_OUTPUT       ":/configtree/Image/configTree/AI_ValueChange/AICali_4_Output.xml"
//#define XML_AI_CALI_5_INPUT        ":/configtree/Image/configTree/AI_ValueChange/AICali_5_Input.xml"
//#define XML_AI_CALI_5_OUTPUT       ":/configtree/Image/configTree/AI_ValueChange/AICali_5_Output.xml"
//#define XML_AI_CALI_6_INPUT        ":/configtree/Image/configTree/AI_ValueChange/AICali_6_Input.xml"
//#define XML_AI_CALI_6_OUTPUT       ":/configtree/Image/configTree/AI_ValueChange/AICali_6_Output.xml"
//#define XML_AI_CALI_7_INPUT        ":/configtree/Image/configTree/AI_ValueChange/AICali_7_Input.xml"
//#define XML_AI_CALI_7_OUTPUT       ":/configtree/Image/configTree/AI_ValueChange/AICali_7_Output.xml"
//#define XML_AI_CALI_8_INPUT        ":/configtree/Image/configTree/AI_ValueChange/AICali_8_Input.xml"
//#define XML_AI_CALI_8_OUTPUT       ":/configtree/Image/configTree/AI_ValueChange/AICali_8_Output.xml"
//#define XML_AI_CALI_9_INPUT        ":/configtree/Image/configTree/AI_ValueChange/AICali_9_Input.xml"
//#define XML_AI_CALI_9_OUTPUT       ":/configtree/Image/configTree/AI_ValueChange/AICali_9_Output.xml"
//#define XML_AI_CALI_10_INPUT       ":/configtree/Image/configTree/AI_ValueChange/AICali_10_Input.xml"
//#define XML_AI_CALI_10_OUTPUT      ":/configtree/Image/configTree/AI_ValueChange/AICali_10_Output.xml"
//#define XML_AI_CALI_11_INPUT       ":/configtree/Image/configTree/AI_ValueChange/AICali_11_Input.xml"
//#define XML_AI_CALI_11_OUTPUT      ":/configtree/Image/configTree/AI_ValueChange/AICali_11_Output.xml"
//#define XML_AI_CALI_12_INPUT       ":/configtree/Image/configTree/AI_ValueChange/AICali_12_Input.xml"
//#define XML_AI_CALI_12_OUTPUT      ":/configtree/Image/configTree/AI_ValueChange/AICali_12_Output.xml"


//其他
#define AI_DELEGATE_ALARM_LVL_VALUE    "Config:AI Delegate:Alarm:Level %1:Value"
#define AI_DELEGATE_ALARM_LVL_HYST     "Config:AI Delegate:Alarm:Level %1:Hysteresis"

#define AI_DELEGATE_CALI_GROUP          "Config:AI Delegate:Calibration correction:%1"
#define AI_DELEGATE_CALI_INPUT          "Config:AI Delegate:Calibration correction:%1:Linearizer input"
#define AI_DELEGATE_CALI_OUTPUT         "Config:AI Delegate:Calibration correction:%1:Linearizer output"
//quint32->RGB
#define COLOR_RED(x)        ((x)>>16 & 0xFF)
#define COLOR_GREEN(x)      ((x)>>8 & 0xFF)
#define COLOR_BLUE(x)       ((x)>>0 & 0xFF)
//quint32->H:M:S
#define ALARM_DELAY_H(x)    ((x)>>16 & 0xFF)
#define ALARM_DELAY_M(x)    ((x)>>8 & 0xFF)
#define ALARM_DELAY_S(x)    ((x)>>0 & 0xFF)

#include "configobject.h"

class ConfigTreeObject_AI : public ConfigObject
{
    Q_OBJECT
public:
    static ConfigTreeObject_AI *instance(QObject *parent);

signals:

private:
    explicit ConfigTreeObject_AI(QObject *parent = 0);
    void updateStrShowOn();

    void updateStrShowRange();
    void updateStrShowAlarm();
    void updateStrShowDisplay();
    void updateStrShowCalib();

    void setAICaliCorrect_Value(quint32 correctCount);  //AI-校准值变更 值解析
    void resetPartialSettings();    //AI部分压缩放大功能是否开启
public slots:
    void slotCreateAIsettingTree(QObject *parent = 0);
    void slotAIBoardInoutCfgTree(quint8 unit, quint8 module, quint8 chNum, bool in);

    void slotAIDelegateChange();
public slots:
    //AI_channel change
    void slotAIFirst_chanSelectchanged();
    void slotAILast_chanSelectchanged();

    //AI_Range Range
    void slotAIRange_RangeTypeChanged();
    void slotAIRange_RangeRangeChanged();
    void slotAIRange_RangeUpperChanged();
    void slotAIRange_RangeLowerChanged();
    void slotAIRange_RangeCalculationChanged();
    void slotAIRange_RangeRefChanChanged();
    //AI_Range Scale
    void slotAIRange_ScaleDecimalChanged();
    void slotAIRange_ScaleLowerChanged();
    void slotAIRange_ScaleUpperChanged();
    void slotAIRange_ScaleUnitChanged();
    //AI_Range Low-cut
    void slotAIRange_LowcutOnoffChanged();
    void slotAIRange_LowcutValueChanged();
    void slotAIRange_LowcutOutputChanged();
    //AI_Range Moving average
    void slotAIRange_MovingOnoffChanged();
    void slotAIRange_MovingCountChanged();
    //AI_Range RJC
    void slotAIRange_RJCModeChanged();
    void slotAIRange_RJCTemperChanged();
    //AI_Range Burnout set
    void slotAIRange_BurnoutModeChanged();
    //AI_Range BIsa
    void slotAIRange_BiasValueChanged();

    //AI_Alarm Level 1
    void slotAIAlarm_Level1OnoffChanged();
    void slotAIAlarm_Level1TypeChanged();
    void slotAIAlarm_Level1ValueChanged();
    void slotAIAlarm_Level1HysterChanged();
    void slotAIAlarm_Level1LoggChanged();
    void slotAIAlarm_Level1OutTypeChanged();
    void slotAIAlarm_Level1OutNoChanged();
    //AI_Alarm Level 2
    void slotAIAlarm_Level2OnoffChanged();
    void slotAIAlarm_Level2TypeChanged();
    void slotAIAlarm_Level2ValueChanged();
    void slotAIAlarm_Level2HysterChanged();
    void slotAIAlarm_Level2LoggChanged();
    void slotAIAlarm_Level2OutTypeChanged();
    void slotAIAlarm_Level2OutNoChanged();
    //AI_Alarm Level 3
    void slotAIAlarm_Level3OnoffChanged();
    void slotAIAlarm_Level3TypeChanged();
    void slotAIAlarm_Level3ValueChanged();
    void slotAIAlarm_Level3HysterChanged();
    void slotAIAlarm_Level3LoggChanged();
    void slotAIAlarm_Level3OutTypeChanged();
    void slotAIAlarm_Level3OutNoChanged();
    //AI_Alarm Level 4
    void slotAIAlarm_Level4OnoffChanged();
    void slotAIAlarm_Level4TypeChanged();
    void slotAIAlarm_Level4ValueChanged();
    void slotAIAlarm_Level4HysterChanged();
    void slotAIAlarm_Level4LoggChanged();
    void slotAIAlarm_Level4OutTypeChanged();
    void slotAIAlarm_Level4OutNoChanged();
    //AI_Alarm Delay H:M:S
    void slotAIAlarm_DelayHourChanged();
    void slotAIAlarm_DelayMinChanged();
    void slotAIAlarm_DelaySecChanged();

    //AI_Disp Color
    void slotAIDisp_ColorColorChanged();
    //AI_Disp Tag
    void slotAIDisp_TagCharChanged();
    void slotAIDisp_TagNoChanged();
    //AI_Disp Zone
    void slotAIDisp_ZoneLowerChanged();
    void slotAIDisp_ZoneUpperChanged();
    //AI_Disp Scale
    void slotAIDisp_ScalePosChanged();
    void slotAIDisp_ScaleDivChanged();
    //AI_Disp Bar graph
    void slotAIDisp_BarBasePosChanged();
    void slotAIDisp_BarDivChanged();
    //AI_Disp Partial
    void slotAIDisp_PartOnoffChanged();
    void slotAIDisp_PartExpandChanged();
    void slotAIDisp_PartBoundChanged();
    //AI_Disp Color scale band
    void slotAIDisp_CsbandBandareaChanged();
    void slotAIDisp_CsbandDispLowChanged();
    void slotAIDisp_CsbandDispUpChanged();
    void slotAIDisp_CsbandColorChanged();
    //AI_Disp Alarm point mark
    void slotAIDisp_AlarmIndicateChanged();
    void slotAIDisp_AlarmMarkTypeChanged();
    void slotAIDisp_AlarmAlarm1Changed();
    void slotAIDisp_AlarmAlarm2Changed();
    void slotAIDisp_AlarmAlarm3Changed();
    void slotAIDisp_AlarmAlarm4Changed();
    //AI_Disp Display characters of each value
    void slotAIDisp_DisVal_0_Changed();
    void slotAIDisp_DisVal_1_Changed();

    //AI_Calibration Mode
    void slotAICali_ModeModeChanged();
    void slotAICali_ModeNumSetChanged();
    //AI_Calibration 1
    void slotAICali_OneLineInChanged();
    void slotAICali_OneLineOutChanged();

    //AI_Calibration 2
    void slotAICali_TwoLineInChanged();
    void slotAICali_TwoLineOutChanged();

    //AI_Calibration 3
    void slotAICali_ThreeLineInChanged();
    void slotAICali_ThreeLineOutChanged();

    //AI_Calibration 4
    void slotAICali_FourLineInChanged();
    void slotAICali_FourLineOutChanged();

    //AI_Calibration 5
    void slotAICali_FiveLineInChanged();
    void slotAICali_FiveLineOutChanged();

    //AI_Calibration 6
    void slotAICali_SixLineInChanged();
    void slotAICali_SixLineOutChanged();

    //AI_Calibration 7
    void slotAICali_SevenLineInChanged();
    void slotAICali_SevenLineOutChanged();

    //AI_Calibration 8
    void slotAICali_EightLineInChanged();
    void slotAICali_EightLineOutChanged();

    //AI_Calibration 9
    void slotAICali_NineLineInChanged();
    void slotAICali_NineLineOutChanged();

    //AI_Calibration 10
    void slotAICali_TenLineInChanged();
    void slotAICali_TenLineOutChanged();

    //AI_Calibration 11
    void slotAICali_ElevenLineInChanged();
    void slotAICali_ElevenLineOutChanged();

    //AI_Calibration 12
    void slotAICali_TwelveLineInChanged();
    void slotAICali_TwelveLineOutChanged();

};

#endif // CONFIGTREEOBJECT_AI_H
