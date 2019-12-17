#ifndef TABLE_H
#define TABLE_H

#include <QObject>
#include <QSqlRecord>

class QSqlDatabase;
class QSqlTableModel;
class QString;

class Table : public QObject
{
public:
    Table(const QSqlDatabase& db, const QString& name);
    ~Table();

protected:
    QSqlTableModel* m_model;


};

#endif // TABLE_H

