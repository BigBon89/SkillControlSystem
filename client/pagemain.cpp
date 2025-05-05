#include <QVBoxLayout>
#include <QPushButton>
#include "pagemain.h"
#include "pagetesttakerstart.h"
#include "enumpages.h"

PageMain::PageMain(MainWindow* parent) : QWidget(parent) {
    QVBoxLayout* layout = new QVBoxLayout(this);
    QPushButton* buttonTaker = new QPushButton("Прохождение теста", this);
    QPushButton* buttonCreate = new QPushButton("Создание теста", this);
    layout->addWidget(buttonTaker);
    layout->addWidget(buttonCreate);

    connect(buttonTaker, &QPushButton::clicked, this, [parent]() {
        parent->SetPage((int)Pages::PageTestTakerStart);
        QString result;
        parent->GetNetwork()->Send("gettests", "", result);
        PageTestTakerStart* page = (PageTestTakerStart*)parent->GetPage(1);
        page->listWidget->clear();
        if (result.isEmpty()) {
            return;
        }
        QStringList list = result.split("\n");
        page->listWidget->addItems(list);
    });

    connect(buttonCreate, &QPushButton::clicked, this, [parent]() {
        parent->SetPage((int)Pages::PageTestCreatorMain);
    });
}
