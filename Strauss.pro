#-------------------------------------------------
#
# Project created by QtCreator 2018-06-17T13:31:53
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += c++17
TARGET = Strauss
TEMPLATE = app
QMAKE_CXXFLAGS+= -Zi
QMAKE_CFLAGS = -Wno-unused-parameter
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
    src/audioplugin.cpp \
    src/audiopluginfactory.cpp \
    src/browser.cpp \
    src/controlchange.cpp \
    src/mainwindow.cpp\
    src/midiplayer.cpp\
    src/midimanager.cpp\
    src/pianorollitem.cpp \
    src/keyboard.cpp \
    src/Controllers/pianorollcontainer.cpp \
    src/pianorollwidget.cpp \
    src/tabwidgetview.cpp \
    src/trackcontainer.cpp \
    src/trackdirector.cpp \
    src/trackwidget.cpp \
    src/velocityview.cpp \
    src/trackmidiview.cpp \
    src/velocityviewitem.cpp \
    src/folderview.cpp \
    src/headercontainer.cpp \
    src/tracklengthview.cpp \
    src/Controllers/controlchangecontainer.cpp \
    src/controlchangeoverlay.cpp \
    src/controlchangeitem.cpp \
    src/collisionitem.cpp \
    src/pianorollhelperview.cpp \
    src/Controllers/plugineditorcontainer.cpp \
    src/plugintrackview.cpp \
    src/pluginview.cpp \
    src/settingsdialog.cpp \
    src/audioengine.cpp \
    src/audiomanager.cpp \
    src/projectmanager.cpp \
    src/midiinter.pb.cc \
    src/folderviewabstractmodel.cpp \
    src/command.cpp \
    src/qdraggingwidget.cpp \
    src/mastertrack.cpp \
    src/trackmidi.cpp \
    src/notation/notationmainwindow.cpp \
    src/notation/measure.cpp \
    src/notation/stave.cpp \
    src/notation/notationheader.cpp \
    src/notation/note.cpp \
    src/notation/element.cpp \
    src/notation/score.cpp \
    src/notation/scoreview.cpp \
    src/notation/page.cpp \
    src/notation/moment.cpp \
    src/notation/chord.cpp \
    src/notation/formatter.cpp \
    src/vst2audioplugin.cpp \
    src/vst3audioplugin.cpp

HEADERS  += src/mainwindow.h\
    src/audioplugin.h \
    src/audiopluginfactory.h \
    src/browser.h \
    src/controlchange.h \
    src/midiplayer.h\
    src/midimanager.h\
    src/pianorollitem.h \
    src/keyboard.h \
    src/Controllers/pianorollcontainer.h \
    src/pianorollwidget.h \
    src/tabwidgetview.h \
    src/trackcontainer.h \
    src/trackdirector.h \
    src/trackwidget.h \
    src/velocityview.h \
    src/trackmidiview.h \
    src/velocityviewitem.h \
    src/folderview.h \
    src/headercontainer.h \
    src/tracklengthview.h \
    src/Controllers/controlchangecontainer.h \
    src/controlchangeoverlay.h \
    src/controlchangeitem.h \
    src/collisionitem.h \
    src/pianorollhelperview.h \
    src/common.h \
    src/Controllers/plugineditorcontainer.h \
    src/plugintrackview.h \
    src/pluginview.h \
    src/settingsdialog.h \
    src/audioengine.h \
    src/audiomanager.h \
    src/projectmanager.h \
    src/midiinter.pb.h \
    src/folderviewabstractmodel.h \
    src/command.h \
    src/qdraggingwidget.h \
    src/mastertrack.h \
    src/trackmidi.h \
    src/notation/notationmainwindow.h \
    src/notation/measure.h \
    src/notation/stave.h \
    src/notation/notationheader.h \
    src/notation/note.h \
    src/notation/n_common.h \
    src/notation/element.h \
    src/notation/score.h \
    src/notation/scoreview.h \
    src/notation/page.h \
    src/notation/moment.h \
    src/notation/chord.h \
    src/notation/formatter.h \
    src/vst2audioplugin.h \
    src/vst3audioplugin.h

LIBS += -lwinmm \


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/./release/ -lportaudio_x64 -llibprotobuf
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/./debug/ -lportaudio_x64 -llibprotobufd

INCLUDEPATH += $$PWD/Debug \
                $$PWD/Release
DEPENDPATH += $$PWD/Debug \
                $$PWD/Release

RESOURCES += \
    graphics.qrc

FORMS += \
    src/trackview.ui
