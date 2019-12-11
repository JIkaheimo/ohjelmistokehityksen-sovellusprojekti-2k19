#include "eventview.h"
#include "ui_eventview.h"

EventView::EventView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EventView)
{
    ui->setupUi(this);
}

EventView::~EventView()
{
    delete ui;
}

void EventView::setEvents(QAbstractItemModel* eventModel)
{
    ui->tableEvents->setModel(eventModel);
    ui->tableEvents->hideColumn(0);
    ui->tableEvents->hideColumn(2);
    ui->tableEvents->sortByColumn(0, Qt::DescendingOrder);
}
