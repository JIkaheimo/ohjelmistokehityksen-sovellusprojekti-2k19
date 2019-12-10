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

private:
    Ui::DepositView *ui;
};

#endif // DEPOSITVIEW_H
