QT += core network sql

CONFIG += c++17 cmdline

SOURCES += \
        core.cpp \
        database.cpp \
        main.cpp \
        network.cpp

HEADERS += \
    core.h \
    database.h \
    network.h \
    utils.h

macx {
    INCLUDEPATH += /opt/homebrew/opt/libpq/include
    LIBS += -L/opt/homebrew/opt/libpq/lib -lpq
}

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    config.json
