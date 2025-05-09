#include "network.h"

Network::Network(QString ip, qint32 port, QObject* parent)
    : QObject{parent},
    server{new QTcpServer(this)} {
    connect(server, &QTcpServer::newConnection, this, &Network::OnNewConnection);
    if (!server->listen(QHostAddress(ip), port)) {
        qDebug() << "Server start error";
    } else {
        qDebug() << "Server started";
    }
}

void Network::OnNewConnection() {
    QTcpSocket* clientSocket = server->nextPendingConnection();
    connect(clientSocket, &QTcpSocket::readyRead, this, [this, clientSocket]() {
        this->Read(clientSocket);
    });
}

void Network::Read(QTcpSocket* clientSocket) {
    QByteArray data = clientSocket->readAll();
    qDebug() << "Message got: " << data;

    emit MessageReceived(clientSocket, data);
}

void Network::Send(QTcpSocket* clientSocket, const QString& message) {
    clientSocket->write(message.toUtf8());
    qDebug() << "Message sended: " << message;
}
