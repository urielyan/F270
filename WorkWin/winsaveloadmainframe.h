/*********************************************************************
 * Copyright(c) 2015, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：winsaveloadframe.h
 * 概   要：保存载入画面中可以滑动的画面的实现
 *         包括选择存储内存，选择组，选择文件类型，选择开关，读入展示数据，读入事件数据，读入设定数据，保存设定数据，文件一览,格式化.
 *
 * 当前版本：
 * 作   者：闫葛
 * 完成日期：
 *
 * 修改版本：
 * 修改说明：
 * 修改作者：
 *
 ********************************************************************/

#ifndef WINSAVELOADMAINFRAME_H
#define WINSAVELOADMAINFRAME_H

#include <QWidget>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QLabel>
#include <QScrollArea>
#include <QGraphicsView>
#include <QDialog>

#include "../Common/sysconfigslidingscreen.h"
#include "windialog.h"

//输入文字时传给输入窗口的ID号
#define ID_SAVELOAD_SAVE_PAREMENTER_NAME 21750
#define ID_SAVELOAD_SAVE_PAREMENTER_COMMENT 21751
#define ID_SAVELOAD_SAVE_SCALE_NAME 21752
#define ID_SAVELOAD_SAVE_REPORT_NAME 21753
#define ID_SAVELOAD_SAVE_ALL_NAME 21754
#define ID_SAVELOAD_FORMATE_NAME 21756

typedef enum panel{
    LOADDISPLAYDATA = 0,
    LOADEVENTDATA,
    LOADSETTINGSDATA,
    SAVESETTINGSDATA,
    FILELIST,
    FORMAT,
}WINSAVELOADMAINFRAME_TYPE;

class WinSvLdFrame;
class ButtonFactory;
class QSocketNotifier;


//The winMainFrame is the widget that can slide in main widget.
class WinSaveLoadMainFrame : public SysConfigSlidingScreen{
    Q_OBJECT
public:
    static WinSaveLoadMainFrame* instance();
    explicit WinSaveLoadMainFrame(QObject *parent = 0);

    void setCurrentWidget(WINSAVELOADMAINFRAME_TYPE index);
    void execute();

    int iCurrentWidget;
private:
    WinSvLdFrame *p_saveloadPanel[6];
};

//The winSVLDFrame is the base class of all saveload windows;
class WinSvLdFrame : public QScrollArea{
    Q_OBJECT
public:
    explicit WinSvLdFrame(QString text = "", QWidget *parent = 0, bool visible = true);
    virtual bool isExecuteAble();//得到界面中按钮是否可执行的状态
    ButtonFactory *getP_executeButton();


protected:
    uint uint_InputID;
    ButtonFactory *p_selectMemeoryButton;
    ButtonFactory *p_selectFileButton;
    ButtonFactory *p_executeButton;

    void insertWidget(QWidget *insert_widget);//在当前对象中插入一个QWidget
    void removeItem();//删除最后一项Item
    inline void insertlabel(QString text = " ");

    inline void insertSelectMemoryButton();
    inline void insertSelectFileButton();

    inline void slotSelectFile();//显示DisplayData文件系统,在子类的槽函数中调用

protected slots:
    virtual void execute();
    virtual void slotReturnClicked();

    void slotSelectMemory();//选择存储内存
    void slotSelectgroup();//选择组
    void slotInputText();//输入文字
    void slotSetButtonRightText();

private:
    QVBoxLayout *vLayoutMain;
    ButtonFactory *p_inputButton;


};

//左边的菜单界面
class WinLeftButtonPanel: public /*QScrollArea*/WinSvLdFrame{
    Q_OBJECT
public:
    //static WinLeftButtonPanel* instance();
    explicit WinLeftButtonPanel( QWidget *parent = 0);
private slots:
    void slotButtonClicked(int type);
};


//File list.
class WinFileList : public WinSvLdFrame
{
    Q_OBJECT
public:
    explicit WinFileList(QWidget *parent = 0);
    void execute();
    bool isExecuteAble();
};


//load display data.
class WinLoadDisplayData : public WinSvLdFrame
{
    Q_OBJECT
public:
    explicit WinLoadDisplayData(QWidget *parent = 0);
    void execute();
    bool isExecuteAble();

private slots:
    void slotSelectFile();
};


//load event data.
class WinLoadEventData : public WinSvLdFrame
{
    Q_OBJECT
public:
    explicit WinLoadEventData(QWidget *parent = 0);
    void execute();
    bool isExecuteAble();

private slots:
    void slotSelectFile();
};


//format
class WinFormat : public WinSvLdFrame
{
    Q_OBJECT
public:
    explicit WinFormat(QWidget *parent = 0);
    void execute();
    bool isExecuteAble();

private:
    ButtonFactory *p_inputLabelButton;
};


//Load settings data
class WinLoadSettingsData : public WinSvLdFrame
{
    Q_OBJECT
public:
    enum SETTINGS
    {
        SETTINGS_SET_PARAMETER= 0,
        SETTINGS_SCALE_IMAGE,
        SETTINGS_REPORT_TEMPLATE,
        SETTINGS_TRUSTED_CERTIFICATE,
        SETTINGS_ALL,

    };
    explicit WinLoadSettingsData(QWidget *parent = 0);
    void execute();
    int iCurrentIndex;

private slots:
    void slotButtonClicked(int index);

private:
    WinSvLdFrame * p_panel[5];
};

//Load setttings data->set parameter.
class WinSetParameter1 : public WinSvLdFrame
{
    Q_OBJECT
public:
    enum SETTINGS_TYPE{
        SETTINGS_ALL = 0,
        SETTINGS_IP,
        SETTINGS_SAFETY,
        SETTINGS_OTHER,
    };
    explicit WinSetParameter1(QWidget *parent = 0);

public slots:
    void execute();
    bool isExecuteAble();

private:
    QFrame *p_frameButton;
    ButtonFactory *p_selectSwitchButton[4];
    //ButtonFactory *p_selectFileButton;
private slots:
    void slotSelectFile();
    void slotSelectSwitch(int settings_type);
};

//Load setttings data->scale image
class WinScaleImage1  : public WinSvLdFrame{
    Q_OBJECT
public:
    enum SCALEIMAGE
    {
        LOADIMAGE = 0,
        DELETEIMAGE,
    };
    explicit WinScaleImage1(QWidget *parent = 0);
    void execute();
private:
    WinSvLdFrame *p_panel[2];
    QButtonGroup *p_buttonGroup;
    //WinSvLdFrame *p_panelDeleteImage;
    int currentWidget;

private slots:
    void slotButtonClicked(int index);
};

//Load setttings data->scale image->Load image
class WinLoadImage1 : public WinSvLdFrame
{
    Q_OBJECT
public:
    explicit WinLoadImage1(QWidget *parent = 0);
    void execute();
    bool isExecuteAble();

private:
    //ButtonFactory *p_selectFileButton;
    ButtonFactory *p_selectGroupButton[2];

private slots:
    void slotSelectFile();
    bool slotStoreScaleImageFromMemory();
};

//Load setttings data->scale image->Delete image
class WinDeleteImage1 : public WinSvLdFrame
{
    Q_OBJECT
public:
    explicit WinDeleteImage1(QWidget *parent = 0);
    void execute();
    bool isExecuteAble();

private:
    ButtonFactory *p_selectGroupButton,*p_Data;
private slots:
    //void slotSelectgroup();
};

//Load  setttings data->report template
class WinReportTemplate1 : public WinSvLdFrame
{
    Q_OBJECT
public:
    explicit WinReportTemplate1(QWidget *parent = 0);
    void execute();
    bool isExecuteAble();

private:
    ButtonFactory *p_databutton;
private slots:
    void slotSelectFileType();
    void slotSelectFile();
    void slotSelectReportType();
};

//Load  setttings data->Trusted Certificate
class WinTrustedCerticicate1 : public WinSvLdFrame
{
    Q_OBJECT
public:
    enum SCALEIMAGE
    {
        LOAD_TRUSTED_CERTIFICATE = 0,
        DELETE_TRUSTED_CERTIFICATE,
    };
    explicit WinTrustedCerticicate1(QWidget *parent = 0);
    void execute();

private:
    WinSvLdFrame *p_panel[2];
    QButtonGroup *p_buttonGroup;
    int currentWidget;

private slots:
    void slotButtonClicked(int index);
};

//Load  setttings data->Trusted Certificate->Load Trusted Certificate
class WinLoadTrustedCerticicate1 : public WinSvLdFrame
{
    Q_OBJECT
public:
    enum SCALEIMAGE
    {
        ENUM_ISSUER = 0,
        ENUM_SUBJECT,
        ENUM_VALIDNOTBEFORE,
        ENUM_VALIDNOTAFTER,
        ENUM_HASHVALUEFIRSTHALF,
        ENUM_HASHVALUESECONDHALF,
    };
    explicit WinLoadTrustedCerticicate1(QWidget *parent = 0);
    void execute();
    bool isExecuteAble();

private:
    QButtonGroup *p_buttonGroup;
    ButtonFactory *p_InformationButton[6];

private slots:
    void slotSelectFile();
};

//Load  setttings data->Trusted Certificate->Delete Trusted Certificate
class WinDeleteTrustedCerticicate1 : public WinSvLdFrame
{
    Q_OBJECT
public:
    enum SCALEIMAGE
    {
        ENUM_ISSUER = 0,
        ENUM_SUBJECT,
        ENUM_VALIDNOTBEFORE,
        ENUM_VALIDNOTAFTER,
        ENUM_HASHVALUEFIRSTHALF,
        ENUM_HASHVALUESECONDHALF,
    };
    explicit WinDeleteTrustedCerticicate1(QWidget *parent = 0);
    void execute();
    bool isExecuteAble();

private:
    QButtonGroup *p_buttonGroup;
    ButtonFactory *p_InformationButton[6];
private slots:
    void slotSelectFile();
};

//Load  setttings data->Above all
class WinAboveAll1 : public WinSvLdFrame
{
    Q_OBJECT
public:
    explicit WinAboveAll1(QWidget *parent = 0);
    void execute();
    bool isExecuteAble();

private:
    ButtonFactory *p_selectFolderButton;
private slots:
    void slotSelectMemory();//重写选择内存槽函数
    void slotSelectFolder();//选择文件夹
};


//Save settings data
class WinSaveSettingsData : public WinSvLdFrame
{
    Q_OBJECT
public:
    enum SETTINGS
    {
        SETTINGS_SET_PARAMETER= 0,
        SETTINGS_SCALE_IMAGE,
        SETTINGS_REPORT_TEMPLATE,
        SETTINGS_TRUSTED_CERTIFICATE,
        SETTINGS_ALL,
    };
    explicit WinSaveSettingsData(QWidget *parent = 0);
    void execute();

    int iCurrentIndex;
private slots:
    void slotButtonClicked(int index);

private:
    WinSvLdFrame *p_panel[5];
};

//Save settings data->set parameter
class WinSetParameter_save1 : public WinSvLdFrame
{
    Q_OBJECT
public:
    enum SETTINGS_TYPE{
        SETTINGS_ALL = 0,
        SETTINGS_IP,
        SETTINGS_SAFETY,
        SETTINGS_OTHER,
    };
    explicit WinSetParameter_save1(QWidget *parent = 0);
    void execute();
    bool isExecuteAble();

private:
    ButtonFactory *p_FileButton;
    ButtonFactory *p_commentButton;
private slots:
    void slotStoreDataToFile();//存储二进制文件到相应存储位置

    void slotInputFileName();
    void slotInputComment();
};

//Save setttings data->scale image
class WinScaleImage_save1 : public WinSvLdFrame
{
    Q_OBJECT
public:
    explicit WinScaleImage_save1(QWidget *parent = 0);
    void execute();
    bool isExecuteAble();

private:
    ButtonFactory *p_inputFileNameButton, *p_selectGroupButton, *p_DataButton;
private slots:
    //void slotSelectgroup();
};

//Save setttings data->report template
class WinReportTemplate_save1 : public WinSvLdFrame
{
    Q_OBJECT
public:
    explicit WinReportTemplate_save1(QWidget *parent = 0);
    void execute();
    bool isExecuteAble();

private:
    ButtonFactory *p_inputFileNameButton;
    ButtonFactory *p_databutton;

private slots:
    void slotSelectFileType();
    void slotSelectReportType();
};

//Save setttings data->Trusted Certificate
class WinTrustedCertificate_save1 : public WinSvLdFrame
{
    Q_OBJECT
public:
    explicit WinTrustedCertificate_save1(QWidget *parent = 0);
    void examineDataButtonIsExist();
    void execute();
    bool isExecuteAble();

private:
    ButtonFactory *p_haveDataButton;

};

//Save  setttings data->Above all
class WinAboveAll_save1 : public WinSvLdFrame
{
    Q_OBJECT
public:
    explicit WinAboveAll_save1(QWidget *parent = 0);
    void execute();
    bool isExecuteAble();

private:
    ButtonFactory *p_inputTextButton;
private slots:
};

//The winSelectMemory class is the display windows when user push a button of select memory.
class WinSelectMemory : public WinSvLdFrame
{
    Q_OBJECT
public:
    enum ENUM_MEMORY_STATE{
        ENUM_NOSD_NOUSB = 0,
        ENUM_SD = 1,
        ENUM_USB,
        ENUM_SD_USB,
    };
    static WinSelectMemory* instance();

    static QString SD,USB,SD_PATH,USB_PATH,STRINGNOSD_NOUSB;
    void setGoalButton(ButtonFactory*b, ButtonFactory *b2);//设置目标按钮
    void insertButtonToList(ButtonFactory*b);//增加一个选择内存的按钮到m_hadSetButton

    ENUM_MEMORY_STATE getMemoryState();
    bool examineSetAllButton();
    bool examineButtonMemoryIsExist(ButtonFactory*p_selectMemeoryButton);
    bool sdIsExist();
    bool usbIsExist();

public slots:

private slots:
    //两个按钮被点击的槽函数
    void slotBSDClicked();
    void slotBUsbClicked();
    void slotBNoneMemoryClicked();
    void slotReturnClicked();
private:
    explicit WinSelectMemory(QWidget *parent = 0);

    ButtonFactory *bsd, *bflash, *bNoSdUsb;
    ButtonFactory *p_goalButton, *p_attachButton;
    QList <ButtonFactory*> m_hadSetButton;//包含所有被按下过的按钮(Select memory)
};

//The winSelectMemory class is the display windows when user push a button of select switch.
class WinSelectSwitch : public WinSvLdFrame
{
    Q_OBJECT
public:
    static WinSelectSwitch* instance();
    void setGoalButton(ButtonFactory*b){p_goalButton = b;}
    void setHideWidget(QWidget* hideWidget){p_hideWidget = hideWidget;}

private:
    explicit WinSelectSwitch(QWidget *parent = 0);

    ButtonFactory *p_goalButton;
    QWidget *p_hideWidget;

private slots:
    //两个按钮被点击的槽函数
    void slotBOnClicked();
    void slotBOffClicked();
};

//The winSelectMemory class is the display windows when user push a button of select group.
class WinSelectGroup : public WinSvLdFrame
{
    Q_OBJECT
public:
    static WinSelectGroup* instance();
    static const QString string_ScaleImagePath, //标尺图像存放路径
            string_fileExist, //组标尺图像存在时设置的按钮文字
            string_fileNotExist;//组标尺图像不存在时设置的按钮文字
    bool examineGroupDataIsExist(int index = 1);
    void setGoalButton(ButtonFactory*b1, ButtonFactory*b2);

private:
    explicit WinSelectGroup(QWidget *parent = 0);

    ButtonFactory *p_goalButton;
    ButtonFactory *p_dataButton;

    //记录所有选择组号码是否有标尺图像的按钮和对应是否有数据的按钮
    QMap <ButtonFactory*, ButtonFactory*> m_hadSetButton;

private slots:
    void slotButtonClicked(int index);

};

//The winSelectFileType class is the display windows when user push a button of select File type.
class WinSelectFileType : public WinSvLdFrame
{
    Q_OBJECT
public:
    static WinSelectFileType* instance();
    static QString string_excel, string_pdf;
    void setGoalButton(ButtonFactory*b){p_goalButton = b;}

private:
    ButtonFactory *p_goalButton;
    explicit WinSelectFileType(QWidget *parent = 0);

private slots:
    void slotPdfButtonClicked();
    void slotExcelButtonClicked();
};

//The winSelectFileType class is the display windows when user push a button of select Report type.
class WinSelectReportType : public WinSvLdFrame
{
    Q_OBJECT
    enum REPORT_TYPE{
        REPORT_Hourly = 0,
        REPORT_Daily,
        REPORT_Weekly,
        REPORT_Monthly,
        REPORT_Hourly_Daily,
        REPORT_Daily_Weekly,
        REPORT_Weekly_Monthly,
        REPORT_Batch,
        REPORT_Daily0custom,
    };
public:
    static WinSelectReportType* instance();
    void setGoalButton(ButtonFactory*b){p_goalButton = b;}

private:
    QMap <int,QString> reportType;
    ButtonFactory *p_goalButton;
    explicit WinSelectReportType(QWidget *parent = 0);

private slots:
    void slotButtonClicked(int index);

};

//若文件已存在弹出这个消息对话框
class WinSureOperateDialog : public WinDialog
{
    Q_OBJECT
public:
    explicit WinSureOperateDialog(QString text);

private slots:
    void slotYesClicked();
};

//存储文件时弹出此对话框
class WinInformationDialog : public WinDialog
{
    Q_OBJECT
public:
    static const QString  string_executeSuccuss,//执行成功时提示文字
                    string_executedefeated,//执行失败时提示文字
                    string_fileNotExist;//当文件不存在时提示信息
    explicit WinInformationDialog(QString text);
};
#endif
