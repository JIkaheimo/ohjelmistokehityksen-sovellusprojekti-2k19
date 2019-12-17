#include "card.h"

#include <QSqlQuery>
#include <QSqlTableModel>

QString Card::validate(const QString& cardNumber, const int cardPin) const
/**
  * Checks if the given card information is valid. Returns
  * IBAN of the associated account if one exists.
  */
{
    QSqlQuery query;
    query.prepare(
        "SELECT account FROM Card "
        "WHERE number=:number AND pin=:pin"
    );

    query.bindValue(":number", cardNumber);
    query.bindValue(":pin", cardPin);

    if (query.exec() && query.first())
        return query.value(0).toString();

    return "";
}
