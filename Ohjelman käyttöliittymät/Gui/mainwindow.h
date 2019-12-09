#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "databasedll.h"
#include <rfiddll.h>

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{

    Q_OBJECT
private:
    DatabaseDLL* mDB;
    RfidDLL* mRFID;

    QStack<QWidget*> mPageHistory;
    QString mCardNumber = "";
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void pinEntered(int pinCode);
    void withdraw(float amount);
    void deposit(float amount);

    void previousPage();

    void cardRead(QString cardNumber);
    void readCard();
    void test();

    void setCurrentPage(QWidget& page);
private:
    Ui::MainWindow *ui;

    void showBalance(float balance);

    // View initializators
    void initWithdrawalView();
    void initDepositView();
};
#endif // MAINWINDOW_H
