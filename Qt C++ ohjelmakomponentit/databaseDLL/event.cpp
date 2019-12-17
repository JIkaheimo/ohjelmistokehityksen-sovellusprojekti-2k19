#include "event.h"

#include <QSqlTableModel>
#include <QDateTime>
#include <QSqlRecord>
#include <QSqlQuery>

const QString Event::TABLE = "Event";
const QString Event::ID = "id";
const QString Event::TYPE = "type";
const QString Event::TIME = "time";
const QString Event::AMOUNT = "amount";
const QString Event::BALANCE = "balance";
const QString Event::ACCOUNT = "account";


const QString TYPES[] = {"withdrawal", "deposit", "invoice", "gift"};


bool Event::addEvent(QString accountIBAN, Event::Type type, float amount, float balance)
{
    QSqlTableModel events;
    events.setTable(TABLE);

    // Generate event timestamp
    QDateTime timestamp = QDateTime::currentDateTime();

    // Create and populate a new event record.
    QSqlRecord newEvent = events.record();

    newEvent.setValue(TYPE, TYPES[type]);
    newEvent.setValue(ACCOUNT, accountIBAN);
    newEvent.setValue(TIME, timestamp.toString("yyyy-MM-dd hh:mm:ss"));
    newEvent.setValue(AMOUNT, amount);
    newEvent.setValue(BALANCE, balance);

    return events.insertRecord(-1, newEvent) && events.submit();
}


QAbstractItemModel* Event::getRecentEvents(QString accountIBAN, int num)
{
    QSqlQueryModel* model = new QSqlQueryModel();

    QSqlQuery query;
    query.prepare(
        "SELECT time, amount, balance, type FROM Event "
        "WHERE account = :account "
        "ORDER BY time DESC LIMIT :num"
    );

    query.bindValue(":account", accountIBAN);
    query.bindValue(":num", num);
    query.exec();

    model->setQuery(query);
    return model;
}


QAbstractItemModel* Event::getEvents(QString accountIBAN)
{
    QSqlQueryModel* model = new QSqlQueryModel();

    QSqlQuery query;
    query.prepare(
        "SELECT time, amount, balance, type FROM Event "
        "WHERE account = :account "
        "ORDER BY time DESC"
    );

    query.bindValue(":account", accountIBAN);
    query.exec();

    model->setQuery(query);
    return model;
}
