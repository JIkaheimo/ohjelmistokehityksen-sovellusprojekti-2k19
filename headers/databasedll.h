#ifndef DATABASEDLL_H
#define DATABASEDLL_H

#include "databaseDLL_global.h"

#include <QObject>
#include <QtSql>
#include <QSqlDatabase>

class DATABASEDLL_EXPORT DatabaseDLL  : public QObject
{
    Q_OBJECT

private:
    QSqlDatabase mDB;
    QSqlTableModel* mAccount;
    QSqlTableModel* mEvents;

    enum EVENT {
        WITHDARWAL,
        DEPOSIT
    };

public:

    explicit DatabaseDLL(QObject *parent = nullptr);
    ~DatabaseDLL();

    bool login(QString cardNumber, int pin);
    bool deposit(float depositAmount);
    bool withdraw(float withdrawAmount);

    QSqlTableModel* getEvents();
    float getBalance();

signals:
    void BalanceChanged(float newBalance);
    void UserAuthenticated();
    void ErrorHappened(QString description);
    void Logger(QString message);

private:
    void addEvent(DatabaseDLL::EVENT type, float amount);
    bool addToBalance(float amount);
};

#endif // DATABASEDLL_H
