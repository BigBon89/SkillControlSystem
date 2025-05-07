#ifndef NETWORK_H
#define NETWORK_H

#include <QObject>
#include <QTcpSocket>

class Network : public QObject {
    Q_OBJECT
public:
    Network(QObject* parent = nullptr);
    bool Connect();
    bool Send(QString command, QString message, QString& result);
private:
    QTcpSocket* socket;
    void Dialog();
    void ReadConfig();
    QString ip;
    qint32 port;
private slots:
    void onDisconnected();
};

#endif // NETWORK_H
