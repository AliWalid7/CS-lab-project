QT += core gui widgets

CONFIG += c++17

SOURCES += \
    main.cpp \
    userscreen/mainwindow.cpp \
    logic/chatmanager.cpp

HEADERS += \
    userscreen/mainwindow.h \
    logic/chatmanager.h

FORMS += \
    userscreen/mainwindow.ui

INCLUDEPATH += $$PWD/logic $$PWD/userscreen

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
