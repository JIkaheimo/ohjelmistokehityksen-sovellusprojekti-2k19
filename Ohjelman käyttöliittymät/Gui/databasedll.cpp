#include "databasedll.h"

#include <QDebug>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlRelationalTableModel>

DatabaseDLL::DatabaseDLL(QObject *parent)
    : QObject(parent)
{
}


bool DatabaseDLL::initConnection()
/**
 * initConnection() - Initializes a connection to the database.
 *
 * @returns bool describing if the connection was succesfull.
 */
{
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName(HOST_NAME);
    db.setUserName(USERNAME);
    db.setPassword(PASSWORD);
    db.setDatabaseName(DB_NAME);

    if (db.open())
    {
        eventModel = new QSqlTableModel(this, db);
        eventModel->setTable(EVENT_TABLE);
        return true;
    }
    return false;
}

bool DatabaseDLL::login(QString cardNumber, int pin)
/**
 * login(cardNumber, pin)
 */
{
    // Create the query string.
    QString queryString = QString("SELECT * FROM %1 WHERE %2 = %3 AND %4 = %5")
            .arg(CARD_TABLE)
            .arg(CARD_NUMBER).arg(cardNumber)
            .arg(CARD_PIN).arg(pin);

    // Submit the query.
    QSqlQuery query(queryString);
    qDebug() << queryString;
    // Check if the card was valid and get the linked account id.
    if (query.first())
    {
        account = query.value(CARD_ACCOUNT).toInt();
        qDebug() << QString("%1 = %2").arg(EVENT_ACCOUNT).arg(account);
        eventModel->setFilter(QString("%1 = %2").arg(EVENT_ACCOUNT).arg(account));
        eventModel->select();
        qDebug() << eventModel->rowCount();
        return true;
    }
    else
    {
        return false;
    }
}



bool DatabaseDLL::deposit(float depositAmount)
{
    if (addToBalance(depositAmount))
    {
        addEvent(DatabaseDLL::EVENT::DEPOSIT, depositAmount);
        return true;
    }
    return false;
}


bool DatabaseDLL::withdraw(float withdrawAmount) {
    if (addToBalance(-withdrawAmount))
    {
        addEvent(DatabaseDLL::EVENT::WITHDARWAL, withdrawAmount);
        return true;
    }
    return false;
}


void DatabaseDLL::addEvent(DatabaseDLL::EVENT evtType, float amount)
{
    QString type = "";

    if (evtType == DatabaseDLL::EVENT::DEPOSIT) {
        type = "deposit";
    }
    else
    {
        type = "withdraw";
    }

    // Generate event timestamp
    QDateTime timestamp = QDateTime::currentDateTime();

    // Create and populate a new event record.
    QSqlRecord event = eventModel->record();

    event.setValue(EVENT_TYPE, type);
    event.setValue(EVENT_ACCOUNT, account);
    event.setValue(EVENT_TIME, timestamp.toString("yyyy-MM-dd hh:mm:ss"));
    event.setValue(EVENT_AMOUNT, amount);

    eventModel->insertRecord(-1, event);
    eventModel->submit();
    eventModel->select();
}

QSqlTableModel* DatabaseDLL::getEvents()
{
    qDebug() << "haaetaanTavarara";
    qDebug() << eventModel->rowCount();
    return eventModel;
}

bool DatabaseDLL::addToBalance(float amount)
{
    QSqlTableModel accountModel;
    accountModel.setTable(ACCOUNT_TABLE);
    accountModel.setFilter(QString("%1 = %2").arg(ACCOUNT_ID).arg(account));
    accountModel.select();

    if (accountModel.rowCount())
    {
        QSqlRecord record = accountModel.record(0);
        record.setValue(ACCOUNT_BALANCE, record.value(ACCOUNT_BALANCE).toInt() + amount);
        accountModel.setRecord(0, record);
        accountModel.submitAll();
        return true;
    }
    return false;
}
