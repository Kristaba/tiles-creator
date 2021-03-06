# #####################################################################
# Automatically generated by qmake (2.01a) dim. 26. juil. 16:34:22 2009
# #####################################################################
TEMPLATE = app
TARGET = 
DESTDIR = ../bin
DEPENDPATH += . \
    debug \
    qtc-gdbmacros \
    release
INCLUDEPATH += C:/TilesCreator/. \
    .
QT += opengl \
    network
TRANSLATIONS = tilescreator_en.ts

# Input
HEADERS += colorsselecter.h \
    pixeleditor.h \
    projectwidget.h \
    tilescreator.h \
    tileseditor.h \
    tileseteditor.h \
    mapeditor.h \
    graphicstileitem.h \
    BugReporter.h \
    tileselector.h \
    graphicstilescene.h \
    graphicstilesetscene.h \
    exportinterface.h \
    tcproject.h \
    array2d.h \
    importinterface.h \
    tilescreatorplugin.h \
    globaltilescreator.h \
    undocommands.h
SOURCES += colorsselecter.cpp \
    main.cpp \
    pixeleditor.cpp \
    projectwidget.cpp \
    tilescreator.cpp \
    tileseditor.cpp \
    tileseteditor.cpp \
    mapeditor.cpp \
    graphicstileitem.cpp \
    BugReporter.cpp \
    tileselector.cpp \
    graphicstilescene.cpp \
    graphicstilesetscene.cpp \
    globaltilescreator.cpp \
    undocommands.cpp
RESOURCES += tilescreator.qrc
