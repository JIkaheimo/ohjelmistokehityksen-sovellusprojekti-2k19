#ifndef EVENTVIEW_H
#define EVENTVIEW_H

#include <QWidget>

namespace Ui {
class EventView;
}

class QAbstractItemModel;

class EventView : public QWidget
{
    Q_OBJECT

public:
    explicit EventView(QWidget *parent = nullptr);
    ~EventView();

public slots:
    void setEvents(QAbstractItemModel* eventModel);

private:
    Ui::EventView *ui;
};

#endif // EVENTVIEW_H
