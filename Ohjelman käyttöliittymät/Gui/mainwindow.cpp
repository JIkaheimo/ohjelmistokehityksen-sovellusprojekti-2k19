#include "depositview.h"
#include "withdrawalview.h"
#include "mainwindow.h"
#include "pindialog.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QMessageBox>

const QString PORT = "com3";

using namespace std::placeholders;

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent),
    mDB(new DatabaseDLL(this)),
    mRFID(new RfidDLL(PORT, this)),
    mPageHistory(),

    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Try to initialize connection to the "bank server".
    if (!mDB->initConnection())
    {
        QMessageBox::critical(this, "Connection Error",
            "Could not connect to the database. Make sure you are in OAMK's network..."
        );
    }

    initWithdrawalView();
    initDepositView();

    connect(
        mDB, &DatabaseDLL::BalanceChanged,
        this, &MainWindow::showBalance
    );

    connect(
        ui->btnReadRfid, &QPushButton::clicked,
        this, &MainWindow::readCard
    );

    connect(
        ui->btnTest, &QPushButton::clicked,
        this, &MainWindow::test
    );

    connect(
        ui->btnToSummary, &QPushButton::clicked,
        [this](){ setCurrentPage(*ui->pageSummary); }
    );

    connect(
        ui->btnToWIthdraw, &QPushButton::clicked,
        [this](){ setCurrentPage(*ui->pageWithdrawal); }
    );

    connect(
        ui->btnToDeposit, &QPushButton::clicked,
        [this](){ setCurrentPage(*ui->pageDeposit); }
    );

    connect(
        mRFID, &RfidDLL::cardRead,
        this, &MainWindow::cardRead
    );

    connect(
        ui->btnBack, &QPushButton::clicked,
        this, &MainWindow::previousPage
    );

    connect(
        ui->btnLogout, &QPushButton::clicked,
        this, &MainWindow::previousPage
    );
}

MainWindow::~MainWindow()
{
    delete ui;
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
            "Could not complete the transaction, because account balance is insufficient.");
    }
}

void MainWindow::deposit(float depositAmount)
{
    if (!mDB->deposit(depositAmount))
    {
        QMessageBox::information(this, "Failed to deposit", "Failed to deposit funds...");
    }
}


void MainWindow::initWithdrawalView()
{
    // Initialize withdrawal view.
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
{
    // Initialize deposit view.
    DepositView* depositView = new DepositView(this);
    ui->layoutDeposit->addWidget(depositView);

    // Listen for any deposit events.
    connect(
        depositView, &DepositView::Deposit,
        this, &MainWindow::deposit
    );
}

void MainWindow::setCurrentPage(QWidget& page)
{
    // Save the previous page to page history.
    mPageHistory.push(ui->stackMain->currentWidget());
    ui->stackMain->setCurrentWidget(&page);
}

void MainWindow::previousPage()
{
    if (!mPageHistory.isEmpty())
    {
        QWidget* previousPage = mPageHistory.pop();
        ui->stackMain->setCurrentWidget(previousPage);
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
    PinDialog *pin = new PinDialog(this);

    korttinumero = cardNumber;

    ui->stackMain->setEnabled(false);
    pin->show();

    connect(pin, &PinDialog::pinEntered, this, &MainWindow::pinLuettu);
}


void MainWindow::pinLuettu(QString pinKoodi)
{

    if (mDB->login(korttinumero, pinKoodi.toInt()))
    {
        setCurrentPage(*ui->pageMain);
        showBalance(mDB->getBalance());
        ui->tableEvents->setModel(mDB->getEvents());
    }
    else
    {
        previousPage();
    }
    ui->stackMain->setEnabled(true);

}


