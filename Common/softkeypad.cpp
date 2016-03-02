/*********************************************************************
 * Copyright(c) 2014, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称：softkeypad.h
 * 概   要：在调用该类时，画面会出现一个软件盘供用户输入, sigOkPressSignal()
 *          信号表示用户输入完毕，sigCancelPressSignal() , 表示用户取消输入.
 *          setTipInfor(QString str)设置输入框的提示信息, getInputStr() 获取
 *          用户输入的内容
 *
 * 当前版本：V1.1.0
 * 作   者：葛 海 浪
 * 完成日期：2015-9-29
 *
 * 修改版本：1.0.0
 * 修改说明：替换原有的softkeypad，舍去UI设计师界面
 * 修改作者：葛 海 浪
 *
 ********************************************************************/
#include "softkeypad.h"
#include <QPushButton>
#include <QTextEdit>
#include <QButtonGroup>
#include <QPainter>
#include <QApplication>
#include <QDesktopWidget>
#include <QLineEdit>
#include <QKeyEvent>
#include <QDebug>

/*
 * 功能：
 * 	  构造函数，初始化有限状态机并run之。
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
SoftKeyPad::SoftKeyPad(QWidget *parent)
    : QWidget(parent)
    , m_currentWidth(0)
    , m_currentHeight(0)
    , m_englishState(LatterSmall)
    , m_chineseState(LatterSmall)
    , m_listCount(0)
    , m_listChineseID(0)
    , m_current_choice(KeyInput_English)
    , m_cursorPosition(0)
    , m_firstInputFlag(true)
    , p_centerWidget(NULL)
    , p_chineseShowWidget(NULL)
    , p_textEdit(NULL)
    , p_lineEdit(NULL)
    , p_groupEnglish(NULL)
    , p_groupChinese(NULL)
    , p_groupSymbol(NULL)
    , p_groupChineseShow(NULL)
    , p_buttonBS(NULL)
    , p_buttonDEL(NULL)
    , p_buttonLeftMove(NULL)
    , p_buttonRightMove(NULL)
    , p_buttonCancel(NULL)
    , p_buttonEnglish(NULL)
    , p_buttonSymbol(NULL)
    , p_buttonChinese(NULL)
    , p_buttonOK(NULL)
    , m_inputStr("")
    , m_singleLetter("")
    , m_inputLetter("")
    , m_lineInputLetter("")
{
    setDefaultStyleSheet();
    setWindowsLayout();
    setCenterWidgetLayout();
    connectDatabase();
    connectFunC();
}

SoftKeyPad::~SoftKeyPad()
{

}

/*
 * 功能：
 * 	  设置信息栏的提示信息
 *
 * 参数：
 * 	  QString
 *
 * 返回值：
 * 	  无
 */
void SoftKeyPad::show(quint32 ID)
{
    //恢复初始界面
    slotButtonChineseClicked();
    //提示信息 最大输入个数
    p_textEdit->clear();

    m_objID = ID;

    switch (m_objID) {
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

void SoftKeyPad::setshow(QString str)
{
    p_lineEdit->clear();
    p_lineEdit->setText(str);
}

/*
 * 功能：
 * 	  获取用户输入的文字
 *
 * 参数：
 * 	  无
 *
 * 返回值：
 * 	  QString
 */
QString SoftKeyPad::getInputStr()
{
    return m_inputStr;
}

int SoftKeyPad::checkDataValid()
{
    m_inputStr = p_lineEdit->text();
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
 * 	  重写paintEvent事件，使窗体透明
 *
 * 参数：
 * 	  QPaintEvent
 *
 * 返回值：
 * 	  无
 */
void SoftKeyPad::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.fillRect(this->rect(), QColor(20, 20, 20, 200));
}

/*
 * 功能：
 * 	  button按钮被点击时会触发该函数
 *
 * 参数：
 * 	  无
 *
 * 返回值：
 * 	  无
 */
void SoftKeyPad::slotButtonCancelClicked()
{
    emit sigCancelPressSignal();
}

/*
 * 功能：
 * 	  英文输入界面按钮单击事件
 *
 * 参数：
 * 	  int id
 *
 * 返回值：
 * 	  无
 */
void SoftKeyPad::slotGroupEnglishClicked(int id)
{
    switch (id) {
    case 0: case 1: case 2: case 3:
    case 5: case 6: case 7: case 8:
    case 9: case 10: case 11: case 12:
    case 13: case 14: case 15: case 16:
    case 17: case 18: case 19: case 20:
    case 21: case 22: case 23: case 24:
    case 25: case 26: case 27: case 28:
    case 29: case 30: case 33: case 34:
    case 35: case 36: case 37: case 38:
    case 39: case 40: case 41: case 42:
    case 43:
        p_lineEdit->insert(p_groupEnglish->button(id)->text());
        break;
    case 4:
    {
        if(LatterSmall == m_englishState) {
            p_lineEdit->insert(p_groupEnglish->button(id)->text());
        } else if(LatterBig == m_englishState) {
            p_lineEdit->insert("&");
        }
        break;
    }
    case 31:
        slotButtonOKClicked();
        break;
    case 44:
    case 46:
    {
        if(LatterSmall == m_englishState) {
            m_englishState = LatterBig;
        } else if(LatterBig == m_englishState) {
            m_englishState = LatterSmall;
        }
        setGroupEndlishText();
        break;
    }
    case 45:
        p_lineEdit->insert(" ");
        break;
    default:
        break;
    }
    m_cursorPosition = p_lineEdit->cursorPosition();
}

/*
 * 功能：
 * 	  符号输入界面按钮单击事件
 *
 * 参数：
 * 	  int id
 *
 * 返回值：
 * 	  无
 */
void SoftKeyPad::slotGroupSymbolClicked(int id)
{
    switch (id) {
    case 7:
        p_lineEdit->insert("&");
        break;
    default:
        p_lineEdit->insert(p_groupSymbol->button(id)->text());
        break;
    }
    m_cursorPosition = p_lineEdit->cursorPosition();
}

/*
 * 功能：
 * 	  中文输入界面按钮单击事件
 *
 * 参数：
 * 	  int id
 *
 * 返回值：
 * 	  无
 */
void SoftKeyPad::slotGroupChineseClicked(int id)
{
    switch (id) {
    case 0: case 1: case 2: case 3: case 5:
    case 6: case 7: case 8: case 9: case 10:
    case 21: case 40: case 41: case 42: case 43:
    {
        if("" == m_inputLetter) {
            p_chineseShowWidget->setHidden(true);
            p_lineEdit->insert(p_groupChinese->button(id)->text());
            m_cursorPosition = p_lineEdit->cursorPosition();
        } else {
            // do nothing
        }
        break;
    }
    case 4:
    {
        if("" == m_inputLetter) {
            if(LatterSmall == m_chineseState) {
                p_lineEdit->insert(p_groupChinese->button(id)->text());
            } else if(LatterBig == m_chineseState) {
                p_lineEdit->insert("&");
            }
            m_cursorPosition = p_lineEdit->cursorPosition();
        } else {
            // do nothing
        }
        break;
    }
    case 11: case 12: case 13: case 14: case 15:
    case 16: case 17: case 18: case 19: case 20:
    case 22: case 23: case 24: case 25: case 26:
    case 27: case 28: case 29: case 30: case 33:
    case 34: case 35: case 36: case 37: case 38:
    case 39: case 46: case 47: case 48:
    {
        p_chineseShowWidget->setHidden(false);
        m_lineInputLetter += p_groupChinese->button(id)->text();
        p_lineEdit->insert(m_lineInputLetter);
        p_lineEdit->setSelection(m_cursorPosition, m_lineInputLetter.size());
        m_listChinese.clear();
        if(m_firstInputFlag) {
            m_singleLetter = p_groupChinese->button(id)->text().toLower();
            m_firstInputFlag = false;
        }
        m_inputLetter += p_groupChinese->button(id)->text().toLower();
        m_listChineseID = 0;
        selectSQL();
        chineseShow();
        break;
    }
    case 31:        // enter
    {
        p_chineseShowWidget->setHidden(true);
        if("" == m_lineInputLetter) {
            slotButtonOKClicked();
        } else {
            p_lineEdit->insert(m_lineInputLetter);
            m_cursorPosition = p_lineEdit->cursorPosition();
            m_firstInputFlag = true;
            m_inputLetter = "";
            m_lineInputLetter = "";
        }
        break;
    }
    case 44:
    case 49:    // Shift
    {
        if(LatterSmall == m_chineseState) {
            m_chineseState = LatterBig;
        } else if(LatterBig == m_chineseState) {
            m_chineseState = LatterSmall;
        }
        if(0 == m_listCount) {
            p_chineseShowWidget->setHidden(true);
        } else {
            p_chineseShowWidget->setHidden(false);
        }
        setGroupChineseText();
        break;
    }
    case 45:
    {
        if("" == m_inputLetter) {
            p_lineEdit->insert(" ");
            m_cursorPosition = p_lineEdit->cursorPosition();
        } else {
            // do nothing
        }
    }
    default:
        break;
    }
}

/*
 * 功能：
 * 	  中文显示界面按钮单击事件
 *
 * 参数：
 * 	  int id
 *
 * 返回值：
 * 	  无
 */
void SoftKeyPad::slotGroupChineseShowClicked(int id)
{
    switch (id) {
    case 0:
        leftMove();
        p_groupChineseShow->button(9)->setEnabled(true);
        break;
    case 1: case 2: case 3: case 4:
    case 5: case 6: case 7: case 8:
        if("" == p_groupChineseShow->button(id)->text()) {
            // do nothing
        } else {
            p_lineEdit->insert(p_groupChineseShow->button(id)->text());
            m_firstInputFlag = true;
            m_inputLetter = "";
            m_lineInputLetter = "";
            m_listCount = 0;
            p_chineseShowWidget->setHidden(true);
            m_cursorPosition = p_lineEdit->cursorPosition();
        }
        break;
    case 9:
        rightMove();
        p_groupChineseShow->button(0)->setEnabled(true);
        break;
    default:
        break;
    }
}

/*
 * 功能：
 * 	  英文切换按钮下押事件，改变当前输入界面
 *
 * 参数：
 * 	  无
 *
 * 返回值：
 * 	  无
 */
void SoftKeyPad::slotButtonEnglishClicked()
{
    m_current_choice = KeyInput_English;
    setEnglishShow();
    defaultStyle();
    p_buttonEnglish->setStyleSheet("background-color: qlineargradient(x1: 0, y1: 0,"\
                                 " x2: 0, y2: 1, stop: 0 rgb(255, 255, 255), stop: 0.13 "\
                                 "rgb(113, 212, 254), stop: 0.14 rgb(58, 197, 255), stop: 1 "\
                                 "rgb(55, 250, 254));color: rgb(255, 255, 255);"\
                                 "border-width: 1px; border-style: solid; border-color: rgb(0, 7, 132);"\
                                 "border-radius: 10px; font-family:'Microsoft YaHei';font-size:30px;");
}

/*
 * 功能：
 * 	  中文切换按钮下押事件，改变当前输入界面
 *
 * 参数：
 * 	  无
 *
 * 返回值：
 * 	  无
 */
void SoftKeyPad::slotButtonChineseClicked()
{
    m_current_choice = KeyInput_Chinese;
    setChineseShow();
    defaultStyle();
    p_buttonChinese->setStyleSheet("background-color: qlineargradient(x1: 0, y1: 0,"\
                                 " x2: 0, y2: 1, stop: 0 rgb(255, 255, 255), stop: 0.13 "\
                                 "rgb(113, 212, 254), stop: 0.14 rgb(58, 197, 255), stop: 1 "\
                                 "rgb(55, 250, 254));color: rgb(255, 255, 255);"\
                                 "border-width: 1px; border-style: solid; border-color: rgb(0, 7, 132);"\
                                 "border-radius: 10px; font-family:'Microsoft YaHei';font-size:30px;");
}

/*
 * 功能：
 * 	  符号切换按钮下押事件，改变当前输入界面
 *
 * 参数：
 * 	  无
 *
 * 返回值：
 * 	  无
 */
void SoftKeyPad::slotButtonSymbolClicked()
{
    m_current_choice = KeyInput_Symbol;
    setSymbolShow();
    defaultStyle();
    p_buttonSymbol->setStyleSheet("background-color: qlineargradient(x1: 0, y1: 0,"\
                                " x2: 0, y2: 1, stop: 0 rgb(255, 255, 255), stop: 0.13 "\
                                "rgb(113, 212, 254), stop: 0.14 rgb(58, 197, 255), stop: 1 "\
                                "rgb(55, 250, 254));color: rgb(255, 255, 255);"\
                                "border-width: 1px; border-style: solid; border-color: rgb(0, 7, 132);"\
                                "border-radius: 10px; font-family:'Microsoft YaHei';font-size:30px;");
}

/*
 * 功能：
 * 	  BS输入界面按钮单击事件
 *
 * 参数：
 * 	  无
 *
 * 返回值：
 * 	  无
 */
void SoftKeyPad::slotButtonBSPressed()
{
    switch (m_current_choice) {
    case KeyInput_English:
    case KeyInput_Symbol:
    {
        QKeyEvent backPress(QEvent::KeyPress, Qt::Key_Backspace, Qt::NoModifier);
        QCoreApplication::sendEvent(p_lineEdit, &backPress);
        m_cursorPosition = p_lineEdit->cursorPosition();
        break;
    }
    case KeyInput_Chinese:
    {
        if("" == m_inputLetter) {
            p_lineEdit->backspace();
            m_cursorPosition = p_lineEdit->cursorPosition();
        } else {
            m_listChinese.clear();
            int size = m_lineInputLetter.size();
            m_lineInputLetter = m_lineInputLetter.remove(size - 1, 1);
            p_lineEdit->insert(m_lineInputLetter);
            p_lineEdit->setSelection(m_cursorPosition, m_lineInputLetter.size());
            m_inputLetter = m_lineInputLetter.toLower();
            if("" == m_inputLetter) {
                p_chineseShowWidget->setHidden(true);
                m_firstInputFlag = true;
            } else {
                p_chineseShowWidget->setHidden(false);
                m_firstInputFlag = false;
            }
            m_listChineseID = 0;
            selectSQL();
            chineseShow();
        }
    }
    default:
        break;
    }
}

/*
 * 功能：
 * 	  DEL按钮单击事件
 *
 * 参数：
 * 	  无
 *
 * 返回值：
 * 	  无
 */
void SoftKeyPad::slotButtonDELPressed()
{
    switch (m_current_choice) {
    case KeyInput_English:
    case KeyInput_Symbol:
    {
        QKeyEvent deletePress(QEvent::KeyPress, Qt::Key_Delete, Qt::NoModifier);
        QCoreApplication::sendEvent(p_lineEdit, &deletePress);
        QKeyEvent deleteRelease(QEvent::KeyRelease, Qt::Key_Delete, Qt::NoModifier);
        QCoreApplication::sendEvent(p_lineEdit, &deleteRelease);
        break;
    }
    case KeyInput_Chinese:
    {
        if("" == m_inputLetter) {
            QKeyEvent deletePress(QEvent::KeyPress, Qt::Key_Delete, Qt::NoModifier);
            QCoreApplication::sendEvent(p_lineEdit, &deletePress);
            QKeyEvent deleteRelease(QEvent::KeyRelease, Qt::Key_Delete, Qt::NoModifier);
            QCoreApplication::sendEvent(p_lineEdit, &deleteRelease);
        } else {
            // do nothing
        }
        break;
    }
    default:
        break;
    }
}

/*
 * 功能：
 * 	  move left按钮单击事件
 *
 * 参数：
 * 	  无
 *
 * 返回值：
 * 	  无
 */
void SoftKeyPad::slotButtonMoveLeftPressed()
{
    switch (m_current_choice) {
    case KeyInput_English:
    case KeyInput_Symbol:
    {
        QKeyEvent leftMovePress(QEvent::KeyPress, Qt::Key_Left, Qt::NoModifier);
        QCoreApplication::sendEvent(p_lineEdit, &leftMovePress);
        m_cursorPosition = p_lineEdit->cursorPosition();
        break;
    }
    case KeyInput_Chinese:
    {
        if("" == m_inputLetter) {
            QKeyEvent leftMovePress(QEvent::KeyPress, Qt::Key_Left, Qt::NoModifier);
            QCoreApplication::sendEvent(p_lineEdit, &leftMovePress);
            m_cursorPosition = p_lineEdit->cursorPosition();
        } else {
            // do nothing
        }
        break;
    }
    default:
        break;
    }
}

/*
 * 功能：
 * 	   move right按钮单击事件
 *
 * 参数：
 * 	  无
 *
 * 返回值：
 * 	  无
 */
void SoftKeyPad::slotButtonMoveRightPressed()
{
    switch (m_current_choice) {
    case KeyInput_English:
    case KeyInput_Symbol:
    {
        QKeyEvent rightMovePressed(QEvent::KeyPress, Qt::Key_Right, Qt::NoModifier);
        QCoreApplication::sendEvent(p_lineEdit, &rightMovePressed);
        m_cursorPosition = p_lineEdit->cursorPosition();
        break;
    }
    case KeyInput_Chinese:
    {
        if("" == m_inputLetter) {
            QKeyEvent rightMovePressed(QEvent::KeyPress, Qt::Key_Right, Qt::NoModifier);
            QCoreApplication::sendEvent(p_lineEdit, &rightMovePressed);
            m_cursorPosition = p_lineEdit->cursorPosition();
        } else {
            // do nothing
        }
        break;
    }
    default:
        break;
    }
}

void SoftKeyPad::slotButtonBSReleased()
{
    switch (m_current_choice) {
    case KeyInput_English:
    case KeyInput_Symbol:
    {
        QKeyEvent backRelease(QEvent::KeyRelease, Qt::Key_Backspace, Qt::NoModifier);
        QCoreApplication::sendEvent(p_lineEdit, &backRelease);
        m_cursorPosition = p_lineEdit->cursorPosition();
        break;
    }
    default:
        break;
    }
}

void SoftKeyPad::slotButtonDELReleased()
{
    switch (m_current_choice) {
    case KeyInput_English:
    case KeyInput_Symbol:
    {
        QKeyEvent deleteRelease(QEvent::KeyRelease, Qt::Key_Delete, Qt::NoModifier);
        QCoreApplication::sendEvent(p_lineEdit, &deleteRelease);
        break;
    }
    case KeyInput_Chinese:
    {
        if("" == m_inputLetter) {
            QKeyEvent deleteRelease(QEvent::KeyRelease, Qt::Key_Delete, Qt::NoModifier);
            QCoreApplication::sendEvent(p_lineEdit, &deleteRelease);
        } else {
            // do nothing
        }
        break;
    }
    default:
        break;
    }
}

void SoftKeyPad::slotButtonMoveLeftReleased()
{
    switch (m_current_choice) {
    case KeyInput_English:
    case KeyInput_Symbol:
    {
        QKeyEvent leftMoveRelease(QEvent::KeyRelease, Qt::Key_Left, Qt::NoModifier);
        QCoreApplication::sendEvent(p_lineEdit, &leftMoveRelease);
        m_cursorPosition = p_lineEdit->cursorPosition();
        break;
    }
    case KeyInput_Chinese:
    {
        if("" == m_inputLetter) {
            QKeyEvent leftMoveRelease(QEvent::KeyRelease, Qt::Key_Left, Qt::NoModifier);
            QCoreApplication::sendEvent(p_lineEdit, &leftMoveRelease);
            m_cursorPosition = p_lineEdit->cursorPosition();
        } else {
            // do nothing
        }
        break;
    }
    default:
        break;
    }
}

void SoftKeyPad::slotButtonMoveRightReleased()
{
    switch (m_current_choice) {
    case KeyInput_English:
    case KeyInput_Symbol:
    {
        QKeyEvent rightMoveRelease(QEvent::KeyRelease, Qt::Key_Right, Qt::NoModifier);
        QCoreApplication::sendEvent(p_lineEdit, &rightMoveRelease);
        m_cursorPosition = p_lineEdit->cursorPosition();
        break;
    }
    case KeyInput_Chinese:
    {
        if("" == m_inputLetter) {
            QKeyEvent rightMoveRelease(QEvent::KeyRelease, Qt::Key_Right, Qt::NoModifier);
            QCoreApplication::sendEvent(p_lineEdit, &rightMoveRelease);
            m_cursorPosition = p_lineEdit->cursorPosition();
        } else {
            // do nothing
        }
        break;
    }
    default:
        break;
    }
}

/*
 * 功能：
 * 	   OK按钮单击事件
 *
 * 参数：
 * 	  无
 *
 * 返回值：
 * 	  无
 */
void SoftKeyPad::slotButtonOKClicked()
{
    int statusCode = checkDataValid();
    if (0 == statusCode) {
        emit sigOkPressSignal();     // 发送信号，输入完毕
    } else if (-1 == statusCode) {
        qDebug() << "softkeypad return errorCode!";
    } else {
        WinInfoListDialog::instance()->showMsg(statusCode);
    }
}

/*
 * 功能：
 * 	   设置窗体大小
 *
 * 参数：
 * 	  无
 *
 * 返回值：
 * 	  无
 */
void SoftKeyPad::setWindowsLayout()
{
    this->setAttribute(Qt::WA_TranslucentBackground, true);
    m_currentWidth = QApplication::desktop()->width();
    m_currentHeight = QApplication::desktop()->height();
    resize(m_currentWidth, m_currentHeight);
}

/*
 * 功能：
 * 	   主窗体设置
 *
 * 参数：
 * 	  无
 *
 * 返回值：
 * 	  无
 */
void SoftKeyPad::setCenterWidgetLayout()
{
    p_centerWidget = new QWidget(this);
    p_centerWidget->setGeometry(0, (int)(0.1416 * m_currentHeight), m_currentWidth, (int)(0.8584 * m_currentHeight));

    p_textEdit = new QTextEdit(p_centerWidget);
    p_textEdit->setGeometry((int)(0.0507 * m_currentWidth), (int)(0.03 * m_currentHeight),\
                          (int)(0.734 * m_currentWidth), (int)(0.198 * m_currentHeight));
    p_textEdit->setFocusPolicy(Qt::NoFocus);

    p_chineseShowWidget = new QWidget(p_textEdit);
    p_chineseShowWidget->setGeometry(2, 2,(int)(0.73 * m_currentWidth), (int)(0.1 * m_currentHeight));
    p_chineseShowWidget->setHidden(true);

    p_groupChineseShow = new QButtonGroup;
    for(int i = 0; i < 10; ++i) {
        QPushButton* chineseShowButton = new QPushButton(p_chineseShowWidget);
        chineseShowButton->setFocusPolicy(Qt::NoFocus);
        chineseShowButton->setGeometry(3 + i * (int)(0.072 * m_currentWidth), 3,\
                                       (int)(0.07 *m_currentWidth), (int)(0.09 * m_currentHeight));
        p_groupChineseShow->addButton(chineseShowButton, i);
    }
    p_groupChineseShow->button(0)->setText("<<");
    p_groupChineseShow->button(9)->setText(">>");

    p_lineEdit = new QLineEdit(p_textEdit);
    p_lineEdit->setAttribute(Qt::WA_TranslucentBackground, true);
    p_lineEdit->setGeometry(2, (int)(0.12 * m_currentHeight),\
                          (int)(0.73 * m_currentWidth), (int)(0.1 * m_currentHeight));
    p_lineEdit->setFrame(false);
    p_lineEdit->setText("");

    p_buttonBS = new QPushButton(p_centerWidget);
    p_buttonBS->setFocusPolicy(Qt::NoFocus);
    p_buttonBS->setGeometry((int)(0.795 * m_currentWidth), (int)(0.03 * m_currentHeight),\
                          (int)(0.0714 *m_currentWidth), (int)(0.0952 * m_currentHeight));
    p_buttonBS->setText(tr("BS"));
    p_buttonBS->setAutoRepeat(true);
    p_buttonBS->setAutoRepeatDelay(500);

    p_buttonDEL = new QPushButton(p_centerWidget);
    p_buttonDEL->setFocusPolicy(Qt::NoFocus);
    p_buttonDEL->setGeometry((int)(0.8778 * m_currentWidth), (int)(0.03 * m_currentHeight),\
                           (int)(0.0714 *m_currentWidth), (int)(0.0952 * m_currentHeight));
    p_buttonDEL->setText(tr("DEL"));
    p_buttonDEL->setAutoRepeat(true);
    p_buttonDEL->setAutoRepeatDelay(500);

    p_buttonLeftMove = new QPushButton(p_centerWidget);
    p_buttonLeftMove->setFocusPolicy(Qt::NoFocus);
    p_buttonLeftMove->setGeometry((int)(0.795 * m_currentWidth), (int)(0.1328 * m_currentHeight),\
                                (int)(0.0714 *m_currentWidth), (int)(0.0952 * m_currentHeight));
    p_buttonLeftMove->setIcon(QIcon(":/calendar/Image/calendar/left.png"));
    p_buttonLeftMove->setIconSize(QSize(p_buttonLeftMove->width(), p_buttonLeftMove->height()));
    p_buttonLeftMove->setAutoRepeat(true);
    p_buttonLeftMove->setAutoRepeatDelay(500);

    p_buttonRightMove = new QPushButton(p_centerWidget);
    p_buttonRightMove->setFocusPolicy(Qt::NoFocus);
    p_buttonRightMove->setGeometry((int)(0.8778 * m_currentWidth), (int)(0.1328 * m_currentHeight),\
                                 (int)(0.0714 *m_currentWidth), (int)(0.0952 * m_currentHeight));
    p_buttonRightMove->setIcon(QIcon(":/calendar/Image/calendar/right.png"));
    p_buttonRightMove->setIconSize(QSize(p_buttonRightMove->width(), p_buttonRightMove->height()));
    p_buttonRightMove->setAutoRepeat(true);
    p_buttonRightMove->setAutoRepeatDelay(500);

    p_groupEnglish = new QButtonGroup;
    p_groupChinese = new QButtonGroup;
    p_groupSymbol = new QButtonGroup;


    p_buttonCancel = new QPushButton(p_centerWidget);
    p_buttonCancel->setFocusPolicy(Qt::NoFocus);
    p_buttonCancel->setGeometry((int)(0.0508 * m_currentWidth), (int)(0.772 * m_currentHeight),\
                              (int)(0.1165 * m_currentWidth), (int)(0.0714 * m_currentHeight));
    p_buttonCancel->setObjectName("ButtonCancel");
    p_buttonCancel->setText(tr("Cancel"));

    p_buttonOK = new QPushButton(p_centerWidget);
    p_buttonOK->setFocusPolicy(Qt::NoFocus);
    p_buttonOK->setGeometry((int)(0.8327 * m_currentWidth), (int)(0.772 * m_currentHeight),\
                          (int)(0.1165 * m_currentWidth), (int)(0.0714 * m_currentHeight));
    p_buttonOK->setObjectName("ButtonOK");
    p_buttonOK->setText(tr("OK"));

    p_buttonEnglish = new QPushButton(p_centerWidget);
    p_buttonEnglish->setFocusPolicy(Qt::NoFocus);
    p_buttonEnglish->setGeometry((int)(0.332 * m_currentWidth), (int)(0.772 * m_currentHeight),\
                               (int)(0.117 * m_currentWidth), (int)(0.0714 * m_currentHeight));
    p_buttonEnglish->setText(tr("English"));

    p_buttonChinese = new QPushButton(p_centerWidget);
    p_buttonChinese->setFocusPolicy(Qt::NoFocus);
    p_buttonChinese->setGeometry((int)(0.449 * m_currentWidth), (int)(0.772 * m_currentHeight),\
                               (int)(0.117 * m_currentWidth), (int)(0.0714 * m_currentHeight));
    p_buttonChinese->setText(tr("Chinese"));

    p_buttonSymbol = new QPushButton(p_centerWidget);
    p_buttonSymbol->setFocusPolicy(Qt::NoFocus);
    p_buttonSymbol->setGeometry((int)(0.566 * m_currentWidth), (int)(0.772 * m_currentHeight),\
                              (int)(0.117 * m_currentWidth), (int)(0.0714 * m_currentHeight));
    p_buttonSymbol->setText(tr("Symbol"));

    /*********  英文界面布局  *********/
    setGroupEndlishLayout();
    setGroupChineseLayout();
    setGroupSymbolLayout();
    slotButtonEnglishClicked();
}

/*
 * 功能：
 * 	   英文界面布局
 *
 * 参数：
 * 	  无
 *
 * 返回值：
 * 	  无
 */
void SoftKeyPad::setGroupEndlishLayout()
{
    int groupEnglishID = 0;
    for(int i = 0; i < 4; ++i)
        for(int j = 0; j < 11; ++j) {
            QPushButton* englishButton = new QPushButton(p_centerWidget);
            englishButton->setGeometry((int)(0.0507 * m_currentWidth) + j * (int)(0.0827 * m_currentWidth), \
                                       (int)(0.2356 * m_currentHeight) + i * (int)(0.1028 * m_currentHeight),\
                                       (int)(0.0714 *m_currentWidth), (int)(0.0952 * m_currentHeight));

            englishButton->setFocusPolicy(Qt::NoFocus);
            p_groupEnglish->addButton(englishButton, groupEnglishID);
            ++groupEnglishID;
        }
    p_groupEnglish->button(31)->setGeometry((int)(0.794 * m_currentWidth), (int)(0.4411 * m_currentHeight),\
                                          (int)(0.1532 *m_currentWidth), (int)(0.0952 * m_currentHeight));

    QPushButton* leftShiftButton = new QPushButton(p_centerWidget);
    leftShiftButton->setGeometry((int)(0.0507 * m_currentWidth), (int)(0.6466 * m_currentHeight),\
                                 (int)(0.1532 *m_currentWidth), (int)(0.0952 * m_currentHeight));
    p_groupEnglish->addButton(leftShiftButton, 44);

    QPushButton* spaceButton = new QPushButton(p_centerWidget);
    spaceButton->setGeometry((int)(0.2161 * m_currentWidth), (int)(0.6466 * m_currentHeight),\
                             (int)(0.5677 *m_currentWidth), (int)(0.0952 * m_currentHeight));
    p_groupEnglish->addButton(spaceButton, 45);

    QPushButton* rightShiftButton = new QPushButton(p_centerWidget);
    rightShiftButton->setGeometry((int)(0.795 * m_currentWidth), (int)(0.6466 * m_currentHeight),\
                                  (int)(0.1532 *m_currentWidth), (int)(0.0952 * m_currentHeight));
    p_groupEnglish->addButton(rightShiftButton, 46);

    for(int i = 44; i < 47; ++i) {
        p_groupEnglish->button(i)->setFocusPolicy(Qt::NoFocus);
    }

    setGroupEndlishText();
}

/*
 * 功能：
 * 	   英文界面显示
 *
 * 参数：
 * 	  无
 *
 * 返回值：
 * 	  无
 */
void SoftKeyPad::setGroupEndlishText()
{
    p_groupEnglish->button(32)->setHidden(true);
    if(LatterSmall == m_englishState)  {  // 小写状态下布局
        for(int i = 0; i < 9; ++i) {
            p_groupEnglish->button(i)->setText(QString("%1").arg(i + 1));
        }
        p_groupEnglish->button(9)->setText("0");
        p_groupEnglish->button(10)->setText("-");
        p_groupEnglish->button(11)->setText("q");
        p_groupEnglish->button(12)->setText("w");
        p_groupEnglish->button(13)->setText("e");
        p_groupEnglish->button(14)->setText("r");
        p_groupEnglish->button(15)->setText("t");
        p_groupEnglish->button(16)->setText("y");
        p_groupEnglish->button(17)->setText("u");
        p_groupEnglish->button(18)->setText("i");
        p_groupEnglish->button(19)->setText("o");
        p_groupEnglish->button(20)->setText("p");
        p_groupEnglish->button(21)->setText("+");
        p_groupEnglish->button(22)->setText("a");
        p_groupEnglish->button(23)->setText("s");
        p_groupEnglish->button(24)->setText("d");
        p_groupEnglish->button(25)->setText("f");
        p_groupEnglish->button(26)->setText("g");
        p_groupEnglish->button(27)->setText("h");
        p_groupEnglish->button(28)->setText("j");
        p_groupEnglish->button(29)->setText("k");
        p_groupEnglish->button(30)->setText("l");
        p_groupEnglish->button(31)->setText("Enter");
        p_groupEnglish->button(33)->setText("z");
        p_groupEnglish->button(34)->setText("x");
        p_groupEnglish->button(35)->setText("c");
        p_groupEnglish->button(36)->setText("v");
        p_groupEnglish->button(37)->setText("b");
        p_groupEnglish->button(38)->setText("n");
        p_groupEnglish->button(39)->setText("m");
        p_groupEnglish->button(40)->setText(",");
        p_groupEnglish->button(41)->setText(".");
        p_groupEnglish->button(42)->setText("/");
        p_groupEnglish->button(43)->setText("?");
        p_groupEnglish->button(44)->setStyleSheet("background-color: qlineargradient(x1: 0, y1: 0,"\
                                                " x2: 0, y2: 1, stop: 0 rgb(241, 250, 255), stop: 0.13 "\
                                                "rgb(211, 238, 255), stop: 0.14 rgb(182, 227, 255), stop: 1 "\
                                                "rgb(211, 248, 255));"\
                                                "border-width: 1px; border-style: solid; border-color: rgb(168, 177, 186);"\
                                                "border-radius: 8px; font-family:'Microsoft YaHei';font-size:30px;");
        p_groupEnglish->button(46)->setStyleSheet("background-color: qlineargradient(x1: 0, y1: 0,"\
                                                " x2: 0, y2: 1, stop: 0 rgb(241, 250, 255), stop: 0.13 "\
                                                "rgb(211, 238, 255), stop: 0.14 rgb(182, 227, 255), stop: 1 "\
                                                "rgb(211, 248, 255));"\
                                                "border-width: 1px; border-style: solid; border-color: rgb(168, 177, 186);"\
                                                "border-radius: 8px; font-family:'Microsoft YaHei';font-size:30px;");
    } else {
        p_groupEnglish->button(0)->setText("!");
        p_groupEnglish->button(1)->setText("@");
        p_groupEnglish->button(2)->setText("#");
        p_groupEnglish->button(3)->setText("%");
        p_groupEnglish->button(4)->setText("&&");
        p_groupEnglish->button(5)->setText("*");
        p_groupEnglish->button(6)->setText("`");
        p_groupEnglish->button(7)->setText("\"");
        p_groupEnglish->button(8)->setText("(");
        p_groupEnglish->button(9)->setText(")");
        p_groupEnglish->button(10)->setText("_");
        p_groupEnglish->button(11)->setText("Q");
        p_groupEnglish->button(12)->setText("W");
        p_groupEnglish->button(13)->setText("E");
        p_groupEnglish->button(14)->setText("R");
        p_groupEnglish->button(15)->setText("T");
        p_groupEnglish->button(16)->setText("Y");
        p_groupEnglish->button(17)->setText("U");
        p_groupEnglish->button(18)->setText("I");
        p_groupEnglish->button(19)->setText("O");
        p_groupEnglish->button(20)->setText("P");
        p_groupEnglish->button(21)->setText("=");
        p_groupEnglish->button(22)->setText("A");
        p_groupEnglish->button(23)->setText("S");
        p_groupEnglish->button(24)->setText("D");
        p_groupEnglish->button(25)->setText("F");
        p_groupEnglish->button(26)->setText("G");
        p_groupEnglish->button(27)->setText("H");
        p_groupEnglish->button(28)->setText("J");
        p_groupEnglish->button(29)->setText("K");
        p_groupEnglish->button(30)->setText("L");
        p_groupEnglish->button(31)->setText("Enter");
        p_groupEnglish->button(33)->setText("Z");
        p_groupEnglish->button(34)->setText("X");
        p_groupEnglish->button(35)->setText("C");
        p_groupEnglish->button(36)->setText("V");
        p_groupEnglish->button(37)->setText("B");
        p_groupEnglish->button(38)->setText("N");
        p_groupEnglish->button(39)->setText("M");
        p_groupEnglish->button(40)->setText(",");
        p_groupEnglish->button(41)->setText(".");
        p_groupEnglish->button(42)->setText("/");
        p_groupEnglish->button(43)->setText(":");
        p_groupEnglish->button(44)->setStyleSheet("background-color: qlineargradient(x1: 0, y1: 0,"\
                                                " x2: 0, y2: 1, stop: 0 rgb(236, 251, 254), stop: 0.13 "\
                                                "rgb(187, 241, 253), stop: 0.14 rgb(139, 234, 254), stop: 1 "\
                                                "rgb(156, 251, 255));"\
                                                "border-width: 1px; border-style: solid; border-color: rgb(168, 177, 186);"\
                                                "border-radius: 8px; font-family:'Microsoft YaHei';font-size:30px;");
        p_groupEnglish->button(46)->setStyleSheet("background-color: qlineargradient(x1: 0, y1: 0,"\
                                                " x2: 0, y2: 1, stop: 0 rgb(236, 251, 254), stop: 0.13 "\
                                                "rgb(187, 241, 253), stop: 0.14 rgb(139, 234, 254), stop: 1 "\
                                                "rgb(156, 251, 255));"\
                                                "border-width: 1px; border-style: solid; border-color: rgb(168, 177, 186);"\
                                                "border-radius: 8px; font-family:'Microsoft YaHei';font-size:30px;");
    }

    p_groupEnglish->button(45)->setText("Space");
    p_groupEnglish->button(44)->setText("Shift");
    p_groupEnglish->button(46)->setText("Shift");
}

/*
 * 功能：
 * 	   中文界面布局
 *
 * 参数：
 * 	  无
 *
 * 返回值：
 * 	  无
 */
void SoftKeyPad::setGroupChineseLayout()
{
    int groupChineseID = 0;
    for(int i = 0; i < 4; ++i)
        for(int j = 0; j < 11; ++j) {
            QPushButton* chineseButton = new QPushButton(p_centerWidget);
            chineseButton->setGeometry((int)(0.0507 * m_currentWidth) + j * (int)(0.0827 * m_currentWidth), \
                                       (int)(0.2356 * m_currentHeight) + i * (int)(0.1028 * m_currentHeight),\
                                       (int)(0.0714 *m_currentWidth), (int)(0.0952 * m_currentHeight));
            chineseButton->setFocusPolicy(Qt::NoFocus);
            p_groupChinese->addButton(chineseButton, groupChineseID);
            ++groupChineseID;
        }
    p_groupChinese->button(31)->setGeometry((int)(0.794 * m_currentWidth), (int)(0.4411 * m_currentHeight),\
                                          (int)(0.1532 *m_currentWidth), (int)(0.0952 * m_currentHeight));

    QPushButton* leftShiftButton = new QPushButton(p_centerWidget);
    leftShiftButton->setGeometry((int)(0.0507 * m_currentWidth), (int)(0.6466 * m_currentHeight),\
                                 (int)(0.1532 *m_currentWidth), (int)(0.0952 * m_currentHeight));
    p_groupChinese->addButton(leftShiftButton, 44);
    leftShiftButton->setFocusPolicy(Qt::NoFocus);

    QPushButton* spaceButton = new QPushButton(p_centerWidget);
    spaceButton->setGeometry((int)(0.2161 * m_currentWidth), (int)(0.6466 * m_currentHeight),\
                             (int)(0.3195 *m_currentWidth), (int)(0.0952 * m_currentHeight));
    spaceButton->setFocusPolicy(Qt::NoFocus);
    p_groupChinese->addButton(spaceButton, 45);

    QPushButton* chButton = new QPushButton(p_centerWidget);
    chButton->setGeometry((int)(0.547 * m_currentWidth), (int)(0.6466 * m_currentHeight),\
                          (int)(0.0714 *m_currentWidth), (int)(0.0952 * m_currentHeight));
    chButton->setText("ch");
    p_groupChinese->addButton(chButton, 46);

    QPushButton* shButton = new QPushButton(p_centerWidget);
    shButton->setGeometry((int)(0.6297 * m_currentWidth), (int)(0.6466 * m_currentHeight),\
                          (int)(0.0714 *m_currentWidth), (int)(0.0952 * m_currentHeight));
    shButton->setText("sh");
    p_groupChinese->addButton(shButton, 47);

    QPushButton* zhButton = new QPushButton(p_centerWidget);
    zhButton->setGeometry((int)(0.7124 * m_currentWidth), (int)(0.6466 * m_currentHeight),\
                          (int)(0.0714 *m_currentWidth), (int)(0.0952 * m_currentHeight));
    zhButton->setText("zh");
    p_groupChinese->addButton(zhButton, 48);
    for(int i = 46; i < 49; ++i) {
        p_groupChinese->button(i)->setFocusPolicy(Qt::NoFocus);
    }

    QPushButton* rightShiftButton = new QPushButton(p_centerWidget);
    rightShiftButton->setFocusPolicy(Qt::NoFocus);
    rightShiftButton->setGeometry((int)(0.795 * m_currentWidth), (int)(0.6466 * m_currentHeight),\
                                  (int)(0.1532 *m_currentWidth), (int)(0.0952 * m_currentHeight));
    p_groupChinese->addButton(rightShiftButton, 49);
    setGroupChineseText();
}

/*
 * 功能：
 * 	   中文界面显示
 *
 * 参数：
 * 	  无
 *
 * 返回值：
 * 	  无
 */
void SoftKeyPad::setGroupChineseText()
{
    p_groupChinese->button(32)->setHidden(true);
    if(LatterSmall == m_chineseState)  {  // 小写状态下布局
        for(int i = 0; i < 9; ++i) {
            p_groupChinese->button(i)->setText(QString("%1").arg(i + 1));
        }
        p_groupChinese->button(9)->setText("0");
        p_groupChinese->button(10)->setText("－");
        p_groupChinese->button(11)->setText("q");
        p_groupChinese->button(12)->setText("w");
        p_groupChinese->button(13)->setText("e");
        p_groupChinese->button(14)->setText("r");
        p_groupChinese->button(15)->setText("t");
        p_groupChinese->button(16)->setText("y");
        p_groupChinese->button(17)->setText("u");
        p_groupChinese->button(18)->setText("i");
        p_groupChinese->button(19)->setText("o");
        p_groupChinese->button(20)->setText("p");
        p_groupChinese->button(21)->setText("＋");
        p_groupChinese->button(22)->setText("a");
        p_groupChinese->button(23)->setText("s");
        p_groupChinese->button(24)->setText("d");
        p_groupChinese->button(25)->setText("f");
        p_groupChinese->button(26)->setText("g");
        p_groupChinese->button(27)->setText("h");
        p_groupChinese->button(28)->setText("j");
        p_groupChinese->button(29)->setText("k");
        p_groupChinese->button(30)->setText("l");
        p_groupChinese->button(31)->setText("Enter");
        p_groupChinese->button(33)->setText("z");
        p_groupChinese->button(34)->setText("x");
        p_groupChinese->button(35)->setText("c");
        p_groupChinese->button(36)->setText("v");
        p_groupChinese->button(37)->setText("b");
        p_groupChinese->button(38)->setText("n");
        p_groupChinese->button(39)->setText("m");
        p_groupChinese->button(40)->setText("，");
        p_groupChinese->button(41)->setText("。");
        p_groupChinese->button(42)->setText("；");
        p_groupChinese->button(43)->setText("：");
        p_groupChinese->button(46)->setText("ch");
        p_groupChinese->button(47)->setText("sh");
        p_groupChinese->button(48)->setText("zh");
        p_groupChinese->button(44)->setStyleSheet("background-color: qlineargradient(x1: 0, y1: 0,"\
                                                " x2: 0, y2: 1, stop: 0 rgb(241, 250, 255), stop: 0.13 "\
                                                "rgb(211, 238, 255), stop: 0.14 rgb(182, 227, 255), stop: 1 "\
                                                "rgb(211, 248, 255));"\
                                                "border-width: 1px; border-style: solid; border-color: rgb(168, 177, 186);"\
                                                "border-radius: 8px; font-family:'Microsoft YaHei';font-size:30px;");
        p_groupChinese->button(49)->setStyleSheet("background-color: qlineargradient(x1: 0, y1: 0,"\
                                                " x2: 0, y2: 1, stop: 0 rgb(241, 250, 255), stop: 0.13 "\
                                                "rgb(211, 238, 255), stop: 0.14 rgb(182, 227, 255), stop: 1 "\
                                                "rgb(211, 248, 255));"\
                                                "border-width: 1px; border-style: solid; border-color: rgb(168, 177, 186);"\
                                                "border-radius: 8px; font-family:'Microsoft YaHei';font-size:30px;");
    } else {
        p_groupChinese->button(0)->setText("!");
        p_groupChinese->button(1)->setText("@");
        p_groupChinese->button(2)->setText("#");
        p_groupChinese->button(3)->setText("%");
        p_groupChinese->button(4)->setText("&&");
        p_groupChinese->button(5)->setText("*");
        p_groupChinese->button(6)->setText("“");
        p_groupChinese->button(7)->setText("”");
        p_groupChinese->button(8)->setText("(");
        p_groupChinese->button(9)->setText(")");
        p_groupChinese->button(10)->setText("－");
        p_groupChinese->button(11)->setText("Q");
        p_groupChinese->button(12)->setText("W");
        p_groupChinese->button(13)->setText("E");
        p_groupChinese->button(14)->setText("R");
        p_groupChinese->button(15)->setText("T");
        p_groupChinese->button(16)->setText("Y");
        p_groupChinese->button(17)->setText("U");
        p_groupChinese->button(18)->setText("I");
        p_groupChinese->button(19)->setText("O");
        p_groupChinese->button(20)->setText("P");
        p_groupChinese->button(21)->setText("=");
        p_groupChinese->button(22)->setText("A");
        p_groupChinese->button(23)->setText("S");
        p_groupChinese->button(24)->setText("D");
        p_groupChinese->button(25)->setText("F");
        p_groupChinese->button(26)->setText("G");
        p_groupChinese->button(27)->setText("H");
        p_groupChinese->button(28)->setText("J");
        p_groupChinese->button(29)->setText("K");
        p_groupChinese->button(30)->setText("L");
        p_groupChinese->button(31)->setText("Enter");
        p_groupChinese->button(33)->setText("Z");
        p_groupChinese->button(34)->setText("X");
        p_groupChinese->button(35)->setText("C");
        p_groupChinese->button(36)->setText("V");
        p_groupChinese->button(37)->setText("B");
        p_groupChinese->button(38)->setText("N");
        p_groupChinese->button(39)->setText("M");
        p_groupChinese->button(40)->setText("，");
        p_groupChinese->button(41)->setText("。");
        p_groupChinese->button(42)->setText("；");
        p_groupChinese->button(43)->setText("：");
        p_groupChinese->button(46)->setText("CH");
        p_groupChinese->button(47)->setText("SH");
        p_groupChinese->button(48)->setText("ZH");
        p_groupChinese->button(44)->setStyleSheet("background-color: qlineargradient(x1: 0, y1: 0,"\
                                                " x2: 0, y2: 1, stop: 0 rgb(236, 251, 254), stop: 0.13 "\
                                                "rgb(187, 241, 253), stop: 0.14 rgb(139, 234, 254), stop: 1 "\
                                                "rgb(156, 251, 255));"\
                                                "border-width: 1px; border-style: solid; border-color: rgb(168, 177, 186);"\
                                                "border-radius: 8px; font-family:'Microsoft YaHei';font-size:30px;");
        p_groupChinese->button(49)->setStyleSheet("background-color: qlineargradient(x1: 0, y1: 0,"\
                                                " x2: 0, y2: 1, stop: 0 rgb(236, 251, 254), stop: 0.13 "\
                                                "rgb(187, 241, 253), stop: 0.14 rgb(139, 234, 254), stop: 1 "\
                                                "rgb(156, 251, 255));"\
                                                "border-width: 1px; border-style: solid; border-color: rgb(168, 177, 186);"\
                                                "border-radius: 8px; font-family:'Microsoft YaHei';font-size:30px;");
    }

    p_groupChinese->button(45)->setText("Space");
    p_groupChinese->button(44)->setText("Shift");
    p_groupChinese->button(49)->setText("Shift");
}

/*
 * 功能：
 * 	   符号界面布局
 *
 * 参数：
 * 	  无
 *
 * 返回值：
 * 	  无
 */
void SoftKeyPad::setGroupSymbolLayout()
{
    int groupSymbolID = 0;
    for(int i = 0; i < 4; ++i)
        for(int j = 0; j < 11; ++j) {
            QPushButton* symbolButton = new QPushButton(p_centerWidget);
            symbolButton->setGeometry((int)(0.0507 * m_currentWidth) + j * (int)(0.0827 * m_currentWidth), \
                                      (int)(0.2356 * m_currentHeight) + i * (int)(0.1028 * m_currentHeight),\
                                      (int)(0.0714 *m_currentWidth), (int)(0.0952 * m_currentHeight));
            symbolButton->setFocusPolicy(Qt::NoFocus);
            p_groupSymbol->addButton(symbolButton, groupSymbolID);
            ++groupSymbolID;
        }
    setGroupSymbolText();
}

/*
 * 功能：
 * 	   符号界面显示
 *
 * 参数：
 * 	  无
 *
 * 返回值：
 * 	  无
 */
void SoftKeyPad::setGroupSymbolText()
{
    p_groupSymbol->button(0)->setText("~");
    p_groupSymbol->button(1)->setText("!");
    p_groupSymbol->button(2)->setText("@");
    p_groupSymbol->button(3)->setText("#");
    p_groupSymbol->button(4)->setText("$");
    p_groupSymbol->button(5)->setText("%");
    p_groupSymbol->button(6)->setText("^");
    p_groupSymbol->button(7)->setText("&&");
    p_groupSymbol->button(8)->setText("*");
    p_groupSymbol->button(9)->setText("(");
    p_groupSymbol->button(10)->setText(")");
    p_groupSymbol->button(11)->setText("\"");
    p_groupSymbol->button(12)->setText(",");
    p_groupSymbol->button(13)->setText(".");
    p_groupSymbol->button(14)->setText(":");
    p_groupSymbol->button(15)->setText("?");
    p_groupSymbol->button(16)->setText("_");
    p_groupSymbol->button(17)->setText("=");
    p_groupSymbol->button(18)->setText("+");
    p_groupSymbol->button(19)->setText("-");
    p_groupSymbol->button(20)->setText("/");
    p_groupSymbol->button(21)->setText("\\");
    p_groupSymbol->button(22)->setText("|");
    p_groupSymbol->button(23)->setText("<");
    p_groupSymbol->button(24)->setText(">");
    p_groupSymbol->button(25)->setText("{");
    p_groupSymbol->button(26)->setText("}");
    p_groupSymbol->button(27)->setText("[");
    p_groupSymbol->button(28)->setText("]");
    p_groupSymbol->button(29)->setText("·");
    p_groupSymbol->button(30)->setText("°");
    p_groupSymbol->button(31)->setText("×");
    p_groupSymbol->button(32)->setText("÷");
    p_groupSymbol->button(33)->setText("¥");
    p_groupSymbol->button(34)->setText("±");
    QFont font;
    font.setStyle(QFont::Style::StyleOblique);
    p_groupSymbol->button(35)->setFont(font);
    p_groupSymbol->button(35)->setText("μ");
    p_groupSymbol->button(36)->setText("℃");
    for(int id = 37; id < 44; ++id) {
        p_groupSymbol->button(id)->setHidden(true);
    }
}

/*
 * 功能：
 * 	   显示英文界面
 *
 * 参数：
 * 	  无
 *
 * 返回值：
 * 	  无
 */
void SoftKeyPad::setEnglishShow()
{
//    qDebug() << p_groupChinese->buttons().size();
    for(int i = 0; i < p_groupChinese->buttons().size(); ++i) {
        p_groupChinese->button(i)->setHidden(true);
    }

    for(int i = 0; i < p_groupSymbol->buttons().size(); ++i) {
        p_groupSymbol->button(i)->setHidden(true);
    }

    for(int i = 0; i < p_groupEnglish->buttons().size(); ++i) {
        p_groupEnglish->button(i)->setHidden(false);
    }

    setGroupEndlishText();
}

/*
 * 功能：
 * 	   显示中文界面
 *
 * 参数：
 * 	  无
 *
 * 返回值：
 * 	  无
 */
void SoftKeyPad::setChineseShow()
{
    for(int i = 0; i < p_groupChinese->buttons().size(); ++i) {
        p_groupChinese->button(i)->setHidden(false);
    }

    for(int i = 0; i < p_groupSymbol->buttons().size(); ++i) {
        p_groupSymbol->button(i)->setHidden(true);
    }

    for(int i = 0; i < p_groupEnglish->buttons().size(); ++i) {
        p_groupEnglish->button(i)->setHidden(true);
    }
    setGroupChineseText();
}

/*
 * 功能：
 * 	   显示符号界面
 *
 * 参数：
 * 	  无
 *
 * 返回值：
 * 	  无
 */
void SoftKeyPad::setSymbolShow()
{
    for(int i = 0; i < p_groupChinese->buttons().size(); ++i) {
        p_groupChinese->button(i)->setHidden(true);
    }

    for(int i = 0; i < p_groupSymbol->buttons().size(); ++i) {
        p_groupSymbol->button(i)->setHidden(false);
    }

    for(int i = 0; i < p_groupEnglish->buttons().size(); ++i) {
        p_groupEnglish->button(i)->setHidden(true);
    }
    setGroupSymbolText();
}

/*
 * 功能：
 * 	   数据库连接
 *
 * 参数：
 * 	  无
 *
 * 返回值：
 * 	  无
 */
void SoftKeyPad::connectDatabase()
{
    if (QSqlDatabase::contains("china.db")) {
        m_db = QSqlDatabase::database("china.db");
    } else {
        m_db = QSqlDatabase::addDatabase("QSQLITE", "china.db");
        m_db.setDatabaseName("china.db");
    }
    if(m_db.open()) {
        qDebug() << "m_db open successful!";
    } else {
        qDebug() << "m_db open failure!";
    }

    m_query = QSqlQuery(m_db);
}

/*
 * 功能：
 * 	   显示中文
 *
 * 参数：
 * 	  无
 *
 * 返回值：
 * 	  无
 */
void SoftKeyPad::chineseShow()
{
    if(0 == m_listCount) {
        p_chineseShowWidget->setHidden(true);
    } else {
        if(m_listChineseID < 8) {
            p_groupChineseShow->button(0)->setEnabled(false);
        }
        while (m_listChineseID < m_listCount) {
            p_groupChineseShow->button(m_listChineseID % 8 + 1)->setText(m_listChinese.at(m_listChineseID));
            if(++m_listChineseID == m_listCount)
            {
                if (0 == (m_listChineseID % 8)) {
                    // do nothing
                } else if (1 == (m_listChineseID % 8)) {
                    for(int i = 2; i < 9; ++i) {
                        p_groupChineseShow->button(i)->setText("");
                    }
                } else if (2 == (m_listChineseID % 8)) {
                    for(int i = 3; i < 9; ++i) {
                        p_groupChineseShow->button(i)->setText("");
                    }
                } else if (3 == (m_listChineseID % 8)) {
                    for(int i = 4; i < 9; ++i) {
                        p_groupChineseShow->button(i)->setText("");
                    }
                } else if (4 == (m_listChineseID % 8)) {
                    for(int i = 5; i < 9; ++i) {
                        p_groupChineseShow->button(i)->setText("");
                    }
                } else if (5 == (m_listChineseID % 8)) {
                    for(int i = 6; i < 9; ++i) {
                        p_groupChineseShow->button(i)->setText("");
                    }
                } else if (6 == (m_listChineseID % 8)) {
                    for(int i = 7; i < 9; ++i) {
                        p_groupChineseShow->button(i)->setText("");
                    }
                } else if (7 == (m_listChineseID % 8)) {
                    p_groupChineseShow->button(8)->setText("");
                }
                p_groupChineseShow->button(9)->setEnabled(false);
            }

            if (0 == (m_listChineseID % 8))
                break;
        }
    }
}

/*
 * 功能：
 * 	   光标左移
 *
 * 参数：
 * 	  无
 *
 * 返回值：
 * 	  无
 */
void SoftKeyPad::leftMove()
{
    int chineseNum = m_listChineseID % 8;
    switch (chineseNum) {
    case 0:
        m_listChineseID -= 16;
        break;
    case 1:
        m_listChineseID -= 9;
        break;
    case 2:
        m_listChineseID -= 10;
        break;
    case 3:
        m_listChineseID -= 11;
        break;
    case 4:
        m_listChineseID -= 12;
        break;
    case 5:
        m_listChineseID -= 13;
        break;
    case 6:
        m_listChineseID -= 14;
        break;
    case 7:
        m_listChineseID -= 15;
        break;
    default:
        break;
    }
    chineseShow();
}

/*
 * 功能：
 * 	   光标右移
 *
 * 参数：
 * 	  无
 *
 * 返回值：
 * 	  无
 */
void SoftKeyPad::rightMove()
{
    chineseShow();
}

/*
 * 功能：
 * 	   SQL语句
 *
 * 参数：
 * 	  无
 *
 * 返回值：
 * 	  无
 */
void SoftKeyPad::selectSQL()
{
    /**************** 根据首字母查询对应的表 ************/
    if ("a" == m_singleLetter) {
        if(m_query.exec(QString("select word from pinyin_a where pinyin like '" + m_inputLetter + "%'"))) {
            qDebug() << "m_query from pinyin_a successful!";
        } else {
            qDebug() << "m_query from pinyin_a fail!";
        }
        qDebug() << "select from pinyin_a";
    } else if ("b" == m_singleLetter) {
        if(m_query.exec(QString("select word from pinyin_b where pinyin like '" + m_inputLetter + "%'"))) {
            qDebug() << "m_query from pinyin_b successful!";
        } else {
            qDebug() << "m_query from pinyin_b fail!";
        }
        qDebug() << "select from pinyin_b";
    } else if (("c" == m_singleLetter) || ("ch" == m_singleLetter)) {
        if(m_query.exec(QString("select word from pinyin_c where pinyin like '" + m_inputLetter + "%'"))) {
            qDebug() << "m_query from pinyin_c successful!";
        } else {
            qDebug() << "m_query from pinyin_c fail!";
        }
        qDebug() << "select from pinyin_c";
    } else if ("d" == m_singleLetter) {
        if(m_query.exec(QString("select word from pinyin_d where pinyin like '" + m_inputLetter + "%'"))) {
            qDebug() << "m_query from pinyin_d successful!";
        } else {
            qDebug() << "m_query from pinyin_d fail!";
        }
        qDebug() << "select from pinyin_d";
    } else if ("e" == m_singleLetter) {
        if(m_query.exec(QString("select word from pinyin_e where pinyin like '" + m_inputLetter + "%'"))) {
            qDebug() << "m_query from pinyin_e successful!";
        } else {
            qDebug() << "m_query from pinyin_e fail!";
        }
        qDebug() << "select from pinyin_e";
    } else if ("f" == m_singleLetter) {
        if(m_query.exec(QString("select word from pinyin_f where pinyin like '" + m_inputLetter + "%'"))) {
            qDebug() << "m_query from pinyin_f successful!";
        } else {
            qDebug() << "m_query from pinyin_f fail!";
        }
        qDebug() << "select from pinyin_f";
    } else if ("g" == m_singleLetter) {
        if(m_query.exec(QString("select word from pinyin_g where pinyin like '" + m_inputLetter + "%'"))) {
            qDebug() << "m_query from pinyin_g successful!";
        } else {
            qDebug() << "m_query from pinyin_g fail!";
        }
        qDebug() << "select from pinyin_g";
    } else if ("h" == m_singleLetter) {
        if(m_query.exec(QString("select word from pinyin_h where pinyin like '" + m_inputLetter + "%'"))) {
            qDebug() << "m_query from pinyin_h successful!";
        } else {
            qDebug() << "m_query from pinyin_h fail!";
        }
        qDebug() << "select from pinyin_h";
    } else if ("i" == m_singleLetter) {
        if(m_query.exec(QString("select word from pinyin_i where pinyin like '" + m_inputLetter + "%'"))) {
            qDebug() << "m_query from pinyin_i successful!";
        } else {
            qDebug() << "m_query from pinyin_i fail!";
        }
        qDebug() << "select from pinyin_i";
    } else if ("j" == m_singleLetter) {
        if(m_query.exec(QString("select word from pinyin_j where pinyin like '" + m_inputLetter + "%'"))) {
            qDebug() << "m_query from pinyin_j successful!";
        } else {
            qDebug() << "m_query from pinyin_j fail!";
        }
        qDebug() << "select from pinyin_j";
    } else if ("k" == m_singleLetter) {
        if(m_query.exec(QString("select word from pinyin_k where pinyin like '" + m_inputLetter + "%'"))) {
            qDebug() << "m_query from pinyin_k successful!";
        } else {
            qDebug() << "m_query from pinyin_k fail!";
        }
        qDebug() << "select from pinyin_k";
    } else if ("l" == m_singleLetter) {
        if(m_query.exec(QString("select word from pinyin_l where pinyin like '" + m_inputLetter + "%'"))) {
            qDebug() << "m_query from pinyin_l successful!";
        } else {
            qDebug() << "m_query from pinyin_l fail!";
        }
        qDebug() << "select from pinyin_l";
    } else if ("m" == m_singleLetter) {
        if(m_query.exec(QString("select word from pinyin_m where pinyin like '" + m_inputLetter + "%'"))) {
            qDebug() << "m_query from pinyin_m successful!";
        } else {
            qDebug() << "m_query from pinyin_m fail!";
        }
        qDebug() << "select from pinyin_m";
    } else if ("n" == m_singleLetter) {
        if(m_query.exec(QString("select word from pinyin_n where pinyin like '" + m_inputLetter + "%'"))) {
            qDebug() << "m_query from pinyin_n successful!";
        } else {
            qDebug() << "m_query from pinyin_n fail!";
        }
        qDebug() << "select from pinyin_n";
    } else if ("o" == m_singleLetter) {
        if(m_query.exec(QString("select word from pinyin_o where pinyin like '" + m_inputLetter + "%'"))) {
            qDebug() << "m_query from pinyin_o successful!";
        } else {
            qDebug() << "m_query from pinyin_o fail!";
        }
        qDebug() << "select from pinyin_o";
    } else if ("p" == m_singleLetter) {
        if(m_query.exec(QString("select word from pinyin_p where pinyin like '" + m_inputLetter + "%'"))) {
            qDebug() << "m_query from pinyin_p successful!";
        } else {
            qDebug() << "m_query from pinyin_p fail!";
        }
        qDebug() << "select from pinyin_p";
    } else if ("q" == m_singleLetter) {
        if(m_query.exec(QString("select word from pinyin_q where pinyin like '" + m_inputLetter + "%'"))) {
            qDebug() << "m_query from pinyin_q successful!";
        } else {
            qDebug() << "m_query from pinyin_q fail!";
        }
        qDebug() << "select from pinyin_q";
    } else if ("r" == m_singleLetter) {
        if(m_query.exec(QString("select word from pinyin_r where pinyin like '" + m_inputLetter + "%'"))) {
            qDebug() << "m_query from pinyin_r successful!";
        } else {
            qDebug() << "m_query from pinyin_r fail!";
        }
        qDebug() << "select from pinyin_r";
    } else if (("s" == m_singleLetter) || ("sh" == m_singleLetter)) {
        if(m_query.exec(QString("select word from pinyin_s where pinyin like '" + m_inputLetter + "%'"))) {
            qDebug() << "m_query from pinyin_s successful!";
        } else {
            qDebug() << "m_query from pinyin_s fail!";
        }
        qDebug() << "select from pinyin_s";
    } else if ("t" == m_singleLetter) {
        if(m_query.exec(QString("select word from pinyin_t where pinyin like '" + m_inputLetter + "%'"))) {
            qDebug() << "m_query from pinyin_t successful!";
        } else {
            qDebug() << "m_query from pinyin_t fail!";
        }
        qDebug() << "select from pinyin_t";
    } else if ("u" == m_singleLetter) {
        if(m_query.exec(QString("select word from pinyin_u where pinyin like '" + m_inputLetter + "%'"))) {
            qDebug() << "m_query from pinyin_u successful!";
        } else {
            qDebug() << "m_query from pinyin_u fail!";
        }
        qDebug() << "select from pinyin_u";
    }  else if ("v" == m_singleLetter) {
        if(m_query.exec(QString("select word from pinyin_v where pinyin like '" + m_inputLetter + "%'"))) {
            qDebug() << "m_query from pinyin_v successful!";
        } else {
            qDebug() << "m_query from pinyin_v fail!";
        }
        qDebug() << "select from pinyin_v";
    } else if ("w" == m_singleLetter) {
        if(m_query.exec(QString("select word from pinyin_w where pinyin like '" + m_inputLetter + "%'"))) {
            qDebug() << "m_query from pinyin_w successful!";
        } else {
            qDebug() << "m_query from pinyin_w fail!";
        }
        qDebug() << "select from pinyin_w";
    } else if ("x" == m_singleLetter) {
        if(m_query.exec(QString("select word from pinyin_x where pinyin like '" + m_inputLetter + "%'"))) {
            qDebug() << "m_query from pinyin_x successful!";
        } else {
            qDebug() << "m_query from pinyin_x fail!";
        }
        qDebug() << "select from pinyin_x";
    } else if ("y" == m_singleLetter) {
        if(m_query.exec(QString("select word from pinyin_y where pinyin like '" + m_inputLetter + "%'"))) {
            qDebug() << "m_query from pinyin_y successful!";
        } else {
            qDebug() << "m_query from pinyin_y fail!";
        }
        qDebug() << "select from pinyin_y";
    } else if (("z" == m_singleLetter) || ("zh" == m_singleLetter)) {
        if(m_query.exec(QString("select word from pinyin_z where pinyin like '" + m_inputLetter + "%'"))) {
            qDebug() << "m_query from pinyin_z successful!";
        } else {
            qDebug() << "m_query from pinyin_z fail!";
        }
        qDebug() << "select from pinyin_z";
    }

    /**************** 将查询到的汉字插入到list链表中 ************/
    while(m_query.next()) {
        m_listChinese << m_query.value(0).toString();
    }

    /**************** 统计list中string的数量 ************/
    m_listCount = m_listChinese.count();
}

/*
 * 功能：
 * 	   默认状态
 *
 * 参数：
 * 	  无
 *
 * 返回值：
 * 	  无
 */
void SoftKeyPad::defaultStyle()
{
    p_buttonChinese->setStyleSheet("background-color: qlineargradient(x1: 0, y1: 0,"\
                                 " x2: 0, y2: 1, stop: 0 rgb(159, 222, 255), stop: 0.13 "\
                                 "rgb(20, 158, 249), stop: 0.14 rgb(0, 143, 248), stop: 1 "\
                                 "rgb(5, 117, 253));color: rgb(255, 255, 255);"\
                                 "border-width: 1px; border-style: solid; border-color: rgb(0, 7, 132);"\
                                 "border-radius: 10px; font-family:'Microsoft YaHei';font-size:30px;");
    p_buttonSymbol->setStyleSheet("background-color: qlineargradient(x1: 0, y1: 0,"\
                                " x2: 0, y2: 1, stop: 0 rgb(159, 222, 255), stop: 0.13 "\
                                "rgb(20, 158, 249), stop: 0.14 rgb(0, 143, 248), stop: 1 "\
                                "rgb(5, 117, 253));color: rgb(255, 255, 255);"\
                                "border-width: 1px; border-style: solid; border-color: rgb(0, 7, 132);"\
                                "border-radius: 10px; font-family:'Microsoft YaHei';font-size:30px;");
    p_buttonEnglish->setStyleSheet("background-color: qlineargradient(x1: 0, y1: 0,"\
                                 " x2: 0, y2: 1, stop: 0 rgb(159, 222, 255), stop: 0.13 "\
                                 "rgb(20, 158, 249), stop: 0.14 rgb(0, 143, 248), stop: 1 "\
                                 "rgb(5, 117, 253));color: rgb(255, 255, 255);"\
                                 "border-width: 1px; border-style: solid; border-color: rgb(0, 7, 132);"\
                                 "border-radius: 10px; font-family:'Microsoft YaHei';font-size:30px;");
}

/*
 * 功能：
 * 	   信号槽连接
 *
 * 参数：
 * 	  无
 *
 * 返回值：
 * 	  无
 */
void SoftKeyPad::connectFunC()
{
    connect(p_buttonCancel, SIGNAL(clicked()), this, SLOT(slotButtonCancelClicked()));
    connect(p_groupEnglish, SIGNAL(buttonClicked(int)), this, SLOT(slotGroupEnglishClicked(int)));
    connect(p_buttonEnglish, SIGNAL(clicked()), this, SLOT(slotButtonEnglishClicked()));
    connect(p_buttonChinese, SIGNAL(clicked()), this, SLOT(slotButtonChineseClicked()));
    connect(p_buttonSymbol, SIGNAL(clicked()), this, SLOT(slotButtonSymbolClicked()));
    connect(p_groupSymbol, SIGNAL(buttonClicked(int)), this, SLOT(slotGroupSymbolClicked(int)));
    connect(p_buttonBS, SIGNAL(pressed()), this, SLOT(slotButtonBSPressed()));
    connect(p_buttonBS, SIGNAL(released()), this, SLOT(slotButtonBSReleased()));
    connect(p_buttonDEL, SIGNAL(pressed()), this, SLOT(slotButtonDELPressed()));
    connect(p_buttonDEL, SIGNAL(released()), this, SLOT(slotButtonDELReleased()));
    connect(p_buttonLeftMove, SIGNAL(pressed()), this, SLOT(slotButtonMoveLeftPressed()));
    connect(p_buttonLeftMove, SIGNAL(released()), this, SLOT(slotButtonMoveLeftReleased()));
    connect(p_buttonRightMove, SIGNAL(pressed()), this, SLOT(slotButtonMoveRightPressed()));
    connect(p_buttonRightMove, SIGNAL(released()), this, SLOT(slotButtonMoveRightReleased()));
    connect(p_groupChinese, SIGNAL(buttonClicked(int)), this, SLOT(slotGroupChineseClicked(int)));
    connect(p_groupChineseShow, SIGNAL(buttonClicked(int)), this, SLOT(slotGroupChineseShowClicked(int)));
    connect(p_buttonOK, SIGNAL(clicked()), this, SLOT(slotButtonOKClicked()));
}

void SoftKeyPad::setDefaultStyleSheet()
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
                  "border-radius: 10px; font-family:'Microsoft YaHei';font-size:30px;}");
}
