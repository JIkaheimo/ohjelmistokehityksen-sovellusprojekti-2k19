#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

#include <QStack>

class DatabaseDLL;
class RfidDLL;
class PinDLL;

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    DatabaseDLL* m_db;
    RfidDLL* m_rfid;
    PinDLL* m_pin;

    QStack<QWidget*> m_viewHistory;
    QString m_cardNumber = "";

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    // RFID & Pin
    void onPinEntered(int pinCode);
    void onCardRead(QString cardNumber);
    void readCard();

    // Account events
    void onWithdrawal(float amount);
    void onDeposit(float amount);
    void onTransaction(int receiverId, float amount);
    void onPayInvoice(int invoiceId);

    // View navigation
    void toSummaryView();   
    void toWithdrawalView();
    void toEventView();
    void toDepositView();
    void toTransactionView();
    void toInvoiceView();

    void setCurrentView(QWidget* view);
    void previousView();

    // Logging and dialogs
    void logger(QString source, QString description);
    void displayError(QString message);
    void displayInfo(QString message);

private:
    Ui::MainWindow *ui;

    void showBalance(float balance);
    void showView(QWidget* view);

    // DLL initializers
    void initRfid();
    bool initDB();
    void initPin();

    // View initializers
    void initWithdrawalView();
    void initDepositView();
    void initEventView();
    void initMainView();
    void initStartView();
    void initTransactionView();
    void initInvoicesView();
};
#endif // MAINWINDOW_H
