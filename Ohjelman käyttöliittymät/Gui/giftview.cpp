#include <QDebug>
#include <QMessageBox>
#include "giftview.h"
#include "ui_giftview.h"

const QString TRANSFER_QUESTION = "Transfer %1 € to account: %2?";

GiftView::GiftView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GiftView)
{
    ui->setupUi(this);

    connect(
        ui->confirmButton, &QPushButton::clicked,
        this, &GiftView::onTransaction
    );
}

GiftView::~GiftView()
{
    delete ui;
}

void GiftView::onTransaction()
{
    QString input = ui->lineEdit->text();
    bool inputOnNumero;
    int tiliNumero = input.toInt(&inputOnNumero);
    float summa = ui->doubleSpinBox->value();

    if (inputOnNumero && summa > 0)
        transfer(summa, tiliNumero);
    else
        QMessageBox::warning(this, "Transaction invalid",
            "Amount too small or invalid account number");
}

void GiftView::transfer(float summa, int tiliNumero)
{
    int reply = QMessageBox::question(
        this, "Confirm transaction",
        TRANSFER_QUESTION
                .arg(summa)
                .arg(tiliNumero)
    );

    if (reply == QMessageBox::Yes)
        emit Transfer(summa, tiliNumero);
}

