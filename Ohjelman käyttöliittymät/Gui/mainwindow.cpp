#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "eventview.h"
#include "mainview.h"
#include "depositview.h"
#include "withdrawalview.h"
#include "startview.h"

#include <QMessageBox>
#include <QDebug>


const QString PORT = "COM3";

using namespace std::placeholders;

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent),
    mDB(new DatabaseDLL(this)),
    mRFID(new RfidDLL(PORT, this)),
    mPin(new PinDLL()),
    mPageHistory(),

    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(
        mRFID, &RfidDLL::CardRead,
        this, &MainWindow::cardRead
    );

    connect(
        mDB, &DatabaseDLL::BalanceChanged,
        this, &MainWindow::showBalance
    );

    connect(
        mPin, &PinDLL::PinEntered,
        this, &MainWindow::pinEntered
    );


    connect(
        ui->btnBack, &QPushButton::clicked,
        this, &MainWindow::previousPage
    );

    // Enable logging, disable for release
    connect(
        mPin, &PinDLL::Logger,
        [](QString source, QString desc){
            qDebug() << QString("%1 '%2'").arg(source).arg(desc);
        }
    );

    connect(
        mDB, &DatabaseDLL::Logger,
        [](QString logged){ qDebug() << logged << endl; }
    );

    connect(
        mDB, &DatabaseDLL::ErrorHappened,
        [this](QString message){ QMessageBox::warning(this, "Database Error", message); }
    );

    connect(
        mRFID, &RfidDLL::Logger,
        [](QString logged){ qDebug() << logged << endl; }
    );

    initStartView();
    initMainView();
    initWithdrawalView();
    initDepositView();
    initEventView();
    setCurrentPage(*ui->pageStart);
}

MainWindow::~MainWindow()
{
    delete mPin;
    delete ui;
    mPin = nullptr;
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
        this, &MainWindow::withdraw
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
        this, &MainWindow::deposit
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
        [this, eventView](){ eventView->setEvents(mDB->getEvents()); }
    );
}


void MainWindow::setCurrentPage(QWidget& page)
{
    // Save the previous page to page history.

    QWidget* currentPage = ui->pageStack->currentWidget();
    mPageHistory.push(currentPage);
    ui->pageStack->setCurrentWidget(&page);

    ui->btnBack->show();

    if (&page == ui->pageStart)
    {
        ui->btnBack->hide();
        ui->labelBalance->setText("");
    }
    else if (&page == ui->pageMain)
    {
        ui->btnBack->setText("Logout");
    }
    else
    {
        ui->btnBack->setText("Back");
    }
}

void MainWindow::previousPage()
{
    if (!mPageHistory.isEmpty())
    {
        QWidget* previousPage = mPageHistory.pop();
        ui->pageStack->setCurrentWidget(previousPage);
    }
}


void MainWindow::readCard()
{
   mRFID->readData();
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
    ui->labelBalance->show();

    QString balanceString;
    balanceString.sprintf("Account balance: %.2f€", static_cast<double>(balance));

    ui->labelBalance->setText(balanceString);
}

void MainWindow::withdraw(float amount)
{
    if (!mDB->withdraw(amount))
    {
        QMessageBox::information(this,
            "Account balance insufficient",
            "Could not complete the transaction, because account balance is insufficient."
        );
    }
}

void MainWindow::deposit(float depositAmount)
{
    if (!mDB->deposit(depositAmount))
    {
        QMessageBox::information(this, "Failed to deposit",
            "Failed to deposit funds..."
        );
    }
}



