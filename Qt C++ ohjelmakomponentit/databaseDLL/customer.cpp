#include "customer.h"
#include <QSqlTableModel>

const QString TABLE = "customer";
const QString ID = "id";
const QString FIRST_NAME = "firstName";
const QString LAST_NAME = "lastName";

Customer::Customer(QSqlDatabase &db)
    : Table(db, TABLE)
{
}

QString Customer::getName(int customerId)
{
    QSqlRecord customer = selectItem(customerId);
    QString firstName = customer.value(FIRST_NAME).toString();
    QString lastName = customer.value(LAST_NAME).toString();

    return QString("%1 %2").arg(firstName).arg(lastName);
}

QSqlRecord Customer::selectItem(int customerId)
{
    if (mLastId != customerId)
    {
        mModel->setFilter(QString("%1 = %2").arg(ID).arg(customerId));
        mModel->select();
        mLastId = customerId;
    }
    return mModel->record(0);
}
