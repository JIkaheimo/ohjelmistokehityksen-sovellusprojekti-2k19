#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "databasedll.h"
#include "rfiddll.h"
#include "pindll.h"

#include "Constants.h"

#include <QMessageBox>
#include <QDebug>

#include <ui_mainwindow.h>


MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent),

    m_db(new DatabaseDLL()),
    m_rfid(new RfidDLL()),
    m_pin(new PinDLL()),

    ui(new Ui::MainWindow)
{

    // GUI initialization

    ui->setupUi(this);

    connect(ui->btnBack, &QPushButton::clicked,
            this, &MainWindow::previousView);

    initStartView();
    initMainView();
    initWithdrawalView();
    initDepositView();
    initTransactionView();
    initInvoicesView();

    showView(ui->startView);

    show();

    // Library initializations

    initPin();
    initRfid();

    if (!initDB())
    {
        ui->startView->setEnabled(false);
    }
}

MainWindow::~MainWindow()
{
    delete ui;

    // Make sure libs are correctly deleted.
    delete m_rfid;
    m_rfid = nullptr;

    delete m_pin;
    m_pin = nullptr;

    delete m_db;
    m_db = nullptr;
}


/** LIB INITIALIZATIONS */


bool MainWindow::initDB()
{

    connect(m_db, &DatabaseDLL::ErrorHappened,
            this, &MainWindow::displayError);

    connect(m_db, &DatabaseDLL::Logger,
            this, [=](QString logged){ logger(Logger::DB, logged); });

    connect(m_db, &DatabaseDLL::BalanceChanged,
            this, &MainWindow::showBalance);

    return m_db->init();
}


void MainWindow::initPin()
{
    connect(m_pin, &PinDLL::Logger,
            this, &MainWindow::logger);

    connect(m_pin, &PinDLL::PinEntered,
            this, &MainWindow::onPinEntered);

    connect(m_pin, &PinDLL::Timeout,
        this, [=]{ displayError(BSError::PinTimeout); });
}


void MainWindow::initRfid()
{
    connect(m_rfid, &RfidDLL::Logger,
            this, [=](QString logged){ logger(Logger::Rfid, logged); });

    connect(m_rfid, &RfidDLL::ErrorHappened,
            this, &MainWindow::displayError);

    if (m_rfid->readData(Default::Port))
    {
        connect(m_rfid, &RfidDLL::CardRead,
                this, &MainWindow::onCardRead);
    }
}


/** VIEW INITIALIZATIONS */


void MainWindow::initMainView()
/**
  * Initializes MainView with any required connections.
  */
{
    // Connect navigation to different views from the main view.

    connect(ui->mainView, &MainView::ToSummary,
            this, &MainWindow::toSummaryView);

    connect(ui->mainView, &MainView::ToWithdrawal,
            this, &MainWindow::toWithdrawalView);

    connect(ui->mainView, &MainView::ToEvents,
            this, &MainWindow::toEventView);

    connect(ui->mainView, &MainView::ToDeposit,
            this, &MainWindow::toDepositView);

    connect(ui->mainView, &MainView::ToTransaction,
            this, &MainWindow::toTransactionView);

    connect(ui->mainView, &MainView::ToInvoices,
            this, &MainWindow::toInvoiceView);
}


void MainWindow::initStartView()
/**
  * Initializes StartView with any required connections.
  */
{
    connect(ui->startView, &StartView::ReadCard,
            this, &MainWindow::readCard);

    connect(ui->startView, &StartView::TestLogin,
            this, [=]{ onCardRead(Default::Card); });
}


void MainWindow::initTransactionView()
{
    connect(ui->transactionView, &TransactionView::Transfer,
            this, &MainWindow::onTransaction);
}


void MainWindow::initInvoicesView()
{
    connect(ui->invoiceView, &InvoiceView::PayInvoice,
            this, &MainWindow::onPayInvoice);

    connect(ui->invoiceView, &InvoiceView::NoInvoices,
            this, [this]{
                displayInfo("No invoices available.");
                previousView();
            });
}


void MainWindow::initWithdrawalView()
/**
  * Initializes WithdrawalView with any required connections.
  */
{
    connect(ui->withdrawalView, &WithdrawalView::Withdraw,
            this, &MainWindow::onWithdrawal);

    connect(m_db, &DatabaseDLL::BalanceChanged,
            ui->withdrawalView, &WithdrawalView::setWithdrawable);
}


void MainWindow::initDepositView()
/**
  * Initializes DepositView with any required connections.
  */
{
    // Listen for any deposit events.
    connect(ui->depositView, &DepositView::Deposit,
            this, &MainWindow::onDeposit);
}


void MainWindow::setCurrentView(QWidget* view)
{
    // Save the previous page to page history.
    QWidget* currentPage = ui->viewStack->currentWidget();
    m_viewHistory.push(currentPage);

    showView(view);
}


void MainWindow::showView(QWidget* view)
{
    bool isStartView = view == ui->startView;
    bool isMainView = view == ui->mainView;

    QString btnText = (isMainView) ? "Logout" : "Back";

    ui->viewStack->setCurrentWidget(view);

    ui->labelBalance->setVisible(!isStartView);

    ui->btnBack->setText(btnText);
    ui->btnBack->setVisible(!isStartView);
    ui->labelHeader->setVisible(isStartView);
}


void MainWindow::previousView()
{    
    if (!m_viewHistory.isEmpty())
    {
        QWidget* previousView = m_viewHistory.pop();
        showView(previousView);
    }

    // If the stack is empty, user is back to the start view
    // -> Logout
    if (m_viewHistory.isEmpty())
    {
        m_db->logout();
        displayInfo(BSMessage::Logout);
    }

}


void MainWindow::onDeposit(float amount)
{
    if (m_db->deposit(amount))
        displayInfo(BSMessage::Deposit);
    else
        displayError(BSError::Deposit);
}


void MainWindow::onTransaction(int receiverId, float amount)
{
    if (m_db->transfer(receiverId, amount))
        displayInfo(BSMessage::Transfer);
    else
        displayError(BSError::Transfer);
}


void MainWindow::onPayInvoice(int invoiceId)
{
    if (m_db->payInvoice(invoiceId))
    {
        displayInfo(BSMessage::Invoice);
        ui->invoiceView->setInvoices(*m_db->getOpenInvoices());
    }
    else
        displayError(BSError::Invoice);
}


void MainWindow::onWithdrawal(float amount)
{
    if (m_db->withdraw(amount))
        displayInfo(BSMessage::Withdrawal);
    else
        displayError(BSError::Withdrawal);
}


void MainWindow::readCard()
{
   m_rfid->readData(Default::Port);
}


void MainWindow::onCardRead(QString cardNumber)
{
    m_cardNumber = cardNumber;
    m_pin->getPin(this);
}

void MainWindow::toSummaryView()
{
    ui->summaryView->setOwner(m_db->getAccountOwner());
    ui->summaryView->setAccountNumber(m_db->getAccountNumber());
    ui->summaryView->setEvents(*m_db->getRecentEvents(Default::NumRecentEvents));

    setCurrentView(ui->summaryView);
}


void MainWindow::toWithdrawalView()
{
    setCurrentView(ui->withdrawalView);
}


void MainWindow::toEventView()
{
    ui->eventView->setEvents(m_db->getEvents());
    setCurrentView(ui->eventView);
}

void MainWindow::toDepositView()
{
    setCurrentView(ui->depositView);
}


void MainWindow::toTransactionView()
{
    setCurrentView(ui->transactionView);
}

void MainWindow::toInvoiceView()
{
    setCurrentView(ui->invoiceView);
    ui->invoiceView->setInvoices(*m_db->getOpenInvoices());
}


void MainWindow::onPinEntered(int pinCode)
{
    if (m_db->login(m_cardNumber, pinCode))
        setCurrentView(ui->mainView);
}


void MainWindow::showBalance(float balance)
{
    QString balanceString;
    balanceString.sprintf("BAL: %9.2f€", static_cast<double>(balance));

    ui->labelBalance->setText(balanceString);
}


void MainWindow::logger(QString source, QString description)
{
    qDebug() << QString("LOGGING - src: %1, desc: %2").arg(source).arg(description);
}


void MainWindow::displayInfo(QString message)
{
    QMessageBox::information(this, "", message);
}


void MainWindow::displayError(QString message)
{
    QMessageBox::warning(this, "Error occured", message);
}


