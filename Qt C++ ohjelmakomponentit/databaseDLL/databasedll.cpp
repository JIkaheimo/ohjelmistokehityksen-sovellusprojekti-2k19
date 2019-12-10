#include "databasedll.h"

#include <QSqlQuery>

const QString HOST_NAME = "mysli.oamk.fi";
const QString USERNAME = "c8ikja00";
const QString PASSWORD = "8uL4khDtm2H6H7ag";
const QString DB_NAME = "opisk_c8ikja00";

// CARD CONSTANTS
const QString CARD_TABLE = "card";
const QString CARD_ID = "id";
const QString CARD_NUMBER = "identifier";
const QString CARD_PIN = "pin";
const QString CARD_ACCOUNT = "idAccount";

// ACCOUNTS CONSTANTS
const QString ACCOUNT_TABLE = "account";
const QString ACCOUNT_ID = "id";
const QString ACCOUNT_NUMBER = "number";
const QString ACCOUNT_BALANCE = "balance";

// EVENT CONSTANTS
const QString EVENT_TABLE = "event";
const QString EVENT_ID = "id";
const QString EVENT_TYPE = "type";
const QString EVENT_TIME = "time";
const QString EVENT_AMOUNT = "amount";
const QString EVENT_ACCOUNT = "idAccount";

const QString DEPOSIT_ERROR = "Could not deposit funds to account.";


DatabaseDLL::DatabaseDLL(QObject *parent)
    : QObject(parent),
      mDB(QSqlDatabase::addDatabase("QMYSQL")),
      mAccount(new QSqlTableModel(this, mDB)),
      mEvents(new QSqlTableModel(this, mDB))
{
    mDB.setHostName(HOST_NAME);
    mDB.setUserName(USERNAME);
    mDB.setPassword(PASSWORD);
    mDB.setDatabaseName(DB_NAME);

    if (mDB.open())
    {
        mAccount->setTable(ACCOUNT_TABLE);
        mEvents->setTable(EVENT_TABLE);
    }
    else
    {
        emit ErrorHappened("Could not connect to database.");
    }
}

DatabaseDLL::~DatabaseDLL()
{
    if (mDB.isOpen()) mDB.close();
}


bool DatabaseDLL::login(QString cardNumber, int pin)
/**
 * login(cardNumber, pin)
 */
{
    QSqlTableModel cardModel;

    cardModel.setTable(CARD_TABLE);
    cardModel.setFilter(
        QString("%1 = '%2' AND %3 = %4")
            .arg(CARD_NUMBER)
            .arg(cardNumber)
            .arg(CARD_PIN)
            .arg(pin)
    );

    if (cardModel.select() && cardModel.rowCount() == 1)
    {
        QString accountID = cardModel.record(0).value(CARD_ACCOUNT).toString();

        mEvents->setFilter("idAccount = " + accountID);
        mEvents->sort(4, Qt::DescendingOrder);
        mAccount->setFilter("id = " + accountID);
        mEvents->select();
        mAccount->select();

        emit Logger("Login successful for account (ID): " + accountID);
        emit BalanceChanged(getBalance());
        emit UserAuthenticated();

        return true;
    }
    else
    {
        emit ErrorHappened("Login failed. Please give correct login credentials.");
        return false;
    }
}

float DatabaseDLL::getBalance()
/**
  * Returns the selected account's balance.
  */
{
    if (!mAccount->select())
        emit Logger("Error fetching account's balance from the database.");

    if (mAccount->rowCount() != 1)
        emit Logger("There were incorrect amount of accounts selected while getting balance...");

    return mAccount->record(0).value(ACCOUNT_BALANCE).toFloat();
}

bool DatabaseDLL::deposit(float depositAmount)
{
    if (addToBalance(depositAmount))
    {
        addEvent(EVENT::DEPOSIT, depositAmount);
        emit BalanceChanged(getBalance());
        return true;
    }

    emit ErrorHappened(DEPOSIT_ERROR);
    return false;
}


bool DatabaseDLL::withdraw(float withdrawAmount)
{
    if (addToBalance(-withdrawAmount))
    {
        addEvent(EVENT::WITHDARWAL, withdrawAmount);
        emit BalanceChanged(getBalance());
        return true;
    }

    emit ErrorHappened("Could not withdraw funds from account.");
    return false;
}


void DatabaseDLL::addEvent(EVENT evtType, float amount)
{
    QString type = "unknown";

    if (evtType == EVENT::DEPOSIT) {
        type = "deposit";
    }
    else
    {
        type = "withdraw";
    }

    // Generate event timestamp
    QDateTime timestamp = QDateTime::currentDateTime();

    // Create and populate a new event record.
    QSqlRecord newEvent = mEvents->record();

    newEvent.setValue(EVENT_TYPE, type);
    newEvent.setValue(EVENT_ACCOUNT, mAccount->record(0).value(ACCOUNT_ID));
    newEvent.setValue(EVENT_TIME, timestamp.toString("yyyy-MM-dd hh:mm:ss"));
    newEvent.setValue(EVENT_AMOUNT, amount);

    mEvents->insertRecord(-1, newEvent);
    mEvents->submitAll();

    mEvents->select();
}

QSqlTableModel* DatabaseDLL::getEvents()
/**
  * Returns the events for the account being used.
  */
{
    if (!mEvents->select())
        emit Logger("Error fetching events from the database.");

    if (!mEvents->rowCount())
        emit Logger("No events available for account");

    return mEvents;
}


bool DatabaseDLL::addToBalance(float amount)
{
    float newBalance = getBalance() + amount;

    // Make sure the balance cannot be negative.
    if (newBalance < 0)
    {
        return false;
    }

    // Update the record
    QSqlRecord record = mAccount->record(0);
    record.setValue(ACCOUNT_BALANCE, newBalance);
    mAccount->setRecord(0, record);

    return mAccount->submit();
}
