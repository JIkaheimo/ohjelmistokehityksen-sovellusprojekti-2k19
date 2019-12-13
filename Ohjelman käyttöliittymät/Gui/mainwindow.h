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
    void onPinEntered(int pinCode);
    void onCardRead(QString cardNumber);
    void readCard();

    void setCurrentView(QWidget* view);
    void previousView();

    void onWithdrawal(float amount);
    void onDeposit(float amount);

    void toSummaryView();   
    void toWithdrawalView();
    void toEventView();
    void toDepositView();

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

};
#endif // MAINWINDOW_H
