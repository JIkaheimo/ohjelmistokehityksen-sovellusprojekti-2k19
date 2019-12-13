#ifndef EVENT_H
#define EVENT_H

#include "table.h"

class QSqlQueryModel;

class Event : public Table
{
public:
    enum Type {
        Withdrawal,
        Deposit
    };
    Q_ENUM(Type)

    Event(QSqlDatabase& db);

    bool addEvent(int accountId, Event::Type type, float amount, float balance);
    QSqlQueryModel *getRecentEvents(int accountId, int num);
    QSqlTableModel *getEvents(int accountId);

private:
        QSqlRecord selectItem(int eventId);
};

#endif // EVENT_H
