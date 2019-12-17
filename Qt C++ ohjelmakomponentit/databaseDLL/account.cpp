#include "account.h"

#include <QSqlTableModel>
#include <QSqlQuery>

bool Account::setBalance(const QString& accountIBAN, const float amount) const
{
    QSqlQuery query;
    query.prepare(
        "UPDATE Account SET balance = :balance "
        "WHERE IBAN =:IBAN"
    );

    query.bindValue(":balance", amount);
    query.bindValue(":IBAN", accountIBAN);

    return query.exec();
}


 QAbstractItemModel* Account::getOtherAccounts(const QString& accountIBAN) const
{
    QSqlQueryModel* model = new QSqlQueryModel();

    QSqlQuery query;
    query.prepare(
        "SELECT IBAN FROM Account WHERE IBAN!=:IBAN"
    );
    query.bindValue(":IBAN", accountIBAN);
    query.exec();

    model->setQuery(query);
    return model;
}


float Account::getBalance(const QString& accountIBAN) const
{
    QSqlQuery query;
    query.prepare(
        "SELECT balance FROM Account "
        "WHERE IBAN=:IBAN"
    );
    query.bindValue(":IBAN", accountIBAN);

    if (query.exec() && query.first())
        return query.value(0).toFloat();

    return -1;
}


int Account::getCustomerId(const QString& accountIBAN) const
{
    QSqlQuery query;
    query.prepare(
        "SELECT customer FROM Account "
        "WHERE IBAN =:IBAN"
    );
    query.bindValue(":IBAN", accountIBAN);

    if (query.exec() && query.first())
        return query.value(0).toInt();

    return -1;
}



