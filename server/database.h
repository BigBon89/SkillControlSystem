#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QString>
#include <QSqlDatabase>

class DataBase : public QObject {
    Q_OBJECT
public:
    DataBase(QObject* parent = nullptr);
    bool InsertTest(const QString& name, const QString& test);
private:
    QSqlDatabase db;
    bool CreateTestsTable();
};

#endif // DATABASE_H
