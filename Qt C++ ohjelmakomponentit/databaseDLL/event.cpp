#include "event.h"

#include <QSqlTableModel>
#include <QDebug>
#include <QDateTime>

const QString TABLE = "event";
const QString ID = "id";
const QString TYPE = "type";
const QString TIME = "time";
const QString AMOUNT = "amount";
const QString BALANCE = "balance";
const QString ACCOUNT_ID = "idAccount";

const QString TYPES[] = {"withdraw", "deposit"};

Event::Event(QSqlDatabase& db) :
    Table(db, TABLE)
{
}


bool Event::addEvent(int accountId, Event::Type type, float amount, float balance)
{
    // Generate event timestamp
    QDateTime timestamp = QDateTime::currentDateTime();

    // Create and populate a new event record.
    QSqlRecord newEvent = mModel->record();

    newEvent.setValue(TYPE, TYPES[type]);
    newEvent.setValue(ACCOUNT_ID, accountId);
    newEvent.setValue(TIME, timestamp.toString("yyyy-MM-dd hh:mm:ss"));
    newEvent.setValue(AMOUNT, amount);
    newEvent.setValue(BALANCE, balance);

    return mModel->insertRecord(-1, newEvent) && mModel->submit();
}


QSqlQueryModel* Event::getRecentEvents(int accountId, int num)
{
    QSqlQueryModel* recentEvents = new QSqlQueryModel();

    QString queryStr =
        QString(
            "SELECT %1, %2, %3, %4 FROM %5 "
            "WHERE %6 = %7 "
            "ORDER BY %1 DESC "
            "LIMIT %8"
        )
        .arg(TIME).arg(TYPE).arg(AMOUNT).arg(BALANCE).arg(TABLE)
        .arg(ACCOUNT_ID).arg(accountId)
        .arg(num);

    recentEvents->setQuery(queryStr);

    return recentEvents;
}


QSqlTableModel *Event::getEvents(int accountId)
{
    mModel->setFilter(QString("%1 = %2").arg(ACCOUNT_ID).arg(accountId));
    mModel->select();
    return mModel;
}

QSqlRecord Event::selectItem(int eventId)
{

}
