#ifndef GIFTVIEW_H
#define GIFTVIEW_H

#include <QWidget>

namespace Ui {
class GiftView;
}

class GiftView : public QWidget
{
    Q_OBJECT

public:
    explicit GiftView(QWidget *parent = nullptr);
    ~GiftView();

signals:
    void Transfer(float amount, int account);

private:
    Ui::GiftView *ui;
    void onTransaction();
    void transfer(float summa, int tiliNumero);
};

#endif // GIFTVIEW_H

