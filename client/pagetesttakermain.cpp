#include <QVBoxLayout>
#include <QPushButton>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include "pagetesttakermain.h"
#include "pagetesttakerend.h"
#include "enumpages.h"

PageTestTakerMain::PageTestTakerMain(MainWindow* parent) : QWidget{parent} {
    labelTestName = new QLabel("null", this);
    QVBoxLayout* layout = new QVBoxLayout(this);
    QPushButton* buttonBack = new QPushButton("Назад", this);
    QPushButton* buttonNext = new QPushButton("Завершить тест", this);
    QHBoxLayout* layoutTestName = new QHBoxLayout(this);
    layout->addLayout(layoutTestName);
    layout->addWidget(buttonBack);

    questionsLayout = new QVBoxLayout();
    layout->addLayout(questionsLayout);

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

        QJsonObject rootObject;
        rootObject["testname"] = labelTestName->text();
        rootObject["answers"] = questionsArray;

        QJsonDocument doc(rootObject);
        QString result;
        parent->GetNetwork()->Send("checktest", doc.toJson(), result);
        PageTestTakerEnd* page = (PageTestTakerEnd*)parent->GetPage((int)Pages::PageTestTakerEnd);
        page->SetResult(result);
        parent->SetPage((int)Pages::PageTestTakerEnd);
    });

    connect(buttonBack, &QPushButton::clicked, this, [parent]() {
        parent->SetPage((int)Pages::PageTestTakerStart);
    });
}

void PageTestTakerMain::AddQuestionsFromJson(const QString& json) {
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(json.toUtf8(), &error);

    if (error.error != QJsonParseError::NoError) {
        return;
    }

    if (!doc.isObject()) {
        return;
    }

    QJsonObject rootObj = doc.object();
    QJsonArray questionsArray = rootObj["questions"].toArray();

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
