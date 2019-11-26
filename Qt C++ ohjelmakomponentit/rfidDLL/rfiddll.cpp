#include "rfiddll.h"


RfidDLL::RfidDLL(QObject *parent)
    : QObject(parent),
      m_serial(new QSerialPort())
{
}

void RfidDLL::on_btnHaeArvo_clicked()
{
    // Vaihda oikea portti
    m_serial->setPortName("com3");
    m_serial->setBaudRate(QSerialPort::Baud9600);
    m_serial->setDataBits(QSerialPort::Data8);
    m_serial->setParity(QSerialPort::NoParity);
    m_serial->setStopBits(QSerialPort::OneStop);
    m_serial->setFlowControl(QSerialPort::HardwareControl);

    if (m_serial->open(QIODevice::ReadWrite))
    {
        qDebug() << m_serial->portName();
    }
    else
    {
        qDebug() << "Portin avaaminen epäonnistui...";
    }

    connect(m_serial, &QSerialPort::readyRead, this, &RfidDLL::lueArvo);

}

void RfidDLL::lueArvo()
{
    QString cardSerialNumber;
    char data[20];
    qint64 bytesread;
    bytesread = m_serial->read(data, 20);
    data[bytesread] = '\0';
    if (bytesread > 10)
    {

        for (int i = 0; i <= 9; i++)
        {
            cardSerialNumber = data;
        }
        cardSerialNumber.remove(0, 3);

        qDebug() << "Kortin numero: " << cardSerialNumber;
        //ui->lblNaytaArvo->setText(QString("Kortin numero: ").append(cardSerialNumber));
    }
    else
    {
        qDebug() << "Virhe korttia luettaessa";
        //ui->lblNaytaArvo->setText("Virhe korttia luettaessa");
    }
}
