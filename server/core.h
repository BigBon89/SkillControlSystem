#ifndef CORE_H
#define CORE_H

#include <QObject>
#include "network.h"
#include "database.h"

class Core : public QObject {
    Q_OBJECT
public:
    Core(QObject* parent = nullptr);
private:
    void HandleIncomingMessage(QTcpSocket* clientSocket, const QByteArray& data);
    void HandleCommands(QTcpSocket* clientSocket, const QString& command, const QString& data);
    Network* network;
    DataBase* db;
};

#endif // CORE_H
