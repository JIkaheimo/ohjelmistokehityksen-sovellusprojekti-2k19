#include "summaryview.h"
#include "ui_summaryview.h"

SummaryView::SummaryView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SummaryView)
{
    ui->setupUi(this);
}

SummaryView::~SummaryView()
{
    delete ui->eventTable->model();
    delete ui;
}


void SummaryView::setEvents(QAbstractItemModel& events)
{
    ui->eventTable->setModel(&events);
}


void SummaryView::setAccountNumber(const QString& accNumber)
{
    ui->numberText->setText(accNumber);
}


void SummaryView::setOwner(const QString& owner)
{
    ui->ownerText->setText(owner);
}


void SummaryView::clear()
{
    delete ui->eventTable->model();
    ui->numberText->clear();
    ui->ownerText->clear();
}
