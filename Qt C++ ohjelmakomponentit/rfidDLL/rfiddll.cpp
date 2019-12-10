#include "rfiddll.h"

const QString PORT_INITIALIZING = "Initializing connection in port %1.";
const QString PORT_READING = "Succesfully connected to port %1.";
const QString PORT_FAIL = "Could not open a connection in port %1.";

const QString CARD_READ = "Card read with number %1.";


RfidDLL::RfidDLL() :
    m_serialPort(new QSerialPort())
{
}

RfidDLL::~RfidDLL()
{
    delete m_serialPort;
    m_serialPort = nullptr;
}

bool RfidDLL::initSerialPort(const QString& port)
/**
 * Initializes serial port for reading data.
 */
{
    emit Logger(PORT_INITIALIZING.arg(port));
    m_serialPort->setPortName(port);
    m_serialPort->setBaudRate(QSerialPort::Baud9600);
    m_serialPort->setDataBits(QSerialPort::Data8);
    m_serialPort->setParity(QSerialPort::NoParity);
    m_serialPort->setStopBits(QSerialPort::OneStop);
    m_serialPort->setFlowControl(QSerialPort::HardwareControl);

    return m_serialPort->open(QIODevice::ReadWrite);
}


bool RfidDLL::readData(const QString& port)
{
    if (initSerialPort(port))
    {
        connect(
            m_serialPort, &QSerialPort::readyRead,
            this, &RfidDLL::onReadyRead
        );
        emit Logger(PORT_READING);

        return true;
    }

    emit Logger(PORT_FAIL.arg(port));
    emit ErrorHappened(PORT_FAIL.arg(port));

    return false;
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
        QString cleanedCardNumber = cardSerialNumber.left(10);

        emit Logger(CARD_READ.arg(cleanedCardNumber));
        emit CardRead(cleanedCardNumber);

    } else {

    }
}
