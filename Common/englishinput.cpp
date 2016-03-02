/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：englishinput.cpp
 * 概   要：英文和符号出入窗体，用户输入完毕按钮OK按钮时发送
 *          sigEnglishInputOver（）信号，此时可以调用public函
 *          数getInputStr（）获取用户输入内容
 *
 * 当前版本：V1.0.0
 * 作   者：葛 海 浪
 * 完成日期：2015-9-17
 *
 * 修改版本：
 * 修改说明：
 * 修改作者：
 *
 ********************************************************************/

#include "englishinput.h"
#include <QPainter>
#include <QApplication>
#include <QDesktopWidget>
#include <QTextEdit>
#include <QLineEdit>
#include <QButtonGroup>
#include <QPushButton>
#include <QKeyEvent>
/*
 * 功能：
 * 	  构造函数，初始化有限状态机并run之。
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
EnglishInput::EnglishInput(QWidget *parent)
    : QWidget(parent)
    , p_centerWidget(NULL)
    , m_currentWidth(0)
    , m_currentHeight(0)
    , p_textEdit(NULL)
    , p_lineEdit(NULL)
    , p_groupABC(NULL)
    , m_inputStr("")
    , m_upLatter(false)
    , m_latterStatus(true)
    , m_objID(0)
{
    setWindowsSize();
    setDefaultStyleSheet();
}

/*
 * 功能：
 * 	  析构函数
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
EnglishInput::~EnglishInput()
{

}

/*
 * 功能：
 * 	  public接口，用户用户输入的内容
 * 参数：
 * 	  无
 * 返回值：
 * 	  QString
 */
QString EnglishInput::getInputStr()
{
    return m_inputStr;
}

void EnglishInput::show(quint32 ID)
{
    //恢复初始界面
    m_upLatter = true;        // 设置为大写状态
    m_latterStatus = true;
    slotGroupABCClicked(44);    // 单击shift，该布局以及色彩为小写状态
    //提示信息 最大输入个数
    p_textEdit->clear();
    p_lineEdit->setEchoMode(QLineEdit::Normal);

    m_objID = ID;

    switch (m_objID) {
    case ID_DataSave_File_NameString:
    case ID_System_Device_TagNo:
        p_textEdit->insertPlainText(tr("Up to 16 characters"));
        break;
    case ID_DataSave_Path_Name:
    case ID_Security_UserSetting_UserSetting_Name:
        p_textEdit->insertPlainText(tr("Up to 20 characters"));
        break;
    case ID_Security_Ope_Function_Pwd:
        setshow("");
        p_lineEdit->setEchoMode(QLineEdit::Password);
        p_textEdit->insertPlainText(tr("Up to 20 characters"));
        break;
    case ID_NetWork_Email_MailHeader_Subject:
    case ID_NetWork_FTP__ServerFirst_userName:
    case ID_NetWork_FTP__ServerSecond_userName:
    case ID_NetWork_SMTP_Client_ServerUserName:
    case ID_NetWork_SMTP_Client_ServerPwd:
    case ID_NetWork_SMTP_Client_POP3ServeruserName:
    case ID_NetWork_SMTP_Client_POP3ServerPwd:
    case ID_Batch_Text_Num_BatchNum:
        p_textEdit->insertPlainText(tr("Up to 32 characters"));
        break;
    case ID_NetWork_FTP__ServerFirst_Pwd:
    case ID_NetWork_FTP__ServerSecond_Pwd:
    {
        setshow("");
        p_lineEdit->setEchoMode(QLineEdit::Password);
        p_textEdit->insertPlainText(tr("Up to 32 characters"));
        break;
    }
    case ID_NetWork_Email_MailHeader_SenderAddr:
    case ID_NetWork_FTP__ServerFirst_Name:
    case ID_NetWork_FTP__ServerFirst_Directory:
    case ID_NetWork_FTP__ServerSecond_Name:
    case ID_NetWork_FTP__ServerSecond_Directory:
    case ID_NetWork_SMTP_Client_ServerName:
    case ID_NetWork_SMTP_Client_POP3ServerName:
    case ID_NetWork_SNTP_ServerName:
    case ID_NetWork_Modbus_ServerName:
        p_textEdit->insertPlainText(tr("Up to 64 characters"));
        break;
    case ID_NetWork_Email_MailHeader_RecipientAddr_1:
    case ID_NetWork_Email_MailHeader_RecipientAddr_2:
        p_textEdit->insertPlainText(tr("Up to 150 characters"));
        break;
    //当前配置语言为非中文时，只能调用英文键盘
    case ID_AI_Range_Scale_Unit:
    case ID_DI_Range_Scale_Unit:
    case ID_DO_Range_Range_Unit:
    case ID_Math_Exp_MathRange_Unit:
    case ID_Comm_Range_Range_Unit:
        p_textEdit->insertPlainText(tr("Up to 6 characters"));
        break;
    case ID_AI_DISP_DI0_Str:
    case ID_AI_DISP_DI1_Str:
    case ID_DI_DISP_DI0_Str:
    case ID_DI_DISP_DI1_Str:
    case ID_DO_DISP_DI0_Str:
    case ID_DO_DISP_DI1_Str:
        p_textEdit->insertPlainText(tr("Up to 8 characters"));
        break;
    case ID_AI_Disp_Tag_No:
    case ID_DI_Disp_Tag_No:
    case ID_DO_Disp_Tag_No:
    case ID_Math_Disp_Tag_No:
    case ID_Comm_Disp_Tag_No:
    case ID_Disp_Grp_Set_Name:
        p_textEdit->insertPlainText(tr("Up to 16 characters"));
        break;
    case ID_Batch_Text_TextField_Title:
        p_textEdit->insertPlainText(tr("Up to 20 characters"));
        break;
    case ID_Batch_Text_TextField_TextStr:
        p_textEdit->insertPlainText(tr("Up to 30 characters"));
        break;
    case ID_AI_Disp_Tag_Char:
    case ID_DI_Disp_Tag_Char:
    case ID_DO_Disp_Tag_Char:
    case ID_Math_Disp_Tag_Char:
    case ID_Comm_Disp_Tag_Char:
    case ID_Disp_Msg_GrpNum_1:
    case ID_Disp_Msg_GrpNum_2:
    case ID_Disp_Msg_GrpNum_3:
    case ID_Disp_Msg_GrpNum_4:
    case ID_Disp_Msg_GrpNum_5:
    case ID_Disp_Msg_GrpNum_6:
    case ID_Disp_Msg_GrpNum_7:
    case ID_Disp_Msg_GrpNum_8:
    case ID_Disp_Msg_GrpNum_9:
    case ID_Disp_Msg_GrpNum_10:
    case ID_Disp_Msg_GrpNum_11:
    case ID_Disp_Msg_GrpNum_12:
    case ID_Disp_Msg_GrpNum_13:
    case ID_Disp_Msg_GrpNum_14:
    case ID_Disp_Msg_GrpNum_15:
    case ID_Disp_Msg_GrpNum_16:
    case ID_Disp_Msg_GrpNum_17:
    case ID_Disp_Msg_GrpNum_18:
    case ID_Disp_Msg_GrpNum_19:
    case ID_Disp_Msg_GrpNum_20:
    case ID_Disp_Msg_GrpNum_21:
    case ID_Disp_Msg_GrpNum_22:
    case ID_Disp_Msg_GrpNum_23:
    case ID_Disp_Msg_GrpNum_24:
    case ID_Disp_Msg_GrpNum_25:
    case ID_Disp_Msg_GrpNum_26:
    case ID_Disp_Msg_GrpNum_27:
    case ID_Disp_Msg_GrpNum_28:
    case ID_Disp_Msg_GrpNum_29:
    case ID_Disp_Msg_GrpNum_30:
    case ID_Disp_Msg_GrpNum_31:
    case ID_Disp_Msg_GrpNum_32:
    case ID_Disp_Msg_GrpNum_33:
    case ID_Disp_Msg_GrpNum_34:
    case ID_Disp_Msg_GrpNum_35:
    case ID_Disp_Msg_GrpNum_36:
    case ID_Disp_Msg_GrpNum_37:
    case ID_Disp_Msg_GrpNum_38:
    case ID_Disp_Msg_GrpNum_39:
    case ID_Disp_Msg_GrpNum_40:
    case ID_Disp_Msg_GrpNum_41:
    case ID_Disp_Msg_GrpNum_42:
    case ID_Disp_Msg_GrpNum_43:
    case ID_Disp_Msg_GrpNum_44:
    case ID_Disp_Msg_GrpNum_45:
    case ID_Disp_Msg_GrpNum_46:
    case ID_Disp_Msg_GrpNum_47:
    case ID_Disp_Msg_GrpNum_48:
    case ID_Disp_Msg_GrpNum_49:
    case ID_Disp_Msg_GrpNum_50:
    case ID_Disp_Msg_GrpNum_51:
    case ID_Disp_Msg_GrpNum_52:
    case ID_Disp_Msg_GrpNum_53:
    case ID_Disp_Msg_GrpNum_54:
    case ID_Disp_Msg_GrpNum_55:
    case ID_Disp_Msg_GrpNum_56:
    case ID_Disp_Msg_GrpNum_57:
    case ID_Disp_Msg_GrpNum_58:
    case ID_Disp_Msg_GrpNum_59:
    case ID_Disp_Msg_GrpNum_60:
    case ID_Disp_Msg_GrpNum_61:
    case ID_Disp_Msg_GrpNum_62:
    case ID_Disp_Msg_GrpNum_63:
    case ID_Disp_Msg_GrpNum_64:
    case ID_Disp_Msg_GrpNum_65:
    case ID_Disp_Msg_GrpNum_66:
    case ID_Disp_Msg_GrpNum_67:
    case ID_Disp_Msg_GrpNum_68:
    case ID_Disp_Msg_GrpNum_69:
    case ID_Disp_Msg_GrpNum_70:
    case ID_Disp_Msg_GrpNum_71:
    case ID_Disp_Msg_GrpNum_72:
    case ID_Disp_Msg_GrpNum_73:
    case ID_Disp_Msg_GrpNum_74:
    case ID_Disp_Msg_GrpNum_75:
    case ID_Disp_Msg_GrpNum_76:
    case ID_Disp_Msg_GrpNum_77:
    case ID_Disp_Msg_GrpNum_78:
    case ID_Disp_Msg_GrpNum_79:
    case ID_Disp_Msg_GrpNum_80:
    case ID_Disp_Msg_GrpNum_81:
    case ID_Disp_Msg_GrpNum_82:
    case ID_Disp_Msg_GrpNum_83:
    case ID_Disp_Msg_GrpNum_84:
    case ID_Disp_Msg_GrpNum_85:
    case ID_Disp_Msg_GrpNum_86:
    case ID_Disp_Msg_GrpNum_87:
    case ID_Disp_Msg_GrpNum_88:
    case ID_Disp_Msg_GrpNum_89:
    case ID_Disp_Msg_GrpNum_90:
    case ID_Disp_Msg_GrpNum_91:
    case ID_Disp_Msg_GrpNum_92:
    case ID_Disp_Msg_GrpNum_93:
    case ID_Disp_Msg_GrpNum_94:
    case ID_Disp_Msg_GrpNum_95:
    case ID_Disp_Msg_GrpNum_96:
    case ID_Disp_Msg_GrpNum_97:
    case ID_Disp_Msg_GrpNum_98:
    case ID_Disp_Msg_GrpNum_99:
    case ID_Disp_Msg_GrpNum_100:
    case ID_System_Device_Tag:
        p_textEdit->insertPlainText(tr("Up to 32 characters"));
        break;
    case ID_DataSave_Title_Info:
    case ID_System_Comment_Comment:
    case ID_Batch_Text_Comment_1:
    case ID_Batch_Text_Comment_2:
    case ID_Batch_Text_Comment_3:
        p_textEdit->insertPlainText(tr("Up to 50 characters"));
        break;
    case ID_NetWork_Email_ContentHeader:
        p_textEdit->insertPlainText(tr("Up to 128 characters"));
        break;
    default:
        QWidget::show();
        return;
    }

    p_lineEdit->setFocus();
    QWidget::show();
}

void EnglishInput::setshow(QString str)
{
    p_lineEdit->clear();
    p_lineEdit->setText(str);
}

int EnglishInput::checkDataValid()
{
    m_inputStr = p_lineEdit->text();  //得到QLineEdit内容
    quint32 LengthOfStr = m_inputStr.size();

    QStringList strList = p_textEdit->toPlainText().split(" ");
    if (LengthOfStr <= strList[2].toUInt()) {
        return 0;
    } else {
        return 5;
    }

    return -1;
}

/*
 * 功能：
 * 	  重写paintEvent()函数
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
void EnglishInput::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.fillRect(this->rect(), QColor(20, 20, 20, 200));
}

/*
 * 功能：
 * 	  软键盘单击事件
 * 参数：
 * 	  int id
 * 返回值：
 * 	  无
 */
void EnglishInput::slotGroupABCClicked(int id)
{
    switch (id) {
    case 55:
    case 56:
    case 57:
    case 58:
        break;
    case 31:
    {
        if (m_latterStatus) {     // 判断是否是字母输入界面，不是字符输入界面时该按钮是'X'符号，否则该按钮是“Enter”键
            int statusCode = checkDataValid();
            if (0 == statusCode) {
                emit sigEnglishInputOver();     // 发送信号，输入完毕
            } else if (-1 == statusCode) {
                qDebug() << "digitaloperation return errorCode!";
            } else {
                WinInfoListDialog::instance()->showMsg(statusCode);
            }
        } else {
            p_lineEdit->insert(p_groupABC->button(id)->text());
        }
        break;
    }
    case 44:        // Shift button 单击
    case 53:
    {
        if (!m_upLatter) {
            p_groupABC->button(44)->setStyleSheet("QPushButton{background-color: qlineargradient(x1: 0, y1: 0,"\
                                                " x2: 0, y2: 1, stop: 0 rgb(236, 251, 254), stop: 0.13 "\
                                                "rgb(187, 241, 253), stop: 0.14 rgb(139, 234, 254), stop: 1 "\
                                                "rgb(156, 251, 255));"\
                                                "border-width: 1px; border-style: solid; border-color: rgb(168, 177, 186);"\
                                                "border-radius: 8px; font-family:'Microsoft YaHei';font-size:30px;}");
            p_groupABC->button(53)->setStyleSheet("QPushButton{background-color: qlineargradient(x1: 0, y1: 0,"\
                                                " x2: 0, y2: 1, stop: 0 rgb(236, 251, 254), stop: 0.13 "\
                                                "rgb(187, 241, 253), stop: 0.14 rgb(139, 234, 254), stop: 1 "\
                                                "rgb(156, 251, 255));"\
                                                "border-width: 1px; border-style: solid; border-color: rgb(168, 177, 186);"\
                                                "border-radius: 8px; font-family:'Microsoft YaHei';font-size:30px;}");
        } else {
            p_groupABC->button(44)->setStyleSheet("QPushButton{background-color: qlineargradient(x1: 0, y1: 0,"\
                                                " x2: 0, y2: 1, stop: 0 rgb(241, 250, 255), stop: 0.13 "\
                                                "rgb(211, 238, 255), stop: 0.14 rgb(182, 227, 255), stop: 1 "\
                                                "rgb(211, 248, 255));"\
                                                "border-width: 1px; border-style: solid; border-color: rgb(168, 177, 186);"\
                                                "border-radius: 8px; font-family:'Microsoft YaHei';font-size:30px;}");
            p_groupABC->button(53)->setStyleSheet("QPushButton{background-color: qlineargradient(x1: 0, y1: 0,"\
                                                " x2: 0, y2: 1, stop: 0 rgb(241, 250, 255), stop: 0.13 "\
                                                "rgb(211, 238, 255), stop: 0.14 rgb(182, 227, 255), stop: 1 "\
                                                "rgb(211, 248, 255));"\
                                                "border-width: 1px; border-style: solid; border-color: rgb(168, 177, 186);"\
                                                "border-radius: 8px; font-family:'Microsoft YaHei';font-size:30px;}");
        }
        m_upLatter = !m_upLatter;
        setTextShow();
        break;
    }
    case 46:        // Space 按钮
        p_lineEdit->insert(" ");
        break;
    case 6:     // &符号, 特殊处理, 大小写时代表不同的字符
    {
        if (m_latterStatus) {
            if (m_upLatter)
                p_lineEdit->insert("&");
            else
                p_lineEdit->insert(p_groupABC->button(id)->text());
        } else {
            p_lineEdit->insert(p_groupABC->button(id)->text());
        }
        break;
    }
    case 7:     // &符号，字符输入和字母输入代表不同
    {
        if (!m_latterStatus) {
            p_lineEdit->insert("&");
        } else {
            p_lineEdit->insert(p_groupABC->button(id)->text());
        }
        break;
    }
    case 59:        // Cancel button
        this->close();
        break;
    case 60: {       // OK button
        int statusCode = checkDataValid();
        if (0 == statusCode) {
            emit sigEnglishInputOver();     // 发送信号，输入完毕
        } else if (-1 == statusCode) {
            qDebug() << "englishinput return errorCode!";
        } else {
            WinInfoListDialog::instance()->showMsg(statusCode);
        }
    }
        break;
    case 61:    // 符号与字母切换按钮
        m_latterStatus = !m_latterStatus;
        setTextShow();
        break;
    default:
        p_lineEdit->insert(p_groupABC->button(id)->text());
        break;
    }
}

void EnglishInput::slotGroupABCPressed(int id)
{
    switch (id) {
    case 55:        // BS button
    {
        QKeyEvent backPress(QEvent::KeyPress, Qt::Key_Backspace, Qt::NoModifier);
        QCoreApplication::sendEvent(p_lineEdit, &backPress);
        break;
    }
    case 56:        // DEL button
    {
        QKeyEvent deletePress(QEvent::KeyPress, Qt::Key_Delete, Qt::NoModifier);
        QCoreApplication::sendEvent(p_lineEdit, &deletePress);
        break;
    }
    case 57:        // left Button
    {
        QKeyEvent leftMovePress(QEvent::KeyPress, Qt::Key_Left, Qt::NoModifier);
        QCoreApplication::sendEvent(p_lineEdit, &leftMovePress);
        break;
    }
    case 58:        // right button
    {
        QKeyEvent rightMovePress(QEvent::KeyPress, Qt::Key_Right, Qt::NoModifier);
        QCoreApplication::sendEvent(p_lineEdit, &rightMovePress);
        break;
    }
    default:
        break;
    }
}

void EnglishInput::slotGroupABCReleased(int id)
{
    switch (id) {
    case 55:        // BS button
    {
        QKeyEvent backRelease(QEvent::KeyRelease, Qt::Key_Backspace, Qt::NoModifier);
        QCoreApplication::sendEvent(p_lineEdit, &backRelease);
        break;
    }
    case 56:        // DEL button
    {
        QKeyEvent deleteRelease(QEvent::KeyRelease, Qt::Key_Delete, Qt::NoModifier);
        QCoreApplication::sendEvent(p_lineEdit, &deleteRelease);
        break;
    }
    case 57:        // left Button
    {
        QKeyEvent leftMoveRelease(QEvent::KeyRelease, Qt::Key_Left, Qt::NoModifier);
        QCoreApplication::sendEvent(p_lineEdit, &leftMoveRelease);
        break;
    }
    case 58:        // right button
    {
        QKeyEvent rightMoveRelease(QEvent::KeyRelease, Qt::Key_Right, Qt::NoModifier);
        QCoreApplication::sendEvent(p_lineEdit, &rightMoveRelease);
        break;
    }
    default:
        break;
    }
}


/*
 * 功能：
 * 	  设置窗体大小
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
void EnglishInput::setWindowsSize()
{
    this->setAttribute(Qt::WA_TranslucentBackground, true);
    m_currentWidth = QApplication::desktop()->width();
    m_currentHeight = QApplication::desktop()->height();
    resize(m_currentWidth, m_currentHeight);
    centerWidgetLayout();
}

/*
 * 功能：
 * 	  主窗体设置
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
void EnglishInput::centerWidgetLayout()
{
    p_centerWidget = new QWidget(this);
    p_centerWidget->setGeometry(0, (int)(0.1416 * m_currentHeight), m_currentWidth, (int)(0.8584 * m_currentHeight));

    p_textEdit = new QTextEdit(p_centerWidget);
    p_textEdit->setGeometry((int)(0.0508 * m_currentWidth), (int)(0.0326 * m_currentHeight), (int)(0.7331 * m_currentWidth), (int)(0.198 * m_currentHeight));
    p_textEdit->setFocusPolicy(Qt::NoFocus);

    p_lineEdit = new QLineEdit(p_centerWidget);
    p_lineEdit->setGeometry((int)(0.0508 * m_currentWidth + 1), (int)(0.1617 * m_currentHeight), (int)(0.7331 * m_currentWidth - 2), (int)(0.0627 * m_currentHeight));
    p_lineEdit->setFocus();
    p_lineEdit->setFrame(false);


    p_groupABC = new QButtonGroup;

    int groupABCCount = 0;
    for(int i = 0; i < 5; ++i)
        for(int j = 0; j < 11; ++j) {
            QPushButton* abcButton = new QPushButton(p_centerWidget);
            abcButton->setFocusPolicy(Qt::NoFocus);
            abcButton->setGeometry((int)(0.0508 * m_currentWidth + 0.0827 * m_currentWidth * j),\
                                   (int)(0.2381 * m_currentHeight + 0.1028 * m_currentHeight * i),\
                                   (int)(0.0714 * m_currentWidth), (int)(0.0952 * m_currentHeight));
            p_groupABC->addButton(abcButton, groupABCCount);
            ++groupABCCount;
        }
    for(int i = 0; i < 2; ++i)
        for (int j = 0; j < 2; ++j) {
            QPushButton* abcButton = new QPushButton(p_centerWidget);
            abcButton->setFocusPolicy(Qt::NoFocus);
            abcButton->setAutoRepeat(true);
            abcButton->setAutoRepeatDelay(500);
            abcButton->setGeometry((int)(0.7951 * m_currentWidth + 0.0827 * m_currentWidth * j),\
                                   (int)(0.0326 * m_currentHeight + 0.1028 * m_currentHeight * i),\
                                   (int)(0.0714 * m_currentWidth), (int)(0.0952 * m_currentHeight));
            p_groupABC->addButton(abcButton, groupABCCount);
            ++groupABCCount;
        }
    QPushButton* canncelButton = new QPushButton(p_centerWidget);
    canncelButton->setFocusPolicy(Qt::NoFocus);
    canncelButton->setGeometry((int)(0.0508 * m_currentWidth), (int)(0.7769 * m_currentHeight),\
                               (int)(0.1165 * m_currentWidth), (int)(0.0652 * m_currentHeight));
    p_groupABC->addButton(canncelButton, groupABCCount);
    p_groupABC->button(groupABCCount)->setObjectName("ButtonCancel");
    ++groupABCCount;

    QPushButton* OKButton = new QPushButton(p_centerWidget);
    OKButton->setFocusPolicy(Qt::NoFocus);
    OKButton->setGeometry((int)(0.8327 * m_currentWidth), (int)(0.7769 * m_currentHeight),\
                          (int)(0.1165 * m_currentWidth), (int)(0.0652 * m_currentHeight));
    p_groupABC->addButton(OKButton, groupABCCount);
    p_groupABC->button(groupABCCount)->setObjectName("ButtonOK");
    ++groupABCCount;

    QPushButton* showButton = new QPushButton(p_centerWidget);
    showButton->setFocusPolicy(Qt::NoFocus);
    showButton->setGeometry((int)(0.4418 * m_currentWidth), (int)(0.7769 * m_currentHeight),\
                            (int)(0.1165 * m_currentWidth), (int)(0.0652 * m_currentHeight));
    p_groupABC->addButton(showButton, groupABCCount);
    p_groupABC->button(groupABCCount)->setObjectName("ButtonShow");

    p_groupABC->button(31)->resize((int)(0.1532 * m_currentWidth), (int)(0.0952 * m_currentHeight));
    p_groupABC->button(32)->setHidden(true);

    p_groupABC->button(44)->resize((int)(0.1532 * m_currentWidth), (int)(0.0952 * m_currentHeight));
    p_groupABC->button(45)->setHidden(true);

    p_groupABC->button(46)->resize((int)(0.5677 * m_currentWidth), (int)(0.0952 * m_currentHeight));
    p_groupABC->button(47)->setHidden(true);
    p_groupABC->button(48)->setHidden(true);
    p_groupABC->button(49)->setHidden(true);
    p_groupABC->button(50)->setHidden(true);
    p_groupABC->button(51)->setHidden(true);
    p_groupABC->button(52)->setHidden(true);
    p_groupABC->button(53)->resize((int)(0.1532 * m_currentWidth), (int)(0.0952 * m_currentHeight));
    p_groupABC->button(54)->setHidden(true);

    connect(p_groupABC, SIGNAL(buttonClicked(int)), this, SLOT(slotGroupABCClicked(int)));
    connect(p_groupABC, SIGNAL(buttonPressed(int)), this, SLOT(slotGroupABCPressed(int)));
    connect(p_groupABC, SIGNAL(buttonReleased(int)), this, SLOT(slotGroupABCReleased(int)));
}

/*
 * 功能：
 * 	  按钮text设置
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
void EnglishInput::setTextShow()
{
    if (m_latterStatus) {
        QFont font;
        font.setStyle(QFont::Style::StyleNormal);
        p_groupABC->button(35)->setFont(font);

        p_groupABC->button(36)->setHidden(false);
        p_groupABC->button(37)->setHidden(false);
        p_groupABC->button(38)->setHidden(false);
        p_groupABC->button(39)->setHidden(false);
        p_groupABC->button(40)->setHidden(false);
        p_groupABC->button(41)->setHidden(false);
        p_groupABC->button(42)->setHidden(false);
        p_groupABC->button(43)->setHidden(false);
        p_groupABC->button(44)->setHidden(false);
        p_groupABC->button(46)->setHidden(false);
        p_groupABC->button(53)->setHidden(false);
        p_groupABC->button(31)->resize((int)(0.1532 * m_currentWidth), (int)(0.0952 * m_currentHeight));
        p_groupABC->button(32)->setHidden(true);
        if (!m_upLatter) {
            p_groupABC->button(0)->setText("1");
            p_groupABC->button(1)->setText("2");
            p_groupABC->button(2)->setText("3");
            p_groupABC->button(3)->setText("4");
            p_groupABC->button(4)->setText("5");
            p_groupABC->button(5)->setText("6");
            p_groupABC->button(6)->setText("7");
            p_groupABC->button(7)->setText("8");
            p_groupABC->button(8)->setText("9");
            p_groupABC->button(9)->setText("0");
            p_groupABC->button(10)->setText("-");
            p_groupABC->button(11)->setText("q");
            p_groupABC->button(12)->setText("w");
            p_groupABC->button(13)->setText("e");
            p_groupABC->button(14)->setText("r");
            p_groupABC->button(15)->setText("t");
            p_groupABC->button(16)->setText("y");
            p_groupABC->button(17)->setText("u");
            p_groupABC->button(18)->setText("i");
            p_groupABC->button(19)->setText("o");
            p_groupABC->button(20)->setText("p");
            p_groupABC->button(21)->setText("+");
            p_groupABC->button(22)->setText("a");
            p_groupABC->button(23)->setText("s");
            p_groupABC->button(24)->setText("d");
            p_groupABC->button(25)->setText("f");
            p_groupABC->button(26)->setText("g");
            p_groupABC->button(27)->setText("h");
            p_groupABC->button(28)->setText("j");
            p_groupABC->button(29)->setText("k");
            p_groupABC->button(30)->setText("l");
            p_groupABC->button(31)->setText("Enter");
            p_groupABC->button(33)->setText("z");
            p_groupABC->button(34)->setText("x");
            p_groupABC->button(35)->setText("c");
            p_groupABC->button(36)->setText("v");
            p_groupABC->button(37)->setText("b");
            p_groupABC->button(38)->setText("n");
            p_groupABC->button(39)->setText("m");
            p_groupABC->button(40)->setText(",");
            p_groupABC->button(41)->setText(".");
            p_groupABC->button(42)->setText("/");
            p_groupABC->button(43)->setText("?");
            p_groupABC->button(44)->setText("Shift");
            p_groupABC->button(46)->setText("SPACE");
            p_groupABC->button(53)->setText("Shift");
        } else {
            p_groupABC->button(0)->setText("!");
            p_groupABC->button(1)->setText("@");
            p_groupABC->button(2)->setText("#");
            p_groupABC->button(3)->setText("$");
            p_groupABC->button(4)->setText("%");
            p_groupABC->button(5)->setText("^");
            p_groupABC->button(6)->setText("&&");
            p_groupABC->button(7)->setText("*");
            p_groupABC->button(8)->setText("(");
            p_groupABC->button(9)->setText(")");
            p_groupABC->button(10)->setText("-");
            p_groupABC->button(11)->setText("Q");
            p_groupABC->button(12)->setText("W");
            p_groupABC->button(13)->setText("E");
            p_groupABC->button(14)->setText("R");
            p_groupABC->button(15)->setText("T");
            p_groupABC->button(16)->setText("Y");
            p_groupABC->button(17)->setText("U");
            p_groupABC->button(18)->setText("I");
            p_groupABC->button(19)->setText("O");
            p_groupABC->button(20)->setText("P");
            p_groupABC->button(21)->setText("=");
            p_groupABC->button(22)->setText("A");
            p_groupABC->button(23)->setText("S");
            p_groupABC->button(24)->setText("D");
            p_groupABC->button(25)->setText("F");
            p_groupABC->button(26)->setText("G");
            p_groupABC->button(27)->setText("H");
            p_groupABC->button(28)->setText("J");
            p_groupABC->button(29)->setText("K");
            p_groupABC->button(30)->setText("L");
            p_groupABC->button(31)->setText("Enter");
            p_groupABC->button(33)->setText("Z");
            p_groupABC->button(34)->setText("X");
            p_groupABC->button(35)->setText("C");
            p_groupABC->button(36)->setText("V");
            p_groupABC->button(37)->setText("B");
            p_groupABC->button(38)->setText("N");
            p_groupABC->button(39)->setText("M");
            p_groupABC->button(40)->setText(",");
            p_groupABC->button(41)->setText(".");
            p_groupABC->button(42)->setText("/");
            p_groupABC->button(43)->setText(":");
            p_groupABC->button(44)->setText("Shift");
            p_groupABC->button(46)->setText("SPACE");
            p_groupABC->button(53)->setText("Shift");
        }
        p_groupABC->button(61)->setText(tr("SYMBOL"));
    } else {
        QFont font;
        font.setStyle(QFont::Style::StyleOblique);
        p_groupABC->button(35)->setFont(font);
        p_groupABC->button(36)->setHidden(true);

        p_groupABC->button(0)->setText("~");
        p_groupABC->button(1)->setText("!");
        p_groupABC->button(2)->setText("@");
        p_groupABC->button(3)->setText("#");
        p_groupABC->button(4)->setText("$");
        p_groupABC->button(5)->setText("%");
        p_groupABC->button(6)->setText("^");
        p_groupABC->button(7)->setText("&&");
        p_groupABC->button(8)->setText("*");
        p_groupABC->button(9)->setText("(");
        p_groupABC->button(10)->setText(")");
        p_groupABC->button(11)->setText("\"");
        p_groupABC->button(12)->setText("\'");
        p_groupABC->button(13)->setText(".");
        p_groupABC->button(14)->setText(":");
        p_groupABC->button(15)->setText("?");
        p_groupABC->button(16)->setText("—");
        p_groupABC->button(17)->setText("=");
        p_groupABC->button(18)->setText("+");
        p_groupABC->button(19)->setText("-");
        p_groupABC->button(20)->setText("/");
        p_groupABC->button(21)->setText("\\");
        p_groupABC->button(22)->setText("|");
        p_groupABC->button(23)->setText("<");
        p_groupABC->button(24)->setText(">");
        p_groupABC->button(25)->setText("{");
        p_groupABC->button(26)->setText("}");
        p_groupABC->button(27)->setText("[");
        p_groupABC->button(28)->setText("]");
        p_groupABC->button(29)->setText("·");
        p_groupABC->button(30)->setText("。");
        p_groupABC->button(31)->resize((int)(0.0714 * m_currentWidth), (int)(0.0952 * m_currentHeight));
        p_groupABC->button(32)->setHidden(false);
        p_groupABC->button(31)->setText("×");
        p_groupABC->button(32)->setText("÷");
        p_groupABC->button(33)->setText("￥");
        p_groupABC->button(34)->setText("±");
        p_groupABC->button(35)->setText("μ");
        p_groupABC->button(61)->setText(tr("ABC"));
        p_groupABC->button(37)->setHidden(true);
        p_groupABC->button(38)->setHidden(true);
        p_groupABC->button(39)->setHidden(true);
        p_groupABC->button(40)->setHidden(true);
        p_groupABC->button(41)->setHidden(true);
        p_groupABC->button(42)->setHidden(true);
        p_groupABC->button(43)->setHidden(true);
        p_groupABC->button(44)->setHidden(true);
        p_groupABC->button(46)->setHidden(true);
        p_groupABC->button(53)->setHidden(true);
    }
    p_groupABC->button(59)->setText(tr("Cancel"));
    p_groupABC->button(60)->setText(tr("OK"));
    p_groupABC->button(55)->setText("BS");
    p_groupABC->button(56)->setText("DEL");
    p_groupABC->button(57)->setText("←");
    p_groupABC->button(58)->setText("→");

}

void EnglishInput::setDefaultStyleSheet()
{
    setStyleSheet("QWidget{background-color: rgb(231, 238, 246);}"\
                  "QTextEdit{background-color: rgb(255, 255, 255);font-family:'Microsoft YaHei';font-size:20px;}"\
                  "QLineEdit{background-color: rgb(255, 255, 255);font-family:'Microsoft YaHei';font-size:30px;}"\
                  "QPushButton{background-color: qlineargradient(x1: 0, y1: 0,"\
                  " x2: 0, y2: 1, stop: 0 rgb(241, 250, 255), stop: 0.13 "\
                  "rgb(211, 238, 255), stop: 0.14 rgb(182, 227, 255), stop: 1 "\
                  "rgb(211, 248, 255));"\
                  "border-width: 1px; border-style: solid; border-color: rgb(168, 177, 186);"\
                  "border-radius: 8px; font-family:'Microsoft YaHei';font-size:30px;}"\
                  "QPushButton:pressed {background-color: qlineargradient(x1: 0, y1: 0,"\
                  " x2: 0, y2: 1, stop: 0 rgb(236, 251, 254), stop: 0.13 "\
                  "rgb(187, 241, 253), stop: 0.14 rgb(139, 234, 254), stop: 1 "\
                  "rgb(156, 251, 255));"\
                  "border-width: 1px; border-style: solid; border-color: rgb(168, 177, 186);"\
                  "border-radius: 8px; font-family:'Microsoft YaHei';font-size:30px;}"\
                  "#ButtonCancel{background-color: qlineargradient(x1: 0, y1: 0,"\
                  " x2: 0, y2: 1, stop: 0 rgb(159, 222, 255), stop: 0.13 "\
                  "rgb(20, 158, 249), stop: 0.14 rgb(0, 143, 248), stop: 1 "\
                  "rgb(5, 117, 253));color: rgb(255, 255, 255);"\
                  "border-width: 1px; border-style: solid; border-color: rgb(0, 7, 132);"\
                  "border-radius: 10px; font-family:'Microsoft YaHei';font-size:30px;}"\
                  "#ButtonCancel:pressed {background-color: qlineargradient(x1: 0, y1: 0,"\
                  " x2: 0, y2: 1, stop: 0 rgb(255, 255, 255), stop: 0.13 "\
                  "rgb(113, 212, 254), stop: 0.14 rgb(58, 197, 255), stop: 1 "\
                  "rgb(55, 250, 254));color: rgb(255, 255, 255);"\
                  "border-width: 1px; border-style: solid; border-color: rgb(0, 7, 132);"\
                  "border-radius: 10px; font-family:'Microsoft YaHei';font-size:30px;}"\
                  "#ButtonOK{background-color: qlineargradient(x1: 0, y1: 0,"\
                  " x2: 0, y2: 1, stop: 0 rgb(159, 222, 255), stop: 0.13 "\
                  "rgb(20, 158, 249), stop: 0.14 rgb(0, 143, 248), stop: 1 "\
                  "rgb(5, 117, 253));;color: rgb(255, 255, 255);"\
                  "border-width: 1px; border-style: solid; border-color: rgb(0, 7, 132);"\
                  "border-radius: 10px; font-family:'Microsoft YaHei';font-size:30px;}"\
                  "#ButtonOK:pressed {background-color: qlineargradient(x1: 0, y1: 0,"\
                  " x2: 0, y2: 1, stop: 0 rgb(255, 255, 255), stop: 0.13 "\
                  "rgb(113, 212, 254), stop: 0.14 rgb(58, 197, 255), stop: 1 "\
                  "rgb(55, 250, 254));color: rgb(255, 255, 255);"\
                  "border-width: 1px; border-style: solid; border-color: rgb(0, 7, 132);"\
                  "border-radius: 10px; font-family:'Microsoft YaHei';font-size:30px;}"\
                  "#ButtonShow{background-color: qlineargradient(x1: 0, y1: 0,"\
                  " x2: 0, y2: 1, stop: 0 rgb(159, 222, 255), stop: 0.13 "\
                  "rgb(20, 158, 249), stop: 0.14 rgb(0, 143, 248), stop: 1 "\
                  "rgb(5, 117, 253));;color: rgb(255, 255, 255);"\
                  "border-width: 1px; border-style: solid; border-color: rgb(0, 7, 132);"\
                  "border-radius: 10px; font-family:'Microsoft YaHei';font-size:30px;}"\
                  "#ButtonShow:pressed {background-color: qlineargradient(x1: 0, y1: 0,"\
                  " x2: 0, y2: 1, stop: 0 rgb(255, 255, 255), stop: 0.13 "\
                  "rgb(113, 212, 254), stop: 0.14 rgb(58, 197, 255), stop: 1 "\
                  "rgb(55, 250, 254));color: rgb(255, 255, 255);"\
                  "border-width: 1px; border-style: solid; border-color: rgb(0, 7, 132);"\
                  "border-radius: 10px; font-family:'Microsoft YaHei';font-size:30px;}");
}
