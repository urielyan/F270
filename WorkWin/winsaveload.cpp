#include "winsaveload.h"
#include "winsaveload_buttonFactory.h"
#include "winsaveloaddisplaydata.h"

#include "winsaveloadmainframe.h"

#include <unistd.h>
#include <QThread>
#include <QHBoxLayout>
/**
 * 功能：
 *          保存载入画面主窗口构造函数
 * 参数：
 *
 * 返回值：
 *
 */

ButtonFactory* winsaveload::p_buttonExecute = NULL;
QStack<WinSvLdFrame*> winsaveload::m_StackExecuteButton;
QStackedLayout winsaveload::m_stackLayout;

winsaveload::winsaveload(QWidget *parent) : QWidget(parent)
{
    QFrame *frameBottom = new QFrame(this);
    frameBottom->setStyleSheet("background-color:rgba(0,0,0,0);");
    frameBottom->setFixedHeight(75);
    p_buttonExecute = ButtonFactory::getButton
            (tr("Execute"), ButtonFactory::BUTTON_LABELPIXMAP_SMALL);
    //m_stackLayout.addWidget(p_buttonExecute);

    QFrame *mainWidget = new QFrame(this);
    mainWidget->setObjectName("mainWidget");

    QHBoxLayout *hLayout_top = new QHBoxLayout;
    hLayout_top->setContentsMargins(0,0,0,0);
    hLayout_top->setSpacing(0);
    hLayout_top->setMargin(0);
    hLayout_top->addWidget(new WinLeftButtonPanel(this));
    hLayout_top->addWidget(WinSaveLoadMainFrame::instance());

    ButtonFactory *p_buttonCancel = ButtonFactory::getButton
            (tr("Cancel"), ButtonFactory::BUTTON_LABELPIXMAP_SMALL,
                      NULL, new QPixmap(":/setting/Image/setting/cancel_icon.png"));

    connect(p_buttonCancel,SIGNAL(clicked(bool)),this,SLOT(slotCancelButtonClicked()));
    //connect(p_buttonExecute,SIGNAL(clicked(bool)),this,SLOT(slotExecuteButtonClicked()));

    QHBoxLayout *hLayout_bottom = new QHBoxLayout(frameBottom);

    hLayout_bottom->addWidget(p_buttonCancel);
    hLayout_bottom->addStretch();
//    hLayout_bottom->addLayout(&m_stackLayout);

    QFrame *tmp_frame = new QFrame(frameBottom);
    tmp_frame->setLayout(&m_stackLayout);
    tmp_frame->setFixedWidth(p_buttonExecute->width());
    hLayout_bottom->addWidget(tmp_frame);

    QVBoxLayout * vLayout_main = new QVBoxLayout(mainWidget);
    vLayout_main->setSpacing(0);
    vLayout_main->setMargin(0);
    vLayout_main->addLayout(hLayout_top);
    vLayout_main->addWidget(frameBottom);

    QStackedLayout *p_stackedLayout = new QStackedLayout(this);
    p_stackedLayout->insertWidget(0,mainWidget);//
    p_stackedLayout->insertWidget(1,DisplayData_FileList::instance());
    p_stackedLayout->insertWidget(2,DisplayData::instance());
    p_stackedLayout->insertWidget(3,DisplayData_Folder::instance());
    p_stackedLayout->insertWidget(4,DisplayData_LoadCertificate::instance());
    p_stackedLayout->insertWidget(5,DisplayData_DeleteCertificate::instance());
    p_stackedLayout->setCurrentIndex(0);

    DisplayData::instance()->setStackLayout(p_stackedLayout);
    DisplayData_FileList::instance()->setStackLayout(p_stackedLayout);
    DisplayData_Folder::instance()->setStackLayout(p_stackedLayout);
    DisplayData_LoadCertificate::instance()->setStackLayout(p_stackedLayout);
    DisplayData_DeleteCertificate::instance()->setStackLayout(p_stackedLayout);
}

void winsaveload::setp_buttonExecuteDisabled()
{
    p_buttonExecute->setEnabled(m_StackExecuteButton.top()->isExecuteAble());
}

//当滑动到下一界面时设置右下端的执行按钮为相应界面的执行按钮对象
void winsaveload::setp_buttonExecuted(WinSvLdFrame *b, bool isClear)
{
    if(isClear)
    {
        m_StackExecuteButton.clear();
    }
    p_buttonExecute = b->getP_executeButton();
    m_StackExecuteButton.push(b);
    m_stackLayout.addWidget(m_StackExecuteButton.top()->getP_executeButton());
    m_stackLayout.setCurrentWidget(b->getP_executeButton());
    qDebug() << m_stackLayout.count();
}

//当滑动到上一界面时设置右下端的执行按钮为相应界面的执行按钮对象并需要检查按钮状态
void winsaveload::setp_buttonExecuted()
{
    if(m_StackExecuteButton.size() <= 1)
        return;
    m_StackExecuteButton.pop();
    p_buttonExecute = m_StackExecuteButton.top()->getP_executeButton();
    m_stackLayout.setCurrentWidget(m_StackExecuteButton.top()->getP_executeButton());

    m_StackExecuteButton.top()->getP_executeButton()
            ->setEnabled(m_StackExecuteButton.top()->isExecuteAble());
    qDebug() << m_StackExecuteButton.top()->isExecuteAble();
    //remove m_StackExecuteButton;
    //m_stackLayout.removeWidget(m_stackLayout.currentWidget());
}

winsaveload::~winsaveload()
{
}

void winsaveload::slotExecuteButtonClicked()
{
    //WinSaveLoadMainFrame::instance()->execute();
}

void winsaveload::slotCancelButtonClicked()
{
    emit sigChangeFrame(Win_Root_Panel);
}

