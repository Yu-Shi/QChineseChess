#-------------------------------------------------
#
# Project created by QtCreator 2018-09-04T09:17:18
#
#-------------------------------------------------

QT       += core gui
QT += network
QT += multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QChineseChess_Server
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
    dialoginit.cpp \
    dialogwait.cpp \
    chessboard.cpp \
    chesscell.cpp \
    utili.cpp \
    dialogconnect.cpp

HEADERS += \
        mainwindow.h \
    dialoginit.h \
    dialogwait.h \
    chessboard.h \
    chesscell.h \
    utili.h \
    dialogconnect.h

FORMS += \
        mainwindow.ui \
    dialoginit.ui \
    dialogwait.ui \
    dialogconnect.ui

RESOURCES += \
    res.qrc
