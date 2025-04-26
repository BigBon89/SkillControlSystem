#include <QVBoxLayout>
#include <QPushButton>
#include "pagemain.h"
#include "pagetesttakerstart.h"
#include "enumpages.h"

PageMain::PageMain(MainWindow* parent) : QWidget(parent) {
    QVBoxLayout* layout = new QVBoxLayout(this);
    QPushButton* button1 = new QPushButton("Прохождение теста", this);
    QPushButton* button2 = new QPushButton("Создание теста", this);
    layout->addWidget(button1);
    layout->addWidget(button2);

    connect(button1, &QPushButton::clicked, this, [parent]() {
        parent->SetPage((int)Pages::PageTestTakerStart);
        QString result;
        parent->GetNetwork()->Send("gettests", "", result);
        PageTestTakerStart* page = (PageTestTakerStart*)parent->GetPage(1);
        QStringList list = result.split("\n");
        page->listWidget->clear();
        page->listWidget->addItems(list);
    });

    connect(button2, &QPushButton::clicked, this, [parent]() {
        parent->SetPage((int)Pages::PageTestCreatorMain);
    });
}
