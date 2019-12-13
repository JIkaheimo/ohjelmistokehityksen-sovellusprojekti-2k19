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
    delete ui;
}

void SummaryView::setEvents(QAbstractItemModel& events)
{
    ui->eventTable->setModel(&events);
}

void SummaryView::setAccountNumber(QString accNumber)
{
    ui->numberText->setText(accNumber);
}

void SummaryView::setOwner(QString owner)
{
    ui->ownerText->setText(owner);
}
