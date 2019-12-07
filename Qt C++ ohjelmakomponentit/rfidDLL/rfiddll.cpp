#include "rfiddll.h"


RfidDLL::RfidDLL(QString port, QObject *parent)
    : QObject(parent),
      m_serialPort(new QSerialPort)
{
    m_serialPort->setPortName(port);
    m_serialPort->setBaudRate(QSerialPort::Baud9600);
    m_serialPort->setDataBits(QSerialPort::Data8);
    m_serialPort->setParity(QSerialPort::NoParity);
    m_serialPort->setStopBits(QSerialPort::OneStop);
    m_serialPort->setFlowControl(QSerialPort::HardwareControl);

    connect(m_serialPort, &QSerialPort::readyRead, this, &RfidDLL::onReadyRead);
}


void RfidDLL::readData()
{
    if (!m_serialPort->open(QIODevice::ReadWrite))
    {

        qDebug() << "Could not open a connection to serial port...";
    }
}


void RfidDLL::onReadyRead()
{
    QString cardSerialNumber;
    char data[20];
    qint64 bytesread;
    bytesread = m_serialPort->read(data, 20);
    data[bytesread] = '\0';
    if (bytesread > 10)

    {
        for (int i = 0; i <= 9; i++)
        {
            cardSerialNumber = data;
        }
        cardSerialNumber.remove(0, 3);

        emit cardRead(cardSerialNumber);
        qDebug() << "Card number: " << cardSerialNumber;
    }
    else
    {
        qDebug() << "Could not read data from serial port...";
    }
}
