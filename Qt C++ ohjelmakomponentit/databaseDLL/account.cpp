#include "account.h"

#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QString>
#include <QSqlRecord>

const QString Account::TABLE = "account";
const QString Account::ID = "id";
const QString Account::CUSTOMER_ID = "idCustomer";
const QString Account::NUMBER = "number";
const QString Account::BALANCE = "balance";

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

 QSqlTableModel* Account::getOthers(int accountId)
{
    mModel->setFilter(QString("%1 != %2").arg(ID).arg(accountId));
    mModel->select();
    return mModel;
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
    if (mLastId != accountId)
    {
        mModel->setFilter(QString("%1 = %2").arg(ID).arg(accountId));
        mLastId = accountId;
    }
    mModel->select();
    return mModel->record(0);
}
