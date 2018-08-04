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

class PluginEditorContainer : public QWidget
{
    Q_OBJECT
public:
    PluginEditorContainer();
    PluginTrackView* addTrack(TrackView *track);
    void addPlugin(PluginTrackView *pluginTrack,Vst2HostCallback *vst2);
    void switchPluginViews(PluginTrackView *pluginTrack);
    void FolderViewDoubleClicked(QString name, QString filepath);
    void pluginTrackClickedOn();
    PluginTrackView *lastPluginTrack = NULL;
private:
    QVBoxLayout *vLayout;
    QHBoxLayout *hlayout;
    QHBoxLayout *hTopLayout;
    QHBoxLayout *hBotttomLayout;
    QScrollArea *trackScrollArea;
    FolderView  *folderView;
};

#endif // PLUGINEDITORCONTAINER_H
