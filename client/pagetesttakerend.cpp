#include <QVBoxLayout>
#include <QPushButton>
#include "pagetesttakerend.h"
#include "enumpages.h"

PageTestTakerEnd::PageTestTakerEnd(MainWindow* parent) : QWidget{parent} {
    QVBoxLayout* layout = new QVBoxLayout(this);
    QPushButton* buttonBack = new QPushButton("Назад", this);
    QPushButton* buttonNext = new QPushButton("Завершить тест", this);
    layout->addWidget(buttonBack);
    layout->addWidget(buttonNext);

    connect(buttonBack, &QPushButton::clicked, this, [parent]() {
        parent->SetPage((int)Pages::PageTestTakerMain);
    });

    connect(buttonNext, &QPushButton::clicked, this, []() {

    });
}
