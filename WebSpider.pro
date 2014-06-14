#-------------------------------------------------
#
# Project created by QtCreator 2014-06-01T16:24:54
#
#-------------------------------------------------

 QT      += webkit network webkitwidgets sql



TARGET = WebSpider
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    WebSpider.cpp \
    brokerdb.cpp \
    stockdb.cpp \
    logdb.cpp \
    summarydb.cpp

HEADERS += \
    WebSpider.h \
    brokerdb.h \
    stockdb.h \
    logdb.h \
    summarydb.h


