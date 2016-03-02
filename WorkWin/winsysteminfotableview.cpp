#include <QHeaderView>
#include <QMessageBox>
#include "winsysteminfodialog.h"
#include "winsysteminfotableview.h"

WinSystemInfoTableView::WinSystemInfoTableView(QWidget *parent) :
    QTableView(parent)
{
    this->setModel(&m_model);
    this->setItemDelegateForColumn(0,&m_delegate);
    this->setItemDelegateForColumn(4,&m_delegate);
    this->verticalHeader()->setVisible(false);
    this->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    this->horizontalHeader()->setHighlightSections(false);
    this->horizontalHeader()->setFixedHeight(50);
    this->horizontalHeader()->setAutoFillBackground(true);
    this->horizontalHeader()->setSectionsClickable(false);

    this->horizontalHeader()->setDefaultSectionSize(100);
    this->horizontalHeader()->setSortIndicatorShown(false);
    this->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Fixed);
    this->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Fixed);
    this->horizontalHeader()->setSectionResizeMode(2,QHeaderView::Stretch);
    this->horizontalHeader()->setSectionResizeMode(3,QHeaderView::Stretch);
    this->horizontalHeader()->setSectionResizeMode(4,QHeaderView::Fixed);
    this->verticalHeader()->setSectionResizeMode(0,QHeaderView::Fixed);
    this->verticalHeader()->setSectionResizeMode(2,QHeaderView::Fixed);
    this->verticalHeader()->setDefaultSectionSize(50);

    this->setMouseTracking(false);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    this->setSelectionMode(QAbstractItemView::SingleSelection);
    this->setSelectionBehavior(QAbstractItemView::SelectItems);
    this->setShowGrid(true);
    this->setSpan(0,0,1,5);
    this->setSpan(2,0,1,5);

    connect(this, SIGNAL(clicked(QModelIndex)),this,SLOT(slotItemClicked(QModelIndex)));
}

void WinSystemInfoTableView::slotItemClicked(QModelIndex index)
{
    if(m_model.flags(index) & Qt::ItemIsUserCheckable){
        //TODO
        WinSystemInfoDialog().exec();
    }
}

WinSystemUnitTableView::WinSystemUnitTableView(int unitIndex,QWidget *parent) :
    QTableView(parent),m_model(unitIndex,this)
{
    this->setModel(&m_model);
    this->setItemDelegateForColumn(0,&m_tableDelege);
    this->verticalHeader()->setVisible(false);
    this->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    this->horizontalHeader()->setHighlightSections(false);
    this->horizontalHeader()->setAutoFillBackground(true);
    this->horizontalHeader()->setSectionsClickable(false);

    if(unitIndex){
        this->horizontalHeader()->setFixedHeight(80);
    }else{
        this->horizontalHeader()->setFixedHeight(50);
    }
    this->horizontalHeader()->setDefaultSectionSize(120);
    this->horizontalHeader()->setSortIndicatorShown(false);
    this->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Fixed);
    this->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Fixed);
    this->horizontalHeader()->setSectionResizeMode(2,QHeaderView::Stretch);
    this->horizontalHeader()->setSectionResizeMode(3,QHeaderView::Stretch);

    this->setMouseTracking(false);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    this->setSelectionMode(QAbstractItemView::SingleSelection);
    this->setSelectionBehavior(QAbstractItemView::SelectItems);
    this->setShowGrid(true);

    connect(this, SIGNAL(clicked(QModelIndex)),this,SLOT(slotItemClicked(QModelIndex)));
}

void WinSystemUnitTableView::slotItemClicked(QModelIndex index)
{
    if(this->model()->flags(index) & Qt::ItemIsUserCheckable){
        //TODO
        WinSystemInfoDialog().exec();
    }
}
