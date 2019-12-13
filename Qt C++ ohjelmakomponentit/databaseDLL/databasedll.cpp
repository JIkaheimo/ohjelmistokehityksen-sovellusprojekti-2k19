#include "card.h"
#include "account.h"
#include "customer.h"
#include "event.h"

#include "databasedll.h"
#include "LibConstants.h"

#include <QSqlQuery>
#include <QAbstractItemModel>
#include <QSqlTableModel>


DatabaseDLL::DatabaseDLL()
    : m_db(QSqlDatabase::addDatabase("QMYSQL"))
{
}


DatabaseDLL::~DatabaseDLL()
{
    if (m_db.isOpen())
    {
        m_db.close();
    }

    delete m_account;
    m_account = nullptr;

    delete m_event;
    m_event = nullptr;

    delete m_card;
    m_card = nullptr;

    delete m_customer;
    m_customer = nullptr;

}


bool DatabaseDLL::init()
/**
  * Initializes the connection to database.
  *
  * Returns:
  * true if the connection was successful,
  * false otherwise
  */
{
    m_db.setHostName(Config::Host);
    m_db.setUserName(Config::User);
    m_db.setPassword(Config::Password);
    m_db.setDatabaseName(Config::Database);

    if (m_db.open())
    {
        m_account = new Account(m_db);
        m_card = new Card(m_db);
        m_event = new Event(m_db);
        m_customer = new Customer(m_db);

        emit Logger(DBSuccess::Connection);

        return true;
    }
    else
    {
        emit ErrorHappened(DBError::Connection);
        emit Logger(DBError::Connection);

        return false;
    }
}


bool DatabaseDLL::login(QString cardNumber, int pin)
/**
 * login(cardNumber, pin)
 */
{
    int accountId = m_card->validate(cardNumber, pin);

    if (accountId > 0)
    {
        m_accountId = accountId;

        emit Logger(QString(DBSuccess::Auth).arg(m_accountId));
        emit BalanceChanged(m_account->getBalance(m_accountId));

        return true;
    }
    else
    {
        emit Logger(DBError::Auth);
        emit ErrorHappened(DBError::Auth);

        return false;
    }
}


bool DatabaseDLL::isLoggedIn()
{
    return m_accountId != -1;
}


void DatabaseDLL::logout()
{
    m_accountId = -1;
}


float DatabaseDLL::getBalance()
{
    if (!checkLogin()) return -1;

    return m_account->getBalance(m_accountId);
}


QString DatabaseDLL::getAccountOwner()
{
    if (!checkLogin()) return "";

    int customerId = m_account->getCustomerId(m_accountId);
    return m_customer->getName(customerId);
}


QString DatabaseDLL::getAccountNumber()
{
    if (!checkLogin()) return "";

    return m_account->getNumber(m_accountId);
}


bool DatabaseDLL::deposit(float amount)
{
    if (amount > 0 && checkLogin() && addToBalance(amount))
    {
        float newBalance = getBalance();
        addEvent(Event::Deposit, amount, newBalance);
        emit BalanceChanged(newBalance);

        return true;
    }

    emit ErrorHappened(DBError::Deposit);
    return false;
}


bool DatabaseDLL::withdraw(float amount)
{
    if (amount > 0 && checkLogin() && addToBalance(-amount))
    {
        float newBalance = getBalance();
        addEvent(Event::Withdrawal, -amount, newBalance);
        emit BalanceChanged(newBalance);

        return true;
    }

    emit ErrorHappened(DBError::Withdraw);
    return false;
}


bool DatabaseDLL::transaction(int receiverId, float amount)
{
    // PLACEHOLDER
    return true;
}


void DatabaseDLL::addEvent(Event::Type evtType, float amount, float balance)
{
    if (!m_event->addEvent(m_accountId, evtType, amount, balance))
        emit Logger("Could not add event to database.");
}


QAbstractItemModel* DatabaseDLL::getEvents()
{
    if (!checkLogin()) return nullptr;

    QAbstractItemModel* events = m_event->getEvents(m_accountId);

    if (!events->rowCount())
        emit Logger("No events available for account.");

    return events;
}


QAbstractItemModel* DatabaseDLL::getRecentEvents(int number)
{
    if (!checkLogin()) return nullptr;

    QAbstractItemModel* recentEvents = m_event->getRecentEvents(m_accountId, number);

    if (!recentEvents->rowCount())
        emit Logger("No recent events available for account.");

    return recentEvents;
}


QAbstractItemModel *DatabaseDLL::getOtherAccounts()
{
    if (!checkLogin()) return nullptr;

    QAbstractItemModel* otherAccounts = m_account->getOthers(m_accountId);

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

    return m_account->setBalance(m_accountId, newBalance);
}


bool DatabaseDLL::checkLogin()
/**
  * Helper to check if the user has actually logged in,
  * with logging and error emits.
  */
{
    if (isLoggedIn())
    {
        return true;
    }
    else
    {
        Logger(DBError::Account);
        ErrorHappened(DBError::Account);
        return false;

    }
}
