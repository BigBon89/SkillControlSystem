#include <QJsonDocument>
#include <QJsonObject>
#include "network.h"

Network::Network(QObject *parent) : QObject{parent} {
    socket = new QTcpSocket();
}

bool Network::Connect(QString ip, int port) {
    socket->connectToHost(ip, port);

    if (!socket->waitForConnected(3000)) {
        qDebug() << "Не удалось подключиться к серверу!";
        return false;
    }
    qDebug() << "Подключение установлено!";
    return true;
}

bool Network::Send(QString command, QString message, QString& result) {
    QJsonObject json;
    json["command"] = command;
    json["message"] = message;

    QJsonDocument doc(json);
    QByteArray data = doc.toJson(QJsonDocument::Compact);

    socket->write(data);

    if (socket->waitForBytesWritten(3000)) {

    }

    if (socket->waitForReadyRead(3000)) {
        QByteArray data = socket->readAll();
        result = QString::fromUtf8(data);
    }

    return true;
}
