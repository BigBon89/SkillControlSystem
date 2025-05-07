#ifndef NETWORK_H
#define NETWORK_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

class Network : public QObject {
    Q_OBJECT
public:
    Network(QString ip, qint32 port, QObject* parent = nullptr);
    void Send(QTcpSocket* clientSocket, const QString& message);
private slots:
    void OnNewConnection();
    void Read(QTcpSocket* clientSocket);
signals:
    void MessageReceived(QTcpSocket* clientSocket, const QByteArray& data);
private:
    QTcpServer* server;
};

#endif // NETWORK_H
