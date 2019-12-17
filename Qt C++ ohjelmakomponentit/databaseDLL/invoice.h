#ifndef INVOICE_H
#define INVOICE_H

#include <qsqlrecord.h>

class QAbstractItemModel;

class Invoice
{
public:
   bool setPaid(int invoiceNumber);

   QSqlRecord getInvoice(int invoiceNumber);

   // Table interface
   QAbstractItemModel* getOpenInvoices(QString accountIBAN);

   static float getAmount(QSqlRecord& invoice);
   static QString getReceiver(QSqlRecord& invoice);

};

#endif // INVOICE_H
