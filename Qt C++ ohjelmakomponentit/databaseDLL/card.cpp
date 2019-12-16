#include "card.h"

#include <QSqlTableModel>

const QString Card::TABLE = "card";
const QString Card::ID = "id";
const QString Card::NUMBER = "identifier";
const QString Card::PIN = "pin";
const QString Card::ACCOUNT_ID = "idAccount";

Card::Card(QSqlDatabase& db) :
    Table(db, TABLE)
{
}

QSqlRecord Card::selectItem(int cardId)
{
    mModel->setFilter(QString("%1 = %2").arg(ID, QString::number(cardId)));
    mModel->select();
    return mModel->record(0);
}

int Card::validate(QString cardNumber, int cardPin)
{
    mModel->setFilter(QString("%1 = '%2' AND %3 = %4")
      .arg(NUMBER, cardNumber, PIN, QString::number(cardPin)));

    mModel->select();

    if (mModel->rowCount() == 1)
        return mModel->record(0).value(ACCOUNT_ID).toInt();

    return -1;
}
