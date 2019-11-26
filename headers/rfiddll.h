#ifndef RFIDDLL_H
#define RFIDDLL_H

#include "rfiddll_global.h"
#include <QDebug>
#include <QtSerialPort/QSerialPort>

class RFIDDLLSHARED_EXPORT RfidDLL : public QObject
{
    Q_OBJECT

    // Change this to correct port.
    const QString PORT = "com3";

private:
    QSerialPort *m_serial = nullptr;
    void initSerialPort();

public:
    RfidDLL(QObject *parent = nullptr);

signals:
    void dataReceived(QString data);

public slots:
    void readData();

};

#endif // RFIDDLL_H
