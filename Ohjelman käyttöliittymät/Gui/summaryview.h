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

public slots:
    void clear();
    void setEvents(QAbstractItemModel& events);
    void setAccountNumber(const QString& accNumber);
    void setOwner(const QString& owner);


private:
    Ui::SummaryView *ui;
};

#endif // SUMMARYVIEW_H
