#ifndef ACCOUNT_H
#define ACCOUNT_H

#include "table.h"

#include <QObject>

class Account : public Table
{
public:

    static const QString TABLE;
    static const QString ID;
    static const QString NUMBER ;
    static const QString BALANCE;
    static const QString CUSTOMER_ID;

    Account(QSqlDatabase& db);

    bool setBalance(int accountId, float amount);

    float getBalance(int accountId);
    QString getNumber(int accountId);
    int getCustomerId(int accountId);

    QSqlTableModel* getOthers(int accountId);

private:
    QSqlRecord selectItem(int id) override;
};

#endif // ACCOUNT_H
