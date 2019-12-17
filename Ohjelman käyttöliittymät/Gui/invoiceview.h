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
    void clear();
    void setLimit(float amount);

private slots:
    void onInvoiceChanged(int index);
    void onPayInvoice();

signals:
    void PayInvoice(int invoiceId);
    void NoInvoices();

private:
    Ui::InvoiceView *ui;
    float m_payLimit = 0;
    QDataWidgetMapper *invoiceMapper = nullptr;
    QAbstractItemModel *invoiceModel = nullptr;
    void updateButton();
};

#endif // INVOICEVIEW_H
