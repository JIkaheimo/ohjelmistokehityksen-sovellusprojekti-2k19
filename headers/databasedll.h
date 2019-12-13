#ifndef DATABASEDLL_H
#define DATABASEDLL_H

#include "databaseDLL_global.h"

#include <QObject>
#include <QtSql>
#include <QSqlDatabase>

#include "event.h"

class Card;
class Account;
class Customer;


class DATABASEDLL_EXPORT DatabaseDLL  : public QObject
{
    Q_OBJECT

private:
    QSqlDatabase mDB;

    int mAccountId;

    Customer* mCustomer;
    Account* mAccount;
    Card* mCard;
    Event* mEvent;

public:

    explicit DatabaseDLL();
    ~DatabaseDLL();

    bool DATABASEDLL_EXPORT login(QString cardNumber, int pin);

    float DATABASEDLL_EXPORT getBalance();
    QString DATABASEDLL_EXPORT getAccountOwner();
    QString DATABASEDLL_EXPORT getAccountNumber();

    bool DATABASEDLL_EXPORT deposit(float depositAmount);
    bool DATABASEDLL_EXPORT withdraw(float withdrawAmount);

    QAbstractItemModel* DATABASEDLL_EXPORT getEvents();
    QAbstractItemModel* DATABASEDLL_EXPORT getRecentEvents(int amount);

signals:
    void DATABASEDLL_EXPORT BalanceChanged(float newBalance);
    void DATABASEDLL_EXPORT ErrorHappened(QString description);
    void DATABASEDLL_EXPORT Logger(QString message);

private:

    void addEvent(Event::Type type, float amount);
    bool addToBalance(float amount);
};

#endif // DATABASEDLL_H
