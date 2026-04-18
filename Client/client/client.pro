QT += core gui widgets

CONFIG += c++17

SOURCES += \
    src/main.cpp \
    src/core/AppState.cpp \
    src/core/AuthManager.cpp \
    logic/ChatManager.cpp \
    src/core/Message.cpp \
    src/gui/ChatWindow.cpp \
    src/gui/LoginWindow.cpp \
    src/gui/MainWindow.cpp \
    src/network/JsonNetworkClient.cpp

HEADERS += \
    src/core/AppState.h \
    src/core/AuthManager.h \
    logic/ChatManager.h \
    src/core/Message.h \
    src/core/types.h \
    src/gui/ChatWindow.h \
    src/gui/LoginWindow.h \
    src/gui/MainWindow.h \
    src/network/JsonNetworkClient.h \
    src/network/Protocol.h

FORMS += \
    src/gui/ChatWindow.ui \
    src/gui/LoginWindow.ui \
    src/gui/MainWindow.ui

INCLUDEPATH += $$PWD/src/core $$PWD/src/gui $$PWD/src/network $$PWD/logic

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
