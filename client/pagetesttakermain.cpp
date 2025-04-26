#include <QVBoxLayout>
#include <QPushButton>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include "pagetesttakermain.h"
#include "enumpages.h"

PageTestTakerMain::PageTestTakerMain(MainWindow* parent) : QWidget{parent} {
    QVBoxLayout* layout = new QVBoxLayout(this);
    QPushButton* buttonBack = new QPushButton("Назад", this);
    QPushButton* buttonNext = new QPushButton("Далее", this);
    layout->addWidget(buttonBack);

    layout->addLayout(questionsLayout);

    layout->addWidget(buttonNext);

    connect(buttonNext, &QPushButton::clicked, this, [parent]() {
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
        qDebug() << questionText;
    }
}
