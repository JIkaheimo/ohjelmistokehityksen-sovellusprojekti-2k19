#ifndef EVENT_H
#define EVENT_H

#include <QString>

class QAbstractItemModel;

class Event
{
public:

    static const QString TABLE;
    static const QString ID;
    static const QString TYPE;
    static const QString TIME;
    static const QString AMOUNT;
    static const QString BALANCE;
    static const QString ACCOUNT;

    enum Type {
        Withdrawal,
        Deposit,
        Invoice,
        Gift
    };

    bool addEvent(QString accountIBAN, Event::Type type, float amount, float balance);

    QAbstractItemModel* getRecentEvents(QString accountIBAN, int num);
    QAbstractItemModel* getEvents(QString accountIBAN);
};

#endif // EVENT_H
