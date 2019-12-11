#ifndef EVENT_H
#define EVENT_H

#include "table.h"

class QSqlQueryModel;

class Event : public Table
{
    public:

        enum Type {
            WITHDARWAL,
            DEPOSIT
        };

        Event(QSqlDatabase& db);

        bool addEvent(int accountId, Event::Type type, float amount);
        QSqlQueryModel *getRecentEvents(int accountId, int num);
        QSqlTableModel *getEvents(int accountId);

private:
        QSqlRecord selectItem(int eventId);
};

#endif // EVENT_H
