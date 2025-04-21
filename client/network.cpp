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

bool Network::Send(QString message) {
    socket->write(message.toUtf8());

    if (socket->waitForBytesWritten(3000)) {
        qDebug() << "Сообщение отправлено серверу!";
    }
    return true;
}
