#ifndef RFIDDLL_H
#define RFIDDLL_H

#include "rfiddll_global.h"
#include <QtSerialPort/QSerialPort>

class RFIDDLLSHARED_EXPORT RfidDLL : public QObject
{
    Q_OBJECT

public:
    explicit RfidDLL();
    ~RfidDLL();

    bool RFIDDLLSHARED_EXPORT readData(const QString& port);

signals:
    void RFIDDLLSHARED_EXPORT CardRead(QString data);
    void RFIDDLLSHARED_EXPORT ErrorHappened(QString message);
    void RFIDDLLSHARED_EXPORT Logger(QString logged);

private slots:
    void onReadyRead();

private:
    QSerialPort *m_serialPort = nullptr;
    bool initSerialPort(const QString &port);
};

#endif // RFIDDLL_H
