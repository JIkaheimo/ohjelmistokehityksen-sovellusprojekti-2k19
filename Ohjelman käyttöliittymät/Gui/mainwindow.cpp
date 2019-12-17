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
    initGiftView();
    initInvoicesView();

    showView(ui->startView);

    show();

    // Library initializations
    initPin();
    initRfid();

    // Disable GUI if DB initialization fails.
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


/** ========= LIB INITIALIZATIONS ================ */


bool MainWindow::initDB()

{
    connect(m_db, &DatabaseDLL::ErrorHappened,
            this, &MainWindow::displayError);

    if (Default::IsLogged)
    {
        connect(m_db, &DatabaseDLL::Logger,
                this, [=](QString logged){ logger(Logger::DB, logged); });
    }

    connect(m_db, &DatabaseDLL::BalanceChanged,
            this, &MainWindow::showBalance);

    return m_db->init();
}


void MainWindow::initPin()
{

    if (Default::IsLogged)
    {
        connect(m_pin, &PinDLL::Logger,
                this, &MainWindow::logger);
    }

    connect(m_pin, &PinDLL::PinEntered,
            this, &MainWindow::onPinEntered);

    connect(m_pin, &PinDLL::Timeout,
        this, [=]{ displayError(BSError::PinTimeout); });
}


void MainWindow::initRfid() // DONE
{
    if (Default::IsLogged)
    {
        connect(m_rfid, &RfidDLL::Logger,
                this, [=](QString logged){ logger(Logger::Rfid, logged); });
    }

    connect(m_rfid, &RfidDLL::ErrorHappened,
            this, &MainWindow::displayError);

    if (m_rfid->readData(Default::Port))
    {
        connect(m_rfid, &RfidDLL::CardRead,
                this, &MainWindow::onCardRead);
    }
}


/** ============= VIEW INITIALIZATIONS =================== */


void MainWindow::initMainView() // DONE
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

    connect(ui->mainView, &MainView::ToGift,
            this, &MainWindow::toGiftView);

    connect(ui->mainView, &MainView::ToInvoices,
            this, &MainWindow::toInvoiceView);
}


void MainWindow::initStartView() // DONE
/**
  * Initializes StartView with any required connections.
  */
{
    connect(ui->startView, &StartView::ReadCard,
            this, &MainWindow::readCard);

    connect(ui->startView, &StartView::TestLogin,
            this, [=]{ onCardRead(Default::Card); });
}


void MainWindow::initGiftView()
{
    connect(m_db, &DatabaseDLL::BalanceChanged,
           ui->giftView, &GiftView::setGiftable);

    connect(ui->giftView, &GiftView::Transfer,
            this, &MainWindow::onTransfer);
}


void MainWindow::initInvoicesView()
{
    connect(ui->invoiceView, &InvoiceView::PayInvoice,
            this, &MainWindow::onPayInvoice);

    connect(ui->invoiceView, &InvoiceView::NoInvoices,
            this, [this]{
                previousView();
                displayInfo("No invoices available.");
            });

    connect(m_db, &DatabaseDLL::BalanceChanged,
            ui->invoiceView, &InvoiceView::setLimit);
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
        logout();

}


void MainWindow::onDeposit(const float amount)
{
    if (m_db->deposit(amount))
        displayInfo(BSMessage::Deposit);
    else
        displayError(BSError::Deposit);
}


void MainWindow::onTransfer(QString IBAN, float amount)
{
    if (m_db->transfer(IBAN, amount))
        displayInfo(BSMessage::Transfer);
    else
        displayError(BSError::Transfer);
}


void MainWindow::onPayInvoice(const int invoiceId)
{
    if (m_db->payInvoice(invoiceId))
    {
        displayInfo(BSMessage::Invoice);
        ui->invoiceView->setInvoices(*m_db->getOpenInvoices());
    }
    else
        displayError(BSError::Invoice);
}


void MainWindow::onWithdrawal(const float amount)
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


void MainWindow::onCardRead(const QString& cardNumber)
{
    if (m_cardNumber.isEmpty())
    {
        m_cardNumber = cardNumber;
        m_pin->getPin(this);
    }
}

void MainWindow::toSummaryView()
/**
  * Fetches any data shown in the summary view and displays it before changing the view.
  */
{
    QAbstractItemModel* recentEvents = m_db->getRecentEvents(Default::NumRecentEvents);
    ui->summaryView->setEvents(*recentEvents);

    setCurrentView(ui->summaryView);
}


void MainWindow::toWithdrawalView()
{
    setCurrentView(ui->withdrawalView);
}


void MainWindow::toEventView()
{
    ui->eventView->setEvents(*m_db->getEvents());
    setCurrentView(ui->eventView);
}

void MainWindow::toDepositView()
{
    setCurrentView(ui->depositView);
}


void MainWindow::toGiftView()
{
    ui->giftView->setReceivers(*m_db->getOtherAccounts());
    setCurrentView(ui->giftView);
}

void MainWindow::toInvoiceView()
{
    setCurrentView(ui->invoiceView);
    ui->invoiceView->setInvoices(*m_db->getOpenInvoices());
}


void MainWindow::onPinEntered(const int pinCode)
{
    if (!m_db->login(m_cardNumber, pinCode))
    {
        m_cardNumber = "";
        return;
    }


    QString accOwner = m_db->getAccountOwner();
    QString accNumber = m_db->getAccountNumber();

    ui->summaryView->setOwner(accOwner);
    ui->summaryView->setAccountNumber(accNumber);

     setCurrentView(ui->mainView);
}


void MainWindow::showBalance(const float balance)
/**
  * Updates the balance displayed.
  */
{
    double balanceAsDouble = static_cast<double>(balance);

    QString balanceString;
    balanceString.sprintf("BAL: %9.2f€", balanceAsDouble);

    ui->labelBalance->setText(balanceString);
}


void MainWindow::logout()
/**
  * Does any cleaning up when the user logouts.
  */
{
    m_cardNumber = "";

    // Make sure account data is cleared on logout.

    ui->summaryView->clear();
    ui->invoiceView->clear();
    ui->giftView->clear();
    ui->eventView->clear();

    // Make sure to logout from the database.
    m_db->logout();

    displayInfo(BSMessage::Logout);
}


/** ======= LOGGING AND MESSAGES ======= */


void MainWindow::logger(const QString& source, const QString& description)
/**
  * Logs information to the application output.
  *
  * Params:
  * source (QString) of the log statement.
  * description (QString)
  */
{
    qDebug() << QString("LOGGING - src: %1, desc: %2").arg(source, description);
}


void MainWindow::displayInfo(const QString& message)
/**
  * Displays informative message to the user.
  *
  * Params:
  * message (QString) being displayed.
  */
{
    QMessageBox::information(this, "", message);
}


void MainWindow::displayError(const QString& message)
/**
  * Displays error message to the user.
  *
  * Params:
  * message (QString) being displayed.
  */
{
    QMessageBox::warning(this, "Error occured", message);
}


