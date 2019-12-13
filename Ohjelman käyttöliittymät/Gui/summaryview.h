#ifndef SUMMARYVIEW_H
#define SUMMARYVIEW_H

#include <QAbstractItemModel>
#include <QWidget>

namespace Ui {
class SummaryView;
}

class SummaryView : public QWidget
{
    Q_OBJECT

public:
    explicit SummaryView(QWidget *parent = nullptr);
    ~SummaryView();

    void setEvents(QAbstractItemModel& events);
    void setAccountNumber(QString accNumber);
    void setOwner(QString owner);

private:
    Ui::SummaryView *ui;
};

#endif // SUMMARYVIEW_H
