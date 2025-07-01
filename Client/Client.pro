QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    CardItem.cpp \
    GamePage.cpp \
    History.cpp \
    HistoryPage.cpp \
    JsonHandler.cpp \
    PlayerInfo.cpp \
    UserController.cpp \
    UserPage.cpp \
    editprofile.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    ButtonItem.h \
    CardItem.h \
    GamePage.h \
    History.h \
    HistoryPage.h \
    JsonHandler.h \
    PauseOverlay.h \
    PlayerInfo.h \
    UserController.h \
    UserPage.h \
    editprofile.h \
    mainwindow.h

FORMS += \
    GamePage.ui \
    UserPage.ui \
    editprofile.ui \
    historypage.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Font.qrc \
    assets.qrc \
    images.qrc
