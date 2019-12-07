#ifndef RFIDDLL_H
#define RFIDDLL_H

#include "rfiddll_global.h"
#include <QDebug>
#include <QTimer>
#include <QtSerialPort/QSerialPort>

class RFIDDLLSHARED_EXPORT RfidDLL : public QObject
{
    Q_OBJECT

public:
    explicit RfidDLL(const QString portNumber, QObject *parent = nullptr);
    void readData();

signals:
    void cardRead(QString data);

private slots:
    void onReadyRead();

private:
    QSerialPort *m_serialPort = nullptr;
};

#endif // RFIDDLL_H
