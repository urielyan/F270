#include "windialog.h"
#include "GlobalData/globalDef.h"

#include <QDebug>

WinDialog::WinDialog(QWidget *parent) :
    QDialog(parent)
{
    this->setWindowFlags(Qt::FramelessWindowHint);
//    this->setAttribute(Qt::WA_TranslucentBackground, true);
    this->setGeometry(0,0,WinMainFrame::instance()->width(),WinMainFrame::instance()->height());
    this->setStyleSheet("WinDialog{background-color:rgb(0,0,0,180)}");
    connect(this,SIGNAL(sigChangeFrame(quint32)),WinMainFrame::instance(),SLOT(slotFrameChanged(quint32)));
}


WinStackedLayout::WinStackedLayout()
{
}

WinStackedLayout::WinStackedLayout(QWidget *parent) :
    QStackedLayout(parent)
{
}

WinStackedLayout::WinStackedLayout(QLayout *parentLayout):
    QStackedLayout(parentLayout)
{
}


void WinStackedLayout::addWidget(QWidget *w)
{
    QStackedLayout::addWidget(w);
}

void WinStackedLayout::addWidget(QWidget *w, Qt::Alignment alignment)
{
    QStackedLayout::addWidget(w);
    setAlignment(w,alignment);
}

void WinStackedLayout::setGeometry(const QRect &rect)
{
    switch (stackingMode()) {
    case StackOne:
    {
        if(QLayoutItem  *item = itemAt(currentIndex())){
           item->setGeometry(rect);
        }
        break;
    }

    case StackAll:
    {
        int i = 0;
        QLayoutItem  *item = NULL;
        while((item = itemAt(i++)) != NULL)
            item->setGeometry(rect);
        break;
    }
    }
}
