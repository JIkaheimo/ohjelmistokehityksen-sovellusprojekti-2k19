#include "mainview.h"
#include "ui_mainview.h"

MainView::MainView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainView)
{
    ui->setupUi(this);

    connect(
        ui->btnToEvents, &QPushButton::clicked,
        [this]{ emit ToEvents(); }
    );

    connect(
        ui->btnToWithdraw, &QPushButton::clicked,
        [this]{ emit ToWithdrawal(); }
    );

    connect(
        ui->btnToDeposit, &QPushButton::clicked,
        [this]{ emit ToDeposit(); }
    );
}

MainView::~MainView()
{
    delete ui;
}

