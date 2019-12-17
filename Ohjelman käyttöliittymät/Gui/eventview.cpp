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

void EventView::setEvents(QAbstractItemModel& eventModel)
{
    clear();
    ui->tableEvents->setModel(&eventModel);
}

void EventView::clear()
{
    // Make sure to delete any "hanging" model.
    delete ui->tableEvents->model();
}
