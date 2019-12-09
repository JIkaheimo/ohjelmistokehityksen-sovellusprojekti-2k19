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
    explicit PinDialog(QWidget *parent = nullptr);
    ~PinDialog();

signals:
    void PinEntered(int pinCode);
    void Timeout();

private slots:
    void onNumberButtonClicked();

private:
    Ui::PinDialog *ui;
    QString mPinCode;

    void addButtonEvent(QPushButton*);
};

#endif // PINDIALOG_H
