#ifndef ALOITUSVIEW_H
#define ALOITUSVIEW_H

#include <QWidget>

namespace Ui {
class AloitusView;
}

class AloitusView : public QWidget
{
    Q_OBJECT

public:
    explicit AloitusView(QWidget *parent = nullptr);
    ~AloitusView();

private slots:
    void on_btnAloita_clicked();

signals:
    void korttiLuettu(QString korttinumero);

private:
    Ui::AloitusView *ui;
};

#endif // ALOITUSVIEW_H
