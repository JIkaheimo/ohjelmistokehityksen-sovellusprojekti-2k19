#ifndef PINDLL_H
#define PINDLL_H

#include "PinDLL_global.h"
#include <QObject>

class PinDialog;

class PINDLL_EXPORT PinDLL : public QObject
{
    Q_OBJECT

public:
    void PINDLL_EXPORT getPin(QWidget* parent);

signals:
    void PINDLL_EXPORT Logger(QString source, QString description);
    void PINDLL_EXPORT Timeout();
    void PINDLL_EXPORT PinEntered(int pinCode);

private slots:
    void onPinEntered(int pinCode);
    void onTimeoutReceived();
    void onLoggableReceived(QString loggable);
};

#endif // PINDLL_H
