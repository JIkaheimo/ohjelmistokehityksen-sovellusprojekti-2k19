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
    explicit RfidDLL(const QString& portNumber, QObject *parent = nullptr);
    ~RfidDLL();
    bool readData();

signals:
    void CardRead(QString data);
    void Logger(QString logged);

private slots:
    void onReadyRead();

private:
    QSerialPort *m_serialPort = nullptr;
    void initSerialPort(const QString &port);
};

#endif // RFIDDLL_H
