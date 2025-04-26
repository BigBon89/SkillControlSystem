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
    bool GetTests(QStringList& testNames);
    bool GetTest(const QString& name, QString& test);
    bool GetTestForClient(const QString& name, QString& test);
private:
    bool CreateTestsTable();
    QSqlDatabase db;
};

#endif // DATABASE_H
