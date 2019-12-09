#include "pindialog.h"
#include "ui_pindialog.h"
#include <QDebug>

PinDialog::PinDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PinDialog)
{
    ui->setupUi(this);

    addButtonEvent(ui->btn0);
    addButtonEvent(ui->btn1);
    addButtonEvent(ui->btn2);
    addButtonEvent(ui->btn3);
    addButtonEvent(ui->btn4);
    addButtonEvent(ui->btn5);
    addButtonEvent(ui->btn6);
    addButtonEvent(ui->btn7);
    addButtonEvent(ui->btn8);
    addButtonEvent(ui->btn9);
}

PinDialog::~PinDialog()
{
    delete ui;
}

void PinDialog::onNumberButtonClicked()
/**
 * Aktivoituu, kun jotain numeronappia painetaan.
 */
{
    // Haetaan numeroarvo painetun napin tekstistä
    QString klikattuNumero = (static_cast<QPushButton*>(sender()))->text();

    QString pinTeksti = ui->editPin->text();

    if (pinTeksti.length() < PIN_SIZE)
    {
        pinKoodi.append(klikattuNumero);
        ui->editPin->setText(pinTeksti.append("*"));
    }

    if (pinTeksti.length() == PIN_SIZE)
    {
        emit pinEntered(pinKoodi.toInt());
        delete(this);
    }


}

void PinDialog::addButtonEvent(QPushButton *button)
{
    connect(button, &QPushButton::clicked, this, &PinDialog::onNumberButtonClicked);
}
