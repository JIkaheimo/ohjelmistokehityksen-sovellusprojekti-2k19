#include "depositview.h"
#include "ui_depositview.h"

DepositView::DepositView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DepositView)
{
    ui->setupUi(this);
}

DepositView::~DepositView()
{
    delete ui;
}
