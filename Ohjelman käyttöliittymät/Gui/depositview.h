#ifndef DEPOSITVIEW_H
#define DEPOSITVIEW_H

#include <QWidget>

namespace Ui {
class DepositView;
}

class DepositView : public QWidget
{
    Q_OBJECT

public:
    explicit DepositView(QWidget *parent = nullptr);
    ~DepositView();

signals:
    void Deposit(float amount);

private slots:
    void onDeposit();

private:
    Ui::DepositView *ui;
    double getDeposit();
};

#endif // DEPOSITVIEW_H
