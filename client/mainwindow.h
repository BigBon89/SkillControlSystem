#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include "network.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    void SetPage(int index);
    QWidget* GetPage(int index);
    Network* network;
private:
    QStackedWidget* stackedWidget;
};
#endif // MAINWINDOW_H
