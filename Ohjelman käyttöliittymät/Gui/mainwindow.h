#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainView;
class StartView;
class WithdrawalView;
class DepositView;
class EventView;
class SummaryView;

class DatabaseDLL;
class RfidDLL;
class PinDLL;


class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    DatabaseDLL* mDB;
    RfidDLL* mRFID;
    PinDLL* mPin;

    MainView* mMainView;
    StartView* mStartView;
    WithdrawalView* mWithdrawalView;
    DepositView* mDepositView;
    EventView* mEventView;
    SummaryView* mSummaryView;

    QStack<QWidget*>* mPageHistory;
    QString mCardNumber = "";

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void pinEntered(int pinCode);

    void previousPage();

    void cardRead(QString cardNumber);
    void readCard();

    void setCurrentPage(QWidget* page);

    void logger(QString source, QString description);
    void displayError(QString message);
    void displayInfo(QString message);

    void onWithdrawal(float amount);
private:
    Ui::MainWindow *ui;

    void showBalance(float balance);
    void showPage(QWidget* page);

    // DLL initializators
    void initRfid();
    void initDB();
    void initPin();

    // View initializators
    void initWithdrawalView();
    void initDepositView();
    void initEventView();
    void initMainView();
    void initStartView();

};
#endif // MAINWINDOW_H
