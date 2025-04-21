#ifndef PAGEMAIN_H
#define PAGEMAIN_H

#include <QWidget>
#include "mainwindow.h"

class PageMain : public QWidget {
    Q_OBJECT
public:
    PageMain(MainWindow* parent = nullptr);
};

#endif // PAGEMAIN_H
