#include <QVBoxLayout>
#include <QPushButton>
#include <QListWidget>
#include <QLabel>
#include "pagetesttakerstart.h"
#include "pagetesttakermain.h"
#include "enumpages.h"

PageTestTakerStart::PageTestTakerStart(MainWindow* parent) : QWidget{parent} {
    QVBoxLayout* layout = new QVBoxLayout(this);
    QPushButton* buttonBack = new QPushButton("Назад", this);
    QLabel* label = new QLabel("Выберите тест", this);
    listWidget = new QListWidget(this);
    listWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    QPushButton* buttonNext = new QPushButton("Далее", this);
    layout->addWidget(buttonBack);
    layout->addWidget(label);
    layout->addWidget(listWidget);
    layout->addWidget(buttonNext);

    connect(buttonBack, &QPushButton::clicked, this, [parent]() {
        parent->SetPage((int)Pages::PageMain);
    });

    connect(buttonNext, &QPushButton::clicked, this, [this, parent]() {
        QString result;
        QListWidgetItem* item = this->listWidget->currentItem();
        if (item == nullptr) {
            return;
        }
        QString testName = item->text();
        parent->GetNetwork()->Send("gettest", testName, result);
        PageTestTakerMain* page = (PageTestTakerMain*)parent->GetPage((int)Pages::PageTestTakerMain);
        page->SetLabelTestName(testName);
        page->AddQuestionsFromJson(result);
        parent->SetPage((int)Pages::PageTestTakerMain);
    });
}
