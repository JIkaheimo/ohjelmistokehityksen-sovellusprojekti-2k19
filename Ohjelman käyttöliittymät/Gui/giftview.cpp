#include "giftview.h"
#include "ui_giftview.h"

#include <QMessageBox>

const QString TRANSFER_QUESTION = "Transfer %1 € to account: %2?";

GiftView::GiftView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GiftView)
{
    ui->setupUi(this);

    connect(ui->btnAction, &QPushButton::clicked,
            this, &GiftView::onTransfer);

}

GiftView::~GiftView()
{
    delete ui->selectAccount->model();
    delete ui;
}


void GiftView::onTransfer()
{
    QString receiverIBAN = ui->selectAccount->currentText();

    double amountAsDouble = ui->editAmount->value();
    float amount = static_cast<float>(amountAsDouble);

    if (amount > 0)
    {
        int reply = QMessageBox::question(
            this, "Confirm transaction",
            TRANSFER_QUESTION
                    .arg(amountAsDouble)
                    .arg(receiverIBAN)
        );

        if (reply == QMessageBox::Yes)
            emit Transfer(receiverIBAN, amount);
    }
}


void GiftView::setGiftable(const float amount)
/**
  * Sets the max amount available for gifting.
  */
{
    double amountAsDouble = static_cast<double>(amount);
    ui->editAmount->setMaximum(amountAsDouble);
    setEnabled(amount > 0);
}

void GiftView::setReceivers(QAbstractItemModel &receivers)
{
    delete ui->selectAccount->model();
    ui->selectAccount->setModel(&receivers);
}

void GiftView::clear()
{
    delete ui->selectAccount->model();
    ui->editAmount->clear();
}
