#include "mainwindow.h"
#include <QApplication>
#include <src/pianoroll.h>
#include <QQuickWidget>
#include <src/midiplayer.h>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <src/pianorollitem.h>
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow window;
    window.show();
    return app.exec();
}
