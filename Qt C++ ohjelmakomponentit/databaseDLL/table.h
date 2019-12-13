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
    QSqlTableModel* mModel;
    int mLastId = -1;

private:
    virtual QSqlRecord selectItem(int id)=0;

};

#endif // TABLE_H

