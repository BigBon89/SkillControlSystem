#include <QPushButton>
#include <QLabel>
#include "pagetestcreatormain.h"
#include "enumpages.h"

PageTestCreatorMain::PageTestCreatorMain(MainWindow* parent) : QWidget{parent} {
    QVBoxLayout* layout = new QVBoxLayout(this);
    QPushButton* buttonBack = new QPushButton("Назад", this);
    QPushButton* buttonAddQuestion = new QPushButton("Добавить вопрос (Вписать ответ)", this);
    QPushButton* buttonNext = new QPushButton("Далее", this);
    layout->addWidget(buttonBack);
    layout->addWidget(buttonAddQuestion);

    questionsLayout = new QVBoxLayout();
    layout->addLayout(questionsLayout);

    layout->addWidget(buttonNext);

    connect(buttonBack, &QPushButton::clicked, this, [parent]() {
        parent->SetPage((int)Pages::PageMain);
    });

    connect(buttonNext, &QPushButton::clicked, this, [this, parent]() {
        auto questionsInputAnswer = GetQuestions();
        if (questionsInputAnswer.empty()) {
            return;
        }

        for (const auto& input : questionsInputAnswer) {
            auto [lineEditQuestion, lineEditAnswer, spinBoxPoints] = input;

            if (lineEditQuestion->text().isEmpty() || lineEditAnswer->text().isEmpty() || spinBoxPoints->value() == 0) {
                return;
            }
        }

        parent->SetPage((int)Pages::PageTestCreatorEnd);
    });

    connect(buttonAddQuestion, &QPushButton::clicked, this, &PageTestCreatorMain::AddQuestion);
}

QList<std::tuple<QLineEdit*, QLineEdit*, QSpinBox*>> PageTestCreatorMain::GetQuestions() {
    return questions;
}

void PageTestCreatorMain::AddQuestion() {
    QHBoxLayout* questionLayout = new QHBoxLayout();

    QLabel* questionLabel = new QLabel("Вопрос:", this);
    QLineEdit* questionEdit = new QLineEdit(this);
    QLabel* answerLabel = new QLabel("Правильный ответ:", this);
    QLineEdit* answerEdit = new QLineEdit(this);
    QLabel* pointsLabel = new QLabel("Баллы за правильный ответ:", this);
    QSpinBox* pointsSpinBox = new QSpinBox(this);
    pointsSpinBox->setMinimum(1);

    QPushButton* deleteButton = new QPushButton("Удалить вопрос", this);
    connect(deleteButton, &QPushButton::clicked, this, [this, questionLayout, questionLabel, questionEdit, answerLabel, answerEdit, pointsLabel, pointsSpinBox, deleteButton]() {
        questionsLayout->removeItem(questionLayout);

        auto it = std::find_if(questions.begin(), questions.end(), [questionEdit, answerEdit, pointsSpinBox](const std::tuple<QLineEdit*, QLineEdit*, QSpinBox*>& item) {
            return std::get<0>(item) == questionEdit && std::get<1>(item) == answerEdit && std::get<2>(item) == pointsSpinBox;
        });
        if (it != questions.end()) {
            questions.erase(it);
        }

        delete questionLabel;
        delete questionEdit;
        delete answerLabel;
        delete answerEdit;
        delete pointsLabel;
        delete pointsSpinBox;
        delete deleteButton;
        delete questionLayout;
    });

    questionLayout->addWidget(questionLabel);
    questionLayout->addWidget(questionEdit);
    questionLayout->addWidget(answerLabel);
    questionLayout->addWidget(answerEdit);
    questionLayout->addWidget(pointsLabel);
    questionLayout->addWidget(pointsSpinBox);
    questionLayout->addWidget(deleteButton);

    questionsLayout->addLayout(questionLayout);

    questions.append({questionEdit, answerEdit, pointsSpinBox});
}
