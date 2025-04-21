#include "network.h"

Network::Network(QObject* parent)
    : QObject{parent},
    server{new QTcpServer(this)} {
    connect(server, &QTcpServer::newConnection, this, &Network::OnNewConnection);
    if (!server->listen(QHostAddress::Any, 1234)) {
        qDebug() << "Не удалось запустить сервер!";
    } else {
        qDebug() << "Сервер запущен!";
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
    qDebug() << "Получено сообщение: " << data;

    emit MessageReceived(data);

    clientSocket->write("Сообщение получено!");
}
