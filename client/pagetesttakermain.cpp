#include <QVBoxLayout>
#include <QPushButton>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QScrollArea>
#include "pagetesttakerstart.h"
#include "pagetesttakermain.h"
#include "pagetesttakerend.h"
#include "enumpages.h"

PageTestTakerMain::PageTestTakerMain(MainWindow* parent) : QWidget{parent} {
    labelTestName = new QLabel("null", this);
    QVBoxLayout* layout = new QVBoxLayout(this);
    QHBoxLayout* layout2 = new QHBoxLayout(this);
    layout2->setAlignment(Qt::AlignLeft);
    QPushButton* buttonNext = new QPushButton("Завершить тест", this);

    QLabel* labelTextTestName = new QLabel("Имя теста:", this);
    layout2->addWidget(labelTextTestName);
    layout2->addWidget(labelTestName);
    layout->addLayout(layout2);

    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    QWidget* scrollWidget = new QWidget();
    scrollWidget->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Preferred);
    scrollWidget->setMinimumWidth(scrollArea->viewport()->width());

    questionsLayout = new QVBoxLayout(scrollWidget);
    scrollWidget->setLayout(questionsLayout);

    scrollArea->setWidget(scrollWidget);
    layout->addWidget(scrollArea);

    layout->addWidget(buttonNext);

    connect(buttonNext, &QPushButton::clicked, this, [this, parent]() {
        QJsonArray questionsArray;

        for (const auto& question : questions) {
            if (std::get<1>(question)->text() == "") {
                return;
            }
        }

        for (const auto& question : questions) {
            QJsonObject questionObject;
            questionObject["question"] = std::get<0>(question)->text();
            questionObject["answer"] = std::get<1>(question)->text();
            questionsArray.append(questionObject);
        }

        PageTestTakerStart* pageTestTakerStart = (PageTestTakerStart*)parent->GetPage((int)Pages::PageTestTakerStart);

        QJsonObject rootObject;
        rootObject["username"] = pageTestTakerStart->GetUsername();
        rootObject["testname"] = labelTestName->text();
        rootObject["answers"] = questionsArray;

        QJsonDocument doc(rootObject);
        QString result;
        parent->GetNetwork()->Send("checktest", doc.toJson(), result);
        PageTestTakerEnd* pageTestTakerEnd = (PageTestTakerEnd*)parent->GetPage((int)Pages::PageTestTakerEnd);
        pageTestTakerEnd->SetResult(result);

        QLayoutItem* item;
        while ((item = questionsLayout->takeAt(0)) != nullptr) {
            if (item->widget()) {
                delete item->widget();
            }
            delete item;
        }
        questions.clear();

        parent->SetPage((int)Pages::PageTestTakerEnd);
    });
}

void PageTestTakerMain::AddQuestionsFromJson(const QString& json) {
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(json.toUtf8(), &error);

    if (error.error != QJsonParseError::NoError) {
        return;
    }

    if (!doc.isArray()) {
        return;
    }

    QJsonArray questionsArray = doc.array();

    for (const QJsonValue& val : questionsArray) {
        if (!val.isObject()) {
            continue;
        }

        QJsonObject obj = val.toObject();
        QString questionText = obj["question"].toString();

        QHBoxLayout* questionLayout2 = new QHBoxLayout();
        QLabel* questionLabel = new QLabel(questionText, this);
        QLabel* answerLabel = new QLabel("Ваш ответ:", this);
        QLineEdit* answerEdit = new QLineEdit(this);
        questionLayout2->addWidget(questionLabel);
        questionLayout2->addWidget(answerLabel);
        questionLayout2->addWidget(answerEdit);
        questionsLayout->addLayout(questionLayout2);
        questions.append({questionLabel, answerEdit});
    }
}

void PageTestTakerMain::SetLabelTestName(const QString& newValue) {
    labelTestName->setText(newValue);
}
