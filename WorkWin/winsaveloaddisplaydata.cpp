#include "winsaveloaddisplaydata.h"
#include "winsaveload_buttonFactory.h"
#include "winsaveloadmainframe.h"
#include "winsaveload.h"

#include "wininfolistdialog.h"

#include "unistd.h"
#include <QDebug>
#include <QMessageBox>
#include <QDateTime>
#include<QScroller>

AbstractDisplayData::AbstractDisplayData(QWidget *parent) :
    QFrame(parent)
{
    p_slidingWidget = new SysConfigSlidingScreen;
    p_slidingWidget->setStyleSheet("margin:0px;border-width:0px;");
    p_slidingWidget->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum);
    p_slidingWidget->setMaximumHeight(10000);
    p_slidingWidget->setMaximumWidth(10000);

    vLayout_main = new QVBoxLayout(this);
    vLayout_main->setSpacing(0);
    vLayout_main->setMargin(0);
    vLayout_main->setSpacing(0);
    vLayout_main->setContentsMargins(0, 0, 0, 0);

    QHBoxLayout *hLayout_up = new QHBoxLayout();
    hLayout_up->setSpacing(0);
    hLayout_up->setContentsMargins(0, 0, 0, 0);

    labelCurrentDir = new QLabel(this);
    labelCurrentDir->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Maximum);
    labelCurrentDir->setObjectName("labelCurrentDir");
    hLayout_up->addWidget(labelCurrentDir, Qt::AlignLeft);

    p_bUpFolder = ButtonFactory::instance()->getButton("", ButtonFactory::BUTTON_PIXMAP,
                                                     this, new QPixmap(":/saveload/Image/saveload/upfolder.png"));
    hLayout_up->addWidget(p_bUpFolder, Qt::AlignRight);
    connect(p_bUpFolder,SIGNAL(clicked(bool)),this,SLOT(onbUpfolderClicked()));

    p_bRefresh = ButtonFactory::instance()->getButton("", ButtonFactory::BUTTON_PIXMAP,
                                                     this, new QPixmap(":/saveload/Image/saveload/refresh.png"));
    hLayout_up->addWidget(p_bRefresh, Qt::AlignRight);
    connect(p_bRefresh,SIGNAL(clicked(bool)),this,SLOT(onbRefreshClicked()));

    p_bRemoveFile = ButtonFactory::instance()->getButton("", ButtonFactory::BUTTON_PIXMAP,
                                                     this, new QPixmap(":/saveload/Image/saveload/delete.png"));
    hLayout_up->addWidget(p_bRemoveFile, Qt::AlignRight);
    connect(p_bRemoveFile,SIGNAL(clicked(bool)),this,SLOT(onbRemoveFileClicked()));


    frameBottom = new QFrame(this);
    frameBottom->setStyleSheet("background-color:rgba(0,0,0,0);");
    frameBottom->setFixedHeight(75);

    vLayout_main->addLayout(hLayout_up);
    vLayout_main->addWidget(p_slidingWidget);
    vLayout_main->addWidget(frameBottom);
    vLayout_main->addWidget(frameBottom);

    labelMemoryInformation = new QLabel(this);
    labelMemoryInformation->setAlignment(Qt::AlignCenter);
    labelMemoryInformation->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);

    this->setStyleSheet("AbstractDisplayData{"
                            "background-color: rgb(255,255,255,255);"
                        "}"
                        "QLabel{"
                            "margin:0px,0px,0px,0px;"
                            "color:black;"
                            "font-size:15px;"
                        "}");

}

void AbstractDisplayData::setStackLayout(QStackedLayout *p_sl)
{
    p_stackedLayout = p_sl;
}


//切换界面为本界面
void AbstractDisplayData::show(){
    //当既没有sd卡也没有usb存储设备时不显示
    if(!QDir(WinSelectMemory::SD_PATH).exists() && !QDir(WinSelectMemory::USB_PATH).exists())
    {
        return;
    }
    p_stackedLayout->setCurrentWidget(this);
}

//切换界面为mainWidget
void AbstractDisplayData::hide()
{
    p_stackedLayout->setCurrentIndex(0);
}

void AbstractDisplayData::setI_fileSystemDepth(int value)
{
    i_fileSystemDepth = value;
}

WinFileSystem *AbstractDisplayData::getNewFileSystem(QString path)
{
    WinFileSystem *p_new_fileSystem = new WinFileSystem;
    p_new_fileSystem->setRootPath(path);
    connect(p_new_fileSystem,SIGNAL(sigDirClicked(QString)),this,SLOT(slotViewDirClicked(QString)));
    connect(p_new_fileSystem,SIGNAL(sigFileClicked()),this,SLOT(slotViewFileClicked()));
    m_stack.push(p_new_fileSystem);
    p_CurrentFileSystem = p_new_fileSystem;
    labelCurrentDir->setText(path);
    if(m_stack.size() == 1)
        p_bUpFolder->setDisabled(true);
    return p_new_fileSystem;
}

void AbstractDisplayData::slotViewDirClicked(QString path)
{
    if(i_fileSystemDepth == m_stack.size())
        return;
    getNewFileSystem(path);
    p_slidingWidget->setNextWidget(p_CurrentFileSystem);
    p_slidingWidget->moveToNextWidget();
    p_bUpFolder->setDisabled(false);
    p_bRemoveFile->setDisabled(true);
}

void AbstractDisplayData::slotViewFileClicked()
{
    p_bRemoveFile->setDisabled(false);
}

void AbstractDisplayData::slotBOkClicked()
{
    p_goalButton->setRightLabelText(p_CurrentFileSystem->getCurrentModelIndexPath());
    this->hide();
    winsaveload::setp_buttonExecuteDisabled();//当选择文件后检查设置按钮状态
}

void AbstractDisplayData::removeSelectedFile(QFileSystemModel *model, QModelIndex index)
{
    WinSureOperateDialog sureDialog(tr("Delete this file?"));
    if(QDialog::Accepted == sureDialog.exec())
    {
        bool ok = model->remove(index);
        if(true == ok)
        {
            WinInfoListDialog::instance()->showMsg(801);
        }else
        {
            WinInfoListDialog::instance()->showMsg(572);
        }
    }
}

//remove slected file
void AbstractDisplayData::onbRemoveFileClicked()
{
    QFileSystemModel *model = m_stack.top()->model;
    QModelIndex index = m_stack.top()->view->currentIndex();
    if (!index.isValid() | model->fileInfo(index).isDir()) {
        //p_label->setText(tr("File deleted err"));
        WinInformationDialog dialog(tr("File deleted err"));
        dialog.exec();
    }else {
        removeSelectedFile(model, index);
    }
}

/**
 * 功能：返回到上一目录
 */
void AbstractDisplayData::onbUpfolderClicked()
{
    p_slidingWidget->moveToPreWidget();

    m_stack.pop();

    labelCurrentDir->setText(m_stack.top()->getRootPath());
    if(m_stack.size() == 1)
    {
        p_bUpFolder->setDisabled(true);
    }
}

void AbstractDisplayData::setRootPath(QString path)
{
    m_stack.clear();

    getNewFileSystem(path);
    p_slidingWidget->setInitWidget(p_CurrentFileSystem);
    p_bUpFolder->setDisabled(true);
    p_bRemoveFile->setDisabled(true);
}

//设置目标按钮
void AbstractDisplayData::setP_goalButton(ButtonFactory *button)
{
    p_goalButton = button;
}

/**
 * 功能：
 *          得到p_selectMemeoryButton按钮上是SD还是USB并设置为相应的文件系统
 * 参数：
 *          p_selectMemeoryButton: 选择文件系统的按钮
 *
 * 返回值：
 *          bool:
 *              true: success
 *              false: can't show file system
 */
bool AbstractDisplayData::setFileSystemBYButton(ButtonFactory *p_selectMemeoryButton)
{
    if(0 == p_selectMemeoryButton->getRightLabelText().compare(WinSelectMemory::SD))
    {
        if(WinSelectMemory::instance()->sdIsExist())
        {
            labelMemoryInformation->setText("Sd memory information");
            labelCurrentDir->setText(WinSelectMemory::SD_PATH);
            setRootPath(WinSelectMemory::SD_PATH);
        }else
        {
            WinInfoListDialog::instance()->showMsg(503);
            return false;
        }
    }else if(0 == p_selectMemeoryButton->getRightLabelText().compare(WinSelectMemory::USB))
    {
        if(WinSelectMemory::instance()->usbIsExist())
        {
            labelMemoryInformation->setText("Usb memory information");
            labelCurrentDir->setText(WinSelectMemory::USB_PATH);
            setRootPath(WinSelectMemory::USB_PATH);
        }else
        {
            WinInfoListDialog::instance()->showMsg(503);
            return false;
        }
    }else
    {
        return false;
    }
    return true;
}



DisplayData_Folder *DisplayData_Folder::instance()
{
    static DisplayData_Folder Instance;
    return &Instance;
}

void DisplayData_Folder::setRootPath(QString path)
{
    p_CurrentFileSystem->setRootPath(path);
    p_slidingWidget->setInitWidget(p_CurrentFileSystem);
    p_bUpFolder->setDisabled(true);
    p_bRemoveFile->setDisabled(true);
}

void DisplayData_Folder::slotViewDirClicked()
{
    p_bOk->setDisabled(false);
}

DisplayData_Folder::DisplayData_Folder(QWidget *parent)
    :AbstractDisplayData(parent)
{
    //只含有一个文件系统类，并保持不变，设置只显示文件夹
    p_CurrentFileSystem = new WinFileSystem();
    p_CurrentFileSystem->setFilter(QDir::Dirs);

    setRootPath(WinSelectMemory::SD_PATH);

    p_bOk =  ButtonFactory::getButton
            (tr("Ok"), ButtonFactory::BUTTON_LABELPIXMAP_SMALL, frameBottom);
    connect(p_bOk,SIGNAL(clicked(bool)),this,SLOT(slotBOkClicked()));

    ButtonFactory *bCancel = ButtonFactory::getButton
            (tr("Cancel"), ButtonFactory::BUTTON_LABELPIXMAP_SMALL,
                      frameBottom, new QPixmap(":/setting/Image/setting/cancel_icon.png"));
    connect(bCancel,SIGNAL(clicked(bool)),this,SLOT(hide()));

    connect(p_CurrentFileSystem,SIGNAL(sigDirClicked(QString)),this,SLOT(slotViewDirClicked()));

    QHBoxLayout *hLayout_bottom = new QHBoxLayout(frameBottom);
    hLayout_bottom->addWidget(bCancel);
    hLayout_bottom->addWidget(labelMemoryInformation);
    hLayout_bottom->addWidget(p_bOk);
}

/**
 * 功能：
 *          一个滑屏类里面有两个窗口，一个查看目录，一个查看文件夹中内容
 * 参数：
 *
 * 返回值：
 *
 */
DisplayData *DisplayData::instance()
{
    static DisplayData Instance;
    return &Instance;
}

DisplayData::DisplayData(QWidget *parent) : AbstractDisplayData(parent)
{
    i_fileSystemDepth = 2;

    ButtonFactory *bCancel = ButtonFactory::getButton
            (tr("Cancel"), ButtonFactory::BUTTON_LABELPIXMAP_SMALL,
                      frameBottom, new QPixmap(":/setting/Image/setting/cancel_icon.png"));
    connect(bCancel,SIGNAL(clicked(bool)),this,SLOT(hide()));

    bOk =  ButtonFactory::getButton
            (tr("Ok"), ButtonFactory::BUTTON_LABELPIXMAP_SMALL, frameBottom);
    connect(bOk,SIGNAL(clicked(bool)),this,SLOT(slotBOkClicked()));

    QHBoxLayout *hLayout_bottom = new QHBoxLayout(frameBottom);
    hLayout_bottom->addWidget(bCancel);
    hLayout_bottom->addWidget(labelMemoryInformation);
    hLayout_bottom->addWidget(bOk);
}

WinFileSystem *DisplayData::getNewFileSystem(QString path)
{
    AbstractDisplayData::getNewFileSystem(path);

    QStringList filters;
    filters << "*." + m_NameFilterType;
    p_CurrentFileSystem->setFilter(filters);

    return p_CurrentFileSystem;
}

//检查曾经被设置过的按钮中的文件是否还存在
void DisplayData::examineHadSetButtonIsExist()
{
    foreach (ButtonFactory *button, m_list) {
        if(QFile(button->getRightLabelText()).exists() || QDir(button->getRightLabelText()).exists())
        {
            continue;
        }else
        {
            button->setRightLabelText("");
        }
    }
}

DisplayData::~DisplayData()
{
}

/**
 * 功能：
 *          设置过滤器
 * 参数：
 *          filterType: 文件后缀名
 */
void DisplayData::setm_NameFilterType(QString filterType)
{
    m_NameFilterType = filterType;
}

// 点击文件时设置按钮状态
void DisplayData::slotViewFileClicked()
{
    AbstractDisplayData::slotViewFileClicked();
    bOk->setDisabled(false);
}

void DisplayData::slotViewDirClicked(QString path)
{
    AbstractDisplayData::slotViewDirClicked(path);
    bOk->setDisabled(true);
}

void DisplayData::setRootPath(QString path)
{
    AbstractDisplayData::setRootPath(path);
    bOk->setDisabled(true);
}

void DisplayData::insertButtonToList(ButtonFactory *button)
{
    if(!m_list.contains(button))
        m_list.append(button);
}

void DisplayData::examineFileIsExist()
{
    foreach (ButtonFactory *object, m_list) {
        if(!QFile(object->getRightLabelText()).exists())
        {
            object->setRightLabelText("");
        }
    }
}

void DisplayData::examineSetButtonEnabled(bool enabled)
{
    foreach (ButtonFactory *object, m_list) {
        object->setEnabled(enabled);
    }
}

DisplayData_FileList *DisplayData_FileList::instance()
{
    static DisplayData_FileList Instance;
    return &Instance;
}

DisplayData_FileList::DisplayData_FileList(QWidget *parent)
    : AbstractDisplayData(parent)
{
    ButtonFactory *bOk =  ButtonFactory::getButton
            (tr("Ok"), ButtonFactory::BUTTON_LABELPIXMAP_SMALL, frameBottom);
    connect(bOk,SIGNAL(clicked(bool)),this,SLOT(hide()));

    QHBoxLayout *hLayout_bottom = new QHBoxLayout(frameBottom);
    hLayout_bottom->addWidget(labelMemoryInformation);
    hLayout_bottom->addWidget(bOk);

    setRootPath(WinSelectMemory::SD_PATH);
}



/**
 * 功能：文件系统ＭＶＣ
 */
WinFileSystem::WinFileSystem(QWidget *parent): QFrame(parent)
{
    currentDir = QString(WinSelectMemory::SD_PATH);



    model = new QFileSystemModel(this);
    model->setNameFilterDisables(false);//没有通过过滤器的文件disable还是隐藏,true为disable false为隐藏
    model->setReadOnly(false);//使能修该也即使能删除
    model->setHeaderData(0, Qt::Horizontal, tr("name"));
    model->setHeaderData(3, Qt::Horizontal, tr("time"));

    view = new DisplayDataTableView(this);
    view->setModel(model);

    QVBoxLayout *mainLayout= new QVBoxLayout(this);
    mainLayout->setSpacing(0);
    mainLayout->setMargin(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addWidget(view);

    connect(view,SIGNAL(clicked(QModelIndex)),
            this,SLOT(slotViewItemClicked(QModelIndex)));

}

QString WinFileSystem::getRootPath() const
{
    return currentDir;
}

/**
 * 功能：设置文件名过滤器和文件类型过滤器
 */
void WinFileSystem::setFilter(QStringList f)
{
    model->setNameFilters(f);
}

void WinFileSystem::setFilter(QDir::Filters f)
{
    model->setFilter(f | QDir::NoDotAndDotDot);
}


/**
 * 功能：
 *          设置当前文件系统路径
 * 参数：
 *          value : 路径
 * 返回值：
 *
 */
void WinFileSystem::setRootPath(const QString &value)
{
    qDebug() << value.at(value.count() - 1) << value << (value.at(value.count() - 1) == QChar('/'));
    if(value.at(value.count() - 1) == QChar('/'))
    {
        currentDir = value;

    }else{
        currentDir == value + "/";
    }
    model->setRootPath(value);
    view->setRootIndex(model->index(value));
    model->sort(0,Qt::AscendingOrder);
}

//设置QTableView的Item的图标
void WinFileSystem::setItemIcon()
{
    static DelegateItem *fdelegateFolder = new DelegateItem(":/saveload/Image/saveload/folder.png",this);
    static DelegateItem *fdelegateFile = new DelegateItem(":/saveload/Image/saveload/file.png",this);

    for(int i = 0; i < model->rowCount(model->index(currentDir)); i++)
    {
        if(model->fileInfo(model->index(i,0,model->index(currentDir))).isDir()){
            view->setItemDelegateForRow(i,fdelegateFolder);
        }
        if(model->fileInfo(model->index(i,0,model->index(currentDir))).isFile()){
            view->setItemDelegateForRow(i,fdelegateFile);
        }
    }
}

//返回被选中的列的完整路径
QString WinFileSystem::getCurrentModelIndexPath()
{
    QModelIndex index = view->currentIndex();
    return model->filePath(index);
}

QString WinFileSystem::getCurrentModelIndexName()
{
    QModelIndex index = view->currentIndex();
    return model->fileName(index);
}

/**
 * 功能：当点击Tableview时判读是目录还是文件并做出相应动作
 */
void WinFileSystem::slotViewItemClicked(QModelIndex)
{
    QModelIndex index = view->currentIndex();

    if (!index.isValid()) {
        return;
    }
    if (model->fileInfo(index).isDir()) {
        emit sigDirClicked(model->filePath(index) + "/");
    }else{
        emit sigFileClicked();
    }
}

void WinFileSystem::paintEvent(QPaintEvent *)
{
    view->setColumnWidth(0,view->width() *2/3);
    view->setColumnWidth(1, 0);
    view->setColumnWidth(2, 0);
    view->setColumnWidth(3,view->width() /3);
    setItemIcon();
}


/**
 * 功能：
 *          重写基类中的paint 方法
 *          修改文件系统第一列文件名的图标，显示文件名;改写文件系统第三列时间格式.
 */
void DelegateItem::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(0 == index.column()){
        QImage image;
        image.load(imagePath);
        int x = option.rect.x();
        int y = option.rect.y();
        int width = option.rect.width();
        int height = option.rect.height();
        painter->setPen(QColor(63,63,63,255));

        painter->drawText(x + (height - image.width()) / 2 + image.width() + 40,
                          y + (height - image.height()) / 2 - 5,
                          width,height + 5,0,index.data().toString());
        painter->drawImage(x + (height - image.width()) / 2 +10,
                           y + (height - image.height()) / 2 ,
                           image, 0, 0, image.width(), image.height());
    }else if(3 == index.column()){
        int x = option.rect.x();
        int y = option.rect.y();
        int width = option.rect.width();
        int height = option.rect.height();
        painter->setPen(QColor(63,63,63,255));

        QDateTime dateTime = QDateTime::fromString(index.data().toString(),"M/d/yy h:m AP");
        painter->drawText(x , y , width, height, 0,
                          dateTime.toString("yyyy/MM/dd     hh:mm"));
    }
}



DisplayData_LoadCertificate *DisplayData_LoadCertificate::instance()
{
    static DisplayData_LoadCertificate Instance;
    return &Instance;
}

DisplayData_LoadCertificate::DisplayData_LoadCertificate(QWidget *parent)
    :AbstractDisplayData(parent)
{
    i_fileSystemDepth = 3;

    ButtonFactory *bCancel = ButtonFactory::getButton
            (tr("Cancel"), ButtonFactory::BUTTON_LABELPIXMAP_SMALL,
                      frameBottom, new QPixmap(":/setting/Image/setting/cancel_icon.png"));
    connect(bCancel,SIGNAL(clicked(bool)),this,SLOT(hide()));

    p_BOk =  ButtonFactory::getButton
            (tr("Ok"), ButtonFactory::BUTTON_LABELPIXMAP_SMALL, frameBottom);
    connect(p_BOk,SIGNAL(clicked(bool)),this,SLOT(slotBOkClicked()));

    QHBoxLayout *hLayout_bottom = new QHBoxLayout(frameBottom);
    hLayout_bottom->addWidget(bCancel);
    hLayout_bottom->addWidget(labelMemoryInformation);
    hLayout_bottom->addWidget(p_BOk);

    setRootPath(WinSelectMemory::SD_PATH);
}

void DisplayData_LoadCertificate::slotViewDirClicked(QString path)
{
    AbstractDisplayData::slotViewDirClicked(path);
    p_BOk->setDisabled(true);
}

void DisplayData_LoadCertificate::slotViewFileClicked()
{
    AbstractDisplayData::slotViewFileClicked();
    p_BOk->setEnabled(true);
}

WinFileSystem *DisplayData_LoadCertificate::getNewFileSystem(QString path)
{
    AbstractDisplayData::getNewFileSystem(path);

    QStringList filters;
    filters <<"*.cer";
    p_CurrentFileSystem->setFilter(filters);

    return p_CurrentFileSystem;
}

QString DisplayData_DeleteCertificate::string_CertificatePath = "/home/yange/certificate/";
DisplayData_DeleteCertificate *DisplayData_DeleteCertificate::instance()
{
    static DisplayData_DeleteCertificate Instance;
    return &Instance;
}

QString DisplayData_DeleteCertificate::getNextStoreFilePath()
{
    QFile file(DisplayData_DeleteCertificate::string_CertificatePath + "nextFileName");
    if (!file.open(QIODevice::ReadWrite | QIODevice::Text))
        return NULL;

    QTextStream textStream(&file);
    QString fileName= textStream.readAll();
    file.resize(0);
    qDebug() << fileName;
    if(fileName.size() == 0)
    {
        textStream << "2";
        file.close();
        return DisplayData_DeleteCertificate::string_CertificatePath + "001_.cer";
    }

    int nextNumber = fileName.toUInt();
    QString returnName;
    if(nextNumber < 10)
    {
        returnName = "00" + QString::number(nextNumber) + "_.cer";
    }else if(nextNumber < 100)
    {
        returnName = "0" + QString::number(nextNumber) + "_.cer";
    }else
    {
        returnName = QString::number(nextNumber) + "_.cer";
    }

    textStream << ++nextNumber;
    file.close();
    return DisplayData_DeleteCertificate::string_CertificatePath + returnName;
}

bool DisplayData_DeleteCertificate::getCertificateIsExist()
{
    QStringList filters;
    filters <<"*.cer";
    QStringList allEntry = QDir(string_CertificatePath).entryList(filters);
    if(allEntry.size() > 0)
        return true;
    else
        return false;
}


void DisplayData_DeleteCertificate::show()
{
    AbstractDisplayData::show();
    p_BOk->setDisabled(true);
}

void DisplayData_DeleteCertificate::slotViewFileClicked()
{
    p_BOk->setEnabled(true);
}

void DisplayData_DeleteCertificate::slotBOkClicked()
{
    p_goalButton->setRightLabelText(p_CurrentFileSystem->getCurrentModelIndexName());

    this->hide();
    winsaveload::setp_buttonExecuteDisabled();//当选择文件后检查设置按钮状态
}

DisplayData_DeleteCertificate::DisplayData_DeleteCertificate(QWidget *parent)
    :AbstractDisplayData(parent)
{
    i_fileSystemDepth = 1;


    ButtonFactory *bCancel = ButtonFactory::getButton
            (tr("Cancel"), ButtonFactory::BUTTON_LABELPIXMAP_SMALL,
                      frameBottom, new QPixmap(":/setting/Image/setting/cancel_icon.png"));
    connect(bCancel,SIGNAL(clicked(bool)),this,SLOT(hide()));

    p_BOk =  ButtonFactory::getButton
            (tr("Ok"), ButtonFactory::BUTTON_LABELPIXMAP_SMALL, frameBottom);
    connect(p_BOk,SIGNAL(clicked(bool)),this,SLOT(slotBOkClicked()));

    QHBoxLayout *hLayout_bottom = new QHBoxLayout(frameBottom);
    hLayout_bottom->addWidget(bCancel);
    hLayout_bottom->addWidget(labelMemoryInformation);
    hLayout_bottom->addWidget(p_BOk);

    getNewFileSystem(string_CertificatePath);
    p_slidingWidget->setInitWidget(p_CurrentFileSystem);
    p_bUpFolder->setDisabled(true);
    p_bRemoveFile->setDisabled(true);
    p_CurrentFileSystem->setFilter(QDir::Files);
    QStringList filters;
    filters <<"*.cer";
    p_CurrentFileSystem->setFilter(filters);

    p_bUpFolder->setDisabled(true);
    p_bRemoveFile->setDisabled(true);
    p_bRefresh->setDisabled(true);

    labelCurrentDir->setText("");
}

DisplayDataTableView::DisplayDataTableView(QWidget *parent) : QTableView(parent)
{
    this->verticalHeader()->hide();
    this->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    this->horizontalHeader()->setStretchLastSection(true);
    this->horizontalHeader()->setSortIndicator(3, Qt::AscendingOrder);
    this->horizontalHeader()->setSortIndicatorShown(true);
    this->setFont(QFont("saria",FONT_SIZE));
    this->setGridStyle(Qt::NoPen);
    this->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setSortingEnabled(true);
    this->setStyleSheet("background-color: rgb(255,255,255,255);\
                            margin:0px,0px,0px,0px;\
                            border:0px solid rgb(255,255,255,255);\
                           ");
    this->setForegroundRole(QPalette::Text);


    /*
     * 添加滑屏操作
    */
    this->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);

    QScroller *scroller = QScroller::scroller(this->viewport());

    QScrollerProperties prop = scroller->scrollerProperties();

    prop.setScrollMetric(QScrollerProperties::AxisLockThreshold, 0.66);
    prop.setScrollMetric(QScrollerProperties::ScrollingCurve, QEasingCurve(QEasingCurve::OutExpo));
    prop.setScrollMetric(QScrollerProperties::DecelerationFactor, 0.05);
    prop.setScrollMetric(QScrollerProperties::MaximumVelocity, 0);
    prop.setScrollMetric(QScrollerProperties::OvershootDragResistanceFactor, 0.33);
    prop.setScrollMetric(QScrollerProperties::OvershootScrollDistanceFactor, 0.33);
    prop.setScrollMetric(QScrollerProperties::SnapPositionRatio, 0.93);
    prop.setScrollMetric(QScrollerProperties::DragStartDistance, 0.001);
    prop.setScrollMetric(QScrollerProperties::SnapPositionRatio,0.5);

    scroller->setScrollerProperties(prop);

    scroller->grabGesture(this, QScroller::TouchGesture);
    scroller->grabGesture(this, QScroller::LeftMouseButtonGesture);
}

void DisplayDataTableView::mousePressEvent(QMouseEvent *event)
{
    QTableView::mousePressEvent(event);
    qDebug() << __LINE__ << __FUNCTION__;
}
