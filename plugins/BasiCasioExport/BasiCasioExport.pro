TEMPLATE = lib
CONFIG += release \
    plugin
INCLUDEPATH += ../../TilesCreator
HEADERS = ../../TilesCreator/pixeleditor.h \
    basicasioexport.h
SOURCES = ../../TilesCreator/pixeleditor.cpp \
    ../../TilesCreator/globaltilescreator.cpp \
    basicasioexport.cpp
TARGET = BasiCasioExporter
DESTDIR = ../../bin/plugins
