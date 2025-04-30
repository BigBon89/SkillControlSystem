#ifndef NETWORK_H
#define NETWORK_H

#include <QObject>
#include <QTcpSocket>

class Network : public QObject {
    Q_OBJECT
public:
    Network(QObject* parent = nullptr);
    bool Connect(QString ip = "127.0.0.1", int port = 1234);
    bool Send(QString command, QString message, QString& result);
private:
    QTcpSocket* socket;
    void Dialog();
private slots:
    void onDisconnected();
};

#endif // NETWORK_H
