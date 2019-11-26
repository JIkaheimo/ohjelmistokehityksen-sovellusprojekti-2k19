#include "koontiview.h"
#include "ui_koontiview.h"

KoontiView::KoontiView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::KoontiView)
{
    ui->setupUi(this);

    connect(ui->btnDeposit, &QPushButton::clicked, this, &KoontiView::onDepositClicked);
    connect(ui->btnWithdraw, &QPushButton::clicked, this, &KoontiView::onWithdrawClicked);
}

KoontiView::~KoontiView()
{
    delete ui;
}

void KoontiView::setEvents(QSqlTableModel *eventModel)
{
    ui->tblEvents->setModel(eventModel);
}

void KoontiView::onDepositClicked()
{
    emit deposit(50);
}

void KoontiView::onWithdrawClicked()
{
    emit withdraw(50);
}
