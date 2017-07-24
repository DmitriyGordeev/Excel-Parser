#-------------------------------------------------
#
# Project created by QtCreator 2016-05-21T11:13:18
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = XLSXParser
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    io.cpp \
    strf.cpp \
    xlsxwrap.cpp \
    loadpreset.cpp \
    categorysettings.cpp \
    categoryrename.cpp \
    ComboDelegate.cpp \
    merchsorter.cpp \
    settingsparser.cpp \
    previewtable.cpp \
    xlsxfilter.cpp

include(qtxlsx/qtxlsx.pri)

HEADERS  += mainwindow.h \
    brand.h \
    io.h \
    merch.h \
    strf.h \
    xlsxwrap.h \
    loadpreset.h \
    categorysettings.h \
    categoryrename.h \
    ComboDelegate.h \
    merchsorter.h \
    settingsparser.h \
    previewtable.h \
    xlsxfilter.h

FORMS    += mainwindow.ui \
    loadpreset.ui \
    categorysettings.ui \
    categoryrename.ui \
    previewtable.ui

CONFIG += c++11
