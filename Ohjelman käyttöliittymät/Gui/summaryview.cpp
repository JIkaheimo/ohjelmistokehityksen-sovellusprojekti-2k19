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

void SummaryView::setEvents(QAbstractItemModel *events)
{
    ui->listRecentEvents->setModel(events);
}
