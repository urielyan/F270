#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <sys/un.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/types.h>
#include <linux/netlink.h>
#include <errno.h>
#include <unistd.h>

#include <QtCore/QSocketNotifier>
#include <QBuffer>

#include "winsaveloadmainframe.h"
#include "winsaveload.h"
#include "winsaveload_buttonFactory.h"
#include "winsaveloaddisplaydata.h"
#include "../Common/softkeypad.h"
#include "../Common/englishinput.h"
#include "winmainframe.h"

#include <QScroller>
#include <QScrollBar>
#include <QProcess>

static SysConfigSlidingScreen * slidingWidget = NULL;
static EnglishInput *winInputText = NULL;

//WinLeftButtonPanel *WinLeftButtonPanel::instance()
//{
//    static int tmp = 0;
//    if(tmp== 0 && NULL ==  m_Instance)
//    {
//        m_Instance = new WinLeftButtonPanel;
//        tmp =1;
//    }
//    return m_Instance;
//}

WinLeftButtonPanel::WinLeftButtonPanel(QWidget *parent)
: WinSvLdFrame("", parent, false)
{
    ButtonFactory *p_button[6];
    p_button[LOADDISPLAYDATA] = ButtonFactory::
            getButton(tr("Load display data"),ButtonFactory::BUTTON_PIXMAPLABEL,
                      this,new QPixmap(":/saveload/Image/saveload/readdisplaydata.png"));
    p_button[LOADEVENTDATA] = ButtonFactory::
            getButton(tr("Load event data"),ButtonFactory::BUTTON_PIXMAPLABEL,
                      this,new QPixmap(":/saveload/Image/saveload/readeventdata.png"));
    p_button[LOADSETTINGSDATA] = ButtonFactory::
            getButton(tr("Load settings data"),ButtonFactory::BUTTON_PIXMAPLABEL,
                      this,new QPixmap(":/saveload/Image/saveload/readsetdata.png"));
    p_button[SAVESETTINGSDATA] = ButtonFactory::
            getButton(tr("Save settings data"),ButtonFactory::BUTTON_PIXMAPLABEL,
                      this,new QPixmap(":/saveload/Image/saveload/savesetdata.png"));
    p_button[FILELIST] = ButtonFactory::
            getButton(tr("File list"),ButtonFactory::BUTTON_PIXMAPLABEL,
                      this,new QPixmap(":/saveload/Image/saveload/fileall.png"));
    p_button[FORMAT] = ButtonFactory::
            getButton(tr("Format"),ButtonFactory::BUTTON_PIXMAPLABEL,
                      this,new QPixmap(":/saveload/Image/saveload/format.png"));

    QButtonGroup *p_buttonGroup = new QButtonGroup(this);
    p_buttonGroup->addButton(p_button[LOADDISPLAYDATA],LOADDISPLAYDATA);
    p_buttonGroup->addButton(p_button[LOADEVENTDATA],LOADEVENTDATA);
    p_buttonGroup->addButton(p_button[LOADSETTINGSDATA],LOADSETTINGSDATA);
    p_buttonGroup->addButton(p_button[SAVESETTINGSDATA],SAVESETTINGSDATA);
    p_buttonGroup->addButton(p_button[FILELIST],FILELIST);
    p_buttonGroup->addButton(p_button[FORMAT],FORMAT);

    connect(p_buttonGroup,SIGNAL(buttonClicked(int)),
            this,SLOT(slotButtonClicked(int)));

    foreach (QAbstractButton *button, p_buttonGroup->buttons()) {
        insertWidget(button);
    }

    //设置宽度保证和按钮保持宽度一致
    this->setMaximumWidth(DESKTOP_WIDTH / 5 );
    this->setMinimumWidth(DESKTOP_WIDTH / 7 );
    p_button[LOADDISPLAYDATA]->click();//init WinSaveLoadMainFrame
}

void WinLeftButtonPanel::slotButtonClicked(int type)
{
    WinSaveLoadMainFrame::instance()->setCurrentWidget(static_cast<WINSAVELOADMAINFRAME_TYPE> (type));
}

WinSaveLoadMainFrame *WinSaveLoadMainFrame::instance()
{
    static WinSaveLoadMainFrame Instance;
    return &Instance;
}

WinSaveLoadMainFrame::WinSaveLoadMainFrame(QObject */*parent*/)
{
    this->setFrameShape(QFrame::NoFrame);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setStyleSheet("*{background-color:rgb(0,0,0,0);padding:0px;border-width:-2px;margin:0px;}");
    this->setObjectName("slidingWidget");
    this->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum);
    this->setMaximumHeight(10000);
    this->setMaximumWidth(10000);

    p_saveloadPanel[LOADDISPLAYDATA] = new WinLoadDisplayData();
    p_saveloadPanel[LOADEVENTDATA] = new WinLoadEventData();
    p_saveloadPanel[LOADSETTINGSDATA] = new WinLoadSettingsData();
    p_saveloadPanel[SAVESETTINGSDATA] = new WinSaveSettingsData();
    p_saveloadPanel[FILELIST] = new WinFileList();
    p_saveloadPanel[FORMAT] = new WinFormat();
    slidingWidget = this;

    //初始化一些按钮上的文字
    WinSelectGroup::instance()->examineGroupDataIsExist();//检查标尺图像是否存在
    WinSelectMemory::instance()->examineSetAllButton();//检查设置选择内存界面的相应内存是否存在；检查选择文件按钮上的文件是否还存在.

}

void WinSaveLoadMainFrame::setCurrentWidget(WINSAVELOADMAINFRAME_TYPE index)
{
    slidingWidget->setInitWidget(p_saveloadPanel[index]);
    winsaveload::setp_buttonExecuted(p_saveloadPanel[index]/*->getP_executeButton()*/, true);
    iCurrentWidget = index;

//    if(iCurrentWidget == LOADSETTINGSDATA){
//        //winsaveload::setp_buttonExecuteDisabled(true);
//        return;
//    }
//    if(iCurrentWidget == SAVESETTINGSDATA){
//         //winsaveload::setp_buttonExecuteDisabled(true);
//        return;
//    }
//    winsaveload::setp_buttonExecuteDisabled(false);
}

void WinSaveLoadMainFrame::execute()
{
    qDebug() << __LINE__;
    //p_saveloadPanel[iCurrentWidget]->execute();
}

/**
 * 功能：
 *          保存载中所有界面的父类
 * 参数：
 *          text: 界面标题
 *          parent: 父节点
 *          visible: 返回按钮是否可见
 */
WinSvLdFrame::WinSvLdFrame(QString text, QWidget *parent, bool visible) :
    QScrollArea(parent)
{
    static int tmp = 1;
    p_selectFileButton = NULL;

    p_executeButton = ButtonFactory::getButton
            (QString::number(tmp++)/*tr("Execute")*/, ButtonFactory::BUTTON_LABELPIXMAP_SMALL);
    p_executeButton->setEnabled(false);

    qDebug() << this << tmp;

    connect(p_executeButton, SIGNAL(clicked(bool)), this, SLOT(execute()));
    execute();

    uint_InputID = 0;
    this->setFrameShape(QFrame::NoFrame);
    this->setWidgetResizable(true);
    vLayoutMain = new QVBoxLayout(this);
    vLayoutMain->setContentsMargins(0,0,0,0);
    vLayoutMain->setSpacing(0);
    vLayoutMain->setMargin(0);

    QFrame *winReturn = new QFrame(this);
    winReturn->setObjectName("winReturn");
    QHBoxLayout *hLayout = new QHBoxLayout(winReturn);
    winReturn->setFixedHeight(60);
    if(visible){
        QPushButton *bReturn = new QPushButton(winReturn);
        bReturn->setFixedHeight(50);
        bReturn->setFocusPolicy(Qt::NoFocus);
        bReturn->setObjectName("bReturn");
        bReturn->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Minimum);
        connect(bReturn,SIGNAL(clicked(bool)),this,SLOT(slotReturnClicked()));
        hLayout->addWidget(bReturn);
    }

    QLabel *lTitle = new QLabel(text,winReturn);
    lTitle->setStyleSheet("background-color:rgb(0,5,146,0);");
    lTitle->setObjectName("lTitle");
    lTitle->setFont(QFont("seria",20));
    lTitle->setAlignment(Qt::AlignCenter);
    hLayout->addWidget(lTitle);

    vLayoutMain->addWidget(winReturn);

    vLayoutMain->addStretch();
    this->setStyleSheet("WinSvLdFrame{"
                        "background-color:rgba(0,0,0,0);"
                        "margin:0px;"
                        "padding : 0px;"
                        "border-width: 0px;}");
    winReturn->setStyleSheet("#winReturn{"
                                  "background-color:rgb(0,5,146,0);"
                                  "margin-bottom:2px;"
                                  "border-width:0px;"
                                  "}"
                                  "QPushButton#bReturn{"
                                  "qproperty-icon: url(:/saveload/Image/saveload/return.png);"
                                  "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
                                  "stop: 0 rgb(119, 170, 225), stop: 0.04 rgb(86, 134, 196),"
                                  " stop: 0.041 rgb(60, 103, 176),"
                                  "stop: 0.51 rgb(0, 7, 134), stop: 1 rgb(20, 58, 169));"
                                  "qproperty-iconSize: 32px;"
                                  "padding-left:-0px;"
                                  "color:cyan;"
                                  "font-size:15px;"
                                  "height:30px;"
                                  "width:68px;"
                                  "margin-bottom:5px;"
                                  "border-top:1px solid rgb(85, 111, 224);"
                                  "text-align:centre"
                                  "}"
                                  "QPushButton#bReturn:pressed{"
                                  "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
                                  "stop: 0 #0000FF, stop: 1 #0000CC);"
                                  "}"
                                  "QLabel{"
                                  "background-color:rgb(100, 41, 200);"
                                  "color:cyan;"
                                  "font-size:15px;"
                                  "text-align:right;"
                                  "}");
}

void WinSvLdFrame::execute()
{
    qDebug() <<this->metaObject()->className() << __LINE__;
}

//得到界面中按钮是否可执行的状态
bool WinSvLdFrame::isExecuteAble()
{
    qDebug() <<this->metaObject()->className() << __LINE__;

    return false;
}

//在当前对象中插入一个QWidget
void WinSvLdFrame::insertWidget(QWidget *insert_widget)
{
    vLayoutMain->insertWidget(vLayoutMain->count() - 1 ,insert_widget);
}

//删除最后一项Item
void WinSvLdFrame::removeItem()
{
    vLayoutMain->removeItem(vLayoutMain->itemAt(vLayoutMain->count() - 1));
}

//得到当前界面的执行按钮并设置状态
ButtonFactory *WinSvLdFrame::getP_executeButton()
{
    p_executeButton->setEnabled(isExecuteAble());
    return p_executeButton;
}

//
void WinSvLdFrame::insertSelectMemoryButton()
{
    p_selectMemeoryButton = ButtonFactory::
            getButton(tr("Select memory"),ButtonFactory::BUTTON_LABELLABEL,this);
    insertWidget(p_selectMemeoryButton);
    connect(p_selectMemeoryButton, SIGNAL(clicked(bool)), this, SLOT(slotSelectMemory()));
    p_selectMemeoryButton->setRightLabelText(WinSelectMemory::SD);
    WinSelectMemory::instance()->insertButtonToList(p_selectMemeoryButton);
}

//插入一个选择文件的按钮
void WinSvLdFrame::insertSelectFileButton()
{
    p_selectFileButton= ButtonFactory::
            getButton(tr("Select file"),ButtonFactory::BUTTON_LABELLABEL,this);
    insertWidget(p_selectFileButton);
    DisplayData::instance()->insertButtonToList(p_selectFileButton);
}

//插入一个固定宽度的label
void WinSvLdFrame::insertlabel(QString text)
{
    QLabel *label = new QLabel(text);
    label->setFixedHeight(30);
    insertWidget(label);
    label->setStyleSheet("background-color:rgba(0, 0, 0, 0);"
                         "color: cyan;"
                         "margin-left: 5px;");
}

//显示DisplayData文件系统
void WinSvLdFrame::slotSelectFile()
{
    DisplayData::instance()->setP_goalButton(p_selectFileButton);
    if(DisplayData::instance()->setFileSystemBYButton(p_selectMemeoryButton) == false)
        return;
    DisplayData::instance()->show();
}

void WinSvLdFrame::slotSelectMemory()
{
    //ButtonFactory *buttonSender = static_cast<ButtonFactory *> (sender());
    WinSelectMemory::instance()->setGoalButton(p_selectMemeoryButton,p_selectFileButton);
    WinSelectMemory::instance()->examineSetAllButton();

    slidingWidget->setNextWidget(WinSelectMemory::instance());
    slidingWidget->moveToNextWidget();
    winsaveload::setp_buttonExecuted(WinSelectMemory::instance()/*->getP_executeButton()*/);
}

void WinSvLdFrame::slotSelectgroup()
{
    ButtonFactory *p_selectButton;
    p_selectButton = static_cast<ButtonFactory *> (sender());

    //因为是否有数据按钮总是在最后一项，找到子节点的最后一项就是哪个按钮指针
    ButtonFactory *p_dataButton = this->findChildren<ButtonFactory *>().at(this->findChildren<ButtonFactory *>().count()-1);
    WinSelectGroup::instance()->setGoalButton(p_selectButton,p_dataButton); //设置组按钮和此按钮是否有数据

    slidingWidget->setNextWidget(WinSelectGroup::instance());
    winsaveload::setp_buttonExecuted(WinSelectGroup::instance());
    slidingWidget->moveToNextWidget();
    //winsaveload::setp_buttonExecuteDisabled(true);
}

void WinSvLdFrame::slotInputText()
{
    if(NULL == winInputText){
        winInputText = new EnglishInput();
        winInputText->setWindowModality(Qt::ApplicationModal);
    }
    connect(winInputText, SIGNAL(sigEnglishInputOver()),
            this, SLOT(slotSetButtonRightText()));
    p_inputButton = static_cast<ButtonFactory *> (sender());
    winInputText->setshow(p_inputButton->getRightLabelText());
    winInputText->show(ID_System_Device_Tag);
    //winInputText->show(uint_InputID);//FORMAT:最多11 个字符(0 ～ 9、大写英文字母)
}

//当按下Ok键时得到输入框中的文字
void WinSvLdFrame::slotSetButtonRightText(){
    p_inputButton->setRightLabelText(winInputText->getInputStr());
    p_executeButton->setEnabled(isExecuteAble());

    winInputText->close();
    disconnect(winInputText, SIGNAL(sigEnglishInputOver()),
            this, SLOT(slotSetButtonRightText()));
}

void WinSvLdFrame::slotReturnClicked()
{
    slidingWidget->moveToPreWidget();
    winsaveload::setp_buttonExecuted();
//    if(QString(this->metaObject()->className()).contains("1"))
//    {
//        winsaveload::setp_buttonExecuteDisabled(true);
//    }else
//    {
//        winsaveload::setp_buttonExecuteDisabled(false);
//    }
}

WinFileList::WinFileList(QWidget *parent) :
    WinSvLdFrame(tr("File list"), parent, false)
{
    insertSelectMemoryButton();
    DisplayData_FileList::instance()->setP_goalButton(p_selectFileButton);
}

void WinFileList::execute()
{
    if(DisplayData_FileList::instance()->setFileSystemBYButton(p_selectMemeoryButton) == false)
        return;
    //ButtonFactory *p_selectButton = static_cast<ButtonFactory *> (sender());
    DisplayData_FileList::instance()->show();
}

bool WinFileList::isExecuteAble()
{
    qDebug() << WinSelectMemory::instance()->getMemoryState();
    return WinSelectMemory::instance()->getMemoryState();
}

WinLoadDisplayData::WinLoadDisplayData(QWidget *parent) :
    WinSvLdFrame(tr("Load display data"), parent, false)
{
    insertSelectMemoryButton();

    insertSelectFileButton();
    connect(p_selectFileButton,SIGNAL(clicked(bool)),this,SLOT(slotSelectFile()));
}

void WinLoadDisplayData::execute()
{
    qDebug() <<this->metaObject()->className() << __LINE__;
    WinInfoListDialog::instance()->showMsg(801);//Execution completed.
}

bool WinLoadDisplayData::isExecuteAble()
{
    return WinSelectMemory::instance()->getMemoryState()
            && (p_selectFileButton->getRightLabelText().compare(""));
}

void WinLoadDisplayData::slotSelectFile()
{
    DisplayData::instance()->setm_NameFilterType("GDS");
    WinSvLdFrame::slotSelectFile();
}


WinLoadEventData::WinLoadEventData(QWidget *parent) :
    WinSvLdFrame(tr("Load event data"), parent, false)
{
    insertSelectMemoryButton();

    insertSelectFileButton();
    connect(p_selectFileButton,SIGNAL(clicked(bool)),this,SLOT(slotSelectFile()));
}

void WinLoadEventData::execute()
{
    qDebug() <<this->metaObject()->className() << __LINE__;
    WinInfoListDialog::instance()->showMsg(801);//Execution completed.
}

bool WinLoadEventData::isExecuteAble()
{
    return WinSelectMemory::instance()->getMemoryState()  &&
            (p_selectFileButton->getRightLabelText().compare(""));
}

void WinLoadEventData::slotSelectFile()
{
    WinSvLdFrame::slotSelectFile();
    DisplayData::instance()->setm_NameFilterType("GEV");
}

WinFormat::WinFormat(QWidget *parent) :
    WinSvLdFrame(tr("Format"), parent, false)
{
    uint_InputID = ID_SAVELOAD_FORMATE_NAME;

    insertSelectMemoryButton();

    p_inputLabelButton = ButtonFactory::
            getButton(tr("Volume name"),ButtonFactory::BUTTON_LABELLABEL,this);
    insertWidget(p_inputLabelButton);
    connect(p_inputLabelButton,SIGNAL(clicked(bool)),this,SLOT(slotInputText()));
}

void WinFormat::execute()
{
    int status = 0;
    if(!WinSelectMemory::instance()->examineButtonMemoryIsExist(p_selectMemeoryButton))
            /*WinSelectMemory::instance()->examineSetAllButton() == false*/
    {
//        WinInformationDialog errDialog(WinInformationDialog::string_fileNotExist);
//        errDialog.exec();
//        return;
        WinInfoListDialog::instance()->showMsg(503);//Media not recognized.
    }
    WinSureOperateDialog sureDialog(tr("Execute format?"));
    if(QDialog::Accepted == sureDialog.exec())
    {

        WinInfoListDialog::instance()->showMsg(808);//Formatting... //TODO
        if(!p_selectMemeoryButton->getRightLabelText().compare(WinSelectMemory::SD))
        {
//            if(!QDir(WinSelectMemory::SD_PATH).exists())
//            {
//                WinInformationDialog errDialog(WinInformationDialog::string_fileNotExist);
//                errDialog.exec();
//                return;
//            }
            status = system(QString("umount ").append(WinSelectMemory::SD_PATH).toLocal8Bit().data());
            if((-1 == status) || WIFEXITED(status) == false || 0 != WEXITSTATUS(status) )
            {
                perror("umount");
                WinInfoListDialog::instance()->showMsg(508);//Format error.
                return;
            }

            status = system(QString("mkfs.ntfs /dev/sdb1 -f -L  ").append(
                                p_inputLabelButton->getRightLabelText()).toLocal8Bit().data());
            if((-1 == status) || WIFEXITED(status) == false || 0 != WEXITSTATUS(status) )
            {
                WinInfoListDialog::instance()->showMsg(508);//Format error.
                return;
            }

            status = system(QString("mount /dev/sdb1 ").append(WinSelectMemory::SD_PATH).toLocal8Bit().data());
            if((-1 == status) || WIFEXITED(status) == false || 0 != WEXITSTATUS(status) )
            {
                WinInfoListDialog::instance()->showMsg(508);//Format error.
                return;
            }
        }else if(!p_selectMemeoryButton->getRightLabelText().compare(WinSelectMemory::USB))
        {
//            if(!QDir(WinSelectMemory::USB_PATH).exists())
//            {
//                WinInformationDialog errDialog(WinInformationDialog::string_fileNotExist);
//                errDialog.exec();
//                return;
//            }
            status = system(QString("umount ").append(WinSelectMemory::USB_PATH).toLocal8Bit().data()) ;
            if((-1 == status) || WIFEXITED(status) == false || 0 != WEXITSTATUS(status) )
            {
                WinInfoListDialog::instance()->showMsg(508);//Format error.
                return;
            }

            status = system(QString("mkfs.ntfs /dev/sdc1/ -f -L  ").append(
                                p_inputLabelButton->getRightLabelText()).toLocal8Bit().data()) ;
            if((-1 == status) || WIFEXITED(status) == false || 0 != WEXITSTATUS(status) )
            {
                WinInfoListDialog::instance()->showMsg(508);//Format error.

                qDebug() << "Can't formate usb:mkfs.ntfs!";
                return;
            }

            status = system(QString("mount /dev/sdc1 ").append(WinSelectMemory::USB_PATH).toLocal8Bit().data()) ;
            if((-1 == status) || WIFEXITED(status) == false || 0 != WEXITSTATUS(status) )
            {
                WinInfoListDialog::instance()->showMsg(508);//Format error.

                qDebug() << "Can't mount usb:mount!";
                return;
            }
        }
    }else
    {
        return;
    }
    WinInfoListDialog::instance()->showMsg(801);//Execution completed.
}

bool WinFormat::isExecuteAble()
{
    qDebug() << (WinSelectMemory::instance()->getMemoryState() &&
                (!p_inputLabelButton->getRightLabelText().compare("")));
    return WinSelectMemory::instance()->getMemoryState() &&
            (p_inputLabelButton->getRightLabelText().compare(""));
}

WinLoadSettingsData::WinLoadSettingsData(QWidget *parent):
    WinSvLdFrame(tr("Load settings data"), parent, false)
{
    QButtonGroup *p_buttonGroup = new QButtonGroup(this);

    ButtonFactory *p_button[5];
    p_button[SETTINGS_SET_PARAMETER] = ButtonFactory::
            getButton(tr("Set parement"),ButtonFactory::BUTTON_LABELPIXMAP,this);

    p_button[SETTINGS_SCALE_IMAGE] = ButtonFactory::
            getButton(tr("Scale image"),ButtonFactory::BUTTON_LABELPIXMAP,this);

    p_button[SETTINGS_REPORT_TEMPLATE] = ButtonFactory::
            getButton(tr("Report template"),ButtonFactory::BUTTON_LABELPIXMAP,this);

    p_button[SETTINGS_TRUSTED_CERTIFICATE] = ButtonFactory::
            getButton(tr("Trusted Certificate"),ButtonFactory::BUTTON_LABELPIXMAP,this);

    p_button[SETTINGS_ALL] = ButtonFactory::
            getButton(tr("Above all"),ButtonFactory::BUTTON_LABELPIXMAP,this);


    for(int i = 0; i < 5; i++)
    {
        insertWidget(p_button[i]);
        p_buttonGroup->addButton(p_button[i],i);
    }
    connect(p_buttonGroup,SIGNAL(buttonClicked(int)),this,SLOT(slotButtonClicked(int)));

    p_panel[SETTINGS_SET_PARAMETER] = new WinSetParameter1;
    p_panel[SETTINGS_SCALE_IMAGE] = new WinScaleImage1;
    p_panel[SETTINGS_REPORT_TEMPLATE] = new WinReportTemplate1;
    p_panel[SETTINGS_TRUSTED_CERTIFICATE] = new WinTrustedCerticicate1;
    p_panel[SETTINGS_ALL] = new WinAboveAll1;
}

void WinLoadSettingsData::execute()
{
    qDebug() << __LINE__;

    //p_panel[iCurrentIndex]->execute();
    //qDebug() << slidingWidget->children().at(slidingWidget->children().count() - 1);
}

void WinLoadSettingsData::slotButtonClicked(int index)
{
    iCurrentIndex = index;
    slidingWidget->setNextWidget(p_panel[index]);
    slidingWidget->moveToNextWidget();

    winsaveload::setp_buttonExecuted(p_panel[index]/*->getP_executeButton()*/);
}

WinSetParameter1::WinSetParameter1(QWidget *parent) :
    WinSvLdFrame("Set parameter", parent)
{
    insertSelectMemoryButton();

    insertSelectFileButton();
    connect(p_selectFileButton,SIGNAL(clicked(bool)),this,SLOT(slotSelectFile()));

    insertlabel();

    p_frameButton = new QFrame(this);
    QVBoxLayout *vLayoutFrameButton = new QVBoxLayout(p_frameButton);
    p_frameButton->setStyleSheet("background-color:rgba(0, 0, 0, 0);");
    vLayoutFrameButton->setContentsMargins(10, 0, 0, 0);
    vLayoutFrameButton->setSpacing(0);

    QButtonGroup *p_buttonGroup = new QButtonGroup(this);

    p_selectSwitchButton[SETTINGS_ALL] = ButtonFactory::
            getButton(tr("All settings"),ButtonFactory::BUTTON_LABELLABEL,this);
    p_selectSwitchButton[SETTINGS_ALL]->setObjectName("All settings");
    p_selectSwitchButton[SETTINGS_ALL]->setRightLabelText(tr("On"));
    p_buttonGroup->addButton(p_selectSwitchButton[SETTINGS_ALL],SETTINGS_ALL);
    insertWidget(p_selectSwitchButton[SETTINGS_ALL]);

    insertlabel();

    p_selectSwitchButton[SETTINGS_IP] = ButtonFactory::
            getButton(tr("IP"),ButtonFactory::BUTTON_LABELLABEL,p_frameButton);
    p_selectSwitchButton[SETTINGS_IP]->setRightLabelText(tr("On"));
    p_buttonGroup->addButton(p_selectSwitchButton[SETTINGS_IP],SETTINGS_IP);
    vLayoutFrameButton->addWidget(p_selectSwitchButton[SETTINGS_IP]);

    p_selectSwitchButton[SETTINGS_SAFETY] = ButtonFactory::
            getButton(tr("Safety"),ButtonFactory::BUTTON_LABELLABEL,p_frameButton);
    p_selectSwitchButton[SETTINGS_SAFETY]->setRightLabelText(tr("On"));
    p_buttonGroup->addButton(p_selectSwitchButton[SETTINGS_SAFETY],SETTINGS_SAFETY);
    vLayoutFrameButton->addWidget(p_selectSwitchButton[SETTINGS_SAFETY]);

    p_selectSwitchButton[SETTINGS_OTHER] = ButtonFactory::
            getButton(tr("Other"),ButtonFactory::BUTTON_LABELLABEL,p_frameButton);
    p_selectSwitchButton[SETTINGS_OTHER]->setRightLabelText(tr("On"));
    p_buttonGroup->addButton(p_selectSwitchButton[SETTINGS_OTHER],SETTINGS_OTHER);
    vLayoutFrameButton->addWidget(p_selectSwitchButton[SETTINGS_OTHER]);

    insertWidget(p_frameButton);
    p_frameButton->hide();

    connect(p_buttonGroup,SIGNAL(buttonClicked(int)),this,SLOT(slotSelectSwitch(int)));
}

void WinSetParameter1::execute()
{
    //TODO
    qDebug() <<this->metaObject()->className() << __LINE__;
    WinInfoListDialog::instance()->showMsg(804);//Loading file from media.

    if(!WinSelectMemory::instance()->examineButtonMemoryIsExist(p_selectMemeoryButton))
    {
        WinInfoListDialog::instance()->showMsg(503);//Media not recognized.
        return;
    }

    QFile file(p_selectFileButton->getRightLabelText());
    if(!file.exists()){
        WinInfoListDialog::instance()->showMsg(570);//The specified file does not exist.
        return;
    }

//    if (!file.open(QIODevice::ReadOnly))
//    {
//        //TODO
//    }

//    if(sizeof(DEVICE_CONFIG) != file.readAll().size())
//    {
//        //TODO
//    }

    DEVICE_CONFIG *p_tmp = (DEVICE_CONFIG *)malloc(sizeof(DEVICE_CONFIG));

    FILE * fileFILE;
    fileFILE = fopen(p_selectFileButton->getRightLabelText().toLocal8Bit().data(),"r");
    if(fileFILE == NULL)
    {
        perror("fopen");
        return;
    }
    if(fread(p_tmp, 1, sizeof(DEVICE_CONFIG), fileFILE) <= 0)
    {
        perror("fread");
        return;
    }

    //516
    if(CfgConfig::instance()->examineMem(p_tmp))
    {
        //文件中数据代替结构体中数据

        memcpy(DevConfigPtr,p_tmp,sizeof(DEVICE_CONFIG));
    }
    free(p_tmp);
    WinInfoListDialog::instance()->showMsg(801);//Execution completed.
    return;
}

bool WinSetParameter1::isExecuteAble()
{
    //TODO depend on all button
    return WinSelectMemory::instance()->getMemoryState() &&
            (p_selectFileButton->getRightLabelText().compare(""));
}

void WinSetParameter1::slotSelectFile()
{
    WinSvLdFrame::slotSelectFile();

    DisplayData::instance()->setm_NameFilterType("GNL");
}

void WinSetParameter1::slotSelectSwitch(int settings_type)
{
    WinSelectSwitch::instance()->setGoalButton(p_selectSwitchButton[settings_type]);
    WinSelectSwitch::instance()->setHideWidget(p_frameButton);
    slidingWidget->setNextWidget(WinSelectSwitch::instance());
    slidingWidget->moveToNextWidget();
    winsaveload::setp_buttonExecuted(WinSelectSwitch::instance());
}


WinScaleImage1::WinScaleImage1(QWidget *parent)
    : WinSvLdFrame(tr("Scale image"),parent),currentWidget(-1)
{
    p_buttonGroup = new QButtonGroup(this);

    ButtonFactory *p_LoadImageButton = ButtonFactory::
            getButton(tr("Load image"),ButtonFactory::BUTTON_LABELPIXMAP,this);
    insertWidget(p_LoadImageButton);

    ButtonFactory *p_deleteImageButton = ButtonFactory::
            getButton(tr("Delete image"),ButtonFactory::BUTTON_LABELPIXMAP,this);
    insertWidget(p_deleteImageButton);

    p_buttonGroup->addButton(p_LoadImageButton,LOADIMAGE);
    p_buttonGroup->addButton(p_deleteImageButton,DELETEIMAGE);

    connect(p_buttonGroup,SIGNAL(buttonClicked(int)),this,SLOT(slotButtonClicked(int)));

    p_panel[0] = new WinLoadImage1;
    p_panel[1] = new WinDeleteImage1;
}

void WinScaleImage1::execute()
{
    qDebug() <<this->metaObject()->className() << __LINE__;
    //p_panel[currentWidget]->execute();
}

void WinScaleImage1::slotButtonClicked(int index)
{
    slidingWidget->setNextWidget(p_panel[index]);
    slidingWidget->moveToNextWidget();
    winsaveload::setp_buttonExecuted(p_panel[index]);
    currentWidget = index;
    WinSelectGroup::instance()->examineGroupDataIsExist();
}

WinLoadImage1::WinLoadImage1(QWidget *parent) : WinSvLdFrame(tr("Load image"),parent)
{
    insertSelectMemoryButton();

    insertSelectFileButton();
    connect(p_selectFileButton,SIGNAL(clicked(bool)),this,SLOT(slotSelectFile()));

    insertlabel();

    p_selectGroupButton[0] = ButtonFactory::
            getButton(tr("Group number"),ButtonFactory::BUTTON_LABELLABEL,this);
    p_selectGroupButton[0]->setRightLabelText("1");
    insertWidget(p_selectGroupButton[0]);
    connect(p_selectGroupButton[0],SIGNAL(clicked(bool)),this,SLOT(slotSelectgroup()));

    p_selectGroupButton[1] = ButtonFactory::
            getButton(tr("Data"),ButtonFactory::BUTTON_LABELLABELDISABLED,this);
    insertWidget(p_selectGroupButton[1]);
    p_selectGroupButton[1]->setRightLabelText(WinSelectGroup::string_fileNotExist);

    WinSelectGroup::instance()->setGoalButton(p_selectGroupButton[0], p_selectGroupButton[1]);
}

void WinLoadImage1::execute()
{
    qDebug() <<this->metaObject()->className() << __LINE__;

    //1.检查内存是否正常。
    if(!WinSelectMemory::instance()->examineButtonMemoryIsExist(p_selectMemeoryButton))
    {
        WinInfoListDialog::instance()->showMsg(503);//Media not recognized.
        return;
    }

    //2.检查标尺图像文件是否已经存在，若存在则弹出确认覆盖对话框。
    if(!p_selectGroupButton[1]->getRightLabelText().compare(WinSelectGroup::string_fileExist))
    {
        WinSureOperateDialog sureDialog(tr("Same file name exists, overwrite?"));
        if(QDialog::Rejected == sureDialog.exec())
        {
            return;
        }
    }

    //3.检查选择的文件是否还存在。
    if(!QFile(p_selectFileButton->getRightLabelText()).exists()/* || !QDir(WinSelectGroup::scaleImagePath).exists()*/)
    {
        WinInfoListDialog::instance()->showMsg(570);
        return;
    }

    WinInfoListDialog::instance()->showMsg(804);//Loading file from media
    QTimer::singleShot(1000, WinInfoListDialog::instance(), SLOT(close()));
    WinInfoListDialog::instance()->exec();

    int status = system(QString("cp ").append(p_selectFileButton->getRightLabelText()).append(" ")
                        .append(WinSelectGroup::string_ScaleImagePath)
                        .append(p_selectGroupButton[0]->getRightLabelText()).append(".png").toLocal8Bit().data());
    if((-1 == status) || WIFEXITED(status) == false || 0 != WEXITSTATUS(status) )
    {
        WinInfoListDialog::instance()->showMsg(518);//Scale image not loaded.
        return;
    }


    //重新检查对应组是否有数据,并更新按钮
    WinSelectGroup::instance()->examineGroupDataIsExist(0);

    WinInfoListDialog::instance()->showMsg(801);//Execution completed.
}

bool WinLoadImage1::isExecuteAble()
{
    return WinSelectMemory::instance()->getMemoryState() &&
            (p_selectFileButton->getRightLabelText().compare(""));
}

void WinLoadImage1::slotSelectFile()
{
    WinSvLdFrame::slotSelectFile();

    DisplayData::instance()->setm_NameFilterType("png");
}

//加载sd或者usb中的标尺图片文件到标尺图像存放位置
bool WinLoadImage1::slotStoreScaleImageFromMemory()
{
    //examine path and file exist.
    if(!QFile(p_selectFileButton->getRightLabelText()).exists() || !QDir(WinSelectGroup::string_ScaleImagePath).exists())
    {
        WinInformationDialog dialog(WinInformationDialog::string_fileNotExist);
        dialog.exec();
        return false;
    }
    int status = system(QString("cp ").append(p_selectFileButton->getRightLabelText()).append(" ")
                        .append(WinSelectGroup::string_ScaleImagePath)
                        .append(p_selectGroupButton[0]->getRightLabelText()).append(".png").toLocal8Bit().data());
    if((-1 == status) || WIFEXITED(status) == false || 0 != WEXITSTATUS(status) )
    {
        perror("cp:");
        return false;
    }
    return true;
}

WinDeleteImage1::WinDeleteImage1(QWidget *parent) : WinSvLdFrame(tr("Load image"),parent)
{
    p_selectGroupButton = ButtonFactory::
            getButton(tr("Group number"),ButtonFactory::BUTTON_LABELLABEL,this);
    p_selectGroupButton->setRightLabelText("1");
    insertWidget(p_selectGroupButton);
    connect(p_selectGroupButton,SIGNAL(clicked(bool)),this,SLOT(slotSelectgroup()));
    //connect(p_selectGroupButton,SIGNAL(),this,SLOT(slotSelectgroup()));

    p_Data = ButtonFactory::
            getButton(tr("Data"),ButtonFactory::BUTTON_LABELLABELDISABLED,this);
    insertWidget(p_Data);
    p_Data->setRightLabelText(WinSelectGroup::string_fileNotExist);

    WinSelectGroup::instance()->setGoalButton(p_selectGroupButton, p_Data);
}

//删除对应组的标尺图像
void WinDeleteImage1::execute(){
//    if(!WinSelectGroup::instance()->examineGroupDataIsExist(p_selectGroupButton->getRightLabelText().toInt()))
//    {
//        WinInformationDialog dialog(WinInformationDialog::string_fileNotExist);
//        dialog.exec();
//        return;
//    }

    WinSureOperateDialog dialog_sure(tr("Deleting scale image, execute?"));
    if(QDialog::Accepted == dialog_sure.exec()){
        int status = system(QString("rm ").append(WinSelectGroup::string_ScaleImagePath)
                            .append(p_selectGroupButton->getRightLabelText()).append(".png").toLocal8Bit().data());
        if((-1 == status) || WIFEXITED(status) == false || 0 != WEXITSTATUS(status) )
        {
            WinInfoListDialog::instance()->showMsg(572);//TODO: just guess

            perror("rm:");
            return;
        }

        WinSelectGroup::instance()->examineGroupDataIsExist(0);

        WinInfoListDialog::instance()->showMsg(801);//Execution completed.
    }
}

bool WinDeleteImage1::isExecuteAble()
{
    //数据有无按钮p_Data显示数据存在则可执行
    return WinSelectMemory::instance()->getMemoryState() &&
            (!p_Data->getRightLabelText().compare(WinSelectGroup::string_fileExist));
}

WinTrustedCerticicate1::WinTrustedCerticicate1(QWidget *parent)
    : WinSvLdFrame(tr("Trusted Certificate"),parent)
{
    p_buttonGroup = new QButtonGroup(this);

    ButtonFactory *p_LoadImageButton = ButtonFactory::
            getButton(tr("Load trusted certificate"),ButtonFactory::BUTTON_LABELPIXMAP,this);
    insertWidget(p_LoadImageButton);

    ButtonFactory *p_deleteImageButton = ButtonFactory::
            getButton(tr("Delete trusted certificate"),ButtonFactory::BUTTON_LABELPIXMAP,this);
    insertWidget(p_deleteImageButton);

    p_buttonGroup->addButton(p_LoadImageButton,LOAD_TRUSTED_CERTIFICATE);
    p_buttonGroup->addButton(p_deleteImageButton,DELETE_TRUSTED_CERTIFICATE);

    connect(p_buttonGroup,SIGNAL(buttonClicked(int)),this,SLOT(slotButtonClicked(int)));

    p_panel[0] = new WinLoadTrustedCerticicate1;
    p_panel[1] = new WinDeleteTrustedCerticicate1;
}

void WinTrustedCerticicate1::execute()
{
    qDebug() <<this->metaObject()->className() << __LINE__;
    //p_panel[currentWidget]->execute();
}

void WinTrustedCerticicate1::slotButtonClicked(int index)
{
    slidingWidget->setNextWidget(p_panel[index]);
    slidingWidget->moveToNextWidget();
    winsaveload::setp_buttonExecuted(p_panel[index]);
    currentWidget = index;
}

WinLoadTrustedCerticicate1::WinLoadTrustedCerticicate1(QWidget *parent)
    : WinSvLdFrame(tr("Load Trusted Certificate"),parent)
{
    p_buttonGroup = new QButtonGroup(this);

    insertSelectMemoryButton();

    insertSelectFileButton();
    connect(p_selectFileButton,SIGNAL(clicked(bool)),this,SLOT(slotSelectFile()));

    insertlabel(tr("Certification Information"));

    p_InformationButton[ENUM_ISSUER] = ButtonFactory::
            getButton(tr("Issuer"),ButtonFactory::BUTTON_LABELLABEL,this);
    p_InformationButton[ENUM_SUBJECT] = ButtonFactory::
            getButton(tr("Subject"),ButtonFactory::BUTTON_LABELLABEL,this);
    p_InformationButton[ENUM_VALIDNOTBEFORE] = ButtonFactory::
            getButton(tr("Valid not before"),ButtonFactory::BUTTON_LABELLABEL,this);
    p_InformationButton[ENUM_VALIDNOTAFTER] = ButtonFactory::
            getButton(tr("Valid not after"),ButtonFactory::BUTTON_LABELLABEL,this);
    p_InformationButton[ENUM_HASHVALUEFIRSTHALF] = ButtonFactory::
            getButton(tr("Hash value(SHA1) first half"),ButtonFactory::BUTTON_LABELLABEL,this);
    p_InformationButton[ENUM_HASHVALUESECONDHALF] = ButtonFactory::
            getButton(tr("Hash value(SHA1) second half"),ButtonFactory::BUTTON_LABELLABEL,this);

    for(int i = 0; i < 6; i++)
    {
        insertWidget(p_InformationButton[i]);
        p_buttonGroup->addButton(p_InformationButton[i],i);
    }
}

void WinLoadTrustedCerticicate1::execute()
{
    //检查内存是否正常。
    if(!WinSelectMemory::instance()->examineButtonMemoryIsExist(p_selectMemeoryButton))
    {
        WinInfoListDialog::instance()->showMsg(503);//Media not recognized.
        return;
    }

    //检查选择的文件是否还存在。
    if(!QFile(p_selectFileButton->getRightLabelText()).exists()/* || !QDir(WinSelectGroup::scaleImagePath).exists()*/)
    {
        WinInfoListDialog::instance()->showMsg(570);
        return;
    }

    WinInfoListDialog::instance()->showMsg(804);
    QTimer::singleShot(1000, WinInfoListDialog::instance(), SLOT(close()));
    WinInfoListDialog::instance()->exec();

    int status = system(QString("cp  ").append(p_selectFileButton->getRightLabelText())
                        .append("  ")
                        .append(DisplayData_DeleteCertificate::instance()->getNextStoreFilePath())
                        .toLocal8Bit().data()   );

    if((-1 == status) || WIFEXITED(status) == false || 0 != WEXITSTATUS(status) )
    {
        WinInfoListDialog::instance()->showMsg(572);//Deletion of the file went wrong.//TODO: just guess

        perror("cp:");
        return;
    }

    WinInfoListDialog::instance()->showMsg(801);//Execution completed.
}

bool WinLoadTrustedCerticicate1::isExecuteAble()
{
    return WinSelectMemory::instance()->getMemoryState() &&
            (p_selectFileButton->getRightLabelText().compare(""));
}

void WinLoadTrustedCerticicate1::slotSelectFile()
{
    //TODO: DisplayData::instance()->setNameFilter("png");

    DisplayData_LoadCertificate::instance()->setP_goalButton(p_selectFileButton);
    if(DisplayData_LoadCertificate::instance()->setFileSystemBYButton(p_selectMemeoryButton) == false)
        return;
    DisplayData_LoadCertificate::instance()->show();
}

WinDeleteTrustedCerticicate1::WinDeleteTrustedCerticicate1(QWidget *parent)
    : WinSvLdFrame(tr("Delete Trusted Certificate"),parent)
{
    p_buttonGroup = new QButtonGroup(this);

    insertSelectFileButton();
    connect(p_selectFileButton,SIGNAL(clicked(bool)),this,SLOT(slotSelectFile()));

    insertlabel(tr("Certification Information"));

    p_InformationButton[ENUM_ISSUER] = ButtonFactory::
            getButton(tr("Issuer"),ButtonFactory::BUTTON_LABELLABEL,this);
    p_InformationButton[ENUM_SUBJECT] = ButtonFactory::
            getButton(tr("Subject"),ButtonFactory::BUTTON_LABELLABEL,this);
    p_InformationButton[ENUM_VALIDNOTBEFORE] = ButtonFactory::
            getButton(tr("Valid not before"),ButtonFactory::BUTTON_LABELLABEL,this);
    p_InformationButton[ENUM_VALIDNOTAFTER] = ButtonFactory::
            getButton(tr("Valid not after"),ButtonFactory::BUTTON_LABELLABEL,this);
    p_InformationButton[ENUM_HASHVALUEFIRSTHALF] = ButtonFactory::
            getButton(tr("Hash value(SHA1) first half"),ButtonFactory::BUTTON_LABELLABEL,this);
    p_InformationButton[ENUM_HASHVALUESECONDHALF] = ButtonFactory::
            getButton(tr("Hash value(SHA1) second half"),ButtonFactory::BUTTON_LABELLABEL,this);

    for(int i = 0; i < 6; i++)
    {
        insertWidget(p_InformationButton[i]);
        p_buttonGroup->addButton(p_InformationButton[i],i);
    }
}

void WinDeleteTrustedCerticicate1::execute()
{
    //检查选择的文件是否还存在。
    if(!QFile(DisplayData_DeleteCertificate::string_CertificatePath + p_selectFileButton->getRightLabelText()).exists()/* || !QDir(WinSelectGroup::scaleImagePath).exists()*/)
    {
        //TODO : is it  necessary?
        WinInfoListDialog::instance()->showMsg(570);//file Not exist
        return;
    }


    WinSureOperateDialog dialog_sure(tr("Deleting the file ?"));
    if(QDialog::Rejected == dialog_sure.exec())
    {
        return;
    }
    int status = system(QString("rm  ").append(DisplayData_DeleteCertificate::string_CertificatePath)
                        .append(p_selectFileButton->getRightLabelText()).toLocal8Bit().data());

    if((-1 == status) || WIFEXITED(status) == false || 0 != WEXITSTATUS(status) )
    {
        WinInfoListDialog::instance()->showMsg(572);//Deletion of the file went wrong.//TODO: just guess

        perror("rm:");
        return;
    }

    WinInfoListDialog::instance()->showMsg(801);//Execution completed.
}

bool WinDeleteTrustedCerticicate1::isExecuteAble()
{
    return p_selectFileButton->getRightLabelText().compare("");
}

void WinDeleteTrustedCerticicate1::slotSelectFile()
{
    //DisplayData::instance()->
    ButtonFactory *p_selectButton = static_cast<ButtonFactory *> (sender());
    DisplayData_DeleteCertificate::instance()->setP_goalButton(p_selectButton);
    //TODO: DisplayData::instance()->setNameFilter("png");
    DisplayData_DeleteCertificate::instance()->show();
}

WinReportTemplate1::WinReportTemplate1(QWidget *parent) : WinSvLdFrame(tr("Report template"),parent)
{
    insertSelectMemoryButton();

    ButtonFactory *p_selectFileType = ButtonFactory::
            getButton(tr("File type"),ButtonFactory::BUTTON_LABELLABEL,this);
    p_selectFileType->setRightLabelText(WinSelectFileType::string_excel);
    insertWidget(p_selectFileType);
    connect(p_selectFileType,SIGNAL(clicked(bool)),this,SLOT(slotSelectFileType()));

    insertSelectFileButton();
    connect(p_selectFileButton,SIGNAL(clicked(bool)),this,SLOT(slotSelectFile()));

    insertlabel();

    ButtonFactory *p_selectReportType = ButtonFactory::
            getButton(tr("Report type"),ButtonFactory::BUTTON_LABELLABEL,this);
    p_selectReportType->setRightLabelText(tr("Hourly"));
    insertWidget(p_selectReportType);
    connect(p_selectReportType,SIGNAL(clicked(bool)),this,SLOT(slotSelectReportType()));

    p_databutton = ButtonFactory::
            getButton(tr("Data"),ButtonFactory::BUTTON_LABELLABELDISABLED,this);
    insertWidget(p_databutton);
    p_databutton->setRightLabelText(WinSelectGroup::string_fileNotExist);
}

void WinReportTemplate1::execute(){
    qDebug() <<this->metaObject()->className() << __LINE__;


    //1.检查内存是否正常。
    if(!WinSelectMemory::instance()->examineButtonMemoryIsExist(p_selectMemeoryButton))
    {
        WinInfoListDialog::instance()->showMsg(503);//Media not recognized.
        return;
    }

    //2.检查选择的文件是否还存在。
    if(!QFile(p_selectFileButton->getRightLabelText()).exists()/* || !QDir(WinSelectGroup::scaleImagePath).exists()*/)
    {
        WinInfoListDialog::instance()->showMsg(570);

        return;
    }

    //TODO

    WinInfoListDialog::instance()->showMsg(801);//Execution completed.
}

bool WinReportTemplate1::isExecuteAble()
{
    return WinSelectMemory::instance()->getMemoryState() &&
            (p_selectFileButton->getRightLabelText().compare(""));
}

void WinReportTemplate1::slotSelectFileType()
{
    ButtonFactory *buttonSender = static_cast<ButtonFactory *> (sender());
    WinSelectFileType::instance()->setGoalButton(buttonSender);

    slidingWidget->setNextWidget(WinSelectFileType::instance());
    slidingWidget->moveToNextWidget();
    winsaveload::setp_buttonExecuted(WinSelectFileType::instance());
}

void WinReportTemplate1::slotSelectFile()
{
    WinSvLdFrame::slotSelectFile();
    DisplayData::instance()->setm_NameFilterType("*");
}

void WinReportTemplate1::slotSelectReportType()
{
    ButtonFactory *p_selectButton = static_cast<ButtonFactory *> (sender());
    WinSelectReportType::instance()->setGoalButton(p_selectButton);
    slidingWidget->setNextWidget(WinSelectReportType::instance());
    slidingWidget->moveToNextWidget();
    winsaveload::setp_buttonExecuted(WinSelectReportType::instance());
}

WinAboveAll1::WinAboveAll1(QWidget *parent) : WinSvLdFrame(tr("Above all"),parent)
{
    insertSelectMemoryButton();
    //重新连接选择文件按钮的槽函数
    disconnect(p_selectMemeoryButton, SIGNAL(clicked(bool)), this, SLOT(slotSelectMemory()));
    connect(p_selectMemeoryButton, SIGNAL(clicked(bool)), this, SLOT(slotSelectMemory()));

    p_selectFolderButton = ButtonFactory::
            getButton(tr("Select folder"),ButtonFactory::BUTTON_LABELLABEL,this);
    insertWidget(p_selectFolderButton);
    connect(p_selectFolderButton,SIGNAL(clicked(bool)),this,SLOT(slotSelectFolder()));
}

void WinAboveAll1::execute(){
    qDebug() <<this->metaObject()->className() << __LINE__;

    //读取文件的条件取决于设定参数、标尺图像和报表模板的各设定内容。请在执行前确认各设定的内容。
    //just guest：从选择的文件夹中得到上面所有选项的文件并一一执行
    //TODO: unknow function

    //1.检查内存是否正常。
    if(!WinSelectMemory::instance()->examineButtonMemoryIsExist(p_selectMemeoryButton))
    {
        WinInfoListDialog::instance()->showMsg(503);//Media not recognized.
        return;
    }

    //2.检查选择的Folder是否还存在。
    if(!QFile(p_selectFolderButton->getRightLabelText()).exists()/* || !QDir(WinSelectGroup::scaleImagePath).exists()*/)
    {
        WinInfoListDialog::instance()->showMsg(570);
        return;
    }

    WinInfoListDialog::instance()->showMsg(801);//Execution completed.
}

bool WinAboveAll1::isExecuteAble()
{
    return WinSelectMemory::instance()->getMemoryState() &&
            (p_selectFolderButton->getRightLabelText().compare(""));
}

void WinAboveAll1::slotSelectMemory()
{
    WinSelectMemory::instance()->setGoalButton(p_selectMemeoryButton,p_selectFolderButton);
    WinSelectMemory::instance()->examineSetAllButton();

    slidingWidget->setNextWidget(WinSelectMemory::instance());
    slidingWidget->moveToNextWidget();
    winsaveload::setp_buttonExecuted(WinSelectMemory::instance()/*->getP_executeButton()*/);
}

void WinAboveAll1::slotSelectFolder()
{
    ButtonFactory *p_selectButton = static_cast<ButtonFactory *> (sender());
    DisplayData_Folder::instance()->setP_goalButton(p_selectButton);

    if(DisplayData_Folder::instance()->setFileSystemBYButton(p_selectMemeoryButton) == false)
        return;
    DisplayData_Folder::instance()->show();
}



WinSaveSettingsData::WinSaveSettingsData(QWidget *parent):
    WinSvLdFrame(tr("Save settings data"), parent, false)
{
    QButtonGroup *p_buttonGroup = new QButtonGroup(this);

    ButtonFactory *p_button[5];
    p_button[SETTINGS_SET_PARAMETER] = ButtonFactory::
            getButton(tr("Set parement"),ButtonFactory::BUTTON_LABELPIXMAP,this);

    p_button[SETTINGS_SCALE_IMAGE] = ButtonFactory::
            getButton(tr("Scale image"),ButtonFactory::BUTTON_LABELPIXMAP,this);

    p_button[SETTINGS_REPORT_TEMPLATE] = ButtonFactory::
            getButton(tr("Report template"),ButtonFactory::BUTTON_LABELPIXMAP,this);


    p_button[SETTINGS_TRUSTED_CERTIFICATE] = ButtonFactory::
            getButton(tr("Trusted Certificate"),ButtonFactory::BUTTON_LABELPIXMAP,this);


    p_button[SETTINGS_ALL] = ButtonFactory::
            getButton(tr("Above all"),ButtonFactory::BUTTON_LABELPIXMAP,this);

    for(int i = 0; i < 5; i++)
    {
        insertWidget(p_button[i]);
        p_buttonGroup->addButton(p_button[i],i);
    }
    connect(p_buttonGroup,SIGNAL(buttonClicked(int)),this,SLOT(slotButtonClicked(int)));

    p_panel[SETTINGS_SET_PARAMETER] = new WinSetParameter_save1;
    p_panel[SETTINGS_SCALE_IMAGE] = new WinScaleImage_save1;
    p_panel[SETTINGS_REPORT_TEMPLATE] = new WinReportTemplate_save1;
    p_panel[SETTINGS_TRUSTED_CERTIFICATE] = new WinTrustedCertificate_save1;
    p_panel[SETTINGS_ALL] = new WinAboveAll_save1;
}

void WinSaveSettingsData::execute()
{
    //p_panel[iCurrentIndex]->execute();
}

void WinSaveSettingsData::slotButtonClicked(int index)
{
    if(index == SETTINGS_TRUSTED_CERTIFICATE)
    {
        //当选择可信赖证书时检查证书是否存在
        //TODO:是否需要在基类中增加一个初始化各界面的接口使,因为下面的语句需要知道函数实现细节
        static_cast<WinTrustedCertificate_save1*> (p_panel[SETTINGS_TRUSTED_CERTIFICATE])->examineDataButtonIsExist();
    }



    iCurrentIndex = index;
    slidingWidget->setNextWidget(p_panel[index]);
    slidingWidget->moveToNextWidget();
    winsaveload::setp_buttonExecuted(p_panel[index]);
}

WinSetParameter_save1::WinSetParameter_save1(QWidget *parent) :
    WinSvLdFrame("Set parameter", parent)
{
    uint_InputID = ID_SAVELOAD_SAVE_PAREMENTER_NAME;

    insertSelectMemoryButton();

    p_FileButton = ButtonFactory::
            getButton(tr("File name"),ButtonFactory::BUTTON_LABELLABEL,this);
    insertWidget(p_FileButton);
    connect(p_FileButton,SIGNAL(clicked(bool)),this,SLOT(slotInputFileName()));

    p_commentButton = ButtonFactory::
            getButton(tr("Comment"),ButtonFactory::BUTTON_LABELLABEL,this);
    insertWidget(p_commentButton);
    connect(p_commentButton,SIGNAL(clicked(bool)),this,SLOT(slotInputComment()));
}

void WinSetParameter_save1::execute()
{
    qDebug() <<this->metaObject()->className() << __LINE__;

    //1.检查内存是否正常。
    if(!WinSelectMemory::instance()->examineButtonMemoryIsExist(p_selectMemeoryButton))
    {
        WinInfoListDialog::instance()->showMsg(503);//Media not recognized.
        return;
    }

    QString tmp_path = "";
    if(!p_selectMemeoryButton->getRightLabelText().compare(WinSelectMemory::SD)){
        tmp_path.append(WinSelectMemory::SD_PATH);
    }else if(!p_selectMemeoryButton->getRightLabelText().compare(WinSelectMemory::USB)){
        tmp_path.append(WinSelectMemory::USB_PATH);
    }
    tmp_path.append(p_FileButton->getRightLabelText()).append(".GNL");
    QFile file(tmp_path);
    if(file.exists()){
        WinSureOperateDialog dialog(tr("The file is exist.Do you want to restore it"));
        if(QDialog::Rejected == dialog.exec())
        {
            return;
        }
    }

    if (!file.open(QIODevice::WriteOnly))//TODO: 如果打开失败怎么办
        return;
    QDataStream out(&file);

    WinInfoListDialog::instance()->showMsg(803);//Saving data to media.

    qint8 *data = (qint8 *)DevConfigPtr;
    for(uint i = 0; i < sizeof(*DevConfigPtr)/sizeof(qint8); i++){
        out << data[i];
        //qDebug() << data[i];
    }
    CfgConfig::instance()->storeDeviceConfig1();

    WinInfoListDialog::instance()->showMsg(801);//Execution completed.
}

bool WinSetParameter_save1::isExecuteAble()
{
    return WinSelectMemory::instance()->getMemoryState() &&
            (p_FileButton->getRightLabelText().compare("")) /*&&
             (p_commentButton->getRightLabelText().compare(""))*/;
}

void WinSetParameter_save1::slotStoreDataToFile()
{
    //USELESS
    if(p_FileButton->getRightLabelText() == NULL)
        return;
    QString tmp_path = "";
    if(!p_selectMemeoryButton->getRightLabelText().compare(WinSelectMemory::SD)){
        tmp_path.append(WinSelectMemory::SD_PATH);
    }else if(!p_selectMemeoryButton->getRightLabelText().compare(WinSelectMemory::USB)){
        tmp_path.append(WinSelectMemory::USB_PATH);
    }
    tmp_path.append(p_FileButton->getRightLabelText()).append(".GNL");

    QFile fileStore(tmp_path);
    if (!fileStore.open(QIODevice::WriteOnly))
        return;
    QDataStream out(&fileStore);

    WinInformationDialog dialogStoring(tr("The file is storing."));
    dialogStoring.exec();
    qint8 *data = (qint8 *)DevConfigPtr;
    for(uint i = 0; i < sizeof(*DevConfigPtr)/sizeof(qint8); i++){
        out << data[i];
        //qDebug() << data[i];
    }
    CfgConfig::instance()->storeDeviceConfig1();
}

void WinSetParameter_save1::slotInputFileName()
{
    uint_InputID = ID_SAVELOAD_SAVE_PAREMENTER_NAME;
    slotInputText();
}

void WinSetParameter_save1::slotInputComment()
{
    uint_InputID = ID_SAVELOAD_SAVE_PAREMENTER_COMMENT;
    slotInputText();
}

WinScaleImage_save1::WinScaleImage_save1(QWidget *parent) : WinSvLdFrame(tr("Scale image"),parent)
{
    uint_InputID = ID_SAVELOAD_SAVE_SCALE_NAME;

    insertSelectMemoryButton();

    p_inputFileNameButton = ButtonFactory::
            getButton(tr("File name"),ButtonFactory::BUTTON_LABELLABEL,this);
    insertWidget(p_inputFileNameButton);
    connect(p_inputFileNameButton,SIGNAL(clicked(bool)),this,SLOT(slotInputText()));

    insertlabel();

    p_selectGroupButton = ButtonFactory::
            getButton(tr("Group number"),ButtonFactory::BUTTON_LABELLABEL,this);
    p_selectGroupButton->setRightLabelText("1");
    insertWidget(p_selectGroupButton);
    connect(p_selectGroupButton,SIGNAL(clicked(bool)),this,SLOT(slotSelectgroup()));

    p_DataButton = ButtonFactory::
            getButton(tr("Data"),ButtonFactory::BUTTON_LABELLABELDISABLED,this);
    insertWidget(p_DataButton);
    p_DataButton->setRightLabelText(WinSelectGroup::string_fileNotExist);

    WinSelectGroup::instance()->setGoalButton(p_selectGroupButton, p_DataButton);
}

//保存相应组的标尺图像到媒体设备中
void WinScaleImage_save1::execute()
{
    qDebug() <<this->metaObject()->className() << __LINE__;

    //1.检查内存是否正常。
    if(!WinSelectMemory::instance()->examineButtonMemoryIsExist(p_selectMemeoryButton))
    {
        WinInfoListDialog::instance()->showMsg(503);//Media not recognized.
        return;
    }

    //2.检查输入的文件名在文件系统中是否已经存在，若存在弹出确认框，确认是否覆盖
    QString storePath;//store file path and name
    if(0 == p_selectMemeoryButton->getRightLabelText().compare(WinSelectMemory::SD))
    {
        storePath = WinSelectMemory::SD_PATH + p_inputFileNameButton->getRightLabelText() + ".png";
    }else if(0 == p_selectMemeoryButton->getRightLabelText().compare(WinSelectMemory::USB))
    {
        storePath = WinSelectMemory::USB_PATH + p_inputFileNameButton->getRightLabelText() + ".png";
    }
    if(QFile(storePath).exists())    {
        WinSureOperateDialog sureDialog(tr("Same file name exists, overwrite?"));
        if(sureDialog.exec() == QDialog::Rejected)
        {
            return;
        }
    }

    WinInfoListDialog::instance()->showMsg(803);//Saving data to media.
    QTimer::singleShot(1000, WinInfoListDialog::instance(),SLOT(close()));
    WinInfoListDialog::instance()->exec();

    //拷贝相应存在标尺图像的组的标尺图像到SD或USB中
    int status = system(QString("cp ").append(WinSelectGroup::string_ScaleImagePath)
                        .append(p_selectGroupButton->getRightLabelText()).append(".png  ")
                        .append(p_inputFileNameButton->getRightLabelText()).toLocal8Bit().data());
    if((-1 == status) || WIFEXITED(status) == false || 0 != WEXITSTATUS(status) )
    {
        //TODO: cp defealt
        return;
    }
    WinInfoListDialog::instance()->showMsg(801);//Execution completed.
}

bool WinScaleImage_save1::isExecuteAble()
{
    return WinSelectMemory::instance()->getMemoryState() &&
            (p_inputFileNameButton->getRightLabelText().compare("")) &&
             (!p_DataButton->getRightLabelText().compare(WinSelectGroup::string_fileExist));
}

WinReportTemplate_save1::WinReportTemplate_save1(QWidget *parent) : WinSvLdFrame(tr("Report template"),parent)
{
    uint_InputID = ID_SAVELOAD_SAVE_REPORT_NAME;

    insertSelectMemoryButton();

    ButtonFactory *p_selectFileType = ButtonFactory::
            getButton(tr("File type"),ButtonFactory::BUTTON_LABELLABEL,this);
    p_selectFileType->setRightLabelText(WinSelectFileType::string_excel);
    insertWidget(p_selectFileType);
    connect(p_selectFileType,SIGNAL(clicked(bool)),this,SLOT(slotSelectFileType()));

    p_inputFileNameButton = ButtonFactory::
            getButton(tr("File name"),ButtonFactory::BUTTON_LABELLABEL,this);
    insertWidget(p_inputFileNameButton);
    connect(p_inputFileNameButton,SIGNAL(clicked(bool)),this,SLOT(slotInputText()));

    insertlabel();

    ButtonFactory *p_selectReportType = ButtonFactory::
            getButton(tr("Report type"),ButtonFactory::BUTTON_LABELLABEL,this);
    p_selectReportType->setRightLabelText(tr("Hourly"));
    insertWidget(p_selectReportType);
    connect(p_selectReportType,SIGNAL(clicked(bool)),this,SLOT(slotSelectReportType()));

    p_databutton = ButtonFactory::
            getButton(tr("Data"),ButtonFactory::BUTTON_LABELLABELDISABLED,this);
    insertWidget(p_databutton);
    p_databutton->setRightLabelText(WinSelectGroup::string_fileNotExist);
}

void WinReportTemplate_save1::execute()
{
    qDebug() <<this->metaObject()->className() << __LINE__;

    //1.检查内存是否正常。
    if(!WinSelectMemory::instance()->examineButtonMemoryIsExist(p_selectMemeoryButton))
    {
        WinInfoListDialog::instance()->showMsg(503);//Media not recognized.
        return;
    }

    //2.检查输入的文件名在文件系统中是否已经存在，若存在弹出确认框，确认是否覆盖
    QString storePath = "";//store file path and name
    if(0 == p_selectMemeoryButton->getRightLabelText().compare(WinSelectMemory::SD))
    {
        storePath = WinSelectMemory::SD_PATH + p_inputFileNameButton->getRightLabelText();
    }else if(0 == p_selectMemeoryButton->getRightLabelText().compare(WinSelectMemory::USB))
    {
        storePath = WinSelectMemory::USB_PATH + p_inputFileNameButton->getRightLabelText();
    }

    if(0 == p_selectMemeoryButton->getRightLabelText().compare(WinSelectFileType::string_excel))
    {
        //TODO: 文件后缀是否是这些
        storePath.append(".xlsx");
    }else if(0 == p_selectMemeoryButton->getRightLabelText().compare(WinSelectFileType::string_pdf))
    {
        storePath.append(".pdf");
    }
    if(QFile(storePath).exists())    {
        WinSureOperateDialog sureDialog(tr("Same file name exists, overwrite?"));
        if(sureDialog.exec() == QDialog::Rejected)
        {
            return;
        }
    }

    //TODO
    WinInfoListDialog::instance()->showMsg(801);//Execution completed.
}

bool WinReportTemplate_save1::isExecuteAble()
{
    return WinSelectMemory::instance()->getMemoryState() &&
            (p_inputFileNameButton->getRightLabelText().compare("")) &&
             (!p_databutton->getRightLabelText().compare(WinSelectGroup::string_fileExist));
}

void WinReportTemplate_save1::slotSelectFileType()
{
    ButtonFactory *buttonSender = static_cast<ButtonFactory *> (sender());
    WinSelectFileType::instance()->setGoalButton(buttonSender);

    slidingWidget->setNextWidget(WinSelectFileType::instance());
    slidingWidget->moveToNextWidget();
    winsaveload::setp_buttonExecuted(WinSelectFileType::instance());
}

void WinReportTemplate_save1::slotSelectReportType()
{
    ButtonFactory *p_selectButton = static_cast<ButtonFactory *> (sender());
    WinSelectReportType::instance()->setGoalButton(p_selectButton);
    slidingWidget->setNextWidget(WinSelectReportType::instance());
    slidingWidget->moveToNextWidget();
    winsaveload::setp_buttonExecuted(WinSelectReportType::instance());
}

WinTrustedCertificate_save1::WinTrustedCertificate_save1(QWidget *parent)
    : WinSvLdFrame(tr("Trusted Certificate"),parent)
{
    insertSelectMemoryButton();
    p_haveDataButton = ButtonFactory::
            getButton(tr("Certificate"),ButtonFactory::BUTTON_LABELLABELDISABLED,this);
    p_haveDataButton->setRightLabelText(WinSelectGroup::string_fileNotExist);
    insertWidget(p_haveDataButton);
}

void WinTrustedCertificate_save1::examineDataButtonIsExist()
{
    p_haveDataButton->setRightLabelText(DisplayData_DeleteCertificate::getCertificateIsExist()?
                                            WinSelectGroup::string_fileExist: WinSelectGroup::string_fileNotExist);
}

void WinTrustedCertificate_save1::execute()
{
    qDebug() <<this->metaObject()->className() << __LINE__;

    //1.检查内存是否正常。
    if(!WinSelectMemory::instance()->examineButtonMemoryIsExist(p_selectMemeoryButton))
    {
        WinInfoListDialog::instance()->showMsg(503);//Media not recognized.
        return;
    }

    WinInfoListDialog::instance()->showMsg(803);//Saving data to media.
    QTimer::singleShot(1000, WinInfoListDialog::instance(),SLOT(close()));
    WinInfoListDialog::instance()->exec();

    QString storePath;//store file path and name
    if(0 == p_selectMemeoryButton->getRightLabelText().compare(WinSelectMemory::SD))
    {
        storePath = WinSelectMemory::SD_PATH;
    }else if(0 == p_selectMemeoryButton->getRightLabelText().compare(WinSelectMemory::USB))
    {
        storePath = WinSelectMemory::USB_PATH;
    }

    storePath += QDateTime::currentDateTime().toString("yyMMdd_hhmmss_");

    QStringList filters;
    filters <<"*.cer";
    QStringList allEntry = QDir(DisplayData_DeleteCertificate::string_CertificatePath).entryList(filters);
    foreach (QString var, allEntry) {
        qDebug() << var;
        int status = system(QString("cp ").append(DisplayData_DeleteCertificate::string_CertificatePath)
                            .append(var).append( "  ")
                            .append(storePath).append(var)
                            .toLocal8Bit().data());
        if((-1 == status) || WIFEXITED(status) == false || 0 != WEXITSTATUS(status) )
        {
            //TODO: cp defealt
            return;
        }
    }



    WinInfoListDialog::instance()->showMsg(801);//Execution completed.
}

bool WinTrustedCertificate_save1::isExecuteAble()
{
    return WinSelectMemory::instance()->getMemoryState() &&
             (!p_haveDataButton->getRightLabelText().compare(WinSelectGroup::string_fileExist));
}

WinAboveAll_save1::WinAboveAll_save1(QWidget *parent) : WinSvLdFrame(tr("Above all"),parent)
{
    uint_InputID = ID_SAVELOAD_SAVE_ALL_NAME;

    insertSelectMemoryButton();

    p_inputTextButton = ButtonFactory::
            getButton(tr("Folder name"),ButtonFactory::BUTTON_LABELLABEL,this);
    insertWidget(p_inputTextButton);
    connect(p_inputTextButton,SIGNAL(clicked(bool)),this,SLOT(slotInputText()));
}

void WinAboveAll_save1::execute(){
    qDebug() <<this->metaObject()->className() << __LINE__;


    //检查内存是否正常。
    if(!WinSelectMemory::instance()->examineButtonMemoryIsExist(p_selectMemeoryButton))
    {
        WinInfoListDialog::instance()->showMsg(503);//Media not recognized.
        return;
    }

    QString path_AboveAll = "";
    if(!p_selectMemeoryButton->getRightLabelText().compare(WinSelectMemory::SD)){
        path_AboveAll.append(WinSelectMemory::SD_PATH);
    }else if(!p_selectMemeoryButton->getRightLabelText().compare(WinSelectMemory::USB)){
        path_AboveAll.append(WinSelectMemory::USB_PATH);
    }
    path_AboveAll.append(p_inputTextButton->getRightLabelText());

    //判断路径是否已近存在，若存在则确认是否继续执行，不存咋则创建目录
    QDir dir(path_AboveAll);
    if(dir.exists()){
        WinSureOperateDialog dialog_sure(tr("Same folder name exists overwrite ?"));
        if(QDialog::Rejected == dialog_sure.exec())
        {
            return;
        }
    }else
    {
        qDebug() << dir.mkpath(path_AboveAll);
    }
    path_AboveAll.append("/");

    WinInfoListDialog::instance()->showMsg(803);//Saving data to media.
    QTimer::singleShot(1000, WinInfoListDialog::instance(), SLOT(close()));
    WinInfoListDialog::instance()->exec();

    //1.保存设置参数,把结构体中数据保存在文件夹下的Config.GNL中
    QFile setParameterFile(path_AboveAll + "Config.GNL");
    if (!setParameterFile.open(QIODevice::WriteOnly))//TODO: 如果打开失败怎么办
        return;
    QDataStream setParameterOut(&setParameterFile);

    qint8 *data = (qint8 *)DevConfigPtr;
    for(uint i = 0; i < sizeof(*DevConfigPtr)/sizeof(qint8); i++){
        setParameterOut << data[i];
    }

    //2.保存标尺图像，把所有存在标尺图像的组的标尺图像存放在文件夹中并以ScaleImage??.png命名(??是组号码)
    QDir scaleImageDir(WinSelectGroup::string_ScaleImagePath);
    QStringList scaleImageFileList = scaleImageDir.entryList(QStringList("*.png"));
    foreach (QString var, scaleImageFileList) {
        QString tmp_filePath = path_AboveAll + "ScaleImage";
        if(var.size() == 5)
            tmp_filePath.append("0");
        tmp_filePath.append(var);

        int status = system(QString("cp ").append(WinSelectGroup::string_ScaleImagePath).append(var)
                            .append("  ").append(tmp_filePath).toLocal8Bit().data());
        if((-1 == status) || WIFEXITED(status) == false || 0 != WEXITSTATUS(status) )
        {
            //TODO: cp defealt ?出错之后是否需要继续保存其他数据?
            return;
        }

    }
    //3.TODO： 保存报表模板

    //4.保存可信赖证书，将客户端的信赖证书文件和服务器的信赖证书文件复制到文件夹下的Client和Server中
    //4.1:保存客户端的信赖证书文件
    QString path_certificateClient = path_AboveAll + "Client/";
    if(!QDir(path_certificateClient).exists())
    {
        QDir(path_certificateClient).mkpath(path_certificateClient);
    }

    path_certificateClient += QDateTime::currentDateTime().toString("yyMMdd_hhmmss_");

    QStringList filters;
    filters <<"*.cer";
    QStringList allEntry = QDir(DisplayData_DeleteCertificate::string_CertificatePath).entryList(filters);
    foreach (QString var, allEntry) {
        qDebug() << var;
        int status = system(QString("cp ").append(DisplayData_DeleteCertificate::string_CertificatePath)
                            .append(var).append( "  ")
                            .append(path_certificateClient).append(var)
                            .toLocal8Bit().data());
        if((-1 == status) || WIFEXITED(status) == false || 0 != WEXITSTATUS(status) )
        {
            //TODO: cp defealt ?出错之后是否需要继续保存其他数据?
            return;
        }
    }
    //4.2: TODO:保存服务器证书到文件夹下Server中

    WinInfoListDialog::instance()->showMsg(801);//Execution completed.
}

bool WinAboveAll_save1::isExecuteAble()
{
    return WinSelectMemory::instance()->getMemoryState() &&
            (p_inputTextButton->getRightLabelText().compare(""));
}



/**
 * 功能：
 *      选择文件系统的单例
 * 参数：
 *
 * 返回值：
 *
 */
QString WinSelectMemory::SD = QString("SD");
QString WinSelectMemory::SD_PATH = QString(tr("/home/yange/sd/"));
QString WinSelectMemory::USB = QString("Usb");
QString WinSelectMemory::USB_PATH = QString(tr("/home/yange/usb/"));
QString WinSelectMemory::STRINGNOSD_NOUSB = QString(tr("Media is not recognized"));

WinSelectMemory *WinSelectMemory::instance()
{
    static WinSelectMemory Instance;
    return &Instance;
}

void WinSelectMemory::setGoalButton(ButtonFactory *b, ButtonFactory *b2)
{
    p_goalButton = b;
    p_attachButton = b2;
}

//增加一个选择内存的按钮到m_hadSetButton
void WinSelectMemory::insertButtonToList(ButtonFactory *b)
{
    if(b != NULL)
        m_hadSetButton.append(b);
}


//当没有sd和usb的时候返回0
WinSelectMemory::ENUM_MEMORY_STATE WinSelectMemory::getMemoryState()
{
    if(!sdIsExist() && !usbIsExist())
    {
        return ENUM_NOSD_NOUSB;//return 0


        //others return 1,2,3;
    }else if(sdIsExist() && !usbIsExist())
    {
        return ENUM_SD;
    }else if(!sdIsExist() && usbIsExist())
    {
        return ENUM_USB;
    }else
    {
        return ENUM_SD_USB;
    }
    //return false;
}

WinSelectMemory::WinSelectMemory(QWidget *parent) : WinSvLdFrame(tr("Select memory"),parent)
{
    bsd = ButtonFactory::getButton(SD,ButtonFactory::BUTTON_LABEL,this);
    connect(bsd,SIGNAL(clicked(bool)),this,SLOT(slotBSDClicked()));

    bflash = ButtonFactory::getButton(USB,ButtonFactory::BUTTON_LABEL,this);
    connect(bflash,SIGNAL(clicked(bool)),this,SLOT(slotBUsbClicked()));
    insertWidget(bsd);
    insertWidget(bflash);

    bNoSdUsb = ButtonFactory::getButton(STRINGNOSD_NOUSB,ButtonFactory::BUTTON_LABEL,this);
    connect(bNoSdUsb,SIGNAL(clicked(bool)),this,SLOT(slotBNoneMemoryClicked()));
    insertWidget(bNoSdUsb);
    bNoSdUsb->hide();
}


/**
 * 功能：
 *      检查设置选择内存界面的相应内存是否存在；检查选择文件按钮上的文件是否还存在,
 *      若既没有SD也没有USB则让所有的选择文件按钮Disabled
 * 参数：
 *
 * 返回值：
 *      true:
 *          Have an sd or usb
 *      false:
 *          There is no sd and usb
 */
bool WinSelectMemory::examineSetAllButton()
{
    //检查选择文件按钮上的文件是否还存在
    DisplayData::instance()->examineFileIsExist();
    DisplayData::instance()->examineSetButtonEnabled(true);

    switch (getMemoryState()) {
    case ENUM_NOSD_NOUSB:
        bflash->hide();
        bsd->hide();
        bNoSdUsb->show();
        foreach (ButtonFactory * button, m_hadSetButton) {
            button->setRightLabelText(WinSelectMemory::STRINGNOSD_NOUSB);
        }

        DisplayData::instance()->examineSetButtonEnabled(false);
        return false;
        break;
    case ENUM_SD:
        bsd->show();
        bflash->hide();
        bNoSdUsb->hide();
        foreach (ButtonFactory * button, m_hadSetButton) {
            button->setRightLabelText(WinSelectMemory::SD);
        }
        return true;
        break;
    case ENUM_USB:
        bsd->hide();
        bflash->show();
        bNoSdUsb->hide();
        foreach (ButtonFactory * button, m_hadSetButton) {
            button->setRightLabelText(WinSelectMemory::USB);
        }
        return true;
        break;
    case ENUM_SD_USB:
        bsd->show();
        bflash->show();
        bNoSdUsb->hide();
        return true;
        break;
    default:
        break;
    }
    return false;
}

//当按钮上的文字显示的文件系统存在时返回true否则返回false
bool WinSelectMemory::examineButtonMemoryIsExist(ButtonFactory *p_selectMemeoryButton)
{
    if(0 == p_selectMemeoryButton->getRightLabelText().compare(WinSelectMemory::SD))
    {
        if(WinSelectMemory::instance()->sdIsExist())
        {
            return true;
        }else
        {
            WinInformationDialog dialog(WinInformationDialog::string_fileNotExist);
            dialog.exec();
            return false;
        }
    }else if(0 == p_selectMemeoryButton->getRightLabelText().compare(WinSelectMemory::USB))
    {
        if(WinSelectMemory::instance()->usbIsExist())
        {
            return true;
        }else
        {
            WinInfoListDialog::instance()->showMsg(572);//572 删除文件失败.Deletion of the file went wrong.
            return false;
        }
    }
    return false;
}

bool WinSelectMemory::sdIsExist()
{
    if(QDir(WinSelectMemory::SD_PATH).exists())
        return true;
    else
        return false;
}

bool WinSelectMemory::usbIsExist()
{
    if(QDir(WinSelectMemory::USB_PATH).exists())
        return true;
    else
        return false;
}


void WinSelectMemory::slotBSDClicked()
{
    if(p_attachButton != NULL && p_goalButton->getRightLabelText().compare(SD))
    {
        //Not sd into this area
        //当改变内存时使选择文件按钮上文字为空
        p_attachButton->setRightLabelText("");
    }
    p_goalButton->setRightLabelText(SD);
    slidingWidget->moveToPreWidget();
    winsaveload::setp_buttonExecuted();
    examineSetAllButton();
}

void WinSelectMemory::slotBUsbClicked()
{
    if(p_attachButton != NULL && p_goalButton->getRightLabelText().compare(USB))
    {
        //Not sd into this area
        //当改变内存时使选择文件按钮上文字为空
        p_attachButton->setRightLabelText("");
    }
    p_goalButton->setRightLabelText(tr(USB.toLocal8Bit().data()));
    slidingWidget->moveToPreWidget();
    winsaveload::setp_buttonExecuted();
    examineSetAllButton();
}

void WinSelectMemory::slotBNoneMemoryClicked()
{
    p_goalButton->setRightLabelText(STRINGNOSD_NOUSB);
    slidingWidget->moveToPreWidget();
    winsaveload::setp_buttonExecuted();
    examineSetAllButton();
}

void WinSelectMemory::slotReturnClicked()
{
    qDebug() << "test";
    WinSvLdFrame::slotReturnClicked();
    examineSetAllButton();
}


WinSelectSwitch *WinSelectSwitch::instance()
{
    static WinSelectSwitch Instance;
    return &Instance;
}
WinSelectSwitch::WinSelectSwitch(QWidget *parent) : WinSvLdFrame(tr("Select Switch"),parent)
{
    ButtonFactory *bon = ButtonFactory::getButton(tr("On"),ButtonFactory::BUTTON_LABEL,this);
    connect(bon,SIGNAL(clicked(bool)),this,SLOT(slotBOnClicked()));

    ButtonFactory *boff = ButtonFactory::getButton(tr("Off"),ButtonFactory::BUTTON_LABEL,this);
    connect(boff,SIGNAL(clicked(bool)),this,SLOT(slotBOffClicked()));
    insertWidget(bon);
    insertWidget(boff);
}
void WinSelectSwitch::slotBOnClicked()
{
    if(!p_goalButton->objectName().compare("All settings"))
        p_hideWidget->hide();
    p_goalButton->setRightLabelText(tr("On"));
    slidingWidget->moveToPreWidget();
    winsaveload::setp_buttonExecuted();
}
void WinSelectSwitch::slotBOffClicked()
{
    if(!p_goalButton->objectName().compare("All settings"))
        p_hideWidget->show();
    p_goalButton->setRightLabelText(tr("Off"));
    slidingWidget->moveToPreWidget();
    winsaveload::setp_buttonExecuted();
}

const QString WinSelectGroup::string_ScaleImagePath = QString("/home/yange/sd/images/");
const QString WinSelectGroup::string_fileExist = QString(tr("Exist"));
const QString WinSelectGroup::string_fileNotExist = QString(tr("Not exist"));
WinSelectGroup *WinSelectGroup::instance()
{
    static WinSelectGroup Instance;
    return &Instance;
}

/**
 * 功能：
 *      examine scale image exist
 * 参数：
 *
 * 返回值：
 *      File exist return true,else false
 */
bool WinSelectGroup::examineGroupDataIsExist(int index)
{
    QMapIterator <ButtonFactory *, ButtonFactory *> i(m_hadSetButton);
    while (i.hasNext()) {
        i.next();
        if(QFile(string_ScaleImagePath + i.key()->getRightLabelText() + ".png").exists())
            i.value()->setRightLabelText(WinSelectGroup::string_fileExist);
        else
            i.value()->setRightLabelText(WinSelectGroup::string_fileNotExist);
    }
    if(QFile(string_ScaleImagePath + QString("%1.png").arg(index)).exists())
        return true;
    else
        return false;
}

/**
 * 功能：
 *      set goal button
 * 参数：
 *      b1: group Button
 *      b2: Is there a data button
 * 返回值：
 *      void
 */
void WinSelectGroup::setGoalButton(ButtonFactory *b1, ButtonFactory *b2)
{
    p_goalButton = b1;
    p_dataButton = b2;
    if(!m_hadSetButton.contains(b1))
        m_hadSetButton.insert(b1,b2);
}

WinSelectGroup::WinSelectGroup(QWidget *parent) : WinSvLdFrame(tr("Select number"),parent)
{
    QScrollArea *wscrollArea = new QScrollArea(this);
    QFrame *wGroupNumber = new  QFrame(wscrollArea);
    wGroupNumber->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    wscrollArea->setWidget(wGroupNumber);

    wscrollArea->setFrameShape(QFrame::NoFrame);
    wscrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    wscrollArea->verticalScrollBar()->setMaximumWidth(3);
    wscrollArea->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    insertWidget(wscrollArea);
    wscrollArea->setWidgetResizable(true);
    removeItem();


    QVBoxLayout *p_vLayoutMain = new QVBoxLayout(wGroupNumber);
    p_vLayoutMain->setMargin(0);
    p_vLayoutMain->setSpacing(0);
    ButtonFactory *p_button[50];
    QButtonGroup *p_buttonGroup = new QButtonGroup(wGroupNumber);
    for(int i = 0; i < 50; i++)
    {
        p_button[i] = ButtonFactory::
                getButton(QString::number(i + 1),ButtonFactory::BUTTON_LABEL,wGroupNumber);
        p_buttonGroup->addButton(p_button[i],i);
        p_vLayoutMain->addWidget(p_button[i]);
    }

    connect(p_buttonGroup,SIGNAL(buttonClicked(int)),
            this,SLOT(slotButtonClicked(int)));
}
void WinSelectGroup::slotButtonClicked(int index)
{
    if(examineGroupDataIsExist(index + 1))
    {
        p_dataButton->setRightLabelText(string_fileExist);
    }else if(!examineGroupDataIsExist(index + 1))
    {
        p_dataButton->setRightLabelText(string_fileNotExist);
    }

    p_goalButton->setRightLabelText(QString::number(index + 1));
    slidingWidget->moveToPreWidget();
    winsaveload::setp_buttonExecuted();
}


QString WinSelectFileType::string_excel = QString(tr("Excel file"));
QString WinSelectFileType::string_pdf = QString(tr("PDF/Printer file"));
WinSelectFileType *WinSelectFileType::instance()
{
    static WinSelectFileType Instance;
    return &Instance;
}
WinSelectFileType::WinSelectFileType(QWidget *parent) : WinSvLdFrame(tr("File type"),parent)
{
    ButtonFactory *bPDF = ButtonFactory::getButton(string_pdf,ButtonFactory::BUTTON_LABEL,this);
    connect(bPDF,SIGNAL(clicked(bool)),this,SLOT(slotPdfButtonClicked()));

    ButtonFactory *bExcel = ButtonFactory::getButton(string_excel,ButtonFactory::BUTTON_LABEL,this);
    connect(bExcel,SIGNAL(clicked(bool)),this,SLOT(slotExcelButtonClicked()));

    insertWidget(bExcel);
    insertWidget(bPDF);
}
void WinSelectFileType::slotPdfButtonClicked()
{
    p_goalButton->setRightLabelText(string_pdf);
    slidingWidget->moveToPreWidget();
    winsaveload::setp_buttonExecuted();
}

void WinSelectFileType::slotExcelButtonClicked()
{
    p_goalButton->setRightLabelText(string_excel);
    slidingWidget->moveToPreWidget();
    winsaveload::setp_buttonExecuted();
}

WinSelectReportType *WinSelectReportType::instance()
{
    static WinSelectReportType Instance;
    return &Instance;
}
WinSelectReportType::WinSelectReportType(QWidget *parent) : WinSvLdFrame(tr("Report type"),parent)
{
    reportType[REPORT_Hourly] = tr("Hourly");
    reportType[REPORT_Daily] = tr("Daily");
    reportType[REPORT_Weekly] = tr("Weekly");
    reportType[REPORT_Monthly] = tr("Monthly");
    reportType[REPORT_Hourly_Daily] = tr("Hourly+Daily");
    reportType[REPORT_Daily_Weekly] = tr("Daily+Weekly");
    reportType[REPORT_Weekly_Monthly] = tr("Weekly+Monthly");
    reportType[REPORT_Batch] = tr("Batch");
    reportType[REPORT_Daily0custom] = tr("Daily custom");

    QButtonGroup *p_buttonGroup = new QButtonGroup(this);
    foreach (QString text, reportType) {
        ButtonFactory *button = ButtonFactory::getButton(text,ButtonFactory::BUTTON_LABEL,this);
        p_buttonGroup->addButton(button,reportType.key(text));
        insertWidget(button);
    }

    connect(p_buttonGroup,SIGNAL(buttonClicked(int)),this,SLOT(slotButtonClicked(int)));
}
void WinSelectReportType::slotButtonClicked(int index)
{
    p_goalButton->setRightLabelText(reportType.value(index));
    slidingWidget->moveToPreWidget();
    winsaveload::setp_buttonExecuted();
}


WinSureOperateDialog::WinSureOperateDialog(QString text) :
    WinDialog()
{
    QFrame *p_mainFrame = new QFrame(this);
    p_mainFrame->setObjectName("BackgrounFrame");
    p_mainFrame->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);

    WinStackedLayout *layout = new WinStackedLayout(this);
    layout->addWidget(p_mainFrame,Qt::AlignCenter);
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(0);

    QPushButton *closeBtn = new QPushButton(p_mainFrame);
    closeBtn->setObjectName("CloseButton");
    closeBtn->setFocusPolicy(Qt::NoFocus);
    connect(closeBtn, SIGNAL(clicked()),this,SLOT(close()));
    connect(closeBtn, SIGNAL(clicked()),this,SLOT(reject()));

    QLabel *titleLabel = new QLabel(p_mainFrame);
    titleLabel->setObjectName("TitleLabel");
    titleLabel->setText(tr(" Confirmation"));
    QHBoxLayout * titleLayout = new QHBoxLayout();
    titleLayout->setContentsMargins(12,0,12,0);
    titleLayout->setSpacing(0);
    titleLayout->addWidget(titleLabel,0,Qt::AlignVCenter);
    titleLayout->addSpacerItem(new QSpacerItem(40,59,QSizePolicy::Expanding,QSizePolicy::Fixed));
    titleLayout->addWidget(closeBtn,0,Qt::AlignVCenter);

    QHBoxLayout * btnLayout = new QHBoxLayout;
    btnLayout->setContentsMargins(24,0,24,0);
    btnLayout->setSpacing(0);
    QPushButton *okBtn = new QPushButton();
    okBtn->setObjectName("OkButton");
    okBtn->setFocusPolicy(Qt::NoFocus);
    okBtn->setText(tr("Yes"));
    connect(okBtn, SIGNAL(clicked()),this,SLOT(slotYesClicked()));
    QPushButton *cancelBtn = new QPushButton();
    cancelBtn->setObjectName("CancelButton");
    cancelBtn->setFocusPolicy(Qt::NoFocus);
    cancelBtn->setText(tr("No"));
    connect(cancelBtn, SIGNAL(clicked()),this,SLOT(reject()));
    btnLayout->addWidget(okBtn,0,Qt::AlignVCenter);
    btnLayout->addStretch();
    btnLayout->addWidget(cancelBtn,0,Qt::AlignVCenter);

    QVBoxLayout * contextLayout = new QVBoxLayout(p_mainFrame);
    contextLayout->setContentsMargins(0,0,0,20);
    contextLayout->setSpacing(0);
    QLabel *txtLabel = new QLabel(p_mainFrame);
    txtLabel->setObjectName("TextLabel");
    txtLabel->setText(text/*"Do you want to delete this file?"*/);
    contextLayout->addLayout(titleLayout);
    contextLayout->addWidget(txtLabel,0,Qt::AlignCenter);
    contextLayout->addLayout(btnLayout);

    p_mainFrame->setStyleSheet("QFrame#BackgrounFrame{\
                        min-width:722px;\
                        max-width:722px;\
                        border: 1px solid #D3D3D3;\
                        background-color:rgb(255,255,255);\
                        background-image:url(:/relay/Image/relay/title_bg.png);\
                        background-repeat:repeat-x;\
                        background-position: top left;\
                        background-origin: content;\
                        border-top-left-radius:5px;\
                        border-top-right-radius:5px;\
                        }\
                        QPushButton#CloseButton{\
                        min-width:34px;\
                        max-width:34px;\
                        width:34px;\
                        height:34px;\
                        background-image:url(:/relay/Image/relay/close_normal.png);\
                        background-repeat: no-repeat;\
                        background-position:center;\
                        border:none;\
                        }\
                        QPushButton#CloseButton:pressed{\
                        background-image:url(:/relay/Image/relay/close_pressed.png);\
                        }\
                        QLabel#TitleLabel{\
                        color:rgb(255,255,255);\
                        font-size:32px;\
                        }\
                        QLabel#TextLabel{\
                        color:black;\
                        margin-top:30px;\
                        margin-bottom:30px;\
                        margin-left:20px;\
                        font-size:28px;\
                        }\
                        QPushButton{\
                        width:124px;\
                        height:52px;\
                        font-size:24px;\
                        color:white;\
                        background-color:none;\
                        background-image:url(:/relay/Image/relay/button_normal.png);\
                        background-repeat:no-repeat;\
                        background-position:center;\
                        border:none;\
                        }\
                        QPushButton:pressed{\
                        background-image:url(:/relay/Image/relay/button_pressed.png);\
                         }");
}

void WinSureOperateDialog::slotYesClicked()
{
    this->accept();
}

const QString WinInformationDialog::string_executeSuccuss = QString(tr("Excute completed!"));
const QString WinInformationDialog::string_executedefeated = QString(tr("Excute defeated!"));
const QString WinInformationDialog::string_fileNotExist = QString(tr("Media not recognized!"));
WinInformationDialog::WinInformationDialog(QString text)
    :WinDialog()
{
    QFrame *mainFrame = new QFrame(this);
    QVBoxLayout *vMainLayout = new QVBoxLayout(this);
    QLabel *labelContent = new QLabel(text/*tr("The file is storing.")*/,mainFrame);
    labelContent->setAlignment(Qt::AlignCenter);
    vMainLayout->addWidget(labelContent);

    QTimer::singleShot(2 * 1000,this,SLOT(close()));

    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setGeometry(0,0,WinMainFrame::instance()->width(),WinMainFrame::instance()->height()/2);
    this->setStyleSheet("WinInformationDialog{"
                            "min-width:722px;"
                            "max-width:722px;"
                            "height:600px;"
                            "border: 1px solid #D3D3D3;"
                            "background-color:rgb(255,255,255);"
                            "background-image:url(:/relay/Image/relay/title_bg.png);"
                            "background-repeat:repeat-x;"
                            "background-position: top left;"
                            "background-origin: content;"
                            "border-top-left-radius:5px;"
                            "border-top-right-radius:5px;"
                        "}"
                        "QLabel{"
                            "color:black;"
                            "text-align: center;"
                            "margin-top:30px;"
                            "margin-bottom:30px;"
                            "margin-left:20px;"
                            "font-size:28px;"
                        "}");
}
