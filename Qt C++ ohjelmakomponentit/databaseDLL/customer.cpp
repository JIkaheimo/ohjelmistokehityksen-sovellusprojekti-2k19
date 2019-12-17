#include "customer.h"

#include <QSqlQuery>
#include <QSqlTableModel>

QString Customer::getName(const int customerId) const
{
    QSqlQuery query;

    query.prepare(
        "SELECT CONCAT(first_name, ' ', last_name) as name "
        "FROM Customer WHERE id=:id"
    );
    query.bindValue(":id", customerId);

    if (query.exec() && query.first())
        return query.value(0).toString();

    return "Name unavailable.";
}

