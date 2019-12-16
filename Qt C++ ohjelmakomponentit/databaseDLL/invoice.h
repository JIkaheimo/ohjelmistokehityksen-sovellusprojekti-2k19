#ifndef INVOICE_H
#define INVOICE_H

#include "table.h"

class QAbstractTableModel;

class Invoice : Table
{
public:
   Invoice(QSqlDatabase &db);

   bool setPaid(int invoiceNumber);

   QSqlRecord getInvoice(int invoiceNumber);
   float getAmount(int invoiceNumber);
   int getReceiver(int invoiceNumber);

   // Table interface
   QAbstractTableModel* getOpenInvoices(int payerId);

   static float getAmount(QSqlRecord& invoice);
   static int getReceiver(QSqlRecord& invoice);

private:
    QSqlRecord selectItem(int id);

};

#endif // INVOICE_H
