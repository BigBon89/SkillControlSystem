#include <QTcpSocket>
#include "core.h"

Core::Core(QObject *parent) : QObject{parent}, server(new QTcpServer(this)) {
    connect(server, &QTcpServer::newConnection, this, &Core::onNewConnection);
    if (!server->listen(QHostAddress::Any, 1234)) {
        qDebug() << "Не удалось запустить сервер!";
    } else {
        qDebug() << "Сервер запущен!";
    }
}

void Core::onNewConnection() {
    QTcpSocket* clientSocket = server->nextPendingConnection();
    connect(clientSocket, &QTcpSocket::readyRead, this, [clientSocket]() {
        QByteArray data = clientSocket->readAll();
        qDebug() << "Получено сообщение: " << data;
        clientSocket->write("Сообщение получено!");
        clientSocket->disconnectFromHost();
    });
}
