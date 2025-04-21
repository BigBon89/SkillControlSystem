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
    void HandleIncomingMessage(const QByteArray& data);
    Network* network;
    DataBase* db;
};

#endif // CORE_H
