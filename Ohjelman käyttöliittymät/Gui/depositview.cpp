#include "depositview.h"
#include "ui_depositview.h"

#include <QDoubleValidator>

DepositView::DepositView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DepositView)
{
    ui->setupUi(this);

    connect(
        ui->btnDeposit, &QPushButton::clicked,
        [this](){ emit Deposit(ui->editDeposit->value()); }
    );
}

DepositView::~DepositView()
{
    delete ui;
}

