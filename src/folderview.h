#ifndef FOLDERVIEW_H
#define FOLDERVIEW_H

class PluginEditorContainer;

#include <QObject>
#include <QWidget>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QFileSystemModel>
#include <QListView>
#include <QTreeView>
#include "pianorollcontainer.h"
#include "src/folderviewabstractmodel.h"
#include "src/mastertrack.h"
class FolderView : public QWidget
{
    Q_OBJECT
public:
    FolderView(FolderViewAbstractModel *qmodel, MasterTrack *master);
    PianoRollContainer * pRollContainer;
    PluginEditorContainer * pluginContainer;
    static int tempFolderID;
    bool isPluginContainerWidget = false;
private:
    QVBoxLayout *vLayout;
    QTreeView *list;
    FolderViewAbstractModel *model;
    QString path = "C:/Program Files/Native Instruments/VSTPlugins 64 bit/";
    MasterTrack *masterTrack;

private slots:
    void itemDoubleClicked();

};

#endif // FOLDERVIEW_H
