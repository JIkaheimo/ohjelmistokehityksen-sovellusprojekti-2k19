#ifndef DATABASEDLL_H
#define DATABASEDLL_H

#include "databaseDLL_global.h"

#include <QObject>
#include <QSqlDatabase>

#include "event.h"

class Card;
class Account;
class Invoice;
class Customer;
class QAbstractItemModel;

class DATABASEDLL_EXPORT DatabaseDLL  : public QObject
{
    Q_OBJECT

private:
    QSqlDatabase m_db;

    QString m_accountIBAN = "";

    Customer* m_customer;
    Invoice* m_invoice;
    Account* m_account;
    Card* m_card;
    Event* m_event;

public:
    explicit DatabaseDLL();
    ~DatabaseDLL();

    bool DATABASEDLL_EXPORT init();

    bool DATABASEDLL_EXPORT login(QString cardNumber, int pin);
    bool DATABASEDLL_EXPORT isLoggedIn();
    void DATABASEDLL_EXPORT logout();

    float DATABASEDLL_EXPORT getBalance();
    QString DATABASEDLL_EXPORT getAccountOwner();
    QString DATABASEDLL_EXPORT getAccountNumber();

    bool DATABASEDLL_EXPORT deposit(float amount);
    bool DATABASEDLL_EXPORT withdraw(float amount);
    bool DATABASEDLL_EXPORT transfer(QString receiverIBAN, float amount);
    bool DATABASEDLL_EXPORT payInvoice(int invoiceId);

    QAbstractItemModel* DATABASEDLL_EXPORT getEvents();
    QAbstractItemModel* DATABASEDLL_EXPORT getRecentEvents(int number);
    QAbstractItemModel* DATABASEDLL_EXPORT getOpenInvoices();
    QAbstractItemModel* DATABASEDLL_EXPORT getOtherAccounts();

signals:
    void DATABASEDLL_EXPORT BalanceChanged(float newBalance);
    void DATABASEDLL_EXPORT Logout();
    void DATABASEDLL_EXPORT ErrorHappened(QString description);
    void DATABASEDLL_EXPORT Logger(QString message);

private:
    void addEvent(Event::Type type, float amount, float balance);
    bool addToBalance(float amount);
    bool checkLogin();
};

#endif // DATABASEDLL_H
