#ifndef RFIDDLL_H
#define RFIDDLL_H

#include "rfiddll_global.h"
#include <QDebug>
#include <QtSerialPort/QSerialPort>

class RFIDDLLSHARED_EXPORT RfidDLL : QObject
{

    Q_OBJECT
private:
    QSerialPort *m_serial;

public:
    RfidDLL(QObject *parent);


private slots:
    void on_btnHaeArvo_clicked();
    void lueArvo();

};

#endif // RFIDDLL_H
