#include "mainwindow.h"
#include <QApplication>
#include <src/pianoroll.h>
#include <src/midiplayer.h>
#include <src/pianorollitem.h>
int main(int argc, char *argv[])

{
    QApplication app(argc, argv);
    app.setApplicationDisplayName("Strauss - A Composers Tool");
    QFile styleFile(":/Styles.qss");
    styleFile.open(QFile::ReadOnly);
    app.setStyleSheet(QString::fromLatin1(styleFile.readAll()));


    QCoreApplication::setOrganizationName("MyDaw");
    QCoreApplication::setApplicationName("Strauss");
   // QCoreApplication::setAttribute(Qt::AA_UseStyleSheetPropagationInWidgetStyles, true);
    MainWindow window;
    window.show();
    return app.exec();
}
