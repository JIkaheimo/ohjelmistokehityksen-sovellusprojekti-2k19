#include "pindll.h"

#include "pindialog.h"

#include <QDebug>

void PinDLL::getPin(QWidget* parent)
{
    PinDialog* pinDialog = new PinDialog(parent);
    pinDialog->setAttribute(Qt::WA_DeleteOnClose);

    connect(
        pinDialog, &PinDialog::PinEntered,
        this, &PinDLL::onPinEntered
    );

    connect(
        pinDialog, &PinDialog::Logger,
        this, &PinDLL::onLoggableReceived
    );

    connect(
        pinDialog, &PinDialog::Timeout,
        this, &PinDLL::onTimeoutReceived
    );

    pinDialog->exec();
}

void PinDLL::onPinEntered(int pinCode)
{
    emit PinEntered(pinCode);
}

void PinDLL::onTimeoutReceived()
{
    emit Timeout();
}

void PinDLL::onLoggableReceived(QString loggable)
{
    emit Logger("PinDLL", loggable);
}
