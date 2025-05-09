#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QtCore/qjsonarray.h>
#include <QFile>
#include "core.h"

Core::Core(QObject* parent)
    : QObject{parent} {

#ifdef QT_DEBUG
    QFile file("../../../server/config.json");
#else
    QFile file("config.json");
#endif
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open" << file.fileName();
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

    QJsonObject root = doc.object();
    QJsonObject networkObj = root["network"].toObject();
    QJsonObject databaseObj = root["database"].toObject();

    QString ip = networkObj["ip"].toString();
    qint32 port = networkObj["port"].toInt();

    network = new Network(ip, port);

    QString hostname = databaseObj.value("hostname").toString();
    qint32 databasePort = databaseObj.value("port").toString().toInt();
    QString databaseName = databaseObj.value("dbname").toString();
    QString username = databaseObj.value("username").toString();
    QString password = databaseObj.value("password").toString();

    db = new DataBase(hostname, databasePort, databaseName, username, password);

    connect(network, &Network::MessageReceived, this, &Core::HandleIncomingMessage);
}

void Core::HandleIncomingMessage(QTcpSocket* clientSocket, const QByteArray& data) {
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);

    if (parseError.error != QJsonParseError::NoError || !doc.isObject()) {
        qDebug() << "JSON parse error:" << parseError.errorString();
        return;
    }

    QJsonObject obj = doc.object();
    QString command = obj.value("command").toString();
    QString message = obj.value("message").toString();

    HandleCommands(clientSocket, command, message);
}

void Core::HandleCommands(QTcpSocket* clientSocket, const QString& command, const QString& data) {
    if (command == "sendtest") {
        QJsonParseError parseError;
        QJsonDocument doc = QJsonDocument::fromJson(data.toUtf8(), &parseError);

        if (parseError.error != QJsonParseError::NoError) {
            qDebug() << "JSON parse error:" << parseError.errorString();
            return;
        }

        if (!doc.isObject()) {
            return;
        }

        QJsonObject obj = doc.object();
        db->InsertTest(obj["testname"].toString(), QString(QJsonDocument(obj["testdata"].toArray()).toJson()), obj["maxpoints"].toInt());
    } else if (command == "gettests") {
        QStringList list;
        db->GetTests(list);
        network->Send(clientSocket, list.join('\n'));
    } else if (command == "gettest") {
        QString testdata;
        db->GetTestForClient(data, testdata);
        network->Send(clientSocket, testdata);
    } else if (command == "checktest") {
        QString result;
        CheckTest(data, result);
        network->Send(clientSocket, result);
    }
}

void Core::CheckTest(const QString& test, QString& result) {
    QJsonParseError parseError;

    QJsonDocument docClient = QJsonDocument::fromJson(test.toUtf8(), &parseError);
    if (parseError.error != QJsonParseError::NoError) {
        qDebug() << "Parsing error JSON: " << parseError.errorString();
        return;
    }

    if (!docClient.isObject()) {
        qDebug() << "Error: Core::CheckTest docClient is not object";
        return;
    }

    QJsonObject objClient = docClient.object();
    QJsonArray arrayClient = objClient["answers"].toArray();

    QString testFromDB;
    QString username = objClient["username"].toString();
    QString testname = objClient["testname"].toString();
    db->GetTestdata(testname, testFromDB);
    QJsonDocument docDB = QJsonDocument::fromJson(testFromDB.toUtf8(), &parseError);
    if (parseError.error != QJsonParseError::NoError) {
        qDebug() << "Parsing error JSON: " << parseError.errorString();
        return;
    }

    if (!docDB.isArray()) {
        qDebug() << "Error: Core::CheckTest docDB is not object";
        return;
    }

    QJsonArray arrayDB = docDB.array();

    qint32 points = 0;

    for (const auto& question : arrayDB) {
        QJsonObject objQuestion = question.toObject();
        for (const auto& answer : arrayClient) {
            QJsonObject objAnswer = answer.toObject();
            if (objQuestion["question"].toString().trimmed().toLower() != objAnswer["question"].toString().trimmed().toLower()) {
                continue;
            }
            if (objQuestion["answer"].toString().trimmed().toLower() == objAnswer["answer"].toString().trimmed().toLower()) {
                points += objQuestion["points"].toInt();
            }
        }
    }

    result = QString::number(points);

    db->InsertResult(username, testname, points);
}

void Core::ReadConfig() {

}
