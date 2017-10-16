#-------------------------------------------------
#
# Project created by QtCreator 2017-05-25T14:13:38
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtProjekt
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    painter.cpp \
    mesh.cpp \
    sphere.cpp \
    scene.cpp \
    camera.cpp \
    tools.cpp

HEADERS  += mainwindow.h \
    painter.h \
    mesh.h \
    sphere.h \
    scene.h \
    camera.h \
    tools.h

FORMS    += mainwindow.ui

RESOURCES += \
    res.qrc
