#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <QSqlRecord>

class QAbstractItemModel;

class Account
{
public:
    bool setBalance(const QString& accountIBAN, const float amount) const;
    float getBalance(const QString& accountIBAN) const;
    int getCustomerId(const QString& accountIBAN) const;

    QAbstractItemModel* getOtherAccounts(const QString& accountIBAN) const;

};

#endif // ACCOUNT_H
