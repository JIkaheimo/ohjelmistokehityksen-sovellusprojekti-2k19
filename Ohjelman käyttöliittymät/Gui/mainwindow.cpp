#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "eventview.h"
#include "mainview.h"
#include "depositview.h"
#include "withdrawalview.h"
#include "startview.h"

#include <QMessageBox>
#include <QDebug>
#include <ui_mainwindow.h>

const QString PIN_TIMEOUT_MSG = "Please input your pin code within 10 seconds.";


const QString PORT = "COM3";

using namespace std::placeholders;

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent),

    mDB(new DatabaseDLL(this)),
    mRFID(new RfidDLL()),
    mPin(new PinDLL()),
    mPageHistory(),

    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(
        mDB, &DatabaseDLL::BalanceChanged,
        this, &MainWindow::showBalance
    );

    connect(
        mPin, &PinDLL::PinEntered,
        this, &MainWindow::pinEntered
    );

    connect(
        mPin, &PinDLL::Timeout,
        [this](){ displayError(PIN_TIMEOUT_MSG); }
    );

    connect(
        ui->btnBack, &QPushButton::clicked,
        this, &MainWindow::previousPage
    );

    // Enable logging, disable for release
    connect(
        mPin, &PinDLL::Logger,
        this, &MainWindow::logger
    );

    connect(
        mDB, &DatabaseDLL::Logger,
        [this](QString logged){ logger("DatabaseDLL", logged); }
    );

    connect(
        mDB, &DatabaseDLL::ErrorHappened,
        this, &MainWindow::displayError
    );

    initStartView();
    initMainView();
    initWithdrawalView();
    initDepositView();
    initEventView();
    setCurrentPage(*ui->pageStart);

    show();

    initRfid();
}

MainWindow::~MainWindow()
{
    delete ui;

    delete mRFID;
    mRFID = nullptr;

    delete mPin;
    mPin = nullptr;
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
    MainView* mainView = new MainView(this);
    ui->layoutMain->addWidget(mainView);

    // Navigation connections.
    connect(
        mainView, &MainView::ToWithdrawal,
        [this]{ setCurrentPage(*ui->pageWithdrawal); }
    );

    connect(
        mainView, &MainView::ToEvents,
        [this]{ setCurrentPage(*ui->pageEvents); }
    );

    connect(
        mainView, &MainView::ToDeposit,
        [this]{ setCurrentPage(*ui->pageDeposit); }
    );
}

void MainWindow::initStartView()
/**
  * Initializes StartView with any required connections.
  */
{
    // Create view.
    StartView* startView = new StartView(this);
    ui->layoutStart->addWidget(startView);

    connect(
        startView, &StartView::ReadCard,
        this, &MainWindow::readCard
    );

    connect(
        startView, &StartView::TestLogin,
        this, &MainWindow::test
    );

}


void MainWindow::initWithdrawalView()
/**
  * Initializes WithdrawalView with any required connections.
  */
{
    // Create view.
    WithdrawalView* withdrawalView = new WithdrawalView(this);
    ui->layoutWithdrawal->addWidget(withdrawalView);

    // Listen for any withdrawal events.
    connect(
        withdrawalView, &WithdrawalView::Withdraw,
        mDB, &DatabaseDLL::withdraw
    );

    // Update withdrawal controls based on the account balance.
    connect(
        mDB, &DatabaseDLL::BalanceChanged,
        withdrawalView, &WithdrawalView::setWithdrawable
    );
}


void MainWindow::initDepositView()
/**
  * Initializes DepositView with any required connections.
  */
{
    // Create view.
    DepositView* depositView = new DepositView(this);
    ui->layoutDeposit->addWidget(depositView);

    // Listen for any deposit events.
    connect(
        depositView, &DepositView::Deposit,
        mDB, &DatabaseDLL::deposit
    );
}


void MainWindow::initEventView()
/**
  * Initializes EventView with any required connections.
  */
{
    // Create view.
    EventView* eventView = new EventView(this);
    ui->layoutEvents->addWidget(eventView);

    // Listen for any login events.
    connect(
        mDB, &DatabaseDLL::UserAuthenticated,
        [this, eventView]{ eventView->setEvents(mDB->getEvents()); }
    );
}


void MainWindow::setCurrentPage(QWidget& page)
{
    // Save the previous page to page history.
    QWidget* currentPage = ui->pageStack->currentWidget();
    mPageHistory.push(currentPage);

    showPage(page);
}

void MainWindow::showPage(QWidget& page)
{
    bool isStartPage = &page == ui->pageStart;
    bool isMainPage = &page == ui->pageMain;

    QPushButton* btn = ui->btnBack;
    QLabel* label = ui->labelBalance;

    ui->pageStack->setCurrentWidget(&page);

    if (isStartPage)
    {
        label->setText("");
        btn->hide();
    }
    else if (isMainPage)
    {
        showBalance(mDB->getBalance());
        btn->show();
        btn->setText("Logout");
    }
    else
    {
        showBalance(mDB->getBalance());
        btn->show();
        btn->setText("Back");
    }
}

void MainWindow::previousPage()
{
    if (!mPageHistory.isEmpty())
    {
        QWidget* previousPage = mPageHistory.pop();
        showPage(*previousPage);
    }
}


void MainWindow::readCard()
{
   mRFID->readData(PORT);
}

void MainWindow::test()
{
    cardRead("0A0079C7BF");
}

void MainWindow::cardRead(QString cardNumber)
{
    mCardNumber = cardNumber;
    mPin->getPin(this);
}


void MainWindow::pinEntered(int pinCode)
{
    if (mDB->login(mCardNumber, pinCode))
    {
        setCurrentPage(*ui->pageMain);
        showBalance(mDB->getBalance());
    }
    else
    {
        previousPage();
    }

}

void MainWindow::showBalance(float balance)
{
    QString balanceString;
    balanceString.sprintf("Account balance: %.2f€", static_cast<double>(balance));

    ui->labelBalance->setText(balanceString);
}


void MainWindow::logger(QString source, QString description)
{
    qDebug() << QString("LOGGING - src: %1, desc: %2").arg(source).arg(description);
}

void MainWindow::displayError(QString message)
{
    QMessageBox::warning(this, "", message);
}


