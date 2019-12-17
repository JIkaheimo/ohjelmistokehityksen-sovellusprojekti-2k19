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
    void onPinEntered(const int pinCode);
    void onCardRead(const QString& cardNumber);
    void readCard();

    // Account events
    void onWithdrawal(const float amount);
    void onDeposit(const float amount);
    void onTransfer(const QString IBAN, const float amount);
    void onPayInvoice(const int invoiceNumber);

    // View navigation
    void toSummaryView();   
    void toWithdrawalView();
    void toEventView();
    void toDepositView();
    void toGiftView();
    void toInvoiceView();

    void setCurrentView(QWidget* view);
    void previousView();

    // Logging and dialogs
    void logger(const QString& source, const QString& description);
    void displayError(const QString& message);
    void displayInfo(const QString& message);

    void logout();
private:
    Ui::MainWindow *ui;

    void showBalance(const float balance);
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
    void initGiftView();
    void initInvoicesView();
};
#endif // MAINWINDOW_H
