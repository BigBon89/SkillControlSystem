#include <QSqlQuery>
#include <QSqlError>
#include "database.h"

DataBase::DataBase(QObject* parent)
    : QObject{parent},
    db{QSqlDatabase::addDatabase("QPSQL")} {
    db.setHostName("localhost");
    db.setPort(5432);
    db.setDatabaseName("alexander");
    db.setUserName("alexander");
    db.setPassword("");

    if (db.open()) {
        qDebug() << "Database connected";
    } else {
        qDebug() << "Database connected failed: " << db.lastError().text();
    }
    CreateTestsTable();
}

bool DataBase::CreateTestsTable() {
    QSqlQuery query(db);
    QString createTable = R"(
        CREATE TABLE IF NOT EXISTS tests (
            id SERIAL PRIMARY KEY,
            name TEXT NOT NULL,
            test TEXT NOT NULL
        )
    )";

    return true;
}

bool DataBase::InsertTest(const QString& name, const QString& test) {
    QSqlQuery query(db);
    query.prepare(R"(
        INSERT INTO tests (name, test)
        VALUES (:name, :test)
    )");

    query.bindValue(":name", name);
    query.bindValue(":test", test);

    if (!query.exec()) {
        qDebug() << "Failed to insert test:" << query.lastError().text();
        return false;
    }

    qDebug() << "Test inserted successfully";
    return true;
}
