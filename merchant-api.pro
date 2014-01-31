#-------------------------------------------------
#
# Project created by QtCreator 2013-09-10T11:45:41
#
#-------------------------------------------------

QT       += core
QT       += network

QT       -= gui

TARGET = merchant-api
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    flubit_api.cpp

HEADERS += \
    flubit_api.h \
    request_list.h

OTHER_FILES += \
    readme.txt
