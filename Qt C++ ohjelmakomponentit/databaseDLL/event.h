#ifndef EVENT_H
#define EVENT_H

#include "table.h"

class QAbstractItemModel;

class Event : public Table
{
public:

    static const QString TABLE;
    static const QString ID;
    static const QString TYPE;
    static const QString TIME;
    static const QString AMOUNT;
    static const QString BALANCE;
    static const QString ACCOUNT_ID;

    enum Type {
        Withdrawal,
        Deposit,
        Invoice
    };
    Q_ENUM(Type)

    Event(QSqlDatabase& db);

    bool addEvent(int accountId, Event::Type type, float amount, float balance);
    QAbstractItemModel* getRecentEvents(int accountId, int num);
    QAbstractItemModel* getEvents(int accountId);

private:
    QSqlRecord selectItem(int eventId);
};

#endif // EVENT_H
