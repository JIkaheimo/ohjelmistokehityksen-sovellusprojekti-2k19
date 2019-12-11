#include "table.h"

#include <QSqlTableModel>

Table::Table(const QSqlDatabase& db, const QString& table)
{
    mModel = new QSqlTableModel(this, db);
    mModel->setTable(table);
}

Table::~Table()
{
    delete mModel;
    mModel = nullptr;
}
