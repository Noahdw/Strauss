#include "mainwindow.h"
#include <QApplication>
#include <pianoroll.h>
#include <QQuickWidget>
#include <midiplayer.h>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <pianoroll.h>
#include <pianorollitem.h>
int main(int argc, char *argv[])
{  
    QApplication app(argc, argv);

    MidiManager *manager = new MidiManager;
    MainWindow window(manager);
    window.show();

    return app.exec();
}
