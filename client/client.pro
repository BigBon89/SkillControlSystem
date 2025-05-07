QT += core network gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    network.cpp \
    pagemain.cpp \
    pagetestcreatorend.cpp \
    pagetestcreatormain.cpp \
    pagetesttakerend.cpp \
    pagetesttakermain.cpp \
    pagetesttakerstart.cpp

HEADERS += \
    enumpages.h \
    mainwindow.h \
    network.h \
    pagemain.h \
    pagetestcreatorend.h \
    pagetestcreatormain.h \
    pagetesttakerend.h \
    pagetesttakermain.h \
    pagetesttakerstart.h

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    config.json
