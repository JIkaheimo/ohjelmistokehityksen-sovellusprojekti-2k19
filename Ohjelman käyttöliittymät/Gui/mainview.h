#ifndef MAINVIEW_H
#define MAINVIEW_H

#include <QWidget>

namespace Ui {
class MainView;
}

class MainView : public QWidget
{
    Q_OBJECT

public:
    explicit MainView(QWidget *parent = nullptr);
    ~MainView();

signals:
    void ToWithdrawal();
    void ToEvents();
    void ToDeposit();
    void ToOverview();

private:
    Ui::MainView *ui;
};

#endif // MAINVIEW_H
