#include "card.h"

#include <QSqlTableModel>

const QString TABLE = "card";
const QString ID = "id";
const QString NUMBER = "identifier";
const QString PIN = "pin";
const QString ACCOUNT_ID = "idAccount";


Card::Card(QSqlDatabase& db) :
    Table(db, TABLE)
{
}

QSqlRecord Card::selectItem(int cardId)
{
    mModel->setFilter(QString("%1 = %2").arg(ID).arg(cardId));
    mModel->select();
    return mModel->record(0);
}

int Card::validate(QString cardNumber, int cardPin)
{
    mModel->setFilter(QString("%1 = '%2' AND %3 = %4")
      .arg(NUMBER)
      .arg(cardNumber)
      .arg(PIN)
      .arg(cardPin));

    mModel->select();

    if (mModel->rowCount() == 1)
    {
        return mModel->record(0).value(ACCOUNT_ID).toInt();
    }

    return -1;
}
