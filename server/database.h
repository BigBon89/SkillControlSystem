#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QString>
#include <QSqlDatabase>

class DataBase : public QObject {
    Q_OBJECT
public:
    DataBase(QObject* parent = nullptr);
    bool GetTests(QStringList& testNames);
    bool InsertTest(const QString& testname, const QString& testdata, const qint32& maxpoints);
    bool GetTestdata(const QString& testname, QString& testdata);
    bool GetTestForClient(const QString& testname, QString& testdata);
    bool InsertResult(const QString& username, const QString& testname, const qint32& points);
private:
    bool CreateTestsTable();
    bool CreateResultsTable();
    QSqlDatabase db;
};

#endif // DATABASE_H
