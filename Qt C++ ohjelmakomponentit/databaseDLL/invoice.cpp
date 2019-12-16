#include "invoice.h"
#include "account.h"
#include "customer.h"

#include <QDateTime>
#include <QSqlRelationalTableModel>
#include <QSqlTableModel>

const QString TABLE = "invoice";
const QString PAYER = "idPayer";
const QString AMOUNT = "amount";
const QString RECEIVER = "idReceiver";
const QString NUMBER = "number";
const QString PAID = "paid";
const QString DATE = "date";

Invoice::Invoice(QSqlDatabase &db)
    : Table(db, TABLE)
{

}

bool Invoice::setPaid(int invoiceNumber)
{
    QSqlRecord invoice = selectItem(invoiceNumber);

    QDateTime timestamp = QDateTime::currentDateTime();

    invoice.setValue(PAID, timestamp.toString("yyyy-MM-dd hh:mm:ss"));
    mModel->setRecord(0, invoice);
    return mModel->submit();
}

QSqlRecord Invoice::getInvoice(int invoiceNumber)
{
    return selectItem(invoiceNumber);
}


float Invoice::getAmount(int invoiceNumber)
{
    QSqlRecord invoice = selectItem(invoiceNumber);
    return invoice.value(AMOUNT).toFloat();
}


int Invoice::getReceiver(int invoiceNumber)
{
    QSqlRecord invoice = selectItem(invoiceNumber);
    return invoice.value(RECEIVER).toInt();
}


QAbstractTableModel *Invoice::getOpenInvoices(int payerId)
{
    QSqlQueryModel* openInvoices = new QSqlQueryModel();

    const QString query = QString(
            "SELECT %1.number, %1.amount, %1.date, %2.number, CONCAT(%3.firstName, %3.lastName) as name "
            "FROM %1 JOIN %2 ON %1.idReceiver = %2.id "
            "JOIN %3 ON %2.idCustomer = %3.id "
            "WHERE %1.idPayer = %4 AND %1.paid IS NULL").arg(TABLE, Account::TABLE, Customer::TABLE, QString::number(payerId));

    openInvoices->setQuery(query);

    return openInvoices;
}

float Invoice::getAmount(QSqlRecord &invoice)
{
    return invoice.value(AMOUNT).toFloat();
}

int Invoice::getReceiver(QSqlRecord &invoice)
{
    return invoice.value(RECEIVER).toInt();
}

QSqlRecord Invoice::selectItem(int invoiceNumber)
{

    mModel->setFilter(QString("%1 = %2").arg(NUMBER, QString::number(invoiceNumber)));
    mModel->select();

    return mModel->record(0);
}
