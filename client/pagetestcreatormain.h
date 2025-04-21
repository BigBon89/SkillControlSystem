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
    QList<std::tuple<QLineEdit*, QLineEdit*, QSpinBox*>> GetQuestionsInputAnswer();
private slots:
    void AddQuestionInputAnswer();
private:
    QVBoxLayout* questionsLayout;
    QList<std::tuple<QLineEdit*, QLineEdit*, QSpinBox*>> questionsInputAnswer;
};

#endif // PAGETESTCREATORMAIN_H
