#include "transactionview.h"
#include "ui_transactionview.h"

TransactionView::TransactionView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TransactionView)
{
    ui->setupUi(this);
}

TransactionView::~TransactionView()
{
    delete ui;
}
