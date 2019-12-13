#ifndef PINDIALOG_H
#define PINDIALOG_H

#include <QDialog>

namespace Ui {
class PinDialog;
}

class QTimer;

class PinDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PinDialog(QWidget *parent = nullptr);
    ~PinDialog();

signals:
    void PinEntered(int pinCode);
    void Logger(QString logged);
    void Timeout();

private slots:
    void onNumberButtonClicked();
    void onOkClicked();
    void onBackspaceClicked();
    void onTimeout();

private:
    Ui::PinDialog *ui;

    QList<QPushButton *> mNumberButtons;

    QString mPinCode;
    QTimer* mTimer;

    void updateButtons();
};

#endif // PINDIALOG_H
