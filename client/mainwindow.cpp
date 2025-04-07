#include <QTcpSocket>
#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    this->resize(800, 600);
    this->setWindowTitle("UserTestingSystem");
    QTcpSocket* socket = new QTcpSocket();

    socket->connectToHost("127.0.0.1", 1234);

    if (!socket->waitForConnected(3000)) {
        qDebug() << "Не удалось подключиться к серверу!";
        return;
    }

    qDebug() << "Подключение установлено!";

    QString message = "Привет, сервер!";
    socket->write(message.toUtf8());

    if (socket->waitForBytesWritten(3000)) {
        qDebug() << "Сообщение отправлено серверу!";
    }
}

MainWindow::~MainWindow() {

}
