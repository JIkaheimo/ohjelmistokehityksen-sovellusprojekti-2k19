#include "giftview.h"
#include "ui_giftview.h"

GiftView::GiftView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GiftView)
{
    ui->setupUi(this);
}

GiftView::~GiftView()
{
    delete ui;
}
