#include "card.h"
#include "account.h"
#include "customer.h"
#include "event.h"
#include "invoice.h"

#include "databasedll.h"
#include "LibConstants.h"

#include <QSqlQuery>
#include <QAbstractItemModel>

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
  * true if the connection was successfully established,
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

        m_account = new Account();
        m_card = new Card();
        m_event = new Event();
        m_customer = new Customer();
        m_invoice = new Invoice();

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
    emit Logger(QString(DBMessage::Auth).arg(cardNumber, QString::number(pin)));

    QString accountIBAN = m_card->validate(cardNumber, pin);

    if (!accountIBAN.isEmpty())
    {
        m_accountIBAN = accountIBAN;

        emit Logger(QString(DBSuccess::Auth).arg(m_accountIBAN));
        emit BalanceChanged(m_account->getBalance(m_accountIBAN));

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
    return !m_accountIBAN.isEmpty();
}


void DatabaseDLL::logout()
{
    m_accountIBAN = "";
    emit BalanceChanged(0);
    emit Logout();
}


float DatabaseDLL::getBalance()
{
    if (!checkLogin()) return -1;

    return m_account->getBalance(m_accountIBAN);
}


QString DatabaseDLL::getAccountOwner()
{
    if (!checkLogin()) return "";

    int customerId = m_account->getCustomerId(m_accountIBAN);

    return m_customer->getName(customerId);
}


QString DatabaseDLL::getAccountNumber()
{
    if (!checkLogin()) return "";

    return m_accountIBAN;
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


bool DatabaseDLL::transfer(QString IBAN, float amount)
{
    float receiverBalance = m_account->getBalance(IBAN);

    float newBalance = getBalance() - amount;

    // Make sure balance cannot be negative.
    if (newBalance < 0)
    {
        emit Logger("Insufficient funds to transfer.");
        return false;
    }

    // Update account balances
    m_account->setBalance(m_accountIBAN, newBalance);
    m_account->setBalance(IBAN, receiverBalance + amount);

    // Add events
    m_event->addEvent(IBAN, Event::Gift, amount, receiverBalance + amount);
    m_event->addEvent(m_accountIBAN, Event::Gift, -amount, newBalance);

    emit BalanceChanged(newBalance);

    return true;
}


bool DatabaseDLL::payInvoice(int invoiceId)
{
    emit Logger("Paying invoice ID: " + QString::number(invoiceId));

    // Get any necessary data.
    QSqlRecord invoice = m_invoice->getInvoice(invoiceId);

    float invoiceAmount = Invoice::getAmount(invoice);
    QString receiverIBAN = Invoice::getReceiver(invoice);

    float receiverBalance = m_account->getBalance(receiverIBAN);

    float newBalance = m_account->getBalance(m_accountIBAN) - invoiceAmount;
    emit Logger(QString::number(newBalance));

    // Make sure balance cannot be negative.
    if (newBalance < 0)
    {
        emit Logger("Insufficient funds to pay the invoice.");
        return false;
    }

    // Update invoice
    m_invoice->setPaid(invoiceId);

    // Update account balances
    m_account->setBalance(m_accountIBAN, newBalance);
    m_account->setBalance(receiverIBAN, receiverBalance + invoiceAmount);

    // Add events
    m_event->addEvent(receiverIBAN, Event::Invoice, invoiceAmount, receiverBalance + invoiceAmount);
    m_event->addEvent(m_accountIBAN, Event::Invoice, -invoiceAmount, newBalance);

    emit BalanceChanged(newBalance);

    emit Logger("Invoice succesfully paid.");

    return true;
}


void DatabaseDLL::addEvent(Event::Type evtType, float amount, float balance)
{
    if (!m_event->addEvent(m_accountIBAN, evtType, amount, balance))
        emit Logger("Could not add event to database.");
}


QAbstractItemModel* DatabaseDLL::getEvents()
/**
  * Returns a model of the account's events.
  *
  * Returns:
  * events (QAbstractItemModel*) as a model.
  */
{
    if (!checkLogin()) return nullptr;

    QAbstractItemModel* events = m_event->getEvents(m_accountIBAN);

    if (!events->rowCount())
        emit Logger("No events available for account " + m_accountIBAN);

    return events;
}


QAbstractItemModel* DatabaseDLL::getRecentEvents(int number)
/**
  * Returns a model of the account's recent events.
  *
  * Params:
  * number (int) of events being returned.
  *
  * Returns:
  * recentEvents (QAbstractItemModel*) as a model.
  *
  */
{
    if (!checkLogin()) return nullptr;

    QAbstractItemModel* recentEvents = m_event->getRecentEvents(m_accountIBAN, number);

    if (!recentEvents->rowCount())
        emit Logger("No recent events available for account " + m_accountIBAN);

    return recentEvents;
}


QAbstractItemModel* DatabaseDLL::getOpenInvoices()
{
    if (!checkLogin()) return nullptr;

    QAbstractItemModel* openInvoices = m_invoice->getOpenInvoices(m_accountIBAN);

    if (!openInvoices->rowCount())
        emit Logger("No open invoices available for the account.");

    return openInvoices;
}


QAbstractItemModel *DatabaseDLL::getOtherAccounts()
{
    if (!checkLogin()) return nullptr;

    QAbstractItemModel* otherAccounts = m_account->getOtherAccounts(m_accountIBAN);

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

    return m_account->setBalance(m_accountIBAN, newBalance);
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
