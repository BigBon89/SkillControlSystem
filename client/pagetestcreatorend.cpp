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
        qint32 maxpoints = 0;
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
            qint32 points = std::get<2>(question)->value();
            maxpoints += points;
            questionObject["points"] = points;
            questionsArray.append(questionObject);
        }

        QJsonObject rootObject;
        rootObject["testname"] = lineEdit->text();
        rootObject["testdata"] = questionsArray;
        rootObject["maxpoints"] = maxpoints;

        QJsonDocument doc(rootObject);
        qWarning() << doc.toJson();
        QString result;
        parent->GetNetwork()->Send("sendtest", doc.toJson(), result);
    });
}
