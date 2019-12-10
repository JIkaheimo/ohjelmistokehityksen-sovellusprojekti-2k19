#include "depositview.h"
#include "ui_depositview.h"

#include <QDoubleValidator>
#include <qmessagebox.h>

const QString DEPOSIT_QUESTION = "Are you sure you want to deposit %1 €?";

DepositView::DepositView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DepositView)
{
    ui->setupUi(this);

    connect(
        ui->btnAction, &QPushButton::clicked,
        this, &DepositView::onDeposit
    );
}

double DepositView::getDeposit()
{
    return ui->editDeposit->value();
}

void DepositView::onDeposit()
{
    double depositAmount = getDeposit();

    auto reply = QMessageBox::question(
        this, "Confirm deposit",
        DEPOSIT_QUESTION.arg(depositAmount)
    );

    if (reply == QMessageBox::Yes)
        emit Deposit(depositAmount);
}

DepositView::~DepositView()
{
    delete ui;
}

