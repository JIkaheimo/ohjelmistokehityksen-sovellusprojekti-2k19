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
            [this]{ emit PayInvoice(ui->selectInvoice->currentText().toInt()); });

    onInvoiceChanged(1);
}

InvoiceView::~InvoiceView()
{

    delete ui;

    delete invoiceMapper;
    invoiceMapper = nullptr;
}

void InvoiceView::onInvoiceChanged(int index)
{
    invoiceMapper->setCurrentIndex(index);
}

void InvoiceView::setInvoices(QAbstractItemModel& invoices)
{
    invoiceModel = &invoices;

    if (invoiceModel->rowCount() == 0)
    {
        setEnabled(false);
        emit NoInvoices();
    }
    else
    {
        ui->selectInvoice->setModel(invoiceModel);
        ui->selectInvoice->setCurrentIndex(0);

        invoiceMapper->setModel(invoiceModel);
        invoiceMapper->addMapping(ui->editNumber, 0);
        invoiceMapper->addMapping(ui->editAmount, 1);
        invoiceMapper->addMapping(ui->editDate, 2);
        invoiceMapper->addMapping(ui->editReceiver, 3);
        invoiceMapper->addMapping(ui->editName, 4);


        invoiceMapper->toFirst();
    }

}

void InvoiceView::removeInvoice()
{
    invoiceModel->removeRow(ui->selectInvoice->currentIndex());
    invoiceMapper->submit();
}
