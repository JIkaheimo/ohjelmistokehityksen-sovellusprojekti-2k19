#include "withdrawalview.h"
#include "ui_withdrawalview.h"

#include <QInputDialog>
#include <QMessageBox>

#include <QDebug>
const QString WITHDRAWAL_QUESTION = "Do you want to withdraw %1€?";


WithdrawalView::WithdrawalView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WithdrawalView)
{
    ui->setupUi(this);

    connectWithdraw(ui->btnWithdraw20, 20.0);
    connectWithdraw(ui->btnWithdraw50, 50.0);
    connectWithdraw(ui->btnWithdraw100, 100.0);
    connectWithdraw(ui->btnWithdraw200, 200.0);
    connectWithdraw(ui->btnWithdraw500, 500.0);

    connect(
        ui->btnWithdrawX, &QPushButton::clicked,
        this, &WithdrawalView::onXWithdrawal
    );
}


WithdrawalView::~WithdrawalView()
{
    delete ui;
}


void WithdrawalView::connectWithdraw(QPushButton* button, double amount)
{
    connect(
        button, &QPushButton::clicked,
        this, [this, amount](){
            confirmAndWithdraw(amount);
        }
    );
}


void WithdrawalView::setWithdrawable(float amount)
{
    ui->btnWithdrawX->setEnabled(amount > 0);
    ui->btnWithdraw20->setEnabled(amount >= 20);
    ui->btnWithdraw50->setEnabled(amount >= 50);
    ui->btnWithdraw100->setEnabled(amount >= 100);
    ui->btnWithdraw200->setEnabled(amount >= 200);
    ui->btnWithdraw500->setEnabled(amount >= 500);
}


void WithdrawalView::onXWithdrawal()
{
    bool ok;

    double withdrawalAmount = QInputDialog::getInt(this,
        "Select the amount to withdraw", "Withdrawal amount:", 20, 20, 10000, 20, &ok);

    if (ok)
        confirmAndWithdraw(withdrawalAmount);
}


void WithdrawalView::confirmAndWithdraw(double amount)
{
    auto reply = QMessageBox::question(
        this, "Confirm withdrawal",
        WITHDRAWAL_QUESTION.arg(amount)
    );

    if (reply == QMessageBox::Yes)
        emit Withdraw(static_cast<float>(amount));
}
