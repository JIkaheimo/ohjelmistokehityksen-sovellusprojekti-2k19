#include "withdrawalview.h"
#include "ui_withdrawalview.h"

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
}

WithdrawalView::~WithdrawalView()
{
    delete ui;
}

void WithdrawalView::connectWithdraw(QPushButton* button, float amount)
{
    connect(
        button, &QPushButton::clicked,
        [this, amount](){ emit Withdraw(amount); }
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
