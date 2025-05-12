#-------------------------------------------------
#
# Project created by QtCreator 2015-11-12T13:11:11
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = test_register
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

GENERIC_PATH = ../../generic/

SOURCES += main.cpp\
    $$GENERIC_PATH/variable.cpp \
    $$GENERIC_PATH/register.cpp \
    $$GENERIC_PATH/hamming.cpp \
    $$GENERIC_PATH/bitfieldparser.cpp \
    $$GENERIC_PATH/bit.cpp \
    $$GENERIC_PATH/translator.cpp

HEADERS  += mainwindow.h \
    $$GENERIC_PATH/variable.h \
    $$GENERIC_PATH/register.h \
    $$GENERIC_PATH/hamming.h \
    $$GENERIC_PATH/compiler.h \
    $$GENERIC_PATH/bitfieldparser.h \
    $$GENERIC_PATH/bit.h \
    $$GENERIC_PATH/translator.h

INCLUDEPATH += ../ $$GENERIC_PATH
