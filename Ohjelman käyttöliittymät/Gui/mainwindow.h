#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "aloitusview.h"
#include "databasedll.h"
#include "koontiview.h"
#include <rfiddll.h>

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    DatabaseDLL* db;
    RfidDLL* rfid;

    AloitusView* aloitusNakyma;
    KoontiView* koontiNakyma;

    QString korttinumero = "";
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void pinLuettu(QString pinKoodi);
    void suoritaTalletus(float);

    void cardRead(QString cardNumber);
private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
