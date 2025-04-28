#include <QSqlQuery>
#include <QSqlError>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include "database.h"

DataBase::DataBase(QObject* parent)
    : QObject{parent},
    db{QSqlDatabase::addDatabase("QPSQL")} {
    QFile file("../../../server/config.json");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open config.json";
        return;
    }

    QByteArray jsonData = file.readAll();
    file.close();

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(jsonData, &parseError);
    if (parseError.error != QJsonParseError::NoError) {
        qDebug() << "JSON parse error:" << parseError.errorString();
        return;
    }

    if (!doc.isObject()) {
        qDebug() << "Invalid JSON format: root is not an object";
        return;
    }

    QJsonObject obj = doc.object();

    QString hostname = obj.value("hostname").toString();
    int port = obj.value("port").toString().toInt();
    QString dbname = obj.value("dbname").toString();
    QString username = obj.value("username").toString();
    QString password = obj.value("password").toString();

    db.setHostName(hostname);
    db.setPort(port);
    db.setDatabaseName(dbname);
    db.setUserName(username);
    db.setPassword(password);

    if (db.open()) {
        qDebug() << "Database connected";
    } else {
        qDebug() << "Database connection failed: " << db.lastError().text();
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
        //qDebug() << "Failed to insert test:" << query.lastError().text();
        return false;
    }

    //qDebug() << "Test inserted successfully";
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

    //qDebug() << "Test names got successfully";
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
        //qDebug() << "Test got successfully";
        return true;
    } else {
        //qDebug() << "No test found with name: " << name;
        return false;
    }
}

bool DataBase::GetTestForClient(const QString& name, QString& test) {
    QString testRaw;
    if (!GetTest(name, testRaw)) {
        return false;
    }

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(testRaw.toUtf8(), &parseError);

    if (parseError.error != QJsonParseError::NoError || !doc.isObject()) {
        qDebug() << "JSON parse error:" << parseError.errorString();
        return false;
    }

    QJsonObject obj = doc.object();

    if (obj.contains("questions") && obj["questions"].isArray()) {
        QJsonArray questions = obj["questions"].toArray();
        for (int i = 0; i < questions.size(); i++) {
            if (questions[i].isObject()) {
                QJsonObject questionObj = questions[i].toObject();
                questionObj.remove("answer");
                questionObj.remove("points");
                questions[i] = questionObj;
            }
        }
        obj["questions"] = questions;
    }

    test = QJsonDocument(obj).toJson(QJsonDocument::Compact);
    return true;
}
