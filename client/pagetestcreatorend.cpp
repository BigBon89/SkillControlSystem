#include <QVBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QFile>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include "pagetestcreatormain.h"
#include "pagetestcreatorend.h"

PageTestCreatorEnd::PageTestCreatorEnd(MainWindow* parent) : QWidget{parent} {
    QVBoxLayout* layout = new QVBoxLayout(this);
    QPushButton* buttonBack = new QPushButton("Назад", this);
    QLineEdit* lineEdit = new QLineEdit("Имя теста", this);
    QPushButton* buttonSave = new QPushButton("Сохранить тест", this);
    layout->addWidget(buttonBack);
    layout->addWidget(lineEdit);
    layout->addWidget(buttonSave);

    connect(buttonBack, &QPushButton::clicked, this, [parent]() {
        parent->SetPage(2);
    });

    connect(buttonSave, &QPushButton::clicked, this, [parent, lineEdit]() {
        PageTestCreatorMain* pageTestCreatorMain = dynamic_cast<PageTestCreatorMain*>(parent->GetPage(2));
        auto questionsInputAnswer = pageTestCreatorMain->GetQuestionsInputAnswer();
        QString fileName = lineEdit->text().trimmed() + ".test";

        QJsonArray questionsArray;

        for (const auto& question : questionsInputAnswer) {
            QJsonObject questionObject;
            questionObject["question"] = std::get<0>(question)->text();
            questionObject["answer"] = std::get<1>(question)->text();
            questionObject["points"] = std::get<2>(question)->value();
            questionsArray.append(questionObject);
        }

        QJsonObject rootObject;
        rootObject["questions"] = questionsArray;

        QJsonDocument doc(rootObject);

        QFile file(fileName);
        if (file.open(QIODevice::WriteOnly)) {
            file.write(doc.toJson());
            file.close();
            qDebug() << ("Тест сохранен");
        }
        QString result;
        QString toServerMessage = lineEdit->text() + "|" + doc.toJson();
        parent->GetNetwork()->Send("sendtest", toServerMessage, result);
    });
}
