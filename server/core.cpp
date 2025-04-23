#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
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
        qDebug() << "Ошибка парсинга JSON:" << parseError.errorString();
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
        db->GetTest(data, test);
        network->Send(clientSocket, test);
    }
}
