#include "invoice.h"
#include "account.h"
#include "customer.h"

#include <QSqlRelationalTableModel>
#include <QSqlTableModel>
#include <QSqlQuery>

const QString TABLE = "Invoice";
const QString PAYER = "payer";
const QString AMOUNT = "amount";
const QString RECEIVER = "receiver";
const QString NUMBER = "number";
const QString PAID = "paid";
const QString DATE = "date";


bool Invoice::setPaid(int invoiceNumber)
{
    QSqlQuery query;
    query.prepare(
        "UPDATE Invoice SET paid=CURDATE() "
        "WHERE number=:number"
    );
    query.bindValue(":number", invoiceNumber);

    return query.exec();
}


QAbstractItemModel* Invoice::getOpenInvoices(QString accountIBAN)
{
    QSqlQueryModel* openInvoices = new QSqlQueryModel();

    QString query = QString(
        "SELECT Invoice.number, Invoice.amount, Invoice.date, Account.IBAN, CONCAT(Customer.first_name, Customer.last_name) as name "
        "FROM Invoice JOIN Account ON Invoice.receiver = Account.IBAN "
        "JOIN Customer ON Account.customer = Customer.id "
        "WHERE Invoice.payer = '%1' AND Invoice.paid IS NULL").arg(accountIBAN);

    openInvoices->setQuery(query);

    return openInvoices;
}


float Invoice::getAmount(QSqlRecord &invoice)
{
    return invoice.value(AMOUNT).toFloat();
}


QString Invoice::getReceiver(QSqlRecord &invoice)
{
    return invoice.value(RECEIVER).toString();
}


QSqlRecord Invoice::getInvoice(int invoiceNumber)
{
    QSqlTableModel model;

    model.setTable(TABLE);
    model.setFilter(QString("%1 = %2").arg(NUMBER, QString::number(invoiceNumber)));
    model.select();

    return model.record(0);
}
