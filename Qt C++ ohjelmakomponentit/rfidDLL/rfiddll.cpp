#include "rfiddll.h"


RfidDLL::RfidDLL(const QString& port, QObject *parent)
    : QObject(parent)
{
    initSerialPort(port);
    readData();

    // Dummy test
    onReadyRead();
}

void RfidDLL::initSerialPort(const QString& port)
/**
 * Initializes serial port for reading data.
 */
{
    m_serialPort = new QSerialPort;
    m_serialPort->setPortName(port);
    m_serialPort->setBaudRate(QSerialPort::Baud9600);
    m_serialPort->setDataBits(QSerialPort::Data8);
    m_serialPort->setParity(QSerialPort::NoParity);
    m_serialPort->setStopBits(QSerialPort::OneStop);
    m_serialPort->setFlowControl(QSerialPort::HardwareControl);
}


void RfidDLL::readData()
{

    if (m_serialPort->open(QIODevice::ReadWrite))
    {
        connect(
            m_serialPort, &QSerialPort::readyRead,
            this, &RfidDLL::onReadyRead
        );
    }
    else
    {
        qDebug() << "Could not open a connection in port: "
                 << m_serialPort->portName() << endl;
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

        emit cardRead(cardSerialNumber.left(10));
    }
}
