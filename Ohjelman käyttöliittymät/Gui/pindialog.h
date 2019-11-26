#ifndef PINDIALOG_H
#define PINDIALOG_H

#include <QDialog>

namespace Ui {
class PinDialog;
}

class PinDialog : public QDialog
{
    Q_OBJECT

public:
    const int PIN_SIZE = 4;

    explicit PinDialog(QWidget *parent = nullptr);
    ~PinDialog();

signals:
    void pinEntered(QString pin);

private slots:
    void onNumberButtonClicked();

private:
    Ui::PinDialog *ui;
    QString pinKoodi = "";

    void addButtonEvent(QPushButton*);
};

#endif // PINDIALOG_H
