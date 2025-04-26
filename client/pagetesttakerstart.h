#ifndef PAGETESTTAKERSTART_H
#define PAGETESTTAKERSTART_H

#include <QWidget>
#include <QListWidget>
#include "mainwindow.h"

class PageTestTakerStart : public QWidget {
    Q_OBJECT
public:
    PageTestTakerStart(MainWindow* parent = nullptr);
    QListWidget* listWidget;
};

#endif // PAGETESTTAKERSTART_H
