#ifndef TRANSACTIONVIEW_H
#define TRANSACTIONVIEW_H

#include <QWidget>

namespace Ui {
class TransactionView;
}

class TransactionView : public QWidget
{
    Q_OBJECT

public:
    explicit TransactionView(QWidget *parent = nullptr);
    ~TransactionView();

signals:
    void Transfer(int receiverId, float amount);

private:
    Ui::TransactionView *ui;
};

#endif // TRANSACTIONVIEW_H
