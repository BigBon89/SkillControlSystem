#ifndef PAGETESTTAKEREND_H
#define PAGETESTTAKEREND_H

#include <QWidget>
#include <QLabel>
#include "mainwindow.h"

class PageTestTakerEnd : public QWidget {
    Q_OBJECT
public:
    PageTestTakerEnd(MainWindow* parent = nullptr);
    void SetResult(const QString& newValue);
private:
    QLabel* labelResult;
};

#endif // PAGETESTTAKEREND_H
