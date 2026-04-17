QT += core gui widgets

CONFIG += c++17

SOURCES += \
    main.cpp \
    userscreen/mainwindow.cpp \
    logic/chatcontroller.cpp

HEADERS += \
    userscreen/mainwindow.h \
    logic/chatcontroller.h

FORMS += \
    userscreen/mainwindow.ui

INCLUDEPATH += $$PWD/logic $$PWD/userscreen

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
