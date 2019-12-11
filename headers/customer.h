#ifndef CUSTOMER_H
#define CUSTOMER_H

#include "table.h"



class Customer : Table
{
public:
    Customer(QSqlDatabase& db);

    QString getName(int customerId);
    // Table interface
private:
    QSqlRecord selectItem(int id);
};

#endif // CUSTOMER_H
