#include "startview.h"
#include "ui_startview.h"

StartView::StartView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StartView)
{
    ui->setupUi(this);

    connect(
        ui->btnAction, &QPushButton::clicked,
        this, [this]{ emit ReadCard(); }
    );

    connect(
        ui->btnTest, &QPushButton::clicked,
        this, [this]{ emit TestLogin(); }
    );
}

StartView::~StartView()
{
    delete ui;
}
