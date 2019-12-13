#include "pindialog.h"
#include "ui_pindialog.h"

#include <QRegularExpression>
#include <QTimer>

const int PIN_SIZE = 4;
const int TIMEOUT_MS = 10000;

PinDialog::PinDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PinDialog),
    mPinCode(""),
    mTimer(new QTimer(this))
{
    ui->setupUi(this);

    // Store number buttons for easier access later
    mNumberButtons = findChildren<QPushButton *>(QRegularExpression("btn\\d+"));

    // Connect signals -> slots
    foreach (QPushButton* button, mNumberButtons)
    {
        connect(
            button, &QPushButton::clicked,
            this, &PinDialog::onNumberButtonClicked
        );
    }

    connect(
        ui->btnOk, &QPushButton::clicked,
        this, &PinDialog::onOkClicked
    );

    connect(
        ui->btnBs, &QPushButton::clicked,
        this, &PinDialog::onBackspaceClicked
    );

    connect(
        mTimer, &QTimer::timeout,
        this, &PinDialog::onTimeout
    );

    // Initialize timer
    mTimer->setSingleShot(true);
    mTimer->setInterval(TIMEOUT_MS);
    mTimer->start();

    // Update button state
    updateButtons();
}

PinDialog::~PinDialog()
{
    delete ui;

    delete mTimer;
    mTimer = nullptr;
}


void PinDialog::onNumberButtonClicked()
/**
  * Gets called when the user clicks any number button.
  * Just appends the pressed "number" to the end of the PIN.
  */
{
    QPushButton* clickedButton = static_cast<QPushButton*>(sender());
    QString clickedNumber = clickedButton->text();

    mPinCode.append(clickedNumber);
    ui->editPin->setText(mPinCode);

    updateButtons();
}

void PinDialog::updateButtons()
/**
  * Enables/disables buttons based on the state of the given PIN.
  */
{
    int pinLength = mPinCode.length();

    foreach (QPushButton* button, mNumberButtons) {
        button->setEnabled(pinLength < PIN_SIZE);
    }

    ui->btnOk->setEnabled(pinLength == PIN_SIZE);
    ui->btnBs->setEnabled(pinLength > 0);
}

void PinDialog::onOkClicked()
/**
  * Gets called when the user clicks the "OK" button.
  */
{
    mTimer->stop();
    emit Logger("Pin " + mPinCode + " entered.");
    emit PinEntered(mPinCode.toInt());
    close();
}

void PinDialog::onBackspaceClicked()
/**
  * Gets called when the user clicks the "BS" button.
  * Removes one character from PIN.
  */
{
    mPinCode.chop(1);
    ui->editPin->setText(mPinCode);
    updateButtons();
}

void PinDialog::onTimeout()
{
    emit Logger("User did not enter pin in time. Timeout occured.");
    emit Timeout();
    close();
}
