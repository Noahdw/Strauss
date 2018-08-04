#ifndef TRACKCONTAINER_H
#define TRACKCONTAINER_H

class TrackMidiView;
class PluginEditorContainer;

#include <QObject>
#include <QWidget>
#include <src/midimanager.h>
#include <src/trackview.h>
#include <QVBoxLayout>
#include <QScrollArea>
#include <src/pianorollcontainer.h>
#include <QSpacerItem>
#include <QSplitter>
#include <QKeyEvent>
#include <QPainter>
#include "src/plugineditorcontainer.h"

class TrackContainer : public QWidget
{
    Q_OBJECT
public:
    TrackContainer(PluginEditorContainer *pluginEditorContainer, PianoRollContainer * pianoRollContainer);

    void addSingleView();
signals:
    void switchControlChange();
public slots:
    void addTrackView(const mSong &song);
signals:
    void addPianoRoll(TrackView *track);
    void requestTrackChange(int id);
protected:
   void mousePressEvent(QMouseEvent *event);
   void keyPressEvent(QKeyEvent * event);
   void paintEvent(QPaintEvent *event);
private:
    PluginEditorContainer *plugin_editor_container;
    PianoRollContainer    *piano_roll_container;
    QSplitter   *vSplitter;
    QVBoxLayout *vLayout;
    QHBoxLayout *hLayout;
    QScrollArea *scrollArea;


};

#endif // TRACKCONTAINER_H
