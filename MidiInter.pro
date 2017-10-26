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


SOURCES += main.cpp\
        mainwindow.cpp\
            midiplayer.cpp\
            midimanager.cpp\
    pianoroll.cpp \
    pianorollitem.cpp \
    trackview.cpp \
    keyboard.cpp \
    pianorollcontainer.cpp \
    trackcontainer.cpp \
    vst2hostcallback.cpp \
    audiomanager.cpp \
    velocityview.cpp \
    trackmidiview.cpp \
    velocityviewitem.cpp

HEADERS  += mainwindow.h\
            midiplayer.h\
            midimanager.h\
    pianoroll.h \
    pianorollitem.h \
    trackview.h \
    keyboard.h \
    pianorollcontainer.h \
    trackcontainer.h \
    vst2hostcallback.h \
    audiomanager.h \
    velocityview.h \
    trackmidiview.h \
    velocityviewitem.h

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
