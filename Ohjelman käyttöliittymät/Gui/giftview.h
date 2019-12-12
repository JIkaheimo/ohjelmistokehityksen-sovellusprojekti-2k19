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

private:
    Ui::GiftView *ui;
};

#endif // GIFTVIEW_H
