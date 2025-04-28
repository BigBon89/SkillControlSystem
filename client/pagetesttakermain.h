#ifndef PAGETESTTAKERMAIN_H
#define PAGETESTTAKERMAIN_H

#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QVBoxLayout>
#include "mainwindow.h"

class PageTestTakerMain : public QWidget {
    Q_OBJECT
public:
    PageTestTakerMain(MainWindow* parent = nullptr);
    void AddQuestionsFromJson(const QString& json);
    void SetLabelTestName(const QString& newValue);
private:
    QLabel* labelTestName;
    QVBoxLayout* questionsLayout;
    QList<QPair<QLabel*, QLineEdit*>> questions;
};

#endif // PAGETESTTAKERMAIN_H
