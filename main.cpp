#include "mainwindow.h"
#include <QApplication>
#include <pianoroll.h>
#include <QQuickWidget>
#include <midiplayer.h>
#include <QQmlApplicationEngine>
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QQmlApplicationEngine engine;
    MainWindow window;
   // PianoRoll piano(&window);
    QQuickWidget *view = new QQuickWidget;
    MidiPlayer *player = new MidiPlayer;
    view->setSource(QUrl::fromLocalFile("C:/Users/Puter/documents/MidiInter/Main.qml"));
    auto rootObject = view->rootObject();
    QObject::connect((QObject*)rootObject,SIGNAL(playNote(QString,QString)),player,SLOT(playNote(QString,QString)));



    view->show();
   // window.show();

    return app.exec();
}
