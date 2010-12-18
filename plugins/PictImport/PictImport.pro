TEMPLATE = lib
CONFIG += release \
    plugin
INCLUDEPATH += ../../TilesCreator
HEADERS = pictimport.h \
    picturecolorsselector.h \
    samplingcolorlabel.h
SOURCES = pictimport.cpp \
    picturecolorsselector.cpp \
    samplingcolorlabel.cpp \
    ../../TilesCreator/globaltilescreator.cpp
TARGET = pict_import
DESTDIR = ../../bin/plugins
TRANSLATIONS = pictimport_en.ts
