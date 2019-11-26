#ifndef RFIDDLL_H
#define RFIDDLL_H

#include "rfiddll_global.h"
#include <QDebug>
#include <QtSerialPort/QSerialPort>

class RFIDDLLSHARED_EXPORT RfidDLL : public QObject
{
    Q_OBJECT
private:
    QSerialPort *m_serial = nullptr;

public:
    RfidDLL(QObject *parent = nullptr);


private slots:
    void on_btnHaeArvo_clicked();
    void lueArvo();

};

#endif // RFIDDLL_H
