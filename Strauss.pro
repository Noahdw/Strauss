#-------------------------------------------------
#
# Project created by QtCreator 2018-06-17T13:31:53
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Strauss
TEMPLATE = app
QMAKE_CXXFLAGS+= -Zi
QMAKE_LFLAGS+=/DEBUG
#Uncomment below is force running app as admin
#QMAKE_LFLAGS += /MANIFESTUAC:\"level=\'requireAdministrator\' uiAccess=\'false\'\"
# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
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
    src/velocityview.cpp \
    src/trackmidiview.cpp \
    src/velocityviewitem.cpp \
    src/folderview.cpp \
    src/headercontainer.cpp \
    src/tracklengthview.cpp \
    src/controlchangeview.cpp \
    src/controlchangecontainer.cpp \
    src/controlchangeoverlay.cpp \
    src/controlchangebridge.cpp \
    src/controlchangeitem.cpp \
    src/collisionitem.cpp \
    src/pianorollhelperview.cpp \
    src/timetracker.cpp \
    src/plugineditorcontainer.cpp \
    src/plugintrackview.cpp \
    src/pluginview.cpp \
    src/settingsdialog.cpp \
    src/audioengine.cpp \
    src/audiomanager.cpp \
    src/projectmanager.cpp \
    src/midiinter.pb.cc \
    src/folderviewabstractmodel.cpp \
    src/command.cpp \
    src/qdraggingwidget.cpp

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
    src/velocityview.h \
    src/trackmidiview.h \
    src/velocityviewitem.h \
    src/folderview.h \
    src/headercontainer.h \
    src/tracklengthview.h \
    src/controlchangeview.h \
    src/controlchangecontainer.h \
    src/controlchangeoverlay.h \
    src/controlchangebridge.h \
    src/controlchangeitem.h \
    src/collisionitem.h \
    src/pianorollhelperview.h \
    src/common.h \
    src/timetracker.h \
    src/plugineditorcontainer.h \
    src/plugintrackview.h \
    src/pluginview.h \
    src/settingsdialog.h \
    src/audioengine.h \
    src/audiomanager.h \
    src/projectmanager.h \
    src/midiinter.pb.h \
    src/folderviewabstractmodel.h \
    src/command.h \
    src/qdraggingwidget.h

LIBS += -lwinmm \


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/./release/ -lportaudio_x64 -llibprotobuf
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/./debug/ -lportaudio_x64 -llibprotobufd

INCLUDEPATH += $$PWD/Debug \
                $$PWD/Release
DEPENDPATH += $$PWD/Debug \
                $$PWD/Release

RESOURCES += \
    graphics.qrc

