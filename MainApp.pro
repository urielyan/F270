#-------------------------------------------------
#
# Project created by QtCreator 2015-06-03T14:18:06
#
#-------------------------------------------------

QT       += core gui
QT += sql printsupport network
CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MainApp
TEMPLATE = app

unix {
    INCLUDEPATH += /usr/local/include/
    LIBS += -L/usr/local/lib/ -lmodbus
}

SOURCES += main.cpp\
    WorkWin/wintrendgroup.cpp \
    WorkWin/wintrendgrouppanel.cpp\
    WorkWin/winmainframe.cpp \
    GlobalData/cfgchannel.cpp \
    GlobalData/cfgtrendgroup.cpp\
    Threads/threaddataprocess.cpp \
    Threads/threaddatacomm.cpp \
    Threads/threaddatasync.cpp \
    DataModel/modeldigitalgroupchannel.cpp \
    WorkWin/winroot.cpp \
    WorkWin/winroot_sysconfig.cpp \
    WorkWin/winroot_generalmenu.cpp \
    WorkWin/winroot_generalwin.cpp \
    WorkWin/windigitalgroup.cpp \
    WorkWin/windigitaldelegate.cpp \
    WorkWin/winbargroup.cpp \
    WorkWin/winoverview.cpp \
    WorkWin/winalarmsummary.cpp \
    WorkWin/winmessagesummary.cpp \
    WorkWin/winmemsummary.cpp \
    WorkWin/winreport.cpp \
    WorkWin/winmodbusclient.cpp \
    WorkWin/winrelayandsw.cpp \
    WorkWin/winmultipanel.cpp \
    WorkWin/winsaveload.cpp \
    WorkWin/winnetworkinfo.cpp \
    WorkWin/winsysteminfo.cpp \
    WorkWin/winlog.cpp \
    Common/slidingscreen.cpp \
    Threads/threaddatarecord.cpp \
    Common/sysconfigslidingscreen.cpp\
    TrendGroupComponent/trendrulerwidget.cpp\
    TrendGroupComponent/trendoverviewwidget.cpp \
    TrendGroupComponent/trenddetailwidget.cpp \
    TrendGroupComponent/trendrecordpaper.cpp\
    TrendGroupComponent/trenddigitalgroup.cpp\
    TrendGroupComponent/trenddigitaldelegate.cpp\
    Common/mathexpanalysis.cpp \
    Common/dataquerytc.cpp \
    Common/dataqueryrtd.cpp \
    Common/circlebuf.cpp\
    WorkWin/winbardelegate.cpp \
    DataModel/modelbargroupchannel.cpp \
    WorkWin/winoverviewchandelegate.cpp \
    WorkWin/winoverviewgroupdelegate.cpp \
    WorkWin/winoverchannelview.cpp \
    WorkWin/windigitalpanel.cpp \
    WorkWin/winbarpanel.cpp \
    DataModel/modeloverviewchanmodel.cpp \
    DataModel/modeloverviewgroupmodel.cpp \
    DataModel/modelmultipanelmodel.cpp \
    DataModel/modeltrenddigitalgroupchannel.cpp \
    WorkWin/winrelaydelegate.cpp \
    WorkWin/winsoftswitchdelegate.cpp \
    DataModel/modelrelaymodel.cpp \
    DataModel/modelsoftswitchmodel.cpp \
    Common/handwritepad.cpp \
    GlobalData/configobject.cpp \
    GlobalData/configtreeobject.cpp \
    Common/digitaloperation.cpp \
    Threads/threaddatasave.cpp \
    WorkWin/windialog.cpp \
    WorkWin/winmultipaneldialog.cpp \
    WorkWin/winchannelinfodialog.cpp \
    Common/eventaction.cpp \
    Common/timer.cpp \
    Common/winmenu.cpp \
    Common/winconfig.cpp \
    Common/channelbutton.cpp \
    WorkWin/winchannelalarmdialog.cpp \
    WorkWin/winchanneleditdialog.cpp \
    WorkWin/winsoftswitchdialog.cpp \
	Common/selectcolor.cpp \
    Common/winmenugroup.cpp \
    GlobalData/configtreeobject_AI.cpp \
    GlobalData/configtreeobject_Batch.cpp \
    GlobalData/configtreeobject_Comm.cpp \
    GlobalData/configtreeobject_DataSave.cpp \
    GlobalData/configtreeobject_DI.cpp \
    GlobalData/configtreeobject_Display.cpp \
    GlobalData/configtreeobject_DO.cpp \
    GlobalData/configtreeobject_EventAction.cpp \
    GlobalData/configtreeobject_Math.cpp \
    GlobalData/configtreeobject_Measure.cpp \
    GlobalData/configtreeobject_Network.cpp \
    GlobalData/configtreeobject_Record.cpp \
    GlobalData/configtreeobject_Report.cpp \
    GlobalData/configtreeobject_SericalClient.cpp \
    GlobalData/configtreeobject_Sys.cpp \
    GlobalData/configtreeobject_Timer.cpp \
    Common/winchannelselector.cpp \
    Common/winnetworkconfig.cpp \
    Common/application.cpp \
    Common/qactionevent.cpp \
    Common/winserialconfig.cpp \
    Common/ipset.cpp \
    Common/englishinput.cpp \
    Common/wininputconfig.cpp \
    WorkWin/winackalarmdialog.cpp \
    Common/timeinput.cpp \
    GlobalData/cfgconfig.cpp \
    DataModel/modelsummaryalarm.cpp \
    DataModel/modelsummarymem.cpp \
    DataModel/modellog.cpp \
    DataModel/modelmodbusclient.cpp \
	Common/setdatetime.cpp \
    WorkWin/winrecorddialog.cpp \
    WorkWin/winmediadialog.cpp \
    WorkWin/winmathdialog.cpp \
    Common/xmlconfigparser.cpp \
    Common/softkeypad.cpp \
    WorkWin/winmultipanelstyledialog.cpp \
    Common/calculation.cpp \
    GlobalData/cfgconfig_ai.cpp \
    GlobalData/cfgconfig_network.cpp \
    GlobalData/cfgconfig_di.cpp \
    GlobalData/cfgconfig_do.cpp \
    GlobalData/cfgconfig_math.cpp \
    GlobalData/cfgconfig_display.cpp \
    GlobalData/cfgconfig_measure.cpp \
    GlobalData/cfgconfig_record.cpp \
    GlobalData/cfgconfig_datasave.cpp \
    GlobalData/cfgconfig_batch.cpp \
    GlobalData/cfgconfig_report.cpp \
    GlobalData/cfgconfig_timer.cpp \
    GlobalData/cfgconfig_event.cpp \
    GlobalData/cfgconfig_serial.cpp \
    GlobalData/cfgconfig_comm.cpp \
    Common/devboard.cpp \
    Common/devgroup.cpp \
    Common/channel.cpp \
    Common/devchannelai.cpp \
    Common/devchanneldi.cpp \
    Common/devchanneldo.cpp \
    Common/devchannelmath.cpp \
    Common/devchannelcomm.cpp \
    WorkWin/wincolibrationleftpanel.cpp \
    WorkWin/wincolibrationmainpanel.cpp \
    WorkWin/winlogdialog.cpp \
    WorkWin/wincolibrationpanel.cpp \
	DataModel/modelreport.cpp \
    WorkWin/wincolibrationbutton.cpp \
    GlobalData/cfgconfig_system.cpp \
	Common/printtest.cpp \
    Common/winaiconfig.cpp \
    Common/windoconfig.cpp \
	WorkWin/winsnapshotdialog.cpp \
    WorkWin/winstandarddialog.cpp \
	WorkWin/wincontextdialog.cpp \
    Common/windiconfig.cpp \
    DataModel/modelsysteminformation.cpp \
    WorkWin/winsysteminfotableview.cpp \
    WorkWin/winsysteminfodelegate.cpp \
    WorkWin/winsysteminfoframe.cpp \
    WorkWin/winsysteminfodialog.cpp \
	WorkWin/winsysteminfotabwidget.cpp \
    WorkWin/winsysteminfoscroll.cpp \
    Common/wincommchannelconfig.cpp \
	WorkWin/winsaveloaddisplaydata.cpp \
    Common/windispconfig.cpp \
    Common/winmathconfig.cpp \
	Common/winsystemconfig.cpp \
    Common/wineventconfig.cpp \
    Common/winmeasureconfig.cpp \
    DataModel/modelsummarymessage.cpp \
    Common/logger.cpp \
    Common/custombutton.cpp \
	Threads/ftpclientprocess.cpp \
    WorkWin/wininfolistdialog.cpp \
	WorkWin/winsummarydialog.cpp \
	Threads/threadmodbusservermsg.cpp \
    WorkWin/winsaveloadmainframe.cpp \
    WorkWin/winsaveload_buttonFactory.cpp \
    Common/channelchoice.cpp \
    Common/channeldeledata.cpp \
    Common/channelview.cpp \
    Common/channemodel.cpp \
    GlobalData/cfgconfig_security.cpp \
    Common/authorization.cpp \
    GlobalData/configtreeobject_Security.cpp \
    WorkWin/winreportsummary.cpp \
    Common/eventcalendar.cpp \
    Common/eventcalendardelegate.cpp \
    Common/eventcalendarmodel.cpp \
    Common/eventcalendarview.cpp \
    Common/eventlistdelegate.cpp \
    Common/eventlistmodel.cpp \
    Common/eventlistview.cpp \
    Common/messagelistdelegate.cpp \
    Common/messagelistview.cpp \
    Common/messagelistmodel.cpp \
    Common/displaymessage.cpp


HEADERS  += \
    WorkWin/wintrendgroup.h \
    WorkWin/wintrendgrouppanel.h\
    GlobalData/cfgGlobalDef.h \
    GlobalData/cfgGlobalDef_AI.h \
    GlobalData/cfgGlobalDef_DI.h \
    GlobalData/cfgGlobalDef_DO.h \
    WorkWin/winmainframe.h \
    GlobalData/globalDef.h \
    GlobalData/cfgGlobalDef_Math.h \
    GlobalData/cfgGlobalDef_Comm.h \
    GlobalData/cfgGlobalDef_Display.h \
    GlobalData/cfgGlobalDef_Measure.h \
    GlobalData/cfgGlobalDef_Record.h \
    GlobalData/cfgGlobalDef_DataSave.h \
    GlobalData/cfgGlobalDef_Batch.h \
    GlobalData/cfgGlobalDef_Report.h \
    GlobalData/cfgGlobalDef_Timer.h \
    GlobalData/cfgGlobalDef_EventAction.h \
    GlobalData/cfgGlobalDef_Network_Basic.h \
    GlobalData/cfgGlobalDef_Network_FTPClient.h \
    GlobalData/cfgGlobalDef_Network_SMTPClient.h \
    GlobalData/cfgGlobalDef_Network_Email.h \
    GlobalData/cfgGlobalDef_Network_SNTPClient.h \
    GlobalData/cfgGlobalDef_Network_ModbusClient.h \
    GlobalData/cfgGlobalDef_Network_Server.h \
    GlobalData/cfgGlobalDef_SerialClient.h \
    GlobalData/cfgGlobalDef_Type.h \
    GlobalData/cfgGlobalDef_Trend.h \
    GlobalData/cfgGlobalDef_SysEnvironment.h \
    GlobalData/dataGlobalDef_ModbusReg.h \
    GlobalData/dataGlobalDef_SysStatus.h \
    GlobalData/cfgchannel.h \
    GlobalData/cfgtrendgroup.h\
    Threads/threaddataprocess.h \
    Threads/threaddatacomm.h \
    Threads/threaddatasync.h \
    GlobalData/dataGlobalDef_Record.h \
    DataModel/modeldigitalgroupchannel.h \
    WorkWin/winroot.h \
    WorkWin/winroot_sysconfig.h \
    WorkWin/winroot_generalmenu.h \
    WorkWin/winroot_generalwin.h \
    WorkWin/windigitalgroup.h \
    WorkWin/windigitaldelegate.h \
    WorkWin/winbargroup.h \
    WorkWin/winoverview.h \
    WorkWin/winalarmsummary.h \
    WorkWin/winmessagesummary.h \
    WorkWin/winmemsummary.h \
    WorkWin/winreport.h \
    WorkWin/winmodbusclient.h \
    WorkWin/winrelayandsw.h \
    WorkWin/winmultipanel.h \
    WorkWin/winsaveload.h \
    WorkWin/winnetworkinfo.h \
    WorkWin/winsysteminfo.h \
    WorkWin/winlog.h \
    Common/slidingscreen.h \
    Threads/threaddatarecord.h \
    Common/sysconfigslidingscreen.h\
    TrendGroupComponent/trendrulerwidget.h\
    TrendGroupComponent/trendoverviewwidget.h \
    TrendGroupComponent/trenddetailwidget.h \
    TrendGroupComponent/trendrecordpaper.h \
    TrendGroupComponent/trenddigitalgroup.h\
    TrendGroupComponent/trenddigitaldelegate.h\
    Common/mathexpanalysis.h \
    Common/dataquerytc.h \
    Common/dataqueryrtd.h \
    Common/circlebuf.h\
    GlobalData/sqlBlock_fileFormat.h \
    GlobalData/sqlBlock_GroupInfo.h \
    GlobalData/sqlBlock_RecordChanInfo.h \
    GlobalData/sqlBlock_fileInfo.h \
    GlobalData/sqlBlock_sysConfig.h \
    GlobalData/sqlBlock_batchInfo.h \
    WorkWin/winbardelegate.h \
    DataModel/modelbargroupchannel.h \
    WorkWin/winoverviewchandelegate.h \
    WorkWin/winoverviewgroupdelegate.h \
    GlobalData/sqlTable_Message.h \
    GlobalData/sqlTable_Alarm.h \
    GlobalData/sqlBlock_reportInfo.h \
    WorkWin/winoverchannelview.h \
    WorkWin/windigitalpanel.h \
    WorkWin/winbarpanel.h \
    DataModel/modeloverviewchanmodel.h \
    DataModel/modeloverviewgroupmodel.h \
    DataModel/modelmultipanelmodel.h \
    DataModel/modeltrenddigitalgroupchannel.h\
    WorkWin/winrelaydelegate.h \
    WorkWin/winsoftswitchdelegate.h \
    DataModel/modelrelaymodel.h \
    DataModel/modelsoftswitchmodel.h \
    Common/handwritepad.h \
    GlobalData/configobject.h \
    GlobalData/configtreeobject.h \
    Common/digitaloperation.h \
    Threads/threaddatasave.h \
    WorkWin/windialog.h \
    WorkWin/winmultipaneldialog.h \
    WorkWin/winchannelinfodialog.h \
    Common/eventaction.h \
    Common/timer.h \
    Common/winmenu.h \
    Common/winconfig.h \
    Common/channelbutton.h \
    WorkWin/winchannelalarmdialog.h \
    WorkWin/winchanneleditdialog.h \
    WorkWin/winsoftswitchdialog.h \
	Common/selectcolor.h \
    Common/winmenugroup.h \
    GlobalData/configtreeobject_AI.h \
    GlobalData/configtreeobject_Batch.h \
    GlobalData/configtreeobject_Comm.h \
    GlobalData/configtreeobject_DataSave.h \
    GlobalData/configtreeobject_DI.h \
    GlobalData/configtreeobject_Display.h \
    GlobalData/configtreeobject_DO.h \
    GlobalData/configtreeobject_EventAction.h \
    GlobalData/configtreeobject_Math.h \
    GlobalData/configtreeobject_Measure.h \
    GlobalData/configtreeobject_Network.h \
    GlobalData/configtreeobject_Record.h \
    GlobalData/configtreeobject_Report.h \
    GlobalData/configtreeobject_SericalClient.h \
    GlobalData/configtreeobject_Sys.h \
    GlobalData/configtreeobject_Timer.h \
    Common/winchannelselector.h \
    Common/winnetworkconfig.h \
    Common/application.h \
    Common/qactionevent.h \
    Common/winserialconfig.h \
    Common/ipset.h \
    Common/englishinput.h \
    Common/wininputconfig.h \
    WorkWin/winackalarmdialog.h \
    Common/timeinput.h \
    GlobalData/cfgconfig.h\
    DataModel/modelsummaryalarm.h \
    DataModel/modelsummarymem.h \
    DataModel/modellog.h \
    DataModel/modelmodbusclient.h \
	Common/setdatetime.h \
    WorkWin/winrecorddialog.h \
    WorkWin/winmediadialog.h \
    WorkWin/winmathdialog.h \
    Common/winaiconfig.h \
    Common/xmlconfigparser.h \
    Common/softkeypad.h \
    WorkWin/winmultipanelstyledialog.h \
    Common/calculation.h \
    Threads/boardinterface.h \
    GlobalData/cfgconfig_ai.h \
    GlobalData/cfgconfig_network.h \
    GlobalData/cfgconfig_di.h \
    GlobalData/cfgconfig_do.h \
    GlobalData/cfgconfig_math.h \
    GlobalData/cfgconfig_display.h \
    GlobalData/cfgconfig_measure.h \
    GlobalData/cfgconfig_record.h \
    GlobalData/cfgconfig_datasave.h \
    GlobalData/cfgconfig_batch.h \
    GlobalData/cfgconfig_timer.h \
    GlobalData/cfgconfig_event.h \
    GlobalData/cfgconfig_serial.h \
    GlobalData/cfgconfig_comm.h \
    Common/devboard.h \
    Common/devgroup.h \
    Common/channel.h \
    Common/devchannelai.h \
    Common/devchanneldi.h \
    Common/devchanneldo.h \
    Common/devchannelmath.h \
    Common/devchannelcomm.h \
    WorkWin/wincolibrationleftpanel.h \
    WorkWin/wincolibrationmainpanel.h \
    WorkWin/winlogdialog.h \
    WorkWin/wincolibrationpanel.h \
	DataModel/modelreport.h \
    WorkWin/wincolibrationbutton.h \
    GlobalData/cfgconfig_system.h \
	Common/printtest.h \
    Common/windoconfig.h \
	WorkWin/winsnapshotdialog.h \
    WorkWin/winstandarddialog.h \
	WorkWin/wincontextdialog.h \
    Common/windiconfig.h \
    DataModel/modelsysteminformation.h \
    WorkWin/winsysteminfotableview.h \
    WorkWin/winsysteminfodelegate.h \
    WorkWin/winsysteminfoframe.h \
    WorkWin/winsysteminfodialog.h \
	WorkWin/winsysteminfotabwidget.h \
    WorkWin/winsysteminfoscroll.h \
    Common/wincommchannelconfig.h \
	WorkWin/winsaveloaddisplaydata.h \
    GlobalData/cfgconfig_report.h \
    Common/windispconfig.h \
    Common/winmathconfig.h \
	Common/winsystemconfig.h \
    Common/wineventconfig.h \
    Common/winmeasureconfig.h \
    DataModel/modelsummarymessage.h \
    Common/logger.h \
    Common/custombutton.h \
	Threads/ftpclientprocess.h \
    WorkWin/wininfolistdialog.h \
	WorkWin/winsummarydialog.h \
	Threads/threadmodbusservermsg.h \
    WorkWin/winsaveloadmainframe.h \
    WorkWin/winsaveload_buttonFactory.h \
    Common/channelchoice.h \
    Common/channeldeledata.h \
    Common/channelview.h \
    Common/channemodel.h \
    GlobalData/cfgGlobalDef_Security.h \
    GlobalData/cfgconfig_security.h \
    Common/authorization.h \
    GlobalData/configtreeobject_Security.h \
    WorkWin/winreportsummary.h \
    Common/eventcalendar.h \
    Common/eventcalendardelegate.h \
    Common/eventcalendarmodel.h \
    Common/eventcalendarview.h \
    Common/eventlistdelegate.h \
    Common/eventlistmodel.h \
    Common/eventlistview.h \
    Common/messagelistdelegate.h \
    Common/messagelistview.h \
    Common/messagelistmodel.h \
    Common/displaymessage.h


RESOURCES += \
    source.qrc
