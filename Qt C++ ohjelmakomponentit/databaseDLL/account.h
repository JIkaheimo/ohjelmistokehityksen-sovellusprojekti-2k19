#ifndef ACCOUNT_H
#define ACCOUNT_H

#include "table.h"

#include <QObject>
#include <QSqlRecord>

class QSqlTableModel;
class QSqlDatabase;

class Account : public Table
{
public:
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
