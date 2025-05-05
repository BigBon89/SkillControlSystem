#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include "pagetesttakerend.h"
#include "enumpages.h"

PageTestTakerEnd::PageTestTakerEnd(MainWindow* parent) : QWidget{parent} {
    labelResult = new QLabel("null", this);
    QVBoxLayout* layout = new QVBoxLayout(this);
    QHBoxLayout* layoutResults = new QHBoxLayout(this);
    layoutResults->setAlignment(Qt::AlignLeft);
    QPushButton* buttonBack = new QPushButton("В главное меню", this);
    QLabel* labelTextResult = new QLabel("Ваш результат:", this);

    layoutResults->addWidget(labelTextResult);
    layoutResults->addWidget(labelResult);

    layout->addWidget(buttonBack);
    layout->addLayout(layoutResults);

    connect(buttonBack, &QPushButton::clicked, this, [parent]() {
        parent->SetPage((int)Pages::PageMain);
    });
}

void PageTestTakerEnd::SetResult(const QString& newValue) {
    labelResult->setText(newValue);
}
