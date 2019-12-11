#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "eventview.h"
#include "mainview.h"
#include "depositview.h"
#include "withdrawalview.h"
#include "startview.h"
#include "summaryview.h"

#include "databasedll.h"
#include "rfiddll.h"
#include "pindll.h"

#include <QMessageBox>
#include <QDebug>
#include <ui_mainwindow.h>

const QString WITHDRAWAL_MSG = "Succesfully withdrawed money from the account. Remember to take the bills from the dispenser.";
const QString WITHDRAWAL_ERROR = "Account doesn't have enough funds for the withdrawal.";

const QString PIN_TIMEOUT_MSG = "Please input your pin code within 10 seconds.";

const QString PORT = "COM3";



MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent),

    mDB(new DatabaseDLL()),
    mRFID(new RfidDLL()),
    mPin(new PinDLL()),
    mPageHistory(new QStack<QWidget*>),

    ui(new Ui::MainWindow)
{

    // GUI initialization
    ui->setupUi(this);

    connect(
        ui->btnBack, &QPushButton::clicked,
        this, &MainWindow::previousPage
    );

    initStartView();
    initMainView();
    initWithdrawalView();
    initDepositView();
    initEventView();
    mSummaryView = new SummaryView(this);
    ui->layoutSummary->addWidget(mSummaryView);
    ui->pageStack->setCurrentWidget(ui->pageStart);

    show();

    // Library initialization
    initRfid();
    initDB();
    initPin();
}

MainWindow::~MainWindow()
{
    delete ui;

    delete mRFID;
    mRFID = nullptr;

    delete mPin;
    mPin = nullptr;

    delete mDB;
    mDB = nullptr;
}


void MainWindow::initDB()
{
    connect(
        mDB, &DatabaseDLL::Logger,
        [this](QString logged){
            logger("DatabaseDLL", logged);
        }
    );

    connect(
        mDB, &DatabaseDLL::ErrorHappened,
        this, &MainWindow::displayError
    );

    connect(
        mDB, &DatabaseDLL::BalanceChanged,
        this, &MainWindow::showBalance
    );

}

void MainWindow::initPin()
{
    connect(
        mPin, &PinDLL::Logger,
        this, &MainWindow::logger
    );

    connect(
        mPin, &PinDLL::PinEntered,
        this, &MainWindow::pinEntered
    );

    connect(
        mPin, &PinDLL::Timeout,
        [this]{
            displayError(PIN_TIMEOUT_MSG);
        }
    );
}

void MainWindow::initRfid()
{
    connect(
        mRFID, &RfidDLL::Logger,
        [this](QString logged){ logger("RfidDLL", logged); }
    );

    connect(
        mRFID, &RfidDLL::ErrorHappened,
        this, &MainWindow::displayError
    );

    if (mRFID->readData(PORT))
    {
        connect(
            mRFID, &RfidDLL::CardRead,
            this, &MainWindow::cardRead
        );
    }
}


/** VIEW INITIALIZATIONS */


void MainWindow::initMainView()
/**
  * Initializes MainView with any required connections.
  */
{
    // Create view.
    mMainView = new MainView(this);
    ui->layoutMain->addWidget(mMainView);

    // Navigation connections.

    connect(
        mMainView, &MainView::ToOverview,
        [this]{
            mSummaryView->setOwner(mDB->getAccountOwner());
            mSummaryView->setAccountNumber(mDB->getAccountNumber());
            mSummaryView->setEvents(mDB->getRecentEvents(5));
            setCurrentPage(ui->pageSummary);
         }
    );

    connect(
        mMainView, &MainView::ToWithdrawal,
        [this]{
            setCurrentPage(ui->pageWithdrawal);
        }
    );

    connect(
        mMainView, &MainView::ToEvents,
        [this]{
            mEventView->setEvents(mDB->getEvents());
            setCurrentPage(ui->pageEvents);
        }
    );

    connect(
        mMainView, &MainView::ToDeposit,
        [this]{
            setCurrentPage(ui->pageDeposit);
        }
    );
}


void MainWindow::initStartView()
/**
  * Initializes StartView with any required connections.
  */
{
    // Create view.
    mStartView = new StartView(this);
    ui->layoutStart->addWidget(mStartView);

    connect(
        mStartView, &StartView::ReadCard,
        this, &MainWindow::readCard
    );

    // Fake login
    connect(
        mStartView, &StartView::TestLogin,
        [this]{
            cardRead("0A0079C7BF");
        }
    );

}


void MainWindow::initWithdrawalView()
/**
  * Initializes WithdrawalView with any required connections.
  */
{
    // Create view.
    mWithdrawalView = new WithdrawalView(this);
    ui->layoutWithdrawal->addWidget(mWithdrawalView);

    // Listen for any withdrawal events.
    connect(
        mWithdrawalView, &WithdrawalView::Withdraw,
        this, &MainWindow::onWithdrawal
    );

    // Update withdrawal controls based on the account balance.
    connect(
        mDB, &DatabaseDLL::BalanceChanged,
        mWithdrawalView, &WithdrawalView::setWithdrawable
    );
}


void MainWindow::initDepositView()
/**
  * Initializes DepositView with any required connections.
  */
{
    // Create view.
    mDepositView = new DepositView(this);
    ui->layoutDeposit->addWidget(mDepositView);

    // Listen for any deposit events.
    connect(
        mDepositView, &DepositView::Deposit,
        mDB, &DatabaseDLL::deposit
    );
}


void MainWindow::initEventView()
/**
  * Initializes EventView with any required connections.
  */
{
    // Create view.
    mEventView = new EventView(this);
    ui->layoutEvents->addWidget(mEventView);

    // Listen for any login events.

}


void MainWindow::setCurrentPage(QWidget* page)
{
    // Save the previous page to page history.
    QWidget* currentPage = ui->pageStack->currentWidget();
    mPageHistory->push(currentPage);

    showPage(page);
}


void MainWindow::showPage(QWidget* page)
{
    bool isStartPage = page == ui->pageStart;
    bool isMainPage = page == ui->pageMain;

    bool showBackButton = !isStartPage;
    bool showBalance = !isStartPage;

    QString btnText = (isMainPage) ? "Logout" : "Back";

    ui->pageStack->setCurrentWidget(page);

    ui->labelBalance->setVisible(showBalance);

    ui->btnBack->setText(btnText);
    ui->btnBack->setVisible(showBackButton);
    ui->labelHeader->setVisible(isStartPage);
}


void MainWindow::previousPage()
{    
    if (!mPageHistory->isEmpty())
    {
        QWidget* previousPage = mPageHistory->pop();
        showPage(previousPage);
    }

    if (mPageHistory->isEmpty())
        // TODO: "Logout" from the database interface.
        displayInfo("Thank you for using BankSimul. See you again!");

}


void MainWindow::onWithdrawal(float amount)
{
    if (mDB->withdraw(amount))
        displayInfo(WITHDRAWAL_MSG);
    else
        displayError(WITHDRAWAL_ERROR);
}

void MainWindow::readCard()
{
   mRFID->readData(PORT);
}


void MainWindow::cardRead(QString cardNumber)
{
    mCardNumber = cardNumber;
    mPin->getPin(this);
}


void MainWindow::pinEntered(int pinCode)
{

    if (mDB->login(mCardNumber, pinCode))
        setCurrentPage(ui->pageMain);
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


