#include "invoiceview.h"
#include "ui_invoiceview.h"

#include <QDebug>

#include <QDataWidgetMapper>

InvoiceView::InvoiceView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InvoiceView),
    invoiceMapper(new QDataWidgetMapper(this))
{
    ui->setupUi(this);

    connect(ui->selectInvoice, SIGNAL(currentIndexChanged(int)),
            this, SLOT(onInvoiceChanged(int)));

    connect(ui->btnAction, &QPushButton::clicked,
            this, &InvoiceView::onPayInvoice);
}

InvoiceView::~InvoiceView()
{
    clear();
    delete ui;
}


void InvoiceView::onInvoiceChanged(int index)
{
    invoiceMapper->setCurrentIndex(index);
    updateButton();
}


void InvoiceView::setInvoices(QAbstractItemModel& invoices)
{
    delete invoiceModel;
    invoiceModel = &invoices;

    ui->selectInvoice->setModel(invoiceModel);

    if (invoiceModel->rowCount() == 0)
    {
        setEnabled(false);
        emit NoInvoices();
    }
    else
    {
        invoiceMapper->setModel(invoiceModel);
        invoiceMapper->addMapping(ui->textNumber, 0, "text");
        invoiceMapper->addMapping(ui->textAmount, 1, "text");
        invoiceMapper->addMapping(ui->textDate, 2, "date");
        invoiceMapper->addMapping(ui->textReceiver, 3, "text");
        invoiceMapper->addMapping(ui->textName, 4, "text");
        invoiceMapper->toFirst();
    }

}

void InvoiceView::setLimit(float limit)
{
    m_payLimit = limit;
    updateButton();
}

void InvoiceView::updateButton()
{
    bool canPay = m_payLimit >= ui->textAmount->text().toFloat();
    ui->btnAction->setEnabled(canPay);
}

void InvoiceView::removeInvoice()
{
    invoiceModel->removeRow(ui->selectInvoice->currentIndex());
    invoiceMapper->submit();
}

void InvoiceView::onPayInvoice()
{
    int invoiceNumber = ui->textNumber->text().toInt();
    emit PayInvoice(invoiceNumber);
}

void InvoiceView::clear()
{
    delete invoiceModel;
    invoiceModel = nullptr;
}
