#ifndef KOONTIVIEW_H
#define KOONTIVIEW_H

#include <QSqlTableModel>
#include <QWidget>

namespace Ui {
class KoontiView;
}

class KoontiView : public QWidget
{
    Q_OBJECT

public:
    explicit KoontiView(QWidget *parent = nullptr);
    ~KoontiView();

    void setEvents(QSqlTableModel* eventModel);

signals:
    void deposit(float depositAmount);
    void withdraw(float withdrawAmount);

private slots:
    void onDepositClicked();
    void onWithdrawClicked();

private:
    Ui::KoontiView *ui;
};

#endif // KOONTIVIEW_H
