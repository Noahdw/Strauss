#ifndef PROJECTMANAGER_H
#define PROJECTMANAGER_H

class MainWindow;
class MasterTrack;
#include <iostream>
#include <fstream>
#include <QObject>
#include <string>
#include "src/midiinter.pb.h"
#include "trackcontainer.h"
#include "src/trackwidget.h"
#include "src/plugintrackview.h"

class ProjectManager
{
public:
    ProjectManager();
    void saveProject();
    void saveAsProject(QString path, MasterTrack *masterTrack);
    void loadProject(QString path, MainWindow *main_window, MasterTrack *masterTrack);

};

#endif // PROJECTMANAGER_H
