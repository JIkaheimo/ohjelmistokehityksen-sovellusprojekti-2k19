#ifndef GIFTVIEW_H
#define GIFTVIEW_H

#include <QWidget>

namespace Ui {
class GiftView;
}

class QAbstractItemModel;

class GiftView : public QWidget
{
    Q_OBJECT

public:
    explicit GiftView(QWidget *parent = nullptr);
    ~GiftView();

public slots:
    void setGiftable(const float amount);
    void setReceivers(QAbstractItemModel& receivers);
    void clear();

signals:
    void Transfer(QString IBAN, float amount);

private slots:
    void onTransfer();

private:
    Ui::GiftView *ui;
};



#endif // GIFTVIEW_H
