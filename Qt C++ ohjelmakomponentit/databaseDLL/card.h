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
    static const QString TABLE;
    static const QString ID;
    static const QString NUMBER;
    static const QString PIN;
    static const QString ACCOUNT_ID;

    Card(QSqlDatabase& db);

    int validate(QString cardNumber, int cardPin);

private:
    QSqlRecord selectItem(int id) override;

};

#endif // CARD_H


