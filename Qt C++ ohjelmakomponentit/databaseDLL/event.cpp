#include "event.h"

#include <QSqlTableModel>
#include <QDebug>
#include <QDateTime>

const QString TABLE = "event";
const QString ID = "id";
const QString TYPE = "type";
const QString TIME = "time";
const QString AMOUNT = "amount";
const QString ACCOUNT_ID = "idAccount";

Event::Event(QSqlDatabase& db) :
    Table(db, TABLE)
{

}

bool Event::addEvent(int accountId, Event::Type type, float amount)
{
    // Generate event timestamp
    QDateTime timestamp = QDateTime::currentDateTime();

    // Create and populate a new event record.
    QSqlRecord newEvent = mModel->record();

    newEvent.setValue(TYPE, type);
    newEvent.setValue(ACCOUNT_ID, accountId);
    newEvent.setValue(TIME, timestamp.toString("yyyy-MM-dd hh:mm:ss"));
    newEvent.setValue(AMOUNT, amount);

    return mModel->insertRecord(-1, newEvent) && mModel->submit();
}


QSqlQueryModel* Event::getRecentEvents(int accountId, int num)
{
    QSqlQueryModel* recentEvents = new QSqlQueryModel();

    QString queryStr =
        QString(
            "SELECT %1, %2, %3 FROM %4 "
            "WHERE %5 = %6 "
            "ORDER BY %1 DESC "
            "LIMIT %7"
        )
        .arg(TIME).arg(TYPE).arg(AMOUNT).arg(TABLE)
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
