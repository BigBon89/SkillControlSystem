#include <QVBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QFile>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include "pagetestcreatormain.h"
#include "pagetestcreatorend.h"
#include "enumpages.h"

PageTestCreatorEnd::PageTestCreatorEnd(MainWindow* parent) : QWidget{parent} {
    QVBoxLayout* layout = new QVBoxLayout(this);
    QPushButton* buttonBack = new QPushButton("Назад", this);
    QLineEdit* lineEdit = new QLineEdit(this);
    lineEdit->setPlaceholderText("Имя теста");
    QPushButton* buttonSave = new QPushButton("Сохранить тест", this);
    layout->addWidget(buttonBack);
    layout->addWidget(lineEdit);
    layout->addWidget(buttonSave);

    connect(buttonBack, &QPushButton::clicked, this, [parent]() {
        parent->SetPage((int)Pages::PageTestCreatorMain);
    });

    connect(buttonSave, &QPushButton::clicked, this, [parent, lineEdit]() {
        if (lineEdit->text().isEmpty()) {
            return;
        }
        PageTestCreatorMain* pageTestCreatorMain = (PageTestCreatorMain*)parent->GetPage((int)Pages::PageTestCreatorMain);
        auto questions = pageTestCreatorMain->GetQuestions();
        QJsonArray questionsArray;

        for (const auto& question : questions) {
            QJsonObject questionObject;
            questionObject["question"] = std::get<0>(question)->text();
            questionObject["answer"] = std::get<1>(question)->text();
            questionObject["points"] = std::get<2>(question)->value();
            questionsArray.append(questionObject);
        }

        QJsonObject rootObject;
        rootObject["questions"] = questionsArray;

        QJsonDocument doc(rootObject);

        QString result;
        QString toServerMessage = lineEdit->text() + "|" + doc.toJson();
        parent->GetNetwork()->Send("sendtest", toServerMessage, result);
    });
}
