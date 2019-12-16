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
        ui->btnToSummary, &QPushButton::clicked,
        this, [this]{ emit ToSummary(); }
    );

    connect(
        ui->btnToTransaction, &QPushButton::clicked,
        this, [this]{ emit ToTransaction(); }
    );

    connect(
        ui->btnToInvoices, &QPushButton::clicked,
        this, [this]{ emit ToInvoices(); }
    );
}

MainView::~MainView()
{
    delete ui;
}

