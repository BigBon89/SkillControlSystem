#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QFile>
#include "network.h"

Network::Network(QObject* parent) : QObject{parent} {
    socket = new QTcpSocket();
    ReadConfig();
    connect(socket, &QTcpSocket::disconnected, this, &Network::onDisconnected);
}

bool Network::Connect() {
    while(true) {
        socket->abort();
        socket->deleteLater();

        socket = new QTcpSocket(this);
        connect(socket, &QTcpSocket::disconnected, this, &Network::onDisconnected);

        socket->connectToHost(ip, port);

        if (!socket->waitForConnected(3000)) {
            Dialog();
            continue;
        }

        return true;
    }
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

void Network::onDisconnected() {
    Dialog();
    Connect();
}

void Network::Dialog() {
    QMessageBox msgBox;
    msgBox.setWindowTitle("Ошибка подключения");
    msgBox.setText("Не удалось подключиться к серверу.");
    msgBox.setInformativeText("Повторить попытку?");
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.setStandardButtons(QMessageBox::Retry | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Retry);

    int ret = msgBox.exec();

    if (ret == QMessageBox::Cancel) {
        exit(0);
    }
}

void Network::ReadConfig() {
#ifdef QT_DEBUG
    QFile file("../../../../../../client/config.json");
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

    QJsonObject obj = doc.object();
    ip = obj["ip"].toString();
    port = obj["port"].toInt();
}
