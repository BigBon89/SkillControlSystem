#include <QVBoxLayout>
#include <QPushButton>
#include "pagetesttakermain.h"

PageTestTakerMain::PageTestTakerMain(MainWindow* parent) : QWidget{parent} {
    QVBoxLayout* layout = new QVBoxLayout(this);
    QPushButton* button = new QPushButton("Назад", this);
    layout->addWidget(button);

    connect(button, &QPushButton::clicked, this, [parent]() {
        parent->SetPage(0);
    });
}
