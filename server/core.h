#ifndef CORE_H
#define CORE_H

#include <QObject>
#include <QTcpServer>

class Core : public QObject {
    Q_OBJECT
public:
    Core(QObject* parent = nullptr);
    void onNewConnection();
private:
    QTcpServer* server;
};

#endif // CORE_H
