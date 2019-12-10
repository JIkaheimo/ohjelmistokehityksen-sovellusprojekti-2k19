#include "startview.h"
#include "ui_startview.h"

StartView::StartView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StartView)
{
    ui->setupUi(this);

    connect(
        ui->btnReadRfid, &QPushButton::clicked,
        [this]{ emit readData(); }
    );

    connect(
        ui->btnTest, &QPushButton::clicked,
        [this]{ emit TestLogin(); }
    );
}

StartView::~StartView()
{
    delete ui;
}
