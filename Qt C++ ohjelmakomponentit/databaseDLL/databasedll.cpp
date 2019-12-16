#include "card.h"
#include "account.h"
#include "customer.h"
#include "event.h"
#include "invoice.h"

#include "databasedll.h"
#include "LibConstants.h"

#include <QSqlQuery>
#include <QAbstractItemModel>
#include <QSqlTableModel>

const bool IS_TEST_MODE = false;


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

    delete m_invoice;
    m_invoice = nullptr;
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
    m_db.setHostName(IS_TEST_MODE ? Config::TestHost : Config::Host);
    m_db.setUserName(IS_TEST_MODE ? Config::TestUser : Config::User);
    m_db.setPassword(IS_TEST_MODE ? Config::TestPassword : Config::Password);
    m_db.setDatabaseName(IS_TEST_MODE ? Config::TestDatabase : Config::Database);

    emit Logger(DBMessage::Init);

    if (m_db.open())
    {
        emit Logger(DBMessage::Open);

        m_account = new Account(m_db);
        m_card = new Card(m_db);
        m_event = new Event(m_db);
        m_customer = new Customer(m_db);
        m_invoice = new Invoice(m_db);

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
    emit Logger(QString(DBMessage::Auth).arg(cardNumber).arg(pin));
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


bool DatabaseDLL::transfer(int receiverId, float amount)
{
    // PLACEHOLDER
    return true;
}


bool DatabaseDLL::payInvoice(int invoiceId)
{
    emit Logger("Paying invoice ID: " + QString::number(invoiceId));

    // Get any necessary data.
    QSqlRecord invoice = m_invoice->getInvoice(invoiceId);

    float invoiceAmount = Invoice::getAmount(invoice);
    int receiverId = Invoice::getReceiver(invoice);

    float receiverBalance = m_account->getBalance(receiverId);

    float newBalance = getBalance() - invoiceAmount;

    if (newBalance < 0)
        return false;

    m_invoice->setPaid(invoiceId);

    // Update account balances
    m_account->setBalance(m_accountId, newBalance);
    m_account->setBalance(receiverId, receiverBalance + invoiceAmount);

    // Add events
    m_event->addEvent(receiverId, Event::Invoice, invoiceAmount, m_account->getBalance(receiverId));
    m_event->addEvent(m_accountId, Event::Invoice, -invoiceAmount, newBalance);

    emit BalanceChanged(newBalance);

    emit Logger("Invoice succesfully paid.");

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


QAbstractTableModel* DatabaseDLL::getOpenInvoices()
{
    if (!checkLogin()) return nullptr;

    QAbstractTableModel* openInvoices = m_invoice->getOpenInvoices(m_accountId);

    if (!openInvoices->rowCount())
        emit Logger("No open invoices available for the account.");

    return openInvoices;
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
