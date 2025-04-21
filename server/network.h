#ifndef NETWORK_H
#define NETWORK_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

class Network : public QObject {
    Q_OBJECT
public:
    Network(QObject* parent = nullptr);
private slots:
    void OnNewConnection();
    void Read(QTcpSocket* clientSocket);
signals:
    void MessageReceived(const QByteArray& data);
private:
    QTcpServer* server;
};

#endif // NETWORK_H
