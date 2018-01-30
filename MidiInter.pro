#-------------------------------------------------
#
# Project created by QtCreator 2017-07-11T17:20:01
#
#-------------------------------------------------

QT       += core gui\
            quickwidgets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MidiInter
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += src/main.cpp\
    src/mainwindow.cpp\
    src/midiplayer.cpp\
    src/midimanager.cpp\
    src/pianoroll.cpp \
    src/pianorollitem.cpp \
    src/trackview.cpp \
    src/keyboard.cpp \
    src/pianorollcontainer.cpp \
    src/trackcontainer.cpp \
    src/vst2hostcallback.cpp \
    src/audiomanager.cpp \
    src/velocityview.cpp \
    src/trackmidiview.cpp \
    src/velocityviewitem.cpp \
    src/folderview.cpp \
    src/headercontainer.cpp

HEADERS  += src/mainwindow.h\
    src/midiplayer.h\
    src/midimanager.h\
    src/pianoroll.h \
    src/pianorollitem.h \
    src/trackview.h \
    src/keyboard.h \
    src/pianorollcontainer.h \
    src/trackcontainer.h \
    src/vst2hostcallback.h \
    src/audiomanager.h \
    src/velocityview.h \
    src/trackmidiview.h \
    src/velocityviewitem.h \
    src/folderview.h \
    src/headercontainer.h

FORMS    += mainwindow.ui
LIBS += -lwinmm


DISTFILES +=
QML +=



win32:CONFIG(release, debug|release): LIBS += -L$$PWD/./release/ -lportaudio_x64
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/./debug/ -lportaudio_x64

INCLUDEPATH += $$PWD/Debug
DEPENDPATH += $$PWD/Debug

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/./release/ -lportaudio_x64
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/./debug/ -lportaudio_x64

INCLUDEPATH += $$PWD/Release
DEPENDPATH += $$PWD/Release
