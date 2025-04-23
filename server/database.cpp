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

    if (!query.exec(createTable)) {
        //qDebug() << "Create tests table error: " << query.lastError().text();
        //return false;
    }

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

bool DataBase::GetTests(QStringList& testNames) {
    QSqlQuery query(db);
    if (!query.exec("SELECT name FROM tests")) {
        qDebug() << "Failed to get test names:" << query.lastError().text();
        return false;
    }

    while (query.next()) {
        testNames << query.value(0).toString();
    }

    qDebug() << "Test names got successfully";
    return true;
}

bool DataBase::GetTest(const QString& name, QString& test) {
    QSqlQuery query(db);
    query.prepare(R"(
        SELECT test FROM tests WHERE name = :name
    )");

    query.bindValue(":name", name);

    if (!query.exec()) {
        qDebug() << "Failed to get test: " << query.lastError().text();
        return false;
    }

    if (query.next()) {
        test = query.value(0).toString();
        qDebug() << "Test got successfully";
        return true;
    } else {
        qDebug() << "No test found with name: " << name;
        return false;
    }
}
