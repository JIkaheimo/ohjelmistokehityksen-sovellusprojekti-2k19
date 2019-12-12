#include "card.h"
#include "account.h"
#include "customer.h"
#include "event.h"

#include "databasedll.h"

#include <QSqlQuery>

const QString HOST_NAME = "mysli.oamk.fi";
const QString USERNAME = "c8ikja00";
const QString PASSWORD = "8uL4khDtm2H6H7ag";
const QString DB_NAME = "opisk_c8ikja00";

const QString DEPOSIT_ERROR = "Could not deposit funds to account.";
const QString CONNECTION_ERROR = "Could not connect to database.";


DatabaseDLL::DatabaseDLL()
    :
      mDB(QSqlDatabase::addDatabase("QMYSQL"))
{
    mDB.setHostName(HOST_NAME);
    mDB.setUserName(USERNAME);
    mDB.setPassword(PASSWORD);
    mDB.setDatabaseName(DB_NAME);

    if (mDB.open())
    {
        mAccount = new Account(mDB);
        mCard = new Card(mDB);
        mEvent = new Event(mDB);
        mCustomer = new Customer(mDB);
    }
    else
    {
        emit ErrorHappened(CONNECTION_ERROR);
        emit Logger(CONNECTION_ERROR);
    }

}

DatabaseDLL::~DatabaseDLL()
{
    if (mDB.isOpen())
        mDB.close();

    delete mAccount;
    mAccount = nullptr;

    delete mEvent;
    mEvent = nullptr;

    delete mCard;
    mCard = nullptr;

    delete mCustomer;
    mCustomer = nullptr;

}


bool DatabaseDLL::login(QString cardNumber, int pin)
/**
 * login(cardNumber, pin)
 */
{
    int accountId = mCard->validate(cardNumber, pin);

    if (accountId > 0)
    {
        mAccountId = accountId;

        emit Logger(QString("Login successful for account with id %1.").arg(mAccountId));
        emit BalanceChanged(mAccount->getBalance(mAccountId));

        return true;
    }
    else
    {
        emit Logger("Login failed. Please give correct login credentials.");
        emit ErrorHappened("Login failed. Please give correct login credentials.");

        return false;
    }
}

float DatabaseDLL::getBalance()
/**
  * Returns the selected account's balance.
  */
{
    return mAccount->getBalance(mAccountId);
}

QString DatabaseDLL::getAccountOwner()
{
    int customerId = mAccount->getCustomerId(mAccountId);
    return mCustomer->getName(customerId);
}

QString DatabaseDLL::getAccountNumber()
{
    return mAccount->getNumber(mAccountId);
}


bool DatabaseDLL::deposit(float depositAmount)
{
    if (depositAmount > 0 && addToBalance(depositAmount))
    {
        addEvent(Event::DEPOSIT, depositAmount);
        emit BalanceChanged(getBalance());
        return true;
    }

    emit ErrorHappened(DEPOSIT_ERROR);
    return false;
}


bool DatabaseDLL::withdraw(float withdrawAmount)
{
    if (withdrawAmount > 0 && addToBalance(-withdrawAmount))
    {
        addEvent(Event::WITHDARWAL, withdrawAmount);
        emit BalanceChanged(getBalance());
        return true;
    }

    emit ErrorHappened("Could not withdraw funds from account.");
    return false;
}


void DatabaseDLL::addEvent(Event::Type evtType, float amount)
{
    QString type = "unknown";

    if (evtType == Event::DEPOSIT)
        type = "deposit";
    else
        type = "withdraw";


    if (!mEvent->addEvent(mAccountId, evtType, amount))
        emit Logger("Could not add event to database.");
}


QAbstractItemModel* DatabaseDLL::getEvents()
{
    QAbstractItemModel* events = mEvent->getEvents(mAccountId);

    if (!events->rowCount())
        emit Logger("No events available for account.");

    return events;
}


QAbstractItemModel* DatabaseDLL::getRecentEvents(int amount)
{
    QAbstractItemModel* recentEvents = mEvent->getRecentEvents(mAccountId, amount);

    if (!recentEvents->rowCount())
        emit Logger("No recent events available for account.");

    return recentEvents;
}

QAbstractItemModel *DatabaseDLL::getOtherAccounts()
{
    QAbstractItemModel* otherAccounts = mAccount->getOthers(mAccountId);

    if (!otherAccounts->rowCount())
        emit Logger("No other accounts available.");

    return otherAccounts;
}


bool DatabaseDLL::addToBalance(float amount)
{
    float newBalance = getBalance() + amount;

    // Make sure the balance cannot be negative.
    if (newBalance < 0)
        return false;

    return mAccount->setBalance(mAccountId, newBalance);
}
