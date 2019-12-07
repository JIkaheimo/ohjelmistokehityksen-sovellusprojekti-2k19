#include "mainwindow.h"
#include "pindialog.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QMessageBox>

const QString PORT = "com3";

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent),
    db(new DatabaseDLL(this)),
    rfid(new RfidDLL(PORT, this)),

    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Try to initialize connection to the "bank server".
    if (!db->initConnection())
    {
        QMessageBox::critical(this, "Connection Error", "Could not connect to the database...");
    }

    connect(ui->btnReadRfid, &QPushButton::clicked, this, &MainWindow::readCard);
    connect(rfid, &RfidDLL::cardRead, this, &MainWindow::cardRead);

    /*
    connect(koontiNakyma, &KoontiView::deposit, db, &DatabaseDLL::deposit);
    connect(koontiNakyma, &KoontiView::withdraw, db, &DatabaseDLL::withdraw);
    connect(rfid, &RfidDLL::dataReceived, this, &MainWindow::cardRead);
    connect(aloitusNakyma, &AloitusView::korttiLuettu, this, &MainWindow::cardRead);
    ui->nakymat->addWidget(aloitusNakyma);
    ui->nakymat->addWidget(koontiNakyma);
    ui->nakymat->setCurrentWidget(aloitusNakyma);
    rfid->readData();
    */
}

MainWindow::~MainWindow()
{
    delete db;
    delete ui;
    //delete rfid;
}

void MainWindow::readCard()
{
   rfid->readData();
}

void MainWindow::cardRead(QString cardNumber)
{
    qDebug() << cardNumber;
    PinDialog *pin = new PinDialog(this);

    this->korttinumero = cardNumber;

    ui->stackMain->setEnabled(false);
    pin->show();

    connect(pin, &PinDialog::pinEntered, this, &MainWindow::pinLuettu);
}


void MainWindow::pinLuettu(QString pinKoodi)
{
            /*
    if (db->login(korttinumero, pinKoodi.toInt()))
    {
        ui->stackMain->setCurrentWidget(koontiNakyma);
        koontiNakyma->setEvents(db->getEvents());
    }
    else
    {
        ui->stackMain->setCurrentWidget(aloitusNakyma);
    }
    ui->stackMain->setEnabled(true);
            */
}

void MainWindow::suoritaTalletus(float depositAmount)
{
    db->deposit(depositAmount);
}


