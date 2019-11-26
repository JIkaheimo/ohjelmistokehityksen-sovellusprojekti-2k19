#ifndef DATABASEDLL_H
#define DATABASEDLL_H

#include <QObject>
#include <QtSql>
#include <QSqlDatabase>

class DatabaseDLL : public QObject
{
    Q_OBJECT

private:
    QSqlDatabase db;
    QSqlTableModel* model;
    int account = 0;

    enum EVENT {
        WITHDARWAL,
        DEPOSIT
    };

    // DATABASE CONFIGS
    // TODO: Change these for oamk.mysli connection
    const QString HOST_NAME = "127.0.0.1";
    const QString USERNAME = "root";
    const QString PASSWORD = "";
    const QString DB_NAME = "banksimul";

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

public:

    explicit DatabaseDLL(QObject *parent = nullptr);

    bool initConnection();

    QSqlTableModel* getEvents();

    bool login(QString cardNumber, int pin);
    bool deposit(float depositAmount);
    bool withdraw(float withdrawAmount);

private:
    void addEvent(DatabaseDLL::EVENT type, float amount);
    QSqlTableModel* eventModel;
    bool addToBalance(float amount);
};

#endif // DATABASEDLL_H
