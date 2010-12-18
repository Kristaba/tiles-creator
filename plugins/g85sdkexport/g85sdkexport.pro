TEMPLATE = lib
CONFIG += release \
    plugin
INCLUDEPATH += ../../TilesCreator
HEADERS = g85sdkexport.h \
    ../../TilesCreator/pixeleditor.h
SOURCES = g85sdkexport.cpp \
    ../../TilesCreator/pixeleditor.cpp \
    ../../TilesCreator/globaltilescreator.cpp
TARGET = g85sdk_export
DESTDIR = ../../bin/plugins
RESOURCES += g85sdkexport.qrc
TRANSLATIONS = g85sdkexport_en.ts
