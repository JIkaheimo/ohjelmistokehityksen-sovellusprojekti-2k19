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
}

void RfidDLL::readData()
{
    if (m_serial->open(QIODevice::ReadWrite))
    {
        connect(m_serial, &QSerialPort::readyRead, this, &RfidDLL::onDataRead);
    }
    else
    {
        qDebug() << "Could not open a connection to serial port...";
    }
}

void RfidDLL::onDataRead()
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

        qDebug() << "Card number: " << cardSerialNumber;
        emit dataReceived(cardSerialNumber);
    }
    else
    {
        qDebug() << "Could not read data from serial port...";
    }
}
