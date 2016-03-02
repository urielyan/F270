#ifndef WINDIALOG_H
#define WINDIALOG_H

#include <QDialog>
#include <QStackedLayout>
#include "winmainframe.h"
#include "./GlobalData/cfgGlobalDef.h"

class WinDialog;
class WinStackedLayout;

class WinDialog : public QDialog
{
    Q_OBJECT
public:
    explicit WinDialog(QWidget *parent = WinMainFrame::instance());

signals:
    void sigChangeFrame(quint32 frameId);//画面切换

public slots:

};

class WinStackedLayout : public QStackedLayout
{
    Q_OBJECT
public:
    WinStackedLayout();
    explicit WinStackedLayout(QWidget *parent);
    explicit WinStackedLayout(QLayout *parentLayout);

    void addWidget(QWidget *w);
    void addWidget(QWidget *w, Qt::Alignment alignment);
    void setGeometry(const QRect &rect);
};

#endif // WINDIALOG_H
