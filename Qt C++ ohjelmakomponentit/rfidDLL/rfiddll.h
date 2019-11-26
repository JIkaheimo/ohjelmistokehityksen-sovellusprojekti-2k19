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
    void readData();

signals:
    void dataReceived(QString data);

private slots:
    void onDataRead();
};

#endif // RFIDDLL_H
