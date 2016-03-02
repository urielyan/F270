/*********************************************************************
 * Copyright(c) 2015, 大豪信息技术有限公司上海研发部
 *
 * All Rights reserved
 *
 * 文件名称： winsaveloaddispladata.h
 * 概   要： 保存载入画面的实现,
 *
 * 当前版本：
 * 作   者： 闫葛
 * 完成日期：
 *
 * 修改版本：
 * 修改说明：
 * 修改作者：
 *
 ********************************************************************/
#ifndef DISPLAYDATA_H
#define DISPLAYDATA_H

#include "../Common/sysconfigslidingscreen.h"
#include <QWidget>
#include <QTreeView>
#include <QListView>
#include <QTableView>

#include <QFileSystemModel>
#include <QStackedLayout>
#include <QPushButton>
#include <QLabel>
#include <QHeaderView>

#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include <QTimer>
#include <QItemDelegate>
#include <QImage>

#define FONT_SIZE 20

#define FLAG_FIRST_VIEW 1
#define FLAG_SECOND_VIEW 2

class ButtonFactory;
class DisplayData;
class DisplayDataTableView;

//文件系统类:通过MVC模式显示文件系统中的文件,目录
class WinFileSystem : public QFrame
{
    Q_OBJECT
public:
    DisplayDataTableView *view;
    QFileSystemModel *model;

    explicit WinFileSystem(QWidget *parent = 0);
    void setFilter(QStringList f);
    void setFilter(QDir::Filters f);
    void refresh(){update();}
    void setRootPath(const QString &value);
    QString getRootPath() const;

    QString getCurrentModelIndexPath();
    QString getCurrentModelIndexName();

signals:
    void sigFileClicked();
    void sigDirClicked(QString dirPath);

protected:
    void paintEvent(QPaintEvent *);

private slots:
    void slotViewItemClicked(QModelIndex);

private:
    void setItemIcon();//设置QTableView的Item的图标

    QString currentDir;//记录当前目录
//    friend class DisplayData;
};

//展示文件系统的抽象类
class AbstractDisplayData : public QFrame
{
    Q_OBJECT
public:
    explicit AbstractDisplayData(QWidget *parent = 0);
    void setStackLayout(QStackedLayout *p_sl); //设置在主界面和展示文件系统界面间切换的布局QStackedLayout
    void setP_goalButton(ButtonFactory* button);

    //得到p_selectMemeoryButton按钮上是SD还是USB并设置为相应的文件系统
    bool setFileSystemBYButton(ButtonFactory* p_selectMemeoryButton);

    void setI_fileSystemDepth(int value);

public slots:
    void show();//切换界面为本界面
    void hide();//切换界面为mainWidget

protected:
    int i_fileSystemDepth;
    SysConfigSlidingScreen *p_slidingWidget;
    QStackedLayout *p_stackedLayout;//layout

    ButtonFactory *p_goalButton;//button

    ButtonFactory *p_bRemoveFile;//删除按钮
    ButtonFactory *p_bUpFolder;//返回上一级目录按钮
    ButtonFactory *p_bRefresh;//刷新按钮

    QLabel *labelCurrentDir;
    QLabel *labelMemoryInformation;

    QVBoxLayout *vLayout_main;
    QFrame *frameBottom;


    WinFileSystem *p_CurrentFileSystem; //记录最上层的文件系统对象
    QStack <WinFileSystem *> m_stack; //记录所有文件系统对象

    virtual void setRootPath(QString path);
    virtual WinFileSystem* getNewFileSystem(QString path);


    void removeSelectedFile(QFileSystemModel *model, QModelIndex index);
protected slots:
    virtual void onbRemoveFileClicked();
    virtual void onbUpfolderClicked();
    virtual void onbRefreshClicked(){}

    virtual void slotViewDirClicked(QString path);
    virtual void slotViewFileClicked();
    virtual void slotBOkClicked();
};

// 一个滑屏类里面有两个窗口，一个查看目录，一个查看文件夹中内容
class DisplayData : public AbstractDisplayData
{
    Q_OBJECT
public:
    static DisplayData* instance();
    ~DisplayData();

    void setm_NameFilterType(QString filterType);
    void setRootPath(QString /*path*/);//DODODO

    //选择文件的按钮加入到列表中
    void insertButtonToList(ButtonFactory *button);
    void examineFileIsExist();
    void examineSetButtonEnabled(bool enabled);


private slots:
    void slotViewFileClicked();//DODODO
    void slotViewDirClicked(QString path);    //DODODO

private:
    explicit DisplayData(QWidget *parent = 0);

    WinFileSystem* getNewFileSystem(QString path);
    void examineHadSetButtonIsExist();//检查m_hadSetButton的按钮中的文件是否还存在

    QList <ButtonFactory *> m_list;//store button that will display file path;
    QString m_NameFilterType;

    ButtonFactory *bOk;//ok button
};

//File list->execute
//文件一览的文件系统展示
class DisplayData_FileList : public AbstractDisplayData
{
    Q_OBJECT
public:
    static DisplayData_FileList* instance();

private:
    explicit DisplayData_FileList(QWidget *parent = 0);


};

//Load  setttings data->Above all->Select folder
//加载设置数据中以上所有的选择文件夹的文件系统展示
class DisplayData_Folder : public AbstractDisplayData
{
    Q_OBJECT
public:
    static DisplayData_Folder* instance();
    void setRootPath(QString path);//只需要使用原来的文件系统对象不需要重新生成一个对象

private slots:
    void slotViewDirClicked();//当选择文件夹时使能Ok按钮

private:
    explicit DisplayData_Folder(QWidget *parent = 0);
    ButtonFactory *p_bOk;
};


//Load  setttings data->Trusted Certificate->Load Trusted Certificate->select file
class DisplayData_LoadCertificate : public AbstractDisplayData
{
    Q_OBJECT
public:
    static DisplayData_LoadCertificate* instance();

private slots:
    void slotViewDirClicked(QString path);
    void slotViewFileClicked();
    WinFileSystem* getNewFileSystem(QString path);

private:
    explicit DisplayData_LoadCertificate(QWidget *parent = 0);
    ButtonFactory *p_BOk;
};

//Load  setttings data->Trusted Certificate->Delete Trusted Certificate->select file
class DisplayData_DeleteCertificate : public AbstractDisplayData
{
    Q_OBJECT
public:
    static DisplayData_DeleteCertificate* instance();
    static QString string_CertificatePath;//信赖证书固定路径
    static QString getNextStoreFilePath();
    static bool getCertificateIsExist();

    void show();//Disabled Ok按钮
private slots:
    void slotViewFileClicked();//只使能Ok按钮
    void slotBOkClicked();
private:
    explicit DisplayData_DeleteCertificate(QWidget *parent = 0);
    ButtonFactory *p_BOk;
};

//重写文件系统的代理类
class DelegateItem : public QItemDelegate{
    Q_OBJECT
public:
    //参数ｐ :  图标路径
    explicit DelegateItem(QString p,QWidget *parent = 0) :
        QItemDelegate(parent), imagePath(p){}

protected:
    void paint(QPainter *painter, const QStyleOptionViewItem &option,
         const QModelIndex & index) const;

private:
    QString imagePath;
};

//重写文件系统的TableView
class DisplayDataTableView : public QTableView
{
    Q_OBJECT
public:
    explicit DisplayDataTableView(QWidget *parent = 0);

protected:
    void mousePressEvent(QMouseEvent *event);
};
#endif // DISPLAYDATA_H

