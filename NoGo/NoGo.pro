#-------------------------------------------------
#
# Project created by QtCreator 2017-12-31T15:20:55
#
#-------------------------------------------------

QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = NoGo
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    gamesetterface.cpp \
    gamehelperface.cpp \
    mainmenuface.cpp \
    chessboard.cpp \
    gamemessage.cpp \
    playingface.cpp \
    chess.cpp \
    waitingarea.cpp \
    outerroom.cpp \
    innerroom.cpp \
    player.cpp \
    setroombasemessage.cpp

HEADERS += \
        mainwindow.h \
    gamesetterface.h \
    gamehelperface.h \
    mainmenuface.h \
    chessboard.h \
    gamemessage.h \
    playingface.h \
    chess.h \
    waitingarea.h \
    outerroom.h \
    innerroom.h \
    player.h \
    setroombasemessage.h

FORMS += \
        mainwindow.ui \
    gamesetterface.ui \
    gamehelperface.ui \
    mainmenuface.ui \
    chessboard.ui \
    playingface.ui \
    chess.ui \
    waitingarea.ui \
    outerroom.ui \
    innerroom.ui \
    player.ui \
    setroombasemessage.ui

SUBDIRS += \
    NoGo.pro

DISTFILES +=

RESOURCES += \
    resource.qrc

CONFIG += c++14

