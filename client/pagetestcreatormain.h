#ifndef PAGETESTCREATORMAIN_H
#define PAGETESTCREATORMAIN_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QSpinBox>
#include "mainwindow.h"

class PageTestCreatorMain : public QWidget {
    Q_OBJECT
public:
    PageTestCreatorMain(MainWindow* parent = nullptr);
    QList<std::tuple<QLineEdit*, QLineEdit*, QSpinBox*>> GetQuestions();
private slots:
    void AddQuestion();
private:
    QVBoxLayout* questionsLayout;
    QList<std::tuple<QLineEdit*, QLineEdit*, QSpinBox*>> questions;
};

#endif // PAGETESTCREATORMAIN_H
