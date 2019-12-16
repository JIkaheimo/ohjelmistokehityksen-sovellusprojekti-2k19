#ifndef CUSTOMER_H
#define CUSTOMER_H

#include "table.h"



class Customer : Table
{
public:
    static const QString TABLE;
    static const QString ID;
    static const QString FIRST_NAME;
    static const QString LAST_NAME;

    Customer(QSqlDatabase& db);

    QString getName(int customerId);
    // Table interface
private:
    QSqlRecord selectItem(int id);
};

#endif // CUSTOMER_H
