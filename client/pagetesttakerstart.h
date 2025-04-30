#ifndef PAGETESTTAKERSTART_H
#define PAGETESTTAKERSTART_H

#include <QWidget>
#include <QListWidget>
#include "mainwindow.h"

class PageTestTakerStart : public QWidget {
    Q_OBJECT
public:
    QString GetUsername();
    PageTestTakerStart(MainWindow* parent = nullptr);
    QListWidget* listWidget;
private:
    QLineEdit* lineEditUsername;
};

#endif // PAGETESTTAKERSTART_H
