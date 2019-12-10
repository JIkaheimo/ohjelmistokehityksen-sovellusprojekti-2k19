#ifndef WITHDRAWALVIEW_H
#define WITHDRAWALVIEW_H

#include <QPushButton>
#include <QWidget>

namespace Ui {
class WithdrawalView;
}

class WithdrawalView : public QWidget
{
    Q_OBJECT

public:
    explicit WithdrawalView(QWidget *parent = nullptr);
    ~WithdrawalView();

signals:
    void Withdraw(float amount);

public slots:
    void setWithdrawable(float amount);

private slots:
    void onXWithdrawal();

private:
    Ui::WithdrawalView *ui;
    void connectWithdraw(QPushButton *button, float amount);
};

#endif // WITHDRAWALVIEW_H
