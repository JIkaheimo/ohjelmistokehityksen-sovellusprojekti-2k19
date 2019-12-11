#ifndef CARD_H
#define CARD_H

#include "table.h"

#include <QObject>
#include <QSqlRecord>

class QSqlTableModel;
class QSqlDatabase;

class Card : public Table
{

public:

    Card(QSqlDatabase& db);

    int validate(QString cardNumber, int cardPin);

private:
    QSqlRecord selectItem(int id) override;

};

#endif // CARD_H


