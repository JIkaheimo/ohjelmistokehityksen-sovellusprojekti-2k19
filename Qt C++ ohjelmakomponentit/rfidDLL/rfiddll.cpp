#include "rfiddll.h"


RfidDLL::RfidDLL(QObject *parent)
    : QObject(parent),
      m_serial(new QSerialPort())
{
    initSerialPort();
}

void RfidDLL::initSerialPort()
{
    m_serial->setPortName(PORT);
    m_serial->setBaudRate(QSerialPort::Baud9600);
    m_serial->setDataBits(QSerialPort::Data8);
    m_serial->setParity(QSerialPort::NoParity);
    m_serial->setStopBits(QSerialPort::OneStop);
    m_serial->setFlowControl(QSerialPort::HardwareControl);

    if (m_serial->open(QIODevice::ReadWrite))
    {
        connect(m_serial, &QSerialPort::readyRead, this, &RfidDLL::readData);
    }
    else
    {
        qDebug() << "Virhe!!" << endl;
    }

}

void RfidDLL::readData()
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

    emit dataReceived(cardSerialNumber);
}
