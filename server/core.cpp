#include <QTcpSocket>
#include "core.h"
#include "utils.h"

Core::Core(QObject* parent)
    : QObject{parent},
    network{new Network()},
    db{new DataBase()} {
    connect(network, &Network::MessageReceived, this, &Core::HandleIncomingMessage);
}

void Core::HandleIncomingMessage(const QByteArray& data) {
    qDebug() << "Core получил сообщение: " << data;

    auto parsed = ParseTestData(QString::fromUtf8(data));
    QString name = parsed.first;
    QString json = parsed.second;
    db->InsertTest(name, json);
}
