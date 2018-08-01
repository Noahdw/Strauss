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
class FolderView : public QWidget
{
    Q_OBJECT
public:
    FolderView();
    PianoRollContainer * pRollContainer;
    PluginEditorContainer * pluginContainer;

    bool isPluginContainerWidget = false;
private:
    QVBoxLayout *vLayout;
    QTreeView *list;
    QString path = "C:/Program Files/Native Instruments/VSTPlugins 64 bit/";
    static int tempFolderID;

private slots:
   void itemDoubleClicked();
};

#endif // FOLDERVIEW_H
