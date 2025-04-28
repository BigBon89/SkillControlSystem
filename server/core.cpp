#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QtCore/qjsonarray.h>
#include "core.h"
#include "utils.h"

Core::Core(QObject* parent)
    : QObject{parent},
    network{new Network()},
    db{new DataBase()} {
    connect(network, &Network::MessageReceived, this, &Core::HandleIncomingMessage);
}

void Core::HandleIncomingMessage(QTcpSocket* clientSocket, const QByteArray& data) {
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);

    if (parseError.error != QJsonParseError::NoError || !doc.isObject()) {
        qDebug() << "Ошибка парсинга JSON: " << parseError.errorString();
        return;
    }

    QJsonObject obj = doc.object();
    QString command = obj.value("command").toString();
    QString message = obj.value("message").toString();

    HandleCommands(clientSocket, command, message);
}

void Core::HandleCommands(QTcpSocket* clientSocket, const QString& command, const QString& data) {
    if (command == "sendtest") {
        auto parsed = ParseTestData(data);
        QString name = parsed.first;
        QString json = parsed.second;
        db->InsertTest(name, json);
    } else if (command == "gettests") {
        QStringList list;
        db->GetTests(list);
        network->Send(clientSocket, list.join('\n'));
    } else if (command == "gettest") {
        QString test;
        db->GetTestForClient(data, test);
        network->Send(clientSocket, test);
    } else if (command == "checktest") {
        QString result;
        CheckTest(data, result);
        network->Send(clientSocket, result);
    }
}

void Core::CheckTest(const QString& test, QString& result) {
    QJsonParseError parseError;

    QJsonDocument docClient = QJsonDocument::fromJson(test.toUtf8(), &parseError);
    if (parseError.error != QJsonParseError::NoError || !docClient.isObject()) {
        qDebug() << "Ошибка парсинга JSON: " << parseError.errorString();
        return;
    }
    QJsonObject objClient = docClient.object();
    QJsonArray arrayClient = objClient["answers"].toArray();

    QString testFromDB;
    db->GetTest(objClient["testname"].toString(), testFromDB);
    QJsonDocument docDB = QJsonDocument::fromJson(testFromDB.toUtf8(), &parseError);
    if (parseError.error != QJsonParseError::NoError || !docDB.isObject()) {
        qDebug() << "Ошибка парсинга JSON: " << parseError.errorString();
        return;
    }
    QJsonObject objDB = docDB.object();
    QJsonArray arrayDB = objDB["questions"].toArray();

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
}
