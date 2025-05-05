#include <QVBoxLayout>
#include <QPushButton>
#include <QListWidget>
#include <QLabel>
#include <QLineEdit>
#include "pagetesttakerstart.h"
#include "pagetesttakermain.h"
#include "enumpages.h"

PageTestTakerStart::PageTestTakerStart(MainWindow* parent) : QWidget{parent} {
    QVBoxLayout* layout = new QVBoxLayout(this);
    QPushButton* buttonBack = new QPushButton("Назад", this);
    QLabel* label = new QLabel("Выберите тест", this);
    lineEditUsername = new QLineEdit(this);
    lineEditUsername->setPlaceholderText("Имя тестируемого");
    listWidget = new QListWidget(this);
    listWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    QPushButton* buttonNext = new QPushButton("Далее", this);
    layout->addWidget(buttonBack);
    layout->addWidget(label);
    layout->addWidget(listWidget);
    layout->addWidget(lineEditUsername);
    layout->addWidget(buttonNext);

    connect(buttonBack, &QPushButton::clicked, this, [parent]() {
        parent->SetPage((int)Pages::PageMain);
    });

    connect(buttonNext, &QPushButton::clicked, this, [this, parent]() {
        QString result;
        if (lineEditUsername->text().isEmpty()) {
            return;
        }
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

QString PageTestTakerStart::GetUsername() {
    return lineEditUsername->text();
}
