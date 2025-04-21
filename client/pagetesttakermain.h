#ifndef PAGETESTTAKERMAIN_H
#define PAGETESTTAKERMAIN_H

#include <QWidget>
#include "mainwindow.h"

class PageTestTakerMain : public QWidget {
    Q_OBJECT
public:
    PageTestTakerMain(MainWindow* parent = nullptr);
};

#endif // PAGETESTTAKERMAIN_H
