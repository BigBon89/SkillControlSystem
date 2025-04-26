#include "mainwindow.h"
#include "network.h"
#include "pagemain.h"
#include "pagetesttakerstart.h"
#include "pagetesttakermain.h"
#include "pagetesttakerend.h"
#include "pagetestcreatorend.h"
#include "pagetestcreatormain.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow{parent} {
    this->resize(800, 600);
    this->setWindowTitle("UserTestingSystem");
    network = new Network();
    network->Connect();

    stackedWidget = new QStackedWidget(this);

    PageMain* pageMain = new PageMain(this);
    PageTestTakerStart* pageTestTakerStart = new PageTestTakerStart(this);
    PageTestTakerMain* pageTestTakerMain = new PageTestTakerMain(this);
    PageTestTakerEnd* pageTestTakerEnd = new PageTestTakerEnd(this);
    PageTestCreatorMain* pageTestCreatorMain = new PageTestCreatorMain(this);
    PageTestCreatorEnd* pageTestCreatorEnd = new PageTestCreatorEnd(this);

    stackedWidget->addWidget(pageMain);
    stackedWidget->addWidget(pageTestTakerStart);
    stackedWidget->addWidget(pageTestTakerMain);
    stackedWidget->addWidget(pageTestTakerEnd);
    stackedWidget->addWidget(pageTestCreatorMain);
    stackedWidget->addWidget(pageTestCreatorEnd);

    this->setCentralWidget(stackedWidget);
}

void MainWindow::SetPage(int index) {
    stackedWidget->setCurrentIndex(index);
}

QWidget* MainWindow::GetPage(int index) {
    return stackedWidget->widget(index);
}

Network* MainWindow::GetNetwork() {
    return this->network;
}
