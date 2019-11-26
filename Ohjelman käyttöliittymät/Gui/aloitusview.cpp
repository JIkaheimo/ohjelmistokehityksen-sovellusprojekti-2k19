#include "aloitusview.h"
#include "ui_aloitusview.h"

AloitusView::AloitusView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AloitusView)
{
    ui->setupUi(this);
    connect(ui->btnAloita, &QPushButton::clicked, this, &AloitusView::on_btnAloita_clicked);
}

AloitusView::~AloitusView()
{
    delete ui;
}

void AloitusView::on_btnAloita_clicked()
{
    // TODO: Vaihda käyttämään lukua RfidDLL-komponentista
    emit korttiLuettu("123412341234");
}
