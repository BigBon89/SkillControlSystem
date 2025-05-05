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
    CreateResultsTable();
}

bool DataBase::CreateTestsTable() {
    QSqlQuery query(db);
    QString createTable = R"(
        CREATE TABLE IF NOT EXISTS tests (
            id SERIAL PRIMARY KEY,
            testname TEXT NOT NULL,
            testdata TEXT NOT NULL,
            maxpoints integer NOT NULL
        )
    )";

    if (!query.exec(createTable)) {

    }

    return true;
}

bool DataBase::CreateResultsTable() {
    QSqlQuery query(db);
    QString createTable = R"(
        CREATE TABLE IF NOT EXISTS results (
            id SERIAL PRIMARY KEY,
            username TEXT NOT NULL,
            testname TEXT NOT NULL,
            points integer NOT NULL,
            end_date TIMESTAMP NOT NULL DEFAULT date_trunc('second', now())
        )
    )";

    if (!query.exec(createTable)) {

    }

    return true;
}

bool DataBase::InsertTest(const QString& testname, const QString& testdata, const qint32& maxpoints) {
    QSqlQuery query(db);
    query.prepare(R"(
        INSERT INTO tests (testname, testdata, maxpoints)
        VALUES (:testname, :testdata, :maxpoints)
    )");

    query.bindValue(":testname", testname);
    query.bindValue(":testdata", testdata);
    query.bindValue(":maxpoints", maxpoints);

    if (!query.exec()) {
        qDebug() << "DataBase::InsertTest failed: " << query.lastError().text();
        return false;
    }

    return true;
}

bool DataBase::InsertResult(const QString& username,
                            const QString& testname,
                            const qint32& points
) {
    QSqlQuery query(db);
    query.prepare(R"(
        INSERT INTO results (username, testname, points)
        VALUES (:username, :testname, :points)
    )");

    query.bindValue(":username", username);
    query.bindValue(":testname", testname);
    query.bindValue(":points", points);

    if (!query.exec()) {
        qDebug() << "DataBase::InsertResult failed: " << query.lastError().text();
        return false;
    }
    return true;
}

bool DataBase::GetTests(QStringList& testNames) {
    QSqlQuery query(db);
    if (!query.exec("SELECT testname FROM tests")) {
        qDebug() << "DataBase::GetTests failed: " << query.lastError().text();
        return false;
    }

    while (query.next()) {
        testNames << query.value(0).toString();
    }

    return true;
}

bool DataBase::GetTestdata(const QString& testname, QString& testdata) {
    QSqlQuery query(db);
    query.prepare(R"(
        SELECT testdata FROM tests WHERE testname = :testname
    )");

    query.bindValue(":testname", testname);

    if (!query.exec()) {
        qDebug() << "[DB] GetTest failed: " << query.lastError().text();
        return false;
    }

    if (query.next()) {
        testdata = query.value(0).toString();
        return true;
    } else {
        return false;
    }
}

bool DataBase::GetTestForClient(const QString& testname, QString& testdata) {
    QString testRaw;
    if (!GetTestdata(testname, testRaw)) {
        return false;
    }

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(testRaw.toUtf8(), &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        qDebug() << "DataBase::GetTestForClient JSON parse error:" << parseError.errorString();
        return false;
    }

    if (!doc.isArray()) {
        qDebug() << "Error: DataBase::GetTestForClient doc is not array";
        return false;
    }

    QJsonArray questions = doc.array();
    QJsonArray cleanedQuestions;

    for (const QJsonValue& val : questions) {
        QJsonObject questionObj = val.toObject();
        questionObj.remove("answer");
        questionObj.remove("points");
        cleanedQuestions.append(questionObj);
    }

    testdata = QJsonDocument(cleanedQuestions).toJson();
    return true;
}
