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
}
