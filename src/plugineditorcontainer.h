#ifndef PLUGINEDITORCONTAINER_H
#define PLUGINEDITORCONTAINER_H

class TrackView;
class PluginTrackView;
class Vst2HostCallback;

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QScrollArea>
#include <src/folderview.h>
#include "mastertrack.h"
class PluginEditorContainer : public QFrame
{
    Q_OBJECT
public:
    PluginEditorContainer(FolderViewAbstractModel *model, MasterTrack *masterTrack);
    PluginTrackView* addTrack(TrackView *track);
    void switchPluginViews(PluginTrackView *pluginTrack);
    void pluginTrackClickedOn();
    PluginTrackView *lastPluginTrack = NULL;
    MasterTrack *masterTrack;
private:
    QVBoxLayout *vLayout;
    QHBoxLayout *hlayout;
    QHBoxLayout *hTopLayout;
    QHBoxLayout *hBotttomLayout;
    QScrollArea *trackScrollArea;
    FolderView  *folderView;
};

#endif // PLUGINEDITORCONTAINER_H
