#include "account.h"

#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QString>
#include <QSqlRecord>

#include <QDebug>

const QString TABLE = "account";
const QString ID = "id";
const QString NUMBER = "number";
const QString BALANCE = "balance";
const QString CUSTOMER_ID = "idCustomer";


Account::Account(QSqlDatabase& db) :
    Table(db, TABLE)
{
}

bool Account::setBalance(int accountId, float amount)
{
    QSqlRecord account = selectItem(accountId);
    account.setValue(BALANCE, amount);
    mModel->setRecord(0, account);
    return mModel->submit();
}

float Account::getBalance(int accountId)
{
    QSqlRecord account = selectItem(accountId);
    return account.value(BALANCE).toFloat();
}


QString Account::getNumber(int accountId)
{
    QSqlRecord account = selectItem(accountId);
    return account.value(NUMBER).toString();
}


int Account::getCustomerId(int accountId)
{
    QSqlRecord account = selectItem(accountId);
    return account.value(CUSTOMER_ID).toInt();
}


QSqlRecord Account::selectItem(int accountId)
{
    mModel->setFilter(QString("%1 = %2").arg(ID).arg(accountId));
    mModel->select();
    return mModel->record(0);
}
