#include "pindialog.h"
#include "ui_pindialog.h"

#include <QTimer>

const int PIN_SIZE = 4;
const int TIMEOUT_MS = 10000;

PinDialog::PinDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PinDialog),
    mPinCode("")
{
    ui->setupUi(this);

    foreach (auto button, findChildren<QPushButton *>()) {
        addButtonEvent(button);
    }

    QTimer::singleShot(TIMEOUT_MS, [this]{ emit Timeout(); } );
}

PinDialog::~PinDialog()
{
    delete ui;
}


void PinDialog::onNumberButtonClicked()
{
    QPushButton* clickedButton = static_cast<QPushButton*>(sender());
    QString clickedNumber = clickedButton->text();

    QLineEdit* editPin = ui->editPin;
    QString pinCode = editPin->text();

    pinCode.append(clickedNumber);
    editPin->setText(pinCode);

    if (pinCode.length() == PIN_SIZE)
    {
        emit PinEntered(pinCode.toInt());
    }
}

void PinDialog::addButtonEvent(QPushButton *button)
{
    connect(
        button, &QPushButton::clicked,
        this, &PinDialog::onNumberButtonClicked
    );
}
