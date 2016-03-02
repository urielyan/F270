#ifndef WINCOLIBRATIONBUTTON_H
#define WINCOLIBRATIONBUTTON_H

#include <QPushButton>
#include <QPixmap>
#include <QLabel>
#include <QVariant>
#include <QHBoxLayout>

//初始化模块所有菜单项按钮封装
class WinColibrationButton : public QPushButton
{
    Q_OBJECT
public:
    enum ButtonType{
        BUTTON_CLASS_TYPE,
        BUTTON_NORMAL_TYPE,
        BUTTON_GROUP_TYPE,
        BUTTON_ITEM_TYPE
    };
    explicit WinColibrationButton(QWidget *parent = 0);
    static WinColibrationButton *getButton(int type);

    //设置菜单按钮左侧图标
    virtual void setPixmap(QPixmap /*icon*/){}
    //设置菜单按钮名
    virtual void setName(QString /*name*/){}
    //设置菜单按钮值
    virtual void setValueText(QString /*value*/){}
    //设置菜单按钮数据
    virtual void setData(QVariant /*data*/){}

    //获取菜单按钮值
    virtual QString getValueText(){return "";}
    //获取数据
    virtual QVariant getData(){return QVariant();}

signals:
    void sigDataChanged(QVariant data);

};
//左侧导航菜单按钮
class WinClassButton: public WinColibrationButton
{
    Q_OBJECT
public:
    explicit WinClassButton(QWidget *parent = 0);
    virtual void setPixmap(QPixmap icon){m_icon.setPixmap(icon);}
    virtual void setName(QString name){m_name.setText(name);}

private:
    QHBoxLayout m_layout;

    QLabel m_icon;
    QLabel m_name;
};
//分组按钮（不能被点击）
class WinGroupButton: public WinColibrationButton
{
    Q_OBJECT
public:
    explicit WinGroupButton(QWidget *parent = 0);
    virtual void setName(QString name){m_name.setText(name);}

private:
    QHBoxLayout m_layout;

    QLabel m_name;
};
//标准按钮
class WinNormalButton: public WinColibrationButton
{
    Q_OBJECT
public:
    explicit WinNormalButton(QWidget *parent = 0);
    virtual void setName(QString name){m_name.setText(name);}
    virtual void setValueText(QString value){m_value.setText(value);}
    virtual QString getValueText(){ return m_value.text();}
    virtual void setData(QVariant data){m_data = data;emit sigDataChanged(m_data);}
    virtual QVariant getData(){return m_data;}

private:
    QHBoxLayout m_layout;

    QLabel m_name;
    QLabel m_value;
    QVariant m_data;
};
//选择项按钮
class WinItemButton: public WinColibrationButton
{
    Q_OBJECT
public:
    explicit WinItemButton(QWidget *parent = 0);
    virtual void setValueText(QString value){setText(value);}
    virtual QString getValueText(){ return text();}
    virtual void setData(QVariant data){m_data = data;}
    virtual QVariant getData(){return m_data;}

private:
//    QHBoxLayout m_layout;
//    QLabel *p_value;
    QVariant m_data;
};

#endif // WINCOLIBRATIONBUTTON_H
