//*********************************************************************
// * Copyright(c) 2015, 大豪信息技术有限公司上海研发部
// *
// * All Rights reserved
// *
// * 文件名称：winsaveloadframe.h
// * 概   要：保存载入画面中按钮的实现
// *
// * 当前版本：
// * 作   者：闫葛
// * 完成日期：
// *
// * 修改版本：
// * 修改说明：
// * 修改作者：
// *
// ********************************************************************/

#ifndef WINSAVELOADFRAME_H
#define WINSAVELOADFRAME_H

#include <QPushButton>
#include <QFrame>
#include <QLabel>
#include <QWidget>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QScrollArea>

class ButtonFactory : public QPushButton
{
    Q_OBJECT
public:
    enum BUTTON_TYPE{
        BUTTON_PIXMAPLABEL = 0,
        BUTTON_LABELLABEL,
        BUTTON_LABELLABELDISABLED,
        BUTTON_LABELPIXMAP,
        BUTTON_LABELPIXMAP_SMALL,
        BUTTON_LABEL,
        BUTTON_PIXMAP,
    };
    static ButtonFactory* instance()
    {
        static ButtonFactory Instance;
        return &Instance;
    }
    ButtonFactory(QWidget *parent = 0);
    static ButtonFactory* getButton(QString text, BUTTON_TYPE type, QWidget *parent = 0,QPixmap *pixmap = 0);
    virtual void setRightLabelText(QString /*text*/){

    }
    virtual QString getRightLabelText(){
        return NULL;
    }
    virtual void setPixmap(QPixmap *){}
};



//界面中左边菜单的按钮实现
class Button_label : public ButtonFactory
{
    Q_OBJECT
public:
    explicit Button_label(QString text, QWidget *parent = 0);
};

//一个按钮上含有两个label的实现
class Button_labelLabel : public ButtonFactory
{
    Q_OBJECT
public:
    //构造函数，l_left指定左标签的内容
    explicit Button_labelLabel(QString l_left, QWidget *parent = 0);
    //设置右标签的内容
    void setRightLabelText(QString value);
    QString getRightLabelText(){return labelRight->text();}
signals:
    void sigRigetTextChanged();
private:
    //一个按钮放上两个标签
    QLabel *labelRight;
    QLabel *labelLeft;
};

//一个按钮上含有两个label的实现
class Button_labelLabelDisabled : public ButtonFactory
{
    Q_OBJECT
public:
    //构造函数，l_left指定左标签的内容
    explicit Button_labelLabelDisabled(QString l_left, QWidget *parent = 0);
    //设置右标签的内容
    void setRightLabelText(QString value)
    {
        labelRight->setText(value);
    }
    QString getRightLabelText(){return labelRight->text();}
private:
    //一个按钮放上两个标签
    QLabel *labelRight;
    QLabel *labelLeft;
};

//左边一个图标右边一个文字
class Button_pixmapLabel : public ButtonFactory
{
    Q_OBJECT
public:
    explicit Button_pixmapLabel(QString text, QPixmap *pixmap, QWidget *parent);
private slots:
    void slottoggled(bool value);
};

class Button_pixmapLabelSmall : public ButtonFactory
{
    Q_OBJECT
public:
    explicit Button_pixmapLabelSmall(QString text, QPixmap *pixmap, QWidget *parent);
};

class Button_labelPixmap : public ButtonFactory
{
    Q_OBJECT
public:
    explicit Button_labelPixmap(QString text, QPixmap *pixmap, QWidget *parent);
};

class Button_pixmap : public ButtonFactory
{
    Q_OBJECT
public:
    explicit Button_pixmap(QPixmap *pixmap, QWidget *parent);
};

#endif
