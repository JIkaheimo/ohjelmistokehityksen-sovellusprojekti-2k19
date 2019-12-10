#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "databasedll.h"
#include "rfiddll.h"
#include "pindll.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{

    Q_OBJECT

private:
    DatabaseDLL* mDB;
    RfidDLL* mRFID;
    PinDLL* mPin;

    QStack<QWidget*> mPageHistory;
    QString mCardNumber = "";

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void pinEntered(int pinCode);

    void previousPage();

    void cardRead(QString cardNumber);
    void readCard();
    void test();

    void setCurrentPage(QWidget& page);

    void logger(QString source, QString description);
    void displayError(QString message);

private:
    Ui::MainWindow *ui;

    void showBalance(float balance);

    void initRfid();

    // View initializators
    void initWithdrawalView();
    void initDepositView();
    void initEventView();
    void initMainView();
    void initStartView();

    void showPage(QWidget &page);

};
#endif // MAINWINDOW_H
