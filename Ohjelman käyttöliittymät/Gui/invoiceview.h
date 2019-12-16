#ifndef INVOICEVIEW_H
#define INVOICEVIEW_H

#include <QWidget>

class QAbstractItemModel;
class QDataWidgetMapper;

namespace Ui {
class InvoiceView;
}

class InvoiceView : public QWidget
{
    Q_OBJECT

public:
    explicit InvoiceView(QWidget *parent = nullptr);
    ~InvoiceView();

public slots:
    void setInvoices(QAbstractItemModel &invoices);
    void removeInvoice();

private slots:
    void onInvoiceChanged(int index);


signals:
    void PayInvoice(int invoiceId);
    void NoInvoices();


private:
    Ui::InvoiceView *ui;
    QDataWidgetMapper *invoiceMapper;
    QAbstractItemModel *invoiceModel;
};

#endif // INVOICEVIEW_H
