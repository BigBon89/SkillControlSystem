#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include "pagetesttakerend.h"
#include "enumpages.h"

PageTestTakerEnd::PageTestTakerEnd(MainWindow* parent) : QWidget{parent} {
    labelResult = new QLabel("0", this);
    QVBoxLayout* layout = new QVBoxLayout(this);
    QPushButton* buttonBack = new QPushButton("В главное меню", this);

    layout->addWidget(buttonBack);
    layout->addWidget(labelResult);

    connect(buttonBack, &QPushButton::clicked, this, [parent]() {
        parent->SetPage((int)Pages::PageMain);
    });
}

void PageTestTakerEnd::SetResult(const QString& newValue) {
    labelResult->setText(newValue);
}
