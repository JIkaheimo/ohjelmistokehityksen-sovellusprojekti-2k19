#include "mainview.h"
#include "ui_mainview.h"

MainView::MainView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainView)
{
    ui->setupUi(this);

    connect(
        ui->btnToEvents, &QPushButton::clicked,
        this, [this]{ emit ToEvents(); }
    );

    connect(
        ui->btnAction, &QPushButton::clicked,
        this, [this]{ emit ToWithdrawal(); }
    );

    connect(
        ui->btnToDeposit, &QPushButton::clicked,
        this, [this]{ emit ToDeposit(); }
    );

    connect(
        ui->btnToOverview, &QPushButton::clicked,
        this, [this]{ emit ToOverview(); }
    );
}

MainView::~MainView()
{
    delete ui;
}

