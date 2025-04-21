#include <QVBoxLayout>
#include <QPushButton>
#include "pagemain.h"

PageMain::PageMain(MainWindow* parent) : QWidget(parent) {
    QVBoxLayout* layout = new QVBoxLayout(this);
    QPushButton* button1 = new QPushButton("Прохождение теста", this);
    QPushButton* button2 = new QPushButton("Создание теста", this);
    layout->addWidget(button1);
    layout->addWidget(button2);

    connect(button1, &QPushButton::clicked, this, [parent]() {
        parent->SetPage(1);
    });

    connect(button2, &QPushButton::clicked, this, [parent]() {
        parent->SetPage(2);
    });
}
