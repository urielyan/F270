#ifndef CFGCONFIG_DISPLAY_H
#define CFGCONFIG_DISPLAY_H

#include <QObject>
#include "./globalDef.h"
#include "./GlobalData/configobject.h"

/**************************************** display 21300 - 21500 ***********************************/
#define Disp_Start_ID                                       21300
#define Disp_End_ID                                         21499

#define Disp_Trend_Start_ID                                 21300
#define Disp_Trend_End_ID                                   21329

#define Disp_Grp_Start_ID                                   21330
#define Disp_Grp_End_ID                                     21369

#define Disp_Baisc_Start_ID                                 21370
#define Disp_Baisc_End_ID                                   21396

#define Disp_Msg_Start_ID                                   21397
#define Disp_Msg_End_ID                                     21499

//21300  - 22000
#define ID_Disp                                             21300

#define ID_Disp_Trend                                       21301   //title

#define ID_Disp_Trend_Basic                                 21314   //group
#define ID_Disp_Trend_Interval                              21302   //趋势周期，最小值依赖于测量周期或事件记录周期
#define ID_Disp_Trend_SecondInterval                        21303   //第二趋势周期
#define ID_Disp_Trend_SecondSwitch                          21304   //第二趋势周期切换开关，0-off, 1-on

#define ID_Disp_Trend_Set                                   21305   //group
#define ID_Disp_Trend_Set_Direction                         21306   //趋势显示方向， 0-横， 1-纵
#define ID_Disp_Trend_Set_Clear                             21307   //采样开始时清除波形，0-off, 1-on
#define ID_Disp_Trend_Set_Line                              21308   //趋势波形线宽， 0-标准， 1-粗， 2-细， 默认0
#define ID_Disp_Trend_Set_Grid                              21309   //趋势栅格，0/4/5/6/7/8/9/10/11/12, 默认0(自动)

#define ID_Disp_Trend_Scale                                 21310   //标尺
#define ID_Disp_Trend_Scale_Mode                            21311   //标尺显示模式：0-标准，1-详细，默认：0
#define ID_Disp_Trend_Scale_Type                            21312    //标尺显示方式：0-标记，1-棒图，默认：0
#define ID_Disp_Trend_Scale_Number                          21313   //当前值标记通道号，0/3/4,   默认3位

#define ID_Disp_Trend_Partial                               21320   //部分压缩放大
#define ID_Disp_Trend_Partial_OnOff                         21321   //部分压缩放大开关,0-off, 1-on

#define ID_Disp_Trend_Msg                                   21322   //message
#define ID_Disp_Trend_Msg_WriteMode                         21323   //趋势信息写入方式，0-共同，1-个别
#define ID_Disp_Trend_Msg_PowerDown                         21324    //停电信息开关，0-off, 1-on
#define ID_Disp_Trend_Msg_IntervalChange                    21325   //趋势周期变更信息写入开关，0-off, 1-on


//group
#define ID_Disp_Grp                                         21330       //titles

#define ID_Disp_Grp_Number                                  21331

#define ID_Disp_Grp_Set                                     21332       //group
#define ID_Disp_Grp_Set_Switch                              21333       //组开关，0-off, 1-on
#define ID_Disp_Grp_Set_CHNumber                            21334      //组通道数，最多20个通道
#define ID_Disp_Grp_Set_Name                                21335       //组名，最多16个半角字符，中英文支持
#define ID_Disp_Grp_Set_CHItem                              21336       //组通道号，有效通道号由groupChannelNum决定

#define ID_Disp_Grp_Set_ScaleImage                          21337
#define ID_Disp_Grp_Set_ScaleImage_Switch                   21338       //组标尺图像开关，0-off, 1-on

#define ID_Disp_Grp_Set_Scale_1                             21340       //分割线
#define ID_Disp_Grp_Set_Scale1_Switch                       21341       //分割线1-4开关，0-off, 1-on, 默认false
#define ID_Disp_Grp_Set_Scale1_Pos                          21342       //分割线1-4位置，单位:%, 0-100,默认50
#define ID_Disp_Grp_Set_Scale1_Width                        21343       //分割线1-4线宽，0-标准， 1-粗， 2-细， 默认0
#define ID_Disp_Grp_Set_Scale1_Color                        21344       //分割线1-4颜色值，BYTE2-Red, BYTE1-Green, BYTE0-Blue

#define ID_Disp_Grp_Set_Scale_2                             21345       //分割线
#define ID_Disp_Grp_Set_Scale2_Switch                       21346
#define ID_Disp_Grp_Set_Scale2_Pos                          21347
#define ID_Disp_Grp_Set_Scale2_Width                        21348
#define ID_Disp_Grp_Set_Scale2_Color                        21349

#define ID_Disp_Grp_Set_Scale_3                             21351       //分割线
#define ID_Disp_Grp_Set_Scale3_Switch                       21352
#define ID_Disp_Grp_Set_Scale3_Pos                          21353
#define ID_Disp_Grp_Set_Scale3_Width                        21354
#define ID_Disp_Grp_Set_Scale3_Color                        21355

#define ID_Disp_Grp_Set_Scale_4                             21360       //分割线
#define ID_Disp_Grp_Set_Scale4_Switch                       21361
#define ID_Disp_Grp_Set_Scale4_Pos                          21362
#define ID_Disp_Grp_Set_Scale4_Width                        21363
#define ID_Disp_Grp_Set_Scale4_Color                        21364




//display badic
#define ID_Disp_Basic                                       21370   //title

#define ID_Disp_Basic_Bar                                   21371
#define ID_Disp_Basic_Bar_Direction                         21372       //棒图显示方向，0-横，1-纵，默认0

#define ID_Disp_Basic_LCD                                   21373
#define ID_Disp_Basic_LCD_Brightness                        21374        //LCD显示辉度，1-6,默认:3

#define ID_Disp_Basic_Backlight                             21382
#define ID_Disp_Basic_Backlight_Mode                        21383   //LCD屏保模式，0-off, 1-变暗，2-熄灭
#define ID_Disp_Basic_Backlight_Time                        21384   //LCD屏保等待时间，单位：min, 范围：1min/2min/5min/10min/30min/1h
#define ID_Disp_Basic_Backlight_Restore                     21385    //LCD屏保恢复模式，0-按键/触摸/报警， 1-按键/触摸，默认：0

#define ID_Disp_Basic_Monitor                               21389
#define ID_Disp_Basic_Background                            21390   //UI背景色，0-白色，1-黑色，默认：0
#define ID_Disp_Basic_Changetime                            21391    //组自动切换时间，单位：s,范围：5s/10s/20s/30s/60s(1min),默认10s
#define ID_Disp_Basic_Wintime                               21392   //自动默认画面跳转时间，单位:min,范围：0(off)/1/2/5/10/20/30/1h,默认：0

#define ID_Disp_Basic_Calendar                              21393
#define ID_Disp_Basic_Calendar_Week                         21394  //周起始日，0-星期日，1-星期一

#define ID_Disp_Basic_Change                                21395
#define ID_Disp_Basic_Change_Switch                         21396   //从监视器修改各值，false-off, true-on


//display message
#define ID_Disp_Msg                                         21397   //title

#define ID_Disp_Msg_GrpNum                                  21398   //normal

#define ID_Disp_Msg_GrpNum_Msg                              21399   //group

#define ID_Disp_Msg_GrpNum_1                                21400
#define ID_Disp_Msg_GrpNum_2                                21401
#define ID_Disp_Msg_GrpNum_3                                21402
#define ID_Disp_Msg_GrpNum_4                                21403
#define ID_Disp_Msg_GrpNum_5                                21404
#define ID_Disp_Msg_GrpNum_6                                21405
#define ID_Disp_Msg_GrpNum_7                                21406
#define ID_Disp_Msg_GrpNum_8                                21407
#define ID_Disp_Msg_GrpNum_9                                21408
#define ID_Disp_Msg_GrpNum_10                               21409

#define ID_Disp_Msg_GrpNum_11                               21410
#define ID_Disp_Msg_GrpNum_12                               21411
#define ID_Disp_Msg_GrpNum_13                               21412
#define ID_Disp_Msg_GrpNum_14                               21413
#define ID_Disp_Msg_GrpNum_15                               21414
#define ID_Disp_Msg_GrpNum_16                               21415
#define ID_Disp_Msg_GrpNum_17                               21416
#define ID_Disp_Msg_GrpNum_18                               21417
#define ID_Disp_Msg_GrpNum_19                               21418
#define ID_Disp_Msg_GrpNum_20                               21419

#define ID_Disp_Msg_GrpNum_21                               21420
#define ID_Disp_Msg_GrpNum_22                               21421
#define ID_Disp_Msg_GrpNum_23                               21422
#define ID_Disp_Msg_GrpNum_24                               21423
#define ID_Disp_Msg_GrpNum_25                               21424
#define ID_Disp_Msg_GrpNum_26                               21425
#define ID_Disp_Msg_GrpNum_27                               21426
#define ID_Disp_Msg_GrpNum_28                               21427
#define ID_Disp_Msg_GrpNum_29                               21428
#define ID_Disp_Msg_GrpNum_30                               21429

#define ID_Disp_Msg_GrpNum_31                               21430
#define ID_Disp_Msg_GrpNum_32                               21431
#define ID_Disp_Msg_GrpNum_33                               21432
#define ID_Disp_Msg_GrpNum_34                               21433
#define ID_Disp_Msg_GrpNum_35                               21434
#define ID_Disp_Msg_GrpNum_36                               21435
#define ID_Disp_Msg_GrpNum_37                               21436
#define ID_Disp_Msg_GrpNum_38                               21437
#define ID_Disp_Msg_GrpNum_39                               21438
#define ID_Disp_Msg_GrpNum_40                               21439


#define ID_Disp_Msg_GrpNum_41                               21440
#define ID_Disp_Msg_GrpNum_42                               21441
#define ID_Disp_Msg_GrpNum_43                               21442
#define ID_Disp_Msg_GrpNum_44                               21443
#define ID_Disp_Msg_GrpNum_45                               21444
#define ID_Disp_Msg_GrpNum_46                               21445
#define ID_Disp_Msg_GrpNum_47                               21446
#define ID_Disp_Msg_GrpNum_48                               21447
#define ID_Disp_Msg_GrpNum_49                               21448
#define ID_Disp_Msg_GrpNum_50                               21449


#define ID_Disp_Msg_GrpNum_51                               21450
#define ID_Disp_Msg_GrpNum_52                               21451
#define ID_Disp_Msg_GrpNum_53                               21452
#define ID_Disp_Msg_GrpNum_54                               21453
#define ID_Disp_Msg_GrpNum_55                               21454
#define ID_Disp_Msg_GrpNum_56                               21455
#define ID_Disp_Msg_GrpNum_57                               21456
#define ID_Disp_Msg_GrpNum_58                               21457
#define ID_Disp_Msg_GrpNum_59                               21458
#define ID_Disp_Msg_GrpNum_60                               21459

#define ID_Disp_Msg_GrpNum_61                               21460
#define ID_Disp_Msg_GrpNum_62                               21461
#define ID_Disp_Msg_GrpNum_63                               21462
#define ID_Disp_Msg_GrpNum_64                               21463
#define ID_Disp_Msg_GrpNum_65                               21464
#define ID_Disp_Msg_GrpNum_66                               21465
#define ID_Disp_Msg_GrpNum_67                               21466
#define ID_Disp_Msg_GrpNum_68                               21467
#define ID_Disp_Msg_GrpNum_69                               21468
#define ID_Disp_Msg_GrpNum_70                               21469

#define ID_Disp_Msg_GrpNum_71                               21470
#define ID_Disp_Msg_GrpNum_72                               21471
#define ID_Disp_Msg_GrpNum_73                               21472
#define ID_Disp_Msg_GrpNum_74                               21473
#define ID_Disp_Msg_GrpNum_75                               21474
#define ID_Disp_Msg_GrpNum_76                               21475
#define ID_Disp_Msg_GrpNum_77                               21476
#define ID_Disp_Msg_GrpNum_78                               21477
#define ID_Disp_Msg_GrpNum_79                               21478
#define ID_Disp_Msg_GrpNum_80                               21479

#define ID_Disp_Msg_GrpNum_81                               21480
#define ID_Disp_Msg_GrpNum_82                               21481
#define ID_Disp_Msg_GrpNum_83                               21482
#define ID_Disp_Msg_GrpNum_84                               21483
#define ID_Disp_Msg_GrpNum_85                               21484
#define ID_Disp_Msg_GrpNum_86                               21485
#define ID_Disp_Msg_GrpNum_87                               21486
#define ID_Disp_Msg_GrpNum_88                               21487
#define ID_Disp_Msg_GrpNum_89                               21488
#define ID_Disp_Msg_GrpNum_90                               21489

#define ID_Disp_Msg_GrpNum_91                               21490
#define ID_Disp_Msg_GrpNum_92                               21491
#define ID_Disp_Msg_GrpNum_93                               21492
#define ID_Disp_Msg_GrpNum_94                               21493
#define ID_Disp_Msg_GrpNum_95                               21494
#define ID_Disp_Msg_GrpNum_96                               21495
#define ID_Disp_Msg_GrpNum_97                               21496
#define ID_Disp_Msg_GrpNum_98                               21497
#define ID_Disp_Msg_GrpNum_99                               21498
#define ID_Disp_Msg_GrpNum_100                              21499




class CfgConfig_Display : public QObject
{
    Q_OBJECT
public:
    static CfgConfig_Display *instance();

    QVariant getDispData(quint32 ID, quint32 group);
    void setDispData(quint32 ID, quint32 group);

    QString getDispObjStr(quint32 ID, quint32 group = 0);
    QString getDispObjValStr(quint32 ID, quint32 group, ConfigObject * cfg);

    void setDeviceDispConfig(DEVICE_CONFIG & deviceConfig) { p_deviceConfig = &deviceConfig;}
private:
    explicit CfgConfig_Display(QObject *parent = 0);

    QVariant getDispBasicData(quint32 ID, quint32 group);
    QVariant getDispGrpData(quint32 ID, quint32 group);
    QVariant getDispMsgData(quint32 ID, quint32 group);
    QVariant getDispTrendData(quint32 ID, quint32 group);

    QVariant getDispBasicBackTimeData(quint8 &type);
    QVariant getDispBasicScrollTimeData(quint8 & type);
    QVariant getDispBasicWinTimeData(quint8 & type);
    QVariant getDispTrendSndInterData(quint32 type);

    QString getDispBasicObjStr(quint32 ID, quint32 group);
    QString getDispGrpObjStr(quint32 ID, quint32 group);
    QString getDispMsgObjStr(quint32 ID, quint32 group);
    QString getDispTrendObjStr(quint32 ID, quint32 group);

    QString getDispBasicObjValStr(quint32 ID, quint32 group, ConfigObject * cfg);
    QString getDispGrpObjValStr(quint32 ID, quint32 group, ConfigObject * cfg);
    QString getDispMsgObjValStr(quint32 ID, quint32 group, ConfigObject * cfg);
    QString getDispTrendObjValStr(quint32 ID, quint32 group, ConfigObject * cfg);

    QString getDispBasicBackTimeObjValStr(int type);
    QString getDispBasicScrollTimeObjValStr(int type);
    QString getDispBasicWinTimeObjValStr(int type);

    QString getDispTrendSndInterObjValStr(int type);


    void setDispBasicData(quint32 ID, quint32 group);
    void setDispGrpData(quint32 ID, quint32 group);
    void setDispMsgData(quint32 ID, quint32 group);
    void setDispTrendData(quint32 ID, quint32 group);



    QVariant getCharToString(uchar *str);
    void setStrToChar(uchar * uData, QString str, int len);
    QString getColorValueObjStr(quint32 x);

signals:

public slots:

private:
    DEVICE_CONFIG *     p_deviceConfig;
};

#endif // CFGCONFIG_DISPLAY_H
