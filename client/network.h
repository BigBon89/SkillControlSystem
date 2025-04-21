#ifndef NETWORK_H
#define NETWORK_H

#include <QObject>
#include <QTcpSocket>

class Network : public QObject {
    Q_OBJECT
public:
    Network(QObject* parent = nullptr);
    bool Send(QString message);
    bool Connect(QString ip = "127.0.0.1", int port = 1234);
private:
    QTcpSocket* socket;
};

#endif // NETWORK_H
